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
#include "GridPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Database.h"
#include "_Project.h"
#include "_Frame.h"

#include "SQLViewDialog.h"
#include "SQLFetchDialog.h"


#define IDC_GRIDPANEL_GRID		201
#define IDC_GRIDPANEL_PROPER	202
#define IDC_GRIDPANEL_TITLE		203
#define IDC_GRIDPANEL_FONTNAME	210
#define IDC_GRIDPANEL_FONTSIZE	211
#define IDC_GRIDPANEL_FONTCOLOR	212
#define IDC_GRIDPANEL_PAINTCOLOR	213
#define IDC_GRIDPANEL_DRAWCOLOR	214

#define GRIDPANEL_GROUPITEM_WIDTH		(float)8
#define GRIDPANEL_GROUPITEM_HEIGHT		(float)7
#define GRIDPANEL_TITLEITEM_WIDTH		(float)15
#define GRIDPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagGridPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hGrid;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}GridPanelDelta;

#define GRIDPANEL_ACCEL_COUNT		5

accel_t	GRIDPANEL_ACCEL[GRIDPANEL_ACCEL_COUNT] = {
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

#define GETGRIDPANELDELTA(ph) 		(GridPanelDelta*)widget_get_user_delta(ph)
#define SETGRIDPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_int)ptd)


void	GridPanel_Switch(res_win_t widget);

bool_t	GridPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	GridPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/***************************************************************************************/
void GridPanel_Switch(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	if (!gridctrl_get_dirty(pdt->hGrid))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		gridctrl_set_dirty(pdt->hGrid, 0);
		break;
	}
}

bool_t GridPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrMeta = create_meta_doc();
	if (!load_dom_doc_from_file(ptrMeta, NULL, szFile))
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	if (compare_text(get_meta_doc_name_ptr(ptrMeta), -1, DOC_GRID, -1, 1) != 0)
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("非表单文档！"));

		return 0;
	}

	get_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, RES_LEN);
	get_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, RES_LEN);

	LINKPTR newGrid = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	LINKPTR orgGrid = gridctrl_detach(pdt->hGrid);
	destroy_grid_doc(orgGrid);

	set_grid_design(newGrid, 1);
	gridctrl_attach(pdt->hGrid, newGrid);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t GridPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

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
		MessageBox(widget, _T("保存网格文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void GridPanel_OnSave(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

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

	if (GridPanel_SaveFile(widget, szFile))
	{
		gridctrl_set_dirty(pdt->hGrid, 0);
	}
}

void GridPanel_OnSaveAs(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml sheet file(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
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
		ShowMsg(MSGICO_ERR, _T("保存网格文档失败！"));
	}
}


void GridPanel_OnPrint(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	print_grid(NULL, ptrGrid);
}

void GridPanel_OnPreview(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	int page = gridctrl_get_cur_page(pdt->hGrid);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_grid(svg, ptrGrid, page);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	xsprintf(token, _T("%s 第%d页"), get_grid_title_ptr(ptrGrid), page);
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WD_SHOW_FULLSCREEN);
}

void GridPanel_OnSchema(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml schema file(*.schema)\0*.schema\0"), _T("schema"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	LINKPTR ptrSch = create_schema_doc();

	export_grid_schema(ptrGrid, ptrSch);

	if (!save_dom_doc_to_file(ptrSch, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("保存模式文件失败！"));
	}

	destroy_schema_doc(ptrSch);
}

void GridPanel_OnExport(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR ptrDom = create_dom_doc();

	export_grid_data(ptrGrid, NULL, ptrDom);

	if (!save_dom_doc_to_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("保存数据失败！"));
		return;
	}

	destroy_dom_doc(ptrDom);
}


void GridPanel_OnImport(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR ptrDom = create_dom_doc();

	if (!load_dom_doc_from_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("导入数据失败！"));
		return;
	}

	import_grid_data(ptrGrid, NULL, ptrDom);

	destroy_dom_doc(ptrDom);

	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_OnExec(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptr_grid = gridctrl_fetch(pdt->hGrid);;

	xrect_t xr = { 0 };
	xr.fw = get_grid_width(ptr_grid);
	xr.fh = get_grid_height(ptr_grid) / 2;

	screen_size_to_pt(RECTSIZE(&xr));
	widget_adjust_size(WD_STYLE_DIALOG, RECTSIZE(&xr));

	res_win_t win = gridctrl_create(NULL, WD_STYLE_DIALOG | WD_STYLE_PAGING, &xr, widget);

	gridctrl_auto_insert(win, 1);

	clr_mod_t clr = { 0 };

	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);
	parse_xcolor(&clr.clr_ico, g_face[g_indFace].ico);

	widgetex_set_color_mode(win, &clr);

	set_grid_design(ptr_grid, 0);
	gridctrl_attach(win, ptr_grid);

	widget_center_window(win, widget);
	widget_show(win, WD_SHOW_NORMAL);

	widget_do_modal(win);

	set_grid_design(ptr_grid, 1);
}

