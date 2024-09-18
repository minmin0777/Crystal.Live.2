/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-07 16:15:12
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-22 14:19:45
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\src\CCaptureThreadWrapper.cpp
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  ClassName         : Description                                           |
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

#include "CCaptureThreadWrapper.h"
#include <Common.h>
#include <regex>
#include <qtranslator.h>
#include <CNetCapture.h>
#include <Record.h>
#include <RtpParse.h>
#include <Config.h>
#include <pjsip.h>
#include <pjlib.h>
#include <pjlib-util.h>
#include <pjsua.h>
#include <pjmedia.h>
 /** --------------------------------------------------------------------------------------------------------------------------
  * @brief 构造函数
  * @param pNetAdapterInfo 网络适配器信息
  * @return 无
  --------------------------------------------------------------------------------------------------------------------------*/

CCaptureThreadWrapper::CCaptureThreadWrapper(const NetworkAdapterInfo* pNetAdapterInfo)
    :m_pNetAdapterInfo(pNetAdapterInfo),
    m_TaskStatus(Thread_Status::Thread_Status_Running),
    _t_Capture(&CCaptureThreadWrapper::Start, this),            // 启动start线程
    _t_ParseQueue(&CCaptureThreadWrapper::ParseQueueThread, this) // 启动ParseQueueThread线程
{

}

/** --------------------------------------------------------------------------------------------------------------------------
 * @brief 析构函数
 * @return 无
 --------------------------------------------------------------------------------------------------------------------------*/
CCaptureThreadWrapper::~CCaptureThreadWrapper()
{
    Stop();
}

