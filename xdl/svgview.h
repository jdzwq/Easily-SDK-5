/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl svg document

	@module	svgview.h | interface file

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

#ifndef _SVGVIEW_H
#define _SVGVIEW_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(XDL_SUPPORT_VIEW)

/*
@FUNCTION draw_svg: draw the svg document in canvas using millimeter coordinate.
@INPUT if_canvas_t* pif: the canvas interface.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT link_t_ptr svg: the svg document.
@RETURN void: none.
*/
EXP_API void	draw_svg(const if_canvas_t* pif, const xrect_t* prt, link_t_ptr svg);


#endif /*XDL_SUPPORT_VIEW*/

#ifdef	__cplusplus
}
#endif

#endif /*SVGING_H*/