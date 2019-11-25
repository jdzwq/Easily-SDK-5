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
#include "TopogPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_TOPOGPANEL_TOPOG		201
#define IDC_TOPOGPANEL_PROPER		202
#define IDC_TOPOGPANEL_TITLE		203
#define IDC_TOPOGPANEL_MENU			204

#define TOPOGPANEL_GROUPITEM_WIDTH		(float)8
#define TOPOGPANEL_GROUPITEM_HEIGHT		(float)7
#define TOPOGPANEL_TITLEITEM_WIDTH		(float)15
#define TOPOGPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagTopogPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hTopog;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}TopogPanelDelta;

#define GETTOPOGPANELDELTA(ph) 		(TopogPanelDelta*)widget_get_user_delta(ph)
#define SETTOPOGPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define TOPOGPANEL_ACCEL_COUNT	5
accel_t	TOPOGPANEL_ACCEL[TOPOGPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	TopogPanel_Switch(res_win_t widget);

bool_t	TopogPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	TopogPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void TopogPanel_Switch(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	if (!topogctrl_get_dirty(pdt->hTopog))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		topogctrl_set_dirty(pdt->hTopog, 0);
		break;
	}
}

void TopogPanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	topogctrl_set_dirty(pdt->hTopog, bDirty);
}

bool_t TopogPanel_GetDirty(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	return topogctrl_get_dirty(pdt->hTopog);
}

/***************************************************************************************************************/
void TopogPanel_OnSelectAll(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);
	XDL_ASSERT(ptrTopog);

	LINKPTR ilk = get_topog_next_spot(ptrTopog, LINK_FIRST);
	while (ilk)
	{
		set_topog_spot_selected(ilk, 1);

		ilk = get_topog_next_spot(ptrTopog, ilk);
	}

	topogctrl_redraw(pdt->hTopog);
}

void TopogPanel_OnInsert(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	topogctrl_set_dirty(pdt->hTopog, 1);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);
	LINKPTR ptrPos = topogctrl_get_focus_spot(pdt->hTopog);

	if (!ptrPos)
		ptrPos = LINK_LAST;

	LINKPTR ilk = insert_topog_spot(ptrTopog, ptrPos);

	int count = get_topog_spot_count(ptrTopog);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("spot%d"), count);

	set_topog_spot_name(ilk, token);

	topogctrl_redraw(pdt->hTopog);
}

void TopogPanel_OnDelete(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);

	XDL_ASSERT(ptrTopog);

	topogctrl_set_dirty(pdt->hTopog, 1);

	LINKPTR ptrSpot = topogctrl_get_focus_spot(pdt->hTopog);

	topogctrl_set_focus_spot(pdt->hTopog, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk, ilk = get_topog_next_spot(ptrTopog, LINK_FIRST);
	while (ilk)
	{
		nlk = get_topog_next_spot(ptrTopog, ilk);
		if (get_topog_spot_selected(ilk))
		{
			delete_topog_spot(ilk);
			ilk = nlk;

			bRedraw = 1;
			continue;
		}

		ilk = nlk;
	}

	if (bRedraw)
		topogctrl_redraw(pdt->hTopog);
}

void TopogPanel_OnCopy(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	widget_copy(pdt->hTopog);
}

void TopogPanel_OnCut(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	widget_cut(pdt->hTopog);
}

void TopogPanel_OnPaste(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	widget_paste(pdt->hTopog);
}

void TopogPanel_OnRedo(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

}

void TopogPanel_OnUndo(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	widget_undo(pdt->hTopog);
}

void TopogPanel_OnAttach(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szExt[10] = { 0 };

	const tchar_t* szFilter = _T("JPG File(*.jpg)\0*.jpg\0PNG File(*.png)\0*.png\0Bitmap File(*.bmp)\0*.bmp\0");
	xscpy(szExt, _T("jpg"));

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, szFilter, szExt, 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);

	res_ctx_t rdc = widget_client_ctx(widget);

	res_bmp_t bmp = load_bitmap_from_file(rdc, szPath);

	widget_release_ctx(widget, rdc);

	topogctrl_set_bitmap(pdt->hTopog, bmp);

	destroy_bitmap(bmp);
}

