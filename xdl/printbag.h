/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl print document

	@module	printbag.h | interface file

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

#ifndef _PRINTBAG_H
#define _PRINTBAG_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef XDL_SUPPORT_VIEW

XDL_API int calc_form_pages(canvas_t canv, link_t_ptr form);

#endif

#ifdef XDK_SUPPORT_CONTEXT_PRINTER

XDL_API void print_form(const dev_prn_t* pdev, link_t_ptr form);

XDL_API void print_grid(const dev_prn_t* pdev, link_t_ptr grid);

XDL_API void print_graph(const dev_prn_t* pdev, link_t_ptr graph);

XDL_API void print_topog(const dev_prn_t* pdev, link_t_ptr topog);

XDL_API void print_dialog(const dev_prn_t* pdev, link_t_ptr dialog);

XDL_API void print_chart(const dev_prn_t* pdev, link_t_ptr chart);

XDL_API void print_memo(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr memo);

XDL_API void print_rich(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr rich);

XDL_API void print_svg(const dev_prn_t* pdev, link_t_ptr svg);

XDL_API void svg_print_form(link_t_ptr svg, link_t_ptr form, int page);

XDL_API void svg_print_grid(link_t_ptr svg, link_t_ptr grid, int page);

XDL_API void svg_print_graph(link_t_ptr svg, link_t_ptr graph, int page);

XDL_API void svg_print_topog(link_t_ptr svg, link_t_ptr topog);

XDL_API void svg_print_dialog(link_t_ptr svg, link_t_ptr dialog);

XDL_API void svg_print_chart(link_t_ptr svg, link_t_ptr chart);

XDL_API void svg_print_memo(link_t_ptr svg, const xfont_t* pxf, const xface_t* pxa, link_t_ptr memo, int page);

XDL_API void svg_print_rich(link_t_ptr svg, const xfont_t* pxf, const xface_t* pxa, link_t_ptr rich, int page);

#endif /*XDK_SUPPORT_CONTEXT_PRINTER*/

#ifdef	__cplusplus
}
#endif


#endif /*PRINTBAG_H*/