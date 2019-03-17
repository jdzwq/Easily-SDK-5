/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc splitor document

	@module	splitor.h | splitor interface file

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

#ifndef _SPLITOR_H
#define _SPLITOR_H

#include "xdcdef.h"


typedef struct _splitor_t{
	res_win_t widget;
	link_t_ptr split;
	link_t_ptr item;
	long x, y;
}splitor_t;


#ifdef	__cplusplus
extern "C" {
#endif

XDC_API bool_t hand_splitor_mouse_move(splitor_t* ptd, dword_t dw, const xpoint_t* pxp);

XDC_API bool_t hand_splitor_lbutton_down(splitor_t* ptd, const xpoint_t* pxp);

XDC_API bool_t hand_splitor_lbutton_up(splitor_t* ptd, const xpoint_t* pxp);

XDC_API void hand_splitor_size(splitor_t* ptd, const xrect_t* pxr);

XDC_API void hand_splitor_paint(splitor_t* ptd, res_ctx_t rdc);

#ifdef	__cplusplus
}
#endif


#endif /*SPLITOR_H*/