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

#if defined(XDL_SUPPORT_GDI)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_svg_interface: create svg canvas interface.
@INPUT canvas_t canv: the svg canvas object.
@RETURN drawing_interface*: if succeeds return svg canvas interface struct, fails return NULL.
*/
EXP_API void svg_get_canvas_interface(canvas_t canv, drawing_interface* pif);

/*
@FUNCTION create_visual_interface: create svg view interface.
@INPUT visual_t view: the context object.
@RETURN if_viewING_t*: if succeeds return view interface struct, fails return NULL.
*/
EXP_API void svg_get_visual_interface(visual_t visu, drawing_interface* pif);


#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_VIEW*/

#endif /*SVGINF_H*/