void GridPanel_OnSelectAll(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	XDL_ASSERT(ptrGrid);

	LINKPTR clk = get_next_col(ptrGrid, LINK_FIRST);
	while (clk)
	{
		set_col_selected(clk, 1);
		
		clk = get_next_col(ptrGrid, clk);
	}

	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_DropDomain(res_win_t widget, DROPDOMAIN* pdrop)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	gridctrl_set_dirty(pdt->hGrid, 1);

	widget_screen_to_client(pdt->hGrid, &pdrop->xp);
	widgetex_point_to_tm(pdt->hGrid, &pdrop->xp);

	tchar_t fname[RES_LEN + 1] = { 0 };
	int len, n_count = 0;

	len = xslen(pdrop->dm.Name);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR clk = get_next_col(ptrGrid, LINK_FIRST);
	while (clk)
	{
		if (compare_text(pdrop->dm.Name, len, get_col_name_ptr(clk), len, 1) == 0)
		{
			n_count++;
		}

		clk = get_next_col(ptrGrid, clk);
	}

	xsprintf(fname, _T("%s%d"), pdrop->dm.Name, n_count);
	clk = insert_col(ptrGrid, LINK_LAST);
	set_col_name(clk, fname);
	set_col_id(clk, fname);
	set_col_title(clk, pdrop->dm.Title);
	if (!is_null(pdrop->dm.DataType))
		set_col_data_type(clk, pdrop->dm.DataType);
	if (!is_null(pdrop->dm.DataLen))
		set_col_data_len(clk, xstol(pdrop->dm.DataLen));
	if (!is_null(pdrop->dm.DataDig))
		set_col_data_dig(clk, xstol(pdrop->dm.DataDig));

	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_AlterDomain(res_win_t widget, DROPDOMAIN* pdrop)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	gridctrl_set_dirty(pdt->hGrid, 1);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR clk = gridctrl_get_focus_col(pdt->hGrid);
	if (!clk)
		return;

	tchar_t cname[RES_LEN + 1] = { 0 };

	xsprintf(cname, _T("%s0"), pdrop->dm.Name);

	set_col_name(clk, cname);
	set_col_id(clk, cname);

	clear_col_datadef(clk);

	if (!is_null(pdrop->dm.DataType))
		set_col_data_type(clk, pdrop->dm.DataType);
	if (!is_null(pdrop->dm.DataLen))
		set_col_data_len(clk, xstol(pdrop->dm.DataLen));
	if (!is_null(pdrop->dm.DataDig))
		set_col_data_dig(clk, xstol(pdrop->dm.DataDig));

	gridctrl_redraw_col(pdt->hGrid, clk, 1);
}
/*****************************************************************************/

