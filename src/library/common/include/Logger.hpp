
/*
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-06 12:43:20
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-06-21 10:00:51
 * @FilePath: \Crystal.Live.2\src\library\common\include\Logger.hpp
 * @Description:
 *
 * #|----------------------------------------------------------------------------|
 * #|  Remark         : Description                                              |
 * #|----------------------------------------------------------------------------|
 * #|  Change History :                                                          |
 * #|  <Date>     | <Version> | <Author>       | <Description>                   |
 * #|----------------------------------------------------------------------------|
 * #|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
 * #|----------------------------------------------------------------------------|
 * #|                                                                            |
 * #|----------------------------------------------------------------------------|
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */


 /*
   *                        _oo0oo_
   *                       o8888888o
   *                       88" . "88
   *                       (| -_- |)
   *                       0\  =  /0
   *                     ___/`---'\___
   *                   .' \\|     |// '.
   *                  / \\|||  :  |||// \
   *                 / _||||| -:- |||||- \
   *                |   | \\\  - /// |   |
   *                | \_|  ''\---/''  |_/ |
   *                \  .-\__  '-'  ___/-. /
   *              ___'. .'  /--.--\  `. .'___
   *           ."" '<  `.___\_<|>_/___.' >' "".
   *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
   *          \  \ `_.   \_ __\ /__ _/   .-` /  /
   *      =====`-.____`.___ \_____/___.-`___.-'=====
   *                        `=---='
   *
   *
   *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *
   *            佛祖保佑     永不宕机     永无BUG
   */

#pragma once

#if !defined(_CPPUNWIND) && !defined(__EXCEPTIONS)
#  define BOOST_NO_EXCEPTIONS
#endif


#include <filesystem>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/exception/exception.hpp>
   //C++20信号量，控制总体并发应答数量
#include <semaphore>
#include "ex_semaphore.hpp"
#include <boost/locale.hpp>
#include <boost/stacktrace.hpp>
#include <ex_semaphore.hpp>
#include <Environment.h>
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace src = boost::log::sources;

using namespace std::literals::chrono_literals;
//typedef src::severity_logger_mt<severity_level> service_logger_mt;

// #define OUTPUT_LINE "-----------------------------------------------------------------------\n"
/**
 * @brief 日志级别定义
 * * TRACE: 跟踪信息
 * * DEBUG: 调试信息
 * * INFO:  信息
 * * WARN:  警告
 * * ERROR: 错误
 * * FATAL: 严重错误
 * * HEAD:  头信息
 * @note 日志级别从低到高排列
 *
 */
#if defined(_WIN_32) || defined(_WIN64)
#undef TRACE
#undef ERROR
#endif


enum severity_level
{
  TRACE = 0,
  DEBUG = 1,
  INFO = 2,
  WARN = 3,
  ERROR = 4,
  FATAL = 5,
  HEAD = 6
};

typedef src::severity_channel_logger_mt<
  severity_level,     // the type of the severity level
  std::string         // the type of the channel name
> service_logger_mt;

/**
 * @brief 全局日志器
 * @warning: 如果要完整的使用日志器，需要在main函数中调用InitBoostlog函数进行初始化
 * @warning: 当编译为静态库时，使用全局日志器时，由于共享同一内存地址，所以channel名称会与主程序相同。
 * @warning: 为避免此情况，应将需要单独记录的module或library编译为动态库
 * @note 该日志器的名称为Service_logger
 * @note 该日志器的通道名称为LOG_PROJECT_NAME
 *
*/

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(Service_logger, service_logger_mt)
{
#if defined(LOG_PROJECT_NAME)
  // Specify the channel name on construction, similarly as with the channel_logger
  return service_logger_mt(keywords::channel = LOG_PROJECT_NAME);
#else
  return service_logger_mt(keywords::channel = "Core");
#endif
}




// 自定义文件格式器
class file_formatter {
public:
  void operator()(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) const;
};

// 自定义颜色格式器
class color_formatter {
public:
  void operator()(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) const;
};



template<typename ValueType>
ValueType set_get_attrib(const char* name, ValueType value)
{

  auto attr = boost::log::attribute_cast<boost::log::attributes::mutable_constant<ValueType>>(boost::log::core::get()->get_global_attributes()[name]);
  attr.set(value);
  return attr.get();
}


