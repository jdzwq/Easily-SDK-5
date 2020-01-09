/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc spit view document

	@module	splitview.c | split view implement file

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

#include "splitview.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdccanv.h"
#include "xdcgdi.h"
#include "xdcdraw.h"
#include "xdcwidg.h"

typedef struct _split_delta_t{
	res_win_t owner;
	xhand_t canvas;

	link_t_ptr split;

	bool_t b_size;
	short org_x, org_y;
	link_t_ptr org_item;
}split_delta_t;

#define GETSPLITDELTA(ph) 		(split_delta_t*)widget_get_user_delta(ph)
#define SETSPLITDELTA(ph,ptd)	widget_set_user_delta(ph,ptd)

#define SPLITBAR_SPAN		10

/*********************************************************************************************/

void	noti_split_reset_page(res_win_t widget);

void	noti_split_reset_split(res_win_t widget, link_t_ptr ilk);

void	noti_split_reset_widget(res_win_t widget, link_t_ptr ilk);

void	noti_split_item_sizing(res_win_t widget, link_t_ptr ilk, short x, short y);

void	noti_split_item_sized(res_win_t widget, short x, short y);

/**********************************************************************************************/

void	hand_split_create(res_win_t widget);

void	hand_split_destroy(res_win_t widget);

void	hand_split_paint(res_win_t widget, res_dc_t dc, const xrect_t* prt);

void	hand_split_erase(res_win_t widget, res_dc_t dc, const xrect_t* prt);

void	hand_split_size(res_win_t widget, const resize_t* prs);

void	hand_split_mouse_move(res_win_t widget, dword_t dw, short x, short y);

void	hand_split_lbutton_down(res_win_t widget, short x, short y);

void	hand_split_lbutton_up(res_win_t widget, short x, short y);

/*************************************************************************************************/

