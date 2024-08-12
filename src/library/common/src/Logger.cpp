/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-06 12:43:20
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-06-19 16:15:58
 * @FilePath: \Crystal.Live.2\src\library\common\src\Logger.cpp
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


#include "Environment.h"
#include "../include/Logger.hpp"
#include <string>

 // Convert file path to only the filename
std::string path_to_filename(std::string path) {
    return path.substr(path.find_last_of("/\\") + 1);
}


//文件后端格式器
boost::shared_ptr<sinks::text_file_backend> Logger::m_pFileBackend = nullptr;

//文件后端格式器-收集全部库的日志
boost::shared_ptr<sinks::text_file_backend> Logger::m_pFileBackendAll = nullptr;

std::shared_ptr<std::ex_counting_semaphore<>> Logger::m_pSemaphore = nullptr;



// 该重载函数用于将 Boost.Log 库中的日志级别枚举类型 severity_level 输出到标准输出流
std::ostream& operator<< (std::ostream& strm, severity_level level)
{
    // 定义日志级别对应的字符串数组
    static const char* strings[] =
    {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL",
        "HEAD"
    };

    // 如果枚举值在有效范围内，则输出对应的字符串，否则输出枚举的数值
    if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[static_cast<int>(level)];
    else
        strm << static_cast<int>(level);

    return strm; // 返回输出流
}

/**
    * @brief:   日志格式化器
    * @details: 日志格式化器，用于格式化日志输出，包括日期、时间、线程ID、日志级别、文件名、行号、日志消息等。
    * @param:   const boost::log::record_view& rec 日志记录视图
    * @param:   boost::log::formatting_ostream& strm 格式化输出流
    * @return:  void
 */


void file_formatter::operator()(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) const
{
    //! 很重要！使用互斥锁保护，避免多线程同时输出日志时出现混乱
    std::lock_guard<std::shared_mutex> guard(Logger::m_log_mtx);

    //行号
    auto line = logging::extract<long>("Line", rec);
    //文件名
    auto file = logging::extract<std::string>("File", rec);
    //日志级别
    auto severity = logging::extract<severity_level>("Severity", rec);
    //格式化日期
    auto time = boost::log::extract<boost::posix_time::ptime>("TimeStamp", rec);
    //日志消息
    std::string strMsg = rec[boost::log::expressions::smessage].get();
    // 提取通道名
    auto channel = boost::log::extract<std::string>("Channel", rec);


    // 如果日志消息中包含 OUTPUT_LINE 字符串，则不输出细节信息
    // 日志等级为严重错误(fatal)或者为页头页脚HEAD 时,则不输出细节信息
    bool bOutputDetail = (strMsg.find(OUTPUT_LINE, 0) == std::string::npos) && (severity != severity_level::FATAL && severity != severity_level::HEAD);

    if (bOutputDetail)
    {
        //输出日期时间 %Y-%m-%d %H:%M:%S.%f
        if (time) {
            std::stringstream time_ss;
            //std::locale会自动释放boost::posix_time::time_facet，不需要手动释放
            time_ss.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S.%f")));
            time_ss << time.get();
            strm << time_ss.str() << " | ";
        }
        else {
            strm << "[Unknown Time] ";
        }
        // 如果通道名存在，将其添加到日志消息中
        if (channel)
        {
            strm << "Channel: " << channel.get() << " | ";
        }

        // 格式化日志级别
        if (severity) {
            strm << std::left << std::setw(5) << std::setfill(' ') << severity << " | ";
        }
        else {
            strm << "[Unknown Severity] ";
        }

        // 输出线程ID,文件名和行号
        strm << "pid:" << std::dec
            << boost::log::extract<boost::log::attributes::current_thread_id::value_type>("ThreadID", rec)
            << " | " << file
            << ':' << line << " | ";

    }
    // 输出日志消息
    strm << rec[boost::log::expressions::smessage];

    return;
}


