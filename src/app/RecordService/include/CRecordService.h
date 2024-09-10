/* ————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-22 16:00:28
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-23 15:13:27
 * @FilePath: \Crystal.Live.2\src\app\RecordService\include\CRecordService.h
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  ClassName         : Description                                           |
 * @#|----------------------------------------------------------------------------|
 * @#|  Change History :                                                          |
 * @#|  <Date>     | <Version> | <Author>       | <Description>                   |
 * @#|----------------------------------------------------------------------------|
 * @#|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
 * @#|----------------------------------------------------------------------------|
 * @#|                                                                            |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ———————————————————————————————————————————————————————————————————————————————————————— */

#pragma once
#include <atomic>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include "../platform/windows/service_base.h"
#include "../platform/windows/service_installer.h"
#else
#include "../platform/linux/service_base.h"
#endif
#include <Common.h>
 //此类继承基础服务类
class CRecordService :public ServiceBase
{
public:

    CRecordService(const std::string& name)
        :ServiceBase(name,
            name,
            SERVICE_AUTO_START,
            SERVICE_ERROR_NORMAL,
            SERVICE_ACCEPT_STOP
        ) {}

private:


    //测试CTI接口


public:

    //如果是以服务的方式运行就到此函数里面直接调用ServiceBase的Run函数。此函数调用完以后，服务会调用OnStart函数。
    //如果是控制台的方式运行就到此函数里面写主要的逻辑代码。此函数一般也会到内部调用OnStart和OnStop函数。此函数为主线程，所以不能退出。
    bool Run(LPCTSTR param = _T(""));

    //服务关闭时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示关闭中）
    void OnStop();
    //服务状态机
    std::atomic<bool> m_bServiceStart = false;

    void OnStart(DWORD argc, char* argv[]);

    void OnTestCTI(DWORD argc, char* argv[]) const;
    void OnInstall() override;
    void OnUninstall() override;
};
