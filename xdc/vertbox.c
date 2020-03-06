/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc vert control document

	@module	vertbox.c | implement file

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

typedef struct _vertbox_delta_t{
	res_win_t target;
}vertbox_delta_t;

#define GETVERTBOXDELTA(ph) 	(vertbox_delta_t*)widget_get_user_delta(ph)
#define SETVERTBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)


/*********************************************************************************/
int hand_vertbox_create(res_win_t widget, void* data)
{
	vertbox_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (vertbox_delta_t*)xmem_alloc(sizeof(vertbox_delta_t));
	xmem_zero((void*)ptd, sizeof(vertbox_delta_t));

	SETVERTBOXDELTA(widget, ptd);

	return 0;
}

void hand_vertbox_destroy(res_win_t widget)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETVERTBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_vertbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);
	
	widget_kill_timer(widget, 0);
}

void hand_vertbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };
	xpoint_t pt;
	xrect_t xr;
	int hint;
	scroll_t sc;
	dword_t dw;

	if (!widget_is_valid(ptd->target))
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;

	widget_point_to_tm(widget, &pt);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	hint = calc_vertbox_hint(&im, &xf, &pt);

	widget_get_scroll_info(ptd->target,0, &sc);
	widget_get_client_rect(ptd->target, &xr);
	dw = widget_get_style(ptd->target);

	if (hint == VERTBOX_HINT_PAGEUP)
	{
		if (dw & WD_STYLE_PAGING)
			widget_post_key(ptd->target, VK_HOME);
		else
			widget_scroll(ptd->target, 0, -sc.max);
	}
	else if (hint == VERTBOX_HINT_LINEUP)
	{
		if (dw & WD_STYLE_PAGING)
			widget_post_key(ptd->target, KEY_PAGEUP);
		else
			widget_scroll(ptd->target, 0, -xr.h);
	}
	else if (hint == VERTBOX_HINT_LINEDOWN)
	{
		if (dw & WD_STYLE_PAGING)
			widget_post_key(ptd->target, KEY_PAGEDOWN);
		else
			widget_scroll(ptd->target, 0, xr.h);
	}
	else if (hint == VERTBOX_HINT_PAGEDOWN)
	{
		if (dw & WD_STYLE_PAGING)
			widget_post_key(ptd->target, KEY_END);
		else
			widget_scroll(ptd->target, 0, sc.max);
	}

	widget_set_timer(widget, DEF_TIPTIME);
}

void hand_vertbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);
	
	widget_erase(widget, NULL);
}

void hand_vertbox_timer(res_win_t widget, var_long tid)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);

	xpoint_t pt;
	xrect_t xr;

	message_position(&pt);

	widget_get_window_rect(widget, &xr);

	if (!pt_in_rect(&pt, &xr))
	{
		widget_kill_timer(widget, tid);

		widget_close(widget, 0);
	}
}

void hand_vertbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);
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

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_vertbox(pif, &cb, &xf);

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t vertbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_vertbox_create)
		EVENT_ON_DESTROY(hand_vertbox_destroy)

		EVENT_ON_PAINT(hand_vertbox_paint)

		EVENT_ON_SIZE(hand_vertbox_size)

		EVENT_ON_LBUTTON_DOWN(hand_vertbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_vertbox_lbutton_up)

		EVENT_ON_TIMER(hand_vertbox_timer)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void vertbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_vertbox_size(&im, &xf, pxs);

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void vertbox_set_target(res_win_t widget, res_win_t target)
{
	vertbox_delta_t* ptd = GETVERTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->target = target;
}

res_win_t show_vertbox(res_win_t owner)
{
	res_win_t wt;
	xrect_t xr = { 0 };
	xsize_t xs = { 0 };
	clr_mod_t clr = { 0 };

	wt = vertbox_create(owner, WD_STYLE_POPUP | WD_STYLE_NOACTIVE, &xr);

	XDL_ASSERT(wt != NULL);

	widget_get_color_mode(owner, &clr);

	widget_set_user_id(wt, IDC_VERTBOX);
	widget_set_color_mode(wt, &clr);
	vertbox_set_target(wt, owner);

	vertbox_popup_size(wt, &xs);
	widget_get_client_rect(owner, &xr);

	xr.x = xr.x + xr.w - xs.cx;
	xr.y = xr.y + xr.h / 2 - xs.cy / 2;
	xr.w = xs.cx;
	xr.h = xs.cy;

	widget_client_to_screen(owner, RECTPOINT(&xr));

	widget_move(wt, RECTPOINT(&xr));
	widget_size(wt, RECTSIZE(&xr));
	widget_set_alpha(wt, 250);

	widget_set_timer(wt, DEF_TIPTIME);

	widget_show(wt, WS_SHOW_NORMAL);

	return wt;
}