/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc preview dialog document

	@module	previewdlg.c | preview dialog widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#include "xdcdlg.h"
#include "handler.h"
#include "winnc.h"
#include "xdcctrl.h"
#include "xdcbox.h"

#define IDC_PREVIEWDLG_SVGCTRL			10
#define IDC_PREVIEWDLG_TREECTRL			11
#define IDC_PREVIEWDLG_STATUSCTRL		12

#define IDC_PREVIEWDLG_PUSHBOX_CLOSE	21
#define IDC_PREVIEWDLG_PUSHBOX_SAVEAS	22
#define IDC_PREVIEWDLG_PUSHBOX_SAVE		23
#define IDC_PREVIEWDLG_PUSHBOX_OPEN		24
#define IDC_PREVIEWDLG_PUSHBOX_PRINT_SEL	25
#define IDC_PREVIEWDLG_PUSHBOX_PRINT_CUR	26
#define IDC_PREVIEWDLG_PUSHBOX_PRINT_ALL	27
#define IDC_PREVIEWDLG_PUSHBOX_SETUP	28

#define IDN_PREVIEWDLG_STATUS_PAPER		_T("paper")
#define IDN_PREVIEWDLG_STATUS_PAGE		_T("page")
#define IDN_PREVIEWDLG_STATUS_NAVI		_T("navi")

#define PREVIEWDLG_TOOL_HEIGHT		(float)8
#define PREVIEWDLG_STATUS_HEIGHT	(float)8
#define PREVIEWDLG_TREE_WIDTH		(float)50

#define PREVIEWDLG_BUTTON_HEIGHT	(float)6 //tm
#define PREVIEWDLG_BUTTON_WIDTH		(float)20 //tm

typedef struct _previewdlg_delta_t{
	link_t_ptr arch;
	dev_prn_t mod;

	res_win_t svgctrl;
	res_win_t treectrl;
	res_win_t statusctrl;
	res_win_t navibox;
}previewdlg_delta_t;

#define GETPREVIEWDLGDELTA(ph) 	(previewdlg_delta_t*)widget_get_user_delta(ph)
#define SETPREVIEWDLGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/************************************************************************************/

void _previewdlg_fresh_tree(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	link_t_ptr st_src = NULL;
	link_t_ptr st_dst = NULL;

	link_t_ptr plk, ilk, tree;
	link_t_ptr nlk;

	tree = treectrl_fetch(ptd->treectrl);
	clear_tree_doc(tree);

	plk = insert_tree_item(tree, LINK_LAST);
	set_tree_item_title(plk, PREVIEWDLG_TREE_TITLE);
	set_tree_item_showcheck(plk, 1);
	set_tree_item_expanded(plk, 1);

	nlk = get_arch_first_child_item(ptd->arch);
	while (nlk)
	{
		ilk = insert_tree_item(plk, LINK_LAST);

		set_tree_item_title(ilk, get_arch_item_title_ptr(nlk));
		set_tree_item_showcheck(ilk, 1);
		if (is_arch_document(ptd->arch,nlk))
		{
			set_tree_item_delta(ilk, (var_long)fetch_arch_document(nlk));
		}

		if (is_arch_catalog(ptd->arch, nlk) && get_dom_first_child_node(nlk))
		{
			if (!st_src)
				st_src = create_stack_table();
			push_stack_node(st_src, (void*)nlk);

			nlk = get_arch_first_child_item(nlk);

			if (!st_dst)
				st_dst = create_stack_table();
			push_stack_node(st_dst, plk);
			plk = ilk;

			continue;
		}

		while (nlk)
		{
			nlk = get_arch_next_sibling_item(nlk);
			if (nlk)
				break;
			nlk = (st_src) ? (link_t_ptr)pop_stack_node(st_src) : NULL;
			plk = (st_dst) ? (link_t_ptr)pop_stack_node(st_dst) : NULL;
		}
	}

	if (st_src)
		destroy_stack_table(st_src);

	if (st_dst)
		destroy_stack_table(st_dst);

	treectrl_redraw(ptd->treectrl);
}

