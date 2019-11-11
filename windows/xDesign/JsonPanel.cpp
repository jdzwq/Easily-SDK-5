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
#include "WEBPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"
#include "_Project.h"
#include "_Database.h"

#define IDC_WEBPANEL_REQUEST		201
#define IDC_WEBPANEL_RESPONSE		202
#define IDC_WEBPANEL_PROPER			203

#define WEBPANEL_GROUPITEM_WIDTH		(float)7
#define WEBPANEL_GROUPITEM_HEIGHT		(float)7

#define WEBPANEL_SECTION		_T("服务设置")

#define WEBSRV_XML			_T("xml")
#define WEBSRV_JSON			_T("json")

typedef struct tagWEBPanelDelta{
	res_win_t hRequest;
	res_win_t hResponse;
	res_win_t hProper;

	tchar_t szType[RES_LEN];
	tchar_t szFile[PATH_LEN];
}WEBPanelDelta;

#define GETWEBPANELDELTA(ph) 		(WEBPanelDelta*)widget_get_user_delta(ph)
#define SETWEBPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define WEBPANEL_ACCEL_COUNT	5
accel_t	WEBPANEL_ACCEL[WEBPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	WEBPanel_Switch(res_win_t widget);

bool_t	WEBPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	WEBPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void WEBPanel_Switch(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	if (!memoctrl_get_dirty(pdt->hRequest))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		memoctrl_set_dirty(pdt->hRequest, 0);
		break;
	}
}

/***************************************************************************************************************/
void WEBPanel_OnSelectAll(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	memoctrl_select_all(pdt->hRequest);
}

void WEBPanel_OnCopy(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	widget_copy(pdt->hRequest);
}

void WEBPanel_OnCut(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	widget_cut(pdt->hRequest);
}

void WEBPanel_OnPaste(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	widget_paste(pdt->hRequest);
}

void WEBPanel_OnRedo(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

}

void WEBPanel_OnUndo(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	widget_undo(pdt->hRequest);
}

