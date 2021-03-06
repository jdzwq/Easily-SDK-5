﻿/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xds utility document

	@module	xdsutil.h | interface file

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

#ifndef _XDSUTIL_H
#define _XDSUTIL_H

#include "xdsdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API void rgb_to_hsl(unsigned char r, unsigned char g, unsigned char b, short* ph, short* ps, short* pl);

EXP_API void hsl_to_rgb(short h, short s, short l, unsigned char* pr, unsigned char* pg, unsigned char* pb);

EXP_API bool_t is_whiteness_xcolor(const xcolor_t* pxc);

EXP_API bool_t is_blackness_xcolor(const xcolor_t* pxc);

EXP_API bool_t is_grayness_xcolor(const xcolor_t* pxc);

EXP_API void parse_xcolor(xcolor_t* pxc, const tchar_t* token);

EXP_API void format_xcolor(const xcolor_t* pxc, tchar_t* buf);

EXP_API void lighten_xcolor(xcolor_t* clr, int n);

EXP_API bool_t is_null_xpen(const xpen_t* pxp);

EXP_API bool_t is_null_xbrush(const xbrush_t* pxb);

EXP_API bool_t is_null_xfont(const xfont_t* pxf);

EXP_API bool_t is_null_xface(const xface_t* pxa);

EXP_API void default_xpen(xpen_t* pxp);

EXP_API void default_xbrush(xbrush_t* pxb);

EXP_API void default_xfont(xfont_t* pxf);

EXP_API void default_xface(xface_t* pxa);

EXP_API void lighten_xpen(xpen_t* pxp, int n);

EXP_API void merge_xpen(xpen_t* pxp_dst, const xpen_t* pxp_src);

EXP_API void merge_xbrush(xbrush_t* pxb_dst, const xbrush_t* pxb_src);

EXP_API void merge_xfont(xfont_t* pxf_dst, const xfont_t* pxf_src);

EXP_API void merge_xface(xface_t* pxa_dst, const xface_t* pxa_src);

EXP_API void lighten_xbrush(xbrush_t* pxb, int n);

EXP_API void lighten_xfont(xfont_t* pxf, int n);

EXP_API void parse_xpen_from_style(xpen_t* pxp, const tchar_t* token);

EXP_API int format_xpen_to_style(const xpen_t* pxp, tchar_t* buf, int len);

EXP_API void parse_xbrush_from_style(xbrush_t* pxb, const tchar_t* token);

EXP_API int format_xbrush_to_style(const xbrush_t* pxb, tchar_t* buf, int len);

EXP_API void parse_xfont_from_style(xfont_t* pxf, const tchar_t* token);

EXP_API int format_xfont_to_style(const xfont_t* pxf, tchar_t* buf, int len);

EXP_API void parse_xface_from_style(xface_t* pxa, const tchar_t* token);

EXP_API int format_xface_to_style(const xface_t* pxa, tchar_t* buf, int len);

EXP_API void parse_ximage_from_source(ximage_t* pxi, const tchar_t* token);

EXP_API int format_ximage_to_source(const ximage_t* pxi, tchar_t* buf, int len);

/*****************************************************************************************/
EXP_API bool_t ft_inside(float x, float y, float x1, float y1, float x2, float y2);

EXP_API bool_t pt_inside(int x, int y, int x1, int y1, int x2, int y2);

EXP_API bool_t ft_in_rect(const xpoint_t* ppt, const xrect_t* prt);

EXP_API bool_t pt_in_rect(const xpoint_t* ppt, const xrect_t* prt);

EXP_API void ft_offset_point(xpoint_t* ppt, float cx, float cy);

EXP_API void pt_offset_point(xpoint_t* ppt, int cx, int cy);

EXP_API void ft_center_rect(xrect_t* pxr, float cx, float cy);

EXP_API void pt_center_rect(xrect_t* pxr, int cx, int cy);

EXP_API void ft_expand_rect(xrect_t* pxr, float cx, float cy);

EXP_API void pt_expand_rect(xrect_t* pxr, int cx, int cy);

EXP_API void ft_offset_rect(xrect_t* pxr, float cx, float cy);

EXP_API void pt_offset_rect(xrect_t* pxr, int cx, int cy);

EXP_API void ft_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt);

EXP_API void pt_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt);

EXP_API bool_t ft_clip_rect(xrect_t* pxr, const xrect_t* pxr_sub);

EXP_API bool_t pt_clip_rect(xrect_t* pxr, const xrect_t* pxr_sub);

EXP_API void ft_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub);

EXP_API void pt_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub);

EXP_API void pt_inter_square(xrect_t* pxr, const xrect_t* pxr_org);

EXP_API void ft_inter_square(xrect_t* pxr, const xrect_t* pxr_org);

EXP_API void ft_cell_rect(xrect_t* pxr, bool_t horz, int rows, int cols, int index);

EXP_API void pt_cell_rect(xrect_t* pxr, bool_t horz, int rows, int cols, int index);

EXP_API bool_t rect_is_empty(const xrect_t* pxr);

EXP_API void empty_rect(xrect_t* pxr);

EXP_API void pt_adjust_rect(xrect_t* pxr, int src_width, int src_height, const tchar_t* horz_align, const tchar_t* vert_align);

EXP_API void ft_adjust_rect(xrect_t* pxr, float src_width, float src_height, const tchar_t* horz_align, const tchar_t* vert_align);

EXP_API int ft_quadrant(const xpoint_t* ppo, const xpoint_t* ppt, bool_t sflag);

EXP_API int pt_quadrant(const xpoint_t* ppo, const xpoint_t* ppt, bool_t sflag);

/**************************************************************************************/
EXP_API int mul_div_int(int m1, int m2, int d);

EXP_API short mul_div_short(short m1, short m2, short d);

/**************************************************************************************/

EXP_API void split_path(const tchar_t* pathfile, tchar_t* path, tchar_t* file, tchar_t* ext);

EXP_API void split_file(const tchar_t* pathfile, tchar_t* path, tchar_t* file);

#ifdef	__cplusplus
}
#endif

#endif