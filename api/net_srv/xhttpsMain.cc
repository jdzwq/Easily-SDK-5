

#include "xhttps.h"
#include "srvlog.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

xhttps_param_t xp = { 0 };


int main(int argc, char* argv[])
{
	res_file_t sok = NULL;
	xhand_t bio = NULL;
	xhand_t http = NULL;
	xhand_t pipe = NULL;

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

	get_param_item(xp.sz_param, _T("CERT"), sz_cert, RES_LEN);

	if (compare_text(sz_cert, 3, _T("SSL"), 3, 1) == 0)
		xp.n_secu = _SECU_SSL;
	else if (compare_text(sz_cert, 3, _T("SSH"), 3, 1) == 0)
		xp.n_secu = _SECU_SSH;
	else
		xp.n_secu = _SECU_NONE;

	pipe = xpipe_srv(NULL, FILE_OPEN_READ);
	if (!pipe)
	{
		raise_user_error(_T("-1"), _T("child process create std pipe failed"));
	}

	sok = socket_dupli(xpipe_handle(pipe), NULL, NULL);
	if (sok == INVALID_FILE)
	{
		raise_user_error(_T("-1"), _T("child process create socket server failed"));
	}

	xpipe_free(pipe);
	pipe = NULL;

	if (xp.n_secu == _SECU_SSL)
		bio = xssl_srv(sok);
	else if (xp.n_secu == _SECU_SSH)
		bio = xssh_srv(sok);
	else
		bio = xtcp_srv(sok);

	if (!bio)
	{
		raise_user_error(_T("-1"), _T("child process create tcp server failed"));
	}

	http = xhttp_server(bio);

	if (!http)
	{
		raise_user_error(_T("-1"), _T("child process create http server failed"));
	}

	_xhttps_dispatch(http, &xp);

	xhttp_close(http);
	http = NULL;

	if (xp.n_secu == _SECU_SSL)
		xssl_close(bio);
	else if (xp.n_secu == _SECU_SSH)
		xssh_close(bio);
	else
		xtcp_close(bio);

	bio = NULL;

	socket_shutdown(sok, 2);

	thread_sleep(500);

	socket_close(sok);
	sok = INVALID_FILE;

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
	{
		if (xp.n_secu == _SECU_SSL)
			xssl_close(bio);
		else if (xp.n_secu == _SECU_SSH)
			xssh_close(bio);
		else
			xtcp_close(bio);
	}

	if (http)
		xhttp_close(http);

	xportm_log_error(errcode, errtext);

	xdl_process_uninit();

	return -1;
}

