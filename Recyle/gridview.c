/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc grid document

	@module	griddoc.c | grid document implement file

	@devnote ����Ȩ 2005.01 - 2007.12	v3.0
	@devnote ����Ȩ 2008.01 - 2009.12	v3.5
	@devnote ����Ȩ 2012.01 - 2015.12	v4.0
	@devnote ����Ȩ 2016.01 -			v4.5
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
#include "gridview.h"
#include "xdldoc.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

int calc_grid_row_scope(link_t_ptr ptr, const xsize_t* pps, int page, link_t_ptr* pfirst, link_t_ptr* plast)
{
	int rowsperpage, rows;
	float ch, rh, th;
	link_t_ptr rlk;

	if (page <= 0)
		return 0;

	th = get_grid_title_height(ptr);
	ch = get_grid_colbar_height(ptr);
	rh = get_grid_rowbar_height(ptr);

	*pfirst = *plast = NULL;

	rowsperpage = (int)((pps->fy - th - ch) / rh);
	if (rowsperpage <= 0)
		return 0;

	*pfirst = get_visible_row_at(ptr, (page - 1) * rowsperpage);
	if (*pfirst == NULL)
		return 0;

	rows = 0;
	rlk = *pfirst;
	while (rlk && rowsperpage--)
	{
		*plast = rlk;
		rows++;
		rlk = get_next_visible_row(ptr, rlk);
	}

	return rows;
}

int calc_grid_row_page(link_t_ptr ptr, const xsize_t* pps, link_t_ptr rlk)
{
	link_t_ptr plk;
	int rowsperpage, count, page;
	float th, ch, rh;

	if (rlk == NULL)
		return 0;

	if (!get_row_visible(rlk))
		return 0;

	th = get_grid_title_height(ptr);
	ch = get_grid_colbar_height(ptr);
	rh = get_grid_rowbar_height(ptr);

	rowsperpage = (int)((pps->fy - th - ch) / rh);
	if (rowsperpage <= 0)
		return 0;

	page = 1;
	count = rowsperpage;
	plk = get_next_visible_row(ptr, LINK_FIRST);
	while (plk && plk != rlk)
	{
		count--;
		if (count == 0)
		{
			page++;
			count = rowsperpage;
		}
		plk = get_next_visible_row(ptr, plk);
	}
	return page;
}

int calc_grid_pages(link_t_ptr ptr, const xsize_t* pps)
{
	int rowsperpage, rows;
	float th, ch, rh;

	th = get_grid_title_height(ptr);
	ch = get_grid_colbar_height(ptr);
	rh = get_grid_rowbar_height(ptr);

	rowsperpage = (int)((pps->fy - th - ch) / rh);
	if (rowsperpage <= 0)
		return 1;

	rows = get_visible_row_count(ptr);	/*calcing use visible row*/
	if (rows == 0)
		return 1;
	if (rows % rowsperpage == 0)
		return rows / rowsperpage;
	else
		return rows / rowsperpage + 1;
}

float calc_grid_page_height(link_t_ptr ptr, const xsize_t* pps, int page)
{
	link_t_ptr rlk, rlk_first, rlk_last;
	float th, ch, rh;
	int pages;

	th = get_grid_title_height(ptr);
	ch = get_grid_colbar_height(ptr);
	rh = get_grid_rowbar_height(ptr);

	pages = calc_grid_pages(ptr, pps);

	if (page > pages)
		return 0;

	rlk_first = rlk_last = NULL;
	calc_grid_row_scope(ptr, pps, page, &rlk_first, &rlk_last);

	th += ch;
	rlk = rlk_first;
	while (rlk)
	{
		if (get_row_visible(rlk))
			th += rh;
		if (rlk == rlk_last)
			break;
		rlk = get_next_row(ptr, rlk);
	}

	if (get_grid_showsum(ptr) && page == pages)
		th += rh;

	return th;
}

