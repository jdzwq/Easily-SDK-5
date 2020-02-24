﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc menu control document

	@module	menubox.c | implement file

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

#include "xdcbox.h"
#include "handler.h"
#include "widgetnc.h"
#include "widgetex.h"

typedef struct _menu_delta_t{
	link_t_ptr menu;
	link_t_ptr item;
}menu_delta_t;

#define GETMENUDELTA(widget) 	((menu_delta_t*)widget_get_user_delta(widget))
#define SETMENUDELTA(widget,ptd) widget_set_user_delta(widget,(var_long)ptd)

/**********************************************************************************************/
static void _menubox_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	canvbox_t cb;
	if_measure_t im = { 0 };

	widgetex_get_canv_rect(widget, &cb);

	get_canvas_measure(widget_get_canvas(widget), &im);

	calc_menu_item_rect(&im, &cb, ptd->menu, ilk, pxr);

	widgetex_rect_to_pt(widget, pxr);
}

static void _menubox_reset_page(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	int pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;
	canvbox_t cb;
	if_measure_t im = { 0 };

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	get_canvas_measure(widget_get_canvas(widget), &im);

	widgetex_get_canv_rect(widget, &cb);

	xs.fx = calc_menu_width(&im, &cb, ptd->menu);
	xs.fy = calc_menu_height(&im, &cb, ptd->menu);

	widgetex_size_to_pt(widget, &xs);

	if (compare_text(get_menu_layer_ptr(ptd->menu), -1, ATTR_LAYER_HORZ, -1, 0) == 0)
	{
		fw = xs.cx;
		fh = ph;
	}
	else
	{
		fw = pw;
		fh = xs.cy;
	}

	xs.fx = get_menu_icon_span(ptd->menu);
	xs.fy = get_menu_icon_span(ptd->menu);
	widgetex_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widgetex_reset_paging(widget, pw, ph, fw, fh, lw, lh);
}

/************************************************************************************************/

void noti_menu_item_changing(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	_menubox_item_rect(widget, ptd->item, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->item = NULL;
	widget_redraw(widget, &xr, 0);
}

void noti_menu_item_changed(res_win_t widget, link_t_ptr plk)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = plk;

	_menubox_item_rect(widget, ptd->item, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

/**************************************************************************************/
int hand_menu_create(res_win_t widget, void* data)
{
	menu_delta_t* ptd;

	widgetex_hand_create(widget);

	ptd = (menu_delta_t*)xmem_alloc(sizeof(menu_delta_t));
	xmem_zero((void*)ptd, sizeof(menu_delta_t));

	SETMENUDELTA(widget, ptd);

	return 0;
}

void hand_menu_destroy(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETMENUDELTA(widget, 0);

	widgetex_hand_destroy(widget);
}

void hand_menu_size(res_win_t widget, int code, const xsize_t* prs)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

	menubox_redraw(widget);
}

void hand_menu_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

}

void hand_menu_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	int nHint;
	link_t_ptr plk = NULL;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;
	if_measure_t im = { 0 };

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 1);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widgetex_point_to_tm(widget, &pt);

	widgetex_get_canv_rect(widget, &cb);

	get_canvas_measure(widget_get_canvas(widget), &im);

	nHint = calc_menu_hint(&im, &cb, &pt, ptd->menu, &plk);

	bRe = (plk == ptd->item) ? 1 : 0;
	if (bRe)
		return;

	if (ptd->item && !bRe)
	{
		noti_menu_item_changing(widget);
	}

	if (plk && !bRe)
	{
		noti_menu_item_changed(widget, plk);
	}
}

void hand_menu_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	
	int code = 0;

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 0);

	if (ptd->item)
		code = get_menu_item_iid(ptd->item);
	else
		code = 0;

	widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), (var_long)widget);
}

void hand_menu_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 1);
}

void hand_menu_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	
	int code = 0;

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 0);

	if (ptd->item)
		code = get_menu_item_iid(ptd->item);
	else
		code = 0;

	widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), (var_long)widget);
}

void hand_menu_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

	widgetex_hand_scroll(widget, bHorz, nLine);
}

void hand_menu_erase(res_win_t widget, res_ctx_t dc)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	
	if (!ptd->menu)
		return;
}

