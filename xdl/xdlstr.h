﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdlstr document

	@module	xdlstr.h | interface file

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

#ifndef _XDLSTR_H
#define _XDLSTR_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API schar_t* a_xsalloc(int len);
XDL_API wchar_t* w_xsalloc(int len);

XDL_API schar_t* a_xsrealloc(schar_t* str,int len);
XDL_API wchar_t* w_xsrealloc(wchar_t* str,int len);

XDL_API void a_xsfree(schar_t* str);
XDL_API void w_xsfree(wchar_t* str);

XDL_API schar_t* a_xsnclone(const schar_t* str, int len);
XDL_API wchar_t* w_xsnclone(const wchar_t* str, int len);

XDL_API schar_t* a_xsclone(const schar_t* str);
XDL_API wchar_t* w_xsclone(const wchar_t* str);

#ifdef	__cplusplus
}
#endif

#if defined(_UNICODE) || defined(UNICODE)
#define xsalloc				w_xsalloc
#define xsrealloc			w_xsrealloc
#define xsfree				w_xsfree
#define xsnclone			w_xsnclone
#define xsclone				w_xsclone

#else
#define xsalloc				a_xsalloc
#define xsrealloc			a_xsrealloc
#define xsfree				a_xsfree
#define xsclone				a_xsclone
#define xsnclone			a_xsnclone

#endif /*UNICODE*/


#endif /*_STRLIB_H*/