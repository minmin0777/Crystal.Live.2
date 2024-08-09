#include "pch.h"
#include <ConfigInfo.h>


CConfigInfo::CConfigInfo()
{
    Load();
}

CConfigInfo::~CConfigInfo()
{
}


bool CConfigInfo::Load()
{
    using namespace boost::locale::conv;
    //m_strConfigPath = "./config/";
    try {

        //定义basic_ptree<std::string, std::string>对象
        boost::property_tree::ptree pt;
        std::string strFilePath = Common::Utility::GetWorkerDirectory();
        boost::algorithm::replace_all(strFilePath, "\\", "/");
        strFilePath += "config/Setting.json";
        //读取json文件
        boost::property_tree::read_json(strFilePath, pt);

        // //得到TCS服务器信息
        // m_TCSServerInfo.ssi = pt.get<int32_t>("tcs_server.ssi");
        // m_TCSServerInfo.uid = from_utf(pt.get<std::string>("tcs_server.uid"), "GBK");
        // m_TCSServerInfo.pwd = from_utf(pt.get<std::string>("tcs_server.pwd"), "GBK");

        // //得到Websock服务启动信息
        // m_wsServerInfo.Port = pt.get<int32_t>("sync.ws_servers.port");
        // m_wsServerInfo.Uid = from_utf(pt.get<std::string>("sync.ws_servers.user"), "GBK");
        // m_wsServerInfo.Pwd = from_utf(pt.get<std::string>("sync.ws_servers.pwd"), "GBK");

        // //得到CQ的RestAPI服务的信息，用于信令传递
        // m_RestAPI_server_info.Url = from_utf(pt.get<std::string>("sync.restapi_server.url"), "GBK");
        // m_RestAPI_server_info.Port = pt.get<int32_t>("sync.restapi_server.port");



        // //获取日志配置属性
        // m_nLog_Output_Mode = pt.get<int>("log.output_mode");
        // m_nLog_KeepDays = pt.get<int>("log.keepdays");
        // m_nLog_Level = pt.get<int>("log.level");
        // boost::log::core::get()->reset_filter();

        // boost::log::core::get()->set_filter
        // (
        //     expr::attr<LOG_level>("Severity") >= m_nLog_Level
        // );

        // //获取任务启动时间和停止时间
        // m_TaskStartTime = from_utf(pt.get<std::string>("task.start_time"), "GBK");
        // m_TaskDuration = pt.get<double>("task.duration");


        return true;
    }
    catch (std::exception& ex) {
        LOG(ERROR) << ex.what();
    }
    return false;
}

bool CConfigInfo::Save()
{
    return false;
}

size_t CConfigInfo::GetExtensionInfoCount() const
{
    return m_vtExtensionInfos.size();
}