void hand_menu_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xrect_t xr = { 0 };
	xcolor_t xc = { 0 };
	res_ctx_t rdc;

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->menu)
		return;

	widgetex_get_xfont(widget, &xf);
	widgetex_get_xbrush(widget, &xb);
	widgetex_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widgetex_get_mask(widget, &pif->clr_msk);
	widgetex_get_iconic(widget, &pif->clr_ico);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);
	draw_rect_raw(rdc, NULL, &xb, &xr);

	widgetex_get_canv_rect(widget, &cb);

	draw_menu(pif, &cb, ptd->menu);

	//draw focus
	if (ptd->item)
	{
		_menubox_item_rect(widget, ptd->item, &xr);
		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*******************************************************************************************************/

res_win_t menubox_create(res_win_t wparent, dword_t wstyle, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_menu_create)
		EVENT_ON_DESTROY(hand_menu_destroy)

		EVENT_ON_ERASE(hand_menu_erase)
		EVENT_ON_PAINT(hand_menu_paint)

		EVENT_ON_SIZE(hand_menu_size)

		EVENT_ON_SCROLL(hand_menu_scroll)

		EVENT_ON_MOUSE_MOVE(hand_menu_mouse_move)

		EVENT_ON_LBUTTON_DOWN(hand_menu_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_menu_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_menu_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_menu_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, wstyle, pxr, wparent, &ev);
}

void menubox_set_data(res_win_t widget, link_t_ptr ptr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_menu_doc(ptr));

	ptd->menu = ptr;
	ptd->item = NULL;

	menubox_redraw(widget);
}

link_t_ptr menubox_get_data(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->menu;
}

void menubox_redraw(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	b_valid = 0;
	ilk = get_menu_next_item(ptd->menu, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		ilk = get_menu_next_item(ptd->menu, ilk);
	}

	if (!b_valid)
	{
		ptd->item = NULL;
	}

	_menubox_reset_page(widget);

	widget_update(widget);
}

void menubox_tabskip(res_win_t widget, int nSkip)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	link_t_ptr plk = ptd->item;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (plk == NULL)
			plk = get_menu_next_item(ptd->menu, LINK_FIRST);
		else
			plk = get_menu_next_item(ptd->menu, ptd->item);

		if (plk)
			menubox_set_focus_item(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (plk == NULL)
			plk = get_menu_prev_item(ptd->menu, LINK_LAST);
		else
			plk = get_menu_prev_item(ptd->menu, ptd->item);

		if (plk)
			menubox_set_focus_item(widget, plk);
		break;
	case WD_TAB_HOME:
		plk = get_menu_next_item(ptd->menu, LINK_FIRST);

		if (plk)
			menubox_set_focus_item(widget, plk);
		break;
	case WD_TAB_END:
		plk = get_menu_prev_item(ptd->menu, LINK_LAST);

		if (plk)
			menubox_set_focus_item(widget, plk);
		break;
	}
}

void menubox_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_menu_item(ptd->menu, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return;

	if (ptd->item && !bRe)
	{
		noti_menu_item_changing(widget);
	}

	if (ilk && !bRe)
	{
		noti_menu_item_changed(widget, ilk);
	}
}

link_t_ptr menubox_get_focus_item(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return NULL;

	return ptd->item;
}

void menubox_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_menu_item(ptd->menu, ilk));
#endif

	_menubox_item_rect(widget, ilk, pxr);
}

void menubox_popup_size(res_win_t widget, xsize_t* pxs)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	canvbox_t cb;
	if_measure_t im = { 0 };

	XDL_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	get_canvas_measure(widget_get_canvas(widget), &im);

	widgetex_get_canv_rect(widget, &cb);

	pxs->fx = calc_menu_width(&im, &cb, ptd->menu);
	pxs->fy = calc_menu_height(&im, &cb, ptd->menu);

	widgetex_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void menubox_layout(res_win_t widget, const xpoint_t* ppt, int lay)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr = { 0 };
	xsize_t xs = { 0 };

	XDL_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	if (widget_is_child(widget))
		return;

	menubox_popup_size(widget,&xs);

	switch (lay)
	{
	case WD_LAYOUT_LEFTBOTTOM:
		xr.x = ppt->x - xs.cx;
		xr.y = ppt->y;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	case WD_LAYOUT_RIGHTBOTTOM:
		xr.x = ppt->x;
		xr.y = ppt->y;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	case WD_LAYOUT_LEFTTOP:
		xr.x = ppt->x - xs.cx;
		xr.y = ppt->y - xs.cy;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	case WD_LAYOUT_RIGHTTOP:
		xr.x = ppt->x;
		xr.y = ppt->y - xs.cy;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	}
	
	widget_size(widget, RECTSIZE(&xr));
	widget_move(widget, RECTPOINT(&xr));
	widget_update(widget);

	widget_show(widget, WD_SHOW_NORMAL);
}
