/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc svg utility document

	@module	svgbag.c | svg utility implement file

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
#include "svgbag.h"
#include "svgcanv.h"
#include "svginf.h"

void svg_print_form(LINKPTR svg, LINKPTR form, int page)
{
	xhand_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	canv = create_svg_canvas(svg);

	pif = create_svg_interface(canv);

	cb.fw = get_svg_width(svg);
	cb.fh = get_svg_height(svg);

	draw_form_page(pif, &cb, form, page);
	
	destroy_svg_canvas(canv);
	destroy_svg_interface(pif);
}

void svg_print_grid(LINKPTR svg, LINKPTR grid, int page)
{
	xhand_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	canv = create_svg_canvas(svg);

	pif = create_svg_interface(canv);

	cb.fw = get_svg_width(svg);
	cb.fh = get_svg_height(svg);

	draw_grid_page(pif, &cb, grid, page);

	destroy_svg_canvas(canv);
	destroy_svg_interface(pif);
}

void svg_print_graph(LINKPTR svg, LINKPTR graph, int page)
{
	xhand_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	canv = create_svg_canvas(svg);

	pif = create_svg_interface(canv);

	cb.fw = get_svg_width(svg);
	cb.fh = get_svg_height(svg);

	draw_graph_page(pif, &cb, graph, page);

	destroy_svg_canvas(canv);
	destroy_svg_interface(pif);
}

void svg_print_memo(LINKPTR svg, LINKPTR memo, int page)
{
	xhand_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	canv = create_svg_canvas(svg);

	pif = create_svg_interface(canv);

	cb.fw = get_svg_width(svg);
	cb.fh = get_svg_height(svg);

	destroy_svg_canvas(canv);
	destroy_svg_interface(pif);
}

void svg_print_rich(LINKPTR svg, LINKPTR memo, int page)
{
	xhand_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	canv = create_svg_canvas(svg);

	pif = create_svg_interface(canv);

	cb.fw = get_svg_width(svg);
	cb.fh = get_svg_height(svg);

	destroy_svg_canvas(canv);
	destroy_svg_interface(pif);
}

void svg_print_topog(LINKPTR svg, LINKPTR topog)
{
	xhand_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	canv = create_svg_canvas(svg);

	pif = create_svg_interface(canv);

	cb.fw = get_svg_width(svg);
	cb.fh = get_svg_height(svg);

	draw_topog(pif, &cb, topog);

	destroy_svg_canvas(canv);
	destroy_svg_interface(pif);
}

void svg_print_dialog(LINKPTR svg, LINKPTR dialog)
{
	xhand_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	canv = create_svg_canvas(svg);

	pif = create_svg_interface(canv);

	cb.fw = get_svg_width(svg);
	cb.fh = get_svg_height(svg);

	draw_dialog(pif, &cb, dialog);

	destroy_svg_canvas(canv);
	destroy_svg_interface(pif);
}
