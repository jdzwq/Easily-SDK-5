/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc svg utility document

	@module	svgbag.h | svg utility interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#ifndef _SVGBAG_H
#define _SVGBAG_H

#include "xdcdef.h"

XDC_API void svg_print_form(LINKPTR svg, LINKPTR form, int page);

XDC_API void svg_print_grid(LINKPTR svg, LINKPTR grid, int page);

XDC_API void svg_print_graph(LINKPTR svg, LINKPTR graph, int page);

XDC_API void svg_print_memo(LINKPTR svg, LINKPTR memo, int page);

XDC_API void svg_print_rich(LINKPTR svg, LINKPTR rich, int page);

XDC_API void svg_print_topog(LINKPTR svg, LINKPTR topog);

XDC_API void svg_print_dialog(LINKPTR svg, LINKPTR dialog);

#endif /*_SVGBAG_H*/