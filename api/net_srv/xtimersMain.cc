
#include "srvlog.h"
#include "xtimers.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

int main(int argc, char* argv[])
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	tchar_t param[4096] = { 0 };
	tchar_t token[PATH_LEN + 1] = { 0 };
	
	xtimers_param_t xp = { 0 };

	int i, len;
	xdate_t dt;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };

	xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_SERVER);

	TRY_CATCH;

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

	get_param_item(param, _T("task"), xp.task, RES_LEN);

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);
	xsprintf(token, _T("Process timer %s attached at %s\r\n"), xp.task, sz_date);
	xportm_log_info(token, -1);

	xtimers_dispatch(&xp);

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);

	xsprintf(token, _T("Process timer %s detached at %s \r\n"), xp.task, sz_date);
	xportm_log_info(token, -1);

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	get_last_error(errcode, errtext, ERR_LEN);

	xsprintf(token, _T("Process timer %s detached at %s with error %s\r\n"),xp.task, sz_date, errtext);
	xportm_log_info(token, -1);

	xdl_process_uninit();

	return -1;
}

