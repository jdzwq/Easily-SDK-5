

#include "xtcps.h"
#include "srvlog.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

xtcps_param_t xp = { 0 };


int main(int argc, char* argv[])
{
	res_sock_t sok = NULL;
	xhand_t tcp = NULL;
	xhand_t pipe = NULL;
	byte_t* sz_buf = NULL;
	dword_t n_size = 0;

	int i, len;

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
		raise_user_error(_T("-1"), _T("child process create socket failed"));
	}

	xpipe_free(pipe);
	pipe = NULL;

	tcp = xtcp_srv(sok);
	xtcp_set_linger(tcp, 1, 10);

	_xtcps_dispatch(tcp, &xp);

	xtcp_close(tcp);
	tcp = NULL;

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

	if (tcp)
		xtcp_close(tcp);

	xdl_process_uninit();

	return -1;
}

