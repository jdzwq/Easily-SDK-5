

/***********************************************************************
	Easily timer dispatch

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc timer dispatch document

	@module	xtimers.cc | timer dispatch implement file

***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "xtimers.h"
#include "srvlog.h"

void _calc_duretime(timer_hint* pth, xdate_t* pdate, dword_t* pdure)
{
	bool_t b = 0;
	int i;
	xdate_t dt1 = { 0 };
	xdate_t dt2 = { 0 };
	dword_t du = 0;

	get_loc_date(&dt1);
	xmem_copy((void*)&dt2, (void*)&dt1, sizeof(xdate_t));

	if (compare_datetime(&dt1, &pth->fdate) < 0)
	{
		du = diff_secs(&dt1, &pth->fdate);
		xmem_copy((void*)&dt1, (void*)&pth->fdate, sizeof(xdate_t));
	}

	while (compare_datetime(&dt1, &pth->ldate) < 0)
	{
		mak_loc_week(&dt1);

		b = 0;
		for (i = 0; i < pth->n_mon; i++)
		{
			if (dt1.mon == pth->p_mon[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && pth->n_mon)
		{
			plus_months(&dt1, 1);
			dt1.day = 1;
			dt1.hour = 0;
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < pth->n_week; i++)
		{
			if (dt1.wday == pth->p_week[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && pth->n_week)
		{
			plus_days(&dt1, 1);
			dt1.hour = 0;
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < pth->n_day; i++)
		{
			if (dt1.day == pth->p_day[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && pth->n_day)
		{
			plus_days(&dt1, 1);
			dt1.hour = 0;
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < pth->n_hour; i++)
		{
			if (dt1.hour == pth->p_hour[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && pth->n_hour)
		{
			plus_hours(&dt1, 1);
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < pth->n_min; i++)
		{
			if (dt1.min == pth->p_min[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && pth->n_min)
		{
			plus_minutes(&dt1, 1);
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < pth->n_sec; i++)
		{
			if (dt1.sec == pth->p_sec[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && pth->n_sec)
		{
			plus_seconds(&dt1, 1);
			continue;
		}

		break;
	}

	du += diff_secs(&dt2, &dt1);

	xmem_copy((void*)pdate, (void*)&dt1, sizeof(xdate_t));

	*pdure = du * 1000;
}

bool_t xtimers_config(const tchar_t* task, tchar_t* tname, tchar_t* tpath, tchar_t* tproc)
{
	link_t_ptr ptr_cfg = NULL;
	link_t_ptr nlk_root, nlk;

	tchar_t token[PATH_LEN + 1] = { 0 };
	tchar_t sz_file[PATH_LEN + 1] = { 0 };

	get_envvar(XSERVICE_ROOT, token, PATH_LEN);
	if (is_null(token))
	{
		get_runpath((res_modu_t)0, token, PATH_LEN);
	}

	xsprintf(sz_file, _T("%s/sch/%s.task"), token, task);

	ptr_cfg = create_xml_doc();
	if (!load_xml_doc_from_file(ptr_cfg, NULL, sz_file))
	{
		destroy_xml_doc(ptr_cfg);
		return 0;
	}

	nlk_root = get_xml_dom_node(ptr_cfg);
	nlk = get_dom_first_child_node(nlk_root);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("name"), -1, 1) == 0 && tname)
		{
			get_dom_node_text(nlk, tname, PATH_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("path"), -1, 1) == 0 && tpath)
		{
			get_dom_node_text(nlk, tpath, PATH_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("proc"), -1, 1) == 0 && tproc)
		{
			get_dom_node_text(nlk, tproc, PATH_LEN);
		}

		nlk = get_dom_next_sibling_node(nlk);
	}

	destroy_xml_doc(ptr_cfg);
	ptr_cfg = NULL;

	return 1;
}

void xtimers_dispatch(xtimers_param_t* pts)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	res_modu_t lib = NULL;
	PF_TIMER_INVOKE pf_invoke = NULL;
	int rt;
	
	tchar_t sz_proc[PATH_LEN + 1] = { 0 };
	timer_block_t tb = { 0 };

	link_t_ptr ptr_cfg = NULL;

	TRY_CATCH;

	if (!xtimers_config(pts->task, tb.name, tb.path, sz_proc))
	{
		raise_user_error(_T("-1"), _T("timer load configure failed"));
	}

	lib = load_library(sz_proc);
	if (!lib)
	{
		raise_user_error(_T("-1"), _T("timer load module failed"));
	}

	pf_invoke = (PF_TIMER_INVOKE)get_address(lib, "timer_invoke");
	if (!pf_invoke)
	{
		raise_user_error(_T("-1"), _T("timer get function failed"));
	}

	rt = (*pf_invoke)(&tb);

	free_library(lib);
	lib = NULL;

	END_CATCH;

	return;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (lib)
		free_library(lib);

	if (xslen(sz_error) < ERR_LEN - 2)
		xscat(sz_error, _T("\r\n"));

	xportm_log_info(sz_error, -1);

	return;
}

