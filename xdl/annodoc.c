/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc annotation document

	@module	annodoc.c | implement file

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

#include "annodoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_anno_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_ANNO, -1);

	return ptr;
}

void destroy_anno_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_anno_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

link_t_ptr get_anno_artiset(link_t_ptr ptr)
{
	return ptr;
}

int get_anno_arti_count(link_t_ptr ptr)
{
	return get_dom_child_node_count(get_anno_artiset(ptr));
}

int get_anno_arti_selected_count(link_t_ptr ptr)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_anno_next_arti(ptr, LINK_FIRST);
	while (ilk)
	{
		if (get_anno_arti_selected(ilk))
			count++;
		ilk = get_anno_next_arti(ptr, ilk);
	}

	return count;
}

link_t_ptr insert_anno_arti(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr blk;

	ptr = get_anno_artiset(ptr);
	blk = insert_dom_node(ptr, pos);
	set_dom_node_name(blk, DOC_ANNO_SPOT, -1);

	return blk;
}

void delete_anno_arti(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr get_anno_next_arti(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_anno_artiset(ptr);

	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_anno_prev_arti(link_t_ptr ptr, link_t_ptr pos)
{
	ptr = get_anno_artiset(ptr);

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

link_t_ptr get_anno_arti_at(link_t_ptr ptr,int index)
{
	ptr = get_anno_artiset(ptr);

	return get_dom_child_node_at(ptr,index);
}

bool_t is_anno_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_ANNO,-1,0) == 0)? 1 : 0;
}

bool_t is_anno_arti(link_t_ptr ptr,link_t_ptr plk)
{
	ptr = get_anno_artiset(ptr);

	return is_dom_child_node(ptr, plk);
}

link_t_ptr anno_doc_from_arti(link_t_ptr ilk)
{
	link_t_ptr ptr;

	ptr = ilk;
	do{
		if(is_anno_doc(ptr))
			break;
		ptr = get_dom_parent_node(ptr);
	}while(ptr);

	return ptr;
}

int get_anno_arti_xpoint(link_t_ptr ilk, xpoint_t* ppt, int max)
{
	return ft_parse_points_from_token(ppt, max, get_anno_arti_points_ptr(ilk), -1);
}

void set_anno_arti_xpoint(link_t_ptr ilk, const xpoint_t* ppt, int n)
{
	tchar_t* buf = NULL;
	int len;

	len = ft_format_points_to_token(ppt, n, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	ft_format_points_to_token(ppt, n, buf, len);

	set_anno_arti_points(ilk, buf);
	xsfree(buf);
}

#endif //XDLDOC
