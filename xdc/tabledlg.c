/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc table dialog document

	@module	tabledlg.c | table dialog widnows implement file

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

#include "xdcdlg.h"
#include "handler.h"
#include "winnc.h"
#include "xdcctrl.h"
#include "xdcbox.h"

#define IDC_TABLEDLG_TABLE			10
#define IDC_TABLEDLG_PUSHBOX_OK		11

#define TABLEDLG_BUTTON_HEIGHT		(float)8 //tm
#define TABLEDLG_BUTTON_WIDTH		(float)12 //tm

typedef struct _tabledlg_delta_t{
	link_t_ptr table;
}tabledlg_delta_t;

#define GETTABLEDLGDELTA(ph) 	(tabledlg_delta_t*)widget_get_user_delta(ph)
#define SETTABLEDLGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/************************************************************************************/
void tabledlg_on_ok(res_win_t widget)
{
	res_win_t ctrl;

	ctrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);
	tablectrl_accept(ctrl, 1);

	widget_close(widget, 1);
}

void tabledlg_on_first(res_win_t widget)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_win_t tablectrl;
	link_t_ptr ilk, root;

	tablectrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);

	ilk = tablectrl_get_focus_item(tablectrl);
	if (!ilk)
		return;

	if (is_first_link(ilk))
		return;

	root = get_root_link(ilk);
	switch_link_first(root, ilk);

	tablectrl_redraw(tablectrl);
}

void tabledlg_on_last(res_win_t widget)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_win_t tablectrl;
	link_t_ptr ilk, root;

	tablectrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);

	ilk = tablectrl_get_focus_item(tablectrl);
	if (!ilk)
		return;

	if (is_last_link(ilk))
		return;

	root = get_root_link(ilk);
	switch_link_last(root, ilk);

	tablectrl_redraw(tablectrl);
}

void tabledlg_on_prev(res_win_t widget)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_win_t tablectrl;
	link_t_ptr ilk, root;

	tablectrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);

	ilk = tablectrl_get_focus_item(tablectrl);
	if (!ilk)
		return;

	if (is_first_link(ilk))
		return;

	root = get_root_link(ilk);
	switch_link_prev(root, ilk);

	tablectrl_redraw(tablectrl);
}

void tabledlg_on_next(res_win_t widget)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_win_t tablectrl;
	link_t_ptr ilk, root;

	tablectrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);

	ilk = tablectrl_get_focus_item(tablectrl);
	if (!ilk)
		return;

	if (is_last_link(ilk))
		return;

	root = get_root_link(ilk);
	switch_link_next(root, ilk);

	tablectrl_redraw(tablectrl);
}

void tabledlg_on_insert(res_win_t widget)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_win_t tablectrl;

	tablectrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);

	tablectrl_insert_item(tablectrl, LINK_LAST);
}

void tabledlg_on_delete(res_win_t widget)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_win_t tablectrl;
	link_t_ptr ilk;

	tablectrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);

	ilk = tablectrl_get_focus_item(tablectrl);
	if (!ilk)
		return;

	tablectrl_delete_item(tablectrl, ilk);
}
/**********************************************************************************/
int hand_tabledlg_create(res_win_t widget, void* data)
{
	tabledlg_delta_t* ptd;

	xrect_t xr;
	xsize_t xs;
	res_win_t tablectrl, pushbox;

	widget_hand_create(widget);

	ptd = (tabledlg_delta_t*)xmem_alloc(sizeof(tabledlg_delta_t));
	xmem_zero((void*)ptd, sizeof(tabledlg_delta_t));
	ptd->table = (link_t_ptr)data;

	xs.fx = TABLEDLG_BUTTON_WIDTH;
	xs.fy = TABLEDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= (xs.cy);

	tablectrl = tablectrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_owner(tablectrl, widget);
	widget_set_user_id(tablectrl, IDC_TABLEDLG_TABLE);

	tablectrl_attach(tablectrl, ptd->table);
	tablectrl_set_lock(tablectrl, 0);

	widget_show(tablectrl, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;
	xr.x = xr.x + xr.w - xs.cx;
	xr.w = xs.cx;

	xs.fx = DEF_SPLIT_FEED;
	xs.fy = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);

	pt_expand_rect(&xr, -xs.cx, -xs.cy);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_TABLEDLG_PUSHBOX_OK);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, TABLEDLG_PUSHBOX_OK, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	SETTABLEDLGDELTA(widget, ptd);

	widget_set_focus(tablectrl);

	return 0;
}

