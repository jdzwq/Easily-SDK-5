/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc navigation control document

	@module	navibox.c | navigation control widnows implement file

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

#include "xdcbox.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcres.h"
#include "xdcgdi.h"
#include "xdccanv.h"

typedef enum{
	_HINT_NAVI_NONE = 0,
	_HINT_NAVI_LEFT = 1,
	_HINT_NAVI_UP = 2,
	_HINT_NAVI_DOWN = 3,
	_HINT_NAVI_RIGHT = 4,
	_HINT_NAVI_CENTER = 5
}_HINT_NAVI;

typedef struct _navibox_delta_t{
	long orgx, orgy;
	int hint;
	long bw, bh;
	bool_t b_drag;
}navibox_delta_t;

#define GETNAVIBOXDELTA(ph) 	(navibox_delta_t*)widget_get_user_delta(ph)
#define SETNAVIBOXDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

#define NAVIBOX_BTN		(long)8

int _navibox_calc_hint(res_win_t widget, long x, long y)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	xrect_t xr;
	xpoint_t xp;

	xp.x = x;
	xp.y = y;

	widget_get_client_rect(widget, &xr);

	xr.x += (xr.w - ptd->bw) / 2;
	xr.y += (xr.h - ptd->bh) / 2;
	xr.w = ptd->bw;
	xr.h = ptd->bh;
	if (pt_in_rect(&xp, &xr))
	{
		return _HINT_NAVI_CENTER;
	}

	widget_get_client_rect(widget, &xr);
	xr.w = ptd->bw;
	xr.y += (xr.h - ptd->bh) / 2;
	xr.h = ptd->bh;
	if(pt_in_rect(&xp,&xr))
	{
		return _HINT_NAVI_LEFT;
	}

	widget_get_client_rect(widget, &xr);
	xr.x = xr.w - ptd->bw;
	xr.w = ptd->bw;
	xr.y += (xr.h - ptd->bh) / 2;
	xr.h = ptd->bh;
	if (pt_in_rect(&xp, &xr))
	{
		return _HINT_NAVI_RIGHT;
	}

	widget_get_client_rect(widget, &xr);
	xr.x += (xr.w - ptd->bw) / 2;
	xr.w = ptd->bw;
	xr.h = ptd->bh;
	if (pt_in_rect(&xp, &xr))
	{
		return _HINT_NAVI_UP;
	}

	widget_get_client_rect(widget, &xr);
	xr.x += (xr.w - ptd->bw) / 2;
	xr.w = ptd->bw;
	xr.y = xr.h - ptd->bh;
	xr.h = ptd->bh;
	if (pt_in_rect(&xp, &xr))
	{
		return _HINT_NAVI_DOWN;
	}

	return _HINT_NAVI_NONE;
}

int hand_navibox_create(res_win_t widget, void* data)
{
	navibox_delta_t* ptd;
	res_dc_t rdc;
	xsize_t xs;
	XHANDLE canv;

	ptd = (navibox_delta_t*)xmem_alloc(sizeof(navibox_delta_t));
	xmem_zero((void*)ptd, sizeof(navibox_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_attach_canvas(widget, canv);

	xs.fx = DEF_TOUCH_SPAN;
	xs.fy = DEF_TOUCH_SPAN;

	screen_size_to_pt(&xs);

	ptd->bw = xs.cx;
	ptd->bh = xs.cy;

	SETNAVIBOXDELTA(widget, ptd);

	return 0;
}

void hand_navibox_destroy(res_win_t widget)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	destroy_display_canvas(widget_detach_canvas(widget));

	SETNAVIBOXDELTA(widget, 0);

	xmem_free(ptd);
}

void hand_navibox_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	int hint;

	if (ptd->b_drag)
	{
		widget_set_cursor(widget, CURS_HAND);
		return;
	}

	hint = _navibox_calc_hint(widget, pxp->x, pxp->y);

	if (hint == _HINT_NAVI_CENTER)
		widget_set_cursor(widget, CURS_HAND);
	else
		widget_set_cursor(widget, CURS_ARROW);
}

void hand_navibox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	
	ptd->hint = _navibox_calc_hint(widget, pxp->x, pxp->y);

	if (ptd->hint == _HINT_NAVI_CENTER)
	{
		ptd->b_drag = 1;

		ptd->orgx = pxp->x;
		ptd->orgy = pxp->y;

		widget_set_capture(widget, 1);
	}

	widget_invalid(widget, NULL, 1);
}

void hand_navibox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	xrect_t rt;
	long cx, cy;
	int hint;

	if (ptd->b_drag)
	{
		widget_set_capture(widget, 0);

		cx = pxp->x - ptd->orgx;
		cy = pxp->y - ptd->orgy;

		widget_get_window_rect(widget, &rt);

		rt.x += cx;
		rt.y += cy;
		
		widget_move(widget, RECTPOINT(&rt));

		ptd->b_drag = 0;
		ptd->hint = _HINT_NAVI_NONE;
	}
	else
	{
		hint = _navibox_calc_hint(widget, pxp->x, pxp->y);
		switch (hint)
		{
		case _HINT_NAVI_CENTER:
			//widget_post_key(NULL, KEY_ENTER);
			break;
		case _HINT_NAVI_LEFT:
			widget_post_key(NULL, KEY_PAGEUP);
			break;
		case _HINT_NAVI_RIGHT:
			widget_post_key(NULL, KEY_DOWN);
			break;
		case _HINT_NAVI_UP:
			widget_post_key(NULL, KEY_HOME);
			break;
		case _HINT_NAVI_DOWN:
			widget_post_key(NULL, KEY_END);
			break;
		}
		ptd->hint = _HINT_NAVI_NONE;
		widget_invalid(widget, NULL, 1);
	}
}

