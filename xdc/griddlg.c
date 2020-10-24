/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc grid dialog document

	@module	griddlg.c | implement file

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
#include "xdcctrl.h"

#define IDC_GRIDDLG_GRID			10
#define IDC_GRIDDLG_PUSHBOX_OK		11
#define IDC_GRIDDLG_EDITBOX			12

#define GRIDDLG_BUTTON_HEIGHT		(float)8 //tm
#define GRIDDLG_BUTTON_WIDTH		(float)12 //tm
#define GRIDDLG_EDITBOX_WIDTH		(float)20 //tm

typedef struct _griddlg_delta_t{
	link_t_ptr grid;
	link_t_ptr* prow;
}griddlg_delta_t;

#define GETGRIDDLGDELTA(ph) 	(griddlg_delta_t*)widget_get_user_delta(ph)
#define SETGRIDDLGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/**********************************************************************************/
void griddlg_on_ok(res_win_t widget)
{
	griddlg_delta_t* ptd = GETGRIDDLGDELTA(widget);
	res_win_t ctrl;

	ctrl = widget_get_child(widget, IDC_GRIDDLG_GRID);
	(*ptd->prow) = gridctrl_get_focus_row(ctrl);

	widget_close(widget, 1);
}

void griddlg_on_find(res_win_t widget)
{
	res_win_t ctrl;
	tchar_t token[RES_LEN];

	ctrl = widget_get_child(widget, IDC_GRIDDLG_EDITBOX);
	editbox_get_text(ctrl, token, RES_LEN);

	ctrl = widget_get_child(widget, IDC_GRIDDLG_GRID);
	gridctrl_find(ctrl, token);
}
/**********************************************************************************/
int hand_griddlg_create(res_win_t widget, void* data)
{
	griddlg_delta_t* ptd;

	xrect_t xr;
	xsize_t xs;
	res_win_t gridctrl, pushbox, editbox;

	widget_hand_create(widget);

	ptd = (griddlg_delta_t*)xmem_alloc(sizeof(griddlg_delta_t));
	xmem_zero((void*)ptd, sizeof(griddlg_delta_t));
	ptd->grid = (link_t_ptr)data;

	xs.fw = GRIDDLG_BUTTON_WIDTH;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= xs.h;

	gridctrl = gridctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_owner(gridctrl, widget);
	widget_set_user_id(gridctrl, IDC_GRIDDLG_GRID);

	gridctrl_attach(gridctrl, ptd->grid);
	gridctrl_set_lock(gridctrl, 1);

	widget_show(gridctrl, WS_SHOW_NORMAL);

	xs.fw = GRIDDLG_EDITBOX_WIDTH;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.h;
	xr.h = xs.h;
	xr.x = xr.x;
	xr.w = xs.w;

	xs.fw = DEF_SPLIT_FEED;
	xs.fh = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);

	pt_expand_rect(&xr, -xs.w, -xs.h);

	editbox = editbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_EDITOR, &xr);
	widget_set_user_id(editbox, IDC_GRIDDLG_EDITBOX);
	widget_set_owner(editbox, widget);
	widget_show(editbox, WS_SHOW_NORMAL);

	xs.fw = GRIDDLG_BUTTON_WIDTH;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

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
	widget_set_user_id(pushbox, IDC_GRIDDLG_PUSHBOX_OK);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, GRIDDLG_PUSHBOX_OK, -1);
	widget_show(pushbox, WS_SHOW_NORMAL);

	SETGRIDDLGDELTA(widget, ptd);

	widget_set_focus(editbox);

	return 0;
}

void hand_griddlg_destroy(res_win_t widget)
{
	griddlg_delta_t* ptd = GETGRIDDLGDELTA(widget);
	res_win_t ctrl;

	XDL_ASSERT(ptd != NULL);

	ctrl = widget_get_child(widget, IDC_GRIDDLG_GRID);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_GRIDDLG_PUSHBOX_OK);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_GRIDDLG_EDITBOX);
	if (ctrl)
		widget_destroy(ctrl);

	xmem_free(ptd);

	SETGRIDDLGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_griddlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	griddlg_delta_t* ptd = GETGRIDDLGDELTA(widget);
	xsize_t xs;
	xrect_t xr;
	res_win_t ctrl;

	xs.fw = GRIDDLG_BUTTON_WIDTH;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.h -= xs.h;

	ctrl = widget_get_child(widget, IDC_GRIDDLG_GRID);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
		widget_update(ctrl);
	}

	xs.fw = GRIDDLG_EDITBOX_WIDTH;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.h;
	xr.h = xs.h;
	xr.x = xr.x;
	xr.w = xs.w;

	xs.fw = DEF_SPLIT_FEED;
	xs.fh = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);

	pt_expand_rect(&xr, -xs.w, -xs.h);

	ctrl = widget_get_child(widget, IDC_GRIDDLG_EDITBOX);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
		widget_update(ctrl);
	}

	xs.fw = GRIDDLG_BUTTON_WIDTH;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.h;
	xr.h = xs.h;
	xr.x = xr.x + xr.w - xs.w;
	xr.w = xs.w;

	xs.fw = DEF_SPLIT_FEED;
	xs.fh = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);

	pt_expand_rect(&xr, -xs.w, -xs.h);

	ctrl = widget_get_child(widget, IDC_GRIDDLG_PUSHBOX_OK);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
		widget_update(ctrl);
	}

	widget_erase(widget, NULL);
}

