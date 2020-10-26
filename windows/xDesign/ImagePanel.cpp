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
#include "ImagePanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_IMAGEPANEL_IMAGE		201
#define IDC_IMAGEPANEL_PROPER		202
#define IDC_IMAGEPANEL_TITLE		203
#define IDC_IMAGEPANEL_MENU			204

#define IMAGEPANEL_GROUPITEM_WIDTH		(float)8
#define IMAGEPANEL_GROUPITEM_HEIGHT		(float)7
#define IMAGEPANEL_TITLEITEM_WIDTH		(float)15
#define IMAGEPANEL_TITLEITEM_HEIGHT		(float)10

typedef struct tagImagePanelDelta{
	res_win_t hProper;
	res_win_t hTitle;
	res_win_t hImage;

	bool_t bDirty;

	tchar_t szFile[PATH_LEN];
	METADATA meta;
}ImagePanelDelta;

#define GETIMAGEPANELDELTA(ph) 		(ImagePanelDelta*)widget_get_user_delta(ph)
#define SETIMAGEPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)

#define IMAGEPANEL_ACCEL_COUNT	5
accel_t	IMAGEPANEL_ACCEL[IMAGEPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

void	ImagePanel_Switch(res_win_t widget);
void	ImagePanel_SetDirty(res_win_t widget, bool_t bDirty);
bool_t	ImagePanel_GetDirty(res_win_t widget);

bool_t	ImagePanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	ImagePanel_OpenFile(res_win_t widget, const tchar_t* szFile);

/*****************************************************************************************************/
void ImagePanel_Switch(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

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

void	ImagePanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	pdt->bDirty = bDirty;
}

bool_t	ImagePanel_GetDirty(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	return pdt->bDirty;
}
/***************************************************************************************************************/
void ImagePanel_OnSelectAll(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR ptrImage = imagesctrl_fetch(pdt->hImage);
	XDL_ASSERT(ptrImage);

	LINKPTR ilk = get_images_next_item(ptrImage, LINK_FIRST);
	while (ilk)
	{
		set_images_item_checked(ilk, 1);
		
		ilk = get_images_next_item(ptrImage, ilk);
	}

	imagesctrl_redraw(pdt->hImage);
}

void ImagePanel_OnDelete(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR ptrImage = imagesctrl_fetch(pdt->hImage);
	XDL_ASSERT(ptrImage);

	imagesctrl_set_focus_item(pdt->hImage, NULL);

	bool_t bRedraw = 0;
	LINKPTR nlk,ilk = get_images_next_item(ptrImage, LINK_FIRST);
	while (ilk)
	{
		nlk = get_images_next_item(ptrImage, ilk);
		if (get_images_item_checked(ilk))
		{
			delete_images_item(ilk);
			ilk = nlk;

			bRedraw = 1;
			continue;
		}

		ilk = nlk;
	}

	if (bRedraw)
		imagesctrl_redraw(pdt->hImage);
}

void ImagePanel_OnCopy(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	widget_post_command(pdt->hImage, COMMAND_COPY, IDC_EDITMENU, 0);
}

void ImagePanel_OnCut(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	widget_post_command(pdt->hImage, COMMAND_CUT, IDC_EDITMENU, 0);
}

void ImagePanel_OnPaste(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR ptrImage = imagesctrl_fetch(pdt->hImage);

	widget_post_command(pdt->hImage, COMMAND_PASTE, IDC_EDITMENU, 0);
}

void ImagePanel_OnAttach(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szExt[10] = { 0 };

	const tchar_t* szFilter = _T("JPG File(*.jpg)\0*.jpg\0PNG File(*.png)\0*.png\0BMP File(*.bmp)\0*.bmp\0");
	xscpy(szExt, _T("jpg"));

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, szFilter, szExt, 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	LINKPTR ptrImage = imagesctrl_fetch(pdt->hImage);

	tchar_t szName[PATH_LEN],szImage[RES_LEN];

	tchar_t* szToken = szFile;
	while (*szToken)
	{
		xsprintf(szName, _T("%s\\%s"), szPath, szToken);
		split_path(szName, NULL, szImage, NULL);

		insert_images_item_from_file(ptrImage, szImage, NULL, szName);

		szToken += (xslen(szToken) + 1);
	}

	imagesctrl_redraw(pdt->hImage);
}

void ImagePanel_OnDetach(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR ptrImage = imagesctrl_fetch(pdt->hImage);

	LINKPTR ptrItem = imagesctrl_get_focus_item(pdt->hImage);

	if (!ptrItem)
		return;

	delete_images_item(ptrItem);

	imagesctrl_redraw(pdt->hImage);
}

void ImagePanel_OnAttributes(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);

	LINKPTR ptrImages = imagesctrl_fetch(pdt->hImage);
	LINKPTR ptrItem = imagesctrl_get_focus_item(pdt->hImage);

	if (ptrItem)
		properbag_write_images_item_attributes(ptrProper, ptrItem);
	else
		properbag_write_images_attributes(ptrProper, ptrImages);

	properctrl_redraw(pdt->hProper);
}

