/***********************************************************************
Easily xDesign v3.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xDesign document

	@module	xDesign implement file

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

#include "stdafx.h"
#include "EasilyResource.h"
#include "EasilyModule.h"
#include "MainFrame.h"
#include "TagTextDialog.h"

#define IDC_TAGDLG_TAG			2000
#define IDC_TAGDLG_OK			2001
#define IDC_TAGDLG_SETTING		2002
#define IDC_TAGDLG_RESET		2003

#define TAGDLG_BAR_HEIGHT		(float)10 //TM
#define TAGDLG_BAR_SPLIT		(float)2 //TM
#define TAGDLG_BTN_HEIGHT		(float)8 //tm
#define TAGDLG_BTN_WIDTH		(float)12 //tm


typedef struct _tagTagDlgDelta{
	res_win_t tagctrl;

	TAGTEXTDLG_PARAM* ptp;
}tagTagDlgDelta;

#define GETTAGDLGDELTA(ph) 	(tagTagDlgDelta*)widget_get_user_delta(ph)
#define SETTAGDLGDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

void TagtextDlg_OnOK(res_win_t widget)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	LINKPTR tag = tagctrl_fetch(ptd->tagctrl);
	
	int len = format_tag_text(tag, NULL, MAX_LONG);
	tchar_t* buf = xsalloc(len + 1);
	format_tag_text(tag, buf, len);

	XHANDLE vs = (ptd->ptp) ? ptd->ptp->vs_tag : NULL;

	if (vs)
	{
		varstr_cpy(vs, buf, len);
	}

	xsfree(buf);

	widget_close(widget, 1);
}

void TagtextDlg_OnSetting(res_win_t widget)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	tagctrl_settingtag(ptd->tagctrl);
}

void TagtextDlg_OnReset(res_win_t widget)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	tagctrl_resettag(ptd->tagctrl);
}

/****************************************************************************/
int TagtextDlg_OnCreate(res_win_t widget, void* data)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	res_dc_t rdc;
	xhand_t canv;
	xrect_t xr, xr_box, xr_btn;
	xsize_t xs;
	int nSplit;
	res_win_t pushbox;

	link_t_ptr ptr_tag;

	ptd = (tagTagDlgDelta*)xmem_alloc(sizeof(tagTagDlgDelta));
	xmem_zero((void*)ptd, sizeof(tagTagDlgDelta));

	ptd->ptp = (TAGTEXTDLG_PARAM*)data;

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
	xr_box.y = xr.y;
	xr_box.h = xr.h - xs.cy;

	ptd->tagctrl = tagctrl_create(NULL, WD_STYLE_CONTROL, &xr_box, widget);
	widget_set_user_id(ptd->tagctrl, IDC_TAGDLG_TAG);

	xface_t xa;
	widget_get_xface(ptd->tagctrl, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_LINEBREAK);
	widget_set_xface(ptd->tagctrl, &xa);

	ptr_tag = create_tag_doc();
	
	XHANDLE vs = (ptd->ptp) ? ptd->ptp->vs_tag : NULL;
	if (vs)
	{
		parse_tag_text(ptr_tag, varstr_ptr(vs), varstr_len(vs));
	}

	tagctrl_attach(ptd->tagctrl, ptr_tag);
	widget_show(ptd->tagctrl, WD_SHOW_NORMAL);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	nSplit = xs.cx;
	xs.fx = TAGDLG_BTN_WIDTH;
	xs.fy = TAGDLG_BTN_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_btn.x = xr.x + xr.w - nSplit - xs.cx;
	xr_btn.y = xr.y + xr.h / 2 - xs.cy / 2;
	xr_btn.w = xs.cx;
	xr_btn.h = xs.cy;

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_btn, PUSHBOX_TITLE, _T("确定"));
	widget_set_user_id(pushbox, IDC_TAGDLG_OK);
	widget_set_owner(pushbox, widget);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr_btn.x -= (2 * xs.cx + nSplit);
	xr_btn.w = 2 * xs.cx;

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_btn, PUSHBOX_TITLE, _T("设置标记"));
	widget_set_user_id(pushbox, IDC_TAGDLG_SETTING);
	widget_set_owner(pushbox, widget);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr_btn.x -= (2 * xs.cx + nSplit);
	xr_btn.w = 2 * xs.cx;

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_btn, PUSHBOX_TITLE, _T("清除标记"));
	widget_set_user_id(pushbox, IDC_TAGDLG_RESET);
	widget_set_owner(pushbox, widget);
	widget_show(pushbox, WD_SHOW_NORMAL);

	SETTAGDLGDELTA(widget, ptd);

	return 0;
}

void TagtextDlg_OnDestroy(res_win_t widget)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);
	res_win_t pushbox;
	link_t_ptr ptr_tag;

	if (!ptd)
		return;

	pushbox = widget_get_child(widget, IDC_TAGDLG_OK);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_TAGDLG_SETTING);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_TAGDLG_RESET);
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

