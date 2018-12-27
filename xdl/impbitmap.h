/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl display context document

	@module	impbitmap.h | display context interface file

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

#ifndef _IMPBITMAP_H
#define _IMPBITMAP_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_CONTEXT_BITMAP)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API res_bmp_t select_compatible_bitmap(res_ctx_t rdc, res_bmp_t obj);

XDL_API res_bmp_t create_compatible_bitmap(res_ctx_t rdc, long cx, long cy);

XDL_API void destroy_bitmap(res_bmp_t bmp);

XDL_API void get_bitmap_size(res_bmp_t rdc, long* pw, long* ph);

XDL_API res_bmp_t create_color_bitmap(res_ctx_t rdc, const xcolor_t* pxc, long w, long h);

XDL_API res_bmp_t create_pattern_bitmap(res_ctx_t rdc, const xcolor_t* pxc_front, const xcolor_t* pxc_back, long w, long h, const tchar_t* lay);

XDL_API res_bmp_t create_gradient_bitmap(res_ctx_t rdc, const xcolor_t* pxc_near, const xcolor_t* pxc_center, long w, long h, const tchar_t* lay);

XDL_API res_bmp_t load_bitmap_from_bytes(res_ctx_t rdc, const byte_t* pb, dword_t len);

XDL_API dword_t	save_bitmap_to_bytes(res_ctx_t rdc, res_bmp_t rb, byte_t* pb, dword_t max);

XDL_API dword_t	get_bitmap_bytes(res_bmp_t rdc);

#ifdef XDK_SUPPORT_SHELL
XDL_API res_bmp_t load_bitmap_from_thumb(res_ctx_t rdc, const tchar_t* fname);

XDL_API res_bmp_t load_bitmap_from_icon(res_ctx_t rdc, const tchar_t* iname);
#endif

#ifdef XDL_SUPPORT_BAR
XDL_API res_bmp_t create_code128_bitmap(res_ctx_t rdc, long w, long h, const tchar_t* text);

XDL_API res_bmp_t create_pdf417_bitmap(res_ctx_t rdc, long w, long h, const tchar_t* text);

XDL_API res_bmp_t create_qrcode_bitmap(res_ctx_t rdc, long w, long h, const tchar_t* text);
#endif

XDL_API res_bmp_t load_bitmap_from_file(res_ctx_t rdc, const tchar_t* fname);

XDL_API bool_t save_bitmap_to_file(res_ctx_t rdc, res_bmp_t rb, const tchar_t* type, const tchar_t* fname);

XDL_API res_bmp_t load_bitmap_from_ximage(res_ctx_t rdc, ximage_t* p, long cx, long cy);

XDL_API bool_t save_bitmap_to_ximage(res_ctx_t rdc, ximage_t* pmi, res_bmp_t bmp);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT_BITMAP*/

#endif /*IMPBITMAP_H*/