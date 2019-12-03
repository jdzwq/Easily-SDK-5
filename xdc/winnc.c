/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc windows nc document

	@module	winwnc.c | implement file

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

#include "winnc.h"
#include "handler.h"

static void _WidgetDrawEdge(res_win_t wt, res_ctx_t dc)
{
	border_t bd = { 0 };
	dword_t ws;
	xbrush_t xb;
	xrect_t rtWnd, rtScr;

	ws = widget_get_style(wt);
	widget_calc_border(ws, &bd);

	if (!bd.edge)
		return;

	widget_get_client_rect(wt, &rtScr);
	widget_client_to_window(wt, RECTPOINT(&rtScr));

	exclip_rect_raw(dc, &rtScr);

	widget_get_window_rect(wt, &rtWnd);
	rtWnd.x = rtWnd.y = 0;

	widget_get_xbrush(wt, &xb);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	draw_rect_raw(dc, NULL, &xb, &rtWnd);
}

static void _WidgetDrawHScroll(res_win_t wt, res_ctx_t dc)
{
	border_t bd = { 0 };
	xrect_t rtWnd, rtScr;
	scroll_t sl = { 0 };
	long ind;

	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };

	widget_calc_border(widget_get_style(wt), &bd);

	if (!bd.hscroll)
		return;

	widget_get_scroll(wt, 1, &sl);

	widget_get_window_rect(wt, &rtWnd);
	rtWnd.x = rtWnd.y = 0;

	widget_get_xbrush(wt, &xb);
	widget_get_xpen(wt, &xp);

	rtScr.x = rtWnd.x + bd.edge;
	rtScr.w = rtWnd.w - 2 * bd.edge - bd.vscroll;
	rtScr.y = rtWnd.y + rtWnd.h - bd.edge - bd.hscroll;
	rtScr.h = bd.hscroll;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	draw_rect_raw(dc, NULL, &xb, &rtScr);

	if (sl.max + sl.page / 2 <= sl.min)
		return;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	xscpy(xp.color, xb.color);

	rtScr.x = rtWnd.x + bd.edge;
	rtScr.w = bd.hscroll;
	rtScr.y = rtWnd.y + rtWnd.h - bd.edge - bd.hscroll;
	rtScr.h = bd.hscroll;
	pt_expand_rect(&rtScr, -3, -3);

	draw_round_raw(dc, &xp, &xb, &rtScr);

	rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll - bd.hscroll;
	rtScr.w = bd.hscroll;
	rtScr.y = rtWnd.y + rtWnd.h - bd.edge - bd.hscroll;
	rtScr.h = bd.hscroll;
	pt_expand_rect(&rtScr, -3, -3);

	draw_round_raw(dc, &xp, &xb, &rtScr);

	if (!sl.pos)
	{
		rtScr.x = rtWnd.x + bd.edge;
		rtScr.w = bd.hscroll;
		rtScr.y = rtWnd.y + rtWnd.h - bd.edge - bd.hscroll;
		rtScr.h = bd.hscroll;
	}
	else if (sl.pos == sl.max)
	{
		rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll - bd.hscroll;
		rtScr.w = bd.hscroll;
		rtScr.y = rtWnd.y + rtWnd.h - bd.edge - bd.hscroll;
		rtScr.h = bd.hscroll;
	}
	else
	{
		ind = (long)((float)sl.pos / (float)sl.max * (float)(rtWnd.w - 2 * bd.edge - bd.vscroll - bd.hscroll));
		rtScr.x = rtWnd.x + bd.edge + ind;
		rtScr.w = bd.hscroll;
		rtScr.y = rtWnd.y + rtWnd.h - bd.edge - bd.hscroll;
		rtScr.h = bd.hscroll;
	}
	pt_expand_rect(&rtScr, -4, -4);

	lighten_xpen(&xp, DEF_SOFT_LIGHTEN);
	lighten_xbrush(&xb, DEF_SOFT_LIGHTEN);
	draw_ellipse_raw(dc, &xp, &xb, &rtScr);
}

