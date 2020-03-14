/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc boxing document

	@module	boxview.h | interface file

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

#ifndef _BOXVIEW_H
#define _BOXVIEW_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_VIEW

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API void draw_pushbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, const tchar_t* text);

typedef enum{
	DATEBOX_HINT_NONE = 0,
	DATEBOX_HINT_PREV = 1,
	DATEBOX_HINT_NEXT = 2,
	DATEBOX_HINT_DAYS = 3,
}DATEBOX_HINT_CODE;

EXP_API void calc_datebox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API void calc_datebox_day_rect(const if_measure_t* pim, const xfont_t* pxf, const xdate_t* pdt, xrect_t* pxr);

EXP_API int	calc_datebox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt, const xdate_t* pdt, int* pday);

EXP_API void draw_datebox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf,  const xdate_t* pdt);

typedef enum{
	TIMEBOX_HINT_NONE = 0,
	TIMEBOX_HINT_YEAR_UP = 1,
	TIMEBOX_HINT_YEAR_DOWN = 2,
	TIMEBOX_HINT_MON_UP = 3,
	TIMEBOX_HINT_MON_DOWN = 4,
	TIMEBOX_HINT_DAY_UP = 5,
	TIMEBOX_HINT_DAY_DOWN = 6,
	TIMEBOX_HINT_HOUR_UP = 7,
	TIMEBOX_HINT_HOUR_DOWN = 8,
	TIMEBOX_HINT_HOUR_ZERO = 9,
	TIMEBOX_HINT_MIN_UP = 10,
	TIMEBOX_HINT_MIN_DOWN = 11,
	TIMEBOX_HINT_MIN_ZERO = 12,
	TIMEBOX_HINT_SEC_UP = 13,
	TIMEBOX_HINT_SEC_DOWN = 14,
	TIMEBOX_HINT_SEC_ZERO = 15,
}TIMEBOX_HINT_CODE;

EXP_API void calc_timebox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API int	calc_timebox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt);

EXP_API void draw_timebox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, const xdate_t* ptt);

typedef enum{
	LISTBOX_HINT_NONE = 0,
	LISTBOX_HINT_ITEM = 1,
}LISTBOX_HINT_CODE;

EXP_API void calc_listbox_size(const if_measure_t* pim, const xfont_t* pxf, link_t_ptr ptr, xsize_t* pxs);

EXP_API int calc_listbox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pilk);

EXP_API void calc_listbox_item_rect(const if_measure_t* pim, const xfont_t* pxf, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr);

EXP_API void draw_listbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, link_t_ptr ptr);

typedef enum{
	DROPBOX_HINT_NONE = 0,
	DROPBOX_HINT_ITEM = 1,
}DROPBOX_HINT_CODE;

EXP_API void calc_dropbox_size(const if_measure_t* pim, const xfont_t* pxf, link_t_ptr ptr, xsize_t* pxs);

EXP_API int calc_dropbox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pilk);

EXP_API void calc_dropbox_item_rect(const if_measure_t* pim, const xfont_t* pxf, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr);

EXP_API void draw_dropbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, link_t_ptr ptr);


typedef enum{
	RADIOBOX_HINT_NONE = 0,
	RADIOBOX_HINT_ON = 1,
	RADIOBOX_HINT_OFF = 2,
}RADIOBOX_HINT_CODE;

EXP_API void calc_radiobox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API int	calc_radiobox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt);

EXP_API void draw_radiobox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, bool_t b_on);


EXP_API void calc_checkbox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API void draw_checkbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, bool_t b_on);


EXP_API void calc_slidebox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API void calc_slidebox_button_rect(const if_measure_t* pim, const xfont_t* pxf, int pos, xrect_t* pxr);

EXP_API int	calc_slidebox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt);

EXP_API void draw_slidebox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, int pos);


typedef enum{
	SPINBOX_HINT_NONE = 0,
	SPINBOX_HINT_PLUS = 1,
	SPINBOX_HINT_MINUS = 2,
}SPINBOX_HINT_CODE;

EXP_API void calc_spinbox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API int	calc_spinbox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt);

