/***********************************************************************
	Easily DICMView v1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc _Frame document

	@module	_Frame implement file

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

#include "_Frame.h"
#include "_Module.h"

#include "DicomPanel.h"

#define MAINFRAME_TOOLBAR_HEIGHT	(float)25
#define MAINFRAME_TREEBAR_WIDTH		(float)50
#define MAINFRAME_TITLEBAR_HEIGHT	(float)8
#define MAINFRAME_STATUSBAR_HEIGHT	(float)8
#define MAINFRAME_CATEBAR_HEIGHT	(float)10

#define MAINFRAME_TITLEITEM_WIDTH		(float)15
#define MAINFRAME_STATUSITEM_WIDTH		(float)20

#define MAINFRAME_TREE_OBJECT		_T("object")
#define MAINFRAME_TREE_RESOURCE		_T("resource")

#define IDC_MAINFRAME_TOOLBAR		100
#define IDC_MAINFRAME_TITLEBAR		101
#define IDC_MAINFRAME_RESBAR		102
#define IDC_MAINFRAME_OBJBAR		104
#define IDC_MAINFRAME_STATUSBAR		105
#define IDC_MAINFRAME_CATEBAR		106
#define IDC_MAINFRAME_MENUBOX		110
#define IDC_MAINFRAME_FACEMENU		111

#define IDC_MAINFRAME_IMAGEPANEL	200

#define MAINFRAME_ACCEL_COUNT		4

accel_t	MAINFRAME_ACCEL[MAINFRAME_ACCEL_COUNT] = {
	FVIRTKEY, VK_F10, IDA_PROJECT_OPEN,
	FVIRTKEY | FCONTROL, _T('O'), IDA_FILE_OPEN,
	FVIRTKEY | FCONTROL, _T('S'), IDA_FILE_SAVE,
	FVIRTKEY | FCONTROL, _T('N'), IDA_FILE_CREATE,
};

typedef struct tagMainFrameDelta{	
	res_win_t hToolBar;
	res_win_t hTitleBar;
	res_win_t hResBar;
	res_win_t hObjBar;
	res_win_t hCateBar;
	res_win_t hClientWnd;
	res_win_t hStatusBar;

	res_win_t hToolTip;
	res_win_t hKeyBox;
	res_win_t hNaviBox;
	res_win_t hFindBox;

	bool_t bDirty;
	bool_t bMode;

	tchar_t szPath[PATH_LEN];
}MainFrameDelta;

#define GETMAINFRAMEDELTA(widget) 			(MainFrameDelta*)widget_get_user_delta(widget)
#define SETMAINFRAMEDELTA(widget,ptd)		widget_set_user_delta(widget,(void*)ptd)

res_win_t	_MainFrame_CreatePanel(res_win_t widget, const tchar_t* fpath);
bool_t		_MainFrame_FindPanel(res_win_t widget, const tchar_t* fpath);
/***********************************************************************************************/
void MainFrame_Switch(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	if (!pdt->bDirty)
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("工程文件尚未保存，是否保存？"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_post_command(widget, 0, IDA_PROJECT_SAVE, NULL);
		break;
	case MSGBTN_NO:
		pdt->bDirty = FALSE;
		break;
	}
}

void MainFrame_MergeTool(res_win_t widget, LINKPTR subTool)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTool = toolctrl_fetch(pdt->hToolBar);

	merge_tool_doc(ptrTool, subTool);

	toolctrl_redraw(pdt->hToolBar);
}

void MainFrame_ClearTool(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTool = toolctrl_fetch(pdt->hToolBar);
	LINKPTR nxt, glk = get_tool_next_group(ptrTool, LINK_FIRST);
	while (glk)
	{
		nxt = get_tool_next_group(ptrTool, glk);

		if (compare_text(get_tool_group_name_ptr(glk), -1, MAINFRAME_TOOLGROUP_PROJECT, -1, 0) != 0 && compare_text(get_tool_group_name_ptr(glk), -1, MAINFRAME_TOOLGROUP_FILE, -1, 0) != 0 && compare_text(get_tool_group_name_ptr(glk), -1, MAINFRAME_TOOLGROUP_SETTING, -1, 0) != 0)
			delete_tool_group(glk);

		glk = nxt;
	}

	toolctrl_redraw(pdt->hToolBar);
}

void MainFrame_CloseProject(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	if (pdt->bDirty)
		return;

	xscpy(pdt->szPath, _T(""));

	LINKPTR ptrResTree = treectrl_fetch(pdt->hResBar);
	clear_tree_doc(ptrResTree);
	treectrl_redraw(pdt->hResBar);

	LINKPTR ptrImages = imagesctrl_fetch(pdt->hObjBar);
	clear_image_list(ptrImages);
	imagesctrl_redraw(pdt->hObjBar);

	widget_set_title(widget, _T("DICMView"));
}

typedef struct _IMAGEENUM_PARAM{
	res_win_t stepWidget;
	LINKPTR ptrFiles;
	LINKPTR ptrTree;
	LINKPTR ptrImages;
	const tchar_t* szPath;
	int count;
}IMAGEENUM_PARAM;