void TopogPanel_OnDetach(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	topogctrl_set_bitmap(pdt->hTopog, NULL);
}


void TopogPanel_OnAttributes(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);
	LINKPTR ptrSpot = topogctrl_get_focus_spot(pdt->hTopog);

	int row = -1, col = -1;
	topogctrl_get_focus_dot(pdt->hTopog, &row, &col);

	if (ptrSpot)
		properbag_write_topog_spot_attributes(ptrProper, ptrSpot);
	else
		properbag_write_topog_attributes(ptrProper, ptrTopog, row, col);

	properctrl_redraw(pdt->hProper);
}

void TopogPanel_OnStyleSheet(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);
	LINKPTR ptrSpot = topogctrl_get_focus_spot(pdt->hTopog);

	if (ptrSpot)
		properbag_parse_stylesheet(ptrProper, get_topog_spot_style_ptr(ptrSpot));
	else
		properbag_parse_stylesheet(ptrProper, get_topog_style_ptr(ptrTopog));

	properctrl_redraw(pdt->hProper);
}

void TopogPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void TopogPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	long n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void TopogPanel_Topog_OnRBClick(res_win_t widget, NOTICE_TOPOG* pnf)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr = { 0 };
	xpoint_t pt = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_TOPOGPANEL_MENU);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);
	widget_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();
	LINKPTR mlk;

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_ATTACH);
	set_menu_item_title(mlk, _T("附加图像"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_DETACH);
	set_menu_item_title(mlk, _T("撤离图像"));

	if (pnf->spot)
	{
		mlk = insert_menu_item(ptrMenu, LINK_LAST);
		set_menu_item_iid(mlk, IDA_EDIT_COPY);
		set_menu_item_title(mlk, _T("拷贝地标"));

		mlk = insert_menu_item(ptrMenu, LINK_LAST);
		set_menu_item_iid(mlk, IDA_EDIT_CUT);
		set_menu_item_title(mlk, _T("剪切地标"));

		mlk = insert_menu_item(ptrMenu, LINK_LAST);
		set_menu_item_iid(mlk, IDA_EDIT_DELETE);
		set_menu_item_title(mlk, _T("删除地标"));
	}
	else
	{
		mlk = insert_menu_item(ptrMenu, LINK_LAST);
		set_menu_item_iid(mlk, IDA_EDIT_PASTE);
		set_menu_item_title(mlk, _T("粘贴地标"));
	}

	menubox_set_data(hMenu, ptrMenu);

	pt.x = ppt->x;
	pt.y = ppt->y;
	widget_client_to_screen(pdt->hTopog, &pt);

	menubox_layout(hMenu, &pt, WD_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void TopogPanel_Topog_OnLBClick(res_win_t widget, NOTICE_TOPOG* pnf)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void TopogPanel_Topog_OnSpotDrop(res_win_t widget, NOTICE_TOPOG* pnf)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void TopogPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);
	LINKPTR ptrSpot = topogctrl_get_focus_spot(pdt->hTopog);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrSpot)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_topog_spot_attributes(pnp->proper, ptrSpot);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_topog_spot_style(ptrSpot, sz_style);
		}
		topogctrl_redraw(pdt->hTopog);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_topog_attributes(pnp->proper, ptrTopog);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_topog_style(ptrTopog, sz_style);
		}
		topogctrl_redraw(pdt->hTopog);
	}
}