int calc_grid_cell_rect(link_t_ptr ptr, const xsize_t* pps, int page, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr)
{
	link_t_ptr row, col;
	link_t_ptr rlk_first, rlk_last;
	float th, rw, ch, rh;
	float xm, ym;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	ch = get_grid_colbar_height(ptr);
	rh = get_grid_rowbar_height(ptr);
	rw = get_grid_rowbar_width(ptr);
	th = get_grid_title_height(ptr);

	if (rlk && !get_row_visible(rlk))
		return 0;

	if (!rlk)
		ym = th;
	else
		ym = th + ch;

	if (!clk)
		xm = 0;
	else
		xm = rw;

	if (!rlk && !clk)
	{
		pxr->fx = xm;
		pxr->fw = rw;
		pxr->fy = ym;
		pxr->fh = ch;
		return 1;
	}

	if (clk)
	{
		col = get_next_visible_col(ptr, LINK_FIRST);
		while (col && col != clk)
		{
			xm += get_col_width(col);
			col = get_next_visible_col(ptr, col);
		}
		pxr->fx = xm;
		pxr->fw = get_col_width(clk);
	}
	else
	{
		pxr->fx = xm;
		pxr->fw = rw;
	}

	if (!rlk)
	{
		pxr->fy = ym;
		pxr->fh = ch;
		return 1;
	}

	rlk_first = rlk_last = NULL;
	calc_grid_row_scope(ptr, pps, page, &rlk_first, &rlk_last);

	row = rlk_first;
	while (row && row != rlk)
	{
		ym += rh;

		row = get_next_visible_row(ptr, row);
		if (row == rlk_last)
			break;
	}

	if (row)
	{
		pxr->fy = ym;
		pxr->fh = rh;
		return 1;
	}

	xmem_zero((void*)pxr, sizeof(xrect_t));
	return 0;
}

int calc_grid_row_rect(link_t_ptr ptr, const xsize_t* pps, int page, link_t_ptr rlk, xrect_t* pxr)
{
	link_t_ptr clk;

	clk = get_prev_visible_col(ptr, LINK_LAST);

	if (!calc_grid_cell_rect(ptr, pps, page, rlk, clk, pxr))
		return 0;

	pxr->fw += pxr->fx;
	pxr->fx = 0;

	return 1;
}

int calc_grid_col_rect(link_t_ptr ptr, const xsize_t* pps, int page, link_t_ptr clk, xrect_t* pxr)
{
	if (!calc_grid_cell_rect(ptr, pps, page, NULL, clk, pxr))
		return 0;

	pxr->fh = calc_grid_page_height(ptr, pps, page) - get_grid_title_height(ptr);

	return 1;
}

