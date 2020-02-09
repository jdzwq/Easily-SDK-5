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
#include "SQLViewDialog.h"

#include "_Database.h"
#include "_Project.h"
#include "_Module.h"



#define IDC_SQLVIEWDLG_GRID		2001
#define IDC_SQLVIEWDLG_EDIT		2002
#define IDC_SQLVIEWDLG_OK		2003
#define IDC_SQLVIEWDLG_CANCEL	2004
#define IDC_SQLVIEWDLG_EXEC		2005

#define SQLVIEWDLG_BAR_HEIGHT	(float)10 //TM
#define SQLVIEWDLG_BAR_SPLIT	(float)2 //TM
#define SQLVIEWDLG_BTN_WIDTH	(float)12 //TM

typedef struct _SQLVIEWDLGDELTA{
	res_win_t hEdit;
	res_win_t hGrid;
	res_win_t hPushOK;
	res_win_t hPushCancel;
	res_win_t hPushExec;
	
	splitor_t sp;
	SQLVIEWDLG_PARAM* psp;
}SQLVIEWDLGDELTA;

#define GETSQLVIEWDLGDELTA(ph) 		(SQLVIEWDLGDELTA*)widget_get_user_delta(ph)
#define SETSQLVIEWDLGDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

void SQLViewDlg_OnOK(res_win_t widget)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	string_t vs = (pdt->psp) ? pdt->psp->vs_sql : NULL;

	int len = editbox_get_text(pdt->hEdit, NULL, MAX_LONG);
	if (vs && len)
	{
		editbox_get_text(pdt->hEdit, string_ensure_buf(vs, len), len);
	}

	widget_close(widget, 1);
}

void SQLViewDlg_OnCancel(res_win_t widget)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	widget_close(widget, 0);
}

void SQLViewDlg_OnExec(res_win_t widget)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	const tchar_t* sz_sql = editbox_get_text_ptr(pdt->hEdit);
	if (is_null(sz_sql))
	{
		return;
	}

	DBCTX* pct = DBOpen(pdt->psp->sz_conn);
	if (!pct)
		return;

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	DBSelect(pct, ptrGrid, sz_sql);
	
	DBClose(pct);

	gridctrl_redraw(pdt->hGrid, 1);
}

int SQLViewDlg_OnCreate(res_win_t widget, void* data)
{
	SQLVIEWDLGDELTA* pdt = (SQLVIEWDLGDELTA*)xmem_alloc(sizeof(SQLVIEWDLGDELTA));

	pdt->psp = (SQLVIEWDLG_PARAM*)data;

	XDL_ASSERT(pdt->psp);

	widgetex_hand_create(widget);

	SETSQLVIEWDLGDELTA(widget, pdt);

	pdt->sp.widget = widget;
	pdt->sp.split = create_split_doc();

	split_item(pdt->sp.split, 1);
	set_split_item_ratio(pdt->sp.split, _T("50%"));

	LINKPTR ilkGrid = get_split_first_child_item(pdt->sp.split);
	LINKPTR ilkEdit = get_split_last_child_item(pdt->sp.split);

	xrect_t xr_reg = { 0 };

	pdt->hGrid = gridctrl_create(NULL, WD_STYLE_CONTROL, &xr_reg, widget);
	XDL_ASSERT(pdt->hGrid);
	widget_set_user_id(pdt->hGrid, IDC_SQLVIEWDLG_GRID);
	widget_set_owner(pdt->hGrid, widget);

	LINKPTR ptrGrid = create_grid_doc();

	gridctrl_attach(pdt->hGrid, ptrGrid);
	widget_show(pdt->hGrid, WD_SHOW_NORMAL);

	set_split_item_delta(ilkGrid, pdt->hGrid);

	pdt->hEdit = editbox_create(widget, WD_STYLE_CONTROL, &xr_reg);
	XDL_ASSERT(pdt->hEdit);
	widget_set_user_id(pdt->hEdit, IDC_SQLVIEWDLG_EDIT);
	widget_set_owner(pdt->hEdit, widget);
	widget_show(pdt->hEdit, WD_SHOW_NORMAL);

	xface_t xa;
	widgetex_get_xface(pdt->hEdit, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_LINEBREAK);
	widgetex_set_xface(pdt->hEdit, &xa);

	set_split_item_delta(ilkEdit, pdt->hEdit);

	xrect_t xr;
	widget_get_client_rect(widget, &xr);

	xsize_t xs;
	xs.fx = SQLVIEWDLG_BAR_SPLIT;
	xs.fy = 0;
	widgetex_size_to_pt(widget, &xs);
	int nSplit = xs.cx;

	xs.fx = SQLVIEWDLG_BTN_WIDTH;
	xs.fy = SQLVIEWDLG_BAR_HEIGHT;
	widgetex_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - nSplit;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xr.h - xs.cy + nSplit;
	xr_push.h = xs.cy - 2 * nSplit;

	pdt->hPushCancel = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_user_id(pdt->hPushCancel, IDC_SQLVIEWDLG_CANCEL);
	widget_set_owner(pdt->hPushCancel, widget);
	pushbox_set_text(pdt->hPushCancel, _T("取消"), -1);
	widget_show(pdt->hPushCancel, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + nSplit);

	pdt->hPushOK = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_owner(pdt->hPushOK, widget);
	widget_set_user_id(pdt->hPushOK, IDC_SQLVIEWDLG_OK);
	pushbox_set_text(pdt->hPushOK, _T("确定"), -1);
	widget_show(pdt->hPushOK, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + nSplit);

	pdt->hPushExec = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_owner(pdt->hPushExec, widget);
	widget_set_user_id(pdt->hPushExec, IDC_SQLVIEWDLG_EXEC);
	pushbox_set_text(pdt->hPushExec, _T("执行"), -1);
	widget_show(pdt->hPushExec, WD_SHOW_NORMAL);

	return 0;
}