// 自定义的文件打开处理器
void Logger::custom_open_handler2(sinks::text_file_backend::stream_type& file_stream)
{
    // 在文件打开时做一些自定义的处理，比如修改文件权限、重命名文件等
    // 这里我们只是简单地输出一条日志
    file_stream << OUTPUT_LINE;
    file_stream << m_strChannelName << "----日志文件初始化\n";
    file_stream << OUTPUT_LINE;
}

// 自定义颜色格式器

void color_formatter::operator()(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) const {

    //! 很重要！使用互斥锁保护，避免多线程同时输出日志时出现混乱
    std::lock_guard<std::shared_mutex> guard(Logger::m_log_mtx);
    // 获取日志级别
    auto severity = logging::extract<severity_level>("Severity", rec);
    // auto severity = rec[boost::log::trivial::severity];

        // 设置颜色
    if (severity == severity_level::TRACE)
    {
        strm << "\033[90m"; // 灰色
    }
    if (severity == severity_level::DEBUG)
    {
        strm << "\033[38;2;152;195;121m"; // 绿色
    }
    if (severity == severity_level::INFO)
    {
        strm << "\033[38;2;97;192;250m"; // 蓝色
    }
    if (severity == severity_level::WARN)
    {
        strm << "\033[38;2;229;192;123m"; // 橙色
    }
    if (severity == severity_level::ERROR)
    {
        strm << "\033[38;2;224;108;117m"; // 红色
    }
    if (severity == severity_level::FATAL)
    {
        strm << "\033[38;2;214;108;239m"; // 紫色
    }
    if (severity == severity_level::HEAD)
    {
        strm << "\033[38;2;214;108;239m"; // 紫色
    }

    // 格式化日期
    auto time = boost::log::extract<boost::posix_time::ptime>("TimeStamp", rec);

    // 提取通道名
    auto channel = boost::log::extract<std::string>("Channel", rec);


    std::string strMsg = rec[boost::log::expressions::smessage].get();
    // 如果日志消息中包含 OUTPUT_LINE 字符串，则不输出细节信息
            // 日志等级为严重错误(fatal)时,则不输出细节信息
    bool bOutputDetail = (strMsg.find(OUTPUT_LINE, 0) == std::string::npos)
        && (severity != severity_level::FATAL && severity != severity_level::HEAD);




    if (bOutputDetail)
    {

        //strm << std::left << std::setw(7) << std::setfill(' ') << severity << " | ";
        if (time) {
            std::stringstream time_ss;
            //std::locale会自动释放boost::posix_time::time_facet，不需要手动释放
            time_ss.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S.%f")));
            time_ss << time.get();
            strm << time_ss.str() << " | ";
        }
        else {
            strm << "[Unknown Time] ";
        }

        // 如果通道名存在，将其添加到日志消息中
        if (channel)
        {
            strm << "Channel: " << channel.get() << " | ";
        }

        // 格式化日志级别
        if (severity) {
            strm << std::left << std::setw(5) << std::setfill(' ') << severity << " | ";
        }
        else {
            strm << "[Unknown Severity] ";
        }

        // strm << "pid:" << std::dec
        //     << boost::log::extract<attrs::current_thread_id::value_type>("ThreadID", rec)
        //     << " | ";
    }
    strm << rec[boost::log::expressions::smessage];

    // 重置颜色
    strm << "\033[0m";
}


