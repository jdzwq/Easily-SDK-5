/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc svg bag document

	@module	svgbag.c | svg bag document implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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
#include "xdllink.h"
#include "hashtable.h"
#include "stacktable.h"
#include "domdoc.h"
#include "xmldoc.h"
#include "xdlstr.h"
#include "xdlmem.h"
#include "xdlcode.h"
#include "xdlutil.h"

#include "svgdoc.h"

#include "formdoc.h"
#include "formview.h"
#include "gridview.h"
#include "griddoc.h"
#include "graphview.h"
#include "graphdoc.h"
#include "treeview.h"
#include "treedoc.h"

/*********************************************************************************/
void save_grid_to_svg_node(link_t_ptr grid, int page, link_t_ptr g)
{
	link_t_ptr clk, rlk;
	link_t_ptr rlk_first, rlk_last;
	long pw, ph, th, rw, ch, rh, offx, offy;
	bool_t b_design, b_sumrow, b_stepdraw, b_tag = 0;
	bool_t b_lastpage;

	const wchar_t *token, *shape, *style;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb_step, xb_bar, xb = { 0 };
	ximage_t xi = { 0 };
	xrect_t xrCell, xrBar;

	xrect_t xview = { 0 };
	link_t_ptr nlk;

	set_svg_node_name(g, DOC_SVG_G);

	xview.x = 0;
	xview.y = 0;
	xview.w = get_dom_node_attr_long(g, SVG_ATTR_WIDTH);
	xview.h = get_dom_node_attr_long(g, SVG_ATTR_HEIGHT);

	calc_grid_row_scope(&xview, grid, page, &rlk_first, &rlk_last);

	if (rlk_last)
		b_lastpage = (get_next_visible_row(grid, rlk_last)) ? 0 : 1;
	else
		b_lastpage = 0;

	b_design = get_grid_design_mode(grid);
	b_sumrow = get_grid_showsum(grid);
	b_stepdraw = get_grid_showstep(grid);

	th = get_grid_title_height(grid);
	rw = get_grid_rowbar_width(grid);
	ch = get_grid_colbar_height(grid);
	rh = get_grid_rowbar_height(grid);

	offx = xview.x;
	offy = xview.y;
	pw = xview.w;
	ph = xview.h;

	style = get_grid_style_ptr(grid);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, style);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, style);

	default_xface(&xa);
	parse_xface_from_style(&xa, style);

	memcpy((void*)&xb_bar, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bar, DEF_MIDD_DARKEN);

	memcpy((void*)&xb_step, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_step, DEF_SOFT_DARKEN);

	shape = get_grid_shape_ptr(grid);

	//draw title
	if (th)
	{
		xrBar.x = -offx;
		xrBar.y = -offy;
		xrBar.w = pw;
		xrBar.h = th;

		nlk = insert_svg_node(g);
		write_text_to_svg_node(nlk, &xf, &xa, &xrBar, get_grid_title_ptr(grid), -1);
	}

	//draw cell
	xrCell.x = rw - offx;
	clk = get_next_visible_col(grid, LINK_FIRST);
	while (clk)
	{
		style = get_col_style_ptr(clk);

		default_xfont(&xf);
		parse_xfont_from_style(&xf, style);

		default_xface(&xa);
		parse_xface_from_style(&xa, style);

		xrCell.y = th + ch - offy;
		xrCell.w = get_col_width(clk);
		xrCell.h = rh;

		b_tag = 0;
		rlk = rlk_first;
		while (rlk)
		{
			if (b_stepdraw && b_tag)
			{
				nlk = insert_svg_node(g);
				write_shape_to_svg_node(nlk, &xp, &xb_step, &xrCell, shape);
				b_tag = 0;
			}
			else
			{
				nlk = insert_svg_node(g);
				write_shape_to_svg_node(nlk, &xp, NULL, &xrCell, shape);
				b_tag = 1;
			}

			token = get_cell_options_text_ptr(rlk, clk);
			nlk = insert_svg_node(g);
			write_text_to_svg_node(nlk, &xf, &xa, &xrCell, token, -1);

			if (rlk_last == rlk)
				break;

			xrCell.y += xrCell.h;
			rlk = get_next_visible_row(grid, rlk);
		}

		if (b_sumrow && b_lastpage)
		{
			xrCell.y += xrCell.h;

			nlk = insert_svg_node(g);
			write_shape_to_svg_node(nlk, &xp, NULL, &xrCell, shape);

			token = get_col_sum_text_ptr(clk);
			nlk = insert_svg_node(g);
			write_text_to_svg_node(nlk, &xf, &xa, &xrCell, token, -1);
		}

		xrCell.x += xrCell.w;
		clk = get_next_visible_col(grid, clk);
	}

	//draw col bar
	xrBar.x = rw - offx;
	xrBar.y = th;
	xrBar.h = ch;
	clk = get_next_visible_col(grid, LINK_FIRST);
	while (clk)
	{
		style = get_col_style_ptr(clk);

		default_xfont(&xf);
		parse_xfont_from_style(&xf, style);

		default_xface(&xa);
		parse_xface_from_style(&xa, style);

		xrBar.w = get_col_width(clk);

		nlk = insert_svg_node(g);
		write_shape_to_svg_node(nlk, &xp, &xb_bar, &xrBar, shape);

		token = get_col_title_ptr(clk);
		nlk = insert_svg_node(g);
		write_text_to_svg_node(nlk, &xf, &xa, &xrBar, token, -1);

		xrBar.x += xrBar.w;
		clk = get_next_visible_col(grid, clk);
	}

	//draw row bar
	xrBar.x = 0;
	xrBar.w = rw;
	xrBar.y = th;
	xrBar.h = ch;

	nlk = insert_svg_node(g);
	write_shape_to_svg_node(nlk, &xp, &xb, &xrBar, shape);

	xrBar.y = th + ch - offy;
	xrBar.h = rh;
	rlk = rlk_first;
	while (rlk)
	{
		nlk = insert_svg_node(g);
		write_shape_to_svg_node(nlk, &xp, NULL, &xrBar, shape);

		if (rlk_last == rlk)
			break;

		xrBar.y += xrBar.h;
		rlk = get_next_visible_row(grid, rlk);
	}

	//draw sum bar
	if (b_sumrow && b_lastpage)
	{
		xrBar.y += xrBar.h;

		nlk = insert_svg_node(g);
		write_shape_to_svg_node(nlk, &xp, NULL, &xrBar, shape);
	}
}

