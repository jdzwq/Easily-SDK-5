/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget default handler document

	@module	handler.c | implement file

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
#include "splitor.h"
#include "docker.h"

/***********************************************************************************************************************/

bool_t	widgetex_has_struct(res_win_t wt)
{
	widget_struct_t* pws;

	pws = GETWIDGSTRUCT(wt);

	return (pws != NULL) ? 1 : 0;
}

void  widgetex_set_xfont(res_win_t wt, const xfont_t* pxf)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)&pwt->xf, (void*)pxf, sizeof(xfont_t));
}

void  widgetex_get_xfont(res_win_t wt, xfont_t* pxf)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)pxf, (void*)&pwt->xf, sizeof(xfont_t));
}

const xfont_t*  widgetex_get_xfont_ptr(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return &pwt->xf;
}

void widgetex_set_xface(res_win_t wt, const xface_t* pxa)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)&pwt->xa, (void*)pxa, sizeof(xface_t));
}

void  widgetex_get_xface(res_win_t wt, xface_t* pxa)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)pxa, (void*)&pwt->xa, sizeof(xface_t));
}

const xface_t*  widgetex_get_xface_ptr(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return &pwt->xa;
}

void  widgetex_set_xbrush(res_win_t wt, const xbrush_t* pxb)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)&pwt->xb, (void*)pxb, sizeof(xbrush_t));
}

void  widgetex_get_xbrush(res_win_t wt, xbrush_t* pxb)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)pxb, (void*)&pwt->xb, sizeof(xbrush_t));
}

const xbrush_t*  widgetex_get_xbrush_ptr(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return &pwt->xb;
}

void  widgetex_set_xpen(res_win_t wt, const xpen_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)&pwt->xp, (void*)pxp, sizeof(xpen_t));
}

void  widgetex_get_xpen(res_win_t wt, xpen_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)pxp, (void*)&pwt->xp, sizeof(xpen_t));
}

const xpen_t*  widgetex_get_xpen_ptr(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return &pwt->xp;
}

void  widgetex_set_mask(res_win_t wt, const xcolor_t* pxc)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)&pwt->msk, (void*)pxc, sizeof(xcolor_t));
}

void  widgetex_get_mask(res_win_t wt, xcolor_t* pxc)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)pxc, (void*)&pwt->msk, sizeof(xcolor_t));
}

const xcolor_t*  widgetex_get_mask_ptr(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return &pwt->msk;
}

void  widgetex_set_iconic(res_win_t wt, const xcolor_t* pxc)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)&pwt->ico, (void*)pxc, sizeof(xcolor_t));
}

void  widgetex_get_iconic(res_win_t wt, xcolor_t* pxc)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)pxc, (void*)&pwt->ico, sizeof(xcolor_t));
}

const xcolor_t*  widgetex_get_iconic_ptr(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return &pwt->ico;
}

void  widgetex_set_point(res_win_t wt, const xpoint_t* ppt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)&pwt->pt, (void*)ppt, sizeof(xpoint_t));
}

void  widgetex_get_point(res_win_t wt, xpoint_t* ppt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	xmem_copy((void*)ppt, (void*)&pwt->pt, sizeof(xpoint_t));
}

canvas_t widget_get_canvas(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return pwt->canv;
}

void  widgetex_attach_menu(res_win_t wt, link_t_ptr menu)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	pwt->menu = menu;
}

link_t_ptr  widgetex_get_menu(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return pwt->menu;
}

link_t_ptr  widgetex_detach_menu(res_win_t wt)
{
	widget_struct_t* pwt;
	link_t_ptr menu;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	menu = pwt->menu;
	pwt->menu = NULL;

	return menu;
}

