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
#include "StatisPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_STATISPANEL_STATIS		201
#define IDC_STATISPANEL_PROPER		202
#define IDC_STATISPANEL_TITLE		203
#define IDC_STATISPANEL_FONTNAME		210
#define IDC_STATISPANEL_FONTSIZE		211
#define IDC_STATISPANEL_FONTCOLOR	212
#define IDC_STATISPANEL_PAINTCOLOR	213
#define IDC_STATISPANEL_DRAWCOLOR	214

#define STATISPANEL_GROUPITEM_WIDTH		(float)7
#define STATISPANEL_GROUPITEM_HEIGHT		(float)7
#define STATISPANEL_TITLEITEM_WIDTH		(float)15
#define STATISPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagStatisPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hStatis;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}StatisPanelDelta;

#define GETSTATISPANELDELTA(ph) 		(StatisPanelDelta*)widget_get_user_delta(ph)
#define SETSTATISPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define STATISPANEL_ACCEL_COUNT	5
accel_t	STATISPANEL_ACCEL[STATISPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	StatisPanel_Switch(res_win_t widget);

bool_t	StatisPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	StatisPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/

void StatisPanel_Switch(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	if (!statisctrl_get_dirty(pdt->hStatis))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		statisctrl_set_dirty(pdt->hStatis, 0);
		break;
	}
}

bool_t StatisPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrMeta = create_meta_doc();
	if (!load_dom_doc_from_file(ptrMeta, NULL, szFile))
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	if (compare_text(get_meta_doc_name_ptr(ptrMeta), -1, DOC_STATIS, -1, 1) != 0)
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("非表单文档！"));

		return 0;
	}

	get_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, RES_LEN);
	get_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, RES_LEN);

	LINKPTR newStatis = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	LINKPTR orgStatis = statisctrl_detach(pdt->hStatis);
	destroy_statis_doc(orgStatis);

	set_statis_design(newStatis, 1);
	statisctrl_attach(pdt->hStatis, newStatis);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t StatisPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrStatis);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrStatis = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		MessageBox(widget, _T("保存图表文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	statisctrl_set_dirty(pdt->hStatis, 0);

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

/***************************************************************************************************************/

void StatisPanel_OnSave(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

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

	if (StatisPanel_SaveFile(widget, szFile))
	{
		statisctrl_set_dirty(pdt->hStatis, 0);
	}
}

void StatisPanel_OnSaveAs(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szType[RES_LEN] = { 0 };
	bool_t rt;

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml sheet file(*.sheet)\0*.sheet\0svg image file(*.svg)\0*.svg\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	split_path(szFile, NULL, NULL, szType);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	if (compare_text(szType, -1, _T("svg"), -1, 1) == 0)
	{
		LINKPTR ptrSvg = create_svg_doc();

		int page = statisctrl_get_cur_page(pdt->hStatis);

		set_statis_design(ptrStatis, 0);

		svg_print_statis(ptrSvg, ptrStatis, page);

		set_statis_design(ptrStatis, 1);

		rt = save_dom_doc_to_file(ptrSvg, NULL, szFile);

		destroy_svg_doc(ptrSvg);
	}
	else
	{
		LINKPTR ptrMeta = create_meta_doc();

		set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
		set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

		attach_meta_body_node(ptrMeta, ptrStatis);

		bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

		ptrStatis = detach_meta_body_node(ptrMeta);
		destroy_meta_doc(ptrMeta);
	}

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void StatisPanel_OnPrint(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);
}

void StatisPanel_OnPreview(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	int page = statisctrl_get_cur_page(pdt->hStatis);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_statis(svg, ptrStatis, page);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	tchar_t token[1024] = { 0 };
	xsprintf(token, _T("%s 第%d页"), get_statis_title_ptr(ptrStatis), page);
	set_arch_item_title(ilk, token);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WS_SHOW_FULLSCREEN);
}

void StatisPanel_OnSchema(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml schema file(*.schema)\0*.schema\0"), _T("schema"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	LINKPTR ptrSch = create_schema_doc();

	export_statis_schema(ptrStatis, ptrSch);

	if (!save_dom_doc_to_file(ptrSch, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("保存模式文件失败！"));
	}

	destroy_schema_doc(ptrSch);
}

void StatisPanel_OnExport(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	LINKPTR ptrDom = create_dom_doc();

	export_statis_data(ptrStatis, NULL, ptrDom);

	if (!save_dom_doc_to_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("保存数据失败！"));
		return;
	}

	destroy_dom_doc(ptrDom);
}