void save_graph_to_svg_node(link_t_ptr graph, int page, link_t_ptr glk)
{
	link_t_ptr xlkFirst, xlkLast, ylk, xlk;
	long th, tw, xw, yh, yt, ph, pw;
	long x1, x2, y1, y2;
	long middnum, stepnum;
	long tmpx, midy, incy, cury, prey;
	float pred, nxtd, dby;
	double total;
	int i, steps, dalen, daprec;
	bool_t b_design;
	const wchar_t *type, *style, *color;
	wchar_t token[NUM_LEN + 1] = { 0 };
	xrect_t xr = { 0 };
	xpen_t xp_shape, xp = { 0 };
	xbrush_t xb_shape, xb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };
	xpoint_t xp1, xp2;
	long MINFEED = 10;

	xrect_t xview = { 0 };
	link_t_ptr nlk;

	set_svg_node_name(glk, DOC_SVG_G);

	xview.x = 0;
	xview.y = 0;
	xview.w = (long)(get_dom_node_attr_numeric(glk, SVG_ATTR_WIDTH) * 10);
	xview.h = (long)(get_dom_node_attr_numeric(glk, SVG_ATTR_HEIGHT) * 10);

	b_design = get_graph_design_mode(graph);
	style = get_graph_style_ptr(graph);
	type = get_graph_type_ptr(graph);

	ph = xview.h;
	pw = xview.w;
	th = get_graph_title_height(graph);
	tw = get_graph_title_width(graph);
	xw = get_graph_xaxbar_width(graph);
	yh = get_graph_yaxbar_height(graph);

	yt = yh * get_yax_count(graph);

	stepnum = get_graph_yaxbar_step(graph);
	middnum = get_graph_yaxbar_midd(graph);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, style);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, style);

	default_xface(&xa);
	parse_xface_from_style(&xa, style);

	//draw frame

	if (ph < th)
		return;

	xp1.x = 0;
	xp1.y = th;
	xp2.x = pw;
	xp2.y = th;
	nlk = insert_svg_node(glk);
	write_line_to_svg_node(nlk, &xp, &xp1, &xp2);

	if (ph <= th + yt)
		return;

	xp1.x = 0;
	xp1.y = th + yt;
	xp2.x = pw;
	xp2.y = th + yt;
	nlk = insert_svg_node(glk);
	write_line_to_svg_node(nlk, &xp, &xp1, &xp2);

	xp1.x = tw;
	xp1.y = th;
	xp2.x = tw;
	xp2.y = ph - th;
	nlk = insert_svg_node(glk);
	write_line_to_svg_node(nlk, &xp, &xp1, &xp2);

	xp1.x = 0;
	xp1.y = ph - th;
	xp2.x = pw;
	xp2.y = ph - th;
	nlk = insert_svg_node(glk);
	write_line_to_svg_node(nlk, &xp, &xp1, &xp2);

	midy = (ph - th * 2 - yt) / 2 + (th + yt);
	incy = (ph - th * 2 - yt) / 10;

	//draw ruler
	xr.x = 0;
	xr.y = midy - incy / 2;
	xr.w = tw - MINFEED;
	xr.h = incy;

	format_numeric(middnum, token, 0, 0);
	xscpy(xa.text_align, ATTR_ALIGNMENT_FAR);

	nlk = insert_svg_node(glk);
	write_text_to_svg_node(nlk, &xf, &xa, &xr, token, -1);

	xp1.x = tw - MINFEED;
	xp1.y = midy;
	xp2.x = tw;
	xp2.y = midy;

	nlk = insert_svg_node(glk);
	write_line_to_svg_node(nlk, &xp, &xp1, &xp2);

	for (i = 1; i < 5; i++)
	{
		xr.x = 0;
		xr.y = midy - incy * i - incy / 2;
		xr.w = tw - MINFEED;
		xr.h = incy;

		format_numeric(middnum + i * 10 * stepnum, token, 0, 0);

		nlk = insert_svg_node(glk);
		write_text_to_svg_node(nlk, &xf, &xa, &xr, token, -1);

		xp1.x = tw - MINFEED;
		xp1.y = midy - incy * i;
		xp2.x = tw;
		xp2.y = midy - incy * i;

		nlk = insert_svg_node(glk);
		write_line_to_svg_node(nlk, &xp, &xp1, &xp2);

		xr.x = 0;
		xr.y = midy + incy * i - incy / 2;
		xr.w = tw - MINFEED;
		xr.h = incy;

		format_numeric(middnum - i * 10 * stepnum, token, 0, 0);
		xscpy(xa.text_align, ATTR_ALIGNMENT_FAR);

		nlk = insert_svg_node(glk);
		write_text_to_svg_node(nlk, &xf, &xa, &xr, token, -1);

		xp1.x = tw - MINFEED;
		xp1.y = midy + incy * i;
		xp2.x = tw;
		xp2.y = midy + incy * i;

		nlk = insert_svg_node(glk);
		write_line_to_svg_node(nlk, &xp, &xp1, &xp2);
	}

	//draw yax bar
	y1 = y2 = th;
	ylk = get_next_yax(graph, LINK_FIRST);
	while (ylk)
	{
		color = get_yax_color_ptr(ylk);
		parse_xcolor(&xc, color);

		default_xpen(&xp_shape);
		format_xcolor(&xc, xp_shape.color);

		default_xbrush(&xb_shape);
		format_xcolor(&xc, xb_shape.color);

		y1 = y2;
		y2 += yh;

		xr.x = MINFEED;
		xr.w = 2 * MINFEED;
		xr.y = (y1 + y2) / 2 - MINFEED;
		xr.h = 2 * MINFEED;

		nlk = insert_svg_node(glk);
		write_rect_to_svg_node(nlk, &xp_shape, &xb_shape, &xr);

		xr.x = 3 * MINFEED;
		xr.y = y1;
		xr.w = tw - 3 * MINFEED;
		xr.h = y2 - y1;

		nlk = insert_svg_node(glk);
		write_text_to_svg_node(nlk, &xf, &xa, &xr, get_yax_title_ptr(ylk), -1);

		ylk = get_next_yax(graph, ylk);

		if (y2 > ph)
			break;
	}

	//draw xax bar
	calc_graph_xax_scope(&xview, graph, page, &xlkFirst, &xlkLast);

	x1 = x2 = tw;
	xlk = xlkFirst;
	while (xlk)
	{
		x1 = x2;
		x2 += xw;
		y1 = y2 = th;

		steps = get_yax_count(graph);
		if (steps == 0)
			goto skip;

		tmpx = (xw - 2) / steps;
		steps = 0;
		nxtd = pred = 0;

		total = 0;
		ylk = get_next_yax(graph, LINK_FIRST);
		while (ylk)
		{
			total += get_spot_numeric(xlk, ylk);
			ylk = get_next_yax(graph, ylk);
		}

		ylk = get_next_yax(graph, LINK_FIRST);
		while (ylk)
		{
			style = get_yax_style_ptr(ylk);

			default_xfont(&xf);
			parse_xfont_from_style(&xf, style);

			default_xface(&xa);
			parse_xface_from_style(&xa, style);

			color = get_yax_color_ptr(ylk);
			parse_xcolor(&xc, color);

			default_xbrush(&xb_shape);
			format_xcolor(&xc, xb_shape.color);

			default_xpen(&xp_shape);
			format_xcolor(&xc, xp_shape.color);

			dalen = get_yax_data_len(ylk);
			daprec = get_yax_data_prec(ylk);

			y1 = y2;
			y2 = y1 + yh;

			xr.x = x1 + MINFEED;
			xr.y = (y1 + y2) / 2 - MINFEED;
			xr.w = 2 * MINFEED;
			xr.h = 2 * MINFEED;


			xr.x = x1 + 3 * MINFEED;
			xr.y = y1;
			xr.w = x2 - x1 - 3 * MINFEED;
			xr.h = y2 - y1;
			format_numeric(get_spot_numeric(xlk, ylk), token, dalen, daprec);

			nlk = insert_svg_node(glk);
			write_text_to_svg_node(nlk, &xf, &xa, &xr, token, -1);

			cury = midy - (long)((get_spot_numeric(xlk, ylk) - middnum) / stepnum);

			if (compare_text(type, -1, ATTR_GRAPH_TYPE_LINE, -1, 0) == 0)
			{
				prey = cury;

				xr.x = (x1 + x2) / 2 - 1;
				xr.y = cury - MINFEED;
				xr.w = 2 * MINFEED;
				xr.h = 2 * MINFEED;

				nlk = insert_svg_node(glk);
				write_ellipse_to_svg_node(nlk, &xp_shape, NULL, &xr);

				if (xlk != xlkFirst)
				{
					xp1.x = (x1 + x2) / 2 - xw;
					xp1.y = prey;
					xp2.x = (x1 + x2) / 2;
					xp2.y = cury;

					nlk = insert_svg_node(glk);
					write_line_to_svg_node(nlk, &xp_shape, &xp1, &xp2);
				}
			}
			else if (compare_text(type, -1, ATTR_GRAPH_TYPE_RECT, -1, 0) == 0)
			{
				steps++;

				xr.y = cury;
				xr.h = ph - cury - yh;
				xr.x = x1 + steps * tmpx + MINFEED;
				xr.w = tmpx;

				nlk = insert_svg_node(glk);
				write_rect_to_svg_node(nlk, &xp_shape, &xb_shape, &xr);
			}
			else if (compare_text(type, -1, ATTR_GRAPH_TYPE_PIE, -1, 0) == 0)
			{
				pred = nxtd;

				if ((int)total == 0)
					dby = 0;
				else
					dby = (float)(get_spot_numeric(xlk, ylk) / total);

				nxtd = pred + (float)(2 * XPI *  dby);
				if (pred != nxtd)
				{
					xr.x = x1 + MINFEED;
					xr.w = xw - 2 * MINFEED;
					xr.y = midy - xw / 2 + MINFEED;
					xr.h = xw - 2 * MINFEED;

					//draw_pie(canv, &xp_shape, &xr, pred, nxtd);
					//fill_pie(canv, &xb_shape, &xr, pred, nxtd);

					xsprintf(token, L"%.2f%c", dby * 100, L'%');

					xr.x = (x1 + x2) / 2 + (long)((float)xw / 4 * cos((nxtd + pred) / 2)) - xw / 4;
					xr.w = xw / 2;
					xr.y = midy - (long)((float)xw / 4 * sin((nxtd + pred) / 2)) - 5;
					xr.h = 10 * MINFEED;

					nlk = insert_svg_node(glk);
					write_text_to_svg_node(nlk, &xf, &xa, &xr, token, -1);
				}
			}
			ylk = get_next_yax(graph, ylk);
		}

		if (compare_text(type, -1, ATTR_GRAPH_TYPE_PIE, -1, 0) == 0)
		{
			y1 = y2;
			y2 = y1 + yh;

			xr.x = x1 + MINFEED;
			xr.y = y1;
			xr.w = 3 * MINFEED;
			xr.h = y2 - y1;

			nlk = insert_svg_node(glk);
			write_text_to_svg_node(nlk, &xf, &xa, &xr, L"∑", -1);

			xr.x = x1 + 3 * MINFEED;
			xr.y = y1;
			xr.w = x2 - x1 - 3 * MINFEED;
			xr.h = y2 - y1;
			format_numeric(total, token, 0, 0);

			nlk = insert_svg_node(glk);
			write_text_to_svg_node(nlk, &xf, &xa, &xr, token, -1);
		}

	skip:
		xr.x = x1;
		xr.y = ph - th;
		xr.w = xw;
		xr.h = th;

		nlk = insert_svg_node(glk);
		write_text_to_svg_node(nlk, &xf, &xa, &xr, get_xax_title_ptr(xlk), -1);

		if (xlk == xlkLast)
			break;
		xlk = get_next_xax(graph, xlk);
	}
}

