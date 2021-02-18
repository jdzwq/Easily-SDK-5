/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tip control document

	@module	tipbox.c | implement file

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

typedef struct _TIPBOX_DATA{
	int type;
	const tchar_t* text;
}TIPBOX_DATA;

typedef struct _tipbox_delta_t{
	int n_type;
	tchar_t* sz_text;
}tipbox_delta_t;

#define GETTIPBOXDELTA(ph) 	(tipbox_delta_t*)widget_get_user_delta(ph)
#define SETTIPBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*********************************************************************************/
int hand_tipbox_create(res_win_t widget, void* data)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);
	TIPBOX_DATA* ppd;

	widget_hand_create(widget);

	ptd = (tipbox_delta_t*)xmem_alloc(sizeof(tipbox_delta_t));
	xmem_zero((void*)ptd, sizeof(tipbox_delta_t));

	ppd = (TIPBOX_DATA*)data;
	ptd->n_type = ppd->type;
	ptd->sz_text = xsalloc(xslen(ppd->text) + 1);
	xscpy(ptd->sz_text, ppd->text);

	SETTIPBOXDELTA(widget, ptd);

	widget_set_timer(widget, DEF_TIPTIME);

	return 0;
}

void hand_tipbox_destroy(res_win_t widget)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (ptd->sz_text)
		xsfree(ptd->sz_text);

	xmem_free(ptd);

	SETTIPBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_tipbox_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);

	widget_close(widget, 0);
}

void hand_tipbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);
	
	widget_erase(widget, NULL);
}

void hand_tipbox_timer(res_win_t widget, var_long tid)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);

	widget_kill_timer(widget, tid);

	widget_close(widget, 0);
}

void hand_tipbox_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);
	visual_t rdc;

	xrect_t xr;
	const tchar_t *token;

	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };

	canvas_t canv;
	const drawing_interface* pif = NULL;
	drawing_interface ifv = {0};

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);
	pif = widget_get_canvas_interface(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	xp.adorn.feed = 2;
	xp.adorn.size = 2;
	(*ifv.pf_draw_rect)(ifv.ctx, &xp, &xb, &xr);

	token = ptd->sz_text;

	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	(*ifv.pf_draw_text)(ifv.ctx, &xf, &xa, &xr, token, -1);

	
	end_canvas_paint(canv, dc, pxr);
	
}

/***************************************************************************************/
res_win_t tipbox_create(res_win_t widget, dword_t style, const xrect_t* pxr, int type, const tchar_t* text)
{
	if_event_t ev = { 0 };
	TIPBOX_DATA pd = { 0 };

	pd.type = type;
	pd.text = text;

	ev.param = (void*)&pd;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_tipbox_create)
		EVENT_ON_DESTROY(hand_tipbox_destroy)

		EVENT_ON_PAINT(hand_tipbox_paint)

		EVENT_ON_SIZE(hand_tipbox_size)

		EVENT_ON_MOUSE_MOVE(hand_tipbox_mouse_move)

		EVENT_ON_TIMER(hand_tipbox_timer)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void tipbox_set_text(res_win_t widget, const tchar_t* sz_text)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);

	XDL_ASSERT(ptd);

	xmem_free(ptd->sz_text);
	ptd->sz_text = xsalloc(xslen(sz_text) + 1);
	xscpy(ptd->sz_text, sz_text);
}

void tipbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	tipbox_delta_t* ptd = GETTIPBOXDELTA(widget);
	xfont_t xf;
	xface_t xa;
	visual_t rdc;
	xrect_t xr = { 0 };
	drawing_interface ifv = {0};

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	rdc = widget_client_ctx(widget);

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_text_rect)(ifv.ctx, &xf, &xa, ptd->sz_text, -1, &xr);

	

	widget_release_ctx(widget, rdc);

	pxs->w = xr.w + 10;
	pxs->h = xr.h + 4;

	widget_adjust_size(widget_get_style(widget), pxs);
}

/////////////////////////////////////////////////////////////////////////////////////////////
res_win_t show_toolbox(const xpoint_t* ppt, const tchar_t* sz_text)
{
	res_win_t wt;
	xrect_t xr = { 0 };
	xsize_t xs = { 0 };
	clr_mod_t clr;

	wt = tipbox_create(NULL, WD_STYLE_POPUP | WD_STYLE_NOACTIVE, &xr, 1, sz_text);
	if (!wt)
		return NULL;

	tipbox_popup_size(wt, RECTSIZE(&xr));

	if (ppt)
	{
		xr.x = ppt->x;
		xr.y = ppt->y;
	}
	else
	{
		get_desktop_size(&xs);
		xr.x = xs.w - xr.w - 1;
		xr.y = xs.h - xr.h;
	}

	widget_get_color_mode(wt, &clr);
	parse_xcolor(&clr.clr_bkg, GDI_ATTR_RGB_SOFTWHITE);
	parse_xcolor(&clr.clr_txt, GDI_ATTR_RGB_DARKCYAN);
	widget_set_color_mode(wt, &clr);

	widget_move(wt, RECTPOINT(&xr));
	widget_size(wt, RECTSIZE(&xr));
	widget_take(wt, (int)WS_TAKE_TOPMOST);
	widget_update(wt);

	widget_set_timer(wt, DEF_TIPTIME);

	widget_show(wt, WS_SHOW_NORMAL);

	return wt;
}

bool_t reset_toolbox(res_win_t widget, const xpoint_t* ppt, const tchar_t* sz_text)
{
	xrect_t xr;
	xsize_t xs;

	if (!widget_is_valid(widget))
		return 0;

	widget_show(widget, WS_SHOW_HIDE);

	widget_kill_timer(widget, 0);

	tipbox_set_text(widget, sz_text);

	tipbox_popup_size(widget, RECTSIZE(&xr));

	if (ppt)
	{
		xr.x = ppt->x;
		xr.y = ppt->y;
	}
	else
	{
		get_desktop_size(&xs);
		xr.x = xs.w - xr.w - 1;
		xr.y = xs.h - xr.h;
	}

	widget_move(widget, RECTPOINT(&xr));
	widget_size(widget, RECTSIZE(&xr));
	widget_take(widget, (int)WS_TAKE_TOPMOST);
	widget_update(widget);

	widget_set_timer(widget, DEF_TIPTIME);

	widget_show(widget, WS_SHOW_NORMAL);

	return 1;
}
