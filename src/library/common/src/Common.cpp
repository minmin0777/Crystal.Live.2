/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-13 16:30:21
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-06-25 14:23:55
 * @FilePath: \Crystal.Live.2\src\library\common\src\Common.cpp
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

#include <string>
#include <iostream>
#include <signal.h>
#include "../include/Common.h"
#include <environment.h>
#include "../include/Logger.hpp"
namespace Common
{
    std::string GetVersion()
    {

        std::string strVer = "1.9.3";
        return strVer;
    }

    bool SetEnv()
    {
        enableVirtualTerminalProcessing();
        return true;
    }


    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    bool InitLog(const std::string& channel)
     * @brief   初始化日志库
     * @param   channel -不同的动态库/静态库/可执行文件拥有不同的channal,可以通过channel来区分日志
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------------------------------------------*/
    bool InitLog(const std::string& channel)
    {
        Logger::InitBoostlog(channel);
        //LOG(INFO) << "Logger init success!";
        return true;
    }

}