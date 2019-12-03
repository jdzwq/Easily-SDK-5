/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc box document

	@module	xdcbox.h | interface file

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

#ifndef _XDCBOX_H
#define _XDCBOX_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/******************************plot box*****************************************************/

/*
@FUNCTION plotbox_create: create a plotbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t plotbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION plotbox_popup_size: calcing the plotbox popup size according to client size.
@INPUT res_win_t widget: the plotbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void plotbox_popup_size(res_win_t widget, xsize_t* pxs);

/******************************numeric box*****************************************************/

/*
@FUNCTION numbox_create: create a numbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t numbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION numbox_popup_size: calcing the numbox popup size according to client size.
@INPUT res_win_t widget: the numbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void numbox_popup_size(res_win_t widget, xsize_t* pxs);

/******************************naviguid box*****************************************************/

/*
@FUNCTION navibox_create: create a navibox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t navibox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION navibox_set_target: binding a navibox to target widget.
@INPUT res_win_t widget: the navibox widget.
@INPUT res_win_t target: the target widget.
@RETURN void: none.
*/
XDC_API void navibox_set_target(res_win_t widget, res_win_t target);

/*
@FUNCTION navibox_popup_size: calcing the navibox popup size according to client size.
@INPUT res_win_t widget: the navibox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void navibox_popup_size(res_win_t widget, xsize_t* pxs);

/******************************vert box*****************************************************/

/*
@FUNCTION vertbox_create: create a vertbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t vertbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION vertbox_set_target: binding a vertbox to target widget.
@INPUT res_win_t widget: the vertbox widget.
@INPUT res_win_t target: the target widget.
@RETURN void: none.
*/
XDC_API void vertbox_set_target(res_win_t widget, res_win_t target);

/*
@FUNCTION vertbox_popup_size: calcing the vertbox popup size according to client size.
@INPUT res_win_t widget: the vertbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void vertbox_popup_size(res_win_t widget, xsize_t* pxs);

XDL_API res_win_t show_vertbox(res_win_t owner);

/******************************horz box*****************************************************/

/*
@FUNCTION horzbox_create: create a horzbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t horzbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION horzbox_set_target: binding a horzbox to target widget.
@INPUT res_win_t widget: the horzbox widget.
@INPUT res_win_t target: the target widget.
@RETURN void: none.
*/
XDC_API void horzbox_set_target(res_win_t widget, res_win_t target);

/*
@FUNCTION horzbox_popup_size: calcing the horzbox popup size according to client size.
@INPUT res_win_t widget: the horzbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void horzbox_popup_size(res_win_t widget, xsize_t* pxs);

XDL_API res_win_t show_horzbox(res_win_t owner);

/******************************tip box*****************************************************/

/*
@FUNCTION tipbox_create: create a tipbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT type: the tipbox type.
@INPUT const tchar_t* text: the tip text token.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t tipbox_create(res_win_t widget, dword_t style, const xrect_t* pxr, int type, const tchar_t* text);

/*
@FUNCTION tipbox_popup_size: calcing the tipbox popup size according to client size.
@INPUT res_win_t widget: the tipbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void tipbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION tipbox_set_text: set the tipbox text.
@INPUT res_win_t widget: the tipbox widget.
@INPUT const tchar_t* sz_text: the text token.
@RETURN void: none.
*/
XDC_API void tipbox_set_text(res_win_t widget, const tchar_t* sz_text);

/*
@FUNCTION show_toolbox: create and popup a tipbox widget.
@INPUT const xpoint_t* ppt: the position in screen coordiante.
@INPUT const tchar_t* sz_text: the tip text token.
@RETURN void: none.
*/
XDC_API res_win_t show_toolbox(const xpoint_t* ppt, const tchar_t* sz_text);

/*
@FUNCTION reset_toolbox: reset the tipbox position and text.
@INPUT res_win_t widget: the tipbox widget.
@INPUT const xpoint_t* ppt: the position in screen coordiante.
@INPUT const tchar_t* sz_text: the tip text token.
@RETURN void: none.
*/
XDC_API bool_t reset_toolbox(res_win_t widget, const xpoint_t* ppt, const tchar_t* sz_text);

/******************************date box*****************************************************/

/*
@FUNCTION datebox_create: create a datebox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t datebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION datebox_popup_size: calcing the datebox popup size according to client size.
@INPUT res_win_t widget: the datebox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void datebox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION datebox_set_date: set the datebox date value.
@INPUT res_win_t widget: the datebox widget.
@INPUT const xdate_t* pxd: the date struct.
@RETURN void: none.
*/
XDC_API void datebox_set_date(res_win_t widget, const xdate_t* pxd);

