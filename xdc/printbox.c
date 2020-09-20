/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc print control document

	@module	printbox.c | implement file

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

#define SVG_LINE_FEED		(int)100

typedef struct _print_delta_t{
	link_t_ptr sheet;

	int pages, page;

	res_win_t hsc;
	res_win_t vsc;
}print_delta_t;

#define GETPRINTDELTA(ph) 		(print_delta_t*)widget_get_user_delta(ph)
#define SETPRINTDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

/************************************************************************************************/
static int _printbox_calc_pages(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	canvbox_t cb = { 0 };
	canvas_t canv;
	xrect_t xr;
	int pages = 0;

	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	canv = widget_get_canvas(widget);

	widget_get_canv_rect(widget, &cb);

	widget_get_xface(widget, &xa);
	widget_get_xfont(widget, &xf);

	if (is_form_doc(ptd->sheet))
	{
		pages = calc_form_pages(&cb, ptd->sheet);
	}
	else if (is_grid_doc(ptd->sheet))
	{
		pages = calc_grid_pages(&cb, ptd->sheet);
	}
	else if (is_statis_doc(ptd->sheet))
	{
		pages = calc_statis_pages(&cb, ptd->sheet);
	}
	else if (is_rich_doc(ptd->sheet))
	{
		xr.fx = cb.fx;
		xr.fy = cb.fy;
		xr.fw = cb.fw;
		xr.fh = cb.fh;

		pages = calc_rich_pages(canv, &xf, &xa, &xr, ptd->sheet);
	}
	else if (is_memo_doc(ptd->sheet))
	{
		xr.fx = cb.fx;
		xr.fy = cb.fy;
		xr.fw = cb.fw;
		xr.fh = cb.fh;

		pages = calc_rich_pages(canv, &xf, &xa, &xr, ptd->sheet);
	}

	return pages;
}

static void _printbox_reset_page(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	int pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	if (is_form_doc(ptd->sheet))
	{
		xs.fx = get_form_width(ptd->sheet);
		xs.fy = get_form_height(ptd->sheet);
	}
	else if (is_grid_doc(ptd->sheet))
	{
		xs.fx = get_grid_width(ptd->sheet);
		xs.fy = get_grid_height(ptd->sheet);
	}
	else if (is_statis_doc(ptd->sheet))
	{
		xs.fx = get_statis_width(ptd->sheet);
		xs.fy = get_statis_height(ptd->sheet);
	}
	else if (is_rich_doc(ptd->sheet))
	{
		xs.fx = get_rich_width(ptd->sheet);
		xs.fy = get_rich_height(ptd->sheet);
	}
	else if (is_memo_doc(ptd->sheet))
	{
		xs.fx = get_memo_width(ptd->sheet);
		xs.fy = get_memo_height(ptd->sheet);
	}

	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = (float)5;
	xs.fy = (float)5;
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

/********************************************************************************************/

int hand_print_create(res_win_t widget, void* data)
{
	print_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (print_delta_t*)xmem_alloc(sizeof(print_delta_t));
	xmem_zero((void*)ptd, sizeof(print_delta_t));

	SETPRINTDELTA(widget, ptd);

	return 0;
}

void hand_print_destroy(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETPRINTDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_print_size(res_win_t widget, int code, const xsize_t* pxs)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	if (!ptd->sheet)
		return;

	_printbox_reset_page(widget);

	widget_erase(widget, NULL);
}

void hand_print_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	if (!ptd->sheet)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_print_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	if (!ptd->sheet)
		return;
}

void hand_print_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	if (!ptd->sheet)
		return;

}

void hand_print_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	if (!ptd->sheet)
		return;

}

void hand_print_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	if (!ptd->sheet)
		return;

}

void hand_print_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	if (!ptd->sheet)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_print_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;

	if (!ptd->sheet)
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

