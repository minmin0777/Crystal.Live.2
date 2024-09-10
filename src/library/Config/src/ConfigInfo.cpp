/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-07 16:15:12
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-26 14:20:36
 * @FilePath: \Crystal.Live.2\src\library\Config\src\ConfigInfo.cpp
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
#include "pch.h"
#include <ConfigInfo.h>
#include <Common.h>
#include <QDebug>
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
        std::string strFilePath = Common::Utility::GetWorkingDirectory();

        boost::algorithm::replace_all(strFilePath, "\\", "/");
        strFilePath += "Configure/system.dat";
        LOG(DEBUG) << "strFilePath:" << strFilePath;
        // 打开文件
        std::ifstream file(strFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file\n";
            return 1;
        }

        // 读取文件内容到字符串
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonStr = buffer.str();

        // 关闭文件
        file.close();


        LOG(DEBUG) << "jsonStr:" << jsonStr;

        auto jv = boost::json::parse(jsonStr);
        boost::json::value log = jv.at("log");
        // 访问解析后的数据
        std::string name = log.at("name").as_string().c_str();
        LOG(DEBUG) << "name:" << name;
        int64_t age = log.at("keepdays").as_int64();
        int64_t level = log.at("level").as_int64();
        boost::json::value pcap = jv.at("pcap");
        int64_t mode = pcap.at("mode").as_int64();
        m_strfilter = pcap.at("filter").as_string().c_str();
        boost::json::value devices = pcap.at("device");
        m_mapNetworkAdapters.clear();
        for (auto& device : devices.as_array())
        {

            std::string DeviceID = device.at("id").as_string().c_str();;

            std::shared_ptr<NetworkAdapterInfo> pNetworkAdapter = std::make_shared<NetworkAdapterInfo>();
            pNetworkAdapter->ID = DeviceID;
            //没有创建的网络适配器信息，进行创建
            if (m_mapNetworkAdapters.find(DeviceID) == m_mapNetworkAdapters.end())
            {
                m_mapNetworkAdapters.insert(std::make_pair(DeviceID, pNetworkAdapter));
            }

        }


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
