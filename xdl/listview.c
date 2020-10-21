/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc list document

	@module	listview.c | implement file

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
#include "listview.h"
#include "xdldoc.h"
#include "xdlview.h"
#include "xdlimp.h"

#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

int calc_list_items_per_row(link_t_ptr ptr, float pw)
{
	int hs;

	hs = (int)(pw / get_list_item_width(ptr));
	if (hs == 0)
		hs = 1;

	return hs;
}

int calc_list_items_per_col(link_t_ptr ptr, float ph)
{
	int hs;

	hs = (int)(ph / get_list_item_height(ptr));
	if (hs == 0)
		hs = 1;

	return hs;
}

int calc_list_cols_per_page(link_t_ptr ptr, float pw)
{
	float iw;

	iw = get_list_item_width(ptr);

	return (int)(pw / iw);
}

float calc_list_height(link_t_ptr ptr, link_t_ptr plk)
{
	float fw, ih;
	int lines, re;
	int count, per;
	
	fw = get_list_width(ptr);
	ih = get_list_item_height(ptr);

	count = get_list_child_item_count(plk) + 1;
	per = calc_list_items_per_row(ptr, fw);

	re = count % per;
	lines = count / per;
	if (re)
		lines++;

	return ih * lines;
}

float calc_list_width(link_t_ptr ptr, link_t_ptr plk)
{
	float fh, iw;
	int lines, re;
	int count, per;

	fh = get_list_height(ptr);
	iw = get_list_item_width(ptr);

	count = get_list_child_item_count(plk) + 1;
	per = calc_list_items_per_col(ptr, fh);

	re = count % per;
	lines = count / per;
	if (re)
		lines++;

	return iw * lines;
}

bool_t calc_list_item_rect( link_t_ptr ptr, link_t_ptr plk, link_t_ptr ilk, xrect_t* pxr)
{
	link_t_ptr nlk;
	int pi, count;
	float fw, fh, iw, ih;
	const tchar_t* sz_lay;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	if (plk != get_list_parent_item(ilk))
		return 0;

	sz_lay = get_list_layer_ptr(ptr);

	fw = get_list_width(ptr);
	fh = get_list_height(ptr);
	iw = get_list_item_width(ptr);
	ih = get_list_item_height(ptr);

	if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
	{
		pi = calc_list_items_per_col(ptr, fh);
	}
	else
	{
		pi = calc_list_items_per_row(ptr, fw);
	}

	count = 1;
	nlk = get_list_first_child_item(plk);
	while (nlk)
	{
		if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		{
			pxr->fx = iw * (count / pi);
			pxr->fy = ih * (count % pi);
		}
		else
		{
			pxr->fx = iw * (count % pi);
			pxr->fy = ih * (count / pi);
		}

		if (nlk == ilk)
		{
			pxr->fw = iw;
			pxr->fh = ih;
			return 1;
		}

		count++;
		nlk = get_list_next_sibling_item(nlk);
	}

	return 0;
}

bool_t calc_list_item_text_rect(link_t_ptr ptr, link_t_ptr plk, link_t_ptr ilk, xrect_t* pxr)
{
	float ih, ic;
	bool_t b_showcheck;

	if (!calc_list_item_rect(ptr, plk, ilk, pxr))
		return 0;

	b_showcheck = get_list_showcheck(ptr);

	ih = get_list_item_height(ptr);
	ic = get_list_icon_span(ptr);

	if (b_showcheck)
	{
		pxr->fx += ic;
		pxr->fw -= ic;
	}

	pxr->fy += (ih - ic);
	pxr->fh = ic;

	return 1;
}

int	calc_list_hint(const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr plk, link_t_ptr* pilk)
{
	link_t_ptr nlk;
	int pi, count;
	float fw, fh, ic, lw, lh;
	float x1, y1, x2, y2;
	bool_t b_showcheck;
	const tchar_t* sz_lay;

	if (pilk)
		*pilk = NULL;

	b_showcheck = get_list_showcheck(ptr);
	sz_lay = get_list_layer_ptr(ptr);

	fw = get_list_width(ptr);
	fh = get_list_height(ptr);
	ic = get_list_icon_span(ptr);
	lw = get_list_item_width(ptr);
	lh = get_list_item_height(ptr);

	if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		pi = calc_list_items_per_col(ptr, fh);
	else
		pi = calc_list_items_per_row(ptr, fw);

	x1 = 0;
	y1 = 0;
	x2 = x1 + lw;
	y2 = y1 + lh;

	if (ft_inside(ppt->fx, ppt->fy, x1, y1, x2, y2))
	{
		*pilk = plk;
		return LIST_HINT_PARENT;
	}

	count = 1;
	nlk = get_list_first_child_item(plk);
	while (nlk)
	{
		if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		{
			x1 = lw * (count / pi);
			y1 = lh * (count % pi);
			x2 = x1 + lw;
			y2 = y1 + lh;
		}
		else
		{
			x1 = lw * (count % pi);
			y1 = lh * (count / pi);
			x2 = x1 + lw;
			y2 = y1 + lh;
		}

		if (ft_inside(ppt->fx, ppt->fy, x1, y1, x2, y2))
		{
			if (pilk)
				*pilk = nlk;

			if (ft_inside(ppt->fx, ppt->fy, x1, y2 - ic, x2, y2))
			{
				if (b_showcheck && ft_inside(ppt->fx, ppt->fy, x1, y2 - ic, x1 + ic, y2))
					return LIST_HINT_CHECK;
				
				return LIST_HINT_TEXT;
			}
			
			return LIST_HINT_ITEM;
		}

		count++;
		nlk = get_list_next_sibling_item(nlk);
	}

	return LIST_HINT_NONE;
}

