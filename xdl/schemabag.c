/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc schema bag document

	@module	schemabag.c | implement file

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

#include "schemabag.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_DOC

void export_grid_schema(link_t_ptr ptr,link_t_ptr sch)
{
	link_t_ptr clk,nlk_rowset,nlk_row,nlk;
	const tchar_t *cname,*cid,*type;
	int len, dig;

	delete_dom_child_nodes(sch);

	nlk_row = insert_schema_declare(sch, LINK_LAST);
	set_schema_element_name(nlk_row, DOC_GRID_ROW);
	set_schema_element_id(nlk_row, DOC_GRID_ROW);
	set_schema_element_type(nlk_row, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		cname = get_col_name_ptr(clk);
		if(is_null(cname))
		{
			clk = get_next_col(ptr,clk);
			continue;
		}

		cid = get_col_id_ptr(clk);
		len = get_col_data_len(clk);
		dig = get_col_data_dig(clk);

		nlk = insert_schema_element(nlk_row, LINK_LAST);
		if (!is_null(cid))
			set_schema_element_name(nlk, cid);
		else
			set_schema_element_name(nlk,cname);

		set_schema_element_id(nlk, cname);

		type = get_col_data_type_ptr(clk);
		if(compare_text(type,-1,ATTR_DATA_TYPE_STRING,-1,0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_STRING);
			if (len)
				set_schema_element_precision(nlk, len);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_BOOLEAN);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_INTEGER);
		}else if(compare_text(type,-1,ATTR_DATA_TYPE_NUMERIC,-1,0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_DECIMAL);
			if (len)
				set_schema_element_precision(nlk, len);
			if (dig)
				set_schema_element_scale(nlk, dig);
		}else if(compare_text(type,-1,ATTR_DATA_TYPE_DATE,-1,0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_DATE);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_DATETIME);
		}

		clk = get_next_col(ptr,clk);
	}

	nlk_rowset = insert_schema_element(sch, LINK_LAST);
	set_schema_element_name(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_id(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_type(nlk_rowset, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	nlk_row = insert_schema_element(nlk_rowset, LINK_LAST);
	set_schema_element_ref(nlk_row, DOC_GRID_ROW);
}

void import_grid_schema(link_t_ptr ptr, link_t_ptr sch)
{
	link_t_ptr clk;
	link_t_ptr slk_rowset,slk_row,slk_ref,slk;
	const tchar_t* type;
	int prec, scale;

	clear_grid_rowset(ptr);
	clear_grid_colset(ptr);

	slk_rowset = get_schema_next_element(sch, LINK_FIRST);
	while (slk_rowset)
	{
		if (is_schema_element(slk_rowset) && compare_text(get_schema_element_id_ptr(slk_rowset), -1, DOC_GRID_ROWSET, -1, 0) == 0)
			break;

		slk_rowset = get_schema_next_element(sch, slk_rowset);
	}

	if (!slk_rowset)
		return;

	slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
	while (slk_row)
	{
		if (is_null(get_schema_element_name_ptr(slk_row)))
		{
			slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
			if (slk_ref && compare_text(get_schema_element_id_ptr(slk_ref), -1, DOC_GRID_ROW, -1, 0) == 0)
			{
				slk_row = slk_ref;
				break;
			}
		}
		else
		{
			if (compare_text(get_schema_element_id_ptr(slk_row), -1, DOC_GRID_ROW, -1, 0) == 0)
			{
				break;
			}
		}

		slk_row = get_schema_next_element(slk_rowset, slk_row);
	}

	if (!slk_row)
		return;

	slk = get_schema_next_element(slk_row, LINK_FIRST);
	while (slk)
	{
		prec = get_schema_element_precision(slk);
		scale = get_schema_element_scale(slk);

		clk = insert_col(ptr, LINK_LAST);
		set_col_name(clk, get_schema_element_id_ptr(slk));
		set_col_id(clk, get_schema_element_name_ptr(slk));
		set_col_title(clk, get_schema_element_id_ptr(slk));

		type = get_schema_element_type_ptr(slk);
		if (compare_text(type, -1, SCHEMA_ATTR_TYPE_STRING, -1, 0) == 0)
		{
			set_col_data_type(clk, ATTR_DATA_TYPE_STRING);
			if (prec)
				set_col_data_len(clk, prec);
		}
		else if (compare_text(type, -1, SCHEMA_ATTR_TYPE_BOOLEAN, -1, 0) == 0)
		{
			set_col_data_type(clk, ATTR_DATA_TYPE_BOOLEAN);
		}
		else if (compare_text(type, -1, SCHEMA_ATTR_TYPE_INTEGER, -1, 0) == 0)
		{
			set_col_data_type(clk, ATTR_DATA_TYPE_INTEGER);
		}
		else if (compare_text(type, -1, SCHEMA_ATTR_TYPE_DECIMAL, -1, 0) == 0)
		{
			set_col_data_type(clk, ATTR_DATA_TYPE_NUMERIC);
			if (prec)
				set_col_data_len(clk, prec);
			if (scale)
				set_col_data_dig(clk, scale);
		}
		else if (compare_text(type, -1, SCHEMA_ATTR_TYPE_DATE, -1, 0) == 0)
		{
			set_col_data_type(clk, ATTR_DATA_TYPE_DATE);
		}
		else if (compare_text(type, -1, SCHEMA_ATTR_TYPE_DATETIME, -1, 0) == 0)
		{
			set_col_data_type(clk, ATTR_DATA_TYPE_DATETIME);
		}
		else 
		{
			set_col_data_type(clk, ATTR_DATA_TYPE_STRING);
		}

		slk = get_schema_next_element(slk_row,slk);
	}
}

void export_grid_data(link_t_ptr grid, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset,slk_row,slk_ref,slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr clk, rlk;
	bool_t b_sch;

	b_sch = (is_empty_schema(sch))? 0 : 1;

	delete_dom_child_nodes(dom);
	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;

	if (b_sch)
	{
		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;
	}

	if (b_sch)
		set_dom_node_name(nlk_rowset, get_schema_element_name_ptr(slk_rowset), -1);
	else
		set_dom_node_name(nlk_rowset, DOC_GRID_ROWSET, -1);

	if (b_sch)
	{
		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}

	rlk = get_next_row(grid, LINK_FIRST);
	while (rlk)
	{
		nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);

		if(b_sch)
			set_dom_node_name(nlk_row, get_schema_element_name_ptr(slk_row), -1);
		else
			set_dom_node_name(nlk_row, DOC_GRID_ROW, -1);

		if(b_sch)
		{
			slk = get_schema_next_element(slk_row, LINK_FIRST);
			while (slk)
			{
				if (is_schema_element(slk))
				{
					nlk = insert_dom_node(nlk_row, LINK_LAST);
					set_dom_node_name(nlk, get_schema_element_name_ptr(slk), -1);

					clk = get_col(grid, get_schema_element_id_ptr(slk));
					if (clk)
					{
						set_dom_node_text(nlk, get_cell_text_ptr(rlk, clk), -1);
					}
				}
				else if (is_schema_attribute(slk))
				{
					clk = get_col(grid, get_schema_element_id_ptr(slk));
					if (clk)
					{
						set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, get_cell_text_ptr(rlk, clk), -1);
					}
					else
					{
						set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, NULL, 0);
					}
				}

				slk = get_schema_next_element(slk_row, slk);
			}
		}
		else
		{
			clk = get_next_col(grid, LINK_FIRST);
			while (clk)
			{
				if (!is_null(get_col_id_ptr(clk)))
				{
					nlk = insert_dom_node(nlk_row, LINK_LAST);
					set_dom_node_name(nlk, get_col_id_ptr(clk), -1);
					set_dom_node_text(nlk, get_cell_text_ptr(rlk, clk), -1);
				}

				clk = get_next_col(grid, clk);
			}
		}

		rlk = get_next_row(grid, rlk);
	}
}

