/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-05-27 09:19:57
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-06-13 15:53:10
 * @FilePath: \Crystal.Live.2\src\library\audio\main.cpp
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
#include <Audio.h> 
#include <csignal>
 // 定义一个信号处理函数
void signalHandler(int signal)
{
    // LOG(fatal) << OUTPUT_LINE << std::endl;
    // LOG(fatal) << "Caught signal " << signal << "----"
    //     << "LibName:" << "Audio library" << ":"
    //     << "Version:" << g_Version
    //     << std::endl;
    // LOG(fatal) << boost::stacktrace::stacktrace();
    // LOG(fatal) << OUTPUT_LINE << std::endl;
    exit(signal);
}
static inline std::string g_Version = "1.0.0.1";
namespace Audio
{
    std::string GetVersion()
    {
        // 注册信号处理函数
        std::signal(SIGSEGV, signalHandler);
        std::signal(SIGABRT, signalHandler);

        return g_Version;
    }
}