static void _WidgetDrawVScroll(res_win_t wt, res_ctx_t dc)
{
	border_t bd = { 0 };
	xrect_t rtWnd, rtScr;
	scroll_t sl = { 0 };
	long ind;

	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };

	widget_calc_border(widget_get_style(wt), &bd);

	if (!bd.vscroll)
		return;

	widget_get_scroll(wt, 0, &sl);

	widget_get_window_rect(wt, &rtWnd);
	rtWnd.x = rtWnd.y = 0;

	widget_get_xbrush(wt, &xb);
	widget_get_xpen(wt, &xp);

	rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll;
	rtScr.w = bd.vscroll;
	rtScr.y = rtWnd.y + bd.edge + bd.title + bd.menu;
	rtScr.h = rtWnd.h - bd.title - bd.menu - 2 * bd.edge;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	draw_rect_raw(dc, NULL, &xb, &rtScr);

	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	xscpy(xp.color, xb.color);

	//up page button
	rtScr.x = rtWnd.x + rtWnd.w - bd.vscroll;
	rtScr.y = rtWnd.y + bd.edge + bd.title + bd.menu;
	rtScr.w = bd.vscroll;
	rtScr.h = bd.vscroll;
	pt_expand_rect(&rtScr, -4, -6);

	draw_shape_raw(dc, &xp, &xb, &rtScr, ATTR_SHAPE_TOPTRIANGLE);

	//down page button
	rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll;
	rtScr.y = rtWnd.y + rtWnd.h - bd.edge - bd.vscroll;
	rtScr.w = bd.vscroll;
	rtScr.h = bd.vscroll;
	pt_expand_rect(&rtScr, -4, -6);

	draw_shape_raw(dc, &xp, &xb, &rtScr, ATTR_SHAPE_BOTTOMTRIANGLE);

	if (sl.max + sl.page / 2 <= sl.min)
		return;

	//up line button
	rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll;
	rtScr.y = rtWnd.y + bd.edge + bd.title + bd.menu + bd.vscroll;
	rtScr.w = bd.vscroll;
	rtScr.h = bd.vscroll;
	pt_expand_rect(&rtScr, -3, -3);

	draw_round_raw(dc, &xp, &xb, &rtScr);

	//down line button
	rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll;
	rtScr.y = rtWnd.y + rtWnd.h - bd.edge - 2 * bd.vscroll;
	rtScr.w = bd.vscroll;
	rtScr.h = bd.vscroll;
	pt_expand_rect(&rtScr, -3, -3);

	draw_round_raw(dc, &xp, &xb, &rtScr);

	if (!sl.pos)
	{
		rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll;
		rtScr.y = rtWnd.y + bd.edge + bd.title + bd.menu + bd.vscroll;
		rtScr.w = bd.vscroll;
		rtScr.h = bd.vscroll;
	}
	else if (sl.pos == sl.max)
	{
		rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll;
		rtScr.y = rtWnd.y + rtWnd.h - bd.edge - 2 * bd.vscroll;
		rtScr.w = bd.vscroll;
		rtScr.h = bd.vscroll;
	}
	else
	{
		ind = (long)((float)sl.pos / (float)sl.max * (float)(rtWnd.h - bd.title - bd.menu - 2 * bd.edge - 3 * bd.vscroll));

		rtScr.x = rtWnd.x + rtWnd.w - bd.edge - bd.vscroll;
		rtScr.y = rtWnd.y + bd.edge + bd.title + bd.menu + bd.vscroll + ind;
		rtScr.w = bd.vscroll;
		rtScr.h = bd.vscroll;
	}

	pt_expand_rect(&rtScr, -4, -4);

	lighten_xpen(&xp, DEF_SOFT_LIGHTEN);
	lighten_xbrush(&xb, DEF_SOFT_LIGHTEN);
	draw_ellipse_raw(dc, &xp, &xb, &rtScr);
}

