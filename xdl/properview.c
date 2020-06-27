/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc property view

	@module	properview.c | implement file

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
#include "properview.h"
#include "xdldoc.h"
#include "xdlimp.h"

#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

float calc_proper_height(const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr slk, elk;
	float th,total = 0;

	th = get_proper_item_height(ptr);

	slk = get_next_section(ptr, LINK_FIRST);
	while (slk)
	{
		total += th;

		if (get_section_expanded(slk))
		{
			elk = get_next_entity(slk, LINK_FIRST);
			while (elk)
			{
				total += th;
				elk = get_next_entity(slk, elk);
			}
		}

		slk = get_next_section(ptr, slk);
	}

	return total;
}

float calc_proper_width(const canvbox_t* pbox, link_t_ptr ptr)
{
	float iw;

	iw = get_proper_item_span(ptr);

	return iw * 2;
}

void calc_proper_section_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr sec, xrect_t* pxr)
{
	link_t_ptr slk, elk;
	float pw, iw, th, total = 0;

	pw = pbox->fw;
	iw = get_proper_item_span(ptr);
	if (pw < 2 * iw)
	{
		pw = iw * 2;
	}

	xmem_zero((void*)pxr, sizeof(xrect_t));

	pw = pbox->fw;
	th = get_proper_item_height(ptr);

	slk = get_next_section(ptr, LINK_FIRST);
	while (slk && slk != sec)
	{
		total += th;

		if (get_section_expanded(slk))
		{
			elk = get_next_entity(slk, LINK_FIRST);
			while (elk)
			{
				total += th;

				elk = get_next_entity(slk, elk);
			}
		}

		slk = get_next_section(ptr, slk);
	}

	pxr->fx =0;
	pxr->fw = pw;
	pxr->fy = total;
	pxr->fh = th;
}

void calc_proper_entity_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ent, xrect_t* pxr)
{
	link_t_ptr slk, elk = NULL;
	float pw, iw, th, total = 0;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	pw = pbox->fw;
	th = get_proper_item_height(ptr);
	iw = get_proper_item_span(ptr);
	if (pw < 2 * iw)
	{
		pw = iw * 2;
	}

	slk = get_next_section(ptr, LINK_FIRST);
	while (slk)
	{
		total += th;
		if (get_section_expanded(slk))
		{
			elk = get_next_entity(slk, LINK_FIRST);
			while (elk && elk != ent)
			{
				total += th;
				elk = get_next_entity(slk, elk);
			}
			if (elk)
				break;
		}
		slk = get_next_section(ptr, slk);
	}

	if (elk == ent)
	{
		pxr->fx = 0;
		pxr->fw = pw;
		pxr->fy = total;
		pxr->fh = th;
	}
}

void calc_proper_entity_text_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ent, xrect_t* pxr)
{
	float ew;

	ew = get_proper_item_span(ptr);
	calc_proper_entity_rect( pbox, ptr,ent, pxr);
	pxr->fx += ew;
	pxr->fw -= ew;
}

int calc_proper_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* psec, link_t_ptr* pent)
{
	int hint, b_find = 0;
	float x1, y1, x2, y2, xp, yp;
	float pw, th, ew;
	link_t_ptr slk, elk;

	xp = ppt->fx;
	yp = ppt->fy;

	*psec = NULL;
	*pent = NULL;
	hint = PROPER_HINT_NONE;

	pw = pbox->fw;
	th = get_proper_item_height(ptr);
	ew = get_proper_item_span(ptr);
	if (pw < 2 * ew)
	{
		pw = ew * 2;
	}

	x1 = 0;
	x2 = pw;
	y2 = 0;

	slk = get_next_section(ptr, LINK_FIRST);
	while (slk)
	{
		y1 = y2;
		y2 = y1 + th;
		if (ft_inside(xp, yp, x1, y1, x2, y2))
		{
			*psec = slk;
			hint = PROPER_HINT_SECTION;
			break;
		}

		y1 = y2;
		if (get_section_expanded(slk))
		{
			b_find = 0;
			elk = get_next_entity(slk, LINK_FIRST);
			while (elk)
			{
				y2 = y1 + th;
				if (ft_inside(xp, yp, x1, y1, x2, y2))
				{
					*pent = elk;
					if (xp >= x1 + ew - 5 && xp <= x1 + ew + 5)
						hint = PROPER_HINT_VERT_SPLIT;
					else
						hint = PROPER_HINT_ENTITY;

					b_find = 1;
					break;
				}
				y1 = y2;
				elk = get_next_entity(slk, elk);
			}
		}

		if (b_find)
			break;

		slk = get_next_section(ptr, slk);
	}

	return hint;
}