void resize_split_item(link_t_ptr ilk)
{
	link_t_ptr plk;
	long x, y, w, h;
	link_t_ptr st = NULL;

	while (ilk)
	{
		if (get_split_item_splited(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_item(st, (void*)ilk);

			ilk = get_split_first_child_item(ilk);

			plk = get_split_parent_item(ilk);
			x = get_split_item_x(plk);
			y = get_split_item_y(plk);
			w = get_split_item_width(plk);
			h = get_split_item_height(plk);

			if (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				h = (long)((float)h * get_split_item_ratio(plk) - SPLITBAR_SPAN);
			}
			else
			{
				w = (long)((float)w * get_split_item_ratio(plk) - SPLITBAR_SPAN);
			}
			set_split_item_x(ilk, x);
			set_split_item_y(ilk, y);
			set_split_item_width(ilk, w);
			set_split_item_height(ilk, h);
			continue;
		}

		while (ilk)
		{
			ilk = get_split_next_sibling_item(ilk);
			if (ilk)
			{
				plk = get_split_parent_item(ilk);
				x = get_split_item_x(plk);
				y = get_split_item_y(plk);
				w = get_split_item_width(plk);
				h = get_split_item_height(plk);

				if (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
				{
					y += (long)((float)h * get_split_item_ratio(plk) + SPLITBAR_SPAN);
					h -= (long)((float)h * get_split_item_ratio(plk) - SPLITBAR_SPAN);
				}
				else
				{
					x += (long)((float)w * get_split_item_ratio(plk) + SPLITBAR_SPAN);
					w -= (long)((float)w * get_split_item_ratio(plk) - SPLITBAR_SPAN);
				}
				set_split_item_x(ilk, x);
				set_split_item_y(ilk, y);
				set_split_item_width(ilk, w);
				set_split_item_height(ilk, h);
				break;
			}

			ilk = (st) ? (link_t_ptr)pop_stack_item(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);
}

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

	rect_tm_to_pt(ptd->canvas, &xr);

	win = (res_win_t)get_split_item_widget(ilk);

	if (widget_is_valid(win))
	{
		widget_resize(win, &xr);
	}

	return 0;
}

void calc_split_item_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr)
{	
	pxr->x = get_split_item_x(cur);
	pxr->y = get_split_item_y(cur);
	pxr->w = get_split_item_width(cur);
	pxr->h = get_split_item_height(cur);
}

void calc_split_bar_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr)
{
	long x, y, w, h;

	x = get_split_item_x(cur);
	y = get_split_item_y(cur);
	w = get_split_item_width(cur);
	h = get_split_item_height(cur);

	if (get_split_item_splited(cur))
	{
		if (compare_text(get_split_item_layer_ptr(cur), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
		{
			y += (long)((float)h * get_split_item_ratio(cur) - SPLITBAR_SPAN);
			h = 2 * SPLITBAR_SPAN;
		}
		else
		{
			x += (long)((float)w * get_split_item_ratio(cur) - SPLITBAR_SPAN);
			w = 2 * SPLITBAR_SPAN;
		}

		pxr->x = x;
		pxr->y = y;
		pxr->w = w;
		pxr->h = h;
	}
	else
	{
		memset((void*)pxr, 0, sizeof(xrect_t));
	}

}

int calc_split_point_hint(link_t_ptr ptr,const xpoint_t* ppt,link_t_ptr* pilk)
{
	link_t_ptr ilk,st = NULL;
	long x,y,w,h;
	int hint;

	*pilk = NULL;
	hint = SPLIT_HINT_NONE;

	ilk = ptr;
	while(ilk)
	{
		x = get_split_item_x(ilk);
		y = get_split_item_y(ilk);
		w = get_split_item_width(ilk);
		h = get_split_item_height(ilk);

		if (get_split_item_splited(ilk))
		{
			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				y += (long)((float)h * get_split_item_ratio(ilk) - SPLITBAR_SPAN);
				h = 2 * SPLITBAR_SPAN;
			}
			else
			{
				x += (long)((float)w * get_split_item_ratio(ilk) - SPLITBAR_SPAN);
				w = 2 * SPLITBAR_SPAN;
			}

			if (pt_in_rect(ppt->x, ppt->y, x, y, x + w, y + h))
			{
				*pilk = ilk;
				hint = SPLIT_HINT_BAR;
				break;
			}
		}
		
		if(get_split_item_splited(ilk))
		{
			if(!st)
				st = create_stack_table();

			push_stack_item(st,(void*)ilk);

			ilk = get_split_first_child_item(ilk);
			continue;
		}

		while(ilk)
		{
			ilk = get_split_next_sibling_item(ilk);
			if (ilk)
			{
				break;
			}

			ilk = (st) ? (link_t_ptr)pop_stack_item(st) : NULL;
		}
	}

	if(st)
		destroy_stack_table(st);

	return hint;
}

void draw_split(xhand_t canv,link_t_ptr ptr,const xpaint_t* pxn)
{
	link_t_ptr ilk;
	link_t_ptr st = NULL;
	xrect_t xr;
	xbrush_t xb = {0};
	xpen_t xp = {0};
	xgradi_t xg_horz, xg_vert;
	xcolor_t xc;

	const wchar_t *style;

	style = get_split_style_ptr(ptr);

	default_xpen(&xp);
	parse_xpen_from_style(&xp,style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb,style);

	xscpy(xg_vert.type, GDI_ATTR_GRADIENT_TYPE_VERT);
	xscpy(xg_vert.opacity, GDI_ATTR_OPACITY_TRANS);
	xscpy(xg_vert.start_color, xb.color);
	parse_xcolor(&xc, xb.color);
	lighten_xcolor(&xc, -10);
	format_xcolor(&xc, xg_vert.end_color);

	xscpy(xg_horz.type, GDI_ATTR_GRADIENT_TYPE_HORZ);
	xscpy(xg_horz.opacity, GDI_ATTR_OPACITY_TRANS);
	xscpy(xg_horz.start_color, xb.color);
	parse_xcolor(&xc, xb.color);
	lighten_xcolor(&xc, -10);
	format_xcolor(&xc, xg_horz.end_color);

	ilk = ptr;
	while (ilk)
	{
		xr.x = get_split_item_x(ilk);
		xr.y = get_split_item_y(ilk);
		xr.w = get_split_item_width(ilk);
		xr.h = get_split_item_height(ilk);

		if (get_split_item_splited(ilk))
		{
			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				xr.y += (long)((float)xr.h * get_split_item_ratio(ilk) - SPLITBAR_SPAN);
				xr.h = 2 * SPLITBAR_SPAN;
			}
			else
			{
				xr.x += (long)((float)xr.w * get_split_item_ratio(ilk) - SPLITBAR_SPAN);
				xr.w = 2 * SPLITBAR_SPAN;
			}

			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
				gradient_rect(canv, &xg_horz, &xr);
			else
				gradient_rect(canv, &xg_vert, &xr);
		}

		if (get_split_item_splited(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_item(st, (void*)ilk);

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

			ilk = (st) ? (link_t_ptr)pop_stack_item(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);

}

/*********************************************************************************************/
int noti_split_owner(res_win_t widget, unsigned long code, link_t_ptr split, link_t_ptr ilk, void* data)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	NOTICE_SPLIT nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.split = split;
	nf.item = ilk;

	widget_notice_owner(ptd->owner, (LPNOTICE)&nf);
	return nf.ret;
}

void noti_split_reset_page(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	long pw, ph;
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	pw = pt_to_tm(ptd->canvas, xr.w, 1) - pt_to_tm(ptd->canvas, 0, 1);
	ph = pt_to_tm(ptd->canvas, xr.h, 0) - pt_to_tm(ptd->canvas, 0, 0);

	set_split_page_width(ptd->split, pw);
	set_split_page_height(ptd->split, ph);

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
	long cx, cy;
	bool_t bHorz;
	xrect_t xr;
	link_t_ptr ilk;
	wchar_t token[NUM_LEN + 1];

	ilk = ptd->org_item;

	ptd->org_item = NULL;
	ptd->b_size = 0;

	bHorz = (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;

	if (bHorz)
	{
		if (y == ptd->org_y)
			return;

		cy = pt_to_tm(ptd->canvas, y, 0) - pt_to_tm(ptd->canvas, ptd->org_y, 0);
		xsprintf(token, L"%.2f", ((float)get_split_item_height(ilk) * get_split_item_ratio(ilk) + cy) / (float)get_split_item_height(ilk));
		if (xstof(token) < 0)
			xscpy(token, L"0.0");
		else if (xstof(token) > 1)
			xscpy(token, L"1.0");

		set_split_item_ratio(ilk, xstof(token));
	}
	else
	{
		if (x == ptd->org_x)
			return;

		cx = pt_to_tm(ptd->canvas, x, 1) - pt_to_tm(ptd->canvas, ptd->org_x, 1);
		xsprintf(token, L"%.2f", ((float)get_split_item_width(ilk) * get_split_item_ratio(ilk) + cx) / (float)get_split_item_width(ilk));
		if (xstof(token) < 0)
			xscpy(token, L"0.0");
		else if (xstof(token) > 1)
			xscpy(token, L"1.0");

		set_split_item_ratio(ilk, xstof(token));
	}

	noti_split_reset_split(widget, ilk);

	//calc_split_item_rect(ptd->split, ilk, &xr);
	calc_split_bar_rect(ptd->split, ilk, &xr);

	rect_tm_to_pt(ptd->canvas, &xr);

	widget_invalid_rect(widget, &xr);

	noti_split_reset_widget(widget, ilk);
}

/****************************************************************************************/

void hand_split_create(res_win_t widget)
{
	split_delta_t* ptd;
	res_dc_t dc;

	ptd = (split_delta_t*)xmem_alloc(sizeof(split_delta_t));
	memset((void*)ptd, 0, sizeof(split_delta_t));

	dc = widget_reference_dc(widget);
	ptd->canvas = create_display_canvas(dc);
	widget_release_dc(widget, dc);

	SETSPLITDELTA(widget, ptd);
}

void hand_split_destroy(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	if (!ptd)
		return;

	if (ptd->split)
	{
		split_view_detach(widget);
	}
	destroy_display_canvas(ptd->canvas);
	ptd->canvas = 0;

	xmem_free(ptd);

	SETSPLITDELTA(widget, 0);
}

void hand_split_erase(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	xpaint_t xn = { 0 };
	xbrush_t xb = { 0 };

	noti_split_owner(widget, NC_SPLITPAINT, ptd->split, NULL, (void*)&xn);

	parse_xbrush_from_style(&xb, get_split_style_ptr(ptd->split));
	if (!is_null(xn.brush_color))
		xscpy(xb.color, xn.brush_color);

	fill_rect_raw(dc, &xb, pxr);
}

void hand_split_paint(res_win_t widget,res_dc_t dc,const xrect_t* pxr)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);
	xpaint_t xn = { 0 };
	xbrush_t xb = { 0 };
	xrect_t xr = { 0 };

	if (!ptd)
		return;

	noti_split_owner(widget, NC_SPLITPAINT, ptd->split, NULL, (void*)&xn);

	begin_display_paint(ptd->canvas, dc, pxr->x, pxr->y, pxr->w, pxr->h);

	parse_xbrush_from_style(&xb, get_split_style_ptr(ptd->split));
	if (!is_null(xn.brush_color))
		xscpy(xb.color, xn.brush_color);

	widget_get_client_rect(widget, &xr);
	rect_pt_to_tm(ptd->canvas, &xr);
	expand_rect(&xr, 1, 1);

	fill_shape(ptd->canvas, &xb, &xr, ATTR_SHAPE_RECT);

	draw_split(ptd->canvas, ptd->split, &xn);

	end_display_paint(ptd->canvas, dc);
}

void hand_split_size(res_win_t widget, const resize_t* prs)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->split || !ptd->canvas)
		return;

	if (prs->code == RESIZE_MAXIMIZED || prs->code == RESIZE_MINIMIZED || prs->code == RESIZE_RESTORE)
		split_view_redraw(widget);
}

