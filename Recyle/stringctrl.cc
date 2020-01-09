/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc string control document

	@module	stringctrl.c | string control widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcclip.h"
#include "xdcgdi.h"
#include "xdccanv.h"

typedef struct _string_delta_t{
	LINKPTR string;
	LINKPTR entity;
	LINKPTR hover;
	long bw, bh;

	bool_t b_delete;
}string_delta_t;

#define GETSTRINGDELTA(ph) 	(string_delta_t*)widget_get_user_delta(ph)
#define SETSTRINGDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

#define set_string_entity_hidden(ilk, b)		set_string_entity_delta(ilk, (void*)b)
#define get_string_entity_hidden(ilk)			(bool_t)get_string_entity_delta(ilk)

#define STRINGCTRL_GUID_SPAN		(float)6

typedef enum{
	_STRING_HINT_NONE = 0,
	_STRING_HINT_KEY = 1,
	_STRING_HINT_VAL = 2,
	_STRING_HINT_CLOSE = 3
}STRING_HINT;
/***************************************************************************************/

link_t_ptr get_string_next_visible_entity(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		ilk = get_string_next_entity(ptr, LINK_FIRST);
	else if (pos == LINK_LAST)
		return NULL;
	else
		ilk = get_string_next_entity(ptr, pos);

	while (ilk)
	{
		if (!get_string_entity_hidden(ilk))
		{
			break;
		}

		ilk = get_string_next_entity(ptr, ilk);
	}

	return ilk;
}

link_t_ptr get_string_prev_visible_entity(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		ilk = get_string_prev_entity(ptr, LINK_LAST);
	else
		ilk = get_string_prev_entity(ptr, pos);

	while (ilk)
	{
		if (!get_string_entity_hidden(ilk))
		{
			break;
		}

		ilk = get_string_prev_entity(ptr, ilk);
	}

	return ilk;
}

static long _stringctrl_calc_width(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	LINKPTR ilk;
	xfont_t xf;
	xface_t xa;
	res_dc_t rdc;
	xsize_t xs;
	long pw;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	rdc = widget_client_dc(widget);

	pw = 0;
	ilk = get_string_next_visible_entity(ptd->string, LINK_FIRST);
	while (ilk)
	{
		text_size_raw(rdc, &xf, get_string_entity_val_ptr(ilk), -1, &xs.cx, &xs.cy);
		if (pw < xs.cx)
			pw = xs.cx;

		ilk = get_string_next_visible_entity(ptd->string, ilk);
	}

	widget_release_dc(widget, rdc);

	return pw + ptd->bw;
}

static long _stringctrl_calc_height(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	LINKPTR ilk;
	xfont_t xf;
	xface_t xa;
	res_dc_t rdc;
	xrect_t xr;
	long pw,ph;

	widget_get_client_rect(widget, &xr);
	pw = xr.w - ptd->bw;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	rdc = widget_client_dc(widget);

	ph = 0;
	ilk = get_string_next_visible_entity(ptd->string, LINK_FIRST);
	while (ilk)
	{
		xr.x = xr.y = 0;
		xr.w = pw;
		xr.h = ptd->bh;
		text_rect_raw(rdc, &xf, &xa, get_string_entity_val_ptr(ilk), -1, &xr);
		if (xr.h < ptd->bh)
			xr.h = ptd->bh;

		ph += (xr.h + ptd->bh);

		ilk = get_string_next_visible_entity(ptd->string, ilk);
	}

	widget_release_dc(widget, rdc);

	return ph;
}

