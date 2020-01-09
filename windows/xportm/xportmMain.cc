

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

#include "xportm.h"

xportm_param_t xm = { 0 };
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
			event_sign(sign, 1);
			return FALSE;
		}
		break;
	}
	return TRUE;
}

int main(int argc, char* argv[])
{
	xhand_t xcons = NULL;

	xdl_process_init(XDL_APARTMENT_PROCESS);

	TRY_CATCH;

	xscpy(xm.pname, _T("xportm"));

	sign = event_create();

	_xportm_start(&xm);

	if (xm.std)
	{
		xcons_sigaction(xm.std, ConsoleHandler);
	}

	if (sign)
	{
		event_wait(sign, -1);
	}

	_xportm_stop(&xm);

	event_destroy(sign);
	sign = NULL;

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	if (sign)
		event_destroy(sign);

	xdl_process_uninit();

	return -1;
}