bool_t TopogPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrMeta = create_meta_doc();
	if (!load_dom_doc_from_file(ptrMeta, NULL, szFile))
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	if (compare_text(get_meta_doc_name_ptr(ptrMeta), -1, DOC_TOPOG, -1, 1) != 0)
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("非表单文档！"));

		return 0;
	}

	get_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, RES_LEN);
	get_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, RES_LEN);

	LINKPTR newTopog = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	LINKPTR orgTopog = topogctrl_detach(pdt->hTopog);
	destroy_topog_doc(orgTopog);

	set_topog_design(newTopog, 1);
	topogctrl_attach(pdt->hTopog, newTopog);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t TopogPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrTopog);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrTopog = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		MessageBox(widget, _T("保存图形文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void TopogPanel_OnSave(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("Sheet file(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	if (TopogPanel_SaveFile(widget, szFile))
	{
		topogctrl_set_dirty(pdt->hTopog, 0);
	}
}

void TopogPanel_OnSaveAs(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szPath[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Sheet file(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrTopog);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrTopog = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void TopogPanel_OnPrint(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);
}

void TopogPanel_OnPreview(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptrTopog = topogctrl_fetch(pdt->hTopog);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_topog(svg, ptrTopog);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	xsprintf(token, _T("%s"), _T("地形图"));
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WD_SHOW_FULLSCREEN);
}

/***********************************************************************************************/

int TopogPanel_OnCreate(res_win_t widget, void* data)
{
	widget_hand_create(widget);

	TopogPanelDelta* pdt = (TopogPanelDelta*)xmem_alloc(sizeof(TopogPanelDelta));

	SETTOPOGPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(TOPOGPANEL_ACCEL, TOPOGPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	xrect_t xr;

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkTopog = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -TOPOGPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hTopog = topogctrl_create(_T("TopogPanel"), WD_STYLE_CONTROL, &xr, widget);

	widget_set_user_id(pdt->hTopog, IDC_TOPOGPANEL_TOPOG);
	widget_set_owner(pdt->hTopog, widget);

	set_split_item_delta(ilkTopog, pdt->hTopog);
	widget_show(pdt->hTopog, WD_SHOW_NORMAL);

	LINKPTR ptrTopog = create_topog_doc();
	set_topog_design(ptrTopog, 1);
	topogctrl_attach(pdt->hTopog, ptrTopog);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("TopogProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_TOPOGPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("TopogTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_TOPOGPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WD_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, TOPOGPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, ICON_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("风格"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, TOPOGPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, ICON_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);
	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!TopogPanel_OpenFile(widget, szParam))
			return -1;

		topogctrl_redraw(pdt->hTopog);
	}

	return 0;
}

void TopogPanel_OnDestroy(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hTopog))
	{
		LINKPTR ptrTopog = topogctrl_detach(pdt->hTopog);
		if (ptrTopog)
			destroy_topog_doc(ptrTopog);

		widget_destroy(pdt->hTopog);
	}

	if (widget_is_valid(pdt->hProper))
	{
		LINKPTR ptrProper = properctrl_detach(pdt->hProper);
		if (ptrProper)
			destroy_proper_doc(ptrProper);

		widget_destroy(pdt->hProper);
	}

	if (widget_is_valid(pdt->hTitle))
	{
		LINKPTR ptrTitle = titlectrl_detach(pdt->hTitle);
		if (ptrTitle)
			destroy_title_doc(ptrTitle);

		widget_destroy(pdt->hTitle);
	}

	xmem_free(pdt);

	widget_hand_destroy(widget);
}

int TopogPanel_OnClose(res_win_t widget)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	TopogPanel_Switch(widget);

	return (TopogPanel_GetDirty(widget)) ? 1 : 0;
}

void TopogPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hTopog))
	{
		widget_set_focus(pdt->hTopog);
	}
}

