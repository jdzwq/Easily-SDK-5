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
#include "XMLPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"
#include "_Project.h"
#include "_Database.h"

#define IDC_XMLPANEL_REQUEST		201
#define IDC_XMLPANEL_RESPONSE		202
#define IDC_XMLPANEL_PROPER		203

#define XMLPANEL_GROUPITEM_WIDTH		(float)8
#define XMLPANEL_GROUPITEM_HEIGHT		(float)7

#define XMLPANEL_SECTION		_T("服务设置")

typedef struct tagXMLPanelDelta{
	res_win_t hRequest;
	res_win_t hResponse;
	res_win_t hProper;

	tchar_t szFile[PATH_LEN];
}XMLPanelDelta;

#define GETXMLPANELDELTA(ph) 		(XMLPanelDelta*)widget_get_user_delta(ph)
#define SETXMLPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_int)ptd)

#define XMLPANEL_ACCEL_COUNT	5
accel_t	XMLPANEL_ACCEL[XMLPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	XMLPanel_Switch(res_win_t widget);

bool_t	XMLPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	XMLPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void XMLPanel_Switch(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

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

static bool_t XMLPanel_Call(const tchar_t* sz_url, const tchar_t* sz_id, const tchar_t* sz_key, LINKPTR xml_in, LINKPTR xml_out)
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

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, sz_key, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, sz_id, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	if (!xhttp_send_xml(xhttp, xml_in))
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

	if (!xhttp_recv_xml(xhttp, xml_out))
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
void XMLPanel_OnSelectAll(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	memoctrl_select_all(pdt->hRequest);
}

void XMLPanel_OnCopy(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	widget_copy(pdt->hRequest);
}

void XMLPanel_OnCut(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	widget_cut(pdt->hRequest);
}

void XMLPanel_OnPaste(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	widget_paste(pdt->hRequest);
}

void XMLPanel_OnRedo(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

}

void XMLPanel_OnUndo(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	widget_undo(pdt->hRequest);
}

void XMLPanel_NewFile(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	LINKPTR ptrXML = create_xml_doc();

	LINKPTR nlk_rowset = get_xml_dom_node(ptrXML);
	set_dom_node_name(nlk_rowset, _T("rowset"), -1);

	LINKPTR nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
	set_dom_node_name(nlk_row, _T("row"), -1);

	LINKPTR nlk = insert_dom_node(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("node_name"), -1);
	set_dom_node_text(nlk, _T("node_value"), -1);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hRequest);

	format_xml_doc_to_memo(ptrXML, ptrMemo);

	destroy_xml_doc(ptrXML);

	memoctrl_redraw(pdt->hRequest);
}

bool_t XMLPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

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

bool_t XMLPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

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

void XMLPanel_OnSave(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("xml file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	XMLPanel_SaveFile(widget, szFile);
}

void XMLPanel_OnSaveAs(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	tchar_t szPath[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
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

void XMLPanel_OnExport(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

}

void XMLPanel_OnSchema(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

}

void XMLPanel_OnPrint(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);
}

void XMLPanel_OnPreview(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

}

void XMLPanel_OnExecute(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_id[RES_LEN] = { 0 };
	tchar_t sz_key[RES_LEN] = { 0 };

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);

	read_proper(ptrProper, XMLPANEL_SECTION, -1, _T("URL"), -1, sz_url, PATH_LEN);
	read_proper(ptrProper, XMLPANEL_SECTION, -1, _T("SECRET-ID"), -1, sz_id, RES_LEN);
	read_proper(ptrProper, XMLPANEL_SECTION, -1, _T("SECRET-KEY"), -1, sz_key, RES_LEN);

	if (is_null(sz_url))
	{
		ShowMsg(0, _T("XML服务地址未设置！"));
		return;
	}

	LINKPTR ptrRequest = memoctrl_fetch(pdt->hRequest);

	LINKPTR xml_in = create_xml_doc();
	
	if (!parse_xml_doc_from_memo(xml_in, ptrRequest))
	{
		destroy_xml_doc(xml_in);

		ShowMsg(0, _T("XML格式有错误！"));
		return;
	}

	LINKPTR ptrResponse = memoctrl_fetch(pdt->hResponse);
	clear_memo_doc(ptrResponse);
	memoctrl_redraw(pdt->hResponse);

	LINKPTR xml_out = create_xml_doc();

	if (XMLPanel_Call(sz_url, sz_id, sz_key, xml_in, xml_out))
	{
		format_xml_doc_to_memo(xml_out, ptrResponse);
	}
	else
	{
		tchar_t err_code[NUM_LEN] = { 0 };
		tchar_t err_text[ERR_LEN] = { 0 };

		get_last_error(err_code, err_text, ERR_LEN);

		parse_memo_doc(ptrResponse, err_text, -1);
	}

	destroy_xml_doc(xml_in);
	destroy_xml_doc(xml_out);

	memoctrl_redraw(pdt->hResponse);
}

/***********************************************************************************************/

int XMLPanel_OnCreate(res_win_t widget, void* data)
{
	XMLPanelDelta* pdt = (XMLPanelDelta*)xmem_alloc(sizeof(XMLPanelDelta));
	xrect_t xr;

	widgetex_hand_create(widget);

	SETXMLPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(XMLPANEL_ACCEL, XMLPANEL_ACCEL_COUNT);
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
	pdt->hRequest = memoctrl_create(_T("XMLRequest"), WD_STYLE_CONTROL, &xr, widget);

	widget_set_user_id(pdt->hRequest, IDC_XMLPANEL_REQUEST);
	widget_set_owner(pdt->hRequest, widget);

	LINKPTR ptrRequest = create_memo_doc();
	memoctrl_attach(pdt->hRequest, ptrRequest);
	memoctrl_set_lock(pdt->hRequest, 0);

	set_split_item_delta(ilkRequest, pdt->hRequest);
	widget_show(pdt->hRequest, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hResponse = memoctrl_create(_T("XMLResponse"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hResponse, IDC_XMLPANEL_RESPONSE);
	widget_set_owner(pdt->hResponse, widget);

	LINKPTR ptrResponse = create_memo_doc();
	memoctrl_attach(pdt->hResponse, ptrResponse);

	set_split_item_delta(ilkResponse, pdt->hResponse);
	widget_show(pdt->hResponse, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("XMLProper"), WD_STYLE_CONTROL, &xr, widget);

	widget_set_user_id(pdt->hProper, IDC_XMLPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	LINKPTR ent;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_id[RES_LEN] = { 0 };
	tchar_t sz_key[RES_LEN] = { 0 };

	LoadPreference(_T("XML"), _T("URL"), sz_url);
	LoadPreference(_T("XML"), _T("SECRET-ID"), sz_id);
	LoadPreference(_T("XML"), _T("SECRET-KEY"), sz_id);

	ent = write_proper(ptrProper, XMLPANEL_SECTION, -1, _T("URL"), -1, sz_url, -1);
	set_entity_icon(ent, ICON_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	ent = write_proper(ptrProper, XMLPANEL_SECTION, -1, _T("SECRET-ID"), -1, sz_id, -1);
	set_entity_icon(ent, ICON_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	ent = write_proper(ptrProper, XMLPANEL_SECTION, -1, _T("SECRET-KEY"), -1, sz_key, -1);
	set_entity_icon(ent, ICON_BOOK);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	set_entity_editable(ent, 1);

	set_section_icon(section_from_entity(ent), ICON_PROPER);

	properctrl_attach(pdt->hProper, ptrProper);
	properctrl_set_lock(pdt->hProper, 0);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	widgetex_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!XMLPanel_OpenFile(widget, szParam))
			return -1;
	}
	else
	{
		XMLPanel_NewFile(widget);
	}

	memoctrl_redraw(pdt->hRequest);

	return 0;
}

void XMLPanel_OnDestroy(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widgetex_detach_splitor(widget);
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

	widgetex_hand_destroy(widget);
}

int XMLPanel_OnClose(res_win_t widget)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	XMLPanel_Switch(widget);

	int rt = (memoctrl_get_dirty(pdt->hRequest)) ? 1 : 0;

	if (!rt)
	{
		tchar_t sz_url[PATH_LEN] = { 0 };
		tchar_t sz_id[RES_LEN] = { 0 };
		tchar_t sz_key[RES_LEN] = { 0 };

		LINKPTR ptrProper = properctrl_fetch(pdt->hProper);

		read_proper(ptrProper, XMLPANEL_SECTION, -1, _T("URL"), -1, sz_url, PATH_LEN);
		read_proper(ptrProper, XMLPANEL_SECTION, -1, _T("SECRET-ID"), -1, sz_id, RES_LEN);
		read_proper(ptrProper, XMLPANEL_SECTION, -1, _T("SECRET-KEY"), -1, sz_key, RES_LEN);

		if (!is_null(sz_url))
		{
			SavePreference(_T("XML"), _T("URL"), sz_url);
		}

		if (!is_null(sz_id))
		{
			SavePreference(_T("XML"), _T("SECRET-ID"), sz_id);
		}

		if (!is_null(sz_key))
		{
			SavePreference(_T("XML"), _T("SECRET-KEY"), sz_id);
		}
	}

	return rt;
}

void XMLPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hRequest))
	{
		widget_set_focus(pdt->hRequest);
	}
}

void XMLPanel_OnShow(res_win_t widget, bool_t bShow)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, XMLPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, XMLPANEL_GROUPITEM_HEIGHT);
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
		set_tool_item_title(ilk, _T("执行"));
		set_tool_item_icon(ilk, ICON_EXECUTE);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void XMLPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	LINKPTR ptr_memo = memoctrl_fetch(pdt->hRequest);
	
}

void XMLPanel_OnParentCommand(res_win_t widget, int code, var_int data)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);
}

void XMLPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_int data)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		XMLPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		XMLPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		XMLPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		XMLPanel_OnPreview(widget);
		break;
	case IDA_FILE_EXPORT:
		XMLPanel_OnExport(widget);
		break;
	case IDA_DATABASE_SCHEMA:
		XMLPanel_OnSchema(widget);
		break;

	case IDA_EDIT_SELECTALL:
		XMLPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_COPY:
		XMLPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		XMLPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		XMLPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		XMLPanel_OnUndo(widget);
		break;

	case IDA_DATABASE_EXECUTE:
		XMLPanel_OnExecute(widget);
		break;
	}

}

void XMLPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	XMLPanelDelta* pdt = GETXMLPANELDELTA(widget);

	if (phdr->id == IDC_XMLPANEL_PROPER)
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
res_win_t XMLPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(XMLPanel_OnCreate)
	EVENT_ON_DESTROY(XMLPanel_OnDestroy)
	EVENT_ON_CLOSE(XMLPanel_OnClose)
	EVENT_ON_SHOW(XMLPanel_OnShow)

	EVENT_ON_NOTICE(XMLPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(XMLPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(XMLPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(XMLPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}