static int _stringctrl_calc_hint(res_win_t widget, const xpoint_t* ppt, LINKPTR* pplk)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	res_dc_t rdc;
	LINKPTR ilk;
	int hint;

	xrect_t xr,xrClose;
	xfont_t xf;
	xface_t xa;
	viewbox_t vb;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_view_rect(widget, &vb);

	xr.x = vb.bx;
	xr.y = vb.by;

	rdc = widget_client_dc(widget);

	*pplk = NULL;
	hint = _STRING_HINT_NONE;

	ilk = get_string_next_visible_entity(ptd->string, LINK_FIRST);
	while (ilk)
	{
		xrClose.x = vb.bx + vb.bw - ptd->bw;
		xrClose.y = xr.y;
		xrClose.w = ptd->bw;
		xrClose.h = ptd->bh;
		if (pt_in_rect(ppt, &xrClose))
		{
			*pplk = ilk;
			hint = _STRING_HINT_CLOSE;
			break;
		}

		xr.w = vb.bw - ptd->bw;
		xr.h = ptd->bh;
		if (pt_in_rect(ppt, &xr))
		{
			*pplk = ilk;
			hint = _STRING_HINT_KEY;
			break;
		}

		xr.w = vb.bw - ptd->bw;
		xr.y += ptd->bh;
		xr.h = ptd->bh;
		text_rect_raw(rdc, &xf, &xa, get_string_entity_val_ptr(ilk), -1, &xr);

		if (xr.h < ptd->bh)
			xr.h = ptd->bh;

		xr.w = vb.bw - ptd->bw;

		if (pt_in_rect(ppt, &xr))
		{
			*pplk = ilk;
			hint = _STRING_HINT_VAL;
			break;
		}

		xr.y += xr.h;
		ilk = get_string_next_visible_entity(ptd->string, ilk);
	}

	widget_release_dc(widget, rdc);

	return hint;
}

static void _stringctrl_entity_rect(res_win_t widget, link_t_ptr plk, xrect_t* pxr)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	res_dc_t rdc;
	LINKPTR ilk;

	xrect_t xr;
	xfont_t xf;
	xface_t xa;
	viewbox_t vb;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_view_rect(widget, &vb);

	xr.x = vb.bx;
	xr.y = vb.by;

	rdc = widget_client_dc(widget);

	ilk = get_string_next_visible_entity(ptd->string, LINK_FIRST);
	while (ilk)
	{
		xr.w = vb.bw - ptd->bw;
		xr.h = ptd->bh;
		text_rect_raw(rdc, &xf, &xa, get_string_entity_val_ptr(ilk), -1, &xr);

		if (xr.h < ptd->bh)
			xr.h = ptd->bh;

		xr.w = vb.bw - ptd->bw;

		if (ilk == plk)
		{
			pxr->x = vb.bx;
			pxr->y = xr.y;
			pxr->w = vb.bw;
			pxr->h = xr.h + ptd->bh;
			break;
		}

		xr.y += (xr.h + ptd->bh);
		ilk = get_string_next_visible_entity(ptd->string, ilk);
	}

	widget_release_dc(widget, rdc);
}

static void _stringctrl_reset_page(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	xrect_t xr;
	long mh;

	mh = _stringctrl_calc_height(widget);

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, xr.w, mh, ptd->bh, ptd->bh);
}

static void _stringctrl_ensure_visible(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	xrect_t xr;

	if (!ptd->entity)
		return;

	_stringctrl_entity_rect(widget, ptd->entity, &xr);
	widget_ensure_visible(widget, &xr, 1);
}

/*************************************************************************/
int noti_string_owner(res_win_t widget, unsigned long code, link_t_ptr string, link_t_ptr ilk, void* data)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	NOTICE_STRING nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.string = string;
	nf.entity = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

bool_t noti_string_entity_changing(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	xrect_t xr;

	XDC_ASSERT(ptd->entity);

	if (noti_string_owner(widget, NC_STRINGENTITYCHANGING, ptd->string, ptd->entity, NULL))
		return 0;

	_stringctrl_entity_rect(widget,ptd->entity, &xr);

	ptd->entity = NULL;

	widget_invalid(widget, &xr, 0);

	return 1;
}

