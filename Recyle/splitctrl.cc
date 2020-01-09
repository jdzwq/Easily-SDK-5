/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc spit control document

	@module	splitctrl.c | split control implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#include "splitctrl.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcgdi.h"
#include "xdcwidg.h"

typedef struct _split_delta_t{
	if_canvas_t* pif;

	link_t_ptr split;

	bool_t b_size;
	short org_x, org_y;
	link_t_ptr org_item;

}split_delta_t;

#define GETSPLITDELTA(ph) 		(split_delta_t*)widget_get_user_delta(ph)
#define SETSPLITDELTA(ph,ptd)	widget_set_user_delta(ph,ptd)

/*********************************************************************************************/

void	noti_split_reset_page(res_win_t widget);

void	noti_split_reset_split(res_win_t widget, link_t_ptr ilk);

void	noti_split_reset_widget(res_win_t widget, link_t_ptr ilk);

void	noti_split_item_sizing(res_win_t widget, link_t_ptr ilk, short x, short y);

void	noti_split_item_sized(res_win_t widget, short x, short y);

/*************************************************************************************************/

int size_split_item_widget(link_t_ptr ilk, void* pv)
{
	split_delta_t* ptd = GETSPLITDELTA((res_win_t)pv);
	res_win_t win;
	xrect_t xr;

	if (get_split_item_splited(ilk))
		return 0;

	xr.x = get_split_item_x(ilk);
	xr.w = get_split_item_width(ilk);
	xr.y = get_split_item_y(ilk);
	xr.h = get_split_item_height(ilk);

	rect_tm_to_pt(ptd->pif->canvas, &xr);

	win = (res_win_t)get_split_item_widget(ilk);

	if (widget_is_valid(win))
	{
		widget_resize(win, &xr);
	}

	return 0;
}

/*********************************************************************************************/

void noti_split_reset_page(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	long pw, ph;
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	pw = pt_to_tm(ptd->pif->canvas, xr.w, 1) - pt_to_tm(ptd->pif->canvas, 0, 1);
	ph = pt_to_tm(ptd->pif->canvas, xr.h, 0) - pt_to_tm(ptd->pif->canvas, 0, 0);

	set_split_item_x(ptd->split, 0);
	set_split_item_y(ptd->split, 0);
	set_split_item_width(ptd->split, pw);
	set_split_item_height(ptd->split, ph);
}

void noti_split_reset_split(res_win_t widget, link_t_ptr ilk)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	resize_split_item(ilk);
}

void noti_split_reset_widget(res_win_t widget, link_t_ptr ilk)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	enum_split_item(ilk, (PF_LINKENUM)size_split_item_widget, (void*)widget);
}

void noti_split_item_sizing(res_win_t widget, link_t_ptr ilk, short x, short y)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	bool_t bHorz;

	ptd->org_item = ilk;
	ptd->org_x = x;
	ptd->org_y = y;

	ptd->b_size = 1;

	bHorz = (compare_text(get_split_item_layer_ptr(ptd->org_item), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;
	if (bHorz)
		widget_set_cursor(widget,CURS_SIZENS);
	else
		widget_set_cursor(widget,CURS_SIZEWE);

}

void noti_split_item_sized(res_win_t widget, short x, short y)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	long cx, cy, h, w, m;
	float r;
	bool_t bHorz;
	xrect_t xr;
	link_t_ptr ilk;
	tchar_t token[NUM_LEN + 1];

	ilk = ptd->org_item;

	ptd->org_item = NULL;
	ptd->b_size = 0;

	bHorz = (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;

	if (bHorz)
	{
		if (y == ptd->org_y)
			return;

		cy = pt_to_tm(ptd->pif->canvas, y, 0) - pt_to_tm(ptd->pif->canvas, ptd->org_y, 0);
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

		cx = pt_to_tm(ptd->pif->canvas, x, 1) - pt_to_tm(ptd->pif->canvas, ptd->org_x, 1);
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

	noti_split_reset_split(widget, ilk);

	calc_split_bar_rect(ptd->split, ilk, &xr);

	rect_tm_to_pt(ptd->pif->canvas, &xr);

	widget_invalid_rect(widget, &xr, 0);

	noti_split_reset_widget(widget, ilk);
}

/****************************************************************************************/
void hand_split_erase(res_win_t widget, res_dc_t dc, const xrect_t* pxr, const xpaint_t* pxn)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	xbrush_t xb = { 0 };
	xrect_t xr = { 0 };

	widget_get_client_rect(widget, &xr);

	parse_xbrush_from_style(&xb, DEF_CTRL_BRUSH);
	if (pxn && !is_null(pxn->brush_color))
		xscpy(xb.color, pxn->brush_color);

	widget_get_client_rect(widget, &xr);

	draw_rect_raw(dc, NULL, &xb, &xr);
}

void hand_split_paint(res_win_t widget,res_dc_t dc,const xrect_t* pxr, const xpaint_t* pxn)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	link_t_ptr ilk;
	link_t_ptr st = NULL;
	xrect_t xr;
	long span;
	xbrush_t xb = { 0 };
	xgradi_t xg_horz, xg_vert;
	res_dc_t rdc;
	float r;
	long m;

	const tchar_t *style;

	if (!ptd)
		return;

	widget_get_client_rect(widget, &xr);
	rdc = begin_display_paint(ptd->pif->canvas, dc, xr.w, xr.h);

	style = get_split_style_ptr(ptd->split);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, style);
	if (pxn && !is_null(pxn->brush_color))
		xscpy(xb.color, pxn->brush_color);

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

			rect_tm_to_pt(ptd->pif->canvas, &xr);

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

	end_display_paint(ptd->pif->canvas, dc, pxr);
}