static int __callback _image_enum_file(link_t_ptr plk, void* pv)
{
	IMAGEENUM_PARAM* ptp = (IMAGEENUM_PARAM*)pv;
	LINKPTR nlk, tlk, ilk, slk;
	tchar_t* token;
	const tchar_t* nname;
	int len;
	tchar_t sz_dst[PATH_LEN] = { 0 };
	tchar_t sz_src[PATH_LEN] = { 0 };
	tchar_t sz_title[PATH_LEN] = { 0 };

	dicm_summary_t sum = { 0 };
	
	tchar_t title[INT_LEN];

	if (is_list_file_item(plk))
	{
		token = sz_dst + PATH_LEN - 1;
		*token = _T('\0');

		nname = get_list_item_name_ptr(plk);
		len = xslen(nname);
		token -= len;
		xsncpy(token, nname, len);

		nlk = get_dom_parent_node(plk);
		while (nlk && nlk != ptp->ptrFiles)
		{
			token--;
			*token = _T('/');

			nname = get_list_item_name_ptr(nlk);
			len = xslen(nname);
			token -= len;
			memcpy((void*)token, (void*)nname, len * sizeof(tchar_t));

			nlk = get_dom_parent_node(nlk);
		}

		nname = get_list_item_name_ptr(plk);

		xscpy(sz_src, ptp->szPath);
		xscat(sz_src, _T("/"));
		xscat(sz_src, token);

		if(load_dicm_summary_from_file(&sum, NULL, sz_src))
		{
			tlk = get_tree_first_child_item(ptp->ptrTree);
			while (tlk)
			{
				if (compare_text(get_tree_item_name_ptr(tlk), -1, sum.PatientID, -1, 0) == 0)
					break;
				tlk = get_tree_next_sibling_item(tlk);
			}

			if (!tlk)
			{
				tlk = insert_tree_item(ptp->ptrTree, LINK_LAST);
				set_tree_item_name(tlk, sum.PatientID);
				xsprintf(sz_title, _T("%s %s %s"), sum.PatientName, sum.PatientSex, sum.PatientAge);
				set_tree_item_title(tlk, sz_title);
				set_tree_item_image(tlk, BMP_PERSON);
			}

			ilk = get_tree_first_child_item(tlk);
			while (ilk)
			{
				if (compare_text(get_tree_item_name_ptr(ilk), -1, sum.StudyInstanceUID, -1, 0) == 0)
					break;
				ilk = get_tree_next_sibling_item(ilk);
			}

			if (!ilk)
			{
				ilk = insert_tree_item(tlk, LINK_LAST);
				set_tree_item_name(ilk, sum.StudyInstanceUID);
				xsprintf(sz_title, _T("%s %s"), sum.StationName, sum.StudyDate);
				set_tree_item_title(ilk, sz_title);
				set_tree_item_image(ilk, BMP_FILE);
			}

			slk = get_tree_first_child_item(ilk);
			while (slk)
			{
				if (compare_text(get_tree_item_name_ptr(slk), -1, sum.SeriesInstanceUID, -1, 0) == 0)
					break;
				slk = get_tree_next_sibling_item(slk);
			}

			if (!slk)
			{
				slk = insert_tree_item(ilk, LINK_LAST);
				set_tree_item_name(slk, sum.SeriesInstanceUID);
				xsprintf(sz_title, _T("Series %s"), sum.SeriesDate);
				set_tree_item_title(slk, sz_title);
				set_tree_item_image(slk, BMP_FILE);
			}

			nlk = insert_tree_item(slk, LINK_LAST);
			set_tree_item_name(nlk, nname);
			set_tree_item_title(nlk, nname);
			set_tree_item_image(nlk, BMP_IMAGE);
			set_dom_node_attr(nlk, _T("path"), -1, sz_src, -1);

			/*jpg_len = dicm_image_to_bmp_bytes(pdi, NULL, MAX_LONG);
			jpg_buf = (byte_t*)xmem_alloc(jpg_len);
			jpg_len = dicm_image_to_jpg_bytes(pdi, jpg_buf, jpg_len);

			dicm_image_free(pdi);
			
			alk = insert_image_item(ptp->ptrImages, LINK_LAST);
			set_image_item_alt(alk, nname);
			set_dom_node_attr(alk, _T("path"), -1, sz_src, -1);
			set_image_item_bytes(alk, GDI_ATTR_IMAGE_TYPE_JPG, jpg_buf, jpg_len);

			xmem_free(jpg_buf);*/

			ltoxs(++ptp->count, title, INT_LEN);
			statusctrl_step_it(ptp->stepWidget, 1, title);
		}
	}

	return 0;
}

void MainFrame_OpenProject(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	MainFrame_CloseProject(widget);

	if (pdt->bDirty)
		return;

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_pathname(widget, NULL, 0, szFile, PATH_LEN))
		return;

	xscpy(pdt->szPath, szFile);

	statusctrl_show_step(pdt->hStatusBar, 1);

	LINKPTR ptrFiles = create_list_doc();

	xfile_tree(NULL, pdt->szPath, ptrFiles);

	LINKPTR ptrResTree = treectrl_fetch(pdt->hResBar);
	LINKPTR ptrImages = imagesctrl_fetch(pdt->hObjBar);

	IMAGEENUM_PARAM tp = { 0 };
	tp.stepWidget = pdt->hStatusBar;
	tp.ptrFiles = ptrFiles;
	tp.ptrTree = ptrResTree;
	tp.ptrImages = ptrImages;
	tp.szPath = pdt->szPath;
	enum_dom_node(ptrFiles, (PF_LINKENUM)_image_enum_file, (void*)&tp);

	destroy_list_doc(ptrFiles);

	statusctrl_show_step(pdt->hStatusBar, 0);

	treectrl_redraw(pdt->hResBar);

	imagesctrl_redraw(pdt->hObjBar);

	tchar_t token[PATH_LEN + 1] = { 0 };
	xscpy(token, _T("DICMView ["));
	xscat(token, pdt->szPath);
	xscat(token, _T("]"));

	widget_set_title(widget, token);
}

