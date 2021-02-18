/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl jion document

	@module	jioning.h | interface file

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

#ifndef _JIONING_H
#define _JIONING_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(XDL_SUPPORT_GDI)

/*
@FUNCTION draw_linecap: draw the linecap in canvas using millimeter coordinate.
@INPUT drawing_interface* pif: the canvas interface.
@INPUT cont xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* pps: the line from.
@INPUT const xpoint_t* ppt: the line to and the arrow at.
@INPUT double angle: the angle for arrow.
@RETURN void: none.
*/
EXP_API void	draw_linecap(const drawing_interface* pif, const xpen_t* pxp, const xpoint_t* pps, const xpoint_t* ppt, double angle, const tchar_t* cap);

#endif /*XDL_SUPPORT_VIEW*/

#ifdef	__cplusplus
}
#endif

#endif /*PRINTBAG_H*/