void noti_string_entity_changed(res_win_t widget, link_t_ptr elk)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	xrect_t xr;

	XDC_ASSERT(!ptd->entity);

	ptd->entity = elk;

	_stringctrl_entity_rect(widget, ptd->entity, &xr);
	
	widget_invalid(widget, &xr, 0);
}

bool_t noti_string_entity_delete(res_win_t widget, link_t_ptr ilk)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	if (noti_string_owner(widget, NC_STRINGENTITYDELETE, ptd->string, ilk, NULL))
		return 0;

	return 1;
}

void noti_string_entity_enter(res_win_t widget, link_t_ptr plk)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(plk);
	XDC_ASSERT(!ptd->hover);

	ptd->hover = plk;

	widget_track_mouse(widget, MOUSE_HOVER | MOUSE_LEAVE);
}

void noti_string_entity_leave(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	widget_track_mouse(widget, MOUSE_HOVER | TRACK_CANCEL);
}

void noti_string_entity_hover(res_win_t widget, long x, long y)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	xpoint_t xp;

	XDC_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_string_owner(widget, NC_STRINGENTITYHOVER, ptd->string, ptd->hover, (void*)&xp);
}
/********************************************************************************************/
int hand_string_create(res_win_t widget, void* data)
{
	string_delta_t* ptd;
	res_dc_t rdc;
	xhand_t canv;
	xfont_t xf = { 0 };
	xsize_t xs;

	ptd = (string_delta_t*)xmem_alloc(sizeof(string_delta_t));

	widget_get_xfont(widget, &xf);

	rdc = widget_client_dc(widget);
	text_metric_raw(rdc, &xf, &xs.cx, &xs.cy);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_attach_canvas(widget, canv);

	ptd->bw = (long)((float)xs.cy * 2);
	ptd->bh = (long)((float)xs.cy * 1.25);

	SETSTRINGDELTA(widget, ptd);

	return 0;
}

void hand_string_destroy(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	destroy_display_canvas(widget_detach_canvas(widget));

	xmem_free(ptd);

	SETSTRINGDELTA(widget, 0);
}

void hand_string_keydown(res_win_t widget, int key)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	if (!ptd->string)
		return;

	switch (key)
	{
	case KEY_ENTER:
		break;
	case KEY_SPACE:
		break;
	case KEY_LEFT:
		stringctrl_tabskip(widget, NF_LEFT);
		break;
	case KEY_RIGHT:
		stringctrl_tabskip(widget, NF_RIGHT);
		break;
	case KEY_HOME:
		stringctrl_tabskip(widget, NF_HOME);
		break;
	case KEY_END:
		stringctrl_tabskip(widget, NF_END);
		break;
	}
}

void hand_string_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	link_t_ptr plk = NULL;
	int hint;

	if (!ptd->string)
		return;

	hint = _stringctrl_calc_hint(widget, pxp, &plk);
	
	if (!ptd->hover && plk)
	{
		noti_string_entity_enter(widget, plk);
		return;
	}

	if (ptd->hover && ptd->hover != plk)
	{
		noti_string_entity_leave(widget);
		return;
	}

	if (ptd->hover)
	{
		noti_string_entity_leave(widget);
	}
}

void hand_string_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	if (!ptd->string)
		return;

	if (ptd->hover)
		noti_string_entity_hover(widget, pxp->x, pxp->y);
}

void hand_string_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	if (!ptd->string)
		return;

	if (ptd->hover)
		noti_string_entity_leave(widget);
}

void hand_string_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	if (!ptd->string)
		return;
}

void hand_string_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	LINKPTR ilk = NULL;
	int hint;

	if (!ptd->string)
		return;

	hint = _stringctrl_calc_hint(widget, pxp, &ilk);

	if (ptd->b_delete && hint == _STRING_HINT_CLOSE)
	{
		if (noti_string_entity_delete(widget, ilk))
		{
			delete_string_entity(ptd->string, ilk);

			stringctrl_redraw(widget, 1);
			return;
		}
	}

	if (ilk != ptd->entity)
	{
		if (ptd->entity)
			noti_string_entity_changing(widget);

		if (ilk)
			noti_string_entity_changed(widget, ilk);
	}

	noti_string_owner(widget, NC_STRINGLBCLK, ptd->string, ptd->entity, (void*)pxp);
}