void TopogPanel_OnShow(res_win_t widget, bool_t bShow)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, TOPOGPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, TOPOGPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_SELECTALL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("全选"));
		set_tool_item_icon(ilk, ICON_SELECTALL);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_DELETE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除"));
		set_tool_item_icon(ilk, ICON_DELETE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_COPY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("拷贝"));
		set_tool_item_icon(ilk, ICON_COPY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_CUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("剪切"));
		set_tool_item_icon(ilk, ICON_CUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_PASTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("粘贴"));
		set_tool_item_icon(ilk, ICON_PASTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_UNDO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("撤销"));
		set_tool_item_icon(ilk, ICON_UNDO);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_INSERT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增"));
		set_tool_item_icon(ilk, ICON_PLUS);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void TopogPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	LINKPTR ptr_topog = topogctrl_fetch(pdt->hTopog);
	LINKPTR ilk = get_topog_next_spot(ptr_topog, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(pfd->sub_str, -1, get_topog_spot_name_ptr(ilk), -1, 1) == 0)
		{
			topogctrl_set_focus_spot(pdt->hTopog, ilk);
			break;
		}
		ilk = get_topog_next_spot(ptr_topog, ilk);
	}
}

void TopogPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_TOPOG);
		pqo->ptrDoc = topogctrl_fetch(pdt->hTopog);
	}
	else if (code == COMMAND_RENAME)
	{
		tchar_t szPath[PATH_LEN], szExt[INT_LEN];
		const tchar_t* nname = (const tchar_t*)data;

		if (!is_null(pdt->szFile) && !is_null(nname))
		{
			split_path(pdt->szFile, szPath, NULL, szExt);
			xsprintf(pdt->szFile, _T("%s\\%s.%s"), szPath, nname, szExt);
		}
	}
	else if (code == COMMAND_REMOVE)
	{
		topogctrl_set_dirty(pdt->hTopog, 0);
	}
}

void TopogPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		TopogPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		TopogPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		TopogPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		TopogPanel_OnPreview(widget);
		break;

	case IDA_EDIT_SELECTALL:
		TopogPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		TopogPanel_OnDelete(widget);
		break;
	case IDA_EDIT_INSERT:
		TopogPanel_OnInsert(widget);
		break;
	case IDA_EDIT_COPY:
		TopogPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		TopogPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		TopogPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		TopogPanel_OnUndo(widget);
		break;
	case IDA_EDIT_ATTACH:
		TopogPanel_OnAttach(widget);
		break;
	case IDA_EDIT_DETACH:
		TopogPanel_OnDetach(widget);
		break;

	case IDA_ATTRIBUTES:
		TopogPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		TopogPanel_OnStyleSheet(widget);
		break;

	case IDC_TOPOGPANEL_MENU:
		widget_destroy((res_win_t)data);
		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;
	}
}

void TopogPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	TopogPanelDelta* pdt = GETTOPOGPANELDELTA(widget);

	if (phdr->id == IDC_TOPOGPANEL_TOPOG)
	{
		NOTICE_TOPOG* pnf = (NOTICE_TOPOG*)phdr;
		switch (pnf->code)
		{
		case NC_TOPOGCALCED:
			break;
		case NC_TOPOGSPOTCALCED:
			break;
		case NC_TOPOGLBCLK:
			TopogPanel_Topog_OnLBClick(widget, pnf);
			break;
		case NC_TOPOGRBCLK:
			TopogPanel_Topog_OnRBClick(widget, pnf);
			break;
		case NC_TOPOGSPOTDROP:
			TopogPanel_Topog_OnSpotDrop(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_TOPOGPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			topogctrl_set_dirty(pdt->hTopog, 1);
			break;
		case NC_ENTITYUPDATE:
			TopogPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_TOPOGPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			TopogPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			TopogPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/*********************************************************************************************************/
res_win_t TopogPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(TopogPanel_OnCreate)
	EVENT_ON_DESTROY(TopogPanel_OnDestroy)
	EVENT_ON_CLOSE(TopogPanel_OnClose)
	EVENT_ON_SHOW(TopogPanel_OnShow)

	EVENT_ON_NOTICE(TopogPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(TopogPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(TopogPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(TopogPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}