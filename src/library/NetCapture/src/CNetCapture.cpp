
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-08 09:56:53
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-09 11:37:34
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\src\CNetCapture.cpp
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  Remark         : Description                                              |
 * @#|----------------------------------------------------------------------------|
 * @#|  Change History :                                                          |
 * @#|  <Date>     | <Version> | <Author>       | <Description>                   |
 * @#|----------------------------------------------------------------------------|
 * @#|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
 * @#|----------------------------------------------------------------------------|
 * @#|                                                                            |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ————————————————————————————————————————————————————————————————————————————————————————*/
#include "CNetCapture.h"
#include <algorithm>
#include <memory>
 // #define PCAP_DONT_INCLUDE_PCAP_BPF_H
 // #include <Packet32.h>
#include <pcap.h>
#include <ntddndis.h>
#include <Common.h>
#define OID_802_3_PERMANENT_ADDRESS             0x01010101

#if defined(_WIN32) || defined(_WIN64)
#include <Winsock2.h>
#include <Iphlpapi.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#endif
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <string>
#include <boost/asio.hpp>

std::vector<std::shared_ptr<NetworkAdapterInfo>> CNetCapture::m_devices;
std::vector<std::shared_ptr<CCaptureThreadWrapper>> CNetCapture::m_vtCaptureThreadWrappers;
CNetCapture::CNetCapture() noexcept
{
    m_devices.clear();
    m_devices.shrink_to_fit();
}

CNetCapture::~CNetCapture() noexcept
{
    //停止抓包线程，关闭管理句柄
    StopCapture();
    //清空设备列表
    m_devices.clear();
    m_devices.shrink_to_fit();

}


size_t CNetCapture::Init() noexcept
{
    return GetDevicesInfo(m_devices);
}

bool CNetCapture::StartCapture() noexcept
{
    m_vtCaptureThreadWrappers.clear();
    m_vtCaptureThreadWrappers.shrink_to_fit();
    for (size_t i = 0; i < m_devices.size(); i++)
    {

        //创建线程并记录起来
        auto pCaptureThreadWrapper =
            std::make_shared<CCaptureThreadWrapper>(m_devices[i].get());
        m_vtCaptureThreadWrappers.emplace_back(pCaptureThreadWrapper);

        /* code */
    }

    return true;
}

bool CNetCapture::StopCapture() noexcept
{
    for (size_t i = 0; i < m_vtCaptureThreadWrappers.size(); i++)
    {
        m_vtCaptureThreadWrappers[i]->Stop();
    }
    m_vtCaptureThreadWrappers.clear();
    m_vtCaptureThreadWrappers.shrink_to_fit();

    return true;
}


/*!
   @note 得到网络适配器的MAC地址，本功能需要Npcap支持
   @param[in] szPcapAdapterID 传入网络适配器的ID
   @returns 返回MAC地址字符串
*/
std::string CNetCapture::GetMacAddress(const std::string& szPcapAdapterID) const
{
    std::string strMacAddress;

#if defined(_WIN32) || defined(_WIN64)  //windows平台下使用Iphlpapi.h库的GetAdaptersAddresses函数获取MAC地址
    // 获取MAC地址
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    ULONG outBufLen = 0;
    DWORD ret = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);
    if (ret == ERROR_BUFFER_OVERFLOW)
    {
        pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(malloc(outBufLen));
        if (pAddresses == nullptr)
        {
            return "";
        }
        ret = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);
        if (ret == NO_ERROR)
        {
            for (PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != nullptr; pCurrAddresses = pCurrAddresses->Next)
            {
                if (pCurrAddresses->IfType == IF_TYPE_ETHERNET_CSMACD)
                {
                    if (szPcapAdapterID.find(pCurrAddresses->AdapterName) != std::string::npos)
                    {
                        strMacAddress = std::format("{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}", pCurrAddresses->PhysicalAddress[0], pCurrAddresses->PhysicalAddress[1],
                            pCurrAddresses->PhysicalAddress[2], pCurrAddresses->PhysicalAddress[3],
                            pCurrAddresses->PhysicalAddress[4], pCurrAddresses->PhysicalAddress[5]);

                        std::transform(strMacAddress.begin(), strMacAddress.end(), strMacAddress.begin(), ::toupper); //转换为大写
                        break;
                    }
                }
            }
        }
        free(pAddresses);
    }

#else
    //linux平台下使用ioctl函数获取MAC地址
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return "";
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth0");

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        perror("ioctl");
        close(fd);
        return "";
    }

    unsigned char* mac = reinterpret_cast<unsigned char*>(ifr.ifr_hwaddr.sa_data);
    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str), "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    std::string strMacAddress(mac_str);
    std::transform(strMacAddress.begin(), strMacAddress.end(), strMacAddress.begin(), ::toupper); //转换为大写
    close(fd);