void ImagePanel_OnStyleSheet(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void ImagePanel_Title_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	properctrl_accept(pdt->hProper, 0);

	LINKPTR ptrProper = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptrProper);
	properctrl_redraw(pdt->hProper);
}

void ImagePanel_Title_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	int n_id = xstol(get_title_item_id_ptr(pnt->item));

	widget_post_command(widget, 0, n_id, NULL);
}

void ImagePanel_Image_OnRBClick(res_win_t widget, NOTICE_IMAGES* pnf)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr = { 0 };
	xpoint_t pt = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_IMAGEPANEL_MENU);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);

	widget_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();
	LINKPTR mlk;

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_ATTACH);
	set_menu_item_title(mlk, _T("附加图像"));

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_DETACH);
	set_menu_item_title(mlk, _T("撤离图像"));

	menubox_set_data(hMenu, ptrMenu);

	pt.x = ppt->x;
	pt.y = ppt->y;
	widget_client_to_screen(pdt->hImage, &pt);

	menubox_layout(hMenu, &pt, WS_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void ImagePanel_Image_OnLBClick(res_win_t widget, NOTICE_IMAGES* pnf)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR ptrItem = titlectrl_get_focus_item(pdt->hTitle);
	if (!ptrItem)
		return;

	int n_id = xstol(get_title_item_id_ptr(ptrItem));
	widget_post_command(widget, 0, n_id, NULL);
}

void ImagePanel_Proper_OnEntityUpdate(res_win_t widget, NOTICE_PROPER* pnp)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR tlk = titlectrl_get_focus_item(pdt->hTitle);
	if (!tlk)
		return;

	int n_id = xstol(get_title_item_id_ptr(tlk));

	LINKPTR ptrImages = imagesctrl_fetch(pdt->hImage);
	LINKPTR ptrItem = imagesctrl_get_focus_item(pdt->hImage);

	tchar_t sz_style[CSS_LEN] = { 0 };

	if (ptrItem)
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_images_item_attributes(pnp->proper, ptrItem);
		}
		imagesctrl_redraw_item(pdt->hImage, ptrItem);
	}
	else
	{
		if (n_id == IDA_ATTRIBUTES)
		{
			properbag_read_images_attributes(pnp->proper, ptrImages);
		}
		imagesctrl_redraw(pdt->hImage);
	}
}

bool_t ImagePanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR ptrMeta = create_meta_doc();
	if (!load_dom_doc_from_file(ptrMeta, NULL, szFile))
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("导入文档失败！"));

		return 0;
	}

	if (compare_text(get_meta_doc_name_ptr(ptrMeta), -1, DOC_IMAGES, -1, 1) != 0)
	{
		destroy_meta_doc(ptrMeta);
		ShowMsg(MSGICO_ERR, _T("非图像列表文档！"));

		return 0;
	}

	get_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, RES_LEN);
	get_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, RES_LEN);

	LINKPTR newImage = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	LINKPTR orgImage = imagesctrl_detach(pdt->hImage);
	destroy_images_doc(orgImage);

	imagesctrl_attach(pdt->hImage, newImage);

	xscpy(pdt->szFile, szFile);

	return 1;
}

