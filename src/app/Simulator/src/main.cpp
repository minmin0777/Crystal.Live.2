/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-16 11:34:52
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-17 15:42:42
 * @FilePath: \Crystal.Live.2\src\Simulator\src\main.cpp
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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <rabbitmq-c/amqp.h>
#include "MyCppClass.h"
#include <QQmlContext>
#include <QProcess>
#include <csignal> // 包含信号处理的头文件
#include <boost/stacktrace.hpp>
#include <boost/locale.hpp>
#include <Common.h>
#include <QDebug>

 // int main(int argc, char** argv) {
 //     if (argc < 2) {
 //         printf("Usage: %s port\n", argv[0]);
 //         return -10;
 //     }
 //     int port = atoi(argv[1]);

 // 	// 创建事件循环
 //     hloop_t* loop = hloop_new(0);
 //     // 创建UDP服务
 //     hio_t* io = hloop_create_udp_server(loop, "0.0.0.0", port);
 //     if (io == NULL) {
 //         return -20;
 //     }
 //     // 设置read回调
 //     hio_setcb_read(io, on_recvfrom);
 //     // 开始读
 //     hio_read(io);
 //     // 运行事件循环
 //     hloop_run(loop);
 //     // 释放事件循环
 //     hloop_free(&loop);
 //     return 0;
 // }


void signalHandler(int signal) {


  qDebug() << "Error: signal" << signal;
  // 假设你已经在某处获取了一个stacktrace实例
  boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace();

  // 将stacktrace转换为std::string
  std::string stacktraceStr = boost::stacktrace::to_string(st);

  // 将std::string转换为QString
  QString qStacktraceStr = QString::fromStdString(stacktraceStr);

  // 
  qDebug() << "Stack trace:" << qStacktraceStr;

  exit(1);
}



int main(int argc, char* argv[]) {
  // 禁用libhv库的日志
  hlog_disable();
  using namespace boost::locale;
  Common::SetEnv();
  LogInfo logInfo;
  logInfo.Channel = "Simulator";
  logInfo.Version = GetVersion();;
  logInfo.Location = Common::Utility::GetAppPath(logInfo.Channel);
  Common::InitLog(logInfo);

  // 注册信号处理函数
  std::signal(SIGSEGV, signalHandler);
  std::signal(SIGABRT, signalHandler);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString& locale : uiLanguages) {
    const QString baseName = "Simulator_" + QLocale(locale).name();
    if (translator.load("./i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }

  QQmlApplicationEngine engine;

  MyCppClass myObject(nullptr);
  qmlRegisterType<MyCppClass>("com.mycompany.MyCppClass", 1, 0, "MyCppClass");
  //engine.rootContext()->setContextProperty("myCppObject", &myObject);
  const QUrl url(QStringLiteral("qrc:/App.qml"));
  QObject::connect(
    &engine, &QQmlApplicationEngine::objectCreated, &app,
    [url](QObject* obj, const QUrl& objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
  engine.load(url);


  const int exec = QGuiApplication::exec();
  if (exec == 931) {
    QProcess::startDetached(qApp->applicationFilePath(), qApp->arguments());
  }
  return exec;
}
