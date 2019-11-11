/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dialog document

	@module	xdcdlg.h | interface file

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

#ifndef _XDCDLG_H
#define _XDCDLG_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION msgdlg_create: create a message dialog.
@INPUT const tchar_t* text: the message token.
@INPUT dword_t button: the button, 
it can be MSGDLG_PUSHBOX_CLOSE, MSGDLG_PUSHBOX_OK, MSGDLG_PUSHBOX_CANCEL, MSGDLG_PUSHBOX_YES, MSGDLG_PUSHBOX_NO, MSGDLG_PUSHBOX_KNOWN or combined.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t msgdlg_create(const tchar_t* text, dword_t button, res_win_t owner);

/*
@FUNCTION msgdlg_popup_size: calcing the message dialog popup size according to client size.
@INPUT res_win_t widget: the dialog widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void msgdlg_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION inputdlg_create: create a input dialog.
@INOUTPUT tchar_t* buf: the string buffer for inputing and outputing text.
@INPUT int max: the string buffer size in characters,
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t inputdlg_create(const tchar_t* title, tchar_t* buf, int max, res_win_t owner);

/*
@FUNCTION inputdlg_popup_size: calcing the input dialog popup size according to client size.
@INPUT res_win_t widget: the dialog widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void inputdlg_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION properdlg_create: create a proper dialog.
@INPUT const tchar_t* title: the title token.
@INPUT link_t_ptr ptr: the proper document.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t properdlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner);

/*
@FUNCTION properdlg_popup_size: calcing the proper dialog popup size according to client size.
@INPUT res_win_t widget: the dialog widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void properdlg_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION tabledlg_create: create a string table dialog.
@INPUT const tchar_t* title: the title token.
@INPUT link_t_ptr ptr: the string table.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t tabledlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner);

/*
@FUNCTION tabledlg_popup_size: calcing the string table dialog popup size according to client size.
@INPUT res_win_t widget: the dialog widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void tabledlg_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION listdlg_create: create a list document dialog.
@INPUT const tchar_t* title: the title token.
@INPUT link_t_ptr ptr: the list document.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t listdlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner);

/*
@FUNCTION listdlg_popup_size: calcing the list document dialog popup size according to client size.
@INPUT res_win_t widget: the dialog widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void listdlg_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION textdlg_create: create a multiple line text dialog.
@INPUT const tchar_t* title: the title token.
@INOUTPUT string_t var: the string object for inputing and outputing.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t textdlg_create(const tchar_t* title, string_t var, res_win_t owner);

/*
@FUNCTION textdlg_popup_size: calcing the multiple line text dialog popup size according to client size.
@INPUT res_win_t widget: the dialog widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void textdlg_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION griddlg_create: create a grid document dialog.
@INPUT const tchar_t* title: the title token.
@INPUT link_t_ptr ptr: the grid document.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t griddlg_create(const tchar_t* title, link_t_ptr ptr, link_t_ptr* prow, res_win_t owner);

/*
@FUNCTION griddlg_popup_size: calcing the grid document dialog popup size according to client size.
@INPUT res_win_t widget: the dialog widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void griddlg_popup_size(res_win_t widget, xsize_t* pxs);

/*
@FUNCTION annodlg_create: create a annotation document dialog.
@INPUT const tchar_t* title: the title token.
@INOUTPUT string_t var: the string object for inputing and outputing based64 bitmap data.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t annodlg_create(const tchar_t* title, string_t var, res_win_t owner);

/*
@FUNCTION previewdlg_create: create a preview dialog.
@INPUT const tchar_t* title: the title token.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t previewdlg_create(const tchar_t* title, res_win_t owner);

/*
@FUNCTION previewdlg_get_arch: get a preview dialog archive document.
@INPUT res_win_t widget: the dialog widget.
@RETURN link_t_ptr: return the archive document link component.
*/
XDC_API link_t_ptr previewdlg_get_arch(res_win_t widget);

/*
@FUNCTION previewdlg_redraw: redraw the preview dialog.
@INPUT res_win_t widget: the dialog widget.
@RETURN void: none.
*/
XDC_API void previewdlg_redraw(res_win_t widget);

/*
@FUNCTION create_dialog: create a dialog .
@INPUT link_t_ptr: the dialog document.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the dialog resource handle.
*/
XDC_API res_win_t create_dialog(link_t_ptr ptr_dlg, res_win_t owner);

XDC_API int sub_dialog_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);

XDC_API int sub_dialog_on_size(res_win_t widget, int code, const xsize_t* pxs, uid_t sid, var_long delta);

#ifdef	__cplusplus
}
#endif

#endif /*XDCDLG_H*/
