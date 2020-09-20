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
#include "SQLFetchDialog.h"

#define IDC_SQLFETCHDLG_EDIT		2001
#define IDC_SQLFETCHDLG_OK			2002
#define IDC_SQLFETCHDLG_CANCEL		2003

#define SQLFETCHDLG_BAR_HEIGHT	(float)10 //TM
#define SQLFETCHDLG_BAR_SPLIT	(float)2 //TM
#define SQLFETCHDLG_BTN_WIDTH	(float)12 //TM

typedef struct tagSQLFetchDlgDelta{
	res_win_t hEdit;
	res_win_t hPushOK;
	res_win_t hPushCancel;

	SQLFETCHDLG_PARAM* pSel;
}SQLFetchDlgDelta;


#define GETSQLFETCHDLGDELTA(ph) 		(SQLFetchDlgDelta*)widget_get_user_delta(ph)
#define SETSQLFETCHDLGDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

void SQLFetchDlg_OnOK(res_win_t widget)
{
	SQLFetchDlgDelta* pdt = GETSQLFETCHDLGDELTA(widget);

	LINKPTR ptrGrid = pdt->pSel->ptrGrid;

	int len = editbox_get_text(pdt->hEdit, NULL, MAX_LONG);
	if (!len)
	{
		widget_close(widget, 0);
		return;
	}

	DBCTX* pct = DBOpen(pdt->pSel->sz_conn);

	tchar_t* sz_sql = xsalloc(len + 1);
	editbox_get_text(pdt->hEdit, sz_sql, len);

	LINKPTR ptr_grid = create_grid_doc();

	if (!DBSelect(pct, ptr_grid, sz_sql))
	{
		xsfree(sz_sql);
		destroy_grid_doc(ptr_grid);

		DBClose(pct);

		widget_close(widget, 0);
		return;
	}

	DBClose(pct);

	xsfree(sz_sql);

	LINKPTR rlk_new, rlk;

	clear_grid_rowset(ptrGrid);
	
	rlk = get_next_row(ptr_grid, LINK_FIRST);
	while (rlk)
	{
		rlk_new = insert_row(ptrGrid, LINK_LAST);
		copy_grid_row(ptrGrid, rlk_new, ptr_grid, rlk);
		set_row_state(rlk_new, dsClean);

		rlk = get_next_row(ptr_grid, rlk);
	}

	refresh_grid_rowset(ptrGrid);

	destroy_grid_doc(ptr_grid);

	widget_close(widget, 1);
}

void SQLFetchDlg_OnCancel(res_win_t widget)
{
	SQLFetchDlgDelta* pdt = GETSQLFETCHDLGDELTA(widget);

	widget_close(widget, 0);
}

/*********************************************************************************************/
int SQLFetchDlg_OnCreate(res_win_t widget, void* data)
{
	SQLFetchDlgDelta* pdt;

	widget_hand_create(widget);

	pdt = (SQLFetchDlgDelta*)xmem_alloc(sizeof(SQLFetchDlgDelta));
	pdt->pSel = (SQLFETCHDLG_PARAM*)data;

	XDL_ASSERT(pdt->pSel);

	SETSQLFETCHDLGDELTA(widget, (void*)pdt);

	xrect_t xr;
	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = SQLFETCHDLG_BAR_SPLIT;
	xs.fy = SQLFETCHDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.cy;

	pdt->hEdit = editbox_create(widget, WD_STYLE_CONTROL, &xr_reg);
	XDL_ASSERT(pdt->hEdit);
	widget_set_user_id(pdt->hEdit, IDC_SQLFETCHDLG_EDIT);
	widget_set_owner(pdt->hEdit, widget);

	xface_t xa = { 0 };
	widget_get_xface(pdt->hEdit, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_LINEBREAK);
	widget_set_xface(pdt->hEdit, &xa);

	LINKPTR ptr_grid = pdt->pSel->ptrGrid;

	int len = format_grid_select_sql(ptr_grid, NULL, MAX_LONG);
	tchar_t* sz_sql = xsalloc(len + 1);
	format_grid_select_sql(ptr_grid, sz_sql, len);
	
	editbox_set_text(pdt->hEdit, sz_sql);
	xsfree(sz_sql);

	widget_show(pdt->hEdit, WS_SHOW_NORMAL);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	xs.fx = SQLFETCHDLG_BTN_WIDTH;
	xs.fy = SQLFETCHDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - xs.cy;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xs.cy;
	xr_push.h = xr.h - 2 * xs.cy;
	pdt->hPushCancel = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_user_id(pdt->hPushCancel, IDC_SQLFETCHDLG_CANCEL);
	widget_set_owner(pdt->hPushCancel, widget);
	pushbox_set_text(pdt->hPushCancel, _T("取消"), -1);
	widget_show(pdt->hPushCancel, WS_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);
	pdt->hPushOK = pushbox_create(widget, WD_STYLE_CONTROL, &xr_push);
	widget_set_owner(pdt->hPushOK, widget);
	widget_set_user_id(pdt->hPushOK, IDC_SQLFETCHDLG_OK);
	pushbox_set_text(pdt->hPushOK, _T("确定"), -1);
	widget_show(pdt->hPushOK, WS_SHOW_NORMAL);

	return 0;
}