void hand_split_mouse_move(res_win_t widget, dword_t dw, short x, short y)
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

	pt.x = pt_to_tm(ptd->canvas, x, 1);
	pt.y = pt_to_tm(ptd->canvas, y, 0);

	nHint = calc_split_point_hint(ptd->split, &pt, &plk);
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

	pt.x = pt_to_tm(ptd->canvas, x, 1);
	pt.y = pt_to_tm(ptd->canvas, y, 0);

	nHint = calc_split_point_hint(ptd->split, &pt, &ilk);
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
void split_view_set_owner(res_win_t widget, res_win_t owner)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	ptd->owner = owner;
}

res_win_t split_view_get_owner(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	return ptd->owner;
}

xhand_t split_view_get_canvas(res_win_t widget)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	return ptd->canvas;
}

void split_view_attach(res_win_t widget, link_t_ptr ptr)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

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

	if (ptd->split)
	{
		data = ptd->split;
		ptd->split = NULL;
	}
	else
	{
		data = NULL;
	}
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

	assert(ptd->split && ptd->canvas);

	noti_split_reset_page(widget);

	noti_split_reset_split(widget, ptd->split);

	widget_invalid_rect(widget, NULL);

	noti_split_reset_widget(widget, ptd->split);
}

void split_view_tm_to_pt(res_win_t widget, xpoint_t* ppt)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	ppt->x = tm_to_pt(ptd->canvas, ppt->x, 1);
	ppt->y = tm_to_pt(ptd->canvas, ppt->y, 0);
}

void split_view_pt_to_tm(res_win_t widget, xpoint_t* ppt)
{
	split_delta_t* ptd = GETSPLITDELTA(widget);

	ppt->x = pt_to_tm(ptd->canvas, ppt->x, 1);
	ppt->y = pt_to_tm(ptd->canvas, ppt->y, 0);
}

res_win_t split_view_create(const wchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_split_create;
	ev.pf_on_destroy = hand_split_destroy;

	ev.pf_on_erase = hand_split_erase;
	ev.pf_on_paint = hand_split_paint;

	ev.pf_on_size = hand_split_size;

	ev.pf_on_mouse_move = hand_split_mouse_move;
	ev.pf_on_lbutton_down = hand_split_lbutton_down;
	ev.pf_on_lbutton_up = hand_split_lbutton_up;

	return widget_create(wname, wstyle, pxr->x, pxr->y, pxr->w, pxr->h, wparent, (void*)&ev);
}