static void _WidgetDrawTitleBar(res_win_t wt, res_ctx_t dc)
{
	long edge, title, hscr, vscr, menu, icon;
	border_t bd = { 0 };
	xrect_t rtWnd, rtScr;
	xpoint_t pt1, pt2;
	dword_t ws;

	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xgradi_t xg = { 0 };
	xcolor_t xc_near, xc_center;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	tchar_t txt[RES_LEN + 1] = { 0 };
	int len;

	xbrush_t xb_shadow = { 0 };
	xpen_t xp_shadow = { 0 };
	tchar_t aa[8] = { 0 };
	xpoint_t pa[12] = { 0 };

	int i = 0;
	int n = 0;
	int feed = 5;

	ws = widget_get_style(wt);

	widget_calc_border(ws, &bd);

	edge = bd.edge;
	title = bd.title;
	hscr = bd.hscroll;
	vscr = bd.vscroll;
	menu = bd.menu;
	icon = bd.icon;

	if (!title)
		return;

	widget_get_window_rect(wt, &rtWnd);
	rtWnd.x = rtWnd.y = 0;

	rtScr.x = rtWnd.x + edge;
	rtScr.y = rtWnd.y + edge;
	rtScr.w = rtWnd.w - 2 * edge;
	rtScr.h = title;

	widget_get_xbrush(wt, &xb);
	widget_get_xpen(wt, &xp);
	widget_get_xfont(wt, &xf);
	widget_get_xface(wt, &xa);

	/*lighten_xbrush(&xb, DEF_SOFT_DARKEN);
	parse_xcolor(&xc_near, xb.color);
	parse_xcolor(&xc_center, xb.color);
	lighten_xcolor(&xc_near, DEF_HARD_DARKEN);
	default_xgradi(&xg);
	format_xcolor(&xc_near, xg.brim_color);
	format_xcolor(&xc_center, xg.core_color);
	xscpy(xg.type, GDI_ATTR_GRADIENT_TYPE_VERT);*/

	//gradient_rect_raw(dc, &xg, &rtScr);

	rtScr.x = rtWnd.x + edge;
	rtScr.y = rtWnd.y + edge;
	rtScr.w = rtWnd.w - 2 * edge;
	rtScr.h = title - feed;

	aa[i] = _T('M');
	pa[n].x = rtScr.x;
	pa[n].y = rtScr.y;
	i++;
	n++;

	aa[i] = _T('L');
	pa[n].x = rtScr.x + rtScr.w;
	pa[n].y = rtScr.y;
	i++;
	n++;

	aa[i] = _T('L');
	pa[n].x = rtScr.x + rtScr.w;
	pa[n].y = rtScr.y + rtScr.h - 2 * feed;
	i++;
	n++;

	aa[i] = _T('A');
	pa[n].x = feed;
	pa[n].y = feed;
	pa[n + 1].x = rtScr.x + rtScr.w - feed;
	pa[n + 1].y = rtScr.y + rtScr.h - feed;
	i++;
	n += 2;

	aa[i] = _T('C');
	pa[n].x = rtScr.x + rtScr.w / 8 * 7;
	pa[n].y = rtScr.y + rtScr.h - 2 * feed;
	pa[n + 1].x = rtScr.x + rtScr.w / 4 * 3;
	pa[n + 1].y = rtScr.y + rtScr.h - feed;
	pa[n + 2].x = rtScr.x + rtScr.w / 2;
	pa[n + 2].y = rtScr.y + rtScr.h;
	i++;
	n += 3;

	aa[i] = _T('S');
	pa[n].x = rtScr.x + rtScr.w / 4;
	pa[n].y = rtScr.y + rtScr.h;
	pa[n + 1].x = rtScr.x + feed;
	pa[n + 1].y = rtScr.y + rtScr.h;
	i++;
	n += 2;

	aa[i] = _T('A');
	pa[n].x = feed;
	pa[n].y = feed;
	pa[n + 1].x = rtScr.x;
	pa[n + 1].y = rtScr.y + rtScr.h - feed;
	i++;
	n += 2;

	aa[i] = _T('Z');
	i++;

	xmem_copy((void*)&xb_shadow, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_shadow, DEF_SOFT_DARKEN);
	xmem_copy((void*)&xp_shadow, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_shadow.color, xb_shadow.color);

	//xb_shadow.shadow.offx = feed;
	//xb_shadow.shadow.offy = feed;
	xp_shadow.adorn.feed = 2;
	xp_shadow.adorn.size = 2;

	draw_path_raw(dc, &xp_shadow, &xb_shadow, aa, pa, n);

	rtScr.x = rtWnd.x + edge;
	rtScr.y = rtWnd.y + edge;
	rtScr.w = title;
	rtScr.h = title;

	pt_center_rect(&rtScr, 16, 16);
	parse_xcolor(&xc_center, xp.color);
	draw_icon_raw(dc, &xc_center, &rtScr, ICON_LOGO);

	/*caption*/
	len = widget_get_title(wt, txt, RES_LEN);
	if (len)
	{
		rtScr.x = rtWnd.x + edge + title;
		rtScr.w = rtWnd.w - 2 * edge - 2 * title;
		rtScr.y = rtWnd.y + edge;
		rtScr.h = title;

		draw_text_raw(dc, &xf, &xa, &rtScr, txt, len);
	}

	if (ws & WD_STYLE_SIZEBOX)
	{
		xscpy(xp.size, _T("2"));

		/*mini box*/
		rtScr.x = rtWnd.x + rtWnd.w - edge - (title / 2 * 3);
		rtScr.w = title / 2;
		rtScr.y = rtWnd.y + edge;
		rtScr.h = title * 2 / 3;
		pt_center_rect(&rtScr, icon, icon);

		rtScr.y += rtScr.h / 2;
		rtScr.h /= 2;
		draw_rect_raw(dc, &xp, &xb, &rtScr);

		/*zoom box*/
		rtScr.x = rtWnd.x + rtWnd.w - edge - title;
		rtScr.w = title / 2;
		rtScr.y = rtWnd.y + edge;
		rtScr.h = title * 2 / 3;
		pt_center_rect(&rtScr, icon, icon);

		draw_round_raw(dc, &xp, &xb, &rtScr);

		if (widget_is_maximized(wt))
		{
			pt_expand_rect(&rtScr, -3, -3);
			draw_rect_raw(dc, &xp, &xb, &rtScr);
		}
	}

	if (ws & WD_STYLE_CLOSEBOX)
	{
		rtScr.x = rtWnd.x + rtWnd.w - edge - (title / 2);
		rtScr.w = title / 2;
		rtScr.y = rtWnd.y + edge;
		rtScr.h = title * 2 / 3;
		pt_center_rect(&rtScr, icon, icon);

		xscpy(xp.size, _T("2"));

		pt1.x = rtScr.x;
		pt1.y = rtScr.y;
		pt2.x = rtScr.x + rtScr.w;
		pt2.y = rtScr.y + rtScr.h;
		draw_line_raw(dc, &xp, &pt1, &pt2);
		
		pt1.x = rtScr.x;
		pt1.y = rtScr.y + rtScr.h;
		pt2.x = rtScr.x + rtScr.w;
		pt2.y = rtScr.y;
		draw_line_raw(dc, &xp, &pt1, &pt2);
	}
	
}

