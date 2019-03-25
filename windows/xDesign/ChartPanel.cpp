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
#include "ChartPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_CHARTPANEL_CHART			201
#define IDC_CHARTPANEL_PROPER			202
#define IDC_CHARTPANEL_TITLE			203
#define IDC_CHARTPANEL_MENU			204
#define IDC_CHARTPANEL_FONTNAME		210
#define IDC_CHARTPANEL_FONTSIZE		211
#define IDC_CHARTPANEL_FONTCOLOR		212
#define IDC_CHARTPANEL_PAINTCOLOR		213
#define IDC_CHARTPANEL_DRAWCOLOR		214

#define CHARTPANEL_GROUPITEM_WIDTH		(float)7
#define CHARTPANEL_GROUPITEM_HEIGHT		(float)7
#define CHARTPANEL_GROUPTEXT_WIDTH		(float)15
#define CHARTPANEL_TITLEITEM_WIDTH		(float)15
#define CHARTPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagChartPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hChart;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}ChartPanelDelta;

#define GETCHARTPANELDELTA(ph) 		(ChartPanelDelta*)widget_get_user_delta(ph)
#define SETCHARTPANELDELTA(ph,ptd)		widget_set_user_delta(ph,(var_long)ptd)

#define CHARTPANEL_ACCEL_COUNT	5
accel_t	CHARTPANEL_ACCEL[CHARTPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY,			VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

/***************************************************************************************/
void ChartPanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	chartctrl_set_dirty(pdt->hChart, bDirty);
}

bool_t ChartPanel_GetDirty(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	return chartctrl_get_dirty(pdt->hChart);
}

void ChartPanel_Switch(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	if (!ChartPanel_GetDirty(widget))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));
	
	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		ChartPanel_SetDirty(widget, 0);
		break;
	}
}

LINKPTR ChartPanel_GetDocument(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	return chartctrl_fetch(pdt->hChart);
}

bool_t ChartPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR newChart = create_chart_doc_from_file(NULL, szFile);
	if (!newChart)
	{
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	LINKPTR orgChart = chartctrl_detach(pdt->hChart);
	destroy_chart_doc(orgChart);

	chartctrl_attach(pdt->hChart, newChart);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t ChartPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrChart);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrChart = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		ShowMsg(MSGICO_ERR, _T("保存XML文档错误"));
		return 0;
	}

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void ChartPanel_SelectAttr(res_win_t widget, const tchar_t* attr_name, const tchar_t* attr_val)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	tchar_t style[CSS_LEN];

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (flk == ptrItem || get_chart_table_selected(flk))
		{
			write_style_attr(get_chart_table_style_ptr(flk), -1, attr_name, -1, attr_val, -1, style, CSS_LEN);
			set_chart_table_style(flk, style);
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	chartctrl_redraw(pdt->hChart);
}


/*********************************************************************************************************/
void ChartPanel_OnSave(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("chart meta file(*.chart)\0*.chart\0"), _T("chart"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	if (ChartPanel_SaveFile(widget, szFile))
	{
		ChartPanel_SetDirty(widget, 0);
	}
}

void ChartPanel_OnSaveAs(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Chart file(*.chart)\0*.chart\0"), _T("chart"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrChart);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrChart = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void ChartPanel_OnPrint(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	//print_chart(NULL, ptrChart);
}

void ChartPanel_OnPreview(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_chart(svg, ptrChart);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WD_SHOW_FULLSCREEN);
}

void ChartPanel_OnExec(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptr_dlg = chartctrl_fetch(pdt->hChart);;

	res_win_t dlg = create_dialog(ptr_dlg, widget);

	clr_mod_t clr = { 0 };

	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);

	widget_set_color_mode(dlg, &clr);

	widget_show(dlg, WD_SHOW_NORMAL);

	widget_do_modal(dlg);
}

