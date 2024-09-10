/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-27 14:44:17
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-27 15:44:32
 * @FilePath: \Crystal.Live.2\src\app\RecordService\platform\windows\service_base.h
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
#pragma once
#ifndef SERVICE_BASE_H_
#define SERVICE_BASE_H_

#include <windows.h>
#include <winsvc.h>
#include <atlstr.h>

#include "copy_disabler.h"
#include <string>

 // Base Service class used to create windows services.
class ServiceBase
{
public:
	//DISABLE_COPY_MOVE(ServiceBase);

	virtual ~ServiceBase() {}

	// Called by windows when starting the service.
	bool Run()
	{
		return RunInternal(this);
	}

	const std::string& GetName() const { return m_name; }
	const std::string& GetDisplayName() const { return m_displayName; }
	const DWORD GetStartType() const { return m_dwStartType; }
	const DWORD GetErrorControlType() const { return m_dwErrorCtrlType; }
	const std::string& GetDependencies() const { return m_depends; }

	// Account info service runs under.
	const std::string& GetAccount() const { return m_account; }
	const std::string& GetPassword() const { return m_password; }
protected:
	ServiceBase(const std::string& name,
		const std::string& displayName,
		DWORD dwStartType,
		DWORD dwErrCtrlType = SERVICE_ERROR_NORMAL,
		DWORD dwAcceptedCmds = SERVICE_ACCEPT_STOP,
		const std::string& depends = (""),
		const std::string& account = (""),
		const std::string& password = (""));

	void SetStatus(DWORD dwState, DWORD dwErrCode = NO_ERROR, DWORD dwWait = 0);

	// TODO(Olster): Move out of class/make static.
	// Writes |msg| to Windows event log.
	void WriteToEventLog(const std::string& msg, WORD type = EVENTLOG_INFORMATION_TYPE);

public: //event
	virtual void OnStart(DWORD argc, char* argv[]) = 0;
	virtual void OnStop() {}
	virtual void OnPause() {}
	virtual void OnContinue() {}
	virtual void OnShutdown() {}

	virtual void OnSessionChange(DWORD /*evtType*/,
		WTSSESSION_NOTIFICATION* /*notification*/) {}
	virtual void OnInstall() = 0;
	virtual void OnUninstall() = 0;
private:
	// Registers handle and starts the service.
	static void WINAPI SvcMain(DWORD argc, char* argv[]);

	// Called whenever service control manager updates service status.
	static DWORD WINAPI ServiceCtrlHandler(DWORD ctrlCode, DWORD evtType,
		void* evtData, void* context);

	static bool RunInternal(ServiceBase* svc);

	void Start(DWORD argc, char* argv[]);
	void Stop();
	void Pause();
	void Continue();
	void Shutdown();

	std::string m_name;
	std::string m_displayName;
	DWORD m_dwStartType;
	DWORD m_dwErrorCtrlType;
	std::string m_depends;
	std::string m_account;
	std::string m_password;

	// Info about service dependencies and user account.
	bool m_hasDepends;/* = false*/;
	bool m_hasAcc;/* = false*/;
	bool m_hasPass;/* = false*/;

	SERVICE_STATUS m_svcStatus;
	SERVICE_STATUS_HANDLE m_svcStatusHandle;

	static ServiceBase* m_service;
	DISABLE_COPY_AND_MOVE(ServiceBase)
};

#endif // SERVICE_BASE_H_