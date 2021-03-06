﻿

/***********************************************************************
	Easily port dispatch

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc http dispatch document

	@module	xportd.cc | http dispatch implement file

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

#include "xportd.h"
#include "xhttps.h"
#include "xtcps.h"
#include "xpnps.h"
#include "xudps.h"
#include "srvlog.h"

LINKPTR g_stack = NULL;

void xportd_start()
{
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_root[PATH_LEN + 1] = { 0 };

	if (g_stack)
		return ;

	get_envvar(XSERVICE_ROOT, sz_root, PATH_LEN);
	if(is_null(sz_root))
	{
		//xscpy(sz_root,_T("."));
		get_runpath((res_modu_t)0, sz_root, PATH_LEN);
	}

	xsprintf(sz_file, _T("%s/xportd.config"), sz_root);

	LINKPTR ptr_cfg = create_xml_doc();
	if (!load_xml_doc_from_file(ptr_cfg, NULL, sz_file))
	{
		destroy_xml_doc(ptr_cfg);

		xsprintf(sz_root, _T("Read %s error ...\r\n"), sz_file);
		xportm_log_info(sz_root, -1);
		return ;
	}

	g_stack = create_stack_table();

	LINKPTR nlk_config = get_xml_dom_node(ptr_cfg);
	LINKPTR nlk_port = get_dom_first_child_node(nlk_config);
	while (nlk_port)
	{
		if (compare_text(get_dom_node_attr_ptr(nlk_port, _T("type"), -1), -1, XPORTD_PORT_TYPE_HTTP, -1, 1) == 0)
		{
			xhttps_param_t* phttps = (xhttps_param_t*)xmem_alloc(sizeof(xhttps_param_t));
			
			get_dom_node_attr(nlk_port, _T("bind"), -1, phttps->sz_port, INT_LEN);

			xszero(sz_file, PATH_LEN);

			LINKPTR nlk_child = get_dom_first_child_node(nlk_port);
			while (nlk_child)
			{
				if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("mode"), -1, 1) == 0)
					get_dom_node_text(nlk_child, phttps->sz_mode, INT_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("module"), -1, 1) == 0)
					get_dom_node_text(nlk_child, sz_file, PATH_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("param"), -1, 1) == 0)
					get_dom_node_text(nlk_child, phttps->sz_param, PATH_LEN);
				
				nlk_child = get_dom_next_sibling_node(nlk_child);
			}

			if (is_null(phttps->sz_mode))
			{
				xscpy(phttps->sz_mode, _T("process"));
			}

			if (!is_null(sz_file))
			{
				printf_path(phttps->sz_module, sz_file);
			}

			xportd_param_t* phttpd = (xportd_param_t*)xmem_alloc(sizeof(xportd_param_t));
			phttpd->param = (void*)phttps;
			phttpd->pf_start = (PF_PORT_START)_xhttps_start;
			phttpd->pf_stop = (PF_PORT_STOP)_xhttps_stop;

			push_stack_node(g_stack, (void*)phttpd);
		}
		else if (compare_text(get_dom_node_attr_ptr(nlk_port, _T("type"), -1), -1, XPORTD_PORT_TYPE_TCP, -1, 1) == 0)
		{
			xtcps_param_t* ptcps = (xtcps_param_t*)xmem_alloc(sizeof(xtcps_param_t));

			get_dom_node_attr(nlk_port, _T("bind"), -1, ptcps->sz_port, INT_LEN);

			xszero(sz_file, PATH_LEN);

			LINKPTR nlk_child = get_dom_first_child_node(nlk_port);
			while (nlk_child)
			{
				if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("mode"), -1, 1) == 0)
					get_dom_node_text(nlk_child, ptcps->sz_mode, INT_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("module"), -1, 1) == 0)
					get_dom_node_text(nlk_child, sz_file, PATH_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("param"), -1, 1) == 0)
					get_dom_node_text(nlk_child, ptcps->sz_param, PATH_LEN);

				nlk_child = get_dom_next_sibling_node(nlk_child);
			}

			if (is_null(ptcps->sz_mode))
			{
				xscpy(ptcps->sz_mode, _T("process"));
			}

			if (!is_null(sz_file))
			{
				printf_path(ptcps->sz_module, sz_file);
			}

			xportd_param_t* phttpd = (xportd_param_t*)xmem_alloc(sizeof(xportd_param_t));
			phttpd->param = (void*)ptcps;
			phttpd->pf_start = (PF_PORT_START)_xtcps_start;
			phttpd->pf_stop = (PF_PORT_STOP)_xtcps_stop;

			push_stack_node(g_stack, (void*)phttpd);
		}
		else if (compare_text(get_dom_node_attr_ptr(nlk_port, _T("type"), -1), -1, XPORTD_PORT_TYPE_UDP, -1, 1) == 0)
		{
			xudps_param_t* pudps = (xudps_param_t*)xmem_alloc(sizeof(xudps_param_t));

			get_dom_node_attr(nlk_port, _T("bind"), -1, pudps->sz_port, INT_LEN);

			xszero(sz_file, PATH_LEN);

			LINKPTR nlk_child = get_dom_first_child_node(nlk_port);
			while (nlk_child)
			{
				if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("mode"), -1, 1) == 0)
					get_dom_node_text(nlk_child, pudps->sz_mode, INT_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("module"), -1, 1) == 0)
					get_dom_node_text(nlk_child, sz_file, PATH_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("param"), -1, 1) == 0)
					get_dom_node_text(nlk_child, pudps->sz_param, PATH_LEN);

				nlk_child = get_dom_next_sibling_node(nlk_child);
			}

			if (is_null(pudps->sz_mode))
			{
				xscpy(pudps->sz_mode, _T("process"));
			}

			if (!is_null(sz_file))
			{
				printf_path(pudps->sz_module, sz_file);
			}

			xportd_param_t* phttpd = (xportd_param_t*)xmem_alloc(sizeof(xportd_param_t));
			phttpd->param = (void*)pudps;
			phttpd->pf_start = (PF_PORT_START)_xudps_start;
			phttpd->pf_stop = (PF_PORT_STOP)_xudps_stop;

			push_stack_node(g_stack, (void*)phttpd);
		}
		else if (compare_text(get_dom_node_attr_ptr(nlk_port, _T("type"), -1), -1, XPORTD_PORT_TYPE_PNP, -1, 1) == 0)
		{
			xpnps_param_t* ppnps = (xpnps_param_t*)xmem_alloc(sizeof(xpnps_param_t));

			get_dom_node_attr(nlk_port, _T("bind"), -1, ppnps->sz_port, INT_LEN);

			xszero(sz_file, PATH_LEN);

			LINKPTR nlk_child = get_dom_first_child_node(nlk_port);
			while (nlk_child)
			{
				if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("mode"), -1, 1) == 0)
					get_dom_node_text(nlk_child, ppnps->sz_mode, INT_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("module"), -1, 1) == 0)
					get_dom_node_text(nlk_child, sz_file, PATH_LEN);
				else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, _T("param"), -1, 1) == 0)
					get_dom_node_text(nlk_child, ppnps->sz_param, PATH_LEN);

				nlk_child = get_dom_next_sibling_node(nlk_child);
			}

			if (is_null(ppnps->sz_mode))
			{
				xscpy(ppnps->sz_mode, _T("process"));
			}

			if (!is_null(sz_file))
			{
				printf_path(ppnps->sz_module, sz_file);
			}

			xportd_param_t* phttpd = (xportd_param_t*)xmem_alloc(sizeof(xportd_param_t));
			phttpd->param = (void*)ppnps;
			phttpd->pf_start = (PF_PORT_START)_xpnps_start;
			phttpd->pf_stop = (PF_PORT_STOP)_xpnps_stop;

			push_stack_node(g_stack, (void*)phttpd);
		}

		nlk_port = get_dom_next_sibling_node(nlk_port);
	}

	destroy_xml_doc(ptr_cfg);

	xsprintf(sz_file, _T("Port Service Starting ...\r\n"));
	xportm_log_info(sz_file, -1);

	xsprintf(sz_file, _T("Service Root Path: %s\r\n"), sz_root);
	xportm_log_info(sz_file, -1);

	xszero(sz_root, PATH_LEN);
	get_envvar(XSERVICE_DATA, sz_root, PATH_LEN);
	xsprintf(sz_file, _T("Service Data Path: %s\r\n"), sz_root);
	xportm_log_info(sz_file, -1);

	int n = get_stack_node_count(g_stack);
	for (int i = 0; i < n; i++)
	{
		xportd_param_t* phttpd = (xportd_param_t*)peek_stack_node(g_stack, i);

		(*phttpd->pf_start)(phttpd->param);

		thread_sleep(100);
	}
}

void xportd_stop()
{
	xportd_param_t* phttpd;
	tchar_t sz_file[PATH_LEN + 1] = { 0 };

	if (!g_stack)
		return;

	while (phttpd = (xportd_param_t*)pop_stack_node(g_stack))
	{
		(*phttpd->pf_stop)(phttpd->param);

		xmem_free(phttpd->param);
		xmem_free(phttpd);
	}

	destroy_stack_table(g_stack);
	g_stack = NULL;

	xsprintf(sz_file, _T("Port Service Exited!\r\n"));
	xportm_log_info(sz_file, -1);
}

int	xportd_state(void)
{
	return (g_stack) ? XPORTD_STATE_RUNNING : XPORTD_STATE_STOPPED;
}