void _previewdlg_fresh_info(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	tchar_t title[RES_LEN] = { 0 };
	link_t_ptr svg, plk;
	link_t_ptr status, ilk;
	int page, count = 0;

	svg = svgctrl_fetch(ptd->svgctrl);

	page = 0;
	plk = get_arch_first_child_item(ptd->arch);
	while (plk)
	{
		count++;

		if (fetch_arch_document(plk) == svg)
			page = count;

		plk = get_arch_next_sibling_item(plk);
	}

	status = statusctrl_fetch(ptd->statusctrl);

	if (count)
		xsprintf(title, PREVIEWDLG_STATUS_PAGEGUID, page, count);
	else
		xscpy(title, _T(""));

	ilk = get_status_item(status, IDN_PREVIEWDLG_STATUS_PAGE);
	set_status_item_title(ilk, title);

	xsprintf(title, _T("%d,%dmm"), ptd->mod.paper_width, ptd->mod.paper_height);
	
	ilk = get_status_item(status, IDN_PREVIEWDLG_STATUS_PAPER);
	set_status_item_title(ilk, title);

	statusctrl_redraw(ptd->statusctrl);
}

/************************************************************************************/
void previewdlg_on_close(res_win_t widget)
{
	widget_close(widget, 0);
}

void previewdlg_on_printcur(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	res_win_t svgctrl;
	link_t_ptr svg;

	svgctrl = widget_get_child(widget, IDC_PREVIEWDLG_SVGCTRL);

	svg = svgctrl_fetch(svgctrl);

	print_svg(&ptd->mod, svg);
}

typedef struct _PREVIEWDLG_ENUM_PARAM{
	res_win_t widget;
	bool_t b_sel;
}PREVIEWDLG_ENUM_PARAM;

static bool_t _enum_tree_item(link_t_ptr ilk, void* param)
{
	PREVIEWDLG_ENUM_PARAM* pp = (PREVIEWDLG_ENUM_PARAM*)param;
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(pp->widget);
	link_t_ptr svg = NULL;

	if (pp->b_sel && get_tree_item_checked(ilk))
		svg = (link_t_ptr)get_tree_item_delta(ilk);
	else if (!pp->b_sel)
		svg = (link_t_ptr)get_tree_item_delta(ilk);
	else
		svg = NULL;

	if (svg)
	{
		print_svg(&ptd->mod, svg);
	}

	return 1;
}

void previewdlg_on_printsel(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	res_win_t treectrl;
	link_t_ptr tree;
	PREVIEWDLG_ENUM_PARAM pa = { 0 };

	treectrl = widget_get_child(widget, IDC_PREVIEWDLG_TREECTRL);
	tree = treectrl_fetch(treectrl);

	pa.b_sel = 1;
	pa.widget = widget;
	enum_dom_node(tree, (CALLBACK_ENUMLINK)_enum_tree_item, (void*)&pa);
}

void previewdlg_on_printall(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	res_win_t treectrl;
	link_t_ptr tree;
	PREVIEWDLG_ENUM_PARAM pa = { 0 };

	treectrl = widget_get_child(widget, IDC_PREVIEWDLG_TREECTRL);
	tree = treectrl_fetch(treectrl);

	pa.b_sel = 0;
	pa.widget = widget;
	enum_dom_node(tree, (CALLBACK_ENUMLINK)_enum_tree_item, (void*)&pa);
}

void previewdlg_on_setup(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	res_win_t svgctrl;
	link_t_ptr svg;

	setup_printer_mode(widget, &ptd->mod);

	svgctrl = widget_get_child(widget, IDC_PREVIEWDLG_SVGCTRL);

	svg = svgctrl_fetch(svgctrl);

	if (ptd->mod.landscape)
	{
		set_svg_width(svg, (float)ptd->mod.paper_height / 10);
		set_svg_height(svg, (float)ptd->mod.paper_width / 10);
	}
	else
	{
		set_svg_width(svg, (float)ptd->mod.paper_width / 10);
		set_svg_height(svg, (float)ptd->mod.paper_height / 10);
	}

	svgctrl_redraw(svgctrl);

	_previewdlg_fresh_info(widget);
}

void previewdlg_on_saveas(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szName[RES_LEN] = { 0 };

	link_t_ptr svg;

	svg = svgctrl_fetch(ptd->svgctrl);
	if (!svg)
		return;

	shell_get_docpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("svg view file(*.svg)\0*.svg\0"), _T("svg"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);

	save_dom_doc_to_file(svg, NULL, szPath);
}

void previewdlg_on_save(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szName[RES_LEN] = { 0 };

	if(!get_arch_item_count(ptd->arch))
		return;

	shell_get_docpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("svg archive file(*.arch)\0*.arch\0"), _T("arch"), 1, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);

	save_dom_doc_to_file(ptd->arch, NULL, szPath);
}

