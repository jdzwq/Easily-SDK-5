/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc icon control document

	@module	iconbox.c | implement file

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
#include "winnc.h"

typedef struct _iconbox_delta_t{
	link_t_ptr string;

	tchar_t layer[RES_LEN];
	tchar_t align[RES_LEN];
}iconbox_delta_t;

#define GETICONBOXDELTA(ph) 	(iconbox_delta_t*)widget_get_user_delta(ph)
#define SETICONBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*********************************************************************************/
void _iconbox_item_rect(res_win_t widget, link_t_ptr ent, xrect_t* pxr)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };
	xsize_t xs;
	canvbox_t cb;

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_get_canv_rect(widget, &cb);
	xs.fx = cb.fw;
	xs.fy = cb.fh;

	calc_iconbox_item_rect(&im, &xf, ptd->layer, ptd->align, &xs, ptd->string, ent, pxr);
	widget_rect_to_tm(widget, pxr);
}

void _iconbox_reset_page(res_win_t widget)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	long vw, vh, lw, lh;
	xrect_t xr;
	xsize_t xs;
	xfont_t xf;
	if_measure_t im = { 0 };

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_get_xfont(widget, &xf);

	text_metric((canvas_t)im.ctx, &xf, &xs);
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	calc_iconbox_size(&im, &xf, ptd->layer, ptd->align, ptd->string, &xs);
	widget_size_to_pt(widget, &xs);
	vw = xs.cx;
	vh = xs.cy;

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, vw, vh, lw, lh);

	widget_reset_scroll(widget, 0);
}
/*********************************************************************************/
void noti_iconbox_command(res_win_t widget, int code, var_long data)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

void iconbox_on_click_item(res_win_t widget, link_t_ptr ent)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);

	widget_update(widget, NULL, 0);

	noti_iconbox_command(widget, xstol(get_string_entity_key_ptr(ent)), (var_long)NULL);
}

/*********************************************************************************/
int hand_iconbox_create(res_win_t widget, void* data)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);

	ptd = (iconbox_delta_t*)xmem_alloc(sizeof(iconbox_delta_t));
	xmem_zero((void*)ptd, sizeof(iconbox_delta_t));

	ptd->string = create_string_table(0);

	SETICONBOXDELTA(widget, ptd);

	return 0;
}

void hand_iconbox_destroy(res_win_t widget)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (ptd->string)
		destroy_string_table(ptd->string);

	xmem_free(ptd);

	SETICONBOXDELTA(widget, 0);
}

void hand_iconbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	
}

void hand_iconbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };
	xpoint_t pt;
	xsize_t xs;
	canvbox_t cb;

	link_t_ptr ilk = NULL;
	int hint;

	pt.x = pxp->x;
	pt.y = pxp->y;

	widget_point_to_tm(widget, &pt);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_get_canv_rect(widget, &cb);
	xs.fx = cb.fw;
	xs.fy = cb.fh;

	hint = calc_iconbox_hint(&im, &xf, ptd->layer, ptd->align, &xs, &pt, ptd->string, &ilk);

	if (hint == ICONBOX_HINT_ITEM)
	{
		iconbox_on_click_item(widget, ilk);
	}
}

void hand_iconbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	
	_iconbox_reset_page(widget);

	widget_update(widget, NULL, 0);
}

void hand_iconbox_erase(res_win_t widget, res_ctx_t rdc)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	
}

void hand_iconbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
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
	widget_get_xcolor(widget, &pif->clr_msk);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_iconbox(pif, &cb, &xf, ptd->layer, ptd->align, ptd->string);

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t iconbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_iconbox_create)
		EVENT_ON_DESTROY(hand_iconbox_destroy)

		EVENT_ON_ERASE(hand_iconbox_erase)
		EVENT_ON_PAINT(hand_iconbox_paint)

		EVENT_ON_SIZE(hand_iconbox_size)

		EVENT_ON_LBUTTON_DOWN(hand_iconbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_iconbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void iconbox_set_options(res_win_t widget, const tchar_t* opt, int len)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	string_table_parse_options(ptd->string, opt, len, OPT_ITEMFEED, OPT_LINEFEED);

	widget_update(widget, NULL, 0);
}

void iconbox_set_layer(res_win_t widget, const tchar_t* layer)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xsncpy(ptd->layer, layer, RES_LEN);
}

void iconbox_set_alignment(res_win_t widget, const tchar_t* align)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xsncpy(ptd->align, align, RES_LEN);
}

void iconbox_get_item_rect(res_win_t widget, const tchar_t* key, xrect_t* pxr)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	link_t_ptr ent;

	XDL_ASSERT(ptd != NULL);

	ent = get_string_entity(ptd->string, key, -1);
	if (ent)
	{
		_iconbox_item_rect(widget, ent, pxr);
	}
	else
	{
		xmem_zero((void*)pxr, sizeof(xrect_t));
	}
}

void iconbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	iconbox_delta_t* ptd = GETICONBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_iconbox_size(&im, &xf, ptd->layer, ptd->align, ptd->string, pxs);

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}
