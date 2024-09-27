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
#include <pjsip.h>
#include <pjlib.h>
#include <pjlib-util.h>
#include <pjsua.h>

#include <pjmedia.h>
#include <exosip2/exosip.h>
#include <osip2/osip.h>
#include <osipparser2/osip_parser.h>
const std::string THIS_FILE = "MyApp.cpp";
#include "../../RecordEngine/include/Record.h"
#include <QObject.h>
#include <Config.h>
#ifndef PJSIP_ALLOC_MEM_SIZE
#  define PJSIP_ALLOC_MEM_SIZE       (8*1024*1024)
#endif
std::vector<std::shared_ptr<NetworkAdapterInfo>> CNetCapture::m_devices;
std::vector<std::shared_ptr<CCaptureThreadWrapper>> CNetCapture::m_vtCaptureThreadWrappers;
CNetCapture::CNetCapture() noexcept
{
    m_devices.clear();
    m_devices.shrink_to_fit();

}

CNetCapture::~CNetCapture() noexcept
{
    Release();
}
/*! --------------------------------------------------------------------------------------------------------------------------
     * @name   bool Init()
     * @brief  获取设备
     * @return 初始化是否成功,成功返回网络设备数量，失败返回0
     * @remark GetDevicesInfo函数用于获取当前计算机的网络设备信息，并将设备信息存储在devices结构中，并返回设备数量。
    --------------------------------------------------------------------------------------------------------------------------*/
size_t CNetCapture::Init() noexcept
{
    pjsip_Init();
    return GetDevicesInfo(m_devices);
}
size_t CNetCapture::Release() noexcept
{
    //停止抓包线程，关闭管理句�?
    StopCapture();
    //清空设备列表
    m_devices.clear();
    m_devices.shrink_to_fit();
    pjsip_Release();

    LOG(INFO) << "NetCapture Release";
    return true;
}
/*! --------------------------------------------------------------------------------------------------------------------------
     * @name   bool StartCapture()
     * @brief  启动网络抓包线程
     * @param
     * @return 无
     --------------------------------------------------------------------------------------------------------------------------*/
bool CNetCapture::StartCapture() noexcept
{

    // for (size_t i = 0; i < Config::GetConfigInfo().m_mapNetworkAdapters.size(); i++)
    // {
    //     for (size_t j = 0; j < m_devices.size(); j++)
    //     {
    //         if (Config::GetConfigInfo().m_mapNetworkAdapters[i]->ID == m_devices[j]->ID)
    //         {
    //             //创建线程并记录
    //             auto pCaptureThreadWrapper =
    //                 std::make_shared<CCaptureThreadWrapper>(m_devices[j].get());
    //             m_vtCaptureThreadWrappers.emplace_back(pCaptureThreadWrapper);
    //         }
    //         /* code */
    //     }

    //     /* code */
    // }


    ;




    m_vtCaptureThreadWrappers.clear();
    m_vtCaptureThreadWrappers.shrink_to_fit();
    for (size_t i = 0; i < m_devices.size(); i++)
    {
        if (Config::GetConfigInfo().m_mapNetworkAdapters.find(m_devices[i]->ID) == Config::GetConfigInfo().m_mapNetworkAdapters.end())
        {
            continue;
        }

        //创建线程并记录起�?
        auto pCaptureThreadWrapper =
            std::make_shared<CCaptureThreadWrapper>(m_devices[i].get());
        m_vtCaptureThreadWrappers.push_back(pCaptureThreadWrapper);

        /* code */
    }
    //测试pjsua的使用
    //pj_status_t status;

    // 初始化 PJSUA
    // status = pjsua_create();
    // if (status != PJ_SUCCESS) return -1;

    // // 初始化 PJSUA 配置
    // pjsua_config cfg;
    // pjsua_config_default(&cfg);

    // // 初始化 PJSUA 日志配置
    // pjsua_logging_config log_cfg;
    // pjsua_logging_config_default(&log_cfg);
    // log_cfg.console_level = 4;

    // // 初始化并启动 PJSUA
    // status = pjsua_init(&cfg, &log_cfg, NULL);
    // if (status != PJ_SUCCESS) return -1;



    return true;
}
/*! --------------------------------------------------------------------------------------------------------------------------
     * @name   bool StopCapture()
     * @brief  停止网络抓包线程
     * @param
     * @return 无
     --------------------------------------------------------------------------------------------------------------------------*/
