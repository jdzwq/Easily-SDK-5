/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text view

	@module	textview.h | text view interface file

	@devnote ����Ȩ 2003.01 - 2007.12	v3.0
	@devnote ����Ȩ 2008.01 - 2009.12	v3.5
	@devnote ����Ȩ 2012.01 - 2014.12	v4.0
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

#ifndef _TEXTVIEW_H
#define _TEXTVIEW_H

#include "xdldef.h"
#include "xdlattr.h"


/*******************************************************************************************************************/
XDL_API long calc_text_height(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr);

XDL_API void calc_text_hint(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk, int* row_at, int* col_at);

XDL_API void calc_text_draw(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr);

#endif //_TEXTVIEW_H
