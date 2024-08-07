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


namespace Utility {

    /** --------------------------------------------------------------------------------------------------------------------------
        * @name  std::string GetWorkerDirectory(bool bEndWithSlash = true) noexcept
        * @brief 获取工作目录
        * @param bEndWithSlash 是否以斜杠结尾
        * @return 工作目录
        --------------------------------------------------------------------------------------------------------------------------*/
    std::string GetWorkerDirectory(bool bEndWithSlash) noexcept
    {

        boost::filesystem::path exePath = boost::filesystem::initial_path();
        std::string strPath = exePath.string();
        if (bEndWithSlash)
        {
            std::string::size_type pos = strPath.find_last_of("\\/");
            if (pos != std::string::npos)
            {
                strPath = strPath.substr(0, pos);
            }
            strPath += "/";
        }
        return strPath;
    }

    /*------------------------------------------------------------------------------------------------------------------------------*/
    /** --------------------------------------------------------------------------------------------------------------------------
     * @name  std::string GetMACAddress(const char* MAC) noexcept
     * @brief 获取MAC地址
     * @param MAC MAC地址
     * @return MAC地址
     --------------------------------------------------------------------------------------------------------------------------*/

    std::string ConvMACAddress(const char MAC[6]) noexcept
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
    std::vector<std::string> SplitString(const std::string& input, const std::string& separator) noexcept
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

    size_t SplitString(const std::string& input, const std::string& separator, std::vector<std::string>& vtRet) noexcept
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
    /*------------------------------------------------------------------------------------------------------------------------------*/

}



