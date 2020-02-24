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
#include "NewFileDialog.h"

#define IDC_NEWFILEDLG_LIST		2001
#define IDC_NEWFILEDLG_OK		2002
#define IDC_NEWFILEDLG_CANCEL	2003

#define NEWFILEDLG_BAR_HEIGHT	(float)10 //TM
#define NEWFILEDLG_BAR_SPLIT	(float)2 //TM
#define NEWFILEDLG_BTN_WIDTH	(float)12 //TM

typedef struct tagNewFileDlgDelta{
	res_win_t hList;
	res_win_t hPushOK;
	res_win_t hPushCancel;

	NEWFILEDLG_PARAM* pSel;
}NewFileDlgDelta;


#define GETNEWFILEDLGDELTA(ph) 		(NewFileDlgDelta*)widget_get_user_delta(ph)
#define SETNEWFILEDLGDELTA(ph,ptd)	widget_set_user_delta(ph,(void*)ptd)

void NewFileDlg_OnOK(res_win_t widget)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	int ret = 0;

	LINKPTR ptrList = listctrl_fetch(pdt->hList);
	LINKPTR llk = listctrl_get_focus_item(pdt->hList);
	if (llk)
	{
		ret = xstol(get_list_item_id_ptr(llk));
		xscpy(pdt->pSel->szPath, get_list_item_title_ptr(llk));
	}

	widget_close(widget, ret);
}

void NewFileDlg_OnCancel(res_win_t widget)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	widget_close(widget, 0);
}

/*********************************************************************************************/
int NewFileDlg_OnCreate(res_win_t widget, void* data)
{
	NewFileDlgDelta* pdt;

	pdt = (NewFileDlgDelta*)xmem_alloc(sizeof(NewFileDlgDelta));
	pdt->pSel = (NEWFILEDLG_PARAM*)data;

	XDC_ASSERT(pdt->pSel);

	SETNEWFILEDLGDELTA(widget, (void*)pdt);

	res_dc_t rdc = widget_client_dc(widget);
	xhand_t canv = create_display_canvas(rdc);
	widget_attach_canvas(widget, canv);
	widget_release_dc(widget, rdc);

	xrect_t xr;
	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = NEWFILEDLG_BAR_SPLIT;
	xs.fy = NEWFILEDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.cy;

	pdt->hList = listctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_LOCKED, &xr_reg, widget);
	XDC_ASSERT(pdt->hList);
	widget_set_user_id(pdt->hList, IDC_NEWFILEDLG_LIST);
	widget_set_owner(pdt->hList, widget);

	LINKPTR ptrList = create_list_doc();
	set_list_images(ptrList, g_ptrImages);

	tchar_t szID[NUM_LEN + 1];

	LINKPTR llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWFORM);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("空白表单"));
	set_list_item_image(llk, BMP_MEMO);

	llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWGRID);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("空白网格"));
	set_list_item_image(llk, BMP_GRID);

	llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWRICH);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("空白富文本"));
	set_list_item_image(llk, BMP_RICH);

	llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWGRAPH);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("空白图形"));
	set_list_item_image(llk, BMP_GRAPH);

	llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWSCHEMA);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("空白模式"));
	set_list_item_image(llk, BMP_EDIT);

	llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWTOPOG);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("空白地形"));
	set_list_item_image(llk, BMP_GROUP);

	llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWIMAGES);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("图像列表"));
	set_list_item_image(llk, BMP_IMAGE);

	llk = insert_list_item(ptrList, LINK_LAST);
	xsprintf(szID, _T("%d"), NEWFILEDLG_RET_NEWDIALOG);
	set_list_item_id(llk, szID);
	set_list_item_title(llk, _T("对话框"));
	set_list_item_image(llk, BMP_DIALOG);

	listctrl_attach(pdt->hList, ptrList);
	widget_show(pdt->hList, WD_SHOW_NORMAL);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	xs.fx = NEWFILEDLG_BTN_WIDTH;
	xs.fy = NEWFILEDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - xs.cy;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xs.cy;
	xr_push.h = xr.h - 2 * xs.cy;
	pdt->hPushCancel = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_user_id(pdt->hPushCancel, IDC_NEWFILEDLG_CANCEL);
	widget_set_owner(pdt->hPushCancel, widget);
	pushbox_set_text(pdt->hPushCancel, _T("取消"), -1);
	widget_show(pdt->hPushCancel, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);
	pdt->hPushOK = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_push);
	widget_set_owner(pdt->hPushOK, widget);
	widget_set_user_id(pdt->hPushOK, IDC_NEWFILEDLG_OK);
	pushbox_set_text(pdt->hPushOK, _T("确定"), -1);
	widget_show(pdt->hPushOK, WD_SHOW_NORMAL);

	return 0;
}