bool CNetCapture::StopCapture() noexcept
{
    // for (size_t i = 0; i < m_vtCaptureThreadWrappers.size(); i++)
    // {
    //     m_vtCaptureThreadWrappers[i]->Stop();

    // }
    m_vtCaptureThreadWrappers.clear();
    m_vtCaptureThreadWrappers.shrink_to_fit();

    return true;
}

/*! --------------------------------------------------------------------------------------------------------------------------
 * @name   std::string GetMacAddress(const std::string& szPcapAdapterID)
 * @brief  获取MAC地址
 * @param  szPcapAdapterID 网络适配器ID
 * @return MAC地址
 --------------------------------------------------------------------------------------------------------------------------*/

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

                        std::transform(strMacAddress.begin(), strMacAddress.end(), strMacAddress.begin(), ::toupper); //转换为大�?
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
    std::transform(strMacAddress.begin(), strMacAddress.end(), strMacAddress.begin(), ::toupper); //转换为大�?
    close(fd);
#endif
    return strMacAddress;
}
/** --------------------------------------------------------------------------------------------------------------------------
* @name   std::string GetNetworkAdapterDeviceNameByPcap(const std::string& strValue) const;
* @brief  获取网络适配器(PCAP)名称
* @note   得到网络适配器的DeviceName，本功能需要Npcap支持，并去除额外的Pcap组织的无效头尾信息
* @param  strValue 网络适配器名称
* @return 网络适配器(PCAP)名称
--------------------------------------------------------------------------------------------------------------------------*/
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


/** --------------------------------------------------------------------------------------------------------------------------
* @name   std::string GetNetworkAdapterIDByPcap(const std::string& strValue) const;
* @brief  获取网络适配器(PCAP)名称
* @note   得到网络适配器的DeviceName，本功能需要Npcap支持，并去除额外的Pcap组织的无效头尾信息
* @param  strValue 网络适配器ID
* @return 网络适配器(PCAP)名称
--------------------------------------------------------------------------------------------------------------------------*/
std::string CNetCapture::GetNetworkAdapterIDByPcap(const std::string& strValue) const
{
    //去除一些Pcap组织的无效头尾信�?
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

bool CNetCapture::SetRecordCallback(std::function<int32_t(std::shared_ptr<CRecord>)> callback)
{
    if (callback == nullptr)
        return false;
    m_RecordCallback = callback;
    return true;
}

/*! --------------------------------------------------------------------------------------------------------------------------
* @name   size_t GetDevicesInfo(std::vector<std::shared_ptr<NetworkAdapterInfo>>& devices) noexcept;
* @brief  获取设备信息
* @return 设备数量(size_t)
* @remark GetDevicesInfo函数用于获取当前计算机的网络设备信息，并将设备信息存储在devices结构中，并返回设备数量。
--------------------------------------------------------------------------------------------------------------------------*/

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
        pNetAdapterInfo->Name = boost::locale::conv::to_utf<char>(iDev->description, "GBK");
        //pNetAdapterInfo->Name = GetNetworkAdapterDeviceNameByPcap(iDev->description);
        pNetAdapterInfo->Flags = iDev->flags;
        if (iDev->addresses != nullptr) //有些设备并没有分配IPV4地址,就无需加入可用列表�?
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


                    LOG(DEBUG) << "IP:" << pNetAdapterInfo->IpAddress
                        << "|" << "网关:" << pNetAdapterInfo->BroadAddress
                        << "|" << "子网掩码:" << pNetAdapterInfo->NetmaskAddress
                        << "|" << "MAC:" << pNetAdapterInfo->MACAddress
                        << "|" << "网卡名称:" << pNetAdapterInfo->Name
                        << "\n";
                    devices.push_back(pNetAdapterInfo);
                }
            }
        }
    }

    // 释放设备列表
    pcap_freealldevs(alldevs);

    return devices.size();
}


// int CNetCapture::txdata_test_uri_params(const std::string& strSipMsg)
// {
//     pjsip_endpoint* endpt;
//     pj_caching_pool caching_pool;
//     pj_status_t rc;
//     pj_status_t status;
//     if ((rc = pj_init()) != PJ_SUCCESS)
//     {

//         return rc;
//     }
//     if ((rc = pjlib_util_init()) != PJ_SUCCESS) {

