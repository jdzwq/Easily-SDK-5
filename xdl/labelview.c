/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc label document

	@module	labelview.c | implement file

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
#include "labelview.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdlgdi.h"

#include "xdldoc.h"


#ifdef XDL_SUPPORT_VIEW

int calc_label_items_per_row(link_t_ptr ptr, float pw)
{
	int hs;

	hs = (int)(pw / get_label_item_width(ptr));
	if (hs == 0)
		hs = 1;

	return hs;
}

int calc_label_items_per_col(link_t_ptr ptr, float ph)
{
	int hs;

	hs = (int)(ph / get_label_item_height(ptr));
	if (hs == 0)
		hs = 1;

	return hs;
}

int calc_label_pages(link_t_ptr ptr)
{
	float fw, fh;
	int itemsperpage, items;

	fw = get_label_width(ptr);
	fh = get_label_height(ptr);

	itemsperpage = calc_label_items_per_row(ptr, fw) * calc_label_items_per_col(ptr, fh);

	items = get_label_item_count(ptr);
	if (items == 0)
		return 1;
	if (items % itemsperpage == 0)
		return items / itemsperpage;
	else
		return items / itemsperpage + 1;
}

void calc_label_item_scope(link_t_ptr ptr, int page, link_t_ptr* firstitem, link_t_ptr* lastitem)
{
	float fw, fh;
	int itemsperpage;
	link_t_ptr ilk;

	fw = get_label_width(ptr);
	fh = get_label_height(ptr);

	itemsperpage = calc_label_items_per_row(ptr, fw) * calc_label_items_per_col(ptr, fh);

	*firstitem = get_label_item_at(ptr, (page - 1) * itemsperpage);
	if (*firstitem == NULL)
	{
		*lastitem = NULL;
		return;
	}

	ilk = *firstitem;
	while (ilk && itemsperpage--)
	{
		*lastitem = ilk;
		ilk = get_label_next_item(ptr, ilk);
	}
}

int calc_label_item_page(link_t_ptr ptr, link_t_ptr ilk)
{
	float fw, fh;
	int itemsperpage, page, count;
	link_t_ptr plk;

	if (ilk == NULL)
		return 0;

	fw = get_label_width(ptr);
	fh = get_label_height(ptr);

	itemsperpage = calc_label_items_per_row(ptr, fw) * calc_label_items_per_col(ptr, fh);

	page = 1;
	count = itemsperpage;
	plk = get_label_next_item(ptr, LINK_FIRST);
	while (plk && plk != ilk)
	{
		count--;
		if (count == 0)
		{
			page++;
			count = itemsperpage;
		}
		plk = get_label_next_item(ptr, plk);
	}
	return page;
}

bool_t calc_label_item_rect(link_t_ptr ptr, int page, link_t_ptr ilk, xrect_t* pxr)
{
	link_t_ptr nlk,first,last;
	int per, count;
	float fw, fh, iw, ih;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	fw = get_label_width(ptr);
	fh = get_label_height(ptr);
	iw = get_label_item_width(ptr);
	ih = get_label_item_height(ptr);

	per = calc_label_items_per_row(ptr, fw);

	calc_label_item_scope(ptr, page, &first, &last);

	count = 0;
	nlk = first;
	while (nlk)
	{
		pxr->fx = iw * (count % per);
		pxr->fy = ih * (count / per);

		if (nlk == ilk)
		{
			pxr->fw = iw;
			pxr->fh = ih;
			return 1;
		}

		if (nlk == last)
			break;

		count++;
		nlk = get_label_next_item(ptr, nlk);
	}

	return 0;
}

int	calc_label_hint(const xpoint_t* ppt, link_t_ptr ptr, int page, link_t_ptr* pilk)
{
	link_t_ptr nlk, first, last;
	int per, count;
	float fw, fh, iw, ih;
	xrect_t xr;

	*pilk = NULL;

	fw = get_label_width(ptr);
	fh = get_label_height(ptr);
	iw = get_label_item_width(ptr);
	ih = get_label_item_height(ptr);
	
	per = calc_label_items_per_row(ptr, fw);

	xr.fx = 0;
	xr.fy = 0;
	xr.fw = iw;
	xr.fh = ih;

	calc_label_item_scope(ptr, page, &first, &last);

	count = 0;
	nlk = first;
	while (nlk)
	{
		xr.fx = iw * (count % per);
		xr.fy = ih * (count / per);

		if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
		{
			*pilk = nlk;

			return LABEL_HINT_ITEM;
		}

		if (nlk == last)
			break;

		count++;
		nlk = get_label_next_item(ptr, nlk);
	}

	return LABEL_HINT_NONE;
}

