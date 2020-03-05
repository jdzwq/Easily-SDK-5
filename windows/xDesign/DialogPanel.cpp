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
#include "DialogPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_DIALOGPANEL_DIALOG			201
#define IDC_DIALOGPANEL_PROPER			202
#define IDC_DIALOGPANEL_TITLE			203
#define IDC_DIALOGPANEL_MENU			204
#define IDC_DIALOGPANEL_FONTNAME		210
#define IDC_DIALOGPANEL_FONTSIZE		211
#define IDC_DIALOGPANEL_FONTCOLOR		212
#define IDC_DIALOGPANEL_PAINTCOLOR		213
#define IDC_DIALOGPANEL_DRAWCOLOR		214

#define DIALOGPANEL_GROUPITEM_WIDTH		(float)8
#define DIALOGPANEL_GROUPITEM_HEIGHT	(float)7
#define DIALOGPANEL_TITLEITEM_WIDTH		(float)15
#define DIALOGPANEL_TITLEITEM_HEIGHT	(float)10

typedef struct tagDialogPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hDialog;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}DialogPanelDelta;

#define GETDIALOGPANELDELTA(ph) 		(DialogPanelDelta*)widget_get_user_delta(ph)
#define SETDIALOGPANELDELTA(ph,ptd)		widget_set_user_delta(ph,(var_long)ptd)

#define DIALOGPANEL_ACCEL_COUNT	5
accel_t	DIALOGPANEL_ACCEL[DIALOGPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY,			VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

/***************************************************************************************/
void DialogPanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	dialogctrl_set_dirty(pdt->hDialog, bDirty);
}

bool_t DialogPanel_GetDirty(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	return dialogctrl_get_dirty(pdt->hDialog);
}

void DialogPanel_Switch(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	if (!DialogPanel_GetDirty(widget))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));
	
	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		DialogPanel_SetDirty(widget, 0);
		break;
	}
}

LINKPTR DialogPanel_GetDocument(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	return dialogctrl_fetch(pdt->hDialog);
}

bool_t DialogPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR newDialog = create_dialog_from_meta_file(NULL, szFile);
	if (!newDialog)
	{
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	LINKPTR orgDialog = dialogctrl_detach(pdt->hDialog);
	destroy_dialog_doc(orgDialog);

	dialogctrl_attach(pdt->hDialog, newDialog);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t DialogPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrDialog);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrDialog = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		ShowMsg(MSGICO_ERR, _T("保存XML文档错误"));
		return 0;
	}

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void DialogPanel_SelectAttr(res_win_t widget, const tchar_t* attr_name, const tchar_t* attr_val)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	tchar_t style[CSS_LEN];

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (flk == ptrItem || get_dialog_item_selected(flk))
		{
			write_style_attr(get_dialog_item_style_ptr(flk), -1, attr_name, -1, attr_val, -1, style, CSS_LEN);
			set_dialog_item_style(flk, style);
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	dialogctrl_redraw(pdt->hDialog);
}


/*********************************************************************************************************/
void DialogPanel_OnSave(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("dialog meta file(*.dialog)\0*.dialog\0"), _T("dialog"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	if (DialogPanel_SaveFile(widget, szFile))
	{
		DialogPanel_SetDirty(widget, 0);
	}
}

void DialogPanel_OnSaveAs(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Dialog file(*.dialog)\0*.dialog\0"), _T("dialog"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrDialog);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrDialog = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void DialogPanel_OnPrint(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	//print_form(NULL, ptrDialog);
}

void DialogPanel_OnPreview(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_dialog(svg, ptrDialog);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);
	set_arch_item_title(ilk, get_dialog_title_ptr(ptrDialog));

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WS_SHOW_FULLSCREEN);
}

static void demoDlg_OnSubbing(res_win_t widget, uid_t sid, var_long delta)
{
	widget_hand_create(widget);
}

static void demoDlg_OnUnSubbing(res_win_t widget, uid_t sid, var_long delta)
{
	widget_del_subproc(widget, sid);

	widget_hand_destroy(widget);
}