void draw_list_child(const if_canvas_t* pif, link_t_ptr ptr, link_t_ptr plk)
{
	link_t_ptr nlk;
	int pi, count;
	float ic, iw, ih;
	bool_t b_showcheck;
	xrect_t xr, xr_img, xr_checkbox, xr_text;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	ximage_t xi = { 0 };
	xcolor_t xc = { 0 };
	const tchar_t* style;
	const tchar_t* layer;
	bool_t b_print;
	float px, py, pw, ph;

	const canvbox_t* pbox = &pif->rect;

	XDL_ASSERT( pif != NULL);

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	b_print = (pif->canvas->tag == _CANVAS_PRINTER) ? 1 : 0;

	b_showcheck = get_list_showcheck(ptr);

	iw = get_list_item_width(ptr);
	ih = get_list_item_height(ptr);
	ic = get_list_icon_span(ptr);

	layer = get_list_layer_ptr(ptr);

	if (compare_text(layer, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		pi = calc_list_items_per_col(ptr, ph);
	else
		pi = calc_list_items_per_row(ptr, pw);

	default_xpen(&xp);
	default_xbrush(&xb);
	default_xfont(&xf);
	default_xface(&xa);

	style = get_list_style_ptr(ptr);

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

	xr.fx = px;
	xr.fy = py;
	xr.fw = iw;
	xr.fh = ih;

	xr_img.fx = xr.fx;
	xr_img.fy = xr.fy;
	xr_img.fw = xr.fw;
	xr_img.fh = xr.fh - ic;
	ft_center_rect(&xr_img, DEF_LARGE_ICON, DEF_LARGE_ICON);

	draw_gizmo(pif, &xc, &xr_img, GDI_ATTR_GIZMO_FOLDER);

	xr_text.fx = xr.fx;
	xr_text.fy = xr.fy + xr.fh - ic;
	xr_text.fw = xr.fw;
	xr_text.fh = ic;
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, _T(". ."), -1);

	count = 1;
	nlk = get_list_first_child_item(plk);
	while (nlk)
	{
		if (compare_text(layer, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		{
			xr.fx = iw * (count / pi) + px;
			xr.fy = ih * (count % pi) + py;
			xr.fw = iw;
			xr.fh = ih;
		}
		else
		{
			xr.fx = iw * (count % pi) + px;
			xr.fy = ih * (count / pi) + py;
			xr.fw = iw;
			xr.fh = ih;
		}

		xr_img.fx = xr.fx;
		xr_img.fy = xr.fy;
		xr_img.fw = xr.fw;
		xr_img.fh = xr.fh - ic;
		ft_center_rect(&xr_img, DEF_LARGE_ICON, DEF_LARGE_ICON);

		draw_gizmo(pif, &xc, &xr_img, GDI_ATTR_GIZMO_FOLDER);

		xr_img.fx = xr.fx;
		xr_img.fy = xr.fy;
		xr_img.fw = xr.fw;
		xr_img.fh = xr.fh - ic;

		xr_img.fy += xr_img.fh / 4;
		xr_img.fh -= xr_img.fh / 4;

		ft_center_rect(&xr_img, DEF_SMALL_ICON, DEF_SMALL_ICON);

		draw_gizmo(pif, &xc, &xr_img, get_list_item_icon_ptr(nlk));

		if (b_showcheck)
		{
			xr_checkbox.fx = xr.fx;
			xr_checkbox.fy = xr.fy + xr.fh - ic;
			xr_checkbox.fw = ic;
			xr_checkbox.fh = ic;

			ft_center_rect(&xr_checkbox, DEF_SMALL_ICON, DEF_SMALL_ICON);

			if (get_list_item_checked(nlk))
			{
				draw_gizmo(pif, &xc, &xr_checkbox, GDI_ATTR_GIZMO_CHECKED);
			}
			else
			{
				draw_gizmo(pif, &xc, &xr_checkbox, GDI_ATTR_GIZMO_CHECKBOX);
			}

			xr_text.fx = xr.fx + ic;
			xr_text.fy = xr.fy + xr.fh - ic;
			xr_text.fw = xr.fw - ic;
			xr_text.fh = ic;
		}
		else
		{
			xr_text.fx = xr.fx;
			xr_text.fy = xr.fy + xr.fh - ic;
			xr_text.fw = xr.fw;
			xr_text.fh = ic;
		}

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_list_item_title_ptr(nlk), -1);

		count++;
		nlk = get_list_next_sibling_item(nlk);
	}
}

#endif //XDLVIEW