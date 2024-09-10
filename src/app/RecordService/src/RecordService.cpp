

/* ————————————————————————————————————————————————————————————————————————————————————————
 * @                       _oo0oo_
 * @                      o8888888o
 * @                      88" . "88
 * @                      (| -_- |)
 * @                      0\  =  /0
 * @                    ___/`---'\___
 * @                  .' \\|     |// '.
 * @                 / \\|||  :  |||// \
 * @                / _||||| -:- |||||- \
 * @               |   | \\\  - /// |   |
 * @               | \_|  ''\---/''  |_/ |
 * @               \  .-\__  '-'  ___/-. /
 * @             ___'. .'  /--.--\  `. .'___
 * @          ."" '<  `.___\_<|>_/___.' >' "".
 * @         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 * @         \  \ `_.   \_ __\ /__ _/   .-` /  /
 * @     =====`-.____`.___ \_____/___.-`___.-'=====
 * @                       `=---='
 * @
 * @
 * @     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * @
 * @           佛祖保佑     永不宕机     永无BUG
 ———————————————————————————————————————————————————————————————————————————————————————— */


#if defined(_WIN32) || defined(_WIN64)
#include <Winsock2.h>
#include <Iphlpapi.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#endif


#include <iostream>
 // #include "../common/Environment.h"
// #include "config.h" 
#include <iostream>
// #include <Codec.h>

#include <openssl/bio.h>
#include <memory>
#include <vector>
// #define PROJECT_NAME "AppLaunch"
#include <Common.h>
#include <cstdlib>       // std::abort
#include <exception>     // std::set_terminate
#include <iostream>      // std::cerr
// #include <NetCapture.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <thread>
#include <chrono>
#include <boost/locale.hpp>
// #include <Config.h>
#include <CRecordService.h>
#include <boost/algorithm/string.hpp>
#include "../res/version.h"
#include <RecordEngine.h>
#include <qDebug>
#include <qtextcodec>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif

//使用chrono字面量
using namespace std::literals::chrono_literals;
// #include <srtp2/srtp.h>
void my_terminate_handler() {
    try {
        LOG(FATAL) << boost::stacktrace::stacktrace();
        RecordEngine::StopRecordService();
        exit(0);
    }
    catch (...) {

    }
}

void handler(int sig)
{
    try {
        std::cout << boost::stacktrace::stacktrace();
        LOG(FATAL) << boost::stacktrace::stacktrace();
        RecordEngine::StopRecordService();
        exit(sig);
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


    std::cout << "捕获到退出信号 | Signal : " << signum << std::endl;
    if (signum == SIGINT)   //退出中断信号
    {
        // 设置退出标志
        RecordEngine::StopRecordService();
        exit(signum);
    }

    //std::abort();
}
namespace RecordService
{
    std::string GetVersion()
    {
        return g_Version;
    }

}

int main(int argc, char* argv[]) {
    using namespace boost::locale;
    std::set_terminate(&my_terminate_handler);
    signal(SIGSEGV, handler);   // 设置信号处理函数
    // 设置SIGINT（Ctrl+C）的信号处理函数
    signal(SIGINT, signalHandler);
    LogInfo logInfo;
    logInfo.Channel = "RecordService";
    logInfo.Version = RecordService::GetVersion();;
    logInfo.Location = Common::Utility::GetAppPath(logInfo.Channel);
    Common::SetEnv();
    Common::InitLog(logInfo);
    auto logger = Service_logger::get();
    std::cout << "logger address:" << &logger << std::endl;
    // 设置QT翻译器
    QGuiApplication app(argc, argv);
    QTranslator translator;

    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale : uiLanguages) {
        LOG(INFO) << QLocale(locale).name().toStdString();
        const QString baseName = "RecordService_" + QLocale(locale).name();
        if (translator.load("./i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    LOG(INFO) << QObject::tr("Start RecordService").toStdString();
    LOG(INFO) << QObject::tr("Open").toStdString();


    RecordEngine::Init();


    //创建服务对象
    CRecordService service("CrystalLive.RecordService");
    //带有参数的调用
    if (argc > 1)
    {
        std::string strCmd1 = argv[1];
        strCmd1 = boost::algorithm::to_lower_copy(strCmd1);
        if (strCmd1 == "-install") {//安装服务
            LOG(WARN) << "录音服务 正在安装 (install)";
            if (!ServiceInstaller::Install(service)) {
                LOG(ERROR) << "录音服务 安装失败(install)";
                return -1;
            }
            LOG(WARN) << "录音服务 安装成功 (install)";
            return 0;
        }

        if (strCmd1 == "-uninstall") {//卸载服务
            LOG(WARN) << "录音服务 正在卸载 (uninstall)";
            if (!ServiceInstaller::Uninstall(service)) {
                LOG(ERROR) << "录音服务 卸载失败(uninstall)";
                return -1;
            }
            LOG(WARN) << "录音服务 卸载成功(uninstall)";
            return 0;
        }

        if (strCmd1 == "-console") {//以控制台的方式运行
            LOG(WARN) << "录音服务 正在以控制台方式运行 (console)";
            service.Run(_T("console"));
            LOG(WARN) << "录音服务 控制台方式运行结束 (console)";
            return 0;
        }

    }
    else
    {//以服务的方式运行
        LOG(WARN) << "录音服务 正在以Windows服务方式运行 (WindowsService)";
        service.Run();
        LOG(WARN) << "录音服务 Windows服务运行停止 (WindowsService)";
    }

    return EXIT_SUCCESS;

}