void GridPanel_OnDelete(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	XDL_ASSERT(ptrGrid);

	gridctrl_set_focus_cell(pdt->hGrid, NULL, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,clk = get_next_col(ptrGrid, LINK_FIRST);
	while (clk)
	{
		nlk = get_next_col(ptrGrid, clk);
		if (get_col_selected(clk))
		{
			delete_col(clk);
			clk = nlk;

			bRedraw = 1;
			continue;
		}

		clk = nlk;
	}

	if (bRedraw)
		gridctrl_redraw(pdt->hGrid,1);
}

void GridPanel_OnCopy(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	widget_copy(pdt->hGrid);
}

void GridPanel_OnCut(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	widget_cut(pdt->hGrid);
}

void GridPanel_OnPaste(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	widget_paste(pdt->hGrid);
}

void GridPanel_OnRedo(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

}

void GridPanel_OnUndo(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	widget_undo(pdt->hGrid);
}

void GridPanel_OnCSSProper(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrProper = create_proper_doc();

	LINKPTR ptr = gridctrl_fetch(pdt->hGrid);
	LINKPTR clk = gridctrl_get_focus_col(pdt->hGrid);

	if (clk)
		properbag_parse_stylesheet(ptrProper, get_col_style_ptr(clk));
	else
		properbag_parse_stylesheet(ptrProper, get_grid_style_ptr(ptr));

	res_win_t hProperDlg = properdlg_create(_T("绘制样式"), ptrProper, g_hMain);

	widget_show(hProperDlg, WD_SHOW_NORMAL);

	int nRet = widget_do_modal(hProperDlg);

	if (nRet)
	{
		tchar_t sz_style[CSS_LEN] = { 0 };
		properbag_format_stylesheet(ptrProper, sz_style, CSS_LEN);

		if (clk)
		{
			set_col_style(clk, sz_style);
			gridctrl_redraw(pdt->hGrid, 1);
		}
		else
		{
			set_grid_style(ptr, sz_style);
			gridctrl_redraw(pdt->hGrid, 1);
		}

		gridctrl_set_dirty(pdt->hGrid, 1);
	}

	destroy_proper_doc(ptrProper);
}

void GridPanel_OnSelectAttr(res_win_t widget, const tchar_t* attr_name, const tchar_t* attr_val)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	gridctrl_set_dirty(pdt->hGrid, 1);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	LINKPTR ptrCol = gridctrl_get_focus_col(pdt->hGrid);

	tchar_t style[CSS_LEN];

	LINKPTR clk = get_next_col(ptrGrid, LINK_FIRST);
	while (clk)
	{
		if (clk == ptrCol || get_col_selected(clk))
		{
			write_style_attr(get_col_style_ptr(clk), -1, attr_name, -1, attr_val, -1, style, CSS_LEN);
			set_col_style(clk, style);
		}

		clk = get_next_col(ptrGrid, clk);
	}

	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_OnFontName(res_win_t widget,void* pv)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);
	
	fontname_menu(widget, IDC_GRIDPANEL_FONTNAME, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GridPanel_OnFontSize(res_win_t widget,void* pv)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);
	
	fontsize_menu(widget, IDC_GRIDPANEL_FONTSIZE, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GridPanel_OnTextColor(res_win_t widget,void* pv)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	color_menu(widget, IDC_GRIDPANEL_FONTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GridPanel_OnPaintColor(res_win_t widget, void* pv)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	color_menu(widget, IDC_GRIDPANEL_PAINTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GridPanel_OnDrawColor(res_win_t widget, void* pv)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	color_menu(widget, IDC_GRIDPANEL_DRAWCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void GridPanel_OnTextBold(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (!get_col_selected_count(ptrGrid))
		return;

	GridPanel_OnSelectAttr(widget, GDI_ATTR_FONT_WEIGHT, GDI_ATTR_FONT_WEIGHT_BOLD);
}

void GridPanel_OnTextNear(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (!get_col_selected_count(ptrGrid))
		return;

	GridPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_NEAR);
}

void GridPanel_OnTextCenter(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (!get_col_selected_count(ptrGrid))
		return;

	GridPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_CENTER);
}

void GridPanel_OnTextFar(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (!get_col_selected_count(ptrGrid))
		return;

	GridPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_FAR);
}

void GridPanel_OnAlignNear(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (!get_col_selected_count(ptrGrid))
		return;

	GridPanel_OnSelectAttr(widget, ATTR_ALIGNMENT, ATTR_ALIGNMENT_NEAR);
}

void GridPanel_OnAlignCenter(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (!get_col_selected_count(ptrGrid))
		return;

	GridPanel_OnSelectAttr(widget, ATTR_ALIGNMENT, ATTR_ALIGNMENT_CENTER);
}

void GridPanel_OnAlignFar(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (!get_col_selected_count(ptrGrid))
		return;

	GridPanel_OnSelectAttr(widget, ATTR_ALIGNMENT, ATTR_ALIGNMENT_FAR);
}

void GridPanel_OnInsertCol(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	LINKPTR ptrPos = gridctrl_get_focus_col(pdt->hGrid);

	gridctrl_set_dirty(pdt->hGrid, 1);

	ptrPos = (ptrPos) ? get_prev_col(ptrGrid, ptrPos) : LINK_LAST;

	LINKPTR clk = insert_col(ptrGrid,ptrPos);

	int count = get_col_count(ptrGrid);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("col%d"), count);

	set_col_name(clk, token);
	set_col_title(clk, token);

	gridctrl_redraw(pdt->hGrid,1);
}

void GridPanel_OnInsertRow(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR rlk = insert_row(ptrGrid, LINK_LAST);
	gridctrl_redraw_row(pdt->hGrid, rlk, 1);
}

void GridPanel_OnDeleteRow(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrRow = gridctrl_get_focus_row(pdt->hGrid);
	if (ptrRow)
		gridctrl_redraw(pdt->hGrid,1);
}

void GridPanel_OnEraseRows(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	clear_grid_rowset(ptrGrid);
	gridctrl_redraw(pdt->hGrid,1);
}

