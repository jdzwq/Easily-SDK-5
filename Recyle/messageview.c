/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc message document

	@module	messageview.c | message view implement file

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
#include "messageview.h"
#include "imageview.h"
#include "messagedoc.h"
#include "xdlbas.h"

#ifdef _XDLVIEW

void calc_message_suit_size(link_t_ptr ptr, xsize_t* pxs)
{
	float ic, iw, ih, w, h;
	const tchar_t* style;
	xfont_t xf = { 0 };
	link_t_ptr ilk;
	int count = 0;

	style = get_message_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	ih = get_message_title_height(ptr);
	ic = get_message_icon_span(ptr);

	w = h = 0;
	ilk = get_message_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				if (w < iw + ic)
					w = iw + ic;

				h += ((ic > ih) ? ic : ih);
			}
		}

		ilk = get_message_next_item(ptr, ilk);

		if (count++ > 7)
			break;
	}

	pxs->fx = w;
	pxs->fy = h;
}

float calc_message_height(link_t_ptr ptr)
{
	float ic, iw, ih, h;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	link_t_ptr ilk;

	style = get_message_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_message_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_message_show_ptr(ptr);
	ic = get_message_icon_span(ptr);

	h = 0;
	ilk = get_message_next_item(ptr, LINK_FIRST);
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
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);

				if (h < ih)
					h = ih;
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
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);

				h += ((ic > ih) ? ic : ih);
			}
		}

		ilk = get_message_next_item(ptr, ilk);
	}

	return h;
}

float calc_message_width(link_t_ptr ptr)
{
	float ic, iw, ih, w;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	link_t_ptr ilk;

	style = get_message_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_message_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_message_show_ptr(ptr);
	ic = get_message_icon_span(ptr);

	w = 0;
	ilk = get_message_next_item(ptr, LINK_FIRST);
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
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				w += iw;

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
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				if (w < iw + ic)
					w = iw + ic;
			}
		}

		ilk = get_message_next_item(ptr, ilk);
	}

	return w;
}

void calc_message_item_rect(link_t_ptr ptr, link_t_ptr plk, xrect_t* pxr)
{
	float ic, iw, ih, w, h;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	link_t_ptr ilk;

	memset((void*)pxr, 0, sizeof(xrect_t));

	style = get_message_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_message_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_message_show_ptr(ptr);
	ic = get_message_icon_span(ptr);

	w = h = 0;
	ilk = get_message_next_item(ptr, LINK_FIRST);
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
			h = ic;

			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w += ic;
			}
			else
			{
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				w += iw;

				if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
					w += ic;

				if (h < ih)
					h = ih;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h += ic;
				w = ic;
			}
			else
			{
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				if (w < iw + ic)
					w = iw + ic;

				h += ((ic > ih) ? ic : ih);
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

		ilk = get_message_next_item(ptr, ilk);
	}
}

int	calc_message_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	int hint = MESSAGE_HINT_NONE;
	float ic, iw, ih, w, h;
	bool_t b_horz;
	const tchar_t* show;
	const tchar_t* style;
	xfont_t xf = { 0 };
	xrect_t xr = { 0 };
	link_t_ptr ilk;

	*pilk = NULL;

	style = get_message_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	b_horz = (compare_text(get_message_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	show = get_message_show_ptr(ptr);
	ic = get_message_icon_span(ptr);

	w = h = 0;
	ilk = get_message_next_item(ptr, LINK_FIRST);
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
			h = ic;

			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w += ic;
			}
			else
			{
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				w += iw;

				if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
					w += ic;

				if (h < ih)
					h = ih;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h += ic;
				w = ic;
			}
			else
			{
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				if (w < iw + ic)
					w = iw + ic;

				h += ((ic > ih) ? ic : ih);
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
			hint = MESSAGE_HINT_ITEM;
			break;
		}

		ilk = get_message_next_item(ptr, ilk);
	}

	return hint;
}

void draw_message(const if_canvas_t* pif, link_t_ptr ptr)
{
	link_t_ptr ilk;
	float iw, ih, ic, w, h;
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
	link_t_ptr imagelist;

	const xrect_t* pbox;

	pbox = &pif->rect;

	b_horz = (compare_text(get_message_layer_ptr(ptr), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	imagelist = get_message_imagelist(ptr);

	show = get_message_show_ptr(ptr);

	default_xfont(&xf);
	default_xface(&xa);

	style = get_message_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);

	parse_xface_from_style(&xa, style);
	format_xcolor(&pif->clr_txt, xa.text_color);

	parse_xpen_from_style(&xp, style);
	format_xcolor(&pif->clr_frg, xp.color);

	parse_xcolor(&xc, xp.color);

	format_xcolor(&pif->clr_msk, xi.color);

	ic = get_message_icon_span(ptr);
	
	w = pbox->fx;
	h = pbox->fy;

	ilk = get_message_next_item(ptr, LINK_FIRST);
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
			h = pbox->fh;

			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				w += ic;
			}
			else
			{
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				w += iw;

				if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) != 0)
					w += ic;

				if (h < ih)
					h = ih;
			}
		}
		else
		{
			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				h += ic;
				w = ic;
			}
			else
			{
				text_mm_size(&xf, get_message_item_title_ptr(ilk), -1, &iw, &ih);
				iw += MESSAGE_FEED;

				if (w < iw + ic)
					w = iw + ic;

				h += ((ic > ih) ? ic : ih);
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
			xr_image.fh = ic;

			if (get_message_item_checked(ilk))
			{
				(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr_image, ATTR_ICON_RADIOED);
			}
			else
			{
				if (imagelist)
				{
					get_ximage_from_image_list(imagelist, get_message_item_image_ptr(ilk), &xi);
					(*pif->pf_draw_image)(pif->canvas, &xi, &xr_image);
				}
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

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_text, get_message_item_title_ptr(ilk), -1);
		}

		ilk = get_message_next_item(ptr, ilk);
	}

}

#endif //XDLVIEW