int calc_grid_hint(link_t_ptr ptr, const xsize_t* pps, int page, const xpoint_t* ppt, link_t_ptr* prlk, link_t_ptr* pclk)
{
	link_t_ptr rlk, firstrlk, lastrlk, clk;
	float th, rw, ch, rh, cw, mx, my;
	float h = 0;
	float w = 0;
	int hint;

	th = get_grid_title_height(ptr);
	rw = get_grid_rowbar_width(ptr);
	ch = get_grid_colbar_height(ptr);
	rh = get_grid_rowbar_height(ptr);

	mx = ppt->fx;
	my = ppt->fy;

	hint = GRID_HINT_NONE;
	*prlk = NULL;
	*pclk = NULL;

	if (mx <= 0 && my <= 0)
	{
		return hint;
	}

	h = th;
	w = rw;

	if (mx < w && my < h)
	{
		hint = GRID_HINT_MENU;
		return hint;
	}

	if (mx >= w && my < h)
	{
		hint = GRID_HINT_TITLE;
		return hint;
	}

	h += ch;

	if (mx < rw - DEF_SPLIT_FEED && my < h - DEF_SPLIT_FEED)
	{
		hint = GRID_HINT_NULBAR;
		return hint;
	}

	if (mx > rw)
	{
		clk = get_next_visible_col(ptr, LINK_FIRST);
		while (clk)
		{
			cw = get_col_width(clk);

			if (mx > w + DEF_SPLIT_FEED && mx < w + cw - DEF_SPLIT_FEED)
			{
				*pclk = clk;
				if (my < h - DEF_SPLIT_FEED)
				{
					hint = GRID_HINT_COLBAR;
					return hint;
				}
			}
			else if (mx >= w + cw - DEF_SPLIT_FEED && mx <= w + cw + DEF_SPLIT_FEED)
			{
				*pclk = clk;
				if (my < h - DEF_SPLIT_FEED)
				{
					hint = GRID_HINT_VERT_SPLIT;
					return hint;
				}
			}

			w += cw;
			clk = get_next_visible_row(ptr, clk);
		}
	}

	firstrlk = lastrlk = NULL;
	calc_grid_row_scope(ptr, pps, page, &firstrlk, &lastrlk);
	rlk = firstrlk;
	while (rlk)
	{
		if (my > h + DEF_SPLIT_FEED && my < h + rh - DEF_SPLIT_FEED)
		{
			*prlk = rlk;

			if (mx < rw - DEF_SPLIT_FEED)
				hint = GRID_HINT_ROWBAR;
			else if (mx < w)
				hint = GRID_HINT_CELL;

			break;
		}
		else if (my >= h + rh - DEF_SPLIT_FEED && my <= h + rh + DEF_SPLIT_FEED && mx < rw - DEF_SPLIT_FEED)
		{
			*prlk = rlk;
			hint = GRID_HINT_HORZ_SPLIT;

			break;
		}

		h += rh;

		if (rlk == lastrlk)
			break;

		rlk = get_next_visible_row(ptr, rlk);
	}

	if (hint == GRID_HINT_NONE)
	{
		*pclk = *prlk = NULL;
	}

	return hint;
}