bool_t ImagePanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	LINKPTR ptrImage = imagesctrl_fetch(pdt->hImage);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrImage);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrImage = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
	{
		MessageBox(widget, _T("保存图形文档错误"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_path(pdt->szFile, NULL, szName, NULL);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void ImagePanel_OnSave(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("Images Meta File(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	ImagePanel_SaveFile(widget, szFile);
}

void ImagePanel_OnSaveAs(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szPath[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("Images Meta File(*.sheet)\0*.sheet\0"), _T("sheet"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrImage = imagesctrl_fetch(pdt->hImage);

	LINKPTR ptrMeta = create_meta_doc();

	set_meta_head_meta(ptrMeta, ATTR_AUTHOR, -1, pdt->meta.Author, -1);
	set_meta_head_meta(ptrMeta, ATTR_COMPANY, -1, pdt->meta.Company, -1);

	attach_meta_body_node(ptrMeta, ptrImage);

	bool_t rt = save_dom_doc_to_file(ptrMeta, NULL, szFile);

	ptrImage = detach_meta_body_node(ptrMeta);
	destroy_meta_doc(ptrMeta);

	if (!rt)
		ShowMsg(MSGICO_ERR, _T("保存文件错误！"));
}

void ImagePanel_OnPrint(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);
}

void ImagePanel_OnPreview(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

}

/***********************************************************************************************/

int ImagePanel_OnCreate(res_win_t widget, void* data)
{
	ImagePanelDelta* pdt = (ImagePanelDelta*)xmem_alloc(sizeof(ImagePanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETIMAGEPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(IMAGEPANEL_ACCEL, IMAGEPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkImage = get_split_first_child_item(ptrSplit);
	LINKPTR ilkRight = get_split_last_child_item(ptrSplit);

	tchar_t token[INT_LEN + 1];
	xsprintf(token, _T("%fmm"), -IMAGEPANEL_TITLEITEM_HEIGHT);

	split_item(ilkRight, 1);
	set_split_item_ratio(ilkRight, token);

	LINKPTR ilkProper = get_split_first_child_item(ilkRight);
	LINKPTR ilkTitle = get_split_last_child_item(ilkRight);

	widget_get_client_rect(widget, &xr);
	pdt->hImage = imagesctrl_create(_T("ImagePanel"), WD_STYLE_CONTROL, &xr, widget);

	widget_set_user_id(pdt->hImage, IDC_IMAGEPANEL_IMAGE);
	widget_set_owner(pdt->hImage, widget);

	set_split_item_delta(ilkImage, pdt->hImage);
	widget_show(pdt->hImage, WS_SHOW_NORMAL);

	LINKPTR ptrImage = create_images_doc();
	set_images_layer(ptrImage, ATTR_LAYER_HORZ);

	imagesctrl_attach(pdt->hImage, ptrImage);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("ImageProper"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_IMAGEPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WS_SHOW_NORMAL);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	widget_get_client_rect(widget, &xr);
	pdt->hTitle = titlectrl_create(_T("ImageTitle"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTitle, IDC_IMAGEPANEL_TITLE);
	widget_set_owner(pdt->hTitle, widget);

	set_split_item_delta(ilkTitle, pdt->hTitle);
	widget_show(pdt->hTitle, WS_SHOW_NORMAL);

	LINKPTR ptrTitle = create_title_doc();

	LINKPTR tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("属性"));
	xsprintf(token, _T("%d"), IDA_ATTRIBUTES);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, IMAGEPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ATTR_GIZMO_PROPER);
	set_title_item_locked(tlk, 1);

	tlk = insert_title_item(ptrTitle, LINK_LAST);
	set_title_item_title(tlk, _T("风格"));
	xsprintf(token, _T("%d"), IDA_STYLESHEET);
	set_title_item_id(tlk, token);
	set_title_item_width(tlk, IMAGEPANEL_TITLEITEM_WIDTH);
	set_title_item_icon(tlk, GDI_ATTR_GIZMO_STYLE);
	set_title_item_locked(tlk, 1);

	titlectrl_attach(pdt->hTitle, ptrTitle);
	titlectrl_set_focus_item(pdt->hTitle, get_title_next_item(ptrTitle, LINK_FIRST));

	widget_attach_splitor(widget, ptrSplit);

	const tchar_t* szParam = (tchar_t*)data;

	if (!is_null(szParam))
	{
		if (!ImagePanel_OpenFile(widget, szParam))
			return -1;

		imagesctrl_redraw(pdt->hImage);
	}

	return 0;
}

void ImagePanel_OnDestroy(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hImage))
	{
		LINKPTR ptrImage = imagesctrl_detach(pdt->hImage);
		if (ptrImage)
			destroy_images_doc(ptrImage);

		widget_destroy(pdt->hImage);
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

int ImagePanel_OnClose(res_win_t widget)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	ImagePanel_Switch(widget);

	return (pdt->bDirty) ? 1 : 0;
}

void ImagePanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hImage))
	{
		widget_set_focus(pdt->hImage);
	}
}

void ImagePanel_OnShow(res_win_t widget, bool_t bShow)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("编辑"));
		set_tool_group_item_width(glk, IMAGEPANEL_GROUPITEM_WIDTH);
		set_tool_group_item_height(glk, IMAGEPANEL_GROUPITEM_HEIGHT);
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
		xsprintf(token, _T("%d"), IDA_EDIT_ATTACH);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("添加"));
		set_tool_item_icon(ilk, GDI_ATTR_GIZMO_PLUS);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void ImagePanel_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		ImagePanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		ImagePanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		ImagePanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		ImagePanel_OnPreview(widget);
		break;

	case IDA_EDIT_SELECTALL:
		ImagePanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		ImagePanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		ImagePanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		ImagePanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		ImagePanel_OnPaste(widget);
		break;
	case IDA_EDIT_ATTACH:
		ImagePanel_OnAttach(widget);
		break;
	case IDA_EDIT_DETACH:
		ImagePanel_OnDetach(widget);
		break;

	case IDA_ATTRIBUTES:
		ImagePanel_OnAttributes(widget);
		break;
	case IDA_STYLESHEET:
		ImagePanel_OnStyleSheet(widget);
		break;

	case IDC_IMAGEPANEL_MENU:
		widget_destroy((res_win_t)data);
		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;
	}
}

