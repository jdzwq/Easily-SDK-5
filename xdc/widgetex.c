/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widgetex default handler document

	@module	widgetex.c | implement file

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

#include "widgetex.h"
#include "widgetnc.h"
#include "xdcimp.h"
#include "splitor.h"
#include "docker.h"

typedef struct _widget_exten_t{
	xrect_t vb;
	xrect_t cb;
	xsize_t sc;

	link_t_ptr menu;
	canvas_t canv;

	union{
		splitor_t splitor;
		docker_t docker;
	};

	if_drawing_t* pif;
}widget_exten_t;

#define GETEXTENSTRUCT(wt)			(widget_exten_t*)widget_get_core_delta(wt)
#define SETEXTENSTRUCT(wt, lp)		widget_set_core_delta(wt, (var_long)lp)


/***********************************************************************************************************************/

const if_drawing_t* widget_get_canvas_interface(res_win_t wt)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->pif)
	{
		widget_get_canv_rect(wt, (canvbox_t*)&(pwt->pif->rect));

		widget_get_color_mode(wt, &(pwt->pif->mode));
	}

	return pwt->pif;
}

canvas_t widget_get_canvas(res_win_t wt)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return pwt->canv;
}

void  widget_attach_menu(res_win_t wt, link_t_ptr menu)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	pwt->menu = menu;
}

link_t_ptr  widget_get_menu(res_win_t wt)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return pwt->menu;
}

link_t_ptr  widget_detach_menu(res_win_t wt)
{
	widget_exten_t* pwt;
	link_t_ptr menu;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	menu = pwt->menu;
	pwt->menu = NULL;

	return menu;
}

void  widget_menu_item_rect(res_win_t wt, int iid, xrect_t* pxr)
{
	border_t bd = { 0 };
	link_t_ptr plk, ilk;
	xrect_t xrItem;
	xsize_t xs;
	const tchar_t* text;
	visual_t rdc;
	xfont_t xf = { 0 };
	widget_exten_t* pwt;
	if_drawing_t ifv = {0};

	pxr->x = pxr->w = pxr->y = pxr->h = 0;

	pwt = GETEXTENSTRUCT(wt);

	plk = (pwt->menu) ? get_menu_item_by_iid(pwt->menu, iid) : NULL;
	if (!plk)
		return;

	widget_calc_border(widget_get_style(wt), &bd);

	if (!bd.menu)
		return;

	xrItem.x = bd.edge;
	xrItem.w = 0;
	xrItem.y = bd.edge + bd.title;
	xrItem.h = bd.menu;

	widget_get_xfont(wt, &xf);

	rdc = widget_window_ctx(wt);

	get_visual_interface(rdc, &ifv);

	ilk = get_menu_next_item(pwt->menu, LINK_FIRST);
	while (ilk)
	{
		text = get_menu_item_title_ptr(ilk);
		(*ifv.pf_text_size)(ifv.ctx, &xf, text, -1, &xs);

		xrItem.w = bd.menu + xs.w;

		if (ilk == plk)
		{
			widget_get_window_rect(wt, pxr);

			pxr->x += xrItem.x;
			pxr->y += xrItem.y;
			pxr->w = xrItem.w;
			pxr->h = xrItem.h;

			break;
		}

		ilk = get_menu_next_item(pwt->menu, ilk);
	}

	
	widget_release_ctx(wt, rdc);
}

void  widget_attach_splitor(res_win_t wt, link_t_ptr split)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	pwt->splitor.widget = wt;
	pwt->splitor.split = split;
}

link_t_ptr  widget_get_splitor(res_win_t wt)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return pwt->splitor.split;
}

link_t_ptr  widget_detach_splitor(res_win_t wt)
{
	widget_exten_t* pwt;
	link_t_ptr splitor;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	splitor = pwt->splitor.split;
	pwt->splitor.split = NULL;
	pwt->splitor.widget = NULL;

	return splitor;
}

void widget_layout_splitor(res_win_t wt)
{
	widget_exten_t* pwt;
	xrect_t xr;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		widget_get_client_rect(wt, &xr);
		hand_splitor_size(&pwt->splitor, &xr);
	}
}

bool_t widget_dock(res_win_t wt, dword_t style, int cx, int cy)
{
	widget_exten_t* pwt;
	int i;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	pwt->docker.widget = wt;

	for (i = 0; i < 4; i++)
	{
		if (!pwt->docker.dock[i].style)
			break;

		if ((pwt->docker.dock[i].style & 0x0000ffff) == (style & 0x0000ffff))
			return 0;
	}

	if (i == 4)
		return 0;

	pwt->docker.dock[i].style = style;
	pwt->docker.dock[i].cx = cx;
	pwt->docker.dock[i].cy = cy;

	return 1;
}

