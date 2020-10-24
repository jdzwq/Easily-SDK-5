/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl focus document

	@module	focusing.h | interface file

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

#ifndef _FOCUSING_H
#define _FOCUSING_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(XDL_SUPPORT_VIEW)


/*
@FUNCTION draw_select_raw: draw select frame in memory or device context using points coordinate.
@INPUT if_drawing_t* piv: the context interface.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void	draw_select_raw(const if_drawing_t* piv, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_focus_raw: draw focus frame in memory or device context using points coordinate.
@INPUT if_drawing_t* piv: the context interface.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void	draw_focus_raw(const if_drawing_t* piv, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_feed_raw: draw feed frame in memory or device context using points coordinate.
@INPUT if_drawing_t* piv: the context interface.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void	draw_feed_raw(const if_drawing_t* piv, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_sizing_raw: draw feed frame in memory or device context using points coordinate.
@INPUT if_drawing_t* piv: the context interface.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void	draw_sizing_raw(const if_drawing_t* piv, const xcolor_t* pxc, const xrect_t* prt, int deep, dword_t pos);



#endif /*XDL_SUPPORT_VIEW*/

#ifdef	__cplusplus
}
#endif

#endif /*PRINTBAG_H*/