void hand_navibox_command_color(res_win_t widget, void* data)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	widget_set_color_mode(widget, (clr_mod_t*)data);

	widget_update_window(widget);
	widget_update_client(widget);
}

void hand_navibox_size(res_win_t widget, int code, const xsize_t* prs)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	widget_invalid(widget, NULL, 0);
}

void hand_navibox_erase(res_win_t widget, res_dc_t dc)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
}

void hand_navibox_paint(res_win_t widget, res_dc_t rdc, const xrect_t* pxr)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xrect_t xr;

	widget_get_xbrush(widget, &xb);

	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	draw_rect_raw(rdc, NULL, &xb, &xr);
	
	widget_get_client_rect(widget, &xr);
	xr.x += (xr.w - ptd->bw) / 2;
	xr.y += (xr.h - ptd->bh) / 2;
	xr.w = ptd->bw;
	xr.h = ptd->bh;
	pt_center_rect(&xr, NAVIBOX_BTN, NAVIBOX_BTN);
	//if (ptd->hint == _HINT_NAVI_CENTER)
		//draw_ellipse_raw(rdc, &xp, &xb, &xr);
	//else
		draw_ellipse_raw(rdc, &xp, NULL, &xr);

	widget_get_client_rect(widget, &xr);
	xr.w = ptd->bw;
	xr.y += (xr.h - ptd->bh) / 2;
	xr.h = ptd->bh;
	pt_center_rect(&xr, NAVIBOX_BTN, NAVIBOX_BTN);
	//if (ptd->hint == _HINT_NAVI_LEFT)
		//draw_shape_raw(rdc, &xp, &xb, &xr, ATTR_SHAPE_LEFTTRIANGLE);
	//else
		draw_shape_raw(rdc, &xp, NULL, &xr, ATTR_SHAPE_LEFTTRIANGLE);

	widget_get_client_rect(widget, &xr);
	xr.x = xr.w - ptd->bw;
	xr.w = ptd->bw;
	xr.y += (xr.h - ptd->bh) / 2;
	xr.h = ptd->bh;
	pt_center_rect(&xr, NAVIBOX_BTN, NAVIBOX_BTN);
	//if (ptd->hint == _HINT_NAVI_RIGHT)
		//draw_shape_raw(rdc, &xp, &xb, &xr, ATTR_SHAPE_RIGHTTRIANGLE);
	//else
		draw_shape_raw(rdc, &xp, NULL, &xr, ATTR_SHAPE_RIGHTTRIANGLE);

	widget_get_client_rect(widget, &xr);
	xr.x += (xr.w - ptd->bw) / 2;
	xr.w = ptd->bw;
	xr.h = ptd->bh;
	pt_center_rect(&xr, NAVIBOX_BTN, NAVIBOX_BTN);
	//if (ptd->hint == _HINT_NAVI_UP)
		//draw_shape_raw(rdc, &xp, &xb, &xr, ATTR_SHAPE_TOPTRIANGLE);
	//else
		draw_shape_raw(rdc, &xp, NULL, &xr, ATTR_SHAPE_TOPTRIANGLE);

	widget_get_client_rect(widget, &xr);
	xr.x += (xr.w - ptd->bw) / 2;
	xr.w = ptd->bw;
	xr.y = xr.h - ptd->bh;
	xr.h = ptd->bh;
	pt_center_rect(&xr, NAVIBOX_BTN, NAVIBOX_BTN);
	//if (ptd->hint == _HINT_NAVI_DOWN)
		//draw_shape_raw(rdc, &xp, &xb, &xr, ATTR_SHAPE_BOTTOMTRIANGLE);
	//else
		draw_shape_raw(rdc, &xp, NULL, &xr, ATTR_SHAPE_BOTTOMTRIANGLE);
}

/**************************************************************************************/
res_win_t navibox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_navibox_create;
	ev.pf_on_destroy = hand_navibox_destroy;

	ev.pf_on_erase = hand_navibox_erase;
	ev.pf_on_paint = hand_navibox_paint;

	ev.pf_on_size = hand_navibox_size;

	ev.pf_on_mouse_move = hand_navibox_mouse_move;
	ev.pf_on_lbutton_down = hand_navibox_lbutton_down;
	ev.pf_on_lbutton_up = hand_navibox_lbutton_up;

	ev.pf_on_command_color = hand_navibox_command_color;

	return widget_create(NULL, style, pxr, widget, &ev);
}

void navibox_popup_size(res_win_t widget, xsize_t* pxs)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	pxs->cx = ptd->bw * 3;
	pxs->cy = ptd->bh * 3;

	widget_adjust_size(widget_get_style(widget), pxs);
}