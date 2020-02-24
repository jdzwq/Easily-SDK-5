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
#include "PreviewDialog.h"

#include "EasilyResource.h"
#include "EasilyModule.h"
#include "EasilyFrame.h"


#define IDC_PREVIEWDLG_SVG		2000
#define IDC_PREVIEWDLG_PRINT	2001
#define IDC_PREVIEWDLG_SETUP	2002
#define IDC_PREVIEWDLG_OPEN		2003
#define IDC_PREVIEWDLG_SAVE		2004
#define IDC_PREVIEWDLG_SAVEALL	2005
#define IDC_PREVIEWDLG_FIRST	2006
#define IDC_PREVIEWDLG_PREV		2007
#define IDC_PREVIEWDLG_NEXT		2008
#define IDC_PREVIEWDLG_LAST		2009
#define IDC_PREVIEWDLG_CLOSE	2010

#define PREVIEWDLG_BAR_HEIGHT	(float)10 //MM
#define PREVIEWDLG_BAR_SPLIT	(float)2 //MM
#define PREVIEWDLG_BTN_WIDTH	(float)6 //MM

typedef struct tagPreviewDlgDelta{
	res_win_t hSvg;

	res_win_t hPushPrint;
	res_win_t hPushSetup;
	res_win_t hPushOpen;
	res_win_t hPushSave;
	res_win_t hPushSaveAll;
	res_win_t hPushFirst;
	res_win_t hPushPrev;
	res_win_t hPushNext;
	res_win_t hPushLast;
	res_win_t hPushClose;

	LINKPTR doc;
	int page;

	dev_mod_t mod;
}PreviewDlgDelta;

#define GETPREVIEWDLGDELTA(ph) 		(PreviewDlgDelta*)widget_get_user_delta(ph)
#define SETPREVIEWDLGDELTA(ph,ptd)	widget_set_user_delta(ph,(void*)ptd)


void	PreviewDlg_Setup(res_win_t widget);
void	PreviewDlg_Print(res_win_t widget);
void	PreviewDlg_Open(res_win_t widget);
void	PreviewDlg_Save(res_win_t widget);
void	PreviewDlg_SaveAll(res_win_t widget);

void	PreviewDlg_First(res_win_t widget);
void	PreviewDlg_Prev(res_win_t widget);
void	PreviewDlg_Next(res_win_t widget);
void	PreviewDlg_Last(res_win_t widget);

void PreviewDlg_First(res_win_t widget)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (!pdt->doc)
		return;

	pdt->page = 1;

	LINKPTR svg = svgctrl_fetch(pdt->hSvg);

	xhand_t svgcanv = create_svg_canvas(svg);

	if_canvas_t* pif = create_svg_interface(svgcanv);

	widget_get_canv_rect(pdt->hSvg, (canvbox_t*)&(pif->rect));

	if (is_form_doc(pdt->doc))
	{
		draw_form_page(pif, pdt->doc, pdt->page);
	}
	else if (is_grid_doc(pdt->doc))
	{
		draw_grid_page(pif, pdt->doc, pdt->page);
	}
	else if (is_graph_doc(pdt->doc))
	{
		draw_graph_page(pif, pdt->doc, pdt->page);
	}
	
	destroy_svg_canvas(svgcanv);
	destroy_svg_interface(pif);

	svgctrl_redraw(pdt->hSvg);
}

void PreviewDlg_Save(res_win_t widget)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (!pdt->doc)
		return;

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("svg view file(*.svg)\0*.svg\0"), _T("svg"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);
	xscpy(szFile, szPath);

	LINKPTR ptrSvg = svgctrl_fetch(pdt->hSvg);

	if (!save_dom_doc_to_file(ptrSvg, NULL, szFile))
	{
		ShowMsg(MSGICO_ERR, _T("±£´æSVGÎÄµµÊ§°Ü£¡"));
	}
}

void PreviewDlg_Setup(res_win_t widget)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (!pdt->doc)
		return;

	setup_printer_mode(widget, &pdt->mod);

	LINKPTR svg = svgctrl_fetch(pdt->hSvg);

	if (pdt->mod.b_landscape)
	{
		set_svg_width(svg, (float)pdt->mod.paper_height / 10);
		set_svg_height(svg, (float)pdt->mod.paper_width / 10);
	}else
	{
		set_svg_width(svg, (float)pdt->mod.paper_width / 10);
		set_svg_height(svg, (float)pdt->mod.paper_height / 10);
	}

	svgctrl_redraw(pdt->hSvg);
}

