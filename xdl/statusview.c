/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc status view

	@module	statusview.c | implement file

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
#include "statusview.h"
#include "xdldoc.h"
#include "xdlimp.h"

#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

float calc_status_width(const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr ilk;
	float w = 0;

	ilk = get_status_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
		w += get_status_item_width(ilk);
		ilk = get_status_next_item(ptr, ilk);
	}

	return w;
}

void calc_status_item_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	link_t_ptr plk;
	float pw, ph, iw, hw;
	bool_t ali_far;
	xrect_t xr;

	ali_far = (compare_text(get_status_alignment_ptr(ptr), -1, ATTR_ALIGNMENT_FAR, -1, 0) == 0) ? 1 : 0;

	pw = pbox->fw;
	ph = pbox->fh;

	if (ali_far)
		hw = pw;
	else
		hw = 0;

	if (ali_far)
		plk = get_status_prev_item(ptr, LINK_LAST);
	else
		plk = get_status_next_item(ptr, LINK_FIRST);

	while (plk)
	{
		iw = get_status_item_width(plk);

		if (ali_far)
		{
			xr.fx = hw - iw;
			xr.fy = 0;
			xr.fw = iw;
			xr.fh = ph;
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

		if (ali_far)
			hw -= iw;
		else
			hw += iw;

		if (ali_far)
			plk = get_status_prev_item(ptr, plk);
		else
			plk = get_status_next_item(ptr, plk);
	}

	xmem_zero((void*)pxr, sizeof(xrect_t));
}

void calc_status_title_rect(const canvbox_t* pbox, link_t_ptr ptr, xrect_t* pxr)
{
	bool_t ali_far;

	pxr->fx = 0;
	pxr->fy = 0;
	pxr->fw = pbox->fw;
	pxr->fh = pbox->fh;

	if (get_status_item_count(ptr) == 0)
		return;

	ali_far = (compare_text(get_status_alignment_ptr(ptr), -1, ATTR_ALIGNMENT_FAR, -1, 0) == 0) ? 1 : 0;
	if (ali_far)
	{
		calc_status_item_rect(pbox, ptr, get_status_next_item(ptr, LINK_FIRST), pxr);
		pxr->fw = pxr->fx;
		pxr->fx = 0;
	}
	else
	{
		calc_status_item_rect(pbox, ptr, get_status_prev_item(ptr, LINK_LAST), pxr);
		pxr->fx = pxr->fx + pxr->fw;
		pxr->fw = pbox->fw - pxr->fx;
	}
}

int calc_status_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr,link_t_ptr* pilk)
{
	link_t_ptr plk;
	float pw, ph, iw, hw, xm, ym;
	bool_t ali_far;
	int nHint;
	xrect_t xr;

	nHint = STATUS_HINT_NONE;
	*pilk = NULL;

	xm = ppt->fx;
	ym = ppt->fy;

	ali_far = (compare_text(get_status_alignment_ptr(ptr), -1, ATTR_ALIGNMENT_FAR, -1, 0) == 0) ? 1 : 0;

	pw = pbox->fw;
	ph = pbox->fh;

	if (ali_far)
		hw = pw;
	else
		hw = 0;

	if (ali_far)
		plk = get_status_prev_item(ptr, LINK_LAST);
	else
		plk = get_status_next_item(ptr, LINK_FIRST);

	while (plk)
	{
		iw = get_status_item_width(plk);

		if (ali_far)
		{
			xr.fx = hw - iw;
			xr.fy = 0;
			xr.fw = iw;
			xr.fh = ph;
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
			nHint = STATUS_HINT_ITEM;
			*pilk = plk;
			break;
		}

		if (ali_far)
			hw -= iw;
		else
			hw += iw;

		if (ali_far)
			plk = get_status_prev_item(ptr, plk);
		else
			plk = get_status_next_item(ptr, plk);
	}

	return nHint;
}

void draw_status(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr plk;
	bool_t ali_far;
	float ic, iw, hw = 0;
	xrect_t xr_image, xr_text, xr = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	ximage_t xi = { 0 };
	xcolor_t xc = { 0 };
	const tchar_t *style;
	bool_t b_print;
	float px, py, pw, ph;

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	b_print = (pif->canvas->tag == _CANVAS_PRINTER) ? 1 : 0;

	default_xpen(&xp);
	default_xbrush(&xb);
	default_xfont(&xf);
	default_xface(&xa);

	style = get_status_style_ptr(ptr);

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

	ali_far = (compare_text(get_status_alignment_ptr(ptr), -1, ATTR_ALIGNMENT_FAR, -1, 0) == 0) ? 1 : 0;
	
	ic = get_status_icon_span(ptr);

	if (ali_far)
		hw = pw;
	else
		hw = 0;

	if (ali_far)
	{
		plk = get_status_prev_item(ptr, LINK_LAST);
	}
	else
	{
		plk = get_status_next_item(ptr, LINK_FIRST);
	}

	while (plk)
	{
		iw = get_status_item_width(plk);

		if (ali_far)
		{
			xr.fx = hw - iw + px;
			xr.fy = py;
			xr.fw = iw;
			xr.fh = ph;
		}
		else
		{
			xr.fx = hw + px;
			xr.fy = py;
			xr.fw = iw;
			xr.fh = ph;
		}

		xr_image.fx = xr.fx;
		xr_image.fy = xr.fy + (xr.fh - ic) / 2;
		xr_image.fw = ic;
		xr_image.fh = ic;

		ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);

		(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr_image, get_status_item_icon_ptr(plk));

		xr_text.fx = xr.fx + ic;
		xr_text.fy = xr.fy;
		xr_text.fw = xr.fw - ic;
		xr_text.fh = xr.fh;

		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_status_item_title_ptr(plk), -1);

		if (ali_far)
			hw -= iw;
		else
			hw += iw;

		if (ali_far)
			plk = get_status_prev_item(ptr, plk);
		else
			plk = get_status_next_item(ptr, plk);
	}
}

#endif //XDLVIEW