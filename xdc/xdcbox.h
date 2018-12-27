/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc box document

	@module	xdcbox.h | box interface file

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

#ifndef _XDCBOX_H
#define _XDCBOX_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	XDC_API res_win_t numbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void numbox_popup_size(res_win_t widget, xsize_t* pxs);


	XDC_API res_win_t navibox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void navibox_set_target(res_win_t widget, res_win_t target);

	XDC_API void navibox_popup_size(res_win_t widget, xsize_t* pxs);


	XDC_API res_win_t tipbox_create(res_win_t widget, dword_t style, const xrect_t* pxr, int type, const tchar_t* text);

	XDC_API void tipbox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void tipbox_set_text(res_win_t widget, const tchar_t* sz_text);

	XDC_API res_win_t show_toolbox(const xpoint_t* ppt, const tchar_t* sz_text);

	XDC_API bool_t reset_toolbox(res_win_t widget, const xpoint_t* ppt, const tchar_t* sz_text);


	XDC_API res_win_t datebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void datebox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void datebox_set_date(res_win_t widget, const xdate_t* pxd);

	XDC_API void datebox_get_date(res_win_t widget, xdate_t* pxd);


	XDC_API res_win_t timebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void timebox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void timebox_set_datetime(res_win_t widget, const xdate_t* pxt);

	XDC_API void timebox_get_datetime(res_win_t widget, xdate_t* pxt);


	XDC_API res_win_t pushbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void pushbox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void pushbox_set_state(res_win_t widget, bool_t bChecked);

	XDC_API bool_t pushbox_get_state(res_win_t widget);

	XDC_API void pushbox_set_text(res_win_t widget, const tchar_t* text, int len);


	XDC_API res_win_t spinbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void spinbox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void spinbox_set_spin(res_win_t widget, int cur);

	XDC_API int spinbox_get_spin(res_win_t widget);


	XDC_API res_win_t slidebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void slidebox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void slidebox_set_slide(res_win_t widget, int pos);

	XDC_API int slidebox_get_slide(res_win_t widget);


	XDC_API res_win_t radiobox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void radiobox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void radiobox_set_state(res_win_t widget, bool_t b_on);

	XDC_API bool_t radiobox_get_state(res_win_t widget);


	XDC_API res_win_t checkbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void checkbox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void checkbox_set_state(res_win_t widget, bool_t b_on);

	XDC_API bool_t checkbox_get_state(res_win_t widget);


	XDC_API res_win_t staticbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void staticbox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void staticbox_set_text(res_win_t widget, const tchar_t* text);



	XDC_API res_win_t shapebox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void shapebox_set_shape(res_win_t widget, const tchar_t* shape);


	XDC_API res_win_t iconbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void iconbox_set_options(res_win_t widget, const tchar_t* opt, int len);

	XDC_API void iconbox_set_layer(res_win_t widget, const tchar_t* layer);

	XDC_API void iconbox_set_alignment(res_win_t widget, const tchar_t* align);

	XDC_API void iconbox_get_item_rect(res_win_t widget, const tchar_t* key, xrect_t* pxr);

	XDC_API void iconbox_popup_size(res_win_t widget, xsize_t* pxs);


	XDC_API res_win_t listbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void listbox_set_options(res_win_t widget, const tchar_t* opt, int len);

	XDC_API const tchar_t* listbox_get_cur_key(res_win_t widget);

	XDC_API int listbox_get_cur_val(res_win_t widget, tchar_t* buf, int max);

	XDC_API void listbox_redraw(res_win_t widget);

	XDC_API void listbox_tabskip(res_win_t widget, int nSkip);

	XDC_API void listbox_find(res_win_t widget, const tchar_t* token);

	XDC_API void listbox_filter(res_win_t widget, const tchar_t* token);

	XDC_API void listbox_popup_size(res_win_t widget, xsize_t* pxs);


	XDC_API res_win_t dropbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void dropbox_set_data(res_win_t widget, link_t_ptr ptr);

	XDC_API link_t_ptr dropbox_get_data(res_win_t widget);

	XDC_API void dropbox_redraw(res_win_t widget);

	XDC_API void dropbox_tabskip(res_win_t widget, int nSkip);

	XDC_API void dropbox_set_focus_item(res_win_t widget, link_t_ptr ilk);

	XDC_API link_t_ptr dropbox_get_focus_item(res_win_t widget);

	XDC_API void dropbox_find(res_win_t widget, const tchar_t* token);

	XDC_API void dropbox_filter(res_win_t widget, const tchar_t* token);

	XDC_API void dropbox_popup_size(res_win_t widget, xsize_t* pxs);


	XDC_API res_win_t wordsbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void	wordsbox_set_data(res_win_t widget, link_t_ptr ptr);

	XDC_API link_t_ptr wordsbox_get_data(res_win_t widget);

	XDC_API void	wordsbox_redraw(res_win_t widget);

	XDC_API void	wordsbox_tabskip(res_win_t widget, int nSkip);

	XDC_API bool_t	wordsbox_set_focus_item(res_win_t widget, link_t_ptr ilk);

	XDC_API link_t_ptr wordsbox_get_focus_item(res_win_t widget);

	XDC_API void	wordsbox_move_first_page(res_win_t widget);

	XDC_API void	wordsbox_move_prev_page(res_win_t widget);

	XDC_API void	wordsbox_move_next_page(res_win_t widget);

	XDC_API void	wordsbox_move_last_page(res_win_t widget);

	XDC_API void	wordsbox_move_to_page(res_win_t widget, int page);

	XDC_API int		wordsbox_get_cur_page(res_win_t widget);

	XDC_API int		wordsbox_get_max_page(res_win_t widget);

	XDC_API void wordsbox_find(res_win_t widget, link_t_ptr pos, const tchar_t* token);

	XDC_API void wordsbox_filter(res_win_t widget, const tchar_t* token);

	XDC_API link_t_ptr wordsbox_seek(res_win_t widget, int index);

	XDC_API void wordsbox_popup_size(res_win_t widget, xsize_t* pse);


	XDC_API res_win_t menubox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void	menubox_set_data(res_win_t widget, link_t_ptr ptr);

	XDC_API link_t_ptr menubox_get_data(res_win_t widget);

	XDC_API void	menubox_redraw(res_win_t widget);

	XDC_API void	menubox_tabskip(res_win_t widget, int nSkip);

	XDC_API void	menubox_set_focus_item(res_win_t widget, link_t_ptr ilk);

	XDC_API link_t_ptr menubox_get_focus_item(res_win_t widget);

	XDC_API void	menubox_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

	XDC_API void	menubox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API void	menubox_layout(res_win_t widget, const xpoint_t* ppt, int lay);


	XDC_API res_win_t keybox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void keybox_popup_size(res_win_t widget, xsize_t* pxs);

	XDC_API res_win_t show_keybox(const xpoint_t* ppt);


	XDC_API res_win_t editbox_create(res_win_t widget, dword_t style, const xrect_t* pxr);

	XDC_API void editbox_redraw(res_win_t widget);

	XDC_API void editbox_selectall(res_win_t widget);

	XDC_API void editbox_set_text(res_win_t widget, const tchar_t* text);

	XDC_API int editbox_get_text(res_win_t widget, tchar_t* buf, int max);

	XDC_API const tchar_t* editbox_get_text_ptr(res_win_t widget);

	XDC_API void editbox_auto_size(res_win_t widget, bool_t bSize);

	XDC_API bool_t editbox_is_select(res_win_t widget);

	XDC_API bool_t editbox_is_multiline(res_win_t widget);

	XDC_API void editbox_set_lock(res_win_t widget, bool_t bLock);

	XDC_API bool_t editbox_get_lock(res_win_t widget);


#ifdef	__cplusplus
}
#endif

#endif /*XDCBOX_H*/