void import_grid_data(link_t_ptr grid, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset,slk_row,slk_ref,slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr clk, rlk;
	bool_t b_sch;

	b_sch = (is_empty_schema(sch)) ? 0 : 1;

	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;

	if(b_sch)
	{
		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;

		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, get_schema_element_name_ptr(slk_rowset), -1, 1) != 0)
			return;

		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}
	else
	{
		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, DOC_GRID_ROWSET, -1, 1) != 0)
			return;
	}

	nlk_row = get_dom_first_child_node(nlk_rowset);
	while (nlk_row)
	{
		if(b_sch)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, get_schema_element_name_ptr(slk_row), -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}
		}
		else
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, DOC_GRID_ROW, -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}
		}

		rlk = insert_row(grid, LINK_LAST);

		if(b_sch)
		{
			slk = get_schema_next_element(slk_row, LINK_FIRST);
			while (slk)
			{
				clk = get_col(grid, get_schema_element_id_ptr(slk));
				if (!clk)
				{
					slk = get_schema_next_element(slk_row, slk);
					continue;
				}

				if (is_schema_element(slk))
				{
					nlk = find_dom_node_by_name(nlk_row, 0, get_schema_element_name_ptr(slk), -1);
					if (nlk)
					{
						set_cell_text(rlk, clk, get_dom_node_text_ptr(nlk), -1);
					}
				}
				else if (is_schema_attribute(slk))
				{
					set_cell_text(rlk, clk, get_dom_node_attr_ptr(nlk_row, get_schema_element_name_ptr(slk), -1), -1);
				}

				slk = get_schema_next_element(slk_row, slk);
			}
		}
		else
		{
			nlk = get_dom_first_child_node(nlk_row);
			while (nlk)
			{
				clk = get_col_by_id(grid, get_dom_node_name_ptr(nlk));
				if (!clk)
				{
					nlk = get_dom_next_sibling_node(nlk);
					continue;
				}

				set_cell_text(rlk, clk, get_dom_node_text_ptr(nlk), -1);

				nlk = get_dom_next_sibling_node(nlk);
			}
		}

		nlk_row = get_dom_next_sibling_node(nlk_row);
	}
}