void hand_split_size(res_win_t widget, short cx, short cy)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	if (!ptd)
		return;

	split_view_redraw(widget);
}

void hand_split_mouse_move(res_win_t widget, short x, short y)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	bool_t bHorz;
	link_t_ptr plk;
	int nHint;
	xpoint_t pt;

	if (!ptd)
		return;

	if (ptd->b_size)
		return;

	pt.x = pt_to_tm(ptd->pif->canvas, x, 1);
	pt.y = pt_to_tm(ptd->pif->canvas, y, 0);

	nHint = calc_split_hint(ptd->split, &pt, &plk);
	if (plk && get_split_item_fixed(plk))
	{
		return;
	}

	if (nHint == SPLIT_HINT_BAR)
	{
		bHorz = (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;
		if (bHorz)
			widget_set_cursor(widget,CURS_SIZENS);
		else
			widget_set_cursor(widget,CURS_SIZEWE);
	}
	else
	{
		widget_set_cursor(widget,NULL);
	}
}

void hand_split_lbutton_down(res_win_t widget, short x, short y)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;

	if (!ptd)
		return;

	pt.x = pt_to_tm(ptd->pif->canvas, x, 1);
	pt.y = pt_to_tm(ptd->pif->canvas, y, 0);

	nHint = calc_split_hint(ptd->split, &pt, &ilk);
	if (nHint == SPLIT_HINT_BAR)
	{
		if (get_split_item_fixed(ilk))
			return;

		widget_set_capture(widget, 1);

		noti_split_item_sizing(widget, ilk, x, y);
	}
}

void hand_split_lbutton_up(res_win_t widget, short x, short y)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	if (!ptd)
		return;

	widget_set_capture(widget,0);

	if (!ptd->b_size)
		return;

	noti_split_item_sized(widget, x, y);

	widget_set_capture(widget, 0);
}

/*************************************************************************************************************/
void split_view_attach(res_win_t widget, link_t_ptr ptr)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	res_dc_t dc;

	if (ptd)
		return;

	ptd = (split_delta_t*)xmem_alloc(sizeof(split_delta_t));
	memset((void*)ptd, 0, sizeof(split_delta_t));

	dc = widget_reference_dc(widget);
	ptd->pif = create_display_canvas(dc);
	widget_release_dc(widget, dc);

	SETSPLITDELTA(widget, ptd);

	ptd->split = ptr;
	if (ptd->split)
	{
		split_view_redraw(widget);
	}
}

link_t_ptr split_view_detach(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	link_t_ptr data;

	if (!ptd)
		return NULL;

	if (ptd->split)
	{
		data = ptd->split;
		ptd->split = NULL;
	}
	else
	{
		data = NULL;
	}

	destroy_display_canvas(ptd->pif);

	xmem_free(ptd);

	SETSPLITDELTA(widget, 0);

	return data;
}

link_t_ptr split_view_fetch(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	return ptd->split;
}

void split_view_redraw(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	assert(ptd->split && ptd->pif->canvas);

	noti_split_reset_page(widget);

	noti_split_reset_split(widget, ptd->split);

	//widget_invalid_rect(widget, NULL);

	noti_split_reset_widget(widget, ptd->split);
}

void split_view_tm_to_pt(res_win_t widget, xpoint_t* ppt)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	ppt->x = tm_to_pt(ptd->pif->canvas, ppt->x, 1);
	ppt->y = tm_to_pt(ptd->pif->canvas, ppt->y, 0);
}

void split_view_pt_to_tm(res_win_t widget, xpoint_t* ppt)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	ppt->x = pt_to_tm(ptd->pif->canvas, ppt->x, 1);
	ppt->y = pt_to_tm(ptd->pif->canvas, ppt->y, 0);
}
