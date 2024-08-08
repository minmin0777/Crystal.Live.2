/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-11 14:09:23
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-09 10:28:29
 * @FilePath: \Crystal.Live.2\src\library\Config\include\Config.h
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
#include <iostream>
#include <signal.h>
#include "Config.Export.h"
#include "ConfigInfo.h"
namespace Config
{
  Config_EXPORT std::string GetVersion();
  //初始化配置库
  Config_EXPORT bool init();
  //获取配置信息
  Config_EXPORT const CConfigInfo& GetConfigInfo();

} // namespace Config