/****************************************************************************************************************/
void export_statis_schema(link_t_ptr ptr, link_t_ptr sch)
{
	link_t_ptr ylk, nlk_rowset, nlk_row, nlk;
	const tchar_t *cname,*cid;
	int len, dig;

	delete_dom_child_nodes(sch);

	nlk_row = insert_schema_declare(sch, LINK_LAST);
	set_schema_element_name(nlk_row, DOC_GRID_ROW);
	set_schema_element_id(nlk_row, DOC_GRID_ROW);
	set_schema_element_type(nlk_row, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	cname = get_statis_xaxis_ptr(ptr);
	if (!is_null(cname))
	{
		nlk = insert_schema_element(nlk_row, LINK_LAST);
		set_schema_element_name(nlk, cname);
		set_schema_element_id(nlk, cname);
		set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_STRING);
	}

	ylk = get_next_yax(ptr, LINK_FIRST);
	while (ylk)
	{
		cname = get_yax_name_ptr(ylk);
		if (is_null(cname))
		{
			ylk = get_next_yax(ptr, ylk);
			continue;
		}

		cid = get_yax_id_ptr(ylk);
		len = get_yax_data_len(ylk);
		dig = get_yax_data_dig(ylk);

		nlk = insert_schema_element(nlk_row, LINK_LAST);
		if (!is_null(cid))
			set_schema_element_name(nlk, cid);
		else
			set_schema_element_name(nlk, cname);

		set_schema_element_id(nlk, cname);

		set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_DECIMAL);
		if (len)
			set_schema_element_precision(nlk, len);
		if (dig)
			set_schema_element_scale(nlk, dig);

		ylk = get_next_yax(ptr, ylk);
	}

	nlk_rowset = insert_schema_element(sch, LINK_LAST);
	set_schema_element_name(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_id(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_type(nlk_rowset, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	nlk_row = insert_schema_element(nlk_rowset, LINK_LAST);
	set_schema_element_ref(nlk_row, DOC_GRID_ROW);
}

void export_statis_data(link_t_ptr statis, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr ylk, xlk;
	bool_t b_sch;

	b_sch = (is_empty_schema(sch)) ? 0 : 1;

	delete_dom_child_nodes(dom);
	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;
	if(b_sch)
	{
		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;
	}

	if(b_sch)
		set_dom_node_name(nlk_rowset, get_schema_element_name_ptr(slk_rowset), -1);
	else
		set_dom_node_name(nlk_rowset, DOC_GRID_ROWSET, -1);

	if(b_sch)
	{
		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}

	xlk = get_next_xax(statis, LINK_FIRST);
	while (xlk)
	{
		nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);

		if(b_sch)
			set_dom_node_name(nlk_row, get_schema_element_name_ptr(slk_row), -1);
		else
			set_dom_node_name(nlk_row, DOC_GRID_ROW, -1);

		if(b_sch)
		{
			slk = get_schema_next_element(slk_row, LINK_FIRST);
			while (slk)
			{
				if (is_schema_element(slk))
				{
					nlk = insert_dom_node(nlk_row, LINK_LAST);
					set_dom_node_name(nlk, get_schema_element_name_ptr(slk), -1);

					ylk = get_yax(statis, get_schema_element_id_ptr(slk));
					if (ylk)
					{
						set_dom_node_text(nlk, get_coor_text_ptr(xlk, ylk), -1);
					}
				}
				else if (is_schema_attribute(slk))
				{
					ylk = get_yax(statis, get_schema_element_id_ptr(slk));
					if (ylk)
					{
						set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, get_coor_text_ptr(xlk, ylk), -1);
					}
					else
					{
						set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, NULL, 0);
					}
				}

				slk = get_schema_next_element(slk_row, slk);
			}
		}
		else
		{
			ylk = get_next_yax(statis, LINK_FIRST);
			while (ylk)
			{
				if (!is_null(get_yax_id_ptr(ylk)))
				{
					nlk = insert_dom_node(nlk_row, LINK_LAST);
					set_dom_node_name(nlk, get_yax_id_ptr(ylk), -1);
					set_dom_node_text(nlk, get_coor_text_ptr(xlk, ylk), -1);
				}

				ylk = get_next_yax(statis, ylk);
			}
		}

		xlk = get_next_xax(statis, xlk);
	}
}

