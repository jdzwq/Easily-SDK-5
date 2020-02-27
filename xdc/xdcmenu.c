/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc menu utility document

	@module	xdcmenu.c | implement file

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

#include "xdcmenu.h"
#include "xdcimp.h"
#include "xdcbox.h"

void textor_menu(res_win_t widget, const xpoint_t* ppt, int lay)
{
	res_win_t editmenu;
	link_t_ptr ptr_menu;
	link_t_ptr ilk;

	xrect_t xr = { 0 };

	clr_mod_t clr;

	widget_get_color_mode(widget, &clr);

	ptr_menu = create_menu_doc();
	set_menu_layer(ptr_menu, ATTR_LAYER_HORZ);
	set_menu_show(ptr_menu, ATTR_SHOW_TEXTONLY);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_iid(ilk, COMMAND_UNDO);
	set_menu_item_title(ilk, TEXTOR_MENU_UNDO);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_iid(ilk, COMMAND_CUT);
	set_menu_item_title(ilk, TEXTOR_MENU_CUT);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_iid(ilk, COMMAND_PASTE);
	set_menu_item_title(ilk, TEXTOR_MENU_PASTE);

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_iid(ilk, COMMAND_COPY);
	set_menu_item_title(ilk, TEXTOR_MENU_COPY);

	editmenu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_NOACTIVE, &xr);
	XDL_ASSERT(editmenu);
	widget_set_user_id(editmenu, IDC_EDITMENU);
	widget_set_owner(editmenu, widget);

	widget_set_color_mode(editmenu, &clr);
	
	menubox_set_data(editmenu, ptr_menu);

	menubox_layout(editmenu, ppt, lay);

	widget_show(editmenu, WD_SHOW_NORMAL);

	widget_do_trace(editmenu);

	destroy_menu_doc(ptr_menu);
}

void fontname_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay)
{
	res_win_t menu;
	xrect_t xr;

	link_t_ptr ent, ptr_string;
	link_t_ptr ilk, ptr_menu;

	int iid = 0;
	clr_mod_t clr;

	widget_get_color_mode(widget, &clr);

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_FAMILY_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ptr_menu = create_menu_doc();
	ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		ilk = insert_menu_item(ptr_menu, LINK_LAST);
		set_menu_item_iid(ilk, ++iid);
		set_menu_item_title(ilk, get_string_entity_val_ptr(ent));

		ent = get_string_next_entity(ptr_string, ent);
	}

	destroy_string_table(ptr_string);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = xr.h = 0;

	menu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL, &xr);
	XDL_ASSERT(menu);
	widget_set_user_id(menu, idc);
	widget_set_owner(menu, widget);

	widget_set_color_mode(menu, &clr);

	menubox_set_data(menu, ptr_menu);

	menubox_layout(menu, ppt, lay);

	widget_show(menu, WD_SHOW_NORMAL);

	widget_do_trace(menu);

	destroy_menu_doc(ptr_menu);
}

void fontname_menu_item(int iid, tchar_t* buf, int max)
{
	link_t_ptr ent, ptr_string;

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_FAMILY_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ent = (iid)? get_string_next_entity(ptr_string, LINK_FIRST) : NULL;
	while (ent && --iid)
	{
		ent = get_string_next_entity(ptr_string, ent);
	}

	if (ent)
	{
		get_string_entity_key(ent, buf, max);
	}

	destroy_string_table(ptr_string);
}

void fontsize_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay)
{
	res_win_t menu;
	xrect_t xr;

	link_t_ptr ent, ptr_string;
	link_t_ptr ilk, ptr_menu;
	int iid = 0;
	clr_mod_t clr;

	widget_get_color_mode(widget, &clr);

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_SIZE_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ptr_menu = create_menu_doc();
	ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		ilk = insert_menu_item(ptr_menu, LINK_LAST);
		set_menu_item_iid(ilk,++iid);
		set_menu_item_title(ilk, get_string_entity_val_ptr(ent));

		ent = get_string_next_entity(ptr_string, ent);
	}

	destroy_string_table(ptr_string);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = xr.h = 0;

	menu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL, &xr);
	XDL_ASSERT(menu);
	widget_set_user_id(menu, idc);
	widget_set_owner(menu, widget);

	widget_set_color_mode(menu, &clr);

	menubox_set_data(menu, ptr_menu);

	menubox_layout(menu, ppt, lay);

	widget_show(menu, WD_SHOW_NORMAL);

	widget_do_trace(menu);

	destroy_menu_doc(ptr_menu);
}