bool Logger::InitBoostlog(const std::string& channel)
{
    m_strChannelName = channel;
    // 添加全局属性"File"和"Line"，这些属性可以在日志消息中使用
    logging::core::get()->add_global_attribute("File", attrs::mutable_constant<std::string>(""));
    logging::core::get()->add_global_attribute("Line", attrs::mutable_constant<long>(0));

    // 创建一个日志文件名，包含了传入的频道名称和时间戳
    std::stringstream strLogFileName, strLogFileNameAll;
    strLogFileName << "Live." << channel << "_%Y%m%d_%3N.log";
    strLogFileNameAll << "Live.Timeline" << "_%Y%m%d_%3N.log";
    // 日志文件将被保存在由GetLogDirectory()函数返回的目录中
    std::string strLogPath = GetLogDirectory() + strLogFileName.str();
    std::string strLogPathAll = GetLogDirectory() + strLogFileNameAll.str();
    // 创建一个文件后端，这个后端负责将日志消息写入文件
    m_pFileBackend = boost::make_shared<sinks::text_file_backend>(
        keywords::file_name = strLogPath,
        keywords::target_file_name = strLogPath,
        keywords::auto_flush = true,
        keywords::rotation_size = 10 * 1024 * 1024, // 文件大小限制
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), // 基于时间的文件旋转
        keywords::format = file_formatter(), // 文件格式
        keywords::open_mode = std::ios_base::app // 使用追加模式打开文件
    );

    // 创建一个文件后端，这个后端负责将日志消息写入文件
    m_pFileBackendAll = boost::make_shared<sinks::text_file_backend>(
        keywords::file_name = strLogPathAll,
        keywords::target_file_name = strLogPathAll,
        keywords::auto_flush = true,
        keywords::rotation_size = 10 * 1024 * 1024, // 文件大小限制
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), // 基于时间的文件旋转
        keywords::format = file_formatter(), // 文件格式
        keywords::open_mode = std::ios_base::app // 使用追加模式打开文件
    );

    // 创建一个文件前端，这个前端负责接收日志消息并将其传递给后端
    typedef sinks::asynchronous_sink<sinks::text_file_backend> file_sink;
    boost::shared_ptr<file_sink> file_logger(new file_sink(m_pFileBackend));

    typedef sinks::asynchronous_sink<sinks::text_file_backend> file_sink_all;
    boost::shared_ptr<file_sink> file_logger_all(new file_sink(m_pFileBackendAll));


    // 设置一个文件打开处理器，这个处理器在文件被打开时会被调用
    m_pFileBackend->set_open_handler(&custom_open_handler2);

    // 设置一个文件打开处理器，这个处理器在文件被打开时会被调用
    m_pFileBackendAll->set_open_handler(&custom_open_handler2);


    // 设置文件前端的格式器
    file_logger->set_formatter(file_formatter());

    // 设置文件前端的格式器
    file_logger_all->set_formatter(file_formatter());


    // 将文件前端的区域设置为"zh_CN.UTF-8"，这意味着日志消息将使用这个区域的规则进行格式化
    std::locale loc = boost::locale::generator()("zh_CN.UTF-8");
    file_logger->imbue(loc);
    file_logger_all->imbue(loc);
    // 将文件前端添加到日志核心，这意味着日志核心将开始接收并处理通过这个前端发送的日志消息
    logging::core::get()->add_sink(file_logger);
    logging::core::get()->add_sink(file_logger_all);


    // 创建一个控制台前端
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > console_sink;
    boost::shared_ptr< console_sink > console = boost::make_shared< console_sink >();

    // 将控制台前端的输出流设置为std::clog
    console->locked_backend()->add_stream(
        boost::shared_ptr< std::ostream >(&std::clog, boost::null_deleter()));

    // 设置控制台前端的格式器
    console->set_formatter(color_formatter());

    console->imbue(loc);
    // 将控制台前端添加到日志核心，这意味着日志消息也将被发送到控制台
    logging::core::get()->add_sink(console);

    // 添加一些常见的日志属性，这些属性可以在日志消息中使用
    logging::add_common_attributes();

    // 返回true，表示初始化成功
    return true;
}






const std::string Logger::GetLogDirectory()
{
    std::string strLogDir = GetWorkingDirectory(true);
    strLogDir += "log/";
    return strLogDir;
}

const std::string Logger::GetWorkingDirectory(bool bEndSlash)
{
    boost::filesystem::path exePath = boost::filesystem::initial_path();
    std::string strPath = exePath.string();
    if (bEndSlash)
    {
        if (strPath.back() != '/' || strPath.back() != '\\')
        {
            strPath += "/";
            return strPath;
        }

    }
    return strPath;
}