static void _WidgetDrawMenuBar(res_win_t wt, res_ctx_t dc)
{
	long edge, title, vscr, hscr, menu;
	dword_t ws;
	border_t bd = { 0 };
	scroll_t sc = { 0 };
	xrect_t rtWnd, rtMenu, rtImage;
	xsize_t xs = { 0 };

	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };

	link_t_ptr ptr, ilk;
	const tchar_t* text;

	ws = widget_get_style(wt);
	widget_calc_border(ws, &bd);

	edge = bd.edge;
	title = bd.title;
	hscr = bd.hscroll;
	vscr = bd.vscroll;
	menu = bd.menu;

	if (!menu)
		return;

	widget_get_window_rect(wt, &rtWnd);
	rtWnd.x = rtWnd.y = 0;

	widget_get_xbrush(wt, &xb);
	lighten_xbrush(&xb, DEF_SOFT_DARKEN);
	widget_get_xpen(wt, &xp);
	widget_get_xfont(wt, &xf);
	widget_get_xface(wt, &xa);
	widget_get_iconic(wt, &xc);

	if (!is_null(xf.size))
	{
		ltoxs(xstol(xf.size) - 1, xf.size, INT_LEN);
	}

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	rtMenu.x = rtWnd.x + edge;
	rtMenu.w = rtWnd.w - 2 * edge;
	rtMenu.y = rtWnd.y + edge + title;
	rtMenu.h = menu;

	draw_rect_raw(dc, NULL, &xb, &rtMenu);

	ptr = widget_get_menu(wt);

	rtImage.x = rtWnd.x + edge;
	rtImage.w = 0;
	rtImage.y = rtWnd.y + edge + title;
	rtImage.h = menu;

	ilk = (ptr)? get_menu_next_item(ptr, LINK_FIRST) : NULL;
	while (ilk)
	{
		rtImage.x += rtImage.w;
		rtImage.w = menu;

		pt_center_rect(&rtImage, 16, 16);

		draw_icon_raw(dc, &xc, &rtImage, get_menu_item_icon_ptr(ilk));
		
		text = get_menu_item_title_ptr(ilk);
		if (!is_null(text))
		{
			text_size_raw(dc, &xf, text, -1, &xs);

			rtImage.x += menu;
			rtImage.w = xs.cx;

			draw_text_raw(dc, &xf, &xa, &rtImage, text, -1);
		}

		ilk = get_menu_next_item(ptr, ilk);
	}
}
/**************************************************************************************************/

