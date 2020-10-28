/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc title control document

	@module	titlectrl.c | implement file

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

typedef struct title_delta_t{
	link_t_ptr title;
	link_t_ptr item;
	link_t_ptr hover;
}title_delta_t;

#define GETTITLEDELTA(ph) 	(title_delta_t*)widget_get_user_delta(ph)
#define SETTITLEDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/****************************************************************************************************/

static void _titlectrl_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	calc_title_item_rect(ptd->title, ptd->item, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _titlectrl_reset_page(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	int pw, ph;
	xrect_t xr;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	widget_reset_paging(widget, pw, ph, pw, ph, 0, 0);
}

/*********************************************control event******************************************/
int noti_title_owner(res_win_t widget, unsigned int code, link_t_ptr title, link_t_ptr ilk, void* data)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	NOTICE_TITLE nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.title = title;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_title_item_leave(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	XDL_ASSERT(ptd->hover);

	ptd->hover = NULL;

	widget_erase(widget, NULL);

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_title_item_enter(res_win_t widget, link_t_ptr plk)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	XDL_ASSERT(!ptd->hover);
	XDL_ASSERT(plk);

	ptd->hover = plk;

	widget_erase(widget, NULL);

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_title_item_hover(res_win_t widget, int x, int y)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_title_owner(widget, NC_TITLEITEMHOVER, ptd->title, ptd->hover, (void*)&xp);
}

bool_t noti_title_item_changing(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	XDL_ASSERT(ptd->item);

	if (noti_title_owner(widget, NC_TITLEITEMCHANGING, ptd->title, ptd->item, NULL))
		return 0;

	ptd->item = NULL;

	widget_erase(widget, NULL);

	return 1;
}

void noti_title_item_changed(res_win_t widget, link_t_ptr plk)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	XDL_ASSERT(!ptd->item);
	XDL_ASSERT(plk);

	ptd->item = plk;

	widget_erase(widget, NULL);

	noti_title_owner(widget, NC_TITLEITEMCHANGED, ptd->title, ptd->item, NULL);
}

/*********************************************************************************************/
int hand_title_create(res_win_t widget, void* data)
{
	title_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (title_delta_t*)xmem_alloc(sizeof(title_delta_t));
	xmem_zero((void*)ptd, sizeof(title_delta_t));

	SETTITLEDELTA(widget, ptd);

	return 0;
}

void hand_title_destroy(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETTITLEDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_title_size(res_win_t widget, int code, const xsize_t* pxs)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	xrect_t xr;

	if (!ptd->title)
		return;

	widget_get_client_rect(widget, &xr);
	widget_rect_to_tm(widget, &xr);

	set_title_width(ptd->title, xr.fw);
	set_title_height(ptd->title, xr.fh);

	titlectrl_redraw(widget);
}

void hand_title_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	int nHint;
	link_t_ptr plk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->title)
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_title_hint(&pt, ptd->title, ptd->item, &plk);
	bRe = (plk == ptd->hover) ? 1 : 0;

	if (nHint == TITLE_HINT_ITEM && !ptd->hover && !bRe)
	{
		noti_title_item_enter(widget, plk);
	}
	else if (nHint == TITLE_HINT_ITEM && ptd->hover && !bRe)
	{
		noti_title_item_leave(widget);
	}
	else if (nHint != TITLE_HINT_ITEM && ptd->hover)
	{
		noti_title_item_leave(widget);
	}
}

void hand_title_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;

	if (ptd->hover)
		noti_title_item_hover(widget, pxp->x, pxp->y);
}

void hand_title_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;

	if (ptd->hover)
		noti_title_item_leave(widget);
}

void hand_title_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;

	noti_title_owner(widget, NC_TITLEDBCLK,ptd->title, ptd->item, (void*)pxp);
}

void hand_title_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;
}

