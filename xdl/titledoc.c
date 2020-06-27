/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc title document

	@module	titledoc.c | implement file

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

#include "titledoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_title_attr(link_t_ptr ptr)
{
	set_title_style(ptr, _T("font-size:9;text-align:near;line-align:center;"));

	set_title_item_width(ptr, DEF_TEXT_WIDTH + DEF_GDI_ATTR_GIZMO_SPAN);
	set_title_item_height(ptr, 25);
	set_title_icon_span(ptr, DEF_GDI_ATTR_GIZMO_SPAN);
	set_title_oritation(ptr,ATTR_ORITATION_TOP);
}

link_t_ptr create_title_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_TITLE,-1);
	default_title_attr(ptr);
	return ptr;
}

void destroy_title_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_title_itemset(link_t_ptr ptr)
{
	return ptr;
}

bool_t is_title_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_TITLE,-1,0) == 0)? 1 : 0;
}

bool_t is_title_item(link_t_ptr ptr, link_t_ptr ilk)
{
	return is_dom_child_node(ptr, ilk);
}

void clear_title_doc(link_t_ptr ptr)
{
	ptr = get_title_itemset(ptr);

	delete_dom_child_nodes(ptr);
}

link_t_ptr insert_title_item(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr ilk;

	ptr = get_title_itemset(ptr);

	ilk = insert_dom_node(ptr,pos);
	set_dom_node_name(ilk,DOC_TITLE_ITEM,-1);

	return ilk;
}

void delete_title_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

int get_title_item_count(link_t_ptr ptr)
{
	ptr = get_title_itemset(ptr);

	return get_dom_child_node_count(ptr);
}

link_t_ptr get_title_next_item(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_title_itemset(ptr);

	if(pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_title_prev_item(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_title_itemset(ptr);

	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_prev_link(pos);
}

link_t_ptr get_title_item(link_t_ptr ptr,const tchar_t* cname)
{
	link_t_ptr tlk;

	tlk = get_title_next_item(ptr,LINK_FIRST);
	while(tlk)
	{
		if(xscmp(get_title_item_name_ptr(tlk),cname) == 0)
			return tlk;
		tlk = get_title_next_item(ptr,tlk);
	}
	return NULL;
}

link_t_ptr get_title_item_by_id(link_t_ptr ptr, const tchar_t* cid)
{
	link_t_ptr tlk;

	tlk = get_title_next_item(ptr, LINK_FIRST);
	while (tlk)
	{
		if (xscmp(get_title_item_id_ptr(tlk), cid) == 0)
			return tlk;
		tlk = get_title_next_item(ptr, tlk);
	}
	return NULL;
}

#endif //XDLDOC