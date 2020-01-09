

#include "xhttpm.h"

xhttpm_param_t xm = { 0 };
res_even_t sign = NULL;

BOOL WINAPI ConsoleHandler(DWORD ev)
{
	switch (ev)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		if (sign)
		{
			xevent_sign(sign, 1);
			return FALSE;
		}
		break;
	}
	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	xhand_t xcons = NULL;

	xdl_process_init(XDL_APARTMENT_PROCESS);

	TRY_CATCH;

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE);

	xscpy(xm.pname, _T("xhttpm"));

	sign = xevent_create(NULL);

	_xhttpm_start(&xm);

	if (sign)
	{
		xevent_wait(sign, -1);
	}

	_xhttpm_stop(&xm);

	xevent_destroy(sign);
	sign = NULL;

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, FALSE);

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, FALSE);

	if (sign)
		xevent_destroy(sign);

	xdl_process_uninit();

	return -1;
}