void StatisPanel_OnImport(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("xml data file(*.xml)\0*.xml\0"), _T("xml"), 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	LINKPTR ptrDom = create_dom_doc();

	if (!load_dom_doc_from_file(ptrDom, NULL, szFile))
	{
		destroy_dom_doc(ptrDom);

		ShowMsg(MSGICO_ERR, _T("导入数据失败！"));
		return;
	}

	import_statis_data(ptrStatis, NULL, ptrDom);

	destroy_dom_doc(ptrDom);

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnExec(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptr_statis = statisctrl_fetch(pdt->hStatis);;

	xrect_t xr = { 0 };
	xr.fw = get_statis_width(ptr_statis);
	xr.fh = get_statis_height(ptr_statis) / 2;

	screen_size_to_pt(RECTSIZE(&xr));
	widget_adjust_size(WD_STYLE_DIALOG, RECTSIZE(&xr));

	res_win_t win = statisctrl_create(NULL, WD_STYLE_DIALOG | WD_STYLE_PAGING, &xr, widget);

	statisctrl_auto_insert(win, 1);

	clr_mod_t clr = { 0 };

	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);
	parse_xcolor(&clr.clr_ico, g_face[g_indFace].ico);

	widget_set_color_mode(win, &clr);

	set_statis_design(ptr_statis, 0);
	statisctrl_attach(win, ptr_statis);

	widget_center_window(win, widget);
	widget_show(win, WS_SHOW_NORMAL);

	widget_do_modal(win);

	set_statis_design(ptr_statis, 1);
}

