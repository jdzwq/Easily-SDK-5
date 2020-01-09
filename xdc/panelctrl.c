/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc panel control documilk

	@module	panelctrl.c | implement file

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

#include "xdcctrl.h"
#include "handler.h"
#include "widgetnc.h"
#include "widgetex.h"
#include "xdcbox.h"

typedef struct _panel_delta_t{
	link_t_ptr arch;
	link_t_ptr item;
	link_t_ptr hover;

	long title_height;
	long item_width;

	res_win_t vsc;

	bool_t b_delete;
}panel_delta_t;

#define GETPANELDELTA(ph) 	(panel_delta_t*)widget_get_user_delta(ph)
#define SETPANELDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

#define PANELCTRL_GUID_SPAN		(float)6
#define PANELCTRL_SPAN_PLUS		(long)10

typedef enum{
	_PANEL_HINT_NONE = 0,
	_PANEL_HINT_ITEM = 1,
	_PANEL_HINT_VIEW = 2,
	_PANEL_HINT_CLOSE = 4
}PANEL_HINT;
/***************************************************************************************/

static long _panelctrl_calc_width(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr ilk, doc;
	xfont_t xf;
	xface_t xa;
	res_ctx_t rdc;
	xrect_t xr;
	xsize_t xs;
	long pw;

	widgetex_get_xfont(widget, &xf);
	widgetex_get_xface(widget, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);

	rdc = widget_client_ctx(widget);

	pw = 0;
	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		

		ilk = get_arch_next_sibling_item(ilk);
	}

	widget_release_ctx(widget, rdc);

	widget_get_client_rect(widget, &xr);
	pw = xr.w;

	return pw;
}

static long _panelctrl_calc_height(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr ilk,doc;
	xfont_t xf;
	xface_t xa;
	res_ctx_t rdc;
	xrect_t xr;
	long pw, ph;

	rdc = widget_client_ctx(widget);

	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);


		ilk = get_arch_next_sibling_item(ilk);
	}

	widget_release_ctx(widget, rdc);

	widget_get_client_rect(widget, &xr);
	ph = xr.h;

	return ph;
}

static int _panelctrl_calc_hint(res_win_t widget, const xpoint_t* ppt, link_t_ptr* pplk)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr ilk,doc;
	int hint, n;
	long total = 0;

	xrect_t xr;
	xfont_t xf;
	xface_t xa;
	viewbox_t vb;

	widgetex_get_xfont(widget, &xf);
	widgetex_get_xface(widget, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);

	widgetex_get_view_rect(widget, &vb);

	*pplk = NULL;
	hint = _PANEL_HINT_NONE;

	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		xr.x = vb.px + total;
		xr.y = vb.py;
		xr.w = ptd->item_width;
		xr.h = ptd->title_height;
		if (pt_in_rect(ppt, &xr))
		{
			*pplk = ilk;
			hint = _PANEL_HINT_ITEM;
			break;
		}

		total += ptd->item_width;

		ilk = get_arch_next_sibling_item(ilk);
	}

	xr.x = vb.px + vb.pw;
	xr.y = vb.py;
	xr.w = ptd->title_height;
	xr.h = ptd->title_height;
	if (pt_in_rect(ppt, &xr))
	{
		*pplk = ilk;
		hint = _PANEL_HINT_CLOSE;
	}

	xr.x = vb.px;
	xr.y = vb.py + ptd->title_height;
	xr.w = vb.pw;
	xr.h = vb.ph - ptd->title_height;
	if (pt_in_rect(ppt, &xr))
	{
		*pplk = ilk;
		hint = _PANEL_HINT_VIEW;
	}

	return hint;
}

static void _panelctrl_item_rect(res_win_t widget, link_t_ptr plk, xrect_t* pxr)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	link_t_ptr ilk,doc;

	xrect_t xr;
	xfont_t xf;
	xface_t xa;
	viewbox_t vb;
	long n,total = 0;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	widgetex_get_xfont(widget, &xf);
	widgetex_get_xface(widget, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);

	widgetex_get_view_rect(widget, &vb);

	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		

		if (ilk == plk)
		{
			pxr->x = vb.px + total;
			pxr->y = vb.py;
			pxr->w = ptd->item_width;
			pxr->h = ptd->title_height;
			break;
		}

		total += ptd->item_width;

		ilk = get_arch_next_sibling_item(ilk);
	}
}