/*
@FUNCTION datebox_get_date: get the datebox date value.
@INPUT res_win_t widget: the datebox widget.
@OUTPUT xdate_t* pxd: the date struct.
@RETURN void: none.
*/
XDC_API void datebox_get_date(res_win_t widget, xdate_t* pxd);


/******************************time box*****************************************************/

/*
@FUNCTION timebox_create: create a timebox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t timebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION timebox_popup_size: calcing the datebox popup size according to client size.
@INPUT res_win_t widget: the timebox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void timebox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION timebox_set_datetime: set the datebox datetime value.
@INPUT res_win_t widget: the datebox widget.
@INPUT const xdate_t* pxd: the date struct.
@RETURN void: none.
*/
XDC_API void timebox_set_datetime(res_win_t widget, const xdate_t* pxt);

/*
@FUNCTION timebox_get_datetime: get the datebox datetime value.
@INPUT res_win_t widget: the datebox widget.
@OUTPUT xdate_t* pxd: the date struct.
@RETURN void: none.
*/
XDC_API void timebox_get_datetime(res_win_t widget, xdate_t* pxt);

/******************************push box*****************************************************/

/*
@FUNCTION pushbox_create: create a pushbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t pushbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION pushbox_popup_size: calcing the pushbox popup size according to client size.
@INPUT res_win_t widget: the pushbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void pushbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION pushbox_set_state: set the pushbox check state.
@INPUT res_win_t widget: the datebox widget.
@INPUT bool_t b_check: nonzero for checked, zero for unchecked.
@RETURN void: none.
*/
XDC_API void pushbox_set_state(res_win_t widget, bool_t b_check);

/*
@FUNCTION pushbox_get_state: get the pushbox check state.
@INPUT res_win_t widget: the datebox widget.
@RETURN bool_t: return nonzero for checked, zero for unchecked.
*/
XDC_API bool_t pushbox_get_state(res_win_t widget);

/*
@FUNCTION pushbox_set_text: set the pushbox text.
@INPUT res_win_t widget: the datebox widget.
@INPUT const tchar_t* text: the text token.
@INPUT int len: the text length in characters.
@RETURN void: none.
*/
XDC_API void pushbox_set_text(res_win_t widget, const tchar_t* text, int len);

/******************************spin box*****************************************************/

/*
@FUNCTION spinbox_create: create a spinbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t spinbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION spinbox_popup_size: calcing the pushbox popup size according to client size.
@INPUT res_win_t widget: the spinbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void spinbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION spinbox_set_spin: set the spinbox currnet value.
@INPUT res_win_t widget: the spinbox widget.
@INPUT int cur: the spin value.
@RETURN void: none.
*/
XDC_API void spinbox_set_spin(res_win_t widget, int cur);

/*
@FUNCTION spinbox_get_spin: get the spinbox value.
@INPUT res_win_t widget: the spinbox widget.
@RETURN int: return the spin value.
*/
XDC_API int spinbox_get_spin(res_win_t widget);

/******************************slide box*****************************************************/

/*
@FUNCTION slidebox_create: create a slidbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t slidebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION slidebox_popup_size: calcing the slidbox popup size according to client size.
@INPUT res_win_t widget: the spinbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void slidebox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION slidebox_set_slide: set the slidebox currnet value.
@INPUT res_win_t widget: the slidebox widget.
@INPUT int pos: the slide value.
@RETURN void: none.
*/
XDC_API void slidebox_set_slide(res_win_t widget, int pos);

/*
@FUNCTION slidebox_get_slide: get the slidebox value.
@INPUT res_win_t widget: the slidebox widget.
@RETURN int: return the slide value.
*/
XDC_API int slidebox_get_slide(res_win_t widget);

/******************************radio box*****************************************************/

/*
@FUNCTION radiobox_create: create a radiobox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t radiobox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION radiobox_popup_size: calcing the radiobox popup size according to client size.
@INPUT res_win_t widget: the radiobox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void radiobox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION radiobox_set_state: set the radiobox currnet state.
@INPUT res_win_t widget: the slidebox widget.
@INPUT bool_t b_on: the radio value.
@RETURN void: none.
*/
XDC_API void radiobox_set_state(res_win_t widget, bool_t b_on);

