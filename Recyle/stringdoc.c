/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc stringdoc document

	@module	stringdoc.c | stringdoc implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#include "stringdoc.h"
#include "hashtable.h"
#include "xdlutil.h"
#include "xdlstr.h"
#include "domdoc.h"

void default_string_doc_attr(link_t_ptr ptr)
{
	set_string_style(ptr, _T("font-size:9;text-align:near;line-align:center;text-color:RGB(10,10,10);"));
	set_string_item_height(ptr, 60);
	set_string_item_width(ptr, 300);
}

link_t_ptr create_string_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_STRING, -1);

	default_string_doc_attr(ptr);

	return ptr;
}

void destroy_string_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_string_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_STRING, -1, 0) == 0) ? 1 : 0;
}

bool_t is_string_item(link_t_ptr ptr, link_t_ptr ilk)
{
	return is_dom_child_node(ptr, ilk);
}

void clear_string_doc(link_t_ptr ptr)
{
	delete_dom_child_node(ptr);
}

void sort_string_doc(link_t_ptr ptr,int desc)
{
	sort_dom_child_node_by_attr(ptr, ATTR_NAME, desc);
}

int get_string_item_count(link_t_ptr ptr)
{
	return get_dom_child_node_count(ptr);
}

link_t_ptr insert_string_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	ilk = insert_dom_node(ptr, pos);

	set_dom_node_name(ilk, DOC_STRING_ITEM, -1);

	return ilk;
}

void delete_string_item(link_t_ptr elk)
{
	delete_dom_node(elk);
}

int get_string_item_visible_count(link_t_ptr ptr)
{
	link_t_ptr elk;
	int count = 0;

	elk = get_string_next_visible_item(ptr, LINK_FIRST);
	while (elk)
	{
		count++;
		elk = get_string_next_visible_item(ptr, elk);
	}

	return count;
}

link_t_ptr	get_string_item(link_t_ptr ptr,const tchar_t* iname)
{
	return find_dom_node_by_attr(ptr, ATTR_NAME, -1, iname, -1);
}

link_t_ptr get_string_next_item(link_t_ptr ptr,link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_string_prev_item(link_t_ptr ptr,link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

link_t_ptr get_string_next_visible_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr elk;

	elk = get_string_next_item(ptr, pos);
	while (elk)
	{
		if (!get_string_item_hidden(elk))
			return elk;

		elk = get_string_next_item(ptr, elk);
	}

	return NULL;
}

link_t_ptr get_string_prev_visible_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr elk;

	elk = get_string_prev_item(ptr, pos);
	while (elk)
	{
		if (!get_string_item_hidden(elk))
			return elk;

		elk = get_string_prev_item(ptr, elk);
	}

	return NULL;
}


link_t_ptr get_string_item_at(link_t_ptr ptr, int index)
{
	link_t_ptr ilk;

	if (index < 0)
		return NULL;

	ilk = get_string_next_item(ptr, LINK_FIRST);
	while (ilk && index--)
		ilk = get_string_next_item(ptr, ilk);

	return ilk;
}

int get_string_item_index(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr plk;
	int index = -1;

	plk = get_string_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		index++;
		if (plk == ilk)
			break;
		plk = get_string_next_item(ptr, ilk);
	}

	return index;
}
