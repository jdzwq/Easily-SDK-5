/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tree document

	@module	treedoc.c | tree document implement file

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

#include "treedoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_tree_attr(link_t_ptr ptr)
{
	set_tree_style(ptr, _T("font-size:10.5;text-align:near;line-align:center;"));
	set_tree_title_height(ptr, 8);

	set_tree_item_height(ptr, DEF_ITEM_HEIGHT);
	set_tree_icon_span(ptr,DEF_ICON_SPAN);
}

link_t_ptr create_tree_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_TREE,-1);
	default_tree_attr(ptr);

	return ptr;
}

void destroy_tree_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_tree_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_TREE,-1,0) == 0)? 1 : 0;
}

void clear_tree_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

bool_t is_tree_item(link_t_ptr ptr,link_t_ptr ilk)
{
	return is_dom_child_node(ptr,ilk);
}

link_t_ptr insert_tree_item(link_t_ptr ilk,link_t_ptr pos)
{
	link_t_ptr tlk;

	tlk = insert_dom_node(ilk,pos);
	set_dom_node_name(tlk,DOC_TREE_ITEM,-1);

	return tlk;
}

void delete_tree_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

void delete_tree_child_items(link_t_ptr ilk)
{
	delete_dom_child_nodes(ilk);
}

int get_tree_item_level(link_t_ptr ilk)
{
	return get_dom_node_level(ilk);
}

int get_tree_child_item_count(link_t_ptr ilk)
{
	return get_dom_child_node_count(ilk);
}

link_t_ptr get_tree_first_child_item(link_t_ptr ilk)
{
	return get_dom_first_child_node(ilk);
}

link_t_ptr get_tree_last_child_item(link_t_ptr ilk)
{
	return get_dom_last_child_node(ilk);
}

link_t_ptr get_tree_parent_item(link_t_ptr ilk)
{
	return get_dom_parent_node(ilk);
}

link_t_ptr get_tree_next_sibling_item(link_t_ptr ilk)
{
	return get_dom_next_sibling_node(ilk);
}

link_t_ptr get_tree_prev_sibling_item(link_t_ptr ilk)
{
	return get_dom_prev_sibling_node(ilk);
}

link_t_ptr get_tree_next_visible_item(link_t_ptr ptr,link_t_ptr pos)
{
	if(pos == LINK_FIRST)
		return get_tree_first_child_item(ptr);
	else if(pos == LINK_LAST)
		return NULL;

	if (get_tree_item_expanded(pos) && get_tree_child_item_count(pos))
		return get_tree_first_child_item(pos);

	while(pos && !get_tree_next_sibling_item(pos))
	{
		pos = get_tree_parent_item(pos);
	}

	return (pos)? get_tree_next_sibling_item(pos) : NULL;
}

link_t_ptr get_tree_prev_visible_item(link_t_ptr ptr,link_t_ptr pos)
{
	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_tree_last_child_item(ptr);

	if(!get_tree_prev_sibling_item(pos))
	{
		pos = get_tree_parent_item(pos);
		if (pos && is_tree_doc(pos))
			return NULL;
		else
			return pos;
	}

	pos = get_tree_prev_sibling_item(pos);
	while (pos && get_tree_item_expanded(pos))
	{
		if (get_tree_child_item_count(pos))
			pos = get_tree_first_child_item(pos);
		else
			break;
	}

	return pos;
}

link_t_ptr find_tree_item_by_name(link_t_ptr ptr, const tchar_t* sz_name)
{
	return find_dom_node_by_attr(ptr, 1, ATTR_NAME, -1, sz_name, -1);
}

link_t_ptr find_tree_item_by_title(link_t_ptr ptr, const tchar_t* sz_title)
{
	return find_dom_node_by_attr(ptr, 1, ATTR_TITLE, -1, sz_title, -1);
}


#endif //XDLDOC