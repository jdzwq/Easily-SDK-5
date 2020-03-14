/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdc display context document

	@module	impbitmap.h | interface file

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

#include "xdcdef.h"

#if defined(XDU_SUPPORT_CONTEXT_BITMAP)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION destroy_bitmap: destroy bitmap object.
@INPUT res_bmp_t bmp: device context resource handle.
@RETURN void: none.
*/
EXP_API void destroy_bitmap(res_bmp_t bmp);

/*
@FUNCTION get_bitmap_size: get bitmap size in points.
@INPUT res_bmp_t bmp: bitmap resource handle.
@OUTPUT int* pw: int value for returning width.
@OUTPUT int* ph: int value for returning height.
@RETURN void: none.
*/
EXP_API void get_bitmap_size(res_bmp_t bmp, int* pw, int* ph);

/*
@FUNCTION create_context_bitmap: create a bitmap from memory context.
@INPUT res_ctx_t rdc: memoey context resource handle.
@RETURN res_bmp_t: if succeeds return bitmap resource handle, fails return NULL.
*/
EXP_API res_bmp_t create_context_bitmap(res_ctx_t rdc);

/*
@FUNCTION create_color_bitmap: create a color based bitmap.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT int w: the bitmap width in points.
@INPUT int h: the bitmap height in points.
@RETURN res_bmp_t: if succeeds return bitmap resource handle, fails return NULL.
*/
EXP_API res_bmp_t create_color_bitmap(res_ctx_t rdc, const xcolor_t* pxc, int w, int h);

/*
@FUNCTION create_pattern_bitmap: create a pattern bitmap.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const xcolor_t* pxc_front: the color struct for front drawing.
@INPUT const xcolor_t* pxc_back: the color struct for background drawing.
@INPUT int w: the bitmap width in points.
@INPUT int h: the bitmap height in points.
@INPUT const tchar_t* lay: the layout mode, it can be GDI_ATTR_LAYOUT_HORZ, GDI_ATTR_LAYOUT_VERT, GDI_ATTR_LAYOUT_CROSS.
@RETURN res_bmp_t: if succeeds return bitmap resource handle, fails return NULL.
*/
EXP_API res_bmp_t create_pattern_bitmap(res_ctx_t rdc, const xcolor_t* pxc_front, const xcolor_t* pxc_back, int w, int h, const tchar_t* lay);

/*
@FUNCTION create_gradient_bitmap: create a gradient bitmap.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const xcolor_t* pxc_near: the color struct for nearest drawing.
@INPUT const xcolor_t* pxc_center: the color struct for center drawing.
@INPUT int w: the bitmap width in points.
@INPUT int h: the bitmap height in points.
@INPUT const tchar_t* lay: the layout mode, it can be GDI_ATTR_LAYOUT_HORZ, GDI_ATTR_LAYOUT_VERT, GDI_ATTR_LAYOUT_CROSS.
@RETURN res_bmp_t: if succeeds return bitmap resource handle, fails return NULL.
*/
EXP_API res_bmp_t create_gradient_bitmap(res_ctx_t rdc, const xcolor_t* pxc_near, const xcolor_t* pxc_center, int w, int h, const tchar_t* lay);

/*
@FUNCTION load_bitmap_from_bytes: create bitmap from dib data buffer.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const byte_t* pb: the dib data buffer.
@INPUT dword_t len: the dib bytes
@RETURN res_bmp_t: if succeeds return bitmap resource handle, fails return NULL.
*/
EXP_API res_bmp_t load_bitmap_from_bytes(res_ctx_t rdc, const byte_t* pb, dword_t len);

/*
@FUNCTION save_bitmap_to_bytes: save bitmap as dib data.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT res_bmp_t rb: bitmap resource handle.
@OUTPUT byte_t* pb: the buffer for returning dib data.
@INPUT dword_t max: the buffer size in bytes
@RETURN res_bmp_t: if succeeds return bitmap resource handle, fails return NULL.
*/
EXP_API dword_t	save_bitmap_to_bytes(res_ctx_t rdc, res_bmp_t rb, byte_t* pb, dword_t max);

