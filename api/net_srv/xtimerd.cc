

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
#include "xtimers.h"

res_queue_t g_queue = NULL;
res_mutx_t g_mutex = NULL;

LINKPTR g_stack = NULL;

void STDCALL thread_timer_proc(void* param, unsigned char wait)
{
	xtimerd_param_t* ptimer = (xtimerd_param_t*)param;

	xdate_t dt = { 0 };
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	tchar_t token[PATH_LEN + 1] = { 0 };

	dword_t dure = 0;
	xtimers_param_t xp = { 0 };

	xdl_thread_init(0);

	xsncpy(xp.task, ptimer->task, RES_LEN);

	xtimers_dispatch(&xp);

	_calc_duretime(&ptimer->hint, &dt, &dure);

	alter_timer(g_queue, ptimer->timer, dure, dure);

	format_datetime(&dt, sz_date);
	xsprintf(token, _T("Thread timer %s attached at %s\r\n"), xp.task, sz_date);
	xportm_log_info(token, -1);

	get_loc_date(&dt);
	format_datetime(&dt, sz_date);
	xsprintf(token, _T("Thread timer %s detached at %s\r\n"), xp.task, sz_date);
	xportm_log_info(token, -1);

	xdl_thread_uninit(0);
}

void STDCALL process_timer_proc(void* param, unsigned char wait)
{
	xtimerd_param_t* ptimer = (xtimerd_param_t*)param;

	proc_info_t pi = { 0 };
	xdate_t dt = { 0 };
	dword_t dure = 0;
	tchar_t cmdline[1024] = { 0 };

	xdl_thread_init(0);

	xsprintf(cmdline, _T("task:%s"), ptimer->task);

	if (create_process(ptimer->module, cmdline, 0, &pi))
	{
		release_process(&pi);
	}
	else
	{
		xsprintf(cmdline, _T("Process timer %s create falied\r\n"), ptimer->task);
		xportm_log_info(cmdline, -1);
	}

	_calc_duretime(&ptimer->hint, &dt, &dure);

	alter_timer(g_queue, ptimer->timer, dure, dure);

	xdl_thread_uninit(0);
}

