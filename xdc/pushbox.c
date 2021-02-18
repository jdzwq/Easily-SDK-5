/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc push control document

	@module	pushbox.c | implement file

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

#define PUSHBOX_ATTR_BUTTON_SPAN	(float)5 //TM

typedef struct _pushbox_delta_t{
	tchar_t* sz_text;
	bool_t b_check;
}pushbox_delta_t;

#define GETPUSHBOXDELTA(ph) 	(pushbox_delta_t*)widget_get_user_delta(ph)
#define SETPUSHBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/********************************************************************************/
void _pushbox_reset_page(res_win_t widget)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, xr.w, xr.h, 0, 0);
}

/**********************************************************************************/
int hand_pushbox_create(res_win_t widget, void* data)
{
	pushbox_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (pushbox_delta_t*)xmem_alloc(sizeof(pushbox_delta_t));
	xmem_zero((void*)ptd, sizeof(pushbox_delta_t));

	SETPUSHBOXDELTA(widget, ptd);

	return 0;
}

void hand_pushbox_destroy(res_win_t widget)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (ptd->sz_text)
		xsfree(ptd->sz_text);

	xmem_free(ptd);

	SETPUSHBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_pushbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);
	dword_t ws;

	ws = widget_get_style(widget);
	
	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	if (!(ws & WD_PUSHBOX_CHECK))
	{
		ptd->b_check = 1;
		widget_erase(widget, NULL);
	}
}

void hand_pushbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);
	dword_t ws;

	ws = widget_get_style(widget);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}

	if (ws & WD_PUSHBOX_CHECK)
	{
		if (ptd->b_check)
			ptd->b_check = 0;
		else
			ptd->b_check = 1;
	}
	else
	{
		ptd->b_check = 0;
	}

	widget_erase(widget, NULL);

	widget_post_command(widget_get_owner(widget), ptd->b_check, widget_get_user_id(widget), (var_long)widget);
}

void hand_pushbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);
	
	_pushbox_reset_page(widget);

	widget_erase(widget, NULL);
}

void hand_pushbox_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);
	visual_t rdc;

	dword_t ws;
	xrect_t xr,xr_box;

	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };

	canvas_t canv;
	const drawing_interface* pif = NULL;
	drawing_interface ifv = {0};

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);
	widget_get_iconic(widget, &xc);

	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);
	pif = widget_get_canvas_interface(widget);
	

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	ws = widget_get_style(widget);

	if (ws & WD_PUSHBOX_CHECK)
	{
		xr_box.fx = pif->rect.fx;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = DEF_SMALL_ICON;
		xr_box.fh = pif->rect.fh;
		ft_center_rect(&xr_box, DEF_SMALL_ICON, DEF_SMALL_ICON);

		if (ptd->b_check)
			draw_gizmo(pif, &xc, &xr_box, GDI_ATTR_GIZMO_CHECKED);
		else
			draw_gizmo(pif, &xc, &xr_box, GDI_ATTR_GIZMO_CHECKBOX);

		xr_box.fx = pif->rect.fx + DEF_SMALL_ICON;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = pif->rect.fw - DEF_SMALL_ICON;
		xr_box.fh = pif->rect.fh;

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		(pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr_box, ptd->sz_text, -1);
	}
	else if (ws & WD_PUSHBOX_ICON)
	{
		xr_box.fx = pif->rect.fx;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = pif->rect.fw;
		xr_box.fh = pif->rect.fh;

		if (ptd->b_check)
		{
			xp.adorn.feed = 2;
			xp.adorn.size = 2;
			(pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr_box);
		}
		else
		{
			ft_expand_rect(&xr_box, -0.5, -0.5);
			(pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr_box);
		}

		xr_box.fx = pif->rect.fx;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = pif->rect.fw;
		xr_box.fh = pif->rect.fh;

		ft_center_rect(&xr_box, DEF_SMALL_ICON, DEF_SMALL_ICON);
		draw_gizmo(pif, &xc, &xr_box, ptd->sz_text);
	}
	else if (ws & WD_PUSHBOX_IMAGE)
	{
		xr_box.fx = pif->rect.fx;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = pif->rect.fw;
		xr_box.fh = pif->rect.fh;

		if (ptd->b_check)
		{
			xp.adorn.feed = 2;
			xp.adorn.size = 2;
			(pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr_box);
		}
		else
		{
			ft_expand_rect(&xr_box, -0.5, -0.5);
			(pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr_box);
		}

		xr_box.fx = pif->rect.fx;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = pif->rect.fw;
		xr_box.fh = pif->rect.fh;

		parse_ximage_from_source(&xi, ptd->sz_text);
		(pif->pf_draw_image)(pif->ctx, &xi, &xr_box);
	}
	else
	{
		xr_box.fx = pif->rect.fx;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = pif->rect.fw;
		xr_box.fh = pif->rect.fh;

		if (ptd->b_check)
		{
			xp.adorn.feed = 2;
			xp.adorn.size = 2;
			(pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr_box);
		}
		else
		{
			ft_expand_rect(&xr_box, -0.5, -0.5);
			(pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr_box);
		}

		xr_box.fx = pif->rect.fx;
		xr_box.fy = pif->rect.fy;
		xr_box.fw = pif->rect.fw;
		xr_box.fh = pif->rect.fh;

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		(pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr_box, ptd->sz_text, -1);
	}

	

	end_canvas_paint(canv, dc, pxr);
	
}

/***************************************************************************************/
res_win_t pushbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_pushbox_create)
		EVENT_ON_DESTROY(hand_pushbox_destroy)

		EVENT_ON_PAINT(hand_pushbox_paint)

		EVENT_ON_SIZE(hand_pushbox_size)

		EVENT_ON_LBUTTON_DOWN(hand_pushbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_pushbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void pushbox_set_state(res_win_t widget, bool_t bChecked)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_check = bChecked;
}

bool_t pushbox_get_state(res_win_t widget)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_check;
}

void pushbox_set_text(res_win_t widget, const tchar_t* text, int len)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (ptd->sz_text)
		xsfree(ptd->sz_text);

	if (len < 0)
		len = xslen(text);

	ptd->sz_text = xsalloc(len + 1);
	xsncpy(ptd->sz_text, text, len);

	widget_erase(widget, NULL);
}

void pushbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	pushbox_delta_t* ptd = GETPUSHBOXDELTA(widget);
	xfont_t xf = { 0 };
	xsize_t xs;
	const drawing_interface* pif = NULL;

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	pif = widget_get_canvas_interface(widget);

	(pif->pf_text_size)(pif->ctx, &xf, ptd->sz_text, -1, &xs);

	if (xs.fw < xs.fh)
		xs.fw = xs.fh;

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}
