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
#include "PanoramaPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_PANORAMAPANEL_PANORAMA			201
#define IDC_PANORAMAPANEL_PROPER			202
#define IDC_PANORAMAPANEL_TITLE			203
#define IDC_PANORAMAPANEL_MENU			204
#define IDC_PANORAMAPANEL_FONTNAME		210
#define IDC_PANORAMAPANEL_FONTSIZE		211
#define IDC_PANORAMAPANEL_FONTCOLOR		212
#define IDC_PANORAMAPANEL_PAINTCOLOR		213
#define IDC_PANORAMAPANEL_DRAWCOLOR		214

#define PANORAMAPANEL_GROUPITEM_WIDTH		(float)8
#define PANORAMAPANEL_GROUPITEM_HEIGHT		(float)7
#define PANORAMAPANEL_GROUPTEXT_WIDTH		(float)15
#define PANORAMAPANEL_TITLEITEM_WIDTH		(float)15
#define PANORAMAPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagPanoramaPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hPanorama;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}PanoramaPanelDelta;

#define GETPANORAMAPANELDELTA(ph) 		(PanoramaPanelDelta*)widget_get_user_delta(ph)
#define SETPANORAMAPANELDELTA(ph,ptd)		widget_set_user_delta(ph,(var_long)ptd)

#define PANORAMAPANEL_ACCEL_COUNT	5
accel_t	PANORAMAPANEL_ACCEL[PANORAMAPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY,			VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

/***************************************************************************************/
void PanoramaPanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	panoramactrl_set_dirty(pdt->hPanorama, bDirty);
}

bool_t PanoramaPanel_GetDirty(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	return panoramactrl_get_dirty(pdt->hPanorama);
}

void PanoramaPanel_Switch(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	if (!PanoramaPanel_GetDirty(widget))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));
	
	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		PanoramaPanel_SetDirty(widget, 0);
		break;
	}
}

LINKPTR PanoramaPanel_GetDocument(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	return panoramactrl_fetch(pdt->hPanorama);
}

bool_t PanoramaPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR newPanorama = create_panorama_from_meta_file(NULL, szFile);
	if (!newPanorama)
	{
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	LINKPTR orgPanorama = panoramactrl_detach(pdt->hPanorama);
	destroy_panorama_doc(orgPanorama);

	panoramactrl_attach(pdt->hPanorama, newPanorama);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t PanoramaPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrPanorama);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrPanorama = detach_meta_body_node(ptrMeta);
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

void PanoramaPanel_SelectAttr(res_win_t widget, const tchar_t* attr_name, const tchar_t* attr_val)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	tchar_t style[CSS_LEN];

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (flk == ptrItem || get_panorama_plot_selected(flk))
		{
			write_style_attr(get_panorama_plot_style_ptr(flk), -1, attr_name, -1, attr_val, -1, style, CSS_LEN);
			set_panorama_plot_style(flk, style);
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	panoramactrl_redraw(pdt->hPanorama);
}


/*********************************************************************************************************/
void PanoramaPanel_OnSave(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("panorama meta file(*.panorama)\0*.panorama\0"), _T("panorama"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	if (PanoramaPanel_SaveFile(widget, szFile))
	{
		PanoramaPanel_SetDirty(widget, 0);
	}
}

void PanoramaPanel_OnSaveAs(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Panorama file(*.panorama)\0*.panorama\0"), _T("panorama"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrPanorama);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrPanorama = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void PanoramaPanel_OnPrint(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	//print_panorama(NULL, ptrPanorama);
}

void PanoramaPanel_OnPreview(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_panorama(svg, ptrPanorama);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WD_SHOW_FULLSCREEN);
}

void PanoramaPanel_OnExec(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptr_dlg = panoramactrl_fetch(pdt->hPanorama);;

	res_win_t dlg = create_dialog(ptr_dlg, widget);

	clr_mod_t clr = { 0 };

	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);
	parse_xcolor(&clr.clr_ico, g_face[g_indFace].ico);

	widget_set_color_mode(dlg, &clr);

	widget_show(dlg, WD_SHOW_NORMAL);

	widget_do_modal(dlg);
}

