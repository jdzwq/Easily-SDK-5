/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc childframe control document

	@module	childframe.c | child frame implement file

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

#include "xdcframe.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcgdi.h"
#include "xdcwidg.h"

typedef struct childfrm_delta_t{
	link_t_ptr split;

	link_t_ptr org_item;
	short org_x, org_y;
}childfrm_delta_t;

#define GETCHILDFRMDELTA(ph) 	(childfrm_delta_t*)widget_get_user_delta(ph)
#define SETCHILDFRMDELTA(ph,ptd) widget_set_user_delta(ph,ptd)

/*********************************************************************************************************/

int _childfrm_resize(link_t_ptr ilk, void* pv)
{
	res_win_t win;
	xrect_t xr;

	if (get_split_item_splited(ilk))
		return 0;

	xr.x = get_split_item_x(ilk);
	xr.w = get_split_item_width(ilk);
	xr.y = get_split_item_y(ilk);
	xr.h = get_split_item_height(ilk);

	win = (res_win_t)get_split_item_widget(ilk);

	widget_rect_to_pt(win, &xr);

	if (widget_is_valid(win))
	{
		widget_move(win, RECTPOINT(&xr));
		widget_size(win, RECTSIZE(&xr));
		widget_invalid(win, NULL, 0);
	}

	return 0;
}

int _childfrm_destroy(link_t_ptr ilk, void* pv)
{
	res_win_t win;

	if (get_split_item_splited(ilk))
		return 0;

	win = (res_win_t)get_split_item_widget(ilk);

	if (widget_is_valid(win))
	{
		widget_destroy(win);
	}

	return 0;
}

void _childfrm_reset(res_win_t widget)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->split != NULL);

	widget_get_client_rect(widget, &xr);

	widget_rect_to_tm(widget, &xr);

	set_split_item_x(ptd->split, 0);
	set_split_item_y(ptd->split, 0);
	set_split_item_width(ptd->split, xr.w);
	set_split_item_height(ptd->split, xr.h);
}

/************************************************************************************/

void noti_childfrm_item_sizing(res_win_t widget, link_t_ptr ilk, short x, short y)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);
	bool_t bHorz;

	ptd->org_item = ilk;
	ptd->org_x = x;
	ptd->org_y = y;

	bHorz = (compare_text(get_split_item_layer_ptr(ptd->org_item), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;
	if (bHorz)
		widget_set_cursor(widget, CURS_SIZENS);
	else
		widget_set_cursor(widget, CURS_SIZEWE);

}

void noti_childfrm_item_sized(res_win_t widget, short x, short y)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);
	long cx, cy, h, w, m;
	float r;
	bool_t bHorz;
	xrect_t xr;
	xpoint_t pt_org, pt_cur;
	link_t_ptr ilk;
	tchar_t token[NUM_LEN + 1];

	ilk = ptd->org_item;
	ptd->org_item = NULL;

	pt_org.x = ptd->org_x;
	pt_org.y = ptd->org_y;
	pt_cur.x = x;
	pt_cur.y = y;

	widget_point_to_tm(widget, &pt_org);
	widget_point_to_tm(widget, &pt_cur);

	bHorz = (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;

	if (bHorz)
	{
		if (y == ptd->org_y)
			return;

		cy = pt_cur.y - pt_org.y;
		r = (float)(xstof(get_split_item_ratio_ptr(ilk)));
		h = get_split_item_height(ilk);

		if (r > 1)
			m = (long)r + cy;
		else
			m = (long)(r * h) + cy;

		if (m > h)
			m = h;

		xsprintf(token, _T("%.2f"), (float)m / (float)h);

		set_split_item_ratio(ilk, token);
	}
	else
	{
		if (x == ptd->org_x)
			return;

		cx = pt_cur.x - pt_org.x;
		r = (float)(xstof(get_split_item_ratio_ptr(ilk)));
		w = get_split_item_width(ilk);

		if (r > 1)
			m = (long)r + cx;
		else
			m = (long)(r * w) + cx;

		if (m > w)
			m = w;

		xsprintf(token, _T("%.2f"), (float)m / (float)w);

		set_split_item_ratio(ilk, token);
	}

	resize_split_item(ilk);
	enum_split_item(ilk, (PF_LINKENUM)_childfrm_resize, NULL);

	calc_split_bar_rect(ptd->split, ilk, &xr);

	widget_rect_to_pt(widget, &xr);

	widget_invalid(widget, &xr, 0);
}

/*************************************************************************************************/
void hand_childfrm_create(res_win_t widget)
{
	childfrm_delta_t* ptd;

	ptd = (childfrm_delta_t*)xmem_alloc(sizeof(childfrm_delta_t));
	memset((void*)ptd, 0, sizeof(childfrm_delta_t));

	SETCHILDFRMDELTA(widget, ptd);
}

void hand_childfrm_destroy(res_win_t widget)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	if (!ptd)
		return;

	if (ptd->split)
	{
		enum_split_item(ptd->split, (PF_LINKENUM)_childfrm_resize, NULL);
	}

	SETCHILDFRMDELTA(widget, 0);
	xmem_free(ptd);
}

