/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc string view

	@module	stringview.c | string view implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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
#include "stringview.h"
#include "stringdoc.h"

#include "hashtable.h"
#include "xdlutil.h"
#include "xdlstr.h"
#include "domdoc.h"
#include "imagelist.h"
/************************************************************************************************************/

void calc_string_suit_size(link_t_ptr ptr, xsize_t* pxs)
{
	int count;

	count = get_string_item_visible_count(ptr);
	if (count > 7)
		count = 7;
	
	pxs->cx = get_string_item_width(ptr) * 2;
	pxs->cy = get_string_item_height(ptr) * count;
}

long calc_string_width(const viewbox_t* pbox, link_t_ptr ptr)
{
	return pbox->bw;
}

long calc_string_height(const viewbox_t* pbox, link_t_ptr ptr)
{
	long ih, total = 0;
	link_t_ptr ilk;

	ih = get_string_item_height(ptr);

	ilk = get_string_next_visible_item(ptr, LINK_FIRST);
	while (ilk)
	{
		if (get_string_item_expanded(ilk))
			total += get_string_item_span(ilk);
		else
			total += ih;

		ilk = get_string_next_visible_item(ptr, ilk);
	}
	
	return total;
}

void calc_string_item_rect(const viewbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	link_t_ptr plk;
	long ih;
	xrect_t xr;

	memset((void*)pxr, 0, sizeof(xrect_t));

	ih = get_string_item_height(ptr);

	xr.x = -pbox->bx;
	xr.y = -pbox->by;
	xr.w = pbox->bw;
	xr.h = 0;

	plk = get_string_next_visible_item(ptr, LINK_FIRST);
	while (plk)
	{
		if (get_string_item_expanded(plk))
			xr.h = get_string_item_span(plk);
		else
			xr.h = ih;

		if (plk == ilk)
		{
			memcpy((void*)pxr, (void*)&xr, sizeof(xrect_t));
			return;
		}

		xr.y += xr.h;

		plk = get_string_next_visible_item(ptr, plk);
	}
}

void calc_string_item_text_rect(const viewbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	calc_string_item_rect(pbox, ptr, ilk, pxr);
	pxr->x += get_string_item_width(ptr);
	pxr->w -= get_string_item_width(ptr);
}

int calc_string_hint(const viewbox_t* pbox, link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr plk;
	long iw, ih;
	long xm, ym;
	int nHint;
	xrect_t xr;

	nHint = STRING_HINT_NONE;
	*pilk = NULL;

	xm = ppt->x;
	ym = ppt->y;

	iw = get_string_item_width(ptr);
	ih = get_string_item_height(ptr);

	xr.x = -pbox->bx;
	xr.y = -pbox->by;
	xr.w = pbox->bw;
	xr.h = 0;

	plk = get_string_next_visible_item(ptr, LINK_FIRST);

	while (plk)
	{
		if (get_string_item_expanded(plk))
			xr.h = get_string_item_span(plk);
		else
			xr.h = ih;

		if (is_inside(xm, ym, xr.x, xr.y, xr.x + iw, xr.y + xr.h))
		{
			nHint = STRING_HINT_KEY;
			*pilk = plk;
			break;
		}
		else if (is_inside(xm, ym, xr.x + iw, xr.y, xr.x + xr.w, xr.y + xr.h))
		{
			nHint = STRING_HINT_VAL;
			*pilk = plk;
			break;
		}

		xr.y += xr.h;

		plk = get_string_next_visible_item(ptr, plk);
	}

	return nHint;
}

void draw_string(const viewbox_t* pbox, const if_canvas_t* pif, link_t_ptr ptr)
{
	link_t_ptr plk;
	long iw, ih;
	xrect_t xr_text, xr = { 0 };
	xpoint_t pt[8];
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa_key, xa = { 0 };
	ximage_t xi = { 0 };
	const tchar_t* style;
	bool_t b_print;

	b_print = ((*pif->pf_canvas_type)(pif->canvas) == CANV_PRINTER) ? 1 : 0;

	style = get_string_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	parse_xface_from_style(&xa, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_txt, xa.text_color);
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
	lighten_xbrush(&xb, DEF_SOFT_LIGHTEN);

	memcpy((void*)&xa_key, (void*)&xa, sizeof(xface_t));
	xscpy(xa_key.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa_key.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa_key.text_wrap, _T(""));

	iw = get_string_item_width(ptr);
	ih = get_string_item_height(ptr);

	xr.x = -pbox->bx;
	xr.y = -pbox->by;
	xr.w = pbox->bw;
	xr.h = 0;

	plk = get_string_next_visible_item(ptr, LINK_FIRST);
	while (plk)
	{
		if (get_string_item_expanded(plk))
			xr.h = get_string_item_span(plk);
		else
			xr.h = ih;

		if (get_string_item_expanded(plk))
		{
			pt[0].x = xr.x + 10;
			pt[0].y = xr.y + (xr.h - ih) / 2 + 10;
			pt[1].x = xr.x + iw;
			pt[1].y = xr.y + (xr.h - ih) / 2 + 10;
			pt[2].x = xr.x + iw;
			pt[2].y = xr.y + 10;
			pt[3].x = xr.x + xr.w - 10;
			pt[3].y = xr.y + 10;
			pt[4].x = xr.x + xr.w - 10;
			pt[4].y = xr.y + xr.h - 10;
			pt[5].x = xr.x + iw;
			pt[5].y = xr.y + xr.h - 10;
			pt[6].x = xr.x + iw;
			pt[6].y = xr.y + (xr.h + ih) / 2 - 10;
			pt[7].x = xr.x + 10;
			pt[7].y = xr.y + (xr.h + ih) / 2 - 10;

			(*pif->pf_draw_polygon)(pif->canvas, &xp, &xb, pt, 8);
		}

		xr_text.x = xr.x;
		xr_text.y = xr.y;
		xr_text.w = iw;
		xr_text.h = xr.h;

		(*pif->pf_draw_text)(pif->canvas, &xf, &xa_key, &xr_text, get_string_item_name_ptr(plk), -1);

		xr_text.x = xr.x + iw;
		xr_text.y = xr.y;
		xr_text.w = xr.w - iw;
		xr_text.h = xr.h;

		if (get_string_item_expanded(plk))
		{
			xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
		}
		else
		{
			xscpy(xa.text_wrap, _T(""));
		}
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_string_item_text_ptr(plk), -1);

		xr.y += xr.h;

		plk = get_string_next_visible_item(ptr, plk);
	}
}

