/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dialog document

	@module	xdcdlg.h | dialog interface file

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

XDC_API res_win_t msgdlg_create(const tchar_t* text, dword_t button, res_win_t owner);

XDC_API void msgdlg_popup_size(res_win_t widget, xsize_t* pxs);

XDC_API res_win_t inputdlg_create(const tchar_t* title, tchar_t* buf, int max, res_win_t owner);

XDC_API void inputdlg_popup_size(res_win_t widget, xsize_t* pxs);

XDC_API res_win_t properdlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner);

XDC_API void properdlg_popup_size(res_win_t widget, xsize_t* pxs);

XDC_API res_win_t tabledlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner);

XDC_API void tabledlg_popup_size(res_win_t widget, xsize_t* pxs);

XDC_API res_win_t listdlg_create(const tchar_t* title, link_t_ptr ptr, res_win_t owner);

XDC_API void listdlg_popup_size(res_win_t widget, xsize_t* pxs);

XDC_API res_win_t textdlg_create(const tchar_t* title, string_t var, res_win_t owner);

XDC_API void textdlg_popup_size(res_win_t widget, xsize_t* pxs);

XDC_API res_win_t griddlg_create(const tchar_t* title, link_t_ptr ptr, link_t_ptr* prow, res_win_t owner);

XDC_API void griddlg_popup_size(res_win_t widget, xsize_t* pxs);

XDC_API res_win_t previewdlg_create(const tchar_t* title, res_win_t owner);

XDC_API link_t_ptr previewdlg_get_arch(res_win_t widget);

XDC_API void previewdlg_redraw(res_win_t widget);

XDC_API res_win_t annodlg_create(const tchar_t* title, string_t var, res_win_t owner);

////////////////////////////////////////////////////////////////////////////////////////
XDC_API res_win_t create_dialog(link_t_ptr ptr_dlg, res_win_t owner);

XDC_API int sub_dialog_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);

XDC_API int sub_dialog_on_size(res_win_t widget, int code, const xsize_t* pxs, uid_t sid, var_long delta);

#ifdef	__cplusplus
}
#endif

#endif /*XDCDLG_H*/