static void _panelctrl_reset_page(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	xrect_t xr;
	long mh;

	mh = _panelctrl_calc_height(widget);

	widget_get_client_rect(widget, &xr);

	widgetex_reset_paging(widget, xr.w, xr.h, xr.w, mh, ptd->item_width, ptd->title_height);
}

static void _panelctrl_ensure_visible(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	xrect_t xr;

	if (!ptd->item)
		return;

	_panelctrl_item_rect(widget, ptd->item, &xr);

	widgetex_ensure_visible(widget, &xr, 1);
}

/*************************************************************************/
int noti_panel_owner(res_win_t widget, unsigned long code, link_t_ptr arch, link_t_ptr item, void* data)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	NOTICE_PANEL nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.arch = arch;
	nf.item = item;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

bool_t noti_panel_item_changing(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (noti_panel_owner(widget, NC_PANELITEMCHANGING, ptd->arch, ptd->item, NULL))
		return 0;

	_panelctrl_item_rect(widget,ptd->item, &xr);

	ptd->item = NULL;

	widget_redraw(widget, NULL, 0);

	return 1;
}

void noti_panel_item_changed(res_win_t widget, link_t_ptr elk)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = elk;

	_panelctrl_item_rect(widget, ptd->item, &xr);
	
	widget_redraw(widget, NULL, 0);
}

bool_t noti_panel_item_delete(res_win_t widget, link_t_ptr ilk)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	
	return 1;
}

void noti_panel_item_enter(res_win_t widget, link_t_ptr plk)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(plk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_panel_item_leave(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_panel_item_hover(res_win_t widget, long x, long y)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_panel_owner(widget, NC_PANELITEMHOVER, ptd->arch, ptd->hover, (void*)&xp);
}
/********************************************************************************************/
int hand_panel_create(res_win_t widget, void* data)
{
	panel_delta_t* ptd;
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xsize_t xs;

	widgetex_hand_create(widget);

	ptd = (panel_delta_t*)xmem_alloc(sizeof(panel_delta_t));

	widgetex_get_xfont(widget, &xf);

	rdc = widget_client_ctx(widget);
	text_metric_raw(rdc, &xf, &xs);
	widget_release_ctx(widget, rdc);

	ptd->item_width = (long)((float)xs.cx * 8);
	ptd->title_height = (long)((float)xs.cy * 1.25);

	SETPANELDELTA(widget, ptd);

	return 0;
}

void hand_panel_destroy(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETPANELDELTA(widget, 0);

	widgetex_hand_destroy(widget);
}

void hand_panel_keydown(res_win_t widget, int key)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	switch (key)
	{
	case KEY_ENTER:
		break;
	case KEY_SPACE:
		break;
	case KEY_LEFT:
		panelctrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		panelctrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_HOME:
		panelctrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		panelctrl_tabskip(widget,WD_TAB_END);
		break;
	}
}

void hand_panel_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr plk = NULL;
	int hint;

	if (!ptd->arch)
		return;

	hint = _panelctrl_calc_hint(widget, pxp, &plk);

	if (!ptd->hover && plk)
	{
		noti_panel_item_enter(widget, plk);
	}
	else if (ptd->hover && ptd->hover != plk)
	{
		noti_panel_item_leave(widget);
	}
}

void hand_panel_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	if (ptd->hover)
		noti_panel_item_hover(widget, pxp->x, pxp->y);
}

void hand_panel_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	if (ptd->hover)
		noti_panel_item_leave(widget);
}