void GridPanel_OnImportCols(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml schema file(*.schema)\0*.schema\0"), _T("schema"), 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrSch = create_schema_doc();
	if (!load_dom_doc_from_file(ptrSch, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("打开模式文件失败！"));
	}

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	if (get_col_count(ptrGrid))
	{
		if (ShowMsg(MSGICO_TIP | MSGBTN_YES | MSGBTN_NO, _T("您确定覆盖当前网格吗？")) != MSGBTN_YES)
			return;
	}

	gridctrl_set_dirty(pdt->hGrid, 1);

	import_grid_schema(ptrGrid, ptrSch);

	destroy_schema_doc(ptrSch);

	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_OnSelectCols(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	SQLVIEWDLG_PARAM pd = { 0 };

	LINKPTR ptr_prj = MainFrame_GetProject(g_hMain);
	if (!ptr_prj)
		return;

	Project_GetConfig(ptr_prj, _T("RDS"), pd.sz_conn, PATH_LEN);

	pd.vs_sql = string_alloc();

	res_win_t hSQLDlg = SQLViewDlg_Create(_T("检索数据列"), &pd);

	if (!widget_do_modal(hSQLDlg))
	{
		string_free(pd.vs_sql);
		return;
	}

	DBCTX* pct = DBOpen(pd.sz_conn);
	if (!pct)
		return;

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	if (get_col_count(ptrGrid))
	{
		if (ShowMsg(MSGICO_TIP | MSGBTN_YES | MSGBTN_NO, _T("您确定覆盖当前网格吗？")) != MSGBTN_YES)
			return;
	}

	gridctrl_set_dirty(pdt->hGrid, 1);

	DBSchema(pct, ptrGrid, string_ptr(pd.vs_sql));

	DBClose(pct);

	string_free(pd.vs_sql);

	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_OnFreshRows(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR ptr_prj = MainFrame_GetProject(g_hMain);
	if (!ptr_prj)
		return;

	/*LINKPTR ptrProper = create_proper_doc();

	tchar_t sz_pkey[RES_LEN+1] = { 0 };
	tchar_t sz_pval[KEY_LEN+1] = { 0 };
	int count = 0;

	LINKPTR ent;
	while (get_grid_next_param(ptrGrid, sz_pkey, RES_LEN))
	{
		xsncpy(sz_pval, get_grid_sql_param_ptr(ptrGrid, sz_pkey, -1), KEY_LEN);

		ent = write_proper(ptrProper, _T("检索参数"), -1, sz_pkey, -1, sz_pval, -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

		count++;
	}

	if (count)
	{
		res_win_t hProperDlg = properdlg_create(_T("设置参数"), ptrProper, g_hMain);

		widget_show(hProperDlg, WD_SHOW_NORMAL);

		int nRet = (int)widget_do_modal(hProperDlg);

		if (!nRet)
		{
			destroy_proper_doc(ptrProper);
			return;
		}

		LINKPTR sec = get_section(ptrProper, _T("检索参数"), -1);
		ent = get_next_entity(sec, LINK_FIRST);
		while (ent)
		{
			set_grid_sql_param(ptrGrid, get_entity_name_ptr(ent), get_entity_value_ptr(ent));

			ent = get_next_entity(sec, ent);
		}
	}

	destroy_proper_doc(ptrProper);*/

	tchar_t sz_conn[PATH_LEN] = { 0 };

	Project_GetConfig(ptr_prj, _T("RDS"), sz_conn, PATH_LEN);


	SQLFETCHDLG_PARAM pd = { 0 };

	pd.ptrGrid = ptrGrid;
	xscpy(pd.sz_conn, sz_conn);

	res_win_t hSqlDlg = SQLFetchDlg_Create(_T("检索数据"), &pd);

	widget_show(hSqlDlg, WD_SHOW_NORMAL);

	int nRet = (int)widget_do_modal(hSqlDlg);

	if (!nRet)
	{
		return;
	}
	
	/*DBCTX* pct = DBOpen(sz_conn);

	if (!pct)
		return;

	if (!DBFetch(pct, ptrGrid))
	{
		DBClose(pct);

		return;
	}

	DBClose(pct);*/

	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_OnUpdateRows(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR ptr_prj = MainFrame_GetProject(g_hMain);
	if (!ptr_prj)
		return;

	if (ShowMsg(MSGICO_TIP | MSGBTN_YES | MSGBTN_NO, _T("您确定提交当期网格更改的数据吗？")) != MSGBTN_YES)
		return;

	tchar_t sz_conn[PATH_LEN] = { 0 };

	Project_GetConfig(ptr_prj, _T("RDS"), sz_conn, PATH_LEN);

	DBCTX* pct = DBOpen(sz_conn);

	if (!pct)
		return;

	if (!DBUpdate(pct, ptrGrid))
	{
		DBClose(pct);

		return;
	}

	DBClose(pct);

	refresh_grid_rowset(ptrGrid);
	gridctrl_redraw(pdt->hGrid, 1);
}

void GridPanel_OnFillCode(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	string_t vs = string_alloc();

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	LINKPTR clk = get_next_visible_col(ptrGrid, LINK_FIRST);
	while (clk)
	{
		string_cat(vs, get_col_name_ptr(clk), -1);
		string_cat(vs, _T("~"), 1);
		string_cat(vs, get_col_title_ptr(clk), -1);
		string_cat(vs, _T(";"), 1);

		clk = get_next_visible_col(ptrGrid, clk);
	}

	LINKPTR ptrProper = create_proper_doc();

	LINKPTR ent = write_proper(ptrProper, _T("生成参数"), -1, _T("代码列"), -1, NULL, 0);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, string_ptr(vs), -1);

	ent = write_proper(ptrProper, _T("生成参数"), -1, _T("代码长度"), -1, NULL, 0);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptrProper, _T("生成参数"), -1, _T("初始值"), -1, NULL, 0);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptrProper, _T("生成参数"), -1, _T("代码前缀"), -1, NULL, 0);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	string_free(vs);

	res_win_t hProperDlg = properdlg_create(_T("生成代码"), ptrProper, g_hMain);

	widget_show(hProperDlg, WD_SHOW_NORMAL);

	int nRet = (int)widget_do_modal(hProperDlg);

	if (!nRet)
	{
		destroy_proper_doc(ptrProper);
		return;
	}

	tchar_t sz_col[RES_LEN + 1] = { 0 };
	tchar_t sz_pre[NUM_LEN + 1] = { 0 };
	int n_len, n_val;

	read_proper(ptrProper, _T("生成参数"), -1, _T("代码列"), -1, sz_col, RES_LEN);
	read_proper(ptrProper, _T("生成参数"), -1, _T("代码前缀"), -1, sz_pre, NUM_LEN);
	n_len = get_proper_integer(ptrProper, _T("生成参数"), _T("代码长度"));
	n_val = get_proper_integer(ptrProper, _T("生成参数"), _T("初始值"));

	destroy_proper_doc(ptrProper);

	tchar_t sz_fmt[10] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	clk = get_col(ptrGrid, sz_col);
	LINKPTR rlk = get_next_visible_row(ptrGrid, LINK_FIRST);
	while (clk && rlk)
	{
		xscpy(sz_num, sz_pre);

		xsprintf(sz_fmt, _T("%c0%dd"), _T('%'), n_len);
		xsprintf(sz_num + xslen(sz_pre), sz_fmt, n_val);

		set_cell_text(rlk, clk, sz_num, -1);
		set_cell_dirty(rlk, clk, 1);
		set_row_dirty(rlk);

		n_val++;
		rlk = get_next_visible_row(ptrGrid, rlk);
	}

	gridctrl_redraw(pdt->hGrid,1);
}

