
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-09 15:53:58
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-10 16:09:21
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\src\CSipServiceThreadWrapper.cpp
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
#include "CSipServiceThreadWrapper.h"
#include <Common.h>
#include <regex>
#include <exception>
#include <QString>
#include <QDebug>
 // #include <esip/parser.h> // 假设的esip头文件
CSipServiceThreadWrapper::CSipServiceThreadWrapper()
    : _t(&CSipServiceThreadWrapper::Start, this) {
    // 构造函数启动线程
    LOG(INFO) << "CSipServiceThreadWrapper created.\n";

}

CSipServiceThreadWrapper::~CSipServiceThreadWrapper()
{
    m_TaskStatus = Thread_Status::Thread_Status_Stop;
    m_stop_cv.notify_all();

    if (_t.joinable()) {
        _t.join(); // 确保线程结束
    }
    LOG(DEBUG) << "CSipServiceThreadWrapper destroyed.\n";
}


void CSipServiceThreadWrapper::on_recvfrom(hio_t* io, void* buf, int readbytes) {
    printf("on_recvfrom fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    char localaddrstr[SOCKADDR_STRLEN] = { 0 };
    char peeraddrstr[SOCKADDR_STRLEN] = { 0 };
    printf("[%s] <=> [%s]\n",
        SOCKADDR_STR(hio_localaddr(io), localaddrstr),
        SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    QString str = QString::fromLocal8Bit((const char*)buf);
    QDebug(&str) << "on_recvfrom fd=" << hio_fd(io) << " readbytes=" << readbytes;
    printf("< %.*s", readbytes, (char*)buf);
    // 回显数据
    printf("> %.*s", readbytes, (char*)buf);


    //hio_write(io, buf, readbytes);
}


uint32_t CSipServiceThreadWrapper::Start()
{

    LOG(HEAD) << "-------------------SipServiceThread Start--------------------";
    //m_pNetAdapterInfo 为空时 退出线程
    try
    {

        const char* host = "0.0.0.0";

        hloop_t* loop = hloop_new(0);
        hio_t* io = hloop_create_udp_server(loop, host, 5060);
        if (io == NULL) {
            return -20;
        }

        hio_setcb_read(io, on_recvfrom);
        hio_read(io);
        hloop_run(loop);
        hloop_free(&loop);
        LOG(HEAD) << "-------------------SipServiceThread End--------------------";
        return true;
    }



    catch (const std::system_error& e)
    {
        LOG(ERROR) << "System Error: " << e.what() << " Error Code: " << e.code() << " Error Message: " << e.code().message();


    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << e.what();


    }

    LOG(HEAD) << "-------------------SipServiceThread End--------------------";

    return 0;
}