void MainFrame_CreateFile(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	_MainFrame_CreatePanel(widget, NULL);
}

void MainFrame_OpenFile(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szFilter[] = _T("Dicom File(*.dcm)\0*.dcm\0Dicom File(*.*)\0*.*\0");

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, szFilter, _T(""), 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);

	_MainFrame_CreatePanel(widget, szPath);
}

void MainFrame_CloseFile(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
}

void MainFrame_ShowFile(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR tlk = treectrl_get_focus_item(pdt->hResBar);
	if (!tlk)
		return;

	if (is_tree_doc(get_tree_parent_item(tlk)))
		return;

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	xscpy(szFile, get_tree_item_title_ptr(tlk));

	xscpy(szPath, pdt->szPath);
	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);

	if (_MainFrame_FindPanel(widget, szPath))
		return;

	_MainFrame_CreatePanel(widget, szPath);
}

void MainFrame_ChangeFace(res_win_t widget, int ind)
{
	g_indFace = ind;

	clr_mod_t clr;
	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);

	widget_set_color_mode(widget, &clr);

	widget_update_window(widget);
	widget_update_client(widget);
}
/************************************************************************************/
void MainFrame_ToolBar_OnLBClick(res_win_t widget, NOTICE_TOOL* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xrect_t xr = { 0 };
	xpoint_t pt = { 0 };

	if (!pnt->item)
		return;

	long nID = xstol(get_tool_item_id_ptr(pnt->item));

	toolctrl_get_item_rect(pdt->hToolBar, pnt->item, &xr);
	pt.x = xr.x;
	pt.y = xr.y + xr.h;
	widget_client_to_screen(pdt->hToolBar, &pt);

	SendMessage(widget, WM_COMMAND, MAKEWPARAM(nID, 0), (LPARAM)&pt);
}

void MainFrame_ToolBar_OnItemHover(res_win_t widget, NOTICE_TOOL* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xpoint_t xp,*ppt;
	ppt = (xpoint_t*)pnt->data;

	xp.x = ppt->x + 10;
	xp.y = ppt->y + 10;

	widget_client_to_screen(pnt->widget, &xp);

	if (widget_is_valid(pdt->hToolTip))
		reset_toolbox(pdt->hToolTip, &xp, get_tool_item_title_ptr(pnt->item));
	else
		pdt->hToolTip = show_toolbox(&xp, get_tool_item_title_ptr(pnt->item));
}

void MainFrame_TitleBar_OnItemInsert(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
}

void MainFrame_TitleBar_OnItemDelete(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = (res_win_t)get_title_item_delta(pnt->item);

	if (!widget_is_valid(hPanel))
		return;

	widget_close(hPanel, 0);

	if (widget_is_valid(hPanel))
	{
		pnt->ret = 1;
		return;
	}
}

void MainFrame_TitleBar_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = (res_win_t)get_title_item_delta(pnt->item);

	if (widget_is_valid(hPanel))
	{
		widget_show(hPanel, WD_SHOW_HIDE);

		navibox_set_target(pdt->hNaviBox, NULL);
	}
}

void MainFrame_TitleBar_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = (res_win_t)get_title_item_delta(pnt->item);

	if (widget_is_valid(hPanel))
	{
		clr_mod_t clr;
		widget_get_color_mode(widget, &clr);

		widget_set_color_mode(hPanel, &clr);

		widget_show(hPanel, WD_SHOW_NORMAL);

		navibox_set_target(pdt->hNaviBox, hPanel);
	}
}

void MainFrame_TitleBar_OnItemHover(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xpoint_t xp, *ppt;
	ppt = (xpoint_t*)pnt->data;

	xp.x = ppt->x + 10;
	xp.y = ppt->y + 10;

	widget_client_to_screen(pnt->widget, &xp);

	if (widget_is_valid(pdt->hToolTip))
		reset_toolbox(pdt->hToolTip, &xp, get_title_item_title_ptr(pnt->item));
	else
		pdt->hToolTip = show_toolbox(&xp, get_title_item_title_ptr(pnt->item));
}

void MainFrame_ResBar_OnLBClick(res_win_t widget, NOTICE_TREE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

}

void MainFrame_ResBar_OnDBClick(res_win_t widget, NOTICE_TREE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_post_command(widget, 0, IDA_FILE_SHOW, NULL);
}

void MainFrame_ObjBar_OnLBClick(res_win_t widget, NOTICE_IMAGES* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
}

void MainFrame_ObjBar_OnDBClick(res_win_t widget, NOTICE_IMAGES* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
}

/**************************************************************************************************/
void _MainFrame_CalcToolBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	
	widget_get_dock_rect(widget, WD_DOCK_TOP, pxr);
}

void _MainFrame_CalcStatusBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_get_dock_rect(widget, WD_DOCK_BOTTOM, pxr);
}

void _MainFrame_CalcResBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_CATEBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_dock_rect(widget, WD_DOCK_LEFT, pxr);
	pxr->h -= xs.cy;
}

