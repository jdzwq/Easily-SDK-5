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
#include "PlotPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_PLOTPANEL_PLOT		201
#define IDC_PLOTPANEL_PROPER		202
#define IDC_PLOTPANEL_TITLE		203
#define IDC_PLOTPANEL_MENU			204

#define PLOTPANEL_GROUPITEM_WIDTH		(float)8
#define PLOTPANEL_GROUPITEM_HEIGHT		(float)7
#define PLOTPANEL_TITLEITEM_WIDTH		(float)15
#define PLOTPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagPlotPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hPlot;

	bool_t bDirty;

	tchar_t szFile[PATH_LEN + 1];
	METADATA meta;
}PlotPanelDelta;

#define GETPLOTPANELDELTA(ph) 		(PlotPanelDelta*)widget_get_user_delta(ph)
#define SETPLOTPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define PLOTPANEL_ACCEL_COUNT	5
accel_t	PLOTPANEL_ACCEL[PLOTPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	PlotPanel_Switch(res_win_t widget);
void	PlotPanel_SetDirty(res_win_t widget, bool_t bDirty);
bool_t	PlotPanel_GetDirty(res_win_t widget);

bool_t	PlotPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	PlotPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void PlotPanel_Switch(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	if (!pdt->bDirty)
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		pdt->bDirty = 0;
		break;
	}
}

void	PlotPanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	pdt->bDirty = bDirty;
}

bool_t	PlotPanel_GetDirty(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	return pdt->bDirty;
}
/***************************************************************************************************************/
void PlotPanel_OnSelectAll(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);
	XDL_ASSERT(ptrPlot);

}

void PlotPanel_OnDelete(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);
	XDL_ASSERT(ptrPlot);

}

void PlotPanel_OnCopy(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	widget_post_command(pdt->hPlot, COMMAND_COPY, IDC_EDITMENU, 0);
}

void PlotPanel_OnCut(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	widget_post_command(pdt->hPlot, COMMAND_CUT, IDC_EDITMENU, 0);
}

void PlotPanel_OnPaste(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);

	widget_post_command(pdt->hPlot, COMMAND_PASTE, IDC_EDITMENU, 0);
}

void PlotPanel_OnAttributes(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);

	properbag_write_plot_attributes(ptrProper, ptrPlot);

	properctrl_redraw(pdt->hProper);
}

void PlotPanel_OnStyleSheet(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);

	properbag_parse_stylesheet(ptrProper, get_plot_style_ptr(ptrPlot));

	properctrl_redraw(pdt->hProper);
}

void PlotPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void PlotPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	int n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void PlotPanel_Plot_OnRBClick(res_win_t widget, NOTICE_PLOT* pnf)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	xpoint_t* ppt = (xpoint_t*)pnf->data;

}

void PlotPanel_Plot_OnLBClick(res_win_t widget, NOTICE_PLOT* pnf)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void PlotPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	LINKPTR tlk = titlectrl_get_focus_item(pdt->hTitle);
	if (!tlk)
		return;

	int n_id = xstol(get_title_item_id_ptr(tlk));

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);

	tchar_t sz_style[CSS_LEN + 1] = { 0 };

	if (n_id == IDA_ATTRIBUTES)
	{
		properbag_read_plot_attributes(pnp->proper, ptrPlot);
	}
	else if (n_id == IDA_STYLESHEET)
	{
		properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
		set_plot_style(ptrPlot, sz_style, -1);
	}

	plotctrl_redraw(pdt->hPlot);
}

