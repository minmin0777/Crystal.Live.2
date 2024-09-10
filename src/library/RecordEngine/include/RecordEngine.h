/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-22 10:22:47
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-22 10:27:30
 * @FilePath: \Crystal.Live.2\src\library\RecordEngine\include\RecordEngine.h
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

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include "RecordEngine.Export.h"
#include <functional>
#include <Record.h>
namespace RecordEngine
{

    /** --------------------------------------------------------------------------------------
     * @name    bool Init()
     * @brief   初始化录制引擎库
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    RecordEngine_EXPORT bool Init() noexcept;

    /** --------------------------------------------------------------------------------------
     * @name    std::string GetVersion()
     * @brief   获取版本号
     * @return  版本号
     --------------------------------------------------------------------------------------*/
    RecordEngine_EXPORT std::string GetVersion() noexcept;

    /** --------------------------------------------------------------------------------------
     * @name    bool StartRecordService()
     * @brief   启动录制服务
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    RecordEngine_EXPORT bool StartRecordService();

    /** --------------------------------------------------------------------------------------
     * @name    bool StopRecordService()
     * @brief   停止录制服务
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    RecordEngine_EXPORT bool StopRecordService();


    RecordEngine_EXPORT int32_t RecordCallback(std::shared_ptr<CRecord> pRecord);


}