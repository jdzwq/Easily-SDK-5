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
#include "xdcimp.h"

typedef struct _plotbox_delta_t{
	vector_t vec;
	plot_t plt;
	tchar_t* title;
	tchar_t type[RES_LEN];
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

	ptd->plt.y_base = 0.0;
	ptd->plt.y_step = 1.0;
	ptd->plt.x_step = 1.0;

	return 0;
}

void hand_plotbox_destroy(res_win_t widget)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	vector_empty(&ptd->vec);
	if (ptd->title)
		xsfree(ptd->title);

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
	
}

void hand_plotbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	xrect_t xr;

	widget_get_client_rect(widget, &xr);
	widget_reset_paging(widget, xr.w, xr.h, xr.w, xr.h, 0, 0);

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

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	if (compare_text(ptd->type,-1,ATTR_PLOT_TYPE_GEOGRAM,-1,0) == 0)
		plot_geogram(canv, &xp, &xb, &xf, (xrect_t*)&cb, ptd->title, &ptd->plt, &ptd->vec);
	else if (compare_text(ptd->type, -1, ATTR_PLOT_TYPE_TRENDGRAM, -1, 0) == 0)
		plot_trendgram(canv, &xp, &xb, &xf, (xrect_t*)&cb, ptd->title, &ptd->plt, &ptd->vec);
	else if (compare_text(ptd->type, -1, ATTR_PLOT_TYPE_SCATTERGRAM, -1, 0) == 0)
		plot_scattergram(canv, &xp, &xb, &xf, (xrect_t*)&cb, ptd->title, &ptd->plt, &ptd->vec);
	else if (compare_text(ptd->type, -1, ATTR_PLOT_TYPE_PANTOGRAM, -1, 0) == 0)
		plot_pantogram(canv, &xp, &xb, &xf, (xrect_t*)&cb, ptd->title, &ptd->plt, &ptd->vec);
	else if (compare_text(ptd->type, -1, ATTR_PLOT_TYPE_HISTOGRAM, -1, 0) == 0)
		plot_histogram(canv, &xp, &xb, &xf, (xrect_t*)&cb, ptd->title, &ptd->plt, &ptd->vec);

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

void plotbox_set_type(res_win_t widget, const tchar_t* type)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xscpy(ptd->type, type);
}

void plotbox_get_type(res_win_t widget, tchar_t* type)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xscpy(type, ptd->type);
}

void plotbox_set_vetor(res_win_t widget, vector_t vt)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	vector_copy(&ptd->vec, &vt);
}

void plotbox_get_vetor(res_win_t widget, vector_t* pvt)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	vector_copy(pvt, &ptd->vec);
}

void plotbox_set_plot(res_win_t widget, const plot_t* plt)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_copy((void*)&ptd->plt, (void*)plt, sizeof(plot_t));
}

void plotbox_get_plot(res_win_t widget, plot_t* plt)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_copy((void*)plt, (void*)&ptd->plt, sizeof(plot_t));
}

void plotbox_set_title(res_win_t widget, const tchar_t* title)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (ptd->title)
		xsfree(ptd->title);

	ptd->title = xsclone(title);
}

int plotbox_get_title(res_win_t widget, tchar_t* title, int max)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	int len;

	XDL_ASSERT(ptd != NULL);

	len = xslen(ptd->title);
	len = (len < max) ? len : max;

	xsncpy(title, ptd->title, len);

	return len;
}

void plotbox_calc_plot(res_win_t widget)
{
	plotbox_delta_t* ptd = GETPLOTBOXDELTA(widget);
	double f_min, f_max;
	int i;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->vec.size)
		return;

	f_min = 0;
	f_max = 0;

	for (i = 0; i < ptd->vec.order * ptd->vec.size; i += ptd->vec.order)
	{
		if (f_min > ptd->vec.data[i])
			f_min = ptd->vec.data[i];

		if (f_max < ptd->vec.data[i])
			f_max = ptd->vec.data[i];
	}

	ptd->plt.x_step = (f_max - f_min) / (ptd->vec.size + 1);
	if (ptd->plt.x_step >(int)ptd->plt.x_step)
		ptd->plt.x_step = (int)ptd->plt.x_step + 0.5;
	
	for (i = 0; i < ptd->vec.order * ptd->vec.size; i += ptd->vec.order)
	{
		if (f_min > ptd->vec.data[i+1])
			f_min = ptd->vec.data[i+1];

		if (f_max < ptd->vec.data[i+1])
			f_max = ptd->vec.data[i+1];
	}

	ptd->plt.y_base = (f_max + f_min) / 2;
	if (ptd->plt.y_base >(int)ptd->plt.y_base)
		ptd->plt.y_base = (int)ptd->plt.y_base + 0.5;

	ptd->plt.y_step = (f_max - f_min) / (ptd->vec.size + 1);
	if (ptd->plt.y_step >(int)ptd->plt.y_step)
		ptd->plt.y_step = ptd->plt.y_step + 0.5;
}