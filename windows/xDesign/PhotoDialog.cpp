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
#include "PhotoDialog.h"

#include "EasilyResource.h"
#include "EasilyModule.h"
#include "EasilyFrame.h"


#define IDC_PHOTODLG_PHOTO		2000
#define IDC_PHOTODLG_MENU		2001
#define IDC_PHOTODLG_OK			2002
#define IDC_PHOTODLG_CLOSE		2003

#define PHOTODLG_MENU_HEIGHT		(float)7 //tm
#define PHOTODLG_BUTTON_SPLIT		(float)1 //TM
#define PHOTODLG_BUTTON_HEIGHT		(float)6 //tm
#define PHOTODLG_BUTTON_WIDTH		(float)10 //tm

#define IDA_PHOTODLG_MASKITEM	1001
#define IDA_PHOTODLG_DRAWCOLOR	1002
#define IDA_PHOTODLG_PAINTCOLOR 1003
#define IDA_PHOTODLG_FONTNAME	1004
#define IDA_PHOTODLG_FONTSIZE	1005
#define IDA_PHOTODLG_FONTCOLOR	1006
#define IDA_PHOTODLG_COMMIT		1007

typedef struct _tagImgDlgDelta{
	res_win_t photo;
	res_win_t menu;

	XHANDLE var;
}tagImgDlgDelta;


#define GETPHOTODLGDELTA(ph) 	(tagImgDlgDelta*)widget_get_user_delta(ph)
#define SETPHOTODLGDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

/*****************************************************************************/
void PhotoDlg_OnOK(res_win_t widget)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	if (ptd->var)
	{
		int len = photoctrl_get_image(ptd->photo, NULL, MAX_LONG);
		photoctrl_get_image(ptd->photo, varstr_ensure_buf(ptd->var, len), len);
	}

	widget_close(widget, 1);
}

void PhotoDlg_OnClose(res_win_t widget)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	widget_close(widget, 0);
}

void PhotoDlg_OnDrawMask(res_win_t widget, res_dc_t rdc, const xrect_t* pxr, photo_mask_t* pma)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	draw_shape_raw(rdc, &pma->pen, &pma->brush, pxr, pma->type);
}

void PhotoDlg_OnShowMaskItem(res_win_t widget, const xrect_t* pxr)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	xpoint_t pt;

	pt.x = pxr->x;
	pt.y = pxr->y + pxr->h;
	res_win_t hMenu = shape_menu(widget, IDC_MENU_SHAPE, &pt);

	xcolor_t xc[4];
	parse_xcolor(&xc[0], g_face[g_indFace].bkg);
	parse_xcolor(&xc[1], g_face[g_indFace].frg);
	parse_xcolor(&xc[2], g_face[g_indFace].txt);
	parse_xcolor(&xc[3], g_face[g_indFace].msk);

	widget_send_command(hMenu, COMMAND_COLOR, IDC_PARENT, (void*)xc);

	widget_do_trace(hMenu);
}

void PhotoDlg_OnSelectMaskItem(res_win_t widget, const tchar_t* mid)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	photo_mask_t mt = { 0 };
	
	default_xbrush(&mt.brush);
	default_xface(&mt.face);
	default_xfont(&mt.font);
	default_xpen(&mt.pen);
	xscpy(mt.type, mid);

	mt.rect.x = 0;
	mt.rect.y = 0;
	mt.rect.w = 20;
	mt.rect.h = 20;

	photoctrl_append_mask(ptd->photo, &mt);

	photoctrl_redraw(ptd->photo);
}

void PhotoDlg_OnShowFontName(res_win_t widget, const xrect_t* pxr)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	xpoint_t pt;

	pt.x = pxr->x;
	pt.y = pxr->y + pxr->h;
	res_win_t hMenu = fontname_menu(widget, IDC_MENU_FONTNAME, &pt);

	xcolor_t xc[4];
	parse_xcolor(&xc[0], g_face[g_indFace].bkg);
	parse_xcolor(&xc[1], g_face[g_indFace].frg);
	parse_xcolor(&xc[2], g_face[g_indFace].txt);
	parse_xcolor(&xc[3], g_face[g_indFace].msk);

	widget_send_command(hMenu, COMMAND_COLOR, IDC_PARENT, (void*)xc);

	widget_do_trace(hMenu);
}

