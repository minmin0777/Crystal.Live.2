/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-07 16:15:12
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-09 09:58:17
 * @FilePath: \Crystal.Live.2\src\library\Config\include\ConfigInfo.h
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
 /*————————————————————————————————————————————————————————————————————————————————————————
  * @Author: jason minmin0777@126.com
  * @Date: 2024-08-07 16:15:12
  * @LastEditors: jason minmin0777@126.com
  * @LastEditTime: 2024-08-08 17:16:12
  * @FilePath: \Crystal.Live.2\src\library\Config\include\ConfigInfo.h
  * @Description: 配置信息类
  * @
  * @#|----------------------------------------------------------------------------|
  * @#|  Class Name  : CConfigInfo |     配置信息类                                |
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
#include <common.h>
#include <string>
#include <ConfigStructDefine.h>
#include <vector>
#include <memory>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/json.hpp>
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

/**
 * @class    CConfigInfo
 * @brief    配置信息类
 * @version  0.0.1
 *
 */
class CConfigInfo
{
public:
    CConfigInfo();
    virtual ~CConfigInfo();
    CConfigInfo(const CConfigInfo&) = delete;
    CConfigInfo& operator=(const CConfigInfo&) = delete;

    friend class boost::serialization::access; // 允许序列化访问

    /**
     * @name    static CConfigInfo& Get()
     * @brief   用于获取全局唯一的CConfigInfo实例
     * @return  CConfigInfo& 返回全局唯一的CConfigInfo实例
     * @remark  无需手动创建CConfigInfo实例，只需调用Get()即可
     * @warning C++11 标准保证了当一个线程首次访问局部静态变量时，其初始化将是线程安全的
     */
    static CConfigInfo& Get() {
        static CConfigInfo instance; // 局部静态变量
        return instance;
    }
protected:
    /**
     * @name    bool Load()
     * @brief   加载配置文件
     * @return  bool 加载成功返回true，否则返回false
     * @remark  无
     * @warning 无
     */
    virtual bool    Load();

    virtual bool    Save();

    // virtual bool    Update(const std::shared_ptr<ExtensionInfo>& pExtensionInfo);

    // virtual bool    Delete(const std::shared_ptr<ExtensionInfo>& pExtensionInfo);

    // virtual bool    Add(const std::shared_ptr<ExtensionInfo>& pExtensionInfo);

    virtual size_t  GetExtensionInfoCount() const;

protected:
    systemInfo m_systemInfo;
    std::vector<std::shared_ptr<ExtensionInfo>> m_vtExtensionInfos;

    std::string m_strConfigPath;
    std::string m_strLogPath;
    std::string m_strLogName;
    std::string m_strLogSuffix;


};