void DialogPanel_OnExec(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	if_subproc_t sb = { 0 };

	SUBPROC_BEGIN_DISPATH(&sb)
		SUBPROC_ON_SUBBING(demoDlg_OnSubbing)
		SUBPROC_ON_UNSUBBING(demoDlg_OnUnSubbing)
		SUBPROC_ON_DIALOG_IMPLEMENT
	SUBPROC_END_DISPATH

	LINKPTR ptr_dlg = dialogctrl_fetch(pdt->hDialog);;

	res_win_t dlg = create_dialog(ptr_dlg, widget);

	widget_set_subproc(dlg, IDS_DIALOG, &sb);

	clr_mod_t clr = { 0 };

	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);
	parse_xcolor(&clr.clr_ico, g_face[g_indFace].ico);

	widget_set_color_mode(dlg, &clr);

	widget_show(dlg, WS_SHOW_NORMAL);

	widget_do_modal(dlg);
}

void DialogPanel_OnSelectAll(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	XDL_ASSERT(ptrDialog);

	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		set_dialog_item_selected(flk, 1);
	
		flk = get_dialog_next_item(ptrDialog, flk);
	}

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnDelete(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	XDL_ASSERT(ptrDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	dialogctrl_set_focus_item(pdt->hDialog, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		nlk = get_dialog_next_item(ptrDialog, flk);
		if (get_dialog_item_selected(flk) || ptrItem == flk)
		{
			delete_dialog_item(flk);
			flk = nlk;

			bRedraw = 1;
			continue;
		}

		flk = nlk;
	}

	if (bRedraw)
		dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnCopy(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	widget_post_command(pdt->hDialog, COMMAND_COPY, IDC_EDITMENU, 0);
}

void DialogPanel_OnCut(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	widget_post_command(pdt->hDialog, COMMAND_CUT, IDC_EDITMENU, 0);
}

void DialogPanel_OnPaste(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	widget_post_command(pdt->hDialog, COMMAND_PASTE, IDC_EDITMENU, 0);
}


void DialogPanel_OnRedo(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

}

void DialogPanel_OnUndo(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	widget_post_command(pdt->hDialog, COMMAND_UNDO, IDC_EDITMENU, 0);
}

void DialogPanel_OnCSSProper(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrProper = create_proper_doc();

	LINKPTR ptr = dialogctrl_fetch(pdt->hDialog);
	LINKPTR flk = dialogctrl_get_focus_item(pdt->hDialog);

	if (flk)
		properbag_parse_stylesheet(ptrProper, get_dialog_item_style_ptr(flk));
	else
		properbag_parse_stylesheet(ptrProper, get_dialog_style_ptr(ptr));

	res_win_t hProperDlg = properdlg_create(_T("绘制样式"), ptrProper, g_hMain);

	widget_show(hProperDlg, WS_SHOW_NORMAL);

	int nRet = widget_do_modal(hProperDlg);

	if (nRet)
	{
		tchar_t sz_style[CSS_LEN] = { 0 };
		properbag_format_stylesheet(ptrProper, sz_style, CSS_LEN);

		if (flk)
		{
			set_dialog_item_style(flk, sz_style);
			dialogctrl_redraw_item(pdt->hDialog, flk);
		}
		else
		{
			set_dialog_style(ptr, sz_style);
			dialogctrl_redraw(pdt->hDialog);
		}

		DialogPanel_SetDirty(widget, 1);
	}

	destroy_proper_doc(ptrProper);
}

void DialogPanel_OnFontName(res_win_t widget, void* pv)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	fontname_menu(widget, IDC_DIALOGPANEL_FONTNAME, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DialogPanel_OnFontSize(res_win_t widget, void* pv)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	fontsize_menu(widget, IDC_DIALOGPANEL_FONTSIZE, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DialogPanel_OnTextColor(res_win_t widget, void* pv)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	color_menu(widget, IDC_DIALOGPANEL_FONTCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DialogPanel_OnPaintColor(res_win_t widget, void* pv)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	color_menu(widget, IDC_DIALOGPANEL_PAINTCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DialogPanel_OnDrawColor(res_win_t widget, void* pv)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	color_menu(widget, IDC_DIALOGPANEL_DRAWCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DialogPanel_OnTextNear(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_NEAR);
}

void DialogPanel_OnTextBold(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SelectAttr(widget, GDI_ATTR_FONT_WEIGHT, GDI_ATTR_FONT_WEIGHT_BOLD);
}

void DialogPanel_OnTextCenter(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_CENTER);
}

void DialogPanel_OnTextFar(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	DialogPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_FAR);
}

void DialogPanel_OnAlignNear(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (!ptrItem)
		return;

	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_dialog_item_selected(flk))
		{
			set_dialog_item_x(flk, get_dialog_item_x(ptrItem));
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnAlignCenter(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (!ptrItem)
		return;

	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_dialog_item_selected(flk))
		{
			set_dialog_item_x(flk, get_dialog_item_x(ptrItem) + get_dialog_item_width(ptrItem) / 2 - get_dialog_item_width(flk) / 2);
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnAlignFar(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (!ptrItem)
		return;

	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_dialog_item_selected(flk))
		{
			set_dialog_item_x(flk, get_dialog_item_x(ptrItem) + get_dialog_item_width(ptrItem) - get_dialog_item_width(flk));
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnSizeHeight(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (!ptrItem)
		return;

	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_dialog_item_selected(flk))
		{
			set_dialog_item_height(flk, get_dialog_item_height(ptrItem));
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnSizeWidth(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (!ptrItem)
		return;

	if (!get_dialog_item_selected_count(ptrDialog))
		return;

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_dialog_item_selected(flk))
		{
			set_dialog_item_width(flk, get_dialog_item_width(ptrItem));
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnSizeHorz(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	int count = get_dialog_item_selected_count(ptrDialog);
	if (count <= 1)
		return;

	DialogPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min,f_max,f_total = 0;
	count = 0;
	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (get_dialog_item_selected(flk))
		{
			pxs[count].fact = (int)(get_dialog_item_x(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_dialog_item_width(flk);
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_dialog_item_x((link_t_ptr)pxs[0].data);
	f_max = get_dialog_item_x((link_t_ptr)pxs[count - 1].data) + get_dialog_item_width((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_dialog_item_x((link_t_ptr)pxs[0].data) + get_dialog_item_width((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_dialog_item_x((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_dialog_item_x((link_t_ptr)pxs[i].data) + get_dialog_item_width((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnSizeVert(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	int count = get_dialog_item_selected_count(ptrDialog);
	if (count <= 1)
		return;

	DialogPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min, f_max, f_total = 0;
	count = 0;
	LINKPTR flk = get_dialog_next_item(ptrDialog, LINK_FIRST);
	while (flk)
	{
		if (get_dialog_item_selected(flk))
		{
			pxs[count].fact = (int)(get_dialog_item_y(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_dialog_item_height(flk);
		}

		flk = get_dialog_next_item(ptrDialog, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_dialog_item_y((link_t_ptr)pxs[0].data);
	f_max = get_dialog_item_y((link_t_ptr)pxs[count - 1].data) + get_dialog_item_height((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_dialog_item_y((link_t_ptr)pxs[0].data) + get_dialog_item_height((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_dialog_item_y((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_dialog_item_y((link_t_ptr)pxs[i].data) + get_dialog_item_height((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	dialogctrl_redraw(pdt->hDialog);
}

void DialogPanel_OnSendBack(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (!ptrItem)
		return;

	DialogPanel_SetDirty(widget, 1);

	set_dialog_item_taborder(ptrItem, 0);

	LINKPTR root = get_dom_child_node_root(ptrDialog);

	switch_link_first(root, ptrItem);

	dialogctrl_redraw(pdt->hDialog);
}


void DialogPanel_OnStaticBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_STATICBOX);

	set_dialog_item_x(flk, 1);
	set_dialog_item_y(flk, 1);
	set_dialog_item_width(flk, DEF_TEXT_WIDTH);
	set_dialog_item_height(flk, DEF_TEXT_HEIGHT);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_STATICBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_STATICBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("Static"), -1);

	dialogctrl_redraw_item(pdt->hDialog,flk);
}

void DialogPanel_OnEditBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_EDITBOX);

	set_dialog_item_x(flk, 1);
	set_dialog_item_y(flk, 1);
	set_dialog_item_width(flk, DEF_TEXT_WIDTH);
	set_dialog_item_height(flk, DEF_TEXT_HEIGHT);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_EDITBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_EDITBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("Text"), -1);

	dialogctrl_redraw_item(pdt->hDialog,flk);
}

void DialogPanel_OnListBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_LISTBOX);

	set_dialog_item_x(flk, 1);
	set_dialog_item_y(flk, 1);
	set_dialog_item_width(flk, DEF_TEXT_WIDTH);
	set_dialog_item_height(flk, DEF_TEXT_HEIGHT * 5);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_LISTBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_LISTBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("0~Item0;1~Item1;2~Item2;"), -1);

	dialogctrl_redraw_item(pdt->hDialog,flk);
}

void DialogPanel_OnNaviBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_NAVIBOX);

	set_dialog_item_x(flk, 1);
	set_dialog_item_y(flk, 1);
	set_dialog_item_width(flk, DEF_TOUCH_SPAN * 8);
	set_dialog_item_height(flk, DEF_TOUCH_SPAN);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_NAVIBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_NAVIBOX, count);

	set_dialog_item_name(flk, token);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnSpinBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_SPINBOX);

	set_dialog_item_x(flk, 1);
	set_dialog_item_y(flk, 1);
	set_dialog_item_width(flk, DEF_TOUCH_SPAN * 1.5);
	set_dialog_item_height(flk, DEF_TOUCH_SPAN);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_SPINBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_SPINBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("0"), -1);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnSlideBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_SLIDEBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, DEF_TOUCH_SPAN * 10);
	set_dialog_item_height(flk, DEF_TOUCH_SPAN);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_SLIDEBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_SLIDEBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("50"), -1);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnRadioBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_RADIOBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, DEF_TOUCH_SPAN * 2);
	set_dialog_item_height(flk, DEF_TOUCH_SPAN);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_RADIOBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_RADIOBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("0"), -1);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnCheckBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_CHECKBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, DEF_TOUCH_SPAN);
	set_dialog_item_height(flk, DEF_TOUCH_SPAN);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_CHECKBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_CHECKBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("0"), -1);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnDateBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_DATEBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, DEF_TOUCH_SPAN * 7);
	set_dialog_item_height(flk, DEF_TOUCH_SPAN * 8);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_DATEBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_DATEBOX, count);

	set_dialog_item_name(flk, token);

	dialogctrl_redraw_item(pdt->hDialog,flk);
}

void DialogPanel_OnTimeBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_TIMEBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, DEF_TOUCH_SPAN * 5);
	set_dialog_item_height(flk, DEF_TOUCH_SPAN * 3);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_TIMEBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_TIMEBOX, count);

	set_dialog_item_name(flk, token);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnShapeBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_SHAPEBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, 40);
	set_dialog_item_height(flk, 40);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_SHAPEBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_SHAPEBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, ATTR_SHAPE_RECT, -1);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnPushBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_PUSHBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, DEF_TEXT_WIDTH);
	set_dialog_item_height(flk, DEF_TEXT_HEIGHT);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_PUSHBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_PUSHBOX, count);

	set_dialog_item_name(flk, token);
	set_dialog_item_text(flk, _T("button"), -1);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnUserBox(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);

	DialogPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_dialog_item(ptrDialog, DOC_DIALOG_USERBOX);

	set_dialog_item_x(flk, 10);
	set_dialog_item_y(flk, 10);
	set_dialog_item_width(flk, 50);
	set_dialog_item_height(flk, 50);

	int count = get_dialog_item_count_by_class(ptrDialog, DOC_DIALOG_USERBOX);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIALOG_USERBOX, count);

	set_dialog_item_name(flk, token);

	dialogctrl_redraw_item(pdt->hDialog, flk);
}

void DialogPanel_OnAttributes(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (ptrItem)
		properbag_write_dialog_item_attributes(ptrProper, ptrItem);
	else
		properbag_write_dialog_attributes(ptrProper, ptrDialog);

	properctrl_redraw(pdt->hProper);
}

void DialogPanel_OnStyleSheet(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	if (ptrItem)
		properbag_parse_stylesheet(ptrProper, get_dialog_item_style_ptr(ptrItem));
	else
		properbag_parse_stylesheet(ptrProper, get_dialog_style_ptr(ptrDialog));

	properctrl_redraw(pdt->hProper);
}

void DialogPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ilk = titlectrl_get_focus_item(pdt->hTitle);
	if (!ilk)
		return;

	int n_id = xstol(get_title_item_id_ptr(ilk));

	LINKPTR ptrDialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR ptrItem = dialogctrl_get_focus_item(pdt->hDialog);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrItem)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_dialog_item_attributes(pnp->proper, ptrItem);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_dialog_item_style(ptrItem, sz_style);
		}
		dialogctrl_redraw_item(pdt->hDialog, ptrItem);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_dialog_attributes(pnp->proper, ptrDialog);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_dialog_style(ptrDialog, sz_style);
		}
		dialogctrl_redraw(pdt->hDialog);
	}
}

void DialogPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void DialogPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	int n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void DialogPanel_Dialog_OnRBClick(res_win_t widget, NOTICE_DIALOG* pnf)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	if (!pnf->item)
		return;

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_DIALOGPANEL_MENU);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);

	widget_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();

	LINKPTR mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_COPY);
	set_menu_item_title(mlk, _T("拷贝部件"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_CUT);
	set_menu_item_title(mlk, _T("剪切部件"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_PASTE);
	set_menu_item_title(mlk, _T("粘贴部件"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_DELETE);
	set_menu_item_title(mlk, _T("删除部件"));

	menubox_set_data(hMenu, ptrMenu);

	xpoint_t xp;
	xp.x = ppt->x;
	xp.y = ppt->y;

	widget_client_to_screen(pdt->hDialog, &xp);

	menubox_layout(hMenu, &xp, WS_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void DialogPanel_Dialog_OnLBClick(res_win_t widget, NOTICE_DIALOG* pnf)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void DialogPanel_Dialog_OnItemSize(res_win_t widget, NOTICE_DIALOG* pnf)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void DialogPanel_Dialog_OnItemMove(res_win_t widget, NOTICE_DIALOG* pnf)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}


/***********************************************************************************************************/
int DialogPanel_OnCreate(res_win_t widget, void* data)
{
	DialogPanelDelta* pdt = (DialogPanelDelta*)xmem_alloc(sizeof(DialogPanelDelta));
	xrect_t xr;
	const tchar_t* szParam;

	widget_hand_create(widget);

	SETDIALOGPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(DIALOGPANEL_ACCEL, DIALOGPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	szParam = (tchar_t*)data;

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkDialog = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -DIALOGPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);
	set_split_item_fixed(ilkRight, 1);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hDialog = dialogctrl_create(_T("DialogPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr, widget);
	widget_set_user_id(pdt->hDialog, IDC_DIALOGPANEL_DIALOG);
	widget_set_owner(pdt->hDialog, widget);

	set_split_item_delta(ilkDialog, pdt->hDialog);
	widget_show(pdt->hDialog, WS_SHOW_NORMAL);

	LINKPTR ptrDialog = create_dialog_doc();
	dialogctrl_attach(pdt->hDialog, ptrDialog);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("DialogProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_DIALOGPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("DialogTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_DIALOGPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WS_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, DIALOGPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ICON_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("样式"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, DIALOGPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ICON_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);

	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	if (!is_null(szParam))
	{
		if (!DialogPanel_OpenFile(widget, szParam))
			return -1;

		dialogctrl_redraw(pdt->hDialog);
	}

	return 0;
}

void DialogPanel_OnDestroy(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hDialog))
	{
		LINKPTR ptrDialog = dialogctrl_detach(pdt->hDialog);
		if (ptrDialog)
			destroy_dialog_doc(ptrDialog);

		widget_destroy(pdt->hDialog);
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

void DialogPanel_OnShow(res_win_t widget, bool_t bShow)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, DIALOGPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, DIALOGPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_SELECTALL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("全选"));
		set_tool_item_icon(ilk, GDI_ICON_SELECTALL);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_DELETE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除"));
		set_tool_item_icon(ilk, GDI_ICON_DELETE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_COPY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("拷贝"));
		set_tool_item_icon(ilk, GDI_ICON_COPY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_CUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("剪切"));
		set_tool_item_icon(ilk, GDI_ICON_CUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_PASTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("粘贴"));
		set_tool_item_icon(ilk, GDI_ICON_PASTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_UNDO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("撤销"));
		set_tool_item_icon(ilk, GDI_ICON_UNDO);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_STYLE);
		set_tool_group_title(glk, _T("样式"));
		set_tool_group_item_width(glk, DIALOGPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, DIALOGPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_NAME);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体名称"));
		set_tool_item_icon(ilk, GDI_ICON_FONTNAME);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_SIZE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字号大小"));
		set_tool_item_icon(ilk, GDI_ICON_FONTSIZE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_WEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体加黑"));
		set_tool_item_icon(ilk, GDI_ICON_FONTWEIGHT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体颜色"));
		set_tool_item_icon(ilk, GDI_ICON_FONTCOLOR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PAINT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("背景颜色"));
		set_tool_item_icon(ilk, GDI_ICON_PRINT);

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_DRAW_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("前景"));
		set_tool_item_icon(ilk, GDI_ICON_STYLE);*/

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PROPER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("CSS属性"));
		set_tool_item_icon(ilk, GDI_ICON_PROPER);*/

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ALIGNNEAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ALIGNCENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ALIGNFAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ARRANGELEFT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ARRANGECENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ARRANGERIGHT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_WIDTH);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等宽"));
		set_tool_item_icon(ilk, GDI_ICON_SIZEHORZ);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等高"));
		set_tool_item_icon(ilk, GDI_ICON_SIZEVERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HORZ);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("水平等距"));
		set_tool_item_icon(ilk, GDI_ICON_SPACEHORZ);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_VERT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("垂直等距"));
		set_tool_item_icon(ilk, GDI_ICON_SPACEVERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SENDBACK);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("置于后端"));
		set_tool_item_icon(ilk, GDI_ICON_ORDER);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_ELEMENT);
		set_tool_group_title(glk, _T("部件"));
		set_tool_group_item_width(glk, DIALOGPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, DIALOGPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_STATICBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("静态文本"));
		set_tool_item_icon(ilk, GDI_ICON_STATIC);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_EDITBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("文本框"));
		set_tool_item_icon(ilk, GDI_ICON_SINGLETEXT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_LISTBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("列表框"));
		set_tool_item_icon(ilk, GDI_ICON_LIST);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_NAVIBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("导航框"));
		set_tool_item_icon(ilk, GDI_ICON_NAVI);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_SPINBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("增减框"));
		set_tool_item_icon(ilk, GDI_ICON_SPIN);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_SLIDEBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("滑动框"));
		set_tool_item_icon(ilk, GDI_ICON_SLIDE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_RADIOBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("状态框"));
		set_tool_item_icon(ilk, GDI_ICON_RADIO);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_CHECKBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("检查框"));
		set_tool_item_icon(ilk, GDI_ICON_CHECK);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_DATEBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("日期框"));
		set_tool_item_icon(ilk, GDI_ICON_DATE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_TIMEBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("时间框"));
		set_tool_item_icon(ilk, GDI_ICON_TIME);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_SHAPEBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("形状框"));
		set_tool_item_icon(ilk, GDI_ICON_SHAPE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_PUSHBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("按钮框"));
		set_tool_item_icon(ilk, GDI_ICON_PUSH);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_USERBOX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("自定义"));
		set_tool_item_icon(ilk, GDI_ICON_USER);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

