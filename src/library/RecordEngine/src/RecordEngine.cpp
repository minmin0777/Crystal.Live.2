
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-22 10:27:19
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-28 11:13:04
 * @FilePath: \Crystal.Live.2\src\library\RecordEngine\src\RecordEngine.cpp
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
#include <Common.h>
#include <regex>
#include <Config.h>
#include <NetCapture.h>
#include <QObject.h>


#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>
#include "RecordEngine.h"
namespace RecordEngine
{

    // 定义回调函数
    int32_t RecordCallback(std::shared_ptr<CRecord> pRecord) {
        // 处理捕获到的数据包，例如打印数据包内容
        if (pRecord == nullptr) {
            LOG(ERROR) << "pRecord is nullptr";
            return 0;
        }


        LOG(INFO) << "开始录音,CallID:" << pRecord->CallID << std::endl;

        return 1;
    }
    // 动态库内部的一个示例函数
    void dummy_function() {}
    /** --------------------------------------------------------------------------------------
     * @name    bool Init()
     * @brief   初始化录制引擎库
     * @return  true:成功 false:失败
     --------------------------------------------------------------------------------------*/
    bool Init() noexcept
    {

        // 初始化日志系统
        Common::SetEnv();
        LogInfo logInfo;
        logInfo.Channel = "RecordEngine";
        logInfo.Version = GetVersion();
        logInfo.Location = Common::Utility::GetAppPath(logInfo.Channel);
        Common::InitLog(logInfo);
        auto logger = Service_logger::get();

        std::cout << "logger address:" << &logger << std::endl;
        LOG(INFO) << "RecordEngine init success";

        Config::init();
        //1. 初始化录制引擎
        NetCapture::Init();

        //2. 读入配置文件
        //3. 初始化录制器
        //4. 初始化录制线程

        //RecordEngine 初始化需要给NetCapture设置信令回调函数
        NetCapture::SetRecordCallback(RecordCallback);
        return true;
    }
    std::string GetVersion() noexcept
    {
        return "1.0.1";
    }

    bool StartRecordService()
    {
        std::string message;
        bool bServiceStart = NetCapture::StartCapture();
        message = std::format("{0}\nRecordService Start {1}! {2}:{3}",
            OUTPUT_LINE,
            bServiceStart == true ? "Success" : "Failed",
            "Config.version",
            Config::GetVersion()
        );
        LOG(INFO) << QObject::tr(message.c_str()).toStdString();
        return bServiceStart;

    }

    bool StopRecordService()
    {
        return NetCapture::StopCapture();
    }




}