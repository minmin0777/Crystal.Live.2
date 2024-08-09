/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-07 16:15:12
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-09 10:07:05
 * @FilePath: \Crystal.Live.2\src\library\Config\src\Config.cpp
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
#include <ConfigInfo.h> 
#include <csignal>
#include "Config.h"
#include <Common.h>
 // 定义一个信号处理函数
// void signalHandler(int signal)
// {
//     // LOG(fatal) << OUTPUT_LINE << std::endl;
//     // LOG(fatal) << "Caught signal " << signal << "----"
//     //     << "LibName:" << "Audio library" << ":"
//     //     << "Version:" << g_Version
//     //     << std::endl;
//     // LOG(fatal) << boost::stacktrace::stacktrace();
//     // LOG(fatal) << OUTPUT_LINE << std::endl;
//     exit(signal);
// }

static inline std::string g_Version = "1.0.0.1";
namespace Config
{
    std::string GetVersion()
    {
        return g_Version;
    }

    bool init()
    {

        Common::SetEnv();
        Common::InitLog("Config");
        LOG(INFO) << "Config init success";
        CConfigInfo& config = CConfigInfo::Get();
        return true;
    }
}


