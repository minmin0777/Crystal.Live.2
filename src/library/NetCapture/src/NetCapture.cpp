

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
#include <CNetCapture.h>
#include <thread>
#include <Common.h>
namespace NetCapture
{
    bool Init() noexcept
    {
        Common::SetEnv();
        Common::InitLog("NetCapture");
        CNetCapture::Get().Init();
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

} // namespace NetCapture