void  widgetex_menu_item_rect(res_win_t wt, int iid, xrect_t* pxr)
{
	border_t bd = { 0 };
	link_t_ptr plk, ilk;
	xrect_t xrItem;
	xsize_t xs;
	const tchar_t* text;
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	widget_struct_t* pwt;

	pxr->x = pxr->w = pxr->y = pxr->h = 0;

	pwt = GETWIDGSTRUCT(wt);

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

	widgetex_get_xfont(wt, &xf);

	rdc = widget_window_ctx(wt);

	ilk = get_menu_next_item(pwt->menu, LINK_FIRST);
	while (ilk)
	{
		text = get_menu_item_title_ptr(ilk);
		text_size_raw(rdc, &xf, text, -1, &xs);
		xrItem.w = bd.menu + xs.cx;

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

void  widgetex_attach_splitor(res_win_t wt, link_t_ptr split)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	pwt->splitor.widget = wt;
	pwt->splitor.split = split;
}

link_t_ptr  widgetex_get_splitor(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	return pwt->splitor.split;
}

link_t_ptr  widgetex_detach_splitor(res_win_t wt)
{
	widget_struct_t* pwt;
	link_t_ptr splitor;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	splitor = pwt->splitor.split;
	pwt->splitor.split = NULL;
	pwt->splitor.widget = NULL;

	return splitor;
}

void widgetex_layout_splitor(res_win_t wt)
{
	widget_struct_t* pwt;
	xrect_t xr;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		widget_get_client_rect(wt, &xr);
		hand_splitor_size(&pwt->splitor, &xr);
	}
}

bool_t widgetex_dock(res_win_t wt, dword_t style, long cx, long cy)
{
	widget_struct_t* pwt;
	int i;

	pwt = GETWIDGSTRUCT(wt);

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

void widgetex_undock(res_win_t wt, dword_t style)
{
	widget_struct_t* pwt;
	int i;

	pwt = GETWIDGSTRUCT(wt);

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

void widgetex_get_dock_rect(res_win_t wt, dword_t style, xrect_t* pxr)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	hand_docker_calc_rect(&pwt->docker, style, pxr);
}

static int STDCALL _widget_set_child_color_mode(res_win_t wt, var_long pv)
{
	dword_t dw;

	if (widgetex_has_struct(wt))
	{
		dw = widget_get_style(wt);
		if (dw & WD_STYLE_NOCHANGE)
			return 1;

		widgetex_set_color_mode(wt, (const clr_mod_t*)pv);
	}

	return 1;
}

void widgetex_set_color_mode(res_win_t wt, const clr_mod_t* pclr)
{
	widget_struct_t* pwt;
	dword_t dw;

	pwt = GETWIDGSTRUCT(wt);

	if (!pwt)
		return;

	format_xcolor(&pclr->clr_bkg, pwt->xb.color);

	format_xcolor(&pclr->clr_frg, pwt->xp.color);

	format_xcolor(&pclr->clr_txt, pwt->xf.color);

	xmem_copy((void*)&pwt->msk, (void*)&pclr->clr_msk, sizeof(xcolor_t));

	xmem_copy((void*)&pwt->ico, (void*)&pclr->clr_ico, sizeof(xcolor_t));

	if (widget_has_subproc(wt))
	{
		widget_send_command(wt, COMMAND_COLOR, IDC_SELF, (var_long)pclr);
	}

	dw = widget_get_style(wt);
	if (dw & WD_STYLE_NOCHANGE)
		return;

	widget_enum_child(wt, _widget_set_child_color_mode, (var_long)pclr);
}

void widgetex_get_color_mode(res_win_t wt, clr_mod_t* pclr)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	if (!pwt)
		return;

	parse_xcolor(&pclr->clr_bkg, pwt->xb.color);

	parse_xcolor(&pclr->clr_frg, pwt->xp.color);

	parse_xcolor(&pclr->clr_txt, pwt->xf.color);

	xmem_copy((void*)&pclr->clr_msk, (void*)&pwt->msk, sizeof(xcolor_t));

	xmem_copy((void*)&pclr->clr_ico, (void*)&pwt->ico, sizeof(xcolor_t));
}

void widget_get_scroll(res_win_t wt, bool_t bHorz, scroll_t* psl)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	if (bHorz)
	{
		psl->max = (pwt->cb.w - pwt->vb.w);
		psl->min = pwt->sc.cx;
		psl->page = 2 * pwt->sc.cx;
		psl->pos = (pwt->vb.x);
	}
	else
	{
		psl->max = (pwt->cb.h - pwt->vb.h);
		psl->min = pwt->sc.cy;
		psl->page = 2 * pwt->sc.cy;
		psl->pos = (pwt->vb.y);
	}
}