void import_statis_data(link_t_ptr statis, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr xlk, ylk;
	bool_t b_sch;

	b_sch = (is_empty_schema(sch)) ? 0 : 1;

	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;

	if(b_sch)
	{
		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;

		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, get_schema_element_name_ptr(slk_rowset), -1, 1) != 0)
			return;

		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}
	else
	{
		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, DOC_GRID_ROWSET, -1, 1) != 0)
			return;
	}

	nlk_row = get_dom_first_child_node(nlk_rowset);
	while (nlk_row)
	{
		if(b_sch)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, get_schema_element_name_ptr(slk_row), -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}
		}
		else
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, DOC_GRID_ROW, -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}
		}

		xlk = insert_xax(statis, LINK_LAST);

		if(b_sch)
		{
			slk = get_schema_next_element(slk_row, LINK_FIRST);
			while (slk)
			{
				ylk = get_yax(statis, get_schema_element_id_ptr(slk));
				if (!ylk)
				{
					slk = get_schema_next_element(slk_row, slk);
					continue;
				}

				if (is_schema_element(slk))
				{
					nlk = find_dom_node_by_name(nlk_row, 0, get_schema_element_name_ptr(slk), -1);
					if (nlk)
					{
						set_coor_text(xlk, ylk, get_dom_node_text_ptr(nlk), -1);
					}
				}
				else if (is_schema_attribute(slk))
				{
					set_coor_text(xlk, ylk, get_dom_node_attr_ptr(nlk_row, get_schema_element_name_ptr(slk), -1), -1);
				}

				slk = get_schema_next_element(slk_row, slk);
			}
		}
		else
		{
			nlk = get_dom_first_child_node(nlk_row);
			while (nlk)
			{
				ylk = get_yax_by_id(statis, get_dom_node_name_ptr(nlk));
				if (!ylk)
				{
					nlk = get_dom_next_sibling_node(nlk);
					continue;
				}

				set_coor_text(xlk, ylk, get_dom_node_text_ptr(nlk), -1);

				nlk = get_dom_next_sibling_node(nlk);
			}
		}

		nlk_row = get_dom_next_sibling_node(nlk_row);
	}
}