//         return rc;
//     }
//     // status = init_report();
//     // if (status != PJ_SUCCESS)
//     //     return status;

//     pj_caching_pool_init(&caching_pool, &pj_pool_factory_default_policy,
//         PJSIP_TEST_MEM_SIZE);

//     rc = pjsip_endpt_create(&caching_pool.factory, "endpt", &endpt);
//     if (rc != PJ_SUCCESS) {
//         pj_caching_pool_destroy(&caching_pool);
//         return rc;
//     }

//     char msgbuf[512];
//     pj_str_t target = pj_str((char*)"sip:alice@wonderland:5061;x-param=param%201"
//         "?X-Hdr-1=Header%201"
//         "&X-Empty-Hdr=");
//     pj_str_t contact;
//     pj_str_t pname = pj_str((char*)"x-param");
//     pj_str_t hname = pj_str((char*)"X-Hdr-1");
//     pj_str_t hemptyname = pj_str((char*)"X-Empty-Hdr");
//     pjsip_from_hdr* from_hdr;
//     pjsip_to_hdr* to_hdr;
//     pjsip_contact_hdr* contact_hdr;
//     pjsip_generic_string_hdr* hdr;
//     pjsip_tx_data* tdata;
//     pjsip_sip_uri* uri;
//     pjsip_param* param;
//     pjsip_via_hdr* via;
//     pjsip_parser_err_report err_list;
//     pjsip_msg* msg;
//     pj_ssize_t len;


//     //PJ_LOG(3, (THIS_FILE, "   header param in URI to create request"));

//     /* Due to #930, contact argument is now parsed as Contact header, so
//      * must enclose it with <> to make it be parsed as URI.
//      */
//     pj_ansi_snprintf(msgbuf, sizeof(msgbuf), "<%.*s>",
//         (int)target.slen, target.ptr);
//     contact.ptr = msgbuf;
//     contact.slen = strlen(msgbuf);

//     /* Create request with header param in target URI. */
//     status = pjsip_endpt_create_request(endpt, &pjsip_invite_method, &target,
//         &target, &target, &contact, NULL, -1,
//         NULL, &tdata);
//     if (status != 0) {
//         //app_perror("   error: Unable to create request", status);
//         return -200;
//     }
//     // msg = pjsip_parse_msg(tdata->pool, msgbuf, len, &err_list);


//     /* Fill up the Via header to prevent syntax error on parsing */
//     via = (pjsip_via_hdr*)pjsip_msg_find_hdr(tdata->msg, PJSIP_H_VIA, NULL);
//     via->transport = pj_str((char*)"TCP");
//     via->sent_by.host = pj_str((char*)"127.0.0.1");

//     /* Print and parse the request.
//      * We'll check that header params are not present in
//      */



//     len = pjsip_msg_print(tdata->msg, msgbuf, sizeof(msgbuf));
//     if (len < 1) {
//         //PJ_LOG(3, (THIS_FILE, "   error: printing message"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -250;
//     }
//     msgbuf[len] = '\0';

//     PJ_LOG(5, (THIS_FILE.c_str(), "%ld bytes request created:--begin-msg--\n"
//         "%s\n"
//         "--end-msg--", len, msgbuf));

//     /* Now parse the message. */
//     pj_list_init(&err_list);

//     msg = pjsip_parse_msg(tdata->pool, msgbuf, len, &err_list);
//     if (msg == NULL) {
//         pjsip_parser_err_report* e;

//         //PJ_LOG(3, (THIS_FILE, "   error: parsing message message"));

//         e = err_list.next;
//         while (e != &err_list) {
//             PJ_LOG(3, (THIS_FILE.c_str(), "     %s in line %d col %d hname=%.*s",
//                 pj_exception_id_name(e->except_code),
//                 e->line, e->col + 1,
//                 (int)e->hname.slen,
//                 e->hname.ptr));
//             e = e->next;
//         }

//         pjsip_tx_data_dec_ref(tdata);
//         return -256;
//     }

//     /* Check the existence of port, other_param, and header param.
//      * Port is now allowed in To and From header.
//      */
//      /* Port in request URI. */
//     uri = (pjsip_sip_uri*)pjsip_uri_get_uri(msg->line.req.uri);
//     if (uri->port != 5061) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: port not present in request URI"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -260;
//     }
//     /* other_param in request_uri */
//     param = pjsip_param_find(&uri->other_param, &pname);
//     if (param == NULL || pj_strcmp2(&param->value, "param 1") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: x-param not present in request URI"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -261;
//     }
//     /* header param in request uri. */
//     if (!pj_list_empty(&uri->header_param)) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: hparam in request URI"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -262;
//     }

