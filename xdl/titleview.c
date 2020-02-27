/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc title view

	@module	titleview.c | implement file

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
#include "titleview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define TITLE_EDGE_DARK	 (float)1
#define	TITLE_EDGE_LIGHT (float)2
#define TITLE_EDGE_ARC	 (float)1
/*********************************************************************************************/
float calc_title_width(const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr plk;
	bool_t b_vert;
	float iw, w = 0;

	b_vert = (compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_LEFT, -1, 0) == 0 || compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_RIGHT, -1, 0) == 0) ? 1 : 0;
	if (b_vert)
		return pbox->fw;

	iw = get_title_item_width(ptr);

	plk = get_title_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		w += iw;
		plk = get_title_next_item(ptr, plk);
	}

	return w;
}

float calc_title_height(const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr plk;
	bool_t b_vert;
	float ih, h = 0;

	b_vert = (compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_LEFT, -1, 0) == 0 || compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_RIGHT, -1, 0) == 0) ? 1 : 0;
	if (!b_vert)
		return pbox->fh;

	ih = get_title_item_height(ptr);

	plk = get_title_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		h += ih;
		plk = get_title_next_item(ptr, plk);
	}

	return h;
}

void calc_title_item_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	link_t_ptr plk;
	float pw, ph, iw, ih, hw = 0;
	bool_t lay_vert;
	xrect_t xr;

	lay_vert = (compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_LEFT, -1, 0) == 0 || compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_RIGHT, -1, 0) == 0) ? 1 : 0;
	
	pw = pbox->fw;
	ph = pbox->fh;
	iw = get_title_item_width(ptr);
	ih = get_title_item_height(ptr);

	plk = get_title_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		if (lay_vert)
		{
			xr.fx = 0;
			xr.fy = hw;
			xr.fw = pw;
			xr.fh = ih;
		}
		else
		{
			xr.fx = hw;
			xr.fy = 0;
			xr.fw = iw;
			xr.fh = ph;
		}

		if (plk == ilk)
		{
			xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
			return;
		}

		if (lay_vert)
		{
			hw += ih;
		}
		else
		{
			hw += iw;
		}

		plk = get_title_next_item(ptr, plk);
	}

	xmem_zero((void*)pxr, sizeof(xrect_t));
}

int calc_title_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr plk_focus, link_t_ptr* pilk)
{
	link_t_ptr plk;
	float pw, ph, iw, ih, ic, hw = 0;
	float xm, ym;
	bool_t lay_vert;
	int nHint;
	xrect_t xr;

	nHint = TITLE_HINT_NONE;
	*pilk = NULL;

	xm = ppt->fx;
	ym = ppt->fy;

	lay_vert = (compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_LEFT, -1, 0) == 0 || compare_text(get_title_oritation_ptr(ptr), -1, ATTR_ORITATION_RIGHT, -1, 0) == 0) ? 1 : 0;
	
	pw = pbox->fw;
	ph = pbox->fh;
	iw = get_title_item_width(ptr);
	ih = get_title_item_height(ptr);
	ic = get_title_icon_span(ptr);

	plk = get_title_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		if (lay_vert)
		{
			xr.fx = 0;
			xr.fy = hw;
			xr.fw = pw;
			xr.fh = ih;
		}
		else
		{
			xr.fx = hw;
			xr.fy = 0;
			xr.fw = iw;
			xr.fh = ph;
		}

		if (ft_inside(xm, ym, xr.fx + 1, xr.fy + 1, xr.fx + xr.fw - 1, xr.fy + xr.fh - 1))
		{
			nHint = TITLE_HINT_ITEM;
			*pilk = plk;

			if (lay_vert)
			{
				if(plk == plk_focus && ft_inside(xm, ym, xr.fx + (xr.fw - ic) / 2 - TITLE_EDGE_DARK / 2, xr.fy, xr.fx + (xr.fw + ic) / 2 - TITLE_EDGE_DARK / 2, xr.fy + ic))
					nHint = TITLE_HINT_CLOSE;
			}
			else
			{
				if (plk == plk_focus && ft_inside(xm, ym, xr.fx, xr.fy + (xr.fh - ic) / 2 - TITLE_EDGE_DARK / 2, xr.fx + ic, xr.fy + (xr.fh + ic) / 2 - TITLE_EDGE_DARK / 2))
					nHint = TITLE_HINT_CLOSE;
			}

			break;
		}

		if (lay_vert)
		{
			hw += ih;
		}
		else
		{
			hw += iw;
		}

		plk = get_title_next_item(ptr, plk);
	}

	return nHint;
}

