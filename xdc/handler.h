/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget default handler document

	@module	handler.h | interface file

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

#ifndef _HANDLER_H
#define _HANDLER_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API void	widgetex_hand_create(res_win_t wt);

XDC_API void	widgetex_hand_destroy(res_win_t wt);

XDC_API bool_t	widgetex_hand_scroll(res_win_t wt, bool_t b_horz, int pos);

/******************************widget splitor implement*****************************************************/

XDC_API void widgetex_splitor_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

XDC_API void widgetex_splitor_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

XDC_API void widgetex_splitor_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

XDC_API void widgetex_splitor_on_size(res_win_t widget, int code, const xsize_t* pxs);

XDC_API void widgetex_splitor_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

XDC_API int widgetex_splitor_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widgetex_splitor_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widgetex_splitor_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widgetex_splitor_sub_size(res_win_t widget, int code, const xsize_t* pxs, uid_t sid, var_long delta);

XDC_API int widgetex_splitor_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);

/******************************widget docker implement*****************************************************/

XDC_API void widgetex_docker_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

XDC_API void widgetex_docker_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

XDC_API void widgetex_docker_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

XDC_API void widgetex_docker_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

XDC_API int widgetex_docker_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widgetex_docker_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widgetex_docker_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widgetex_docker_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);


#ifdef	__cplusplus
}
#endif


#endif /*HANDLER_H*/