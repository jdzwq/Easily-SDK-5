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
#include "RichPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_RICHPANEL_RICH			201
#define IDC_RICHPANEL_PROPER		202
#define IDC_RICHPANEL_MENU			203

#define RICHPANEL_GROUPITEM_WIDTH		(float)8
#define RICHPANEL_GROUPITEM_HEIGHT		(float)7

typedef struct tagRichPanelDelta{
	res_win_t hProper;
	res_win_t hRich;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}RichPanelDelta;

#define GETRICHPANELDELTA(ph) 		(RichPanelDelta*)widget_get_user_delta(ph)
#define SETRICHPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define RICHPANEL_ACCEL_COUNT	5
accel_t	RICHPANEL_ACCEL[RICHPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	RichPanel_Switch(res_win_t widget);

bool_t	RichPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	RichPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void RichPanel_Switch(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	if (!richctrl_get_dirty(pdt->hRich))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		richctrl_set_dirty(pdt->hRich, 0);
		break;
	}
}

/***************************************************************************************************************/
void RichPanel_OnSelectAll(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	richctrl_select_all(pdt->hRich);
}

void RichPanel_OnDelete(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);
	XDL_ASSERT(ptrRich);

	LINKPTR ptrNode = richctrl_get_focus_anch(pdt->hRich);

	if (!ptrNode)
		return;

	richctrl_delete_anch(pdt->hRich, ptrNode);
}

void RichPanel_OnInsert(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);
	LINKPTR ptrPos = richctrl_get_focus_anch(pdt->hRich);

	ptrPos = (ptrPos) ? get_rich_prev_anch(ptrRich, ptrPos) : LINK_LAST;

	LINKPTR ilk = richctrl_insert_anch(pdt->hRich, ptrPos);
	if (!ilk)
		return;

	tchar_t token[INT_LEN + 1] = { 0 };

	xsprintf(token, _T("anch%d"), get_rich_anch_count(ptrRich));
	set_rich_anch_name(ilk, token);

	xsprintf(token, _T("标题%d"), get_rich_anch_count(ptrRich));
	set_rich_anch_title(ilk, token);

	richctrl_redraw(pdt->hRich);
}

void RichPanel_OnCopy(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	widget_post_command(pdt->hRich, COMMAND_COPY, IDC_EDITMENU, 0);
}

void RichPanel_OnCut(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	widget_post_command(pdt->hRich, COMMAND_CUT, IDC_EDITMENU, 0);
}

void RichPanel_OnPaste(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	widget_post_command(pdt->hRich, COMMAND_PASTE, IDC_EDITMENU, 0);
}

void RichPanel_OnRedo(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

}

void RichPanel_OnUndo(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	widget_post_command(pdt->hRich, COMMAND_UNDO, IDC_EDITMENU, 0);
}

void RichPanel_Rich_OnNodeChange(res_win_t widget, NOTICE_RICH* pnf)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);

	if (pnf->anch)
	{
		properbag_write_rich_anch_attributes(ptrProper, pnf->anch);
	}
	else
	{
		properbag_write_rich_attributes(ptrProper, pnf->rich);
	}

	properctrl_redraw(pdt->hProper);
}

void RichPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);
	LINKPTR ptrNode = richctrl_get_focus_anch(pdt->hRich);

	if (ptrNode)
		properbag_read_rich_anch_attributes(pnp->proper, ptrNode);
	else
		properbag_read_rich_attributes(pnp->proper, ptrRich);

	richctrl_redraw(pdt->hRich);
}

