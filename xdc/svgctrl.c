/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg control document

	@module	svgctrl.c | implement file

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
#include "handler.h"
#include "widgetnc.h"
#include "widgetex.h"
#include "xdcbox.h"

#define SVG_LINE_FEED		(int)100

typedef struct _svg_delta_t{
	link_t_ptr svg;

	res_win_t hsc;
	res_win_t vsc;
}svg_delta_t;

#define GETSVGDELTA(ph) 		(svg_delta_t*)widget_get_user_delta(ph)
#define SETSVGDELTA(ph,ptd)		widget_set_user_delta(ph,(var_long)ptd)

/************************************************************************************************/

static void _svgctrl_reset_page(res_win_t widget)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);
	int pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_svg_width(ptd->svg);
	xs.fy = get_svg_height(ptd->svg);
	widgetex_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = (float)5;
	xs.fy = (float)5;
	widgetex_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widgetex_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

/*********************************************control event**************************************/
int noti_svg_owner(res_win_t widget, unsigned int code, link_t_ptr svg, void* data)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);
	NOTICE_SVG nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;
	nf.ret = 0;

	nf.svg = svg;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

/********************************************************************************************/

int hand_svg_create(res_win_t widget, void* data)
{
	svg_delta_t* ptd;

	widgetex_hand_create(widget);

	ptd = (svg_delta_t*)xmem_alloc(sizeof(svg_delta_t));
	xmem_zero((void*)ptd, sizeof(svg_delta_t));

	SETSVGDELTA(widget, ptd);

	return 0;
}

void hand_svg_destroy(res_win_t widget)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETSVGDELTA(widget, 0);

	widgetex_hand_destroy(widget);
}

void hand_svg_size(res_win_t widget, int code, const xsize_t* pxs)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;

	_svgctrl_reset_page(widget);

	widget_redraw(widget, NULL, 0);
}

void hand_svg_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_svg_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;

	noti_svg_owner(widget, NC_SVGLBCLK, ptd->svg, (void*)pxp);
}

void hand_svg_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;

	noti_svg_owner(widget, NC_SVGDBCLK, ptd->svg, (void*)pxp);
}

void hand_svg_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;

}

void hand_svg_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;

	noti_svg_owner(widget, NC_SVGRBCLK, ptd->svg, (void*)pxp);
}

void hand_svg_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;

	widgetex_hand_scroll(widget, bHorz, nLine);
}

void hand_svg_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);
	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;

	if (!ptd->svg)
		return;

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widgetex_hand_scroll(widget, bHorz, nLine))
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

void hand_svg_erase(res_win_t widget, res_ctx_t dc)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	if (!ptd->svg)
		return;
}

void hand_svg_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->svg)
		return;

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

	if (widget_can_paging(widget))
	{
		parse_xcolor(&xc, xp.color);
		lighten_xcolor(&xc, DEF_HARD_DARKEN);

		draw_corner(canv, &xc, (const xrect_t*)&cb);
	}

	if (ptd->svg)
	{
		xr.fx = cb.fx;
		xr.fy = cb.fy;
		xr.fw = cb.fw;
		xr.fh = cb.fh;
		draw_svg(pif->canvas, &xr, ptd->svg);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*****************************************************************************************************/

res_win_t svgctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_svg_create)
		EVENT_ON_DESTROY(hand_svg_destroy)

		EVENT_ON_ERASE(hand_svg_erase)
		EVENT_ON_PAINT(hand_svg_paint)

		EVENT_ON_SIZE(hand_svg_size)

		EVENT_ON_SCROLL(hand_svg_scroll)
		EVENT_ON_WHEEL(hand_svg_wheel)

		EVENT_ON_LBUTTON_DBCLICK(hand_svg_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_svg_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_svg_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_svg_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_svg_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void svgctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_svg_doc(ptr));

	ptd->svg = ptr;

	svgctrl_redraw(widget);
}

link_t_ptr svgctrl_detach(res_win_t widget)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);
	link_t_ptr ptr;

	XDL_ASSERT(ptd != NULL);

	ptr = ptd->svg;
	ptd->svg = NULL;

	widget_redraw(widget, NULL, 0);
	return ptr;
}

link_t_ptr svgctrl_fetch(res_win_t widget)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->svg;
}

void svgctrl_redraw(res_win_t widget)
{
	svg_delta_t* ptd = GETSVGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->svg)
		return;

	_svgctrl_reset_page(widget);

	widget_update(widget);
}