void widget_nc_on_paint(res_win_t wt, res_ctx_t dc)
{
	border_t bd = { 0 };

	widget_calc_border(widget_get_style(wt), &bd);

	if (bd.edge)
	{
		_WidgetDrawEdge(wt, dc);
	}

	if (bd.edge)
	{
		_WidgetDrawEdge(wt, dc);
	}

	if (bd.title)
	{
		_WidgetDrawTitleBar(wt, dc);
	}

	if (bd.menu)
	{
		_WidgetDrawMenuBar(wt, dc);
	}

	if (bd.hscroll)
	{
		_WidgetDrawHScroll(wt, dc);
	}

	if (bd.vscroll)
	{
		_WidgetDrawVScroll(wt, dc);
	}
}

void widget_nc_on_calcsize(res_win_t wt, xrect_t* pxr)
{
	border_t bd = { 0 };

	widget_calc_border(widget_get_style(wt), &bd);

	pxr->x += (bd.edge);
	pxr->w -= (2 * bd.edge + bd.vscroll);
	pxr->y += (bd.edge + bd.title + bd.menu);
	pxr->h -= (2 * bd.edge + bd.title + bd.menu + bd.hscroll);
}

int widget_nc_on_hittest(res_win_t wt, const xpoint_t* pxp)
{
	long edge, title, vscr, hscr, menu;
	border_t bd = { 0 };
	dword_t ws;
	xrect_t xr, rt;
	xpoint_t pt;

	ws = widget_get_style(wt);

	widget_calc_border(ws, &bd);

	edge = bd.edge;
	title = bd.title;
	hscr = bd.hscroll;
	vscr = bd.vscroll;
	menu = bd.menu;

	if (!edge && !title && !vscr && !hscr && !menu)
		return HINT_CLIENT;

	widget_get_window_rect(wt, &xr);

	pt.x = pxp->x;
	pt.y = pxp->y;

	if (menu)
	{
		rt.x = xr.x + edge;
		rt.w = xr.w - 2 * edge;
		rt.y = xr.y + edge + title;
		rt.h = menu;

		if (pt_in_rect(pxp, &rt))
			return HINT_MENUBAR;
	}

	if (hscr)
	{
		rt.x = xr.x + edge;
		rt.w = hscr;
		rt.y = xr.y + xr.h - edge - hscr;
		rt.h = hscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_LINELEFT;

		rt.x = xr.x + edge + hscr;
		rt.w = xr.w - 2 * edge - vscr - 2 * hscr;
		rt.y = xr.y + xr.h - edge - hscr;
		rt.h = hscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_HSCROLL;

		rt.x = xr.x + xr.w - edge - vscr - hscr;
		rt.w = hscr;
		rt.y = xr.y + xr.h - edge - hscr;
		rt.h = hscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_LINERIGHT;
	}

	if (vscr)
	{
		rt.x = xr.x + xr.w - edge - vscr;
		rt.w = vscr;
		rt.y = xr.y + edge + title + menu;
		rt.h = vscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_PAGEUP;

		rt.x = xr.x + xr.w - edge - vscr;
		rt.w = vscr;
		rt.y = xr.y + edge + title + menu + vscr;
		rt.h = vscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_LINEUP;

		rt.x = xr.x + xr.w - edge - vscr;
		rt.w = vscr;
		rt.y = xr.y + edge + title + menu + 2 * vscr;
		rt.h = xr.h - 2 * edge - title - menu - 4 * vscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_VSCROLL;

		rt.x = xr.x + xr.w - edge - vscr;
		rt.w = vscr;
		rt.y = xr.y + xr.h - edge - 2 * vscr;
		rt.h = vscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_LINEDOWN;

		rt.x = xr.x + xr.w - edge - vscr;
		rt.w = vscr;
		rt.y = xr.y + xr.h - edge - vscr;
		rt.h = vscr;

		if (pt_in_rect(pxp, &rt))
			return HINT_PAGEDOWN;
	}

	if (title)
	{
		rt.x = xr.x + edge;
		rt.w = title;
		rt.y = xr.y + edge;
		rt.h = title;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_ICON;
		}

		rt.x = xr.x + xr.w - edge - (title / 2 * 3);
		rt.w = title / 2;
		rt.y = xr.y + edge;
		rt.h = title;
		if (pt_in_rect(pxp, &rt))
		{
			if (ws & WD_STYLE_SIZEBOX)
				return HINT_MINIMIZE;
			else
				return HINT_TITLE;
		}

		rt.x = xr.x + xr.w - edge - title;
		rt.w = title / 2;
		rt.y = xr.y + edge;
		rt.h = title;
		if (pt_in_rect(pxp, &rt))
		{
			if (ws & WD_STYLE_SIZEBOX)
			{
				if (widget_is_maximized(wt))
					return HINT_RESTORE;
				else
					return HINT_MAXIMIZE;
			}
			else
				return HINT_TITLE;
		}

		rt.x = xr.x + xr.w - edge - (title / 2);
		rt.w = title / 2;
		rt.y = xr.y + edge;
		rt.h = title;
		if (pt_in_rect(pxp, &rt))
		{
			if (ws & WD_STYLE_CLOSEBOX)
				return HINT_CLOSE;
			else
				return HINT_TITLE;
		}

		rt.x = xr.x + edge;
		rt.w = xr.w - 2 * edge;
		rt.y = xr.y + edge;
		rt.h = title;

		if (pt_in_rect(pxp, &rt))
			return HINT_TITLE;
	}

	if (edge)
	{
		rt.x = xr.x;
		rt.w = edge;
		rt.y = xr.y;
		rt.h = edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_TOPLEFT;
		}

		rt.x = xr.x + edge;
		rt.w = xr.w - 2 * edge;
		rt.y = xr.y;
		rt.h = edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_TOP;
		}

		rt.x = xr.x + xr.w - edge;
		rt.w = edge;
		rt.y = xr.y;
		rt.h = edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_TOPRIGHT;
		}

		rt.x = xr.x;
		rt.w = edge;
		rt.y = xr.y + edge;
		rt.h = xr.h - 2 * edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_LEFT;
		}

		rt.x = xr.x + xr.w - edge;
		rt.w = edge;
		rt.y = xr.y + edge;
		rt.h = xr.h - 2 * edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_RIGHT;
		}

		rt.x = xr.x + xr.w - edge;
		rt.w = edge;
		rt.y = xr.y + xr.h - edge;
		rt.h = edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_RIGHTBOTTOM;
		}

		rt.x = xr.x + edge;
		rt.w = xr.w - 2 * edge;
		rt.y = xr.y + xr.h - edge;
		rt.h = edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_BOTTOM;
		}

		rt.x = xr.x;
		rt.w = edge;
		rt.y = xr.y + xr.h - edge;
		rt.h = edge;
		if (pt_in_rect(pxp, &rt))
		{
			return HINT_LEFTBOTTOM;
		}
	}

	rt.x = xr.x + edge;
	rt.w = xr.w - 2 * edge - vscr;
	rt.y = xr.y + edge + title + menu;
	rt.h = xr.h - 2 * edge - title - menu - hscr;
	if (pt_in_rect(pxp, &rt))
	{
		return HINT_CLIENT;
	}

	return HINT_NOWHERE;
}

