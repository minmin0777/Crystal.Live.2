
#include "service_base.h"
#include <cassert>

ServiceBase* ServiceBase::m_service = nullptr;

ServiceBase::ServiceBase(const std::string& name,
	const std::string& displayName,
	DWORD dwStartType,
	DWORD dwErrCtrlType,
	DWORD dwAcceptedCmds,
	const std::string& depends,
	const std::string& account,
	const std::string& password)
	: m_name(name),
	m_displayName(displayName),
	m_dwStartType(dwStartType),
	m_dwErrorCtrlType(dwErrCtrlType),
	m_depends(depends),
	m_account(account),
	m_password(password),
	m_svcStatusHandle(nullptr)
{

	m_hasDepends = !m_depends.empty();
	m_hasAcc = !m_account.empty();
	m_hasPass = !m_password.empty();
	m_svcStatus.dwControlsAccepted = dwAcceptedCmds;
	m_svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_svcStatus.dwCurrentState = SERVICE_START_PENDING;
	m_svcStatus.dwWin32ExitCode = NO_ERROR;
	m_svcStatus.dwServiceSpecificExitCode = 0;
	m_svcStatus.dwCheckPoint = 0;
	m_svcStatus.dwWaitHint = 0;
}

void ServiceBase::SetStatus(DWORD dwState, DWORD dwErrCode, DWORD dwWait)
{
	m_svcStatus.dwCurrentState = dwState;
	m_svcStatus.dwWin32ExitCode = dwErrCode;
	m_svcStatus.dwWaitHint = dwWait;
	::SetServiceStatus(m_svcStatusHandle, &m_svcStatus);
}

void ServiceBase::WriteToEventLog(const std::string& msg, WORD type)
{
	HANDLE hSource = RegisterEventSourceA(nullptr, m_name.c_str());
	if (hSource)
	{
		const char* msgData[2] = { m_name.c_str(), msg.c_str() };
		ReportEventA(hSource, type, 0, 0, nullptr, 2, 0, msgData, nullptr);
		DeregisterEventSource(hSource);
	}
}

// static
void WINAPI ServiceBase::SvcMain(DWORD argc, char* argv[])
{
	assert(m_service);
	m_service->m_svcStatusHandle = ::RegisterServiceCtrlHandlerExA(m_service->GetName().c_str(), ServiceCtrlHandler, NULL);
	if (!m_service->m_svcStatusHandle)
	{
		m_service->WriteToEventLog(("Can't set service control handler"), EVENTLOG_ERROR_TYPE);
		return;
	}

	m_service->Start(argc, argv);
}

// static
DWORD WINAPI ServiceBase::ServiceCtrlHandler(DWORD ctrlCode, DWORD evtType, void* evtData, void* /*context*/)
{
	switch (ctrlCode)
	{
	case SERVICE_CONTROL_STOP:
		m_service->Stop();
		break;

	case SERVICE_CONTROL_PAUSE:
		m_service->Pause();
		break;

	case SERVICE_CONTROL_CONTINUE:
		m_service->Continue();
		break;

	case SERVICE_CONTROL_SHUTDOWN:
		m_service->Shutdown();
		break;

	case SERVICE_CONTROL_SESSIONCHANGE:
		m_service->OnSessionChange(evtType, reinterpret_cast<WTSSESSION_NOTIFICATION*>(evtData));
		break;

	default:
		break;
	}

	return 0;
}

bool ServiceBase::RunInternal(ServiceBase* svc)
{
	m_service = svc;
	LPSTR svcName = (char*)m_service->GetName().c_str();

	SERVICE_TABLE_ENTRYA tableEntry[] =
	{
		{svcName, (LPSERVICE_MAIN_FUNCTIONA)SvcMain},
		{nullptr, nullptr}
	};

	return ::StartServiceCtrlDispatcherA(tableEntry) == TRUE;
}

void ServiceBase::Start(DWORD argc, char* argv[])
{
	SetStatus(SERVICE_START_PENDING);
	OnStart(argc, argv);
	SetStatus(SERVICE_RUNNING);
}

void ServiceBase::Stop()
{
	SetStatus(SERVICE_STOP_PENDING);
	OnStop();
	SetStatus(SERVICE_STOPPED);
}

void ServiceBase::Pause()
{
	SetStatus(SERVICE_PAUSE_PENDING);
	OnPause();
	SetStatus(SERVICE_PAUSED);
}

void ServiceBase::Continue()
{
	SetStatus(SERVICE_CONTINUE_PENDING);
	OnContinue();
	SetStatus(SERVICE_RUNNING);
}

void ServiceBase::Shutdown()
{
	OnShutdown();
	SetStatus(SERVICE_STOPPED);
}