#include <ConfigInfo.h>

CConfigInfo::CConfigInfo()
{
}

CConfigInfo::~CConfigInfo()
{
}

bool CConfigInfo::Load()
{
    m_strConfigPath = "./config/";
    return false;
}

size_t CConfigInfo::GetExtensionInfoCount() const
{
    return m_vtExtensionInfos.size();
}
