/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc canvas interface document

	@module	gdiinf.h | interface file

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

#ifndef _GDIINF_H
#define _GDIINF_H

#include "xdcdef.h"

#if defined(XDU_SUPPORT_CONTEXT)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_canvas_interface: create canvas interface.
@INPUT canvas_t canv: the canvas object.
@RETURN if_canvas_t*: if succeeds return canvas interface struct, fails return NULL.
*/
EXP_API if_canvas_t* create_canvas_interface(canvas_t canv);

/*
@FUNCTION destroy_canvas_interface: destroy canvas interface.
@INPUT if_canvas_t*: the canvas interface struct.
@RETURN void: none.
*/
EXP_API void	destroy_canvas_interface(if_canvas_t* pic);

/*
@FUNCTION get_canvas_visual: fill the view functions.
@INPUT canvas_t canv: the canvas object.
@OUTPUT if_visual_t piv: the measure struct for returning inner function.
@RETURN void: none.
*/
EXP_API void get_visual_interface(canvas_t canv, if_visual_t* piv);

/*
@FUNCTION create_visual_interface: create view interface.
@INPUT visual_t ctx: the context object.
@RETURN if_viewING_t*: if succeeds return view interface struct, fails return NULL.
*/
EXP_API if_visual_t* create_visual_interface(visual_t rdc);

/*
@FUNCTION destroy_view_interface: destroy view interface.
@INPUT if_visual_t*: the view interface struct.
@RETURN void: none.
*/
EXP_API void	destroy_visual_interface(if_visual_t* pic);

LOC_API void	get_visual_measure(visual_t view, if_measure_t* pim);

LOC_API void	get_canvas_measure(canvas_t canv, if_measure_t* pim);

#ifdef	__cplusplus
}
#endif

#endif /*XDU_SUPPORT_CONTEXT*/

#endif /*GDIINF_H*/