void GridPanel_OnFillHelp(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);

	string_t vs = string_alloc();

	LINKPTR clk = get_next_visible_col(ptrGrid, LINK_FIRST);
	while (clk)
	{
		string_cat(vs, get_col_name_ptr(clk), -1);
		string_cat(vs, _T("~"), 1);
		string_cat(vs, get_col_title_ptr(clk), -1);
		string_cat(vs, _T(";"), 1);

		clk = get_next_visible_col(ptrGrid, clk);
	}

	LINKPTR ptrProper = create_proper_doc();

	LINKPTR ent = write_proper(ptrProper, _T("生成参数"), -1, _T("文本列"), -1, NULL, 0);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, string_ptr(vs), -1);

	ent = write_proper(ptrProper, _T("生成参数"), -1, _T("拼音列"), -1, NULL, 0);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, string_ptr(vs), -1);

	ent = write_proper(ptrProper, _T("生成参数"), -1, _T("拼音长度"), -1, _T("6"), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	string_free(vs);

	res_win_t hProperDlg = properdlg_create(_T("生成拼音"), ptrProper, g_hMain);

	widget_show(hProperDlg, WD_SHOW_NORMAL);

	int nRet = (int)widget_do_modal(hProperDlg);

	if (!nRet)
	{
		destroy_proper_doc(ptrProper);
		return;
	}

	tchar_t sz_text[RES_LEN + 1] = { 0 };
	tchar_t sz_help[RES_LEN + 1] = { 0 };
	int n_len;

	read_proper(ptrProper, _T("生成参数"), -1, _T("文本列"), -1, sz_text, RES_LEN);
	read_proper(ptrProper, _T("生成参数"), -1, _T("拼音列"), -1, sz_help, RES_LEN);
	n_len = get_proper_integer(ptrProper, _T("生成参数"), _T("拼音长度"));

	destroy_proper_doc(ptrProper);

	tchar_t* sz_py = xsalloc(n_len + 1);

	LINKPTR clk_text = get_col(ptrGrid, sz_text);
	LINKPTR clk_help = get_col(ptrGrid, sz_help);
	LINKPTR rlk = get_next_visible_row(ptrGrid, LINK_FIRST);
	while (clk_text && clk_help && rlk)
	{
		help_code(get_cell_text_ptr(rlk, clk_text), -1, sz_py, n_len);

		set_cell_text(rlk, clk_help, sz_py, -1);
		set_cell_dirty(rlk, clk_help, 1);
		set_row_dirty(rlk);

		rlk = get_next_visible_row(ptrGrid, rlk);
	}

	xsfree(sz_py);

	gridctrl_redraw(pdt->hGrid,1);
}

