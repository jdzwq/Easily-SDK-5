/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc diagram document

	@module	diagramview.c | implement file

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
#include "diagramview.h"
#include "xdlview.h"
#include "xdldoc.h"
#include "xdlimp.h"

#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define DIAGRAM_TITLE_HEIGHT		(float)10

void calc_diagram_entity_rect(link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	pxr->fx = get_diagram_entity_x(ilk);
	pxr->fy = get_diagram_entity_y(ilk);
	pxr->fw = get_diagram_entity_width(ilk);
	pxr->fh = get_diagram_entity_height(ilk);
}

int calc_diagram_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr ilk;
	int nHit;
	float xm, ym;
	xrect_t di;

	xm = ppt->fx;
	ym = ppt->fy;

	nHit = DIAGRAM_HINT_NONE;
	*pilk = NULL;

	ilk = get_diagram_prev_entity(ptr, LINK_LAST);
	while (ilk)
	{
		calc_diagram_entity_rect(ptr, ilk, &di);

		if (ft_inside(xm, ym, di.fx, di.fy, di.fx + di.fw, di.fy + di.fh))
		{
			*pilk = ilk;

			if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh / 2 - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh / 2 + DEF_SPLIT_FEED))
			{
				nHit = DIAGRAM_HINT_VERT_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw / 2 - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw / 2 + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = DIAGRAM_HINT_HORZ_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = DIAGRAM_HINT_CROSS_SPLIT;
				break;
			}

			nHit = DIAGRAM_HINT_ENTITY;
			break;
		}
		ilk = get_diagram_prev_entity(ptr, ilk);
	}

	return nHit;
}

void draw_diagram(const if_canvas_t* pif, link_t_ptr ptr)
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
	const tchar_t* entity;
	bool_t b_print;

	xpoint_t pt[5] = { 0 };

	const canvbox_t* pbox = &pif->rect;

	b_print = (pif->canvas->tag == _CANVAS_PRINTER) ? 1 : 0;

	default_xfont(&xf);
	default_xface(&xa);
	default_xpen(&xp);
	default_xbrush(&xb);

	style = get_diagram_style_ptr(ptr);

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


	ilk = get_diagram_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		default_xfont(&xf);
		default_xface(&xa);
		
		style = get_diagram_entity_style_ptr(ilk);

		calc_diagram_entity_rect(ptr, ilk, &xr);
		ft_offset_rect(&xr, pbox->fx, pbox->fy);

		parse_xfont_from_style(&xf, style);
		parse_xface_from_style(&xa, style);
		if (!b_print)
		{
			format_xcolor(&pif->clr_txt, xf.color);
		}

		entity = get_diagram_entity_class_ptr(ilk);

		if (compare_text(entity, -1, DOC_DIAGRAM_PROCESS, -1, 0) == 0)
		{
			(*pif->pf_draw_rect)(pif->canvas, &xp,NULL, &xr);
		}
		else if (compare_text(entity, -1, DOC_DIAGRAM_JOINT, -1, 0) == 0)
		{
			pt[0].fx = xr.fx;
			pt[0].fy = xr.fy + xr.fh / 2;
			pt[1].fx = xr.fx + xr.fw / 4;
			pt[1].fy = xr.fy + xr.fh / 4;
			pt[2].fx = xr.fx + xr.fw / 2;
			pt[2].fy = xr.fy + xr.fh / 2;
			pt[3].fx = xr.fx + xr.fw / 4 * 3;
			pt[3].fy = xr.fy + xr.fh / 4 * 3;
			pt[4].fx = xr.fx + xr.fw;
			pt[4].fy = xr.fy + xr.fh / 2;

			(*pif->pf_draw_curve)(pif->canvas, &xp, pt, 5);
		}
		/*else if (compare_text(entity, -1, DOC_DIAGRAM_DOT, -1, 0) == 0)
		{
			rows = get_diagram_dot_entity_rows(ilk);
			cols = get_diagram_dot_entity_cols(ilk);

			rx = get_diagram_dot_entity_rx(ilk);
			ry = get_diagram_dot_entity_ry(ilk);

			if (rows && cols)
			{
				pmt = matrix_alloc(rows, cols);
				matrix_parse(pmt, get_diagram_entity_text_ptr(ilk), -1);

				//draw_dot_entity(pif->canvas, &xp, &xb, &xf, &xr, rx, ry, pmt);

				matrix_free(pmt);
			}
		}
		else if (compare_text(entity, -1, DOC_DIAGRAM_COUNTER, -1, 0) == 0)
		{
			//draw_counter_entity(pif->canvas, &xp, &xb, &xf, &xr, get_diagram_counter_entity_layer_ptr(ilk), get_diagram_entity_text_ptr(ilk), get_diagram_counter_entity_size(ilk));
		}
		else if (compare_text(entity, -1, DOC_DIAGRAM_LINE, -1, 0) == 0)
		{
			n = get_diagram_line_entity_size(ilk);
			pvt = vector_alloc(n);
			if (n)
			{
				//draw_line_entity(pif->canvas, &xp, &xb, &xf, &xr, get_diagram_line_entity_base(ilk), get_diagram_line_entity_span(ilk), pvt);
			}
			vector_free(pvt);
		}
		else if (compare_text(entity, -1, DOC_DIAGRAM_BAR, -1, 0) == 0)
		{
			n = get_diagram_bar_entity_size(ilk);
			pvt = vector_alloc(n);
			if (n)
			{
				//draw_bar_entity(pif->canvas, &xp, &xb, &xf, &xr, get_diagram_bar_entity_base(ilk), get_diagram_bar_entity_span(ilk), pvt);
			}
			vector_free(pvt);
		}*/

		ilk = get_diagram_next_entity(ptr, ilk);
	}
}

#endif //XDLVIEW
