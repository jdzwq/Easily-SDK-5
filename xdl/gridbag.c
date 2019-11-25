/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc grid bag document

	@module	gridbag.c | implement file

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

#include "gridbag.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void save_form_to_grid_row(link_t_ptr ptrForm, link_t_ptr ptrGrid, link_t_ptr ptrRow)
{
	link_t_ptr rlk, clk, flk;
	const tchar_t* tmp;
	const tchar_t* src;

	rlk = ptrRow;
	flk = get_next_field(ptrForm, LINK_FIRST);
	while (flk)
	{
		if (!IS_DATA_FIELD(get_field_class_ptr(flk)))
		{
			flk = get_next_field(ptrForm, flk);
			continue;
		}

		src = get_field_id_ptr(flk);
		if (is_null(src))
		{
			flk = get_next_field(ptrForm, flk);
			continue;
		}

		clk = get_col(ptrGrid, src);
		if (clk == NULL)
		{
			flk = get_next_field(ptrForm, flk);
			continue;
		}

		tmp = get_field_text_ptr(flk);
		if (compare_data(tmp, get_cell_text_ptr(rlk, clk), get_col_data_type_ptr(clk)) != 0)
		{
			set_cell_text(rlk, clk, tmp, -1);

			set_cell_dirty(rlk, clk, 1);
			set_row_dirty(rlk);
		}

		flk = get_next_field(ptrForm, flk);
	}
}

void load_form_from_grid_row(link_t_ptr ptrForm, link_t_ptr ptrGrid, link_t_ptr ptrRow)
{
	link_t_ptr clk, flk;
	const tchar_t* src;
	const tchar_t* tmp;

	flk = get_next_field(ptrForm, LINK_FIRST);
	while (flk)
	{
		if (!IS_DATA_FIELD(get_field_class_ptr(flk)))
		{
			flk = get_next_field(ptrForm, flk);
			continue;
		}

		src = get_field_id_ptr(flk);
		if (is_null(src))
		{
			flk = get_next_field(ptrForm, flk);
			continue;
		}

		clk = get_col(ptrGrid, src);
		if (clk == NULL)
		{
			flk = get_next_field(ptrForm, flk);
			continue;
		}

		tmp = get_cell_text_ptr(ptrRow, clk);
		if (compare_data(tmp, get_field_text_ptr(flk), get_field_data_type_ptr(clk)) != 0)
		{
			set_field_text(flk, tmp, -1);
		}

		flk = get_next_field(ptrForm, flk);
	}
}

void save_statis_to_grid(link_t_ptr ptr, link_t_ptr grid)
{
	link_t_ptr xlk, ylk;
	int rs;
	link_t_ptr clk_xax, clk, rlk_new;

	clk_xax = get_col(grid, get_statis_xaxis_ptr(ptr));

	xlk = get_next_xax(ptr, LINK_FIRST);
	while (xlk)
	{
		rs = get_xax_state(xlk);
		if (rs == dsNewClean || rs == dsDelete)
		{
			xlk = get_next_xax(ptr, xlk);
			continue;
		}

		rlk_new = insert_row(grid, LINK_LAST);
		set_row_state(rlk_new, dsNewClean);

		if (clk_xax)
		{
			set_cell_text(rlk_new, clk_xax, get_xax_text_ptr(xlk), -1);
			set_cell_dirty(rlk_new, clk_xax, 1);
			set_row_dirty(rlk_new);
		}

		ylk = get_next_yax(ptr, LINK_FIRST);
		while (ylk)
		{
			clk = get_col(grid, get_yax_id_ptr(ylk));
			if (clk == NULL)
			{
				ylk = get_next_yax(ptr, ylk);
				continue;
			}

			set_cell_text(rlk_new, clk, get_coor_text_ptr(xlk, ylk), -1);
			set_cell_dirty(rlk_new, clk, 1);
			set_row_dirty(rlk_new);

			ylk = get_next_yax(ptr, ylk);
		}

		xlk = get_next_xax(ptr, xlk);
	}
}

void load_statis_from_grid(link_t_ptr ptr, link_t_ptr grid)
{
	link_t_ptr rlk, clk, clk_xax;
	int rs;
	link_t_ptr ylk, xlk_new;

	clk_xax = get_col(grid, get_statis_xaxis_ptr(ptr));

	rlk = get_next_row(grid, LINK_FIRST);
	while (rlk)
	{
		rs = get_row_state(rlk);

		if (rs == dsDelete || rs == dsNewClean)
		{
			rlk = get_next_row(grid, rlk);
			continue;
		}

		xlk_new = insert_xax(ptr, LINK_LAST);

		if (clk_xax)
		{
			set_xax_text(xlk_new, get_cell_text_ptr(rlk, clk_xax));
		}

		ylk = get_next_yax(ptr, LINK_FIRST);
		while (ylk)
		{
			clk = get_col(grid, get_yax_id_ptr(ylk));
			if (clk == NULL)
			{
				ylk = get_next_yax(ptr, ylk);
				continue;
			}

			set_coor_text(xlk_new, ylk, get_cell_text_ptr(rlk, clk), -1);

			ylk = get_next_yax(ptr, ylk);
		}

		rlk = get_next_row(grid, rlk);
	}
}

void save_rich_to_grid_row(link_t_ptr ptr, link_t_ptr grid, link_t_ptr rlk)
{
	link_t_ptr nlk;
	link_t_ptr clk;

	nlk = get_rich_next_anch(ptr, LINK_FIRST);
	while (nlk)
	{
		clk = get_col(grid, get_rich_anch_name_ptr(nlk));
		if (!clk)
		{
			nlk = get_rich_next_anch(ptr, nlk);
			continue;
		}

		set_cell_text(rlk, clk, get_rich_anch_text_ptr(nlk), -1);
		set_cell_dirty(rlk, clk, 1);
		set_row_dirty(rlk);

		nlk = get_rich_next_anch(ptr, nlk);
	}
}

void load_rich_from_grid_row(link_t_ptr ptr, link_t_ptr grid, link_t_ptr rlk)
{
	link_t_ptr nlk;
	link_t_ptr clk;
	int rs;

	rs = get_row_state(rlk);
	if (rs == dsNewClean || rs == dsDelete)
		return;

	nlk = get_rich_next_anch(ptr, LINK_FIRST);
	while (nlk)
	{
		clk = get_col(grid, get_rich_anch_name_ptr(nlk));
		if (!clk)
		{
			nlk = get_rich_next_anch(ptr, nlk);
			continue;
		}

		set_rich_anch_text(nlk, get_cell_text_ptr(rlk, clk), -1);

		nlk = get_rich_next_anch(ptr, nlk);
	}
}

#endif //XDLDOC