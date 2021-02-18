/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc menu document

	@module	menuview.c | implement file

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
#include "menuview.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdlgdi.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_VIEW

#define MENU_FEED  (float)2

float calc_menu_height(const measure_interface* pif, link_t_ptr ptr)
{
	float ic, h;
	xsize_t xs;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	link_t_ptr ilk;

	style = get_menu_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_menu_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_menu_show_ptr(ptr);
	ic = get_menu_icon_span(ptr);

	h = 0;
	ilk = get_menu_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
		if (b_horz)
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h = ic;
			}
			else
			{
				h = ic;
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				if (h < xs.fh)
					h = xs.fh;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h += ic;
			}
			else
			{
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				h += ((ic > xs.fh) ? ic : xs.fh);
			}
		}

		ilk = get_menu_next_item(ptr, ilk);
	}

	return h;
}

float calc_menu_width(const measure_interface* pif, link_t_ptr ptr)
{
	float ic,w;
	xsize_t xs;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	link_t_ptr ilk;

	style = get_menu_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_menu_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_menu_show_ptr(ptr);
	ic = get_menu_icon_span(ptr);

	w = 0;
	ilk = get_menu_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
		if (b_horz)
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w += ic;
			}
			else
			{
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				w += (xs.fw + MENU_FEED);

				if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
					w += ic;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w = ic;
			}
			else
			{
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);
				xs.fw += (MENU_FEED + ic);

				if (w < xs.fw)
					w = xs.fw;
			}
		}

		ilk = get_menu_next_item(ptr, ilk);
	}

	return w;
}

void calc_menu_item_rect(const measure_interface* pif, link_t_ptr ptr, link_t_ptr plk, xrect_t* pxr)
{
	float fw, fh, ic, w, h;
	xsize_t xs;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	link_t_ptr ilk;

	memset((void*)pxr, 0, sizeof(xrect_t));

	style = get_menu_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_menu_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_menu_show_ptr(ptr);
	ic = get_menu_icon_span(ptr);
	fw = pif->rect.fw;
	fh = pif->rect.fh;

	if (b_horz)
	{
		w = 0;
		h = fh;
	}
	else
	{
		w = fw;
		h = 0;
	}

	ilk = get_menu_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
		if (ilk == plk)
		{
			if (b_horz)
			{
				pxr->fx = w;
				pxr->fy = 0;
			}
			else
			{
				pxr->fx = 0;
				pxr->fy = h;
			}
		}

		if (b_horz)
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w += ic;
			}
			else
			{
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				w += (xs.fw + MENU_FEED);

				if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
					w += ic;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h += ic;
			}
			else
			{
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				h += ((ic > xs.fh) ? ic : xs.fh);
			}
		}

		if (ilk == plk)
		{
			if (b_horz)
			{
				pxr->fw = w - pxr->fx;
				pxr->fh = h;
			}
			else
			{
				pxr->fw = w;
				pxr->fh = h - pxr->fy;
			}

			break;
		}

		ilk = get_menu_next_item(ptr, ilk);
	}
}

int	calc_menu_hint(const measure_interface* pif, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pilk)
{
	int hint = MENU_HINT_NONE;
	float fw, fh, ic, w, h;
	xsize_t xs;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	xrect_t xr = { 0 };
	link_t_ptr ilk;

	*pilk = NULL;

	style = get_menu_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_menu_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_menu_show_ptr(ptr);
	ic = get_menu_icon_span(ptr);
	fw = pif->rect.fw;
	fh = pif->rect.fh;

	if (b_horz)
	{
		w = 0;
		h = fh;
	}
	else
	{
		w = fw;
		h = 0;
	}

	ilk = get_menu_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
		if (b_horz)
		{
			xr.fx = w;
			xr.fy = 0;
		}
		else
		{
			xr.fx = 0;
			xr.fy = h;
		}

		if (b_horz)
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w += ic;
			}
			else
			{
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				w += (xs.fw + MENU_FEED);

				if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
					w += ic;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h += ic;
			}
			else
			{
				(*pif->pf_measure_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				h += ((ic > xs.fh) ? ic : xs.fh);
			}
		}

		if (b_horz)
		{
			xr.fw = w - xr.fx;
			xr.fh = h;
		}
		else
		{
			xr.fw = w;
			xr.fh = h - xr.fy;
		}

		if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
		{
			*pilk = ilk;
			hint = MENU_HINT_ITEM;
			break;
		}

		ilk = get_menu_next_item(ptr, ilk);
	}

	return hint;
}

