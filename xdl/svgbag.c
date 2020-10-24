/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg export document

	@module	svgbag.c | implement file

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

#include "svgbag.h"
#include "svginf.h"
#include "svgcanv.h"

#include "xdlstd.h"
#include "xdlgdi.h"
#include "xdldoc.h"
#include "xdlview.h"

#ifdef XDL_SUPPORT_VIEW

void svg_print_form(link_t_ptr svg, link_t_ptr form, int page)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);
	ifd.rect.fx = 0;
	ifd.rect.fy = 0;
	ifd.rect.fw = get_form_width(form);
	ifd.rect.fh = get_form_height(form);

	set_svg_width(svg, ifd.rect.fw);
	set_svg_height(svg, ifd.rect.fh);
	reset_svg_viewbox(svg);

	draw_form_page(&ifd, form, page);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_grid(link_t_ptr svg, link_t_ptr grid, int page)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);

	ifd.rect.fx = 0;
	ifd.rect.fy = 0;
	ifd.rect.fw = get_grid_width(grid);
	ifd.rect.fh = get_grid_height(grid);

	set_svg_width(svg, ifd.rect.fw);
	set_svg_height(svg, ifd.rect.fh);
	reset_svg_viewbox(svg);

	draw_grid_page(&ifd, grid, page);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_statis(link_t_ptr svg, link_t_ptr statis, int page)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);

	ifd.rect.fx = 0;
	ifd.rect.fy = 0;
	ifd.rect.fw = get_statis_width(statis);
	ifd.rect.fh = get_statis_height(statis);

	set_svg_width(svg, ifd.rect.fw);
	set_svg_height(svg, ifd.rect.fh);
	reset_svg_viewbox(svg);

	draw_statis_page(&ifd, statis, page);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_topog(link_t_ptr svg, link_t_ptr topog)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);

	ifd.rect.fx = 0;
	ifd.rect.fy = 0;
	ifd.rect.fw = get_topog_width(topog);
	ifd.rect.fh = get_topog_height(topog);

	set_svg_width(svg, ifd.rect.fw);
	set_svg_height(svg, ifd.rect.fh);
	reset_svg_viewbox(svg);

	draw_topog(&ifd, topog);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_dialog(link_t_ptr svg, link_t_ptr dialog)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);

	ifd.rect.fx = 0;
	ifd.rect.fy = 0;
	ifd.rect.fw = get_dialog_width(dialog);
	ifd.rect.fh = get_dialog_height(dialog);

	set_svg_width(svg, ifd.rect.fw);
	set_svg_height(svg, ifd.rect.fh);
	reset_svg_viewbox(svg);

	draw_dialog(&ifd, dialog);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_diagram(link_t_ptr svg, link_t_ptr diagram)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);
	ifd.rect.fx = 0;
	ifd.rect.fy = 0;
	ifd.rect.fw = get_diagram_width(diagram);
	ifd.rect.fh = get_diagram_height(diagram);

	set_svg_width(svg, ifd.rect.fw);
	set_svg_height(svg, ifd.rect.fh);
	reset_svg_viewbox(svg);

	draw_diagram(&ifd, diagram);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_plot(link_t_ptr svg, link_t_ptr plot)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);

	ifd.rect.fx = 0;
	ifd.rect.fy = 0;
	ifd.rect.fw = get_plot_width(plot);
	ifd.rect.fh = get_plot_height(plot);

	set_svg_width(svg, ifd.rect.fw);
	set_svg_height(svg, ifd.rect.fh);
	reset_svg_viewbox(svg);

	draw_plot(&ifd, plot);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_memo(link_t_ptr svg, const xfont_t* pxf, const xface_t* pxa, link_t_ptr memo, int page)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};
	xrect_t xr = { 0 };

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);

	xr.fw = get_memo_width(memo);
	xr.fh = get_memo_height(memo);

	set_svg_width(svg, xr.fw);
	set_svg_height(svg, xr.fh);
	reset_svg_viewbox(svg);

	draw_memo_text(&ifd, pxf, pxa, &xr, memo, page);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}

void svg_print_rich(link_t_ptr svg, const xfont_t* pxf, const xface_t* pxa, link_t_ptr rich, int page)
{
	visual_t view;
	canvas_t canv;
	if_drawing_t ifd = {0};
	xrect_t xr = { 0 };

	view = create_svg_visual(svg);

	canv = create_svg_canvas(view);

	svg_get_canvas_interface(canv, &ifd);

	xr.fw = get_rich_width(svg);
	xr.fh = get_rich_height(svg);

	set_svg_width(svg, xr.fw);
	set_svg_height(svg, xr.fh);
	reset_svg_viewbox(svg);

	draw_rich_text(&ifd, pxf, pxa, &xr, rich, page);

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
	
}



#endif //
