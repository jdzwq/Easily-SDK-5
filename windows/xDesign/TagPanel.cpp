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
#include "TagPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_TAGPANEL_TAG			201
#define IDC_TAGPANEL_PROPER		202
#define IDC_TAGPANEL_MENU			203

#define TAGPANEL_GROUPITEM_WIDTH		(float)8
#define TAGPANEL_GROUPITEM_HEIGHT		(float)7

typedef struct tagTagPanelDelta{
	res_win_t hProper;
	res_win_t hTag;

	tchar_t szFile[PATH_LEN + 1];
}TagPanelDelta;

#define GETTAGPANELDELTA(ph) 		(TagPanelDelta*)widget_get_user_delta(ph)
#define SETTAGPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define TAGPANEL_ACCEL_COUNT	5
accel_t	TAGPANEL_ACCEL[TAGPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	TagPanel_Switch(res_win_t widget);

bool_t	TagPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	TagPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void TagPanel_Switch(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	if (!tagctrl_get_dirty(pdt->hTag))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		tagctrl_set_dirty(pdt->hTag, 0);
		break;
	}
}

/***************************************************************************************************************/
void TagPanel_OnSelectAll(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	tagctrl_select_all(pdt->hTag);
}

void TagPanel_OnDelete(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptrTag = tagctrl_fetch(pdt->hTag);
	XDL_ASSERT(ptrTag);

	LINKPTR ptrNode = tagctrl_get_focus_joint(pdt->hTag);

	if (!ptrNode)
		return;

	tagctrl_clean_tag(pdt->hTag);
}

void TagPanel_OnInsert(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptrTag = tagctrl_fetch(pdt->hTag);
	LINKPTR ptrPos = tagctrl_get_focus_joint(pdt->hTag);

	tchar_t token[INT_LEN + 1] = { 0 };

	xsprintf(token, _T("tag%d"), get_tag_joint_count(ptrTag));

	tagctrl_setup_tag(pdt->hTag, token);
}

void TagPanel_OnCopy(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	widget_post_command(pdt->hTag, COMMAND_COPY, IDC_EDITMENU, 0);
}

void TagPanel_OnCut(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	widget_post_command(pdt->hTag, COMMAND_CUT, IDC_EDITMENU, 0);
}

void TagPanel_OnPaste(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	widget_post_command(pdt->hTag, COMMAND_PASTE, IDC_EDITMENU, 0);
}

void TagPanel_OnRedo(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

}

void TagPanel_OnUndo(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	widget_post_command(pdt->hTag, COMMAND_UNDO, IDC_EDITMENU, 0);
}

void TagPanel_Tag_OnNodeChange(res_win_t widget, NOTICE_TAG* pnf)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);

	if (pnf->joint)
	{
		LINKPTR ent = write_proper(ptrProper, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_tag_joint_name_ptr(pnf->joint), -1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

		if (is_tag_text_joint(pnf->joint))
			set_entity_editable(ent, 0);
		else
			set_entity_editable(ent, 1);
	}
	else
	{
		clear_proper_doc(ptrProper);
	}

	properctrl_redraw(pdt->hProper);
}

void TagPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptrTag = tagctrl_fetch(pdt->hTag);
	LINKPTR ptrNode = tagctrl_get_focus_joint(pdt->hTag);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);

	if (ptrNode && !is_tag_text_joint(ptrNode))
	{
		set_tag_joint_name(ptrNode, get_proper_ptr(ptrProper, PROPERTY_BAG_IDENTIFY, ATTR_NAME));
	}
	
	tagctrl_redraw(pdt->hTag);
}

bool_t TagPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptrTag = tagctrl_fetch(pdt->hTag);

	if (!load_tag_from_text_file(ptrTag, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("导入文档失败！"));

		return 0;
	}

	xscpy(pdt->szFile, szFile);

	tagctrl_redraw(pdt->hTag);

	return 1;
}

