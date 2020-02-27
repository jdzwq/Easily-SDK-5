/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdc print document

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

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API int calc_form_pages(canvas_t canv, link_t_ptr form);

#if defined(XDU_SUPPORT_CONTEXT_PRINTER)

XDC_API void print_form(const dev_prn_t* pdev, link_t_ptr form);

XDC_API void print_grid(const dev_prn_t* pdev, link_t_ptr grid);

XDC_API void print_statis(const dev_prn_t* pdev, link_t_ptr statis);

XDC_API void print_topog(const dev_prn_t* pdev, link_t_ptr topog);

XDC_API void print_dialog(const dev_prn_t* pdev, link_t_ptr dialog);

XDC_API void print_diagram(const dev_prn_t* pdev, link_t_ptr diagram);

XDC_API void print_memo(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr memo);

XDC_API void print_rich(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr rich);

XDC_API void print_svg(const dev_prn_t* pdev, link_t_ptr svg);

XDC_API void svg_print_form(link_t_ptr svg, link_t_ptr form, int page);

XDC_API void svg_print_grid(link_t_ptr svg, link_t_ptr grid, int page);

XDC_API void svg_print_statis(link_t_ptr svg, link_t_ptr statis, int page);

XDC_API void svg_print_topog(link_t_ptr svg, link_t_ptr topog);

XDC_API void svg_print_dialog(link_t_ptr svg, link_t_ptr dialog);

XDC_API void svg_print_diagram(link_t_ptr svg, link_t_ptr diagram);

XDC_API void svg_print_memo(link_t_ptr svg, const xfont_t* pxf, const xface_t* pxa, link_t_ptr memo, int page);

XDC_API void svg_print_rich(link_t_ptr svg, const xfont_t* pxf, const xface_t* pxa, link_t_ptr rich, int page);

#endif /*XDU_SUPPORT_CONTEXT_PRINTER*/

#ifdef	__cplusplus
}
#endif

#endif /*PRINTBAG_H*/