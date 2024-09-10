/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-22 16:02:15
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-28 09:36:11
 * @FilePath: \Crystal.Live.2\src\app\RecordService\src\CRecordService.cpp
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
 ————————————————————————————————————————————————————————————————————————————————————————*/
#include "CRecordService.h"
#include <QObject.h>
#include <RecordEngine.h>
bool CRecordService::Run(LPCTSTR param)
{
    if (_tcscmp(param, _T("console")) == 0)
    {

        //Todo：控制台运行处理 调用OnStart和OnStop

        TCHAR cinCmd[128];


        while (1)//控制台运行主线程不能退出 
        {
            LOG(HEAD) << "请输入命令,按 ? 可以查看支持的命令:";
            //if (strCmd1 == _T("-stop")) {//以控制台的方式运行
            //	LOG(WARN) << "信令服务 正在t (console)";
            //	service.Run(_T("console"));
            //	LOG(WARN) << "信令服务 控制台方式运行结束 (console)";
            //	return 0;
            //}
            _tscanf_s(_T("%s"), cinCmd, 128);
            if (_tcsncmp(cinCmd, _T("?"), 1) == 0)
            {
                LOG(HEAD) << "========================================";
                LOG(HEAD) << std::left << std::setw(10) << std::setfill(' ') << "\"-?\"" << std::setw(10) << std::setfill(' ') << " " << "-查看支持的命令";
                LOG(HEAD) << std::left << std::setw(10) << std::setfill(' ') << "\"-start\"" << std::setw(10) << std::setfill(' ') << " " << "-运行信令服务";
                LOG(HEAD) << std::left << std::setw(10) << std::setfill(' ') << "\"-stop\"" << std::setw(10) << std::setfill(' ') << " " << "-停止信令服务";
                LOG(HEAD) << std::left << std::setw(10) << std::setfill(' ') << "\"-TestCTI\"" << std::setw(10) << std::setfill(' ') << " " << "-测试CTI请求";
                LOG(HEAD) << std::left << std::setw(10) << std::setfill(' ') << "\"-exit\"" << std::setw(10) << std::setfill(' ') << " " << "-退出";
                LOG(HEAD) << "========================================";
            }
            else if (_tcsncmp(cinCmd, _T("start"), 5) == 0)
            {
                if (!m_bServiceStart)
                {
                    LOG(HEAD) << "========================================";
                    LOG(HEAD) << "		启动服务";
                    LOG(HEAD) << "========================================";

                    OnStart(0, NULL);

                }

            }
            else if (_tcsncmp(cinCmd, _T("TestCTI"), 5) == 0)
            {

                LOG(HEAD) << "========================================";
                LOG(HEAD) << "		测试CTI通讯接口";
                LOG(HEAD) << "========================================";

                OnTestCTI(0, NULL);

            }
            else if (_tcsncmp(cinCmd, _T("stop"), 4) == 0)
            {

                LOG(HEAD) << "========================================";
                LOG(HEAD) << "		停止服务";
                LOG(HEAD) << "========================================";
                if (m_bServiceStart)
                {
                    OnStop();
                }

            }
            else if (_tcsncmp(cinCmd, _T("exit"), 4) == 0)
            {

                LOG(HEAD) << "========================================";
                LOG(HEAD) << "			退出服务";
                LOG(HEAD) << "========================================";
                //exit(0);
                break;
            }
            else
            {
                LOG(HEAD) << "========================================";
                LOG(HEAD) << "		无效的命令";
                LOG(HEAD) << "========================================";
            }
        }

        if (m_bServiceStart)
            OnStop();
    }
    return ServiceBase::Run();//服务的方式运行

}

void CRecordService::OnStop()
{
    LOG(INFO) << "CRecordService::OnStop";
    RecordEngine::StopRecordService();
}

void CRecordService::OnStart(DWORD argc, char* argv[])
{
    LOG(INFO) << "CRecordService::OnStart";
    RecordEngine::StartRecordService();

}

void CRecordService::OnTestCTI(DWORD argc, char* argv[]) const
{
}

void CRecordService::OnInstall()
{

}

void CRecordService::OnUninstall()
{
}


