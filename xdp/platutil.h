/***********************************************************************
	Easily xdp v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdp utility document

	@module	xdputil.h | interface file

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

#ifndef _XDPUTIL_H
#define _XDPUTIL_H

#include "platform.h"

#ifdef	__cplusplus
extern "C" {
#endif

extern void rgb_to_hsl(unsigned char r, unsigned char g, unsigned char b, short* ph, short* ps, short* pl);

extern void hsl_to_rgb(short h, short s, short l, unsigned char* pr, unsigned char* pg, unsigned char* pb);

extern void parse_xcolor(xcolor_t* pxc, const tchar_t* token);

extern void format_xcolor(const xcolor_t* pxc, tchar_t* buf);

extern void lighten_xcolor(xcolor_t* clr, int n);

extern bool_t is_null_xpen(const xpen_t* pxp);

extern bool_t is_null_xbrush(const xbrush_t* pxb);

extern bool_t is_null_xfont(const xfont_t* pxf);

extern bool_t is_null_xface(const xface_t* pxa);

extern void default_xpen(xpen_t* pxp);

extern void default_xbrush(xbrush_t* pxb);

extern void default_xfont(xfont_t* pxf);

extern void default_xface(xface_t* pxa);

extern void default_xgradi(xgradi_t* pxg);

extern void lighten_xpen(xpen_t* pxp, int n);

extern void merge_xpen(xpen_t* pxp_dst, const xpen_t* pxp_src);

extern void merge_xbrush(xbrush_t* pxb_dst, const xbrush_t* pxb_src);

extern void merge_xfont(xfont_t* pxf_dst, const xfont_t* pxf_src);

extern void merge_xface(xface_t* pxa_dst, const xface_t* pxa_src);

extern void lighten_xbrush(xbrush_t* pxb, int n);

extern void lighten_xfont(xfont_t* pxf, int n);

extern void lighten_xgradi(xgradi_t* pxg, int n);

/*****************************************************************************************/
extern bool_t ft_inside(float x, float y, float x1, float y1, float x2, float y2);

extern bool_t pt_inside(int x, int y, int x1, int y1, int x2, int y2);

extern bool_t ft_in_rect(const xpoint_t* ppt, const xrect_t* prt);

extern bool_t pt_in_rect(const xpoint_t* ppt, const xrect_t* prt);

extern void ft_offset_point(xpoint_t* ppt, float cx, float cy);

extern void pt_offset_point(xpoint_t* ppt, int cx, int cy);

extern void ft_center_rect(xrect_t* pxr, float cx, float cy);

extern void pt_center_rect(xrect_t* pxr, int cx, int cy);

extern void ft_expand_rect(xrect_t* pxr, float cx, float cy);

extern void pt_expand_rect(xrect_t* pxr, int cx, int cy);

extern void ft_offset_rect(xrect_t* pxr, float cx, float cy);

extern void pt_offset_rect(xrect_t* pxr, int cx, int cy);

extern void ft_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt);

extern void pt_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt);

extern void ft_cell_rect(xrect_t* pxr, int rows, int cols, int index);

extern void pt_cell_rect(xrect_t* pxr, int rows, int cols, int index);

extern bool_t ft_clip_rect(xrect_t* pxr, const xrect_t* pxr_sub);

extern bool_t pt_clip_rect(xrect_t* pxr, const xrect_t* pxr_sub);

extern void ft_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub);

extern void pt_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub);

extern bool_t rect_is_empty(const xrect_t* pxr);

extern void empty_rect(xrect_t* pxr);

/**************************************************************************************/
extern int mul_div_int(int m1, int m2, int d);

extern short mul_div_short(short m1, short m2, short d);

/**************************************************************************************/

extern void split_path(const tchar_t* pathfile, tchar_t* path, tchar_t* file, tchar_t* ext);

extern void split_file(const tchar_t* pathfile, tchar_t* path, tchar_t* file);

#ifdef	__cplusplus
}
#endif

#endif