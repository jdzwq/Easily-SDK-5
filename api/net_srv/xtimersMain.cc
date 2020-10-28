

#include <xdl.h>
#include "srvlog.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

int main(int argc, char* argv[])
{
	tchar_t param[4096] = { 0 };
	tchar_t token[PATH_LEN + 1] = { 0 };
	timer_block_t tb = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	int i, len, rt;
	res_modu_t lib = NULL;
	PF_TIMER_INVOKE pf_invoke = NULL;

	xdate_t dt;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };

	xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_SERVER);

	TRY_CATCH;

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);
	xsprintf(token, _T("Process timer attached at %s\r\n"), sz_date);
	xportm_log_info(token, -1);

	for (i = 1; i < argc; i++)
	{
		if (a_is_null(argv[i]))
			continue;

		len = xslen(param);
#ifdef _UNICODE
		mbs_to_ucs((const schar_t*)argv[i], -1, param + len, 4096 - len);
#else
		a_xsncpy(param + len, argv[i], 4096 - len);
#endif
		xsncat(param, _T(" "), 1);
	}

	get_param_item(param, _T("proc"), token, PATH_LEN);
	printf_path(tb.path, token);

	lib = load_library(tb.path);
	if (!lib)
	{
		raise_user_error(_T("xtimers"), _T("load library failed"));
	}

	pf_invoke = (PF_TIMER_INVOKE)get_address(lib, "timer_invoke");
	if (!pf_invoke)
	{
		raise_user_error(_T("xtimers"), _T("load address failed"));
	}

	xszero(tb.path, PATH_LEN);
	get_param_item(param, _T("path"), token, PATH_LEN);
	printf_path(tb.path, token);

	get_param_item(param, _T("task"), tb.task, RES_LEN);

	rt = (*pf_invoke)(&tb);

	free_library(lib);
	lib = NULL;

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);
	if (rt)
		xsprintf(token, _T("Process timer detached at %s with invoke error\r\n"), sz_date);
	else
		xsprintf(token, _T("Process timer detached at %s with invoke succeed\r\n"), sz_date);
	xportm_log_info(token, -1);

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	get_last_error(errcode, errtext, ERR_LEN);

	if (lib)
		free_library(lib);

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);
	xsprintf(token, _T("Process timer detached at %s with error %s\r\n"), sz_date, errtext);
	xportm_log_info(token, -1);

	xdl_process_uninit();

	return -1;
}

