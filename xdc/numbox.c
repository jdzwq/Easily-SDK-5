/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc number control document

	@module	numbox.c | implement file

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

typedef struct _numbox_delta_t{
	int index;
	int bw, bh;
}numbox_delta_t;

#define NUMBOX_COUNT	13
#define NUMBOX_ROWS		4
#define NUMBOX_COLS		4

static tchar_t NUMBOX_DATA[] = _T("1234567890.-\n");

typedef struct _NUMBOX_RECT{
	int x, y, w, h;
}NUMBOX_RECT;

static NUMBOX_RECT NUMBOX_POS[NUMBOX_COUNT] = {
	{ 0, 0, 1, 1 },
	{ 1, 0, 1, 1 },
	{ 2, 0, 1, 1 },
	{ 0, 1, 1, 1 },
	{ 1, 1, 1, 1 },
	{ 2, 1, 1, 1 },
	{ 0, 2, 1, 1 },
	{ 1, 2, 1, 1 },
	{ 2, 2, 1, 1 },
	{ 0, 3, 2, 1 },
	{ 2, 3, 1, 1 },
	{ 3, 0, 1, 1 },
	{ 3, 1, 1, 3 }
};

#define GETNUMBOXDELTA(ph) 	(numbox_delta_t*)widget_get_user_delta(ph)
#define SETNUMBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)


int hand_numbox_create(res_win_t widget, void* data)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);
	xsize_t xs;

	widget_hand_create(widget);

	ptd = (numbox_delta_t*)xmem_alloc(sizeof(numbox_delta_t));
	xmem_zero((void*)ptd, sizeof(numbox_delta_t));

	xs.fx = DEF_TOUCH_SPAN;
	xs.fy = DEF_TOUCH_SPAN;

	widget_size_to_pt(widget, &xs);

	ptd->bw = xs.cx;
	ptd->bh = xs.cy;

	ptd->index = -1;

	SETNUMBOXDELTA(widget, ptd);

	return 0;
}

void hand_numbox_destroy(res_win_t widget)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETNUMBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_numbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);
	
	xrect_t xr;
	int i;

	widget_set_capture(widget, 1);

	for (i = 0; i < NUMBOX_COUNT; i++)
	{
		xr.x = ptd->bw * NUMBOX_POS[i].x;
		xr.w = ptd->bw * NUMBOX_POS[i].w;
		xr.y = ptd->bh * NUMBOX_POS[i].y;
		xr.h = ptd->bh * NUMBOX_POS[i].h;

		if (pt_in_rect(pxp, &xr))
		{
			ptd->index = i;
			break;
		}
	}

	widget_erase(widget, NULL);
}

void hand_numbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);
	xrect_t xr;
	int i;
	tchar_t ch = 0;

	widget_set_capture(widget, 0);

	for (i = 0; i < NUMBOX_COUNT; i++)
	{
		xr.x = ptd->bw * NUMBOX_POS[i].x;
		xr.w = ptd->bw * NUMBOX_POS[i].w;
		xr.y = ptd->bh * NUMBOX_POS[i].y;
		xr.h = ptd->bh * NUMBOX_POS[i].h;

		if (ptd->index == i)
		{
			ch = NUMBOX_DATA[i];
			break;
		}
	}

	ptd->index = -1;

	widget_erase(widget, &xr);

	if (!ch)
		return;

	if (ch == _T('-'))
		widget_post_key(NULL, KEY_BACK);
	else if (ch == _T('\n'))
		widget_post_key(NULL, KEY_ENTER);
	else
		widget_post_char(NULL, ch);
}

void hand_numbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	widget_erase(widget, NULL);
}

void hand_numbox_erase(res_win_t widget, res_ctx_t dc)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);
	
}

void hand_numbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);
	canvas_t canv;
	res_ctx_t rdc;
	xfont_t xf;
	xface_t xa;
	xbrush_t xb, xb_bark, xb_focus;
	xrect_t xr, xr_focus;
	int i;
	tchar_t tk[2] = { 0 };

	widget_get_xbrush(widget, &xb);
	xmem_copy((void*)&xb_focus, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_focus, DEF_SOFT_LIGHTEN);

	xmem_copy((void*)&xb_bark, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bark, DEF_SOFT_DARKEN);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	canv = widget_get_canvas(widget);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb_bark, &xr);

	for (i = 0; i < NUMBOX_COUNT; i++)
	{
		xr.x = ptd->bw * NUMBOX_POS[i].x;
		xr.w = ptd->bw * NUMBOX_POS[i].w;
		xr.y = ptd->bh * NUMBOX_POS[i].y;
		xr.h = ptd->bh * NUMBOX_POS[i].h;

		xr_focus.x = xr.x + 2;
		xr_focus.y = xr.y + 2;
		xr_focus.w = xr.w - 4;
		xr_focus.h = xr.h - 4;

		if (ptd->index == i)
			draw_rect_raw(rdc, NULL, &xb_focus, &xr_focus);
		else
			draw_rect_raw(rdc, NULL, &xb, &xr_focus);

		tk[0] = NUMBOX_DATA[i];

		if (tk[0] == _T('\n'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("√"), -1);
		else if (tk[0] == _T('-'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("CE"), -1);
		else
			draw_text_raw(rdc, &xf, &xa, &xr, tk, -1);
	}

	end_canvas_paint(canv, dc, pxr);
}

/***************************************************************************************/
res_win_t numbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_numbox_create)
		EVENT_ON_DESTROY(hand_numbox_destroy)

		EVENT_ON_ERASE(hand_numbox_erase)
		EVENT_ON_PAINT(hand_numbox_paint)

		EVENT_ON_SIZE(hand_numbox_size)

		EVENT_ON_LBUTTON_DOWN(hand_numbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_numbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void numbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	numbox_delta_t* ptd = GETNUMBOXDELTA(widget);

	pxs->cx = ptd->bw * NUMBOX_COLS;
	pxs->cy = ptd->bh * NUMBOX_ROWS;

	widget_adjust_size(widget_get_style(widget), pxs);
}
