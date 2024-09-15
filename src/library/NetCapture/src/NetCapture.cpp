
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-24 13:26:36
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-06-24 13:26:49
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\src\NetCapture.cpp
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
 // #define NetCapture_EXPORTS
#include "../include/NetCapture.h"
#include "pch.h"
#include <CNetCapture.h>
// #include <Record.h>
extern class CRecord;
namespace NetCapture
{
    bool Init() noexcept
    {
        Common::SetEnv();
        LogInfo logInfo;
        logInfo.Channel = "NetCapture";
        logInfo.Version = GetVersion();
        logInfo.Location = Common::Utility::GetAppPath(logInfo.Channel);
        Common::InitLog(logInfo);

        CNetCapture::Get().Init();
        LOG(INFO) << "NetCapture Init";
        return  true;
    }

    std::string GetVersion()
    {
        return "0.0.1";
    }
    bool StartCapture()
    {
        return CNetCapture::Get().StartCapture();
    }
    bool StopCapture()
    {
        return CNetCapture::Get().StopCapture();

    }

    size_t GetDevicesInfo(std::vector<std::shared_ptr<NetworkAdapterInfo>>& devices)
    {
        return CNetCapture::Get().GetDevicesInfo(devices);
    }


    /** --------------------------------------------------------------------------------------
     * @name    bool SetRecordCallback(std::function<int32_t(std::shared_ptr<CRecord>)> callback)
     * @brief   设置录音的回调函数
     * @param   callback 录音的回调函数
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    bool SetRecordCallback(std::function<int32_t(std::shared_ptr<CRecord>)> callback)
    {
        return CNetCapture::Get().SetRecordCallback(callback);
    }


} // namespace NetCapture