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
#include "JsonPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"
#include "_Project.h"
#include "_Database.h"

#define IDC_JSONPANEL_REQUEST		201
#define IDC_JSONPANEL_RESPONSE		202
#define IDC_JSONPANEL_PROPER		203

#define JSONPANEL_GROUPITEM_WIDTH		(float)8
#define JSONPANEL_GROUPITEM_HEIGHT		(float)7

#define JSONPANEL_SECTION		_T("服务设置")

typedef struct tagJsonPanelDelta{
	res_win_t hRequest;
	res_win_t hResponse;
	res_win_t hProper;

	tchar_t szType[RES_LEN];
	tchar_t szFile[PATH_LEN];
}JsonPanelDelta;

#define GETJSONPANELDELTA(ph) 		(JsonPanelDelta*)widget_get_user_delta(ph)
#define SETJSONPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define JSONPANEL_ACCEL_COUNT	5
accel_t	JSONPANEL_ACCEL[JSONPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	JsonPanel_Switch(res_win_t widget);

bool_t	JsonPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	JsonPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void JsonPanel_Switch(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

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

static bool_t JsonPanel_Call(const tchar_t* sz_url, const tchar_t* sz_id, const tchar_t* sz_key, LINKPTR json_in, LINKPTR json_out)
{
	xhand_t xhttp = NULL;

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_errcode[NUM_LEN + 1] = { 0 };
	tchar_t sz_errtext[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, sz_key, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, sz_id, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	if (!xhttp_send_json(xhttp, json_in))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xhttp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, sz_errcode, sz_errtext, ERR_LEN);
		raise_user_error(sz_errcode, sz_errtext);
	}

	if (!xhttp_recv_json(xhttp, json_out))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(xhttp);
	xhttp = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}
/***************************************************************************************************************/
void JsonPanel_OnSelectAll(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	memoctrl_select_all(pdt->hRequest);
}

void JsonPanel_OnCopy(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	widget_post_command(pdt->hRequest, COMMAND_COPY, IDC_EDITMENU, 0);
}

void JsonPanel_OnCut(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	widget_post_command(pdt->hRequest, COMMAND_CUT, IDC_EDITMENU, 0);
}

void JsonPanel_OnPaste(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	widget_post_command(pdt->hRequest, COMMAND_PASTE, IDC_EDITMENU, 0);
}

void JsonPanel_OnRedo(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

}

void JsonPanel_OnUndo(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	widget_post_command(pdt->hRequest, COMMAND_UNDO, IDC_EDITMENU, 0);
}

void JsonPanel_NewFile(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	LINKPTR ptrJSON = create_json_doc();

	LINKPTR nlk_rowset = ptrJSON;
	set_dom_node_name(nlk_rowset, _T("rowset"), -1);

	LINKPTR nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
	set_dom_node_name(nlk_row, _T("row"), -1);

	LINKPTR nlk = insert_dom_node(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("item_name"), -1);
	set_dom_node_text(nlk, _T("item_value"), -1);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hRequest);

	format_json_doc_to_memo(ptrJSON, ptrMemo);

	destroy_json_doc(ptrJSON);

	memoctrl_redraw(pdt->hRequest);
}

bool_t JsonPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

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

bool_t JsonPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

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

