/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text dialog document

	@module	textdlg.c | text dialog widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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
#include "xdcctrl.h"
#include "xdcbox.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcgdi.h"
#include "xdccanv.h"
#include "xdcres.h"

#define IDC_TEXTDLG_TXT			10
#define IDC_PUSHBOX_CODE		11
#define IDC_PUSHBOX_IMAGE		12
#define IDC_PUSHBOX_SIGN		13
#define IDC_PUSHBOX_HERF		14

#define TEXTDLG_BAR_HEIGHT		(float)8 //TM
#define TEXTDLG_BAR_SPLIT		(float)1 //TM
#define TEXTDLG_BTN_HEIGHT		(float)6 //tm
#define TEXTDLG_BTN_WIDTH		(float)6 //tm

typedef struct _textdlg_delta_t{
	res_win_t textctrl;
}textdlg_delta_t;


#define GETTEXTDLGDELTA(ph) 	(textdlg_delta_t*)widget_get_user_delta(ph)
#define SETTEXTDLGDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)
/*****************************************************************************/
int hand_textdlg_create(res_win_t widget, void* data)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);

	res_dc_t rdc;
	xhand_t canv;
	xrect_t xr, xr_box, xr_btn;
	xsize_t xs;
	int nSplit;
	res_win_t pushbox;

	link_t_ptr ptr_txt;

	ptd = (textdlg_delta_t*)xmem_alloc(sizeof(textdlg_delta_t));
	xmem_zero((void*)ptd, sizeof(textdlg_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_set_canvas(widget, canv);

	xs.fx = TEXTDLG_BAR_SPLIT;
	xs.fy = TEXTDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y + xs.cy;
	xr_box.h = xr.h - xs.cy;

	ptd->textctrl = textctrl_create(NULL, WD_STYLE_CONTROL, &xr_box, widget);
	widget_set_user_id(ptd->textctrl, IDC_TEXTDLG_TXT);
	
	ptr_txt = create_text_doc();
	textctrl_attach(ptd->textctrl, ptr_txt);
	widget_show(ptd->textctrl, WD_SHOW_NORMAL);

	xr.h = xs.cy;
	nSplit = xs.cx;
	xs.fx = TEXTDLG_BTN_WIDTH;
	xs.fy = TEXTDLG_BTN_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_btn.x = xr.x + xr.w - nSplit - xs.cx;
	xr_btn.y = xr.y + xr.h / 2 - xs.cy / 2;
	xr_btn.w = xs.cx;
	xr_btn.h = xs.cy;

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_btn, PUSHBOX_TITLE, NULL);
	widget_set_user_id(pushbox, IDC_PUSHBOX_HERF);
	widget_set_owner(pushbox, widget);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_btn, PUSHBOX_TITLE, NULL);
	widget_set_user_id(pushbox, IDC_PUSHBOX_SIGN);
	widget_set_owner(pushbox, widget);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_btn, PUSHBOX_TITLE, NULL);
	widget_set_user_id(pushbox, IDC_PUSHBOX_IMAGE);
	widget_set_owner(pushbox, widget);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_btn, PUSHBOX_TITLE, NULL);
	widget_set_user_id(pushbox, IDC_PUSHBOX_CODE);
	widget_set_owner(pushbox, widget);
	widget_show(pushbox, WD_SHOW_NORMAL);

	SETTEXTDLGDELTA(widget, ptd);

	return 0;
}

void hand_textdlg_destroy(res_win_t widget)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	res_win_t pushbox;
	link_t_ptr ptr_txt;

	if (!ptd)
		return;

	pushbox = widget_get_child(widget, IDC_PUSHBOX_HERF);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_SIGN);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_IMAGE);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_CODE);
	if (pushbox)
		widget_destroy(pushbox);

	ptr_txt = textctrl_fetch(ptd->textctrl);
	if (ptr_txt)
		destroy_text_doc(ptr_txt);

	widget_destroy(ptd->textctrl);

	destroy_display_canvas(widget_get_canvas(widget));

	SETTEXTDLGDELTA(widget, 0);
	xmem_free(ptd);
}

void hand_textdlg_notice(res_win_t widget, NOTICE* pnt)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	NOTICE_TEXT* pne;
	
	if (!ptd)
		return;

	if (pnt->id == IDC_TEXTDLG_TXT)
	{
		pne = (NOTICE_TEXT*)pnt;
	}
}

