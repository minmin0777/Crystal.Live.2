/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-24 23:46:50
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-09 14:16:22
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\include\NetCapture.h
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
#include "NetCapture.Export.h"
#include <CNetDefine.h>
// #include <Record.h>
extern class CRecord;
namespace NetCapture
{

    /** --------------------------------------------------------------------------------------
     * @name    bool Init()
     * @brief   初始化网络捕获库
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    NetCapture_EXPORT bool Init() noexcept;

    /** --------------------------------------------------------------------------------------
     * @name    std::string GetVersion()
     * @brief   获取版本号
     * @return  版本号
     --------------------------------------------------------------------------------------*/
    NetCapture_EXPORT std::string GetVersion();

    /** --------------------------------------------------------------------------------------
     * @name    bool StartCapture()
     * @brief   开始抓包录制
     * @param   void
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    NetCapture_EXPORT bool StartCapture();

    /** --------------------------------------------------------------------------------------
     * @name    bool StopCapture()
     * @brief   停止抓包录制
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    NetCapture_EXPORT bool StopCapture();

    /** --------------------------------------------------------------------------------------
     * @name    size_t GetDevicesInfo(std::vector<std::shared_ptr<NetworkAdapterInfo>>& devices)
     * @brief   获取网络设备信息
     * @param   devices 存放网络设备信息列表的集合
     * @return  设备数量(size_t)

     --------------------------------------------------------------------------------------*/
    NetCapture_EXPORT size_t GetDevicesInfo(std::vector<std::shared_ptr<NetworkAdapterInfo>>& devices);


    /** --------------------------------------------------------------------------------------
     * @name    bool SetRecordCallback(std::function<int32_t(std::shared_ptr<CRecord>)> callback)
     * @brief   设置录音的回调函数
     * @param   callback 录音的回调函数
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    NetCapture_EXPORT bool SetRecordCallback(std::function<int32_t(std::shared_ptr<CRecord>)> callback);

}