long widget_nc_on_calcscroll(res_win_t wt, bool_t horz, const xpoint_t* pxp)
{
	long edge, title, hscr, vscr, menu;
	border_t bd = { 0 };
	long pos;
	xrect_t xr;
	scroll_t sc = { 0 };

	widget_calc_border(widget_get_style(wt), &bd);

	edge = bd.edge;
	title = bd.title;
	hscr = bd.hscroll;
	vscr = bd.vscroll;
	menu = bd.menu;

	if (horz && !hscr)
		return 0;
	else if (!horz && !vscr)
		return 0;

	widget_get_window_rect(wt, &xr);
	xr.x = xr.y = 0;

	if (horz)
	{
		widget_get_scroll(wt, 1, &sc);

		if (!sc.pos &&  pxp->x < edge + hscr)
		{
			return -sc.min;
		}
		else if (sc.pos == sc.max && pxp->x > xr.w - edge - vscr - hscr)
		{
			return sc.max + sc.min;
		}

		if (sc.max > 0)
		{
			pos = (long)((float)(pxp->x - edge) / (float)(xr.w - 2 * edge - hscr - vscr) * (float)sc.max);
			if (pos < 0)
				pos = 0;
			else if (pos > sc.max)
				pos = sc.max;
		}
		else
		{
			pos = 0;
		}
	}
	else
	{
		widget_get_scroll(wt, 0, &sc);

		if (!sc.pos && pxp->y < edge + title + menu + 2 * vscr)
		{
			return -sc.min;
		}
		else if (sc.pos == sc.max && pxp->y > xr.h - edge - 2 * vscr)
		{
			return sc.max + sc.min;
		}

		if (sc.max > 0)
		{
			pos = (long)((float)(pxp->y - edge - title - menu - vscr) / (float)(xr.h - 2 * edge - title - menu - 2 * vscr) * (float)sc.max);
			if (pos < 0)
				pos = 0;
			else if (pos > sc.max)
				pos = sc.max;
		}
		else
		{
			pos = 0;
		}
	}

	return pos;
}

void widget_redraw_scroll(res_win_t wt, bool_t horz)
{
	res_ctx_t dc;

	dc = widget_window_ctx(wt);

	if (horz)
		_WidgetDrawHScroll(wt, dc);
	else
		_WidgetDrawVScroll(wt, dc);

	widget_release_ctx(wt, dc);
}

