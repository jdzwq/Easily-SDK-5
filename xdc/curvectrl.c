/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc curve control documilk

	@module	curvectrl.c | implement file

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
#include "xdcimp.h"
#include "xdcbox.h"

typedef struct _curve_delta_t{
	vector_t vt;

	res_win_t hsc;
	res_win_t vsc;
}curve_delta_t;

#define GETCURVEDELTA(ph) 	(curve_delta_t*)widget_get_user_delta(ph)
#define SETCURVEDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/***************************************************************************************/

static int noti_curve_curve(res_win_t widget, unsigned int code, void* data)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	NOTICE_CURVE nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

static void _curvectrl_reset_page(res_win_t widget)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	xrect_t xr;
	PAGEINFO pc = { 0 };

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, xr.w, xr.h, 10, 10);
}

/********************************************************************************************/
int hand_curve_create(res_win_t widget, void* data)
{
	curve_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (curve_delta_t*)xmem_alloc(sizeof(curve_delta_t));

	SETCURVEDELTA(widget, ptd);

	return 0;
}

void hand_curve_destroy(res_win_t widget)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETCURVEDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_curve_keydown(res_win_t widget, dword_t ks, int key)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);
	int ch;

	XDL_ASSERT(ptd != NULL);

	ch = key;
}

void hand_curve_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);
}

void hand_curve_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_curve_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_curve_curve(widget, NC_CURVELBCLK, (void*)pxp);
}

void hand_curve_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_curve_curve(widget, NC_CURVEDBCLK, (void*)pxp);
}

void hand_curve_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);
}

void hand_curve_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_curve_curve(widget, NC_CURVERBCLK, (void*)pxp);
}

void hand_curve_size(res_win_t widget, int code, const xsize_t* prs)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	curvectrl_redraw(widget);
}

void hand_curve_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_curve_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;

	XDL_ASSERT(ptd != NULL);

	widget_get_scroll_info(widget, bHorz, &scr);

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

void hand_curve_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	visual_t rdc;
	canvas_t canv;
	xrect_t xr = { 0 };
	viewbox_t vb = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };

	xpoint_t pt[5];
	int i;
	int w;

	if_visual_t* piv;

	XDL_ASSERT(ptd != NULL);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	piv = create_visual_interface(rdc);

	(*piv->pf_draw_rect_raw)(piv->visual, NULL, &xb, &xr);

	w = xr.w / 5;
	for (i = 0; i < 10; i++)
	{
		xr.x = i * w;
		xr.w = w;

		pt[0].x = xr.x;
		pt[0].y = xr.y + xr.h / 2;
		pt[1].x = xr.x + xr.w / 4;
		pt[1].y = xr.y + xr.h / 4;
		pt[2].x = xr.x + xr.w / 2;
		pt[2].y = xr.y + xr.h / 2;
		pt[3].x = xr.x + xr.w / 4 * 3;
		pt[3].y = xr.y + xr.h / 4 * 3;
		pt[4].x = xr.x + xr.w;
		pt[4].y = xr.y + xr.h / 2;

		(*piv->pf_draw_curve_raw)(piv->visual, &xp, pt, 5);
	}

	destroy_visual_interface(piv);

	end_canvas_paint(canv, dc, pxr);
}

/************************************************************************************************/
res_win_t curvectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_curve_create)
		EVENT_ON_DESTROY(hand_curve_destroy)

		EVENT_ON_PAINT(hand_curve_paint)

		EVENT_ON_SIZE(hand_curve_size)

		EVENT_ON_SCROLL(hand_curve_scroll)
		EVENT_ON_WHEEL(hand_curve_wheel)

		EVENT_ON_KEYDOWN(hand_curve_keydown)

		EVENT_ON_MOUSE_MOVE(hand_curve_mouse_move)

		EVENT_ON_LBUTTON_DOWN(hand_curve_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_curve_lbutton_up)
		EVENT_ON_LBUTTON_DBCLICK(hand_curve_lbutton_dbclick)
		EVENT_ON_RBUTTON_DOWN(hand_curve_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_curve_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void curvectrl_redraw(res_win_t widget)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	_curvectrl_reset_page(widget);

	widget_update(widget);

	widget_erase(widget, NULL);
}

void curvectrl_set_vector(res_win_t widget, vector_t vt)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

}

void curvectrl_get_vector(res_win_t widget, vector_t* pvt)
{
	curve_delta_t* ptd = GETCURVEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

}