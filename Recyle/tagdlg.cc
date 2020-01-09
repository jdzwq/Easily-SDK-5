/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tag dialog document

	@module	tagdlg.c | tag dialog widnows implement file

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
#include "xdcbox.h"
#include "xdcctrl.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcgdi.h"
#include "xdccanv.h"
#include "xdcres.h"

#define IDC_TAGDLG_TAG			10
#define IDC_PUSHBOX_CODE		11
#define IDC_PUSHBOX_IMAGE		12
#define IDC_PUSHBOX_SIGN		13
#define IDC_PUSHBOX_HERF		14

#define TAGDLG_BAR_HEIGHT		(float)8 //TM
#define TAGDLG_BAR_SPLIT		(float)1 //TM
#define TAGDLG_BTN_HEIGHT		(float)6 //tm
#define TAGDLG_BTN_WIDTH		(float)6 //tm

typedef struct _tagdlg_delta_t{
	res_win_t tagctrl;
}tagdlg_delta_t;

#define GETTAGDLGDELTA(ph) 	(tagdlg_delta_t*)widget_get_user_delta(ph)
#define SETTAGDLGDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

/****************************************************************************/
int hand_tagdlg_create(res_win_t widget, void* data)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);

	res_dc_t rdc;
	xhand_t canv;
	xrect_t xr, xr_box, xr_btn;
	xsize_t xs;
	int nSplit;
	res_win_t pushbox;

	link_t_ptr ptr_tag;

	ptd = (tagdlg_delta_t*)xmem_alloc(sizeof(tagdlg_delta_t));
	xmem_zero((void*)ptd, sizeof(tagdlg_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_set_canvas(widget, canv);

	xs.fx = TAGDLG_BAR_SPLIT;
	xs.fy = TAGDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y + xs.cy;
	xr_box.h = xr.h - xs.cy;

	ptd->tagctrl = tagctrl_create(NULL, WD_STYLE_CONTROL, &xr_box, widget);
	widget_set_user_id(ptd->tagctrl, IDC_TAGDLG_TAG);

	ptr_tag = create_tag_doc();
	tagctrl_attach(ptd->tagctrl, ptr_tag);
	widget_show(ptd->tagctrl, WD_SHOW_NORMAL);

	xr.h = xs.cy;
	nSplit = xs.cx;
	xs.fx = TAGDLG_BTN_WIDTH;
	xs.fy = TAGDLG_BTN_HEIGHT;
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

	SETTAGDLGDELTA(widget, ptd);

	return 0;
}

void hand_tagdlg_destroy(res_win_t widget)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);
	res_win_t pushbox;
	link_t_ptr ptr_tag;

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

	ptr_tag = tagctrl_detach(ptd->tagctrl);
	if (ptr_tag)
	{
		destroy_tag_doc(ptr_tag);
	}
	widget_destroy(ptd->tagctrl);

	destroy_display_canvas(widget_get_canvas(widget));

	SETTAGDLGDELTA(widget, 0);
	xmem_free(ptd);
}

void hand_tagdlg_notice(res_win_t widget, NOTICE* pnt)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);

	NOTICE_TAG* pne;

	if (!ptd)
		return;

	if (pnt->id == IDC_TAGDLG_TAG)
	{
		pne = (NOTICE_TAG*)pnt;
	}
}

void hand_tagdlg_command_color(res_win_t widget, void* data)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);
	res_win_t pushbox;

	if (!ptd)
		return;

	widget_hand_command_color(widget, (clr_mod_t*)data);

	widget_hand_command_color(ptd->tagctrl, (clr_mod_t*)data);

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

void hand_tagdlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);
	xsize_t xs;
	xrect_t xr, xr_box, xr_btn;
	int nSplit;
	res_win_t pushbox;

	if (!ptd)
		return;

	xs.fx = TAGDLG_BAR_SPLIT;
	xs.fy = TAGDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y + xs.cy;
	xr_box.h = xr.h - xs.cy;

	widget_move(ptd->tagctrl, RECTPOINT(&xr_box));
	widget_size(ptd->tagctrl, RECTSIZE(&xr_box));
	widget_update_client(ptd->tagctrl);

	xr.h = xs.cy;
	nSplit = xs.cx;
	xs.fx = TAGDLG_BTN_WIDTH;
	xs.fy = TAGDLG_BTN_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_btn.x = xr.x + xr.w - nSplit - xs.cx;
	xr_btn.y = xr.y + xr.h / 2 - xs.cy / 2;
	xr_btn.w = xs.cx;
	xr_btn.h = xs.cy;

	pushbox = widget_get_child(widget, IDC_PUSHBOX_HERF);
	if (pushbox)
	{
		widget_move(pushbox, RECTPOINT(&xr_btn));
		widget_update_client(pushbox);
	}

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_SIGN);
	if (pushbox)
	{
		widget_move(pushbox, RECTPOINT(&xr_btn));
		widget_update_client(pushbox);
	}

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_IMAGE);
	if (pushbox)
	{
		widget_move(pushbox, RECTPOINT(&xr_btn));
		widget_update_client(pushbox);
	}

	xr_btn.x -= (xr_btn.w + nSplit);

	pushbox = widget_get_child(widget, IDC_PUSHBOX_CODE);
	if (pushbox)
	{
		widget_move(pushbox, RECTPOINT(&xr_btn));
		widget_update_client(pushbox);
	}

	widget_invalid(widget, NULL, 0);
}

void hand_tagdlg_set_focus(res_win_t widget, res_win_t wt)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);

	if (widget_is_valid(ptd->tagctrl))
	{
		widget_set_focus(ptd->tagctrl);
	}
}

void hand_tagdlg_erase(res_win_t widget, res_dc_t dc)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);

	if (!ptd)
		return;
}

void hand_tagdlg_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);
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

	xs.cx = 0;
	xs.fy = TAGDLG_BAR_HEIGHT;

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
res_win_t tagdlg_create(res_win_t widget, dword_t style, const xrect_t* pxr, const tchar_t* title)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_tagdlg_create;
	ev.pf_on_destroy = hand_tagdlg_destroy;
	ev.pf_on_set_focus = hand_tagdlg_set_focus;

	ev.pf_on_erase = hand_tagdlg_erase;
	ev.pf_on_paint = hand_tagdlg_paint;

	ev.pf_on_size = hand_tagdlg_size;
	ev.pf_on_command_color = hand_tagdlg_command_color;
	ev.pf_on_notice = hand_tagdlg_notice;

	return widget_create(title, style, pxr, widget, &ev);
}

void tagdlg_set_text(res_win_t widget, const tchar_t* token)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);
	link_t_ptr tag;

	XDC_ASSERT(ptd != NULL);

	tag = tagctrl_fetch(ptd->tagctrl);
	if (!tag)
		return;

	parse_tag_text(tag, token, -1);

	tagctrl_redraw(widget, 1);
}

int tagdlg_get_text(res_win_t widget, tchar_t* buf, int max)
{
	tagdlg_delta_t* ptd = GETTAGDLGDELTA(widget);
	link_t_ptr tag;

	XDC_ASSERT(ptd != NULL);

	tag = tagctrl_fetch(ptd->tagctrl);
	if (!tag)
		return 0;

	return format_tag_text(tag, buf, max);
}