void previewdlg_on_open(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN * 10] = { 0 };
	tchar_t szName[RES_LEN] = { 0 };

	res_win_t svgctrl;

	shell_get_docpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, _T("svg archive file(*.arch)\0*.arch\0"), _T("arch"), 0, szPath, PATH_LEN, szFile, PATH_LEN * 10))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);

	svgctrl = widget_get_child(widget, IDC_PREVIEWDLG_SVGCTRL);
	svgctrl_detach(svgctrl);

	clear_arch_doc(ptd->arch);

	load_dom_doc_from_file(ptd->arch, NULL, szPath);
	
	_previewdlg_fresh_tree(widget);

	_previewdlg_fresh_info(widget);
}

void previewdlg_on_select(res_win_t widget, link_t_ptr ilk)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	if (!ilk)
		return;

	svgctrl_attach(ptd->svgctrl, ilk);

	_previewdlg_fresh_info(widget);
}

/**********************************************************************************/
void _previewdlg_calc_toolbar(res_win_t widget, xrect_t* pxr)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	widget_get_dock_rect(widget, WD_DOCK_TOP, pxr);
}

void _previewdlg_calc_treebar(res_win_t widget, xrect_t* pxr)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	widget_get_dock_rect(widget, WD_DOCK_LEFT, pxr);
}

void _previewdlg_calc_statusbar(res_win_t widget, xrect_t* pxr)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	widget_get_dock_rect(widget, WD_DOCK_BOTTOM, pxr);
}

void _previewdlg_calc_svgbar(res_win_t widget, xrect_t* pxr)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	
	widget_get_dock_rect(widget, 0, pxr);
}

void _previewdlg_create_toolbar(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	res_win_t pushbox;
	xrect_t xr;
	xsize_t xs;
	long nSplit;

	_previewdlg_calc_toolbar(widget, &xr);

	xs.fx = PREVIEWDLG_BUTTON_WIDTH;
	xs.fy = PREVIEWDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr.y += (xr.h - xs.cy) / 2;
	xr.h = xs.cy;
	xr.x = xr.x + xr.w - xs.cx;
	xr.w = xs.cx;

	xs.fx = DEF_SPLIT_FEED;
	xs.fy = 0;
	widget_size_to_pt(widget, &xs);
	nSplit = xs.cx;

	pt_expand_rect(&xr, -xs.cx, -xs.cy);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_CLOSE);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_CLOSE, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_PRINT_CUR);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_PRINTCUR, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_PRINT_SEL);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_PRINTSEL, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_PRINT_ALL);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_PRINTALL, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_SETUP);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_SETUP, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_SAVEAS);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_SAVEAS, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_SAVE);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_SAVE, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);
	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_PREVIEWDLG_PUSHBOX_OPEN);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, PREVIEWDLG_PUSHBOX_OPEN, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);
}