void draw_menu(const drawing_interface* pif, link_t_ptr ptr)
{
	link_t_ptr ilk;
	float ic, w, h;
	xsize_t xs;
	int count = 0;
	xrect_t xr = { 0 };
	xrect_t xr_image, xr_text;
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };
	const tchar_t* style;
	bool_t b_horz;
	const tchar_t* show;

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	b_horz = (compare_text(get_menu_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_menu_show_ptr(ptr);

	default_xpen(&xp);
	default_xfont(&xf);
	default_xface(&xa);

	style = get_menu_style_ptr(ptr);

	parse_xface_from_style(&xa, style);

	parse_xfont_from_style(&xf, style);
	format_xcolor(&pif->mode.clr_txt, xf.color);

	parse_xpen_from_style(&xp, style);
	format_xcolor(&pif->mode.clr_frg, xp.color);

	format_xcolor(&pif->mode.clr_msk, xi.color);

	xmem_copy((void*)&xc, (void*)&pif->mode.clr_ico, sizeof(xcolor_t));

	ic = get_menu_icon_span(ptr);
	
	if (b_horz)
	{
		w = pbox->fx;
		h = pbox->fh;
	}
	else
	{
		w = pbox->fw;
		h = pbox->fy;
	}

	ilk = get_menu_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
		if (b_horz)
		{
			xr.fx = w;
			xr.fy = pbox->fy;
		}
		else
		{
			xr.fx = pbox->fx;
			xr.fy = h;
		}

		if (b_horz)
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w += ic;
			}
			else
			{
				(*pif->pf_text_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				w += (xs.fw + MENU_FEED);

				if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
					w += ic;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h += ic;
			}
			else
			{
				(*pif->pf_text_size)(pif->ctx, &xf, get_menu_item_title_ptr(ilk), -1, &xs);

				h += ((ic > xs.fh) ? ic : xs.fh);
			}
		}

		if (b_horz)
		{
			xr.fw = w - xr.fx;
			xr.fh = h;
		}
		else
		{
			xr.fw = w;
			xr.fh = h - xr.fy;
		}

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

		if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
		{
			xr_image.fx = xr.fx;
			xr_image.fw = ic;
			xr_image.fy = xr.fy;
			xr_image.fh = xr.fh;

			ft_center_rect(&xr_image, DEF_SMALL_ICON, DEF_SMALL_ICON);

			if (get_menu_item_checked(ilk))
			{
				draw_gizmo(pif, &xc, &xr_image, GDI_ATTR_GIZMO_CHECKED);
			}
			else
			{
				draw_gizmo(pif, &xc, &xr_image, get_menu_item_icon_ptr(ilk));
			}
		}
		
		if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) != 0)
		{
			if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) == 0)
			{
				xr_text.fx = xr.fx;
				xr_text.fw = xr.fw;
				xr_text.fy = xr.fy;
				xr_text.fh = xr.fh;
			}
			else
			{
				xr_text.fx = xr.fx + ic;
				xr_text.fw = xr.fw - ic;
				xr_text.fy = xr.fy;
				xr_text.fh = xr.fh;
			}

			(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr_text, get_menu_item_title_ptr(ilk), -1);
		}

		ilk = get_menu_next_item(ptr, ilk);
	}

}

#endif //XDLVIEW