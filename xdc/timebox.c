/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc time control document

	@module	timebox.c | implement file

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

typedef struct _timebox_delta_t{
	xdate_t tt;
}timebox_delta_t;

#define GETTIMEBOXDELTA(ph) 	(timebox_delta_t*)widget_get_user_delta(ph)
#define SETTIMEBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*********************************************************************************/
void noti_timebox_command(res_win_t widget, int code, var_long data)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

void timebox_on_year_up(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.year++;
	if (ptd->tt.year > MAX_YEAR)
		ptd->tt.year = MIN_YEAR;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_year_down(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.year--;
	if (ptd->tt.year < MIN_YEAR)
		ptd->tt.year = MIN_YEAR;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_mon_up(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.mon++;
	if (ptd->tt.mon > 12)
		ptd->tt.mon = 1;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_mon_down(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.mon--;
	if (ptd->tt.mon < 1)
		ptd->tt.mon = 12;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_day_up(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.day++;
	if (ptd->tt.day > max_mon_days(ptd->tt.year, ptd->tt.mon))
		ptd->tt.day = 1;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_day_down(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.day--;
	if (ptd->tt.day < 1)
		ptd->tt.day = max_mon_days(ptd->tt.year, ptd->tt.mon);

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_hour_up(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.hour ++;
	if (ptd->tt.hour > 23)
		ptd->tt.hour = 0;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_hour_down(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.hour--;
	if (ptd->tt.hour < 0)
		ptd->tt.hour = 23;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_hour_zero(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.hour = 0;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_minute_up(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.min++;
	if (ptd->tt.min > 59)
		ptd->tt.min = 0;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_minute_down(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.min--;
	if (ptd->tt.min < 0)
		ptd->tt.min = 59;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_minute_zero(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.min = 0;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_second_up(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.sec++;
	if (ptd->tt.sec > 59)
		ptd->tt.sec = 0;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_second_down(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.sec--;
	if (ptd->tt.sec < 0)
		ptd->tt.sec = 59;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

void timebox_on_second_zero(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	ptd->tt.sec = 0;

	widget_redraw(widget, NULL, 1);

	noti_timebox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}
/*********************************************************************************/
int hand_timebox_create(res_win_t widget, void* data)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	widgetex_hand_create(widget);

	ptd = (timebox_delta_t*)xmem_alloc(sizeof(timebox_delta_t));
	xmem_zero((void*)ptd, sizeof(timebox_delta_t));

	SETTIMEBOXDELTA(widget, ptd);

	get_loc_date(&ptd->tt);

	return 0;
}

void hand_timebox_destroy(res_win_t widget)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETTIMEBOXDELTA(widget, 0);

	widgetex_hand_destroy(widget);
}

void hand_timebox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);
	
}

void hand_timebox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);
	
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

	hint = calc_timebox_hint(&im, &xf, &pt);

	if (hint == TIMEBOX_HINT_YEAR_UP)
		timebox_on_year_up(widget);
	else if (hint == TIMEBOX_HINT_YEAR_DOWN)
		timebox_on_year_down(widget);
	else if (hint == TIMEBOX_HINT_MON_UP)
		timebox_on_mon_up(widget);
	else if (hint == TIMEBOX_HINT_MON_DOWN)
		timebox_on_mon_down(widget);
	else if (hint == TIMEBOX_HINT_DAY_UP)
		timebox_on_day_up(widget);
	else if (hint == TIMEBOX_HINT_DAY_DOWN)
		timebox_on_day_down(widget);
	else if (hint == TIMEBOX_HINT_HOUR_UP)
		timebox_on_hour_up(widget);
	else if (hint == TIMEBOX_HINT_HOUR_DOWN)
		timebox_on_hour_down(widget);
	else if (hint == TIMEBOX_HINT_HOUR_ZERO)
		timebox_on_hour_zero(widget);
	else if (hint == TIMEBOX_HINT_MIN_UP)
		timebox_on_minute_up(widget);
	else if (hint == TIMEBOX_HINT_MIN_DOWN)
		timebox_on_minute_down(widget);
	else if (hint == TIMEBOX_HINT_MIN_ZERO)
		timebox_on_minute_zero(widget);
	else if (hint == TIMEBOX_HINT_SEC_UP)
		timebox_on_second_up(widget);
	else if (hint == TIMEBOX_HINT_SEC_DOWN)
		timebox_on_second_down(widget);
	else if (hint == TIMEBOX_HINT_SEC_ZERO)
		timebox_on_second_zero(widget);
	else
	{
		noti_timebox_command(widget, COMMAND_CHANGE, (var_long)NULL);
	}
}

void hand_timebox_size(res_win_t widget, int code, const xsize_t* prs)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);
	
	widget_redraw(widget, NULL, 0);
}

void hand_timebox_erase(res_win_t widget, res_ctx_t rdc)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);
	
}

void hand_timebox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);
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

	draw_timebox(pif, &cb, &xf, &ptd->tt);

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t timebox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_timebox_create)
		EVENT_ON_DESTROY(hand_timebox_destroy)

		EVENT_ON_ERASE(hand_timebox_erase)
		EVENT_ON_PAINT(hand_timebox_paint)

		EVENT_ON_SIZE(hand_timebox_size)

		EVENT_ON_LBUTTON_DOWN(hand_timebox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_timebox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void timebox_popup_size(res_win_t widget, xsize_t* pxs)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widgetex_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_timebox_size(&im, &xf, pxs);

	widgetex_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void timebox_set_datetime(res_win_t widget, const xdate_t* pxt)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (pxt)
		xmem_copy((void*)&ptd->tt, (void*)pxt, sizeof(xdate_t));
	else
		get_loc_date(&ptd->tt);

	widget_redraw(widget, NULL, 1);
}

void timebox_get_datetime(res_win_t widget, xdate_t* pxt)
{
	timebox_delta_t* ptd = GETTIMEBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (pxt)
		xmem_copy((void*)pxt, (void*)&ptd->tt, sizeof(xdate_t));
}