void PhotoDlg_OnSelectFontName(res_win_t widget, const tchar_t* mid)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	photo_mask_t *pmt = photoctrl_get_focus_mask(ptd->photo);
	if (!pmt)
		return;

	xscpy(pmt->font.family, mid);

	photoctrl_redraw(ptd->photo);
}

void PhotoDlg_OnShowFontSize(res_win_t widget, const xrect_t* pxr)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	xpoint_t pt;

	pt.x = pxr->x;
	pt.y = pxr->y + pxr->h;
	res_win_t hMenu = fontsize_menu(widget, IDC_MENU_FONTSIZE, &pt);

	xcolor_t xc[4];
	parse_xcolor(&xc[0], g_face[g_indFace].bkg);
	parse_xcolor(&xc[1], g_face[g_indFace].frg);
	parse_xcolor(&xc[2], g_face[g_indFace].txt);
	parse_xcolor(&xc[3], g_face[g_indFace].msk);

	widget_send_command(hMenu, COMMAND_COLOR, IDC_PARENT, (void*)xc);

	widget_do_trace(hMenu);
}

void PhotoDlg_OnSelectFontSize(res_win_t widget, const tchar_t* mid)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	photo_mask_t *pmt = photoctrl_get_focus_mask(ptd->photo);
	if (!pmt)
		return;

	xscpy(pmt->font.size, mid);

	photoctrl_redraw(ptd->photo);
}

void PhotoDlg_OnShowFontColor(res_win_t widget, const xrect_t* pxr)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	xpoint_t pt;

	pt.x = pxr->x;
	pt.y = pxr->y + pxr->h;

	res_win_t hMenu = color_menu(widget, IDC_MENU_FONTCOLOR, &pt);

	xcolor_t xc[4];
	parse_xcolor(&xc[0], g_face[g_indFace].bkg);
	parse_xcolor(&xc[1], g_face[g_indFace].frg);
	parse_xcolor(&xc[2], g_face[g_indFace].txt);
	parse_xcolor(&xc[3], g_face[g_indFace].msk);

	widget_send_command(hMenu, COMMAND_COLOR, IDC_PARENT, (void*)xc);

	widget_do_trace(hMenu);
}

void PhotoDlg_OnSelectFontColor(res_win_t widget, const tchar_t* mid)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	photo_mask_t *pmt = photoctrl_get_focus_mask(ptd->photo);
	if (!pmt)
		return;

	xscpy(pmt->font.color, mid);

	photoctrl_redraw(ptd->photo);
}

void PhotoDlg_OnShowDrawColor(res_win_t widget, const xrect_t* pxr)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	xpoint_t pt;

	pt.x = pxr->x;
	pt.y = pxr->y + pxr->h;
	res_win_t hMenu = color_menu(widget, IDC_MENU_DRAWCOLOR, &pt);

	xcolor_t xc[4];
	parse_xcolor(&xc[0], g_face[g_indFace].bkg);
	parse_xcolor(&xc[1], g_face[g_indFace].frg);
	parse_xcolor(&xc[2], g_face[g_indFace].txt);
	parse_xcolor(&xc[3], g_face[g_indFace].msk);

	widget_send_command(hMenu, COMMAND_COLOR, IDC_PARENT, (void*)xc);

	widget_do_trace(hMenu);
}

void PhotoDlg_OnSelectDrawColor(res_win_t widget, const tchar_t* mid)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	photo_mask_t *pmt = photoctrl_get_focus_mask(ptd->photo);
	if (!pmt)
		return;

	xscpy(pmt->pen.color, mid);

	photoctrl_redraw(ptd->photo);
}

void PhotoDlg_OnShowPaintColor(res_win_t widget, const xrect_t* pxr)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	xpoint_t pt;

	pt.x = pxr->x;
	pt.y = pxr->y + pxr->h;
	res_win_t hMenu = color_menu(widget, IDC_MENU_PAINTCOLOR, &pt);

	xcolor_t xc[4];
	parse_xcolor(&xc[0], g_face[g_indFace].bkg);
	parse_xcolor(&xc[1], g_face[g_indFace].frg);
	parse_xcolor(&xc[2], g_face[g_indFace].txt);
	parse_xcolor(&xc[3], g_face[g_indFace].msk);

	widget_send_command(hMenu, COMMAND_COLOR, IDC_PARENT, (void*)xc);

	widget_do_trace(hMenu);
}

