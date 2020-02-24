/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc split control document

	@module	splitctrl.h | split control interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#ifndef _SPLITCTRL_H
#define _SPLITCTRL_H

#include "xdcdef.h"

/*********************************************************************************************/
XDC_API void	hand_split_erase(res_win_t widget, res_dc_t dc, const xrect_t* pxr, const xpaint_t* pxn);

XDC_API void	hand_split_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr, const xpaint_t* pxn);

XDC_API void	hand_split_size(res_win_t widget, short cx, short cy);

XDC_API void	hand_split_mouse_move(res_win_t widget, short x, short y);

XDC_API void	hand_split_lbutton_down(res_win_t widget, short x, short y);

XDC_API void	hand_split_lbutton_up(res_win_t widget, short x, short y);

/*********************************************************************************************/
XDC_API void		split_view_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr	split_view_detach(res_win_t widget);

XDC_API link_t_ptr	split_view_fetch(res_win_t widget);

XDC_API void		split_view_tm_to_pt(res_win_t widget, xpoint_t *ppt);

XDC_API void		split_view_pt_to_tm(res_win_t widget, xpoint_t *ppt);

XDC_API void		split_view_redraw(res_win_t widget);

#endif