/*
@FUNCTION get_bitmap_bytes: get bitmap dib bytes.
@INPUT res_bmp_t bmp: memory resource handle.
@RETURN dword_t: if succeeds return dib bytes, fails return zero.
*/
EXP_API dword_t	get_bitmap_bytes(res_bmp_t bmp);

#ifdef XDU_SUPPORT_SHELL
/*
@FUNCTION load_bitmap_from_thumb: create thumb bitmap from file.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const tchar_t* fname: the file path name for loading thumb bitmap.
@RETURN res_bmp_t: if succeeds return memory resource handle, fails return NULL.
*/
EXP_API res_bmp_t load_bitmap_from_thumb(res_ctx_t rdc, const tchar_t* fname);

/*
@FUNCTION load_bitmap_from_thumb: create thumb bitmap from icon.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const tchar_t* iname: the icon name.
@RETURN res_bmp_t: if succeeds return memory resource handle, fails return NULL.
*/
EXP_API res_bmp_t load_bitmap_from_icon(res_ctx_t rdc, const tchar_t* iname);
#endif

#ifdef XDC_SUPPORT_BAR
/*
@FUNCTION create_code128_bitmap: create code128 bar bitmap.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT int w: the bitmap width in points.
@INPUT int w: the bitmap height in points.
@INPUT const tchar_t* text: the string data.
@RETURN res_bmp_t: if succeeds return memory resource handle, fails return NULL.
*/
EXP_API res_bmp_t create_code128_bitmap(res_ctx_t rdc, int w, int h, const tchar_t* text);

/*
@FUNCTION create_pdf417_bitmap: create pdf417 bar bitmap.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT int w: the bitmap width in points.
@INPUT int w: the bitmap height in points.
@INPUT const tchar_t* text: the string data.
@RETURN res_bmp_t: if succeeds return memory resource handle, fails return NULL.
*/
EXP_API res_bmp_t create_pdf417_bitmap(res_ctx_t rdc, int w, int h, const tchar_t* text);

/*
@FUNCTION create_qrcode_bitmap: create qrcode bar bitmap.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT int w: the bitmap width in points.
@INPUT int w: the bitmap height in points.
@INPUT const tchar_t* text: the string data.
@RETURN res_bmp_t: if succeeds return memory resource handle, fails return NULL.
*/
EXP_API res_bmp_t create_qrcode_bitmap(res_ctx_t rdc, int w, int h, const tchar_t* text);
#endif

/*
@FUNCTION load_bitmap_from_file: create bitmap from image file.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const tchar_t* fname: the image file name.
@RETURN res_bmp_t: if succeeds return memory resource handle, fails return NULL.
*/
EXP_API res_bmp_t load_bitmap_from_file(res_ctx_t rdc, const tchar_t* fname);

/*
@FUNCTION save_bitmap_to_file: save bitmap to image file.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT res_bmp_t rb: bitmap resource handle.
@INPUT const tchar_t* type: the image type, it can be GDI_ATTR_IMAGE_TYPE_JPG, GDI_ATTR_IMAGE_TYPE_PNG, GDI_ATTR_IMAGE_TYPE_BMP.
@INPUT const tchar_t* fname: the image file name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_bitmap_to_file(res_ctx_t rdc, res_bmp_t rb, const tchar_t* type, const tchar_t* fname);

/*
@FUNCTION load_bitmap_from_ximage: create bitmap from ximage struct.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT const ximage_t* p: the image struct.
@INPUT int cx: the bitmap width in points.
@INPUT int cy: the bitmap height in points.
@RETURN res_bmp_t: if succeeds return memory resource handle, fails return NULL.
*/
EXP_API res_bmp_t load_bitmap_from_ximage(res_ctx_t rdc, const ximage_t* p, int cx, int cy);

/*
@FUNCTION save_bitmap_to_ximage: save bitmap to ximage struct.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT res_bmp_t bmp: bitmap resource handle.
@INPUT ximage_t* pmi: the ximage struct for saving data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_bitmap_to_ximage(res_ctx_t rdc, res_bmp_t bmp, ximage_t* pmi);

#ifdef	__cplusplus
}
#endif

#endif /*XDU_SUPPORT_CONTEXT_BITMAP*/

#endif /*IMPBITMAP_H*/