void draw_label(const if_drawing_t* pif, link_t_ptr ptr, int page)
{
	link_t_ptr nlk, first, last;
	link_t_ptr ilk;
	int per, count;
	float iw, ih, ew, eh;
	xrect_t xr, xr_shape, xr_image, xr_text;
	xsize_t xs;
	xbrush_t xb_shape, xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa_title, xa = { 0 };
	xcolor_t xc = { 0 };
	const tchar_t* style;
	const tchar_t* type;
	bool_t b_print;
	float SHAPE_FEED = 1.0;
	float px, py, pw, ph;

	string_t vs_text;
	link_t_ptr st_table;
	link_t_ptr st_array;
	ximage_t xi_ico = { 0 };
	ximage_t xi = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	b_print = (pif->tag == _CANVAS_PRINTER) ? 1 : 0;

	default_xpen(&xp);
	default_xbrush(&xb);
	default_xfont(&xf);
	default_xface(&xa);

	style = get_label_style_ptr(ptr);

	parse_xface_from_style(&xa, style);

	parse_xfont_from_style(&xf, style);
	if (!b_print)
	{
		format_xcolor(&pif->mode.clr_txt, xf.color);
	}

	parse_xpen_from_style(&xp, style);
	if (!b_print)
	{
		format_xcolor(&pif->mode.clr_frg, xp.color);
	}

	parse_xbrush_from_style(&xb, style);
	if (!b_print)
	{
		format_xcolor(&pif->mode.clr_bkg, xb.color);
	}

	if (!b_print)
	{
		format_xcolor(&pif->mode.clr_msk, xi.color);
		format_xcolor(&pif->mode.clr_msk, xi_ico.color);
	}

	if (!b_print)
	{
		xmem_copy((void*)&xc, (void*)&pif->mode.clr_ico, sizeof(xcolor_t));
	}
	else
	{
		parse_xcolor(&xc, xp.color);
	}

	xmem_copy((void*)&xa_title, (void*)&xa, sizeof(xface_t));

	xmem_copy((void*)&xb_shape, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_shape, DEF_SOFT_DARKEN);

	(*pif->pf_text_metric)(pif->ctx, &xf, &xs);

	iw = get_label_item_width(ptr);
	ih = get_label_item_height(ptr);
	eh = (float)(xs.fh * 1.25);
	ew = (iw - 2 * SHAPE_FEED) / 2;

	per = calc_label_items_per_row(ptr, pw);

	xr.fw = iw;
	xr.fh = ih;

	calc_label_item_scope(ptr, page, &first, &last);

	count = 0;
	nlk = first;
	while (nlk)
	{
		type = get_label_item_type_ptr(nlk);

		xr.fx = iw * (count % per) + px;
		xr.fy = ih * (count / per) + py;

		xscpy(xa_title.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		xr_text.fx = xr.fx + eh + SHAPE_FEED;
		xr_text.fy = xr.fy + SHAPE_FEED;
		xr_text.fw = xr.fw - 2 * eh - 2 * SHAPE_FEED;
		xr_text.fh = eh;
		(*pif->pf_draw_text)(pif->ctx, &xf, &xa_title, &xr_text, get_label_item_title_ptr(nlk), -1);

		xr_text.fx = xr.fx + xr.fw - SHAPE_FEED - eh;
		xr_text.fy = xr.fy + SHAPE_FEED;
		xr_text.fw = eh;
		xr_text.fh = eh;

		ft_center_rect(&xr_text, DEF_SMALL_ICON, DEF_SMALL_ICON);
		draw_gizmo(pif, &xc, &xr_text, get_label_item_icon_ptr(nlk));
		
		xr_shape.fx = xr.fx + SHAPE_FEED;
		xr_shape.fy = xr.fy + SHAPE_FEED;
		xr_shape.fw = xr.fw - 2 * SHAPE_FEED;
		xr_shape.fh = xr.fh - 2 * SHAPE_FEED;
		(*pif->pf_draw_round)(pif->ctx, &xp, NULL, &xr_shape);

		xr_shape.fy = xr.fy + SHAPE_FEED + eh;
		xr_shape.fh = xr.fh - 2 * SHAPE_FEED - eh;
		(*pif->pf_draw_rect)(pif->ctx, &xp, &xb_shape, &xr_shape);

		if (compare_text(type, -1, ATTR_TEXT_TYPE_TEXT, -1, 0) == 0)
		{
			xr_text.fx = xr.fx + SHAPE_FEED;
			xr_text.fy = xr.fy + SHAPE_FEED + eh;
			xr_text.fw = xr.fw - 2 * SHAPE_FEED;
			xr_text.fh = xr.fh - 2 * SHAPE_FEED - eh;

			vs_text = string_alloc();
			string_cpy(vs_text, get_label_item_text_ptr(nlk), -1);

			draw_var_text(pif, &xf, &xa, &xr_text, vs_text);

			string_free(vs_text);
		}
		else if (compare_text(type, -1, ATTR_TEXT_TYPE_IMAGE, -1, 0) == 0)
		{
			xr_image.fx = xr.fx + SHAPE_FEED;
			xr_image.fy = xr.fy + SHAPE_FEED + eh;
			xr_image.fw = xr.fw - 2 * SHAPE_FEED;
			xr_image.fh = xr.fh - 2 * SHAPE_FEED - eh;

			memset((void*)&xi, 0, sizeof(ximage_t));

			parse_ximage_from_source(&xi, get_label_item_text_ptr(nlk));

			(*pif->pf_draw_image)(pif->ctx, &xi, &xr_image);
		}
		else if (compare_text(type, -1, ATTR_TEXT_TYPE_WORDSTABLE, -1, 0) == 0)
		{
			st_array = create_words_table(0);
			words_table_parse_tokens(st_array, get_label_item_text_ptr(nlk), -1, _T('\n'));

			xr_text.fy = xr.fy + SHAPE_FEED + eh;
			ilk = get_words_next_item(st_array, LINK_FIRST);
			while (ilk)
			{
				xr_text.fx = xr.fx + SHAPE_FEED;
				xr_text.fw = iw - 2 * SHAPE_FEED;
				xr_text.fh = eh;

				(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr_text, get_words_item_text_ptr(ilk), -1);

				xr_text.fy += eh;
				ilk = get_words_next_item(st_array, ilk);
			}

			destroy_words_table(st_array);
		}
		else if (compare_text(type, -1, ATTR_TEXT_TYPE_STRINGTABLE, -1, 0) == 0)
		{ 
			st_table = create_string_table(0);
			string_table_parse_options(st_table, get_label_item_text_ptr(nlk), -1, OPT_ITEMFEED, OPT_LINEFEED);

			xmem_copy((void*)&xa_title, (void*)&xa, sizeof(xface_t));
			xscpy(xa_title.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

			xr_text.fy = xr.fy + SHAPE_FEED + eh;
			ilk = get_string_next_entity(st_table, LINK_FIRST);
			while (ilk)
			{
				xr_text.fx = xr.fx + SHAPE_FEED;
				xr_text.fw = ew;
				xr_text.fh = eh;

				(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr_text, get_string_entity_key_ptr(ilk), -1);

				xr_text.fx = xr.fx + SHAPE_FEED + ew;
				xr_text.fw = ew;
				xr_text.fh = eh;

				(*pif->pf_draw_text)(pif->ctx, &xf, &xa_title, &xr_text, get_string_entity_val_ptr(ilk), -1);

				xr_text.fy += eh;
				ilk = get_string_next_entity(st_table, ilk);
			}

			destroy_string_table(st_table);
		}

		if (nlk == last)
			break;

		count++;
		nlk = get_label_next_item(ptr, nlk);
	}
}

#endif //XDLVIEW