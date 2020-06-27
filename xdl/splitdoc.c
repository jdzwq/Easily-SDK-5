/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc split document

	@module	splitdoc.c | implement file

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

#include "splitdoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_split_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_SPLIT,-1);

	return ptr;
}

void destroy_split_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_split_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_SPLIT,-1,0) == 0)? 1 : 0;
}

void clear_split_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

bool_t is_split_item(link_t_ptr ptr,link_t_ptr ilk)
{
	return is_dom_child_node(ptr,ilk);
}

link_t_ptr enum_split_item(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	return enum_dom_node(ptr, pf, param);
}

void split_item(link_t_ptr ilk, bool_t b_horz)
{
	link_t_ptr tlk;

	if (get_split_item_splited(ilk))
		return;

	if (b_horz)
		set_split_item_layer(ilk, ATTR_LAYER_HORZ);
	else
		set_split_item_layer(ilk, ATTR_LAYER_VERT);

	tlk = insert_dom_node(ilk,LINK_LAST);
	set_dom_node_name(tlk,DOC_SPLIT_ITEM,-1);

	tlk = insert_dom_node(ilk, LINK_LAST);
	set_dom_node_name(tlk, DOC_SPLIT_ITEM, -1);

	set_split_item_splited(ilk, 1);
	set_split_item_fixed(ilk, 0);
	set_split_item_span(ilk, DEF_SPLIT_SPAN);
	set_split_item_ratio(ilk, _T("50%"));
}

void unsplit_item(link_t_ptr ilk)
{
	if (!get_split_item_splited(ilk))
		return;

	delete_dom_child_nodes(ilk);

	set_split_item_splited(ilk, 0);
	set_split_item_fixed(ilk, 0);
	set_split_item_ratio(ilk, 0);
	set_split_item_span(ilk, 0);
}

link_t_ptr get_split_first_child_item(link_t_ptr ilk)
{
	return get_dom_first_child_node(ilk);
}

link_t_ptr get_split_last_child_item(link_t_ptr ilk)
{
	return get_dom_last_child_node(ilk);
}

link_t_ptr get_split_parent_item(link_t_ptr ilk)
{
	return get_dom_parent_node(ilk);
}

link_t_ptr get_split_next_sibling_item(link_t_ptr ilk)
{
	return get_dom_next_sibling_node(ilk);
}

link_t_ptr get_split_prev_sibling_item(link_t_ptr ilk)
{
	return get_dom_prev_sibling_node(ilk);
}

#endif //XDLDOC