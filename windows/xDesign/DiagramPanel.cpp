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
#include "DiagramPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_DIAGRAMPANEL_DIAGRAM			201
#define IDC_DIAGRAMPANEL_PROPER			202
#define IDC_DIAGRAMPANEL_TITLE			203
#define IDC_DIAGRAMPANEL_MENU			204
#define IDC_DIAGRAMPANEL_FONTNAME		210
#define IDC_DIAGRAMPANEL_FONTSIZE		211
#define IDC_DIAGRAMPANEL_FONTCOLOR		212
#define IDC_DIAGRAMPANEL_PAINTCOLOR		213
#define IDC_DIAGRAMPANEL_DRAWCOLOR		214

#define DIAGRAMPANEL_GROUPITEM_WIDTH		(float)8
#define DIAGRAMPANEL_GROUPITEM_HEIGHT		(float)7
#define DIAGRAMPANEL_GROUPTEXT_WIDTH		(float)15
#define DIAGRAMPANEL_TITLEITEM_WIDTH		(float)15
#define DIAGRAMPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagDiagramPanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hDiagram;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}DiagramPanelDelta;

#define GETDIAGRAMPANELDELTA(ph) 		(DiagramPanelDelta*)widget_get_user_delta(ph)
#define SETDIAGRAMPANELDELTA(ph,ptd)		widget_set_user_delta(ph,(var_long)ptd)

#define DIAGRAMPANEL_ACCEL_COUNT	5
accel_t	DIAGRAMPANEL_ACCEL[DIAGRAMPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY,			VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

/***************************************************************************************/
void DiagramPanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	diagramctrl_set_dirty(pdt->hDiagram, bDirty);
}

bool_t DiagramPanel_GetDirty(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	return diagramctrl_get_dirty(pdt->hDiagram);
}

void DiagramPanel_Switch(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	if (!DiagramPanel_GetDirty(widget))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("文件尚未保存，是否保存文件？"));
	
	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		DiagramPanel_SetDirty(widget, 0);
		break;
	}
}

LINKPTR DiagramPanel_GetDocument(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	return diagramctrl_fetch(pdt->hDiagram);
}

bool_t DiagramPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR newDiagram = create_diagram_from_meta_file(NULL, szFile);
	if (!newDiagram)
	{
		ShowMsg(MSGICO_ERR, _T("导入表单失败！"));

		return 0;
	}

	LINKPTR orgDiagram = diagramctrl_detach(pdt->hDiagram);
	destroy_diagram_doc(orgDiagram);

	diagramctrl_attach(pdt->hDiagram, newDiagram);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t DiagramPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrDiagram);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrDiagram = detach_meta_body_node(ptrMeta);
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

void DiagramPanel_SelectAttr(res_win_t widget, const tchar_t* attr_name, const tchar_t* attr_val)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	tchar_t style[CSS_LEN];

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (flk == ptrItem || get_diagram_entity_selected(flk))
		{
			write_style_attr(get_diagram_entity_style_ptr(flk), -1, attr_name, -1, attr_val, -1, style, CSS_LEN);
			set_diagram_entity_style(flk, style);
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	diagramctrl_redraw(pdt->hDiagram);
}