void _MainFrame_CalcObjBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_CATEBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_dock_rect(widget, WD_DOCK_LEFT, pxr);
	pxr->h -= xs.cy;
}

void _MainFrame_CalcCateBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_CATEBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_dock_rect(widget, WD_DOCK_LEFT, pxr);
	pxr->y = pxr->y + pxr->h - xs.cy;
	pxr->h = xs.cy;
}

void _MainFrame_CalcTitleBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_TITLEBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_dock_rect(widget, 0, pxr);
	pxr->h = xs.cy;
}

void _MainFrame_CalcPanelBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_TITLEBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_dock_rect(widget, 0, pxr);
	pxr->y += xs.cy;
	pxr->h -= xs.cy;
}

void _MainFrame_CreateToolBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcToolBar(widget, &xr);

	pdt->hToolBar = toolctrl_create(_T("ToolBar"), WD_STYLE_CONTROL | WD_STYLE_HOTOVER, &xr, widget);
	widget_set_user_id(pdt->hToolBar, IDC_MAINFRAME_TOOLBAR);
	widget_set_owner(pdt->hToolBar, widget);

	LINKPTR ptrTool = create_tool_doc();
	set_tool_images(ptrTool, g_ptrImages);

	LINKPTR glk = insert_tool_group(ptrTool, LINK_LAST);
	set_tool_group_name(glk, MAINFRAME_TOOLGROUP_PROJECT);
	set_tool_group_title(glk, _T("目录"));
	set_tool_group_item_width(glk, MAINFRAME_TOOLGROUP_MAXWIDTH);
	set_tool_group_item_height(glk, MAINFRAME_TOOLGROUP_HEIGHT);

	tchar_t token[NUM_LEN + 1] = { 0 };

	LINKPTR ilk;

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_PROJECT_OPEN);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("打开目录"));
	set_tool_item_image(ilk, BMP_FILE);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_PROJECT_CLOSE);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("关闭目录"));
	set_tool_item_image(ilk, BMP_CLOSE);

	glk = insert_tool_group(ptrTool, LINK_LAST);
	set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);
	set_tool_group_name(glk, MAINFRAME_TOOLGROUP_FILE);
	set_tool_group_title(glk, _T("文件"));
	set_tool_group_item_width(glk, MAINFRAME_TOOLGROUP_MINWIDTH);
	set_tool_group_item_height(glk, MAINFRAME_TOOLGROUP_HEIGHT);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_CREATE);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("新建文件"));
	set_tool_item_image(ilk, BMP_NEW);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_OPEN);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("打开文件"));
	set_tool_item_image(ilk, BMP_OPEN);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_SAVE);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("保存文件"));
	set_tool_item_image(ilk, BMP_SAVE);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_SAVEAS);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("另存文件"));
	set_tool_item_image(ilk, BMP_SAVEAS);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_EXPORT);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("导出数据"));
	set_tool_item_image(ilk, BMP_OUTPUT);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_IMPORT);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("导入数据"));
	set_tool_item_image(ilk, BMP_INPUT);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_PRINT);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("打印文件"));
	set_tool_item_image(ilk, BMP_PRINT);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_FILE_PREVIEW);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("预览文件"));
	set_tool_item_image(ilk, BMP_PREVIEW);

	toolctrl_attach(pdt->hToolBar, ptrTool);
	widget_show(pdt->hToolBar, WD_SHOW_NORMAL);
}

void _MainFrame_CreateTitleBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcTitleBar(widget, &xr);
	
	pdt->hTitleBar = titlectrl_create(_T("TitleBar"), WD_STYLE_CONTROL | WD_STYLE_HOTOVER, &xr, widget);
	widget_set_user_id(pdt->hTitleBar, IDC_MAINFRAME_TITLEBAR);
	widget_set_owner(pdt->hTitleBar, widget);
	titlectrl_auto_focus(pdt->hTitleBar, 1);

	LINKPTR ptrTitle = create_title_doc();
	set_title_images(ptrTitle, g_ptrImages);
	set_title_oritation(ptrTitle, ATTR_ORITATION_BOTTOM);

	titlectrl_attach(pdt->hTitleBar, ptrTitle);

	widget_show(pdt->hTitleBar, WD_SHOW_NORMAL);
}

void _MainFrame_CreateResBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcResBar(widget, &xr);

	pdt->hResBar = treectrl_create(_T("ResBar"), WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_user_id(pdt->hResBar, IDC_MAINFRAME_RESBAR);
	widget_set_owner(pdt->hResBar, widget);

	LINKPTR ptrTree = create_tree_doc();
	set_tree_images(ptrTree, g_ptrImages);
	treectrl_attach(pdt->hResBar, ptrTree);
	treectrl_set_lock(pdt->hResBar, 1);

	//widget_show(pdt->hResBar, WD_SHOW_NORMAL);
}

void _MainFrame_CreateObjBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcObjBar(widget, &xr);

	pdt->hObjBar = imagesctrl_create(_T("ObjBar"), WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_user_id(pdt->hObjBar, IDC_MAINFRAME_OBJBAR);
	widget_set_owner(pdt->hObjBar, widget);

	LINKPTR ptrImages = create_image_list();
	set_image_item_width(ptrImages, MAINFRAME_TREEBAR_WIDTH - 10);
	set_image_item_height(ptrImages, MAINFRAME_TREEBAR_WIDTH - 10);
	set_image_icon_span(ptrImages, 4);
	set_image_list_layer(ptrImages, ATTR_LAYER_VERT);
	imagesctrl_attach(pdt->hObjBar, ptrImages);
	imagesctrl_set_lock(pdt->hObjBar, 1);

	//widget_show(pdt->hObjBar, WD_SHOW_NORMAL);
}