void draw_grid_page(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr, int page)
{
	link_t_ptr clk, rlk;
	link_t_ptr rlk_first, rlk_last;
	float th, rw, ch, rh;
	bool_t b_design, b_print;
	bool_t b_lastpage, b_sumrow, b_stepdraw, b_showcheck, b_tag = 0;

	const tchar_t *token, *shape, *style, *type, *colfmt;
	bool_t zeronull;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb_step, xb_bar, xb = { 0 };
	ximage_t xi = { 0 };
	xrect_t xrCell, xrBar, xrCheck;
	xcolor_t xc_check;
	float px, py, pw, ph;

	tchar_t sz_format[RES_LEN] = { 0 };
	xdate_t dt;
	xtime_t tt;

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	rlk_first = rlk_last = NULL;
	calc_grid_row_scope(ptr, RECTSIZE(pbox), page, &rlk_first, &rlk_last);

	if (rlk_last)
		b_lastpage = (get_next_visible_row(ptr, rlk_last)) ? 0 : 1;
	else
		b_lastpage = 0;

	b_design = grid_is_design(ptr);
	b_print = ((*pif->pf_canvas_type)(pif->canvas) == CANV_PRINTER) ? 1 : 0;

	b_sumrow = get_grid_showsum(ptr);
	b_stepdraw = get_grid_showstep(ptr);
	b_showcheck = get_grid_showcheck(ptr);

	th = get_grid_title_height(ptr);
	rw = get_grid_rowbar_width(ptr);
	ch = get_grid_colbar_height(ptr);
	rh = get_grid_rowbar_height(ptr);

	default_xfont(&xf);
	default_xface(&xa);

	style = get_grid_style_ptr(ptr);

	parse_xbrush_from_style(&xb, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_bkg, xb.color);
	}

	parse_xpen_from_style(&xp, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_frg, xp.color);
	}

	parse_xface_from_style(&xa, style);

	parse_xfont_from_style(&xf, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_txt, xf.color);
	}

	if (!b_print)
	{
		format_xcolor(&pif->clr_msk, xi.color);
	}

	xmem_copy((void*)&xb_bar, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bar, DEF_MIDD_DARKEN);

	xmem_copy((void*)&xb_step, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_step, DEF_SOFT_DARKEN);

	parse_xcolor(&xc_check, xp.color);

	shape = get_grid_shape_ptr(ptr);

	//draw title bar
	if (th)
	{
		xrBar.fx = px + rw;
		xrBar.fy = py;
		xrBar.fw = pw - rw;
		xrBar.fh = th;

		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrBar, get_grid_title_ptr(ptr), -1);
	}

	//draw col bar
	xrBar.fx = rw + px;
	xrBar.fy = th + py;
	xrBar.fh = ch;
	clk = get_next_visible_col(ptr, LINK_FIRST);
	while (clk)
	{
		default_xfont(&xf);
		default_xface(&xa);

		style = get_col_style_ptr(clk);

		parse_xface_from_style(&xa, style);

		parse_xfont_from_style(&xf, style);
		if (!b_print)
		{
			format_xcolor(&pif->clr_txt, xf.color);
		}

		xrBar.fw = get_col_width(clk);

		(*pif->pf_draw_shape)(pif->canvas, &xp, &xb_bar, &xrBar, shape);

		if (get_col_fixed(clk))
		{
			xrCheck.fx = xrBar.fx + xrBar.fw - xrBar.fh;
			xrCheck.fw = xrBar.fh;
			xrCheck.fy = xrBar.fy;
			xrCheck.fh = xrBar.fh;
			ft_center_rect(&xrCheck, DEF_SMALL_ICON, DEF_SMALL_ICON);
			(*pif->pf_draw_gizmo)(pif->canvas, &xc_check, &xrCheck, ATTR_ICON_FIXED);
		}

		token = get_col_title_ptr(clk);
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrBar, token, -1);

		xrBar.fx += xrBar.fw;
		clk = get_next_visible_col(ptr, clk);
	}

	//draw row bar
	xrBar.fx = px;
	xrBar.fw = rw;
	xrBar.fy = th + py;
	xrBar.fh = ch;

	(*pif->pf_draw_shape)(pif->canvas, &xp, &xb, &xrBar, shape);

	if (b_showcheck && get_rowset_checked(ptr))
	{
		xmem_copy((void*)&xrCheck, (void*)&xrBar, sizeof(xrect_t));
		ft_center_rect(&xrCheck, DEF_SMALL_ICON, DEF_SMALL_ICON);
		(*pif->pf_draw_gizmo)(pif->canvas, &xc_check, &xrCheck, ATTR_ICON_SELECTED);
	}

	xrBar.fy = th + ch + py;
	xrBar.fh = rh;
	rlk = rlk_first;
	while (rlk)
	{
		(*pif->pf_draw_shape)(pif->canvas, &xp, &xb, &xrBar, shape);

		if (b_showcheck && get_row_checked(rlk))
		{
			xmem_copy((void*)&xrCheck, (void*)&xrBar, sizeof(xrect_t));
			ft_center_rect(&xrCheck, DEF_SMALL_ICON, DEF_SMALL_ICON);
			(*pif->pf_draw_gizmo)(pif->canvas, &xc_check, &xrCheck, ATTR_ICON_SELECTED);
		}

		if (rlk_last == rlk)
			break;

		xrBar.fy += xrBar.fh;
		rlk = get_next_visible_row(ptr, rlk);
	}

	//draw sum bar
	if (b_sumrow && b_lastpage)
	{
		xrBar.fy += xrBar.fh;

		(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xrBar, shape);

		xmem_copy((void*)&xrCheck, (void*)&xrBar, sizeof(xrect_t));
		ft_center_rect(&xrCheck, DEF_SMALL_ICON, DEF_SMALL_ICON);
		(*pif->pf_draw_gizmo)(pif->canvas, &xc_check, &xrCheck, ATTR_ICON_SUM);
	}

	//draw cell
	xrCell.fx = rw + px;
	clk = get_next_visible_col(ptr, LINK_FIRST);
	while (clk)
	{
		default_xfont(&xf);
		default_xface(&xa);

		style = get_col_style_ptr(clk);

		parse_xfont_from_style(&xf, style);

		if (!b_print)
		{
			format_xcolor(&pif->clr_txt, xf.color);
		}

		parse_xface_from_style(&xa, style);
		xscpy(xa.text_align, get_col_alignment_ptr(clk));

		type = get_col_data_type_ptr(clk);
		colfmt = get_col_format_ptr(clk);
		zeronull = get_col_zeronull(clk);

		xrCell.fy = th + ch + py;
		xrCell.fw = get_col_width(clk);
		xrCell.fh = rh;

		b_tag = 0;
		rlk = rlk_first;
		while (rlk)
		{
			if (b_stepdraw && b_tag)
			{
				(*pif->pf_draw_shape)(pif->canvas, &xp, &xb_step, &xrCell, shape);
				b_tag = 0;
			}
			else
			{
				(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xrCell, shape);
				b_tag = 1;
			}

			if (compare_text(type, -1, ATTR_DATA_TYPE_BINARY, -1, 0) == 0)
			{
				parse_ximage_from_source(&xi, get_cell_text_ptr(rlk,clk));

				(*pif->pf_draw_image)(pif->canvas, &xi, &xrCell);
			}
			else if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
			{
				if (zeronull && !get_cell_integer(rlk,clk))
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, NULL, 0);
				}
				else
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, get_cell_text_ptr(rlk, clk), -1);
				}
			}
			else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
			{
				if (zeronull && is_zero_numeric(get_cell_numeric(rlk, clk),get_col_data_dig(clk)))
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, NULL, 0);
				}
				else if (!is_null(colfmt))
				{
					format_numeric_ex(get_cell_numeric(rlk, clk), colfmt, sz_format, RES_LEN);
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, sz_format, -1);
				}
				else
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, get_cell_text_ptr(rlk, clk), -1);
				}
			}
			else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
			{
				if (!is_null(colfmt))
				{
					get_cell_datetime(rlk, clk, &dt);
					format_datetime_ex(&dt, colfmt, sz_format, RES_LEN);
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, sz_format, -1);
				}
				else
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, get_cell_text_ptr(rlk,clk), -1);
				}
			}
			else if (compare_text(type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0)
			{
				if (!is_null(colfmt))
				{
					get_cell_date(rlk, clk, &dt);
					format_datetime_ex(&dt, colfmt, sz_format, RES_LEN);
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, sz_format, -1);
				}
				else
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, get_cell_text_ptr(rlk, clk), -1);
				}
			}
			else if (compare_text(type, -1, ATTR_DATA_TYPE_TIME, -1, 0) == 0)
			{
				if (!is_null(colfmt))
				{
					get_cell_time(rlk, clk, &tt);
					format_time_ex(&tt, colfmt, sz_format, RES_LEN);
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, sz_format, -1);
				}
				else
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, get_cell_text_ptr(rlk, clk), -1);
				}
			}
			else
			{
				if (get_col_password(clk))
				{
					format_password(get_cell_text_ptr(rlk, clk), sz_format, NUM_LEN);
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, sz_format, -1);
				}
				else
				{
					token = get_cell_options_text_ptr(rlk, clk);
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, token, -1);
				}
			}

			if (rlk_last == rlk)
				break;

			xrCell.fy += xrCell.fh;
			rlk = get_next_visible_row(ptr, rlk);
		}

		if (b_sumrow && b_lastpage)
		{
			xrCell.fy += xrCell.fh;

			(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xrCell, shape);

			token = get_col_sum_text_ptr(clk);

			if (!is_null(token))
			{
				if (!is_null(colfmt) && compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
				{
					format_numeric_ex(xstof(token), colfmt, sz_format, RES_LEN);

					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, sz_format, -1);
				}
				else
				{
					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrCell, token, -1);
				}
			}
		}

		xrCell.fx += xrCell.fw;
		clk = get_next_visible_col(ptr, clk);
	}
}

#endif //XDLVIEW