/*
@FUNCTION radiobox_get_state: get the radiobox value.
@INPUT res_win_t widget: the radiobox widget.
@RETURN int: return the radio value.
*/
XDC_API bool_t radiobox_get_state(res_win_t widget);

/******************************check box*****************************************************/

/*
@FUNCTION checkbox_create: create a checkbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t checkbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION checkbox_popup_size: calcing the checkbox popup size according to client size.
@INPUT res_win_t widget: the checkbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void checkbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION checkbox_set_state: set the checkbox currnet state.
@INPUT res_win_t widget: the checkbox widget.
@INPUT bool_t b_on: the check value.
@RETURN void: none.
*/
XDC_API void checkbox_set_state(res_win_t widget, bool_t b_on);

/*
@FUNCTION checkbox_get_state: get the checkbox value.
@INPUT res_win_t widget: the checkbox widget.
@RETURN int: return the check value.
*/
XDC_API bool_t checkbox_get_state(res_win_t widget);

/******************************static box*****************************************************/

/*
@FUNCTION staticbox_create: create a staticbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t staticbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION staticbox_popup_size: calcing the staicbox popup size according to client size.
@INPUT res_win_t widget: the staticbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void staticbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION staticbox_set_text: set the staticbox text.
@INPUT res_win_t widget: the staticbox widget.
@INPUT const tchar_t* text: the static text.
@RETURN void: none.
*/
XDC_API void staticbox_set_text(res_win_t widget, const tchar_t* text);

/******************************shape box*****************************************************/

/*
@FUNCTION shapebox_create: create a shapebox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t shapebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION shapebox_set_shape: set the shapebox shape type.
@INPUT res_win_t widget: the staticbox widget.
@INPUT const tchar_t* shape: the shape type.
@RETURN void: none.
*/
XDC_API void shapebox_set_shape(res_win_t widget, const tchar_t* shape);

/******************************icon box*****************************************************/

/*
@FUNCTION iconbox_create: create a iconbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t iconbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION iconbox_popup_size: calcing the iconbox popup size according to client size.
@INPUT res_win_t widget: the iconbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void iconbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION iconbox_set_options: set the iconbox options token.
@INPUT res_win_t widget: the iconbox widget.
@INPUT const tchar_t* opt: the options string token.
@INPUT int len: the options token length in characters.
@RETURN void: none.
*/
XDC_API void iconbox_set_options(res_win_t widget, const tchar_t* opt, int len);

/*
@FUNCTION iconbox_set_layer: set the iconbox layer type.
@INPUT res_win_t widget: the iconbox widget.
@INPUT const tchar_t* layer: the layer string token.
@RETURN void: none.
*/
XDC_API void iconbox_set_layer(res_win_t widget, const tchar_t* layer);

/*
@FUNCTION iconbox_set_alignment: set the iconbox alignment type.
@INPUT res_win_t widget: the iconbox widget.
@INPUT const tchar_t* align: the alignment string token.
@RETURN void: none.
*/
XDC_API void iconbox_set_alignment(res_win_t widget, const tchar_t* align);

/*
@FUNCTION iconbox_get_item_rect: get the iconbox item rect.
@INPUT res_win_t widget: the iconbox widget.
@INPUT const tchar_t* key: the icon name.
@OUTPUT xrect_t* pxr: the rect struct.
@RETURN void: none.
*/
XDC_API void iconbox_get_item_rect(res_win_t widget, const tchar_t* key, xrect_t* pxr);

/******************************list box*****************************************************/

/*
@FUNCTION listbox_create: create a listbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t listbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION listbox_popup_size: calcing the listbox popup size according to client size.
@INPUT res_win_t widget: the iconbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void listbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION listbox_set_options: set the listbox options token.
@INPUT res_win_t widget: the listbox widget.
@INPUT const tchar_t* opt: the options string token.
@INPUT int len: the options token length in characters.
@RETURN void: none.
*/
XDC_API void listbox_set_options(res_win_t widget, const tchar_t* opt, int len);

/*
@FUNCTION listbox_get_cur_key: get the listbox focus item key token.
@INPUT res_win_t widget: the listbox widget.
@RETURN const tchar_t*: return the key token.
*/
XDC_API const tchar_t* listbox_get_cur_key(res_win_t widget);

/*
@FUNCTION listbox_get_cur_val: get the listbox focus item value token.
@INPUT res_win_t widget: the listbox widget.
@RETURN const tchar_t*: return the value token.
*/
XDC_API int listbox_get_cur_val(res_win_t widget, tchar_t* buf, int max);

