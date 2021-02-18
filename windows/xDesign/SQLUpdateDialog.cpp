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
#include "_Define.h"
#include "_Module.h"
#include "_Database.h"
#include "SQLUpdateDialog.h"

#define IDC_SQLUPDATEDLG_EDIT		2001
#define IDC_SQLUPDATEDLG_OK		2002
#define IDC_SQLUPDATEDLG_CANCEL	2003

#define SQLUPDATEDLG_BAR_HEIGHT	(float)10 //TM
#define SQLUPDATEDLG_BAR_SPLIT	(float)2 //TM
#define SQLUPDATEDLG_BTN_WIDTH	(float)12 //TM

typedef struct tagSQLUpdateDlgDelta{
	res_win_t hEdit;
	res_win_t hPushOK;
	res_win_t hPushCancel;

	SQLUPDATEDLG_PARAM* pSel;
}SQLUpdateDlgDelta;


#define GETSQLUPDATEDLGDELTA(ph) 		(SQLUpdateDlgDelta*)widget_get_user_delta(ph)
#define SETSQLUPDATEDLGDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

void SQLUpdateDlg_OnOK(res_win_t widget)
{
	SQLUpdateDlgDelta* pdt = GETSQLUPDATEDLGDELTA(widget);

	int ret = 0;

	LINKPTR ptrGrid = pdt->pSel->ptrGrid;

	DBCTX* pct = DBOpen(pdt->pSel->sz_conn);
	if (!pct)
		return;

	if (DBUpdate(pct, ptrGrid))
	{
		refresh_grid_rowset(ptrGrid);
		ret = 1;
	}

	DBClose(pct);

	widget_close(widget, ret);
}

void SQLUpdateDlg_OnCancel(res_win_t widget)
{
	SQLUpdateDlgDelta* pdt = GETSQLUPDATEDLGDELTA(widget);

	widget_close(widget, 0);
}

/*********************************************************************************************/
int SQLUpdateDlg_OnCreate(res_win_t widget, void* data)
{
	SQLUpdateDlgDelta* pdt;

	widget_hand_create(widget);

	pdt = (SQLUpdateDlgDelta*)xmem_alloc(sizeof(SQLUpdateDlgDelta));
	pdt->pSel = (SQLUPDATEDLG_PARAM*)data;

	XDL_ASSERT(pdt->pSel);

	SETSQLUPDATEDLGDELTA(widget, (void*)pdt);

	xrect_t xr;
	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fw = SQLUPDATEDLG_BAR_SPLIT;
	xs.fh = SQLUPDATEDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.h;

	pdt->hEdit = editbox_create(widget, WD_STYLE_CONTROL, &xr_reg);
	XDL_ASSERT(pdt->hEdit);
	widget_set_user_id(pdt->hEdit, IDC_SQLUPDATEDLG_EDIT);
	widget_set_owner(pdt->hEdit, widget);

	LINKPTR ptr_grid = pdt->pSel->ptrGrid;

	int len = format_grid_exec_sql(ptr_grid, NULL, MAX_LONG);
	tchar_t* sz_sql = xsalloc(len + 1);
	format_grid_exec_sql(ptr_grid, sz_sql, len);
	
	editbox_set_text(pdt->hEdit, sz_sql);
	xsfree(sz_sql);

	widget_show(pdt->hEdit, WS_SHOW_NORMAL);

	xr.y = xr.y + xr.h - xs.h;
	xr.h = xs.h;

	xs.fw = SQLUPDATEDLG_BTN_WIDTH;
	xs.fh = SQLUPDATEDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.w - xs.h;
	xr_push.w = xs.w;
	xr_push.y = xr.y + xs.h;
	xr_push.h = xr.h - 2 * xs.h;
	pdt->hPushCancel = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_user_id(pdt->hPushCancel, IDC_SQLUPDATEDLG_CANCEL);
	widget_set_owner(pdt->hPushCancel, widget);
	pushbox_set_text(pdt->hPushCancel, _T("取消"), -1);
	widget_show(pdt->hPushCancel, WS_SHOW_NORMAL);

	xr_push.x -= (xs.w + xs.h);
	pdt->hPushOK = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_owner(pdt->hPushOK, widget);
	widget_set_user_id(pdt->hPushOK, IDC_SQLUPDATEDLG_OK);
	pushbox_set_text(pdt->hPushOK, _T("确定"), -1);
	widget_show(pdt->hPushOK, WS_SHOW_NORMAL);

	return 0;
}

