/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc static control document

	@module	staticbox.c | implement file

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

typedef struct _staticbox_delta_t{
	tchar_t* text;

	int bw,bh;
}staticbox_delta_t;

#define GETSTATICBOXDELTA(ph) 	(staticbox_delta_t*)widget_get_user_delta(ph)
#define SETSTATICBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*********************************************************************************/
int hand_staticbox_create(res_win_t widget, void* data)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);
	xsize_t xs;

	widget_hand_create(widget);

	ptd = (staticbox_delta_t*)xmem_alloc(sizeof(staticbox_delta_t));
	xmem_zero((void*)ptd, sizeof(staticbox_delta_t));

	SETSTATICBOXDELTA(widget, ptd);

	xs.fx = DEF_TOUCH_SPAN;
	xs.fy = DEF_TOUCH_SPAN;

	widget_size_to_pt(widget, &xs);

	ptd->bw = xs.cx;
	ptd->bh = xs.cy;

	return 0;
}

void hand_staticbox_destroy(res_win_t widget)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETSTATICBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_staticbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);
	
}

void hand_staticbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);
	
	widget_send_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

	widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
}

void hand_staticbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);
	
	widget_erase(widget, NULL);
}

void hand_staticbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);
	res_ctx_t rdc;

	xrect_t xr;
	xfont_t xf;
	xface_t xa;
	xbrush_t xb;

	canvas_t canv;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	widget_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_client_rect(widget, &xr);
	draw_text_raw(rdc, &xf, &xa, &xr, ptd->text, -1);

	end_canvas_paint(canv, dc, pxr);
}

/***************************************************************************************/
res_win_t staticbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_staticbox_create)
		EVENT_ON_DESTROY(hand_staticbox_destroy)

		EVENT_ON_PAINT(hand_staticbox_paint)

		EVENT_ON_SIZE(hand_staticbox_size)

		EVENT_ON_LBUTTON_DOWN(hand_staticbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_staticbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void staticbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	pxs->cx = ptd->bw * 3;
	pxs->cy = ptd->bh * 2;
	
	widget_adjust_size(widget_get_style(widget), pxs);
}

void staticbox_set_text(res_win_t widget, const tchar_t* text)
{
	staticbox_delta_t* ptd = GETSTATICBOXDELTA(widget);
	int len;

	XDL_ASSERT(ptd != NULL);

	xsfree(ptd->text);
	
	len = xslen(text);

	ptd->text = xsalloc(len + 1);
	xsncpy(ptd->text, text, len);

	widget_erase(widget, NULL);
}