void widget_reset_scroll(res_win_t wt, bool_t horz)
{
	widget_struct_t* pwt;
	scroll_t sc = { 0 };

	pwt = GETWIDGSTRUCT(wt);

	if (!pwt)
		return;

	if (horz)
	{
		sc.max = (pwt->cb.w - pwt->vb.w);
		sc.min = pwt->sc.cx;
		sc.page = 2 * pwt->sc.cx;
		sc.pos = pwt->vb.x;
	}
	else
	{
		sc.max = (pwt->cb.h - pwt->vb.h);
		sc.min = pwt->sc.cy;
		sc.page = 2 * pwt->sc.cy;
		sc.pos = pwt->vb.y;
	}

	widget_set_scroll(wt, horz, &sc);

	if (widget_get_style(wt) & WD_STYLE_OWNERNC)
	{
		widgetex_redraw_scroll(wt, horz);
	}
}

void widgetex_reset_paging(res_win_t wt, long ww, long wh, long vw, long vh, long lw, long lh)
{
	long cx, cy;
	float min_cx, min_cy;
	canvas_t canv;

	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

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

	pwt->sc.cy = lh;
	pwt->sc.cx = lw;
}

void widgetex_get_view_rect(res_win_t wt, viewbox_t* pbox)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	pbox->px = pwt->cb.x - pwt->vb.x;
	pbox->py = pwt->cb.y - pwt->vb.y;
	pbox->pw = pwt->cb.w - 2 * pwt->cb.x;
	pbox->ph = pwt->cb.h - 2 * pwt->cb.y;
}

void widgetex_get_canv_rect(res_win_t wt, canvbox_t* pbox)
{
	canvas_t canv;
	xrect_t xr;

	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

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

void widgetex_get_canv_size(res_win_t wt, xsize_t* pps)
{
	canvas_t canv;

	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	pps->cx = pwt->cb.w - 2 * pwt->cb.x;
	pps->cy = pwt->cb.h - 2 * pwt->cb.y;

	size_pt_to_tm(canv, pps);
}

void widgetex_ensure_visible(res_win_t wt, const xrect_t* pxr, bool_t scroll)
{
	bool_t b_horz = 0;
	bool_t b_vert = 0;

	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

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
		widget_redraw(wt, NULL, 0);
}

void widgetex_rect_to_pt(res_win_t wt, xrect_t* pxr)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widgetex_get_canv_rect(wt, &cb);

	ft_offset_rect(pxr, cb.fx, cb.fy);

	rect_tm_to_pt(canv, pxr);
}

void widgetex_rect_to_tm(res_win_t wt, xrect_t* pxr)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widgetex_get_canv_rect(wt, &cb);

	rect_pt_to_tm(canv, pxr);

	ft_offset_rect(pxr, -cb.fx, -cb.fy);
}

void widgetex_point_to_pt(res_win_t wt, xpoint_t* ppt)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widgetex_get_canv_rect(wt, &cb);

	ft_offset_point(ppt, cb.fx, cb.fy);

	point_tm_to_pt(canv, ppt);
}

void widgetex_point_to_tm(res_win_t wt, xpoint_t* ppt)
{
	canvas_t canv;
	canvbox_t cb;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	widgetex_get_canv_rect(wt, &cb);

	point_pt_to_tm(canv, ppt);

	ft_offset_point(ppt, -cb.fx, -cb.fy);
}

void widgetex_size_to_pt(res_win_t wt, xsize_t* pxs)
{
	canvas_t canv;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	size_tm_to_pt(canv, pxs);
}

void widgetex_size_to_tm(res_win_t wt, xsize_t* pxs)
{
	canvas_t canv;

	canv = widget_get_canvas(wt);

	XDL_ASSERT(canv != NULL);

	size_pt_to_tm(canv, pxs);
}

bool_t widgetex_point_corner(res_win_t wt, const xpoint_t* ppt)
{
	xrect_t rt;
	int corn = 10;

	widget_get_client_rect(wt, &rt);
	rt.x = rt.w - corn;
	rt.y = rt.h - corn;

	return pt_in_rect(ppt, &rt);
}