void JsonPanel_OnSave(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("json file(*.json)\0*.json\0"), _T("json"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	JsonPanel_SaveFile(widget, szFile);
}

void JsonPanel_OnSaveAs(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	tchar_t szPath[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("json file(*.json)\0*.json\0"), _T("json"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
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

void JsonPanel_OnExport(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

}

void JsonPanel_OnSchema(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

}

void JsonPanel_OnPrint(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);
}

void JsonPanel_OnPreview(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

}

void JsonPanel_OnExecute(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_id[RES_LEN] = { 0 };
	tchar_t sz_key[RES_LEN] = { 0 };

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);

	read_proper(ptrProper, JSONPANEL_SECTION, -1, _T("URL"), -1, sz_url, PATH_LEN);
	read_proper(ptrProper, JSONPANEL_SECTION, -1, _T("SECRET-ID"), -1, sz_id, RES_LEN);
	read_proper(ptrProper, JSONPANEL_SECTION, -1, _T("SECRET-KEY"), -1, sz_key, RES_LEN);

	if (is_null(sz_url))
	{
		ShowMsg(0, _T("Json服务地址未设置！"));
		return;
	}

	LINKPTR ptrRequest = memoctrl_fetch(pdt->hRequest);

	LINKPTR json_in = create_json_doc();
	
	if (!parse_json_doc_from_memo(json_in, ptrRequest))
	{
		destroy_json_doc(json_in);

		ShowMsg(0, _T("Json格式有错误！"));
		return;
	}

	LINKPTR ptrResponse = memoctrl_fetch(pdt->hResponse);
	clear_memo_doc(ptrResponse);
	memoctrl_redraw(pdt->hResponse);

	LINKPTR json_out = create_json_doc();

	if (JsonPanel_Call(sz_url, sz_id, sz_key, json_in, json_out))
	{
		format_json_doc_to_memo(json_out, ptrResponse);
	}
	else
	{
		tchar_t err_code[NUM_LEN] = { 0 };
		tchar_t err_text[ERR_LEN] = { 0 };

		get_last_error(err_code, err_text, ERR_LEN);

		parse_memo_doc(ptrResponse, err_text, -1);
	}

	destroy_json_doc(json_in);
	destroy_json_doc(json_out);

	memoctrl_redraw(pdt->hResponse);
}

/***********************************************************************************************/

int JsonPanel_OnCreate(res_win_t widget, void* data)
{
	JsonPanelDelta* pdt = (JsonPanelDelta*)xmem_alloc(sizeof(JsonPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETJSONPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(JSONPANEL_ACCEL, JSONPANEL_ACCEL_COUNT);
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
	pdt->hRequest = memoctrl_create(_T("JsonRequest"), WD_STYLE_CONTROL, &xr, widget);

	widget_set_user_id(pdt->hRequest, IDC_JSONPANEL_REQUEST);
	widget_set_owner(pdt->hRequest, widget);

	LINKPTR ptrRequest = create_memo_doc();
	memoctrl_attach(pdt->hRequest, ptrRequest);
	memoctrl_set_lock(pdt->hRequest, 0);

	set_split_item_delta(ilkRequest, pdt->hRequest);
	widget_show(pdt->hRequest, WS_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hResponse = memoctrl_create(_T("JsonResponse"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hResponse, IDC_JSONPANEL_RESPONSE);
	widget_set_owner(pdt->hResponse, widget);

	LINKPTR ptrResponse = create_memo_doc();
	memoctrl_attach(pdt->hResponse, ptrResponse);

	set_split_item_delta(ilkResponse, pdt->hResponse);
	widget_show(pdt->hResponse, WS_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("JsonProper"), WD_STYLE_CONTROL, &xr, widget);

	widget_set_user_id(pdt->hProper, IDC_JSONPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	LINKPTR ent;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_id[RES_LEN] = { 0 };
	tchar_t sz_key[RES_LEN] = { 0 };

	LoadPreference(_T("JSON"), _T("URL"), sz_url);
	LoadPreference(_T("JSON"), _T("SECRET-ID"), sz_id);
	LoadPreference(_T("JSON"), _T("SECRET-KEY"), sz_key);

	ent = write_proper(ptrProper, JSONPANEL_SECTION, -1, _T("URL"), -1, sz_url, -1);
	set_entity_icon(ent, GDI_ATTR_GIZMO_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	ent = write_proper(ptrProper, JSONPANEL_SECTION, -1, _T("SECRET-ID"), -1, sz_id, -1);
	set_entity_icon(ent, GDI_ATTR_GIZMO_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	ent = write_proper(ptrProper, JSONPANEL_SECTION, -1, _T("SECRET-KEY"), -1, sz_key, -1);
	set_entity_icon(ent, GDI_ATTR_GIZMO_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	set_section_icon(section_from_entity(ent), GDI_ATTR_GIZMO_PROPER);

	properctrl_attach(pdt->hProper, ptrProper);
	properctrl_set_lock(pdt->hProper, 0);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!JsonPanel_OpenFile(widget, szParam))
			return -1;
	}
	else
	{
		JsonPanel_NewFile(widget);
	}

	memoctrl_redraw(pdt->hRequest);
	
	return 0;
}

void JsonPanel_OnDestroy(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

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

int JsonPanel_OnClose(res_win_t widget)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	JsonPanel_Switch(widget);

	int rt = (memoctrl_get_dirty(pdt->hRequest)) ? 1 : 0;

	if (!rt)
	{
		tchar_t sz_url[PATH_LEN] = { 0 };
		tchar_t sz_id[RES_LEN] = { 0 };
		tchar_t sz_key[RES_LEN] = { 0 };

		LINKPTR ptrProper = properctrl_fetch(pdt->hProper);

		read_proper(ptrProper, JSONPANEL_SECTION, -1, _T("URL"), -1, sz_url, PATH_LEN);
		read_proper(ptrProper, JSONPANEL_SECTION, -1, _T("SECRET-ID"), -1, sz_id, RES_LEN);
		read_proper(ptrProper, JSONPANEL_SECTION, -1, _T("SECRET-KEY"), -1, sz_key, RES_LEN);

		if (!is_null(sz_url))
		{
			SavePreference(_T("JSON"), _T("URL"), sz_url);
		}

		if (!is_null(sz_id))
		{
			SavePreference(_T("JSON"), _T("SECRET-ID"), sz_id);
		}

		if (!is_null(sz_key))
		{
			SavePreference(_T("JSON"), _T("SECRET-KEY"), sz_key);
		}
	}

	return rt;
}

void JsonPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hRequest))
	{
		widget_set_focus(pdt->hRequest);
	}
}

void JsonPanel_OnShow(res_win_t widget, bool_t bShow)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, JSONPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, JSONPANEL_GROUPITEM_HEIGHT);
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
		xsprintf(token, _T("%d"), IDA_DATABASE_EXECUTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("执行"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_EXECUTE);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void JsonPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	LINKPTR ptr_memo = memoctrl_fetch(pdt->hRequest);
	
}

void JsonPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);
}

void JsonPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		JsonPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		JsonPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		JsonPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		JsonPanel_OnPreview(widget);
		break;
	case IDA_FILE_EXPORT:
		JsonPanel_OnExport(widget);
		break;

	case IDA_EDIT_SELECTALL:
		JsonPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_COPY:
		JsonPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		JsonPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		JsonPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		JsonPanel_OnUndo(widget);
		break;

	case IDA_DATABASE_EXECUTE:
		JsonPanel_OnExecute(widget);
		break;
	case IDA_DATABASE_SCHEMA:
		JsonPanel_OnSchema(widget);
		break;
	}

}

void JsonPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	JsonPanelDelta* pdt = GETJSONPANELDELTA(widget);

	if (phdr->id == IDC_JSONPANEL_PROPER)
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
res_win_t JsonPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(JsonPanel_OnCreate)
	EVENT_ON_DESTROY(JsonPanel_OnDestroy)
	EVENT_ON_CLOSE(JsonPanel_OnClose)
	EVENT_ON_SHOW(JsonPanel_OnShow)

	EVENT_ON_NOTICE(JsonPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(JsonPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(JsonPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(JsonPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}