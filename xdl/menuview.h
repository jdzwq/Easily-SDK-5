/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc menu document

	@module	menuview.h | interface file

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

#ifndef _MENUVIEW_H
#define _MENUVIEW_H

#include "xdldef.h"


#ifdef XDL_SUPPORT_VIEW

typedef enum{
	MENU_HINT_NONE,
	MENU_HINT_ITEM,
}MENU_HINT_CODE;

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API float calc_menu_height(const if_measure_t* pif, const canvbox_t* pbox, link_t_ptr ptr);

	XDL_API float calc_menu_width(const if_measure_t* pif, const canvbox_t* pbox, link_t_ptr ptr);

	XDL_API void calc_menu_item_rect(const if_measure_t* pif, const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr);

	XDL_API int	calc_menu_hint(const if_measure_t* pif, const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pilk);

	XDL_API void draw_menu(const if_canvas_t* pcanv, const canvbox_t* pbox, link_t_ptr ptr);

#ifdef	__cplusplus
}
#endif

#endif //XDLVIEW

#endif	/*_MENUVIEW_H*/