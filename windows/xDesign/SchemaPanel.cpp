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
#include "SchemaPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_SCHEMAPANEL_SCHEMA				201
#define IDC_SCHEMAPANEL_MENU				202

#define SCHEMAPANEL_GROUPITEM_WIDTH		(float)8
#define SCHEMAPANEL_GROUPITEM_HEIGHT	(float)7

typedef struct tagSchemaPanelDelta{
	res_win_t hMemo;
	tchar_t szFile[PATH_LEN];
}SchemaPanelDelta;

#define GETSCHEMAPANELDELTA(ph) 		(SchemaPanelDelta*)widget_get_user_delta(ph)
#define SETSCHEMAPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_int)ptd)

bool_t	SchemaPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	SchemaPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

LINKPTR SchemaPanel_GetDocument(res_win_t widget);

/***************************************************************************************/

void SchemaPanel_Switch(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	if (!memoctrl_get_dirty(pdt->hMemo))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("ÎÄ¼þÉÐÎ´±£´æ£¬ÊÇ·ñ±£´æÎÄ¼þ£¿"));
	
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

bool_t SchemaPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	LINKPTR ptr_xml = create_xml_doc();

	if (!load_xml_doc_from_file(ptr_xml, NULL, szFile))
	{
		destroy_xml_doc(ptr_xml);
		ShowMsg(MSGICO_ERR, _T("´ò¿ªXMLÎÄµµÊ§°Ü£¡"));
		return 0;
	}

	xscpy(pdt->szFile, szFile);

	LINKPTR ptr_memo = memoctrl_fetch(pdt->hMemo);
	clear_memo_doc(ptr_memo);

	format_xml_doc_to_memo(ptr_xml, ptr_memo);

	destroy_xml_doc(ptr_xml);

	memoctrl_redraw(pdt->hMemo);

	return 1;
}

bool_t SchemaPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	LINKPTR ptr_memo = memoctrl_fetch(pdt->hMemo);

	LINKPTR ptr_xml = create_xml_doc();

	if (!parse_xml_doc_from_memo(ptr_xml, ptr_memo))
	{
		destroy_xml_doc(ptr_xml);
		ShowMsg(MSGICO_ERR, _T("½âÎöXMLÎÄµµÊ§°Ü£¡"));
		return 0;
	}

	if (!save_xml_doc_to_file(ptr_xml, NULL, szFile))
	{
		destroy_xml_doc(ptr_xml);
		ShowMsg(MSGICO_ERR, _T("±£´æXMLÎÄµµÊ§°Ü£¡"));
		return 0;
	}

	destroy_xml_doc(ptr_xml);

	xscpy(pdt->szFile, szFile);

	memoctrl_set_dirty(pdt->hMemo, 0);

	return 1;
}

/*********************************************************************************************************/
void SchemaPanel_OnSelectAll(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	memoctrl_select_all(pdt->hMemo);
}

void SchemaPanel_OnDelete(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	widget_post_key(pdt->hMemo, KEY_BACK);
}

void SchemaPanel_OnCopy(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	widget_copy(pdt->hMemo);
}

void SchemaPanel_OnCut(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	widget_cut(pdt->hMemo);
}

void SchemaPanel_OnPaste(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	widget_paste(pdt->hMemo);
}


void SchemaPanel_OnRedo(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

}

void SchemaPanel_OnUndo(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	widget_undo(pdt->hMemo);
}

