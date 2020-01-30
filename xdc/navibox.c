/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc navi control document

	@module	navibox.c | implement file

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

typedef struct _navibox_delta_t{
	res_win_t target;
	res_win_t keybox;
}navibox_delta_t;

#define GETNAVIBOXDELTA(ph) 	(navibox_delta_t*)widget_get_user_delta(ph)
#define SETNAVIBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_int)ptd)

/*********************************************************************************/
void navibox_on_home(res_win_t widget)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	widget_post_key(ptd->target, VK_HOME);

	widget_redraw(widget, NULL, 1);
}

void navibox_on_end(res_win_t widget)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	widget_post_key(ptd->target, KEY_END);

	widget_redraw(widget, NULL, 1);
}

void navibox_on_prev(res_win_t widget)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	widget_post_key(ptd->target, KEY_PAGEUP);

	widget_redraw(widget, NULL, 1);
}

void navibox_on_next(res_win_t widget)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	widget_post_key(ptd->target, KEY_PAGEDOWN);

	widget_redraw(widget, NULL, 1);
}

void navibox_on_keyboard(res_win_t widget)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	xrect_t xr_owner, xr = { 0 };
	clr_mod_t clr;
	res_win_t owner;

	if (widget_is_valid(ptd->keybox))
	{
		widget_destroy(ptd->keybox);
		ptd->keybox = NULL;

		widget_redraw(widget, NULL, 1);
		return;
	}

	owner = widget_get_owner(widget);
	if (!widget_is_valid(owner))
		return;

	ptd->keybox = keybox_create(widget, WD_STYLE_POPUP | WD_STYLE_NOACTIVE, &xr);
	
	widgetex_get_color_mode(widget, &clr);

	widgetex_set_color_mode(ptd->keybox, &clr);

	widget_get_window_rect(owner, &xr_owner);
	widget_get_window_rect(widget, &xr);

	xr_owner.x = xr.x;
	xr_owner.w = xr.w;
	if (xr.y - xr_owner.y < (xr_owner.y + xr_owner.h) - (xr.y + xr.h))
	{
		xr.y += (xr.h + 1);
		keybox_popup_size(ptd->keybox, RECTSIZE(&xr));
	}
	else
	{
		keybox_popup_size(ptd->keybox, RECTSIZE(&xr));
		xr.y -= (xr.h + 1);
	}
	if (xr.x + xr.w > xr_owner.x + xr_owner.w)
	{
		xr.x = xr_owner.x + xr_owner.w - xr.w;
	}
	
	widget_move(ptd->keybox, RECTPOINT(&xr));
	widget_size(ptd->keybox, RECTSIZE(&xr));
	widget_take(ptd->keybox, (int)WD_ZORDER_TOPMOST);
	widget_update(ptd->keybox);
	widget_show(ptd->keybox, WD_SHOW_NORMAL);

	widget_redraw(widget, NULL, 1);
}

/*********************************************************************************/
int hand_navibox_create(res_win_t widget, void* data)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	widgetex_hand_create(widget);

	ptd = (navibox_delta_t*)xmem_alloc(sizeof(navibox_delta_t));
	xmem_zero((void*)ptd, sizeof(navibox_delta_t));

	SETNAVIBOXDELTA(widget, ptd);

	return 0;
}

void hand_navibox_destroy(res_win_t widget)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETNAVIBOXDELTA(widget, 0);

	widgetex_hand_destroy(widget);
}

void hand_navibox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	
}

void hand_navibox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
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

	hint = calc_navibox_hint(&im, &xf, &pt);

	if (hint == NAVIBOX_HINT_HOME)
		navibox_on_home(widget);
	else if(hint == NAVIBOX_HINT_PREV)
		navibox_on_prev(widget);
	else if (hint == NAVIBOX_HINT_NEXT)
		navibox_on_next(widget);
	else if (hint == NAVIBOX_HINT_END)
		navibox_on_end(widget);
	else if (hint == NAVIBOX_HINT_KEYBOARD)
		navibox_on_keyboard(widget);
}

void hand_navibox_size(res_win_t widget, int code, const xsize_t* prs)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	
	widget_redraw(widget, NULL, 0);
}

void hand_navibox_erase(res_win_t widget, res_ctx_t rdc)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	
}

void hand_navibox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	res_ctx_t rdc;
	xrect_t xr;
	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };
	NAVISTATE ns = { 0 };

	xfont_t xf;
	xbrush_t xb;
	xpen_t xp;
	xgradi_t xg = { 0 };

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

	xscpy(xg.brim_color, xb.color);
	xscpy(xg.core_color, xb.color);
	lighten_xgradi(&xg, DEF_SOFT_DARKEN);
	xscpy(xg.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	gradient_rect_raw(rdc, &xg, &xr);

	widgetex_get_canv_rect(widget, &cb);

	ns.keyboxed = widget_is_valid(ptd->keybox);

	draw_navibox(pif, &cb, &xf, &ns);

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t navibox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_navibox_create)
		EVENT_ON_DESTROY(hand_navibox_destroy)

		EVENT_ON_ERASE(hand_navibox_erase)
		EVENT_ON_PAINT(hand_navibox_paint)

		EVENT_ON_SIZE(hand_navibox_size)

		EVENT_ON_LBUTTON_DOWN(hand_navibox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_navibox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void navibox_set_target(res_win_t widget, res_win_t target)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);

	ptd->target = target;
}

void navibox_popup_size(res_win_t widget, xsize_t* pxs)
{
	navibox_delta_t* ptd = GETNAVIBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widgetex_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_navibox_size(&im, &xf, pxs);

	widgetex_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