/** --------------------------------------------------------------------------------------------------------------------------
 * @name    void StartCaptureByPcap(const std::string& strFilter);
 * @brief   通过pcap抓包
 * @param   strFilter 过滤表达式
 * @return  获取的包数量 -1：失败 0:没有包 >0:成功
--------------------------------------------------------------------------------------------------------------------------*/
uint32_t CCaptureThreadWrapper::StartCaptureByPcap(const std::string& strFilter)
{
    uint nRet = 0;
    pcap_t* pcap = nullptr;
    try
    {
        // 线程要执行的任务
        struct pcap_pkthdr* pPacketHeader = nullptr;      // 网络包头
        char* pPacketData = nullptr;
        // 保存出错信息
        char szErrBuf[PCAP_ERRBUF_SIZE] = { 0 };
        // 打开网络适配器
        pcap = pcap_open(m_pNetAdapterInfo->ID.c_str(), PCAP_SNAP_LEN, PCAP_OPENFLAG_PROMISCUOUS, PCAP_READ_TIMEOUT, NULL, szErrBuf);  // 网络包内容
        // 设置内核缓冲区大小
        pcap_setbuff(pcap, PCAP_KERNEL_BUFFER_SIZE);
        // 设置最小包大小
        pcap_setmintocopy(pcap, PCAP_MIN_TO_COPY_SIZE);
        // 设置用户缓冲区大小
        pcap_setuserbuffer(pcap, PCAP_USER_BUFFER_SIZE);
        int ret = -1;
        bpf_program filter = { 0 };
        //std::string szFilter = "port 5060 and src host 192.168.8.147 or dst host 192.168.8.147";
        ret = pcap_compile(pcap, &filter, strFilter.c_str(), 1, 0xffffff);
        if (ret != 0)
        {
            // 创建一个错误码
            std::error_code ec(-101, std::generic_category());
            std::string strError = std::format("无法解析过滤表达式:{0} \n err:{1}", strFilter, pcap_geterr(pcap));
            throw std::system_error(ec, strError);

        }
        // 设置pcap过滤器
        ret = pcap_setfilter(pcap, &filter);



        // 检查sip消息正则表达式，用于过滤sip消息
        std::regex sip_regex(R"(^SIP/2\.0 |^INVITE |^ACK |^BYE |^CANCEL |^OPTIONS |^REGISTER |^NOTIFY |^SUBSCRIBE |^REFER |^INFO |^MESSAGE)");
        while (1)
        {
            try
            {
                // 检查线程状态
                if (m_TaskStatus == Thread_Status::Thread_Status_Stop)
                {
                    LOG(WARN) << "m_TaskStatus == Thread_Status::Thread_Status_Stop ,thread exit";
                    break;
                }
                // 从网卡中读取一个包
                int ret = pcap_next_ex(pcap, &pPacketHeader, (const u_char**)&pPacketData);
                if (ret != PCAP_READ_PACKET_RETURN_SUCCESS)
                {

                    if (ret == PCAP_READ_PACKET_RETURN_TIMEOUT) //抓包超时，打印等待包的日志
                    {
                        LOG_EVERY(WARN, 100) << QObject::tr("pcap_next_ex Timeout ret:").toStdString() << ret;
                    }
                    continue;
                }
                if (ret == PCAP_READ_PACKET_RETURN_SUCCESS)
                {

                    LOG_EVERY(WARN, 10000) << "pcap_next_ex return : " << ret;
                    std::shared_ptr<PCAP_PACKAGE> pPcapPackage = std::make_shared<PCAP_PACKAGE>();

                    pPcapPackage->nContentLen = pPacketHeader->caplen;
                    //PCAP_PACKAGE pcapPackage;
                    memcpy(&pPcapPackage->header, pPacketHeader, sizeof(*pPacketHeader));
                    pPcapPackage->pData = new char[pPacketHeader->caplen];
                    //pcapPackage.pData = (char*)HeapAlloc(g_cRuntimeCommon.GetDefaultHeap(), HEAP_ZERO_MEMORY, pPacketHeader->caplen);
                    memcpy(pPcapPackage->pData, pPacketData, pPacketHeader->caplen);
                    const char* pCurPointer = (const char*)pPacketData;       // 指针游标，当前处理位置
                    // 先取出Ethernet Header
                    PETHERNET_HEADER pEthernet = (PETHERNET_HEADER)pCurPointer;
                    pCurPointer += sizeof(ETHERNET_HEADER);
                    //

                    pPcapPackage->nContentLen -= sizeof(ETHERNET_HEADER);

                    if (0x8 != pEthernet->EthernetType
                        && 0x800 != pEthernet->EthernetType
                        && 0x81 != pEthernet->EthernetType)
                    {

                        //HeapFree(g_cRuntimeCommon.GetDefaultHeap(), 0, pcapPackage.pData);
                        continue;
                    }
                    if (0x81 == pEthernet->EthernetType)
                    {
                        // VLAN, 游标需要后移4 Bytes
                        pCurPointer += 4;
                        pPcapPackage->nContentLen -= 4;
                    }

                    PIP_HEADER pIPHeader = (PIP_HEADER)pCurPointer;
                    pIPHeader->TotalLength = ntohs(pIPHeader->TotalLength);
                    if (IP_PACKET_TCP != pIPHeader->Protocol && IP_PACKET_UDP != pIPHeader->Protocol)
                    {
                        continue;
                    }
                    pCurPointer += sizeof(IP_HEADER);
                    pPcapPackage->nContentLen -= sizeof(IP_HEADER);


                    if (IP_PACKET_TCP == pIPHeader->Protocol)
                    {
                        PTCP_HEADER pTcpHeader = (PTCP_HEADER)pCurPointer;

                        // 网络字节序转成主机字节序
                        pTcpHeader->SourcePort = ntohs(pTcpHeader->SourcePort);
                        pTcpHeader->DestPort = ntohs(pTcpHeader->DestPort);
                        pCurPointer += sizeof(TCP_HEADER);
                        pPcapPackage->nContentLen -= sizeof(TCP_HEADER);
                        //LOG(INFO) << "TCP Header: " << pTcpHeader->SourcePort << " -> " << pTcpHeader->DestPort;

                    }

                    if (IP_PACKET_UDP == pIPHeader->Protocol)
                    {
                        PUDP_HEADER pUDPHeader = (PUDP_HEADER)pCurPointer;

                        // 网络字节序转成主机字节序
                        pUDPHeader->SourcePort = ntohs(pUDPHeader->SourcePort);
                        pUDPHeader->DestPort = ntohs(pUDPHeader->DestPort);
                        pCurPointer += sizeof(UDP_HEADER);
                        pPcapPackage->nContentLen -= sizeof(UDP_HEADER);
                        //LOG(INFO) << "UDP Header: " << pUDPHeader->SourcePort << " -> " << pUDPHeader->DestPort;

                    }



                    //LOG(INFO) << Common::Utility::GetWorkingDirectory();

                    // std::string strSrcMacAddress = Utility::GetMACAddress(pEthernet->SourceHostMac);
                    // std::string strDstMacAddress = Utility::GetMACAddress(pEthernet->DestHostMac);


                    // LOG(DEBUG) << "Ethernet Header: " << strSrcMacAddress << " -> " << strDstMacAddress << " Type: " << pEthernet->EthernetType;
                    if (std::regex_search(pCurPointer, sip_regex))
                    {
                        pPcapPackage->pContent = new char[pPcapPackage->nContentLen + 1];
                        pPcapPackage->nPackageType = PCAP_PACKAGE::ENUM_PACKAGE_TYPE::CONTROL;
                        memset(pPcapPackage->pContent, NULL, pPcapPackage->nContentLen + 1);
                        memcpy(pPcapPackage->pContent, pCurPointer, pPcapPackage->nContentLen);
                        // 进一步处理SIP消息...拷贝到消息队列
                        // LOG(HEAD) << "SIP message detected :" << pPcapPackage->nContentLen << "\n" << pPcapPackage->pContent;
                        m_queue.Enqueue(pPcapPackage);
                        //CNetCapture::ParserSipMessage(strSipMsg);

                        // LOG(DEBUG) << "Queue size: " << m_queue.Size()
                        //     << " | pcapPackage.data :" << Common::Utility::toHex(pPcapPackage->pData, 10);

                    }
                    else
                    {
                        //rtp_header rtpHeader;

                        // RTP消息
                        if (!IsRTPPacket((const char*)pCurPointer, pPcapPackage->nContentLen))
                        {
                            // RTP消息

                            continue;
                        }
                        //parse_rtp_header((uint8_t*)pCurPointer, &rtpHeader);
                        // LOG(HEAD) << "UDP message detected \n" << pCurPointer;

                        pPcapPackage->pContent = new char[pPcapPackage->nContentLen + 1];
                        pPcapPackage->nPackageType = PCAP_PACKAGE::ENUM_PACKAGE_TYPE::RTP;
                        memset(pPcapPackage->pContent, NULL, pPcapPackage->nContentLen + 1);
                        memcpy(pPcapPackage->pContent, pCurPointer, pPcapPackage->nContentLen);

                        m_queue.Enqueue(pPcapPackage);

                        // LOG(DEBUG) << "Queue size: " << m_queue.Size()
                        //     << " | pcapPackage.data :" << Common::Utility::toHex(pPcapPackage->pData, 10);
                        // 进一步处理SIP消息...
                    }
                }

            }
            catch (const std::system_error& e)
            {
                LOG(ERROR) << "System Error: " << e.what() << " Error Code: " << e.code() << " Error Message: " << e.code().message();
                continue;

            }

            catch (const std::exception& e)
            {
                LOG(ERROR) << e.what();
                continue;
            }
        }
        nRet = 1;   // 成功
    }

    catch (const std::system_error& e)
    {
        nRet = -1;
        LOG(ERROR) << "System Error: " << e.what() << " Error Code: " << e.code() << " Error Message: " << e.code().message();

    }

    catch (const std::exception& e)
    {
        nRet = -1;  // 失败
        LOG(ERROR) << e.what();
    }

    if (pcap)
    {
        pcap_close(pcap);
        pcap = nullptr;
    }

    return nRet;
}

