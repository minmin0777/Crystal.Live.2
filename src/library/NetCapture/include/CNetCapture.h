
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-08 09:56:21
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-08 09:56:39
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\include\CNetCapture.h
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
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <CNetDefine.h>
#include <mutex>
#include <atomic>
#include <Common.h>
 // #include <Record.h>
#include <CCaptureThreadWrapper.h>
#include <pjsip.h>
#include <pjlib.h>
// #include "../../RecordEngine/include/Record.h"

extern class CRecord;

 /**
  * @brief 网络抓包类
  * @version 0.0.1
  * @date 2024-07-08
  * @remark
  */
class CNetCapture
{
    CNetCapture() noexcept;                                 //构造函数
    virtual ~CNetCapture() noexcept;                    //析构函数
    //CNetCapture() = delete;                                 // 删除默认构造函数
    //~CNetCapture() = delete;                                // 删除默认析构函数
    CNetCapture(const CNetCapture&) = delete;               // 删除拷贝构造函数
    CNetCapture& operator=(const CNetCapture&) = delete;    // 删除拷贝赋值函数
    CNetCapture(CNetCapture&&) = delete;                    // 删除移动构造函数
    CNetCapture& operator=(CNetCapture&&) = delete;         // 删除移动赋值函数

public:
    //获取单例接口
    static CNetCapture& Get()
    {

        static CNetCapture instance;
        return instance;
    }
public:

    /*! --------------------------------------------------------------------------------------------------------------------------
     * @name   size_t GetDevicesInfo(std::vector<std::shared_ptr<NetworkAdapterInfo>>& devices) noexcept;
     * @brief  获取设备信息
     * @return 设备数量(size_t)
     * @remark GetDevicesInfo函数用于获取当前计算机的网络设备信息，并将设备信息存储在devices结构中，并返回设备数量。
     --------------------------------------------------------------------------------------------------------------------------*/
    size_t GetDevicesInfo(std::vector<std::shared_ptr<NetworkAdapterInfo>>& devices) noexcept;

    /*! --------------------------------------------------------------------------------------------------------------------------
     * @name   bool Init()
     * @brief  获取设备
     * @return 初始化是否成功,成功返回网络设备数量，失败返回0
     * @remark GetDevicesInfo函数用于获取当前计算机的网络设备信息，并将设备信息存储在devices结构中，并返回设备数量。
    --------------------------------------------------------------------------------------------------------------------------*/
    size_t Init() noexcept;

    /*! --------------------------------------------------------------------------------------------------------------------------
     * @name   bool StartCapture()
     * @brief  启动网络抓包线程
     * @param
     * @return 无
     --------------------------------------------------------------------------------------------------------------------------*/
    bool StartCapture() noexcept;

    /*! --------------------------------------------------------------------------------------------------------------------------
     * @name   bool StopCapture()
     * @brief  停止网络抓包线程
     * @param
     * @return 无
     --------------------------------------------------------------------------------------------------------------------------*/
    bool StopCapture() noexcept;

    static int txdata_test_uri_params(const std::string& strSipMsg);

    static int ParserSipMessage(const std::string& strSipMsg);
    static int parse_sip_message_from_string(const std::string& sip_message_str, void* pData);

    /*! --------------------------------------------------------------------------------------------------------------------------
     * @name   bool SetRecordCallback(std::function<void(const std::string&)> callback)
     * @brief  设置录音的回调函数
     * @param  callback 录音的回调函数
     * @return true:成功 false:失败
     --------------------------------------------------------------------------------------------------------------------------*/
    bool SetRecordCallback(std::function<int32_t(std::shared_ptr<CRecord>)> callback);
protected:

    /*! --------------------------------------------------------------------------------------------------------------------------
     * @name   std::string GetMacAddress(const std::string& szPcapAdapterID)
     * @brief  获取MAC地址
     * @param  szPcapAdapterID 网络适配器ID
     * @return MAC地址
     --------------------------------------------------------------------------------------------------------------------------*/

    std::string GetMacAddress(const std::string& szPcapAdapterID) const;


    /** --------------------------------------------------------------------------------------------------------------------------
     * @name   std::string GetNetworkAdapterDeviceNameByPcap(const std::string& strValue) const;
     * @brief  获取网络适配器(PCAP)名称
     * @note   得到网络适配器的DeviceName，本功能需要Npcap支持，并去除额外的Pcap组织的无效头尾信息
     * @param  strValue 网络适配器ID
     * @return 网络适配器(PCAP)名称
     --------------------------------------------------------------------------------------------------------------------------*/
    std::string GetNetworkAdapterDeviceNameByPcap(const std::string& strValue) const;


    /** --------------------------------------------------------------------------------------------------------------------------
     * @name   bool GetNetworkAdapterInfo(const std::string& szPcapAdapterID, NetworkAdapterInfo& adapterInfo)
     * @brief  获取网络适配器ID
     * @param  strValue 网络适配器名称
     * @return 网络适配器ID
     --------------------------------------------------------------------------------------------------------------------------*/
    std::string GetNetworkAdapterIDByPcap(const std::string& strValue) const;

    /** --------------------------------------------------------------------------------------------------------------------------
         * @name    void StopCaptureByPcap();
         * @brief   判断是否正在抓包
         * @return  true:正在抓包 false:没有抓包
         --------------------------------------------------------------------------------------------------------------------------*/
    bool isCapturing();


    bool setFilter(const std::string& filter);
    bool setDevice(const std::string& device);


public:
    //设备信息 ,inline 修饰符用于提供static函数的定义，以便在头文件中使用
    static std::vector<std::shared_ptr<NetworkAdapterInfo>> m_devices;
    //抓包线程
    static std::vector<std::shared_ptr<CCaptureThreadWrapper>> m_vtCaptureThreadWrappers;
    //条件变量，用于线程同步，通知线程停止
    static inline std::condition_variable m_stop_cv;
    //线程状态
    static inline std::atomic<Thread_Status> m_TaskStatus;

    static inline std::mutex m_mutex;

    static inline std::function<int32_t(std::shared_ptr<CRecord>) > m_RecordCallback = nullptr;


};