void hand_title_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	link_t_ptr plk;
	int nHint;
	xpoint_t pt;
	bool_t bRe;

	if (!ptd->title)
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_title_hint(&pt, ptd->title, ptd->item, &plk);

	if (nHint == TITLE_HINT_CLOSE)
	{
		titlectrl_delete_item(widget, plk);
		return;
	}

	bRe = (!plk || plk == ptd->item) ? 1 : 0;

	if (ptd->item && !bRe)
	{
		if (!noti_title_item_changing(widget))
			bRe = 1;
	}

	if (!bRe && plk)
	{
		noti_title_item_changed(widget, plk);
	}

	noti_title_owner(widget, NC_TITLELBCLK, ptd->title, ptd->item, (void*)pxp);
}

void hand_title_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;
}

void hand_title_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;

	noti_title_owner(widget, NC_TITLERBCLK, ptd->title, ptd->item, (void*)pxp);
}

void hand_title_mouse_wheel(res_win_t widget, dword_t dw, short delta)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;
}

void hand_title_keydown(res_win_t widget, dword_t ks, int nKey)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	if (!ptd->title)
		return;

	switch (nKey)
	{
	case KEY_LEFT:
	case KEY_PAGEUP:
		titlectrl_tabskip(widget,TABORDER_LEFT);
		break;
	case KEY_RIGHT:
	case KEY_PAGEDOWN:
		titlectrl_tabskip(widget,TABORDER_RIGHT);
		break;
	case KEY_HOME:
		titlectrl_tabskip(widget,TABORDER_HOME);
		break;
	case KEY_END:
		titlectrl_tabskip(widget,TABORDER_END);
		break;
	}
}

void hand_title_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	visual_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xrect_t xr = { 0 };
	xcolor_t xc_brim = { 0 };
	xcolor_t xc_core = { 0 };
	tchar_t token[RES_LEN + 1] = { 0 };
	const tchar_t* orita;

	canvas_t canv;
	const if_drawing_t* pif = NULL;
	if_drawing_t ifv = {0};

	if (!ptd->title)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = widget_get_canvas_interface(widget);
	

	
	
	
	
	

	orita = get_title_oritation_ptr(ptd->title);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	parse_xcolor(&xc_brim, xb.color);
	lighten_xbrush(&xb, DEF_SOFT_DARKEN);
	parse_xcolor(&xc_core, xb.color);

	if (compare_text(orita, -1, ATTR_ORITATION_BOTTOM, -1, 0) == 0 || compare_text(orita, -1, ATTR_ORITATION_TOP, -1, 0) == 0)
		xscpy(token, GDI_ATTR_GRADIENT_VERT);
	else
		xscpy(token, GDI_ATTR_GRADIENT_HORZ);

	(*ifv.pf_gradient_rect)(ifv.ctx, &xc_brim, &xc_core, token, &xr);

	draw_title(pif, ptd->title, ptd->item);

	

	end_canvas_paint(canv, dc, pxr);
	
}

/************************************************************************************************/

res_win_t titlectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_title_create)
		EVENT_ON_DESTROY(hand_title_destroy)

		EVENT_ON_PAINT(hand_title_paint)

		EVENT_ON_SIZE(hand_title_size)

		EVENT_ON_KEYDOWN(hand_title_keydown)

		EVENT_ON_MOUSE_MOVE(hand_title_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_title_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_title_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_title_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_title_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_title_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_title_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_title_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void titlectrl_attach(res_win_t widget, link_t_ptr ptr)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_title_doc(ptr));

	ptd->item = NULL;
	ptd->title = ptr;

	widget_get_client_rect(widget, &xr);
	widget_rect_to_tm(widget, &xr);

	set_title_width(ptd->title, xr.fw);
	set_title_height(ptd->title, xr.fh);

	titlectrl_redraw(widget);

	titlectrl_set_focus_item(widget, get_title_next_item(ptd->title, LINK_FIRST));
}

link_t_ptr titlectrl_detach(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	if (ptd->title)
	{
		data = ptd->title;
		ptd->title = NULL;
	}
	else
		data = NULL;
	return data;
}

link_t_ptr titlectrl_fetch(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->title;
}