bool_t TagPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptrTag = tagctrl_fetch(pdt->hTag);

	bool_t rt = save_tag_to_text_file(ptrTag, NULL, szFile);

	if (!rt)
	{
		MessageBox(widget, _T("保存文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	tagctrl_set_dirty(pdt->hTag, 0);

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void TagPanel_OnSave(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	tchar_t szFile[PATH_LEN + 1] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN + 1] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("Tag Text file(*.txt)\0*.txt\0"), _T("txt"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	TagPanel_SaveFile(widget, szFile);
}

void TagPanel_OnSaveAs(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	tchar_t szFile[PATH_LEN + 1] = { 0 };
	tchar_t szPath[PATH_LEN + 1] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Tag Text file(*.txt)\0*.txt\0"), _T("txt"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrTag = tagctrl_fetch(pdt->hTag);

	bool_t rt = save_dom_doc_to_file(ptrTag, NULL, szFile);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void TagPanel_OnPrint(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);
}

void TagPanel_OnPreview(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptrTag = tagctrl_fetch(pdt->hTag);

	int page = tagctrl_get_cur_page(pdt->hTag);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	xfont_t xf;
	xface_t xa;

	widget_get_xfont(pdt->hTag, &xf);
	widget_get_xface(pdt->hTag, &xa);

	//svg_print_tag(svg, &xf, &xa, ptrTag, page);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	//xsprintf(token, _T("%s 第%d页"), get_tag_title_ptr(ptrTag), page);
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WS_SHOW_FULLSCREEN);
}

/***********************************************************************************************/

int TagPanel_OnCreate(res_win_t widget, void* data)
{
	TagPanelDelta* pdt = (TagPanelDelta*)xmem_alloc(sizeof(TagPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETTAGPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(TAGPANEL_ACCEL, TAGPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkTag = get_split_first_child_item(ptrSplit);
	LINKPTR ilkProper = get_split_last_child_item(ptrSplit);

	widget_get_client_rect(widget, &xr);
	pdt->hTag = tagctrl_create(_T("TagPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr, widget);

	widget_set_user_id(pdt->hTag, IDC_TAGPANEL_TAG);
	widget_set_owner(pdt->hTag, widget);

	xface_t xa;
	widget_get_xface(pdt->hTag, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	widget_set_xface(pdt->hTag, &xa);

	LINKPTR ptrTag = create_tag_doc();
	tagctrl_attach(pdt->hTag, ptrTag);
	tagctrl_set_lock(pdt->hTag, 0);

	set_split_item_delta(ilkTag, pdt->hTag);
	widget_show(pdt->hTag, WS_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("TagProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_TAGPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!TagPanel_OpenFile(widget, szParam))
			return -1;

		tagctrl_redraw(pdt->hTag);
	}

	return 0;
}

void TagPanel_OnDestroy(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hTag))
	{
		LINKPTR ptrTag = tagctrl_detach(pdt->hTag);
		if (ptrTag)
			destroy_tag_doc(ptrTag);

		widget_destroy(pdt->hTag);
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

int TagPanel_OnClose(res_win_t widget)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	TagPanel_Switch(widget);

	return (tagctrl_get_dirty(pdt->hTag)) ? 1 : 0;
}

void TagPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hTag))
	{
		widget_set_focus(pdt->hTag);
	}
}

void TagPanel_OnShow(res_win_t widget, bool_t bShow)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, TAGPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, TAGPANEL_GROUPITEM_HEIGHT);
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

void TagPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	LINKPTR ptr_tag = tagctrl_fetch(pdt->hTag);
	LINKPTR ilk = get_tag_next_joint(ptr_tag, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(pfd->sub_str, -1, get_tag_joint_name_ptr(ilk), -1, 1) == 0)
		{
			tagctrl_set_focus_joint(pdt->hTag, ilk);
			break;
		}
		ilk = get_tag_next_joint(ptr_tag, ilk);
	}
}

void TagPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_TAG);
		pqo->ptrDoc = tagctrl_fetch(pdt->hTag);
	}
	else if (code == COMMAND_RENAME)
	{
		tchar_t szPath[PATH_LEN + 1], szExt[INT_LEN + 1];
		const tchar_t* nname = (const tchar_t*)data;

		if (!is_null(pdt->szFile) && !is_null(nname))
		{
			split_path(pdt->szFile, szPath, NULL, szExt);
			xsprintf(pdt->szFile, _T("%s\\%s.%s"), szPath, nname, szExt);
		}
	}
	else if (code == COMMAND_REMOVE)
	{
		tagctrl_set_dirty(pdt->hTag, 0);
	}
}

void TagPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		TagPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		TagPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		TagPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		TagPanel_OnPreview(widget);
		break;

	case IDA_EDIT_SELECTALL:
		TagPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		TagPanel_OnDelete(widget);
		break;
	case IDA_EDIT_INSERT:
		TagPanel_OnInsert(widget);
		break;
	case IDA_EDIT_COPY:
		TagPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		TagPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		TagPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		TagPanel_OnUndo(widget);
		break;

	case IDC_TAGPANEL_MENU:
		widget_destroy((res_win_t)data);
		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;
	}

}

void TagPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	TagPanelDelta* pdt = GETTAGPANELDELTA(widget);

	if (phdr->id == IDC_TAGPANEL_TAG)
	{
		NOTICE_TAG* pnf = (NOTICE_TAG*)phdr;
		switch (pnf->code)
		{
		case NC_TAGCALCED:
			break;
		case NC_TAGLBCLK:
			break;
		case NC_TAGJOINTCHANGED:
			TagPanel_Tag_OnNodeChange(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_TAGPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			tagctrl_set_dirty(pdt->hTag, 1);
			break;
		case NC_ENTITYUPDATE:
			TagPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
}

/*********************************************************************************************************/
res_win_t TagPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(TagPanel_OnCreate)
	EVENT_ON_DESTROY(TagPanel_OnDestroy)
	EVENT_ON_CLOSE(TagPanel_OnClose)
	EVENT_ON_SHOW(TagPanel_OnShow)

	EVENT_ON_NOTICE(TagPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(TagPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(TagPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(TagPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}