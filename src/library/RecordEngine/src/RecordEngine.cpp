
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
#include "Record.h"


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
using namespace boost::multi_index;
// 定义多索引容器的索引
struct int_key_tag {};
struct str_key_tag {};
static Common::ThreadSafeMap<std::string, std::shared_ptr<CRecord>> g_RecordMap;

namespace RecordEngine
{

    // 定义回调函数
    int32_t RecordCallback(std::shared_ptr<CRecord> pRecord) {
        // 处理捕获到的数据包，例如打印数据包内容
        if (pRecord == nullptr) {
            LOG(ERROR) << "pRecord is nullptr";
            return 0;
        }
        if (pRecord->RecordType == "INVITE")
        {

            if (g_RecordMap.insert(pRecord->CallID, pRecord) == false)
            {
                LOG(ERROR) << "CallID:" << pRecord->CallID << "已经存在" << " | 当前录音数量:" << g_RecordMap.size();
                return 0;
            }


            LOG(INFO) << "开始录音,CallID:" << pRecord->CallID << " | 当前录音数量:" << g_RecordMap.size() << std::endl;
        }

        if (pRecord->RecordType == "BYE")
        {
            g_RecordMap.erase(pRecord->CallID);
            LOG(INFO) << "结束录音,CallID:" << pRecord->CallID << " | 当前录音数量:" << g_RecordMap.size() << std::endl;
        }

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
        //1. 读入配置文件
        Config::init();
        //2. 初始化录制引擎
        NetCapture::Init();
        //3. 初始化录音容器





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