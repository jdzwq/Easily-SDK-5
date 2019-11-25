/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc panorama document

	@module	panoramaview.c | implement file

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
#include "panoramaview.h"
#include "xdlview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define PANORAMA_TITLE_HEIGHT		(float)10

void calc_panorama_plot_rect(link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	pxr->fx = get_panorama_plot_x(ilk);
	pxr->fy = get_panorama_plot_y(ilk);
	pxr->fw = get_panorama_plot_width(ilk);
	pxr->fh = get_panorama_plot_height(ilk);
}

int calc_panorama_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr ilk;
	int nHit;
	float xm, ym;
	xrect_t di;

	xm = ppt->fx;
	ym = ppt->fy;

	nHit = PANORAMA_HINT_NONE;
	*pilk = NULL;

	ilk = get_panorama_prev_plot(ptr, LINK_LAST);
	while (ilk)
	{
		calc_panorama_plot_rect(ptr, ilk, &di);

		if (ft_inside(xm, ym, di.fx, di.fy, di.fx + di.fw, di.fy + di.fh))
		{
			*pilk = ilk;

			if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh / 2 - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh / 2 + DEF_SPLIT_FEED))
			{
				nHit = PANORAMA_HINT_VERT_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw / 2 - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw / 2 + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = PANORAMA_HINT_HORZ_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = PANORAMA_HINT_CROSS_SPLIT;
				break;
			}

			nHit = PANORAMA_HINT_PLOT;
			break;
		}
		ilk = get_panorama_prev_plot(ptr, ilk);
	}

	return nHit;
}

void draw_panorama(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
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
	const tchar_t* plot;
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

	style = get_panorama_style_ptr(ptr);

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


	ilk = get_panorama_next_plot(ptr, LINK_FIRST);
	while (ilk)
	{
		default_xfont(&xf);
		default_xface(&xa);
		
		style = get_panorama_plot_style_ptr(ilk);

		calc_panorama_plot_rect(ptr, ilk, &xr);
		ft_offset_rect(&xr, pbox->fx, pbox->fy);

		(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr, ATTR_SHAPE_RECT);

		parse_xfont_from_style(&xf, style);
		parse_xface_from_style(&xa, style);
		if (!b_print)
		{
			format_xcolor(&pif->clr_txt, xf.color);
		}

		plot = get_panorama_plot_class_ptr(ilk);

		/*if (compare_text(plot, -1, DOC_PANORAMA_MAP, -1, 0) == 0)
		{
			rows = get_panorama_map_plot_rows(ilk);
			cols = get_panorama_map_plot_cols(ilk);

			rx = get_panorama_map_plot_rx(ilk);
			ry = get_panorama_map_plot_ry(ilk);

			if (rows && cols)
			{
				pmt = matrix_alloc(rows, cols);
				matrix_parse(pmt, get_panorama_plot_text_ptr(ilk), -1);

				//draw_map_plot(pif->canvas, &xp, &xb, &xf, &xr, rx, ry, pmt);

				matrix_free(pmt);
			}
		}
		else if (compare_text(plot, -1, DOC_PANORAMA_DOT, -1, 0) == 0)
		{
			rows = get_panorama_dot_plot_rows(ilk);
			cols = get_panorama_dot_plot_cols(ilk);

			rx = get_panorama_dot_plot_rx(ilk);
			ry = get_panorama_dot_plot_ry(ilk);

			if (rows && cols)
			{
				pmt = matrix_alloc(rows, cols);
				matrix_parse(pmt, get_panorama_plot_text_ptr(ilk), -1);

				//draw_dot_plot(pif->canvas, &xp, &xb, &xf, &xr, rx, ry, pmt);

				matrix_free(pmt);
			}
		}
		else if (compare_text(plot, -1, DOC_PANORAMA_COUNTER, -1, 0) == 0)
		{
			//draw_counter_plot(pif->canvas, &xp, &xb, &xf, &xr, get_panorama_counter_plot_layer_ptr(ilk), get_panorama_plot_text_ptr(ilk), get_panorama_counter_plot_size(ilk));
		}
		else if (compare_text(plot, -1, DOC_PANORAMA_LINE, -1, 0) == 0)
		{
			n = get_panorama_line_plot_size(ilk);
			pvt = vector_alloc(n);
			if (n)
			{
				//draw_line_plot(pif->canvas, &xp, &xb, &xf, &xr, get_panorama_line_plot_base(ilk), get_panorama_line_plot_span(ilk), pvt);
			}
			vector_free(pvt);
		}
		else if (compare_text(plot, -1, DOC_PANORAMA_BAR, -1, 0) == 0)
		{
			n = get_panorama_bar_plot_size(ilk);
			pvt = vector_alloc(n);
			if (n)
			{
				//draw_bar_plot(pif->canvas, &xp, &xb, &xf, &xr, get_panorama_bar_plot_base(ilk), get_panorama_bar_plot_span(ilk), pvt);
			}
			vector_free(pvt);
		}*/

		ilk = get_panorama_next_plot(ptr, ilk);
	}
}

#endif //XDLVIEW