void ChartPanel_OnSelectAll(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	XDL_ASSERT(ptrChart);

	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		set_chart_table_selected(flk, 1);
	
		flk = get_chart_next_table(ptrChart, flk);
	}

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnDelete(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	XDL_ASSERT(ptrChart);

	ChartPanel_SetDirty(widget, 1);

	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	chartctrl_set_focus_table(pdt->hChart, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		nlk = get_chart_next_table(ptrChart, flk);
		if (get_chart_table_selected(flk) || ptrItem == flk)
		{
			delete_chart_table(flk);
			flk = nlk;

			bRedraw = 1;
			continue;
		}

		flk = nlk;
	}

	if (bRedraw)
		chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnCopy(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	widget_copy(pdt->hChart);
}

void ChartPanel_OnCut(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	widget_cut(pdt->hChart);
}

void ChartPanel_OnPaste(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	widget_paste(pdt->hChart);
}


void ChartPanel_OnRedo(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

}

void ChartPanel_OnUndo(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	widget_undo(pdt->hChart);
}

void ChartPanel_OnCSSProper(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrProper = create_proper_doc();

	LINKPTR ptr = chartctrl_fetch(pdt->hChart);
	LINKPTR flk = chartctrl_get_focus_table(pdt->hChart);

	if (flk)
		propertybag_parse_stylesheet(ptrProper, get_chart_table_style_ptr(flk));
	else
		propertybag_parse_stylesheet(ptrProper, get_chart_style_ptr(ptr));

	res_win_t hProperDlg = properdlg_create(_T("绘制样式"), ptrProper, g_hMain);

	widget_show(hProperDlg, WD_SHOW_NORMAL);

	int nRet = widget_do_modal(hProperDlg);

	if (nRet)
	{
		tchar_t sz_style[CSS_LEN] = { 0 };
		propertybag_format_stylesheet(ptrProper, sz_style, CSS_LEN);

		if (flk)
		{
			set_chart_table_style(flk, sz_style);
			chartctrl_redraw_table(pdt->hChart, flk);
		}
		else
		{
			set_chart_style(ptr, sz_style);
			chartctrl_redraw(pdt->hChart);
		}

		ChartPanel_SetDirty(widget, 1);
	}

	destroy_proper_doc(ptrProper);
}

void ChartPanel_OnFontName(res_win_t widget, void* pv)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	fontname_menu(widget, IDC_CHARTPANEL_FONTNAME, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void ChartPanel_OnFontSize(res_win_t widget, void* pv)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	fontsize_menu(widget, IDC_CHARTPANEL_FONTSIZE, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void ChartPanel_OnTextColor(res_win_t widget, void* pv)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	color_menu(widget, IDC_CHARTPANEL_FONTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void ChartPanel_OnPaintColor(res_win_t widget, void* pv)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	color_menu(widget, IDC_CHARTPANEL_PAINTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void ChartPanel_OnDrawColor(res_win_t widget, void* pv)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	color_menu(widget, IDC_CHARTPANEL_DRAWCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void ChartPanel_OnTextNear(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_NEAR);
}

void ChartPanel_OnTextBold(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SelectAttr(widget, GDI_ATTR_FONT_WEIGHT, GDI_ATTR_FONT_WEIGHT_BOLD);
}

void ChartPanel_OnTextCenter(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_CENTER);
}

void ChartPanel_OnTextFar(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	ChartPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_FAR);
}

void ChartPanel_OnAlignNear(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (!ptrItem)
		return;

	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_chart_table_selected(flk))
		{
			set_chart_table_x(flk, get_chart_table_x(ptrItem));
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnAlignCenter(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (!ptrItem)
		return;

	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_chart_table_selected(flk))
		{
			set_chart_table_x(flk, get_chart_table_x(ptrItem) + get_chart_table_width(ptrItem) / 2 - get_chart_table_width(flk) / 2);
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnAlignFar(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (!ptrItem)
		return;

	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_chart_table_selected(flk))
		{
			set_chart_table_x(flk, get_chart_table_x(ptrItem) + get_chart_table_width(ptrItem) - get_chart_table_width(flk));
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnSizeHeight(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (!ptrItem)
		return;

	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_chart_table_selected(flk))
		{
			set_chart_table_height(flk, get_chart_table_height(ptrItem));
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnSizeWidth(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (!ptrItem)
		return;

	if (!get_chart_table_selected_count(ptrChart))
		return;

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_chart_table_selected(flk))
		{
			set_chart_table_width(flk, get_chart_table_width(ptrItem));
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnSizeHorz(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	int count = get_chart_table_selected_count(ptrChart);
	if (count <= 1)
		return;

	ChartPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min,f_max,f_total = 0;
	count = 0;
	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (get_chart_table_selected(flk))
		{
			pxs[count].fact = (int)(get_chart_table_x(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_chart_table_width(flk);
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_chart_table_x((link_t_ptr)pxs[0].data);
	f_max = get_chart_table_x((link_t_ptr)pxs[count - 1].data) + get_chart_table_width((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_chart_table_x((link_t_ptr)pxs[0].data) + get_chart_table_width((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_chart_table_x((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_chart_table_x((link_t_ptr)pxs[i].data) + get_chart_table_width((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnSizeVert(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	int count = get_chart_table_selected_count(ptrChart);
	if (count <= 1)
		return;

	ChartPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min, f_max, f_total = 0;
	count = 0;
	LINKPTR flk = get_chart_next_table(ptrChart, LINK_FIRST);
	while (flk)
	{
		if (get_chart_table_selected(flk))
		{
			pxs[count].fact = (int)(get_chart_table_y(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_chart_table_height(flk);
		}

		flk = get_chart_next_table(ptrChart, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_chart_table_y((link_t_ptr)pxs[0].data);
	f_max = get_chart_table_y((link_t_ptr)pxs[count - 1].data) + get_chart_table_height((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_chart_table_y((link_t_ptr)pxs[0].data) + get_chart_table_height((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_chart_table_y((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_chart_table_y((link_t_ptr)pxs[i].data) + get_chart_table_height((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	chartctrl_redraw(pdt->hChart);
}

void ChartPanel_OnSendBack(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (!ptrItem)
		return;

	ChartPanel_SetDirty(widget, 1);

	set_chart_table_taborder(ptrItem, 0);

	LINKPTR root = get_dom_child_node_root(ptrChart);

	switch_link_first(root, ptrItem);

	chartctrl_redraw(pdt->hChart);
}


void ChartPanel_OnLineTable(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_chart_table(ptrChart, DOC_CHART_LINE);

	set_chart_table_x(flk, 1);
	set_chart_table_y(flk, 1);
	set_chart_table_width(flk, DEF_TABLE_WIDTH);
	set_chart_table_height(flk, DEF_TABLE_HEIGHT);

	int count = get_chart_table_count_by_class(ptrChart, DOC_CHART_LINE);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_CHART_LINE, count);

	set_chart_table_name(flk, token);

	chartctrl_redraw_table(pdt->hChart,flk);
}

void ChartPanel_OnBarTable(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_chart_table(ptrChart, DOC_CHART_BAR);

	set_chart_table_x(flk, 1);
	set_chart_table_y(flk, 1);
	set_chart_table_width(flk, DEF_TABLE_WIDTH);
	set_chart_table_height(flk, DEF_TABLE_HEIGHT);

	int count = get_chart_table_count_by_class(ptrChart, DOC_CHART_BAR);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_CHART_BAR, count);

	set_chart_table_name(flk, token);

	chartctrl_redraw_table(pdt->hChart,flk);
}

void ChartPanel_OnPieTable(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_chart_table(ptrChart, DOC_CHART_PIE);

	set_chart_table_x(flk, 1);
	set_chart_table_y(flk, 1);
	set_chart_table_width(flk, DEF_TABLE_WIDTH);
	set_chart_table_height(flk, DEF_TABLE_HEIGHT);

	int count = get_chart_table_count_by_class(ptrChart, DOC_CHART_PIE);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_CHART_BAR, count);

	set_chart_table_name(flk, token);

	chartctrl_redraw_table(pdt->hChart,flk);
}

void ChartPanel_OnMapTable(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_chart_table(ptrChart, DOC_CHART_MAP);

	set_chart_table_x(flk, 1);
	set_chart_table_y(flk, 1);
	set_chart_table_width(flk, DEF_TABLE_WIDTH);
	set_chart_table_height(flk, DEF_TABLE_HEIGHT);

	int count = get_chart_table_count_by_class(ptrChart, DOC_CHART_MAP);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_CHART_MAP, count);

	set_chart_table_name(flk, token);

	chartctrl_redraw_table(pdt->hChart, flk);
}

void ChartPanel_OnDotTable(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_chart_table(ptrChart, DOC_CHART_DOT);

	set_chart_table_x(flk, 1);
	set_chart_table_y(flk, 1);
	set_chart_table_width(flk, DEF_TABLE_WIDTH);
	set_chart_table_height(flk, DEF_TABLE_HEIGHT);

	int count = get_chart_table_count_by_class(ptrChart, DOC_CHART_DOT);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_CHART_DOT, count);

	set_chart_table_name(flk, token);

	chartctrl_redraw_table(pdt->hChart, flk);
}

void ChartPanel_OnCounterTable(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);

	ChartPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_chart_table(ptrChart, DOC_CHART_COUNTER);

	set_chart_table_x(flk, 10);
	set_chart_table_y(flk, 10);
	set_chart_table_width(flk, DEF_TEXT_WIDTH);
	set_chart_table_height(flk, DEF_TEXT_HEIGHT);

	int count = get_chart_table_count_by_class(ptrChart, DOC_CHART_COUNTER);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_CHART_COUNTER, count);

	set_chart_table_name(flk, token);

	chartctrl_redraw_table(pdt->hChart, flk);
}

void ChartPanel_OnAttributes(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (ptrItem)
		propertybag_write_chart_table_attributes(ptrProper, ptrItem);
	else
		propertybag_write_chart_attributes(ptrProper, ptrChart);

	properctrl_redraw(pdt->hProper);
}

void ChartPanel_OnStyleSheet(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	if (ptrItem)
		propertybag_parse_stylesheet(ptrProper, get_chart_table_style_ptr(ptrItem));
	else
		propertybag_parse_stylesheet(ptrProper, get_chart_style_ptr(ptrChart));

	properctrl_redraw(pdt->hProper);
}

void ChartPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ilk = titlectrl_get_focus_item(pdt->hTitle);
	if (!ilk)
		return;

	long n_id = xstol(get_title_item_id_ptr(ilk));

	LINKPTR ptrChart = chartctrl_fetch(pdt->hChart);
	LINKPTR ptrItem = chartctrl_get_focus_table(pdt->hChart);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrItem)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			propertybag_read_chart_table_attributes(pnp->proper, ptrItem);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			propertybag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_chart_table_style(ptrItem, sz_style);
		}
		chartctrl_redraw_table(pdt->hChart, ptrItem);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			propertybag_read_chart_attributes(pnp->proper, ptrChart);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			propertybag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_chart_style(ptrChart, sz_style);
		}
		chartctrl_redraw(pdt->hChart);
	}
}

void ChartPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void ChartPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	long n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void ChartPanel_Chart_OnRBClick(res_win_t widget, NOTICE_CHART* pnf)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	if (!pnf->table)
		return;

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_CHARTPANEL_MENU);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);

	widget_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();

	LINKPTR mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_COPY);
	set_menu_item_title(mlk, _T("拷贝部件"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_CUT);
	set_menu_item_title(mlk, _T("剪切部件"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_PASTE);
	set_menu_item_title(mlk, _T("粘贴部件"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_DELETE);
	set_menu_item_title(mlk, _T("删除部件"));

	menubox_set_data(hMenu, ptrMenu);

	xpoint_t xp;
	xp.x = ppt->x;
	xp.y = ppt->y;

	widget_client_to_screen(pdt->hChart, &xp);

	menubox_layout(hMenu, &xp, WD_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void ChartPanel_Chart_OnLBClick(res_win_t widget, NOTICE_CHART* pnf)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void ChartPanel_Chart_OnItemSize(res_win_t widget, NOTICE_CHART* pnf)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void ChartPanel_Chart_OnItemMove(res_win_t widget, NOTICE_CHART* pnf)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}


/***********************************************************************************************************/
int ChartPanel_OnCreate(res_win_t widget, void* data)
{
	ChartPanelDelta* pdt = (ChartPanelDelta*)xmem_alloc(sizeof(ChartPanelDelta));
	xrect_t xr;
	const tchar_t* szParam;

	widget_hand_create(widget);

	SETCHARTPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(CHARTPANEL_ACCEL, CHARTPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	szParam = (tchar_t*)data;

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkChart = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -CHARTPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);
	set_split_item_fixed(ilkRight, 1);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hChart = chartctrl_create(_T("ChartPanel"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hChart, IDC_CHARTPANEL_CHART);
	widget_set_owner(pdt->hChart, widget);

	set_split_item_delta(ilkChart, pdt->hChart);
	widget_show(pdt->hChart, WD_SHOW_NORMAL);

	LINKPTR ptrChart = create_chart_doc();
	chartctrl_attach(pdt->hChart, ptrChart);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("ChartProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_CHARTPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("ChartTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_CHARTPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WD_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();
	set_title_images(ptrTitle, g_imagelist);

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, CHARTPANEL_TITLEITEM_WIDTH);
	set_title_item_image(tlk, BMP_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("样式"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, CHARTPANEL_TITLEITEM_WIDTH);
	set_title_item_image(tlk, BMP_DRAW);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);

	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	if (!is_null(szParam))
	{
		if (!ChartPanel_OpenFile(widget, szParam))
			return -1;

		chartctrl_redraw(pdt->hChart);
	}

	return 0;
}

void ChartPanel_OnDestroy(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hChart))
	{
		LINKPTR ptrChart = chartctrl_detach(pdt->hChart);
		if (ptrChart)
			destroy_chart_doc(ptrChart);

		widget_destroy(pdt->hChart);
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

void ChartPanel_OnShow(res_win_t widget, bool_t bShow)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, CHARTPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, CHARTPANEL_GROUPITEM_HEIGHT);
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
		set_tool_group_item_width(glk, CHARTPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, CHARTPANEL_GROUPITEM_HEIGHT);
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

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PAINT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("背景颜色"));
		set_tool_item_image(ilk, BMP_PAINT);

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_DRAW_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("前景"));
		set_tool_item_image(ilk, BMP_DRAW);*/

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PROPER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("CSS属性"));
		set_tool_item_image(ilk, BMP_PROPER);*/

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

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_WIDTH);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等宽"));
		set_tool_item_image(ilk, BMP_SIZEWIDTH);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等高"));
		set_tool_item_image(ilk, BMP_SIZEHEIGHT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HORZ);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("水平等距"));
		set_tool_item_image(ilk, BMP_SIZEHORZ);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_VERT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("垂直等距"));
		set_tool_item_image(ilk, BMP_SIZEVERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SENDBACK);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("置于后端"));
		set_tool_item_image(ilk, BMP_SENDBACK);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_ELEMENT);
		set_tool_group_title(glk, _T("部件"));
		set_tool_group_item_width(glk, CHARTPANEL_GROUPTEXT_WIDTH);
		set_tool_group_item_height(glk, CHARTPANEL_GROUPITEM_HEIGHT);
		//set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_MAPTABLE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("全景图"));
		set_tool_item_image(ilk, BMP_MAPCHART);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_LINETABLE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("趋势图"));
		set_tool_item_image(ilk, BMP_LINECHART);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_BARTABLE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("分布图"));
		set_tool_item_image(ilk, BMP_BARCHART);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_PIETABLE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("构成图"));
		set_tool_item_image(ilk, BMP_PIECHART);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_DOTTABLE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("密度图"));
		set_tool_item_image(ilk, BMP_DOTCHART);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_COUNTERTABLE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("计数器"));
		set_tool_item_image(ilk, BMP_COUNTER);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

int ChartPanel_OnClose(res_win_t widget)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	ChartPanel_Switch(widget);

	return (chartctrl_get_dirty(pdt->hChart)) ? 1 : 0;
}

void ChartPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	LINKPTR ptr_chart = chartctrl_fetch(pdt->hChart);
	LINKPTR flk = get_chart_next_table(ptr_chart, LINK_FIRST);
	while (flk)
	{
		if (compare_text(pfd->sub_str, -1, get_chart_table_name_ptr(flk), -1, 1) == 0)
		{
			chartctrl_set_focus_table(pdt->hChart, flk);
			break;
		}
		flk = get_chart_next_table(ptr_chart, flk);
	}
}

void ChartPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_CHART);
		pqo->ptrDoc = chartctrl_fetch(pdt->hChart);
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
		chartctrl_set_dirty(pdt->hChart, 0);
	}
}

void ChartPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	tchar_t token[RES_LEN];

	switch (cid)
	{
	case IDA_FILE_SAVE:
		ChartPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		ChartPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		ChartPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		ChartPanel_OnPreview(widget);
		break;
	case IDA_FILE_EXEC:
		ChartPanel_OnExec(widget);
		break;
	case IDA_EDIT_SELECTALL:
		ChartPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		ChartPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		ChartPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		ChartPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		ChartPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		ChartPanel_OnUndo(widget);
		break;
	case IDA_STYLE_PROPER:
		ChartPanel_OnCSSProper(widget);
		break;
	case IDA_STYLE_FONT_NAME:
		ChartPanel_OnFontName(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_SIZE:
		ChartPanel_OnFontSize(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_WEIGHT:
		ChartPanel_OnTextBold(widget);
		break;
	case IDA_STYLE_TEXT_COLOR:
		ChartPanel_OnTextColor(widget, (void*)data);
		break;
	case IDA_STYLE_PAINT_COLOR:
		ChartPanel_OnPaintColor(widget, (void*)data);
		break;
	case IDA_STYLE_DRAW_COLOR:
		ChartPanel_OnDrawColor(widget, (void*)data);
		break;
	case IDA_STYLE_TEXT_NEAR:
		ChartPanel_OnTextNear(widget);
		break;
	case IDA_STYLE_TEXT_CENTER:
		ChartPanel_OnTextCenter(widget);
		break;
	case IDA_STYLE_TEXT_FAR:
		ChartPanel_OnTextFar(widget);
		break;
	case IDA_STYLE_ALIGN_NEAR:
		ChartPanel_OnAlignNear(widget);
		break;
	case IDA_STYLE_ALIGN_CENTER:
		ChartPanel_OnAlignCenter(widget);
		break;
	case IDA_STYLE_ALIGN_FAR:
		ChartPanel_OnAlignFar(widget);
		break;
	case IDA_STYLE_SIZE_HORZ:
		ChartPanel_OnSizeHorz(widget);
		break;
	case IDA_STYLE_SIZE_VERT:
		ChartPanel_OnSizeVert(widget);
		break;
	case IDA_STYLE_SIZE_WIDTH:
		ChartPanel_OnSizeWidth(widget);
		break;
	case IDA_STYLE_SIZE_HEIGHT:
		ChartPanel_OnSizeHeight(widget);
		break;
	case IDA_STYLE_SENDBACK:
		ChartPanel_OnSendBack(widget);
		break;

	case IDA_APPEND_MAPTABLE:
		ChartPanel_OnMapTable(widget);
		break;
	case IDA_APPEND_LINETABLE:
		ChartPanel_OnLineTable(widget);
		break;
	case IDA_APPEND_BARTABLE:
		ChartPanel_OnBarTable(widget);
		break;
	case IDA_APPEND_PIETABLE:
		ChartPanel_OnPieTable(widget);
		break;
	case IDA_APPEND_DOTTABLE:
		ChartPanel_OnDotTable(widget);
		break;
	case IDA_APPEND_COUNTERTABLE:
		ChartPanel_OnCounterTable(widget);
		break;

	case IDA_ATTRIBUTES:
		ChartPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		ChartPanel_OnStyleSheet(widget);
		break;

	case IDC_CHARTPANEL_FONTNAME:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontname_menu_item(code, token, RES_LEN);
			ChartPanel_SelectAttr(widget, GDI_ATTR_FONT_FAMILY, token);
		}
		break;
	case IDC_CHARTPANEL_FONTSIZE:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontsize_menu_item(code, token, RES_LEN);
			ChartPanel_SelectAttr(widget, GDI_ATTR_FONT_SIZE, token);
		}
		break;
	case IDC_CHARTPANEL_FONTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			ChartPanel_SelectAttr(widget, GDI_ATTR_FONT_COLOR, token);
		}
		break;
	case IDC_CHARTPANEL_PAINTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			ChartPanel_SelectAttr(widget, GDI_ATTR_FILL_COLOR, token);
		}
		break;
	case IDC_CHARTPANEL_DRAWCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			ChartPanel_SelectAttr(widget, GDI_ATTR_STROKE_COLOR, token);
		}
		break;
	}
}

void ChartPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	ChartPanelDelta* pdt = GETCHARTPANELDELTA(widget);

	if (phdr->id == IDC_CHARTPANEL_CHART)
	{
		NOTICE_CHART* pnf = (NOTICE_CHART*)phdr;
		switch (pnf->code)
		{
		case NC_CHARTCALCED:
			break;
		case NC_FIELDCALCED:
			break;
		case NC_CHARTLBCLK:
			ChartPanel_Chart_OnLBClick(widget, pnf);
			break;
		case NC_CHARTRBCLK:
			ChartPanel_Chart_OnRBClick(widget, pnf);
			break;
		case NC_CHARTTABLESIZED:
			ChartPanel_Chart_OnItemSize(widget, pnf);
			break;
		case NC_CHARTTABLEDROP:
			ChartPanel_Chart_OnItemMove(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_CHARTPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			ChartPanel_SetDirty(widget, 1);
			break;
		case NC_ENTITYUPDATE:
			ChartPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_CHARTPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			ChartPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			ChartPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/**************************************************************************************************************/
res_win_t ChartPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(ChartPanel_OnCreate)
		EVENT_ON_DESTROY(ChartPanel_OnDestroy)
		EVENT_ON_CLOSE(ChartPanel_OnClose)
		EVENT_ON_SHOW(ChartPanel_OnShow)

		EVENT_ON_NOTICE(ChartPanel_OnNotice)
		EVENT_ON_COMMAND_FIND(ChartPanel_OnCommandFind)
		EVENT_ON_PARENT_COMMAND(ChartPanel_OnParentCommand)
		EVENT_ON_MENU_COMMAND(ChartPanel_OnMenuCommand)

		EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	ev.param = (void*)fpath;

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}