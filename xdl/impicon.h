/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc icon document

	@module	impicon.h | icon interface file

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

#ifndef _IMPICON_H
#define _IMPICON_H

#include "xdldef.h"

#define ICON_TEXT_POINT		_T("↖")
#define ICON_TEXT_CROSS		_T("×")
#define ICON_TEXT_RECT		_T("■")
#define ICON_TEXT_ELLIPSE	_T("●")
#define ICON_TEXT_STAR		_T("★")
#define ICON_TEXT_DIAMOND	_T("◆")

#if defined(XDK_SUPPORT_CONTEXT) && defined(XDK_SUPPORT_CONTEXT_GRAPHIC)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void	draw_logo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_collapse_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_expand_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_insert_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_delete_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_plus_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_minus_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_guider_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_close_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_maximize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_minimize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_restore_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_folder_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_sum_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_checkbox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_checked_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_radiobox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_radioed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_selected_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_home_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_up_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_down_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_end_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_first_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_prev_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_next_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_last_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_zoomin_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_zoomout_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_keybox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_keyboxed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_touch_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_touched_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_line_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_rect_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_ellipse_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_fixed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_color_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_point_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_cross_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_star_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_diamond_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

XDL_API void	draw_numeric_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* no);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*IMPICON_H*/