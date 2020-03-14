/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tool document

	@module	tooldoc.c | implement file

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

#include "tooldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_tool_attr(link_t_ptr ptr)
{
	set_tool_style(ptr, _T("font-size:9;text-align:center;line-align:center;"));
	set_tool_title_height(ptr, DEF_ITEM_HEIGHT);
	set_tool_icon_span(ptr, DEF_GDI_ATTR_GIZMO_SPAN);
}

void default_tool_group_attr(link_t_ptr glk)
{
	set_tool_group_item_width(glk, 20);
	set_tool_group_item_height(glk, DEF_ITEM_HEIGHT);
}

link_t_ptr create_tool_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_TOOL,-1);
	default_tool_attr(ptr);
	return ptr;
}

void destroy_tool_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_tool_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_TOOL,-1,0) == 0)? 1 : 0;
}

bool_t is_tool_group_item(link_t_ptr ptr,link_t_ptr ilk)
{
	return is_dom_child_node(ptr,ilk);
}

link_t_ptr get_tool_group(link_t_ptr ptr, const tchar_t* sz_name)
{
	link_t_ptr glk;

	if (is_null(sz_name))
		return NULL;

	glk = get_tool_next_group(ptr, LINK_FIRST);
	while (glk)
	{
		if (compare_text(sz_name, -1, get_tool_group_name_ptr(glk), -1, 1) == 0)
			return glk;
		glk = get_tool_next_group(ptr, glk);
	}

	return NULL;
}

void clear_tool_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

void merge_tool_doc(link_t_ptr ptr_dst, link_t_ptr ptr_src)
{
	link_t_ptr glk;

	while (glk = detach_dom_node(ptr_src, LINK_FIRST))
	{
		attach_dom_node(ptr_dst, LINK_LAST, glk);
	}
}

link_t_ptr insert_tool_group(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	ilk = insert_dom_node(ptr, pos);
	set_dom_node_name(ilk, DOC_TOOL_GROUP, -1);

	default_tool_group_attr(ilk);

	return ilk;
}

void delete_tool_group(link_t_ptr glk)
{
	delete_dom_node(glk);
}

int get_tool_group_count(link_t_ptr ptr)
{
	return get_dom_child_node_count(ptr);
}

link_t_ptr get_tool_next_group(link_t_ptr ptr, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_tool_prev_group(link_t_ptr ptr, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_prev_link(pos);
}

link_t_ptr insert_tool_group_item(link_t_ptr glk,link_t_ptr pos)
{
	link_t_ptr ilk;

	ilk = insert_dom_node(glk,pos);
	set_dom_node_name(ilk,DOC_TOOL_ITEM,-1);

	return ilk;
}

void delete_tool_group_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

int get_tool_group_item_count(link_t_ptr glk)
{
	return get_dom_child_node_count(glk);
}

link_t_ptr get_tool_group_item(link_t_ptr glk, const tchar_t* sz_name)
{
	link_t_ptr ilk;

	if (is_null(sz_name))
		return NULL;

	ilk = get_tool_group_next_item(glk, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(sz_name, -1, get_tool_item_name_ptr(ilk), -1, 1) == 0)
			return ilk;
		ilk = get_tool_group_next_item(glk, ilk);
	}

	return NULL;
}

link_t_ptr get_tool_group_next_item(link_t_ptr glk,link_t_ptr pos)
{
	if(pos == LINK_FIRST)
		return get_dom_first_child_node(glk);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_tool_group_prev_item(link_t_ptr glk,link_t_ptr pos)
{
	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_dom_last_child_node(glk);
	else
		return get_prev_link(pos);
}

#endif //XDLDOC