/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc textor document

	@module	textor.h | textor interface file

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

#ifndef _TEXTOR_H
#define _TEXTOR_H

#include "xdcdef.h"


typedef struct _TEXTORUNDO*	TEXTORUNDO_PTR;

typedef struct _TEXTORUNDO{
	int cur_row, cur_col;
	int sel_row, sel_col;
	int page;
	bool_t b_select;

	tchar_t* text;
	int len;

	TEXTORUNDO_PTR next;
}TEXTORUNDO;

typedef enum{
	_TEXTOR_DRAG_FIRST = 1,
	_TEXTOR_DRAG_LAST = 2
}TEXTOR_DRAG;

typedef enum{
	_TEXTOR_PRESS_IGNORE = 0,
	_TEXTOR_PRESS_HANDLE = 1,
	_TEXTOR_PRESS_ACCEPT = 2
}TEXTOR_PRESS;

typedef void(*PF_SCAN_TEXT)(void* data, if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, long bx, long by, long bw, long bh, bool_t paged, PF_SCAN_TEXTOR_CALLBACK pf, void* pp);
typedef int(*PF_GET_TEXT)(void* data, tchar_t* buf, int max);
typedef void(*PF_SET_TEXT)(void* data, const tchar_t* buf, int len);
typedef bool_t(*PF_GET_PAGING)(res_win_t wt, xsize_t* pse);

typedef struct _textor_t{
	res_win_t widget;
	res_ctx_t dc;
	viewbox_t vb;

	void* data;
	void* object;
	bool_t atom;

	bool_t paged;
	int pages;
	int page;

	int cur_row, cur_col;
	int sel_row, sel_col;
	int max_row, max_col;
	int n_drag;
	bool_t b_select;

	PF_SCAN_TEXT pf_scan_text;
	PF_GET_TEXT	pf_get_text;
	PF_SET_TEXT pf_set_text;
	PF_GET_PAGING pf_get_paging;

	int max_undo;
	TEXTORUNDO* ptu;
}textor_t;

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API void hand_textor_set_focus(textor_t* ptd, res_win_t wt);

XDC_API void hand_textor_kill_focus(textor_t* ptd, res_win_t wt);

XDC_API int hand_textor_word(textor_t* ptd, tchar_t* pch);

XDC_API void hand_textor_lbutton_down(textor_t* ptd, const xpoint_t* pxp);

XDC_API void hand_textor_lbutton_up(textor_t* ptd, const xpoint_t* pxp);

XDC_API void hand_textor_mousemove(textor_t* ptd, dword_t mk, const xpoint_t* ppt);

XDC_API void hand_textor_size(textor_t* ptd, int code, const xsize_t* prs);

XDC_API void hand_textor_scroll(textor_t* ptd, bool_t bHorz, long nLine);

XDC_API void hand_textor_paint(textor_t* ptd, res_ctx_t dc, const xrect_t* pxr);

XDC_API void hand_textor_selectall(textor_t* ptd);

XDC_API void hand_textor_selectline(textor_t* ptd);

XDC_API void hand_textor_selectcur(textor_t* ptd);

XDC_API void hand_textor_selectobj(textor_t* ptd);

XDC_API void hand_textor_findobj(textor_t* ptd, void* obj);

XDC_API void hand_textor_redraw(textor_t* ptd);

XDC_API int hand_textor_back(textor_t* ptd);

XDC_API int hand_textor_delete(textor_t* ptd);

XDC_API int hand_textor_escape(textor_t* ptd);

XDC_API int hand_textor_left(textor_t* ptd);

XDC_API int hand_textor_right(textor_t* ptd);

XDC_API int hand_textor_up(textor_t* ptd);

XDC_API int hand_textor_down(textor_t* ptd);

XDC_API int hand_textor_home(textor_t* ptd);

XDC_API int hand_textor_end(textor_t* ptd);

XDC_API int hand_textor_move_to_page(textor_t* ptd, int page);

XDC_API int hand_textor_move_first_page(textor_t* ptd);

XDC_API int hand_textor_move_prev_page(textor_t* ptd);

XDC_API int hand_textor_move_next_page(textor_t* ptd);

XDC_API int hand_textor_move_last_page(textor_t* ptd);

XDC_API int hand_textor_undo(textor_t* ptd);

XDC_API int hand_textor_copy(textor_t* ptd);

XDC_API int hand_textor_cut(textor_t* ptd);

XDC_API int hand_textor_paste(textor_t* ptd);

XDC_API void hand_textor_clean(textor_t* ptd);

XDC_API void hand_textor_done(textor_t* ptd);

XDC_API void hand_textor_discard(textor_t* ptd);

XDC_API int hand_textor_selected_text(textor_t* ptd, tchar_t* buf, int max);

XDC_API int hand_textor_replace_text(textor_t* ptd, const tchar_t* token, int len);

XDC_API int textor_cur_page(textor_t* ptd);

XDC_API int textor_max_page(textor_t* ptd);

XDC_API void textor_object_rect(textor_t* ptd, void* object, xrect_t* pxr);

XDC_API void textor_cursor_point(textor_t* ptd, xpoint_t* ppt);

XDC_API void textor_cursor_rect(textor_t* ptd, xrect_t* pxr);

XDC_API bool_t textor_is_select(textor_t* ptd);

XDC_API bool_t textor_at_atom(textor_t* ptd);

#ifdef	__cplusplus
}
#endif

#endif /**/