//     /* Port in From header. */
//     from_hdr = (pjsip_from_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_FROM, NULL);
//     uri = (pjsip_sip_uri*)pjsip_uri_get_uri(from_hdr->uri);
//     if (uri->port != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: port most not exist in From header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -270;
//     }
//     /* other_param in From header */
//     param = pjsip_param_find(&uri->other_param, &pname);
//     if (param == NULL || pj_strcmp2(&param->value, "param 1") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: x-param not present in From header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -271;
//     }
//     /* header param in From header. */
//     if (!pj_list_empty(&uri->header_param)) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: hparam in From header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -272;
//     }


//     /* Port in To header. */
//     to_hdr = (pjsip_to_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_TO, NULL);
//     uri = (pjsip_sip_uri*)pjsip_uri_get_uri(to_hdr->uri);
//     if (uri->port != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: port most not exist in To header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -280;
//     }
//     /* other_param in To header */
//     param = pjsip_param_find(&uri->other_param, &pname);
//     if (param == NULL || pj_strcmp2(&param->value, "param 1") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: x-param not present in To header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -281;
//     }
//     /* header param in From header. */
//     if (!pj_list_empty(&uri->header_param)) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: hparam in To header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -282;
//     }



//     /* Port in Contact header. */
//     contact_hdr = (pjsip_contact_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_CONTACT, NULL);
//     uri = (pjsip_sip_uri*)pjsip_uri_get_uri(contact_hdr->uri);
//     if (uri->port != 5061) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: port not present in Contact header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -290;
//     }
//     /* other_param in Contact header */
//     param = pjsip_param_find(&uri->other_param, &pname);
//     if (param == NULL || pj_strcmp2(&param->value, "param 1") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: x-param not present in Contact header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -291;
//     }
//     /* header param in Contact header. */
//     if (pj_list_empty(&uri->header_param)) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: hparam is missing in Contact header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -292;
//     }
//     /* Check for X-Hdr-1 */
//     param = pjsip_param_find(&uri->header_param, &hname);
//     if (param == NULL || pj_strcmp2(&param->value, "Header 1") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: hparam is missing in Contact header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -293;
//     }
//     /* Check for X-Empty-Hdr */
//     param = pjsip_param_find(&uri->header_param, &hemptyname);
//     if (param == NULL || pj_strcmp2(&param->value, "") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: hparam is missing in Contact header"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -294;
//     }


//     /* Check that headers are present in the request. */
//     hdr = (pjsip_generic_string_hdr*)
//         pjsip_msg_find_hdr_by_name(msg, &hname, NULL);
//     if (hdr == NULL || pj_strcmp2(&hdr->hvalue, "Header 1") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: header X-Hdr-1 not created"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -300;
//     }

//     hdr = (pjsip_generic_string_hdr*)
//         pjsip_msg_find_hdr_by_name(msg, &hemptyname, NULL);
//     if (hdr == NULL || pj_strcmp2(&param->value, "") != 0) {
//         PJ_LOG(3, (THIS_FILE.c_str(), "   error: header X-Empty-Hdr not created"));
//         pjsip_tx_data_dec_ref(tdata);
//         return -330;
//     }

//     pjsip_tx_data_dec_ref(tdata);
//     return 0;
// }


// int CNetCapture::ParserSipMessage(const std::string& strSipMsg)
// {


//     osip_t* osip;
//     osip_message_t* sip_msg;
//     // char* sip_message_str = "INVITE sip:bob@biloxi.com SIP/2.0\r\nVia: SIP/2.0/UDP server10.biloxi.com;branch=z9hG4bKnashds8\r\n...\r\n";
//     int i;

//     // 初始化oSIP
//     i = osip_init(&osip);
//     if (i != 0) {
//         // 初始化失败
//         return -1;
//     }

//     // 解析SIP消息
//     i = osip_message_init(&sip_msg);
//     if (i != 0) {
//         // 消息初始化失败
//         osip_free(osip);
//         return -1;
//     }

//     i = osip_message_parse(sip_msg, strSipMsg.c_str(), strSipMsg.length());

