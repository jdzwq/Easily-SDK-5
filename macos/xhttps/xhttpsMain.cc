

#include "xhttps.h"
#include "srvlog.h"

xhttps_param_t xp = { 0 };


int main(int argc, char* argv[])
{
	res_file_t sok = NULL;
	xhand_t bio = NULL;
	xhand_t http = NULL;
	xhand_t pipe = NULL;
    
	int i, len;
	tchar_t sz_cert[RES_LEN] = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    int fd = open("/dev/null",O_RDWR,0);
    if(fd != -1){
        dup2(fd,STDOUT_FILENO);
        dup2(fd,STDERR_FILENO);
    }

	xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_SERVER);

	TRY_CATCH;
    
	for (i = 1; i < argc; i++)
	{
        if(a_is_null(argv[i]))
            continue;
        
		len = xslen(xp.sz_param);
#ifdef _UNICODE
		mbs_to_ucs((const schar_t*)argv[i], a_xslen(argv[i]), xp.sz_param + len, PATH_LEN - len);
#else
		a_xsncpy(xp.sz_param + len, argv[i], a_xslen(argv[i]));
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
		raise_user_error(_T("-1"), _T("child process create socket failed"));
	}

	xpipe_free(pipe);
	pipe = NULL;

    if (xp.n_secu == _SECU_SSL)
        bio = xssl_srv(sok);
    else  if (xp.n_secu == _SECU_SSH)
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

	socket_close(sok);
	sok = NULL;
    
    xportm_log_error(_T("xhttps"), _T("child process terminated"));

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	get_last_error(errcode, errtext, ERR_LEN);

    XDL_TRACE_LAST;
    
	if (pipe)
		xpipe_free(pipe);

    if (http)
        xhttp_close(http);
    
    if (bio)
	{
        if (xp.n_secu == _SECU_SSL)
            xssl_close(bio);
        else if (xp.n_secu == _SECU_SSH)
            xssh_close(bio);
        else
            xtcp_close(bio);
	}
    
    if (sok != INVALID_FILE)
        socket_close(sok);
    
    xportm_log_error(errcode, errtext);

	xdl_process_uninit();

	return -1;
}