/******************************************************************************************************************/
void export_form_schema(link_t_ptr ptr, link_t_ptr sch)
{
	link_t_ptr flk, nlk_rowset, nlk_row, nlk;
	const tchar_t *cname,*cid, *type;
	int len, dig;

	delete_dom_child_nodes(sch);

	nlk_row = insert_schema_declare(sch, LINK_LAST);
	set_schema_element_name(nlk_row, DOC_GRID_ROW);
	set_schema_element_id(nlk_row, DOC_GRID_ROW);
	set_schema_element_type(nlk_row, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	flk = get_next_field(ptr, LINK_FIRST);
	while (flk)
	{
		if (!IS_DATA_FIELD(get_field_class_ptr(flk)))
		{
			flk = get_next_field(ptr, flk);
			continue;
		}

		cname = get_field_name_ptr(flk);
		if (is_null(cname))
		{
			flk = get_next_field(ptr, flk);
			continue;
		}

		cid = get_field_id_ptr(flk);
		len = get_field_data_len(flk);
		dig = get_field_data_dig(flk);

		nlk = insert_schema_element(nlk_row, LINK_LAST);
		if (!is_null(cid))
			set_schema_element_name(nlk, cid);
		else
			set_schema_element_name(nlk, cname);

		set_schema_element_id(nlk, cname);

		type = get_field_data_type_ptr(flk);

		if (compare_text(type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_STRING);
			if (len)
				set_schema_element_precision(nlk, len);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_BOOLEAN);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_INTEGER);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_DECIMAL);
			if (len)
				set_schema_element_precision(nlk, len);
			if (dig)
				set_schema_element_scale(nlk, dig);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_DATE);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_DATETIME);
		}
		else
		{
			set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_STRING);
		}

		flk = get_next_field(ptr, flk);
	}

	nlk_rowset = insert_schema_element(sch, LINK_LAST);
	set_schema_element_name(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_id(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_type(nlk_rowset, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	nlk_row = insert_schema_element(nlk_rowset, LINK_LAST);
	set_schema_element_ref(nlk_row, DOC_GRID_ROW);
}

void export_form_data(link_t_ptr form, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr flk;
	bool_t b_sch;

	b_sch = (is_empty_schema(sch)) ? 0 : 1;

	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;

	if(b_sch)
	{
		delete_dom_child_nodes(nlk_rowset);

		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;
	}

	if(b_sch)
		set_dom_node_name(nlk_rowset, get_schema_element_name_ptr(slk_rowset), -1);
	else
		set_dom_node_name(nlk_rowset, DOC_GRID_ROWSET, -1);

	if(b_sch)
	{
		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}

	nlk_row = get_dom_first_child_node(nlk_rowset);
	if (!nlk_row)
	{
		nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
	}

	if(b_sch)
	{
		set_dom_node_name(nlk_row, get_schema_element_name_ptr(slk_row), -1);

		slk = get_schema_next_element(slk_row, LINK_FIRST);
		while (slk)
		{
			if (is_schema_element(slk))
			{
				nlk = insert_dom_node(nlk_row, LINK_LAST);
				set_dom_node_name(nlk, get_schema_element_name_ptr(slk), -1);

				flk = get_field(form, get_schema_element_id_ptr(slk));

				if (flk && IS_DATA_FIELD(get_field_class_ptr(flk)))
				{
					set_dom_node_text(nlk, get_field_text_ptr(flk), -1);
				}
			}
			else if (is_schema_attribute(slk))
			{
				flk = get_field(form, get_schema_element_id_ptr(slk));

				if (flk && IS_DATA_FIELD(get_field_class_ptr(flk)))
				{
					set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, get_field_text_ptr(flk), -1);
				}
				else
				{
					set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, NULL, 0);
				}
			}

			slk = get_schema_next_element(slk_row, slk);
		}
	}
	else
	{
		set_dom_node_name(nlk_row, DOC_GRID_ROW, -1);

		flk = get_next_field(form, LINK_FIRST);
		while (flk)
		{
			if (is_null(get_field_id_ptr(flk)))
			{
				flk = get_next_field(form, flk);
				continue;
			}

			nlk = find_dom_node_by_name(nlk_row, 0, get_field_id_ptr(flk), -1);
			if (!nlk)
			{
				nlk = insert_dom_node(nlk_row, LINK_LAST);
				set_dom_node_name(nlk, get_field_id_ptr(flk), -1);
			}
			set_dom_node_text(nlk, get_field_text_ptr(flk), -1);

			flk = get_next_field(form, flk);
		}
	}
}

