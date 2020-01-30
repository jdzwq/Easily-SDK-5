/***********************************************************************
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
#include "handler.h"
#include "widgetnc.h"
#include "widgetex.h"

typedef struct _spinbox_delta_t{
	int cur;
}spinbox_delta_t;

#define GETSPINBOXDELTA(ph) 	(spinbox_delta_t*)widget_get_user_delta(ph)
#define SETSPINBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_int)ptd)

/*********************************************************************************/
void noti_spinbox_command(res_win_t widget, int code, var_int data)
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

	widget_redraw(widget, NULL, 0);

	noti_spinbox_command(widget, COMMAND_UPDATE, (var_int)NULL);
}

void spinbox_on_minus(res_win_t widget)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	ptd->cur--;

	widget_redraw(widget, NULL, 0);

	noti_spinbox_command(widget, COMMAND_UPDATE, (var_int)NULL);
}

/*********************************************************************************/
int hand_spinbox_create(res_win_t widget, void* data)
{
	spinbox_delta_t* ptd;

	widgetex_hand_create(widget);

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

	widgetex_hand_destroy(widget);
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

	widgetex_point_to_tm(widget, &pt);

	widgetex_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	hint = calc_spinbox_hint(&im, &xf, &pt);

	if (hint == SPINBOX_HINT_PLUS)
		spinbox_on_plus(widget);
	else if (hint == SPINBOX_HINT_MINUS)
		spinbox_on_minus(widget);
}

void hand_spinbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	
	widget_redraw(widget, NULL, 0);
}

void hand_spinbox_erase(res_win_t widget, res_ctx_t rdc)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	
}

void hand_spinbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);
	res_ctx_t rdc;
	xrect_t xr;
	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	xfont_t xf;
	xbrush_t xb;
	xpen_t xp;

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

	draw_spinbox(pif, &cb, &xf, ptd->cur);

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t spinbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_spinbox_create)
		EVENT_ON_DESTROY(hand_spinbox_destroy)

		EVENT_ON_ERASE(hand_spinbox_erase)
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

	widgetex_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_spinbox_size(&im, &xf, pxs);

	widgetex_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void spinbox_set_spin(res_win_t widget, int cur)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->cur = cur;

	widget_redraw(widget, NULL, 1);
}

int spinbox_get_spin(res_win_t widget)
{
	spinbox_delta_t* ptd = GETSPINBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->cur;
}