std::string path_to_filename(std::string path);

#define LOG(sev) \
   BOOST_LOG_STREAM_WITH_PARAMS( \
      (Service_logger::get()), \
         (set_get_attrib("File", path_to_filename(__FILE__))) \
         (set_get_attrib("Line", (long)__LINE__)) \
         (::boost::log::keywords::severity = sev) \
   )

//这里的宏用于进行条件输出
#define LOG_EVERY_N_VARNAME_CONCAT(base, line) base##line
#define LOG_EVERY_N_VARNAME(base, line) LOG_EVERY_N_VARNAME_CONCAT(base, line)

#define LOG_EVERY_N_VARNAME_CONCAT_SAFE(base, line,pid) base##line##pid
#define LOG_EVERY_N_VARNAME_SAFE(base, line, pid) LOG_EVERY_N_VARNAME_CONCAT_SAFE(base, line, pid)

#define LOG_OCCURRENCES LOG_EVERY_N_VARNAME(occurrences_, __LINE__)
#define LOG_OCCURRENCES_MOD_N LOG_EVERY_N_VARNAME(occurrences_mod_n_, __LINE__)

#define LOG_OCCURRENCES_SAFE(pid) LOG_EVERY_N_VARNAME_SAFE(occurrences_, __LINE__,pid)
#define LOG_OCCURRENCES_MOD_N_SAFE(pid) LOG_EVERY_N_VARNAME_SAFE(occurrences_mod_n_, __LINE__,pid)

#define LOG_EVERY(sev,n)  \
   static std::atomic<int> LOG_OCCURRENCES(0), LOG_OCCURRENCES_MOD_N(0);\
  ++LOG_OCCURRENCES;                                                    \
  if (++LOG_OCCURRENCES_MOD_N > n) LOG_OCCURRENCES_MOD_N -= n;          \
  if (LOG_OCCURRENCES_MOD_N == 1)                                       \
	LOG(sev)															\

#define LOG_EVERY_SAFE(sev,n,pid)  \
   static std::atomic<int> LOG_OCCURRENCES_SAFE(pid), LOG_OCCURRENCES_MOD_N_SAFE(pid);\
  ++LOG_OCCURRENCES_SAFE;                                                    \
  if (++LOG_OCCURRENCES_MOD_N_SAFE > n) LOG_OCCURRENCES_MOD_N_SAFE -= n;          \
  if (LOG_OCCURRENCES_MOD_N_SAFE == 1)                                       \
	LOG(sev)	


//判断输出
#define LOG_IF(severity, condition) \
      if(condition)                  \
          LOG(severity)





/**
 * @brief 日志类
 * ! 该类为单例模式
 */
class Logger {

public:
  Logger() = delete;                          // 删除默认构造函数
  ~Logger() = delete;                         // 删除默认析构函数
  Logger(const Logger&) = delete;             // 删除拷贝构造函数
  Logger& operator=(const Logger&) = delete;  // 删除拷贝赋值函数
  Logger(Logger&&) = delete;                  // 删除移动构造函数
  Logger& operator=(Logger&&) = delete;       // 删除移动赋值函数

  /**
   * @brief 初始化日志
   * @note
   * @param  channel 日志通道名称
   * @return true
   * @return false
   */
  static bool InitBoostlog(const std::string& channel);


  static const std::string GetLogDirectory();
  static const std::string GetWorkingDirectory(bool bBackslash = true);
  // 自定义的文件打开处理器
  static void custom_open_handler2(sinks::text_file_backend::stream_type& file_stream);


  //static std::string path_to_filename(std::string path);

//文件后端格式器
  static boost::shared_ptr<sinks::text_file_backend> m_pFileBackend;
  //文件后端格式器-收集全部库的日志
  static boost::shared_ptr<sinks::text_file_backend> m_pFileBackendAll;

  static inline std::shared_mutex m_log_mtx;
  //信号量
  static std::shared_ptr<std::ex_counting_semaphore<>> m_pSemaphore;

  static inline std::string m_strChannelName = "Core";
};  // class Logger