void import_form_data(link_t_ptr form, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr flk;
	bool_t b_sch;

	b_sch = (is_empty_schema(sch)) ? 0 : 1;

	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;

	if(b_sch)
	{
		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;

		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, get_schema_element_name_ptr(slk_rowset), -1, 1) != 0)
			return;

		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}
	else
	{
		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, DOC_GRID_ROWSET, -1, 1) != 0)
			return;
	}

	nlk_row = get_dom_first_child_node(nlk_rowset);
	while (nlk_row)
	{
		if(b_sch)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, get_schema_element_name_ptr(slk_row), -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}

			slk = get_schema_next_element(slk_row, LINK_FIRST);
			while (slk)
			{
				flk = get_field(form, get_schema_element_id_ptr(slk));
				if (!flk)
				{
					slk = get_schema_next_element(slk_row, slk);
					continue;
				}

				if (!IS_DATA_FIELD(get_field_class_ptr(flk)))
				{
					slk = get_schema_next_element(slk_row, slk);
					continue;
				}

				if (is_schema_element(slk))
				{
					nlk = find_dom_node_by_name(nlk_row, 0, get_schema_element_name_ptr(slk), -1);
					if (nlk)
					{
						set_field_text(flk, get_dom_node_text_ptr(nlk), -1);
					}
				}
				else if (is_schema_attribute(slk))
				{
					set_field_text(flk, get_dom_node_attr_ptr(nlk_row, get_schema_element_name_ptr(slk), -1), -1);
				}

				slk = get_schema_next_element(slk_row, slk);
			}
		}
		else
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, DOC_GRID_ROW, -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}

			nlk = get_dom_first_child_node(nlk_row);
			while (nlk)
			{
				flk = get_field_by_id(form, get_dom_node_name_ptr(nlk));
				if (!flk)
				{
					nlk = get_dom_next_sibling_node(nlk);
					continue;
				}

				set_field_text(flk, get_dom_node_text_ptr(nlk), -1);

				nlk = get_dom_next_sibling_node(nlk);
			}
		}

		nlk_row = get_dom_next_sibling_node(nlk_row);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void export_rich_schema(link_t_ptr ptr, link_t_ptr sch)
{
	link_t_ptr plk, nlk_rowset, nlk_row, nlk;
	const tchar_t *cname;

	delete_dom_child_nodes(sch);

	nlk_row = insert_schema_declare(sch, LINK_LAST);
	set_schema_element_name(nlk_row, DOC_GRID_ROW);
	set_schema_element_id(nlk_row, DOC_GRID_ROW);
	set_schema_element_type(nlk_row, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	plk = get_rich_next_anch(ptr, LINK_FIRST);
	while (plk)
	{
		cname = get_rich_anch_name_ptr(plk);
		if (is_null(cname))
		{
			plk = get_rich_next_anch(ptr, plk);
			continue;
		}

		nlk = insert_schema_element(nlk_row, LINK_LAST);
		set_schema_element_name(nlk, cname);
		set_schema_element_id(nlk, cname);
		set_schema_element_type(nlk, SCHEMA_ATTR_TYPE_STRING);
		
		plk = get_rich_next_anch(ptr, plk);
	}

	nlk_rowset = insert_schema_element(sch, LINK_LAST);
	set_schema_element_name(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_id(nlk_rowset, DOC_GRID_ROWSET);
	set_schema_element_type(nlk_rowset, SCHEMA_ATTR_TYPE_COMPLEXTYPE);

	nlk_row = insert_schema_element(nlk_rowset, LINK_LAST);
	set_schema_element_ref(nlk_row, DOC_GRID_ROW);
}

void export_rich_data(link_t_ptr rich, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr plk;
	bool_t b_sch;

	b_sch = (is_empty_schema(sch)) ? 0 : 1;

	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;

	if(b_sch)
	{
		delete_dom_child_nodes(nlk_rowset);

		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;
	}

	if(b_sch)
		set_dom_node_name(nlk_rowset, get_schema_element_name_ptr(slk_rowset), -1);
	else
		set_dom_node_name(nlk_rowset, DOC_GRID_ROWSET, -1);

	if(b_sch)
	{
		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}

	nlk_row = get_dom_first_child_node(nlk_rowset);
	if (!nlk_row)
	{
		nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
	}

	if(b_sch)
	{
		set_dom_node_name(nlk_row, get_schema_element_name_ptr(slk_row), -1);

		slk = get_schema_next_element(slk_row, LINK_FIRST);
		while (slk)
		{
			if (is_schema_element(slk))
			{
				nlk = insert_dom_node(nlk_row, LINK_LAST);
				set_dom_node_name(nlk, get_schema_element_name_ptr(slk), -1);

				plk = get_rich_anch(rich, get_schema_element_id_ptr(slk));

				if (plk)
				{
					set_dom_node_text(nlk, get_rich_anch_text_ptr(plk), -1);
					set_dom_node_attr(nlk, ATTR_TITLE, -1, get_rich_anch_title_ptr(plk), -1);
				}
			}
			else if (is_schema_attribute(slk))
			{
				plk = get_rich_anch(rich, get_schema_element_id_ptr(slk));

				if (plk)
				{
					set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, get_rich_anch_text_ptr(plk), -1);
					set_dom_node_attr(nlk_row, ATTR_TITLE, -1, get_rich_anch_title_ptr(plk), -1);
				}
				else
				{
					set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, NULL, 0);
				}
			}

			slk = get_schema_next_element(slk_row, slk);
		}
	}
	else
	{
		set_dom_node_name(nlk_row, DOC_GRID_ROW, -1);

		plk = get_rich_next_anch(rich, LINK_FIRST);
		while (plk)
		{
			if (is_null(get_rich_anch_name_ptr(plk)))
			{
				plk = get_rich_next_anch(rich, plk);
				continue;
			}

			nlk = find_dom_node_by_name(nlk_row, 0, get_rich_anch_name_ptr(plk), -1);
			if (!nlk)
			{
				nlk = insert_dom_node(nlk_row, LINK_LAST);
				set_dom_node_name(nlk, get_rich_anch_name_ptr(plk), -1);
			}
			set_dom_node_text(nlk, get_rich_anch_text_ptr(plk), -1);
			set_dom_node_attr(nlk, ATTR_TITLE, -1, get_rich_anch_title_ptr(plk), -1);

			plk = get_rich_next_anch(rich, plk);
		}
	}
}