void PhotoDlg_OnSelectPaintColor(res_win_t widget, const tchar_t* mid)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	photo_mask_t *pmt = photoctrl_get_focus_mask(ptd->photo);
	if (!pmt)
		return;

	xscpy(pmt->brush.color, mid);

	photoctrl_redraw(ptd->photo);
}

void PhotoDlg_OnCommit(res_win_t widget)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	photoctrl_commit(ptd->photo);
}
/*****************************************************************************/
int PhotoDlg_OnCreate(res_win_t widget, void* data)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	res_dc_t rdc;
	xhand_t canv;
	xrect_t xr, xr_box, xr_btn;
	xsize_t xs;
	int nSplit, nBar;
	res_win_t pushbox;

	ptd = (tagImgDlgDelta*)xmem_alloc(sizeof(tagImgDlgDelta));
	xmem_zero((void*)ptd, sizeof(tagImgDlgDelta));

	ptd->var = (XHANDLE)data;

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_attach_canvas(widget, canv);

	widget_get_client_rect(widget, &xr);

	LINKPTR ptr_menu = create_menu_doc();
	set_menu_images(ptr_menu, g_ptrImages);
	set_menu_layer(ptr_menu, ATTR_LAYER_HORZ);
	set_menu_show(ptr_menu, ATTR_SHOW_IMAGEONLY);

	tchar_t token[INT_LEN];

	LINKPTR ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_name(ilk, _T("MaskItem"));
	ltoxs(IDA_PHOTODLG_MASKITEM, token, INT_LEN);
	set_menu_item_id(ilk, token);
	set_menu_item_image(ilk, BMP_SHAPE);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_name(ilk, _T("FontName"));
	ltoxs(IDA_PHOTODLG_FONTNAME, token, INT_LEN);
	set_menu_item_id(ilk, token);
	set_menu_item_image(ilk, BMP_FONTNAME);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_name(ilk, _T("FontSize"));
	ltoxs(IDA_PHOTODLG_FONTSIZE, token, INT_LEN);
	set_menu_item_id(ilk, token);
	set_menu_item_image(ilk, BMP_FONTSIZE);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_name(ilk, _T("FontColor"));
	ltoxs(IDA_PHOTODLG_FONTCOLOR, token, INT_LEN);
	set_menu_item_id(ilk, token);
	set_menu_item_image(ilk, BMP_FONTCOLOR);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_name(ilk, _T("DrawColor"));
	ltoxs(IDA_PHOTODLG_DRAWCOLOR, token, INT_LEN);
	set_menu_item_id(ilk, token);
	set_menu_item_image(ilk, BMP_DRAW);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_name(ilk, _T("PaintColor"));
	ltoxs(IDA_PHOTODLG_PAINTCOLOR, token, INT_LEN);
	set_menu_item_id(ilk, token);
	set_menu_item_image(ilk, BMP_PAINT);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_name(ilk, _T("Commit"));
	ltoxs(IDA_PHOTODLG_COMMIT, token, INT_LEN);
	set_menu_item_id(ilk, token);
	set_menu_item_image(ilk, BMP_CHECK);

	xs.fx = PHOTODLG_MENU_HEIGHT;
	xs.fy = PHOTODLG_MENU_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y;
	xr_box.h = xs.cy;

	ptd->menu = menuctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_NOACTIVE, &xr_box, widget);
	widget_set_user_id(ptd->menu, IDC_PHOTODLG_MENU);
	widget_set_owner(ptd->menu, widget);
	menuctrl_attach(ptd->menu, ptr_menu);

	widget_show(ptd->menu, WD_SHOW_NORMAL);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y + xs.cy;
	xr_box.h = xr.h - xs.cx - xs.cy;

	ptd->photo = photoctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_PAGING, &xr_box, widget);
	widget_set_user_id(ptd->photo, IDC_PHOTODLG_PHOTO);
	widget_set_owner(ptd->photo, widget);

	photoctrl_set_image(ptd->photo, varstr_ptr(ptd->var));

	widget_show(ptd->photo, WD_SHOW_NORMAL);

	xs.fx = PHOTODLG_BUTTON_SPLIT;
	xs.fy = PHOTODLG_MENU_HEIGHT;
	widget_size_to_pt(widget, &xs);

	nSplit = xs.cx;
	nBar = xs.cy;

	xs.fx = PHOTODLG_BUTTON_WIDTH;
	xs.fy = PHOTODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_btn.x = xr.x + xr.w - nSplit - xs.cx;
	xr_btn.y = xr.y + xr.h - nBar;
	xr_btn.w = xs.cx;
	xr_btn.h = nBar;
	pt_center_rect(&xr_btn, xs.cx, xs.cy);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_btn);
	widget_set_user_id(pushbox, IDC_PHOTODLG_CLOSE);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, _T("¹Ø±Õ"), -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr_btn.x -= (nSplit + xs.cx);
	
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr_btn);
	widget_set_user_id(pushbox, IDC_PHOTODLG_OK);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, _T("È·¶¨"), -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	SETPHOTODLGDELTA(widget, ptd);

	return 0;
}