void _previewdlg_create_treebar(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	xrect_t xr;
	link_t_ptr ptr_tree;

	_previewdlg_calc_treebar(widget, &xr);

	ptd->treectrl = treectrl_create(_T("TreeBar"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(ptd->treectrl, IDC_PREVIEWDLG_TREECTRL);
	widget_set_owner(ptd->treectrl, widget);

	ptr_tree = create_tree_doc();

	treectrl_attach(ptd->treectrl, ptr_tree);
	treectrl_set_lock(ptd->treectrl, 1);

	widget_show(ptd->treectrl, WD_SHOW_NORMAL);
}

void _previewdlg_create_statusbar(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	xrect_t xr;
	link_t_ptr ilk, ptr_status;

	_previewdlg_calc_statusbar(widget, &xr);

	ptd->statusctrl = statusctrl_create(_T("StatusBar"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(ptd->statusctrl, IDC_PREVIEWDLG_STATUSCTRL);
	widget_set_owner(ptd->statusctrl, widget);

	ptr_status = create_status_doc();
	set_status_alignment(ptr_status, ATTR_ALIGNMENT_FAR);

	ilk = insert_status_item(ptr_status, LINK_LAST);
	set_status_item_name(ilk, IDN_PREVIEWDLG_STATUS_NAVI);
	set_status_item_width(ilk, DEF_TOUCH_SPAN * 6 + 1);

	ilk = insert_status_item(ptr_status, LINK_LAST);
	set_status_item_name(ilk, IDN_PREVIEWDLG_STATUS_PAGE);
	set_status_item_width(ilk, 30);

	ilk = insert_status_item(ptr_status, LINK_LAST);
	set_status_item_name(ilk, IDN_PREVIEWDLG_STATUS_PAPER);
	set_status_item_width(ilk, 30);

	statusctrl_attach(ptd->statusctrl, ptr_status);

	widget_show(ptd->statusctrl, WD_SHOW_NORMAL);

	ilk = get_status_item(ptr_status, IDN_PREVIEWDLG_STATUS_NAVI);
	statusctrl_get_item_rect(ptd->statusctrl, ilk, &xr);
	ptd->navibox = navibox_create(ptd->statusctrl, WD_STYLE_CONTROL, &xr);
	widget_set_owner(ptd->navibox, widget);
	widget_show(ptd->navibox, WD_SHOW_NORMAL);
}

void _previewdlg_create_svgbar(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	xrect_t xr;

	_previewdlg_calc_svgbar(widget, &xr);

	ptd->svgctrl = svgctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_VSCROLL | WD_STYLE_HSCROLL | WD_STYLE_PAGING, &xr, widget);
	widget_set_owner(ptd->svgctrl, widget);
	widget_set_user_id(ptd->svgctrl, IDC_PREVIEWDLG_SVGCTRL);

	widget_show(ptd->svgctrl, WD_SHOW_NORMAL);
}

/*******************************************************************************/
int hand_previewdlg_create(res_win_t widget, void* data)
{
	previewdlg_delta_t* ptd;
	xsize_t xs;

	widget_hand_create(widget);

	ptd = (previewdlg_delta_t*)xmem_alloc(sizeof(previewdlg_delta_t));
	xmem_zero((void*)ptd, sizeof(previewdlg_delta_t));

	SETPREVIEWDLGDELTA(widget, ptd);

	ptd->arch = create_arch_doc();

	xs.fx = 0;
	xs.fy = PREVIEWDLG_TOOL_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_TOP, 0, xs.cy);

	xs.fx = 0;
	xs.fy = PREVIEWDLG_STATUS_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_BOTTOM, 0, xs.cy);

	xs.fx = PREVIEWDLG_TREE_WIDTH;
	xs.fy = 0;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_LEFT | WD_DOCK_DYNA, xs.cx, 0);

	_previewdlg_create_toolbar(widget);

	_previewdlg_create_treebar(widget);

	_previewdlg_create_statusbar(widget);

	_previewdlg_create_svgbar(widget);

	widget_set_focus(ptd->svgctrl);

	return 0;
}

void hand_previewdlg_destroy(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	res_win_t pushbox;
	link_t_ptr ptr;

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->svgctrl))
		widget_destroy(ptd->svgctrl);

	if (widget_is_valid(ptd->navibox))
		widget_destroy(ptd->navibox);

	if (widget_is_valid(ptd->treectrl))
	{
		ptr = treectrl_detach(ptd->treectrl);
		destroy_tree_doc(ptr);
	}

	if (widget_is_valid(ptd->statusctrl))
	{
		ptr = statusctrl_detach(ptd->statusctrl);
		destroy_status_doc(ptr);
	}

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_CLOSE);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_PRINT_CUR);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_PRINT_SEL);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_PRINT_ALL);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_SETUP);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_SAVEAS);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_SAVE);
	if (pushbox)
		widget_destroy(pushbox);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_OPEN);
	if (pushbox)
		widget_destroy(pushbox);

	if (ptd->arch)
		destroy_arch_doc(ptd->arch);

	xmem_free(ptd);

	SETPREVIEWDLGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_previewdlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	xsize_t xs;
	xrect_t xr;
	res_win_t pushbox;
	long nSplit;
	link_t_ptr status, ilk;

	_previewdlg_calc_treebar(widget, &xr);

	if (widget_is_valid(ptd->treectrl))
	{
		widget_move(ptd->treectrl, RECTPOINT(&xr));
		widget_size(ptd->treectrl, RECTSIZE(&xr));
	}

	_previewdlg_calc_statusbar(widget, &xr);

	if (widget_is_valid(ptd->statusctrl))
	{
		widget_move(ptd->statusctrl, RECTPOINT(&xr));
		widget_size(ptd->statusctrl, RECTSIZE(&xr));

		status = statusctrl_fetch(ptd->statusctrl);
		ilk = get_status_item(status, IDN_PREVIEWDLG_STATUS_NAVI);
		statusctrl_get_item_rect(ptd->statusctrl, ilk, &xr);

		widget_move(ptd->navibox, RECTPOINT(&xr));
		widget_size(ptd->navibox, RECTSIZE(&xr));
	}

	_previewdlg_calc_svgbar(widget, &xr);

	if (widget_is_valid(ptd->svgctrl))
	{
		widget_move(ptd->svgctrl, RECTPOINT(&xr));
		widget_size(ptd->svgctrl, RECTSIZE(&xr));
	}

	_previewdlg_calc_toolbar(widget, &xr);

	xs.fx = PREVIEWDLG_BUTTON_WIDTH;
	xs.fy = PREVIEWDLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr.y += (xr.h - xs.cy) / 2;
	xr.h = xs.cy;
	xr.x = xr.x + xr.w - xs.cx;
	xr.w = xs.cx;

	xs.fx = DEF_SPLIT_FEED;
	xs.fy = 0;
	widget_size_to_pt(widget, &xs);
	nSplit = xs.cx;

	pt_expand_rect(&xr, -xs.cx, -xs.cy);

	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_CLOSE);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);
	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_PRINT_CUR);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);
	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_PRINT_SEL);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);
	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_PRINT_ALL);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);
	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_SETUP);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);
	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_SAVEAS);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);
	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_SAVE);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);
	pushbox = widget_get_child(widget, IDC_PREVIEWDLG_PUSHBOX_OPEN);
	if (widget_is_valid(pushbox))
	{
		widget_move(pushbox, RECTPOINT(&xr));
		widget_size(pushbox, RECTSIZE(&xr));
	}

	widget_invalid(widget, NULL, 0);
}

