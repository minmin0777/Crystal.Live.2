
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-09 15:53:58
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-10 16:09:21
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\src\CSipUdpClientThreadWrapper.cpp
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
#include "CSipUdpClientThreadWrapper.h"
#include <Common.h>
#include <regex>
#include <exception>
#include <QString>
#include <QDebug>
#include <chrono>
#include <boost/exception/all.hpp>
 //CallbackType CSipUdpClientThreadWrapper::onMessageSentCallback = nullptr;
 // #include <esip/parser.h> // 假设的esip头文件
CSipUdpClientThreadWrapper::CSipUdpClientThreadWrapper()
    : _t(&CSipUdpClientThreadWrapper::Start, this) {
    // 构造函数启动线程
    LOG(INFO) << "CSipUdpClientThreadWrapper created.\n";

}

CSipUdpClientThreadWrapper::~CSipUdpClientThreadWrapper()
{
    m_TaskStatus = Thread_Status::Thread_Status_Stop;
    m_stop_cv.notify_all();

    if (_t.joinable()) {
        _t.join(); // 确保线程结束
    }
    // 释放事件循环
    //hloop_free(&_loop);

    if (m_cli)
    {
        m_cli->stop();
        delete m_cli;
        m_cli = nullptr;
    }
    LOG(DEBUG) << "CSipUdpClientThreadWrapper destroyed.\n";
}


void CSipUdpClientThreadWrapper::on_recvfrom(hio_t* io, void* buf, int readbytes) {
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

void on_timer(htimer_t* timer) {
    // 使用chrono获取当前时间
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    // 格式化时间
    std::ostringstream formatted_time;
    formatted_time << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");

    std::string str = formatted_time.str(); // 将格式化后的时间转换为string

    std::cout << "> " << str << std::endl;
    // 获取userdata
    hio_t* io = (hio_t*)hevent_userdata(timer);
    // 发送时间字符串
    hio_write(io, str.c_str(), str.length());
}

void CSipUdpClientThreadWrapper::on_recv(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
    printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    // 回显数据
    QString str = QString::fromLocal8Bit((const char*)buf->data());
    if (onMessageSentCallback)
        onMessageSentCallback(str);



    //hio_write(io, buf, readbytes);
}

void CSipUdpClientThreadWrapper::OnWriteComplete(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
    printf("> %.*s\n", (int)buf->size(), (char*)buf->data());


    //hio_write(io, buf, readbytes);
}
uint32_t CSipUdpClientThreadWrapper::Start()
{

    LOG(HEAD) << "-------------------SipServiceThread Start--------------------";
    //m_pNetAdapterInfo 为空时 退出线程
    try
    {
        m_cli = new hv::UdpClient();
        int sockfd = m_cli->createsocket(5060);
        if (sockfd < 0) {
            return -20;
        }
        printf("client sendto port %d, sockfd=%d ...\n", 5060, sockfd);


        m_cli->onMessage = std::bind(&CSipUdpClientThreadWrapper::on_recv, this, std::placeholders::_1, std::placeholders::_2);
        m_cli->onWriteComplete = std::bind(&CSipUdpClientThreadWrapper::OnWriteComplete, this, std::placeholders::_1, std::placeholders::_2);
        m_cli->start();
        m_cli->loop();


        // _loop = hloop_new(0);
        // // 创建UDP客户端
        // _io = hloop_create_udp_client(_loop, "127.0.0.1", 5060);
        // if (_io == NULL) {
        //     return -20;
        // }
        // // 设置read回调
        // hio_setcb_read(_io, on_recvfrom);
        // // 开始读
        // hio_read(_io);
        // // 添加一个定时器
        // //htimer_t* timer = htimer_add(hevent_loop(_io), on_timer, 1000, INFINITE);
        // // 设置userdata
        // //hevent_set_userdata(sendSipMessagebyUdp, _io);
        // // 运行事件循环
        // hloop_run(_loop);


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



uint32_t CSipUdpClientThreadWrapper::sendSipMessagebyUdp(const char* buf, int len)
{



    LOG(HEAD) << "-------------------SipServiceThread Start--------------------";
    //m_pNetAdapterInfo 为空时 退出线程
    try
    {

        // 创建套接字
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return -1;
        }

        // 设置服务器地址
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(5060); // 服务器端口
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器IP

        // 发送数据
        //const char* message = "Hello, UDP Server!";
        if (::sendto(sockfd, buf, len, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            std::cerr << "Error sending message" << std::endl;
            close(sockfd);
            return -1;
        }
        std::cout << "Message sent to UDP server successfully" << std::endl;

        // 接收响应
        char buffer[1024];
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, &fromlen);
        if (n < 0) {
            std::cerr << "Error receiving response" << std::endl;
            close(sockfd);
            return 1;
        }
        //stroutput.append(QString::fromLocal8Bit(buffer));
        //stroutput.append("------------------------------------------------------------------------------\n");
        memset(buffer, NULL, sizeof(buffer));
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, &fromlen);
        if (n < 0) {
            std::cerr << "Error receiving response" << std::endl;
            close(sockfd);
            return 1;
        }
        //stroutput.append(QString::fromLocal8Bit(buffer));


        // 关闭套接字
        close(sockfd);



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


uint32_t CSipUdpClientThreadWrapper::sendSipMessagebyUdp2(const std::string& ip, const int32_t& port, const char* buf, int len)
{



    LOG(HEAD) << "-------------------SipServiceThread Start--------------------";
    //m_pNetAdapterInfo 为空时 退出线程
    try
    {

        sockaddr_u peer_addr;
        memset(&peer_addr, 0, sizeof(peer_addr));
        std::string strIP = ip;
        if (strIP.empty()) {
            strIP = "127.0.0.1";
        }
        int ret = sockaddr_set_ipport(&peer_addr, strIP.c_str(), port);
        if (ret != 0) {
            return NABS(ret);
        }
        int bytes_written = m_cli->sendto(buf, len, &peer_addr.sa);

        //int bytes_written = ::sendto(hio_fd(_io), buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));

        // 发送时间字符串
        //int bytes_written = hio_write(_io, buf, len);
        if (bytes_written < 0) {
            // 处理错误
            printf("Failed to write data\n");
        }
        else {
            printf("Successfully wrote %d bytes\n", bytes_written);
        }




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
