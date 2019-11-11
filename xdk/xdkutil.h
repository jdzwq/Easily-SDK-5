/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdk utility document

	@module	xdkutil.h | interface file

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

#ifndef _XDKUTIL_H
#define _XDKUTIL_H

#include "xdkdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDK_API void rgb_to_hsl(unsigned char r, unsigned char g, unsigned char b, short* ph, short* ps, short* pl);

XDK_API void hsl_to_rgb(short h, short s, short l, unsigned char* pr, unsigned char* pg, unsigned char* pb);

XDK_API void parse_xcolor(xcolor_t* pxc, const tchar_t* token);

XDK_API void format_xcolor(const xcolor_t* pxc, tchar_t* buf);

XDK_API void lighten_xcolor(xcolor_t* clr, int n);

XDK_API bool_t is_null_xpen(const xpen_t* pxp);

XDK_API bool_t is_null_xbrush(const xbrush_t* pxb);

XDK_API bool_t is_null_xfont(const xfont_t* pxf);

XDK_API bool_t is_null_xface(const xface_t* pxa);

XDK_API void default_xpen(xpen_t* pxp);

XDK_API void default_xbrush(xbrush_t* pxb);

XDK_API void default_xfont(xfont_t* pxf);

XDK_API void default_xface(xface_t* pxa);

XDK_API void default_xgradi(xgradi_t* pxg);

XDK_API void lighten_xpen(xpen_t* pxp, int n);

XDK_API void merge_xpen(xpen_t* pxp_dst, const xpen_t* pxp_src);

XDK_API void merge_xbrush(xbrush_t* pxb_dst, const xbrush_t* pxb_src);

XDK_API void merge_xfont(xfont_t* pxf_dst, const xfont_t* pxf_src);

XDK_API void merge_xface(xface_t* pxa_dst, const xface_t* pxa_src);

XDK_API void lighten_xbrush(xbrush_t* pxb, int n);

XDK_API void lighten_xfont(xfont_t* pxf, int n);

XDK_API void lighten_xgradi(xgradi_t* pxg, int n);

#ifdef	__cplusplus
}
#endif

#endif