void xtimerd_start()
{
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_path[PATH_LEN + 1] = { 0 };
	tchar_t sz_time[DATE_LEN + 1] = { 0 };
	xdate_t dt = { 0 };
	dword_t dure;

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

	g_mutex = mutex_create(XTIMERD_MUTEX_NAME);
	if (!g_mutex)
	{
		destroy_xml_doc(ptr_cfg);

		destroy_timer_queue(g_queue);
		g_queue = NULL;
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

			LINKPTR nlk_hint, nlk_child = get_dom_first_child_node(nlk_timer);
			while (nlk_child)
			{
				if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_HINT, -1, 1) == 0)
				{
					nlk_hint = get_dom_first_child_node(nlk_child);

					while (nlk_hint)
					{
						if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_FIRSTTIME, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							if (is_null(sz_time))
								get_loc_date(&(ptimer->hint.fdate));
							else
								parse_datetime(&(ptimer->hint.fdate), sz_time);
						}
						else if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_LASTTIME, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							if (is_null(sz_time))
							{
								get_loc_date(&(ptimer->hint.ldate));
								plus_years(&(ptimer->hint.ldate), 1);
							}
							else
								parse_datetime(&(ptimer->hint.ldate), sz_time);
						}
						else if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_MONTH, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							ptimer->hint.n_mon = parse_intset(sz_time, -1, NULL, MAX_LONG);
							ptimer->hint.p_mon = (int*)xmem_alloc(ptimer->hint.n_mon * sizeof(int));
							parse_intset(sz_time, -1, ptimer->hint.p_mon, ptimer->hint.n_mon);
						}
						else if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_WEEK, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							ptimer->hint.n_week = parse_intset(sz_time, -1, NULL, MAX_LONG);
							ptimer->hint.p_week = (int*)xmem_alloc(ptimer->hint.n_week * sizeof(int));
							parse_intset(sz_time, -1, ptimer->hint.p_week, ptimer->hint.n_week);
						}
						else if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_DAY, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							ptimer->hint.n_day = parse_intset(sz_time, -1, NULL, MAX_LONG);
							ptimer->hint.p_day = (int*)xmem_alloc(ptimer->hint.n_day * sizeof(int));
							parse_intset(sz_time, -1, ptimer->hint.p_day, ptimer->hint.n_day);
						}
						else if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_HOUR, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							ptimer->hint.n_hour = parse_intset(sz_time, -1, NULL, MAX_LONG);
							ptimer->hint.p_hour = (int*)xmem_alloc(ptimer->hint.n_hour * sizeof(int));
							parse_intset(sz_time, -1, ptimer->hint.p_hour, ptimer->hint.n_hour);
						}
						else if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_MINUTE, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							ptimer->hint.n_min = parse_intset(sz_time, -1, NULL, MAX_LONG);
							ptimer->hint.p_min = (int*)xmem_alloc(ptimer->hint.n_min * sizeof(int));
							parse_intset(sz_time, -1, ptimer->hint.p_min, ptimer->hint.n_min);
						}
						else if (compare_text(get_dom_node_name_ptr(nlk_hint), -1, XTIMERD_ATTR_SECOND, -1, 1) == 0)
						{
							get_dom_node_text(nlk_hint, sz_time, DATE_LEN);
							ptimer->hint.n_sec = parse_intset(sz_time, -1, NULL, MAX_LONG);
							ptimer->hint.p_sec = (int*)xmem_alloc(ptimer->hint.n_sec * sizeof(int));
							parse_intset(sz_time, -1, ptimer->hint.p_sec, ptimer->hint.n_sec);
						}

						nlk_hint = get_dom_next_sibling_node(nlk_hint);
					}
				}
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_MODE, -1, 1) == 0)
					get_dom_node_text(nlk_child, ptimer->mode, INT_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_TASK, -1, 1) == 0)
					get_dom_node_text(nlk_child, ptimer->task, RES_LEN);
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

		dure = 0;
		_calc_duretime(&ptimer->hint, &dt, &dure);

		if (dure)
		{
			if (compare_text(ptimer->mode, -1, _T("thread"), -1, 1) == 0)
				ptimer->timer = create_timer(g_queue, dure, dure, thread_timer_proc, (void*)ptimer);
			else
				ptimer->timer = create_timer(g_queue, dure, dure, process_timer_proc, (void*)ptimer);
		}
		else
		{
			ptimer->timer = NULL;
		}

		thread_sleep(100);
	}
}

void xtimerd_stop()
{
	xtimerd_param_t* ptimer;
	tchar_t sz_file[PATH_LEN + 1] = { 0 };

	if (!g_stack)
		return;

	while (ptimer = (xtimerd_param_t*)pop_stack_node(g_stack))
	{
		if (ptimer->timer)
		{
			destroy_timer(g_queue, ptimer->timer);
		}

		if (ptimer->hint.p_mon)
			xmem_free(ptimer->hint.p_mon);
		if (ptimer->hint.p_week)
			xmem_free(ptimer->hint.p_week);
		if (ptimer->hint.p_day)
			xmem_free(ptimer->hint.p_day);
		if (ptimer->hint.p_hour)
			xmem_free(ptimer->hint.p_hour);
		if (ptimer->hint.n_min)
			xmem_free(ptimer->hint.p_min);
		if (ptimer->hint.p_sec)
			xmem_free(ptimer->hint.p_sec);

		xmem_free(ptimer);
	}

	destroy_timer_queue(g_queue);
	g_queue = NULL;

	mutex_destroy(XTIMERD_MUTEX_NAME, g_mutex);
	g_mutex = NULL;

	destroy_stack_table(g_stack);
	g_stack = NULL;

	xsprintf(sz_file, _T("Timer Service Exited!\r\n"));
	xportm_log_info(sz_file, -1);
}

int	xtimerd_state(void)
{
	return (g_stack) ? XTIMERD_STATE_RUNNING : XTIMERD_STATE_STOPPED;
}
