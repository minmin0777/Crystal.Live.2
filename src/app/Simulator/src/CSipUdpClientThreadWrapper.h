#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

#include <string>
#include <functional>
#include <memory>
#include <atomic>
#include <condition_variable>
#ifdef _MSC_VER
#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif
#endif


#include "hv/hloop.h"
#include "hv/hsocket.h"
#include <hv/UdpClient.h>
#include <hv/Channel.h>
#include <hv/Buffer.h>
#include <Common.h>
#include <QString>
#include <QDebug>
class CSipUdpClientThreadWrapper {
public:
    CSipUdpClientThreadWrapper();
    ~CSipUdpClientThreadWrapper();

    void on_recv(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

    void OnWriteComplete(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

    uint32_t Start();
    uint32_t sendSipMessagebyUdp2(const std::string& ip, const int32_t& port, const char* buf, int len);
    uint32_t sendSipMessagebyUdp(const char* buf, int len);
    void Stop() {
        // 线程停止 
        // 线程要执行的任务

        std::cout << "Thread started.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Thread finished.\n";
    }
    void on_recvfrom(hio_t* io, void* buf, int readbytes);

    // 定义回调函数类型
    using CallbackType = std::function<size_t(const QString&)>;
    // 设置回调函数
    void setOnMessageSentCallback(const CallbackType& callback) {
        onMessageSentCallback = nullptr;
        onMessageSentCallback = callback;
        qDebug() << "设置了Callback\n";
    }
private:
    CallbackType onMessageSentCallback = nullptr;
private:
    std::jthread _t;    //
    hio_t* _io;    // UDP客户端句柄
    hloop_t* _loop;  // 事件循环句柄

    //条件变量，用于线程同步，通知线程停止
    static inline std::condition_variable m_stop_cv;

    //线程状态
    static inline std::atomic<Thread_Status> m_TaskStatus;

    static inline std::mutex m_stop_mtx;
    hv::UdpClient* m_cli;

};