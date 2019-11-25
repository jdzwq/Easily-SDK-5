/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc diagram document

	@module	diagramdoc.c | implement file

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

#include "diagramdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_diagram_attr(link_t_ptr ptr)
{
	set_diagram_style(ptr, _T("font-size:12;text-align:near;line-align:center;"));

	set_diagram_width(ptr, 150);
	set_diagram_height(ptr, 100);
}

void default_diagram_process_attr(link_t_ptr ilk)
{
	set_diagram_entity_style(ilk, _T("font-size:12;text-align:near;line-align:center;"));

	set_diagram_entity_x(ilk, 0);
	set_diagram_entity_y(ilk, 0);
	set_diagram_entity_width(ilk, DEF_TEXT_WIDTH);
	set_diagram_entity_height(ilk, DEF_TEXT_HEIGHT);
}

link_t_ptr create_diagram_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_DIAGRAM,-1);
	default_diagram_attr(ptr);

	return ptr;
}

void destroy_diagram_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_diagram_entityset(link_t_ptr ptr)
{
	return ptr;
}

void clear_diagram_doc(link_t_ptr ptr)
{
	ptr = get_diagram_entityset(ptr);

	delete_dom_child_nodes(ptr);
}

bool_t is_diagram_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_DIAGRAM,-1,0) == 0)? 1 : 0;
}

bool_t is_diagram_entity(link_t_ptr ptr,link_t_ptr ilk)
{
	ptr = get_diagram_entityset(ptr);

	return is_dom_child_node(ptr,ilk);
}

link_t_ptr insert_diagram_entity(link_t_ptr ptr, const tchar_t* sz_class)
{
	link_t_ptr ilk;

	ptr = get_diagram_entityset(ptr);

	ilk = insert_dom_node(ptr,LINK_LAST);
	set_dom_node_name(ilk, sz_class, -1);

	if (compare_text(sz_class, -1, DOC_DIAGRAM_PROCESS, -1, 0) == 0)
	{
		default_diagram_process_attr(ilk);
	}

	return ilk;
}

void delete_diagram_entity(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

int get_diagram_entity_count(link_t_ptr ptr)
{
	ptr = get_diagram_entityset(ptr);

	return get_dom_child_node_count(ptr);
}

int get_diagram_entity_count_by_class(link_t_ptr ptr, const tchar_t* sz_class)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_diagram_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(get_diagram_entity_class_ptr(ilk), -1, sz_class, -1, 0) == 0)
			count++;

		ilk = get_diagram_next_entity(ptr, ilk);
	}
	return count;
}

int get_diagram_entity_selected_count(link_t_ptr ptr)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_diagram_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		if (get_diagram_entity_selected(ilk))
			count++;
		ilk = get_diagram_next_entity(ptr, ilk);
	}

	return count;
}

link_t_ptr get_diagram_next_entity(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_diagram_entityset(ptr);

	if(pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_diagram_prev_entity(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_diagram_entityset(ptr);

	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_prev_link(pos);
}

link_t_ptr get_diagram_entity(link_t_ptr ptr, const tchar_t* cname)
{
	link_t_ptr tlk;

	tlk = get_diagram_next_entity(ptr, LINK_FIRST);
	while (tlk)
	{
		if (xscmp(get_diagram_entity_name_ptr(tlk), cname) == 0)
			return tlk;
		tlk = get_diagram_next_entity(ptr, tlk);
	}
	return NULL;
}

int _diagram_entity_compare_func(link_t_ptr ilk1, link_t_ptr ilk2, void* parm)
{
	int to1, to2;

	to1 = get_diagram_entity_taborder(ilk1);
	to2 = get_diagram_entity_taborder(ilk2);

	if (to1 > to2)
		return 1;
	else if (to1 < to2)
		return -1;
	else
		return 0;
}

void reset_diagram_taborder(link_t_ptr ptr)
{
	link_t_ptr root, ilk;
	int taborder = 0;

	root = get_dom_child_node_root(get_diagram_entityset(ptr));

	heap_sort_link(root, (CALLBACK_SORTLINK)_diagram_entity_compare_func, 0, NULL);

	ilk = get_diagram_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		taborder += 10;
		set_diagram_entity_taborder(ilk, taborder);

		ilk = get_diagram_next_entity(ptr, ilk);
	}
}


#endif //XDLDOC