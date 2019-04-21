/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tool control document

	@module	toolctrl.c | tool control implement file

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
#include "winnc.h"

typedef struct _tool_delta_t{
	link_t_ptr tool;
	link_t_ptr item;
	link_t_ptr hover;

	bool_t b_press;
}tool_delta_t;

#define GETTOOLDELTA(ph) 		(tool_delta_t*)widget_get_user_delta(ph)
#define SETTOOLDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

/********************************************************************************************************/
static void _toolctrl_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_tool_group_item_rect(&cb, ptd->tool, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _toolctrl_reset_page(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	long pw, ph;
	xrect_t xr;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	widget_reset_paging(widget, pw, ph, pw, ph, 0, 0);
}

/*********************************************************************************************************/
int noti_tool_owner(res_win_t widget, unsigned long code, link_t_ptr tool, link_t_ptr ilk, void* data)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	NOTICE_TOOL nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;
	nf.ret = 0;
	nf.tool = tool;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_tool_item_leave(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	XDL_ASSERT(ptd->hover);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_tool_item_enter(res_win_t widget, link_t_ptr plk)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	XDL_ASSERT(!ptd->hover);
	XDL_ASSERT(plk);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_tool_item_hover(res_win_t widget, long x, long y)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover);

	xp.x = x;
	xp.y = y;
	noti_tool_owner(widget, NC_TOOLITEMHOVER, ptd->tool, ptd->hover, (void*)&xp);
}

bool_t noti_tool_item_changing(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	_toolctrl_item_rect(widget, ptd->item, &xr);
	ptd->item = NULL;

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	return 1;
}

void noti_tool_item_changed(res_win_t widget, link_t_ptr plk)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = plk;
	_toolctrl_item_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

/*******************************************************************************/
int hand_tool_create(res_win_t widget, void* data)
{
	tool_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (tool_delta_t*)xmem_alloc(sizeof(tool_delta_t));
	xmem_zero((void*)ptd, sizeof(tool_delta_t));

	SETTOOLDELTA(widget, ptd);

	return 0;
}

void hand_tool_destroy(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETTOOLDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_tool_size(res_win_t widget, int code, const xsize_t* prs)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	if (!ptd->tool)
		return;

	toolctrl_redraw(widget);
}

void hand_tool_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	int nHint;
	link_t_ptr plk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->tool)
		return;

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_tool_point_hint(&cb, &pt, ptd->tool, &plk);
	bRe = (plk == ptd->hover) ? 1 : 0;

	if (nHint == TOOL_HINT_ITEM && !ptd->hover && !bRe)
	{
		noti_tool_item_enter(widget, plk);
		return;
	}
	else if (nHint == TOOL_HINT_ITEM && ptd->hover && !bRe)
	{
		noti_tool_item_leave(widget);
	}
	else if (nHint != TOOL_HINT_ITEM && ptd->hover)
	{
		noti_tool_item_leave(widget);
	}
}

void hand_tool_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	if (!ptd->tool)
		return;

	if (ptd->hover)
		noti_tool_item_hover(widget, pxp->x, pxp->y);
}

void hand_tool_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	if (!ptd->tool)
		return;

	if (ptd->hover)
		noti_tool_item_leave(widget);
}

void hand_tool_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	if (!ptd->tool)
		return;

	noti_tool_owner(widget, NC_TOOLDBCLK, ptd->tool, ptd->item, (void*)pxp);
}

void hand_tool_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	xrect_t xr;

	if (!ptd->tool)
		return;

	ptd->b_press = 1;

	if (ptd->hover)
	{
		_toolctrl_item_rect(widget, ptd->hover, &xr);
		widget_update(widget, &xr, 0);
	}
}

void hand_tool_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	int nHint;
	link_t_ptr plk;
	bool_t bRe;
	xpoint_t pt;
	xrect_t xr;
	canvbox_t cb;

	if (!ptd->tool)
		return;

	ptd->b_press = 0;

	if (ptd->hover)
	{
		_toolctrl_item_rect(widget, ptd->hover, &xr);
		widget_update(widget, &xr, 0);
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_tool_point_hint(&cb, &pt, ptd->tool, &plk);

	bRe = (plk == ptd->item) ? 1 : 0;

	if (ptd->item && !bRe)
	{
		if (!noti_tool_item_changing(widget))
			bRe = 1;
	}

	if (plk && !bRe)
	{
		noti_tool_item_changed(widget, plk);
	}

	noti_tool_owner(widget, NC_TOOLLBCLK, ptd->tool, ptd->item, (void*)pxp);
}

void hand_tool_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	if (!ptd->tool)
		return;
}

void hand_tool_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	int nHint;
	link_t_ptr plk;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->tool)
		return;

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_tool_point_hint(&cb, &pt, ptd->tool, &plk);

	noti_tool_owner(widget, NC_TOOLRBCLK, ptd->tool, plk, (void*)pxp);
}