void widget_undock(res_win_t wt, dword_t style)
{
	widget_exten_t* pwt;
	int i;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	for (i = 0; i < 4; i++)
	{
		if ((pwt->docker.dock[i].style & 0x0000ffff) == (style & 0x0000ffff))
			break;
	}

	if (i == 4)
		return;

	for (; i < 3; i++)
	{
		pwt->docker.dock[i].style = pwt->docker.dock[i + 1].style;
		pwt->docker.dock[i].cx = pwt->docker.dock[i + 1].cx;
		pwt->docker.dock[i].cy = pwt->docker.dock[i + 1].cy;

		pwt->docker.dock[i + 1].style = 0;
		pwt->docker.dock[i + 1].cx = 0;
		pwt->docker.dock[i + 1].cy = 0;
	}
}

void widget_get_dock_rect(res_win_t wt, dword_t style, xrect_t* pxr)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	hand_docker_calc_rect(&pwt->docker, style, pxr);
}

void widget_calc_scroll(res_win_t wt, bool_t bHorz, scroll_t* psl)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	if (bHorz)
	{
		psl->max = (pwt->cb.w - pwt->vb.w);
		psl->min = pwt->sc.w;
		psl->page = 2 * pwt->sc.w;
		psl->pos = (pwt->vb.x);
	}
	else
	{
		psl->max = (pwt->cb.h - pwt->vb.h);
		psl->min = pwt->sc.h;
		psl->page = 2 * pwt->sc.h;
		psl->pos = (pwt->vb.y);
	}
}

void widget_reset_scroll(res_win_t wt, bool_t horz)
{
	widget_exten_t* pwt;
	scroll_t sc = { 0 };

	pwt = GETEXTENSTRUCT(wt);

	if (!pwt)
		return;

	if (horz)
	{
		sc.max = (pwt->cb.w - pwt->vb.w);
		sc.min = pwt->sc.w;
		sc.page = 2 * pwt->sc.w;
		sc.pos = pwt->vb.x;
	}
	else
	{
		sc.max = (pwt->cb.h - pwt->vb.h);
		sc.min = pwt->sc.h;
		sc.page = 2 * pwt->sc.h;
		sc.pos = pwt->vb.y;
	}

	widget_set_scroll_info(wt, horz, &sc);

#ifdef XDU_SUPPORT_WIDGET_NC
	if (widget_get_style(wt) & WD_STYLE_OWNERNC)
	{
		widget_draw_scroll(wt, horz);
	}
#endif
}

void widget_reset_paging(res_win_t wt, int ww, int wh, int vw, int vh, int lw, int lh)
{
	int cx, cy;
	float min_cx, min_cy;
	canvas_t canv;

	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	if (widget_can_paging(wt))
	{
		cx = tm_to_pt(canv, DEF_PAGE_FEED, 1) - tm_to_pt(canv, 0, 1);
		cy = tm_to_pt(canv, DEF_PAGE_FEED, 0) - tm_to_pt(canv, 0, 0);
	}
	else
	{
		cx = 0;
		cy = 0;
	}

	if (cx > 0)
	{
		cx = (cx > (ww - vw) / 2) ? cx : (ww - vw) / 2;
	}
	else
	{
		cx = 0;
	}

	if (cy > 0)
	{
		cy = (cy > (wh - vh) / 2) ? cy : (wh - vh) / 2;
	}
	else
	{
		cy = 0;
	}

	pwt->cb.x = cx;
	pwt->cb.y = cy;
	pwt->cb.w = vw + 2 * cx;
	pwt->cb.h = vh + 2 * cy;

	if (widget_can_paging(wt))
	{
		min_cx = pt_to_tm(canv, pwt->cb.x, 1) - pt_to_tm(canv, 0, 1);
		min_cy = pt_to_tm(canv, pwt->cb.y, 0) - pt_to_tm(canv, 0, 0);

		set_canvas_horz_feed(canv, min_cx);
		set_canvas_vert_feed(canv, min_cy);
	}

	pwt->vb.w = ww;
	pwt->vb.h = wh;

	cx = pwt->cb.w - pwt->vb.w;
	if (cx < 0)
		cx = 0;

	if (pwt->vb.x > cx)
		pwt->vb.x = cx;

	cy = pwt->cb.h - pwt->vb.h;
	if (cy < 0)
		cy = 0;

	if (pwt->vb.y > cy)
		pwt->vb.y = cy;

	pwt->sc.h = lh;
	pwt->sc.w = lw;
}

