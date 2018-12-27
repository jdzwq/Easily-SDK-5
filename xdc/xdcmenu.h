/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc menu utility document

	@module	xdcmenu.h | menu utility interface file

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

#ifndef _XDCMENU_H
#define _XDCMENU_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API void textor_menu(res_win_t widget, const xpoint_t* ppt, int lay);

XDC_API void fontname_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay);

XDC_API void fontname_menu_item(int iid, tchar_t* buf, int max);

XDC_API void fontsize_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay);

XDC_API void fontsize_menu_item(int iid, tchar_t* buf, int max);

XDC_API void fontstyle_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay);

XDC_API void fontstyle_menu_item(int iid, tchar_t* buf, int max);

XDC_API void fontweight_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay);

XDC_API void fontweight_menu_item(int iid, tchar_t* buf, int max);

XDC_API void color_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay);

XDC_API void color_menu_item(int iid, tchar_t* buf, int max);

XDC_API void shape_menu(res_win_t widget, dword_t idc, const xpoint_t* ppt, int lay);

XDC_API void shape_menu_item(int iid, tchar_t* buf, int max);

XDC_API void track_popup_menu(res_win_t widget, dword_t idc, link_t_ptr menu, const xpoint_t* ppt, int lay);

#ifdef	__cplusplus
}
#endif

#endif /*XDCMENU_H*/
