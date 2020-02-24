/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc docker document

	@module	docker.h | interface file

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

#ifndef _DOCKER_H
#define _DOCKER_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API void hand_docker_mouse_move(docker_t* ptd, dword_t dw, const xpoint_t* pxp);

XDC_API void hand_docker_lbutton_down(docker_t* ptd, const xpoint_t* pxp);

XDC_API void hand_docker_lbutton_up(docker_t* ptd, const xpoint_t* pxp);

XDC_API void hand_docker_paint(docker_t* ptd, res_ctx_t rdc, const xrect_t* pxr);

XDC_API void hand_docker_calc_rect(docker_t* ptd, dword_t style, xrect_t* pxr);

#ifdef	__cplusplus
}
#endif


#endif /*DOCKER_H*/