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
#include "SQLPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"
#include "_Project.h"
#include "_Database.h"

#define IDC_SQLPANEL_MEMO			201
#define IDC_SQLPANEL_GRID			202

#define SQLPANEL_GROUPITEM_WIDTH		(float)8
#define SQLPANEL_GROUPITEM_HEIGHT		(float)7

typedef struct tagSQLPanelDelta{
	res_win_t hGrid;
	res_win_t hMemo;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}SQLPanelDelta;

#define GETSQLPANELDELTA(ph) 		(SQLPanelDelta*)widget_get_user_delta(ph)
#define SETSQLPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define SQLPANEL_ACCEL_COUNT	5
accel_t	SQLPANEL_ACCEL[SQLPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	SQLPanel_Switch(res_win_t widget);

bool_t	SQLPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	SQLPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void SQLPanel_Switch(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	if (!memoctrl_get_dirty(pdt->hMemo))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		memoctrl_set_dirty(pdt->hMemo, 0);
		break;
	}
}

/***************************************************************************************************************/
void SQLPanel_OnSelectAll(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	memoctrl_select_all(pdt->hMemo);
}

void SQLPanel_OnCopy(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	widget_copy(pdt->hMemo);
}

void SQLPanel_OnCut(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	widget_cut(pdt->hMemo);
}

void SQLPanel_OnPaste(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	widget_paste(pdt->hMemo);
}

void SQLPanel_OnRedo(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

}

void SQLPanel_OnUndo(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	widget_undo(pdt->hMemo);
}

bool_t SQLPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hMemo);

	if (!load_memo_from_text_file(ptrMemo, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("导入文件失败！"));

		return 0;
	}

	memoctrl_redraw(pdt->hMemo);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t SQLPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hMemo);

	bool_t rt = save_memo_to_text_file(ptrMemo, NULL, szFile);

	if (!rt)
	{
		MessageBox(widget, _T("保存文件错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	memoctrl_set_dirty(pdt->hMemo, 0);

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void SQLPanel_OnSave(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("SQL file(*.sql)\0*.sql\0"), _T("sql"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	SQLPanel_SaveFile(widget, szFile);
}

void SQLPanel_OnSaveAs(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	tchar_t szPath[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("SQL file(*.sql)\0*.sql\0"), _T("sql"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hMemo);

	bool_t rt = save_memo_to_text_file(ptrMemo, NULL, szFile);

	if (!rt)
	{
		MessageBox(widget, _T("保存文件错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
	}
}

void SQLPanel_OnSheet(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("grid sheet file(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrGrid);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrGrid = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		ShowMsg(MSGICO_ERR, _T("保存网格文件失败！"));
	}
}

void SQLPanel_OnExport(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("grid csv file(*.csv)\0*.csv\0"), _T("csv"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	if(!save_grid_to_csv_file(ptrGrid, 1, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("保存网格数据失败！"));
		return;
	}
}

void SQLPanel_OnExecute(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	LINKPTR ptr_prj = MainFrame_GetProject(g_hMain);
	if (!ptr_prj)
	{
		ShowMsg(MSGICO_TIP, _T("未定义工程！"));
		return;
	}

	int len = memoctrl_get_selected_text(pdt->hMemo, NULL, MAX_LONG);
	if (!len)
		return;

	tchar_t sz_conn[PATH_LEN] = { 0 };
	Project_GetConfig(ptr_prj, _T("RDS"), sz_conn, PATH_LEN);
	if (is_null(sz_conn))
	{
		ShowMsg(MSGICO_TIP, _T("未定义数据连接服务！"));
		return;
	}

	DBCTX* pct = DBOpen(sz_conn);

	if (!pct)
		return;

	tchar_t* szSQL = xsalloc(len + 1);
	memoctrl_get_selected_text(pdt->hMemo, szSQL, len);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	if (!DBSelect(pct, ptrGrid, szSQL))
	{
		xsfree(szSQL);

		DBClose(pct);
		return;
	}

	xsfree(szSQL);
	DBClose(pct);

	gridctrl_redraw(pdt->hGrid, 1);
}

void SQLPanel_OnSchema(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	LINKPTR ptr_prj = MainFrame_GetProject(g_hMain);
	if (!ptr_prj)
	{
		ShowMsg(MSGICO_TIP, _T("未定义工程！"));
		return;
	}

	int len = memoctrl_get_selected_text(pdt->hMemo, NULL, MAX_LONG);
	if (!len)
		return;

	tchar_t sz_conn[PATH_LEN] = { 0 };
	Project_GetConfig(ptr_prj, _T("RDS"), sz_conn, PATH_LEN);
	if (is_null(sz_conn))
	{
		ShowMsg(MSGICO_TIP, _T("未定义数据连接服务！"));
		return;
	}

	DBCTX* pct = DBOpen(sz_conn);

	if (!pct)
		return;

	tchar_t* szSQL = xsalloc(len + 1);
	memoctrl_get_selected_text(pdt->hMemo, szSQL, len);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	if (!DBSchema(pct, ptrGrid, szSQL))
	{
		xsfree(szSQL);

		DBClose(pct);
		return;
	}

	xsfree(szSQL);
	DBClose(pct);

	gridctrl_redraw(pdt->hGrid, 1);
}

void SQLPanel_OnPrint(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);
}

void SQLPanel_OnPreview(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hMemo);

	int page = memoctrl_get_cur_page(pdt->hMemo);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	xfont_t xf;
	xface_t xa;

	widgetex_get_xfont(pdt->hMemo, &xf);
	widgetex_get_xface(pdt->hMemo, &xa);

	svg_print_memo(svg, &xf, &xa, ptrMemo, page);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	xsprintf(token, _T("第%d页"), page);
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WD_SHOW_FULLSCREEN);
}

/***********************************************************************************************/

int SQLPanel_OnCreate(res_win_t widget, void* data)
{
	SQLPanelDelta* pdt = (SQLPanelDelta*)xmem_alloc(sizeof(SQLPanelDelta));
	xrect_t xr;

	widgetex_hand_create(widget);

	SETSQLPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(SQLPANEL_ACCEL, SQLPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 1);
	set_split_item_ratio(ptrSplit, _T("70%"));

	LINKPTR ilkMemo = get_split_first_child_item(ptrSplit);
	LINKPTR ilkGrid = get_split_last_child_item(ptrSplit);

	widget_get_client_rect(widget, &xr);
	pdt->hMemo = memoctrl_create(_T("SQLMemo"), WD_STYLE_CONTROL, &xr, widget);

	widget_set_user_id(pdt->hMemo, IDC_SQLPANEL_MEMO);
	widget_set_owner(pdt->hMemo, widget);

	LINKPTR ptrMemo = create_memo_doc();
	memoctrl_attach(pdt->hMemo, ptrMemo);
	memoctrl_set_lock(pdt->hMemo, 0);

	set_split_item_delta(ilkMemo, pdt->hMemo);
	widget_show(pdt->hMemo, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hGrid = gridctrl_create(_T("SQLGrid"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hGrid, IDC_SQLPANEL_GRID);
	widget_set_owner(pdt->hGrid, widget);

	LINKPTR ptrGrid = create_grid_doc();
	gridctrl_attach(pdt->hGrid, ptrGrid);

	set_split_item_delta(ilkGrid, pdt->hGrid);
	widget_show(pdt->hGrid, WD_SHOW_NORMAL);

	widgetex_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!SQLPanel_OpenFile(widget, szParam))
			return -1;

		memoctrl_redraw(pdt->hMemo);
	}

	return 0;
}

void SQLPanel_OnDestroy(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widgetex_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hMemo))
	{
		LINKPTR ptrMemo = memoctrl_detach(pdt->hMemo);
		if (ptrMemo)
			destroy_memo_doc(ptrMemo);

		widget_destroy(pdt->hMemo);
	}

	if (widget_is_valid(pdt->hGrid))
	{
		LINKPTR ptrGrid = gridctrl_detach(pdt->hGrid);
		if (ptrGrid)
			destroy_grid_doc(ptrGrid);

		widget_destroy(pdt->hGrid);
	}

	xmem_free(pdt);

	widgetex_hand_destroy(widget);
}

int SQLPanel_OnClose(res_win_t widget)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	SQLPanel_Switch(widget);

	return (memoctrl_get_dirty(pdt->hMemo)) ? 1 : 0;
}

void SQLPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hMemo))
	{
		widget_set_focus(pdt->hMemo);
	}
}

