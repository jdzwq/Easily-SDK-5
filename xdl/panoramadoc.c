/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc panorama document

	@module	panoramadoc.c | implement file

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

#include "panoramadoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_panorama_attr(link_t_ptr ptr)
{
	set_panorama_style(ptr, _T("font-size:12;text-align:near;line-align:center;"));

	set_panorama_width(ptr, 150);
	set_panorama_height(ptr, 100);
}

void default_panorama_plot_attr(link_t_ptr ilk)
{
	set_panorama_plot_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));

	set_panorama_plot_x(ilk, 0);
	set_panorama_plot_y(ilk, 0);
	set_panorama_plot_width(ilk, 30);
	set_panorama_plot_height(ilk, 20);
}


link_t_ptr create_panorama_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_PANORAMA,-1);
	default_panorama_attr(ptr);

	return ptr;
}

void destroy_panorama_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_panorama_plotset(link_t_ptr ptr)
{
	return ptr;
}

void clear_panorama_doc(link_t_ptr ptr)
{
	ptr = get_panorama_plotset(ptr);

	delete_dom_child_nodes(ptr);
}

bool_t is_panorama_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_PANORAMA,-1,0) == 0)? 1 : 0;
}

bool_t is_panorama_plot(link_t_ptr ptr,link_t_ptr ilk)
{
	ptr = get_panorama_plotset(ptr);

	return is_dom_child_node(ptr,ilk);
}

link_t_ptr insert_panorama_plot(link_t_ptr ptr,const tchar_t* sz_class)
{
	link_t_ptr ilk;

	ptr = get_panorama_plotset(ptr);

	ilk = insert_dom_node(ptr,LINK_LAST);
	set_dom_node_name(ilk, sz_class, -1);

	default_panorama_plot_attr(ilk);

	return ilk;
}

void delete_panorama_plot(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

int get_panorama_plot_count(link_t_ptr ptr)
{
	ptr = get_panorama_plotset(ptr);

	return get_dom_child_node_count(ptr);
}

int get_panorama_plot_count_by_class(link_t_ptr ptr, const tchar_t* sz_class)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_panorama_next_plot(ptr, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(get_panorama_plot_class_ptr(ilk), -1, sz_class, -1, 0) == 0)
			count++;

		ilk = get_panorama_next_plot(ptr, ilk);
	}
	return count;
}

int get_panorama_plot_selected_count(link_t_ptr ptr)
{
	link_t_ptr flk;
	int count = 0;

	flk = get_panorama_next_plot(ptr, LINK_FIRST);
	while (flk)
	{
		if (get_panorama_plot_selected(flk))
			count++;
		flk = get_panorama_next_plot(ptr, flk);
	}

	return count;
}

link_t_ptr get_panorama_next_plot(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_panorama_plotset(ptr);

	if(pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_panorama_prev_plot(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_panorama_plotset(ptr);

	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_prev_link(pos);
}

link_t_ptr get_panorama_plot(link_t_ptr ptr, const tchar_t* cname)
{
	link_t_ptr tlk;

	tlk = get_panorama_next_plot(ptr, LINK_FIRST);
	while (tlk)
	{
		if (xscmp(get_panorama_plot_name_ptr(tlk), cname) == 0)
			return tlk;
		tlk = get_panorama_next_plot(ptr, tlk);
	}
	return NULL;
}

int _panorama_plot_compare_func(link_t_ptr ilk1, link_t_ptr ilk2, void* parm)
{
	int to1, to2;

	to1 = get_panorama_plot_taborder(ilk1);
	to2 = get_panorama_plot_taborder(ilk2);

	if (to1 > to2)
		return 1;
	else if (to1 < to2)
		return -1;
	else
		return 0;
}

void reset_panorama_taborder(link_t_ptr ptr)
{
	link_t_ptr root, ilk;
	int taborder = 0;

	root = get_dom_child_node_root(get_panorama_plotset(ptr));

	heap_sort_link(root, (CALLBACK_SORTLINK)_panorama_plot_compare_func, 0, NULL);

	ilk = get_panorama_next_plot(ptr, LINK_FIRST);
	while (ilk)
	{
		taborder += 10;
		set_panorama_plot_taborder(ilk, taborder);

		ilk = get_panorama_next_plot(ptr, ilk);
	}
}


#endif //XDLDOC