void PanoramaPanel_OnSelectAll(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	XDL_ASSERT(ptrPanorama);

	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		set_panorama_plot_selected(flk, 1);
	
		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnDelete(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	XDL_ASSERT(ptrPanorama);

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	panoramactrl_set_focus_plot(pdt->hPanorama, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		nlk = get_panorama_next_plot(ptrPanorama, flk);
		if (get_panorama_plot_selected(flk) || ptrItem == flk)
		{
			delete_panorama_plot(flk);
			flk = nlk;

			bRedraw = 1;
			continue;
		}

		flk = nlk;
	}

	if (bRedraw)
		panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnCopy(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	widget_copy(pdt->hPanorama);
}

void PanoramaPanel_OnCut(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	widget_cut(pdt->hPanorama);
}

void PanoramaPanel_OnPaste(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	widget_paste(pdt->hPanorama);
}


void PanoramaPanel_OnRedo(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

}

void PanoramaPanel_OnUndo(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	widget_undo(pdt->hPanorama);
}

void PanoramaPanel_OnCSSProper(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrProper = create_proper_doc();

	LINKPTR ptr = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR flk = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (flk)
		properbag_parse_stylesheet(ptrProper, get_panorama_plot_style_ptr(flk));
	else
		properbag_parse_stylesheet(ptrProper, get_panorama_style_ptr(ptr));

	res_win_t hProperDlg = properdlg_create(_T("绘制样式"), ptrProper, g_hMain);

	widget_show(hProperDlg, WD_SHOW_NORMAL);

	int nRet = widget_do_modal(hProperDlg);

	if (nRet)
	{
		tchar_t sz_style[CSS_LEN] = { 0 };
		properbag_format_stylesheet(ptrProper, sz_style, CSS_LEN);

		if (flk)
		{
			set_panorama_plot_style(flk, sz_style);
			panoramactrl_redraw_plot(pdt->hPanorama, flk);
		}
		else
		{
			set_panorama_style(ptr, sz_style);
			panoramactrl_redraw(pdt->hPanorama);
		}

		PanoramaPanel_SetDirty(widget, 1);
	}

	destroy_proper_doc(ptrProper);
}

void PanoramaPanel_OnFontName(res_win_t widget, void* pv)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	fontname_menu(widget, IDC_PANORAMAPANEL_FONTNAME, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void PanoramaPanel_OnFontSize(res_win_t widget, void* pv)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	fontsize_menu(widget, IDC_PANORAMAPANEL_FONTSIZE, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void PanoramaPanel_OnTextColor(res_win_t widget, void* pv)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	color_menu(widget, IDC_PANORAMAPANEL_FONTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void PanoramaPanel_OnPaintColor(res_win_t widget, void* pv)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	color_menu(widget, IDC_PANORAMAPANEL_PAINTCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void PanoramaPanel_OnDrawColor(res_win_t widget, void* pv)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	color_menu(widget, IDC_PANORAMAPANEL_DRAWCOLOR, (xpoint_t*)pv, WD_LAYOUT_RIGHTBOTTOM);
}

void PanoramaPanel_OnTextNear(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_NEAR);
}

void PanoramaPanel_OnTextBold(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SelectAttr(widget, GDI_ATTR_FONT_WEIGHT, GDI_ATTR_FONT_WEIGHT_BOLD);
}

void PanoramaPanel_OnTextCenter(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_CENTER);
}

void PanoramaPanel_OnTextFar(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	PanoramaPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_FAR);
}

void PanoramaPanel_OnAlignNear(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (!ptrItem)
		return;

	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_panorama_plot_selected(flk))
		{
			set_panorama_plot_x(flk, get_panorama_plot_x(ptrItem));
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnAlignCenter(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (!ptrItem)
		return;

	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_panorama_plot_selected(flk))
		{
			set_panorama_plot_x(flk, get_panorama_plot_x(ptrItem) + get_panorama_plot_width(ptrItem) / 2 - get_panorama_plot_width(flk) / 2);
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnAlignFar(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (!ptrItem)
		return;

	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_panorama_plot_selected(flk))
		{
			set_panorama_plot_x(flk, get_panorama_plot_x(ptrItem) + get_panorama_plot_width(ptrItem) - get_panorama_plot_width(flk));
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnSizeHeight(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (!ptrItem)
		return;

	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_panorama_plot_selected(flk))
		{
			set_panorama_plot_height(flk, get_panorama_plot_height(ptrItem));
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnSizeWidth(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (!ptrItem)
		return;

	if (!get_panorama_plot_selected_count(ptrPanorama))
		return;

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_panorama_plot_selected(flk))
		{
			set_panorama_plot_width(flk, get_panorama_plot_width(ptrItem));
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnSizeHorz(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	int count = get_panorama_plot_selected_count(ptrPanorama);
	if (count <= 1)
		return;

	PanoramaPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min,f_max,f_total = 0;
	count = 0;
	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (get_panorama_plot_selected(flk))
		{
			pxs[count].fact = (int)(get_panorama_plot_x(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_panorama_plot_width(flk);
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_panorama_plot_x((link_t_ptr)pxs[0].data);
	f_max = get_panorama_plot_x((link_t_ptr)pxs[count - 1].data) + get_panorama_plot_width((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_panorama_plot_x((link_t_ptr)pxs[0].data) + get_panorama_plot_width((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_panorama_plot_x((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_panorama_plot_x((link_t_ptr)pxs[i].data) + get_panorama_plot_width((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnSizeVert(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	int count = get_panorama_plot_selected_count(ptrPanorama);
	if (count <= 1)
		return;

	PanoramaPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min, f_max, f_total = 0;
	count = 0;
	LINKPTR flk = get_panorama_next_plot(ptrPanorama, LINK_FIRST);
	while (flk)
	{
		if (get_panorama_plot_selected(flk))
		{
			pxs[count].fact = (int)(get_panorama_plot_y(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_panorama_plot_height(flk);
		}

		flk = get_panorama_next_plot(ptrPanorama, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_panorama_plot_y((link_t_ptr)pxs[0].data);
	f_max = get_panorama_plot_y((link_t_ptr)pxs[count - 1].data) + get_panorama_plot_height((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_panorama_plot_y((link_t_ptr)pxs[0].data) + get_panorama_plot_height((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_panorama_plot_y((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_panorama_plot_y((link_t_ptr)pxs[i].data) + get_panorama_plot_height((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	panoramactrl_redraw(pdt->hPanorama);
}

void PanoramaPanel_OnSendBack(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (!ptrItem)
		return;

	PanoramaPanel_SetDirty(widget, 1);

	set_panorama_plot_taborder(ptrItem, 0);

	LINKPTR root = get_dom_child_node_root(ptrPanorama);

	switch_link_first(root, ptrItem);

	panoramactrl_redraw(pdt->hPanorama);
}


void PanoramaPanel_OnTrends(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_panorama_plot(ptrPanorama, DOC_PANORAMA_TRENDS);

	set_panorama_plot_x(flk, 1);
	set_panorama_plot_y(flk, 1);
	set_panorama_plot_width(flk, DEF_PLOT_WIDTH);
	set_panorama_plot_height(flk, DEF_PLOT_HEIGHT);

	int count = get_panorama_plot_count_by_class(ptrPanorama, DOC_PANORAMA_TRENDS);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_PANORAMA_TRENDS, count);

	set_panorama_plot_name(flk, token);

	panoramactrl_redraw_plot(pdt->hPanorama,flk);
}

void PanoramaPanel_OnScatter(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_panorama_plot(ptrPanorama, DOC_PANORAMA_SCATTER);

	set_panorama_plot_x(flk, 1);
	set_panorama_plot_y(flk, 1);
	set_panorama_plot_width(flk, DEF_PLOT_WIDTH);
	set_panorama_plot_height(flk, DEF_PLOT_HEIGHT);

	int count = get_panorama_plot_count_by_class(ptrPanorama, DOC_PANORAMA_SCATTER);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_PANORAMA_SCATTER, count);

	set_panorama_plot_name(flk, token);

	panoramactrl_redraw_plot(pdt->hPanorama,flk);
}

void PanoramaPanel_OnPanto(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_panorama_plot(ptrPanorama, DOC_PANORAMA_PANTO);

	set_panorama_plot_x(flk, 1);
	set_panorama_plot_y(flk, 1);
	set_panorama_plot_width(flk, DEF_PLOT_WIDTH);
	set_panorama_plot_height(flk, DEF_PLOT_HEIGHT);

	int count = get_panorama_plot_count_by_class(ptrPanorama, DOC_PANORAMA_PANTO);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_PANORAMA_SCATTER, count);

	set_panorama_plot_name(flk, token);

	panoramactrl_redraw_plot(pdt->hPanorama,flk);
}

void PanoramaPanel_OnGeomap(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_panorama_plot(ptrPanorama, DOC_PANORAMA_GEOMAP);

	set_panorama_plot_x(flk, 1);
	set_panorama_plot_y(flk, 1);
	set_panorama_plot_width(flk, DEF_PLOT_WIDTH);
	set_panorama_plot_height(flk, DEF_PLOT_HEIGHT);

	int count = get_panorama_plot_count_by_class(ptrPanorama, DOC_PANORAMA_GEOMAP);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_PANORAMA_GEOMAP, count);

	set_panorama_plot_name(flk, token);

	panoramactrl_redraw_plot(pdt->hPanorama, flk);
}

void PanoramaPanel_OnDensity(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_panorama_plot(ptrPanorama, DOC_PANORAMA_DENSITY);

	set_panorama_plot_x(flk, 1);
	set_panorama_plot_y(flk, 1);
	set_panorama_plot_width(flk, DEF_PLOT_WIDTH);
	set_panorama_plot_height(flk, DEF_PLOT_HEIGHT);

	int count = get_panorama_plot_count_by_class(ptrPanorama, DOC_PANORAMA_DENSITY);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_PANORAMA_DENSITY, count);

	set_panorama_plot_name(flk, token);

	panoramactrl_redraw_plot(pdt->hPanorama, flk);
}

void PanoramaPanel_OnCounter(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);

	PanoramaPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_panorama_plot(ptrPanorama, DOC_PANORAMA_COUNTER);

	set_panorama_plot_x(flk, 10);
	set_panorama_plot_y(flk, 10);
	set_panorama_plot_width(flk, DEF_TEXT_WIDTH);
	set_panorama_plot_height(flk, DEF_TEXT_HEIGHT);

	int count = get_panorama_plot_count_by_class(ptrPanorama, DOC_PANORAMA_COUNTER);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_PANORAMA_COUNTER, count);

	set_panorama_plot_name(flk, token);

	panoramactrl_redraw_plot(pdt->hPanorama, flk);
}

void PanoramaPanel_OnAttributes(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (ptrItem)
		properbag_write_panorama_plot_attributes(ptrProper, ptrItem);
	else
		properbag_write_panorama_attributes(ptrProper, ptrPanorama);

	properctrl_redraw(pdt->hProper);
}

void PanoramaPanel_OnStyleSheet(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	if (ptrItem)
		properbag_parse_stylesheet(ptrProper, get_panorama_plot_style_ptr(ptrItem));
	else
		properbag_parse_stylesheet(ptrProper, get_panorama_style_ptr(ptrPanorama));

	properctrl_redraw(pdt->hProper);
}

void PanoramaPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ilk = titlectrl_get_focus_item(pdt->hTitle);
	if (!ilk)
		return;

	long n_id = xstol(get_title_item_id_ptr(ilk));

	LINKPTR ptrPanorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR ptrItem = panoramactrl_get_focus_plot(pdt->hPanorama);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrItem)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_panorama_plot_attributes(pnp->proper, ptrItem);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_panorama_plot_style(ptrItem, sz_style);
		}
		panoramactrl_redraw_plot(pdt->hPanorama, ptrItem);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_panorama_attributes(pnp->proper, ptrPanorama);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_panorama_style(ptrPanorama, sz_style);
		}
		panoramactrl_redraw(pdt->hPanorama);
	}
}

void PanoramaPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void PanoramaPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	long n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void PanoramaPanel_Panorama_OnRBClick(res_win_t widget, NOTICE_PANORAMA* pnf)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	if (!pnf->plot)
		return;

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_PANORAMAPANEL_MENU);
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

	widget_client_to_screen(pdt->hPanorama, &xp);

	menubox_layout(hMenu, &xp, WD_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void PanoramaPanel_Panorama_OnLBClick(res_win_t widget, NOTICE_PANORAMA* pnf)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void PanoramaPanel_Panorama_OnItemSize(res_win_t widget, NOTICE_PANORAMA* pnf)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void PanoramaPanel_Panorama_OnItemMove(res_win_t widget, NOTICE_PANORAMA* pnf)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	long n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}


/***********************************************************************************************************/
int PanoramaPanel_OnCreate(res_win_t widget, void* data)
{
	PanoramaPanelDelta* pdt = (PanoramaPanelDelta*)xmem_alloc(sizeof(PanoramaPanelDelta));
	xrect_t xr;
	const tchar_t* szParam;

	widget_hand_create(widget);

	SETPANORAMAPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(PANORAMAPANEL_ACCEL, PANORAMAPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	szParam = (tchar_t*)data;

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkPanorama = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -PANORAMAPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);
	set_split_item_fixed(ilkRight, 1);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hPanorama = panoramactrl_create(_T("PanoramaPanel"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hPanorama, IDC_PANORAMAPANEL_PANORAMA);
	widget_set_owner(pdt->hPanorama, widget);

	set_split_item_delta(ilkPanorama, pdt->hPanorama);
	widget_show(pdt->hPanorama, WD_SHOW_NORMAL);

	LINKPTR ptrPanorama = create_panorama_doc();
	panoramactrl_attach(pdt->hPanorama, ptrPanorama);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("PanoramaProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_PANORAMAPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("PanoramaTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_PANORAMAPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WD_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, PANORAMAPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, ICON_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("样式"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, PANORAMAPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, ICON_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);

	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	if (!is_null(szParam))
	{
		if (!PanoramaPanel_OpenFile(widget, szParam))
			return -1;

		panoramactrl_redraw(pdt->hPanorama);
	}

	return 0;
}

void PanoramaPanel_OnDestroy(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hPanorama))
	{
		LINKPTR ptrPanorama = panoramactrl_detach(pdt->hPanorama);
		if (ptrPanorama)
			destroy_panorama_doc(ptrPanorama);

		widget_destroy(pdt->hPanorama);
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

void PanoramaPanel_OnShow(res_win_t widget, bool_t bShow)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, PANORAMAPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, PANORAMAPANEL_GROUPITEM_HEIGHT);
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
		set_tool_group_item_width(glk, PANORAMAPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, PANORAMAPANEL_GROUPITEM_HEIGHT);
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
		xsprintf(token, _T("%d"), IDA_STYLE_PAINT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("背景颜色"));
		set_tool_item_icon(ilk, ICON_BACKGROUND);

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_DRAW_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("前景"));
		set_tool_item_icon(ilk, ICON_STYLE);*/

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PROPER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("CSS属性"));
		set_tool_item_icon(ilk, ICON_PROPER);*/

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, ICON_ALIGNNEAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, ICON_ALIGNCENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
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

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_WIDTH);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等宽"));
		set_tool_item_icon(ilk, ICON_SIZEHORZ);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等高"));
		set_tool_item_icon(ilk, ICON_SIZEVERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HORZ);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("水平等距"));
		set_tool_item_icon(ilk, ICON_SPACEHORZ);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_VERT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("垂直等距"));
		set_tool_item_icon(ilk, ICON_SPACEVERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SENDBACK);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("置于后端"));
		set_tool_item_icon(ilk, ICON_ORDER);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_ELEMENT);
		set_tool_group_title(glk, _T("部件"));
		set_tool_group_item_width(glk, PANORAMAPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, PANORAMAPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_GEOMAP);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("地理图"));
		set_tool_item_icon(ilk, ICON_LOCATION);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_TRENDS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("趋势图"));
		set_tool_item_icon(ilk, ICON_TRENDS);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_SCATTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("分布图"));
		set_tool_item_icon(ilk, ICON_SCATTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_PANTO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("构成图"));
		set_tool_item_icon(ilk, ICON_PANTO);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_DENSITY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("密度图"));
		set_tool_item_icon(ilk, ICON_DENSITY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_COUNTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("计数器"));
		set_tool_item_icon(ilk, ICON_COUNTER);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

int PanoramaPanel_OnClose(res_win_t widget)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	PanoramaPanel_Switch(widget);

	return (panoramactrl_get_dirty(pdt->hPanorama)) ? 1 : 0;
}

void PanoramaPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	LINKPTR ptr_panorama = panoramactrl_fetch(pdt->hPanorama);
	LINKPTR flk = get_panorama_next_plot(ptr_panorama, LINK_FIRST);
	while (flk)
	{
		if (compare_text(pfd->sub_str, -1, get_panorama_plot_name_ptr(flk), -1, 1) == 0)
		{
			panoramactrl_set_focus_plot(pdt->hPanorama, flk);
			break;
		}
		flk = get_panorama_next_plot(ptr_panorama, flk);
	}
}

void PanoramaPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_PANORAMA);
		pqo->ptrDoc = panoramactrl_fetch(pdt->hPanorama);
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
		panoramactrl_set_dirty(pdt->hPanorama, 0);
	}
}

void PanoramaPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	tchar_t token[RES_LEN];

	switch (cid)
	{
	case IDA_FILE_SAVE:
		PanoramaPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		PanoramaPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		PanoramaPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		PanoramaPanel_OnPreview(widget);
		break;
	case IDA_FILE_EXEC:
		PanoramaPanel_OnExec(widget);
		break;
	case IDA_EDIT_SELECTALL:
		PanoramaPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		PanoramaPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		PanoramaPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		PanoramaPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		PanoramaPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		PanoramaPanel_OnUndo(widget);
		break;
	case IDA_STYLE_PROPER:
		PanoramaPanel_OnCSSProper(widget);
		break;
	case IDA_STYLE_FONT_NAME:
		PanoramaPanel_OnFontName(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_SIZE:
		PanoramaPanel_OnFontSize(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_WEIGHT:
		PanoramaPanel_OnTextBold(widget);
		break;
	case IDA_STYLE_TEXT_COLOR:
		PanoramaPanel_OnTextColor(widget, (void*)data);
		break;
	case IDA_STYLE_PAINT_COLOR:
		PanoramaPanel_OnPaintColor(widget, (void*)data);
		break;
	case IDA_STYLE_DRAW_COLOR:
		PanoramaPanel_OnDrawColor(widget, (void*)data);
		break;
	case IDA_STYLE_TEXT_NEAR:
		PanoramaPanel_OnTextNear(widget);
		break;
	case IDA_STYLE_TEXT_CENTER:
		PanoramaPanel_OnTextCenter(widget);
		break;
	case IDA_STYLE_TEXT_FAR:
		PanoramaPanel_OnTextFar(widget);
		break;
	case IDA_STYLE_ALIGN_NEAR:
		PanoramaPanel_OnAlignNear(widget);
		break;
	case IDA_STYLE_ALIGN_CENTER:
		PanoramaPanel_OnAlignCenter(widget);
		break;
	case IDA_STYLE_ALIGN_FAR:
		PanoramaPanel_OnAlignFar(widget);
		break;
	case IDA_STYLE_SIZE_HORZ:
		PanoramaPanel_OnSizeHorz(widget);
		break;
	case IDA_STYLE_SIZE_VERT:
		PanoramaPanel_OnSizeVert(widget);
		break;
	case IDA_STYLE_SIZE_WIDTH:
		PanoramaPanel_OnSizeWidth(widget);
		break;
	case IDA_STYLE_SIZE_HEIGHT:
		PanoramaPanel_OnSizeHeight(widget);
		break;
	case IDA_STYLE_SENDBACK:
		PanoramaPanel_OnSendBack(widget);
		break;

	case IDA_APPEND_GEOMAP:
		PanoramaPanel_OnGeomap(widget);
		break;
	case IDA_APPEND_TRENDS:
		PanoramaPanel_OnTrends(widget);
		break;
	case IDA_APPEND_SCATTER:
		PanoramaPanel_OnScatter(widget);
		break;
	case IDA_APPEND_PANTO:
		PanoramaPanel_OnPanto(widget);
		break;
	case IDA_APPEND_DENSITY:
		PanoramaPanel_OnDensity(widget);
		break;
	case IDA_APPEND_COUNTER:
		PanoramaPanel_OnCounter(widget);
		break;

	case IDA_ATTRIBUTES:
		PanoramaPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		PanoramaPanel_OnStyleSheet(widget);
		break;

	case IDC_PANORAMAPANEL_FONTNAME:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontname_menu_item(code, token, RES_LEN);
			PanoramaPanel_SelectAttr(widget, GDI_ATTR_FONT_FAMILY, token);
		}
		break;
	case IDC_PANORAMAPANEL_FONTSIZE:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontsize_menu_item(code, token, RES_LEN);
			PanoramaPanel_SelectAttr(widget, GDI_ATTR_FONT_SIZE, token);
		}
		break;
	case IDC_PANORAMAPANEL_FONTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			PanoramaPanel_SelectAttr(widget, GDI_ATTR_FONT_COLOR, token);
		}
		break;
	case IDC_PANORAMAPANEL_PAINTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			PanoramaPanel_SelectAttr(widget, GDI_ATTR_FILL_COLOR, token);
		}
		break;
	case IDC_PANORAMAPANEL_DRAWCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			PanoramaPanel_SelectAttr(widget, GDI_ATTR_STROKE_COLOR, token);
		}
		break;
	}
}

void PanoramaPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	PanoramaPanelDelta* pdt = GETPANORAMAPANELDELTA(widget);

	if (phdr->id == IDC_PANORAMAPANEL_PANORAMA)
	{
		NOTICE_PANORAMA* pnf = (NOTICE_PANORAMA*)phdr;
		switch (pnf->code)
		{
		case NC_PANORAMACALCED:
			break;
		case NC_FIELDCALCED:
			break;
		case NC_PANORAMALBCLK:
			PanoramaPanel_Panorama_OnLBClick(widget, pnf);
			break;
		case NC_PANORAMARBCLK:
			PanoramaPanel_Panorama_OnRBClick(widget, pnf);
			break;
		case NC_PANORAMAPLOTSIZED:
			PanoramaPanel_Panorama_OnItemSize(widget, pnf);
			break;
		case NC_PANORAMAPLOTDROP:
			PanoramaPanel_Panorama_OnItemMove(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_PANORAMAPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			PanoramaPanel_SetDirty(widget, 1);
			break;
		case NC_ENTITYUPDATE:
			PanoramaPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_PANORAMAPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			PanoramaPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			PanoramaPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/**************************************************************************************************************/
res_win_t PanoramaPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(PanoramaPanel_OnCreate)
		EVENT_ON_DESTROY(PanoramaPanel_OnDestroy)
		EVENT_ON_CLOSE(PanoramaPanel_OnClose)
		EVENT_ON_SHOW(PanoramaPanel_OnShow)

		EVENT_ON_NOTICE(PanoramaPanel_OnNotice)
		EVENT_ON_COMMAND_FIND(PanoramaPanel_OnCommandFind)
		EVENT_ON_PARENT_COMMAND(PanoramaPanel_OnParentCommand)
		EVENT_ON_MENU_COMMAND(PanoramaPanel_OnMenuCommand)

		EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	ev.param = (void*)fpath;

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}