void import_rich_data(link_t_ptr rich, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr plk;
	bool_t b_sch;
	bool_t b_dyn;

	b_sch = (is_empty_schema(sch)) ? 0 : 1;
	b_dyn = get_rich_dynamic(rich);

	nlk_rowset = dom;

	slk_rowset = slk_row = NULL;

	if(b_sch)
	{
		slk_rowset = get_schema_next_element(sch, LINK_FIRST);
		while (slk_rowset)
		{
			if (is_schema_element(slk_rowset))
				break;

			slk_rowset = get_schema_next_element(sch, slk_rowset);
		}

		if (!slk_rowset)
			return;

		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, get_schema_element_name_ptr(slk_rowset), -1, 1) != 0)
			return;

		slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
		while (slk_row)
		{
			if (is_null(get_schema_element_name_ptr(slk_row)))
			{
				slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
				if (slk_ref && is_schema_declare(slk_ref))
				{
					slk_row = slk_ref;
					break;
				}
			}
			else if (is_schema_element(slk_row))
			{
				break;
			}

			slk_row = get_schema_next_element(slk_rowset, slk_row);
		}

		if (!slk_row)
			return;
	}
	else
	{
		if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, DOC_GRID_ROWSET, -1, 1) != 0)
			return;
	}

	nlk_row = get_dom_first_child_node(nlk_rowset);
	while (nlk_row)
	{
		if(b_sch)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, get_schema_element_name_ptr(slk_row), -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}

			slk = get_schema_next_element(slk_row, LINK_FIRST);
			while (slk)
			{
				plk = get_rich_anch(rich, get_schema_element_id_ptr(slk));
				if (!plk)
				{
					slk = get_schema_next_element(slk_row, slk);
					continue;
				}

				if (is_schema_element(slk))
				{
					nlk = find_dom_node_by_name(nlk_row, 0, get_schema_element_name_ptr(slk), -1);
					if (nlk)
					{
						set_rich_anch_text(plk, get_dom_node_text_ptr(nlk), -1);
						if (!get_rich_anch_fixed(plk))
						{
							set_rich_anch_title(plk, get_dom_node_attr_ptr(nlk, ATTR_TITLE, -1));
						}
					}
				}
				else if (is_schema_attribute(slk))
				{
					set_rich_anch_text(plk, get_dom_node_attr_ptr(nlk_row, get_schema_element_name_ptr(slk), -1), -1);
					if (!get_rich_anch_fixed(plk))
					{
						set_rich_anch_title(plk, get_dom_node_attr_ptr(nlk_row, ATTR_TITLE, -1));
					}
				}

				slk = get_schema_next_element(slk_row, slk);
			}
		}
		else
		{
			if (compare_text(get_dom_node_name_ptr(nlk_row), -1, DOC_GRID_ROW, -1, 1) != 0)
			{
				nlk_row = get_dom_next_sibling_node(nlk_row);
				continue;
			}

			nlk = get_dom_first_child_node(nlk_row);
			while (nlk)
			{
				plk = get_rich_anch(rich, get_dom_node_name_ptr(nlk));
				if (!plk && b_dyn)
				{
					plk = insert_rich_anch(rich, LINK_LAST);
					set_rich_anch_name(plk, get_dom_node_name_ptr(nlk));
				}
				else
				{
					plk = NULL;
				}

				if (plk)
				{
					set_rich_anch_text(plk, get_dom_node_text_ptr(nlk), -1);
					if (!get_rich_anch_fixed(plk))
					{
						set_rich_anch_title(plk, get_dom_node_attr_ptr(nlk, ATTR_TITLE, -1));
					}
				}

				nlk = get_dom_next_sibling_node(nlk);
			}
		}

		nlk_row = get_dom_next_sibling_node(nlk_row);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void export_dom_node(link_t_ptr node, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr plk;

	delete_dom_child_node(dom);
	nlk_rowset = dom;

	slk_rowset = get_schema_next_element(sch, LINK_FIRST);
	while (slk_rowset)
	{
		if (is_schema_element(slk_rowset))
			break;

		slk_rowset = get_schema_next_element(sch, slk_rowset);
	}

	if (!slk_rowset)
		return;

	set_dom_node_name(nlk_rowset, get_schema_element_name_ptr(slk_rowset), -1);

	slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
	while (slk_row)
	{
		if (is_null(get_schema_element_name_ptr(slk_row)))
		{
			slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
			if (slk_ref && is_schema_declare(slk_ref))
			{
				slk_row = slk_ref;
				break;
			}
		}
		else if (is_schema_element(slk_row))
		{
			break;
		}

		slk_row = get_schema_next_element(slk_rowset, slk_row);
	}

	if (!slk_row)
		return;

	nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
	set_dom_node_name(nlk_row, get_schema_element_name_ptr(slk_row), -1);

	slk = get_schema_next_element(slk_row, LINK_FIRST);
	while (slk)
	{
		if (is_schema_element(slk))
		{
			nlk = insert_dom_node(nlk_row, LINK_LAST);
			set_dom_node_name(nlk, get_schema_element_name_ptr(slk), -1);

			plk = find_dom_node_by_name(node, get_schema_element_id_ptr(slk),-1);

			if (plk)
			{
				set_dom_node_text(nlk, get_dom_node_text_ptr(plk), -1);
			}
		}
		else if (is_schema_attribute(slk))
		{
			plk = find_dom_node_by_name(node, get_schema_element_id_ptr(slk), -1);

			if (plk)
			{
				set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, get_dom_node_text_ptr(plk), -1);
			}
			else
			{
				set_dom_node_attr(nlk_row, get_schema_element_name_ptr(slk), -1, NULL, 0);
			}
		}

		slk = get_schema_next_element(slk_row, slk);
	}
}