void widget_get_view_rect(res_win_t wt, viewbox_t* pbox)
{
	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	pbox->px = pwt->cb.x - pwt->vb.x;
	pbox->py = pwt->cb.y - pwt->vb.y;
	pbox->pw = pwt->cb.w - 2 * pwt->cb.x;
	pbox->ph = pwt->cb.h - 2 * pwt->cb.y;
}

void widget_get_canv_rect(res_win_t wt, canvbox_t* pbox)
{
	canvas_t canv;
	xrect_t xr;

	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	xr.x = pwt->cb.x - pwt->vb.x;
	xr.y = pwt->cb.y - pwt->vb.y;
	xr.w = pwt->cb.w - 2 * pwt->cb.x;
	xr.h = pwt->cb.h - 2 * pwt->cb.y;

	rect_pt_to_tm(canv, &xr);

	pbox->fx = xr.fx;
	pbox->fy = xr.fy;
	pbox->fw = xr.fw;
	pbox->fh = xr.fh;
}

void widget_get_canv_size(res_win_t wt, xsize_t* pps)
{
	canvas_t canv;

	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	pps->w = pwt->cb.w - 2 * pwt->cb.x;
	pps->h = pwt->cb.h - 2 * pwt->cb.y;

	size_pt_to_tm(canv, pps);
}

void widget_ensure_visible(res_win_t wt, const xrect_t* pxr, bool_t scroll)
{
	bool_t b_horz = 0;
	bool_t b_vert = 0;

	widget_exten_t* pwt;

	pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pxr->y < 0)
	{
		pwt->vb.y += pxr->y;
		b_vert = 1;
	}
	else if (pxr->y + pxr->h > pwt->vb.h)
	{
		if (pwt->vb.h < pxr->h)
			pwt->vb.y = pxr->y;
		else
			pwt->vb.y += (pxr->y + pxr->h - pwt->vb.h);

		b_vert = 1;
	}

	if (b_vert && scroll)
	{
		widget_reset_scroll(wt, 0);
	}

	if (pxr->x < 0)
	{
		pwt->vb.x += pxr->x;
		b_horz = 1;
	}
	else if (pxr->x + pxr->w > pwt->vb.w)
	{
		if (pwt->vb.w < pxr->w)
			pwt->vb.x = pxr->x;
		else
			pwt->vb.x += (pxr->x + pxr->w - pwt->vb.w);

		b_horz = 1;
	}

	if (b_horz && scroll)
	{
		widget_reset_scroll(wt, 1);
	}

	if (b_vert || b_horz)
		widget_erase(wt, NULL);
}

void widget_rect_to_pt(res_win_t wt, xrect_t* pxr)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widget_get_canv_rect(wt, &cb);

	ft_offset_rect(pxr, cb.fx, cb.fy);

	rect_tm_to_pt(canv, pxr);
}

void widget_rect_to_tm(res_win_t wt, xrect_t* pxr)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widget_get_canv_rect(wt, &cb);

	rect_pt_to_tm(canv, pxr);

	ft_offset_rect(pxr, -cb.fx, -cb.fy);
}

void widget_point_to_pt(res_win_t wt, xpoint_t* ppt)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widget_get_canv_rect(wt, &cb);

	ft_offset_point(ppt, cb.fx, cb.fy);

	point_tm_to_pt(canv, ppt);
}

void widget_point_to_tm(res_win_t wt, xpoint_t* ppt)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widget_get_canv_rect(wt, &cb);

	point_pt_to_tm(canv, ppt);

	ft_offset_point(ppt, -cb.fx, -cb.fy);
}

void widget_size_to_pt(res_win_t wt, xsize_t* pxs)
{
	canvas_t canv;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	size_tm_to_pt(canv, pxs);
}

void widget_size_to_tm(res_win_t wt, xsize_t* pxs)
{
	canvas_t canv;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	size_pt_to_tm(canv, pxs);
}

bool_t widget_point_corner(res_win_t wt, const xpoint_t* ppt)
{
	xrect_t rt;
	int corn = 10;

	widget_get_client_rect(wt, &rt);
	rt.x = rt.w - corn;
	rt.y = rt.h - corn;

	return pt_in_rect(ppt, &rt);
}

/************************************default widget handler**************************************************/

void widget_hand_create(res_win_t wt)
{
	widget_exten_t* pwt;
	visual_t rdc;

	pwt = (widget_exten_t*)xmem_alloc(sizeof(widget_exten_t));

	rdc = widget_client_ctx(wt);
	pwt->canv = create_display_canvas(rdc);
	widget_release_ctx(wt, rdc);

	pwt->pif = (if_drawing_t*)xmem_alloc(sizeof(if_drawing_t));
	get_canvas_interface(pwt->canv, pwt->pif);

	SETEXTENSTRUCT(wt, pwt);
}

