/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dataing document

	@module	dataing.h | interface file

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

#ifndef _DATAING_H
#define _DATAING_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_VIEW)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION draw_pass: draw the password in canvas using millimeter coordinate.
@INPUT if_drawing_t* pif: the canvas interface.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT const tchar_t* txt: the password text.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	draw_pass(const if_drawing_t* pif,  const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

/*
@FUNCTION draw_data: draw the formated data in canvas using millimeter coordinate.
@INPUT if_drawing_t* pif: the canvas interface.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT const tchar_t* data: the data text.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@INPUT int dig: the digits of numeric text.
@INPUT const tchar_t* type: the data type, it can be ATTR_DATATYPE_*.
@INPUT const tchar_t* fmt: the text format token.
@INPUT bool_t zeronull: if nonzero, the numeric equal to zero, the text will not to be drawed.
@INPUT bool_t autowap: if nonzero, the text will auto
@RETURN void: none.
*/
EXP_API void	draw_data(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap);


#ifdef	__cplusplus
}
#endif

#endif

#endif /*SHAPING_H*/