/** --------------------------------------------------------------------------------------------------------------------------
 * @name    void StopCaptureByPcap();
 * @brief   判断是否正在抓包
 * @return  true:正在抓包 false:没有抓包
 --------------------------------------------------------------------------------------------------------------------------*/

bool CCaptureThreadWrapper::isCapturing()
{
    return m_TaskStatus == Thread_Status::Thread_Status_Running;
}

/** --------------------------------------------------------------------------------------------------------------------------
     * @name    uint32_t Start();
     * @brief   开始抓包主线程
     * @return  >0:成功 <=0:失败
     * @note    无
     --------------------------------------------------------------------------------------------------------------------------*/
uint32_t CCaptureThreadWrapper::Start()
{
    uint32_t nRet = 0;

    LOG(HEAD) << OUTPUT_LINE
        << QObject::tr("*** CaptureThread Start ***").toStdString() << "\n"
        << QObject::tr("Network Adapter Name: ").toStdString() << m_pNetAdapterInfo->Name << "\n"
        << QObject::tr("Network Adapter ID: ").toStdString() << m_pNetAdapterInfo->ID << "\n"
        << QObject::tr("IP Address : ").toStdString() << m_pNetAdapterInfo->IpAddress << "\n"
        << QObject::tr("Thread id : ").toStdString() << std::this_thread::get_id() << "\n"
        << OUTPUT_LINE;
    //m_pNetAdapterInfo 为空时 退出线程
    if (m_pNetAdapterInfo == nullptr)
    {
        LOG(ERROR) << "m_pNetAdapterInfo is nullptr, thread exit.";
        return -1;
    }

    try
    {

        //std::string szFilter = std::format("port 5060 and (udp and  port not 8500 and port not 5353 and port not 53 and port not 1900 and port not 5355 and port not 138 and port not 137 and port not 443 and port not 80) ");

        // for (size_t i = 0; i < count; i++)
        // {
        //     /* code */
        // }



        nRet = StartCaptureByPcap(Config::GetConfigInfo().m_strfilter);
    }
    catch (const std::system_error& e)
    {
        LOG(ERROR) << "System Error: " << e.what() << " Error Code: " << e.code() << " Error Message: " << e.code().message();
        nRet = -1;
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << e.what();
        nRet = -1;

    }

    LOG(HEAD) << OUTPUT_LINE
        << QObject::tr("*** CaptureThread end ***").toStdString() << "\n"
        << QObject::tr("Network Adapter Name: ").toStdString() << m_pNetAdapterInfo->Name << "\n"
        << QObject::tr("Network Adapter ID: ").toStdString() << m_pNetAdapterInfo->ID << "\n"
        << QObject::tr("IP Address : ").toStdString() << m_pNetAdapterInfo->IpAddress << "\n"
        << QObject::tr("Thread id : ").toStdString() << std::this_thread::get_id() << "\n"
        << OUTPUT_LINE;
    return nRet;
}

