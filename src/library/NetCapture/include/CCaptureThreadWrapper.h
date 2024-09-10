
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-09 11:27:00
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-09 15:54:08
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\include\CCaptureThreadWrapper.h
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  Remark         : 抓包线程封装类                                           |
 * @#|----------------------------------------------------------------------------|
 * @#|  Change History :                                                          |
 * @#|  <Date>     | <Version> | <Author>       | <Description>                   |
 * @#|----------------------------------------------------------------------------|
 * @#|  2024/08/01 | 1.0.0.1   | jason.chen     | Create file                     |
 * @#|----------------------------------------------------------------------------|
 * @#| 核心类-抓包的工作线程处理类,pcap的处理依赖这个线程实现。                   |
 * @#|一个线程负责一个网卡的抓包工作。收取到的包由本类交付给上层的协议处理。      |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ————————————————————————————————————————————————————————————————————————————————————————*/

#pragma once

#include <CNetDefine.h>
#include <Common.h>
#include <thread>
#include <atomic>

class CCaptureThreadWrapper {
public:
    CCaptureThreadWrapper(const NetworkAdapterInfo* pNetAdapterInfo);
    ~CCaptureThreadWrapper();

    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    uint32_t Start();
     * @brief   开始抓包主线程
     * @return  >0:成功 <=0:失败
     * @note    无
     --------------------------------------------------------------------------------------------------------------------------*/
    uint32_t Start();

    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    void Stop();
     * @brief   停止抓包主线程
     * @return  无
     --------------------------------------------------------------------------------------------------------------------------*/
    void Stop();

    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    void ParseQueueThread();
     * @brief   解析队列线程,从队列中取出数据，进行解析
     * @return  无
    --------------------------------------------------------------------------------------------------------------------------*/
    void ParseQueueThread();

    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    void StartCaptureByPcap(const std::string& strFilter);
     * @brief   通过pcap抓包
     * @param   strFilter 过滤表达式
     * @return  获取的包数量 -1：失败 0:没有包 >0:成功
    --------------------------------------------------------------------------------------------------------------------------*/
    uint32_t StartCaptureByPcap(const std::string& strFilter);

    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    void StopCaptureByPcap();
     * @brief   判断是否正在抓包
     * @return  true:正在抓包 false:没有抓包
     --------------------------------------------------------------------------------------------------------------------------*/
    bool isCapturing();

    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    void IsRepeatedPacket(const std::shared_ptr<PCAP_PACKAGE>& pPackage) const;
     * @brief   判断当前包是否是重复的
     * @param   pCurPackage 当前包
     * @return  返回 true:重复包 false:不是重复包
    -------------------------------------------------------------------------------------------------------------------------- */
    bool IsRepeatedPacket(const std::shared_ptr<PCAP_PACKAGE>& pCurPackage) const;

private:
    std::jthread _t_Capture;     //线程1, 抓包线程
    std::jthread _t_ParseQueue;  //线程2, 数据分析线程

    //绑定的网络适配器信息
    const NetworkAdapterInfo* m_pNetAdapterInfo = nullptr;
    //条件变量，用于线程同步，通知线程停止
    static inline std::condition_variable m_stop_cv;
    //线程状态
    std::atomic<Thread_Status> m_TaskStatus;
    //线程停止标志
    std::mutex m_stop_mtx;
    //warning: 抓包队列，存放抓包数据
    Common::ThreadSafeQueue<std::shared_ptr<PCAP_PACKAGE>> m_queue;
    //最后的一个包，记录最后的包的作用是用于重复包的比较
    std::shared_ptr<PCAP_PACKAGE> m_pLastPackage = nullptr;
};