void hand_print_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->sheet)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
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

	if (widget_can_paging(widget))
	{
		parse_xcolor(&xc, xp.color);
		lighten_xcolor(&xc, DEF_HARD_DARKEN);

		draw_corner(canv, &xc, (const xrect_t*)&cb);
	}

	if (ptd->sheet)
	{
		if (is_form_doc(ptd->sheet))
		{
			draw_form_page(pif, &cb, ptd->sheet, ptd->page);
		}
		else if (is_grid_doc(ptd->sheet))
		{
			draw_grid_page(pif, &cb, ptd->sheet, ptd->page);
		}
		else if (is_statis_doc(ptd->sheet))
		{
			draw_statis_page(pif, &cb, ptd->sheet, ptd->page);
		}
		else if (is_rich_doc(ptd->sheet))
		{
			xr.fx = cb.fx;
			xr.fy = cb.fy;
			xr.fw = cb.fw;
			xr.fh = cb.fh;

			draw_rich_text(pif->canvas, &xf, &xa, &xr, ptd->sheet, ptd->page);
		}
		else if (is_memo_doc(ptd->sheet))
		{
			xr.fx = cb.fx;
			xr.fy = cb.fy;
			xr.fw = cb.fw;
			xr.fh = cb.fh;

			draw_memo_text(pif->canvas, &xf, &xa, &xr, ptd->sheet, ptd->page);
		}
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*****************************************************************************************************/

res_win_t printbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_print_create)
		EVENT_ON_DESTROY(hand_print_destroy)

		EVENT_ON_PAINT(hand_print_paint)

		EVENT_ON_SIZE(hand_print_size)

		EVENT_ON_SCROLL(hand_print_scroll)
		EVENT_ON_WHEEL(hand_print_wheel)

		EVENT_ON_LBUTTON_DBCLICK(hand_print_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_print_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_print_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_print_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_print_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void printbox_set_data(res_win_t widget, link_t_ptr ptr)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->sheet = ptr;

	ptd->page = 1;

	printbox_redraw(widget);
}

link_t_ptr printbox_get_data(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->sheet;
}

void printbox_redraw(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	int pages;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return;

	pages = _printbox_calc_pages(widget);
	if (ptd->page > pages)
		ptd->page = pages;

	_printbox_reset_page(widget);

	widget_update(widget);
}

void printbox_move_prev_page(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return;

	nCurPage = ptd->page;

	if (nCurPage > 1)
	{
		nCurPage--;
		ptd->page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void printbox_move_next_page(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	int nCurPage, nMaxPage;
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return;

	widget_get_canv_size(widget, &xs);

	nCurPage = ptd->page;
	nMaxPage = _printbox_calc_pages(widget);

	if (nCurPage < nMaxPage)
	{
		nCurPage++;
		ptd->page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void printbox_move_first_page(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return;

	nCurPage = ptd->page;

	if (nCurPage != 1)
	{
		nCurPage = 1;
		ptd->page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void printbox_move_last_page(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	int nCurPage, nMaxPage;
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return;

	widget_get_canv_size(widget, &xs);

	nCurPage = ptd->page;
	nMaxPage = _printbox_calc_pages(widget);

	if (nCurPage != nMaxPage)
	{
		nCurPage = nMaxPage;
		ptd->page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void printbox_move_to_page(res_win_t widget, int page)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	int nCurPage, nMaxPage;
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return;

	widget_get_canv_size(widget, &xs);

	nCurPage = ptd->page;
	nMaxPage = _printbox_calc_pages(widget);

	if (page > 0 && page != nCurPage && page <= nMaxPage)
	{
		nCurPage = page;
		ptd->page = nCurPage;

		widget_erase(widget, NULL);
	}
}

int printbox_get_max_page(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return 0;

	widget_get_canv_size(widget, &xs);

	return _printbox_calc_pages(widget);
}

int printbox_get_page(res_win_t widget)
{
	print_delta_t* ptd = GETPRINTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->sheet)
		return 0;

	return ptd->page;
}