void hand_string_size(res_win_t widget, int code, const xsize_t* prs)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	if (!ptd->string)
		return;

	stringctrl_redraw(widget, 1);
}

void hand_string_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	if (!ptd->string)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_string_command_color(res_win_t widget, void* data)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	widget_set_color_mode(widget, (clr_mod_t*)data);

	widget_update_window(widget);
	widget_update_client(widget);
}

void hand_string_command_switch(res_win_t widget, bool_t bkey)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	dword_t ws;

	ws = widget_get_style(widget);

	if (bkey)
		ws |= (WD_STYLE_TOUCH);
	else
		ws &= ~(WD_STYLE_TOUCH);

	widget_set_style(widget, ws);

	widget_update_window(widget);
}

void hand_string_erase(res_win_t widget, res_dc_t rdc)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	
	if (!ptd->string)
		return;
}

void hand_string_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	res_dc_t rdc;
	xhand_t canv;

	link_t_ptr ilk;
	xrect_t xr_btn,xr_txt,xr;
	xpoint_t pt_org, pt_cur;

	viewbox_t vb = { 0 };
	xfont_t xf_top,xf = { 0 };
	xface_t xa_top,xa = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };

	if (!ptd->string)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	parse_xcolor(&xc, xp.color);

	memcpy((void*)&xf_top, (void*)&xf, sizeof(xfont_t));
	xscpy(xf_top.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	memcpy((void*)&xa_top, (void*)&xa, sizeof(xface_t));

	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	xscpy(xa_top.text_wrap, _T(""));

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_view_rect(widget, &vb);

	xr.x = vb.bx;
	xr.y = vb.by;
	xr.w = vb.bw;

	ilk = get_string_next_visible_entity(ptd->string, LINK_FIRST);
	while (ilk)
	{
		xr_txt.x = xr.x + ptd->bw;
		xr_txt.y = xr.y;
		xr_txt.w = vb.bw;
		xr_txt.h = ptd->bh;

		draw_text_raw(rdc, &xf_top, &xa_top, &xr_txt, get_string_entity_key_ptr(ilk), -1);

		pt_cur.x = xr.x + ptd->bw / 2;
		pt_cur.y = xr.y + ptd->bh / 2;

		if (ilk != get_string_next_visible_entity(ptd->string, LINK_FIRST))
		{
			draw_line_raw(rdc, &xp, &pt_cur, &pt_org);
		}

		xr_btn.x = xr.x;
		xr_btn.y = xr.y;
		xr_btn.w = ptd->bw;
		xr_btn.h = ptd->bh;

		if (ilk == ptd->entity)
			draw_icon_raw(rdc, &xc, &xr_btn, ATTR_ICON_GUIDER);
		else
			draw_icon_raw(rdc, &xc, &xr_btn, ATTR_ICON_RADIOED);

		if (ptd->b_delete)
		{
			xr_btn.x = vb.bx + vb.bw - ptd->bw;
			xr_btn.y = xr.y;
			xr_btn.w = ptd->bw;
			xr_btn.h = ptd->bh;

			draw_icon_raw(rdc, &xc, &xr_btn, ATTR_ICON_CLOSE);
		}

		xr_txt.x = xr.x + ptd->bw;
		xr_txt.y = xr.y + ptd->bh;
		xr_txt.w = vb.bw - ptd->bw;
		xr_txt.h = ptd->bh;
		text_rect_raw(rdc, &xf, &xa, get_string_entity_val_ptr(ilk), -1, &xr_txt);

		if (xr_txt.h < ptd->bh)
			xr_txt.h = ptd->bh;
		xr_txt.w = vb.bw - ptd->bw;

		draw_text_raw(rdc, &xf, &xa, &xr_txt, get_string_entity_val_ptr(ilk), -1);

		pt_org.x = pt_cur.x;
		pt_org.y = pt_cur.y;

		xr.y += (xr_txt.h + ptd->bh);
		ilk = get_string_next_visible_entity(ptd->string, ilk);
	}

	end_display_paint(canv, dc, pxr);
}