void PhotoDlg_OnDestroy(res_win_t widget)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	res_win_t pushbox;

	if (!ptd)
		return;

	if (widget_is_valid(ptd->menu))
	{
		LINKPTR ptr_menu = menuctrl_detach(ptd->menu);
		destroy_menu_doc(ptr_menu);

		widget_destroy(ptd->menu);
	}

	if (widget_is_valid(ptd->photo))
		widget_destroy(ptd->photo);

	pushbox = widget_get_child(widget, IDC_PHOTODLG_OK);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PHOTODLG_CLOSE);
	if (pushbox)
		widget_destroy(pushbox);

	destroy_display_canvas(widget_get_canvas(widget));

	SETPHOTODLGDELTA(widget, 0);
	xmem_free(ptd);
}

void PhotoDlg_OnNotice(res_win_t widget, NOTICE* pnt)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	int ida;

	if (!ptd)
		return;

	if (pnt->id == IDC_PHOTODLG_PHOTO)
	{
		NOTICE_PHOTO* pne = (NOTICE_PHOTO*)pnt;
		switch (pne->code)
		{
		case NC_PHOTOMASKDRAW:
			PhotoDlg_OnDrawMask(widget, (res_dc_t)pne->rdc, &pne->rect, (photo_mask_t*)pne->data);
			break;
		}
	}
	else if (pnt->id == IDC_PHOTODLG_MENU)
	{
		NOTICE_MENU* pnm = (NOTICE_MENU*)pnt;
		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (!pnm->item)
				break;

			ida = xstol(get_menu_item_id_ptr(pnm->item));
			if (ida == IDA_PHOTODLG_FONTNAME)
				PhotoDlg_OnShowFontName(widget, &pnm->rect);
			else if (ida == IDA_PHOTODLG_FONTSIZE)
				PhotoDlg_OnShowFontSize(widget, &pnm->rect);
			else if (ida == IDA_PHOTODLG_FONTCOLOR)
				PhotoDlg_OnShowFontColor(widget, &pnm->rect);
			else if (ida == IDA_PHOTODLG_DRAWCOLOR)
				PhotoDlg_OnShowDrawColor(widget, &pnm->rect);
			else if (ida == IDA_PHOTODLG_PAINTCOLOR)
				PhotoDlg_OnShowPaintColor(widget, &pnm->rect);
			else if (ida == IDA_PHOTODLG_MASKITEM)
				PhotoDlg_OnShowMaskItem(widget, &pnm->rect);
			else if (ida == IDA_PHOTODLG_COMMIT)
				PhotoDlg_OnCommit(widget);
			break;
		}
	}
	else if (pnt->id == IDC_MENU_FONTNAME)
	{
		NOTICE_MENU* pnm = (NOTICE_MENU*)pnt;
		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (pnm->item)
			{
				PhotoDlg_OnSelectFontName(widget, get_menu_item_id_ptr(pnm->item));
			}
			menuctrl_destroy(pnm->widget);
			break;
		}
	}
	else if (pnt->id == IDC_MENU_FONTSIZE)
	{
		NOTICE_MENU* pnm = (NOTICE_MENU*)pnt;
		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (pnm->item)
			{
				PhotoDlg_OnSelectFontSize(widget, get_menu_item_id_ptr(pnm->item));
			}
			menuctrl_destroy(pnm->widget);
			break;
		}
	}
	else if (pnt->id == IDC_MENU_FONTCOLOR)
	{
		NOTICE_MENU* pnm = (NOTICE_MENU*)pnt;
		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (pnm->item)
			{
				PhotoDlg_OnSelectFontColor(widget, get_menu_item_id_ptr(pnm->item));
			}
			menuctrl_destroy(pnm->widget);
			break;
		}
	}
	else if (pnt->id == IDC_MENU_DRAWCOLOR)
	{
		NOTICE_MENU* pnm = (NOTICE_MENU*)pnt;
		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (pnm->item)
			{
				PhotoDlg_OnSelectDrawColor(widget, get_menu_item_id_ptr(pnm->item));
			}
			menuctrl_destroy(pnm->widget);
			break;
		}
	}
	else if (pnt->id == IDC_MENU_PAINTCOLOR)
	{
		NOTICE_MENU* pnm = (NOTICE_MENU*)pnt;
		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (pnm->item)
			{
				PhotoDlg_OnSelectPaintColor(widget, get_menu_item_id_ptr(pnm->item));
			}
			menuctrl_destroy(pnm->widget);
			break;
		}
	}
	else if (pnt->id == IDC_MENU_SHAPE)
	{
		NOTICE_MENU* pnm = (NOTICE_MENU*)pnt;
		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (pnm->item)
			{
				PhotoDlg_OnSelectMaskItem(widget, get_menu_item_id_ptr(pnm->item));
			}
			menuctrl_destroy(pnm->widget);
			break;
		}
	}
}

