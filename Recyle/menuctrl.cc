/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc menu control document

	@module	menuctrl.c | menu control implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcgdi.h"
#include "xdccanv.h"
#include "xdcwidg.h"

typedef struct _menu_delta_t{
	LINKPTR menu;
	LINKPTR item;
}menu_delta_t;

#define GETMENUDELTA(widget) 	((menu_delta_t*)widget_get_user_delta(widget))
#define SETMENUDELTA(widget,ptd) widget_set_user_delta(widget,ptd)

/**********************************************************************************************/
static void _menuctrl_item_rect(res_win_t widget, LINKPTR ilk, xrect_t* pxr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_menu_item_rect(ptd->menu, &cb, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _menuctrl_reset_page(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = calc_menu_width(ptd->menu);
	xs.fy = calc_menu_height(ptd->menu);

	widget_size_to_pt(widget, &xs);

	if (compare_text(get_menu_layer_ptr(ptd->menu), -1, ATTR_LAYER_HORZ, -1, 0) == 0)
	{
		fw = xs.cx;
		fh = ph;
	}
	else
	{
		fw = pw;
		fh = xs.cy;
	}

	xs.fx = get_menu_icon_span(ptd->menu);
	xs.fy = get_menu_icon_span(ptd->menu);
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);
}

/************************************************************************************************/
int noti_menu_owner(res_win_t widget, unsigned long code, LINKPTR menu, LINKPTR ilk, void* data)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	NOTICE_MENU nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;
	nf.menu = menu;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_menu_item_changing(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr;

	XDC_ASSERT(ptd->item);

	_menuctrl_item_rect(widget, ptd->item, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->item = NULL;
	widget_invalid(widget, &xr, 0);
}

void noti_menu_item_changed(res_win_t widget, LINKPTR plk)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr;

	XDC_ASSERT(!ptd->item);

	ptd->item = plk;

	_menuctrl_item_rect(widget, ptd->item, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_invalid(widget, &xr, 0);
}

/**************************************************************************************/
int hand_menu_create(res_win_t widget, void* data)
{
	menu_delta_t* ptd;
	res_dc_t dc;
	XHANDLE canv;

	ptd = (menu_delta_t*)xmem_alloc(sizeof(menu_delta_t));
	xmem_zero((void*)ptd, sizeof(menu_delta_t));

	dc = widget_client_dc(widget);
	canv = create_display_canvas(dc);
	widget_release_dc(widget, dc);

	widget_attach_canvas(widget, canv);

	SETMENUDELTA(widget, ptd);

	return 0;
}

void hand_menu_destroy(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	destroy_display_canvas(widget_detach_canvas(widget));

	SETMENUDELTA(widget, 0);

	xmem_free(ptd);
}

void hand_menu_size(res_win_t widget, int code, const xsize_t* prs)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

	menuctrl_redraw(widget);
}

void hand_menu_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	int nHint;
	LINKPTR plk = NULL;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->menu)
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	nHint = calc_menu_hint(ptd->menu, &cb, &pt, &plk);

	bRe = (plk == ptd->item) ? 1 : 0;
	if (bRe)
		return;

	if (ptd->item && !bRe)
	{
		noti_menu_item_changing(widget);
	}

	if (plk && !bRe)
	{
		noti_menu_item_changed(widget, plk);
	}
}

void hand_menu_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 1);
}

void hand_menu_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr = { 0 };

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 0);

	if (ptd->item)
	{
		_menuctrl_item_rect(widget, ptd->item, &xr);
	}
	else
	{
		xr.x = pxp->x;
		xr.y = pxp->y;
	}

	widget_client_to_screen(widget, RECTPOINT(&xr));
	noti_menu_owner(widget, NC_MENULBCLK, ptd->menu, ptd->item, (void*)&xr);
}

void hand_menu_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 1);
}

