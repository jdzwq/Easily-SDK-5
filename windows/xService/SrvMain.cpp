
#include "xportd.h"

#define XPORTD_SERVER_NAME		L"xService"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, tchar_t** argv);
void ControlHandler(DWORD request);

int main(int argc, tchar_t* argv[])
{
	xdl_process_init(XDL_APARTMENT_THREAD);

	xportd_start();

	SERVICE_TABLE_ENTRY ServiceTable[2];

	ServiceTable[0].lpServiceName = XPORTD_SERVER_NAME;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	StartServiceCtrlDispatcher(ServiceTable);

	xportd_stop();

	xdl_process_uninit();

	return 0;
}

void ServiceMain(int argc, tchar_t** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = 0;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 10000;

	hStatus = RegisterServiceCtrlHandler(XPORTD_SERVER_NAME,(LPHANDLER_FUNCTION)ControlHandler);
	if (hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		return;
	}

	if (xportd_state() == XPORTD_STATE_RUNNING)
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	else
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

	SetServiceStatus(hStatus, &ServiceStatus);
}

void ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_PAUSE:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
		SetServiceStatus(hStatus, &ServiceStatus);

		xportd_stop();

		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;;
	case SERVICE_CONTROL_CONTINUE:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
		SetServiceStatus(hStatus, &ServiceStatus);

		xportd_start();

		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hStatus, &ServiceStatus);

		xportd_stop();

		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;
	default:
		break;
	}

	if (xportd_state() == XPORTD_STATE_RUNNING)
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	else
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;

	SetServiceStatus(hStatus, &ServiceStatus);
}
