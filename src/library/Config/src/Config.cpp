
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-07 16:15:12
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-26 14:34:51
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
#include <QtProtobuf>
#include <QProtobufSerializer>
 // 定义一个信号处理函数



static inline std::string g_Version = "1.0.0.1";


namespace qt::examples::sensors {
    class Coordinates;
    class Temperature;
    class WarningNotification;
}
namespace Config
{
    std::string GetVersion()
    {
        return g_Version;
    }

    bool init()
    {

        Common::SetEnv();

        LogInfo logInfo;
        logInfo.Channel = "Config";
        logInfo.Version = GetVersion();
        logInfo.Location = Common::Utility::GetAppPath(logInfo.Channel);

        Common::InitLog(logInfo);
        LOG(DEBUG) << "Config init success";

        auto logger = Service_logger::get();
        std::cout << "logger address:" << &logger << std::endl;
        return true;
    }

    const CConfigInfo& GetConfigInfo()
    {
        return CConfigInfo::Get();

    }



}