void save_form_to_svg_node(link_t_ptr form, int page, link_t_ptr glk)
{
	link_t_ptr flk;
	xrect_t xr;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	ximage_t xi = { 0 };

	bool_t b_design;
	const wchar_t *sz_class, *sz_shape, *style;

	xrect_t xview = { 0 };
	link_t_ptr nlk;

	set_svg_node_name(glk, DOC_SVG_G);

	xview.x = 0;
	xview.y = 0;
	xview.w = get_dom_node_attr_long(glk, SVG_ATTR_WIDTH);
	xview.h = get_dom_node_attr_long(glk, SVG_ATTR_HEIGHT);

	b_design = get_form_design_mode(form);

	if (b_design)
		flk = get_next_field(form, LINK_FIRST);
	else
		flk = get_next_visible_field(form, LINK_FIRST);

	while (flk)
	{
		style = get_field_style_ptr(flk);

		calc_form_field_rect(&xview, form, flk, &xr);

		sz_shape = get_field_shape_ptr(flk);
		if (!is_null(sz_shape))
		{
			default_xpen(&xp);
			parse_xpen_from_style(&xp, style);
			
			nlk = insert_svg_node(glk);
			write_shape_to_svg_node(nlk, &xp, NULL, &xr, sz_shape);
		}

		sz_class = get_field_class_ptr(flk);
		if (compare_text(sz_class, -1, DOC_FORM_LABEL, -1, 0) == 0 || compare_text(sz_class, -1, DOC_FORM_MEMO, -1, 0) == 0)
		{
			default_xfont(&xf);
			parse_xfont_from_style(&xf, style);
			
			default_xface(&xa);
			parse_xface_from_style(&xa, style);
			
			nlk = insert_svg_node(glk);
			write_text_to_svg_node(nlk, &xf, &xa, &xr, get_field_text_ptr(flk), -1);
		}
		else if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 0) == 0)
		{
			default_xfont(&xf);
			parse_xfont_from_style(&xf, style);
			
			default_xface(&xa);
			parse_xface_from_style(&xa, style);

			nlk = insert_svg_node(glk);
			write_text_to_svg_node(nlk, &xf, &xa, &xr, get_field_options_text_ptr(flk), -1);
		}
		else if (compare_text(sz_class, -1, DOC_FORM_GRID, -1, 0) == 0)
		{
			nlk = insert_svg_node(glk);
			set_svg_node_name(nlk, DOC_SVG_G);
			xr.x = -xr.x;
			xr.y = -xr.y;
			write_viewbox_to_svg_node(nlk, &xr);

			save_grid_to_svg_node(get_field_object(flk), page, nlk);
		}
		else if (compare_text(sz_class, -1, DOC_FORM_GRAPH, -1, 0) == 0)
		{
			nlk = insert_svg_node(glk);
			set_svg_node_name(nlk, DOC_SVG_G);
			xr.x = -xr.x;
			xr.y = -xr.y;
			write_viewbox_to_svg_node(nlk, &xr);

			save_graph_to_svg_node(get_field_object(flk), page, nlk);
		}

		if (b_design)
			flk = get_next_field(form, flk);
		else
			flk = get_next_visible_field(form, flk);
	}
}