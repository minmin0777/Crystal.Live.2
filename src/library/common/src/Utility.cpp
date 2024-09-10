/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-10 14:39:02
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-10 16:08:49
 * @FilePath: \Crystal.Live.2\src\library\common\src\Utility.cpp
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  Remark         : Description        Utility 通用函数库                    |
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
 /** --------------------------------------------------------------------------------------------------------------------------
  * @brief Utility 通用函数库
  * @version 0.0.0.1
  --------------------------------------------------------------------------------------------------------------------------*/

#include "Utility.h"
#include <format>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>
namespace Common
{



    /** --------------------------------------------------------------------------------------------------------------------------
    * @name  std::string Utility::GetWorkingDirectory(bool bEndWithSlash, bool bUseLinuxSlash)
    * @brief 获取工作目录
    * @param bEndWithSlash 是否以斜杠结尾
    * @return 工作目录
    --------------------------------------------------------------------------------------------------------------------------*/
    std::string Utility::GetWorkingDirectory(bool bEndWithSlash, bool bUseLinuxSlash) noexcept
    {

        boost::filesystem::path exePath = boost::dll::program_location();
        if (exePath.empty())
        {
            return "";
        }
        boost::filesystem::path exeDir = exePath.parent_path();
        std::string strPath = exeDir.string();
        if (boost::filesystem::is_directory(exeDir))
        {
            if (strPath.back() != '/' && strPath.back() != '\\')
            {
                if (bEndWithSlash)  strPath += "/";
            }

        }
        else
        {
            std::string::size_type pos = strPath.find_last_of("\\/");
            if (pos != std::string::npos)
            {
                strPath = strPath.substr(0, pos);
            }
            if (bEndWithSlash)  strPath += "/";
        }
        if (bUseLinuxSlash)
            strPath = boost::replace_all_copy(strPath, "\\", "/");
        else
            strPath = boost::replace_all_copy(strPath, "/", "\\");

        return strPath;
    }
    /** --------------------------------------------------------------------------------------------------------------------------
     * @name    static std::string GetAppPath(const std::string& appName, bool bUseLinuxSlash = true) noexcept;
     * @brief   获取目标App的完整路径
     * @param   appName 目标App名称
     * @param   bUseLinuxSlash 是否使用Linux斜杠
     * @return  目标App的完整路径
     --------------------------------------------------------------------------------------------------------------------------*/
    std::string Utility::GetAppPath(const std::string& appName, bool bUseLinuxSlash) noexcept
    {
        boost::filesystem::path appPath;

#ifdef _WIN32
        char pathBuffer[MAX_PATH] = { 0 };
        std::string strAppName[2] = { appName + ".dll" , appName + ".exe" }; ;
        HMODULE hModule = nullptr;
        for (size_t n = 0;n < 2;n++)
        {
            hModule = GetModuleHandleA(strAppName[n].c_str());
            if (hModule != nullptr)
            {
                if (GetModuleFileNameA(hModule, pathBuffer, MAX_PATH)) {
                    appPath = boost::filesystem::path(pathBuffer);
                    break;
                }
            }


        }

#else
        char pathBuffer[MAX_PATH];
        ssize_t count = readlink("/proc/self/exe", pathBuffer, PATH_MAX);
        if (count != -1) {
            appPath = boost::filesystem::path(std::string(pathBuffer, count));
        }
#endif

        // 如果需要，将路径分隔符替换为Linux风格
        if (bUseLinuxSlash) {
            return appPath.generic_string();
        }
        else {
            return appPath.string();
        }
    }

    /*------------------------------------------------------------------------------------------------------------------------------*/
    /** --------------------------------------------------------------------------------------------------------------------------
     * @name  std::string GetMACAddress(const char* MAC) noexcept
     * @brief 获取MAC地址
     * @param MAC MAC地址
     * @return MAC地址
     --------------------------------------------------------------------------------------------------------------------------*/

    std::string Utility::ConvMACAddress(const char MAC[6]) noexcept
    {
        if (MAC == nullptr)
        {
            return "";
        }

        std::string strRet = std::format("{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}", MAC[0], MAC[1],
            MAC[2], MAC[3],
            MAC[4], MAC[5]);
        return strRet;
    }


    /** --------------------------------------------------------------------------------------------------------------------------
     * @name  std::vector<std::string> SplitString(const std::string& input, const std::string& separator) noexcept
     *  @brief 分割字符串
     *  @param input 输入字符串
     *  @param separator 分隔符
     *  @return 分割后的字符串数组
     --------------------------------------------------------------------------------------------------------------------------*/
    std::vector<std::string> Utility::SplitString(const std::string& input, const std::string& separator) noexcept
    {

        std::vector<std::string> result;
        size_t start = 0;
        size_t end = input.find(';');
        while (end != std::string::npos) {
            result.push_back(input.substr(start, end - start));
            start = end + 1;
            end = input.find(separator, start);
        }
        // 添加最后一个分号后的字符串
        result.push_back(input.substr(start));
        return result;

    }
    /** --------------------------------------------------------------------------------------------------------------------------
     * @name  size_t SplitString(const std::string& input, const std::string& separator, std::vector<std::string>& vtRet) noexcept
     * @brief 分割字符串
     * @param input 输入字符串
     * @param separator 分隔符
     * @param vtRet 分割后的字符串数组
     * @return 分割后的字符串数组数量
     ----------------------------------------------------------------------------------------------------------------------------*/

    size_t Utility::SplitString(const std::string& input, const std::string& separator, std::vector<std::string>& vtRet) noexcept
    {
        vtRet.clear();
        vtRet.shrink_to_fit();

        size_t start = 0;
        size_t end = input.find(';');
        while (end != std::string::npos) {
            vtRet.push_back(input.substr(start, end - start));
            start = end + 1;
            end = input.find(separator, start);
        }
        // 添加最后一个分号后的字符串
        vtRet.push_back(input.substr(start));
        return vtRet.size();
    }

    /** --------------------------------------------------------------------------------------------------------------------------
    * 转换字节到十六进制字符串
    * @name  toHex
    * @param data 字节数组
    * @param length 字节数组长度
    * @return 十六进制字符串
    --------------------------------------------------------------------------------------------------------------------------*/
    std::string Utility::toHex(const char* data, size_t length)
    {
        std::string result;
        boost::algorithm::hex(data, data + length, std::back_inserter(result));

        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }
    /** --------------------------------------------------------------------------------------------------------------------------
     * @brief 二进制比较
     * @param data1
     * @param length1
     * @param data2
     * @param length2
     * @return 是否相等
    --------------------------------------------------------------------------------------------------------------------------*/
    bool Utility::BinaryCompare(const char* src, size_t src_len, const char* target, size_t target_len)  noexcept
    {
        if (src_len != target_len)
            return false;
        return memcmp(src, target, src_len) == 0;
    }

}