void draw_title(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr plk_focus)
{
	link_t_ptr plk;
	bool_t lay_vert;
	float iw, ih, ic, hw = 0;
	xrect_t xr_image, xr_text, xr = { 0 };
	xpoint_t pt_arc = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf_focus, xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };
	xpoint_t pt[6];
	const tchar_t *style, *orita, *icon;
	bool_t b_print;
	float px, py, pw, ph;

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	b_print = (pif->canvas->tag == _CANVAS_PRINTER) ? 1 : 0;

	default_xfont(&xf);
	default_xface(&xa);

	style = get_title_style_ptr(ptr);
	orita = get_title_oritation_ptr(ptr);

	parse_xface_from_style(&xa, style);

	parse_xfont_from_style(&xf, style);
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

	if (!b_print)
	{
		format_xcolor(&pif->clr_msk, xi.color);
	}

	if (!b_print)
	{
		xmem_copy((void*)&xc, (void*)&pif->clr_ico, sizeof(xcolor_t));
	}
	else
	{
		parse_xcolor(&xc, xp.color);
	}

	xmem_copy((void*)&xf_focus, (void*)&xf, sizeof(xfont_t));
	xscpy(xf_focus.style, GDI_ATTR_FONT_STYLE_UNDERLINE);
	xscpy(xf_focus.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	lay_vert = (compare_text(orita, -1, ATTR_ORITATION_LEFT, -1, 0) == 0 || compare_text(orita, -1, ATTR_ORITATION_RIGHT, -1, 0) == 0) ? 1 : 0;
	
	if (lay_vert)
		xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	else
		xscpy(xa.text_wrap, _T(""));

	lighten_xbrush(&xb, DEF_SOFT_LIGHTEN);

	iw = get_title_item_width(ptr);
	ih = get_title_item_height(ptr);
	ic = get_title_icon_span(ptr);;

	plk = get_title_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		if (lay_vert)
		{
			xr.fx = px;
			xr.fy = hw + py;
			xr.fw = pw;
			xr.fh = ih;
		}
		else
		{
			xr.fx = hw + px;
			xr.fy = py;
			xr.fw = iw;
			xr.fh = ph;
		}

		icon = get_title_item_icon_ptr(plk);

		if (plk == plk_focus)
		{
			if (lay_vert)
			{
				if (compare_text(orita, -1, ATTR_ORITATION_LEFT, -1, 0) == 0)
				{
					/*xr_arc.fx = xr.fx;
					xr_arc.fy = xr.fy;
					xr_arc.fw = TITLE_EDGE_LIGHT;
					xr_arc.fh = ph;

					(*pif->pf_draw_rect)(pif->canvas, NULL, &xb, &xr_arc);*/

					pt[0].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[0].fy = 0;
					pt[1].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[1].fy = xr.fy;
					pt[2].fx = xr.fx + xr.fw - TITLE_EDGE_DARK;
					pt[2].fy = xr.fy;
					pt[3].fx = xr.fx + xr.fw - TITLE_EDGE_DARK;
					pt[3].fy = xr.fy + xr.fh - TITLE_EDGE_ARC;
					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 4);

					pt_arc.fx = xr.fx + xr.fw - TITLE_EDGE_DARK - TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + xr.fh - TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, 0, -XPI / 2);

					pt[0].fx = xr.fx + xr.fw - TITLE_EDGE_DARK - TITLE_EDGE_ARC;
					pt[0].fy = xr.fy + xr.fh;
					pt[1].fx = xr.fx + TITLE_EDGE_LIGHT + TITLE_EDGE_ARC;
					pt[1].fy = xr.fy + xr.fh;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					pt_arc.fx = xr.fx + TITLE_EDGE_LIGHT + TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + xr.fh + TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, XPI / 2, XPI);

					pt[0].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[0].fy = xr.fy + xr.fh + TITLE_EDGE_ARC;
					pt[1].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[1].fy = ph;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					/*pt[0].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[0].fy = 0;
					pt[1].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[1].fy = xr.fy;
					pt[2].fx = xr.fx + xr.fw - TITLE_EDGE_DARK;
					pt[2].fy = xr.fy;
					pt[3].fx = xr.fx + xr.fw - TITLE_EDGE_DARK;
					pt[3].fy = xr.fy + xr.fh;
					pt[4].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[4].fy = xr.fy + xr.fh;
					pt[5].fx = xr.fx + TITLE_EDGE_LIGHT;
					pt[5].fy = ph;

					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 6);*/

					xr_image.fx = xr.fx + (xr.fw - ic) / 2 - TITLE_EDGE_DARK / 2;
					xr_image.fy = xr.fy;
					xr_image.fw = ic;
					xr_image.fh = ic;

					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);

					if (get_title_item_locked(plk))
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);
					}
					else
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, GDI_ICON_CLOSE);
					}

					xr_text.fx = xr.fx;
					xr_text.fy = xr.fy + ic;
					xr_text.fw = xr.fw - TITLE_EDGE_DARK;
					xr_text.fh = xr.fh - ic;

					(*pif->pf_draw_text)(pif->canvas, &xf_focus, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
				else if (compare_text(orita, -1, ATTR_ORITATION_RIGHT, -1, 0) == 0)
				{
					/*xr_arc.fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					xr_arc.fy = xr.fy;
					xr_arc.fw = TITLE_EDGE_LIGHT;
					xr_arc.fh = ph;

					(*pif->pf_draw_rect)(pif->canvas, NULL, &xb, &xr_arc);*/

					pt[0].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[0].fy = 0;
					pt[1].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[1].fy = xr.fy;
					pt[2].fx = xr.fx + TITLE_EDGE_DARK;
					pt[2].fy = xr.fy;
					pt[3].fx = xr.fx + TITLE_EDGE_DARK;
					pt[3].fy = xr.fy + xr.fh - TITLE_EDGE_ARC;
					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 4);

					pt_arc.fx = xr.fx + TITLE_EDGE_DARK + TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + xr.fh - TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, XPI, XPI / 2 * 3);

					pt[0].fx = xr.fx + TITLE_EDGE_DARK + TITLE_EDGE_ARC;
					pt[0].fy = xr.fy + xr.fh;
					pt[1].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT - TITLE_EDGE_ARC;
					pt[1].fy = xr.fy + xr.fh;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					pt_arc.fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT - TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + xr.fh + TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, 0, XPI / 2);

					pt[0].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[0].fy = xr.fy + xr.fh + TITLE_EDGE_ARC;
					pt[1].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[1].fy = ph;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					/*pt[0].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[0].fy = 0;
					pt[1].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[1].fy = xr.fy;
					pt[2].fx = xr.fx + TITLE_EDGE_DARK;
					pt[2].fy = xr.fy;
					pt[3].fx = xr.fx + TITLE_EDGE_DARK;
					pt[3].fy = xr.fy + xr.fh;
					pt[4].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[4].fy = xr.fy + xr.fh;
					pt[5].fx = xr.fx + xr.fw - TITLE_EDGE_LIGHT;
					pt[5].fy = ph;

					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 6);*/

					xr_image.fx = xr.fx + (xr.fw - ic) / 2 + TITLE_EDGE_DARK / 2;
					xr_image.fy = xr.fy;
					xr_image.fw = ic;
					xr_image.fh = ic;
						
					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);
					
					if (get_title_item_locked(plk))
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);
					}
					else
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, GDI_ICON_CLOSE);
					}

					xr_text.fx = xr.fx + TITLE_EDGE_DARK;
					xr_text.fy = xr.fy + ic;
					xr_text.fw = xr.fw - TITLE_EDGE_DARK;
					xr_text.fh = xr.fh - ic;

					(*pif->pf_draw_text)(pif->canvas, &xf_focus, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
			}
			else
			{
				if (compare_text(orita, -1, ATTR_ORITATION_TOP, -1, 0) == 0)
				{
					/*xr_arc.fx = 0;
					xr_arc.fy = xr.fy;
					xr_arc.fw = pw;
					xr_arc.fh = TITLE_EDGE_LIGHT;

					(*pif->pf_draw_rect)(pif->canvas, NULL, &xb, &xr_arc);*/

					pt[0].fx = 0;
					pt[0].fy = xr.fy + TITLE_EDGE_LIGHT;
					pt[1].fx = xr.fx;
					pt[1].fy = xr.fy + TITLE_EDGE_LIGHT;
					pt[2].fx = xr.fx;
					pt[2].fy = xr.fy + xr.fh - TITLE_EDGE_DARK;
					pt[3].fx = xr.fx + xr.fw - TITLE_EDGE_ARC;
					pt[3].fy = xr.fy + xr.fh - TITLE_EDGE_DARK;
					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 4);

					pt_arc.fx = xr.fx + xr.fw - TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + xr.fh - TITLE_EDGE_DARK - TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, XPI / 2 * 3, 2 * XPI);

					pt[0].fx = xr.fx + xr.fw;
					pt[0].fy = xr.fy + xr.fh - TITLE_EDGE_DARK - TITLE_EDGE_ARC;
					pt[1].fx = xr.fx + xr.fw;
					pt[1].fy = xr.fy + TITLE_EDGE_LIGHT + TITLE_EDGE_ARC;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					pt_arc.fx = xr.fx + xr.fw + TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + TITLE_EDGE_LIGHT + TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, XPI / 2, XPI);

					pt[0].fx = xr.fx + xr.fw + TITLE_EDGE_ARC;
					pt[0].fy = xr.fy + TITLE_EDGE_LIGHT;
					pt[1].fx = pw;
					pt[1].fy = xr.fy + TITLE_EDGE_LIGHT;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					/*pt[0].fx = 0;
					pt[0].fy = xr.fy + TITLE_EDGE_LIGHT;
					pt[1].fx = xr.fx;
					pt[1].fy = xr.fy + TITLE_EDGE_LIGHT;
					pt[2].fx = xr.fx;
					pt[2].fy = xr.fy + xr.fh - TITLE_EDGE_DARK;
					pt[3].fx = xr.fx + xr.fw;
					pt[3].fy = xr.fy + xr.fh - TITLE_EDGE_DARK;
					pt[4].fx = xr.fx + xr.fw;
					pt[4].fy = xr.fy + TITLE_EDGE_LIGHT;
					pt[5].fx = pw;
					pt[5].fy = xr.fy + TITLE_EDGE_LIGHT;

					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 6);*/

					xr_image.fx = xr.fx;
					xr_image.fy = xr.fy + (xr.fh - ic) / 2 - TITLE_EDGE_DARK / 2;
					xr_image.fw = ic;
					xr_image.fh = ic;
								
					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);
					
					if (get_title_item_locked(plk))
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);
					}
					else
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, GDI_ICON_CLOSE);
					}

					xr_text.fx = xr.fx + ic;
					xr_text.fy = xr.fy;
					xr_text.fw = xr.fw - ic;
					xr_text.fh = xr.fh - TITLE_EDGE_DARK;

					(*pif->pf_draw_text)(pif->canvas, &xf_focus, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
				else if (compare_text(orita, -1, ATTR_ORITATION_BOTTOM, -1, 0) == 0)
				{
					/*xr_arc.fx = 0;
					xr_arc.fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					xr_arc.fw = pw;
					xr_arc.fh = TITLE_EDGE_LIGHT;

					(*pif->pf_draw_rect)(pif->canvas, NULL, &xb, &xr_arc);*/

					pt[0].fx = 0;
					pt[0].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					pt[1].fx = xr.fx;
					pt[1].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					pt[2].fx = xr.fx;
					pt[2].fy = xr.fy + TITLE_EDGE_DARK;
					pt[3].fx = xr.fx + xr.fw -  TITLE_EDGE_ARC;
					pt[3].fy = xr.fy + TITLE_EDGE_DARK;
					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 4);

					pt_arc.fx = xr.fx + xr.fw - TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + TITLE_EDGE_DARK + TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, 0, XPI / 2);

					pt[0].fx = xr.fx + xr.fw;
					pt[0].fy = xr.fy + TITLE_EDGE_DARK + TITLE_EDGE_ARC;
					pt[1].fx = xr.fx + xr.fw;
					pt[1].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT - TITLE_EDGE_ARC;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					pt_arc.fx = xr.fx + xr.fw + TITLE_EDGE_ARC;
					pt_arc.fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT - TITLE_EDGE_ARC;
					(*pif->pf_draw_arc)(pif->canvas, &xp, &pt_arc, TITLE_EDGE_ARC, TITLE_EDGE_ARC, XPI, XPI / 2 * 3);

					pt[0].fx = xr.fx + xr.fw + TITLE_EDGE_ARC;
					pt[0].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					pt[1].fx = pw;
					pt[1].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

					/*pt[0].fx = 0;
					pt[0].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					pt[1].fx = xr.fx;
					pt[1].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					pt[2].fx = xr.fx;
					pt[2].fy = xr.fy + TITLE_EDGE_DARK;
					pt[3].fx = xr.fx + xr.fw;
					pt[3].fy = xr.fy + TITLE_EDGE_DARK;
					pt[4].fx = xr.fx + xr.fw;
					pt[4].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;
					pt[5].fx = pw;
					pt[5].fy = xr.fy + xr.fh - TITLE_EDGE_LIGHT;

					(*pif->pf_draw_polyline)(pif->canvas, &xp, pt, 6);*/

					xr_image.fx = xr.fx;
					xr_image.fy = xr.fy + (xr.fh - ic) / 2 + TITLE_EDGE_DARK / 2;
					xr_image.fw = ic;
					xr_image.fh = ic;
						
					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);
					
					if (get_title_item_locked(plk))
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);
					}
					else
					{
						(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, GDI_ICON_CLOSE);
					}

					xr_text.fx = xr.fx + ic;
					xr_text.fy = xr.fy + TITLE_EDGE_DARK;
					xr_text.fw = xr.fw - ic;
					xr_text.fh = xr.fh - TITLE_EDGE_DARK;

					(*pif->pf_draw_text)(pif->canvas, &xf_focus, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
			}
		}
		else
		{
			if (lay_vert)
			{
				if (compare_text(orita, -1, ATTR_ORITATION_LEFT, -1, 0) == 0)
				{
					xr_image.fx = xr.fx + (xr.fw - ic) / 2 + TITLE_EDGE_DARK / 2;
					xr_image.fy = xr.fy;
					xr_image.fw = ic;
					xr_image.fh = ic;

					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);

					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);

					xr_text.fx = xr.fx + TITLE_EDGE_LIGHT;
					xr_text.fy = xr.fy + ic;
					xr_text.fw = xr.fw - TITLE_EDGE_LIGHT;
					xr_text.fh = xr.fh - ic;

					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
				else if (compare_text(orita, -1, ATTR_ORITATION_RIGHT, -1, 0) == 0)
				{
					xr_image.fx = xr.fx + (xr.fw - ic) / 2 - TITLE_EDGE_DARK / 2;
					xr_image.fy = xr.fy;
					xr_image.fw = ic;
					xr_image.fh = ic;

					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);

					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);

					xr_text.fx = xr.fx;
					xr_text.fy = xr.fy + ic;
					xr_text.fw = xr.fw - TITLE_EDGE_LIGHT;
					xr_text.fh = xr.fh - ic;

					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
			}
			else
			{
				if (compare_text(orita, -1, ATTR_ORITATION_TOP, -1, 0) == 0)
				{
					xr_image.fx = xr.fx;
					xr_image.fy = xr.fy + (xr.fh - ic) / 2 + TITLE_EDGE_DARK / 2;
					xr_image.fw = ic;
					xr_image.fh = ic;

					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);

					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);

					xr_text.fx = xr.fx + ic;
					xr_text.fy = xr.fy + TITLE_EDGE_LIGHT;
					xr_text.fw = xr.fw - ic;
					xr_text.fh = xr.fh - TITLE_EDGE_LIGHT;

					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
				else if (compare_text(orita, -1, ATTR_ORITATION_BOTTOM, -1, 0) == 0)
				{
					xr_image.fx = xr.fx;
					xr_image.fy = xr.fy + (xr.fh - ic) / 2 - TITLE_EDGE_DARK / 2;
					xr_image.fw = ic;
					xr_image.fh = ic;

					ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);

					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_image, icon);

					xr_text.fx = xr.fx + ic;
					xr_text.fy = xr.fy;
					xr_text.fw = xr.fw - ic;
					xr_text.fh = xr.fh - TITLE_EDGE_LIGHT;

					(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_title_item_title_ptr(plk), -1);
				}
			}
		}

		if (lay_vert)
		{
			hw += ih;
		}
		else
		{
			hw += iw;
		}

		plk = get_title_next_item(ptr, plk);
	}
}

#endif //XDLVIEW