void NewFileDlg_OnDestroy(res_win_t widget)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (widget_is_valid(pdt->hList))
	{
		LINKPTR ptrList = listctrl_detach(pdt->hList);
		if (ptrList)
			destroy_list_doc(ptrList);

		widget_destroy(pdt->hList);
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

void NewFileDlg_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = NEWFILEDLG_BAR_SPLIT;
	xs.fy = NEWFILEDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y;
	xr_reg.h = xr.h - xs.cy;

	widget_move(pdt->hList, RECTPOINT(&xr_reg));
	widget_size(pdt->hList, RECTSIZE(&xr_reg));
	widget_update_client(pdt->hList);

	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;

	xs.fx = NEWFILEDLG_BTN_WIDTH;
	xs.fy = NEWFILEDLG_BAR_SPLIT;
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

void NewFileDlg_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (phdr->id == IDC_NEWFILEDLG_LIST)
	{
		NOTICE_LIST* pnl = (NOTICE_LIST*)phdr;
	}
}

static int __stdcall  _NewFileDlg_SetChildColor(res_win_t widget, long_ptr pv)
{
	widget_send_command(widget, COMMAND_COLOR, IDC_PARENT, (void*)pv);

	return 1;
}

void NewFileDlg_OnCommandColor(res_win_t widget, void* data)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);
	
	widget_set_color_mode(widget, (clr_mod_t*)data);

	widget_enum_child(widget, _NewFileDlg_SetChildColor, (long_ptr)data);
}

void NewFileDlg_OnCommandSwitch(res_win_t widget, bool_t bkey)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	widget_send_command(pdt->hList, COMMAND_SWITCH, IDC_PARENT, (void*)bkey);
}

void NewFileDlg_OnMenuCommand(res_win_t widget, int code, int cid, void* data)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	switch (cid)
	{
	case IDC_NEWFILEDLG_OK:
		NewFileDlg_OnOK(widget);
		break;
	case IDC_NEWFILEDLG_CANCEL:
		NewFileDlg_OnCancel(widget);
		break;
	}
}

void NewFileDlg_OnPaint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	NewFileDlgDelta* pdt = GETNEWFILEDLGDELTA(widget);

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

	xs.fx = NEWFILEDLG_BAR_SPLIT;
	xs.fy = NEWFILEDLG_BAR_HEIGHT;

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

/************************************************************************************************************/
res_win_t NewFileDlg_Create(const TCHAR* title, NEWFILEDLG_PARAM* ppd)
{
	res_win_t widget;

	if_event_t ev = { 0 };

	ev.data = (void*)ppd;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(NewFileDlg_OnCreate)
	EVENT_ON_DESTROY(NewFileDlg_OnDestroy)

	EVENT_ON_SIZE(NewFileDlg_OnSize)
	EVENT_ON_PAINT(NewFileDlg_OnPaint)

	EVENT_ON_NOTICE(NewFileDlg_OnNotice)
	EVENT_ON_COMMAND_COLOR(NewFileDlg_OnCommandColor)
	EVENT_ON_COMMAND_SWITCH(NewFileDlg_OnCommandSwitch)
	EVENT_ON_MENU_COMMAND(NewFileDlg_OnMenuCommand)

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