/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc label control document

	@module	labelctrl.c | implement file

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

typedef struct label_delta_t{
	link_t_ptr label;
	link_t_ptr item;
	link_t_ptr hover;

	int cur_page;
	bool_t b_drag;

	res_win_t hsc;
	res_win_t vsc;

}label_delta_t;

#define GETLABELDELTA(ph) 	(label_delta_t*)widget_get_user_delta(ph)
#define SETLABELDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/********************************************************************************************/
void _labelctrl_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	canvbox_t cb;

	widgetex_get_canv_rect(widget, &cb);

	calc_label_item_rect(&cb, ptd->label, ptd->cur_page, ilk, pxr);

	widgetex_rect_to_pt(widget, pxr);
}

void _labelctrl_reset_page(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_label_width(ptd->label);
	xs.fy = get_label_height(ptd->label);
	widgetex_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = get_label_item_width(ptd->label);
	xs.fy = get_label_item_height(ptd->label);
	widgetex_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widgetex_reset_paging(widget, pw, ph, pw, ph, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

void _labelctrl_ensure_visible(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	xrect_t xr;

	if (!ptd->item)
		return;

	_labelctrl_item_rect(widget, ptd->item, &xr);

	widgetex_ensure_visible(widget, &xr, 1);
}

/******************************************control event***************************************/

int noti_label_owner(res_win_t widget, unsigned long code, link_t_ptr label, link_t_ptr ilk, void* data)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	NOTICE_LABEL nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.label = label;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

bool_t noti_label_item_changing(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (noti_label_owner(widget, NC_LABELITEMCHANGING, ptd->label, ptd->item, NULL))
		return 0;

	_labelctrl_item_rect(widget, ptd->item, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->item = NULL;

	widget_redraw(widget, &xr, 0);

	return 1;
}

void noti_label_item_changed(res_win_t widget, link_t_ptr plk)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = plk;

	_labelctrl_item_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	noti_label_owner(widget, NC_LABELITEMCHANGED, ptd->label, ptd->item, NULL);
}

void noti_label_item_enter(res_win_t widget, link_t_ptr plk)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(plk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_label_item_leave(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_label_item_hover(res_win_t widget, long x, long y)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_label_owner(widget, NC_LABELITEMHOVER, ptd->label, ptd->hover, (void*)&xp);
}

void noti_label_item_drag(res_win_t widget, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item);

	ptd->b_drag = 1;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	pt.x = pxp->x;
	pt.y = pxp->y;
	noti_label_owner(widget, NC_LABELITEMDRAG, ptd->label, ptd->item, (void*)&pt);
}

void noti_label_item_drop(res_win_t widget, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item);

	ptd->b_drag = 0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	pt.x = pxp->x;
	pt.y = pxp->y;
	noti_label_owner(widget, NC_LABELITEMDROP, ptd->label, ptd->item, (void*)&pt);
}

/******************************************************************************/

int hand_label_create(res_win_t widget, void* data)
{
	label_delta_t* ptd;

	widgetex_hand_create(widget);

	ptd = (label_delta_t*)xmem_alloc(sizeof(label_delta_t));
	xmem_zero((void*)ptd, sizeof(label_delta_t));

	SETLABELDELTA(widget, ptd);

	return 0;
}

void hand_label_destroy(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETLABELDELTA(widget, 0);

	widgetex_hand_destroy(widget);
}

void hand_label_size(res_win_t widget, int code, const xsize_t* prs)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	labelctrl_redraw(widget);
}

void hand_label_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	widgetex_hand_scroll(widget, bHorz, nLine);
}

void hand_label_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->label)
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

		if (bHorz && !(widget_get_style(widget) & WD_STYLE_HSCROLL))
		{
			if (!widget_is_valid(ptd->hsc))
			{
				ptd->hsc = show_horzbox(widget);
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

void hand_label_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	canvbox_t cb;
	xpoint_t pt;

	if (!ptd->label)
		return;

	if (ptd->b_drag)
		return;

	widgetex_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widgetex_point_to_tm(widget, &pt);

	nHint = calc_label_hint(&cb, &pt, ptd->label, ptd->cur_page, &ilk);

	if (nHint == LABEL_HINT_ITEM && ilk == ptd->item && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_label_item_drag(widget, pxp);
			return;
		}
	}

	if (nHint == LABEL_HINT_ITEM && !ptd->hover && ilk)
	{
		noti_label_item_enter(widget, ilk);
	}
	else if (nHint == LABEL_HINT_ITEM && ptd->hover && ptd->hover != ilk)
	{
		noti_label_item_leave(widget);
	}
	else if (nHint == LABEL_HINT_NONE && ptd->hover)
	{
		noti_label_item_leave(widget);
	}
}