void StatisPanel_OnSelectAll(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	XDL_ASSERT(ptrStatis);

	LINKPTR ylk = get_next_yax(ptrStatis, LINK_FIRST);
	while (ylk)
	{
		set_yax_selected(ylk, 1);

		ylk = get_next_yax(ptrStatis, ylk);
	}

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnDelete(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	XDL_ASSERT(ptrStatis);

	LINKPTR ptrGax = statisctrl_get_focus_gax(pdt->hStatis);
	if (ptrGax)
	{
		delete_gax(ptrGax);
		statisctrl_redraw(pdt->hStatis, 1);
		return;
	}

	statisctrl_set_focus_coor(pdt->hStatis, NULL, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,ylk = get_next_yax(ptrStatis, LINK_FIRST);
	while (ylk)
	{
		nlk = get_next_yax(ptrStatis, ylk);
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
		statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnCopy(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	widget_post_command(pdt->hStatis, COMMAND_COPY, IDC_EDITMENU, 0);
}

void StatisPanel_OnCut(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	widget_post_command(pdt->hStatis, COMMAND_CUT, IDC_EDITMENU, 0);
}

void StatisPanel_OnPaste(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	widget_post_command(pdt->hStatis, COMMAND_PASTE, IDC_EDITMENU, 0);
}

void StatisPanel_OnRedo(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

}

void StatisPanel_OnUndo(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	widget_post_command(pdt->hStatis, COMMAND_UNDO, IDC_EDITMENU, 0);
}

void StatisPanel_OnSelectAttr(res_win_t widget, const tchar_t* attr_name, const tchar_t* attr_val)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	LINKPTR ptrYax = statisctrl_get_focus_yax(pdt->hStatis);

	statisctrl_set_dirty(pdt->hStatis, 1);

	tchar_t style[CSS_LEN] = { 0 };

	LINKPTR ylk = get_next_yax(ptrStatis, LINK_FIRST);
	while (ylk)
	{
		if (ylk == ptrYax || get_yax_selected(ylk))
		{
			//write_style_attr(get_yax_style_ptr(ylk), -1, attr_name, -1, attr_val, -1, style, CSS_LEN);
			//set_yax_style(ylk, style);
		}

		ylk = get_next_yax(ptrStatis, ylk);
	}

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnFontName(res_win_t widget, void* pv)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	fontname_menu(widget, IDC_STATISPANEL_FONTNAME, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void StatisPanel_OnFontSize(res_win_t widget, void* pv)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	fontsize_menu(widget, IDC_STATISPANEL_FONTSIZE, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void StatisPanel_OnTextColor(res_win_t widget, void* pv)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	color_menu(widget, IDC_STATISPANEL_FONTCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void StatisPanel_OnPaintColor(res_win_t widget, void* pv)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	color_menu(widget, IDC_STATISPANEL_PAINTCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void StatisPanel_OnDrawColor(res_win_t widget, void* pv)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	color_menu(widget, IDC_STATISPANEL_DRAWCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}


void StatisPanel_OnTextNear(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	if (!get_yax_selected_count(ptrStatis))
		return;

	StatisPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_NEAR);
}

void StatisPanel_OnTextBold(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	if (!get_yax_selected_count(ptrStatis))
		return;

	StatisPanel_OnSelectAttr(widget, GDI_ATTR_FONT_WEIGHT, GDI_ATTR_FONT_WEIGHT_BOLD);
}

void StatisPanel_OnTextCenter(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	if (!get_yax_selected_count(ptrStatis))
		return;

	StatisPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_CENTER);
}

void StatisPanel_OnTextFar(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	StatisPanel_OnSelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_FAR);
}

void StatisPanel_OnInsertGax(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	statisctrl_set_dirty(pdt->hStatis, 1);

	LINKPTR glk = insert_gax(ptrStatis, LINK_LAST);

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnInsertYax(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	LINKPTR ptrPos = statisctrl_get_focus_yax(pdt->hStatis);

	statisctrl_set_dirty(pdt->hStatis, 1);

	ptrPos = (ptrPos)? get_prev_yax(ptrStatis, ptrPos) : LINK_LAST;
	
	LINKPTR ylk = insert_yax(ptrStatis,ptrPos);

	int count = get_yax_count(ptrStatis);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("Y%d"), count);

	set_yax_name(ylk, token);
	set_yax_title(ylk, token);

	if (count < COLOR_TABLE_SIZE)
		set_yax_color(ylk, COLOR_TABLE[count]);

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnInsertXax(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	LINKPTR xlk = insert_xax(ptrStatis, LINK_LAST);

	int count = get_xax_count(ptrStatis);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("X%d"), count);

	set_xax_name(xlk, token);
	set_xax_text(xlk, token);

	/*double f_num = get_statis_yaxbar_step(ptrStatis) * 10;
	double f_xax = get_statis_yaxbar_step(ptrStatis) * get_xax_count(ptrStatis) * 10;
	count = 0;
	LINKPTR ylk = get_next_yax(ptrStatis, LINK_FIRST);
	while (ylk)
	{
		set_coor_numeric(xlk, ylk, (++count) * f_num + f_xax);
		ylk = get_next_yax(ptrStatis, ylk);
	}

	float midd = 0;
	float step = 0;

	calc_statis_baseline(ptrStatis, &midd, &step);

	set_statis_yaxbar_midd(ptrStatis, midd);
	set_statis_yaxbar_step(ptrStatis, step);*/

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnDeleteXax(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrXax = statisctrl_get_focus_xax(pdt->hStatis);
	if (ptrXax)
		statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnEraseXaxs(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	clear_statis_xaxset(ptrStatis);
	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnImportXaxs(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnUpdateXaxs(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	statisctrl_redraw(pdt->hStatis, 1);
}

void StatisPanel_OnAttributes(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	LINKPTR ptrGax = statisctrl_get_focus_gax(pdt->hStatis);
	LINKPTR ptrYax = statisctrl_get_focus_yax(pdt->hStatis);

	if (ptrGax)
		properbag_write_gax_attributes(ptrProper, ptrGax);
	else if (ptrYax)
		properbag_write_yax_attributes(ptrProper, ptrYax);
	else
		properbag_write_statis_attributes(ptrProper, ptrStatis);

	properctrl_redraw(pdt->hProper);
}

void StatisPanel_OnStyleSheet(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);

	
	properbag_parse_stylesheet(ptrProper, get_statis_style_ptr(ptrStatis));

	properctrl_redraw(pdt->hProper);
}

void StatisPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void StatisPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	int n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void StatisPanel_Statis_OnLBClick(res_win_t widget, NOTICE_STATIS* pnf)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void StatisPanel_Statis_OnYaxSize(res_win_t widget, NOTICE_STATIS* pnf)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void StatisPanel_Statis_OnYaxMove(res_win_t widget, NOTICE_STATIS* pnf)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

}

void StatisPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));

	LINKPTR ptrStatis = statisctrl_fetch(pdt->hStatis);
	LINKPTR ptrYax = statisctrl_get_focus_yax(pdt->hStatis);
	LINKPTR ptrGax = statisctrl_get_focus_gax(pdt->hStatis);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrGax)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_gax_attributes(pnp->proper, ptrGax);
		}

		statisctrl_redraw(pdt->hStatis, 1);
	}
	else if (ptrYax)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_yax_attributes(pnp->proper, ptrYax);
		}
		
		statisctrl_redraw(pdt->hStatis, 1);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_statis_attributes(pnp->proper, ptrStatis);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_statis_style(ptrStatis, sz_style);
		}
		statisctrl_redraw(pdt->hStatis, 1);
	}
}