void SQLUpdateDlg_OnDestroy(res_win_t widget)
{
	SQLUpdateDlgDelta* pdt = GETSQLUPDATEDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	if (widget_is_valid(pdt->hEdit))
	{
		widget_destroy(pdt->hEdit);
	}

	if (widget_is_valid(pdt->hPushCancel))
	{
		widget_destroy(pdt->hPushCancel);
	}

	if (widget_is_valid(pdt->hPushOK))
	{
		widget_destroy(pdt->hPushOK);
	}

	xmem_free(pdt);

	widget_hand_destroy(widget);
}

void SQLUpdateDlg_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	SQLUpdateDlgDelta* pdt = GETSQLUPDATEDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fw = SQLUPDATEDLG_BAR_SPLIT;
	xs.fh = SQLUPDATEDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.h;

	widget_move(pdt->hEdit, RECTPOINT(&xr_reg));
	widget_size(pdt->hEdit, RECTSIZE(&xr_reg));
	widget_update(pdt->hEdit);

	xr.y = xr.y + xr.h - xs.h;
	xr.h = xs.h;

	xs.fw = SQLUPDATEDLG_BTN_WIDTH;
	xs.fh = SQLUPDATEDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.w - xs.h;
	xr_push.w = xs.w;
	xr_push.y = xr.y + xs.h;
	xr_push.h = xr.h - 2 * xs.h;

	widget_move(pdt->hPushCancel, RECTPOINT(&xr_push));
	widget_update(pdt->hPushCancel);

	xr_push.x -= (xs.w + xs.h);

	widget_move(pdt->hPushOK, RECTPOINT(&xr_push));
	widget_update(pdt->hPushOK);

	widget_erase(widget, NULL);
}

void SQLUpdateDlg_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	SQLUpdateDlgDelta* pdt = GETSQLUPDATEDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	switch (cid)
	{
	case IDC_SQLUPDATEDLG_OK:
		SQLUpdateDlg_OnOK(widget);
		break;
	case IDC_SQLUPDATEDLG_CANCEL:
		SQLUpdateDlg_OnCancel(widget);
		break;
	}
}

void SQLUpdateDlg_OnPaint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	SQLUpdateDlgDelta* pdt = GETSQLUPDATEDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	xbrush_t xb = { 0 };
	xcolor_t xc_brim, xc_core;
	xrect_t xr, xr_bar;
	xsize_t xs;

	visual_t rdc;
	canvas_t canv;
	drawing_interface ifv = {0};

	widget_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	xs.fw = SQLUPDATEDLG_BAR_SPLIT;
	xs.fh = SQLUPDATEDLG_BAR_HEIGHT;

	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y + xr.h - xs.h;
	xr_bar.w = xr.w;
	xr_bar.h = xs.h;

	parse_xcolor(&xc_brim, xb.color);
	parse_xcolor(&xc_core, xb.color);
	lighten_xcolor(&xc_core, DEF_MIDD_DARKEN);

	(*ifv.pf_gradient_rect)(ifv.ctx, &xc_brim, &xc_core, GDI_ATTR_GRADIENT_VERT, &xr_bar);

	
	end_canvas_paint(canv, dc, pxr);
}

/************************************************************************************************************/
res_win_t SQLUpdateDlg_Create(const TCHAR* title, SQLUPDATEDLG_PARAM* ppd)
{
	res_win_t widget;

	if_event_t ev = { 0 };

	ev.param = (void*)ppd;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(SQLUpdateDlg_OnCreate)
	EVENT_ON_DESTROY(SQLUpdateDlg_OnDestroy)

	EVENT_ON_SIZE(SQLUpdateDlg_OnSize)
	EVENT_ON_PAINT(SQLUpdateDlg_OnPaint)

	EVENT_ON_MENU_COMMAND(SQLUpdateDlg_OnMenuCommand)

	EVENT_ON_NC_IMPLEMENT
	EVENT_END_DISPATH

	xrect_t xr = { 0 };

	xr.x = 0;
	xr.y = 0;
	xr.w = 500;
	xr.h = 400;

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
	parse_xcolor(&clr.clr_ico, g_face[g_indFace].ico);

	widget_set_color_mode(widget, &clr);

	widget_center_window(widget, g_hMain);
	widget_update(widget);
	widget_show(widget, WS_SHOW_NORMAL);

	return widget;
}