void hand_tabledlg_destroy(res_win_t widget)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_win_t ctrl;

	if (!ptd)
		return;

	ctrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_TABLEDLG_PUSHBOX_OK);
	if (ctrl)
		widget_destroy(ctrl);

	xmem_free(ptd);

	SETTABLEDLGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_tabledlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	xsize_t xs;
	xrect_t xr;
	res_win_t ctrl;

	xs.fx = TABLEDLG_BUTTON_WIDTH;
	xs.fy = TABLEDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= (xs.cy);

	ctrl = widget_get_child(widget, IDC_TABLEDLG_TABLE);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;
	xr.x = xr.x + xr.w - xs.cx;
	xr.w = xs.cx;

	xs.fx = DEF_SPLIT_FEED;
	xs.fy = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);

	pt_expand_rect(&xr, -xs.cx, -xs.cy);

	ctrl = widget_get_child(widget, IDC_TABLEDLG_PUSHBOX_OK);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	widget_update(widget, NULL, 0);
}

void hand_tabledlg_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);

	if (cid == IDC_TABLEDLG_PUSHBOX_OK)
	{
		tabledlg_on_ok(widget);
	}
}

void hand_tabledlg_erase(res_win_t widget, res_ctx_t dc)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
}

void hand_tabledlg_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr,xr_bar;
	xsize_t xs;
	canvas_t canv;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	xs.fx = TABLEDLG_BUTTON_WIDTH;
	xs.fy = TABLEDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y + xr.h - xs.cy;
	xr_bar.w = xr.w;
	xr_bar.h = xs.cy;

	xscpy(xg.brim_color, xb.color);
	xscpy(xg.core_color, xb.color);
	lighten_xgradi(&xg, DEF_MIDD_DARKEN);
	xscpy(xg.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	gradient_rect_raw(rdc, &xg, &xr_bar);

	end_canvas_paint(canv, dc, pxr);
}

/***************************************************************************************/
res_win_t tabledlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner)
{
	if_event_t ev = { 0 };
	res_win_t dlg;
	xrect_t xr = { 0 };
	clr_mod_t clr;

	ev.param = (void*)ptr;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_tabledlg_create)
		EVENT_ON_DESTROY(hand_tabledlg_destroy)

		EVENT_ON_ERASE(hand_tabledlg_erase)
		EVENT_ON_PAINT(hand_tabledlg_paint)

		EVENT_ON_SIZE(hand_tabledlg_size)

		EVENT_ON_MENU_COMMAND(hand_tabledlg_menu_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	dlg = widget_create(NULL, WD_STYLE_DIALOG, &xr, owner, &ev);
	widget_set_owner(dlg, owner);
	widget_set_user_id(dlg, IDC_TABLEDLG);
	widget_set_title(dlg, title);

	tabledlg_popup_size(dlg, RECTSIZE(&xr));
	widget_size(dlg, RECTSIZE(&xr));
	widget_center_window(dlg, owner);

	if (widget_is_valid(owner))
	{
		widget_get_color_mode(owner, &clr);
		widget_set_color_mode(dlg, &clr);
	}

	return dlg;
}

void tabledlg_popup_size(res_win_t widget, xsize_t* pxs)
{
	tabledlg_delta_t* ptd = GETTABLEDLGDELTA(widget);
	int n;

	XDL_ASSERT(ptd->table != NULL);

	n = get_string_entity_count(ptd->table);
	if (n < 10)
		n = 10;
	else if (n > 20)
		n = 20;

	pxs->fx = DEF_TOUCH_SPAN * 8 + TABLEDLG_BUTTON_WIDTH;
	pxs->fy = n * DEF_TOUCH_SPAN + TABLEDLG_BUTTON_HEIGHT;

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}



