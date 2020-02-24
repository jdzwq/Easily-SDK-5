

#include "xpacss.h"
#include "srvlog.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

xpacss_param_t xp = { 0 };


int main(int argc, char* argv[])
{
	res_sock_t sok = NULL;
	xhand_t bio = NULL;
	xhand_t pacs = NULL;
	xhand_t pipe = NULL;
	byte_t* sz_buf = NULL;
	dword_t n_size = 0;

	int i, len;

	tchar_t sz_cert[RES_LEN] = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	xdl_process_init(XDL_APARTMENT_PROCESS);

	TRY_CATCH;

	get_runpath(NULL, xp.sz_root, PATH_LEN);

	for (i = 1; i < argc; i++)
	{
		len = xslen(xp.sz_param);
#ifdef _UNICODE
		mbs_to_ucs((const schar_t*)argv[i], -1, xp.sz_param + len, PATH_LEN - len);
#else
		a_xsncpy(xp.sz_param + len, argv[i], PATH_LEN - len);
#endif
		xsncat(xp.sz_param, _T(" "), 1);
	}

	get_param_item(xp.sz_param, _T("CERT"), sz_cert, RES_LEN);

	if (compare_text(sz_cert, -1, _T("SSL"), -1, 1) == 0)
		xp.n_secu = _SECU_SSL;
	else if (compare_text(sz_cert, -1, _T("TLS"), -1, 1) == 0)
		xp.n_secu = _SECU_TLS;
	else
		xp.n_secu = _SECU_NONE;

	pipe = xpipe_cli(PIPE_STD);

	if (!pipe)
	{
		raise_user_error(_T("-1"), _T("child process create std pipe failed"));
	}

	n_size = xsocket_share_size();
	sz_buf = (byte_t*)xmem_alloc(n_size);

	if (!xpipe_read(pipe, sz_buf, &n_size))
	{
		xsprintf(errtext, _T("child process recv data failed, the recved size is %d"), n_size);
		raise_user_error(_T("-1"), errtext);
	}

	sok = xsocket_dupli(sz_buf, n_size);

	xmem_free(sz_buf);
	sz_buf = NULL;

	if (!sok)
	{
		raise_user_error(_T("-1"), _T("child process create socket server failed"));
	}

	xpipe_free(pipe);
	pipe = NULL;

	if (xp.n_secu == _SECU_SSL)
		bio = xssl_srv(sok);
	else if (xp.n_secu == _SECU_TLS)
		bio = xtls_srv(sok);
	else
		bio = xtcp_srv(sok);

	if (!bio)
	{
		raise_user_error(_T("-1"), _T("child process create tcp server failed"));
	}

	pacs = xpacs_scp(bio);

	if (!pacs)
	{
		raise_user_error(_T("-1"), _T("child process create pacs server failed"));
	}

	_xpacss_dispatch(pacs, &xp);

	xpacs_close(pacs);
	pacs = NULL;

	if (xp.n_secu == _SECU_SSL)
		xssl_close(bio);
	else if (xp.n_secu == _SECU_TLS)
		xtls_close(bio);
	else
		xtcp_close(bio);

	bio = NULL;

	xsocket_close(sok);
	sok = NULL;

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	get_last_error(errcode, errtext, ERR_LEN);

	if (sz_buf)
		xmem_free(sz_buf);

	if (pipe)
		xpipe_free(pipe);

	if (sok)
		xsocket_close(sok);

	if (bio)
	{
		if (xp.n_secu == _SECU_SSL)
			xssl_close(bio);
		else if (xp.n_secu == _SECU_TLS)
			xtls_close(bio);
		else
			xtcp_close(bio);
	}

	if (pacs)
		xpacs_close(pacs);

	xdl_process_uninit();

	return -1;
}

