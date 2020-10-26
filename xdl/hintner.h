/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc view hintner document

	@module	hintner.h | interface file

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

#ifndef _HINTNER_H
#define _HINTNER_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_VIEW

typedef enum{
	HINT_NONE,
	HINT_OBJECT,
	HINT_GROUP,
	HINT_VERT_SPLIT,
	HINT_HORZ_SPLIT,
	HINT_CROSS_SPLIT,
	HINT_DRAG_CORNER
}HINT_CODE;


typedef enum{
	_HINTNER_OPERA_STOP = 0,
	_HINTNER_OPERA_NEXT = 1
}HINTNER_OPERA;

typedef enum{
	_HINTNER_STATE_BEGIN = 0,
	_HINTNER_STATE_OBJECT = 1,
	_HINTNER_STATE_END = -1,
}HINTNER_STATE;

typedef int(*PF_HINT_CALLBACK)(int state, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr, bool_t focus, bool_t drag, bool_t sizew, bool_t sizeh, void* pp);


typedef int(*PF_HINT_NEXT)(void* param, link_t_ptr* p_xlk, link_t_ptr* p_ylk, xrect_t* p_rect, bool_t* p_focus, bool_t* p_drag, bool_t* p_sizew, bool_t* p_sizeh);

typedef struct _if_hint_t{
	PF_HINT_NEXT	pf_hint_next;
	void* param;
}if_hint_t;


#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API void hint_test(if_hint_t* pit, PF_HINT_CALLBACK pf, void* pp);


#ifdef	__cplusplus
}
#endif

#endif

#endif /*HINTNER_H*/