void widget_hand_destroy(res_win_t wt)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	if (pwt)
	{
		destroy_display_canvas(pwt->canv);

		if (pwt->pif)
			xmem_free(pwt->pif);

		xmem_free(pwt);
	}

	SETEXTENSTRUCT(wt, 0);
}

bool_t widget_hand_scroll(res_win_t wt, bool_t bHorz, int nLine)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);
	int nCur, nMax;

	XDL_ASSERT(pwt != NULL);

	if (bHorz)
	{
		nMax = (pwt->cb.w - pwt->vb.w);
		if (nMax < 0)
			nMax = 0;

		if (nLine < 0 && pwt->vb.x == 0)
			return 0;

		if (nLine > 0 && pwt->vb.x == nMax)
			return 0;

		nCur = (pwt->vb.x + nLine);

		if (nCur > nMax)
			nCur = nMax;
		if (nCur < 0)
			nCur = 0;

		pwt->vb.x = nCur;
	}
	else
	{
		nMax = (pwt->cb.h - pwt->vb.h);
		if (nMax < 0)
			nMax = 0;

		if (nLine < 0 && pwt->vb.y == 0)
			return 0;

		if (nLine > 0 && pwt->vb.y == nMax)
			return 0;

		nCur = (pwt->vb.y + nLine);

		if (nCur > nMax)
			nCur = nMax;
		if (nCur < 0)
			nCur = 0;

		pwt->vb.y = nCur;
	}

	widget_erase(wt, NULL);

	widget_reset_scroll(wt, bHorz);

	return 1;
}

/************************************default widget splitting dispatch**************************************************/

void widget_splitor_on_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_mouse_move(&pwt->splitor, dw, pxp);
	}
}

void widget_splitor_on_lbuttondown(res_win_t wt, const xpoint_t* pxp)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_down(&pwt->splitor, pxp);
	}
}

void widget_splitor_on_lbuttonup(res_win_t wt, const xpoint_t* pxp)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_up(&pwt->splitor, pxp);
	}
}

void widget_splitor_on_size(res_win_t wt, int code, const xsize_t* pxs)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);
	xrect_t xr;

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		widget_get_client_rect(wt, &xr);
		hand_splitor_size(&pwt->splitor, &xr);
	}
}

void widget_splitor_on_paint(res_win_t wt, visual_t rdc, const xrect_t* prt)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_paint(&pwt->splitor, rdc);
	}
}

int widget_splitor_sub_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_mouse_move(&pwt->splitor, dw, pxp);
		return 1;
	}

	return 0;
}

int widget_splitor_sub_lbuttondown(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_down(&pwt->splitor, pxp);
		return 1;
	}

	return 0;
}

int widget_splitor_sub_lbuttonup(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_up(&pwt->splitor, pxp);

		return 1;
	}

	return 0;
}

int widget_splitor_sub_size(res_win_t wt, int code, const xsize_t* pxs, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);
	xrect_t xr;

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		widget_get_client_rect(wt, &xr);
		hand_splitor_size(&pwt->splitor, &xr);

		return 1;
	}

	return 0;
}

int widget_splitor_sub_paint(res_win_t wt, visual_t rdc, const xrect_t* prt, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_paint(&pwt->splitor, rdc);

		return 1;
	}

	return 0;
}

/************************************default widget docking dispatch**************************************************/

void widget_docker_on_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_mouse_move(&pwt->docker, dw, pxp);
	}
}

void widget_docker_on_lbuttondown(res_win_t wt, const xpoint_t* pxp)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_down(&pwt->docker, pxp);
	}
}

void widget_docker_on_lbuttonup(res_win_t wt, const xpoint_t* pxp)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_up(&pwt->docker, pxp);
	}
}

void widget_docker_on_paint(res_win_t wt, visual_t rdc, const xrect_t* prt)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_paint(&pwt->docker, rdc, prt);
	}
}

int widget_docker_sub_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_mouse_move(&pwt->docker, dw, pxp);
	}

	return 1;
}

int widget_docker_sub_lbuttondown(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_down(&pwt->docker, pxp);
	}
	return 1;
}

int widget_docker_sub_lbuttonup(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_up(&pwt->docker, pxp);
	}

	return 1;
}

int widget_docker_sub_paint(res_win_t wt, visual_t rdc, const xrect_t* prt, uid_t sid, var_long delta)
{
	widget_exten_t* pwt = GETEXTENSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_paint(&pwt->docker, rdc, prt);
	}

	return 1;
}