void TagtextDlg_OnNotice(res_win_t widget, NOTICE* pnt)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	NOTICE_TAG* pne;

	if (!ptd)
		return;

	if (pnt->id == IDC_TAGDLG_TAG)
	{
		pne = (NOTICE_TAG*)pnt;
	}
}

void TagtextDlg_OnCommandColor(res_win_t widget, void* data)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);
	res_win_t pushbox;

	if (!ptd)
		return;

	widget_hand_command_color(widget, (clr_mod_t*)data);

	widget_hand_command_color(ptd->tagctrl, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_TAGDLG_OK);
	if (pushbox)
		widget_hand_command_color(pushbox, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_TAGDLG_SETTING);
	if (pushbox)
		widget_hand_command_color(pushbox, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_TAGDLG_RESET);
	if (pushbox)
		widget_hand_command_color(pushbox, (clr_mod_t*)data);
}

void TagtextDlg_OnMenuCommand(res_win_t widget, int code, int cid, void* data)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	if (!ptd)
		return;

	switch (cid)
	{
	case IDC_TAGDLG_OK:
		TagtextDlg_OnOK(widget);
		break;
	case IDC_TAGDLG_SETTING:
		TagtextDlg_OnSetting(widget);
		break;
	case IDC_TAGDLG_RESET:
		TagtextDlg_OnReset(widget);
		break;
	}
}

void TagtextDlg_OnSize(res_win_t widget, int code, const xsize_t* prs)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);
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
	xr_box.y = xr.y;
	xr_box.h = xr.h - xs.cy;

	widget_move(ptd->tagctrl, RECTPOINT(&xr_box));
	widget_size(ptd->tagctrl, RECTSIZE(&xr_box));
	widget_update_client(ptd->tagctrl);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	nSplit = xs.cx;
	xs.fx = TAGDLG_BTN_WIDTH;
	xs.fy = TAGDLG_BTN_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_btn.x = xr.x + xr.w - nSplit - xs.cx;
	xr_btn.y = xr.y + xr.h / 2 - xs.cy / 2;
	xr_btn.w = xs.cx;
	xr_btn.h = xs.cy;

	pushbox = widget_get_child(widget, IDC_TAGDLG_OK);
	if (pushbox)
	{
		widget_move(pushbox, RECTPOINT(&xr_btn));
		widget_update_client(pushbox);
	}

	xr_btn.x -= (2 * xs.cx + nSplit);
	xr_btn.w = 2 * xs.cx;

	pushbox = widget_get_child(widget, IDC_TAGDLG_SETTING);
	if (pushbox)
	{
		widget_move(pushbox, RECTPOINT(&xr_btn));
		widget_update_client(pushbox);
	}

	xr_btn.x -= (2 * xs.cx + nSplit);
	xr_btn.w = 2 * xs.cx;

	pushbox = widget_get_child(widget, IDC_TAGDLG_RESET);
	if (pushbox)
	{
		widget_move(pushbox, RECTPOINT(&xr_btn));
		widget_update_client(pushbox);
	}

	widget_invalid(widget, NULL, 0);
}

void TagtextDlg_OnSetFocus(res_win_t widget, res_win_t wt)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	if (widget_is_valid(ptd->tagctrl))
	{
		widget_set_focus(ptd->tagctrl);
	}
}

void TagtextDlg_OnErase(res_win_t widget, res_dc_t dc)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);

	if (!ptd)
		return;
}

void TagtextDlg_OnPaint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	tagTagDlgDelta* ptd = GETTAGDLGDELTA(widget);
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
	xr_bar.y = xr.y + xr.h - xs.cy;
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
res_win_t TagtextDlg_Create(const tchar_t* title, TAGTEXTDLG_PARAM* ptp)
{
	res_win_t widget;
	if_event_t ev = { 0 };

	ev.data = (void*)ptp;
	ev.pf_on_create = TagtextDlg_OnCreate;
	ev.pf_on_destroy = TagtextDlg_OnDestroy;
	ev.pf_on_set_focus = TagtextDlg_OnSetFocus;

	ev.pf_on_erase = TagtextDlg_OnErase;
	ev.pf_on_paint = TagtextDlg_OnPaint;

	ev.pf_on_size = TagtextDlg_OnSize;
	ev.pf_on_command_color = TagtextDlg_OnCommandColor;
	ev.pf_on_menu_command = TagtextDlg_OnMenuCommand;
	ev.pf_on_notice = TagtextDlg_OnNotice;

	xrect_t xr = { 0 };

	xr.x = 0;
	xr.y = 0;
	xr.w = 400;
	xr.h = 300;

	widget = widget_create(title, WD_STYLE_DIALOG | WD_STYLE_CLOSEBOX, &xr, g_hMain, &ev);

	if (!widget)
	{
		return 0;
	}

	clr_mod_t clr;
	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);
	widget_send_command(widget, COMMAND_COLOR, IDC_PARENT, (void*)&clr);

	widget_center_window(widget, g_hMain);
	widget_show(widget, WD_SHOW_NORMAL);
	widget_update_window(widget);
	widget_update_client(widget);

	return widget;
}

