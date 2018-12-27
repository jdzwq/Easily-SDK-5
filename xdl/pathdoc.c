/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc path document

	@module	pathdoc.c | path document implement file

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

#include "pathdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_path_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_PATH, -1);

	return ptr;
}

void destroy_path_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_path_nodeset(link_t_ptr ptr)
{
	return ptr;
}

bool_t is_path_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_PATH, -1, 0) == 0) ? 1 : 0;
}

void clear_path_doc(link_t_ptr ptr)
{
	ptr = get_path_nodeset(ptr);

	delete_dom_child_nodes(ptr);
}

int get_path_node_count(link_t_ptr ptr)
{
	ptr = get_path_nodeset(ptr);

	return get_dom_child_node_count(ptr);
}

link_t_ptr insert_path_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr nlk;

	ptr = get_path_nodeset(ptr);

	nlk = insert_dom_node(ptr, pos);

	return nlk;
}

void delete_path_node(link_t_ptr nlk)
{
	delete_dom_node(nlk);
}

link_t_ptr get_path_next_node(link_t_ptr ptr, link_t_ptr pos)
{
	ptr = get_path_nodeset(ptr);

	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_path_prev_node(link_t_ptr ptr, link_t_ptr pos)
{
	ptr = get_path_nodeset(ptr);

	if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else if (pos == LINK_FIRST)
		return NULL;
	else
		return get_dom_prev_sibling_node(pos);
}

void set_path_move_node(link_t_ptr nlk, const xpoint_t* ppt)
{
	set_path_node_name(nlk, DOC_PATH_MOVE);

	set_dom_node_attr_integer(nlk, ATTR_X, ppt->x);
	set_dom_node_attr_integer(nlk, ATTR_Y, ppt->y);
}

void get_path_move_node(link_t_ptr nlk, xpoint_t* ppt)
{
	if (compare_text(get_path_node_name_ptr(nlk), -1, DOC_PATH_MOVE, -1, 0) != 0)
		return;

	ppt->x = get_dom_node_attr_integer(nlk, ATTR_X);
	ppt->y = get_dom_node_attr_integer(nlk, ATTR_Y);
}

void set_path_line_node(link_t_ptr nlk, const xpoint_t* ppt)
{
	set_path_node_name(nlk, DOC_PATH_LINE);

	set_dom_node_attr_integer(nlk, ATTR_X, ppt->x);
	set_dom_node_attr_integer(nlk, ATTR_Y, ppt->y);
}

void get_path_line_node(link_t_ptr nlk, xpoint_t* ppt)
{
	if (compare_text(get_path_node_name_ptr(nlk), -1, DOC_PATH_LINE, -1, 0) != 0)
		return;

	ppt->x = get_dom_node_attr_integer(nlk, ATTR_X);
	ppt->y = get_dom_node_attr_integer(nlk, ATTR_Y);
}

void set_path_arc_node(link_t_ptr nlk, const xsize_t* pps, bool_t ccw, const xpoint_t* ppt)
{
	set_path_node_name(nlk, DOC_PATH_ARC);

	set_dom_node_attr_integer(nlk, ATTR_RX, pps->cx);
	set_dom_node_attr_integer(nlk, ATTR_RY, pps->cy);

	set_dom_node_attr_boolean(nlk, ATTR_CLOCKWISE, ccw);

	set_dom_node_attr_integer(nlk, ATTR_X, ppt->x);
	set_dom_node_attr_integer(nlk, ATTR_Y, ppt->y);
}

void get_path_arc_node(link_t_ptr nlk, xsize_t* pps, bool_t* pccw, xpoint_t* ppt)
{
	if (compare_text(get_path_node_name_ptr(nlk), -1, DOC_PATH_ARC, -1, 0) != 0)
		return;

	pps->cx = get_dom_node_attr_integer(nlk, ATTR_RX);
	pps->cy = get_dom_node_attr_integer(nlk, ATTR_RY);

	*pccw = get_dom_node_attr_boolean(nlk, ATTR_CLOCKWISE);

	ppt->x = get_dom_node_attr_integer(nlk, ATTR_X);
	ppt->y = get_dom_node_attr_integer(nlk, ATTR_Y);
}

void set_path_bezier_node(link_t_ptr nlk, const xpoint_t* ppt, int n)
{
	tchar_t* token;
	int i, len, total = 0;;

	set_path_node_name(nlk, DOC_PATH_BEZIER);

	token = (tchar_t*)xmem_alloc(INT_LEN * n);
	for (i = 0; i < n; i++)
	{
		len = xsprintf(token + total, _T("%d,%d "), ppt[i].x, ppt[i].y);
		total += len;
	}

	set_dom_node_attr(nlk, ATTR_COORDINATE, -1, token, total);

	xmem_free(token);
}

int get_path_bezier_node(link_t_ptr nlk, xpoint_t* ppt)
{
	const tchar_t *token, *xtk, *ytk;
	int xlen, ylen;
	int i = 0;

	if (compare_text(get_path_node_name_ptr(nlk), -1, DOC_PATH_BEZIER, -1, 0) != 0)
		return 0;

	token = get_dom_node_attr_ptr(nlk, ATTR_COORDINATE, -1);
	while (*token)
	{
		xtk = token;
		xlen = 0;
		while (*token != _T(',') && *token != _T('\0'))
		{
			token++;
			xlen++;
		}

		if (*token == _T(','))
			token++;

		ytk = token;
		ylen = 0;
		while (*token != _T(' ') && *token != _T('\0'))
		{
			token++;
			ylen++;
		}

		if (*token == _T(' '))
			token++;

		if (ppt)
		{
			ppt[i].x = xsntol(xtk, xlen);
			ppt[i].y = xsntol(ytk, ylen);
		}

		i++;
	}

	return i;
}

#endif //XDLDOC