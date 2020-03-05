/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc shape control document

	@module	shapebox.c | implement file

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
#include "xdcimp.h"

typedef struct _shapebox_delta_t{
	tchar_t shape[INT_LEN];
}shapebox_delta_t;

#define GETSHAPEBOXDELTA(ph) 	(shapebox_delta_t*)widget_get_user_delta(ph)
#define SETSHAPEBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*********************************************************************************/
int hand_shapebox_create(res_win_t widget, void* data)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);

	widget_hand_create(widget);

	ptd = (shapebox_delta_t*)xmem_alloc(sizeof(shapebox_delta_t));
	xmem_zero((void*)ptd, sizeof(shapebox_delta_t));

	SETSHAPEBOXDELTA(widget, ptd);

	return 0;
}

void hand_shapebox_destroy(res_win_t widget)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETSHAPEBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_shapebox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);
	
}

void hand_shapebox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);
	
}

void hand_shapebox_size(res_win_t widget, int code, const xsize_t* prs)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);
	
	widget_erase(widget, NULL);
}

void hand_shapebox_erase(res_win_t widget, res_ctx_t rdc)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);
	
}

void hand_shapebox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);
	res_ctx_t rdc;

	xrect_t xr;
	xpen_t xp;
	xbrush_t xb;

	canvas_t canv;

	widget_get_xpen(widget, &xp);
	widget_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_client_rect(widget, &xr);
	xr.w -= 1;
	xr.h -= 1;
	draw_shape_raw(rdc, &xp, &xb, &xr, ptd->shape);

	end_canvas_paint(canv, dc, pxr);
}

/***************************************************************************************/
res_win_t shapebox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_shapebox_create)
		EVENT_ON_DESTROY(hand_shapebox_destroy)

		EVENT_ON_ERASE(hand_shapebox_erase)
		EVENT_ON_PAINT(hand_shapebox_paint)

		EVENT_ON_SIZE(hand_shapebox_size)

		EVENT_ON_LBUTTON_DOWN(hand_shapebox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_shapebox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void shapebox_set_shape(res_win_t widget, const tchar_t* shape)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xsncpy(ptd->shape, shape, INT_LEN);

	widget_erase(widget, NULL);
}

int shapebox_get_shape(res_win_t widget, tchar_t* buf)
{
	shapebox_delta_t* ptd = GETSHAPEBOXDELTA(widget);
	int len;

	XDL_ASSERT(ptd != NULL);

	len = xslen(ptd->shape);

	if (buf)
		xscpy(buf, ptd->shape);

	return len;
}
