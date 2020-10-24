﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc spin control document

	@module	spinbox.c | implement file

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

typedef struct _spinbox_delta_t{
	int cur;
}spinbox_delta_t;

#define GETSPINBOXDELTA(ph) 	(spinbox_delta_t*)widget_get_user_delta(ph)
#define SETSPINBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*********************************************************************************/
void noti_spinbox_command(res_win_t widget, int code, var_long data)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

void spinbox_on_plus(res_win_t widget)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	ptd->cur++;

	widget_erase(widget, NULL);

	noti_spinbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void spinbox_on_minus(res_win_t widget)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	ptd->cur--;

	widget_erase(widget, NULL);

	noti_spinbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

/*********************************************************************************/
int hand_spinbox_create(res_win_t widget, void* data)
{
	spinbox_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (spinbox_delta_t*)xmem_alloc(sizeof(spinbox_delta_t));
	xmem_zero((void*)ptd, sizeof(spinbox_delta_t));

	SETSPINBOXDELTA(widget, ptd);

	return 0;
}

void hand_spinbox_destroy(res_win_t widget)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETSPINBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_spinbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	
}

void hand_spinbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };
	xpoint_t pt;
	int hint;

	pt.x = pxp->x;
	pt.y = pxp->y;

	widget_point_to_tm(widget, &pt);

	widget_get_xfont(widget, &xf);

	get_canvas_measure(widget_get_canvas(widget), &im);

	hint = calc_spinbox_hint(&im, &xf, &pt);

	if (hint == SPINBOX_HINT_PLUS)
		spinbox_on_plus(widget);
	else if (hint == SPINBOX_HINT_MINUS)
		spinbox_on_minus(widget);
}

void hand_spinbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	
	widget_erase(widget, NULL);
}

void hand_spinbox_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	visual_t rdc;
	xrect_t xr;
	canvas_t canv;
	const if_drawing_t* pif = NULL;
	if_drawing_t ifv = {0};

	xfont_t xf;
	xbrush_t xb;
	xpen_t xp;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = widget_get_canvas_interface(widget);
	

	
	
	
	
	

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	draw_spinbox(pif, &xf, ptd->cur);

	

	end_canvas_paint(canv, dc, pxr);
	
}

/***************************************************************************************/
res_win_t spinbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_spinbox_create)
		EVENT_ON_DESTROY(hand_spinbox_destroy)

		EVENT_ON_PAINT(hand_spinbox_paint)

		EVENT_ON_SIZE(hand_spinbox_size)

		EVENT_ON_LBUTTON_DOWN(hand_spinbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_spinbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void spinbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	get_canvas_measure(widget_get_canvas(widget), &im);

	calc_spinbox_size(&im, &xf, pxs);

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void spinbox_set_spin(res_win_t widget, int cur)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->cur = cur;

	widget_erase(widget, NULL);
}

int spinbox_get_spin(res_win_t widget)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->cur;
}
