/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc grid view document

	@module	gridview.h | grid view interface file

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

#ifndef _GRIDVIEW_H
#define _GRIDVIEW_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_VIEW

typedef enum{
	GRID_HINT_NONE,
	GRID_HINT_MENU,
	GRID_HINT_TITLE,
	GRID_HINT_NULBAR,
	GRID_HINT_COLBAR,
	GRID_HINT_ROWBAR,
	GRID_HINT_CELL,
	GRID_HINT_VERT_SPLIT,
	GRID_HINT_HORZ_SPLIT,
}GRID_HINT_CODE;

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API float calc_grid_page_width(link_t_ptr ptr);

XDL_API float calc_grid_page_height(const canvbox_t* pbox, link_t_ptr ptr, int page);

XDL_API int calc_grid_row_scope(const canvbox_t* pbox, link_t_ptr ptr, int page, link_t_ptr* pfirst, link_t_ptr* plast);

XDL_API int calc_grid_row_page(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr rlk);

XDL_API int calc_grid_pages(const canvbox_t* pbox, link_t_ptr ptr);

XDL_API int calc_grid_cell_rect(const canvbox_t* pbox, link_t_ptr ptr, int page, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr);

XDL_API int calc_grid_row_rect(const canvbox_t* pbox, link_t_ptr ptr, int page, link_t_ptr rlk, xrect_t* pxr);

XDL_API int calc_grid_col_rect(const canvbox_t* pbox, link_t_ptr ptr, int page, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr);

XDL_API int calc_grid_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, int page, link_t_ptr* prlk, link_t_ptr* pclk);

XDL_API void draw_grid_page(const if_canvas_t* pcanv, const canvbox_t* pbox, link_t_ptr ptr, int page);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_VIEW*/

#endif /*GRIDVIEW_H*/