void hand_label_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	if (ptd->hover)
		noti_label_item_hover(widget, pxp->x, pxp->y);
}

void hand_label_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	if (ptd->hover)
		noti_label_item_leave(widget);
}

void hand_label_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	noti_label_owner(widget, NC_LABELDBCLK,ptd->label, ptd->item, (void*)pxp);
}

void hand_label_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_label_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	canvbox_t cb;
	xpoint_t pt;
	link_t_ptr plk;
	int nHint;
	bool_t bRe;

	if (!ptd->label)
		return;

	if (ptd->b_drag)
	{
		noti_label_item_drop(widget, pxp);
		return;
	}

	widgetex_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widgetex_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_label_hint(&cb, &pt, ptd->label, ptd->cur_page, &plk);

	bRe = (plk == ptd->item) ? 1 : 0;

	if (ptd->item && !bRe)
	{
		if (!noti_label_item_changing(widget))
			bRe = 1;
	}

	if (plk && !bRe)
	{
		noti_label_item_changed(widget, plk);

		_labelctrl_ensure_visible(widget);
	}

	noti_label_owner(widget, NC_LABELLBCLK, ptd->label, ptd->item, (void*)pxp);
}

void hand_label_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;
}

void hand_label_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	noti_label_owner(widget, NC_LABELRBCLK, ptd->label, ptd->item, (void*)pxp);
}

void hand_label_keydown(res_win_t widget, int nKey)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;

	switch (nKey)
	{
	case KEY_ENTER:
		break;
	case KEY_SPACE:
		break;
	case KEY_LEFT:
	case KEY_UP:
		labelctrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
	case KEY_DOWN:
		labelctrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_PAGEUP:
		labelctrl_tabskip(widget,WD_TAB_PAGEUP);
		break;
	case KEY_PAGEDOWN:
		labelctrl_tabskip(widget,WD_TAB_PAGEDOWN);
		break;
	case KEY_HOME:
		labelctrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		labelctrl_tabskip(widget,WD_TAB_END);
		break;
	}
}

void hand_label_erase(res_win_t widget, res_ctx_t dc)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	if (!ptd->label)
		return;
}

void hand_label_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	res_ctx_t rdc;
	xrect_t xr = { 0 };
	xfont_t xf = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->label)
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

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widgetex_get_canv_rect(widget, &cb);

	draw_label(pif, &cb, ptd->label, ptd->cur_page);

	//draw focus
	if (ptd->item)
	{
		_labelctrl_item_rect(widget, ptd->item, &xr);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/**********************************************control method****************************************/
res_win_t labelctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_label_create)
		EVENT_ON_DESTROY(hand_label_destroy)

		EVENT_ON_ERASE(hand_label_erase)
		EVENT_ON_PAINT(hand_label_paint)

		EVENT_ON_SIZE(hand_label_size)

		EVENT_ON_SCROLL(hand_label_scroll)
		EVENT_ON_WHEEL(hand_label_wheel)

		EVENT_ON_KEYDOWN(hand_label_keydown)

		EVENT_ON_MOUSE_MOVE(hand_label_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_label_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_label_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_label_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_label_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_label_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_label_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_label_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void labelctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_label_doc(ptr));

	ptd->label = ptr;
	ptd->cur_page = 1;
	labelctrl_redraw(widget);
}

link_t_ptr labelctrl_detach(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	if (ptd->label)
	{
		data = ptd->label;
		ptd->label = NULL;
	}
	else
	{
		data = NULL;
	}

	ptd->cur_page = 0;

	return data;
}

link_t_ptr labelctrl_fetch(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->label;
}

void labelctrl_redraw(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;
	bool_t b;
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	b_valid = 0;
	ilk = get_label_next_item(ptd->label, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		noti_label_owner(widget, NC_LABELITEMCALCED, ptd->label, ilk, NULL);

		ilk = get_label_next_item(ptd->label, ilk);
	}

	noti_label_owner(widget, NC_LABELCALCED, ptd->label, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}
	ptd->hover = NULL;

	b = (widget_get_style(widget) & WD_STYLE_PAGING) ? 1 : 0;
	if (!b)
	{
		widget_get_client_rect(widget, &xr);
		widgetex_rect_to_tm(widget, &xr);
		set_label_height(ptd->label, xr.fh);
	}

	_labelctrl_reset_page(widget);

	widget_update(widget);
}