int DialogPanel_OnClose(res_win_t widget)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	DialogPanel_Switch(widget);

	return (dialogctrl_get_dirty(pdt->hDialog)) ? 1 : 0;
}

void DialogPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	LINKPTR ptr_dialog = dialogctrl_fetch(pdt->hDialog);
	LINKPTR flk = get_dialog_next_item(ptr_dialog, LINK_FIRST);
	while (flk)
	{
		if (compare_text(pfd->sub_str, -1, get_dialog_item_name_ptr(flk), -1, 1) == 0)
		{
			dialogctrl_set_focus_item(pdt->hDialog, flk);
			break;
		}
		flk = get_dialog_next_item(ptr_dialog, flk);
	}
}

void DialogPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_DIALOG);
		pqo->ptrDoc = dialogctrl_fetch(pdt->hDialog);
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
		dialogctrl_set_dirty(pdt->hDialog, 0);
	}
}

void DialogPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	tchar_t token[RES_LEN];

	switch (cid)
	{
	case IDA_FILE_SAVE:
		DialogPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		DialogPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		DialogPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		DialogPanel_OnPreview(widget);
		break;
	case IDA_FILE_EXEC:
		DialogPanel_OnExec(widget);
		break;
	case IDA_EDIT_SELECTALL:
		DialogPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		DialogPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		DialogPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		DialogPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		DialogPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		DialogPanel_OnUndo(widget);
		break;
	case IDA_STYLE_PROPER:
		DialogPanel_OnCSSProper(widget);
		break;
	case IDA_STYLE_FONT_NAME:
		DialogPanel_OnFontName(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_SIZE:
		DialogPanel_OnFontSize(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_WEIGHT:
		DialogPanel_OnTextBold(widget);
		break;
	case IDA_STYLE_TEXT_COLOR:
		DialogPanel_OnTextColor(widget, (void*)data);
		break;
	case IDA_STYLE_PAINT_COLOR:
		DialogPanel_OnPaintColor(widget, (void*)data);
		break;
	case IDA_STYLE_DRAW_COLOR:
		DialogPanel_OnDrawColor(widget, (void*)data);
		break;
	case IDA_STYLE_TEXT_NEAR:
		DialogPanel_OnTextNear(widget);
		break;
	case IDA_STYLE_TEXT_CENTER:
		DialogPanel_OnTextCenter(widget);
		break;
	case IDA_STYLE_TEXT_FAR:
		DialogPanel_OnTextFar(widget);
		break;
	case IDA_STYLE_ALIGN_NEAR:
		DialogPanel_OnAlignNear(widget);
		break;
	case IDA_STYLE_ALIGN_CENTER:
		DialogPanel_OnAlignCenter(widget);
		break;
	case IDA_STYLE_ALIGN_FAR:
		DialogPanel_OnAlignFar(widget);
		break;
	case IDA_STYLE_SIZE_HORZ:
		DialogPanel_OnSizeHorz(widget);
		break;
	case IDA_STYLE_SIZE_VERT:
		DialogPanel_OnSizeVert(widget);
		break;
	case IDA_STYLE_SIZE_WIDTH:
		DialogPanel_OnSizeWidth(widget);
		break;
	case IDA_STYLE_SIZE_HEIGHT:
		DialogPanel_OnSizeHeight(widget);
		break;
	case IDA_STYLE_SENDBACK:
		DialogPanel_OnSendBack(widget);
		break;

	case IDA_APPEND_STATICBOX:
		DialogPanel_OnStaticBox(widget);
		break;
	case IDA_APPEND_EDITBOX:
		DialogPanel_OnEditBox(widget);
		break;
	case IDA_APPEND_LISTBOX:
		DialogPanel_OnListBox(widget);
		break;
	case IDA_APPEND_NAVIBOX:
		DialogPanel_OnNaviBox(widget);
		break;
	case IDA_APPEND_SPINBOX:
		DialogPanel_OnSpinBox(widget);
		break;
	case IDA_APPEND_SLIDEBOX:
		DialogPanel_OnSlideBox(widget);
		break;
	case IDA_APPEND_RADIOBOX:
		DialogPanel_OnRadioBox(widget);
		break;
	case IDA_APPEND_CHECKBOX:
		DialogPanel_OnCheckBox(widget);
		break;
	case IDA_APPEND_DATEBOX:
		DialogPanel_OnDateBox(widget);
		break;
	case IDA_APPEND_TIMEBOX:
		DialogPanel_OnTimeBox(widget);
		break;
	case IDA_APPEND_SHAPEBOX:
		DialogPanel_OnShapeBox(widget);
		break;
	case IDA_APPEND_PUSHBOX:
		DialogPanel_OnPushBox(widget);
		break;
	case IDA_APPEND_USERBOX:
		DialogPanel_OnUserBox(widget);
		break;

	case IDA_ATTRIBUTES:
		DialogPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		DialogPanel_OnStyleSheet(widget);
		break;

	case IDC_DIALOGPANEL_FONTNAME:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontname_menu_item(code, token, RES_LEN);
			DialogPanel_SelectAttr(widget, GDI_ATTR_FONT_FAMILY, token);
		}
		break;
	case IDC_DIALOGPANEL_FONTSIZE:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontsize_menu_item(code, token, RES_LEN);
			DialogPanel_SelectAttr(widget, GDI_ATTR_FONT_SIZE, token);
		}
		break;
	case IDC_DIALOGPANEL_FONTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			DialogPanel_SelectAttr(widget, GDI_ATTR_FONT_COLOR, token);
		}
		break;
	case IDC_DIALOGPANEL_PAINTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			DialogPanel_SelectAttr(widget, GDI_ATTR_FILL_COLOR, token);
		}
		break;
	case IDC_DIALOGPANEL_DRAWCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			DialogPanel_SelectAttr(widget, GDI_ATTR_STROKE_COLOR, token);
		}
		break;
	}
}

void DialogPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	DialogPanelDelta* pdt = GETDIALOGPANELDELTA(widget);

	if (phdr->id == IDC_DIALOGPANEL_DIALOG)
	{
		NOTICE_DIALOG* pnf = (NOTICE_DIALOG*)phdr;
		switch (pnf->code)
		{
		case NC_DIALOGCALCED:
			break;
		case NC_FIELDCALCED:
			break;
		case NC_DIALOGLBCLK:
			DialogPanel_Dialog_OnLBClick(widget, pnf);
			break;
		case NC_DIALOGRBCLK:
			DialogPanel_Dialog_OnRBClick(widget, pnf);
			break;
		case NC_DIALOGITEMSIZED:
			DialogPanel_Dialog_OnItemSize(widget, pnf);
			break;
		case NC_DIALOGITEMDROP:
			DialogPanel_Dialog_OnItemMove(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_DIALOGPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			DialogPanel_SetDirty(widget, 1);
			break;
		case NC_ENTITYUPDATE:
			DialogPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_DIALOGPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			DialogPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			DialogPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/**************************************************************************************************************/
res_win_t DialogPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(DialogPanel_OnCreate)
		EVENT_ON_DESTROY(DialogPanel_OnDestroy)
		EVENT_ON_CLOSE(DialogPanel_OnClose)
		EVENT_ON_SHOW(DialogPanel_OnShow)

		EVENT_ON_NOTICE(DialogPanel_OnNotice)
		EVENT_ON_COMMAND_FIND(DialogPanel_OnCommandFind)
		EVENT_ON_PARENT_COMMAND(DialogPanel_OnParentCommand)
		EVENT_ON_MENU_COMMAND(DialogPanel_OnMenuCommand)

		EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	ev.param = (void*)fpath;

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}