/** --------------------------------------------------------------------------------------------------------------------------
 * @name    void Stop();
 * @brief   停止抓包主线程
 * @return  无
 --------------------------------------------------------------------------------------------------------------------------*/
void CCaptureThreadWrapper::Stop()
{
    // 设置线程状态为停止
    m_TaskStatus = Thread_Status::Thread_Status_Stop;
    // 通知线程停止，触发停止的条件变量
    m_stop_cv.notify_all();

    // 等待_t_Capture线程结束
    if (_t_Capture.joinable()) {
        _t_Capture.join(); // 确保线程结束
    }

    // 等待_t_ParseQueue线程结束
    if (_t_ParseQueue.joinable()) {
        _t_ParseQueue.join(); // 确保线程结束
    }
    m_queue.shrink_to_fit();
}

/** --------------------------------------------------------------------------------------------------------------------------
 * @name    void ParseQueueThread();
 * @brief   解析队列线程,从队列中取出数据，进行解析
 * @return  无
--------------------------------------------------------------------------------------------------------------------------*/
void CCaptureThreadWrapper::ParseQueueThread()
{
    LOG(HEAD) << OUTPUT_LINE
        << QObject::tr("*** ParseQueueThread Start ***").toStdString() << "\n"
        << QObject::tr("Network Adapter : ").toStdString() << m_pNetAdapterInfo->Name << "\n"
        << QObject::tr("IP Address : ").toStdString() << m_pNetAdapterInfo->IpAddress << "\n"
        << QObject::tr("Thread id : ").toStdString() << std::this_thread::get_id() << "\n"
        << OUTPUT_LINE;
    try
    {

        while (m_TaskStatus != Thread_Status::Thread_Status_Stop)
        {
            std::shared_ptr<PCAP_PACKAGE> pPcapPackage = nullptr;
            if (m_queue.WaitAndDequeue(pPcapPackage, 5) && pPcapPackage != nullptr)
            {
                LOG_EVERY(DEBUG, 10000) << "Queue size: " << m_queue.Size()
                    << " | pPcapPackage->nContentLen :" << pPcapPackage->nContentLen;
                //     << " | pPcapPackage->pContent : \n" << pPcapPackage->pContent;

                // warning:判断是否是重复包，是的话就过滤掉
                if (IsRepeatedPacket(pPcapPackage))
                {
                    continue;
                }

                // warning:判断包类型，无法分辨的包类型直接过滤掉
                if (pPcapPackage->nPackageType == PCAP_PACKAGE::ENUM_PACKAGE_TYPE::UNKNOWN)
                {
                    LOG(ERROR) << "Package Type is UNKNOWN";
                    continue;
                }


                // 解析控制信息包信息，一般为SIP消息或CTI消息
                if (pPcapPackage->nPackageType == PCAP_PACKAGE::ENUM_PACKAGE_TYPE::CONTROL)
                {
                    std::shared_ptr<CRecord> pRecord = std::make_shared<CRecord>();
                    CNetCapture::parse_sip_message_from_string(pPcapPackage->pContent, pRecord.get());
                    LOG(HEAD) << OUTPUT_LINE << "| Recv SIP [" << pRecord->RecordType << "] message\n"
                        << "| CallID: " << pRecord->CallID << "\n"
                        << "| Caller: " << pRecord->Caller << " -> Callee: " << pRecord->Callee << "\n"
                        << "| StartTime: " << pRecord->StartTime << "\n"
                        << OUTPUT_LINE;
                    CNetCapture::m_RecordCallback(pRecord);
                    // if (pRecord->RecordType == "INVITE")
                    // {


                    //     //收到INVITE消息，创建一个新的录音对象
                    //     if (CNetCapture::m_RecordCallback)
                    //     {
                    //         CNetCapture::m_RecordCallback(pRecord);
                    //     }

                    // }

                    // if (pRecord->RecordType == "BYE")
                    // {
                    //     CNetCapture::m_RecordCallback(pRecord);
                    //     //收到BYE消息，结束录音,删除录音对象
                    // }
                    // LOG_EVERY(INFO, 10000) << pRecord->CallID;
                    //m_pLastPackage = pPcapPackage; // warning:记录最后一个包,用于下次比较,防止重复包
                    // 
                    if (pRecord->CallID.empty())
                    {
                        LOG(ERROR) << "CallID is empty";
                        continue;
                    }

                }
                // 解析控制信息包信息，一般为SIP消息或CTI消息
                if (pPcapPackage->nPackageType == PCAP_PACKAGE::ENUM_PACKAGE_TYPE::RTP)
                {
                    //LOG(INFO) << "RTP Count :" << nRTPCount;
                    // RTP消息
                    rtp_header rtpHeader = { 0 };
                    parse_rtp_header((uint8_t*)pPcapPackage->pContent, &rtpHeader);
                    if (rtpHeader.marker == 1)
                    {
                        LOG(INFO) << "RTP Header: " << rtpHeader.ssrc << " -> " << rtpHeader.seq_num;
                    }

                    continue;

                }
                m_pLastPackage = pPcapPackage; // warning:记录最后一个包,用于下次比较,防止重复包
            }
            else
            {
                // 使用移动语义创建一个新的队列，并交换旧队列和新队列的内容
                //m_queue.Clear();
                LOG_EVERY(DEBUG, 10000) << L"WaitAndDequeue - Timeout流程";
                // 等待信号通知线程退出
                std::unique_lock<std::mutex> lk(m_stop_mtx);
                // 等待1s，如果返回值为std::cv_status::no_timeout，说明有信号通知线程退出
                // warning :防止条件变量虚假唤醒，必须满足两个条件
                if (m_stop_cv.wait_for(lk, 1s) == std::cv_status::no_timeout
                    && m_TaskStatus == Thread_Status::Thread_Status_Stop)
                {
                    std::wstring strMsg = L"检测到退出信号,退出线程...";
                    throw ExceptionEx(strMsg);
                }

            }
        }
    }
    catch (const std::system_error& e)
    {
        LOG(ERROR) << "System Error: " << e.what()
            << " Error Code: " << e.code()
            << " Error Message: " << e.code().message();
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << e.what();
    }
    LOG(HEAD) << OUTPUT_LINE
        << QObject::tr("*** ParseQueueThread end ***").toStdString() << "\n"
        << QObject::tr("Network Adapter : ").toStdString() << m_pNetAdapterInfo->Name << "\n"
        << QObject::tr("IP Address : ").toStdString() << m_pNetAdapterInfo->IpAddress << "\n"
        << QObject::tr("Thread id : ").toStdString() << std::this_thread::get_id() << "\n"
        << OUTPUT_LINE;
}

/** --------------------------------------------------------------------------------------------------------------------------
 * @name    void IsRepeatedPacket(const std::shared_ptr<PCAP_PACKAGE>& pPackage);
 * @brief   判断当前包是否是重复的
 * @param   pCurPackage 当前包
 * @return  返回 true:重复包 false:不是重复包
 * @warning 比较的时机-放入Queue的pop过程中进行比较
-------------------------------------------------------------------------------------------------------------------------- */
bool CCaptureThreadWrapper::IsRepeatedPacket(const std::shared_ptr<PCAP_PACKAGE>& pCurPackage) const
{
    if (pCurPackage == nullptr)
        return false;

    //未设置的m_pLastPackage 无需比较
    if (m_pLastPackage == nullptr)
        return false;

    if (m_pLastPackage->CompareContent(pCurPackage))
    {
        LOG(DEBUG) << "RepeatedPacket:" << pCurPackage->pContent;
        return true;
    }
    return false;


}