void hand_panel_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_panel_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr ilk = NULL;
	int hint;
	bool_t bRe;

	if (!ptd->arch)
		return;

	hint = _panelctrl_calc_hint(widget, pxp, &ilk);

	if (ptd->b_delete && hint == _PANEL_HINT_CLOSE)
	{
		if (noti_panel_item_delete(widget, ilk))
		{
			delete_arch_item(ilk);

			panelctrl_redraw(widget);
			return;
		}
	}

	bRe = (ilk == ptd->item) ? 1 : 0;

	if (!bRe && ptd->item)
	{
		if (!noti_panel_item_changing(widget))
			bRe = 1;
	}

	if (ilk && !bRe)
	{
		noti_panel_item_changed(widget, ilk);
	}

	noti_panel_owner(widget, NC_PANELLBCLK, ptd->arch, ptd->item, (void*)pxp);
}

void hand_panel_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	noti_panel_owner(widget, NC_PANELDBCLK, ptd->arch, ptd->item, (void*)pxp);
}

void hand_panel_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;
}

void hand_panel_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	noti_panel_owner(widget, NC_PANELRBCLK, ptd->arch, ptd->item, (void*)pxp);
}

void hand_panel_size(res_win_t widget, int code, const xsize_t* prs)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	panelctrl_redraw(widget);
}

void hand_panel_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	if (!ptd->arch)
		return;

	widgetex_hand_scroll(widget, bHorz, nLine);
}

void hand_panel_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->arch)
		return;

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widgetex_hand_scroll(widget, bHorz, nLine))
	{
		if (!bHorz && !(widget_get_style(widget) & WD_STYLE_VSCROLL))
		{
			if (!widget_is_valid(ptd->vsc))
			{
				ptd->vsc = show_vertbox(widget);
			}
		}

		return;
	}

	win = widget_get_parent(widget);

	if (widget_is_valid(win))
	{
		widget_scroll(win, bHorz, nLine);
	}
}

void hand_panel_erase(res_win_t widget, res_ctx_t rdc)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	
	if (!ptd->arch)
		return;
}

void hand_panel_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	res_ctx_t rdc;
	canvas_t canv;

	link_t_ptr ilk,doc;
	xrect_t xr_icon,xr_view,xr;
	xpoint_t pt[9];
	xsize_t xs;
	long total = 0;;

	viewbox_t vb = { 0 };
	xfont_t xf_top,xf = { 0 };
	xface_t xa_top,xa = { 0 };
	xbrush_t xb_bar, xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };

	const tchar_t* title = NULL;

	if (!ptd->arch)
		return;

	widgetex_get_xfont(widget, &xf);
	widgetex_get_xface(widget, &xa);
	widgetex_get_xbrush(widget, &xb);
	widgetex_get_xpen(widget, &xp);
	widgetex_get_iconic(widget, &xc);

	memcpy((void*)&xf_top, (void*)&xf, sizeof(xfont_t));
	xscpy(xf_top.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	memcpy((void*)&xa_top, (void*)&xa, sizeof(xface_t));

	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	xscpy(xa_top.text_wrap, _T(""));

	memcpy((void*)&xb_bar, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bar, DEF_SOFT_DARKEN);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widgetex_get_view_rect(widget, &vb);

	pt_offset_rect(&xr, vb.px, vb.py);

	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		if (ilk == ptd->item)
		{
			pt[0].x = 0;
			pt[0].y = xr.y + ptd->title_height;
			pt[1].x = xr.x;
			pt[1].y = xr.y + ptd->title_height;
			pt[2].x = xr.x;
			pt[2].y = xr.y;
			pt[3].x = xr.x + ptd->item_width;
			pt[3].y = xr.y;
			pt[4].x = xr.x + ptd->item_width;
			pt[4].y = xr.y + ptd->title_height;
			pt[5].x = xr.w;
			pt[5].y = xr.y + ptd->title_height;
			pt[6].x = xr.w;
			pt[6].y = xr.h;
			pt[7].x = 0;
			pt[7].y = xr.h;
			pt[8].x = 0;
			pt[8].y = xr.y + ptd->title_height;

			draw_polyline_raw(rdc, &xp, pt, 9);
		}

		xr_icon.x = xr.x;
		xr_icon.y = xr.y;
		xr_icon.w = ptd->title_height;
		xr_icon.h = ptd->title_height;
		pt_center_rect(&xr_icon, 12, 12);
		draw_icon_raw(rdc, &xc, &xr_icon, ICON_BOOK);

		xr_icon.x = xr.x + 12;
		xr_icon.y = xr.y;
		xr_icon.w = ptd->item_width - 12;
		xr_icon.h = ptd->title_height;
		draw_text_raw(rdc, &xf, &xa, &xr_icon, title, -1);

		xr.x += ptd->item_width;

		ilk = get_arch_next_sibling_item(ilk);
	}

	end_canvas_paint(canv, dc, pxr);
}