/************************************************************************************************/
res_win_t stringctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_string_create;
	ev.pf_on_destroy = hand_string_destroy;

	ev.pf_on_erase = hand_string_erase;
	ev.pf_on_paint = hand_string_paint;

	ev.pf_on_size = hand_string_size;

	ev.pf_on_scroll = hand_string_scroll;

	ev.pf_on_keydown = hand_string_keydown;

	ev.pf_on_mouse_move = hand_string_mouse_move;
	ev.pf_on_mouse_hover = hand_string_mouse_hover;
	ev.pf_on_mouse_leave = hand_string_mouse_leave;

	ev.pf_on_lbutton_down = hand_string_lbutton_down;
	ev.pf_on_lbutton_up = hand_string_lbutton_up;

	ev.pf_on_command_color = hand_string_command_color;
	ev.pf_on_command_switch = hand_string_command_switch;

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void stringctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	ptd->string = ptr;

	stringctrl_redraw(widget, 1);
}

link_t_ptr stringctrl_detach(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	link_t_ptr data;

	XDC_ASSERT(ptd != NULL);

	data = ptd->string;
	ptd->string = NULL;

	stringctrl_redraw(widget, 1);

	return data;
}

link_t_ptr stringctrl_fetch(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	return ptd->string;
}

void stringctrl_enable_delete(res_win_t widget, bool_t bDelete)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	ptd->b_delete = bDelete;
}

void stringctrl_redraw(res_win_t widget, bool_t bCalc)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	b_valid = 0;
	ilk = get_string_next_entity(ptd->string, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->entity)
			b_valid = 1;

		if (bCalc)
			noti_string_owner(widget, NC_STRINGENTITYCALCED, ptd->string, ilk, NULL);

		ilk = get_string_next_entity(ptd->string, ilk);
	}

	if (bCalc)
		noti_string_owner(widget, NC_STRINGCALCED, ptd->string, NULL, NULL);

	if (!b_valid)
	{
		ptd->entity = NULL;
	}

	_stringctrl_reset_page(widget);

	widget_update_window(widget);

	widget_invalid(widget, NULL, 0);
}

void stringctrl_redraw_entity(res_win_t widget, link_t_ptr ent, bool_t bCalc)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	xrect_t xr;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

#ifdef _DEBUG
	XDC_ASSERT(is_string_entity(ptd->string, ent));
#endif

	if (bCalc)
		noti_string_owner(widget, NC_STRINGENTITYCALCED, ptd->string, ent, NULL);

	_stringctrl_entity_rect(widget, ent, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_invalid(widget, &xr, 0);
}

bool_t stringctrl_set_focus_entity(res_win_t widget, link_t_ptr ent)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	bool_t bRe;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return 0;

	if (ent)
	{
#ifdef _DEBUG
		XDC_ASSERT(is_string_entity(ptd->string, ent));
#endif
	}

	bRe = (ent == ptd->entity) ? 1 : 0;

	if (!bRe && ptd->entity)
	{
		if (!noti_string_entity_changing(widget))
			return 0;
	}

	if (!bRe && ent)
	{
		noti_string_entity_changed(widget, ent);

		_stringctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr stringctrl_get_focus_entity(res_win_t widget)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return NULL;

	return ptd->entity;
}

void stringctrl_get_entity_rect(res_win_t widget, link_t_ptr elk, xrect_t* prt)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

#ifdef _DEBUG
	XDC_ASSERT(is_string_entity(ptd->string, elk));
#endif

	_stringctrl_entity_rect(widget, elk, prt);
}