//     if (i != 0) {
//         // 消息解析失败
//         osip_message_free(sip_msg);
//         osip_free(osip);
//         return -1;
//     }
//     osip_body_t* body;
//     int j = 0;

//     j = osip_message_get_body(sip_msg, 0, &body);
//     osip_list_t* headers = body->headers;

//     // 在这里处理解析后的SIP消息，例如访问消息头
//     // ...

//     // 清理
//     osip_message_free(sip_msg);
//     osip_free(osip);
//     return 0;
// }
/* Syntax error handler for parser. */
static void on_syntax_error(pj_scanner* scanner)
{
    PJ_UNUSED_ARG(scanner);
    PJ_THROW(PJSIP_SYN_ERR_EXCEPTION);
}

int CNetCapture::parse_sip_message_from_string(const std::string& sip_message_str, void* pData) {

    int nRet = 0;
    pj_status_t status = PJ_FALSE;
    pjsip_msg* msg = nullptr;
    // pj_scan_state scan_state;
    pj_pool_t* pool = nullptr;
    // pjsip_endpoint* endpt = nullptr;
    //pj_caching_pool caching_pool;
    pjmedia_sdp_session* sdp_session = nullptr;
    pj_scanner scanner;
    pj_thread_desc desc;
    pj_thread_t* thread = nullptr;
    try
    {
        CUSTOM_ASSERT(pData != nullptr);
        CRecord* pRecord = static_cast<CRecord*>(pData);
        status = pj_thread_register("MyPJSIPThread", desc, &thread);
        if (status != PJ_SUCCESS) {
            // 处理错误
        }
        // 创建内存池

        pool = pj_pool_create(&m_caching_pool.factory, "sip_parse", 4000, 4000, NULL);
        if (pool == NULL) {
            throw QObject::tr("Error creating pool").toStdString();
        }
        // 初始化扫描状态
        pj_scan_init(&scanner, (char*)sip_message_str.c_str(), sip_message_str.length(), 0, &on_syntax_error);
        // 解析 SIP 消息
        pjsip_parser_err_report err_list;
        pj_list_init(&err_list);
        pjsip_msg* msg_t = pjsip_parse_msg(pool, (char*)sip_message_str.c_str(), sip_message_str.length(), &err_list);
        if (status == PJ_SUCCESS && msg_t != NULL) {
            //获取Callid
            pjsip_cid_hdr* cid_hdr = (pjsip_cid_hdr*)pjsip_msg_find_hdr(msg_t, PJSIP_H_CALL_ID, NULL);

            if (cid_hdr) {
                // 从Call-ID头部获取呼叫ID字符串
                pj_str_t* call_id = &cid_hdr->id;

                // 将PJSIP字符串转换为C字符串

                std::string strCallid = std::string(call_id->ptr, call_id->slen);
                // 打印或处理呼叫ID
                LOG(DEBUG) << "Callid: " << strCallid;
                pRecord->CallID = strCallid;
            }
            else {
                // Call-ID头部未找到
                throw(QObject::tr("Call-ID header not found").toStdString());
            }

            std::string SipMethodName(static_cast<char*>(msg_t->line.req.method.name.ptr), msg_t->line.req.method.name.slen);
            pRecord->RecordType = SipMethodName;
            //     if (pjsip_method_cmp(&msg_t->line.req.method, &pjsip_invite_method) == 0)
            //     {
            //         pRecord->RecordType = "INVITE";
            //     }
            // if (pjsip_method_cmp(&msg_t->line.req.method, &pjsip_bye_method) == 0)
            // {
            //     pRecord->RecordType = "BYE";
            // }
            // 查找Content-Type头部
            pjsip_ctype_hdr* ctype_hdr = (pjsip_ctype_hdr*)pjsip_msg_find_hdr(msg_t, PJSIP_H_CONTENT_TYPE, NULL);

            if (ctype_hdr && pj_stricmp2(&ctype_hdr->media.type, "application") == 0 &&
                pj_stricmp2(&ctype_hdr->media.subtype, "sdp") == 0) {
                // 消息体包含SDP信息

                // 获取消息体
                pjsip_msg_body* body = msg_t->body;
                if (body && body->data && body->len > 0) {
                    // 将消息体转换为C字符串
                    std::string sdp_str(static_cast<char*>(body->data), body->len);

                    // 在这里，你可以使用sdp_str，它包含SDP信息的字符串表示
                    // 例如，打印SDP信息
                    LOG(DEBUG) << "SDP: " << sdp_str;
                    // 解析SDP字符串
                    status = pjmedia_sdp_parse(pool, (char*)sdp_str.c_str(), sdp_str.length(), &sdp_session);
                    if (status != PJ_SUCCESS) {
                        //throw(QObject::tr("SDP Parse Error").toStdString());
                        // 错误处理
                        // 处理错误
                        char errbuf[PJ_ERR_MSG_SIZE] = { 0 };
                        pj_strerror(status, errbuf, sizeof(errbuf));
                        LOG(ERROR) << "Error parsing SDP: " << errbuf;
                    }
                    else {
                        // SDP解析成功，可以处理parsed_sdp
                        // 例如，打印会话名称
                        if (sdp_session->name.ptr) {
                            printf("Session Name: %.*s\n", (int)sdp_session->name.slen, sdp_session->name.ptr);
                        }
                        LOG(DEBUG) << "sdp_session->attr_count :" << sdp_session->attr_count << " | sdp_session->media_count: " << sdp_session->media_count;

                        for (size_t n = 0; n < sdp_session->media_count; n++)
                        {

                            pjmedia_sdp_media* media = sdp_session->media[n];
                            std::string typeName(static_cast<char*>(media->desc.media.ptr), media->desc.media.slen);
                            if (typeName == "audio")
                            {
                                unsigned port = media->desc.port; // 获取端口号
                                LOG(DEBUG) << "media type: " << typeName << " | media port: " << port;
                            }
                        }
                    }
                    // 如果需要进一步解析SDP信息，可以使用PJSIP的SDP解析器或其他SDP解析库
                    // 注意：这里没有展示SDP解析的具体实现，因为它依赖于具体的解析库或PJSIP的SDP解析功能
                }
            }
            else {
                // 没有找到Content-Type头部或消息体不包含SDP信息
            }

            // 获取 From 头部
           // 获取 From 头部
            pjsip_from_hdr* from_hdr = (pjsip_from_hdr*)pjsip_msg_find_hdr(msg_t, PJSIP_H_FROM, NULL);
            if (from_hdr != NULL) {
                // 获取 From 头部的显示名
                pj_str_t display_name;
                // 获取 From 头部的地址
                pjsip_uri* uri = from_hdr->uri;

                // 检查 URI 类型是否为 pjsip_name_addr
                if (PJSIP_URI_SCHEME_IS_SIP(uri) || PJSIP_URI_SCHEME_IS_SIPS(uri)) {
                    pjsip_sip_uri* sip_uri = (pjsip_sip_uri*)pjsip_uri_get_uri(uri);
                    // 获取 From 头部的地址的用户名
                    pj_str_t username = sip_uri->user;
                    // 获取 From 头部的地址的域名
                    pj_str_t host = sip_uri->host;
                    // 获取 From 头部的地址的域名

                    // 获取 From 头部的地址的显示名
                    pjsip_name_addr* name_addr = (pjsip_name_addr*)uri;
                    display_name = name_addr->display;

                    // 打印 To 头部信息
                    //PJ_LOG(3, ("YourApp", "To: %.*s@%.*s", (int)username.slen, username.ptr, (int)host.slen, host.ptr));
                    std::string strFrom = std::string(display_name.ptr, display_name.slen) + "<" + std::string(username.ptr, username.slen) + "@" + std::string(host.ptr, host.slen) + ":" + std::to_string(sip_uri->port) + ">";
                    pRecord->Caller = strFrom;
                    LOG(DEBUG) << "From: " << strFrom;
                }
            }

            // 获取 From 头部
            pjsip_to_hdr* to_hdr = (pjsip_to_hdr*)pjsip_msg_find_hdr(msg_t, PJSIP_H_TO, NULL);
            if (from_hdr != NULL) {
                // 获取 From 头部的显示名
                pj_str_t display_name;
                // 获取 From 头部的地址
                pjsip_uri* uri = to_hdr->uri;
                // 检查 URI 类型是否为 pjsip_name_addr
                if (PJSIP_URI_SCHEME_IS_SIP(uri) || PJSIP_URI_SCHEME_IS_SIPS(uri)) {
                    pjsip_sip_uri* sip_uri = (pjsip_sip_uri*)pjsip_uri_get_uri(uri);
                    // 获取 From 头部的地址的用户名
                    pj_str_t username = sip_uri->user;
                    // 获取 From 头部的地址的域名
                    pj_str_t host = sip_uri->host;
                    // 获取 From 头部的地址的域名

                    // 获取 From 头部的地址的显示名
                    pjsip_name_addr* name_addr = (pjsip_name_addr*)uri;
                    display_name = name_addr->display;

                    // 打印 To 头部信息
                    //PJ_LOG(3, ("YourApp", "To: %.*s@%.*s", (int)username.slen, username.ptr, (int)host.slen, host.ptr));
                    std::string strTo = std::string(display_name.ptr, display_name.slen) + "<" + std::string(username.ptr, username.slen) + "@" + std::string(host.ptr, host.slen) + ":" + std::to_string(sip_uri->port) + ">";
                    pRecord->Callee = strTo;
                    LOG(DEBUG) << "To: " << strTo;

                }
            }
        }
        else {
            nRet = 0;
            // 解析失败处理
        }
        nRet = 1;
    }
    catch (const std::exception& e)
    {
        nRet = 0;
        LOG(ERROR) << e.what();
    }
    // if (thread)
    // {
    //     pj_thread_destroy(thread);
    //     thread = nullptr;
    // }
    if (&scanner)
    {
        pj_scan_fini(&scanner);
    }
    if (pool)
    {
        pj_pool_release(pool);
        pool = nullptr;
    }
    // if (endpt)
    // {
    //     pjsip_endpt_destroy(endpt);
    //     endpt = nullptr;
    // }
    // if (&caching_pool)
    //     pj_caching_pool_destroy(&caching_pool);
    return nRet;
}