void _MainFrame_CreateCateBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcCateBar(widget, &xr);

	pdt->hCateBar = titlectrl_create(_T("CateBar"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hCateBar, IDC_MAINFRAME_CATEBAR);
	widget_set_owner(pdt->hCateBar, widget);

	LINKPTR ptrTitle = create_title_doc();
	set_title_images(ptrTitle, g_ptrImages);
	set_title_oritation(ptrTitle, ATTR_ORITATION_TOP);
	set_title_item_width(ptrTitle, MAINFRAME_TITLEITEM_WIDTH);

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("文件"));
	set_title_item_name(tlk, MAINFRAME_TREE_RESOURCE);
	set_title_item_image(tlk, BMP_GROUP);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("胶片"));
	set_title_item_name(tlk, MAINFRAME_TREE_OBJECT);
	set_title_item_image(tlk, BMP_MACRO);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hCateBar, ptrTitle);

	titlectrl_set_focus_item(pdt->hCateBar, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_show(pdt->hCateBar, WD_SHOW_NORMAL);
}

void _MainFrame_CreateStatusBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcStatusBar(widget, &xr);

	pdt->hStatusBar = statusctrl_create(_T("StatusBar"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hStatusBar, IDC_MAINFRAME_STATUSBAR);
	widget_set_owner(pdt->hStatusBar, widget);

	LINKPTR ptrStatus = create_status_doc();
	set_status_images(ptrStatus, g_ptrImages);
	set_status_alignment(ptrStatus, ATTR_ALIGNMENT_FAR);

	LINKPTR ilk;

	ilk = insert_status_item(ptrStatus, LINK_LAST);
	set_status_item_name(ilk, _T("findbox"));
	set_status_item_title(ilk, _T("搜索栏"));
	set_status_item_width(ilk, DEF_TOUCH_SPAN * 10);
	set_status_item_image(ilk, BMP_SEARCH);

	ilk = insert_status_item(ptrStatus, LINK_LAST);
	set_status_item_name(ilk, _T("windowbox"));
	set_status_item_title(ilk, _T("窗宽栏"));
	set_status_item_width(ilk, DEF_TOUCH_SPAN * 8);
	set_status_item_image(ilk, BMP_SIZEWIDTH);

	ilk = insert_status_item(ptrStatus, LINK_LAST);
	set_status_item_name(ilk, _T("centerbox"));
	set_status_item_title(ilk, _T("窗位栏"));
	set_status_item_width(ilk, DEF_TOUCH_SPAN * 5);
	set_status_item_image(ilk, BMP_SIZEHEIGHT);

	ilk = insert_status_item(ptrStatus, LINK_LAST);
	set_status_item_name(ilk, _T("navibox"));
	set_status_item_title(ilk, _T("导航栏"));
	set_status_item_width(ilk, DEF_TOUCH_SPAN * 6);

	statusctrl_attach(pdt->hStatusBar, ptrStatus);

	widget_show(pdt->hStatusBar, WD_SHOW_NORMAL);

	ilk = get_status_item(ptrStatus, _T("navibox"));
	statusctrl_get_item_rect(pdt->hStatusBar, ilk, &xr);
	
	pdt->hNaviBox = navibox_create(pdt->hStatusBar, WD_STYLE_CONTROL, &xr);
	widget_set_owner(pdt->hNaviBox, pdt->hStatusBar);
	widget_show(pdt->hNaviBox, WD_SHOW_NORMAL);

	ilk = get_status_item(ptrStatus, _T("findbox"));
	statusctrl_get_item_rect(pdt->hStatusBar, ilk, &xr);
	xr.x += 16;
	xr.w -= 16;
	pt_expand_rect(&xr, -4, -4);

	pdt->hFindBox = editbox_create(pdt->hStatusBar, WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_BORDER | WD_STYLE_EDITOR, &xr);
	widget_set_owner(pdt->hFindBox, pdt->hStatusBar);
	widget_update_window(pdt->hFindBox);
	widget_show(pdt->hFindBox, WD_SHOW_NORMAL);
}

res_win_t _MainFrame_CreatePanel(res_win_t widget, const tchar_t* fpath)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = NULL;
	xrect_t xr;

	link_t_ptr ptr_title, ilk;

	_MainFrame_CalcPanelBar(widget, &xr);

	tchar_t wname[RES_LEN + 1] = { 0 };

	split_file(fpath, NULL, wname);

	if (is_null(wname))
		xscpy(wname, _T("NewDicm"));

	hPanel = DicomPanel_Create(_T("DicomPanel"), WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, fpath);
	if (!hPanel)
		return NULL;

	widget_set_owner(hPanel, widget);

	ptr_title = titlectrl_fetch(pdt->hTitleBar);
	XDL_ASSERT(ptr_title != NULL);

	ilk = insert_title_item(ptr_title, LINK_LAST);

	set_title_item_name(ilk, wname);
	set_title_item_title(ilk, wname);
	set_title_item_delta(ilk, (void*)hPanel);

	titlectrl_redraw(pdt->hTitleBar);
	titlectrl_set_focus_item(pdt->hTitleBar, ilk);

	return hPanel;
}

