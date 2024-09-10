#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#if defined(_WIN32) || defined(_WIN64)
    #ifndef _WINSOCK2API_
        #include <winsock2.h>
    #endif
#else
    #include <unistd.h>
    #include <dirent.h>     // for mkdir,rmdir,chdir,getcwd
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <netinet/in.h>
    #include <net/if.h>
    #include <arpa/inet.h>
#endif

#include "hv/hloop.h"
#include "hv/hsocket.h"
#include <Common.h>

class CSipServiceThreadWrapper {
public:
    CSipServiceThreadWrapper();
    ~CSipServiceThreadWrapper();

    uint32_t Start();
    void Stop() {
        // 线程停止 
        // 线程要执行的任务

        std::cout << "Thread started.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Thread finished.\n";
    }
    static void on_recvfrom(hio_t* io, void* buf, int readbytes);
private:
    std::thread _t;
    //条件变量，用于线程同步，通知线程停止
    static inline std::condition_variable m_stop_cv;
    //线程状态
    static inline std::atomic<Thread_Status> m_TaskStatus;

    static inline std::mutex m_stop_mtx;

};