void SQLViewDlg_OnDestroy(res_win_t widget)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	if (widget_is_valid(pdt->hGrid))
	{
		LINKPTR ptrGrid = gridctrl_detach(pdt->hGrid);
		if (ptrGrid)
			destroy_grid_doc(ptrGrid);

		widget_destroy(pdt->hGrid);
	}

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

	if (widget_is_valid(pdt->hPushExec))
	{
		widget_destroy(pdt->hPushExec);
	}

	destroy_split_doc(pdt->sp.split);

	xmem_free(pdt);

	widgetex_hand_destroy(widget);
}

void SQLViewDlg_OnMouseMove(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	if (hand_splitor_mouse_move(&pdt->sp, dw, pxp))
		return;
}

void SQLViewDlg_OnLButtonDown(res_win_t widget, const xpoint_t* pxp)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	if (hand_splitor_lbutton_down(&pdt->sp, pxp))
		return;
}

void SQLViewDlg_OnLButtonUp(res_win_t widget, const xpoint_t* pxp)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	if (hand_splitor_lbutton_up(&pdt->sp, pxp))
		return;
}

void SQLViewDlg_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	xrect_t xr;
	
	widget_get_client_rect(widget, &xr);

	xsize_t xs;
	xs.fx = 0;
	xs.fy = SQLVIEWDLG_BAR_HEIGHT;
	widgetex_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.cy;

	hand_splitor_size(&pdt->sp, &xr_reg);

	xs.fx = SQLVIEWDLG_BAR_SPLIT;
	xs.fy = 0;
	widgetex_size_to_pt(widget, &xs);
	int nSplit = xs.cx;

	xs.fx = SQLVIEWDLG_BTN_WIDTH;
	xs.fy = SQLVIEWDLG_BAR_HEIGHT;
	widgetex_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - nSplit;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xr.h - xs.cy + nSplit;
	xr_push.h = xs.cy - 2 * nSplit;

	widget_move(pdt->hPushCancel, RECTPOINT(&xr_push));
	widget_update(pdt->hPushCancel);

	xr_push.x -= (xs.cx + nSplit);

	widget_move(pdt->hPushOK, RECTPOINT(&xr_push));
	widget_update(pdt->hPushOK);

	xr_push.x -= (xs.cx + nSplit);

	widget_move(pdt->hPushExec, RECTPOINT(&xr_push));
	widget_update(pdt->hPushExec);

	widget_redraw(widget, NULL, 0);
}

void SQLViewDlg_OnPaint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	canvas_t canv;
	res_ctx_t rdc;
	xbrush_t xb = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr, xr_bar;
	xsize_t xs;

	widgetex_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	hand_splitor_paint(&pdt->sp, rdc);

	xs.fx = SQLVIEWDLG_BAR_SPLIT;
	xs.fy = SQLVIEWDLG_BAR_HEIGHT;

	widgetex_size_to_pt(widget, &xs);

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

void SQLViewDlg_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);
	
	XDL_ASSERT(pdt != NULL);

	if (phdr->id == IDC_SQLVIEWDLG_GRID)
	{
		NOTICE_GRID* pnl = (NOTICE_GRID*)phdr;
		
	}
}

void SQLViewDlg_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	SQLVIEWDLGDELTA* pdt = GETSQLVIEWDLGDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	switch (cid)
	{
	case IDC_SQLVIEWDLG_OK:
		SQLViewDlg_OnOK(widget);
		break;
	case IDC_SQLVIEWDLG_CANCEL:
		SQLViewDlg_OnCancel(widget);
		break;
	case IDC_SQLVIEWDLG_EXEC:
		SQLViewDlg_OnExec(widget);
		break;
	}
}

/*************************************************************************/
res_win_t SQLViewDlg_Create(const TCHAR* title, SQLVIEWDLG_PARAM* ppd)
{
	res_win_t widget;

	if_event_t ev = { 0 };

	ev.param = (void*)ppd;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(SQLViewDlg_OnCreate)
	EVENT_ON_DESTROY(SQLViewDlg_OnDestroy)

	EVENT_ON_SIZE(SQLViewDlg_OnSize)
	EVENT_ON_PAINT(SQLViewDlg_OnPaint)
	EVENT_ON_MOUSE_MOVE(SQLViewDlg_OnMouseMove)
	EVENT_ON_LBUTTON_DOWN(SQLViewDlg_OnLButtonDown)
	EVENT_ON_LBUTTON_UP(SQLViewDlg_OnLButtonUp)

	EVENT_ON_NOTICE(SQLViewDlg_OnNotice)
	EVENT_ON_MENU_COMMAND(SQLViewDlg_OnMenuCommand)

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

	widgetex_set_color_mode(widget, &clr);

	widget_center_window(widget, g_hMain);
	widget_update(widget);
	widget_show(widget, WD_SHOW_NORMAL);

	return widget;
}