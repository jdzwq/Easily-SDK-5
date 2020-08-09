

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

res_queue_t g_queue = NULL;

LINKPTR g_stack = NULL;

void xtimerd_start()
{
	tchar_t sz_file[PATH_LEN] = { 0 };
	tchar_t sz_path[PATH_LEN] = { 0 };

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
					ptimer->duetime = (dword_t)xstol(get_dom_node_text_ptr(nlk_child));
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_PERIOD, -1, 1) == 0)
					ptimer->period = (dword_t)xstol(get_dom_node_text_ptr(nlk_child));
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_PARAM, -1, 1) == 0)
					get_dom_node_text(nlk_child, ptimer->param, PATH_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, XTIMERD_ATTR_MODULE, -1, 1) == 0)
					get_dom_node_text(nlk_child, sz_path, PATH_LEN);
				
				nlk_child = get_dom_next_sibling_node(nlk_child);
			}

			if (!is_null(sz_path))
			{
				printf_path(sz_file, sz_path);

				ptimer->module = load_library(sz_file);
			}

			push_stack_node(g_stack, (void*)ptimer);
		}
		
		nlk_timer = get_dom_next_sibling_node(nlk_timer);
	}

	destroy_xml_doc(ptr_cfg);

	int n = get_stack_node_count(g_stack);
	for (int i = 0; i < n; i++)
	{
		xtimerd_param_t* ptimer = (xtimerd_param_t*)peek_stack_node(g_stack, i);

		if (ptimer->module)
		{
			PF_TIMERFUNC pf = (PF_TIMERFUNC)get_address(ptimer->module, "timer_invoke");
			if (pf)
			{
				ptimer->timer = create_timer(g_queue, ptimer->duetime, ptimer->period, pf, (void*)ptimer->param);
			}
		}

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

		if (ptimer->module)
		{
			free_library(ptimer->module);
		}

		xmem_free(ptimer);
	}

	destroy_timer_queue(g_queue);
	g_queue = NULL;

	destroy_stack_table(g_stack);
	g_stack = NULL;
}

int	xtimerd_state(void)
{
	return (g_stack) ? XTIMERD_STATE_RUNNING : XTIMERD_STATE_STOPPED;
}
