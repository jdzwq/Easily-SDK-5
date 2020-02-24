/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc table document

	@module	tableview.c | implement file

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
#include "tableview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

float calc_table_height(const if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, const canvbox_t* pbox, link_t_ptr ptr)
{
	float lr,lh, h;
	xsize_t xs;
	link_t_ptr ilk;

	if (is_null(pxa->line_height))
		lr = xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		lr = xstof(pxa->line_height);

	if (lr < 1)
		lr = 1.0;

	(*pif->pf_text_metric)(pif->ctx, pxf, &xs);

	lh = (float)(xs.fy * lr);

	h = 0;
	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		h += lh;

		ilk = get_menu_next_item(ptr, ilk);
	}

	return h;
}

float calc_table_width(const if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, const canvbox_t* pbox, link_t_ptr ptr)
{
	float lw, w;
	xsize_t xs;
	link_t_ptr ilk;

	w = 0;
	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		lw = 0;
		(*pif->pf_text_size)(pif->ctx, pxf, get_string_entity_key_ptr(ilk), -1, &xs);
		lw += xs.fx;

		(*pif->pf_text_size)(pif->ctx, pxf, get_string_entity_val_ptr(ilk), -1, &xs);
		lw += xs.fx;

		if (w < lw)
			w = lw;

		ilk = get_menu_next_item(ptr, ilk);
	}

	return w;
}

void calc_table_item_rect(const if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr plk, xrect_t* pxr)
{
	float lr, lh, h;
	xsize_t xs;
	link_t_ptr ilk;

	if (is_null(pxa->line_height))
		lr = (float)xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		lr = (float)xstof(pxa->line_height);

	if (lr < 1)
		lr = 1.0;

	(*pif->pf_text_metric)(pif->ctx, pxf, &xs);

	lh = (float)(xs.fy * lr);

	h = 0;
	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		if (ilk == plk)
			break;

		h += lh;

		ilk = get_menu_next_item(ptr, ilk);
	}

	pxr->fx = 0;
	pxr->fy = h;
	pxr->fw = pbox->fw;
	pxr->fh = lh;
}

void calc_table_item_key_rect(const if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, const canvbox_t* pbox, link_t_ptr ptr, float ratio, link_t_ptr plk, xrect_t* pxr)
{
	calc_table_item_rect(pif, pxf, pxa, pbox, ptr, plk, pxr);
	pxr->fw = pbox->fw * ratio;
}

void calc_table_item_val_rect(const if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, const canvbox_t* pbox, link_t_ptr ptr, float ratio, link_t_ptr plk, xrect_t* pxr)
{
	calc_table_item_rect(pif, pxf, pxa, pbox, ptr, plk, pxr);
	pxr->fx = pbox->fw * ratio;
	pxr->fw = pbox->fw * (1 - ratio);
}

int	calc_table_hint(const if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, float ratio, link_t_ptr* pilk)
{
	float lr, lh, h;
	float kw, vw;
	xrect_t xr;
	xsize_t xs;
	link_t_ptr ilk;
	int hint;

	if (is_null(pxa->line_height))
		lr = (float)xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		lr = (float)xstof(pxa->line_height);

	if (lr < 1)
		lr = 1.0;

	(*pif->pf_text_metric)(pif->ctx, pxf, &xs);

	lh = (float)(xs.fy * lr);

	kw = pbox->fw * ratio;
	vw = pbox->fw - kw;

	*pilk = NULL;
	hint = TABLE_HINT_NONE;
	h = 0;
	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		xr.fx = 0;
		xr.fy = h;
		xr.fw = kw - 1;
		xr.fh = lh;

		if (ft_in_rect(ppt, &xr))
		{
			*pilk = ilk;
			hint = TABLE_HINT_KEY;
			break;
		}

		xr.fx = kw - 1;
		xr.fy = h;
		xr.fw = 2;
		xr.fh = lh;

		if (ft_in_rect(ppt, &xr))
		{
			*pilk = ilk;
			hint = TABLE_HINT_SPLIT;
			break;
		}

		xr.fx = kw + 1;
		xr.fy = h;
		xr.fw = vw;
		xr.fh = lh;

		if (ft_in_rect(ppt, &xr))
		{
			*pilk = ilk;
			hint = TABLE_HINT_VAL;
			break;
		}

		h += lh;

		ilk = get_string_next_entity(ptr, ilk);
	}

	return hint;
}

void draw_table(const if_canvas_t* pif, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xbrush_t* pxb, const canvbox_t* pbox, link_t_ptr ptr, float ratio)
{
	float lr, lh, h;
	float kw, vw;
	xrect_t xr;
	xsize_t xs;
	link_t_ptr ilk;
	int step;

	if (is_null(pxa->line_height))
		lr = (float)xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		lr = (float)xstof(pxa->line_height);

	if (lr < 1)
		lr = 1.0;

	(*pif->pf_measure_metric)(pif->canvas, pxf, &xs);

	lh = (float)(xs.fy * lr);

	kw = pbox->fw * ratio;
	vw = pbox->fw - kw;

	step = 0;
	h = 0;
	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		xr.fx = pbox->fx;
		xr.fy = h + pbox->fy;
		xr.fw = kw;
		xr.fh = lh;

		if (step % 2)
			(*pif->pf_draw_rect)(pif->canvas, pxp, pxb, &xr);
		else
			(*pif->pf_draw_rect)(pif->canvas, pxp, NULL, &xr);

		(*pif->pf_draw_text)(pif->canvas, pxf, pxa, &xr, get_string_entity_key_ptr(ilk), -1);

		xr.fx = kw + pbox->fx;
		xr.fy = h + pbox->fy;
		xr.fw = vw;
		xr.fh = lh;

		(*pif->pf_draw_rect)(pif->canvas, pxp, NULL, &xr);

		(*pif->pf_draw_text)(pif->canvas, pxf, pxa, &xr, get_string_entity_val_ptr(ilk), -1);

		h += lh;

		ilk = get_string_next_entity(ptr, ilk);
	}
}

#endif //XDLVIEW