/************************************************************************************************/
res_win_t panelctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_panel_create)
		EVENT_ON_DESTROY(hand_panel_destroy)

		EVENT_ON_ERASE(hand_panel_erase)
		EVENT_ON_PAINT(hand_panel_paint)

		EVENT_ON_SIZE(hand_panel_size)

		EVENT_ON_SCROLL(hand_panel_scroll)
		EVENT_ON_WHEEL(hand_panel_wheel)

		EVENT_ON_KEYDOWN(hand_panel_keydown)

		EVENT_ON_MOUSE_MOVE(hand_panel_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_panel_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_panel_mouse_leave)

		EVENT_ON_LBUTTON_DOWN(hand_panel_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_panel_lbutton_up)
		EVENT_ON_LBUTTON_DBCLICK(hand_panel_lbutton_dbclick)
		EVENT_ON_RBUTTON_DOWN(hand_panel_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_panel_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void panelctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_arch_doc(ptr));

	ptd->arch = ptr;

	panelctrl_redraw(widget);
}

link_t_ptr panelctrl_detach(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->arch;
	ptd->arch = NULL;

	panelctrl_redraw(widget);

	return data;
}

link_t_ptr panelctrl_fetch(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->arch;
}

void panelctrl_enable_delete(res_win_t widget, bool_t bDelete)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_delete = bDelete;
}

void panelctrl_redraw(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr ilk, doc;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	b_valid = 0;
	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		if (ilk == ptd->item)
			b_valid = 1;

		noti_panel_owner(widget, NC_PANELITEMCALCED, ptd->arch, ilk, NULL);

		ilk = get_arch_next_sibling_item(ilk);
	}

	noti_panel_owner(widget, NC_PANELCALCED, ptd->arch, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}

	_panelctrl_reset_page(widget);

	widget_update(widget);
}

void panelctrl_redraw_item(res_win_t widget, link_t_ptr ilk)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	noti_panel_owner(widget, NC_PANELITEMCALCED, ptd->arch, ilk, NULL);

	_panelctrl_item_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

bool_t panelctrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return 0;

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_arch_document(ptd->arch, ilk) || is_arch_catalog(ptd->arch, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? 1 : 0;

	if (!bRe && ptd->item)
	{
		if (!noti_panel_item_changing(widget))
			return 0;
	}

	if (!bRe && ilk)
	{
		noti_panel_item_changed(widget, ilk);

		_panelctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr panelctrl_get_focus_item(res_win_t widget)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return NULL;

	return ptd->item;
}

void panelctrl_get_item_rect(res_win_t widget, link_t_ptr elk, xrect_t* prt)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	_panelctrl_item_rect(widget, elk, prt);
}

void panelctrl_tabskip(res_win_t widget, int nSkip)
{
	panel_delta_t* ptd = GETPANELDELTA(widget);
	link_t_ptr plk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->item)
			plk = get_arch_next_sibling_item(ptd->item);
		else
			plk = get_arch_first_child_item(ptd->arch);

		if (plk)
			panelctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->item)
			plk = get_arch_prev_sibling_item(ptd->item);
		else
			plk = get_arch_last_child_item(ptd->arch);

		if (plk)
			panelctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_HOME:
		plk = get_arch_first_child_item(ptd->arch);

		if (plk)
			panelctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_END:
		plk = get_arch_last_child_item(ptd->arch);

		if (plk)
			panelctrl_set_focus_item(widget, plk);
		break;
	}
}
