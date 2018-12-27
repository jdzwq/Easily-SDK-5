/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc text view

	@module	textview.h | text view interface file

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

#ifndef _TEXTVIEW_H
#define _TEXTVIEW_H

#include "xdldef.h"
#include "scanner.h"

#if defined(XDL_SUPPORT_VIEW)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void scan_var_text(string_t vs, if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, long bx, long by, long bw, long bh, bool_t paged, PF_SCAN_TEXTOR_CALLBACK pf, void* pp);

XDL_API void scan_fix_text(tchar_t* text, int size, if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, long bx, long by, long bw, long bh, bool_t paged, PF_SCAN_TEXTOR_CALLBACK pf, void* pp);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_VIEW*/

#endif /*TEXTVIEW_H*/
