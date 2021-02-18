/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc plot control document

	@module	plotctrl.c | implement file

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

#include "xdcctrl.h"
#include "xdcimp.h"
#include "xdcbox.h"

#define PLOT_LINE_FEED		(int)100

typedef struct _plot_delta_t{
	link_t_ptr plot;

	res_win_t hsc;
	res_win_t vsc;
}plot_delta_t;

#define GETPLOTDELTA(ph) 		(plot_delta_t*)widget_get_user_delta(ph)
#define SETPLOTDELTA(ph,ptd)		widget_set_user_delta(ph,(var_long)ptd)

/************************************************************************************************/

static void _plotctrl_reset_page(res_win_t widget)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);
	int pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fw = get_plot_width(ptd->plot);
	xs.fh = get_plot_height(ptd->plot);
	widget_size_to_pt(widget, &xs);
	fw = xs.w;
	fh = xs.h;

	xs.fw = (float)5;
	xs.fh = (float)5;
	widget_size_to_pt(widget, &xs);
	lw = xs.w;
	lh = xs.h;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

/*********************************************control event**************************************/
int noti_plot_owner(res_win_t widget, unsigned int code, link_t_ptr plot, void* data)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);
	NOTICE_PLOT nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;
	nf.ret = 0;

	nf.plot = plot;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

/********************************************************************************************/

int hand_plot_create(res_win_t widget, void* data)
{
	plot_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (plot_delta_t*)xmem_alloc(sizeof(plot_delta_t));
	xmem_zero((void*)ptd, sizeof(plot_delta_t));

	SETPLOTDELTA(widget, ptd);

	return 0;
}

void hand_plot_destroy(res_win_t widget)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETPLOTDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_plot_size(res_win_t widget, int code, const xsize_t* pxs)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	if (!ptd->plot)
		return;

	_plotctrl_reset_page(widget);

	widget_erase(widget, NULL);
}

void hand_plot_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	if (!ptd->plot)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_plot_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	if (!ptd->plot)
		return;

	noti_plot_owner(widget, NC_PLOTLBCLK, ptd->plot, (void*)pxp);
}

void hand_plot_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	if (!ptd->plot)
		return;

	noti_plot_owner(widget, NC_PLOTDBCLK, ptd->plot, (void*)pxp);
}

void hand_plot_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	if (!ptd->plot)
		return;

}

void hand_plot_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	if (!ptd->plot)
		return;

	noti_plot_owner(widget, NC_PLOTRBCLK, ptd->plot, (void*)pxp);
}

void hand_plot_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	if (!ptd->plot)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_plot_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);
	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;

	if (!ptd->plot)
		return;

	widget_get_scroll_info(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widget_hand_scroll(widget, bHorz, nLine))
	{
		if (!bHorz && !(widget_get_style(widget) & WD_STYLE_VSCROLL))
		{
			if (!widget_is_valid(ptd->vsc))
			{
				ptd->vsc = show_vertbox(widget);
			}
		}

		if (bHorz && !(widget_get_style(widget) & WD_STYLE_HSCROLL))
		{
			if (!widget_is_valid(ptd->hsc))
			{
				ptd->hsc = show_horzbox(widget);
			}
		}

		return;
	}

	win = widget_get_parent(widget);

	if (widget_is_valid(win))
	{
		widget_scroll(win, bHorz, nLine);
	}
}

void hand_plot_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);
	visual_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	const drawing_interface* pif = NULL;
	drawing_interface ifv = {0};

	if (!ptd->plot)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = widget_get_canvas_interface(widget);
	

	
	
	
	
	

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	if (widget_can_paging(widget))
	{
		parse_xcolor(&xc, xp.color);
		lighten_xcolor(&xc, DEF_HARD_DARKEN);

		draw_corner(pif, &xc, (const xrect_t*)&(pif->rect));
	}

	if (ptd->plot)
	{
		draw_plot(pif, ptd->plot);
	}

	

	end_canvas_paint(canv, dc, pxr);
	
}

/*****************************************************************************************************/

res_win_t plotctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_plot_create)
		EVENT_ON_DESTROY(hand_plot_destroy)

		EVENT_ON_PAINT(hand_plot_paint)

		EVENT_ON_SIZE(hand_plot_size)

		EVENT_ON_SCROLL(hand_plot_scroll)
		EVENT_ON_WHEEL(hand_plot_wheel)

		EVENT_ON_LBUTTON_DBCLICK(hand_plot_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_plot_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_plot_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_plot_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_plot_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void plotctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_plot_doc(ptr));

	ptd->plot = ptr;

	plotctrl_redraw(widget);
}

link_t_ptr plotctrl_detach(res_win_t widget)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);
	link_t_ptr ptr;

	XDL_ASSERT(ptd != NULL);

	ptr = ptd->plot;
	ptd->plot = NULL;

	widget_erase(widget, NULL);
	return ptr;
}

link_t_ptr plotctrl_fetch(res_win_t widget)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->plot;
}

void plotctrl_redraw(res_win_t widget)
{
	plot_delta_t* ptd = GETPLOTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->plot)
		return;

	_plotctrl_reset_page(widget);

	widget_update(widget);
}