void hand_previewdlg_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	switch (cid)
	{
	case IDC_PREVIEWDLG_PUSHBOX_CLOSE:
		previewdlg_on_close(widget);
		break;
	case IDC_PREVIEWDLG_PUSHBOX_PRINT_CUR:
		previewdlg_on_printcur(widget);
		break;
	case IDC_PREVIEWDLG_PUSHBOX_PRINT_SEL:
		previewdlg_on_printsel(widget);
		break;
	case IDC_PREVIEWDLG_PUSHBOX_PRINT_ALL:
		previewdlg_on_printall(widget);
		break;
	case IDC_PREVIEWDLG_PUSHBOX_SETUP:
		previewdlg_on_setup(widget);
		break;
	case IDC_PREVIEWDLG_PUSHBOX_SAVEAS:
		previewdlg_on_saveas(widget);
		break;
	case IDC_PREVIEWDLG_PUSHBOX_SAVE:
		previewdlg_on_save(widget);
		break;
	case IDC_PREVIEWDLG_PUSHBOX_OPEN:
		previewdlg_on_open(widget);
		break;
	}
}

void hand_previewdlg_notice(res_win_t widget, LPNOTICE phdr)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	if (phdr->id == IDC_PREVIEWDLG_TREECTRL)
	{
		NOTICE_TREE* pnt = (NOTICE_TREE*)phdr;
		switch (pnt->code)
		{
		case NC_TREELBCLK:
			if (pnt->item)
			{
				previewdlg_on_select(widget, (link_t_ptr)get_tree_item_delta(pnt->item));
			}
			break;
		}
	}
}
/***************************************************************************************/
res_win_t previewdlg_create(const tchar_t* title, res_win_t owner)
{
	res_win_t dlg;
	xrect_t xr = { 0 };
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_previewdlg_create)
		EVENT_ON_DESTROY(hand_previewdlg_destroy)

		EVENT_ON_SIZE(hand_previewdlg_size)

		EVENT_ON_NOTICE(hand_previewdlg_notice)

		EVENT_ON_MENU_COMMAND(hand_previewdlg_menu_command)

		EVENT_ON_NC_IMPLEMENT

		EVENT_ON_DOCKER_IMPLEMENT

	EVENT_END_DISPATH

	dlg = widget_create(NULL, WD_STYLE_DIALOG | WD_STYLE_DOCKER, &xr, owner, &ev);
	widget_set_owner(dlg, owner);
	widget_set_user_id(dlg, IDC_PREVIEWDLG);
	widget_set_title(dlg, title);

	return dlg;
}

link_t_ptr previewdlg_get_arch(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);

	return ptd->arch;
}

void previewdlg_redraw(res_win_t widget)
{
	previewdlg_delta_t* ptd = GETPREVIEWDLGDELTA(widget);
	link_t_ptr plk;

	svgctrl_detach(ptd->svgctrl);

	_previewdlg_fresh_tree(widget);

	_previewdlg_fresh_info(widget);

	plk = get_arch_first_child_item(ptd->arch);
	if (!plk)
		return;

	if (!is_arch_document(ptd->arch, plk))
		return;

	svgctrl_attach(ptd->svgctrl, fetch_arch_document(plk));
}
