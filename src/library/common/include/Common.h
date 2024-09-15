/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-11 14:09:23
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-09 10:28:29
 * @FilePath: \Crystal.Live.2\src\library\common\include\Common.h
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
#include "Common.Export.h"
#include "logger.hpp"
#include "ThreadSafeQueue.h"
#include "ThreadSafeMap.h"
#include "ThreadSafeVector.h"
#include "CommonStructDefine.h"
#include "Utility.h"
#include "ExceptionEx.h"
namespace Common
{
  Common_EXPORT std::string GetVersion();

  Common_EXPORT bool SetEnv();

  Common_EXPORT bool InitLog(const LogInfo& logInfo);


} // namespace Common