/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg interface document

	@module	svginf.h | interface file

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

#ifndef _SVGINF_H
#define _SVGINF_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_SVG)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_svg_interface: create svg canvas interface.
@INPUT canvas_t canv: the svg canvas object.
@RETURN if_canvas_t*: if succeeds return svg canvas interface struct, fails return NULL.
*/
EXP_API if_canvas_t* svg_create_canvas_interface(canvas_t canv);

/*
@FUNCTION destroy_svg_interface: destroy svg canvas interface.
@INPUT if_canvas_t*: the svg canvas interface struct.
@RETURN void: none.
*/
EXP_API void svg_destroy_canvas_interface(if_canvas_t* pic);

/*
@FUNCTION svg_get_visual_interface: fill the svg canvas measure functions.
@INPUT canvas_t canv: the svg canvas object.
@OUTPUT if_measure_t pif: the measure struct for returning inner function.
@RETURN void: none.
*/
EXP_API void svg_get_visual_interface(canvas_t canv, if_visual_t* piv);

/*
@FUNCTION create_visual_interface: create svg view interface.
@INPUT visual_t view: the context object.
@RETURN if_viewING_t*: if succeeds return view interface struct, fails return NULL.
*/
EXP_API if_visual_t* svg_create_visual_interface(visual_t view);

/*
@FUNCTION destroy_view_interface: destroy svg view interface.
@INPUT if_visual_t*: the view interface struct.
@RETURN void: none.
*/
EXP_API void	svg_destroy_visual_interface(if_visual_t* pic);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_SVG*/

#endif /*SVGINF_H*/