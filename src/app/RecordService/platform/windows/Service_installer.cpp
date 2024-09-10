#include "service_installer.h"
#include <strsafe.h>
#include <tchar.h>

namespace
{
	class ServiceHandle
	{
	public:
		ServiceHandle(SC_HANDLE handle) : m_handle(handle) {}

		~ServiceHandle()
		{
			if (m_handle)
			{
				::CloseServiceHandle(m_handle);
			}
		}

		operator SC_HANDLE()
		{
			return m_handle;
		}

	private:
		SC_HANDLE m_handle;
	};
}

VOID SvcInstall()
{
	SC_HANDLE schSCManager = nullptr;
	SC_HANDLE schService = nullptr;
	TCHAR szUnquotedPath[MAX_PATH];

	if (!GetModuleFileName(NULL, szUnquotedPath, MAX_PATH))
	{
		printf("Cannot install service (%d)\n", GetLastError());
		return;
	}

	// In case the path contains a space, it must be quoted so that
	// it is correctly interpreted. For example,
	// "d:\my share\myservice.exe" should be specified as
	// ""d:\my share\myservice.exe"".
	TCHAR szPath[MAX_PATH];
	StringCbPrintf(szPath, MAX_PATH, TEXT("\"%s\""), szUnquotedPath);

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return;
	}

	// Create the service
	schService = CreateService(
		schSCManager,              // SCM database 
		SVCNAME,                   // name of service 
		SVCNAME,                   // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_AUTO_START,      // start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		szPath,                    // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,						// no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 

	if (schService == NULL)
	{
		printf("CreateService failed (%d)\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return;
	}
	else
	{
		SERVICE_DESCRIPTIONA sd;
		sd.lpDescription = const_cast<LPSTR>("This service is used to synchronize recording files and records between CQ recording servers.");
		//strDescription.ReleaseBuffer();
		if (!ChangeServiceConfig2A(
			schService,                 // handle to service
			SERVICE_CONFIG_DESCRIPTION, // change: description
			&sd))                      // new description
		{
			printf("ChangeServiceConfig2 failed\n");
		}
		else
		{
			printf("Service installed successfully\n");

			// Æô¶¯·þÎñ
			BOOL bSucc = StartService(schService, NULL, NULL);
			printf("StartService: %u\n", bSucc);

		}
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
}

//static
bool ServiceInstaller::Install(const ServiceBase& service)
{
	SC_HANDLE schSCManager = nullptr;
	SC_HANDLE schService = nullptr;
	TCHAR szUnquotedPath[MAX_PATH];

	if (!GetModuleFileName(NULL, szUnquotedPath, MAX_PATH))
	{
		printf("Cannot install service (%d)\n", GetLastError());
		return false;
	}

	// In case the path contains a space, it must be quoted so that
	// it is correctly interpreted. For example,
	// "d:\my share\myservice.exe" should be specified as
	// ""d:\my share\myservice.exe"".
	TCHAR szPath[MAX_PATH];
	StringCbPrintf(szPath, MAX_PATH, TEXT("\"%s\""), szUnquotedPath);


	schSCManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
	if (!schSCManager)
	{
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	const std::string& depends = service.GetDependencies();
	const std::string& acc = service.GetAccount();
	const std::string& pass = service.GetPassword();

	// Create the service
	schService = CreateService(
		schSCManager,              // SCM database 
		SVCNAME,                   // name of service 
		SVCNAME,                   // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_AUTO_START,      // start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		szPath,                    // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,						// no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 

	if (schService == NULL)
	{
		printf("CreateService failed (%d)\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return false;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	return true;
}

//static
bool ServiceInstaller::Uninstall(const ServiceBase& service)
{
	ServiceHandle svcControlManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);

	if (!svcControlManager)
	{
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	ServiceHandle servHandle = ::OpenServiceA(svcControlManager, service.GetName().c_str(),
		SERVICE_QUERY_STATUS | SERVICE_STOP | DELETE);

	if (!servHandle)
	{
		//_tprintf(_T("Couldn't open service control manager: %d\n"), ::GetLastError());
		return false;
	}

	SERVICE_STATUS servStatus = {};
	if (::ControlService(servHandle, SERVICE_CONTROL_STOP, &servStatus))
	{
		//_tprintf(_T("Stoping service %s\n"), (LPCTSTR)service.GetName());

		while (::QueryServiceStatus(servHandle, &servStatus))
		{
			if (servStatus.dwCurrentState != SERVICE_STOP_PENDING)
			{
				break;
			}
		}

		if (servStatus.dwCurrentState != SERVICE_STOPPED)
		{
			_tprintf(_T("Failed to stop the service\n"));
		}
		else
		{
			_tprintf(_T("Service stopped\n"));
		}
	}
	else
	{
		_tprintf(_T("Didn't control service: %d\n"), ::GetLastError());
	}

	if (!::DeleteService(servHandle))
	{
		_tprintf(_T("Failed to delete the service: %d\n"), GetLastError());
		return false;
	}

	return true;
}