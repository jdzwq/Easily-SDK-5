/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc list dialog document

	@module	listdlg.c | implement file

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
#include "xdcimp.h"
#include "xdcctrl.h"
#include "xdcbox.h"

#define IDC_LISTDLG_LIST			10
#define IDC_LISTDLG_PUSHBOX_OK		11

#define LISTDLG_BUTTON_HEIGHT		(float)8 //tm
#define LISTDLG_BUTTON_WIDTH		(float)12 //tm

typedef struct _listdlg_delta_t{
	link_t_ptr list;
}listdlg_delta_t;

#define GETLISTDLGDELTA(ph) 	(listdlg_delta_t*)widget_get_user_delta(ph)
#define SETLISTDLGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/**********************************************************************************/
void listdlg_on_ok(res_win_t widget)
{
	res_win_t ctrl;
	link_t_ptr ilk;
	int ret;

	ctrl = widget_get_child(widget, IDC_LISTDLG_LIST);

	ilk = listctrl_get_focus_item(ctrl);
	ret = (ilk) ? xstol(get_list_item_id_ptr(ilk)) : 0;

	widget_close(widget, ret);
}
/**********************************************************************************/
int hand_listdlg_create(res_win_t widget, void* data)
{
	listdlg_delta_t* ptd;

	xrect_t xr;
	xsize_t xs;
	res_win_t listctrl, pushbox;

	widget_hand_create(widget);

	ptd = (listdlg_delta_t*)xmem_alloc(sizeof(listdlg_delta_t));
	xmem_zero((void*)ptd, sizeof(listdlg_delta_t));
	ptd->list = (link_t_ptr)data;

	xs.fw = LISTDLG_BUTTON_WIDTH;
	xs.fh = LISTDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= xs.h;

	listctrl = listctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_owner(listctrl, widget);
	widget_set_user_id(listctrl, IDC_LISTDLG_LIST);

	listctrl_attach(listctrl, ptd->list);
	widget_show(listctrl, WS_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.h;
	xr.h = xs.h;
	xr.x = xr.x + xr.w - xs.w;
	xr.w = xs.w;

	xs.fw = DEF_SPLIT_FEED;
	xs.fh = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);

	pt_expand_rect(&xr, -xs.w, -xs.h);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_LISTDLG_PUSHBOX_OK);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, LISTDLG_PUSHBOX_OK, -1);
	widget_show(pushbox, WS_SHOW_NORMAL);

	SETLISTDLGDELTA(widget, ptd);

	return 0;
}

void hand_listdlg_destroy(res_win_t widget)
{
	listdlg_delta_t* ptd = GETLISTDLGDELTA(widget);
	res_win_t ctrl;

	XDL_ASSERT(ptd != NULL);

	ctrl = widget_get_child(widget, IDC_LISTDLG_LIST);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_LISTDLG_PUSHBOX_OK);
	if (ctrl)
		widget_destroy(ctrl);

	xmem_free(ptd);

	SETLISTDLGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_listdlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	listdlg_delta_t* ptd = GETLISTDLGDELTA(widget);
	xsize_t xs;
	xrect_t xr;
	res_win_t ctrl;

	xs.fw = LISTDLG_BUTTON_WIDTH;
	xs.fh = LISTDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= xs.h;

	ctrl = widget_get_child(widget, IDC_LISTDLG_LIST);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
		widget_update(ctrl);
	}

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.h;
	xr.h = xs.h;
	xr.x = xr.x + xr.w - xs.w;
	xr.w = xs.w;

	xs.fw = DEF_SPLIT_FEED;
	xs.fh = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);

	pt_expand_rect(&xr, -xs.w, -xs.h);

	ctrl = widget_get_child(widget, IDC_LISTDLG_PUSHBOX_OK);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
		widget_update(ctrl);
	}

	widget_erase(widget, NULL);
}

void hand_listdlg_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	listdlg_delta_t* ptd = GETLISTDLGDELTA(widget);

	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc_brim, xc_core;
	xrect_t xr,xr_bar;
	xsize_t xs;	
	
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifv = {0};

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	xs.fw = LISTDLG_BUTTON_WIDTH;
	xs.fh = LISTDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y + xr.h - xs.h;
	xr_bar.w = xr.w;
	xr_bar.h = xs.h;

	parse_xcolor(&xc_brim, xb.color);
	parse_xcolor(&xc_core, xb.color);
	lighten_xcolor(&xc_brim, DEF_MIDD_DARKEN);

	(*ifv.pf_gradient_rect)(ifv.ctx, &xc_brim, &xc_core, GDI_ATTR_GRADIENT_VERT, &xr_bar);

	

	end_canvas_paint(canv, dc, pxr);
}

void hand_listdlg_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	listdlg_delta_t* ptd = GETLISTDLGDELTA(widget);

	if (cid == IDC_LISTDLG_PUSHBOX_OK)
	{
		listdlg_on_ok(widget);
	}
}

void hand_listdlg_notice(res_win_t widget, NOTICE* pnt)
{
	listdlg_delta_t* ptd = GETLISTDLGDELTA(widget);

	if (pnt->id == IDC_LISTDLG_LIST)
	{
		NOTICE_LIST* pnl = (NOTICE_LIST*)pnt;
		switch (pnl->code)
		{
		case NC_LISTDBCLK:
			widget_post_command(widget, 0, IDC_LISTDLG_PUSHBOX_OK, 0);
			break;
		}
	}
}
/***************************************************************************************/
res_win_t listdlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner)
{
	if_event_t ev = { 0 };
	res_win_t dlg;
	xrect_t xr = { 0 };
	clr_mod_t clr;

	ev.param = (void*)ptr;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_listdlg_create)
		EVENT_ON_DESTROY(hand_listdlg_destroy)

		EVENT_ON_PAINT(hand_listdlg_paint)

		EVENT_ON_SIZE(hand_listdlg_size)

		EVENT_ON_MENU_COMMAND(hand_listdlg_menu_command)
		EVENT_ON_NOTICE(hand_listdlg_notice)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	dlg = widget_create(NULL, WD_STYLE_DIALOG | WD_STYLE_OWNERNC, &xr, owner, &ev);
	widget_set_owner(dlg, owner);
	widget_set_user_id(dlg, IDC_LISTDLG);
	widget_set_title(dlg, title);

	listdlg_popup_size(dlg, RECTSIZE(&xr));
	widget_size(dlg, RECTSIZE(&xr));
	widget_update(dlg);
	widget_center_window(dlg, owner);

	if (widget_is_valid(owner))
	{
		widget_get_color_mode(owner, &clr);
		widget_set_color_mode(dlg, &clr);
	}

	return dlg;
}

void listdlg_popup_size(res_win_t widget, xsize_t* pxs)
{
	listdlg_delta_t* ptd = GETLISTDLGDELTA(widget);

	XDL_ASSERT(ptd->list != NULL);

	pxs->fw = get_list_item_width(ptd->list) * 5 + DEF_TOUCH_SPAN;
	pxs->fh = get_list_item_height(ptd->list) * 3 + LISTDLG_BUTTON_HEIGHT;
	
	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