void labelctrl_tabskip(res_win_t widget, int nSkip)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	link_t_ptr plk = ptd->item;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (plk == NULL)
			plk = get_label_next_item(ptd->label, LINK_FIRST);
		else
			plk = get_label_next_item(ptd->label, plk);

		if (plk)
			labelctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (plk == NULL)
			plk = get_label_prev_item(ptd->label, LINK_LAST);
		else
			plk = get_label_prev_item(ptd->label, plk);

		if (plk)
			labelctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_HOME:
		labelctrl_move_first_page(widget);
		break;
	case WD_TAB_END:
		labelctrl_move_last_page(widget);
		break;
	case WD_TAB_PAGEUP:
		labelctrl_move_prev_page(widget);
		break;
	case WD_TAB_PAGEDOWN:
		labelctrl_move_next_page(widget);
		break;
	}
}

void labelctrl_redraw_item(res_win_t widget, link_t_ptr plk)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_label_item(ptd->label, plk));
#endif

	noti_label_owner(widget, NC_LABELITEMCALCED, ptd->label, plk, NULL);

	_labelctrl_item_rect(widget, plk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

bool_t labelctrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return 0;

	if (ilk == LINK_FIRST)
		ilk = get_label_next_item(ptd->label, LINK_FIRST);
	else if (ilk == LINK_LAST)
		ilk = get_label_prev_item(ptd->label, LINK_LAST);

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->item && !bRe)
	{
		if (!noti_label_item_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		noti_label_item_changed(widget, ilk);

		_labelctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr labelctrl_get_focus_item(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->item;
}

void labelctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_label_item(ptd->label, ilk));
#endif

	_labelctrl_item_rect(widget, ilk, pxr);
}

void labelctrl_move_first_page(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	nCurPage = ptd->cur_page;

	if (nCurPage != 1)
	{
		nCurPage = 1;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void labelctrl_move_prev_page(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	nCurPage = ptd->cur_page;

	if (nCurPage > 1)
	{
		nCurPage--;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void labelctrl_move_next_page(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	widgetex_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_label_pages(&cb, ptd->label);

	if (nCurPage < nMaxPage)
	{
		nCurPage++;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void labelctrl_move_last_page(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	widgetex_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_label_pages(&cb, ptd->label);

	if (nCurPage != nMaxPage)
	{
		nCurPage = nMaxPage;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void labelctrl_move_to_page(res_win_t widget, int page)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	widgetex_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_label_pages(&cb, ptd->label);

	if (page > 0 && page != nCurPage && page <= nMaxPage)
	{
		nCurPage = page;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

int labelctrl_get_max_page(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return 0;

	widgetex_get_canv_rect(widget, &cb);

	return calc_label_pages(&cb, ptd->label);
}

int labelctrl_get_cur_page(res_win_t widget)
{
	label_delta_t* ptd = GETLABELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return 0;

	return ptd->cur_page;
}

void calc_label_suit_size(link_t_ptr ptr, xsize_t* pxs)
{
	float ih, iw;
	int count;

	ih = get_label_item_height(ptr);
	iw = get_label_item_width(ptr);

	count = get_label_item_count(ptr);

	if (count > 10)
	{
		pxs->fx = 3 * iw;
		pxs->fy = 5 * ih;
	}
	else if (count > 5)
	{
		pxs->fx = 2 * iw;
		pxs->fy = 5 * ih;
	}
	else
	{
		pxs->fx = iw;
		pxs->fy = count * ih;
	}
}

void labelctrl_popup_size(res_win_t widget, xsize_t* pse)
{
	label_delta_t* ptd = GETLABELDELTA(widget);
	float ih, iw;
	int count;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->label)
		return;

	ih = get_label_item_height(ptd->label);
	iw = get_label_item_width(ptd->label);

	count = get_label_item_count(ptd->label);

	if (count > 10)
	{
		pse->fx = 3 * iw;
		pse->fy = 5 * ih;
	}
	else if (count > 5)
	{
		pse->fx = 2 * iw;
		pse->fy = 5 * ih;
	}
	else
	{
		pse->fx = iw;
		pse->fy = count * ih;
	}

	widgetex_size_to_pt(widget, pse);

	widget_adjust_size(widget_get_style(widget), pse);
}