void PreviewDlg_Print(res_win_t widget)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (!pdt->doc)
		return;

	LINKPTR svg = svgctrl_fetch(pdt->hSvg);

	print_svg(&pdt->mod, svg);
}
/***************************************************************************************/
int PreviewDlg_OnCreate(res_win_t widget, void* data)
{
	PreviewDlgDelta* pdt = (PreviewDlgDelta*)xmem_alloc(sizeof(PreviewDlgDelta));

	SETPREVIEWDLGDELTA(widget, pdt);

	PREVIEWDLGDATA* ppd = (PREVIEWDLGDATA*)data;
	pdt->doc = ppd->doc;

	res_dc_t rdc = widget_client_dc(widget);
	xhand_t canv = create_display_canvas(rdc);

	widget_attach_canvas(widget, canv);
	widget_release_dc(widget, rdc);

	xrect_t xr;
	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = PREVIEWDLG_BAR_SPLIT;
	xs.fy = PREVIEWDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr.h = xs.cy;
	xs.fx = PREVIEWDLG_BTN_WIDTH;
	xs.fy = PREVIEWDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - xs.cy;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xs.cy;
	xr_push.h = xr.h - 2 * xs.cy;

	LINKPTR ilk;
	const tchar_t* token;

	ilk = get_image_item(g_ptrImages, BMP_CLOSE, -1);
	token = (ilk)? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushClose = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushClose, IDC_PREVIEWDLG_CLOSE);
	widget_set_owner(pdt->hPushClose, widget);
	pushbox_set_text(pdt->hPushClose, token, -1);
	widget_show(pdt->hPushClose, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_LAST, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushLast = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushLast, IDC_PREVIEWDLG_LAST);
	widget_set_owner(pdt->hPushLast, widget);
	pushbox_set_text(pdt->hPushLast, token, -1);
	widget_show(pdt->hPushLast, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_NEXT, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushNext = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushNext, IDC_PREVIEWDLG_NEXT);
	widget_set_owner(pdt->hPushNext, widget);
	pushbox_set_text(pdt->hPushNext, token, -1);
	widget_show(pdt->hPushNext, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_PREV, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushPrev = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushPrev, IDC_PREVIEWDLG_PREV);
	widget_set_owner(pdt->hPushPrev, widget);
	pushbox_set_text(pdt->hPushPrev, token, -1);
	widget_show(pdt->hPushPrev, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_FIRST, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushFirst = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushFirst, IDC_PREVIEWDLG_FIRST);
	widget_set_owner(pdt->hPushFirst, widget);
	pushbox_set_text(pdt->hPushFirst, token, -1);
	widget_show(pdt->hPushFirst, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_SAVEAS, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushSaveAll = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushSaveAll, IDC_PREVIEWDLG_SAVEALL);
	widget_set_owner(pdt->hPushSaveAll, widget);
	pushbox_set_text(pdt->hPushSaveAll, token, -1);
	widget_show(pdt->hPushSaveAll, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_SAVE, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushSave = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushSave, IDC_PREVIEWDLG_SAVE);
	widget_set_owner(pdt->hPushSave, widget);
	pushbox_set_text(pdt->hPushSave, token, -1);
	widget_show(pdt->hPushSave, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_OPEN, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushOpen = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushOpen, IDC_PREVIEWDLG_OPEN);
	widget_set_owner(pdt->hPushOpen, widget);
	pushbox_set_text(pdt->hPushOpen, token, -1);
	widget_show(pdt->hPushOpen, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_PRINTER, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushSetup = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushSetup, IDC_PREVIEWDLG_SETUP);
	widget_set_owner(pdt->hPushSetup, widget);
	pushbox_set_text(pdt->hPushSetup, token, -1);
	widget_show(pdt->hPushSetup, WD_SHOW_NORMAL);

	xr_push.x -= (xs.cx + xs.cy);

	ilk = get_image_item(g_ptrImages, BMP_PRINT, -1);
	token = (ilk) ? get_image_item_src_ptr(ilk) : NULL;
	pdt->hPushPrint = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_IMAGE, &xr_push);
	widget_set_user_id(pdt->hPushPrint, IDC_PREVIEWDLG_PRINT);
	widget_set_owner(pdt->hPushPrint, widget);
	pushbox_set_text(pdt->hPushPrint, token, -1);
	widget_show(pdt->hPushPrint, WD_SHOW_NORMAL);

	default_printer_mode(&pdt->mod);

	xs.fx = PREVIEWDLG_BAR_SPLIT;
	xs.fy = PREVIEWDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y + xs.cy;
	xr_reg.h = xr.h - xs.cy;

	pdt->hSvg = svgctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_PAGING, &xr_reg, widget);
	widget_set_user_id(pdt->hSvg, IDC_PREVIEWDLG_SVG);
	widget_set_owner(pdt->hSvg, widget);

	LINKPTR svg = create_svg_doc();
	set_svg_width(svg, (float)pdt->mod.paper_width / 10);
	set_svg_width(svg, (float)pdt->mod.paper_width / 10);

	svgctrl_attach(pdt->hSvg, svg);

	widget_show(pdt->hSvg, WD_SHOW_NORMAL);

	PreviewDlg_First(widget);

	return 0;
}