void hand_menu_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr = { 0 };

	if (!ptd->menu)
		return;

	widget_set_capture(widget, 0);

	if (ptd->item)
	{
		_menuctrl_item_rect(widget, ptd->item, &xr);
	}
	else
	{
		xr.x = pxp->x;
		xr.y = pxp->y;
	}

	widget_client_to_screen(widget, RECTPOINT(&xr));
	noti_menu_owner(widget, NC_MENURBCLK, ptd->menu, ptd->item, (void*)&xr);
}

void hand_menu_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	if (!ptd->menu)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_menu_command_color(res_win_t widget, void* data)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	widget_set_color_mode(widget, (clr_mod_t*)data);

	widget_update_window(widget);
	widget_update_client(widget);
}

void hand_menu_command_switch(res_win_t widget, bool_t bkey)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	dword_t ws;

	ws = widget_get_style(widget);

	if (bkey)
		ws |= (WD_STYLE_TOUCH);
	else
		ws &= ~(WD_STYLE_TOUCH);

	widget_set_style(widget, ws);

	widget_update_window(widget);
}

void hand_menu_erase(res_win_t widget, res_dc_t dc)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	
	if (!ptd->menu)
		return;
}

void hand_menu_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xrect_t xr = { 0 };
	xcolor_t xc = { 0 };
	res_dc_t rdc;

	XHANDLE canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->menu)
		return;

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

	rdc = begin_display_paint(pif->canvas, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_menu(pif, &cb, ptd->menu);

	//draw focus
	if (ptd->item)
	{
		_menuctrl_item_rect(widget, ptd->item, &xr);
		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_FOCUS_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	end_display_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*******************************************************************************************************/

res_win_t menuctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_menu_create;
	ev.pf_on_destroy = hand_menu_destroy;

	ev.pf_on_erase = hand_menu_erase;
	ev.pf_on_paint = hand_menu_paint;

	ev.pf_on_size = hand_menu_size;

	ev.pf_on_mouse_move = hand_menu_mouse_move;
	ev.pf_on_lbutton_down = hand_menu_lbutton_down;
	ev.pf_on_lbutton_up = hand_menu_lbutton_up;
	ev.pf_on_rbutton_down = hand_menu_rbutton_down;
	ev.pf_on_rbutton_up = hand_menu_rbutton_up;
	ev.pf_on_scroll = hand_menu_scroll;

	ev.pf_on_command_color = hand_menu_command_color;
	ev.pf_on_command_switch = hand_menu_command_switch;

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void menuctrl_attach(res_win_t widget, LINKPTR ptr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	XDC_ASSERT(ptr && is_menu_doc(ptr));

	ptd->menu = ptr;
	ptd->item = NULL;

	menuctrl_redraw(widget);
}

LINKPTR menuctrl_detach(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	LINKPTR data;

	XDC_ASSERT(ptd != NULL);

	if (ptd->menu)
	{
		data = ptd->menu;
		ptd->menu = NULL;
		ptd->item = NULL;
	}
	else
	{
		data = NULL;
	}
	return data;
}

LINKPTR menuctrl_fetch(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	return ptd->menu;
}

void menuctrl_redraw(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	LINKPTR ilk;
	bool_t b_valid;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	b_valid = 0;
	ilk = get_menu_next_item(ptd->menu, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		noti_menu_owner(widget, NC_MENUITEMCALCED, ptd->menu, ilk, NULL);

		ilk = get_menu_next_item(ptd->menu, ilk);
	}

	noti_menu_owner(widget, NC_MENUCALCED, ptd->menu, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}

	_menuctrl_reset_page(widget);

	widget_update_window(widget);

	widget_invalid(widget, NULL, 0);
}

void menuctrl_redraw_item(res_win_t widget, LINKPTR plk)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

#ifdef _DEBUG
	XDC_ASSERT(is_menu_item(ptd->menu, plk));
#endif

	noti_menu_owner(widget, NC_MENUITEMCALCED, ptd->menu, plk, NULL);

	_menuctrl_item_rect(widget, plk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_invalid(widget, &xr, 0);
}

void menuctrl_tabskip(res_win_t widget, int nSkip)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	LINKPTR plk = ptd->item;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	switch (nSkip)
	{
	case NF_RIGHT:
	case NF_DOWN:
		if (plk == NULL)
			plk = get_menu_next_item(ptd->menu, LINK_FIRST);
		else
			plk = get_menu_next_item(ptd->menu, ptd->item);

		if (plk)
			menuctrl_set_focus_item(widget, plk);
		break;
	case NF_LEFT:
	case NF_UP:
		if (plk == NULL)
			plk = get_menu_prev_item(ptd->menu, LINK_LAST);
		else
			plk = get_menu_prev_item(ptd->menu, ptd->item);

		if (plk)
			menuctrl_set_focus_item(widget, plk);
		break;
	case NF_HOME:
		plk = get_menu_next_item(ptd->menu, LINK_FIRST);

		if (plk)
			menuctrl_set_focus_item(widget, plk);
		break;
	case NF_END:
		plk = get_menu_prev_item(ptd->menu, LINK_LAST);

		if (plk)
			menuctrl_set_focus_item(widget, plk);
		break;
	}
}

void menuctrl_set_focus_item(res_win_t widget, LINKPTR ilk)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	bool_t bRe;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	if (ilk)
	{
#ifdef _DEBUG
		XDC_ASSERT(is_menu_item(ptd->menu, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return;

	if (ptd->item && !bRe)
	{
		noti_menu_item_changing(widget);
	}

	if (ilk && !bRe)
	{
		noti_menu_item_changed(widget, ilk);
	}
}

LINKPTR menuctrl_get_focus_item(res_win_t widget)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return NULL;

	return ptd->item;
}

void menuctrl_get_item_rect(res_win_t widget, LINKPTR ilk, xrect_t* pxr)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	
	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

#ifdef _DEBUG
	XDC_ASSERT(is_menu_item(ptd->menu, ilk));
#endif

	_menuctrl_item_rect(widget, ilk, pxr);
}

void menuctrl_popup_size(res_win_t widget, xsize_t* pxs)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	float ic;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	if (compare_text(get_menu_layer_ptr(ptd->menu), -1, ATTR_LAYER_HORZ, -1, 0) == 0)
	{
		pxs->fx = calc_menu_width(ptd->menu);
		pxs->fy = calc_menu_height(ptd->menu);
	}
	else
	{
		pxs->fx = calc_menu_width(ptd->menu);
		pxs->fy = calc_menu_height(ptd->menu);
		ic = get_menu_icon_span(ptd->menu);
		if (pxs->fy > ic * 7)
			pxs->fy = ic * 7;
	}

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void menuctrl_layout(res_win_t widget, const xpoint_t* ppt, int lay)
{
	menu_delta_t* ptd = GETMENUDELTA(widget);
	xrect_t xr = { 0 };
	xsize_t xs = { 0 };

	XDC_ASSERT(ptd != NULL);

	if (!ptd->menu)
		return;

	if (widget_is_child(widget))
		return;

	menuctrl_popup_size(widget,&xs);

	switch (lay)
	{
	case WIDGET_LAYOUT_LEFTBOTTOM:
		xr.x = ppt->x - xs.cx;
		xr.y = ppt->y;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	case WIDGET_LAYOUT_RIGHTBOTTOM:
		xr.x = ppt->x;
		xr.y = ppt->y;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	case WIDGET_LAYOUT_LEFTTOP:
		xr.x = ppt->x - xs.cx;
		xr.y = ppt->y - xs.cy;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	case WIDGET_LAYOUT_RIGHTTOP:
		xr.x = ppt->x;
		xr.y = ppt->y - xs.cy;
		xr.w = xs.cx;
		xr.h = xs.cy;
		break;
	}
	
	widget_size(widget, RECTSIZE(&xr));
	widget_move(widget, RECTPOINT(&xr));
	widget_show(widget, WD_SHOW_NORMAL);
}