void hand_textdlg_command_color(res_win_t widget, void* data)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	res_win_t pushbox;

	if (!ptd)
		return;

	widget_hand_command_color(widget, (clr_mod_t*)data);

	widget_hand_command_color(ptd->textctrl, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_HERF);
	if (pushbox)
		widget_hand_command_color(pushbox, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_SIGN);
	if (pushbox)
		widget_hand_command_color(pushbox, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_IMAGE);
	if (pushbox)
		widget_hand_command_color(pushbox, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_CODE);
	if (pushbox)
		widget_hand_command_color(pushbox, (clr_mod_t*)data);
}

void hand_textdlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	xsize_t xs;
	xrect_t xr, xr_box, xr_btn;
	int nSplit;
	res_win_t pushbox;

	if (!ptd)
		return;

	xs.fx = TEXTDLG_BAR_SPLIT;
	xs.fy = TEXTDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y + xs.cy;
	xr_box.h = xr.h - xs.cy;

	widget_move(ptd->textctrl, RECTPOINT(&xr_box));
	widget_size(ptd->textctrl, RECTSIZE(&xr_box));
	widget_update_client(ptd->textctrl);

	xr.h = xs.cy;
	nSplit = xs.cx;
	xs.fx = TEXTDLG_BTN_WIDTH;
	xs.fy = TEXTDLG_BTN_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_btn.x = xr.x + xr.w - nSplit - xs.cx;
	xr_btn.y = xr.y + xr.h / 2 - xs.cy / 2;
	xr_btn.w = xs.cx;
	xr_btn.h = xs.cy;

	pushbox = widget_get_child(widget, IDC_PUSHBOX_HERF);
	widget_move(pushbox, RECTPOINT(&xr_btn));
	widget_update_client(pushbox);

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_SIGN);
	widget_move(pushbox, RECTPOINT(&xr_btn));
	widget_update_client(pushbox);

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_IMAGE);
	widget_move(pushbox, RECTPOINT(&xr_btn));
	widget_update_client(pushbox);

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_CODE);
	widget_move(pushbox, RECTPOINT(&xr_btn));
	widget_update_client(pushbox);

	widget_invalid(widget, NULL, 0);
}

void hand_textdlg_set_focus(res_win_t widget, res_win_t wt)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);

	if (!ptd)
		return;

	if (widget_is_valid(ptd->textctrl))
		widget_set_focus(ptd->textctrl);
}

void hand_textdlg_erase(res_win_t widget, res_dc_t dc)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);

	if (!ptd)
		return;
}

void hand_textdlg_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	res_dc_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr,xr_bar;
	xsize_t xs;

	xhand_t canv;

	if (!ptd)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	xs.fx = 0;
	xs.fy = TEXTDLG_BAR_HEIGHT;

	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y;
	xr_bar.w = xr.w;
	xr_bar.h = xs.cy;

	xscpy(xg.brim_color, xb.color);
	xscpy(xg.core_color, xb.color);
	lighten_xgradi(&xg, DEF_MIDD_DARKEN);
	xscpy(xg.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	gradient_rect_raw(rdc, &xg, &xr_bar);

	end_display_paint(canv, dc, pxr);
}

/***************************************************************************************/
res_win_t textdlg_create(res_win_t widget, dword_t style, const xrect_t* pxr, const tchar_t* title)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_textdlg_create;
	ev.pf_on_destroy = hand_textdlg_destroy;
	ev.pf_on_set_focus = hand_textdlg_set_focus;

	ev.pf_on_erase = hand_textdlg_erase;
	ev.pf_on_paint = hand_textdlg_paint;

	ev.pf_on_size = hand_textdlg_size;
	ev.pf_on_command_color = hand_textdlg_command_color;
	ev.pf_on_notice = hand_textdlg_notice;

	return widget_create(title, style, pxr, widget, &ev);
}

void textdlg_set_text(res_win_t widget, const tchar_t* token)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	link_t_ptr txt;

	XDC_ASSERT(ptd != NULL);

	txt = textctrl_fetch(ptd->textctrl);
	if (!txt)
		return;

	parse_text_doc(txt, token, -1);

	textctrl_redraw(widget, 1);
}

int textdlg_get_text(res_win_t widget, tchar_t* buf, int max)
{
	textdlg_delta_t* ptd = GETTEXTDLGDELTA(widget);
	link_t_ptr txt;

	XDC_ASSERT(ptd != NULL);

	txt = textctrl_fetch(ptd->textctrl);
	if (!txt)
		return 0;

	return format_text_doc(txt, buf, max);
}