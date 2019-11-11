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
#include "GraphPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_GRAPHPANEL_GRAPH		201
#define IDC_GRAPHPANEL_PROPER		202
#define IDC_GRAPHPANEL_TITLE		203
#define IDC_GRAPHPANEL_FONTNAME		210
#define IDC_GRAPHPANEL_FONTSIZE		211
#define IDC_GRAPHPANEL_FONTCOLOR	212
#define IDC_GRAPHPANEL_PAINTCOLOR	213
#define IDC_GRAPHPANEL_DRAWCOLOR	214

#define GRAPHPANEL_GROUPITEM_WIDTH		(float)7
#define GRAPHPANEL_GROUPITEM_HEIGHT		(float)7
#define GRAPHPANEL_TITLEITEM_WIDTH		(float)15
#define GRAPHPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagGraphPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hGraph;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}GraphPanelDelta;

#define GETGRAPHPANELDELTA(ph) 		(GraphPanelDelta*)widget_get_user_delta(ph)
#define SETGRAPHPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define GRAPHPANEL_ACCEL_COUNT	5
accel_t	GRAPHPANEL_ACCEL[GRAPHPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	GraphPanel_Switch(res_win_t widget);

bool_t	GraphPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	GraphPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/

void GraphPanel_Switch(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	if (!graphctrl_get_dirty(pdt->hGraph))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		graphctrl_set_dirty(pdt->hGraph, 0);
		break;
	}
}