EXP_API void draw_spinbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, int cur);

typedef enum{
	NAVIBOX_HINT_NONE = 0,
	NAVIBOX_HINT_HOME = 1,
	NAVIBOX_HINT_END = 2,
	NAVIBOX_HINT_PREV = 3,
	NAVIBOX_HINT_NEXT = 4,
	NAVIBOX_HINT_KEYBOARD = 5
}NAVIBOX_HINT_CODE;

typedef struct _NAVISTATE{
	bool_t keyboxed;
}NAVISTATE;

EXP_API void calc_navibox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API int	calc_navibox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt);

EXP_API void draw_navibox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, const NAVISTATE* pns);


typedef enum{
	ICONBOX_HINT_NONE = 0,
	ICONBOX_HINT_ITEM = 1,
}ICONBOX_HINT_CODE;

EXP_API void calc_iconbox_size(const if_measure_t* pim, const xfont_t* pxf, const tchar_t* layer, const tchar_t* align, link_t_ptr str, xsize_t* pxs);

EXP_API void calc_iconbox_item_rect(const if_measure_t* pim, const xfont_t* pxf, const tchar_t* layer, const tchar_t* align, const xsize_t* pxs, link_t_ptr str, link_t_ptr ilk, xrect_t* pxr);

EXP_API int calc_iconbox_hint(const if_measure_t* pim, const xfont_t* pxf, const tchar_t* layer, const tchar_t* align, const xsize_t* pxs, const xpoint_t* ppt, link_t_ptr str, link_t_ptr* pilk);

EXP_API void draw_iconbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, const tchar_t* layer, const tchar_t* align, link_t_ptr str);


typedef enum{
	WORDSBOX_HINT_NONE = 0,
	WORDSBOX_HINT_ITEM = 1,
}WORDSBOX_HINT_CODE;

EXP_API int calc_wordsbox_pages(link_t_ptr ptr);

EXP_API int calc_wordsbox_item_page(link_t_ptr ptr, link_t_ptr plk);

EXP_API void calc_wordsbox_item_scope(link_t_ptr ptr, int page, link_t_ptr* pfirst, link_t_ptr* plast);

EXP_API link_t_ptr calc_wordsbox_item(link_t_ptr ptr, int page, int index);

EXP_API void calc_wordsbox_size(const if_measure_t* pim, const xfont_t* pxf, link_t_ptr ptr, xsize_t* pxs);

EXP_API void calc_wordsbox_item_rect(const if_measure_t* pim, const xfont_t* pxf, link_t_ptr ptr, int page, link_t_ptr ilk, xrect_t* pxr);

EXP_API int calc_wordsbox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt, link_t_ptr ptr, int page, link_t_ptr* pilk);

EXP_API void draw_wordsbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf, link_t_ptr ptr, int page);

typedef enum{
	VERTBOX_HINT_NONE = 0,
	VERTBOX_HINT_PAGEUP = 1,
	VERTBOX_HINT_LINEUP = 2,
	VERTBOX_HINT_LINEDOWN = 3,
	VERTBOX_HINT_PAGEDOWN = 4,
}VERTBOX_HINT_CODE;

EXP_API void calc_vertbox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API int	calc_vertbox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt);

EXP_API void draw_vertbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf);

typedef enum{
	HORZBOX_HINT_NONE = 0,
	HORZBOX_HINT_PAGELEFT = 1,
	HORZBOX_HINT_LINELEFT = 2,
	HORZBOX_HINT_LINERIGHT = 3,
	HORZBOX_HINT_PAGERIGHT = 4,
}HORZBOX_HINT_CODE;

EXP_API void calc_horzbox_size(const if_measure_t* pim, const xfont_t* pxf, xsize_t* pxs);

EXP_API int	calc_horzbox_hint(const if_measure_t* pim, const xfont_t* pxf, const xpoint_t* ppt);

EXP_API void draw_horzbox(const if_canvas_t* pif, const canvbox_t* pbox, const xfont_t* pxf);

#ifdef	__cplusplus
}
#endif

#endif //XDLVIEW

#endif	/*_BOXVIEW_H*/