#endif
    return strMacAddress;
}

/*!
   @note 得到网络适配器的DeviceName，本功能需要Npcap支持，并去除额外的Pcap组织的无效头尾信息
   @param[out] strValue 返回格式化后的DeviceName
   @returns 返回MAC地址字符串
  */
std::string CNetCapture::GetNetworkAdapterDeviceNameByPcap(const std::string& strValue) const
{
    //去除一些Pcap组织的无效头尾信息
    std::string strSrc = strValue;

    auto nIndex = strSrc.find("'");
    std::string strRet;
    if (nIndex != std::string::npos)
    {
        strRet = strSrc.substr(nIndex + 1, strSrc.length() - (nIndex + 1));

        size_t nIndex2 = strRet.find("'");
        if (nIndex2 > 0)
            strRet = strRet.substr(0, nIndex2);
    }
    return strRet;
}


/*!
   @note 得到网络适配器的DeviceName，本功能需要Npcap支持，并去除额外的Pcap组织的无效头尾信息
   @param[out] strValue 返回格式化后的DeviceName
   @returns 返回MAC地址字符串
*/
std::string CNetCapture::GetNetworkAdapterIDByPcap(const std::string& strValue) const
{
    //去除一些Pcap组织的无效头尾信息
    std::string strSrc = strValue;
    size_t nIndex = strSrc.rfind("/");
    std::string strRet;
    if (nIndex >= 0)
    {
        strRet = strSrc.substr(nIndex + 1, strSrc.length() - (nIndex + 1));

        size_t nIndex2 = strRet.find("'");
        if (nIndex2 > 0)
            strRet = strRet.substr(0, nIndex2);
    }
    return strRet;
}


size_t CNetCapture::GetDevicesInfo(std::vector<std::shared_ptr<NetworkAdapterInfo>>& devices) noexcept
{
    //清空设备列表
    devices.clear();
    devices.shrink_to_fit();
    //获取设备列表
    pcap_if_t* alldevs = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE] = { 0 };
    if (pcap_findalldevs(&alldevs, errbuf) == -1 || alldevs == nullptr) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return 0;
    }
    int i = 0;
    for (const pcap_if_t* iDev = alldevs; iDev != nullptr; iDev = iDev->next)
    {
        auto pNetAdapterInfo = std::make_shared<NetworkAdapterInfo>();
        pNetAdapterInfo->ID = iDev->name;
        pNetAdapterInfo->Name = iDev->description;
        //pNetAdapterInfo->Name = GetNetworkAdapterDeviceNameByPcap(iDev->description);
        pNetAdapterInfo->Flags = iDev->flags;
        if (iDev->addresses != nullptr) //有些设备并没有分配IPV4地址,就无需加入可用列表了
        {
            const pcap_addr_t* iAddr = nullptr;
            for (iAddr = iDev->addresses; iAddr != nullptr; iAddr = iAddr->next)
            {
                if (iAddr->addr->sa_family == AF_INET)  //只获取IPV4地址
                {
                    // 从sockaddr_in提取地址
                    auto ipv4_local = reinterpret_cast<sockaddr_in*>(iAddr->addr);
                    boost::asio::ip::address_v4 IpAddress(ntohl(ipv4_local->sin_addr.s_addr));
                    pNetAdapterInfo->IpAddress = IpAddress.to_string();

                    //获取网关地址
                    const auto ipv4_broad = (struct sockaddr_in*)iAddr->broadaddr;
                    boost::asio::ip::address_v4 BroadAddress(ntohl(ipv4_broad->sin_addr.s_addr));
                    pNetAdapterInfo->BroadAddress = BroadAddress.to_string();

                    //获取掩码地址
                    const auto ipv4_netmask = (struct sockaddr_in*)iAddr->netmask;
                    boost::asio::ip::address_v4 NetmaskAddress(ntohl(ipv4_netmask->sin_addr.s_addr));
                    pNetAdapterInfo->NetmaskAddress = NetmaskAddress.to_string();
                    //获取MAC 地址

                    pNetAdapterInfo->MACAddress = GetMacAddress(iDev->name);


                    // LOG(INFO) << "IP:" << pNetAdapterInfo->IpAddress
                    //     << "|" << "网关:" << pNetAdapterInfo->BroadAddress
                    //     << "|" << "子网掩码:" << pNetAdapterInfo->NetmaskAddress
                    //     << "|" << "MAC:" << pNetAdapterInfo->MACAddress
                    //     << "\n";
                    devices.push_back(pNetAdapterInfo);
                }
            }
        }
    }

    // 释放设备列表
    pcap_freealldevs(alldevs);

    return devices.size();
}


