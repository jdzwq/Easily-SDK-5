/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc statis document

	@module	statisview.h | interface file

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

#ifndef _STATISVIEW_H
#define _STATISVIEW_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_VIEW

typedef enum{
	STATIS_HINT_NONE,
	STATIS_HINT_MENU,
	STATIS_HINT_TITLE,
	STATIS_HINT_NULBAR,
	STATIS_HINT_GAXBAR,
	STATIS_HINT_YAXBAR,
	STATIS_HINT_XAXBAR,
	STATIS_HINT_COOR,
	STATIS_HINT_VERT_SPLIT,
	STATIS_HINT_HORZ_SPLIT,
	STATIS_HINT_MIDD_SPLIT,
}STATIS_HINT_CODE;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API float calc_statis_page_width(link_t_ptr ptr, int page);

	EXP_API int calc_statis_pages(link_t_ptr ptr);

	EXP_API void calc_statis_xax_scope(link_t_ptr ptr, int page, link_t_ptr* firstxax, link_t_ptr* lastxax);

	EXP_API int calc_statis_xax_page(link_t_ptr ptr, link_t_ptr xlk);

	EXP_API void calc_statis_gax_rect(link_t_ptr ptr, link_t_ptr ylk, xrect_t* pxr);

	EXP_API void calc_statis_yax_rect(link_t_ptr ptr, link_t_ptr glk, xrect_t* pxr);

	EXP_API int calc_statis_coor_rect(link_t_ptr ptr, int page, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr);

	EXP_API void calc_statis_xax_rect(link_t_ptr ptr, int page, link_t_ptr xlk, xrect_t* pxr);

	EXP_API int calc_statis_hint(const xpoint_t* ppt, link_t_ptr ptr, int page, link_t_ptr* pxlk, link_t_ptr* pylk, link_t_ptr* pglk);

	EXP_API void draw_statis_page(const drawing_interface* pcanv, link_t_ptr ptr, int page);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_VIEW*/

#endif /*STATISVIEW_H*/