void hand_griddlg_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	griddlg_delta_t* ptd = GETGRIDDLGDELTA(widget);

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

	xs.fw = GRIDDLG_BUTTON_WIDTH;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
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

void hand_griddlg_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	griddlg_delta_t* ptd = GETGRIDDLGDELTA(widget);
	res_win_t ctl;

	switch (cid)
	{
	case IDC_GRIDDLG_PUSHBOX_OK:
		griddlg_on_ok(widget);
		break;
	case IDC_GRIDDLG_EDITBOX:
		if (code == COMMAND_UPDATE)
		{
			griddlg_on_find(widget);
		}
		else if (code == COMMAND_COMMIT)
		{
			widget_post_command(widget, 0, IDC_GRIDDLG_PUSHBOX_OK, 0);
		}
		else if (code == COMMAND_TABORDER)
		{
			ctl = widget_get_child(widget, IDC_GRIDDLG_GRID);
			if (widget_is_valid(ctl))
			{
				switch ((int)data)
				{
				case TABORDER_UP:
					widget_post_key(ctl, KEY_UP);
					break;
				case TABORDER_DOWN:
					widget_post_key(ctl, KEY_DOWN);
					break;
				}
			}
		}
		break;
	}
}

void hand_griddlg_notice(res_win_t widget, NOTICE* pnt)
{
	griddlg_delta_t* ptd = GETGRIDDLGDELTA(widget);

	if (pnt->id == IDC_GRIDDLG_GRID)
	{
		NOTICE_GRID* png = (NOTICE_GRID*)pnt;
		switch (png->code)
		{
		case NC_GRIDDBCLK:
			widget_post_command(widget, 0, IDC_GRIDDLG_PUSHBOX_OK, 0);
			break;
		}
	}
}

/***************************************************************************************/
res_win_t griddlg_create(const tchar_t* title, link_t_ptr ptr, link_t_ptr* prow, res_win_t owner)
{
	if_event_t ev = { 0 };
	res_win_t dlg;
	xrect_t xr = { 0 };
	clr_mod_t clr;
	griddlg_delta_t* ptd;

	ev.param = (void*)ptr;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_griddlg_create)
		EVENT_ON_DESTROY(hand_griddlg_destroy)

		EVENT_ON_PAINT(hand_griddlg_paint)

		EVENT_ON_SIZE(hand_griddlg_size)

		EVENT_ON_MENU_COMMAND(hand_griddlg_menu_command)
		EVENT_ON_NOTICE(hand_griddlg_notice)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	dlg = widget_create(NULL, WD_STYLE_DIALOG, &xr, owner, &ev);
	widget_set_owner(dlg, owner);
	widget_set_user_id(dlg, IDC_GRIDDLG);
	widget_set_title(dlg, title);

	griddlg_popup_size(dlg, RECTSIZE(&xr));
	widget_size(dlg, RECTSIZE(&xr));
	widget_update(dlg);
	widget_center_window(dlg, owner);

	if (widget_is_valid(owner))
	{
		widget_get_color_mode(owner, &clr);
		widget_set_color_mode(dlg, &clr);
	}

	ptd = GETGRIDDLGDELTA(dlg);
	ptd->prow = prow;

	return dlg;
}

void griddlg_popup_size(res_win_t widget, xsize_t* pxs)
{
	griddlg_delta_t* ptd = GETGRIDDLGDELTA(widget);
	xrect_t xr;
	xsize_t xs;
	res_win_t ctrl;

	XDL_ASSERT(ptd->grid != NULL);

	ctrl = widget_get_child(widget, IDC_GRIDDLG_GRID);

	gridctrl_popup_size(ctrl, RECTSIZE(&xr));

	xs.fw = 0;
	xs.fh = GRIDDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xs.w = xr.w;
	xs.h += xr.h;

	widget_adjust_size(widget_get_style(widget), &xs);

	pxs->w = xs.w;
	pxs->h = xs.h;
}