void import_dom_node(link_t_ptr node, link_t_ptr sch, link_t_ptr dom)
{
	link_t_ptr slk_rowset, slk_row, slk_ref, slk;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr plk;

	nlk_rowset = dom;

	slk_rowset = get_schema_next_element(sch, LINK_FIRST);
	while (slk_rowset)
	{
		if (is_schema_element(slk_rowset))
			break;

		slk_rowset = get_schema_next_element(sch, slk_rowset);
	}

	if (!slk_rowset)
		return;

	if (compare_text(get_dom_node_name_ptr(nlk_rowset), -1, get_schema_element_name_ptr(slk_rowset), -1, 1) != 0)
		return;

	slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
	while (slk_row)
	{
		if (is_null(get_schema_element_name_ptr(slk_row)))
		{
			slk_ref = get_schema_element(sch, get_schema_element_ref_ptr(slk_row));
			if (slk_ref && is_schema_declare(slk_ref))
			{
				slk_row = slk_ref;
				break;
			}
		}
		else if (is_schema_element(slk_row))
		{
			break;
		}

		slk_row = get_schema_next_element(slk_rowset, slk_row);
	}

	if (!slk_row)
		return;

	nlk_row = get_dom_first_child_node(nlk_rowset);
	while (nlk_row)
	{
		if (compare_text(get_dom_node_name_ptr(nlk_row), -1, get_schema_element_name_ptr(slk_row), -1, 1) != 0)
		{
			nlk_row = get_dom_next_sibling_node(nlk_row);
			continue;
		}

		slk = get_schema_next_element(slk_row, LINK_FIRST);
		while (slk)
		{
			plk = find_dom_node_by_name(node, get_schema_element_id_ptr(slk), -1);
			if (!plk)
			{
				plk = insert_dom_node(node, LINK_LAST);
				set_dom_node_name(plk, get_schema_element_id_ptr(slk), -1);
			}

			if (is_schema_element(slk))
			{
				nlk = find_dom_node_by_name(nlk_row, get_schema_element_name_ptr(slk), -1);
				if (nlk)
				{
					set_dom_node_text(plk, get_dom_node_text_ptr(nlk), -1);
				}
			}
			else if (is_schema_attribute(slk))
			{
				set_dom_node_text(plk, get_dom_node_attr_ptr(nlk_row, get_schema_element_name_ptr(slk), -1), -1);
			}

			slk = get_schema_next_element(slk_row, slk);
		}

		nlk_row = get_dom_next_sibling_node(nlk_row);
	}
}*/

#endif //XDLDOC