void draw_proper(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr sec, ent;
	xrect_t xr, xr_draw;
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xgradi_t xg = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	ximage_t xi = { 0 };
	xcolor_t xc = { 0 };
	float ic, iw, ih;
	const tchar_t *style, *shape;
	bool_t b_print;
	float px, py, pw, ph;

	XDL_ASSERT(pif);

	b_print = (pif->canvas->tag == _CANVAS_PRINTER) ? 1 : 0;

	ih = get_proper_item_height(ptr);
	iw = get_proper_item_span(ptr);
	ic = get_proper_icon_span(ptr);

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	default_xfont(&xf);
	default_xface(&xa);
	default_xgradi(&xg);

	style = get_proper_style_ptr(ptr);

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

	xscpy(xg.brim_color, xb.color);
	xscpy(xg.core_color, xb.color);
	lighten_xgradi(&xg, DEF_SOFT_DARKEN);

	shape = get_proper_shape_ptr(ptr);

	xr.fx = px;
	xr.fy = py;
	xr.fw = pw;
	xr.fh = 0;

	sec = get_next_section(ptr, LINK_FIRST);
	while (sec)
	{
		xr_draw.fx = xr.fx;
		xr_draw.fy = xr.fy;
		xr_draw.fw = xr.fw;
		xr_draw.fh = ih;

		(*pif->pf_gradient_rect)(pif->canvas, &xg, &xr_draw);

		if (is_null(shape))
		{
			(*pif->pf_draw_rect)(pif->canvas, NULL, NULL, &xr_draw);
		}
		else
		{
			(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr_draw);
		}

		xr_draw.fx = xr.fx;
		xr_draw.fw = ic;
		xr_draw.fy = xr.fy;
		xr_draw.fh = ih;

		ft_center_rect(&xr_draw, DEF_SMALL_ICON, DEF_SMALL_ICON);

		(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr_draw, get_section_icon_ptr(sec));

		xr_draw.fx = xr.fx + xr.fw - ic;
		xr_draw.fw = ic;
		xr_draw.fy = xr.fy;
		xr_draw.fh = ih;
		ft_center_rect(&xr_draw, DEF_SMALL_ICON, DEF_SMALL_ICON);

		if (get_section_expanded(sec))
		{
			(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr_draw, GDI_ATTR_GIZMO_EXPAND);
		}
		else
		{
			(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr_draw, GDI_ATTR_GIZMO_COLLAPSE);
		}

		xr_draw.fx = xr.fx + ic;
		xr_draw.fw = xr.fw - ic;
		xr_draw.fy = xr.fy;
		xr_draw.fh = ih;

		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_draw, get_section_name_ptr(sec), -1);

		xr.fy += ih;

		if (!get_section_expanded(sec))
		{
			sec = get_next_section(ptr, sec);
			continue;
		}

		ent = get_next_entity(sec, LINK_FIRST);
		while (ent)
		{
			//key shape
			xr_draw.fx = xr.fx;
			xr_draw.fw = iw;
			xr_draw.fy = xr.fy;
			xr_draw.fh = ih;

			if (!is_null(shape))
			{
				(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr_draw, shape);
			}

			//val shape
			xr_draw.fx = xr.fx + iw;
			xr_draw.fw = pw - iw;
			xr_draw.fy = xr.fy;
			xr_draw.fh = ih;

			if (!is_null(shape))
			{
				(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr_draw, shape);
			}

			//key image
			xr_draw.fx = xr.fx;
			xr_draw.fw = ic;
			xr_draw.fy = xr.fy;
			xr_draw.fh = ih;

			ft_center_rect(&xr_draw, DEF_SMALL_ICON, DEF_SMALL_ICON);

			(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr_draw, get_entity_icon_ptr(ent));

			//key text
			xr_draw.fx = xr.fx + ic;
			xr_draw.fw = iw - ic;
			xr_draw.fy = xr.fy;
			xr_draw.fh = ih;

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_draw, get_entity_name_ptr(ent), -1);

			//val text
			xr_draw.fx = xr.fx + iw;
			xr_draw.fw = pw - iw;
			xr_draw.fy = xr.fy;
			xr_draw.fh = ih;

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_draw, get_entity_options_text_ptr(ent), -1);

			xr.fy += ih;
			ent = get_next_entity(sec, ent);
		}

		sec = get_next_section(ptr, sec);
	}
}

#endif //XDLVIEW