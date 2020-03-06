/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc check control document

	@module	checkbox.c | implement file

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

typedef struct _checkbox_delta_t{
	bool_t on;
}checkbox_delta_t;

#define GETCHECKBOXDELTA(ph) 	(checkbox_delta_t*)widget_get_user_delta(ph)
#define SETCHECKBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/********************************************************************************/
void _checkbox_reset_page(res_win_t widget)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, xr.w, xr.h, 0, 0);

	widget_reset_scroll(widget, 0);
}
////////////////////////////////////////////////////////////////////////////////////////
void noti_checkbox_command(res_win_t widget, int code, var_long data)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

void checkbox_on_switch(res_win_t widget)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	if (ptd->on)
		ptd->on = 0;
	else
		ptd->on = 1;

	widget_erase(widget, NULL);

	noti_checkbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

/*********************************************************************************/
int hand_checkbox_create(res_win_t widget, void* data)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	widget_hand_create(widget);

	ptd = (checkbox_delta_t*)xmem_alloc(sizeof(checkbox_delta_t));
	xmem_zero((void*)ptd, sizeof(checkbox_delta_t));

	SETCHECKBOXDELTA(widget, ptd);

	return 0;
}

void hand_checkbox_destroy(res_win_t widget)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETCHECKBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_checkbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);
	
}

void hand_checkbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	checkbox_on_switch(widget);
}

void hand_checkbox_keydown(res_win_t widget, dword_t ks, int key)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	switch (key)
	{
	case KEY_SPACE:
		checkbox_on_switch(widget);
		break;
	}
}

void hand_checkbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);
	
	_checkbox_reset_page(widget);

	widget_erase(widget, NULL);
}

void hand_checkbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);
	res_ctx_t rdc;
	xrect_t xr;
	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	xfont_t xf;
	xbrush_t xb;
	xpen_t xp;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widget_get_mask(widget, &pif->clr_msk);
	widget_get_iconic(widget, &pif->clr_ico);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_checkbox(pif, &cb, &xf, ptd->on);

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t checkbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_checkbox_create)
		EVENT_ON_DESTROY(hand_checkbox_destroy)

		EVENT_ON_PAINT(hand_checkbox_paint)

		EVENT_ON_SIZE(hand_checkbox_size)

		EVENT_ON_KEYDOWN(hand_checkbox_keydown)

		EVENT_ON_LBUTTON_DOWN(hand_checkbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_checkbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void checkbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_checkbox_size(&im, &xf, pxs);

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void checkbox_set_state(res_win_t widget, bool_t cur)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->on = cur;

	widget_erase(widget, NULL);
}

bool_t checkbox_get_state(res_win_t widget)
{
	checkbox_delta_t* ptd = GETCHECKBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->on;
}
