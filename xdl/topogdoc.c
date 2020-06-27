/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc topog document

	@module	topogdoc.c | implement file

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

#include "topogdoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_topog_attr(link_t_ptr ptr) 
{
	set_topog_rows(ptr, 100);
	set_topog_cols(ptr, 60);

	set_topog_rx(ptr, 5);
	set_topog_ry(ptr, 5);
}

void default_topog_spot_attr(link_t_ptr ilk)
{
	set_topog_spot_style(ilk, _T("font-size:10.5;font-weight:500;text-wrap:line-break;"));

	set_topog_spot_type(ilk, ATTR_SPOT_TYPE_ICON);
	//set_topog_spot_title(ilk, GDI_ATTR_GIZMO_STAR);
}

link_t_ptr create_topog_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_TOPOG,-1);
	default_topog_attr(ptr);

	return ptr;
}

void destroy_topog_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_topog_spotset(link_t_ptr ptr)
{
	return ptr;
}

void clear_topog_doc(link_t_ptr ptr)
{
	ptr = get_topog_spotset(ptr);

	delete_dom_child_nodes(ptr);
}

int get_topog_spot_count(link_t_ptr ptr)
{
	ptr = get_topog_spotset(ptr);

	return get_dom_child_node_count(ptr);
}

int get_topog_spot_selected_count(link_t_ptr ptr)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_topog_next_spot(ptr, LINK_FIRST);
	while (ilk)
	{
		if (get_topog_spot_selected(ilk))
			count++;
		ilk = get_topog_next_spot(ptr, ilk);
	}

	return count;
}

link_t_ptr insert_topog_spot(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr blk;

	ptr = get_topog_spotset(ptr);

	blk = insert_dom_node(ptr, pos);
	set_dom_node_name(blk, DOC_TOPOG_SPOT, -1);

	default_topog_spot_attr(blk);

	return blk;
}

void delete_topog_spot(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr get_topog_next_spot(link_t_ptr ptr,link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_topog_prev_spot(link_t_ptr ptr, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

link_t_ptr get_topog_spot_at(link_t_ptr ptr,int index)
{
	ptr = get_topog_spotset(ptr);

	return get_dom_child_node_at(ptr,index);
}

link_t_ptr get_topog_spot(link_t_ptr ptr, const tchar_t* cname)
{
	link_t_ptr tlk;

	tlk = get_topog_next_spot(ptr, LINK_FIRST);
	while (tlk)
	{
		if (xscmp(get_topog_spot_name_ptr(tlk), cname) == 0)
			return tlk;

		tlk = get_topog_next_spot(ptr, tlk);
	}

	return NULL;
}

bool_t is_topog_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_TOPOG,-1,0) == 0)? 1 : 0;
}

bool_t is_topog_spot(link_t_ptr ptr,link_t_ptr plk)
{
	return is_dom_child_node(ptr, plk);
}

link_t_ptr topog_doc_from_spot(link_t_ptr ilk)
{
	link_t_ptr ptr;

	ptr = ilk;
	do{
		if(is_topog_doc(ptr))
			break;
		ptr = get_dom_parent_node(ptr);
	}while(ptr);

	return ptr;
}

bool_t set_topog_matrix_value(link_t_ptr ptr, int row, int col, double v)
{
	int rows, cols;
	matrix_t* mt;
	int len;
	tchar_t* buf;

	rows = get_topog_rows(ptr);
	cols = get_topog_cols(ptr);

	if (row < 0 || row >= rows)
		return 0;

	if (col < 0 || col >= cols)
		return 0;

	mt = matrix_alloc(rows, cols);

	matrix_parse(mt, get_topog_matrix_ptr(ptr), -1);

	matrix_set_value(mt, row, col, v);

	len = matrix_format(mt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	len = matrix_format(mt, buf, len);

	matrix_free(mt);

	set_topog_matrix(ptr, buf, len);
	xsfree(buf);

	return 1;
}

double get_topog_matrix_value(link_t_ptr ptr, int row, int col)
{
	int rows, cols;
	matrix_t* mt;
	double v;

	rows = get_topog_rows(ptr);
	cols = get_topog_cols(ptr);

	if (row < 0 || row >= rows)
		return 0;

	if (col < 0 || col >= cols)
		return 0;

	mt = matrix_alloc(rows, cols);

	matrix_parse(mt, get_topog_matrix_ptr(ptr), -1);

	v = matrix_get_value(mt, row, col);

	matrix_free(mt);

	return v;
}

#endif //XDLDOC
