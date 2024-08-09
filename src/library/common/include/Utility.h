
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-10 13:40:24
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-10 13:42:11
 * @FilePath: \Crystal.Live.2\src\library\common\include\Utility.h
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  Remark         : Description                                              |
 * @#|----------------------------------------------------------------------------|
 * @#|  Change History :                                                          |
 * @#|  <Date>     | <Version> | <Author>       | <Description>                   |
 * @#|----------------------------------------------------------------------------|
 * @#|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
 * @#|----------------------------------------------------------------------------|
 * @#|  通用函数库                                                                |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ————————————————————————————————————————————————————————————————————————————————————————*/

#pragma once
#include <Common.Export.h>
#include <string>
#include <boost/filesystem.hpp>
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <vector>
namespace Common
{
    class Common_EXPORT Utility {

    public:
        /** --------------------------------------------------------------------------------------------------------------------------
         * @name  std::string GetWorkerDirectory(bool bEndWithSlash = true) noexcept
         * @brief 获取工作目录
         * @param bEndWithSlash 是否以斜杠结尾
         * @return 工作目录
         --------------------------------------------------------------------------------------------------------------------------*/
        static std::string GetWorkerDirectory(bool bEndWithSlash = true) noexcept;


        /** --------------------------------------------------------------------------------------------------------------------------
         * @name  std::string GetMACAddress(const char* MAC) noexcept
         * @brief 获取MAC地址
         * @param MAC MAC地址
         * @return MAC地址
         --------------------------------------------------------------------------------------------------------------------------*/
        static std::string GetMACAddress(const char* MAC) noexcept;

        /** --------------------------------------------------------------------------------------------------------------------------
         * @name  std::vector<std::string> SplitString(const std::string& input, const std::string& separator) noexcept
         *  @brief 分割字符串
         *  @param input 输入字符串
         *  @param separator 分隔符
         *  @return 分割后的字符串数组
         --------------------------------------------------------------------------------------------------------------------------*/
        static std::vector<std::string> SplitString(const std::string& input, const std::string& separator) noexcept;

        /** --------------------------------------------------------------------------------------------------------------------------
         * @name  size_t SplitString(const std::string& input, const std::string& separator, std::vector<std::string>& vtRet) noexcept
         * @brief 分割字符串
         * @param input 输入字符串
         * @param separator 分隔符
         * @param vtRet 分割后的字符串数组
         * @return 分割后的字符串数组数量
         --------------------------------------------------------------------------------------------------------------------------*/
        static size_t SplitString(const std::string& input, const std::string& separator, std::vector<std::string>& vtRet) noexcept;
        /** --------------------------------------------------------------------------------------------------------------------------
         * @name  std::string GetMACAddress(const char* MAC) noexcept
         * @brief 获取MAC地址
         * @param MAC MAC地址
         * @return MAC地址
         --------------------------------------------------------------------------------------------------------------------------*/
        static std::string ConvMACAddress(const char MAC[6]) noexcept;

    };


}