void PreviewDlg_OnDestroy(res_win_t widget)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	if (widget_is_valid(pdt->hSvg))
	{
		widget_destroy(pdt->hSvg);
	}

	if (widget_is_valid(pdt->hPushClose))
	{
		widget_destroy(pdt->hPushClose);
	}

	if (widget_is_valid(pdt->hPushLast))
	{
		widget_destroy(pdt->hPushLast);
	}

	if (widget_is_valid(pdt->hPushNext))
	{
		widget_destroy(pdt->hPushNext);
	}

	if (widget_is_valid(pdt->hPushPrev))
	{
		widget_destroy(pdt->hPushPrev);
	}

	if (widget_is_valid(pdt->hPushFirst))
	{
		widget_destroy(pdt->hPushFirst);
	}

	if (widget_is_valid(pdt->hPushSaveAll))
	{
		widget_destroy(pdt->hPushSaveAll);
	}

	if (widget_is_valid(pdt->hPushSave))
	{
		widget_destroy(pdt->hPushSave);
	}

	if (widget_is_valid(pdt->hPushOpen))
	{
		widget_destroy(pdt->hPushOpen);
	}

	if (widget_is_valid(pdt->hPushSetup))
	{
		widget_destroy(pdt->hPushSetup);
	}

	if (widget_is_valid(pdt->hPushPrint))
	{
		widget_destroy(pdt->hPushPrint);
	}

	destroy_display_canvas(widget_get_canvas(widget));

	xmem_free(pdt);
}

int PreviewDlg_OnClose(res_win_t widget)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	widget_destroy(widget);

	return 0;
}

void PreviewDlg_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	xsize_t xs;

	xs.fx = PREVIEWDLG_BAR_SPLIT;
	xs.fy = PREVIEWDLG_BAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_reg;

	xr_reg.x = xr.x;
	xr_reg.w = xr.w;
	xr_reg.y = xr.y + xs.cy;
	xr_reg.h = xr.h - xs.cy;

	widget_move(pdt->hSvg, RECTPOINT(&xr_reg));
	widget_size(pdt->hSvg, RECTSIZE(&xr_reg));
	widget_update_client(pdt->hSvg);

	xr.h = xs.cy;
	xs.fx = PREVIEWDLG_BTN_WIDTH;
	xs.fy = PREVIEWDLG_BAR_SPLIT;
	widget_size_to_pt(widget, &xs);

	xrect_t xr_push;

	xr_push.x = xr.x + xr.w - xs.cx - xs.cy;
	xr_push.w = xs.cx;
	xr_push.y = xr.y + xs.cy;
	xr_push.h = xr.h - 2 * xs.cy;

	widget_move(pdt->hPushClose, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushClose);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushLast, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushLast);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushNext, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushNext);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushPrev, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushPrev);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushFirst, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushFirst);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushSaveAll, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushSaveAll);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushSave, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushSave);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushOpen, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushOpen);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushSetup, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushSetup);

	xr_push.x -= (xs.cx + xs.cy);

	widget_move(pdt->hPushPrint, RECTPOINT(&xr_push));
	widget_update_client(pdt->hPushPrint);

	widget_invalid(widget, NULL, 0);
}

void PreviewDlg_OnPaint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	res_dc_t rdc;
	xbrush_t xb = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr, xr_bar;
	xsize_t xs;

	xhand_t canv;

	widget_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	xs.fx = PREVIEWDLG_BAR_SPLIT;
	xs.fy = PREVIEWDLG_BAR_HEIGHT;

	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y;
	xr_bar.w = xr.w;
	xr_bar.h = xs.cy;

	xscpy(xg.brim_color, xb.color);
	xscpy(xg.core_color, xb.color);
	lighten_xgradi(&xg, DEF_MIDD_DARKEN);
	xscpy(xg.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	gradient_rect_raw(rdc, &xg, &xr_bar);

	end_display_paint(canv, dc, pxr);
}

void PreviewDlg_OnCommandSwitch(res_win_t widget, bool_t bkey)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	widget_send_command(pdt->hSvg, COMMAND_SWITCH, IDC_PARENT, (void*)bkey);
}

void PreviewDlg_OnMenuCommand(res_win_t widget, int code, int cid, void* data)
{
	PreviewDlgDelta* pdt = GETPREVIEWDLGDELTA(widget);

	XDC_ASSERT(pdt != NULL);

	switch (cid)
	{
	case IDC_PREVIEWDLG_CLOSE:
		widget_close(widget, 0);
		break;
	case IDC_PREVIEWDLG_SAVE:
		PreviewDlg_Save(widget);
		break;
	case IDC_PREVIEWDLG_SETUP:
		PreviewDlg_Setup(widget);
		break;
	case IDC_PREVIEWDLG_PRINT:
		PreviewDlg_Print(widget);
		break;
	}
}
/***********************************************************************************************************/
res_win_t PreviewDlg_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, PREVIEWDLGDATA* ppd)
{
	if_event_t ev = { 0 };

	ev.data = (void*)ppd;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(PreviewDlg_OnCreate)
	EVENT_ON_DESTROY(PreviewDlg_OnDestroy)
	EVENT_ON_CLOSE(PreviewDlg_OnClose)

	EVENT_ON_PAINT(PreviewDlg_OnPaint)

	EVENT_ON_SIZE(PreviewDlg_OnSize)
	EVENT_ON_MENU_COMMAND(PreviewDlg_OnMenuCommand)
	EVENT_ON_COMMAND_SWITCH(PreviewDlg_OnCommandSwitch)

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, NULL, &ev);
}