/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc chart document

	@module	chartview.c | chart document implement file

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
#include "chartview.h"
#include "xdlview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define CHART_TITLE_HEIGHT		(float)10

void calc_chart_table_rect(link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	pxr->fx = get_chart_table_x(ilk);
	pxr->fy = get_chart_table_y(ilk);
	pxr->fw = get_chart_table_width(ilk);
	pxr->fh = get_chart_table_height(ilk);
}

int calc_chart_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr ilk;
	int nHit;
	float xm, ym;
	xrect_t di;

	xm = ppt->fx;
	ym = ppt->fy;

	nHit = CHART_HINT_NONE;
	*pilk = NULL;

	ilk = get_chart_prev_table(ptr, LINK_LAST);
	while (ilk)
	{
		calc_chart_table_rect(ptr, ilk, &di);

		if (ft_inside(xm, ym, di.fx, di.fy, di.fx + di.fw, di.fy + di.fh))
		{
			*pilk = ilk;

			if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh / 2 - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh / 2 + DEF_SPLIT_FEED))
			{
				nHit = CHART_HINT_VERT_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw / 2 - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw / 2 + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = CHART_HINT_HORZ_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = CHART_HINT_CROSS_SPLIT;
				break;
			}

			nHit = CHART_HINT_TABLE;
			break;
		}
		ilk = get_chart_prev_table(ptr, ilk);
	}

	return nHit;
}

void draw_chart(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr obj,ilk;
	xrect_t xr;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	canvbox_t cb = { 0 };

	const tchar_t* style;
	const tchar_t* table;
	bool_t b_print;

	int rows, cols, n;
	float rx, ry;
	matrix_t* pmt;

	vector_t* pvt;

	b_print = ((*pif->pf_canvas_type)(pif->canvas) == _CANV_PRINTER) ? 1 : 0;

	default_xfont(&xf);
	default_xface(&xa);
	default_xpen(&xp);
	default_xbrush(&xb);

	style = get_chart_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);
	parse_xface_from_style(&xa, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_txt, xf.color);
	}

	parse_xpen_from_style(&xp, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_frg, xp.color);
	}

	parse_xbrush_from_style(&xb, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_bkg, xb.color);
	}
	lighten_xbrush(&xb, DEF_SOFT_DARKEN);


	ilk = get_chart_next_table(ptr, LINK_FIRST);
	while (ilk)
	{
		default_xfont(&xf);
		default_xface(&xa);
		
		style = get_chart_table_style_ptr(ilk);

		calc_chart_table_rect(ptr, ilk, &xr);
		ft_offset_rect(&xr, pbox->fx, pbox->fy);

		//(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr, ATTR_SHAPE_RECT);

		parse_xfont_from_style(&xf, style);
		parse_xface_from_style(&xa, style);
		if (!b_print)
		{
			format_xcolor(&pif->clr_txt, xf.color);
		}

		table = get_chart_table_class_ptr(ilk);

		if (compare_text(table, -1, DOC_CHART_MAP, -1, 0) == 0)
		{
			rows = get_chart_map_table_rows(ilk);
			cols = get_chart_map_table_cols(ilk);

			rx = get_chart_map_table_rx(ilk);
			ry = get_chart_map_table_ry(ilk);

			if (rows && cols)
			{
				pmt = matrix_alloc(rows, cols);
				matrix_parse(pmt, get_chart_table_text_ptr(ilk), -1);

				draw_map_table(pif->canvas, &xp, &xb, &xf, &xr, rx, ry, pmt);

				matrix_free(pmt);
			}
		}
		else if (compare_text(table, -1, DOC_CHART_DOT, -1, 0) == 0)
		{
			rows = get_chart_dot_table_rows(ilk);
			cols = get_chart_dot_table_cols(ilk);

			rx = get_chart_dot_table_rx(ilk);
			ry = get_chart_dot_table_ry(ilk);

			if (rows && cols)
			{
				pmt = matrix_alloc(rows, cols);
				matrix_parse(pmt, get_chart_table_text_ptr(ilk), -1);

				draw_dot_table(pif->canvas, &xp, &xb, &xf, &xr, rx, ry, pmt);

				matrix_free(pmt);
			}
		}
		else if (compare_text(table, -1, DOC_CHART_COUNTER, -1, 0) == 0)
		{
			draw_counter_table(pif->canvas, &xp, &xb, &xf, &xr, get_chart_counter_table_layer_ptr(ilk), get_chart_table_text_ptr(ilk), get_chart_counter_table_size(ilk));
		}
		else if (compare_text(table, -1, DOC_CHART_LINE, -1, 0) == 0)
		{
			n = get_chart_line_table_size(ilk);
			pvt = vector_alloc(n);
			if (n)
			{
				draw_line_table(pif->canvas, &xp, &xb, &xf, &xr, get_chart_line_table_base(ilk), get_chart_line_table_span(ilk), pvt);
			}
			vector_free(pvt);
		}
		else if (compare_text(table, -1, DOC_CHART_BAR, -1, 0) == 0)
		{
			n = get_chart_bar_table_size(ilk);
			pvt = vector_alloc(n);
			if (n)
			{
				draw_bar_table(pif->canvas, &xp, &xb, &xf, &xr, get_chart_bar_table_base(ilk), get_chart_bar_table_span(ilk), pvt);
			}
			vector_free(pvt);
		}

		ilk = get_chart_next_table(ptr, ilk);
	}
}

#endif //XDLVIEW
