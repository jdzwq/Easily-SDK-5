/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tool view

	@module	toolview.c | implement file

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
#include "toolview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define TOOLGROUP_SPLIT		(float)1

/***************************************************************************************************/
int _calc_tool_group_cols(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr glk)
{
	int n, count;
	float th, ih;
	const tchar_t* show;

	show = get_tool_group_show_ptr(glk);

	ih = get_tool_group_item_height(glk);
	th = get_tool_title_height(ptr);

	count = get_tool_group_item_count(glk);

	if (is_null(show))
	{
		return count;
	}

	n = (int)((pbox->fh - th - 2 * TOOLGROUP_SPLIT) / ih);

	if (n < 1)
		n = 1;
	else if (n < 0)
		n = 0;

	if (n)
	{
		if (count % n)
			n = count / n + 1;
		else
			n = count / n;
	}

	return n;
}

int _calc_tool_group_rows(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr glk)
{
	int n, count;
	float th, ih;
	const tchar_t* show;

	show = get_tool_group_show_ptr(glk);

	if (is_null(show))
	{
		return 1;
	}

	th = get_tool_title_height(ptr);
	ih = get_tool_group_item_height(glk);

	count = get_tool_group_item_count(glk);

	n = (int)((pbox->fh - th - 2 * TOOLGROUP_SPLIT) / ih);

	if (n < 0)
		n = 0;
	else if (n < 1)
		n = 1;

	return n;
}

float calc_tool_group_width(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr glk)
{
	return _calc_tool_group_cols(pbox, ptr, glk) * get_tool_group_item_width(glk) + 2 * TOOLGROUP_SPLIT;
}

float calc_tool_group_height(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr glk)
{
	return pbox->fh - 2 * TOOLGROUP_SPLIT;
	//return _calc_tool_group_rows(ptr, glk) * get_tool_group_item_height(glk) + get_tool_title_height(ptr) + 2 * TOOLGROUP_SPLIT;
}

float calc_tool_width(const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr glk;
	float w = 0;

	glk = get_tool_next_group(ptr, LINK_FIRST);
	while (glk)
	{
		w += calc_tool_group_width(pbox, ptr, glk);
		glk = get_tool_next_group(ptr, glk);
	}

	return w;
}


void calc_tool_group_entire_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr glk, xrect_t* pxr)
{
	link_t_ptr plk;

	pxr->fx = 0;
	pxr->fy = 0;

	plk = get_tool_next_group(ptr, LINK_FIRST);
	while (plk)
	{
		pxr->fw = calc_tool_group_width(pbox, ptr, plk);
		pxr->fh = calc_tool_group_height(pbox, ptr, plk);

		if (plk == glk)
		{
			break;
		}

		pxr->fx += pxr->fw;

		plk = get_tool_next_group(ptr, plk);
	}
}

void calc_tool_group_title_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr glk, xrect_t* pxr)
{
	calc_tool_group_entire_rect(pbox, ptr, glk, pxr);

	pxr->fy = pxr->fy + pxr->fh - get_tool_title_height(ptr);
	pxr->fh = get_tool_title_height(ptr);
}

void calc_tool_group_item_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	link_t_ptr plk, glk;
	xrect_t xr = { 0 };
	int rows, cols, index;

	XDL_ASSERT(pxr != NULL);

	glk = get_dom_parent_node(ilk);

	calc_tool_group_entire_rect(pbox, ptr, glk, &xr);
	xr.fx += TOOLGROUP_SPLIT;
	xr.fw -= 2 * TOOLGROUP_SPLIT;
	xr.fy += TOOLGROUP_SPLIT;
	xr.fh -= (get_tool_title_height(ptr) + 2 * TOOLGROUP_SPLIT);

	index = -1;
	plk = get_tool_group_next_item(glk, LINK_FIRST);
	while (plk)
	{
		index++;

		if (plk == ilk)
			break;

		plk = get_tool_group_next_item(glk, plk);
	}

	if (index < 0 || index >= get_tool_group_item_count(glk))
		return;

	rows = _calc_tool_group_rows(pbox, ptr, glk);
	cols = _calc_tool_group_cols(pbox, ptr, glk);

	ft_cell_rect(&xr, rows, cols, index);
	xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
}

