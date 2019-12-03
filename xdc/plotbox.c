/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc plot control document

	@module	plotbox.c | implement file

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

typedef struct _plotbox_delta_t{
	bool_t on;
}plotbox_delta_t;

#define GETPLOTBOXDELTA(ph) 	(plotbox_delta_t*)widget_get_user_delta(ph)
#define SETPLOTBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/********************************************************************************/
void noti_plotbox_command(res_win_t widget, int code, var_long data)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}
/*********************************************************************************/
int hand_plotbox_create(res_win_t widget, void* data)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	widget_hand_create(widget);

	ptd = (plotbox_delta_t*)xmem_alloc(sizeof(plotbox_delta_t));
	xmem_zero((void*)ptd, sizeof(plotbox_delta_t));

	SETPLOTBOXDELTA(widget, ptd);

	return 0;
}

void hand_plotbox_destroy(res_win_t widget)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETPLOTBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_plotbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	
}

void hand_plotbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };
	xpoint_t pt;
	int hint;

	pt.x = pxp->x;
	pt.y = pxp->y;

	widget_point_to_tm(widget, &pt);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	
}

void hand_plotbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	
	widget_redraw(widget, NULL, 0);
}

void hand_plotbox_erase(res_win_t widget, res_ctx_t rdc)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	
}

void hand_plotbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
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
	
	xscpy(xp.size, _T("2"));
	draw_rect_raw(rdc, &xp, &xb, &xr);

	widget_get_canv_rect(widget, &cb);


	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t plotbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_plotbox_create)
		EVENT_ON_DESTROY(hand_plotbox_destroy)

		EVENT_ON_ERASE(hand_plotbox_erase)
		EVENT_ON_PAINT(hand_plotbox_paint)

		EVENT_ON_SIZE(hand_plotbox_size)

		EVENT_ON_LBUTTON_DOWN(hand_plotbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_plotbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void plotbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}


