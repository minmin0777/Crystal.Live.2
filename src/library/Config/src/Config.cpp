#include <ConfigInfo.h> 
#include <csignal>
#include "Config.h"
// 定义一个信号处理函数
void signalHandler(int signal)
{
    // LOG(fatal) << OUTPUT_LINE << std::endl;
    // LOG(fatal) << "Caught signal " << signal << "----"
    //     << "LibName:" << "Audio library" << ":"
    //     << "Version:" << g_Version
    //     << std::endl;
    // LOG(fatal) << boost::stacktrace::stacktrace();
    // LOG(fatal) << OUTPUT_LINE << std::endl;
    exit(signal);
}

static inline std::string g_Version = "1.0.0.1";
namespace Config
{
    std::string GetVersion()
    {
        return g_Version;
    }

    bool init()
    {
        return true;
    }
}



Config_EXPORT const CConfigInfo& Config::GetConfigInfo()
{
    // TODO: 在此处插入 return 语句
    return CConfigInfo();
}