/*********************************************************************************************************/
void DiagramPanel_OnSave(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("diagram meta file(*.diagram)\0*.diagram\0"), _T("diagram"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	if (DiagramPanel_SaveFile(widget, szFile))
	{
		DiagramPanel_SetDirty(widget, 0);
	}
}

void DiagramPanel_OnSaveAs(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Diagram file(*.diagram)\0*.diagram\0"), _T("diagram"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrDiagram);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrDiagram = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void DiagramPanel_OnPrint(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	//print_diagram(NULL, ptrDiagram);
}

void DiagramPanel_OnPreview(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	res_win_t hPreviewDlg = previewdlg_create(_T("打印预览"), g_hMain);

	LINKPTR svg = create_svg_doc();

	svg_print_diagram(svg, ptrDiagram);

	LINKPTR ptr_arch = previewdlg_get_arch(hPreviewDlg);

	LINKPTR ilk = insert_arch_document(ptr_arch, LINK_LAST, svg);

	previewdlg_redraw(hPreviewDlg);

	widget_show(hPreviewDlg, WS_SHOW_FULLSCREEN);
}

void DiagramPanel_OnExec(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptr_dlg = diagramctrl_fetch(pdt->hDiagram);;

	res_win_t dlg = create_dialog(ptr_dlg, widget);

	clr_mod_t clr = { 0 };

	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);
	parse_xcolor(&clr.clr_ico, g_face[g_indFace].ico);

	widget_set_color_mode(dlg, &clr);

	widget_show(dlg, WS_SHOW_NORMAL);

	widget_do_modal(dlg);
}

