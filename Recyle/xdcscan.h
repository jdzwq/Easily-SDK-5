/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text scanner document

	@module	xdcscan.h | text scan interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#ifndef _XDCSCAN_H
#define _XDCSCAN_H

#include "xdcdef.h"

typedef enum{
	_TEXTOR_SCAN_BEGIN = 0,
	_TEXTOR_SCAN_RETURN = 1,
	_TEXTOR_SCAN_OBJECT_BEGIN = 2,
	_TEXTOR_SCAN_INDENT = 3,
	_TEXTOR_SCAN_WORDS = 4,
	_TEXTOR_SCAN_CARRIAGE = 5,
	_TEXTOR_SCAN_OBJECT_END = 6,
	_TEXTOR_SCAN_END = 7,
}_TEXTOR_SCAN;

typedef enum{
	_TEXTOR_OPERA_STOP = 0,
	_TEXTOR_OPERA_NEXT = 1,
	_TEXTOR_OPERA_DEL = 3,
	_TEXTOR_OPERA_INS = 4,
}_TEXTOR_OPERA;

typedef struct _TEXTMEASURE
{
	long lw, lh;
	long x, y, cx, cy;
	long min_x, max_x, min_y, max_y;
}TEXTMEASURE;

//textor interface
typedef int(__cdecl *PF_SCAN_TEXT_CALLBACK)(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf, const xcolor_t* pxc, void* pp);

typedef void(__cdecl *PF_SCAN_TEXT)(res_dc_t rdc, void* data, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp);

typedef int(__cdecl *PF_GET_TEXT)(void* data, tchar_t* buf, int max);

typedef void(__cdecl *PF_SET_TEXT)(void* data, const tchar_t* buf, int len);

void scan_fix_text(res_dc_t rdc, const tchar_t* text, int len, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp);

void scan_var_text(res_dc_t rdc, void* data, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp);

void scan_tag_text(res_dc_t rdc, void* data, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp);

void scan_text_doc(res_dc_t rdc, void* data, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp);

#endif