bool_t GraphPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrMeta = create_meta_doc();
	if (!load_dom_doc_from_file(ptrMeta, NULL, szFile))
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	if (compare_text(get_meta_doc_name_ptr(ptrMeta), -1, DOC_GRAPH, -1, 1) != 0)
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("非表单文档！"));

		return 0;
	}

	get_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, RES_LEN);
	get_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, RES_LEN);

	LINKPTR newGraph = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	LINKPTR orgGraph = graphctrl_detach(pdt->hGraph);
	destroy_graph_doc(orgGraph);

	set_graph_design(newGraph, 1);
	graphctrl_attach(pdt->hGraph, newGraph);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t GraphPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrGraph);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrGraph = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		MessageBox(widget, _T("保存图表文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	graphctrl_set_dirty(pdt->hGraph, 0);

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

/***************************************************************************************************************/

void GraphPanel_OnSave(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("xml sheet file(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	if (GraphPanel_SaveFile(widget, szFile))
	{
		graphctrl_set_dirty(pdt->hGraph, 0);
	}
}

void GraphPanel_OnSaveAs(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szPath[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml sheet file(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrGraph);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrGraph = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void GraphPanel_OnPrint(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);
}

void GraphPanel_OnPreview(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	int page = graphctrl_get_cur_page(pdt->hGraph);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_graph(svg, ptrGraph, page);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	xsprintf(token, _T("%s 第%d页"), get_graph_title_ptr(ptrGraph), page);
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WD_SHOW_FULLSCREEN);
}

void GraphPanel_OnSchema(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml schema file(*.schema)\0*.schema\0"), _T("schema"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	LINKPTR ptrSch = create_schema_doc();

	export_graph_schema(ptrGraph, ptrSch);

	if (!save_dom_doc_to_file(ptrSch, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("保存模式文件失败！"));
	}

	destroy_schema_doc(ptrSch);
}

void GraphPanel_OnExport(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	LINKPTR ptrDom = create_dom_doc();

	export_graph_data(ptrGraph, NULL, ptrDom);

	if (!save_dom_doc_to_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("保存数据失败！"));
		return;
	}

	destroy_dom_doc(ptrDom);
}

void GraphPanel_OnImport(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	LINKPTR ptrDom = create_dom_doc();

	if (!load_dom_doc_from_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("导入数据失败！"));
		return;
	}

	import_graph_data(ptrGraph, NULL, ptrDom);

	destroy_dom_doc(ptrDom);

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnExec(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptr_graph = graphctrl_fetch(pdt->hGraph);;

	xrect_t xr = { 0 };
	xr.fw = get_graph_width(ptr_graph);
	xr.fh = get_graph_height(ptr_graph) / 2;

	screen_size_to_pt(RECTSIZE(&xr));
	widget_adjust_size(WD_STYLE_DIALOG | WD_STYLE_VSCROLL | WD_STYLE_HSCROLL, RECTSIZE(&xr));

	res_win_t win = graphctrl_create(NULL, WD_STYLE_DIALOG | WD_STYLE_VSCROLL | WD_STYLE_HSCROLL | WD_STYLE_PAGING, &xr, widget);

	graphctrl_auto_insert(win, 1);

	clr_mod_t clr = { 0 };

	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);

	widget_set_color_mode(win, &clr);

	set_graph_design(ptr_graph, 0);
	graphctrl_attach(win, ptr_graph);

	widget_center_window(win, widget);
	widget_show(win, WD_SHOW_NORMAL);

	widget_do_modal(win);

	set_graph_design(ptr_graph, 1);
}

void GraphPanel_OnSelectAll(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	XDL_ASSERT(ptrGraph);

	LINKPTR ylk = get_next_yax(ptrGraph, LINK_FIRST);
	while (ylk)
	{
		set_yax_selected(ylk, 1);

		ylk = get_next_yax(ptrGraph, ylk);
	}

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnDelete(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	XDL_ASSERT(ptrGraph);

	LINKPTR ptrGax = graphctrl_get_focus_gax(pdt->hGraph);
	if (ptrGax)
	{
		delete_gax(ptrGax);
		graphctrl_redraw(pdt->hGraph, 1);
		return;
	}

	graphctrl_set_focus_coor(pdt->hGraph, NULL, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,ylk = get_next_yax(ptrGraph, LINK_FIRST);
	while (ylk)
	{
		nlk = get_next_yax(ptrGraph, ylk);
		if (get_yax_selected(ylk))
		{
			delete_yax(ylk);
			ylk = nlk;

			bRedraw = 1;
			continue;
		}

		ylk = nlk;
	}

	if (bRedraw)
		graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnCopy(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	widget_copy(pdt->hGraph);
}

void GraphPanel_OnCut(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	widget_cut(pdt->hGraph);
}

void GraphPanel_OnPaste(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	widget_paste(pdt->hGraph);
}

void GraphPanel_OnRedo(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

}

void GraphPanel_OnUndo(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	widget_undo(pdt->hGraph);
}

void GraphPanel_OnSelectAttr(res_win_t widget, const tchar_t* attr_name, const tchar_t* attr_val)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	LINKPTR ptrYax = graphctrl_get_focus_yax(pdt->hGraph);

	graphctrl_set_dirty(pdt->hGraph, 1);

	tchar_t style[CSS_LEN] = { 0 };

	LINKPTR ylk = get_next_yax(ptrGraph, LINK_FIRST);
	while (ylk)
	{
		if (ylk == ptrYax || get_yax_selected(ylk))
		{
			//write_style_attr(get_yax_style_ptr(ylk), -1, attr_name, -1, attr_val, -1, style, CSS_LEN);
			//set_yax_style(ylk, style);
		}

		ylk = get_next_yax(ptrGraph, ylk);
	}

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnFontName(res_win_t widget, void* pv)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	fontname_menu(widget, IDC_GRAPHPANEL_FONTNAME, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GraphPanel_OnFontSize(res_win_t widget, void* pv)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	fontsize_menu(widget, IDC_GRAPHPANEL_FONTSIZE, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GraphPanel_OnTextColor(res_win_t widget, void* pv)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	color_menu(widget, IDC_GRAPHPANEL_FONTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GraphPanel_OnPaintColor(res_win_t widget, void* pv)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	color_menu(widget, IDC_GRAPHPANEL_PAINTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GraphPanel_OnDrawColor(res_win_t widget, void* pv)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	color_menu(widget, IDC_GRAPHPANEL_DRAWCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}


void GraphPanel_OnTextNear(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	if (!get_yax_selected_count(ptrGraph))
		return;

	GraphPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_NEAR);
}

void GraphPanel_OnTextBold(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	if (!get_yax_selected_count(ptrGraph))
		return;

	GraphPanel_OnSelectAttr(widget, GDI_ATTR_FONT_WEIGHT, GDI_ATTR_FONT_WEIGHT_BOLD);
}

void GraphPanel_OnTextCenter(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	if (!get_yax_selected_count(ptrGraph))
		return;

	GraphPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_CENTER);
}

void GraphPanel_OnTextFar(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	GraphPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_FAR);
}

void GraphPanel_OnInsertGax(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	graphctrl_set_dirty(pdt->hGraph, 1);

	LINKPTR glk = insert_gax(ptrGraph, LINK_LAST);

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnInsertYax(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	LINKPTR ptrPos = graphctrl_get_focus_yax(pdt->hGraph);

	graphctrl_set_dirty(pdt->hGraph, 1);

	ptrPos = (ptrPos)? get_prev_yax(ptrGraph, ptrPos) : LINK_LAST;
	
	LINKPTR ylk = insert_yax(ptrGraph,ptrPos);

	int count = get_yax_count(ptrGraph);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("Y%d"), count);

	set_yax_name(ylk, token);
	set_yax_title(ylk, token);

	if (count < COLOR_TABLE_SIZE)
		set_yax_color(ylk, COLOR_TABLE[count]);

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnInsertXax(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	LINKPTR xlk = insert_xax(ptrGraph, LINK_LAST);

	int count = get_xax_count(ptrGraph);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("X%d"), count);

	set_xax_name(xlk, token);
	set_xax_text(xlk, token);

	/*double f_num = get_graph_yaxbar_step(ptrGraph) * 10;
	double f_xax = get_graph_yaxbar_step(ptrGraph) * get_xax_count(ptrGraph) * 10;
	count = 0;
	LINKPTR ylk = get_next_yax(ptrGraph, LINK_FIRST);
	while (ylk)
	{
		set_coor_numeric(xlk, ylk, (++count) * f_num + f_xax);
		ylk = get_next_yax(ptrGraph, ylk);
	}

	float midd = 0;
	float step = 0;

	calc_graph_baseline(ptrGraph, &midd, &step);

	set_graph_yaxbar_midd(ptrGraph, midd);
	set_graph_yaxbar_step(ptrGraph, step);*/

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnDeleteXax(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrXax = graphctrl_get_focus_xax(pdt->hGraph);
	if (ptrXax)
		graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnEraseXaxs(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	clear_graph_xaxset(ptrGraph);
	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnImportXaxs(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnUpdateXaxs(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	graphctrl_redraw(pdt->hGraph, 1);
}

void GraphPanel_OnAttributes(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	LINKPTR ptrGax = graphctrl_get_focus_gax(pdt->hGraph);
	LINKPTR ptrYax = graphctrl_get_focus_yax(pdt->hGraph);

	if (ptrGax)
		properbag_write_gax_attributes(ptrProper, ptrGax);
	else if (ptrYax)
		properbag_write_yax_attributes(ptrProper, ptrYax);
	else
		properbag_write_graph_attributes(ptrProper, ptrGraph);

	properctrl_redraw(pdt->hProper);
}

void GraphPanel_OnStyleSheet(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);

	
	properbag_parse_stylesheet(ptrProper, get_graph_style_ptr(ptrGraph));

	properctrl_redraw(pdt->hProper);
}

void GraphPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void GraphPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	long n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void GraphPanel_Graph_OnLBClick(res_win_t widget, NOTICE_GRAPH* pnf)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void GraphPanel_Graph_OnYaxSize(res_win_t widget, NOTICE_GRAPH* pnf)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void GraphPanel_Graph_OnYaxMove(res_win_t widget, NOTICE_GRAPH* pnf)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

}

void GraphPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));

	LINKPTR ptrGraph = graphctrl_fetch(pdt->hGraph);
	LINKPTR ptrYax = graphctrl_get_focus_yax(pdt->hGraph);
	LINKPTR ptrGax = graphctrl_get_focus_gax(pdt->hGraph);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrGax)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_gax_attributes(pnp->proper, ptrGax);
		}

		graphctrl_redraw(pdt->hGraph, 1);
	}
	else if (ptrYax)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_yax_attributes(pnp->proper, ptrYax);
		}
		
		graphctrl_redraw(pdt->hGraph, 1);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_graph_attributes(pnp->proper, ptrGraph);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_graph_style(ptrGraph, sz_style);
		}
		graphctrl_redraw(pdt->hGraph, 1);
	}
}