bool_t PlotPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	LINKPTR ptrMeta = create_meta_doc();
	if (!load_dom_doc_from_file(ptrMeta, NULL, szFile))
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("导入文档失败！"));

		return 0;
	}

	if (compare_text(get_meta_doc_name_ptr(ptrMeta), -1, DOC_PLOT, -1, 1) != 0)
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("非矢量图文档！"));

		return 0;
	}

	get_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, RES_LEN);
	get_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, RES_LEN);

	LINKPTR newPlot = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	LINKPTR orgPlot = plotctrl_detach(pdt->hPlot);
	destroy_plot_doc(orgPlot);

	plotctrl_attach(pdt->hPlot, newPlot);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t PlotPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrPlot);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrPlot = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		MessageBox(widget, _T("保存矢量图文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void PlotPanel_OnSave(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	tchar_t szFile[PATH_LEN + 1] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN + 1] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("Plot Meta File(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	PlotPanel_SaveFile(widget, szFile);
}

void PlotPanel_OnSaveAs(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	tchar_t szFile[PATH_LEN + 1] = { 0 };
	tchar_t szPath[PATH_LEN + 1] = { 0 };
	tchar_t szType[RES_LEN + 1] = { 0 };
	bool_t rt;

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Plot Meta File(*.sheet)\0*.sheet\0Svg Image File(*.svg)\0*.svg\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	split_path(szFile, NULL, NULL, szType);

	LINKPTR ptrPlot = plotctrl_fetch(pdt->hPlot);

	if (compare_text(szType, -1, _T("svg"), -1, 1) == 0)
	{
		LINKPTR ptrSvg = create_svg_doc();

		svg_print_plot(ptrSvg, ptrPlot);

		rt = save_dom_doc_to_file(ptrSvg, NULL, szFile);

		destroy_svg_doc(ptrSvg);
	}
	else
	{
		LINKPTR ptrMeta = create_meta_doc();

		set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
		set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

		attach_meta_body_node(ptrMeta, ptrPlot);

		rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

		ptrPlot = detach_meta_body_node(ptrMeta);
		destroy_meta_doc(ptrMeta);
	}

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void PlotPanel_OnPrint(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);
}

void PlotPanel_OnPreview(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

}

/***********************************************************************************************/

int PlotPanel_OnCreate(res_win_t widget, void* data)
{
	PlotPanelDelta* pdt = (PlotPanelDelta*)xmem_alloc(sizeof(PlotPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETPLOTPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(PLOTPANEL_ACCEL, PLOTPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkPlot = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -PLOTPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hPlot = plotctrl_create(_T("PlotPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr, widget);

	widget_set_user_id(pdt->hPlot, IDC_PLOTPANEL_PLOT);
	widget_set_owner(pdt->hPlot, widget);

	set_split_item_delta(ilkPlot, pdt->hPlot);
	widget_show(pdt->hPlot, WS_SHOW_NORMAL);

	LINKPTR ptrPlot = create_plot_doc();

	set_plot_type(ptrPlot, ATTR_PLOT_TYPE_BARGRAM, -1);
	set_plot_width(ptrPlot, 100);
	set_plot_height(ptrPlot, 100);

	plotctrl_attach(pdt->hPlot, ptrPlot);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("PlotProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_PLOTPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("PlotTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_PLOTPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WS_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, PLOTPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ATTR_GIZMO_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("风格"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, PLOTPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ATTR_GIZMO_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);
	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!PlotPanel_OpenFile(widget, szParam))
			return -1;

		plotctrl_redraw(pdt->hPlot);
	}

	return 0;
}

void PlotPanel_OnDestroy(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hPlot))
	{
		LINKPTR ptrPlot = plotctrl_detach(pdt->hPlot);
		if (ptrPlot)
			destroy_plot_doc(ptrPlot);

		widget_destroy(pdt->hPlot);
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

int PlotPanel_OnClose(res_win_t widget)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	PlotPanel_Switch(widget);

	return (pdt->bDirty) ? 1 : 0;
}

void PlotPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hPlot))
	{
		widget_set_focus(pdt->hPlot);
	}
}

void PlotPanel_OnShow(res_win_t widget, bool_t bShow)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, PLOTPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, PLOTPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

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

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void PlotPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		PlotPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		PlotPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		PlotPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		PlotPanel_OnPreview(widget);
		break;

	case IDA_EDIT_SELECTALL:
		PlotPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		PlotPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		PlotPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		PlotPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		PlotPanel_OnPaste(widget);
		break;

	case IDA_ATTRIBUTES:
		PlotPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		PlotPanel_OnStyleSheet(widget);
		break;

	case IDC_PLOTPANEL_MENU:
		widget_destroy((res_win_t)data);
		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;
	}
}

void PlotPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	if (code == COMMAND_RENAME)
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
		pdt->bDirty = 0;
	}
}

void PlotPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	PlotPanelDelta* pdt = GETPLOTPANELDELTA(widget);

	if (phdr->id == IDC_PLOTPANEL_PLOT)
	{
		NOTICE_PLOT* pnf = (NOTICE_PLOT*)phdr;
		switch (pnf->code)
		{
		case NC_PLOTLBCLK:
			PlotPanel_Plot_OnLBClick(widget, pnf);
			break;
		case NC_PLOTRBCLK:
			PlotPanel_Plot_OnRBClick(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_PLOTPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			break;
		case NC_ENTITYUPDATE:
			PlotPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_PLOTPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			PlotPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			PlotPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/*********************************************************************************************************/
res_win_t PlotPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(PlotPanel_OnCreate)
	EVENT_ON_DESTROY(PlotPanel_OnDestroy)
	EVENT_ON_CLOSE(PlotPanel_OnClose)
	EVENT_ON_SHOW(PlotPanel_OnShow)

	EVENT_ON_NOTICE(PlotPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(PlotPanel_OnMenuCommand)
	EVENT_ON_PARENT_COMMAND(PlotPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}