int calc_tool_point_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pplk)
{
	link_t_ptr glk, ilk;
	int nHint;
	int rows, cols;
	xrect_t xrGroup, xrTitle, xrItem;
	float xm, ym;

	XDL_ASSERT(ppt != NULL && pplk != NULL);

	nHint = TOOL_HINT_NONE;
	*pplk = NULL;

	xm = ppt->fx;
	ym = ppt->fy;

	glk = get_tool_next_group(ptr, LINK_FIRST);
	while (glk)
	{
		rows = _calc_tool_group_rows(pbox, ptr, glk);
		cols = _calc_tool_group_cols(pbox, ptr, glk);

		calc_tool_group_entire_rect(pbox, ptr, glk, &xrGroup);
		calc_tool_group_title_rect(pbox, ptr, glk, &xrTitle);

		if (ft_inside(xm, ym, xrTitle.fx, xrTitle.fy, xrTitle.fx + xrTitle.fw, xrTitle.fy + xrTitle.fh))
		{
			nHint = TOOL_HINT_TITLE;

			return nHint;
		}
		else if (ft_inside(xm, ym, xrGroup.fx, xrGroup.fy, xrGroup.fx + xrGroup.fw, xrGroup.fy + xrGroup.fh))
		{
			nHint = TOOL_HINT_GROUP;

			ilk = get_tool_group_next_item(glk, LINK_FIRST);
			while (ilk)
			{
				calc_tool_group_item_rect(pbox, ptr, ilk, &xrItem);

				if (ft_inside(xm, ym, xrItem.fx, xrItem.fy, xrItem.fx + xrItem.fw, xrItem.fy + xrItem.fh))
				{
					nHint = TOOL_HINT_ITEM;
					*pplk = ilk;
					return nHint;
				}
				ilk = get_tool_group_next_item(glk, ilk);
			}

			return nHint;
		}

		glk = get_tool_next_group(ptr, glk);
	}

	return nHint;
}

void draw_tool(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr glk, ilk;
	xrect_t xrGroup, xrTitle, xrItem, xrClip;
	xrect_t xr = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	ximage_t xi = { 0 };
	xcolor_t xc = { 0 };
	xbrush_t xb_group;
	const tchar_t *style;
	bool_t b_print;
	link_t_ptr imagelist;
	float px, py, pw, ph;
	const tchar_t* show;

	XDL_ASSERT(pif != NULL);

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	b_print = ((*pif->pf_canvas_type)(pif->canvas) == _CANV_PRINTER) ? 1 : 0;

	imagelist = get_tool_images(ptr);

	style = get_tool_style_ptr(ptr);

	default_xfont(&xf);
	default_xface(&xa);

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

	xmem_copy((void*)&xb_group, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb, DEF_SOFT_LIGHTEN);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	glk = get_tool_next_group(ptr, LINK_FIRST);
	while (glk)
	{
		if (get_tool_group_item_count(glk))
		{
			calc_tool_group_entire_rect(pbox, ptr, glk, &xrGroup);
			ft_offset_rect(&xrGroup, px, py);
			ft_expand_rect(&xrGroup, -DEF_SPLIT_FEED, -DEF_SPLIT_FEED);

			(*pif->pf_draw_round)(pif->canvas, &xp, &xb_group, &xrGroup);

			calc_tool_group_title_rect(pbox, ptr, glk, &xrTitle);
			ft_offset_rect(&xrTitle, px, py);
			ft_expand_rect(&xrTitle, -DEF_SPLIT_FEED, -DEF_SPLIT_FEED);
			xrTitle.fy -= DEF_SPLIT_FEED;
			xrTitle.fh += DEF_SPLIT_FEED;

			xmem_copy((void*)&xrClip, (void*)&xrGroup, sizeof(xrect_t));
			ft_clip_rect(&xrClip, &xrTitle);

			(*pif->pf_draw_rect)(pif->canvas, &xp, &xb, &xrClip);

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrTitle, get_tool_group_title_ptr(glk), -1);
		}

		show = get_tool_group_show_ptr(glk);

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

		ilk = get_tool_group_next_item(glk, LINK_FIRST);
		while (ilk)
		{
			calc_tool_group_item_rect(pbox, ptr, ilk, &xrItem);
			ft_offset_rect(&xrItem, px, py);

			if (compare_text(show, -1, ATTR_SHOW_IMAGEONLY, -1, 0) == 0)
			{
				if (imagelist)
				{
					get_ximage(imagelist, get_tool_item_image_ptr(ilk), &xi);
					(*pif->pf_draw_image)(pif->canvas, &xi, &xrItem);
				}
			}
			else if (compare_text(show, -1, ATTR_SHOW_TEXTONLY, -1, 0) == 0)
			{
				(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrItem, get_tool_item_title_ptr(ilk), -1);
			}
			else
			{
				if (imagelist)
				{
					get_ximage(imagelist, get_tool_item_image_ptr(ilk), &xi);

					xrItem.fh /= 2;
					(*pif->pf_draw_image)(pif->canvas, &xi, &xrItem);
					xrItem.fy += xrItem.fh;
				}
				(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xrItem, get_tool_item_title_ptr(ilk), -1);
			}

			ilk = get_tool_group_next_item(glk, ilk);
		}

		glk = get_tool_next_group(ptr, glk);
	}

}

#endif //XDLVIEW