void ImagePanel_OnParentCommand(res_win_t widget, int code, var_long data)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

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
		pdt->bDirty = 0;
	}
}

void ImagePanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	ImagePanelDelta* pdt = GETIMAGEPANELDELTA(widget);

	if (phdr->id == IDC_IMAGEPANEL_IMAGE)
	{
		NOTICE_IMAGES* pnf = (NOTICE_IMAGES*)phdr;
		switch (pnf->code)
		{
		case NC_IMAGESCALCED:
			break;
		case NC_IMAGEITEMCALCED:
			break;
		case NC_XAXCALCED:
			break;
		case NC_IMAGESLBCLK:
			ImagePanel_Image_OnLBClick(widget, pnf);
			break;
		case NC_IMAGESRBCLK:
			ImagePanel_Image_OnRBClick(widget, pnf);
			break;
		}
	}
	else if (phdr->id == IDC_IMAGEPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			break;
		case NC_ENTITYUPDATE:
			ImagePanel_Proper_OnEntityUpdate(widget, pnp);
			break;

		}
	}
	else if (phdr->id == IDC_IMAGEPANEL_TITLE)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLECALCED:
			break;
		case NC_TITLEITEMCHANGING:
			ImagePanel_Title_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			ImagePanel_Title_OnItemChanged(widget, pnt);
			break;
		}
	}
}

/*********************************************************************************************************/
res_win_t ImagePanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(ImagePanel_OnCreate)
	EVENT_ON_DESTROY(ImagePanel_OnDestroy)
	EVENT_ON_CLOSE(ImagePanel_OnClose)
	EVENT_ON_SHOW(ImagePanel_OnShow)

	EVENT_ON_NOTICE(ImagePanel_OnNotice)
	EVENT_ON_MENU_COMMAND(ImagePanel_OnMenuCommand)
	EVENT_ON_PARENT_COMMAND(ImagePanel_OnParentCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}