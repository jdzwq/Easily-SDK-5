/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc fire editor document

	@module	xdcfire.h | fire editor interface file

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

#ifndef _XDCFIRE_H
#define _XDCFIRE_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API res_win_t fireedit_create(res_win_t widget, const xrect_t* pxr);

XDC_API res_win_t firecheck_create(res_win_t widget, const xrect_t* pxr);

XDC_API res_win_t firedate_create(res_win_t widget, const xrect_t* pxr);

XDC_API res_win_t firetime_create(res_win_t widget, const xrect_t* pxr);

XDC_API res_win_t firenum_create(res_win_t widget, const xrect_t* pxr);

XDC_API res_win_t firelist_create(res_win_t widget, const xrect_t* pxr, link_t_ptr data);

XDC_API link_t_ptr firelist_get_data(res_win_t widget);

XDC_API link_t_ptr firelist_get_item(res_win_t widget);

XDC_API res_win_t firegrid_create(res_win_t widget, const xrect_t* pxr, link_t_ptr data);

XDC_API link_t_ptr firegrid_get_data(res_win_t widget);

XDC_API link_t_ptr firegrid_get_item(res_win_t widget);

XDC_API res_win_t firewords_create(res_win_t widget, const xrect_t* pxr, link_t_ptr data);

XDC_API link_t_ptr firewords_get_data(res_win_t widget);

XDC_API link_t_ptr firewords_get_item(res_win_t widget);

#ifdef	__cplusplus
}
#endif

#endif /*XDCFIRE_H*/