void hand_tool_keydown(res_win_t widget, int nKey)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	if (!ptd->tool)
		return;

	switch (nKey)
	{
	case KEY_LEFT:
	case KEY_PAGEUP:
		toolctrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
	case KEY_PAGEDOWN:
		toolctrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_HOME:
		toolctrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		toolctrl_tabskip(widget,WD_TAB_END);
		break;
	}
}

void hand_tool_erase(res_win_t widget, res_ctx_t dc)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	if (!ptd->tool)
		return;
}

void hand_tool_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->tool)
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

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	default_xgradi(&xg);
	xscpy(xg.brim_color, xb.color);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	xscpy(xg.core_color, xb.color);

	gradient_rect_raw(rdc, &xg, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_tool(pif, &cb, ptd->tool);

	//draw focus
	if (ptd->hover && ptd->b_press)
	{
		_toolctrl_item_rect(widget, ptd->hover, &xr);
		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_ALPHA_COLOR);

		draw_3drect_raw(rdc, &xp, &xr);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*******************************************************************************************/

res_win_t toolctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_tool_create)
		EVENT_ON_DESTROY(hand_tool_destroy)

		EVENT_ON_ERASE(hand_tool_erase)
		EVENT_ON_PAINT(hand_tool_paint)

		EVENT_ON_SIZE(hand_tool_size)

		EVENT_ON_KEYDOWN(hand_tool_keydown)

		EVENT_ON_MOUSE_MOVE(hand_tool_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_tool_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_tool_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_tool_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_tool_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_tool_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_tool_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_tool_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void toolctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_tool_doc(ptr));

	ptd->item = NULL;
	ptd->tool = ptr;

	toolctrl_redraw(widget);
}

link_t_ptr toolctrl_detach(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	if (ptd->tool)
	{
		data = ptd->tool;
		ptd->tool = NULL;
	}
	else
		data = NULL;
	return data;
}

link_t_ptr toolctrl_fetch(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	return ptd->tool;
}

void toolctrl_redraw(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	link_t_ptr glk,ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tool)
		return;

	b_valid = 0;
	glk = get_tool_next_group(ptd->tool, LINK_FIRST);
	while (glk)
	{
		ilk = get_tool_group_next_item(glk, LINK_FIRST);
		while (ilk)
		{
			if (ilk == ptd->item)
				b_valid = 1;

			noti_tool_owner(widget, NC_TOOLITEMCALCED, ptd->tool, ilk, NULL);

			ilk = get_tool_group_next_item(glk, ilk);
		}

		glk = get_tool_next_group(ptd->tool, glk);
	}

	noti_tool_owner(widget, NC_TOOLCALCED, ptd->tool, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}
	ptd->hover = NULL;

	_toolctrl_reset_page(widget);

	widget_update(widget, NULL, 0);
}

void toolctrl_redraw_item(res_win_t widget, link_t_ptr plk)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tool)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_tool_group_item(ptd->tool, plk));
#endif

	noti_tool_owner(widget, NC_TOOLITEMCALCED, ptd->tool, plk, NULL);

	if (plk == ptd->item)
	{
		_toolctrl_item_rect(widget, ptd->item, &xr);

		pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
	}
	else
	{
		widget_get_client_rect(widget, &xr);
	}

	widget_update(widget, &xr, 0);
}

void toolctrl_tabskip(res_win_t widget, int nSkip)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	link_t_ptr plk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tool)
		return;

	plk = ptd->item;

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (plk == NULL)
			plk = get_tool_group_next_item(ptd->tool, LINK_FIRST);
		else
			plk = get_tool_group_next_item(ptd->tool, plk);

		if (plk)
			toolctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (plk == NULL)
			plk = get_tool_group_prev_item(ptd->tool, LINK_LAST);
		else
			plk = get_tool_group_prev_item(ptd->tool, plk);

		if (plk)
			toolctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_HOME:
		plk = get_tool_group_next_item(ptd->tool, LINK_FIRST);

		if (plk)
			toolctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_END:
		plk = get_tool_group_prev_item(ptd->tool, LINK_LAST);

		if (plk)
			toolctrl_set_focus_item(widget, plk);
		break;
	}
}

bool_t toolctrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tool)
		return 0;

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_tool_group_item(ptd->tool, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->item && !bRe)
	{
		if (!noti_tool_item_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		noti_tool_item_changed(widget, ilk);
	}

	return 1;
}

link_t_ptr toolctrl_get_focus_item(res_win_t widget)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tool)
		return NULL;

	return ptd->item;
}

void toolctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* prt)
{
	tool_delta_t* ptd = GETTOOLDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->tool)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_tool_group_item(ptd->tool, ilk));
#endif

	_toolctrl_item_rect(widget, ilk, prt);
}