bool_t _MainFrame_FindPanel(res_win_t widget, const tchar_t* fpath)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	tchar_t wname[RES_LEN + 1] = { 0 };

	split_file(fpath, NULL, wname);

	LINKPTR ptrTitle = titlectrl_fetch(pdt->hTitleBar);

	LINKPTR ilk = get_title_next_item(ptrTitle, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(wname, -1, get_title_item_name_ptr(ilk),-1,0) == 0 && compare_text(wname, -1, get_title_item_title_ptr(ilk), -1, 1) == 0)
		{
			titlectrl_set_focus_item(pdt->hTitleBar, ilk);
			return 1;
		}

		ilk = get_title_next_item(ptrTitle, ilk);
	}

	return 0;
}

void _MainFrame_DestroyToolBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTool = toolctrl_detach(pdt->hToolBar);
	if (ptrTool)
		destroy_tool_doc(ptrTool);
	widget_destroy(pdt->hToolBar);
}

void _MainFrame_DestroyTitleBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTitle = titlectrl_detach(pdt->hTitleBar);
	if (ptrTitle)
		destroy_title_doc(ptrTitle);
	widget_destroy(pdt->hTitleBar);
}

void _MainFrame_DestroyResBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTree = treectrl_detach(pdt->hResBar);
	if (ptrTree)
		destroy_tree_doc(ptrTree);
	widget_destroy(pdt->hResBar);
}

void _MainFrame_DestroyObjBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrImages = imagesctrl_detach(pdt->hObjBar);
	if (ptrImages)
		destroy_image_list(ptrImages);
	widget_destroy(pdt->hObjBar);
}

void _MainFrame_DestroyCateBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrCate = titlectrl_detach(pdt->hCateBar);
	if (ptrCate)
		destroy_title_doc(ptrCate);
	widget_destroy(pdt->hCateBar);
}

void _MainFrame_DestroyStatusBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrStatus = statusctrl_detach(pdt->hStatusBar);
	if (ptrStatus)
		destroy_status_doc(ptrStatus);
	widget_destroy(pdt->hStatusBar);
}


/**********************************************************************************************/
int MainFrame_OnCreate(res_win_t widget, void* data)
{
	MainFrameDelta* pdt;

	widget_hand_create(widget);

	res_acl_t hac = create_accel_table(MAINFRAME_ACCEL, MAINFRAME_ACCEL_COUNT);

	widget_attach_accel(widget, hac);

	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_TOOLBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_TOP, 0, xs.cy);

	xs.fx = 0;
	xs.fy = MAINFRAME_STATUSBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_BOTTOM, 0, xs.cy);

	xs.fx = MAINFRAME_TREEBAR_WIDTH;
	xs.fy = 0;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_LEFT | WD_DOCK_DYNA, xs.cx, 0);

	pdt = (MainFrameDelta*)xmem_alloc(sizeof(MainFrameDelta));
	SETMAINFRAMEDELTA(widget, (void*)pdt);

	res_ctx_t rdc = widget_client_ctx(widget);

	res_bmp_t bmp = load_bitmap_from_images(rdc, g_ptrImages, _T("book"));

	widget_attach_logo(widget, bmp);

	widget_release_ctx(widget, rdc);

	_MainFrame_CreateToolBar(widget);

	_MainFrame_CreateTitleBar(widget);

	_MainFrame_CreateStatusBar(widget);

	_MainFrame_CreateResBar(widget);

	_MainFrame_CreateObjBar(widget);

	_MainFrame_CreateCateBar(widget);

	return 0;
}

void MainFrame_OnDestroy(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_acl_t hac = widget_get_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	res_bmp_t bmp = widget_detach_logo(widget);
	if (bmp)
		delete_bitmap(bmp);

	_MainFrame_DestroyToolBar(widget);

	_MainFrame_DestroyTitleBar(widget);

	_MainFrame_DestroyResBar(widget);

	_MainFrame_DestroyObjBar(widget);

	_MainFrame_DestroyCateBar(widget);

	_MainFrame_DestroyStatusBar(widget);

	if (widget_is_valid(pdt->hToolTip))
		widget_destroy(pdt->hToolTip);

	if (widget_is_valid(pdt->hKeyBox))
		widget_destroy(pdt->hKeyBox);

	xmem_free(pdt);

	widget_hand_destroy(widget);
}

int MainFrame_OnClose(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTitle = titlectrl_fetch(pdt->hTitleBar);
	LINKPTR plk;

	while (plk = titlectrl_get_focus_item(pdt->hTitleBar))
	{
		if (!titlectrl_delete_item(pdt->hTitleBar, plk))
			break;
	}

	if (get_title_item_count(ptrTitle))
		return 1;

	MainFrame_Switch(widget);

	if (pdt->bDirty)
		return 1;

	widget_destroy(widget);

	PostQuitMessage(0);

	return 0;
}