void GridPanel_OnAttributes(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	LINKPTR ptrCol = gridctrl_get_focus_col(pdt->hGrid);

	if (ptrCol)
		properbag_write_col_attributes(ptrProper, ptrCol);
	else
		properbag_write_grid_attributes(ptrProper, ptrGrid);

	properctrl_redraw(pdt->hProper);
}

void GridPanel_OnStyleSheet(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	LINKPTR ptrCol = gridctrl_get_focus_col(pdt->hGrid);

	if (ptrCol)
		properbag_parse_stylesheet(ptrProper, get_col_style_ptr(ptrCol));
	else
		properbag_parse_stylesheet(ptrProper, get_grid_style_ptr(ptrGrid));

	properctrl_redraw(pdt->hProper);
}

void GridPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void GridPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	int n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void GridPanel_Grid_OnLBClick(res_win_t widget, NOTICE_GRID* pnf)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void GridPanel_Grid_OnColSize(res_win_t widget, NOTICE_GRID* pnf)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void GridPanel_Grid_OnColMove(res_win_t widget, NOTICE_GRID* pnf)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

}

void GridPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));

	LINKPTR ptrGrid = gridctrl_fetch(pdt->hGrid);
	LINKPTR ptrCol = gridctrl_get_focus_col(pdt->hGrid);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrCol)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_col_attributes(pnp->proper, ptrCol);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_col_style(ptrCol, sz_style);
		}
		gridctrl_redraw(pdt->hGrid, 1);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_grid_attributes(pnp->proper, ptrGrid);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_grid_style(ptrGrid, sz_style);
		}
		gridctrl_redraw(pdt->hGrid, 1);
	}
}

/***********************************************************************************/

int GridPanel_OnCreate(res_win_t widget, void* data)
{
	GridPanelDelta* pdt = (GridPanelDelta*)xmem_alloc(sizeof(GridPanelDelta));

	widgetex_hand_create(widget);

	SETGRIDPANELDELTA(widget, pdt);

	const tchar_t* szParam = (const tchar_t*)data;

	res_acl_t hac = create_accel_table(GRIDPANEL_ACCEL, GRIDPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkGrid = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -GRIDPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	xrect_t xr;

	widget_get_client_rect(widget, &xr);
	pdt->hGrid = gridctrl_create(_T("GridPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr, widget);
	XDL_ASSERT(pdt->hGrid);
	widget_set_user_id(pdt->hGrid, IDC_GRIDPANEL_GRID);
	widget_set_owner(pdt->hGrid, widget);
	set_split_item_delta(ilkGrid, pdt->hGrid);

	LINKPTR ptrGrid = create_grid_doc();
	set_grid_design(ptrGrid, 1);

	gridctrl_attach(pdt->hGrid, ptrGrid);
	gridctrl_set_lock(pdt->hGrid, 0);
	widget_show(pdt->hGrid, WD_SHOW_NORMAL);


	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("GridProper"), WD_STYLE_CONTROL, &xr, widget);
	XDL_ASSERT(pdt->hProper);
	widget_set_user_id(pdt->hProper, IDC_GRIDPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("GridTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_GRIDPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WD_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, GRIDPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, ICON_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("风格"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, GRIDPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, ICON_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);
	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle,LINK_FIRST));

	widgetex_attach_splitor(widget, ptrSplit);

	if (!is_null(szParam))
	{
		GridPanel_OpenFile(widget, szParam);
	}

	return 0;
}

void GridPanel_OnDestroy(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widgetex_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hGrid))
	{
		LINKPTR ptrGrid = gridctrl_detach(pdt->hGrid);
		if (ptrGrid)
			destroy_grid_doc(ptrGrid);

		widget_destroy(pdt->hGrid);
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

	widgetex_hand_destroy(widget);
}

int GridPanel_OnClose(res_win_t widget)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	GridPanel_Switch(widget);

	return (gridctrl_get_dirty(pdt->hGrid)) ? 1 : 0;
}

void GridPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hGrid))
	{
		widget_set_focus(pdt->hGrid);
	}
}

void GridPanel_OnShow(res_win_t widget, bool_t bShow)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, GRIDPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, GRIDPANEL_GROUPITEM_HEIGHT);
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

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_STYLE);
		set_tool_group_title(glk, _T("样式"));
		set_tool_group_item_width(glk, GRIDPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, GRIDPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_NAME);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体名称"));
		set_tool_item_icon(ilk, ICON_FONTNAME);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_SIZE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字号大小"));
		set_tool_item_icon(ilk, ICON_FONTSIZE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_WEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体加黑"));
		set_tool_item_icon(ilk, ICON_FONTWEIGHT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体颜色"));
		set_tool_item_icon(ilk, ICON_FONTCOLOR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("文本居左"));
		set_tool_item_icon(ilk, ICON_ALIGNNEAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("文本居中"));
		set_tool_item_icon(ilk, ICON_ALIGNCENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("文本居右"));
		set_tool_item_icon(ilk, ICON_ALIGNFAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, ICON_ARRANGELEFT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, ICON_ARRANGECENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_icon(ilk, ICON_ARRANGERIGHT);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_ELEMENT);
		set_tool_group_title(glk, _T("网格"));
		set_tool_group_item_width(glk, GRIDPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, GRIDPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_COL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增列"));
		set_tool_item_icon(ilk, ICON_PLUS);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_ROW);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增行"));
		set_tool_item_icon(ilk, ICON_APPEND);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_DELETE_ROW);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除行"));
		set_tool_item_icon(ilk, ICON_REMOVE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_ERASE_ROWS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("清除行集"));
		set_tool_item_icon(ilk, ICON_CLEAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_FRESH_ROWS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("刷新行集"));
		set_tool_item_icon(ilk, ICON_FETCH);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_UPDATE_ROWS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("更新行集"));
		set_tool_item_icon(ilk, ICON_UPDATE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_FILL_CODE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("生成序列码"));
		set_tool_item_icon(ilk, ICON_HELPC);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_FILL_HELP);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("生成助记符"));
		set_tool_item_icon(ilk, ICON_HELPP);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void GridPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	LINKPTR ptr_grid = gridctrl_fetch(pdt->hGrid);
	LINKPTR clk = get_next_col(ptr_grid, LINK_FIRST);
	while (clk)
	{
		if (compare_text(pfd->sub_str, -1, get_col_name_ptr(clk), -1, 1) == 0)
		{
			gridctrl_set_focus_cell(pdt->hGrid, NULL, clk);
			break;
		}
		clk = get_next_col(ptr_grid, clk);
	}
}

void GridPanel_OnParentCommand(res_win_t widget, int code, var_int data)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	if (code == COMMAND_QUERYDROP)
	{
		if (widget_key_state(widget, KEY_CONTROL))
			GridPanel_AlterDomain(widget, (DROPDOMAIN*)data);
		else
			GridPanel_DropDomain(widget, (DROPDOMAIN*)data);

	}
	else if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_GRID);
		pqo->ptrDoc = gridctrl_fetch(pdt->hGrid);
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
		gridctrl_set_dirty(pdt->hGrid, 0);
	}
}

void GridPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_int data)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	tchar_t token[RES_LEN];

	switch (cid)
	{
	case IDA_FILE_SAVE:
		GridPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		GridPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		GridPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		GridPanel_OnPreview(widget);
		break;
	case IDA_FILE_SCHEMA:
		GridPanel_OnSchema(widget);
		break;
	case IDA_FILE_EXPORT:
		GridPanel_OnExport(widget);
		break;
	case IDA_FILE_IMPORT:
		GridPanel_OnImport(widget);
		break;
	case IDA_FILE_EXEC:
		GridPanel_OnExec(widget);
		break;

	case IDA_EDIT_SELECTALL:
		GridPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		GridPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		GridPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		GridPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		GridPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		GridPanel_OnUndo(widget);
		break;
	case IDA_STYLE_FONT_NAME:
		GridPanel_OnFontName(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_SIZE:
		GridPanel_OnFontSize(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_WEIGHT:
		GridPanel_OnTextBold(widget);
		break;
	case IDA_STYLE_TEXT_COLOR:
		GridPanel_OnTextColor(widget, (void*)data);
		break;
	case IDA_STYLE_PAINT_COLOR:
		GridPanel_OnPaintColor(widget, (void*)data);
		break;
	case IDA_STYLE_DRAW_COLOR:
		GridPanel_OnDrawColor(widget, (void*)data);
		break;
	case IDA_STYLE_TEXT_NEAR:
		GridPanel_OnTextNear(widget);
		break;
	case IDA_STYLE_TEXT_CENTER:
		GridPanel_OnTextCenter(widget);
		break;
	case IDA_STYLE_TEXT_FAR:
		GridPanel_OnTextFar(widget);
		break;
	case IDA_STYLE_ALIGN_NEAR:
		GridPanel_OnAlignNear(widget);
		break;
	case IDA_STYLE_ALIGN_CENTER:
		GridPanel_OnAlignCenter(widget);
		break;
	case IDA_STYLE_ALIGN_FAR:
		GridPanel_OnAlignFar(widget);
		break;

	case IDA_INSERT_COL:
		GridPanel_OnInsertCol(widget);
		break;
	case IDA_INSERT_ROW:
		GridPanel_OnInsertRow(widget);
		break;
	case IDA_DELETE_ROW:
		GridPanel_OnDeleteRow(widget);
		break;
	case IDA_ERASE_ROWS:
		GridPanel_OnEraseRows(widget);
		break;
	case IDA_SELECT_COLS:
		//GridPanel_OnSelectCols(widget);
		break;
	case IDA_IMPORT_COLS:
		//GridPanel_OnImportCols(widget);
		break;
	case IDA_FRESH_ROWS:
		GridPanel_OnFreshRows(widget);
		break;
	case IDA_UPDATE_ROWS:
		GridPanel_OnUpdateRows(widget);
		break;
	case IDA_FILL_CODE:
		GridPanel_OnFillCode(widget);
		break;
	case IDA_FILL_HELP:
		GridPanel_OnFillHelp(widget);
		break;

	case IDA_ATTRIBUTES:
		GridPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		GridPanel_OnStyleSheet(widget);
		break;

	case IDC_GRIDPANEL_FONTNAME:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontname_menu_item(code, token, RES_LEN);
			GridPanel_OnSelectAttr(widget, GDI_ATTR_FONT_FAMILY, token);
		}
		break;
	case IDC_GRIDPANEL_FONTSIZE:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontsize_menu_item(code, token, RES_LEN);
			GridPanel_OnSelectAttr(widget, GDI_ATTR_FONT_SIZE, token);
		}
		break;
	case IDC_GRIDPANEL_FONTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			GridPanel_OnSelectAttr(widget, GDI_ATTR_FONT_COLOR, token);
		}
		break;
	case IDC_GRIDPANEL_PAINTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			GridPanel_OnSelectAttr(widget, GDI_ATTR_FILL_COLOR, token);
		}
		break;
	case IDC_GRIDPANEL_DRAWCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			GridPanel_OnSelectAttr(widget, GDI_ATTR_STROKE_COLOR, token);
		}
		break;
	}

}

void GridPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	GridPanelDelta* pdt = GETGRIDPANELDELTA(widget);

	if (phdr->id == IDC_GRIDPANEL_GRID)
	{
		NOTICE_GRID* png = (NOTICE_GRID*)phdr;
		switch (png->code)
		{
		case NC_GRIDCALCED:
			break;
		case NC_COLCALCED:
			break;
		case NC_ROWCALCED:
			break;
		case NC_GRIDLBCLK:
			GridPanel_Grid_OnLBClick(widget, png);
			break;
		case NC_COLSIZING:
			break;
		case NC_COLSIZED:
			GridPanel_Grid_OnColSize(widget, png);
			break;
		case NC_COLDRAG:
			break;
		case NC_COLDROP:
			GridPanel_Grid_OnColMove(widget, png);
			break;
		}
	}
	else if (phdr->id == IDC_GRIDPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			gridctrl_set_dirty(pdt->hGrid, 1);
			break;
		case NC_ENTITYUPDATE:
			GridPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_GRIDPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			GridPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			GridPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}


/*********************************************************************************************************/
res_win_t GridPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(GridPanel_OnCreate)
	EVENT_ON_DESTROY(GridPanel_OnDestroy)
	EVENT_ON_CLOSE(GridPanel_OnClose)
	EVENT_ON_SHOW(GridPanel_OnShow)

	EVENT_ON_NOTICE(GridPanel_OnNotice)

	EVENT_ON_COMMAND_FIND(GridPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(GridPanel_OnParentCommand)
	EVENT_ON_MENU_COMMAND(GridPanel_OnMenuCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}