bool_t RichPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptrMeta = create_meta_doc();
	if (!load_dom_doc_from_file(ptrMeta, NULL, szFile))
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	if (compare_text(get_meta_doc_name_ptr(ptrMeta), -1, DOC_RICH, -1, 1) != 0)
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("非表单文档！"));

		return 0;
	}

	get_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, RES_LEN);
	get_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, RES_LEN);

	LINKPTR newRich = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	LINKPTR orgRich = richctrl_detach(pdt->hRich);
	destroy_rich_doc(orgRich);

	richctrl_attach(pdt->hRich, newRich);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t RichPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrRich);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrRich = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		MessageBox(widget, _T("保存文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	richctrl_set_dirty(pdt->hRich, 0);

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void RichPanel_OnSave(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("Rich Meta File(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	RichPanel_SaveFile(widget, szFile);
}

void RichPanel_OnSaveAs(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szType[RES_LEN] = { 0 };
	bool_t rt;

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Rich Meta File(*.sheet)\0*.sheet\0Svg Image File(*.svg)\0*.svg\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	split_path(szFile, NULL, NULL, szType);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);

	if (compare_text(szType, -1, _T("svg"), -1, 1) == 0)
	{
		LINKPTR ptrSvg = create_svg_doc();

		xfont_t xf;
		xface_t xa;

		widget_get_xfont(pdt->hRich, &xf);
		widget_get_xface(pdt->hRich, &xa);

		int page = richctrl_get_cur_page(pdt->hRich);

		svg_print_rich(ptrSvg, &xf, &xa, ptrRich, page);

		rt = save_dom_doc_to_file(ptrSvg, NULL, szFile);

		destroy_svg_doc(ptrSvg);
	}
	else
	{
		LINKPTR ptrMeta = create_meta_doc();

		set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
		set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

		attach_meta_body_node(ptrMeta, ptrRich);

		bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

		ptrRich = detach_meta_body_node(ptrMeta);
		destroy_meta_doc(ptrMeta);
	}

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void RichPanel_OnSchema(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml schema file(*.schema)\0*.schema\0"), _T("schema"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);
	LINKPTR ptrSch = create_schema_doc();

	export_rich_schema(ptrRich, ptrSch);

	if (!save_dom_doc_to_file(ptrSch, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("保存模式文件失败！"));
	}

	destroy_schema_doc(ptrSch);
}

void RichPanel_OnExport(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);

	LINKPTR ptrDom = create_dom_doc();

	export_rich_data(ptrRich, NULL, ptrDom);

	if (!save_dom_doc_to_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("保存模式数据失败！"));
		return;
	}

	destroy_dom_doc(ptrDom);
}

void RichPanel_OnImport(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);

	LINKPTR ptrDom = create_dom_doc();

	if (!load_dom_doc_from_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("导入模式数据失败！"));
		return;
	}

	import_rich_data(ptrRich, NULL, ptrDom);

	destroy_dom_doc(ptrDom);

	richctrl_redraw(pdt->hRich);
}

void RichPanel_OnPrint(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);
}

void RichPanel_OnPreview(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptrRich = richctrl_fetch(pdt->hRich);

	int page = richctrl_get_cur_page(pdt->hRich);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	xfont_t xf;
	xface_t xa;

	widget_get_xfont(pdt->hRich, &xf);
	widget_get_xface(pdt->hRich, &xa);

	svg_print_rich(svg, &xf, &xa, ptrRich, page);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	xsprintf(token, _T("%s 第%d页"), get_rich_title_ptr(ptrRich), page);
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WS_SHOW_FULLSCREEN);
}

/***********************************************************************************************/

