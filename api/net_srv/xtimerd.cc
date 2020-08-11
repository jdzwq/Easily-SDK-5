

/***********************************************************************
	Easily timer dispatch

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc http dispatch document

	@module	xtimerd.cc | http dispatch implement file

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

#include "xtimerd.h"
#include "srvlog.h"

res_queue_t g_queue = NULL;

LINKPTR g_stack = NULL;

static dword_t _calc_duetime(const tchar_t* sz_time)
{
	xdate_t dt, dt_cur;
	dword_t ms;

	if (is_null(sz_time))
		return 0;

	parse_datetime(&dt, sz_time);

	get_loc_date(&dt_cur);

	while (compare_datetime(&dt, &dt_cur) < 0)
	{
		plus_days(&dt, 1);
	}

	ms = diff_secs(&dt_cur, &dt) * 1000;

	return ms;
}

void STDCALL thread_timer_proc(void* param, unsigned char wait)
{
	xtimerd_param_t* ptimer = (xtimerd_param_t*)param;

	res_modu_t lib = NULL;
	PF_TIMER_INVOKE pf_invoke = NULL;
	int rt;
	timer_block_t tb = { 0 };

	tchar_t token[PATH_LEN] = { 0 };

	xdate_t dt = { 0 };
	tchar_t sz_date[DATE_LEN] = { 0 };
	
	xdl_thread_init(0);

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);
	xsprintf(token, _T("Thread timer attached at %s\r\n"), sz_date);
	xportm_log_info(token, -1);
	
	xszero(token, PATH_LEN);
	get_param_item(ptimer->param, _T("proc"), token, PATH_LEN);
	printf_path(tb.path, token);

	lib = load_library(tb.path);
	if (!lib)
	{
		xsprintf(token, _T("Thread timer load %s failed\r\n"), tb.path);
		xportm_log_info(token, -1);

		xdl_thread_uninit(0);
		return;
	}

	pf_invoke = (PF_TIMER_INVOKE)get_address(lib, "timer_invoke");
	if (!pf_invoke)
	{
		free_library(lib);

		xsprintf(token, _T("Thread timer get timer_invoke address failed\r\n"));
		xportm_log_info(token, -1);

		xdl_thread_uninit(0);
		return;
	}

	xszero(tb.path, PATH_LEN);
	get_param_item(ptimer->param, _T("path"), token, PATH_LEN);
	printf_path(tb.path, token);
	
	get_param_item(ptimer->param, _T("task"), tb.task, RES_LEN);

	rt = (*pf_invoke)(&tb);

	free_library(lib);

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);
	if (rt)
		xsprintf(token, _T("Thread timer detached at %s with invoke error\r\n"), sz_date);
	else
		xsprintf(token, _T("Thread timer detached at %s with invoke succeed\r\n"), sz_date);
	xportm_log_info(token, -1);

	xdl_thread_uninit(0);
}

void STDCALL process_timer_proc(void* param, unsigned char wait)
{
	xtimerd_param_t* ptimer = (xtimerd_param_t*)param;

	proc_info_t pi = { 0 };

	if (create_process(ptimer->module, (tchar_t*)ptimer->param, 0, &pi))
	{
		release_process(&pi);
	}
	else
	{
		xportm_log_info(_T("Process timer create falied\r\n"), -1);
	}
}

void xtimerd_start()
{
	tchar_t sz_file[PATH_LEN] = { 0 };
	tchar_t sz_path[PATH_LEN] = { 0 };
	tchar_t sz_time[DATE_LEN] = { 0 };

	if (g_stack)
		return ;

	get_envvar(XSERVICE_ROOT, sz_path, PATH_LEN);
	if (is_null(sz_path))
	{
		//xscpy(sz_path,_T("."));
		get_runpath((res_modu_t)0, sz_path, PATH_LEN);
	}

	xsprintf(sz_file, _T("%s/xtimerd.config"), sz_path);

	LINKPTR ptr_cfg = create_xml_doc();
	if (!load_xml_doc_from_file(ptr_cfg, NULL, sz_file))
	{
		destroy_xml_doc(ptr_cfg);

		xsprintf(sz_path, _T("Read %s error ...\r\n"), sz_file);
		xportm_log_info(sz_path, -1);
		return ;
	}

	g_queue = create_timer_queue();
	if (!g_queue)
	{
		destroy_xml_doc(ptr_cfg);
		return;
	}

	g_stack = create_stack_table();

	LINKPTR nlk_config = get_xml_dom_node(ptr_cfg);
	LINKPTR nlk_timer = get_dom_first_child_node(nlk_config);
	while (nlk_timer)
	{
		if (compare_text(get_dom_node_name_ptr(nlk_timer), -1, XTIMERD_ATTR_SCHEDULE, -1, 1) == 0)
		{
			xtimerd_param_t* ptimer = (xtimerd_param_t*)xmem_alloc(sizeof(xtimerd_param_t));
			
			xszero(sz_file, PATH_LEN);

			LINKPTR nlk_child = get_dom_first_child_node(nlk_timer);
			while (nlk_child)
			{
				if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_DUETIME, -1, 1) == 0)
				{
					get_dom_node_text(nlk_child, sz_time, DATE_LEN);
					ptimer->duetime = _calc_duetime(sz_time);
				}
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_PERIOD, -1, 1) == 0)
					ptimer->period = (dword_t)xstol(get_dom_node_text_ptr(nlk_child));
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_MODE, -1, 1) == 0)
					get_dom_node_text(nlk_child, ptimer->mode, INT_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_PARAM, -1, 1) == 0)
					get_dom_node_text(nlk_child, ptimer->param, 4096);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_MODULE, -1, 1) == 0)
				{
					get_dom_node_text(nlk_child, sz_path, PATH_LEN);
					printf_path(ptimer->module, sz_path);
				}
				
				nlk_child = get_dom_next_sibling_node(nlk_child);
			}

			push_stack_node(g_stack, (void*)ptimer);
		}
		
		nlk_timer = get_dom_next_sibling_node(nlk_timer);
	}

	destroy_xml_doc(ptr_cfg);

	xsprintf(sz_file, _T("Timer Service Starting ...\r\n"));
	xportm_log_info(sz_file, -1);

	int n = get_stack_node_count(g_stack);
	for (int i = 0; i < n; i++)
	{
		xtimerd_param_t* ptimer = (xtimerd_param_t*)peek_stack_node(g_stack, i);

		if (compare_text(ptimer->mode,-1,_T("thread"),-1,1) == 0)
			ptimer->timer = create_timer(g_queue, ptimer->duetime, ptimer->period, thread_timer_proc, (void*)ptimer);
		else
			ptimer->timer = create_timer(g_queue, ptimer->duetime, ptimer->period, process_timer_proc, (void*)ptimer);

		thread_sleep(100);
	}
}

void xtimerd_stop()
{
	xtimerd_param_t* ptimer;
	tchar_t sz_file[PATH_LEN] = { 0 };

	if (!g_stack)
		return;

	while (ptimer = (xtimerd_param_t*)pop_stack_node(g_stack))
	{
		if (ptimer->timer)
		{
			destroy_timer(g_queue, ptimer->timer);
		}

		xmem_free(ptimer);
	}

	destroy_timer_queue(g_queue);
	g_queue = NULL;

	destroy_stack_table(g_stack);
	g_stack = NULL;

	xsprintf(sz_file, _T("Timer Service Exited!\r\n"));
	xportm_log_info(sz_file, -1);
}

int	xtimerd_state(void)
{
	return (g_stack) ? XTIMERD_STATE_RUNNING : XTIMERD_STATE_STOPPED;
}
