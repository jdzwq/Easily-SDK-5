/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc text scanner document

	@module	scanner.h | interface file

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

#ifndef _SCANNER_H
#define _SCANNER_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_GDI

typedef enum{
	_SCANNER_OPERA_STOP = 0,
	_SCANNER_OPERA_NEXT = 1,
	_SCANNER_OPERA_DEL = 2,
	_SCANNER_OPERA_INS = 3,
	_SCANNER_OPERA_PAGED = 4,
}SCANNER_OPERA;

typedef enum{
	_SCANNER_STATE_BEGIN = 0,
	_SCANNER_STATE_CATOR = 1,
	_SCANNER_STATE_WORDS = 2,
	_SCANNER_STATE_LINEBREAK = 3,
	_SCANNER_STATE_PAGEBREAK = 4,
	_SCANNER_STATE_NEWLINE = 5,
	_SCANNER_STATE_NEWPAGE = 6,
	_SCANNER_STATE_END = -1,
}SCANNER_STATE;

typedef struct _word_place_t
{
	int char_w, char_h, line_h;
	int cur_x, cur_y, cur_w, cur_h;
	int min_x, min_y, max_x, max_y;
}word_place_t;

typedef int(*PF_SCAN_TEXTOR_CALLBACK)(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_word, int cur_count, tchar_t* ret_word, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp);

typedef bool_t(*PF_SCAN_IS_PAGING)(void* ctx);
typedef bool_t(*PF_SCAN_BREAK_PAGE)(void* ctx);
typedef int(*PF_SCAN_NEXT_PAGE)(void* ctx);
typedef int(*PF_SCAN_NEXT_WORD)(void* ctx, tchar_t** ppch, xsize_t* pse, bool_t* pins, bool_t* pdel, bool_t* psel, bool_t* patom);
typedef int(*PF_SCAN_INSERT_WORD)(void* ctx, tchar_t* pch, xsize_t* pse);
typedef int(*PF_SCAN_DELETE_WORD)(void* ctx);
typedef void(*PF_SCAN_CUR_OBJECT)(void* ctx, void** pobj);

typedef struct _wordscan_interface{
	void* ctx;

	PF_SCAN_IS_PAGING	pf_is_paging;
	PF_SCAN_BREAK_PAGE	pf_break_page;
	PF_SCAN_NEXT_PAGE	pf_next_page;
	PF_SCAN_NEXT_WORD	pf_next_word;
	PF_SCAN_INSERT_WORD	pf_insert_word;
	PF_SCAN_DELETE_WORD	pf_delete_word;
	PF_SCAN_CUR_OBJECT	pf_cur_object;
}wordscan_interface;

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API void scan_object_text(const measure_interface* pim, const xfont_t* pxf, const xface_t* pxa, int bx, int by, int bw, int bh, wordscan_interface* pit, PF_SCAN_TEXTOR_CALLBACK pf, void* pp);

#ifdef	__cplusplus
}
#endif

#endif

#endif /*SCANNER_H*/