void MainFrame_OnMenuCommand(res_win_t widget, int code, int cid, void* data)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = MainFrame_GetActivePanel(widget);
	if (hPanel && !code)
	{
		if (widget_send_command(hPanel, code, cid, data))
			return;
	}

	switch (cid)
	{
	case IDA_PROJECT_OPEN:
		MainFrame_OpenProject(widget);
		break;
	case IDA_PROJECT_CLOSE:
		MainFrame_CloseProject(widget);
		break;
	case IDA_FILE_SHOW:
		MainFrame_ShowFile(widget);
		break;
	case IDA_FILE_OPEN:
		MainFrame_OpenFile(widget);
		break;
	case IDA_FILE_CREATE:
		MainFrame_CreateFile(widget);
		break;

	case IDC_MAINFRAME_MENUBOX:
		widget_destroy((res_win_t)data);

		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;

	case IDC_MAINFRAME_FACEMENU:
		widget_destroy((res_win_t)data);

		if (code)
		{
			MainFrame_ChangeFace(widget, code - 1);
		}
		break;
	}
}

void MainFrame_OnSysColor(res_win_t widget, const xpoint_t* ppt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_MAINFRAME_FACEMENU);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);

	widget_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();

	for (int i = 0; i < FACE_COUNT; i++)
	{
		LINKPTR mlk = insert_menu_item(ptrMenu, LINK_LAST);
		set_menu_item_iid(mlk, i + 1);
		set_menu_item_title(mlk, g_face[i].title);
		if (i == g_indFace)
			set_menu_item_checked(mlk, 1);
	}

	menubox_set_data(hMenu, ptrMenu);

	xpoint_t xp;
	xp.x = ppt->x;
	xp.y = ppt->y;

	menubox_layout(hMenu, &xp, WD_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void MainFrame_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	if (phdr->id == IDC_MAINFRAME_TOOLBAR)
	{
		NOTICE_TOOL* pnt = (NOTICE_TOOL*)phdr;
		switch (pnt->code)
		{
		case NC_TOOLLBCLK:
			MainFrame_ToolBar_OnLBClick(widget, pnt);
			break;
		case NC_TOOLITEMHOVER:
			MainFrame_ToolBar_OnItemHover(widget, pnt);
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_TITLEBAR)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLEITEMCHANGING:
			MainFrame_TitleBar_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			MainFrame_TitleBar_OnItemChanged(widget, pnt);
			break;
		case NC_TITLEITEMINSERT:
			MainFrame_TitleBar_OnItemInsert(widget, pnt);
			break;
		case NC_TITLEITEMDELETE:
			MainFrame_TitleBar_OnItemDelete(widget, pnt);
			break;
		case NC_TITLEITEMHOVER:
			MainFrame_TitleBar_OnItemHover(widget, pnt);
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_RESBAR)
	{
		NOTICE_TREE* pnt = (NOTICE_TREE*)phdr;
		switch (pnt->code)
		{
		case NC_TREELBCLK:
			MainFrame_ResBar_OnLBClick(widget, pnt);
			break;
		case NC_TREEDBCLK:
			MainFrame_ResBar_OnDBClick(widget, pnt);
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_OBJBAR)
	{
		NOTICE_IMAGES* pnt = (NOTICE_IMAGES*)phdr;
		switch (pnt->code)
		{
		case NC_IMAGESLBCLK:
			MainFrame_ObjBar_OnLBClick(widget, pnt);
			break;
		case NC_IMAGESDBCLK:
			MainFrame_ObjBar_OnDBClick(widget, pnt);
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_CATEBAR)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLEITEMCHANGING:
			if (compare_text(get_title_item_name_ptr(pnt->item), -1, MAINFRAME_TREE_RESOURCE, -1, 0) == 0)
			{
				widget_show(pdt->hResBar, WD_SHOW_HIDE);
			}
			else if (compare_text(get_title_item_name_ptr(pnt->item), -1, MAINFRAME_TREE_OBJECT, -1, 0) == 0)
			{
				widget_show(pdt->hObjBar, WD_SHOW_HIDE);
			}
			break;
		case NC_TITLEITEMCHANGED:
			if(compare_text(get_title_item_name_ptr(pnt->item),-1,MAINFRAME_TREE_RESOURCE,-1,0) == 0)
			{
				widget_show(pdt->hResBar, WD_SHOW_NORMAL);
			}
			else if (compare_text(get_title_item_name_ptr(pnt->item), -1, MAINFRAME_TREE_OBJECT, -1, 0) == 0)
			{
				widget_show(pdt->hObjBar, WD_SHOW_NORMAL);
				widget_post_command(widget, 0, IDA_OBJECT_FRESH, NULL);
			}
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_STATUSBAR)
	{
		NOTICE_STATUS* pnt = (NOTICE_STATUS*)phdr;
		switch (pnt->code)
		{
		case NC_STATUSLBCLK:
			break;
		}
	}
}

void MainFrame_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr;

	_MainFrame_CalcToolBar(widget, &xr);
	widget_move(pdt->hToolBar, RECTPOINT(&xr));
	widget_size(pdt->hToolBar, RECTSIZE(&xr));

	_MainFrame_CalcStatusBar(widget, &xr);
	widget_move(pdt->hStatusBar, RECTPOINT(&xr));
	widget_size(pdt->hStatusBar, RECTSIZE(&xr));

	_MainFrame_CalcResBar(widget, &xr);
	widget_move(pdt->hResBar, RECTPOINT(&xr));
	widget_size(pdt->hResBar, RECTSIZE(&xr));

	_MainFrame_CalcObjBar(widget, &xr);
	widget_move(pdt->hObjBar, RECTPOINT(&xr));
	widget_size(pdt->hObjBar, RECTSIZE(&xr));

	_MainFrame_CalcCateBar(widget, &xr);
	widget_move(pdt->hCateBar, RECTPOINT(&xr));
	widget_size(pdt->hCateBar, RECTSIZE(&xr));

	_MainFrame_CalcTitleBar(widget, &xr);
	widget_move(pdt->hTitleBar, RECTPOINT(&xr));
	widget_size(pdt->hTitleBar, RECTSIZE(&xr));

	_MainFrame_CalcPanelBar(widget, &xr);

	LINKPTR ptrTitle = titlectrl_fetch(pdt->hTitleBar);
	LINKPTR plk = get_title_next_item(ptrTitle, LINK_FIRST);
	while (plk)
	{
		res_win_t hPanel = (res_win_t)get_title_item_delta(plk);

		if (widget_is_valid(hPanel))
		{
			widget_move(hPanel, RECTPOINT(&xr));
			widget_size(hPanel, RECTSIZE(&xr));
		}

		plk = get_title_next_item(ptrTitle, plk);
	}

	widget_invalid(widget, NULL, 0);
}