/** --------------------------------------------------------------------------------------
 * @name    bool pjsip_Init();
 * @brief   PJSIP库的初始化过程
 * @param
 * @return  true:成功 false:失败
 --------------------------------------------------------------------------------------*/
bool CNetCapture::pjsip_Init()
{
    pj_status_t status;
    status = pj_init();
    if (status != PJ_SUCCESS) {
        return false;
    }
    status = pjlib_util_init();
    if (status != PJ_SUCCESS) {
        return false;
    }

    // status = pj_thread_init();
    // if (status != PJ_SUCCESS) {
    //     return false;
    // }
    // 设置日志等级为0，禁用日志输出
    pj_log_set_level(3);
    pj_caching_pool_init(&m_caching_pool, &pj_pool_factory_default_policy, PJSIP_ALLOC_MEM_SIZE);

    pj_status_t rc = pjsip_endpt_create(&m_caching_pool.factory, "endpt", &m_endpt);
    if (rc != PJ_SUCCESS) {
        pj_caching_pool_destroy(&m_caching_pool);
        throw(QObject::tr("pjsip_endpt_create failed").toStdString());
    }

    // 注册当前线程
    // status = pj_thread_register("MyPJSIPThread", m_desc, &m_thread);
    // if (status != PJ_SUCCESS) {
    //     // 处理错误
    //     throw(QObject::tr("pj_thread_register failed").toStdString());
    // }



    return true;
}

/** --------------------------------------------------------------------------------------
 * @name    bool pjsip_Release();
 * @brief   PJSIP库的释放过程
 * @param
 * @return  true:成功 false:失败
 --------------------------------------------------------------------------------------*/
bool CNetCapture::pjsip_Release()
{
    pj_bool_t thread_registered = PJ_FALSE;
    pj_thread_desc desc;
    pj_thread_t* pthread = nullptr;

    //注册一个线程用于释放pjsip资源
    if (!thread_registered && !pj_thread_is_registered())
    {
        if (pj_thread_register(NULL, desc, &pthread) == PJ_SUCCESS)
        {
            thread_registered = PJ_TRUE;
        }
    }

    if (m_endpt)
    {
        pjsip_endpt_destroy(m_endpt);
        m_endpt = nullptr;
    }
    if (&m_caching_pool)
        pj_caching_pool_destroy(&m_caching_pool);




    if (pthread)
    {
        if (pj_thread_is_registered())
            pj_thread_destroy(pthread);
        pthread = nullptr;
    }
    pj_shutdown();
    // if (&m_scanner)
    // {
    //     pj_scan_fini(&m_scanner);
    // }







    return true;
}