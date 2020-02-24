/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc frame document

	@module	xdcframe.h | frame interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#ifndef _XDCFRM_H
#define _XDCFRM_H

#include "xdcdef.h"
#include "xdcinit.h"

#define DOCK_AT_TOP				0x00000001
#define DOCK_AT_BOTTOM			0x00000002
#define DOCK_AT_LEFT			0x00000004
#define DOCK_AT_RIGHT			0x00000008

#define DOCK_IS_DYNA			0x00010000

XDC_API res_win_t frame_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr);

XDC_API bool_t frame_dock(res_win_t widget, dword_t style, long cx, long cy);

XDC_API void frame_undock(res_win_t widget, dword_t style);

XDC_API void frame_get_dock_rect(res_win_t widget, dword_t style, xrect_t* pxr);

#endif