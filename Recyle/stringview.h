/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc string view

	@module	stringview.h | string view interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#ifndef _STRVIEW_H
#define _STRVIEW_H

#include "xdldef.h"
#include "xdlattr.h"

/*************************************************************************************************************/
XDL_API void calc_string_suit_size(link_t_ptr ptr, xsize_t* pxs);

XDL_API long calc_string_width(const viewbox_t* pbox, link_t_ptr ptr);

XDL_API long calc_string_height(const viewbox_t* pbox, link_t_ptr ptr);

XDL_API void calc_string_item_rect(const viewbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr);

XDL_API void calc_string_item_text_rect(const viewbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr);

XDL_API int calc_string_hint(const viewbox_t* pbox, link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk);

XDL_API void draw_string(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr);

#endif /*_STRVIEW_H*/
