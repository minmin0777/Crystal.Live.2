/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-26 11:56:49
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-26 13:11:42
 * @FilePath: \Crystal.Live.2\src\library\common\include\ExceptionEx.h
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
 * @#|                                                                            |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ————————————————————————————————————————————————————————————————————————————————————————*/
#pragma once
#include <exception>
#include <string>
#include <cstring>
#include <cwchar>
#include <boost/locale.hpp>

class ExceptionEx : public std::exception {
private:
    std::wstring messageW; // 使用std::wstring存储错误信息
    mutable std::string messageA; // 用于what()方法返回

public:
    // 接受std::string的构造函数
    ExceptionEx(const std::string& msg) : messageW(boost::locale::conv::utf_to_utf<wchar_t>(msg, boost::locale::conv::stop)) {}
    // 接受std::wstring的构造函数
    ExceptionEx(const std::wstring& msg) : messageW(msg) {}

    // 返回错误信息（const char*）
    virtual const char* what() const noexcept override {
        // 将std::wstring转换为std::string
        messageA = boost::locale::conv::utf_to_utf<char>(messageW, boost::locale::conv::stop);
        return messageA.c_str();
    }

    // 获取错误信息（std::wstring）
    std::wstring getMessageW() const noexcept {
        return messageW;
    }
};