void PhotoDlg_OnCommandColor(res_win_t widget, void* data)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	res_win_t pushbox;

	if (!ptd)
		return;

	widget_set_color_mode(widget, (clr_mod_t*)data);

	widget_send_command(ptd->photo, COMMAND_COLOR, IDC_PARENT, (clr_mod_t*)data);

	widget_send_command(ptd->menu, COMMAND_COLOR, IDC_PARENT, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_PHOTODLG_OK);
	if (pushbox)
		widget_send_command(pushbox, COMMAND_COLOR, IDC_PARENT, (clr_mod_t*)data);

	pushbox = widget_get_child(widget, IDC_PHOTODLG_CLOSE);
	if (pushbox)
		widget_send_command(pushbox, COMMAND_COLOR, IDC_PARENT, (clr_mod_t*)data);
}

void PhotoDlg_OnMenuCommand(res_win_t widget, int code, int cid, void* data)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	if (!ptd)
		return;

	switch (cid)
	{
	case IDC_PHOTODLG_OK:
		PhotoDlg_OnOK(widget);
		break;
	case IDC_PHOTODLG_CLOSE:
		PhotoDlg_OnClose(widget);
		break;
	}
}

void PhotoDlg_OnSize(res_win_t widget, int code, const xsize_t* prs)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	xsize_t xs;
	xrect_t xr, xr_box, xr_btn;
	int nSplit,nBar;
	res_win_t pushbox;

	widget_get_client_rect(widget, &xr);

	xs.fx = PHOTODLG_MENU_HEIGHT;
	xs.fy = PHOTODLG_MENU_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y;
	xr_box.h = xs.cy;

	widget_move(ptd->menu, RECTPOINT(&xr_box));
	widget_size(ptd->menu, RECTSIZE(&xr_box));
	widget_update_client(ptd->menu);

	xr_box.x = xr.x;
	xr_box.w = xr.w;
	xr_box.y = xr.y + xs.cy;
	xr_box.h = xr.h - xs.cx - xs.cy;

	widget_move(ptd->photo, RECTPOINT(&xr_box));
	widget_size(ptd->photo, RECTSIZE(&xr_box));
	widget_update_client(ptd->photo);

	xs.fx = PHOTODLG_BUTTON_SPLIT;
	xs.fy = PHOTODLG_MENU_HEIGHT;
	widget_size_to_pt(widget, &xs);

	nSplit = xs.cx;
	nBar = xs.cy;

	xs.fx = PHOTODLG_BUTTON_WIDTH;
	xs.fy = PHOTODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_btn.x = xr.x + xr.w - nSplit - xs.cx;
	xr_btn.y = xr.y + xr.h - nBar;
	xr_btn.w = xs.cx;
	xr_btn.h = nBar;
	pt_center_rect(&xr_btn, xs.cx, xs.cy);

	pushbox = widget_get_child(widget, IDC_PHOTODLG_CLOSE);
	widget_move(pushbox, RECTPOINT(&xr_btn));
	widget_update_client(pushbox);

	xr_btn.x -= (nSplit + xs.cx);

	pushbox = widget_get_child(widget, IDC_PHOTODLG_OK);
	widget_move(pushbox, RECTPOINT(&xr_btn));
	widget_update_client(pushbox);

	widget_invalid(widget, NULL, 0);
}