/***********************************************************************************************/

int GraphPanel_OnCreate(res_win_t widget, void* data)
{
	GraphPanelDelta* pdt = (GraphPanelDelta*)xmem_alloc(sizeof(GraphPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETGRAPHPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(GRAPHPANEL_ACCEL, GRAPHPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkGraph = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -GRAPHPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hGraph = graphctrl_create(_T("GraphPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);

	widget_set_user_id(pdt->hGraph, IDC_GRAPHPANEL_GRAPH);
	widget_set_owner(pdt->hGraph, widget);

	set_split_item_delta(ilkGraph, pdt->hGraph);
	widget_show(pdt->hGraph, WD_SHOW_NORMAL);

	LINKPTR ptrGraph = create_graph_doc();
	set_graph_design(ptrGraph, 1);
	graphctrl_attach(pdt->hGraph, ptrGraph);
	graphctrl_set_lock(pdt->hGraph, 0);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("GraphProper"), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_GRAPHPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("GraphTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_GRAPHPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WD_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();
	set_title_images(ptrTitle, g_imagelist);

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, GRAPHPANEL_TITLEITEM_WIDTH);
	set_title_item_image(tlk, BMP_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("风格"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, GRAPHPANEL_TITLEITEM_WIDTH);
	set_title_item_image(tlk, BMP_DRAW);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);
	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (const tchar_t*)data;

	if (!is_null(szParam))
	{
		GraphPanel_OpenFile(widget, szParam);
	}

	return 0;
}

void GraphPanel_OnDestroy(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hGraph))
	{
		LINKPTR ptrGraph = graphctrl_detach(pdt->hGraph);
		if (ptrGraph)
			destroy_graph_doc(ptrGraph);

		widget_destroy(pdt->hGraph);
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

int GraphPanel_OnClose(res_win_t widget)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	GraphPanel_Switch(widget);

	return (graphctrl_get_dirty(pdt->hGraph)) ? 1 : 0;
}

void GraphPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hGraph))
	{
		widget_set_focus(pdt->hGraph);
	}
}

void GraphPanel_OnShow(res_win_t widget, bool_t bShow)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, GRAPHPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, GRAPHPANEL_GROUPITEM_HEIGHT);
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

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_STYLE);
		set_tool_group_title(glk, _T("样式"));
		set_tool_group_item_width(glk, GRAPHPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, GRAPHPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_NAME);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体名称"));
		set_tool_item_image(ilk, BMP_FONTNAME);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_SIZE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字号大小"));
		set_tool_item_image(ilk, BMP_FONTSIZE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_WEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体加黑"));
		set_tool_item_image(ilk, BMP_FONTBOLD);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体颜色"));
		set_tool_item_image(ilk, BMP_FONTCOLOR);

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PAint_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("背景"));
		set_tool_item_image(ilk, BMP_PAint);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_DRAW_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("前景"));
		set_tool_item_image(ilk, BMP_DRAW);*/

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PROPER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("CSS属性"));
		set_tool_item_image(ilk, BMP_PROPER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_image(ilk, BMP_TEXTNEAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_image(ilk, BMP_TEXTCENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_image(ilk, BMP_TEXTFAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_image(ilk, BMP_ALIGNLEFT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_image(ilk, BMP_ALIGNCENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_image(ilk, BMP_ALIGNRIGHT);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_ELEMENT);
		set_tool_group_title(glk, _T("图形"));
		set_tool_group_item_width(glk, GRAPHPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, GRAPHPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_GAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增坐标"));
		set_tool_item_image(ilk, BMP_GRAPH);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_YAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增Y轴"));
		set_tool_item_image(ilk, BMP_INSERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_XAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增X轴"));
		set_tool_item_image(ilk, BMP_PLUS);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_DELETE_XAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除X轴"));
		set_tool_item_image(ilk, BMP_MINUS);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_ERASE_XAXS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("清除数据集"));
		set_tool_item_image(ilk, BMP_ERASE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_IMPORT_XAXS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("导入数据集"));
		set_tool_item_image(ilk, BMP_INPUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_UPDATE_XAXS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("更新数据集"));
		set_tool_item_image(ilk, BMP_OUTPUT);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void GraphPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	LINKPTR ptr_graph = graphctrl_fetch(pdt->hGraph);
	LINKPTR ylk = get_next_yax(ptr_graph, LINK_FIRST);
	while (ylk)
	{
		if (compare_text(pfd->sub_str, -1, get_yax_name_ptr(ylk), -1, 1) == 0)
		{
			graphctrl_set_focus_coor(pdt->hGraph, NULL, ylk);
			break;
		}
		ylk = get_next_yax(ptr_graph, ylk);
	}
}

void GraphPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_GRAPH);
		pqo->ptrDoc = graphctrl_fetch(pdt->hGraph);
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
		graphctrl_set_dirty(pdt->hGraph, 0);
	}
}

void GraphPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	tchar_t token[RES_LEN];

	switch (cid)
	{
	case IDA_FILE_SAVE:
		GraphPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		GraphPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		GraphPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		GraphPanel_OnPreview(widget);
		break;
	case IDA_FILE_SCHEMA:
		GraphPanel_OnSchema(widget);
		break;
	case IDA_FILE_EXPORT:
		GraphPanel_OnExport(widget);
		break;
	case IDA_FILE_IMPORT:
		GraphPanel_OnImport(widget);
		break;
	case IDA_FILE_EXEC:
		GraphPanel_OnExec(widget);
		break;

	case IDA_EDIT_SELECTALL:
		GraphPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		GraphPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		GraphPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		GraphPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		GraphPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		GraphPanel_OnUndo(widget);
		break;
	case IDA_STYLE_FONT_NAME:
		GraphPanel_OnFontName(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_SIZE:
		GraphPanel_OnFontSize(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_WEIGHT:
		GraphPanel_OnTextBold(widget);
		break;
	case IDA_STYLE_TEXT_COLOR:
		GraphPanel_OnTextColor(widget, (void*)data);
		break;
	case IDA_STYLE_PAINT_COLOR:
		GraphPanel_OnPaintColor(widget, (void*)data);
		break;
	case IDA_STYLE_DRAW_COLOR:
		GraphPanel_OnDrawColor(widget, (void*)data);
		break;
	case IDA_STYLE_TEXT_NEAR:
		GraphPanel_OnTextNear(widget);
		break;
	case IDA_STYLE_TEXT_CENTER:
		GraphPanel_OnTextCenter(widget);
		break;
	case IDA_STYLE_TEXT_FAR:
		GraphPanel_OnTextFar(widget);
		break;
	case IDA_INSERT_GAX:
		GraphPanel_OnInsertGax(widget);
		break;
	case IDA_INSERT_YAX:
		GraphPanel_OnInsertYax(widget);
		break;
	case IDA_INSERT_XAX:
		GraphPanel_OnInsertXax(widget);
		break;
	case IDA_DELETE_XAX:
		GraphPanel_OnDeleteXax(widget);
		break;
	case IDA_ERASE_XAXS:
		GraphPanel_OnEraseXaxs(widget);
		break;
	case IDA_IMPORT_XAXS:
		GraphPanel_OnImportXaxs(widget);
		break;
	case IDA_UPDATE_XAXS:
		GraphPanel_OnUpdateXaxs(widget);
		break;

	case IDA_ATTRIBUTES:
		GraphPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		GraphPanel_OnStyleSheet(widget);
		break;

	case IDC_GRAPHPANEL_FONTNAME:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontname_menu_item(code, token, RES_LEN);
			GraphPanel_OnSelectAttr(widget, GDI_ATTR_FONT_FAMILY, token);
		}
		break;
	case IDC_GRAPHPANEL_FONTSIZE:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontsize_menu_item(code, token, RES_LEN);
			GraphPanel_OnSelectAttr(widget, GDI_ATTR_FONT_SIZE, token);
		}
		break;
	case IDC_GRAPHPANEL_FONTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			GraphPanel_OnSelectAttr(widget, GDI_ATTR_FONT_COLOR, token);
		}
		break;
	case IDC_GRAPHPANEL_PAINTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			GraphPanel_OnSelectAttr(widget, GDI_ATTR_FILL_COLOR, token);
		}
		break;
	case IDC_GRAPHPANEL_DRAWCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			GraphPanel_OnSelectAttr(widget, GDI_ATTR_STROKE_COLOR, token);
		}
		break;
	}
}

void GraphPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	GraphPanelDelta* pdt = GETGRAPHPANELDELTA(widget);

	if (phdr->id == IDC_GRAPHPANEL_GRAPH)
	{
		NOTICE_GRAPH* pnf = (NOTICE_GRAPH*)phdr;
		switch (pnf->code)
		{
		case NC_GRAPHCALCED:
			break;
		case NC_YAXCALCED:
			break;
		case NC_XAXCALCED:
			break;
		case NC_GRAPHLBCLK:
			GraphPanel_Graph_OnLBClick(widget, pnf);
			break;
		case NC_YAXDRAG:
			GraphPanel_Graph_OnYaxMove(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_GRAPHPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			graphctrl_set_dirty(pdt->hGraph, 1);
			break;
		case NC_ENTITYUPDATE:
			GraphPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_GRAPHPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			GraphPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			GraphPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/*********************************************************************************************************/
res_win_t GraphPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(GraphPanel_OnCreate)
	EVENT_ON_DESTROY(GraphPanel_OnDestroy)
	EVENT_ON_CLOSE(GraphPanel_OnClose)
	EVENT_ON_SHOW(GraphPanel_OnShow)

	EVENT_ON_NOTICE(GraphPanel_OnNotice)

	EVENT_ON_MENU_COMMAND(GraphPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(GraphPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(GraphPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}