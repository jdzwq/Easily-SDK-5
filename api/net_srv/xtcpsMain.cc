﻿

#include "xtcps.h"
#include "srvlog.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

xtcps_param_t xp = { 0 };


int main(int argc, char* argv[])
{
	res_file_t sok = NULL;
	xhand_t bio = NULL;
	xhand_t pipe = NULL;
	dword_t dw;

	int i, len;

	tchar_t sz_cert[RES_LEN + 1] = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_SERVER);

	TRY_CATCH;

	for (i = 1; i < argc; i++)
	{
		if (a_is_null(argv[i]))
			continue;

		len = xslen(xp.sz_param);
#ifdef _UNICODE
		mbs_to_ucs((const schar_t*)argv[i], -1, xp.sz_param + len, PATH_LEN - len);
#else
		a_xsncpy(xp.sz_param + len, argv[i], PATH_LEN - len);
#endif
		xsncat(xp.sz_param, _T(" "), 1);
	}

	pipe = xpipe_srv(NULL, FILE_OPEN_READ);

	if (!pipe)
	{
		raise_user_error(_T("-1"), _T("child process create std pipe failed"));
	}

	dw = 0;
	sok = socket_dupli(xpipe_handle(pipe), NULL, &dw);

	if (sok == INVALID_FILE)
	{
		raise_user_error(_T("-1"), _T("child process create socket server failed"));
	}

	xpipe_free(pipe);
	pipe = NULL;

	bio = xtcp_srv(sok);

	if (!bio)
	{
		raise_user_error(_T("-1"), _T("child process create tcp server failed"));
	}

	_xtcps_dispatch(bio, &xp);

	xtcp_close(bio);

	bio = NULL;

	socket_shutdown(sok, 2);

	thread_sleep(500);

	socket_close(sok);
	sok = INVALID_FILE;

	xportm_log_error(_T("tcps"), _T("process terminated"));

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	get_last_error(errcode, errtext, ERR_LEN);

	if (sok != INVALID_FILE)
		socket_close(sok);

	if (pipe)
		xpipe_free(pipe);

	if (bio)
		xtcp_close(bio);

	xportm_log_error(errcode, errtext);

	xdl_process_uninit();

	return -1;
}