void PhotoDlg_OnSetFocus(res_win_t widget, res_win_t wt)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);

	if (widget_is_valid(ptd->photo))
	{
		widget_set_focus(ptd->photo);
	}
}

void PhotoDlg_OnErase(res_win_t widget, res_dc_t dc)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
}

void PhotoDlg_OnPaint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	tagImgDlgDelta* ptd = GETPHOTODLGDELTA(widget);
	res_dc_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb_bar,xb = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr,xr_bar;
	xsize_t xs;
	viewbox_t vb = { 0 };

	xhand_t canv;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	xmem_copy((void*)&xb_bar, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bar, DEF_MIDD_DARKEN);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_client_rect(widget, &xr);

	xs.fx = ZERO_WIDTH;
	xs.fy = PHOTODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y + xr.h - xs.cy;
	xr_bar.w = xr.w;
	xr_bar.h = xs.cy;

	xscpy(xg.brim_color, xb.color);
	xscpy(xg.core_color, xb.color);
	lighten_xgradi(&xg, DEF_MIDD_DARKEN);
	xscpy(xg.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	gradient_rect_raw(rdc, &xg, &xr_bar);

	end_display_paint(canv, dc, pxr);
}

/***************************************************************************************/
res_win_t PhotoDlg_Create(const tchar_t* title, XHANDLE vs)
{
	res_win_t widget;
	if_event_t ev = { 0 };

	ev.data = (void*)vs;
	ev.pf_on_create = PhotoDlg_OnCreate;
	ev.pf_on_destroy = PhotoDlg_OnDestroy;
	ev.pf_on_set_focus = PhotoDlg_OnSetFocus;

	ev.pf_on_erase = PhotoDlg_OnErase;
	ev.pf_on_paint = PhotoDlg_OnPaint;

	ev.pf_on_size = PhotoDlg_OnSize;
	ev.pf_on_command_color = PhotoDlg_OnCommandColor;
	ev.pf_on_menu_command = PhotoDlg_OnMenuCommand;
	ev.pf_on_notice = PhotoDlg_OnNotice;

	xrect_t xr = { 0 };

	xr.x = 0;
	xr.y = 0;
	xr.w = 400;
	xr.h = 300;

	widget = widget_create(title, WD_STYLE_DIALOG, &xr, g_hMain, &ev);

	if (!widget)
	{
		return 0;
	}

	clr_mod_t clr;
	parse_xcolor(&clr.clr_bkg, g_face[g_indFace].bkg);
	parse_xcolor(&clr.clr_frg, g_face[g_indFace].frg);
	parse_xcolor(&clr.clr_txt, g_face[g_indFace].txt);
	parse_xcolor(&clr.clr_msk, g_face[g_indFace].msk);
	widget_send_command(widget, COMMAND_COLOR, IDC_PARENT, (void*)&clr);

	widget_center_window(widget, g_hMain);
	widget_show(widget, WD_SHOW_NORMAL);
	widget_update_window(widget);
	widget_update_client(widget);

	return widget;
}