void SQLFetchDlg_OnDestroy(res_win_t widget)
{
	SQLFetchDlgDelta* pdt = GETSQLFETCHDLGDELTA(widget);

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

void SQLFetchDlg_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	SQLFetchDlgDelta* pdt = GETSQLFETCHDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = SQLFETCHDLG_BAR_SPLIT;
	xs.fy = SQLFETCHDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.cy;

	widget_move(pdt->hEdit, RECTPOINT(&xr_reg));
	widget_size(pdt->hEdit, RECTSIZE(&xr_reg));
	widget_update(pdt->hEdit);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	xs.fx = SQLFETCHDLG_BTN_WIDTH;
	xs.fy = SQLFETCHDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - xs.cy;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xs.cy;
	xr_push.h = xr.h - 2 * xs.cy;

	widget_move(pdt->hPushCancel, RECTPOINT(&xr_push));
	widget_update(pdt->hPushCancel);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushOK, RECTPOINT(&xr_push));
	widget_update(pdt->hPushOK);

	widget_erase(widget, NULL);
}

void SQLFetchDlg_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	SQLFetchDlgDelta* pdt = GETSQLFETCHDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	switch (cid)
	{
	case IDC_SQLFETCHDLG_OK:
		SQLFetchDlg_OnOK(widget);
		break;
	case IDC_SQLFETCHDLG_CANCEL:
		SQLFetchDlg_OnCancel(widget);
		break;
	}
}

void SQLFetchDlg_OnPaint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	SQLFetchDlgDelta* pdt = GETSQLFETCHDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	res_ctx_t rdc;
	xbrush_t xb = { 0 };
	xcolor_t xc_brim, xc_core;
	xrect_t xr, xr_bar;
	xsize_t xs;

	canvas_t canv;

	widget_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	xs.fx = SQLFETCHDLG_BAR_SPLIT;
	xs.fy = SQLFETCHDLG_BAR_HEIGHT;

	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y + xr.h - xs.cy;
	xr_bar.w = xr.w;
	xr_bar.h = xs.cy;

	parse_xcolor(&xc_brim, xb.color);
	parse_xcolor(&xc_core, xb.color);
	lighten_xcolor(&xc_core, DEF_MIDD_DARKEN);

	gradient_rect_raw(rdc, &xc_brim, &xc_core, GDI_ATTR_GRADIENT_VERT, &xr_bar);

	end_canvas_paint(canv, dc, pxr);
}

/************************************************************************************************************/
res_win_t SQLFetchDlg_Create(const TCHAR* title, SQLFETCHDLG_PARAM* ppd)
{
	res_win_t widget;

	if_event_t ev = { 0 };

	ev.param = (void*)ppd;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(SQLFetchDlg_OnCreate)
	EVENT_ON_DESTROY(SQLFetchDlg_OnDestroy)

	EVENT_ON_SIZE(SQLFetchDlg_OnSize)
	EVENT_ON_PAINT(SQLFetchDlg_OnPaint)

	EVENT_ON_MENU_COMMAND(SQLFetchDlg_OnMenuCommand)

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