void fontsize_menu_item(int iid, tchar_t* buf, int max)
{
	link_t_ptr ent, ptr_string;

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_SIZE_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ent = (iid) ? get_string_next_entity(ptr_string, LINK_FIRST) : NULL;
	while (ent && --iid)
	{
		ent = get_string_next_entity(ptr_string, ent);
	}

	if (ent)
	{
		get_string_entity_key(ent, buf, max);
	}

	destroy_string_table(ptr_string);
}

void fontstyle_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay)
{
	res_win_t menu;
	xrect_t xr;

	link_t_ptr ent, ptr_string;
	link_t_ptr ilk, ptr_menu;
	int iid = 0;
	clr_mod_t clr;

	widget_get_color_mode(widget, &clr);

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_STYLE_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ptr_menu = create_menu_doc();
	ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		ilk = insert_menu_item(ptr_menu, LINK_LAST);
		set_menu_item_iid(ilk, ++iid);
		set_menu_item_title(ilk, get_string_entity_val_ptr(ent));

		ent = get_string_next_entity(ptr_string, ent);
	}

	destroy_string_table(ptr_string);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = xr.h = 0;

	menu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL, &xr);
	XDL_ASSERT(menu);
	widget_set_user_id(menu, idc);
	widget_set_owner(menu, widget);

	widget_set_color_mode(menu, &clr);

	menubox_set_data(menu, ptr_menu);

	menubox_layout(menu, ppt, lay);

	widget_show(menu, WD_SHOW_NORMAL);

	widget_do_trace(menu);

	destroy_menu_doc(ptr_menu);
}

void fontstyle_menu_item(int iid, tchar_t* buf, int max)
{
	link_t_ptr ent, ptr_string;

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_STYLE_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ent = (iid) ? get_string_next_entity(ptr_string, LINK_FIRST) : NULL;
	while (ent && --iid)
	{
		ent = get_string_next_entity(ptr_string, ent);
	}

	if (ent)
	{
		get_string_entity_key(ent, buf, max);
	}

	destroy_string_table(ptr_string);
}

void fontweight_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay)
{
	res_win_t menu;
	xrect_t xr;

	link_t_ptr ent, ptr_string;
	link_t_ptr ilk, ptr_menu;
	int iid = 0;
	clr_mod_t clr;

	widget_get_color_mode(widget, &clr);

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_WEIGHT_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ptr_menu = create_menu_doc();
	ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		ilk = insert_menu_item(ptr_menu, LINK_LAST);
		set_menu_item_iid(ilk, ++iid);
		set_menu_item_title(ilk, get_string_entity_val_ptr(ent));

		ent = get_string_next_entity(ptr_string, ent);
	}

	destroy_string_table(ptr_string);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = xr.h = 0;

	menu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL, &xr);
	XDL_ASSERT(menu);
	widget_set_user_id(menu, idc);
	widget_set_owner(menu, widget);

	widget_set_color_mode(menu, &clr);

	menubox_set_data(menu, ptr_menu);

	menubox_layout(menu, ppt, lay);

	widget_show(menu, WD_SHOW_NORMAL);

	widget_do_trace(menu);

	destroy_menu_doc(ptr_menu);
}

void fontweight_menu_item(int iid, tchar_t* buf, int max)
{
	link_t_ptr ent, ptr_string;

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_FONT_WEIGHT_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ent = (iid) ? get_string_next_entity(ptr_string, LINK_FIRST) : NULL;
	while (ent && --iid)
	{
		ent = get_string_next_entity(ptr_string, ent);
	}

	if (ent)
	{
		get_string_entity_key(ent, buf, max);
	}

	destroy_string_table(ptr_string);
}