void DiagramPanel_OnSelectAll(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	XDL_ASSERT(ptrDiagram);

	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		set_diagram_entity_selected(flk, 1);
	
		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnDelete(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	XDL_ASSERT(ptrDiagram);

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	diagramctrl_set_focus_entity(pdt->hDiagram, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		nlk = get_diagram_next_entity(ptrDiagram, flk);
		if (get_diagram_entity_selected(flk) || ptrItem == flk)
		{
			delete_diagram_entity(flk);
			flk = nlk;

			bRedraw = 1;
			continue;
		}

		flk = nlk;
	}

	if (bRedraw)
		diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnCopy(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	widget_post_command(pdt->hDiagram, COMMAND_COPY, IDC_EDITMENU, 0);
}

void DiagramPanel_OnCut(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	widget_post_command(pdt->hDiagram, COMMAND_CUT, IDC_EDITMENU, 0);
}

void DiagramPanel_OnPaste(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	widget_post_command(pdt->hDiagram, COMMAND_PASTE, IDC_EDITMENU, 0);
}


void DiagramPanel_OnRedo(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

}

void DiagramPanel_OnUndo(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	widget_post_command(pdt->hDiagram, COMMAND_UNDO, IDC_EDITMENU, 0);
}

void DiagramPanel_OnCSSProper(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrProper = create_proper_doc();

	LINKPTR ptr = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR flk = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (flk)
		properbag_parse_stylesheet(ptrProper, get_diagram_entity_style_ptr(flk));
	else
		properbag_parse_stylesheet(ptrProper, get_diagram_style_ptr(ptr));

	res_win_t hProperDlg = properdlg_create(_T("绘制样式"), ptrProper, g_hMain);

	widget_show(hProperDlg, WS_SHOW_NORMAL);

	int nRet = widget_do_modal(hProperDlg);

	if (nRet)
	{
		tchar_t sz_style[CSS_LEN] = { 0 };
		properbag_format_stylesheet(ptrProper, sz_style, CSS_LEN);

		if (flk)
		{
			set_diagram_entity_style(flk, sz_style);
			diagramctrl_redraw_entity(pdt->hDiagram, flk);
		}
		else
		{
			set_diagram_style(ptr, sz_style);
			diagramctrl_redraw(pdt->hDiagram);
		}

		DiagramPanel_SetDirty(widget, 1);
	}

	destroy_proper_doc(ptrProper);
}

void DiagramPanel_OnFontName(res_win_t widget, void* pv)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	fontname_menu(widget, IDC_DIAGRAMPANEL_FONTNAME, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DiagramPanel_OnFontSize(res_win_t widget, void* pv)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	fontsize_menu(widget, IDC_DIAGRAMPANEL_FONTSIZE, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DiagramPanel_OnTextColor(res_win_t widget, void* pv)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	color_menu(widget, IDC_DIAGRAMPANEL_FONTCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DiagramPanel_OnPaintColor(res_win_t widget, void* pv)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	color_menu(widget, IDC_DIAGRAMPANEL_PAINTCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DiagramPanel_OnDrawColor(res_win_t widget, void* pv)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	color_menu(widget, IDC_DIAGRAMPANEL_DRAWCOLOR, (xpoint_t*)pv, WS_LAYOUT_RIGHTBOTTOM);
}

void DiagramPanel_OnTextNear(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_NEAR);
}

void DiagramPanel_OnTextBold(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SelectAttr(widget, GDI_ATTR_FONT_WEIGHT, GDI_ATTR_FONT_WEIGHT_BOLD);
}

void DiagramPanel_OnTextCenter(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_CENTER);
}

void DiagramPanel_OnTextFar(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	DiagramPanel_SelectAttr(widget, GDI_ATTR_TEXT_ALIGN, GDI_ATTR_TEXT_ALIGN_FAR);
}

void DiagramPanel_OnAlignNear(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (!ptrItem)
		return;

	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_diagram_entity_selected(flk))
		{
			set_diagram_entity_x(flk, get_diagram_entity_x(ptrItem));
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnAlignCenter(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (!ptrItem)
		return;

	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_diagram_entity_selected(flk))
		{
			set_diagram_entity_x(flk, get_diagram_entity_x(ptrItem) + get_diagram_entity_width(ptrItem) / 2 - get_diagram_entity_width(flk) / 2);
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnAlignFar(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (!ptrItem)
		return;

	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_diagram_entity_selected(flk))
		{
			set_diagram_entity_x(flk, get_diagram_entity_x(ptrItem) + get_diagram_entity_width(ptrItem) - get_diagram_entity_width(flk));
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnSizeHeight(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (!ptrItem)
		return;

	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_diagram_entity_selected(flk))
		{
			set_diagram_entity_height(flk, get_diagram_entity_height(ptrItem));
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnSizeWidth(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (!ptrItem)
		return;

	if (!get_diagram_entity_selected_count(ptrDiagram))
		return;

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (flk != ptrItem && get_diagram_entity_selected(flk))
		{
			set_diagram_entity_width(flk, get_diagram_entity_width(ptrItem));
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnSizeHorz(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	int count = get_diagram_entity_selected_count(ptrDiagram);
	if (count <= 1)
		return;

	DiagramPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min,f_max,f_total = 0;
	count = 0;
	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (get_diagram_entity_selected(flk))
		{
			pxs[count].fact = (int)(get_diagram_entity_x(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_diagram_entity_width(flk);
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_diagram_entity_x((link_t_ptr)pxs[0].data);
	f_max = get_diagram_entity_x((link_t_ptr)pxs[count - 1].data) + get_diagram_entity_width((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_diagram_entity_x((link_t_ptr)pxs[0].data) + get_diagram_entity_width((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_diagram_entity_x((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_diagram_entity_x((link_t_ptr)pxs[i].data) + get_diagram_entity_width((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnSizeVert(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	int count = get_diagram_entity_selected_count(ptrDiagram);
	if (count <= 1)
		return;

	DiagramPanel_SetDirty(widget, 1);

	xsort_t* pxs = (xsort_t*)xmem_alloc(sizeof(xsort_t) * count);

	float f_min, f_max, f_total = 0;
	count = 0;
	LINKPTR flk = get_diagram_next_entity(ptrDiagram, LINK_FIRST);
	while (flk)
	{
		if (get_diagram_entity_selected(flk))
		{
			pxs[count].fact = (int)(get_diagram_entity_y(flk) * 10);
			pxs[count].data = (void*)flk;
			count++;

			f_total += get_diagram_entity_height(flk);
		}

		flk = get_diagram_next_entity(ptrDiagram, flk);
	}

	bubble_xsort(pxs, count);

	f_min = get_diagram_entity_y((link_t_ptr)pxs[0].data);
	f_max = get_diagram_entity_y((link_t_ptr)pxs[count - 1].data) + get_diagram_entity_height((link_t_ptr)pxs[count - 1].data);

	float f_avg = (f_max - f_min - f_total) / (count - 1);
	if (f_avg < 0)
		f_avg = 0;

	f_min = get_diagram_entity_y((link_t_ptr)pxs[0].data) + get_diagram_entity_height((link_t_ptr)pxs[0].data);
	for (int i = 1; i < count; i++)
	{
		set_diagram_entity_y((link_t_ptr)pxs[i].data, f_min + f_avg);
		f_min = get_diagram_entity_y((link_t_ptr)pxs[i].data) + get_diagram_entity_height((link_t_ptr)pxs[i].data);
	}

	xmem_free(pxs);

	diagramctrl_redraw(pdt->hDiagram);
}

void DiagramPanel_OnSendBack(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (!ptrItem)
		return;

	DiagramPanel_SetDirty(widget, 1);

	set_diagram_entity_taborder(ptrItem, 0);

	LINKPTR root = get_dom_child_node_root(ptrDiagram);

	switch_link_first(root, ptrItem);

	diagramctrl_redraw(pdt->hDiagram);
}


void DiagramPanel_OnProcess(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_diagram_entity(ptrDiagram, DOC_DIAGRAM_PROCESS);

	set_diagram_entity_x(flk, 1);
	set_diagram_entity_y(flk, 1);
	set_diagram_entity_width(flk, DEF_TEXT_WIDTH);
	set_diagram_entity_height(flk, DEF_TEXT_HEIGHT);

	int count = get_diagram_entity_count_by_class(ptrDiagram, DOC_DIAGRAM_PROCESS);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIAGRAM_PROCESS, count);

	set_diagram_entity_name(flk, token);

	diagramctrl_redraw_entity(pdt->hDiagram,flk);
}

void DiagramPanel_OnDocument(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_diagram_entity(ptrDiagram, DOC_DIAGRAM_DOCUMENT);

	set_diagram_entity_x(flk, 1);
	set_diagram_entity_y(flk, 1);
	set_diagram_entity_width(flk, DEF_TEXT_WIDTH);
	set_diagram_entity_height(flk, DEF_TEXT_HEIGHT);

	int count = get_diagram_entity_count_by_class(ptrDiagram, DOC_DIAGRAM_DOCUMENT);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIAGRAM_DOCUMENT, count);

	set_diagram_entity_name(flk, token);

	diagramctrl_redraw_entity(pdt->hDiagram,flk);
}

void DiagramPanel_OnSheet(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_diagram_entity(ptrDiagram, DOC_DIAGRAM_SHEET);

	set_diagram_entity_x(flk, 1);
	set_diagram_entity_y(flk, 1);
	set_diagram_entity_width(flk, DEF_TEXT_WIDTH);
	set_diagram_entity_height(flk, DEF_TEXT_HEIGHT);

	int count = get_diagram_entity_count_by_class(ptrDiagram, DOC_DIAGRAM_SHEET);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIAGRAM_DOCUMENT, count);

	set_diagram_entity_name(flk, token);

	diagramctrl_redraw_entity(pdt->hDiagram,flk);
}

void DiagramPanel_OnJudge(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_diagram_entity(ptrDiagram, DOC_DIAGRAM_JUDGE);

	set_diagram_entity_x(flk, 1);
	set_diagram_entity_y(flk, 1);
	set_diagram_entity_width(flk, DEF_TEXT_WIDTH);
	set_diagram_entity_height(flk, DEF_TEXT_HEIGHT);

	int count = get_diagram_entity_count_by_class(ptrDiagram, DOC_DIAGRAM_JUDGE);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIAGRAM_JUDGE, count);

	set_diagram_entity_name(flk, token);

	diagramctrl_redraw_entity(pdt->hDiagram, flk);
}

void DiagramPanel_OnJoint(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);

	DiagramPanel_SetDirty(widget, 1);

	LINKPTR flk = insert_diagram_entity(ptrDiagram, DOC_DIAGRAM_JOINT);

	set_diagram_entity_x(flk, 1);
	set_diagram_entity_y(flk, 1);
	set_diagram_entity_width(flk, DEF_TEXT_WIDTH);
	set_diagram_entity_height(flk, DEF_TEXT_HEIGHT);

	int count = get_diagram_entity_count_by_class(ptrDiagram, DOC_DIAGRAM_JOINT);

	tchar_t token[RES_LEN + 1];
	xsprintf(token, _T("%s%d"), DOC_DIAGRAM_JOINT, count);

	set_diagram_entity_name(flk, token);

	diagramctrl_redraw_entity(pdt->hDiagram, flk);
}

void DiagramPanel_OnAttributes(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (ptrItem)
		properbag_write_diagram_entity_attributes(ptrProper, ptrItem);
	else
		properbag_write_diagram_attributes(ptrProper, ptrDiagram);

	properctrl_redraw(pdt->hProper);
}

void DiagramPanel_OnStyleSheet(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	if (ptrItem)
		properbag_parse_stylesheet(ptrProper, get_diagram_entity_style_ptr(ptrItem));
	else
		properbag_parse_stylesheet(ptrProper, get_diagram_style_ptr(ptrDiagram));

	properctrl_redraw(pdt->hProper);
}

void DiagramPanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ilk = titlectrl_get_focus_item(pdt->hTitle);
	if (!ilk)
		return;

	int n_id = xstol(get_title_item_id_ptr(ilk));

	LINKPTR ptrDiagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR ptrItem = diagramctrl_get_focus_entity(pdt->hDiagram);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrItem)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_diagram_entity_attributes(pnp->proper, ptrItem);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_diagram_entity_style(ptrItem, sz_style);
		}
		diagramctrl_redraw_entity(pdt->hDiagram, ptrItem);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_diagram_attributes(pnp->proper, ptrDiagram);
		}
		else if (n_id == IDA_STYLESHEET)
		{
			properbag_format_stylesheet(pnp->proper, sz_style, CSS_LEN);
			set_diagram_style(ptrDiagram, sz_style);
		}
		diagramctrl_redraw(pdt->hDiagram);
	}
}

void DiagramPanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void DiagramPanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	int n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void DiagramPanel_Diagram_OnRBClick(res_win_t widget, NOTICE_DIAGRAM* pnf)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	if (!pnf->entity)
		return;

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_DIAGRAMPANEL_MENU);
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

	widget_client_to_screen(pdt->hDiagram, &xp);

	menubox_layout(hMenu, &xp, WS_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void DiagramPanel_Diagram_OnLBClick(res_win_t widget, NOTICE_DIAGRAM* pnf)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void DiagramPanel_Diagram_OnItemSize(res_win_t widget, NOTICE_DIAGRAM* pnf)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void DiagramPanel_Diagram_OnItemMove(res_win_t widget, NOTICE_DIAGRAM* pnf)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}


/***********************************************************************************************************/
int DiagramPanel_OnCreate(res_win_t widget, void* data)
{
	DiagramPanelDelta* pdt = (DiagramPanelDelta*)xmem_alloc(sizeof(DiagramPanelDelta));
	xrect_t xr;
	const tchar_t* szParam;

	widget_hand_create(widget);

	SETDIAGRAMPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(DIAGRAMPANEL_ACCEL, DIAGRAMPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	szParam = (tchar_t*)data;

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkDiagram = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -DIAGRAMPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);
	set_split_item_fixed(ilkRight, 1);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hDiagram = diagramctrl_create(_T("DiagramPanel"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hDiagram, IDC_DIAGRAMPANEL_DIAGRAM);
	widget_set_owner(pdt->hDiagram, widget);

	set_split_item_delta(ilkDiagram, pdt->hDiagram);
	widget_show(pdt->hDiagram, WS_SHOW_NORMAL);

	LINKPTR ptrDiagram = create_diagram_doc();
	diagramctrl_attach(pdt->hDiagram, ptrDiagram);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("DiagramProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_DIAGRAMPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("DiagramTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_DIAGRAMPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WS_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, DIAGRAMPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ICON_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("样式"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, DIAGRAMPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ICON_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);

	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	if (!is_null(szParam))
	{
		if (!DiagramPanel_OpenFile(widget, szParam))
			return -1;

		diagramctrl_redraw(pdt->hDiagram);
	}

	return 0;
}

void DiagramPanel_OnDestroy(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hDiagram))
	{
		LINKPTR ptrDiagram = diagramctrl_detach(pdt->hDiagram);
		if (ptrDiagram)
			destroy_diagram_doc(ptrDiagram);

		widget_destroy(pdt->hDiagram);
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

void DiagramPanel_OnShow(res_win_t widget, bool_t bShow)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, DIAGRAMPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, DIAGRAMPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_SELECTALL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("全选"));
		set_tool_item_icon(ilk, GDI_ICON_SELECTALL);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_DELETE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("删除"));
		set_tool_item_icon(ilk, GDI_ICON_DELETE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_COPY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("拷贝"));
		set_tool_item_icon(ilk, GDI_ICON_COPY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_CUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("剪切"));
		set_tool_item_icon(ilk, GDI_ICON_CUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_PASTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("粘贴"));
		set_tool_item_icon(ilk, GDI_ICON_PASTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_UNDO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("撤销"));
		set_tool_item_icon(ilk, GDI_ICON_UNDO);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_STYLE);
		set_tool_group_title(glk, _T("样式"));
		set_tool_group_item_width(glk, DIAGRAMPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, DIAGRAMPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_NAME);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体名称"));
		set_tool_item_icon(ilk, GDI_ICON_FONTNAME);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_SIZE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字号大小"));
		set_tool_item_icon(ilk, GDI_ICON_FONTSIZE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_FONT_WEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体加黑"));
		set_tool_item_icon(ilk, GDI_ICON_FONTWEIGHT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("字体颜色"));
		set_tool_item_icon(ilk, GDI_ICON_FONTCOLOR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PAINT_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("背景颜色"));
		set_tool_item_icon(ilk, GDI_ICON_PRINT);

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_DRAW_COLOR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("前景"));
		set_tool_item_icon(ilk, GDI_ICON_STYLE);*/

		/*ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_PROPER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("CSS属性"));
		set_tool_item_icon(ilk, GDI_ICON_PROPER);*/

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ALIGNNEAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ALIGNCENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_TEXT_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ALIGNFAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_NEAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("左对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ARRANGELEFT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_CENTER);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("居中对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ARRANGECENTER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_ALIGN_FAR);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("右对齐"));
		set_tool_item_icon(ilk, GDI_ICON_ARRANGERIGHT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_WIDTH);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等宽"));
		set_tool_item_icon(ilk, GDI_ICON_SIZEHORZ);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HEIGHT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("等高"));
		set_tool_item_icon(ilk, GDI_ICON_SIZEVERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_HORZ);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("水平等距"));
		set_tool_item_icon(ilk, GDI_ICON_SPACEHORZ);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SIZE_VERT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("垂直等距"));
		set_tool_item_icon(ilk, GDI_ICON_SPACEVERT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_STYLE_SENDBACK);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("置于后端"));
		set_tool_item_icon(ilk, GDI_ICON_ORDER);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_ELEMENT);
		set_tool_group_title(glk, _T("部件"));
		set_tool_group_item_width(glk, DIAGRAMPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, DIAGRAMPANEL_GROUPITEM_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_PROCESS);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("过程"));
		set_tool_item_icon(ilk, GDI_ICON_PROCESS);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_JUDGE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("判断"));
		set_tool_item_icon(ilk, GDI_ICON_JUDGE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_DOCUMENT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("文档"));
		set_tool_item_icon(ilk, GDI_ICON_RICH);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_SHEET);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("表单"));
		set_tool_item_icon(ilk, GDI_ICON_DOC);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_APPEND_JOINT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("连接"));
		set_tool_item_icon(ilk, GDI_ICON_DIAGRAM);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

