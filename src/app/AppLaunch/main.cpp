/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-24 23:46:50
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-26 16:27:49
 * @FilePath: \Crystal.Live.2\src\AppLaunch\main.cpp
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


#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include <iostream>
  // #include "../common/Environment.h"
#include "config.h" 
#include <iostream>
#include <Codec.h>

#include <openssl/bio.h>
#include <memory>
#include <vector>
// #define PROJECT_NAME "AppLaunch"
#include <Common.h>
#include <NetCapture.h>
#include <cstdlib>       // std::abort
#include <exception>     // std::set_terminate
#include <iostream>      // std::cerr
#include <pcap.h>
#include <NetCapture.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <thread>
#include <chrono>
#include <boost/locale.hpp>
#include <Config.h>


//使用chrono字面量
using namespace  std::literals::chrono_literals;
// #include <srtp2/srtp.h>
void my_terminate_handler() {
  try {
    LOG(FATAL) << boost::stacktrace::stacktrace();
  }
  catch (...) {

  }
  std::abort();
}

void handler(int sig)
{
  try {
    LOG(FATAL) << boost::stacktrace::stacktrace();
  }
  catch (...) {

  }
}
void PrintEvery(int nEvery, int nCount) {
  for (size_t i = 0; i < nCount; i++)
  {
    LOG_EVERY(INFO, 100) << "Test Every : " << std::this_thread::get_id() << " | pos :" << i;
    std::this_thread::sleep_for(1ms);
  }

}
// 信号处理函数
void signalHandler(int signum) {
  std::cout << "捕获到信号 " << signum << std::endl;
  // 设置退出标志
  NetCapture::StopCapture();
  exit(signum);
  //std::abort();
}

int main(int argc, char* argv[]) {
  using namespace boost::locale;
  std::set_terminate(&my_terminate_handler);
  signal(SIGSEGV, handler);   // 设置信号处理函数
  // 设置SIGINT（Ctrl+C）的信号处理函数
  signal(SIGINT, signalHandler);

  Common::SetEnv();
  Common::InitLog("AppLaunch");
  QGuiApplication app(argc, argv);
  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString& locale : uiLanguages) {
    const QString baseName = "AppLaunch_" + QLocale(locale).name();
    if (translator.load("./i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }
  Config::GetVersion();
  Config::init();
  NetCapture::Init();
  // std::vector<std::shared_ptr<NetworkAdapterInfo>> vtDevices;
  // size_t nDevCount = NetCapture::GetDevicesInfo(vtDevices);
  // for (size_t i = 0; i < vtDevices.size(); i++)
  // {
  //   LOG(INFO) << vtDevices[i]->ID
  //     << " | " << vtDevices[i]->Name
  //     << " | " << vtDevices[i]->IpAddress
  //     << " | " << vtDevices[i]->BroadAddress
  //     << " | " << vtDevices[i]->NetmaskAddress
  //     << " | " << vtDevices[i]->MACAddress;
  //   /* code */
  // }



  //init_logging();
  boost::locale::generator gen;

  // 指定多语言文件的根目录
  gen.add_messages_path(R"(.\i18n)");

  // 对应语言文件名，不需要写扩展名。等同于 foo.mo
  gen.add_messages_domain("foo");
  // generator 会去对应的文件夹下找语言文件。这里会尝试从 zh 和 zh_CN 文件夹下寻找文件
  std::locale loc = gen.generate("zh_CN.utf-8");

  // 设置全局locale
  std::locale::global(loc);

  NetCapture::StartCapture();

  LOG(INFO) << QObject::tr("Start Capture").toStdString();




  // std::jthread t1(PrintEvery, 10, 10000);
  // std::jthread t2(PrintEvery, 20, 10000);


  // for (int i = 0; i < 10000; i++) {
  //   LOG_EVERY(INFO, 100) << "Test Every" << i;
  //   LOG_EVERY(INFO, 105) << "Test Every2" << i;
  //   LOG(INFO) << i;
  // }
  LOG(WARN) << "Hello, world!";

  LOG(ERROR) << "Hello, world!";
  LOG(FATAL) << "Hello, world!";
  LOG(INFO) << OUTPUT_LINE;
  LOG(INFO) << QObject::tr("Hello, world!").toStdString();
  LOG(INFO) << OUTPUT_LINE;
  std::string strCodecVer = Codec::GetVersion();
  std::string strNetCaptureVer = NetCapture::GetVersion();
  // std::this_thread::sleep_for(5s);
  // NetCapture::StopCapture();
  /*BOOST_LOG_SEV(lg, info) << "This is an informational message.";
  BOOST_LOG_SEV(lg, error) << "This is an error message.";*/
  // std::cout << "Number of logical cores: " << NUMBER_OF_LOGICAL_CORES
  //   << std::endl;
  // std::cout << "Number of physical cores: " << NUMBER_OF_PHYSICAL_CORES
  //   << std::endl;
  // std::cout << "Total virtual memory in megabytes: " << TOTAL_VIRTUAL_MEMORY
  //   << std::endl;
  // std::cout << "Available virtual memory in megabytes: "
  //   << AVAILABLE_VIRTUAL_MEMORY << std::endl;
  // std::cout << "Total physical memory in megabytes: " << TOTAL_PHYSICAL_MEMORY
  //   << std::endl;
  // std::cout << "Available physical memory in megabytes: "
  //   << AVAILABLE_PHYSICAL_MEMORY << std::endl;
  // std::cout << "Processor is 64Bit: " << IS_64BIT << std::endl;
  // std::cout << "Processor has floating point unit: " << HAS_FPU << std::endl;
  // std::cout << "Processor supports MMX instructions: " << HAS_MMX << std::endl;
  // std::cout << "Processor supports Ext. MMX instructions: " << HAS_MMX_PLUS
  //   << std::endl;
  // std::cout << "Processor supports SSE instructions: " << HAS_SSE << std::endl;
  // std::cout << "Processor supports SSE2 instructions: " << HAS_SSE2
  //   << std::endl;
  // std::cout << "Processor supports SSE FP instructions: " << HAS_SSE_FP
  //   << std::endl;
  // std::cout << "Processor supports SSE MMX instructions: " << HAS_SSE_MMX
  //   << std::endl;
  // std::cout << "Processor supports 3DNow instructions: " << HAS_AMD_3DNOW
  //   << std::endl;
  // std::cout << "Processor supports 3DNow+ instructions: " << HAS_AMD_3DNOW_PLUS
  //   << std::endl;
  // std::cout << "IA64 processor emulating x86 : " << HAS_IA64 << std::endl;
  // std::cout << "OS name: " << OS_NAME << std::endl;
  // std::cout << "OS sub-type: " << OS_RELEASE << std::endl;
  // std::cout << "OS build ID: " << OS_VERSION << std::endl;
  // std::cout << "OS platform: " << OS_PLATFORM << std::endl;

  //std::cout << "Audio version: " << Audio::GetVersion() << std::endl;
  std::cout << "Codec version: " << Codec::GetVersion() << std::endl;

  // file_backend->set_file_collector(sinks::file::make_collector(
  //   keywords::target = "log/20240601",               // 每天创建一个新的目录来存储日志文件
  //   keywords::max_size = 16 * 1024 * 1024,          // 单个日志文件的最大大小
  //   keywords::min_free_space = 100 * 1024 * 1024,   // 磁盘最小剩余空间
  //   keywords::max_files = 512                      // 存储的日志文件数量上限
  // ));

  // file_backend->scan_for_files(sinks::file::scan_matching);
  //
  while (true)
  {

    std::this_thread::sleep_for(60s);
  }

  return EXIT_SUCCESS;

}