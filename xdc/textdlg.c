/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc text dialog document

	@module	textdlg.c | implement file

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
#include "xdcbox.h"

#define IDC_TEXTDLG_EDIT			10
#define IDC_TEXTDLG_PUSHBOX_OK		11

#define TEXTDLG_BUTTON_HEIGHT		(float)8 //tm
#define TEXTDLG_BUTTON_WIDTH		(float)12 //tm

typedef struct _textdlg_delta_t{
	string_t var;
}textdlg_delta_t;

#define GETTEXTDLGDELTA(ph) 	(textdlg_delta_t*)widget_get_user_delta(ph)
#define SETTEXTDLGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/**********************************************************************************/
void textdlg_on_ok(res_win_t widget)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);

	res_win_t ctrl;

	ctrl = widget_get_child(widget, IDC_TEXTDLG_EDIT);

	if (ptd->var)
	{
		int len = editbox_get_text(ctrl, NULL, MAX_LONG);

		editbox_get_text(ctrl, string_ensure_buf(ptd->var, len), len);
	}

	widget_close(widget, 1);
}
/**********************************************************************************/
int hand_textdlg_create(res_win_t widget, void* data)
{
	textdlg_delta_t* ptd;

	xrect_t xr;
	xsize_t xs;
	res_win_t editbox, pushbox;

	widget_hand_create(widget);

	ptd = (textdlg_delta_t*)xmem_alloc(sizeof(textdlg_delta_t));
	xmem_zero((void*)ptd, sizeof(textdlg_delta_t));
	ptd->var = (string_t)data;

	xs.fw = TEXTDLG_BUTTON_WIDTH;
	xs.fh = TEXTDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= xs.h;

	editbox = editbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr);
	widget_set_owner(editbox, widget);
	widget_set_user_id(editbox, IDC_TEXTDLG_EDIT);

	if (ptd->var)
	{
		editbox_set_text(editbox, string_ptr(ptd->var));
	}

	widget_show(editbox, WS_SHOW_NORMAL);

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
	widget_set_user_id(pushbox, IDC_TEXTDLG_PUSHBOX_OK);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, TEXTDLG_PUSHBOX_OK, -1);
	widget_show(pushbox, WS_SHOW_NORMAL);

	SETTEXTDLGDELTA(widget, ptd);

	return 0;
}

void hand_textdlg_destroy(res_win_t widget)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	res_win_t ctrl;

	XDL_ASSERT(ptd != NULL);

	ctrl = widget_get_child(widget, IDC_TEXTDLG_EDIT);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_TEXTDLG_PUSHBOX_OK);
	if (ctrl)
		widget_destroy(ctrl);

	xmem_free(ptd);

	SETTEXTDLGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_textdlg_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);

	if (cid == IDC_TEXTDLG_PUSHBOX_OK)
	{
		textdlg_on_ok(widget);
	}
}

void hand_textdlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	xsize_t xs;
	xrect_t xr;
	res_win_t ctrl;

	xs.fw = TEXTDLG_BUTTON_WIDTH;
	xs.fh = TEXTDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= xs.h;

	ctrl = widget_get_child(widget, IDC_TEXTDLG_EDIT);
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

	ctrl = widget_get_child(widget, IDC_TEXTDLG_PUSHBOX_OK);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
		widget_update(ctrl);
	}

	widget_erase(widget, NULL);
}

void hand_textdlg_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	visual_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc_brim = { 0 };
	xcolor_t xc_core = { 0 };
	xrect_t xr,xr_bar;
	xsize_t xs;

	canvas_t canv;
	drawing_interface ifv = {0};

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	xs.fw = TEXTDLG_BUTTON_WIDTH;
	xs.fh = TEXTDLG_BUTTON_HEIGHT;
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

/***************************************************************************************/
res_win_t textdlg_create(const tchar_t* title, string_t var, res_win_t owner)
{
	if_event_t ev = { 0 };
	res_win_t dlg;
	xrect_t xr = { 0 };
	clr_mod_t clr;

	ev.param = (void*)var;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_textdlg_create)
		EVENT_ON_DESTROY(hand_textdlg_destroy)

		EVENT_ON_PAINT(hand_textdlg_paint)

		EVENT_ON_SIZE(hand_textdlg_size)

		EVENT_ON_MENU_COMMAND(hand_textdlg_menu_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	dlg = widget_create(NULL, WD_STYLE_DIALOG, &xr, owner, &ev);
	widget_set_owner(dlg, owner);
	widget_set_user_id(dlg, IDC_TEXTDLG);
	widget_set_title(dlg, title);

	textdlg_popup_size(dlg, RECTSIZE(&xr));
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

void textdlg_popup_size(res_win_t widget, xsize_t* pxs)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);

	pxs->fw = DEF_TOUCH_SPAN * 10 + TEXTDLG_BUTTON_WIDTH;
	pxs->fh = DEF_TOUCH_SPAN * 10 + TEXTDLG_BUTTON_HEIGHT;

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