void color_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay)
{
	res_win_t menu;
	xrect_t xr;

	link_t_ptr ent, ptr_string;
	link_t_ptr ilk, ptr_menu;
	int iid = 0;
	clr_mod_t clr;

	widget_get_color_mode(widget, &clr);

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_RGB_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ptr_menu = create_menu_doc();
	ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		ilk = insert_menu_item(ptr_menu, LINK_LAST);
		set_menu_item_iid(ilk, ++iid);
		set_menu_item_title(ilk, get_string_entity_val_ptr(ent));

		ent = get_string_next_entity(ptr_string, ent);
	}

	destroy_string_table(ptr_string);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = xr.h = 0;

	menu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL, &xr);
	XDL_ASSERT(menu);
	widget_set_user_id(menu, idc);
	widget_set_owner(menu, widget);

	widget_set_color_mode(menu, &clr);

	menubox_set_data(menu, ptr_menu);

	menubox_layout(menu, ppt, lay);

	widget_show(menu, WD_SHOW_NORMAL);

	widget_do_trace(menu);

	destroy_menu_doc(ptr_menu);
}

void color_menu_item(int iid, tchar_t* buf, int max)
{
	link_t_ptr ent, ptr_string;

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_RGB_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ent = (iid) ? get_string_next_entity(ptr_string, LINK_FIRST) : NULL;
	while (ent && --iid)
	{
		ent = get_string_next_entity(ptr_string, ent);
	}

	if (ent)
	{
		get_string_entity_key(ent, buf, max);
	}

	destroy_string_table(ptr_string);
}

void shape_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay)
{
	res_win_t menu;
	xrect_t xr;

	link_t_ptr ent, ptr_string;
	link_t_ptr ilk, ptr_menu;
	int iid = 0;
	clr_mod_t clr;

	widget_get_color_mode(widget, &clr);

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, ATTR_SHAPE_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ptr_menu = create_menu_doc();

	ilk = insert_menu_item(ptr_menu, LINK_LAST);
	set_menu_item_iid(ilk, ++iid);
	set_menu_item_title(ilk, _T("None"));

	ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		ilk = insert_menu_item(ptr_menu, LINK_LAST);
		set_menu_item_iid(ilk, ++iid);
		set_menu_item_title(ilk, get_string_entity_val_ptr(ent));

		ent = get_string_next_entity(ptr_string, ent);
	}
	destroy_string_table(ptr_string);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = xr.h = 0;

	menu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL, &xr);
	XDL_ASSERT(menu);
	widget_set_user_id(menu, idc);
	widget_set_owner(menu, widget);

	widget_set_color_mode(menu, &clr);

	menubox_set_data(menu, ptr_menu);

	menubox_layout(menu, ppt, lay);

	widget_show(menu, WD_SHOW_NORMAL);

	widget_do_trace(menu);

	destroy_menu_doc(ptr_menu);
}

void shape_menu_item(int iid, tchar_t* buf, int max)
{
	link_t_ptr ent, ptr_string;

	if (!iid)
		return;
	else if (iid == 1)
	{
		xscpy(buf, _T(""));
		return;
	}
	else
		iid--;

	ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, ATTR_SHAPE_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	ent = (iid) ? get_string_next_entity(ptr_string, LINK_FIRST) : NULL;
	while (ent && --iid)
	{
		ent = get_string_next_entity(ptr_string, ent);
	}

	if (ent)
	{
		get_string_entity_key(ent, buf, max);
	}

	destroy_string_table(ptr_string);
}

void track_popup_menu(res_win_t widget, dword_t idc, link_t_ptr menu, const xpoint_t* ppt, int lay)
{
	xrect_t xr = { 0 };
	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_NOACTIVE, &xr);
	widget_set_user_id(hMenu, idc);
	widget_set_owner(hMenu, widget);

	menubox_set_data(hMenu, menu);

	xpoint_t pt;
	pt.x = ppt->x;
	pt.y = ppt->y;
	menubox_layout(hMenu, &pt, WD_LAYOUT_RIGHTBOTTOM);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);
	widget_set_color_mode(hMenu, &clr);
	widget_update(hMenu);
	widget_show(hMenu, WD_SHOW_NORMAL);

	widget_do_trace(hMenu);
}