bool_t WEBPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hRequest);

	if (!load_memo_from_text_file(ptrMemo, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("导入文件失败！"));

		return 0;
	}

	memoctrl_redraw(pdt->hRequest);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t WEBPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hRequest);

	bool_t rt = save_memo_to_text_file(ptrMemo, NULL, szFile);

	if (!rt)
	{
		MessageBox(widget, _T("保存文件错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	memoctrl_set_dirty(pdt->hRequest, 0);

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void WEBPanel_OnSave(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (compare_text(pdt->szType, -1, WEBSRV_XML, -1, 0) == 0)
		{
			if (!shell_get_filename(widget, szPath, _T("xml file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
				return;
		}
		else if (compare_text(pdt->szType, -1, WEBSRV_JSON, -1, 0) == 0)
		{
			if (!shell_get_filename(widget, szPath, _T("json file(*.json)\0*.json\0"), _T("json"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
				return;
		}
		else
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	WEBPanel_SaveFile(widget, szFile);
}

void WEBPanel_OnSaveAs(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	tchar_t szPath[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (compare_text(pdt->szType, -1, WEBSRV_XML, -1, 0) == 0)
	{
		if (!shell_get_filename(widget, szPath, _T("xml file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;
	}
	else if (compare_text(pdt->szType, -1, WEBSRV_JSON, -1, 0) == 0)
	{
		if (!shell_get_filename(widget, szPath, _T("json file(*.json)\0*.json\0"), _T("json"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;
	}
	else
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hRequest);

	bool_t rt = save_memo_to_text_file(ptrMemo, NULL, szFile);

	if (!rt)
	{
		MessageBox(widget, _T("保存文件错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
	}
}

void WEBPanel_OnExport(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

}

void WEBPanel_OnExecute(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

}

void WEBPanel_OnSchema(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

}

void WEBPanel_OnPrint(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);
}

void WEBPanel_OnPreview(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

}

/***********************************************************************************************/

int WEBPanel_OnCreate(res_win_t widget, void* data)
{
	WEBPanelDelta* pdt = (WEBPanelDelta*)xmem_alloc(sizeof(WEBPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETWEBPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(WEBPANEL_ACCEL, WEBPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("85%"));

	LINKPTR ilkLeft = get_split_first_child_item(ptrSplit);
	LINKPTR ilkProper = get_split_last_child_item(ptrSplit);

	split_item(ilkLeft, 1);
	set_split_item_ratio(ilkLeft, _T("50%"));

	LINKPTR ilkRequest = get_split_first_child_item(ilkLeft);
	LINKPTR ilkResponse = get_split_last_child_item(ilkLeft);

	widget_get_client_rect(widget, &xr);
	pdt->hRequest = memoctrl_create(_T("WEBRequest"), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, widget);

	widget_set_user_id(pdt->hRequest, IDC_WEBPANEL_REQUEST);
	widget_set_owner(pdt->hRequest, widget);

	LINKPTR ptrRequest = create_memo_doc();
	memoctrl_attach(pdt->hRequest, ptrRequest);
	memoctrl_set_lock(pdt->hRequest, 0);

	set_split_item_delta(ilkRequest, pdt->hRequest);
	widget_show(pdt->hRequest, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hResponse = memoctrl_create(_T("WEBResponse"), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_user_id(pdt->hResponse, IDC_WEBPANEL_RESPONSE);
	widget_set_owner(pdt->hResponse, widget);

	LINKPTR ptrResponse = create_memo_doc();
	memoctrl_attach(pdt->hResponse, ptrResponse);

	set_split_item_delta(ilkResponse, pdt->hResponse);
	widget_show(pdt->hResponse, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("WEBProper"), WD_STYLE_CONTROL | WD_STYLE_HSCROLL, &xr, widget);

	widget_set_user_id(pdt->hProper, IDC_WEBPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	LINKPTR ent = write_proper(ptrProper, WEBPANEL_SECTION, -1, _T("Type"), -1, _T(""), -1);
	set_entity_image(ent, BMP_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_editable(ent, 1);
	set_entity_options(ent, _T("xml~xml;json~json;"), -1);

	ent = write_proper(ptrProper, WEBPANEL_SECTION, -1, _T("URL"), -1, _T(""), -1);
	set_entity_image(ent, BMP_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	ent = write_proper(ptrProper, WEBPANEL_SECTION, -1, _T("SECRET-ID"), -1, _T(""), -1);
	set_entity_image(ent, BMP_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	ent = write_proper(ptrProper, WEBPANEL_SECTION, -1, _T("SECRET-KEY"), -1, _T(""), -1);
	set_entity_image(ent, BMP_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	set_section_image(section_from_entity(ent), BMP_PROPER);

	properctrl_attach(pdt->hProper, ptrProper);
	properctrl_set_lock(pdt->hProper, 0);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!WEBPanel_OpenFile(widget, szParam))
			return -1;

		memoctrl_redraw(pdt->hRequest);
	}

	return 0;
}

void WEBPanel_OnDestroy(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hRequest))
	{
		LINKPTR ptrMemo = memoctrl_detach(pdt->hRequest);
		if (ptrMemo)
			destroy_memo_doc(ptrMemo);

		widget_destroy(pdt->hRequest);
	}

	if (widget_is_valid(pdt->hResponse))
	{
		LINKPTR ptrMemo = memoctrl_detach(pdt->hResponse);
		if (ptrMemo)
			destroy_memo_doc(ptrMemo);

		widget_destroy(pdt->hResponse);
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

int WEBPanel_OnClose(res_win_t widget)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	WEBPanel_Switch(widget);

	return (memoctrl_get_dirty(pdt->hRequest)) ? 1 : 0;
}

void WEBPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hRequest))
	{
		widget_set_focus(pdt->hRequest);
	}
}

void WEBPanel_OnShow(res_win_t widget, bool_t bShow)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, WEBPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, WEBPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_SELECTALL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("全选"));
		set_tool_item_image(ilk, BMP_SELECTALL);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_DELETE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除"));
		set_tool_item_image(ilk, BMP_DELETE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_COPY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("拷贝"));
		set_tool_item_image(ilk, BMP_COPY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_CUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("剪切"));
		set_tool_item_image(ilk, BMP_CUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_PASTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("粘贴"));
		set_tool_item_image(ilk, BMP_PASTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_UNDO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("撤销"));
		set_tool_item_image(ilk, BMP_UNDO);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_DATABASE_EXECUTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("执行"));
		set_tool_item_image(ilk, BMP_EXEC);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void WEBPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	LINKPTR ptr_memo = memoctrl_fetch(pdt->hRequest);
	
}

void WEBPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);
}

void WEBPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		WEBPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		WEBPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		WEBPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		WEBPanel_OnPreview(widget);
		break;
	case IDA_FILE_EXPORT:
		WEBPanel_OnExport(widget);
		break;

	case IDA_EDIT_SELECTALL:
		WEBPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_COPY:
		WEBPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		WEBPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		WEBPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		WEBPanel_OnUndo(widget);
		break;

	case IDA_DATABASE_EXECUTE:
		WEBPanel_OnExecute(widget);
		break;
	case IDA_DATABASE_SCHEMA:
		WEBPanel_OnSchema(widget);
		break;
	}

}

void WEBPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	WEBPanelDelta* pdt = GETWEBPANELDELTA(widget);

	if (phdr->id == IDC_WEBPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_ENTITYCOMMIT:
			break;
		case NC_ENTITYUPDATE:
			break;
		}
	}
}

/*********************************************************************************************************/
res_win_t WEBPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(WEBPanel_OnCreate)
	EVENT_ON_DESTROY(WEBPanel_OnDestroy)
	EVENT_ON_CLOSE(WEBPanel_OnClose)
	EVENT_ON_SHOW(WEBPanel_OnShow)

	EVENT_ON_NOTICE(WEBPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(WEBPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(WEBPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(WEBPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}