void titlectrl_tabskip(res_win_t widget, int nSkip)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	link_t_ptr plk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return;

	plk = ptd->item;

	switch (nSkip)
	{
	case TABORDER_RIGHT:
	case TABORDER_DOWN:
		if (plk == NULL)
			plk = get_title_next_item(ptd->title, LINK_FIRST);
		else
			plk = get_title_next_item(ptd->title, plk);

		if (plk)
			titlectrl_set_focus_item(widget, plk);
		break;
	case TABORDER_LEFT:
	case TABORDER_UP:
		if (plk == NULL)
			plk = get_title_prev_item(ptd->title, LINK_LAST);
		else
			plk = get_title_prev_item(ptd->title, plk);

		if (plk)
			titlectrl_set_focus_item(widget, plk);
		break;
	case TABORDER_HOME:
		plk = get_title_next_item(ptd->title, LINK_FIRST);

		if (plk)
			titlectrl_set_focus_item(widget, plk);
		break;
	case TABORDER_END:
		plk = get_title_prev_item(ptd->title, LINK_LAST);

		if (plk)
			titlectrl_set_focus_item(widget, plk);
		break;
	}
}

void titlectrl_redraw(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return;

	b_valid = 0;
	ilk = get_title_next_item(ptd->title, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		noti_title_owner(widget, NC_TITLEITEMCALCED, ptd->title, ilk, NULL);

		ilk = get_title_next_item(ptd->title, ilk);
	}

	noti_title_owner(widget, NC_TITLECALCED, ptd->title, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}
	ptd->hover = NULL;

	_titlectrl_reset_page(widget);

	widget_erase(widget, NULL);
}

void titlectrl_redraw_item(res_win_t widget, link_t_ptr plk)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_title_item(ptd->title, plk));
#endif

	noti_title_owner(widget, NC_TITLEITEMCALCED, ptd->title, plk, NULL);

	if (plk == ptd->item)
	{
		_titlectrl_item_rect(widget, ptd->item, &xr);

		pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
	}
	else
	{
		widget_get_client_rect(widget, &xr);
	}

	widget_erase(widget, &xr);
}

link_t_ptr titlectrl_insert_item(res_win_t widget, link_t_ptr pos)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	link_t_ptr ilk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return NULL;

	if (pos == LINK_FIRST)
		pos = get_title_next_item(ptd->title, LINK_FIRST);
	else if (pos == LINK_LAST)
		pos = get_title_prev_item(ptd->title, LINK_LAST);
	else
	{
#ifdef _DEBUG
		XDL_ASSERT(is_title_item(ptd->title, pos));
#endif
	}

	if (!pos) pos = LINK_FIRST;

	ilk = insert_title_item(ptd->title, pos);

	noti_title_owner(widget, NC_TITLEITEMINSERT,ptd->title, ilk, NULL);

	titlectrl_redraw(widget);

	titlectrl_set_focus_item(widget, ilk);

	return ilk;
}

bool_t titlectrl_delete_item(res_win_t widget, link_t_ptr plk)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	link_t_ptr nlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_title_item(ptd->title, plk));
#endif

	if (get_title_item_locked(plk))
		return 0;

	if (noti_title_owner(widget, NC_TITLEITEMDELETE, ptd->title, plk, NULL))
		return 0;

	if (plk == ptd->item)
	{
		noti_title_item_changing(widget);
	}

	nlk = get_title_prev_item(ptd->title, plk);
	if (!nlk)
		nlk = get_title_next_item(ptd->title, plk);

	delete_title_item(plk);

	titlectrl_redraw(widget);

	if (nlk)
		noti_title_item_changed(widget, nlk);

	return 1;
}

bool_t titlectrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return 0;

	if (ilk == LINK_FIRST)
		ilk = get_title_next_item(ptd->title, LINK_FIRST);
	else if (ilk == LINK_LAST)
		ilk = get_title_prev_item(ptd->title, LINK_LAST);

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->item && !bRe)
	{
		if (!noti_title_item_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		noti_title_item_changed(widget, ilk);
	}

	return 1;
}

link_t_ptr titlectrl_get_focus_item(res_win_t widget)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return NULL;

	return ptd->item;
}

void titlectrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	title_delta_t* ptd = GETTITLEDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->title)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_title_item(ptd->title, ilk));
#endif

	_titlectrl_item_rect(widget, ilk, pxr);
}