void hand_childfrm_paint(res_win_t widget, res_dc_t rdc, const xrect_t* prt)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	link_t_ptr ilk;
	link_t_ptr st = NULL;
	xrect_t xr;
	long span;
	xbrush_t xb = { 0 };
	xgradi_t xg_horz, xg_vert;
	float r;
	long m;

	const tchar_t *style;

	if (!ptd || !ptd->split)
		return;

	widget_get_client_rect(widget, &xr);

	style = get_split_style_ptr(ptd->split);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, style);

	default_xgradi(&xg_vert);
	xscpy(xg_vert.color, xb.color);
	xscpy(xg_vert.type, GDI_ATTR_GRADIENT_TYPE_HORZ);

	default_xgradi(&xg_horz);
	xscpy(xg_horz.color, xb.color);
	xscpy(xg_vert.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	ilk = ptd->split;
	while (ilk)
	{
		xr.x = get_split_item_x(ilk);
		xr.y = get_split_item_y(ilk);
		xr.w = get_split_item_width(ilk);
		xr.h = get_split_item_height(ilk);
		span = get_split_item_span(ilk);
		r = (float)(xstof(get_split_item_ratio_ptr(ilk)));

		if (get_split_item_splited(ilk))
		{
			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				if (r > 1)
					m = ((long)r < xr.h) ? (long)r : xr.h;
				else
					m = (long)(r * xr.h);

				xr.y += (m - span);
				xr.h = 2 * span;
			}
			else
			{
				if (r > 1)
					m = ((long)r < xr.w) ? (long)r : xr.w;
				else
					m = (long)(r * xr.w);

				xr.x += (m - span);
				xr.w = 2 * span;
			}

			widget_rect_to_pt(widget, &xr);

			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
				gradient_rect_raw(rdc, &xg_horz, &xr);
			else
				gradient_rect_raw(rdc, &xg_vert, &xr);
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

void hand_childfrm_size(res_win_t widget, const xsize_t* prs)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	if (!ptd || !ptd->split)
		return;

	_childfrm_reset(widget);
	
	resize_split_item(ptd->split);
	enum_split_item(ptd->split, (PF_LINKENUM)_childfrm_resize, NULL);

	widget_invalid(widget, NULL, 0);
}

void hand_childfrm_mouse_move(res_win_t widget, dword_t dw, short x, short y)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	bool_t bHorz;
	link_t_ptr plk;
	int nHint;
	xpoint_t pt;

	if (!ptd || !ptd->split)
		return;

	pt.x = x;
	pt.y = y;

	widget_point_to_tm(widget, &pt);

	nHint = calc_split_hint(ptd->split, &pt, &plk);
	if (plk && get_split_item_fixed(plk))
	{
		return;
	}

	if (nHint == SPLIT_HINT_BAR)
	{
		bHorz = (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;
		if (bHorz)
			widget_set_cursor(widget, CURS_SIZENS);
		else
			widget_set_cursor(widget, CURS_SIZEWE);
	}
	else
	{
		widget_set_cursor(widget, NULL);
	}
}

void hand_childfrm_lbutton_down(res_win_t widget, short x, short y)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;

	if (!ptd || !ptd->split)
		return;

	pt.x = x;
	pt.y = y;

	widget_point_to_tm(widget, &pt);

	nHint = calc_split_hint(ptd->split, &pt, &ilk);
	if (nHint == SPLIT_HINT_BAR)
	{
		if (get_split_item_fixed(ilk))
			return;

		widget_set_capture(widget, 1);

		noti_childfrm_item_sizing(widget, ilk, x, y);
	}
}

void hand_childfrm_lbutton_up(res_win_t widget, short x, short y)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	if (!ptd || !ptd->split)
		return;

	widget_set_capture(widget, 0);

	if (ptd->org_item)
	{
		noti_childfrm_item_sized(widget, x, y);
	}
}

/*************************************************************************************************************/
res_win_t childfrm_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_childfrm_create;
	ev.pf_on_destroy = hand_childfrm_destroy;

	ev.pf_on_paint = hand_childfrm_paint;

	ev.pf_on_size = hand_childfrm_size;
	ev.pf_on_mouse_move = hand_childfrm_mouse_move;
	ev.pf_on_lbutton_down = hand_childfrm_lbutton_down;
	ev.pf_on_lbutton_up = hand_childfrm_lbutton_up;

	return widget_create(wname, wstyle, pxr->x, pxr->y, pxr->w, pxr->h, wparent, &ev);
}

void childfrm_layout(res_win_t widget)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (ptd->split)
	{
		_childfrm_reset(widget);

		resize_split_item(ptd->split);
		enum_split_item(ptd->split, (PF_LINKENUM)_childfrm_resize, NULL);
	}

	widget_invalid(widget, NULL, 1);
}

void childfrm_attach_split(res_win_t widget, link_t_ptr ptr)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->split = ptr;

	childfrm_layout(widget);
}

link_t_ptr childfrm_detach_split(res_win_t widget)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->split;
	ptd->split = NULL;

	childfrm_layout(widget);

	return data;
}

link_t_ptr childfrm_fetch_split(res_win_t widget)
{
	childfrm_delta_t* ptd = GETCHILDFRMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->split;
}


