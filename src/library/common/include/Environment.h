/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-05-27 09:19:57
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-06-13 15:18:00
 * @FilePath: \Crystal.Live.2\src\library\common\src\Environment.h
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

#pragma region <--过滤警告项-->
 //@warning 过滤掉不必要的编译警告
#pragma warning(disable:4100)	//warning C4100: 未引用的形参
#pragma warning(disable:4244)	//从“int”转换到“char”，可能丢失数据
#pragma warning(disable:4310)	//类型强制转换截断常量值
#pragma warning(disable:4267)	//warning C4267: “参数”: 从“size_t”转换到“int”，可能丢失数据
#pragma warning(disable:4996)	//warning C4996: This function or variable may be unsafe.
#pragma warning(disable:4091)	//warning C4091: This function or variable may be unsafe.
#pragma warning(disable:4005)	//warning C4005: 宏重定义
#pragma endregion 


#define WIN32_LEAN_AND_MEAN
 // #define _CRT_SECURE_NO_WARNINGS
#if defined(_WIN_32) || defined(_WIN64)
#include "windows.h"
#include "minwindef.h"
#include "shellapi.h"
#include "winbase.h"
#pragma comment(lib, "dbghelp.lib")
#include <direct.h>
#pragma message("Linking to lib file: dbghelp.lib")

#endif

#include <filesystem>

typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long COLORREF;

//RGB宏定义可能已经存在于wingdi.h，这里做了判断,避免重复定义
#ifndef RGB
#define RGB(r,g,b)    ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
#endif

#pragma region <--装载boost核心-->

#define BOOST_LIB_DIAGNOSTIC

#include <boost/circular_buffer.hpp> // Include the missing header file for boost/circular_buffer.hpp
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/code_converter.hpp> //编码转换
#include <boost/lexical_cast.hpp>             //字符串数字互转
#include <boost/locale.hpp> //本地信息，语言，操作系统，日期格式等
#include <boost/locale/boundary.hpp>
#include <boost/property_tree/ini_parser.hpp> //属性树，用于配置文件ini
#include <boost/property_tree/json_parser.hpp> //json解析
#include <boost/property_tree/ptree.hpp>       //属性树，用于配置文件
#include <boost/property_tree/xml_parser.hpp>  //xml解析

// guid ,uuid
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <boost/bind/bind.hpp>

#include <boost/thread.hpp>
#include <boost/stacktrace.hpp>
#include <exception>
#include <boost/shared_ptr.hpp>
#include <boost/core/null_deleter.hpp>
#include <csignal>
#include <iostream>
#include <boost/exception/all.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iomanip>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <ostream>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/locale.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/exception/exception.hpp>
#include <boost/filesystem.hpp>
#include <iomanip>
#include <thread>
#undef ERROR
#define OUTPUT_LINE "-----------------------------------------------------------------------\n"

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace src = boost::log::sources;
namespace bpt = boost::property_tree;

using namespace boost::placeholders;
using namespace std::literals::chrono_literals;
// #include <boost/program_options.hpp>
#pragma endregion < --装载boost核心-->

namespace env {
  enum color : const COLORREF {
    red = RGB(255, 0, 0),             // 红色
    dark_red = RGB(128, 0, 0),        // 洋红色
    black = RGB(0, 0, 0),             // 黑色
    dark_grey = RGB(122, 122, 122),   // 黑灰色
    grey = RGB(192, 192, 192),        // 灰色
    blue = RGB(0, 0, 128),            // 蓝色
    sapphire = RGB(9, 92, 142),       // 宝蓝
    white = RGB(255, 255, 255),       // 白色
    white2 = RGB(244, 247, 252),      // 奶白
    magenta = RGB(255, 0, 255),       // 品红
    dark_magenta = RGB(189, 99, 197), // 深品红
    gold = RGB(255, 215, 0),          // 金色
    dark_orange = RGB(255, 128, 0),   // 深橘色
    purple = RGB(82, 52, 172),        // 紫色
    light_green = RGB(106, 173, 47),  // 亮绿色
    orange = RGB(211, 72, 39),        // 橘红色
    sky_blue = RGB(37, 113, 235),     // 天蓝色
    green = RGB(25, 150, 27),         // 苍绿色
    rose_red = RGB(172, 25, 61)       // 玫红色

  };

}

// 判断指针是否为空的宏
#define assert_ptr(f)                                                          \
  if ((f)) {                                                                   \
    LOG(ERROR) << "Pointer is null, var : " #f << "\n";                        \
    throw std::exception("Pointer is null, var : " #f);                    \
  }


void enableVirtualTerminalProcessing();