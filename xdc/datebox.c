/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc number control document

	@module	datebox.c | number control widnows implement file

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

typedef struct _datebox_delta_t{
	xdate_t dt;
}datebox_delta_t;

#define GETDATEBOXDELTA(ph) 	(datebox_delta_t*)widget_get_user_delta(ph)
#define SETDATEBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/**************************************************************************************************/
void noti_datebox_command(res_win_t widget, int code, var_long data)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

void datebox_on_prev_month(res_win_t widget)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	ptd->dt.mon--;

	if (!ptd->dt.mon)
	{
		ptd->dt.year--;
		ptd->dt.mon = 12;
	}

	widget_update(widget, NULL, 1);
}

void datebox_on_next_month(res_win_t widget)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	ptd->dt.mon++;

	if (ptd->dt.mon > 12)
	{
		ptd->dt.year++;
		ptd->dt.mon = 1;
	}

	widget_update(widget, NULL, 1);
}

void datebox_on_select_day(res_win_t widget, int day)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	ptd->dt.day = day;

	widget_update(widget, NULL, 1);

	noti_datebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}
/**************************************************************************************************/
int hand_datebox_create(res_win_t widget, void* data)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	widget_hand_create(widget);

	ptd = (datebox_delta_t*)xmem_alloc(sizeof(datebox_delta_t));

	SETDATEBOXDELTA(widget, ptd);

	get_loc_date(&ptd->dt);

	return 0;
}

void hand_datebox_destroy(res_win_t widget)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	xmem_free(ptd);

	SETDATEBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_datebox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

}

void hand_datebox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	if_measure_t im = { 0 };
	xfont_t xf = { 0 };
	xpoint_t pt;
	int hint;
	int day;

	pt.x = pxp->x;
	pt.y = pxp->y;

	widget_point_to_tm(widget, &pt);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	day = 0;
	hint = calc_datebox_hint(&im, &xf, &pt, &ptd->dt, &day);

	if (hint == DATEBOX_HINT_PREV)
		datebox_on_prev_month(widget);
	else if (hint == DATEBOX_HINT_NEXT)
		datebox_on_next_month(widget);
	else if (hint == DATEBOX_HINT_DAYS)
	{
		datebox_on_select_day(widget, day);

		noti_datebox_command(widget, COMMAND_CHANGE, (var_long)NULL);
	}
}

void hand_datebox_size(res_win_t widget, int code, const xsize_t* prs)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);
	
	widget_update(widget, NULL, 0);
}

void hand_datebox_erase(res_win_t widget, res_ctx_t rdc)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);
	
}

void hand_datebox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);
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

	draw_datebox(pif, &cb, &xf, &ptd->dt);

	end_canvas_paint(canv, dc, pxr);

	destroy_canvas_interface(pif);
}

/*******************************************************************************************************/
res_win_t datebox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_datebox_create)
		EVENT_ON_DESTROY(hand_datebox_destroy)

		EVENT_ON_ERASE(hand_datebox_erase)
		EVENT_ON_PAINT(hand_datebox_paint)

		EVENT_ON_SIZE(hand_datebox_size)

		EVENT_ON_LBUTTON_DOWN(hand_datebox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_datebox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void datebox_popup_size(res_win_t widget, xsize_t* pxs)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_datebox_size(&im, &xf, pxs);

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void datebox_set_date(res_win_t widget, const xdate_t* pxd)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (pxd)
		xmem_copy((void*)&ptd->dt, (void*)pxd, sizeof(xdate_t));
	else
		get_loc_date(&ptd->dt);

	widget_update(widget, NULL, 1);
}

void datebox_get_date(res_win_t widget, xdate_t* pxd)
{
	datebox_delta_t* ptd = GETDATEBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (pxd)
		xmem_copy((void*)pxd, (void*)&ptd->dt, sizeof(xdate_t));
}