/*
@FUNCTION listbox_redraw: redraw the listbox.
@INPUT res_win_t widget: the listbox widget.
@RETURN void: none.
*/
XDC_API void listbox_redraw(res_win_t widget);

/*
@FUNCTION listbox_tabskip: tab the listbox focus item to next.
@INPUT res_win_t widget: the listbox widget.
@INPUT int skip: the skip code, it can be WD_TAB_LEFT, WD_TAB_RIGHT, WD_TAB_TOP, WD_TAB_BOTTOM.
@RETURN void: none.
*/
XDC_API void listbox_tabskip(res_win_t widget, int skip);

/*
@FUNCTION listbox_find: find the listbox item by key.
@INPUT res_win_t widget: the listbox widget.
@INPUT const tchar_t* key: the key string token.
@RETURN void: none.
*/
XDC_API void listbox_find(res_win_t widget, const tchar_t* key);

/*
@FUNCTION listbox_filter: filter the listbox item by token.
@INPUT res_win_t widget: the listbox widget.
@INPUT const tchar_t* token: the filter string token.
@RETURN void: none.
*/
XDC_API void listbox_filter(res_win_t widget, const tchar_t* token);

/******************************drop box*****************************************************/

/*
@FUNCTION dropbox_create: create a dropbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t dropbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION dropbox_popup_size: calcing the dropbox popup size according to client size.
@INPUT res_win_t widget: the dropbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void dropbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION dropbox_set_data: set the dropbox options table.
@INPUT res_win_t widget: the dropbox widget.
@INPUT link_t_ptr ptr: the string table link component.
@RETURN void: none.
*/
XDC_API void dropbox_set_data(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION dropbox_get_data: get the dropbox options table.
@INPUT res_win_t widget: the dropbox widget.
@RETURN link_t_ptr: return the string table link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr dropbox_get_data(res_win_t widget);

/*
@FUNCTION dropbox_redraw: redraw the dropbox.
@INPUT res_win_t widget: the dropbox widget.
@RETURN void: none.
*/
XDC_API void dropbox_redraw(res_win_t widget);

/*
@FUNCTION dropbox_tabskip: tab the dropbox focus item to next.
@INPUT res_win_t widget: the dropbox widget.
@INPUT int skip: the skip code, it can be WD_TAB_LEFT, WD_TAB_RIGHT, WD_TAB_TOP, WD_TAB_BOTTOM.
@RETURN void: none.
*/
XDC_API void dropbox_tabskip(res_win_t widget, int skip);

/*
@FUNCTION dropbox_set_focus_item: set the dropbox focus item.
@INPUT res_win_t widget: the dropbox widget.
@INPUT link_t_ptr ilk: the string entity link component.
@RETURN void: none.
*/
XDC_API void dropbox_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION dropbox_get_focus_item: get the dropbox focus item.
@INPUT res_win_t widget: the dropbox widget.
@RETURN link_t_ptr: return the string entity link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr dropbox_get_focus_item(res_win_t widget);

/*
@FUNCTION dropbox_find: find the dropbox item by key.
@INPUT res_win_t widget: the dropbox widget.
@INPUT const tchar_t* key: the key string token.
@RETURN void: none.
*/
XDC_API void dropbox_find(res_win_t widget, const tchar_t* token);

/*
@FUNCTION dropbox_filter: filter the dropbox item by token.
@INPUT res_win_t widget: the dropbox widget.
@INPUT const tchar_t* token: the filter string token.
@RETURN void: none.
*/
XDC_API void dropbox_filter(res_win_t widget, const tchar_t* token);

/******************************words box*****************************************************/

/*
@FUNCTION wordsbox_create: create a wordsbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t wordsbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION wordsbox_popup_size: calcing the wordsbox popup size according to client size.
@INPUT res_win_t widget: the wordsbox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void wordsbox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION wordsbox_set_data: set the wordsbox words table.
@INPUT res_win_t widget: the wordsbox widget.
@INPUT link_t_ptr ptr: the words table link component.
@RETURN void: none.
*/
XDC_API void	wordsbox_set_data(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION wordsbox_get_data: get the wordsbox words table.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN link_t_ptr: return the words table link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr wordsbox_get_data(res_win_t widget);

/*
@FUNCTION wordsbox_redraw: redraw the wordsbox.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN void: none.
*/
XDC_API void	wordsbox_redraw(res_win_t widget);

/*
@FUNCTION wordsbox_tabskip: tab the wordsbox focus item to next.
@INPUT res_win_t widget: the wordsbox widget.
@INPUT int skip: the skip code, it can be WD_TAB_LEFT, WD_TAB_RIGHT, WD_TAB_TOP, WD_TAB_BOTTOM.
@RETURN void: none.
*/
XDC_API void	wordsbox_tabskip(res_win_t widget, int nSkip);

/*
@FUNCTION wordsbox_set_focus_item: set the wordsbox focus item.
@INPUT res_win_t widget: the dropbox widget.
@INPUT link_t_ptr ilk: the words item link component.
@RETURN void: none.
*/
XDC_API bool_t	wordsbox_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION wordsbox_get_focus_item: get the wordsbox focus item.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN link_t_ptr: return the words item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr wordsbox_get_focus_item(res_win_t widget);

/*
@FUNCTION wordsbox_move_first_page: move to the first page.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN void: none.
*/
XDC_API void	wordsbox_move_first_page(res_win_t widget);

/*
@FUNCTION wordsbox_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN void: none.
*/
XDC_API void	wordsbox_move_prev_page(res_win_t widget);

/*
@FUNCTION wordsbox_move_next_page: move to the next page.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN void: none.
*/
XDC_API void	wordsbox_move_next_page(res_win_t widget);

/*
@FUNCTION wordsbox_move_last_page: move to the last page.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN void: none.
*/
XDC_API void	wordsbox_move_last_page(res_win_t widget);

/*
@FUNCTION wordsbox_move_to_page: move to the page.
@INPUT res_win_t widget: the wordsbox widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void	wordsbox_move_to_page(res_win_t widget, int page);

/*
@FUNCTION wordsbox_get_cur_page: get the current page.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN int: return the 1-based page index.
*/
XDC_API int		wordsbox_get_cur_page(res_win_t widget);

/*
@FUNCTION wordsbox_get_max_page: get the maximized page.
@INPUT res_win_t widget: the wordsbox widget.
@RETURN int: return the 1-based page number.
*/
XDC_API int		wordsbox_get_max_page(res_win_t widget);

/*
@FUNCTION wordsbox_find: find wordsbox item and focus it by key from the sratr position.
@INPUT res_win_t widget: the dropbox widget.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@INPUT const tchar_t* key: the key string token.
@RETURN void: none.
*/
XDC_API void wordsbox_find(res_win_t widget, link_t_ptr pos, const tchar_t* token);

/*
@FUNCTION wordsbox_filter: filter wordsbox items by token.
@INPUT res_win_t widget: the dropbox widget.
@INPUT const tchar_t* token: the token string token.
@RETURN void: none.
*/
XDC_API void wordsbox_filter(res_win_t widget, const tchar_t* token);

/*
@FUNCTION wordsbox_seek: focus next wordsbox item by step.
@INPUT res_win_t widget: the dropbox widget.
@INPUT int index: the offset from current.
@RETURN void: none.
*/
XDC_API link_t_ptr wordsbox_seek(res_win_t widget, int index);

/******************************menu box*****************************************************/

/*
@FUNCTION menubox_create: create a menubox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t menubox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION menubox_popup_size: calcing the menubox popup size according to client size.
@INPUT res_win_t widget: the menubox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void	menubox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION menubox_set_data: set the menubox menu document.
@INPUT res_win_t widget: the menubox widget.
@INPUT link_t_ptr ptr: the menu document link component.
@RETURN void: none.
*/
XDC_API void	menubox_set_data(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION menubox_get_data: get the menubox menu document.
@INPUT res_win_t widget: the menubox widget.
@RETURN link_t_ptr: return the menu link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr menubox_get_data(res_win_t widget);

/*
@FUNCTION menubox_redraw: redraw the menubox.
@INPUT res_win_t widget: the menubox widget.
@RETURN void: none.
*/
XDC_API void	menubox_redraw(res_win_t widget);

/*
@FUNCTION menubox_tabskip: tab the menubox focus item to next.
@INPUT res_win_t widget: the menubox widget.
@INPUT int skip: the skip code, it can be WD_TAB_LEFT, WD_TAB_RIGHT, WD_TAB_TOP, WD_TAB_BOTTOM.
@RETURN void: none.
*/
XDC_API void	menubox_tabskip(res_win_t widget, int skip);

/*
@FUNCTION menubox_set_focus_item: set the menubox focus item.
@INPUT res_win_t widget: the menubox widget.
@INPUT link_t_ptr ilk: the menu document link component.
@RETURN void: none.
*/
XDC_API void	menubox_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION menubox_get_focus_item: get the menubox focus item.
@INPUT res_win_t widget: the menubox widget.
@RETURN link_t_ptr: return the menu link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr menubox_get_focus_item(res_win_t widget);

/*
@FUNCTION menubox_get_item_rect: get the menubox item rect.
@INPUT res_win_t widget: the iconbox widget.
@INPUT link_t_ptr ilk: the menu item link component.
@OUTPUT xrect_t* pxr: the rect struct.
@RETURN void: none.
*/
XDC_API void	menubox_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION menubox_layout: layout the menubox at position.
@INPUT res_win_t widget: the menubox widget.
@INPUT const xpoint_t* ppt: the base position.
@INPUT int lay: the layout mode, it can be WD_LAYOUT_LEFTBOTTOM, WD_LAYOUT_RIGHTBOTTOM, WD_LAYOUT_LEFTTOP, WD_LAYOUT_RIGHTTOP.
@RETURN void: none.
*/
XDC_API void	menubox_layout(res_win_t widget, const xpoint_t* ppt, int lay);

/******************************key box*****************************************************/

/*
@FUNCTION keybox_create: create a keybox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t keybox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION keybox_popup_size: calcing the keybox popup size according to client size.
@INPUT res_win_t widget: the menubox widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void keybox_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION show_keybox: create and show keybox.
@INPUT const xpoint_t* ppt: the left top position showing at.
@RETURN res_win_t: return the new keybox widget.
*/
XDC_API res_win_t show_keybox(const xpoint_t* ppt);

/******************************edit box*****************************************************/

/*
@FUNCTION editbox_create: create a editbox widget.
@INPUT res_win_t widget: the owner widget.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t editbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

/*
@FUNCTION editbox_redraw: redraw the editbox.
@INPUT res_win_t widget: the menubox widget.
@RETURN void: none.
*/
XDC_API void editbox_redraw(res_win_t widget);

/*
@FUNCTION editbox_selectall: select all of the editbox text.
@INPUT res_win_t widget: the menubox widget.
@RETURN void: none.
*/
XDC_API void editbox_selectall(res_win_t widget);

/*
@FUNCTION editbox_set_text: set the editbox text.
@INPUT res_win_t widget: the editbox widget.
@INPUT const tchar_t* text: the text token.
@RETURN void: none.
*/
XDC_API void editbox_set_text(res_win_t widget, const tchar_t* text);

/*
@FUNCTION editbox_get_text: copy the editbox text.
@INPUT res_win_t widget: the editbox widget.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN void: none.
*/
XDC_API int editbox_get_text(res_win_t widget, tchar_t* buf, int max);

/*
@FUNCTION editbox_get_text_ptr: get the editbox text.
@INPUT res_win_t widget: the editbox widget.
@RETURN const tchar_t*: return the editbox text token.
*/
XDC_API const tchar_t* editbox_get_text_ptr(res_win_t widget);

/*
@FUNCTION editbox_auto_size: enable or disable the editbox widget size adjusting.
@INPUT res_win_t widget: the editbox widget.
@INPUT bool_t b: nonzero for sizing widget automatic.
@RETURN void: none.
*/
XDC_API void editbox_auto_size(res_win_t widget, bool_t b);

/*
@FUNCTION editbox_is_select: test the editbox text is in select mode.
@INPUT res_win_t widget: the editbox widget.
@RETURN bool_t: return  nonzero for being in select mode.
*/
XDC_API bool_t editbox_is_select(res_win_t widget);

/*
@FUNCTION editbox_is_multiline: test the editbox text is multiple line.
@INPUT res_win_t widget: the editbox widget.
@RETURN bool_t: return  nonzero for being multiple line.
*/
XDC_API bool_t editbox_is_multiline(res_win_t widget);

/*
@FUNCTION editbox_set_lock: enable or disable editing.
@INPUT res_win_t widget: the editbox widget.
@INPUT bool_t b: nonzero for enable, zero for disable.
@RETURN void: none.
*/
XDC_API void editbox_set_lock(res_win_t widget, bool_t b);

/*
@FUNCTION editbox_get_lock: get editbox can be editing.
@INPUT res_win_t widget: the editbox widget.
@RETURN bool_t b: return nonzero for enable, zero for disable editing.
*/
XDC_API bool_t editbox_get_lock(res_win_t widget);


#ifdef	__cplusplus
}
#endif

#endif /*XDCBOX_H*/