int DiagramPanel_OnClose(res_win_t widget)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	DiagramPanel_Switch(widget);

	return (diagramctrl_get_dirty(pdt->hDiagram)) ? 1 : 0;
}

void DiagramPanel_OnCommandFind(res_win_t widget, str_find_t* pfd)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	LINKPTR ptr_diagram = diagramctrl_fetch(pdt->hDiagram);
	LINKPTR flk = get_diagram_next_entity(ptr_diagram, LINK_FIRST);
	while (flk)
	{
		if (compare_text(pfd->sub_str, -1, get_diagram_entity_name_ptr(flk), -1, 1) == 0)
		{
			diagramctrl_set_focus_entity(pdt->hDiagram, flk);
			break;
		}
		flk = get_diagram_next_entity(ptr_diagram, flk);
	}
}

void DiagramPanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	if (code == COMMAND_QUERYINFO)
	{
		QUERYOBJECT* pqo = (QUERYOBJECT*)data;
		xscpy(pqo->szDoc, DOC_DIAGRAM);
		pqo->ptrDoc = diagramctrl_fetch(pdt->hDiagram);
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
		diagramctrl_set_dirty(pdt->hDiagram, 0);
	}
}

void DiagramPanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	tchar_t token[RES_LEN];

	switch (cid)
	{
	case IDA_FILE_SAVE:
		DiagramPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		DiagramPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		DiagramPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		DiagramPanel_OnPreview(widget);
		break;
	case IDA_FILE_EXEC:
		DiagramPanel_OnExec(widget);
		break;
	case IDA_EDIT_SELECTALL:
		DiagramPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		DiagramPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		DiagramPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		DiagramPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		DiagramPanel_OnPaste(widget);
		break;
	case IDA_EDIT_UNDO:
		DiagramPanel_OnUndo(widget);
		break;
	case IDA_STYLE_PROPER:
		DiagramPanel_OnCSSProper(widget);
		break;
	case IDA_STYLE_FONT_NAME:
		DiagramPanel_OnFontName(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_SIZE:
		DiagramPanel_OnFontSize(widget, (void*)data);
		break;
	case IDA_STYLE_FONT_WEIGHT:
		DiagramPanel_OnTextBold(widget);
		break;
	case IDA_STYLE_TEXT_COLOR:
		DiagramPanel_OnTextColor(widget, (void*)data);
		break;
	case IDA_STYLE_PAINT_COLOR:
		DiagramPanel_OnPaintColor(widget, (void*)data);
		break;
	case IDA_STYLE_DRAW_COLOR:
		DiagramPanel_OnDrawColor(widget, (void*)data);
		break;
	case IDA_STYLE_TEXT_NEAR:
		DiagramPanel_OnTextNear(widget);
		break;
	case IDA_STYLE_TEXT_CENTER:
		DiagramPanel_OnTextCenter(widget);
		break;
	case IDA_STYLE_TEXT_FAR:
		DiagramPanel_OnTextFar(widget);
		break;
	case IDA_STYLE_ALIGN_NEAR:
		DiagramPanel_OnAlignNear(widget);
		break;
	case IDA_STYLE_ALIGN_CENTER:
		DiagramPanel_OnAlignCenter(widget);
		break;
	case IDA_STYLE_ALIGN_FAR:
		DiagramPanel_OnAlignFar(widget);
		break;
	case IDA_STYLE_SIZE_HORZ:
		DiagramPanel_OnSizeHorz(widget);
		break;
	case IDA_STYLE_SIZE_VERT:
		DiagramPanel_OnSizeVert(widget);
		break;
	case IDA_STYLE_SIZE_WIDTH:
		DiagramPanel_OnSizeWidth(widget);
		break;
	case IDA_STYLE_SIZE_HEIGHT:
		DiagramPanel_OnSizeHeight(widget);
		break;
	case IDA_STYLE_SENDBACK:
		DiagramPanel_OnSendBack(widget);
		break;

	case IDA_APPEND_JUDGE:
		DiagramPanel_OnJudge(widget);
		break;
	case IDA_APPEND_PROCESS:
		DiagramPanel_OnProcess(widget);
		break;
	case IDA_APPEND_DOCUMENT:
		DiagramPanel_OnDocument(widget);
		break;
	case IDA_APPEND_SHEET:
		DiagramPanel_OnSheet(widget);
		break;
	case IDA_APPEND_JOINT:
		DiagramPanel_OnJoint(widget);
		break;

	case IDA_ATTRIBUTES:
		DiagramPanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		DiagramPanel_OnStyleSheet(widget);
		break;

	case IDC_DIAGRAMPANEL_FONTNAME:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontname_menu_item(code, token, RES_LEN);
			DiagramPanel_SelectAttr(widget, GDI_ATTR_FONT_FAMILY, token);
		}
		break;
	case IDC_DIAGRAMPANEL_FONTSIZE:
		widget_destroy((res_win_t)data);
		if (code)
		{
			fontsize_menu_item(code, token, RES_LEN);
			DiagramPanel_SelectAttr(widget, GDI_ATTR_FONT_SIZE, token);
		}
		break;
	case IDC_DIAGRAMPANEL_FONTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			DiagramPanel_SelectAttr(widget, GDI_ATTR_FONT_COLOR, token);
		}
		break;
	case IDC_DIAGRAMPANEL_PAINTCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			DiagramPanel_SelectAttr(widget, GDI_ATTR_FILL_COLOR, token);
		}
		break;
	case IDC_DIAGRAMPANEL_DRAWCOLOR:
		widget_destroy((res_win_t)data);
		if (code)
		{
			color_menu_item(code, token, RES_LEN);
			DiagramPanel_SelectAttr(widget, GDI_ATTR_STROKE_COLOR, token);
		}
		break;
	}
}

void DiagramPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	DiagramPanelDelta* pdt = GETDIAGRAMPANELDELTA(widget);

	if (phdr->id == IDC_DIAGRAMPANEL_DIAGRAM)
	{
		NOTICE_DIAGRAM* pnf = (NOTICE_DIAGRAM*)phdr;
		switch (pnf->code)
		{
		case NC_DIAGRAMCALCED:
			break;
		case NC_FIELDCALCED:
			break;
		case NC_DIAGRAMLBCLK:
			DiagramPanel_Diagram_OnLBClick(widget, pnf);
			break;
		case NC_DIAGRAMRBCLK:
			DiagramPanel_Diagram_OnRBClick(widget, pnf);
			break;
		case NC_DIAGRAMENTITYSIZED:
			DiagramPanel_Diagram_OnItemSize(widget, pnf);
			break;
		case NC_DIAGRAMENTITYDROP:
			DiagramPanel_Diagram_OnItemMove(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_DIAGRAMPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			DiagramPanel_SetDirty(widget, 1);
			break;
		case NC_ENTITYUPDATE:
			DiagramPanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_DIAGRAMPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			DiagramPanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			DiagramPanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/**************************************************************************************************************/
res_win_t DiagramPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(DiagramPanel_OnCreate)
		EVENT_ON_DESTROY(DiagramPanel_OnDestroy)
		EVENT_ON_CLOSE(DiagramPanel_OnClose)
		EVENT_ON_SHOW(DiagramPanel_OnShow)

		EVENT_ON_NOTICE(DiagramPanel_OnNotice)
		EVENT_ON_COMMAND_FIND(DiagramPanel_OnCommandFind)
		EVENT_ON_PARENT_COMMAND(DiagramPanel_OnParentCommand)
		EVENT_ON_MENU_COMMAND(DiagramPanel_OnMenuCommand)

		EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	ev.param = (void*)fpath;

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}