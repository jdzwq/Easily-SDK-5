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
#include "EasilyModule.h"
#include "MainFrame.h"
#include "ProperDialog.h"

#define IDC_PROPERDLG_PROPER	2001
#define IDC_PROPERDLG_OK		2002
#define IDC_PROPERDLG_CANCEL	2003

#define PROPERDLG_BAR_HEIGHT	(float)10 //MM
#define PROPERDLG_BAR_SPLIT		(float)2 //MM
#define PROPERDLG_BTN_WIDTH		(float)12 //MM

typedef struct tagProperDlgDelta{
	res_win_t hProper;
	res_win_t hPushOK;
	res_win_t hPushCancel;

	PROPERDLG_PARAM* pData;
}ProperDlgDelta;

#define GETPROPERDLGDELTA(ph) 		(ProperDlgDelta*)widget_get_user_delta(ph)
#define SETPROPERDLGDELTA(ph,ptd)	widget_set_user_delta(ph,(void*)ptd)


void ProperDlg_OnOK(res_win_t widget)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	widget_close(widget, 1);
}

void ProperDlg_OnCancel(res_win_t widget)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	widget_close(widget, 0);
}

int ProperDlg_OnCreate(res_win_t widget, void* data)
{
	ProperDlgDelta* pdt;

	pdt = (ProperDlgDelta*)xmem_alloc(sizeof(ProperDlgDelta));

	XDC_ASSERT(data != NULL);
	pdt->pData = (PROPERDLG_PARAM*)data;

	SETPROPERDLGDELTA(widget, pdt);

	res_dc_t rdc = widget_client_dc(widget);
	xhand_t canv = create_display_canvas(rdc);
	widget_attach_canvas(widget, canv);
	widget_release_dc(widget, rdc);

	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = PROPERDLG_BAR_SPLIT;
	xs.fy = PROPERDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.cy;

	pdt->hProper = properctrl_create(NULL, WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_PROPERDLG_PROPER);
	widget_set_owner(pdt->hProper, widget);

	properctrl_attach(pdt->hProper, pdt->pData->ptrProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	xs.fx = PROPERDLG_BTN_WIDTH;
	xs.fy = PROPERDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - xs.cy;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xs.cy;
	xr_push.h = xr.h - 2 * xs.cy;
	pdt->hPushCancel = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_user_id(pdt->hPushCancel, IDC_PROPERDLG_CANCEL);
	widget_set_owner(pdt->hPushCancel, widget);
	pushbox_set_text(pdt->hPushCancel, _T("取消"), -1);
	widget_show(pdt->hPushCancel, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);
	pdt->hPushOK = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_owner(pdt->hPushOK, widget);
	widget_set_user_id(pdt->hPushOK, IDC_PROPERDLG_OK);
	pushbox_set_text(pdt->hPushOK, _T("确定"), -1);
	widget_show(pdt->hPushOK, WD_SHOW_NORMAL);

	return 0;
}

void ProperDlg_OnDestroy(res_win_t widget)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (widget_is_valid(pdt->hProper))
	{
		widget_destroy(pdt->hProper);
	}

	if (widget_is_valid(pdt->hPushCancel))
	{
		widget_destroy(pdt->hPushCancel);
	}

	if (widget_is_valid(pdt->hPushOK))
	{
		widget_destroy(pdt->hPushOK);
	}

	destroy_display_canvas(widget_get_canvas(widget));

	xmem_free(pdt);
}

void ProperDlg_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = PROPERDLG_BAR_SPLIT;
	xs.fy = PROPERDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.cy;

	widget_move(pdt->hProper, RECTPOINT(&xr_reg));
	widget_size(pdt->hProper, RECTSIZE(&xr_reg));
	widget_update_client(pdt->hProper);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	xs.fx = PROPERDLG_BTN_WIDTH;
	xs.fy = PROPERDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - xs.cy;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xs.cy;
	xr_push.h = xr.h - 2 * xs.cy;

	widget_move(pdt->hPushCancel, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushCancel);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushOK, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushOK);

	widget_invalid(widget, NULL, 0);
}

void ProperDlg_OnPaint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	res_dc_t rdc;
	xbrush_t xb = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr, xr_bar;
	xsize_t xs;

	xhand_t canv;

	widget_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	xs.fx = PROPERDLG_BAR_SPLIT;
	xs.fy = PROPERDLG_BAR_HEIGHT;

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

void ProperDlg_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);
	
	XDC_ASSERT(pdt != NULL);

	if (phdr->id == IDC_PROPERDLG_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
	}
}

static int __stdcall _ProperDlg_SetChildColor(res_win_t widget, long_ptr pv)
{
	widget_send_command(widget, COMMAND_COLOR, IDC_PARENT, (void*)pv);

	return 1;
}

void ProperDlg_OnCommandColor(res_win_t widget, void* data)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	widget_set_color_mode(widget, (clr_mod_t*)data);
		
	widget_enum_child(widget, _ProperDlg_SetChildColor, (long_ptr)data);
}

void ProperDlg_OnCommandSwitch(res_win_t widget, bool_t bkey)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	widget_send_command(pdt->hProper, COMMAND_SWITCH, IDC_PARENT, (void*)bkey);
}

void ProperDlg_OnMenuCommand(res_win_t widget, int code, int cid, void* data)
{
	ProperDlgDelta* pdt = GETPROPERDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	switch (cid)
	{
	case IDC_PROPERDLG_OK:
		ProperDlg_OnOK(widget);
		break;
	case IDC_PROPERDLG_CANCEL:
		ProperDlg_OnCancel(widget);
		break;
	}
}

/***************************************************************************************************/
res_win_t ProperDlg_Create(const tchar_t* title, PROPERDLG_PARAM* ppd)
{
	res_win_t widget;

	if_event_t ev = { 0 };

	ev.data = (void*)ppd;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(ProperDlg_OnCreate)
	EVENT_ON_DESTROY(ProperDlg_OnDestroy)

	EVENT_ON_SIZE(ProperDlg_OnSize)
	EVENT_ON_PAINT(ProperDlg_OnPaint)

	EVENT_ON_NOTICE(ProperDlg_OnNotice)
	EVENT_ON_COMMAND_COLOR(ProperDlg_OnCommandColor)
	EVENT_ON_COMMAND_SWITCH(ProperDlg_OnCommandSwitch)
	EVENT_ON_MENU_COMMAND(ProperDlg_OnMenuCommand)

	EVENT_END_DISPATH

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
	widget_send_command(widget, COMMAND_SWITCH, IDC_PARENT, (void*)ppd->bMode);

	widget_center_window(widget, g_hMain);
	widget_show(widget, WD_SHOW_NORMAL);
	widget_update_window(widget);
	widget_update_client(widget);

	return widget;
}