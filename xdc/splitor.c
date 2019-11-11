/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc splitor document

	@module	splitor.c | implement file

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

#include "splitor.h"
#include "handler.h"
#include "winnc.h"

static bool_t _splitor_item_resize(link_t_ptr ilk, void* pv)
{
	res_win_t owner, win;
	xrect_t xr;

	if (get_split_item_splited(ilk))
		return 1;

	owner = (res_win_t)(pv);

	xr.fx = get_split_item_x(ilk);
	xr.fw = get_split_item_width(ilk);
	xr.fy = get_split_item_y(ilk);
	xr.fh = get_split_item_height(ilk);

	widget_rect_to_pt(owner, &xr);

	win = (res_win_t)get_split_item_delta(ilk);

	if (widget_is_valid(win))
	{
		widget_move(win, RECTPOINT(&xr));
		widget_size(win, RECTSIZE(&xr));
	}

	return 1;
}

/************************************************************************************/

void noti_splitor_item_sizing(splitor_t* ptd, link_t_ptr ilk, const xpoint_t* pxp)
{
	bool_t bHorz;

	ptd->item = ilk;
	ptd->x = pxp->x;
	ptd->y = pxp->y;

	bHorz = (compare_text(get_split_item_layer_ptr(ptd->item), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;

	if (bHorz)
		widget_set_cursor(ptd->widget, CURSOR_SIZENS);
	else
		widget_set_cursor(ptd->widget, CURSOR_SIZEWE);

}

void noti_splitor_item_sized(splitor_t* ptd, const xpoint_t* pxp)
{
	bool_t bHorz;
	xrect_t xr;
	xsize_t xs;
	xpoint_t pt_org, pt_cur;
	link_t_ptr ilk;

	ilk = ptd->item;
	ptd->item = NULL;

	pt_org.x = ptd->x;
	pt_org.y = ptd->y;
	pt_cur.x = pxp->x;
	pt_cur.y = pxp->y;

	bHorz = (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;

	if (bHorz)
	{
		if (pxp->y == ptd->y)
			return;

		xs.cx = 0;
		xs.cy = pt_cur.y - pt_org.y;
		widget_size_to_tm(ptd->widget, &xs);

		adjust_split_item(ilk, xs.fy);
	}
	else
	{
		if (pxp->x == ptd->x)
			return;

		xs.cx = pt_cur.x - pt_org.x;
		xs.cy = 0;
		widget_size_to_tm(ptd->widget, &xs);

		adjust_split_item(ilk, xs.fx);
	}

	calc_split_item_rect(ptd->split, ilk, &xr);
	widget_rect_to_pt(ptd->widget, &xr);

	enum_split_item(ilk, (CALLBACK_ENUMLINK)_splitor_item_resize, ptd->widget);

	widget_update(ptd->widget, &xr, 0);
}

/*************************************************************************************************/

bool_t hand_splitor_mouse_move(splitor_t* ptd, dword_t dw, const xpoint_t* pxp)
{
	bool_t bHorz;
	link_t_ptr plk;
	int nHint;
	xpoint_t pt;

	XDL_ASSERT(ptd != NULL);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(ptd->widget, &pt);

	nHint = calc_split_hint(ptd->split, &pt, &plk);

	if (plk && get_split_item_fixed(plk))
	{
		return 1;
	}

	if (nHint == SPLIT_HINT_BAR)
	{
		bHorz = (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;
		if (bHorz)
			widget_set_cursor(ptd->widget, CURSOR_SIZENS);
		else
			widget_set_cursor(ptd->widget, CURSOR_SIZEWE);
		
		return 1;
	}
	else
	{
		widget_set_cursor(ptd->widget, CURSOR_ARROW);
		return 0;
	}
}

bool_t hand_splitor_lbutton_down(splitor_t* ptd, const xpoint_t* pxp)
{
	xpoint_t pt;
	int nHint;
	link_t_ptr ilk;

	XDL_ASSERT(ptd != NULL);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(ptd->widget, &pt);

	nHint = calc_split_hint(ptd->split, &pt, &ilk);

	if (nHint == SPLIT_HINT_BAR)
	{
		if (get_split_item_fixed(ilk))
			return 1;

		widget_set_capture(ptd->widget, 1);

		noti_splitor_item_sizing(ptd, ilk, pxp);

		return 1;
	}

	return 0;
}

bool_t hand_splitor_lbutton_up(splitor_t* ptd, const xpoint_t* pxp)
{
	XDL_ASSERT(ptd != NULL);

	if (ptd->item)
	{ 
		widget_set_capture(ptd->widget, 0);
		noti_splitor_item_sized(ptd, pxp);
		return 1;
	}

	return 0;
}

void hand_splitor_size(splitor_t* ptd, const xrect_t* pxr)
{
	xrect_t xr;

	XDL_ASSERT(ptd->split != NULL);

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	widget_rect_to_tm(ptd->widget, &xr);

	set_split_item_x(ptd->split, xr.fx);
	set_split_item_y(ptd->split, xr.fy);
	set_split_item_width(ptd->split, xr.fw);
	set_split_item_height(ptd->split, xr.fh);

	resize_split_item(ptd->split);
	enum_split_item(ptd->split, (CALLBACK_ENUMLINK)_splitor_item_resize, ptd->widget);

	widget_update(ptd->widget, pxr, 0);
}

void hand_splitor_paint(splitor_t* ptd, res_ctx_t rdc)
{
	link_t_ptr ilk;
	link_t_ptr st = NULL;
	xrect_t xr;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xgradi_t xg_horz, xg_vert;

	XDL_ASSERT(ptd != NULL);

	widget_get_xbrush(ptd->widget, &xb);

	default_xgradi(&xg_vert);
	xscpy(xg_vert.brim_color, xb.color);
	xscpy(xg_vert.core_color, xb.color);
	lighten_xgradi(&xg_vert, DEF_MIDD_DARKEN);
	xscpy(xg_vert.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	default_xgradi(&xg_horz);
	xscpy(xg_horz.brim_color, xb.color);
	xscpy(xg_horz.core_color, xb.color);
	lighten_xgradi(&xg_horz, DEF_MIDD_DARKEN);
	xscpy(xg_vert.type, GDI_ATTR_GRADIENT_TYPE_HORZ);

	default_xpen(&xp);
	xscpy(xp.color, xg_horz.brim_color);
	xscpy(xp.size, _T("2"));

	widget_get_client_rect(ptd->widget, &xr);

	draw_rect_raw(rdc, &xp, NULL, &xr);

	ilk = ptd->split;
	while (ilk)
	{
		if (get_split_item_splited(ilk))
		{
			calc_split_span_rect(ptd->split, ilk, &xr);
			widget_rect_to_pt(ptd->widget, &xr);

			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				gradient_rect_raw(rdc, &xg_horz, &xr);
			}
			else
			{
				gradient_rect_raw(rdc, &xg_vert, &xr);
			}
		}

		if (get_split_item_splited(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_split_first_child_item(ilk);
			continue;
		}

		while (ilk)
		{
			ilk = get_split_next_sibling_item(ilk);
			if (ilk)
			{
				break;
			}

			ilk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);
}