void stringctrl_tabskip(res_win_t widget, int nSkip)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	link_t_ptr plk = NULL;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	switch (nSkip)
	{
	case NF_RIGHT:
	case NF_DOWN:
		if (ptd->entity)
			plk = get_string_next_visible_entity(ptd->string, ptd->entity);
		else
			plk = get_string_next_visible_entity(ptd->string, LINK_FIRST);

		if (plk)
			stringctrl_set_focus_entity(widget, plk);
		break;
	case NF_LEFT:
	case NF_UP:
		if (ptd->entity)
			plk = get_string_prev_visible_entity(ptd->string, ptd->entity);
		else
			plk = get_string_prev_visible_entity(ptd->string, LINK_LAST);

		if (plk)
			stringctrl_set_focus_entity(widget, plk);
		break;
	case NF_HOME:
		plk = get_string_next_visible_entity(ptd->string, LINK_FIRST);

		if (plk)
			stringctrl_set_focus_entity(widget, plk);
		break;
	case NF_END:
		plk = get_string_prev_visible_entity(ptd->string, LINK_LAST);

		if (plk)
			stringctrl_set_focus_entity(widget, plk);
		break;
	}
}

void stringctrl_find(res_win_t widget, link_t_ptr pos, const tchar_t* token)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	link_t_ptr elk;
	int tlen, hlen;
	tchar_t* help;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	tlen = xslen(token);
	help = xsalloc(tlen + 1);

	if (pos == LINK_FIRST)
		elk = get_string_next_visible_entity(ptd->string, LINK_FIRST);
	if (pos == LINK_LAST)
		elk = NULL;
	else
		elk = get_string_next_visible_entity(ptd->string, pos);

	while (elk)
	{
		if (xsnicmp(get_string_entity_key_ptr(elk), token, tlen) == 0)
			break;

		hlen = get_string_entity_val(elk, NULL, MAX_LONG);
		if (hlen)
		{
			help_code(get_string_entity_val_ptr(elk), hlen, help, tlen);
			if (xsnicmp(help, token, tlen) == 0)
				break;
		}

		elk = get_string_next_visible_entity(ptd->string, elk);
	}

	xsfree(help);

	if (elk)
		stringctrl_set_focus_entity(widget, elk);
}

void stringctrl_filter(res_win_t widget, const tchar_t* token)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	link_t_ptr elk;
	int tlen, hlen;
	tchar_t* help;
	bool_t b_redraw, b_hidden;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	stringctrl_set_focus_entity(widget, NULL);

	tlen = xslen(token);
	help = xsalloc(tlen + 1);

	b_redraw = 0;
	elk = get_string_next_entity(ptd->string, LINK_FIRST);
	while (elk)
	{
		b_hidden = 1;

		if (is_null(token))
		{
			b_hidden = 0;
		}
		else
		{
			if (xsnicmp(get_string_entity_key_ptr(elk), token, tlen) == 0)
			{
				b_hidden = 0;
			}

			hlen = get_string_entity_val(elk, NULL, MAX_LONG);
			if (hlen)
			{
				help_code(get_string_entity_val_ptr(elk), hlen, help, tlen);
				if (xsnicmp(help, token, tlen) == 0)
					b_hidden = 0;
			}
		}

		if (b_hidden != get_string_entity_hidden(elk))
		{
			set_string_entity_hidden(elk, b_hidden);
			b_redraw = 1;
		}

		elk = get_string_next_entity(ptd->string, elk);
	}

	xsfree(help);

	if (b_redraw)
		stringctrl_redraw(widget, 1);
}

void stringctrl_popup_size(res_win_t widget, xsize_t* pse)
{
	string_delta_t* ptd = GETSTRINGDELTA(widget);
	int count;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	count = get_string_entity_count(ptd->string);
	if (count > 7)
		count = 7;

	pse->cy = count * ptd->bh;
	pse->cx = _stringctrl_calc_width(widget);

	widget_adjust_size(widget, pse);
}
