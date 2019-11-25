﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc user control documilk

	@module	userctrl.c | implement file

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
#include "winnc.h"
#include "xdcbox.h"

typedef struct _user_delta_t{
	var_long var;

	res_win_t hsc;
	res_win_t vsc;
}user_delta_t;

#define GETUSERDELTA(ph) 	(user_delta_t*)widget_get_user_delta(ph)
#define SETUSERDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/***************************************************************************************/

static int noti_user_owner(res_win_t widget, unsigned long code, void* data)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	NOTICE_USER nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

static void _userctrl_reset_page(res_win_t widget)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	xrect_t xr;
	PAGE_CALC pc = { 0 };

	noti_user_owner(widget, NC_USERCALC, (void*)&pc);

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, pc.total_width, pc.total_height, pc.line_width, pc.line_height);
}

/********************************************************************************************/
int hand_user_create(res_win_t widget, void* data)
{
	user_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (user_delta_t*)xmem_alloc(sizeof(user_delta_t));

	SETUSERDELTA(widget, ptd);

	return 0;
}

void hand_user_destroy(res_win_t widget)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	SETUSERDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_user_keydown(res_win_t widget, int key)
{
	user_delta_t* ptd = GETUSERDELTA(widget);
	int ch;

	XDL_ASSERT(ptd != NULL);

	ch = key;

	noti_user_owner(widget, NC_USERKEY, (void*)&ch);
}

void hand_user_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_user_owner(widget, NC_USERMOVE, (void*)pxp);
}

void hand_user_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_user_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_user_owner(widget, NC_USERLBCLK, (void*)pxp);
}

void hand_user_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_user_owner(widget, NC_USERDBCLK, (void*)pxp);
}

void hand_user_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);
}

void hand_user_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_user_owner(widget, NC_USERRBCLK, (void*)pxp);
}

void hand_user_size(res_win_t widget, int code, const xsize_t* prs)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	userctrl_redraw(widget);
}

void hand_user_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_user_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	XDL_ASSERT(ptd != NULL);

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widget_hand_scroll(widget, bHorz, nLine))
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

void hand_user_erase(res_win_t widget, res_ctx_t rdc)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);
}

void hand_user_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	res_ctx_t rdc;
	canvas_t canv;
	xrect_t xr = { 0 };
	viewbox_t vb = { 0 };
	xbrush_t xb = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xbrush(widget, &xb);
	
	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_view_rect(widget, &vb);

	noti_user_owner(widget, NC_USERDRAW, (void*)rdc);

	end_canvas_paint(canv, dc, pxr);
}

/************************************************************************************************/
res_win_t userctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_user_create)
		EVENT_ON_DESTROY(hand_user_destroy)

		EVENT_ON_ERASE(hand_user_erase)
		EVENT_ON_PAINT(hand_user_paint)

		EVENT_ON_SIZE(hand_user_size)

		EVENT_ON_SCROLL(hand_user_scroll)
		EVENT_ON_WHEEL(hand_user_wheel)

		EVENT_ON_KEYDOWN(hand_user_keydown)

		EVENT_ON_MOUSE_MOVE(hand_user_mouse_move)

		EVENT_ON_LBUTTON_DOWN(hand_user_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_user_lbutton_up)
		EVENT_ON_LBUTTON_DBCLICK(hand_user_lbutton_dbclick)
		EVENT_ON_RBUTTON_DOWN(hand_user_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_user_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void userctrl_redraw(res_win_t widget)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	_userctrl_reset_page(widget);

	widget_update(widget);

	widget_redraw(widget, NULL, 0);
}

void userctrl_set_delta(res_win_t widget, var_long var)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->var = var;
}

var_long userctrl_get_delta(res_win_t widget)
{
	user_delta_t* ptd = GETUSERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->var;
}