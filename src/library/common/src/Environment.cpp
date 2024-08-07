/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-06 12:43:20
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-06-21 11:09:15
 * @FilePath: \Crystal.Live.2\src\library\common\src\Environment.cpp
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
#ifdef _WIN32
 //windows平台时需要包含的头文件
#include <windows.h>
#endif

#include "../include/Environment.h"


//启用虚拟终端处理  
void enableVirtualTerminalProcessing() {
    boost::locale::generator gen;
    std::locale loc = gen("zh_CN.UTF-8");

    // 将全局和本地环境设置为中文环境
    std::locale::global(loc);
    std::cout.imbue(loc);
#ifdef _WIN32
    // 获取控制台输出的句柄
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to get standard output handle.\n";
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        std::cerr << "Failed to get console mode.\n";
        return;
    }

    // 启用虚拟终端处理
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hOut, dwMode)) {
        std::cerr << "Failed to set console mode.\n";
    }

    //设置控制台为utf-8
    SetConsoleOutputCP(CP_UTF8);
#else
    return;
#endif
}