int RichPanel_OnCreate(res_win_t widget, void* data)
{
	RichPanelDelta* pdt = (RichPanelDelta*)xmem_alloc(sizeof(RichPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETRICHPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(RICHPANEL_ACCEL, RICHPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkRich = get_split_first_child_item(ptrSplit);
	LINKPTR ilkProper = get_split_last_child_item(ptrSplit);

	widget_get_client_rect(widget, &xr);
	pdt->hRich = richctrl_create(_T("RichPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr, widget);

	widget_set_user_id(pdt->hRich, IDC_RICHPANEL_RICH);
	widget_set_owner(pdt->hRich, widget);

	LINKPTR ptrRich = create_rich_doc();
	richctrl_attach(pdt->hRich, ptrRich);
	richctrl_set_lock(pdt->hRich, 0);

	set_split_item_delta(ilkRich, pdt->hRich);
	widget_show(pdt->hRich, WS_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("RichProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_RICHPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!RichPanel_OpenFile(widget, szParam))
			return -1;

		richctrl_redraw(pdt->hRich);
	}

	return 0;
}

void RichPanel_OnDestroy(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hRich))
	{
		LINKPTR ptrRich = richctrl_detach(pdt->hRich);
		if (ptrRich)
			destroy_rich_doc(ptrRich);

		widget_destroy(pdt->hRich);
	}

	if (widget_is_valid(pdt->hProper))
	{
		LINKPTR ptrProper = properctrl_detach(pdt->hProper);
		if (ptrProper)
			destroy_proper_doc(ptrProper);

		widget_destroy(pdt->hProper);
	}

	xmem_free(pdt);

	widget_hand_destroy(widget);
}

int RichPanel_OnClose(res_win_t widget)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	RichPanel_Switch(widget);

	return (richctrl_get_dirty(pdt->hRich)) ? 1 : 0;
}

void RichPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hRich))
	{
		widget_set_focus(pdt->hRich);
	}
}

void RichPanel_OnShow(res_win_t widget, bool_t bShow)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, RICHPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, RICHPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_SELECTALL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("全选"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_SELECTALL);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_DELETE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_DELETE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_COPY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("拷贝"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_COPY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_CUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("剪切"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_CUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_PASTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("粘贴"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_PASTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_UNDO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("撤销"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_UNDO);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_INSERT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_PLUS);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void RichPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	LINKPTR ptr_rich = richctrl_fetch(pdt->hRich);
	LINKPTR ilk = get_rich_next_anch(ptr_rich, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(pfd->sub_str, -1, get_rich_anch_name_ptr(ilk), -1, 1) == 0)
		{
			richctrl_set_focus_anch(pdt->hRich, ilk);
			break;
		}
		ilk = get_rich_next_anch(ptr_rich, ilk);
	}
}

void RichPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_RICH);
		pqo->ptrDoc = richctrl_fetch(pdt->hRich);
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
		richctrl_set_dirty(pdt->hRich, 0);
	}
}

void RichPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		RichPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		RichPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		RichPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		RichPanel_OnPreview(widget);
		break;
	case IDA_FILE_SCHEMA:
		RichPanel_OnSchema(widget);
		break;
	case IDA_FILE_EXPORT:
		RichPanel_OnExport(widget);
		break;
	case IDA_FILE_IMPORT:
		RichPanel_OnImport(widget);
		break;

	case IDA_EDIT_SELECTALL:
		RichPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		RichPanel_OnDelete(widget);
		break;
	case IDA_EDIT_INSERT:
		RichPanel_OnInsert(widget);
		break;
	case IDA_EDIT_COPY:
		RichPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		RichPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		RichPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		RichPanel_OnUndo(widget);
		break;

	case IDC_RICHPANEL_MENU:
		widget_destroy((res_win_t)data);
		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;
	}

}

void RichPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	RichPanelDelta* pdt = GETRICHPANELDELTA(widget);

	if (phdr->id == IDC_RICHPANEL_RICH)
	{
		NOTICE_RICH* pnf = (NOTICE_RICH*)phdr;
		switch (pnf->code)
		{
		case NC_RICHCALCED:
			break;
		case NC_RICHLBCLK:
			break;
		case NC_RICHANCHCHANGED:
			RichPanel_Rich_OnNodeChange(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_RICHPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			richctrl_set_dirty(pdt->hRich, 1);
			break;
		case NC_ENTITYUPDATE:
			RichPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
}

/*********************************************************************************************************/
res_win_t RichPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(RichPanel_OnCreate)
	EVENT_ON_DESTROY(RichPanel_OnDestroy)
	EVENT_ON_CLOSE(RichPanel_OnClose)
	EVENT_ON_SHOW(RichPanel_OnShow)

	EVENT_ON_NOTICE(RichPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(RichPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(RichPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(RichPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}