/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc chart document

	@module	chartdoc.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#include "chartdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_chart_attr(link_t_ptr ptr)
{
	set_chart_style(ptr, _T("font-size:12;text-align:near;line-align:center;"));

	set_chart_width(ptr, 150);
	set_chart_height(ptr, 100);
}

void default_map_table_attr(link_t_ptr ilk)
{
	set_chart_table_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));

	set_chart_table_x(ilk, 0);
	set_chart_table_y(ilk, 0);
	set_chart_table_width(ilk, 30);
	set_chart_table_height(ilk, 20);

	set_chart_map_table_rows(ilk, 10);
	set_chart_map_table_cols(ilk, 10);
	set_chart_map_table_rx(ilk, 5);
	set_chart_map_table_ry(ilk, 5);
}

void default_dot_table_attr(link_t_ptr ilk)
{
	set_chart_table_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));

	set_chart_table_x(ilk, 0);
	set_chart_table_y(ilk, 0);
	set_chart_table_width(ilk, 30);
	set_chart_table_height(ilk, 20);

	set_chart_dot_table_rows(ilk, 10);
	set_chart_dot_table_cols(ilk, 10);
	set_chart_dot_table_rx(ilk, 5);
	set_chart_dot_table_ry(ilk, 5);
}

void default_line_table_attr(link_t_ptr ilk)
{
	set_chart_table_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));

	set_chart_table_x(ilk, 0);
	set_chart_table_y(ilk, 0);
	set_chart_table_width(ilk, 30);
	set_chart_table_height(ilk, 20);

	set_chart_line_table_size(ilk, 10);
	set_chart_line_table_base(ilk, 0);
	set_chart_line_table_span(ilk, 1);
}

void default_bar_table_attr(link_t_ptr ilk)
{
	set_chart_table_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));

	set_chart_table_x(ilk, 0);
	set_chart_table_y(ilk, 0);
	set_chart_table_width(ilk, 30);
	set_chart_table_height(ilk, 20);

	set_chart_bar_table_size(ilk, 10);
	set_chart_bar_table_base(ilk, 0);
	set_chart_bar_table_span(ilk, 1);
}

void default_pie_table_attr(link_t_ptr ilk)
{
	set_chart_table_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));

	set_chart_table_x(ilk, 0);
	set_chart_table_y(ilk, 0);
	set_chart_table_width(ilk, 30);
	set_chart_table_height(ilk, 20);

	set_chart_pie_table_size(ilk, 10);
}

void default_counter_table_attr(link_t_ptr ilk)
{
	set_chart_table_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));

	set_chart_table_x(ilk, 0);
	set_chart_table_y(ilk, 0);
	set_chart_table_width(ilk, 30);
	set_chart_table_height(ilk, 20);

	set_chart_counter_table_size(ilk, 6);
}

link_t_ptr create_chart_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_CHART,-1);
	default_chart_attr(ptr);

	return ptr;
}

void destroy_chart_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_chart_tableset(link_t_ptr ptr)
{
	return ptr;
}

void clear_chart_doc(link_t_ptr ptr)
{
	ptr = get_chart_tableset(ptr);

	delete_dom_child_nodes(ptr);
}

bool_t is_chart_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_CHART,-1,0) == 0)? 1 : 0;
}

bool_t is_chart_table(link_t_ptr ptr,link_t_ptr ilk)
{
	ptr = get_chart_tableset(ptr);

	return is_dom_child_node(ptr,ilk);
}

link_t_ptr insert_chart_table(link_t_ptr ptr,const tchar_t* sz_class)
{
	link_t_ptr ilk;

	ptr = get_chart_tableset(ptr);

	ilk = insert_dom_node(ptr,LINK_LAST);
	set_dom_node_name(ilk, sz_class, -1);

	if (compare_text(sz_class,-1,DOC_CHART_MAP,-1,0) == 0)
		default_map_table_attr(ilk);
	else if (compare_text(sz_class, -1, DOC_CHART_DOT, -1, 0) == 0)
		default_dot_table_attr(ilk);
	else if (compare_text(sz_class, -1, DOC_CHART_LINE, -1, 0) == 0)
		default_line_table_attr(ilk);
	else if (compare_text(sz_class, -1, DOC_CHART_BAR, -1, 0) == 0)
		default_bar_table_attr(ilk);
	else if (compare_text(sz_class, -1, DOC_CHART_PIE, -1, 0) == 0)
		default_pie_table_attr(ilk);
	else if (compare_text(sz_class, -1, DOC_CHART_COUNTER, -1, 0) == 0)
		default_counter_table_attr(ilk);

	return ilk;
}

void delete_chart_table(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

int get_chart_table_count(link_t_ptr ptr)
{
	ptr = get_chart_tableset(ptr);

	return get_dom_child_node_count(ptr);
}

int get_chart_table_count_by_class(link_t_ptr ptr, const tchar_t* sz_class)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_chart_next_table(ptr, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(get_chart_table_class_ptr(ilk), -1, sz_class, -1, 0) == 0)
			count++;

		ilk = get_chart_next_table(ptr, ilk);
	}
	return count;
}

int get_chart_table_selected_count(link_t_ptr ptr)
{
	link_t_ptr flk;
	int count = 0;

	flk = get_chart_next_table(ptr, LINK_FIRST);
	while (flk)
	{
		if (get_chart_table_selected(flk))
			count++;
		flk = get_chart_next_table(ptr, flk);
	}

	return count;
}

link_t_ptr get_chart_next_table(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_chart_tableset(ptr);

	if(pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_chart_prev_table(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_chart_tableset(ptr);

	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_prev_link(pos);
}

link_t_ptr get_chart_table(link_t_ptr ptr, const tchar_t* cname)
{
	link_t_ptr tlk;

	tlk = get_chart_next_table(ptr, LINK_FIRST);
	while (tlk)
	{
		if (xscmp(get_chart_table_name_ptr(tlk), cname) == 0)
			return tlk;
		tlk = get_chart_next_table(ptr, tlk);
	}
	return NULL;
}

int _chart_table_compare_func(link_t_ptr ilk1, link_t_ptr ilk2, void* parm)
{
	int to1, to2;

	to1 = get_chart_table_taborder(ilk1);
	to2 = get_chart_table_taborder(ilk2);

	if (to1 > to2)
		return 1;
	else if (to1 < to2)
		return -1;
	else
		return 0;
}

void reset_chart_taborder(link_t_ptr ptr)
{
	link_t_ptr root, ilk;
	int taborder = 0;

	root = get_dom_child_node_root(get_chart_tableset(ptr));

	heap_sort_link(root, (CALLBACK_SORTLINK)_chart_table_compare_func, 0, NULL);

	ilk = get_chart_next_table(ptr, LINK_FIRST);
	while (ilk)
	{
		taborder += 10;
		set_chart_table_taborder(ilk, taborder);

		ilk = get_chart_next_table(ptr, ilk);
	}
}


#endif //XDLDOC