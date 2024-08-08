#pragma once
#include <string>
#include <ConfigStructDefine.h>
#include <vector>
#include <memory>

class CConfigInfo
{
public:
    CConfigInfo();
    virtual ~CConfigInfo();
    CConfigInfo(const CConfigInfo&) = delete;
    CConfigInfo& operator=(const CConfigInfo&) = delete;

protected:

    virtual bool    Load();

    // virtual bool    Save();

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