void SQLPanel_OnShow(res_win_t widget, bool_t bShow)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, SQLPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, SQLPANEL_GROUPITEM_HEIGHT);
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
		xsprintf(token, _T("%d"), IDA_DATABASE_EXECUTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("查询"));
		set_tool_item_icon(ilk, ICON_EXECUTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_DATABASE_SCHEMA);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("模式"));
		set_tool_item_icon(ilk, ICON_SCHEMA);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void SQLPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	LINKPTR ptr_memo = memoctrl_fetch(pdt->hMemo);
	
}

void SQLPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_MEMO);
		pqo->ptrDoc = memoctrl_fetch(pdt->hMemo);
	}
	else if (code == COMMAND_RENAME)
	{
		tchar_t szPath[PATH_LEN], szExt[INT_LEN];
		const tchar_t* nname = (const tchar_t*)data;

		if (!is_null(pdt->szFile) && !is_null(nname))
		{
			split_path(pdt->szFile, szPath, NULL, szExt);
			xsprintf(pdt->szFile, _T("%s/%s.%s"), szPath, nname, szExt);
		}
	}
	else if (code == COMMAND_REMOVE)
	{
		memoctrl_set_dirty(pdt->hMemo, 0);
	}
}

void SQLPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		SQLPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		SQLPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		SQLPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		SQLPanel_OnPreview(widget);
		break;
	case IDA_FILE_SCHEMA:
		SQLPanel_OnSheet(widget);
		break;
	case IDA_FILE_EXPORT:
		SQLPanel_OnExport(widget);
		break;

	case IDA_EDIT_SELECTALL:
		SQLPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_COPY:
		SQLPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		SQLPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		SQLPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		SQLPanel_OnUndo(widget);
		break;

	case IDA_DATABASE_EXECUTE:
		SQLPanel_OnExecute(widget);
		break;
	case IDA_DATABASE_SCHEMA:
		SQLPanel_OnSchema(widget);
		break;
	}

}

void SQLPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	SQLPanelDelta* pdt = GETSQLPANELDELTA(widget);

	if (phdr->id == IDC_SQLPANEL_MEMO)
	{
		NOTICE_MEMO* pnf = (NOTICE_MEMO*)phdr;
		switch (pnf->code)
		{
		case NC_MEMOCALCED:
			break;
		case NC_MEMOLBCLK:
			break;
		}
	}
}

/*********************************************************************************************************/
res_win_t SQLPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(SQLPanel_OnCreate)
	EVENT_ON_DESTROY(SQLPanel_OnDestroy)
	EVENT_ON_CLOSE(SQLPanel_OnClose)
	EVENT_ON_SHOW(SQLPanel_OnShow)

	EVENT_ON_NOTICE(SQLPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(SQLPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(SQLPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(SQLPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}