void MainFrame_OnMove(res_win_t widget, const xpoint_t* ppt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr;

	_MainFrame_CalcToolBar(widget, &xr);
	widget_move(pdt->hToolBar, RECTPOINT(&xr));

	_MainFrame_CalcStatusBar(widget, &xr);
	widget_move(pdt->hStatusBar, RECTPOINT(&xr));

	_MainFrame_CalcResBar(widget, &xr);
	widget_move(pdt->hResBar, RECTPOINT(&xr));

	_MainFrame_CalcObjBar(widget, &xr);
	widget_move(pdt->hObjBar, RECTPOINT(&xr));

	_MainFrame_CalcCateBar(widget, &xr);
	widget_move(pdt->hCateBar, RECTPOINT(&xr));
	
	_MainFrame_CalcTitleBar(widget, &xr);
	widget_move(pdt->hTitleBar, RECTPOINT(&xr));

	_MainFrame_CalcPanelBar(widget, &xr);

	LINKPTR ptrTitle = titlectrl_fetch(pdt->hTitleBar);
	LINKPTR plk = get_title_next_item(ptrTitle, LINK_FIRST);
	while (plk)
	{
		res_win_t hPanel = (res_win_t)get_title_item_delta(plk);

		if (widget_is_valid(hPanel))
		{
			widget_move(hPanel, RECTPOINT(&xr));
		}

		plk = get_title_next_item(ptrTitle, plk);
	}
}

/***********************************************************************************************/
res_win_t MainFrame_Create(const tchar_t* mname)
{
	res_win_t widget;
	xrect_t xr = { 0 };

	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(MainFrame_OnCreate)
		EVENT_ON_DESTROY(MainFrame_OnDestroy)
		EVENT_ON_CLOSE(MainFrame_OnClose)

		EVENT_ON_SIZE(MainFrame_OnSize)
		EVENT_ON_MOVE(MainFrame_OnMove)

		EVENT_ON_NOTICE(MainFrame_OnNotice)
		EVENT_ON_SYSCMD_CLICK(MainFrame_OnSysColor)

		EVENT_ON_MENU_COMMAND(MainFrame_OnMenuCommand)

		EVENT_ON_NC_IMPLEMENT
		EVENT_ON_DOCKER_IMPLEMENT

	SUBPROC_END_DISPATH

	get_desktop_size(RECTSIZE(&xr));

	widget = widget_create(_T("DICMView"), WD_STYLE_FRAME | WD_STYLE_DOCKER | WD_STYLE_OWNERNC, &xr, NULL, &ev);

	if (!widget)
	{
		return 0;
	}

	clr_mod_t clr;
	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);

	widget_set_color_mode(widget, &clr);

	widget_show(widget, WD_SHOW_NORMAL);
	widget_update_window(widget);
	widget_update_client(widget);

	return widget;
}

res_win_t MainFrame_GetActivePanel(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	LINKPTR nlk = titlectrl_get_focus_item(pdt->hTitleBar);
	if (!nlk)
		return NULL;

	return (res_win_t)get_title_item_delta(nlk);
}

void MainFrame_UpdatePanel(res_win_t widget, res_win_t hPanel, const tchar_t* sz_title)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	LINKPTR ptrTitle = titlectrl_fetch(pdt->hTitleBar);
	LINKPTR plk = get_title_next_item(ptrTitle, LINK_FIRST);
	while (plk)
	{
		if ((res_win_t)get_title_item_delta(plk) == hPanel)
		{
			set_title_title(plk, sz_title);
			titlectrl_redraw_item(pdt->hTitleBar, plk);
			break;
		}

		plk = get_title_next_item(ptrTitle, plk);
	}
}

void MainFrame_UpdateWindow(res_win_t widget, const tchar_t* szWindow)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrStatus = statusctrl_fetch(pdt->hStatusBar);

	LINKPTR ilk = get_status_item(ptrStatus, _T("windowbox"));
	set_status_item_title(ilk, szWindow);
	statusctrl_redraw_item(pdt->hStatusBar, ilk);
}

void MainFrame_UpdateCenter(res_win_t widget, const tchar_t* szCenter)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrStatus = statusctrl_fetch(pdt->hStatusBar);

	LINKPTR ilk = get_status_item(ptrStatus, _T("centerbox"));
	set_status_item_title(ilk, szCenter);
	statusctrl_redraw_item(pdt->hStatusBar, ilk);
}