/***********************************************************************************************/

int StatisPanel_OnCreate(res_win_t widget, void* data)
{
	StatisPanelDelta* pdt = (StatisPanelDelta*)xmem_alloc(sizeof(StatisPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETSTATISPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(STATISPANEL_ACCEL, STATISPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkStatis = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -STATISPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hStatis = statisctrl_create(_T("StatisPanel"), WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr, widget);

	widget_set_user_id(pdt->hStatis, IDC_STATISPANEL_STATIS);
	widget_set_owner(pdt->hStatis, widget);

	set_split_item_delta(ilkStatis, pdt->hStatis);
	widget_show(pdt->hStatis, WS_SHOW_NORMAL);

	LINKPTR ptrStatis = create_statis_doc();
	set_statis_design(ptrStatis, 1);
	statisctrl_attach(pdt->hStatis, ptrStatis);
	statisctrl_set_lock(pdt->hStatis, 0);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("StatisProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_STATISPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("StatisTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_STATISPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WS_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, STATISPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ATTR_GIZMO_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("风格"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, STATISPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ATTR_GIZMO_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);
	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (const tchar_t*)data;

	if (!is_null(szParam))
	{
		StatisPanel_OpenFile(widget, szParam);
	}

	return 0;
}

void StatisPanel_OnDestroy(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hStatis))
	{
		LINKPTR ptrStatis = statisctrl_detach(pdt->hStatis);
		if (ptrStatis)
			destroy_statis_doc(ptrStatis);

		widget_destroy(pdt->hStatis);
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

int StatisPanel_OnClose(res_win_t widget)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	StatisPanel_Switch(widget);

	return (statisctrl_get_dirty(pdt->hStatis)) ? 1 : 0;
}

void StatisPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hStatis))
	{
		widget_set_focus(pdt->hStatis);
	}
}

void StatisPanel_OnShow(res_win_t widget, bool_t bShow)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, STATISPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, STATISPANEL_GROUPITEM_HEIGHT);
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

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_STYLE);
		set_tool_group_title(glk, _T("排版"));
		set_tool_group_item_width(glk, STATISPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, STATISPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_NAME);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体名称"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_FONTNAME);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_SIZE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字号大小"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_FONTSIZE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_WEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体加黑"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_FONTWEIGHT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体颜色"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_FONTCOLOR);

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PAint_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("背景"));
		set_tool_item_icon(ilk, BMP_PAint);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_DRAW_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("前景"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_STYLE);*/

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PROPER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("CSS属性"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_PROPER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_ALIGNNEAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_ALIGNCENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_ALIGNFAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_ARRANGELEFT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_ARRANGECENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_ARRANGERIGHT);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_ELEMENT);
		set_tool_group_title(glk, _T("图形"));
		set_tool_group_item_width(glk, STATISPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, STATISPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_GAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增坐标"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_GRAPH);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_YAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增Y轴"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_INSERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_INSERT_XAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("新增X轴"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_PLUS);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_DELETE_XAX);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除X轴"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_MINUS);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_ERASE_XAXS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("清除数据集"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_REMOVE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_IMPORT_XAXS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("导入数据集"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_INPUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_UPDATE_XAXS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("更新数据集"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_OUTPUT);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void StatisPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	LINKPTR ptr_statis = statisctrl_fetch(pdt->hStatis);
	LINKPTR ylk = get_next_yax(ptr_statis, LINK_FIRST);
	while (ylk)
	{
		if (compare_text(pfd->sub_str, -1, get_yax_name_ptr(ylk), -1, 1) == 0)
		{
			statisctrl_set_focus_coor(pdt->hStatis, NULL, ylk);
			break;
		}
		ylk = get_next_yax(ptr_statis, ylk);
	}
}

void StatisPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_STATIS);
		pqo->ptrDoc = statisctrl_fetch(pdt->hStatis);
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
		statisctrl_set_dirty(pdt->hStatis, 0);
	}
}

void StatisPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	tchar_t token[RES_LEN];

	switch (cid)
	{
	case IDA_FILE_SAVE:
		StatisPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		StatisPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		StatisPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		StatisPanel_OnPreview(widget);
		break;
	case IDA_FILE_SCHEMA:
		StatisPanel_OnSchema(widget);
		break;
	case IDA_FILE_EXPORT:
		StatisPanel_OnExport(widget);
		break;
	case IDA_FILE_IMPORT:
		StatisPanel_OnImport(widget);
		break;
	case IDA_FILE_EXEC:
		StatisPanel_OnExec(widget);
		break;

	case IDA_EDIT_SELECTALL:
		StatisPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		StatisPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		StatisPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		StatisPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		StatisPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		StatisPanel_OnUndo(widget);
		break;
	case IDA_STYLE_FONT_NAME:
		StatisPanel_OnFontName(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_SIZE:
		StatisPanel_OnFontSize(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_WEIGHT:
		StatisPanel_OnTextBold(widget);
		break;
	case IDA_STYLE_TEXT_COLOR:
		StatisPanel_OnTextColor(widget, (void*)data);
		break;
	case IDA_STYLE_PAINT_COLOR:
		StatisPanel_OnPaintColor(widget, (void*)data);
		break;
	case IDA_STYLE_DRAW_COLOR:
		StatisPanel_OnDrawColor(widget, (void*)data);
		break;
	case IDA_STYLE_TEXT_NEAR:
		StatisPanel_OnTextNear(widget);
		break;
	case IDA_STYLE_TEXT_CENTER:
		StatisPanel_OnTextCenter(widget);
		break;
	case IDA_STYLE_TEXT_FAR:
		StatisPanel_OnTextFar(widget);
		break;
	case IDA_INSERT_GAX:
		StatisPanel_OnInsertGax(widget);
		break;
	case IDA_INSERT_YAX:
		StatisPanel_OnInsertYax(widget);
		break;
	case IDA_INSERT_XAX:
		StatisPanel_OnInsertXax(widget);
		break;
	case IDA_DELETE_XAX:
		StatisPanel_OnDeleteXax(widget);
		break;
	case IDA_ERASE_XAXS:
		StatisPanel_OnEraseXaxs(widget);
		break;
	case IDA_IMPORT_XAXS:
		StatisPanel_OnImportXaxs(widget);
		break;
	case IDA_UPDATE_XAXS:
		StatisPanel_OnUpdateXaxs(widget);
		break;

	case IDA_ATTRIBUTES:
		StatisPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		StatisPanel_OnStyleSheet(widget);
		break;

	case IDC_STATISPANEL_FONTNAME:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontname_menu_item(code, token, RES_LEN);
			StatisPanel_OnSelectAttr(widget, GDI_ATTR_FONT_FAMILY, token);
		}
		break;
	case IDC_STATISPANEL_FONTSIZE:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontsize_menu_item(code, token, RES_LEN);
			StatisPanel_OnSelectAttr(widget, GDI_ATTR_FONT_SIZE, token);
		}
		break;
	case IDC_STATISPANEL_FONTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			StatisPanel_OnSelectAttr(widget, GDI_ATTR_FONT_COLOR, token);
		}
		break;
	case IDC_STATISPANEL_PAINTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			StatisPanel_OnSelectAttr(widget, GDI_ATTR_FILL_COLOR, token);
		}
		break;
	case IDC_STATISPANEL_DRAWCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			StatisPanel_OnSelectAttr(widget, GDI_ATTR_STROKE_COLOR, token);
		}
		break;
	}
}

void StatisPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	StatisPanelDelta* pdt = GETSTATISPANELDELTA(widget);

	if (phdr->id == IDC_STATISPANEL_STATIS)
	{
		NOTICE_STATIS* pnf = (NOTICE_STATIS*)phdr;
		switch (pnf->code)
		{
		case NC_STATISCALCED:
			break;
		case NC_YAXCALCED:
			break;
		case NC_XAXCALCED:
			break;
		case NC_STATISLBCLK:
			StatisPanel_Statis_OnLBClick(widget, pnf);
			break;
		case NC_YAXDRAG:
			StatisPanel_Statis_OnYaxMove(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_STATISPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			statisctrl_set_dirty(pdt->hStatis, 1);
			break;
		case NC_ENTITYUPDATE:
			StatisPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_STATISPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			StatisPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			StatisPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/*********************************************************************************************************/
res_win_t StatisPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(StatisPanel_OnCreate)
	EVENT_ON_DESTROY(StatisPanel_OnDestroy)
	EVENT_ON_CLOSE(StatisPanel_OnClose)
	EVENT_ON_SHOW(StatisPanel_OnShow)

	EVENT_ON_NOTICE(StatisPanel_OnNotice)

	EVENT_ON_MENU_COMMAND(StatisPanel_OnMenuCommand)
	EVENT_ON_COMMAND_FIND(StatisPanel_OnCommandFind)
	EVENT_ON_PARENT_COMMAND(StatisPanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}