void SchemaPanel_Schema_OnRBClick(res_win_t widget, NOTICE_MEMO* pnf)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr;
	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = xr.h = 0;

	xpoint_t pt = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_SCHEMAPANEL_MENU);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widgetex_get_color_mode(widget, &clr);
	widgetex_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();

	LINKPTR mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_COPY);
	set_menu_item_title(mlk, _T("¿½±´×Ö¶Î"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_CUT);
	set_menu_item_title(mlk, _T("¼ôÇÐ×Ö¶Î"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_PASTE);
	set_menu_item_title(mlk, _T("Õ³Ìù×Ö¶Î"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_DELETE);
	set_menu_item_title(mlk, _T("É¾³ý×Ö¶Î"));

	menubox_set_data(hMenu, ptrMenu);

	pt.x = ppt->x;
	pt.y = ppt->y;
	widget_client_to_screen(pdt->hMemo, &pt);

	menubox_layout(hMenu, &pt, WD_LAYOUT_RIGHTBOTTOM);
	widget_show(hMenu, WD_SHOW_NORMAL);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void SchemaPanel_Schema_OnLBClick(res_win_t widget, NOTICE_MEMO* pnf)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

}

void SchemaPanel_OnSave(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);
	
	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("data schema file(*.schema)\0*.schema\0"), _T("schema"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	if (!SchemaPanel_SaveFile(widget, szFile))
		return;

	memoctrl_set_dirty(pdt->hMemo, 0);
}

void SchemaPanel_OnSaveAs(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("data schema file(*.schema)\0*.schema\0"), _T("schema"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptr_memo = memoctrl_fetch(pdt->hMemo);

	LINKPTR ptr_xml = create_xml_doc();

	if (!parse_xml_doc_from_memo(ptr_xml, ptr_memo))
	{
		destroy_xml_doc(ptr_xml);
		ShowMsg(MSGICO_ERR, _T("½âÎöXMLÎÄµµÊ§°Ü£¡"));
		return;
	}

	if (!save_xml_doc_to_file(ptr_xml, NULL, szFile))
	{
		destroy_xml_doc(ptr_xml);
		ShowMsg(MSGICO_ERR, _T("±£´æXMLÎÄµµÊ§°Ü£¡"));
		return;
	}

	destroy_xml_doc(ptr_xml);
}

void SchemaPanel_OnPrint(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

}

void SchemaPanel_OnPreview(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	LINKPTR ptrMemo = memoctrl_fetch(pdt->hMemo);

	int page = memoctrl_get_cur_page(pdt->hMemo);

	res_win_t hPreviewDlg = previewdlg_create(_T("´òÓ¡Ô¤ÀÀ"), g_hMain);

	LINKPTR svg = create_svg_doc();

	xfont_t xf;
	xface_t xa;

	widgetex_get_xfont(pdt->hMemo, &xf);
	widgetex_get_xface(pdt->hMemo, &xa);

	svg_print_memo(svg, &xf, &xa, ptrMemo, page);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	xsprintf(token, _T("µÚ%dÒ³"), page);
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WD_SHOW_FULLSCREEN);
}
/***********************************************************************************************************/
int SchemaPanel_OnCreate(res_win_t widget, void* data)
{
	SchemaPanelDelta* pdt = (SchemaPanelDelta*)xmem_alloc(sizeof(SchemaPanelDelta));
	xrect_t xr;
	const tchar_t* szParam = (tchar_t*)data;

	widgetex_hand_create(widget);

	SETSCHEMAPANELDELTA(widget, pdt);

	widget_get_client_rect(widget, &xr);
	pdt->hMemo = memoctrl_create(_T("SchemaPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr, widget);
	widget_set_user_id(pdt->hMemo, IDC_SCHEMAPANEL_SCHEMA);
	widget_set_owner(pdt->hMemo, widget);

	LINKPTR ptr_xml = create_xml_doc();
	LINKPTR nlk = get_xml_dom_node(ptr_xml);
	set_dom_node_name(nlk, DOC_SCHEMA, -1);
	set_dom_node_text(nlk, _T("\n"), -1);

	LINKPTR ptr_memo = create_memo_doc();

	format_xml_doc_to_memo(ptr_xml, ptr_memo);
	destroy_xml_doc(ptr_xml);

	memoctrl_attach(pdt->hMemo, ptr_memo);
	memoctrl_set_lock(pdt->hMemo, 0);

	widget_show(pdt->hMemo, WD_SHOW_NORMAL);

	if (!is_null(szParam))
	{
		SchemaPanel_OpenFile(widget, szParam);
	}

	return 0;
}

void SchemaPanel_OnDestroy(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	if (widget_is_valid(pdt->hMemo))
	{
		LINKPTR ptrText = memoctrl_detach(pdt->hMemo);
		if (ptrText)
			destroy_form_doc(ptrText);

		widget_destroy(pdt->hMemo);
	}

	xmem_free(pdt);

	widgetex_hand_destroy(widget);
}

void SchemaPanel_OnShow(res_win_t widget, bool_t bShow)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("±à¼­"));
		set_tool_group_item_width(glk, SCHEMAPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, SCHEMAPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_SELECTALL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("È«Ñ¡"));
		set_tool_item_icon(ilk, ICON_SELECTALL);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_DELETE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("É¾³ý"));
		set_tool_item_icon(ilk, ICON_DELETE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_COPY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("¿½±´"));
		set_tool_item_icon(ilk, ICON_COPY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_CUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("¼ôÇÐ"));
		set_tool_item_icon(ilk, ICON_CUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_PASTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("Õ³Ìù"));
		set_tool_item_icon(ilk, ICON_PASTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_UNDO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("³·Ïú"));
		set_tool_item_icon(ilk, ICON_UNDO);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

int SchemaPanel_OnClose(res_win_t widget)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	SchemaPanel_Switch(widget);

	return (memoctrl_get_dirty(pdt->hMemo)) ? 1 : 0;
}

void SchemaPanel_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);
	xrect_t xr;

	if (!pdt)
		return;

	widget_get_client_rect(widget, &xr);
	widget_move(pdt->hMemo, RECTPOINT(&xr));
	widget_size(pdt->hMemo, RECTSIZE(&xr));
}

void SchemaPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_int data)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	switch (cid)
	{
	case IDA_FILE_SAVE:
		SchemaPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		SchemaPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		SchemaPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		SchemaPanel_OnPreview(widget);
		break;
	case IDA_EDIT_SELECTALL:
		SchemaPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		SchemaPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		SchemaPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		SchemaPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		SchemaPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		SchemaPanel_OnUndo(widget);
		break;

	case IDC_SCHEMAPANEL_MENU:
		widget_destroy((res_win_t)data);
		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;
	}
}

void SchemaPanel_OnParentCommand(res_win_t widget, int code, var_int data)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	if (code == COMMAND_RENAME)
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
		memoctrl_set_dirty(pdt->hMemo, 0);
	}
}

void SchemaPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	SchemaPanelDelta* pdt = GETSCHEMAPANELDELTA(widget);

	if (phdr->id == IDC_SCHEMAPANEL_SCHEMA)
	{
		NOTICE_MEMO* pnt = (NOTICE_MEMO*)phdr;
		switch (pnt->code)
		{
		case NC_MEMOCALCED:
			break;
		case NC_MEMOLBCLK:
			SchemaPanel_Schema_OnLBClick(widget, pnt);
			break;
		case NC_MEMORBCLK:
			SchemaPanel_Schema_OnRBClick(widget, pnt);
			break;
		}
	}
}

/**************************************************************************************************************/
res_win_t SchemaPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(SchemaPanel_OnCreate)
	EVENT_ON_DESTROY(SchemaPanel_OnDestroy)
	EVENT_ON_CLOSE(SchemaPanel_OnClose)
	EVENT_ON_SHOW(SchemaPanel_OnShow)

	EVENT_ON_SIZE(SchemaPanel_OnSize)

	EVENT_ON_NOTICE(SchemaPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(SchemaPanel_OnMenuCommand)
	EVENT_ON_PARENT_COMMAND(SchemaPanel_OnParentCommand)

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}