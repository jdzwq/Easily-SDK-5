/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc plot view document

	@module	plotview.c | implement file

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

#include "plotview.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdlgdi.h"
#include "xdldoc.h"

#if defined(XDL_SUPPORT_VIEW)

typedef struct _plot_t{
	tchar_t* style;
	int ruler;

	double** y_grades;
	double** y_bases;
	double** y_steps;

	tchar_t** y_stages;
	tchar_t** y_labels;
	tchar_t** y_colors;
	tchar_t** y_shapes;

	tchar_t** x_labels;
	tchar_t** x_colors;
}plot_t;

static void _plot_calendar(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa;

	xrect_t xr, xr_dot;
	xpoint_t pt;
	xsize_t xs;
	xpoint_t pa[12] = { 0 };
	tchar_t sa[10] = { 0 };

	float dx, dy, dd;
	float px;

	int i, j;
	int y_count, x_count;

	const tchar_t* label;
	const tchar_t* shape;
	const tchar_t* color;

	xdate_t dt;
	tchar_t year[NUM_LEN + 1] = { 0 };
	tchar_t month[NUM_LEN + 1] = { 0 };
	tchar_t day[NUM_LEN + 1] = { 0 };
	tchar_t week[NUM_LEN + 1] = { 0 };
	tchar_t solar[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));

	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));

	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 1;

	dx = pbox->fw / 10;
	dy = pbox->fh / 8;
	dd = (dx > dy) ? dx : dy;

	xr.fx = pbox->fx;
	xr.fy = pbox->fy;
	xr.fw = pbox->fw;
	xr.fh = pbox->fh;

	sa[0] = _T('M');
	pa[0].fx = xr.fx;
	pa[0].fy = xr.fy + dy;
	
	sa[1] = _T('A');
	pa[1].fx = 1;
	pa[1].fy = 0;
	pa[2].fx = dy + dx;
	pa[2].fy = dy + dx;
	pa[3].fx = xr.fx + dx;
	pa[3].fy = xr.fy;

	sa[2] = _T('L');
	pa[4].fx = xr.fx + xr.fw - dx;
	pa[4].fy = xr.fy;

	sa[3] = _T('A');
	pa[5].fx = 1;
	pa[5].fy = 0;
	pa[6].fx = dy + dx;
	pa[6].fy = dy + dx;
	pa[7].fx = xr.fx + xr.fw;
	pa[7].fy = xr.fy + dy;

	sa[4] = _T('L');
	pa[8].fx = xr.fx;
	pa[8].fy = xr.fy + dy;

	sa[5] = _T('Z');

	(*pif->pf_draw_path)(pif->ctx, &xp, &xb, sa, pa, 9);

	lighten_xbrush(&xb, DEF_HARD_LIGHTEN);

	sa[0] = _T('M');
	pa[0].fx = xr.fx;
	pa[0].fy = xr.fy + dy;

	sa[1] = _T('L');
	pa[1].fx = xr.fx;
	pa[1].fy = xr.fy + xr.fh - dy / 2;

	sa[2] = _T('A');
	pa[2].fx = 0;
	pa[2].fy = 0;
	pa[3].fx = dd / 2;
	pa[3].fy = dd / 2;
	pa[4].fx = xr.fx + dx / 2;
	pa[4].fy = xr.fy + xr.fh;

	sa[3] = _T('L');
	pa[5].fx = xr.fx + xr.fw - dx / 2;
	pa[5].fy = xr.fy + xr.fh;

	sa[4] = _T('A');
	pa[6].fx = 0;
	pa[6].fy = 0;
	pa[7].fx = dd / 2;
	pa[7].fy = dd / 2;
	pa[8].fx = xr.fx + xr.fw;
	pa[8].fy = xr.fy + xr.fh - dy / 2;

	sa[5] = _T('L');
	pa[9].fx = xr.fx + xr.fw;
	pa[9].fy = xr.fy + dy;

	sa[6] = _T('Z');

	(*pif->pf_draw_path)(pif->ctx, &xp, &xb, sa, pa, 10);

	color = get_string_ptr(plt->y_colors, 0);
	if (!is_null(color))
	{
		xsncpy(xp_dot.color, color, CLR_LEN);
		xsncpy(xb_dot.color, color, CLR_LEN);
		lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
	}
	else
	{
		xsncpy(xp_dot.color, xp.color, CLR_LEN);
		xsncpy(xb_dot.color, xb.color, CLR_LEN);
		lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
	}

	sa[0] = _T('M');
	pa[0].fx = xr.fx + dx / 2;
	pa[0].fy = xr.fy + dy + dx / 2;

	sa[1] = _T('L');
	pa[1].fx = xr.fx + xr.fw - dx / 2;
	pa[1].fy = xr.fy + dy + dx / 2;

	sa[2] = _T('L');
	pa[2].fx = xr.fx + xr.fw - dx / 2;
	pa[2].fy = xr.fy + xr.fh - dx;

	sa[3] = _T('A');
	pa[3].fx = 1;
	pa[3].fy = 0;
	pa[4].fx = dd / 2;
	pa[4].fy = dd / 2;
	pa[5].fx = xr.fx + xr.fw - dx;
	pa[5].fy = xr.fy + xr.fh - dx / 2;

	sa[4] = _T('L');
	pa[6].fx = xr.fx + dx;
	pa[6].fy = xr.fy + xr.fh - dx / 2;

	sa[5] = _T('A');
	pa[7].fx = 1;
	pa[7].fy = 0;
	pa[8].fx = dd / 2;
	pa[8].fy = dd / 2;
	pa[9].fx = xr.fx + dx / 2;
	pa[9].fy = xr.fy + xr.fh - dx;

	sa[6] = _T('L');
	pa[10].fx = xr.fx + dx / 2;
	pa[10].fy = xr.fy + dy + dx / 2;

	sa[7] = _T('Z');

	(*pif->pf_draw_path)(pif->ctx, &xp_dot, &xb_dot, sa, pa, 11);

	label = get_string_ptr(plt->x_labels, 0);
	if (is_null(label))
		get_loc_date(&dt);
	else
		parse_date(&dt, label);

	cn_date_token(&dt, year, month, day, week, solar);

	font_metric_by_pt(dy, NULL, &px);
	ftoxs(px, xf_dot.size, NUM_LEN);
	xscpy(xf_dot.color, xb_dot.color);

	xr_dot.fx = xr.fx + dx;
	xr_dot.fy = xr.fy;
	xr_dot.fw = xr.fw - 2 * dx;
	xr_dot.fh = dy;
	(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr_dot, year, -1);

	font_metric_by_pt(xr.fh / 4, NULL, &px);
	ftoxs(px, xf.size, NUM_LEN);

	(*pif->pf_text_size)(pif->ctx, &xf, month, -1, &xs);
	pt.fx = xr.fx + (xr.fw - xs.fw) / 2;
	pt.fy = xr.fy + xr.fh / 2 - xs.fh;
	(*pif->pf_text_out)(pif->ctx, &xf, &pt, month, -1);

	pt.fy = xr.fy + xr.fh / 2;
	(*pif->pf_text_out)(pif->ctx, &xf, &pt, day, -1);
	(*pif->pf_text_size)(pif->ctx, &xf, day, -1, &xs);

	font_metric_by_pt(xr.fh / 6, NULL, &px);
	if (px < 6.0f)
		px = 6.0f;
	ftoxs(px, xf.size, NUM_LEN);

	pt.fx += xs.fw;
	pt.fy += xs.fh;
	(*pif->pf_text_size)(pif->ctx, &xf, week, -1, &xs);
	pt.fy -= xs.fh;
	(*pif->pf_text_out)(pif->ctx, &xf, &pt, week, -1);

	font_metric_by_pt(xr.fh / 4, NULL, &px);
	ftoxs(px, xf.size, NUM_LEN);

	i = xslen(solar) / 2;
	(*pif->pf_text_size)(pif->ctx, &xf, solar, i, &xs);
	pt.fx = xr.fx + dx;
	pt.fy = xr.fy + dy + dx;
	(*pif->pf_text_out)(pif->ctx, &xf, &pt, solar, i);

	font_metric_by_pt(xr.fh / 6, NULL, &px);
	if (px < 6.0f)
		px = 6.0f;
	ftoxs(px, xf.size, NUM_LEN);

	pt.fx += xs.fw;
	pt.fy += xs.fh;
	(*pif->pf_text_size)(pif->ctx, &xf, (solar + i), -1, &xs);
	pt.fy -= xs.fh;
	(*pif->pf_text_out)(pif->ctx, &xf, &pt, (solar + i), -1);

	font_metric_by_pt(xr.fh / 4, NULL, &px);
	ftoxs(px, xf.size, NUM_LEN);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	label = get_string_ptr(plt->y_labels, 0);
	(*pif->pf_text_size)(pif->ctx, &xf, label, -1, &xs);
	xr_dot.fx = xr.fx + dx;
	xr_dot.fy = xr.fy + xr.fh - dx - xs.fh;
	xr_dot.fw = xr.fw - 2 * dx;
	xr_dot.fh = xs.fh;
	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr_dot, label, -1);

	xr_dot.fx = xr.fx + xr.fw / 4 - dy / 4;
	xr_dot.fy = xr.fy + dy / 2 - dy / 4;
	xr_dot.fw = dy / 2;
	xr_dot.fh = dy / 2;
	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb_dot, &xr_dot);

	xr_dot.fx = xr.fx + xr.fw * 3 / 4 - dy / 4;
	xr_dot.fy = xr.fy + dy / 2 - dy / 4;
	xr_dot.fw = dy / 2;
	xr_dot.fh = dy / 2;
	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb_dot, &xr_dot);
}

static void _plot_indicator(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf;
	xface_t xa;

	xrect_t xr;

	float dx, dy;
	float middy, middx;

	int i, j;
	int y_count, x_count;

	const tchar_t* label;
	const tchar_t* shape;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));

	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	
	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 1;

	//horz bar width
	dx = pbox->fw / y_count;

	//vert height
	dy = (pbox->fh / x_count) / 2;

	//bar
	for (i = 0; i < x_count; i++)
	{
		for (j = 0; j < y_count; j++)
		{
			middy = pbox->fy + i * 2 * dy + dy;
			middx = pbox->fx + j * dx + dx / 2;

			color = get_string_ptr(plt->y_colors, j);
			if (!is_null(color))
			{
				xsncpy(xp_dot.color, color, CLR_LEN);
				xsncpy(xb_dot.color, color, CLR_LEN);
				lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
			}
			else
			{
				xsncpy(xp_dot.color, xp.color, CLR_LEN);
				xsncpy(xb_dot.color, xb.color, CLR_LEN);
				lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
			}

			label = get_string_ptr(plt->y_labels, j);

			xr.fx = middx - dx / 2;
			xr.fw = dx;
			xr.fy = middy - dy;
			xr.fh = dy;
			(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, label, -1);

			shape = get_string_ptr(plt->y_shapes, j);
			if (is_null(shape))
				shape = ATTR_SHAPE_RECT;

			xr.fx = middx - dx / 2 + 1.0f;
			xr.fw = dx - 2.0f;
			xr.fy = middy;
			xr.fh = dy - 1.0f;
			draw_shape(pif, &xp_dot, &xb_dot, &xr, shape);
		}
	}
}

static void _plot_thermometer(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt1, pt2;
	xrect_t xr;
	xpoint_t pa[15] = { 0 };
	tchar_t sa[10] = { 0 };

	int i, j;
	float dd, dx, dy, dr;
	float zeroy, zerox, middx, middy;

	int y_count, y_ruler, x_count;

	const tchar_t* label;
	const tchar_t* color;

	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double y_base, y_step;
	double dbl;

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	parse_xface_from_style(&xa, plt->style);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xb.color);

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 1;

	//ruler count
	y_ruler = (plt->ruler) ? plt->ruler : 5;

	//horz label width
	dx = pbox->fw / x_count;
	dy = pbox->fh;
	if (dy < dx)
		dd = dy / 3;
	else
		dd = dx / 3;

	dr = (dy - 3 * dd) / y_ruler;
	if (dr < 1.0f)
	{
		dr = 1.0f;
		y_ruler = (int)((dy - 3 * dd) / 1.0f);
	}

	//vert ruler and labels
	for (i = 0; i < x_count; i++)
	{
		zeroy = pbox->fy + pbox->fh - 2 * dd;
		zerox = pbox->fx + i * dx + (dx - 3 * dd) / 2;

		//outer
		//y labels
		label = get_string_ptr(plt->x_labels, i);

		xr.fx = zerox;
		xr.fw = 3 * dd;
		xr.fy = zeroy + 2 * dd;
		xr.fh = dd / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, label, -1);

		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		dbl = get_numeric(plt->y_bases, i);

		for (j = 1; j <= y_ruler; j++)
		{
			pt1.fx = zerox + dd - 2.5f;
			pt1.fy = zeroy - j * dr;
			pt2.fx = zerox + dd - 0.5f;
			pt2.fy = zeroy - j * dr;
			(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

			if (IS_VALID_DOUBLE(dbl))
				dbl += get_numeric(plt->y_steps, i);

			xr.fx = zerox + 2 * dd + 0.5f;
			xr.fw = dd;
			xr.fy = zeroy - j * dr - dr / 2;
			xr.fh = dr;
			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.2f"), dbl);
			else
				xscpy(numstr, _T("N"));
			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);
		}

		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_SOLID);

		//outside
		sa[0] = _T('M');
		pa[0].fx = zerox + dd;
		pa[0].fy = zeroy - y_ruler * dr;

		sa[1] = _T('A');
		pa[1].fx = 1;
		pa[1].fy = 0;
		pa[2].fx = dd / 2;
		pa[2].fy = dd / 2;
		pa[3].fx = zerox + 2 * dd;
		pa[3].fy = zeroy - y_ruler * dr;

		sa[2] = _T('L');
		pa[4].fx = zerox + 2 * dd;
		pa[4].fy = zeroy;

		sa[3] = _T('A');
		pa[5].fx = 1;
		pa[5].fy = 1;
		pa[6].fx = (float)(1.5f * dd);
		pa[6].fy = (float)(1.5f * dd);
		pa[7].fx = zerox + dd;
		pa[7].fy = zeroy;

		sa[4] = _T('L');
		pa[8].fx = zerox + dd;
		pa[8].fy = zeroy - y_ruler * dr;

		sa[5] = _T('Z');

		(*pif->pf_draw_path)(pif->ctx, &xp, &xb, sa, pa, 9);

		color = get_string_ptr(plt->x_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
		}

		middx = zerox + 3 * dd / 2;

		y_step = get_numeric(plt->y_steps, i);
		y_base = get_numeric(plt->y_bases, i);
		dbl = matrix_get_value(pmt, 0, i);

		if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			middy = zeroy - (float)((dbl - y_base) / (y_step)* dr);
		else
			middy = zeroy;

		//inside
		sa[0] = _T('M');
		pa[0].fx = middx - dd / 4;
		pa[0].fy = middy;

		sa[1] = _T('A');
		pa[1].fx = 1;
		pa[1].fy = 0;
		pa[2].fx = dd / 4;
		pa[2].fy = dd / 4;
		pa[3].fx = middx + dd / 4;
		pa[3].fy = middy;

		sa[2] = _T('L');
		pa[4].fx = middx + dd / 4;
		pa[4].fy = zeroy + dd / 2;

		sa[3] = _T('A');
		pa[5].fx = 1;
		pa[5].fy = 1;
		pa[6].fx = dd;
		pa[6].fy = dd;
		pa[7].fx = middx - dd / 4;
		pa[7].fy = zeroy + dd / 2;

		sa[4] = _T('L');
		pa[8].fx = middx - dd / 4;
		pa[8].fy = middy;

		sa[5] = _T('Z');

		(*pif->pf_draw_path)(pif->ctx, &xp_dot, &xb_dot, sa, pa, 9);
	}
}

static void _plot_bargram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pk;
	xrect_t xr, xr_pi;
	xsize_t xs;

	int i;
	float dd, dx, dy, dr;
	float middy, middx;

	int y_count, x_count;

	const tchar_t* label;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl, dbr;
	double y_base, y_step;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 1;

	//ruler height
	dy = pbox->fh / x_count;
	
	//bar height
	dd = dy / 2;
	dr = dd / 2;

	//horz bar width
	dx = pbox->fw - dr;

	//bar
	for (i = 0; i < x_count; i++)
	{
		middy = pbox->fy + i * dy + dy / 2;
		middx = pbox->fx + pbox->fw / 2;

		label = get_string_ptr(plt->x_labels, i);

		xr.fx = middx - dx / 2 + dd / 2;
		xr.fy = middy - dd / 2;
		xr.fw = dx - dd;
		xr.fh = dd;

		(*pif->pf_text_size)(pif->ctx, &xf, label, -1, &xs);
		pk.fx = xr.fx;
		pk.fy = xr.fy - xs.fh;
		(*pif->pf_text_out)(pif->ctx, &xf, &pk, label, -1);

		xr_pi.fx = xr.fx - dd / 2;
		xr_pi.fy = xr.fy;
		xr_pi.fw = dd;
		xr_pi.fh = xr.fh;
		(*pif->pf_draw_pie)(pif->ctx, &xp, &xb, &xr_pi, XPI / 2, 3 * XPI / 2);

		(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

		xr_pi.fx = xr.fx + xr.fw - dd / 2;
		xr_pi.fy = xr.fy;
		xr_pi.fw = dd;
		xr_pi.fh = xr.fh;
		(*pif->pf_draw_pie)(pif->ctx, &xp, &xb, &xr_pi, 3 * XPI / 2, 5 * XPI / 2);

		color = get_string_ptr(plt->x_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		y_base = get_numeric(plt->y_bases, i);
		y_step = get_numeric(plt->y_steps, i);
		dbl = matrix_get_value(pmt, 0, i);

		if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			dbr = ((dbl - y_base) / y_step);
		else
			dbr = 0;

		xr.fx = middx - dx / 2 + dd / 2 + dr / 2;
		xr.fy = middy - dr / 2;
		xr.fw = (float)((dx - dd - dr) * dbr);
		xr.fh = dr;

		xr_pi.fx = xr.fx - dr / 2;
		xr_pi.fy = xr.fy;
		xr_pi.fw = dr;
		xr_pi.fh = xr.fh;
		(*pif->pf_draw_pie)(pif->ctx, &xp_dot, &xb_dot, &xr_pi, XPI / 2, 3 * XPI / 2);

		(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);

		xr_pi.fx = xr.fx + xr.fw - dr / 2;
		xr_pi.fy = xr.fy;
		xr_pi.fw = dr;
		xr_pi.fh = xr.fh;
		(*pif->pf_draw_pie)(pif->ctx, &xp_dot, &xb_dot, &xr_pi, 3 * XPI / 2, 5 * XPI / 2);

		xsprintf(numstr, _T("%.2f%"), dbr * 100);
		(*pif->pf_text_size)(pif->ctx, &xf_dot, numstr, -1, &xs);
		pk.fx = xr.fx + xr.fw + dr / 2 + 0.5f;
		pk.fy = xr.fy + (xr.fh - xs.fh) / 2;
		(*pif->pf_text_out)(pif->ctx, &xf_dot, &pk, numstr, -1);
	}
}

static void _plot_contragram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xrect_t xr;
	xpoint_t pa[20] = { 0 };
	tchar_t sa[10] = { 0 };

	float dd, dr, dx, dy, bw;
	float middy, middx;

	int i;
	int y_count;
	int x_count;

	const tchar_t* label;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double db1,db2, dbr;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) + 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);

	y_count = 2;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 1;

	dy = (pbox->fh / x_count) / 2;
	dx = pbox->fw / y_count;
	dd = dy / 2;
	dr = dd / 2;
	
	for (i = 0; i < x_count; i++)
	{
		middy = pbox->fy + i * 2 * dy + dy;
		middx = pbox->fx + pbox->fw / 2;

		sa[0] = _T('M');
		pa[0].fx = middx - dx + dd;
		pa[0].fy = middy + dd;

		sa[1] = _T('A');
		pa[1].fx = 1;
		pa[1].fy = 0;
		pa[2].fx = dd;
		pa[2].fy = dd;
		pa[3].fx = middx - dx + dd;
		pa[3].fy = middy - dd;

		sa[2] = _T('L');
		pa[4].fx = middx - dd;
		pa[4].fy = middy - dd;

		sa[3] = _T('A');
		pa[5].fx = 1;
		pa[5].fy = 0;
		pa[6].fx = dd + dr;
		pa[6].fy = dd + dr;
		pa[7].fx = middx + dd;
		pa[7].fy = middy - dd;

		sa[4] = _T('L');
		pa[8].fx = middx + dx - dd;
		pa[8].fy = middy - dd;

		sa[5] = _T('A');
		pa[9].fx = 1;
		pa[9].fy = 0;
		pa[10].fx = dd;
		pa[10].fy = dd;
		pa[11].fx = middx + dx - dd;
		pa[11].fy = middy + dd;

		sa[6] = _T('L');
		pa[12].fx = middx - dx + dd;
		pa[12].fy = middy + dd;

		sa[9] = _T('Z');

		(*pif->pf_draw_path)(pif->ctx, &xp, &xb, sa, pa, 13);

		xr.fx = middx - dd;
		xr.fy = middy - dd;
		xr.fw = dd * 2;
		xr.fh = dd * 2;

		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, _T("VS"), -1);

		label = get_string_ptr(plt->y_labels, 0);

		xr.fx = middx - dx;
		xr.fy = middy - 2 * dd;
		xr.fw = dx - dd;
		xr.fh = dd;

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);
		(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, label, -1);

		label = get_string_ptr(plt->y_labels, 1);

		xr.fx = middx + dd;
		xr.fy = middy - 2 * dd;
		xr.fw = dx - dd;
		xr.fh = dd;

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, label, -1);

		color = get_string_ptr(plt->y_colors, 0);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		db1 = matrix_get_value(pmt, i, 0);
		db2 = matrix_get_value(pmt, i, 1);

		if (IS_VALID_DOUBLE(db1) && IS_VALID_DOUBLE(db2) && (!IS_ZERO_DOUBLE(db1) || !IS_ZERO_DOUBLE(db2)))
			dbr = ((db1) / (db1 + db2));
		else
			dbr = 0;

		bw = (float)(xr.fw * dbr);

		sa[0] = _T('M');
		pa[0].fx = middx - dx + dd;
		pa[0].fy = middy + dr;

		sa[1] = _T('A');
		pa[1].fx = 1;
		pa[1].fy = 0;
		pa[2].fx = dr;
		pa[2].fy = dr;
		pa[3].fx = middx - dx + dd;
		pa[3].fy = middy - dr;

		sa[2] = _T('L');
		pa[4].fx = middx - dx + dd + bw;
		pa[4].fy = middy - dr;

		sa[3] = _T('L');
		pa[5].fx = middx - dx + dd + bw;
		pa[5].fy = middy + dr;

		sa[4] = _T('L');
		pa[6].fx = middx - dx + dd;
		pa[6].fy = middy + dr;

		sa[5] = _T('Z');

		(*pif->pf_draw_path)(pif->ctx, &xp_dot, &xb_dot, sa, pa, 7);

		color = get_string_ptr(plt->y_colors, 1);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		if (IS_VALID_DOUBLE(db1) && IS_VALID_DOUBLE(db2) && (!IS_ZERO_DOUBLE(db1) || !IS_ZERO_DOUBLE(db2)))
			dbr = ((db2) / (db1 + db2));
		else
			dbr = 0;

		bw = (float)(xr.fw * dbr);

		sa[0] = _T('M');
		pa[0].fx = middx + dx - dd;
		pa[0].fy = middy - dr;

		sa[1] = _T('A');
		pa[1].fx = 1;
		pa[1].fy = 0;
		pa[2].fx = dr;
		pa[2].fy = dr;
		pa[3].fx = middx + dx - dd;
		pa[3].fy = middy + dr;

		sa[2] = _T('L');
		pa[4].fx = middx + dx - dd - bw;
		pa[4].fy = middy + dr;

		sa[3] = _T('L');
		pa[5].fx = middx + dx - dd - bw;
		pa[5].fy = middy - dr;

		sa[4] = _T('L');
		pa[6].fx = middx + dx - dd;
		pa[6].fy = middy - dr;

		sa[5] = _T('Z');

		(*pif->pf_draw_path)(pif->ctx, &xp_dot, &xb_dot, sa, pa, 7);

		label = get_string_ptr(plt->x_labels, i);

		xr.fx = middx - dx;
		xr.fy = middy + dd;
		xr.fw = 2 * dx;
		xr.fh = dr;

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, label, -1);
	}
}

static void _plot_balancegram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk, t1, t2, t3, k1, k2, k3;
	xrect_t xr;
	xsize_t xs;
	xpoint_t pa[10] = { 0 };
	tchar_t sa[10] = { 0 };

	float dd, dx, dy, dr;
	float middy, middx;
	double fang, tang;

	int i;
	int y_count;
	int x_count;

	const tchar_t* label;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double db1, db2, dbr;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);
	xscpy(xb_dot.style, GDI_ATTR_FILL_STYLE_GRADIENT);
	xscpy(xb_dot.gradient, GDI_ATTR_GRADIENT_RADIAL);

	y_count = 2;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 1;

	dx = (pbox->fw) / 2;

	dy = (pbox->fh / x_count) / 2;
	dd = dy / 2;
	dr = dd / 2;

	dx -= 2 * dr;

	for (i = 0; i < x_count; i++)
	{
		middy = pbox->fy + i * 2 * dy + dy / 2;
		middx = pbox->fx + pbox->fw / 2;

		xr.fx = middx - 2.0f;
		xr.fy = middy;
		xr.fw = 4.0f;
		xr.fh = 4.0f;
		(*pif->pf_draw_triangle)(pif->ctx, &xp, &xb, &xr, GDI_ATTR_ORIENT_TOP);

		label = get_string_ptr(plt->x_labels, i);

		xr.fx = middx - dx;
		xr.fy = middy + 4.0f;
		xr.fw = 2 * dx;
		xr.fh = dr;

		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, label, -1);

		db1 = matrix_get_value(pmt, i, 0);
		db2 = matrix_get_value(pmt, i, 1);

		if (IS_ZERO_DOUBLE(db1) && IS_ZERO_DOUBLE(db2))
			dbr = 0;
		else
			dbr = (db1 - db2) / (db1 + db2);

		fang = XPI - (dbr * XPI / 4);
		tang = 2 * XPI - (dbr * XPI / 4);

		pt.fx = middx;
		pt.fy = middy;

		xs.fw = dx;
		xs.fh = dx;
		(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, tang, pa, 4);

		pt.fx = pa[0].fx;
		pt.fy = pa[0].fy;
		pk.fx = pa[1].fx;
		pk.fy = pa[1].fy;

		fang = XPI / 2 - XPI / 10;
		tang = XPI / 2 + XPI / 10;
		xs.fw = dy;
		xs.fh = dy;
		(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, tang, pa, 4);
		t1.fx = pa[0].fx;
		t1.fy = pa[0].fy;
		t2.fx = pa[1].fx;
		t2.fy = pa[1].fy;

		fang = XPI / 2;
		tang = XPI / 2;
		xs.fw = dy;
		xs.fh = dy;
		(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, tang, pa, 2);
		t3.fx = pa[0].fx;
		t3.fy = pa[0].fy;

		fang = XPI / 2 - XPI / 10;
		tang = XPI / 2 + XPI / 10;
		xs.fw = dy;
		xs.fh = dy;
		(*pif->pf_calc_fan)(pif->ctx, &pk, &xs, fang, tang, pa, 4);
		k1.fx = pa[0].fx;
		k1.fy = pa[0].fy;
		k2.fx = pa[1].fx;
		k2.fy = pa[1].fy;

		fang = XPI / 2;
		tang = XPI / 2;
		xs.fw = dy;
		xs.fh = dy;
		(*pif->pf_calc_fan)(pif->ctx, &pk, &xs, fang, tang, pa, 2);
		k3.fx = pa[0].fx;
		k3.fy = pa[0].fy;

		xsprintf(xp.size, _T("%d"), xstol(xp.size) + 2);
		(*pif->pf_draw_line)(pif->ctx, &xp, &pt, &pk);

		xsprintf(xp.size, _T("%d"), xstol(xp.size) - 2);
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		(*pif->pf_draw_line)(pif->ctx, &xp, &pt, &t1);

		(*pif->pf_draw_line)(pif->ctx, &xp, &pt, &t2);

		xr.fx = pt.fx - 0.5f;
		xr.fy = pt.fy - 0.5f;
		xr.fw = 1.0f;
		xr.fh = 1.0f;
		(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

		(*pif->pf_draw_line)(pif->ctx, &xp, &pk, &k1);

		(*pif->pf_draw_line)(pif->ctx, &xp, &pk, &k2);

		xr.fx = pk.fx - 0.5f;
		xr.fy = pk.fy - 0.5f;
		xr.fw = 1.0f;
		xr.fh = 1.0f;
		(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_SOLID);

		sa[0] = _T('M');
		pa[0].fx = t1.fx;
		pa[0].fy = t1.fy;

		sa[1] = _T('A');
		pa[1].fx = 1;
		pa[1].fy = 0;
		pa[2].fx = dy;
		pa[2].fy = dy;
		pa[3].fx = t2.fx;
		pa[3].fy = t2.fy;

		sa[2] = _T('A');
		pa[4].fx = 1;
		pa[4].fy = 0;
		pa[5].fx = dy;
		pa[5].fy = dy;
		pa[6].fx = t1.fx;
		pa[6].fy = t1.fy;

		(*pif->pf_draw_path)(pif->ctx, &xp, &xb, sa, pa, 7);

		color = get_string_ptr(plt->y_colors, 0);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
		}

		if (IS_ZERO_DOUBLE(db1) && IS_ZERO_DOUBLE(db2))
			dbr = 0;
		else
			dbr = (db1) / (db1 + db2);

		xr.fx = t2.fx;
		xr.fw = (t1.fx - t2.fx);
		xr.fh = (t1.fx - t2.fx);
		xr.fy = (t2.fy - xr.fh);
		ft_center_rect(&xr, (float)(xr.fw * dbr), (float)(xr.fh * dbr));
		xr.fy = t2.fy - xr.fh - (t3.fy - t2.fy) * 2;
		(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_TOP);

		label = get_string_ptr(plt->y_labels, 0);
		(*pif->pf_text_size)(pif->ctx, &xf, label, -1, &xs);

		xr.fx = t2.fx + (t1.fx - t2.fx - xs.fw) / 2;
		xr.fy = t3.fy + 1.0f;
		xr.fw = xs.fw;
		xr.fh = dr;
		(*pif->pf_text_out)(pif->ctx, &xf, RECTPOINT(&xr), label, -1);

		sa[0] = _T('M');
		pa[0].fx = k1.fx;
		pa[0].fy = k1.fy;

		sa[1] = _T('A');
		pa[1].fx = 1;
		pa[1].fy = 0;
		pa[2].fx = dy;
		pa[2].fy = dy;
		pa[3].fx = k2.fx;
		pa[3].fy = k2.fy;

		sa[2] = _T('A');
		pa[4].fx = 1;
		pa[4].fy = 0;
		pa[5].fx = dy;
		pa[5].fy = dy;
		pa[6].fx = k1.fx;
		pa[6].fy = k1.fy;

		(*pif->pf_draw_path)(pif->ctx, &xp, &xb, sa, pa, 7);

		color = get_string_ptr(plt->y_colors, 1);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
		}

		if (IS_ZERO_DOUBLE(db1) && IS_ZERO_DOUBLE(db2))
			dbr = 0;
		else
			dbr = (db2) / (db1 + db2);

		xr.fx = k2.fx;
		xr.fw = (k1.fx - k2.fx);
		xr.fh = (k1.fx - k2.fx);
		xr.fy = (k2.fy - xr.fh);
		ft_center_rect(&xr, (float)(xr.fw * dbr), (float)(xr.fh * dbr));
		xr.fy = k2.fy - xr.fh - (k3.fy - k2.fy) * 2;
		(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_TOP);

		label = get_string_ptr(plt->y_labels, 1);
		(*pif->pf_text_size)(pif->ctx, &xf, label, -1, &xs);

		xr.fx = k2.fx + (k1.fx - k2.fx - xs.fw) / 2;
		xr.fy = k3.fy + 1.0f;
		xr.fw = xs.fw;
		xr.fh = dr;
		(*pif->pf_text_out)(pif->ctx, &xf, RECTPOINT(&xr), label, -1);
	}
}

static void _plot_kpigram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xrect_t xr;
	xsize_t xs;
	xpoint_t pt1, pt2;
	xpoint_t pa[15] = { 0 };
	tchar_t sa[10] = { 0 };

	int i, j;
	float dx, dy, dd;
	float datay, origy;
	float zerox, zeroy;

	int x_count;
	int y_count;
	int g_count;

	const tchar_t* label;
	const tchar_t* color;
	const tchar_t* stage;

	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl;
	double y_base, y_step, y_grade;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xscpy(xb.opacity, _T("100"));

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.size, xp.size);
	xscpy(xp_dot.color, xp.color);
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);
	xscpy(xb_dot.style, _T("gradient"));
	xscpy(xb_dot.gradient, _T("vert"));
	xscpy(xb_dot.opacity, _T("100"));

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 1;

	g_count = get_numeric_array_size(plt->y_grades);
	if (!g_count)
		g_count = 1;

	//ruler height
	dy = pbox->fh / (g_count + 1);
	if (dy < 3.0f)
	{
		dy = 3.0f;
		g_count = (int)(pbox->fh / dy) - 1;
	}

	dd = dy / 2;
	if (dd < 3.0f)
		dd = 3.0f;

	//horz label width
	dx = (pbox->fw - dy) / y_count;
	if (dx < 10.0f)
	{
		dx = 10.0f;
		y_count = (int)((pbox->fw - dy) / dx);
	}

	zeroy = pbox->fy + pbox->fh - dy / 2;
	zerox = pbox->fx + dy;

	//stages
	origy = 0;
	for (i = 0; i<g_count; i++)
	{
		stage = get_string_ptr(plt->y_stages, i);

		y_base = get_numeric(plt->y_bases, 0);
		y_step = get_numeric(plt->y_steps, 0);
		y_grade = get_numeric(plt->y_grades, i);

		datay = (float)((y_grade - y_base) / y_step) * dy;

		(*pif->pf_text_size)(pif->ctx, &xf_dot, stage, -1, &xs);
		pt1.fx = zerox - xs.fw;
		pt1.fy = zeroy - datay - xs.fh / 2;
		(*pif->pf_text_out)(pif->ctx, &xf_dot, &pt1, stage, -1);

		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
		}

		pt1.fx = zerox;
		pt1.fy = zeroy - datay;
		pt2.fx = zerox + y_count * dx;
		pt2.fy = zeroy - datay;
		(*pif->pf_draw_line)(pif->ctx, &xp_dot, &pt1, &pt2);

		origy = datay;
	}

	//kpi lines
	for (j = 0; j < y_count; j++)
	{
		label = get_string_ptr(plt->y_labels, j);

		xr.fx = zerox + j * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);

		y_base = get_numeric(plt->y_bases, j);
		y_step = get_numeric(plt->y_steps, j);
		dbl = matrix_get_value(pmt, 0, j);

		if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			datay = (float)((dbl - y_base) / y_step) * dy;
		else
			datay = 0;

		if (!IS_ZERO_FLOAT(datay))
		{
			sa[0] = _T('M');
			pa[0].fx = zerox + j * dx;
			pa[0].fy = zeroy;

			sa[1] = _T('Q');
			pa[1].fx = zerox + j * dx + dx / 3;
			pa[1].fy = zeroy;
			pa[2].fx = zerox + j * dx + 2 * dx / 5;
			pa[2].fy = zeroy - datay + dd;
			if (pa[2].fy > zeroy)
			{
				pa[2].fy = zeroy;
			}

			sa[2] = _T('Q');
			pa[3].fx = zerox + j * dx + 2 * dx / 5;
			pa[3].fy = zeroy - datay + dd - 2.0f;
			if (pa[3].fy > zeroy)
			{
				pa[3].fy = zeroy;
			}
			pa[4].fx = zerox + j * dx + dx / 5;
			pa[4].fy = zeroy - datay + dd;
			if (pa[4].fy > zeroy)
			{
				pa[4].fy = zeroy;
			}

			sa[3] = _T('L');
			pa[5].fx = zerox + j * dx + dx / 2;
			pa[5].fy = zeroy - datay;

			sa[4] = _T('L');
			pa[6].fx = zerox + (j + 1) * dx - dx / 5;
			pa[6].fy = zeroy - datay + dd;
			if (pa[6].fy > zeroy)
			{
				pa[6].fy = zeroy;
			}

			sa[5] = _T('Q');
			pa[7].fx = zerox + (j + 1) * dx - 2 * dx / 5;
			pa[7].fy = zeroy - datay + dd - 2.0f;
			if (pa[7].fy > zeroy)
			{
				pa[7].fy = zeroy;
			}
			pa[8].fx = zerox + (j + 1) * dx - 2 * dx / 5;
			pa[8].fy = zeroy - datay + dd;
			if (pa[8].fy > zeroy)
			{
				pa[8].fy = zeroy;
			}

			sa[6] = _T('Q');
			pa[9].fx = zerox + j * dx + 2 * dx / 3;
			pa[9].fy = zeroy;
			pa[10].fx = zerox + (j + 1) * dx;
			pa[10].fy = zeroy;

			(*pif->pf_draw_path)(pif->ctx, &xp, &xb, sa, pa, 11);
		}

		/*if (IS_VALID_DOUBLE(dbl))
			xsprintf(numstr, _T("%.f"), dbl);
		else
			xscpy(numstr, _T("N"));

		xr.fx = zerox + j * dx;
		xr.fw = dx;
		xr.fy = zeroy - datay - dy / 2;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);*/
	}
}

static void _plot_taskgram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt1, pt2;
	xrect_t xr;
	xpoint_t pa[15] = { 0 };
	tchar_t sa[10] = { 0 };

	int i, j;
	float dx, dy;
	float zeroy, zerox;

	int x_count;
	int y_count;

	const tchar_t* label;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));

	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));

	y_count = pmt->cols;
	if (!y_count)
		y_count = 3;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 7;

	//ruler height
	dy = pbox->fh / (y_count + 1);
	if (dy < 5.0f)
	{
		dy = 5.0f;
		y_count = (int)(pbox->fh / dy) - 1;
	}

	//horz label width
	dx = (pbox->fw - dy) / x_count;
	if (dx < 10.0f)
	{
		dx = 10.0f;
		x_count = (int)((pbox->fw - dy) / dx);
	}

	zeroy = pbox->fy + pbox->fh - dy / 2;
	zerox = pbox->fx + dy;

	//vert ruler and labels
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	for (i = 0; i < y_count; i++)
	{
		//y labels
		label = get_string_ptr(plt->y_labels, i);

		xr.fx = zerox - dy;
		xr.fw = dy;
		xr.fy = zeroy - (i + 1) * dy;
		xr.fh = dy;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);

		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		pt1.fx = zerox;
		pt1.fy = zeroy - (i + 1) * dy;
		pt2.fx = zerox + x_count * dx;
		pt2.fy = zeroy - (i + 1) * dy;
		//(*pif->pf_draw_line)(pif->ctx, &xp_dot, &pt1, &pt2);
	}

	//task rect
	for (i = 0; i < x_count; i++)
	{
		label = get_string_ptr(plt->x_labels, i);

		xr.fx = zerox + i * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);

		color = get_string_ptr(plt->x_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}

		for (j = 0; j < y_count; j++)
		{
			dbl = matrix_get_value(pmt, i, j);
			if (!IS_VALID_DOUBLE(dbl))
				dbl = 0;

			xr.fx = zerox + i * dx;
			xr.fw = dx;
			xr.fy = zeroy - (j + 1) * dy;
			xr.fh = dy;

			ft_expand_rect(&xr, -1.0f, -1.0f);

			if ((int)dbl)
			{
				(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);
			}
			else
			{
				(*pif->pf_draw_rect)(pif->ctx, &xp_dot, NULL, &xr);
			}
		}
	}
}

static void _plot_scattergram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt1, pt2;
	xrect_t xr;

	int i, j;
	float dd, dx, dy;
	float middy, zeroy, zerox;

	int y_count;
	int y_ruler;
	int x_count;
	int x_ruler;

	const tchar_t* label;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl;
	double y_base, y_step;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 5;

	//y-ruler count
	y_ruler = 10;
	x_ruler = (x_count > 10) ? x_count : 5;

	//y-label width
	dd = 10.0f;

	//y-ruler height
	dy = pbox->fh / (y_ruler + 2);
	if (dy < 5.0f)
	{
		dy = 5.0f;
		y_ruler = (int)(pbox->fh / dy) - 2;
	}

	//x-ruler width
	dx = (pbox->fw - dd * y_count) / (x_ruler + 2);
	if (dx < 10.0f)
	{
		dx = 10.0f;
		x_ruler = (int)((pbox->fw - dd * y_count) / dx) - 2;
	}

	zeroy = pbox->fy + pbox->fh - dy;
	zerox = pbox->fx + dd * y_count;
	middy = zeroy - (int)(y_ruler / 2) * dy;
	
	//y-axis
	pt1.fx = zerox;
	pt1.fy = zeroy - y_ruler * dy;
	pt2.fx = zerox;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//y ruler and labels
	for (i = 0; i< y_count; i++)
	{
		//y labels
		label = get_string_ptr(plt->y_labels, i);

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - y_ruler * dy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);

		dbl = get_numeric(plt->y_bases, i); 

		for (j = y_ruler / 2 + 1; j < y_ruler; j++)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
			}

			if (IS_VALID_DOUBLE(dbl))
				dbl += get_numeric(plt->y_steps, i);

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 4;
			xr.fh = dd / 2;
			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.2f"), dbl);
			else
				xscpy(numstr, _T("N"));

			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);
		}

		dbl = get_numeric(plt->y_bases, i);
		for (j = y_ruler / 2; j > 0; j--)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
			}

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 4;
			xr.fh = dd / 2;
			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.2f"), dbl);
			else
				xscpy(numstr, _T("N"));
			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);

			dbl -= get_numeric(plt->y_steps, i);
		}
	}

	//vert line color
	for (i = 0; i < y_count; i++)
	{
		//y colors
		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		xr.fx = zerox - (i + 1) * dd + 0.5f;
		xr.fw = dy;
		xr.fy = zeroy - dy;
		xr.fh = dy;
		ft_center_rect(&xr, 3.0f, 3.0f);
		xr.fy = zeroy - xr.fh;
		(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);
	}

	//x axis
	pt1.fx = zerox;
	pt1.fy = zeroy;
	pt2.fx = zerox + x_ruler * dx;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//x middle axis
	pt1.fx = zerox;
	pt1.fy = middy;
	pt2.fx = zerox + x_ruler * dx;
	pt2.fy = middy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//x ruler and labels
	for (i = 1; i <= x_ruler; i++)
	{
		if (i < x_ruler)
		{
			pt1.fx = zerox + i * dx;
			pt1.fy = zeroy;
			pt2.fx = zerox + i * dx;
			pt2.fy = zeroy + 2.0f;
			(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
		}

		label = get_string_ptr(plt->x_labels, i - 1);
	
		xr.fx = zerox + (i - 1) * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr, label, -1);
	}

	//scatter dot
	for (i = 0; i < pmt->cols; i++)
	{
		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
		{
			xscpy(xb_dot.color, color);
		}
		else
		{
			xscpy(xb_dot.color, xb.color);
		}

		for (j = 0; j < pmt->rows; j++)
		{
			y_base = get_numeric(plt->y_bases, i);
			y_step = get_numeric(plt->y_steps, i);
			dbl = matrix_get_value(pmt, j, i);

			pt1.fx = zerox + (j + 1) * dx - dx / 2;
			if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
				pt1.fy = middy - (float)((dbl - y_base) / y_step) * dy;
			else
				pt1.fy = 0;

			xr.fx = pt1.fx - 1.0f;
			xr.fy = pt1.fy - 1.0f;
			xr.fw = 2.0f;
			xr.fh = 2.0f;
			(*pif->pf_draw_ellipse)(pif->ctx, NULL, &xb_dot, &xr);
		}
	}
}

static void _plot_mediangram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt1, pt2;
	xrect_t xr;
	xsize_t xs;
	xpoint_t* pa1;
	xpoint_t* pa2;

	int i, j;
	float dd, dx, dy, fr;
	float grady, zeroy, zerox;

	int y_ruler;
	int x_count;
	int x_ruler;
	int g_count;

	const tchar_t* label;
	const tchar_t* color;
	const tchar_t* stage;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double db_val,db_min,db_mid,db_max;
	double y_base, y_step, y_grade;

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xscpy(xb.opacity, _T("100"));

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);

	x_count = pmt->rows;
	if (!x_count)
		x_count = 7;

	g_count = get_numeric_array_size(plt->y_grades);

	//y-ruler count
	y_ruler = 10;
	
	//y-ruler height
	dy = pbox->fh / (y_ruler + 2);
	if (dy < 5.0f)
	{
		dy = 5.0f;
		y_ruler = (int)(pbox->fh / dy) - 2;
	}

	//x-ruler width
	dx = pbox->fw / (x_count + 2);
	if (dx < 8.0f)
	{
		dx = 8.0f;
	}
	x_ruler = (int)((pbox->fw - dx) / dx);
	dd = 5.0f;

	zeroy = pbox->fy + pbox->fh - dy;
	zerox = pbox->fx + dx;

	//y-axis
	pt1.fx = zerox;
	pt1.fy = zeroy - y_ruler * dy;
	pt2.fx = zerox;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	db_val = 0;
	for (j = 1; j <= y_ruler; j++)
	{
		if (IS_VALID_DOUBLE(db_val))
			db_val += get_numeric(plt->y_steps, 0);

		pt1.fx = zerox - 2.0f;
		pt1.fy = zeroy - j * dy;
		pt2.fx = zerox;
		pt2.fy = zeroy - j * dy;
		(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

		xr.fx = zerox - dx;
		xr.fw = dx;
		xr.fy = zeroy - j * dy - dx / 4;
		xr.fh = dx / 2;

		if (IS_VALID_DOUBLE(db_val))
			xsprintf(numstr, _T("%.2f"), db_val);
		else
			xscpy(numstr, _T("N"));

		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);
	}

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);
	//stages
	for (i = 0; i<g_count; i++)
	{
		stage = get_string_ptr(plt->y_stages, i);

		y_base = get_numeric(plt->y_bases, 0);
		y_step = get_numeric(plt->y_steps, 0);
		y_grade = get_numeric(plt->y_grades, i);

		grady = (float)((y_grade - y_base) / y_step) * dy;

		(*pif->pf_text_size)(pif->ctx, &xf_dot, stage, -1, &xs);
		pt1.fx = zerox + x_ruler * dx - xs.fw;
		pt1.fy = zeroy - grady - xs.fh;
		(*pif->pf_text_out)(pif->ctx, &xf_dot, &pt1, stage, -1);

		color = get_string_ptr(plt->y_colors, 0);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
		}

		pt1.fx = zerox;
		pt1.fy = zeroy - grady;
		pt2.fx = zerox + x_ruler * dx;
		pt2.fy = zeroy - grady;
		(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
	}

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_SOLID);

	//x axis
	pt1.fx = zerox;
	pt1.fy = zeroy;
	pt2.fx = zerox + x_ruler * dx;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//x ruler and labels
	for (i = 1; i <= x_ruler; i++)
	{
		if (i < x_ruler)
		{
			pt1.fx = zerox + i * dx;
			pt1.fy = zeroy;
			pt2.fx = zerox + i * dx;
			pt2.fy = zeroy + 2.0f;
			(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
		}

		label = get_string_ptr(plt->x_labels, i - 1);

		xr.fx = zerox + (i - 1) * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr, label, -1);
	}

	pa1 = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * pmt->rows);
	pa2 = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * pmt->rows);
	//median rect
	for (i = 0; i < pmt->rows; i++)
	{
		y_base = get_numeric(plt->y_bases, 0);
		y_step = get_numeric(plt->y_steps, 0);

		db_val = matrix_get_value(pmt, i, 0);
		db_min = matrix_get_value(pmt, i, 1);
		db_mid = matrix_get_value(pmt, i, 2);
		db_max = matrix_get_value(pmt, i, 3);

		if (IS_VALID_DOUBLE(db_max) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			fr = (float)((db_val - y_base) / (y_step));
		else
			fr = 0;

		pa1[i].fx = zerox + dx * i + dx / 2;
		pa1[i].fy = zeroy - dy * fr;

		if (IS_VALID_DOUBLE(db_max) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			fr = (float)((db_mid - y_base) / (y_step));
		else
			fr = 0;

		pa2[i].fx = zerox + dx * i + dx / 2;
		pa2[i].fy = zeroy - dy * fr;

		if (IS_VALID_DOUBLE(db_max) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			fr = (float)((db_max - y_base) / (y_step));
		else
			fr = 0;

		pt1.fx = zerox + dx * i;
		pt1.fy = zeroy - dy * fr;

		if (IS_VALID_DOUBLE(db_min) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			fr = (float)((db_min - y_base) / (y_step));
		else
			fr = 0;

		pt2.fx = zerox + dx * (i + 1);
		pt2.fy = zeroy - dy * fr;

		xr.fx = pt1.fx;
		xr.fy = pt1.fy;
		xr.fw = pt2.fx - pt1.fx;
		xr.fh = pt2.fy - pt1.fy;

		ft_expand_rect(&xr, (dd - dx) / 2, 0);

		(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

		if (IS_VALID_DOUBLE(db_max))
			xsprintf(numstr, _T("%.f"), db_val);
		else
			xscpy(numstr, _T("N"));

		(*pif->pf_text_size)(pif->ctx, &xf_dot, numstr, -1, &xs);
		pt1.fx = xr.fx + (xr.fw - xs.fw) / 2;
		pt1.fy = xr.fy - xs.fh;
		(*pif->pf_text_out)(pif->ctx, &xf_dot, &pt1, numstr, -1);

		if (IS_VALID_DOUBLE(db_min))
			xsprintf(numstr, _T("%.f"), db_mid);
		else
			xscpy(numstr, _T("N"));

		(*pif->pf_text_size)(pif->ctx, &xf_dot, numstr, -1, &xs);
		pt2.fx = xr.fx + (xr.fw - xs.fw) / 2;
		pt2.fy = xr.fy + xr.fh;
		(*pif->pf_text_out)(pif->ctx, &xf_dot, &pt2, numstr, -1);
	}

	color = get_string_ptr(plt->y_colors, 0);
	if (!is_null(color))
	{
		xscpy(xp_dot.color, color);
		xscpy(xb_dot.color, color);
		lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
	}
	else
	{
		xscpy(xp_dot.color, xp.color);
		xscpy(xb_dot.color, xb.color);
	}

	(*pif->pf_draw_curve)(pif->ctx, &xp_dot, pa1, pmt->rows);

	for (i = 0; i < pmt->rows; i++)
	{
		xr.fx = pa1[i].fx - 1.0f;
		xr.fy = pa1[i].fy - 1.0f;
		xr.fw = 2.0f;
		xr.fh = 2.0f;

		(*pif->pf_draw_ellipse)(pif->ctx, &xp_dot, &xb_dot, &xr);
	}

	xr.fx = zerox;
	xr.fy = zeroy - dy * (y_ruler + 1);
	xr.fw = dy;
	xr.fh = dy;
	ft_center_rect(&xr, 2.0f, 2.0f);
	(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);

	xr.fx = zerox + dy;
	xr.fy = zeroy - dy * (y_ruler + 1);
	xr.fw = dx;
	xr.fh = dy;
	(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr, _T("得分值"), -1);

	color = get_string_ptr(plt->y_colors, 1);
	if (!is_null(color))
	{
		xscpy(xp_dot.color, color);
		xscpy(xb_dot.color, color);
		lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
	}
	else
	{
		xscpy(xp_dot.color, xp.color);
		xscpy(xb_dot.color, xb.color);
	}

	(*pif->pf_draw_curve)(pif->ctx, &xp_dot, pa2, pmt->rows);

	for (i = 0; i < pmt->rows; i++)
	{
		xr.fx = pa2[i].fx - 1.0f;
		xr.fy = pa2[i].fy - 1.0f;
		xr.fw = 2.0f;
		xr.fh = 2.0f;

		(*pif->pf_draw_ellipse)(pif->ctx, &xp_dot, &xb_dot, &xr);
	}

	xr.fx = zerox + dy + dx;
	xr.fy = zeroy - dy * (y_ruler + 1);
	xr.fw = dy;
	xr.fh = dy;
	ft_center_rect(&xr, 2.0f, 2.0f);
	(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);

	xr.fx = zerox + 2 * dy + dx;
	xr.fy = zeroy - dy * (y_ruler + 1);
	xr.fw = dx;
	xr.fh = dy;
	(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr, _T("中位值"), -1);

	xmem_free(pa1);
	xmem_free(pa2);
}

static void _plot_histogram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt1, pt2;
	xrect_t xr;
	xsize_t xs;

	int i, j;
	float dd, dx, cx, dy, fr;
	float middy, grady, zeroy, zerox;

	int g_count;
	int y_count;
	int y_ruler;
	int x_count;
	int x_ruler;

	const tchar_t* label;
	const tchar_t* color;
	const tchar_t* stage;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl;
	double y_base, y_step, y_grade;

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	parse_xface_from_style(&xa, plt->style);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);
	xscpy(xb_dot.opacity, _T("100"));

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;
	if (!x_count)
		x_count = 5;

	g_count = get_numeric_array_size(plt->y_grades);

	//y-ruler count
	y_ruler = 10;
	x_ruler = (x_count > 10) ? x_count : 5;

	//y-label width
	dd = 10.0f;

	//y-ruler height
	dy = pbox->fh / (y_ruler + 2);
	if (dy < 5.0f)
	{
		dy = 5.0f;
		y_ruler = (int)(pbox->fh / dy) - 2;
	}

	//x-ruler width
	dx = (pbox->fw - dd * y_count) / (x_ruler + 2);
	if (dx < 5.0f)
	{
		dx = 5.0f;
	}

	cx = dx / y_count;
	if (cx < 2.0f)
		cx = 2.0f;

	zeroy = pbox->fy + pbox->fh - dy;
	zerox = pbox->fx + dd * y_count;
	middy = zeroy - (int)(y_ruler / 2) * dy;

	//y-axis
	pt1.fx = zerox;
	pt1.fy = zeroy - y_ruler * dy;
	pt2.fx = zerox;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//y ruler and labels
	for (i = 0; i < y_count; i++)
	{
		//y labels
		label = get_string_ptr(plt->y_labels, i);

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - y_ruler * dy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);

		dbl = get_numeric(plt->y_bases, i);

		for (j = y_ruler / 2 + 1; j < y_ruler; j++)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
			}

			if (IS_VALID_DOUBLE(dbl))
				dbl += get_numeric(plt->y_steps, i);

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 4;
			xr.fh = dd / 2;
			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.2f"), dbl);
			else
				xscpy(numstr, _T("N"));

			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);
		}

		dbl = get_numeric(plt->y_bases, i);

		for (j = y_ruler / 2; j > 0; j--)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
			}

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 4;
			xr.fh = dd / 2;
			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.2f"), dbl);
			else
				xscpy(numstr, _T("N"));
			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);

			if (IS_VALID_DOUBLE(dbl))
				dbl -= get_numeric(plt->y_steps, i);
		}

		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - dy;
		xr.fh = dd;
		ft_center_rect(&xr, 2.0f, 2.0f);
		xr.fy = zeroy - xr.fh;
		(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);
	}

	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);
	//stages
	for (i = 0; i<g_count; i++)
	{
		stage = get_string_ptr(plt->y_stages, i);

		y_base = get_numeric(plt->y_bases, 0);
		y_step = get_numeric(plt->y_steps, 0);
		y_grade = get_numeric(plt->y_grades, i);

		grady = (float)((y_grade - y_base) / y_step) * dy;

		(*pif->pf_text_size)(pif->ctx, &xf_dot, stage, -1, &xs);
		pt1.fx = zerox + x_ruler * dx - xs.fw;
		pt1.fy = middy - grady - xs.fh;
		(*pif->pf_text_out)(pif->ctx, &xf_dot, &pt1, stage, -1);

		color = get_string_ptr(plt->y_colors, 0);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
		}

		pt1.fx = zerox;
		pt1.fy = middy - grady;
		pt2.fx = zerox + x_ruler * dx;
		pt2.fy = middy - grady;
		(*pif->pf_draw_line)(pif->ctx, &xp_dot, &pt1, &pt2);
	}

	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_SOLID);

	//x axis
	pt1.fx = zerox;
	pt1.fy = zeroy;
	pt2.fx = zerox + x_ruler * dx;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//x middle axis
	pt1.fx = zerox;
	pt1.fy = middy;
	pt2.fx = zerox + x_ruler * dx;
	pt2.fy = middy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//x ruler and labels
	for (i = 1; i <= x_ruler; i++)
	{
		if (i < x_ruler)
		{
			pt1.fx = zerox + i * dx;
			pt1.fy = zeroy;
			pt2.fx = zerox + i * dx;
			pt2.fy = zeroy + 2.0f;
			(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
		}

		label = get_string_ptr(plt->x_labels, i - 1);

		xr.fx = zerox + (i - 1) * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr, label, -1);
	}

	//histo rect
	for (i = 0; i < pmt->rows; i++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			color = get_string_ptr(plt->y_colors, j);
			if (!is_null(color))
			{
				xscpy(xp_dot.color, color);
				xscpy(xb_dot.color, color);
				lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
			}
			else
			{
				xscpy(xp_dot.color, xp.color);
				xscpy(xb_dot.color, xb.color);
			}

			y_base = get_numeric(plt->y_bases, j);
			y_step = get_numeric(plt->y_steps, j);
			dbl = matrix_get_value(pmt, i, j);

			if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
				fr = (float)((dbl - y_base) / (y_step));
			else
				fr = 0;

			xr.fx = zerox + dx * i + cx * j;
			xr.fw = cx;
			if (fr > 0)
			{
				xr.fy = middy - dy * fr;
				xr.fh = dy * fr;
			}
			else if (fr < 0)
			{
				xr.fy = middy;
				xr.fh = -dy * fr;
			}
			else
			{
				xr.fy = middy - 0.5f;
				xr.fh = 1.0f;
			}

			ft_expand_rect(&xr, -0.5f, 0);

			(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);

			if (fr >= 0)
			{
				xr.fy -= dy;
				xr.fh = dy;
			}
			else
			{
				xr.fy += xr.fh;
				xr.fh = dy;
			}

			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.f"), dbl);
			else
				xscpy(numstr, _T("N"));
			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr, numstr, -1);
		}
	}
}

static void _plot_trendgram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt1, pt2;
	xrect_t xr;
	xsize_t xs;
	xpoint_t* pa;

	int i, j, n;
	float dx, dy, dd;
	float miny, maxy, middy, zeroy, zerox;

	int g_count;
	int y_count;
	int y_ruler;
	int x_count;
	int x_ruler;

	const tchar_t* label;
	const tchar_t* color;
	const tchar_t* shape;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl;
	double y_grade, y_base, y_step;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xb.color);

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	x_count = pmt->rows;

	g_count = get_numeric_array_size(plt->y_grades);

	//ruler count
	y_ruler = (plt->ruler) ? plt->ruler : 10;
	x_ruler = (x_count > 7) ? x_count : 7;

	dy = pbox->fh / (y_ruler + 2);
	if (dy < 5.0f)
	{
		dy = 5.0f;
		y_ruler = (int)(pbox->fh / dy) - 2;
	}

	//x-ruler width
	dd = 8.0f;
	dx = (pbox->fw - y_count * dd) / (x_ruler + 1);
	if (dx < dd)
	{
		dx = dd;
		x_ruler = (int)((pbox->fw - y_count * dd) / dx) - 1;
	}
	
	zeroy = pbox->fy + pbox->fh - dy;
	zerox = pbox->fx + dd * y_count;
	middy = zeroy - (int)(y_ruler / 2) * dy;

	//vert axis
	pt1.fx = zerox;
	pt1.fy = zeroy - y_ruler * dy;
	pt2.fx = zerox;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//vert ruler and labels
	for (i = 0; i < y_count; i++)
	{
		label = get_string_ptr(plt->y_labels, i);

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - y_ruler * dy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);

		dbl = get_numeric(plt->y_bases, i);

		for (j = y_ruler / 2 + 1; j < y_ruler; j++)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
			}

			if (IS_VALID_DOUBLE(dbl))
				dbl += get_numeric(plt->y_steps, i);

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dy / 2;
			xr.fh = dy;
			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.2f"), dbl);
			else
				xscpy(numstr, _T("N"));
			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);
		}

		dbl = get_numeric(plt->y_bases, i);

		for (j = y_ruler / 2; j > 0; j--)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
			}

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dy / 2;
			xr.fh = dy;
			if (IS_VALID_DOUBLE(dbl))
				xsprintf(numstr, _T("%.2f"), dbl);
			else
				xscpy(numstr, _T("N"));
			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);

			dbl -= get_numeric(plt->y_steps, i);
		}

		//y colors
		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - dy;
		xr.fh = dy;
		ft_center_rect(&xr, 2.0f, 2.0f);
		xr.fy = zeroy - xr.fh;

		shape = get_string_ptr(plt->y_shapes, i);

		if (compare_text(shape, -1, _T("ellipse"), -1, 1) == 0)
			(*pif->pf_draw_ellipse)(pif->ctx, &xp_dot, &xb_dot, &xr);
		else if (compare_text(shape, -1, _T("rect"), -1, 1) == 0)
			(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);
		else if (compare_text(shape, -1, _T("round"), -1, 1) == 0)
			(*pif->pf_draw_round)(pif->ctx, &xp_dot, &xb_dot, &xr);
		else if (compare_text(shape, -1, _T("left-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_LEFT);
		else if (compare_text(shape, -1, _T("right-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_RIGHT);
		else if (compare_text(shape, -1, _T("top-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_TOP);
		else if (compare_text(shape, -1, _T("bottom-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_BOTTOM);
		else
			(*pif->pf_draw_ellipse)(pif->ctx, &xp_dot, &xb_dot, &xr);
	}

	//horz axis
	pt1.fx = zerox;
	pt1.fy = zeroy;
	pt2.fx = zerox + (x_ruler + 1) * dx;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	//horz stage axis
	miny = zeroy;
	maxy = zeroy - y_ruler * dy;

	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_FAR);

	for (i = 0; i < g_count; i++)
	{
		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
			xscpy(xp_dot.color, color);
		else
			xscpy(xp_dot.color, xp.color);

		y_grade = get_numeric(plt->y_grades, i);
		y_base = get_numeric(plt->y_bases, i);
		y_step = get_numeric(plt->y_steps, i);

		pt1.fx = zerox;
		if (IS_VALID_DOUBLE(y_grade) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			pt1.fy = middy - (float)((y_grade - y_base) / y_step) * dy;
		else
			pt1.fy = middy;

		pt2.fx = zerox + (x_ruler + 1) * dx;
		pt2.fy = pt1.fy;
		(*pif->pf_draw_line)(pif->ctx, &xp_dot, &pt1, &pt2);

		if (miny > pt1.fy)
			miny = pt1.fy;
		if (maxy < pt1.fy)
			maxy = pt1.fy;

		xr.fx = pt2.fx - 2 * dx;
		xr.fw = 2 * dx;
		xr.fy = pt2.fy - dy / 2;
		xr.fh = dy / 2;

		label = get_string_ptr(plt->y_labels, i);
		if (!is_null(label))
		{
			xscpy(numstr, label);
			n = xslen(numstr);
			if (IS_VALID_DOUBLE(y_grade))
				xsprintf((numstr + n), _T(" %.f"), y_grade);
			else
				xscat((numstr + n), _T(" N"));

			(*pif->pf_text_size)(pif->ctx, &xf_dot, numstr, -1, &xs);

			pt1.fx = pt2.fx - xs.fw;
			pt1.fy = pt2.fy - xs.fh;
			(*pif->pf_text_out)(pif->ctx, &xf_dot, &pt1, numstr, -1);
		}
	}

	//trend lines
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_SOLID);
	//xb_dot.shadow.offx = 1;
	//xb_dot.shadow.offy = 1;

	pa = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * pmt->rows);

	for (i = 0; i < pmt->cols; i++)
	{
		color = get_string_ptr(plt->y_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xb_dot.color, color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		shape = get_string_ptr(plt->y_shapes, i);

		for (j = 0; j < pmt->rows; j++)
		{
			y_base = get_numeric(plt->y_bases, i);
			y_step = get_numeric(plt->y_steps, i);
			dbl = matrix_get_value(pmt, j, i);

			pa[j].fx = zerox + (j + 1) * dx - dx / 2;
			if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
				pa[j].fy = middy - (float)((dbl - y_base) / y_step) * dy;
			else
				pa[j].fy = middy;
		}

		(*pif->pf_draw_curve)(pif->ctx, &xp_dot, pa, pmt->rows);

		for (j = 0; j < pmt->rows;j++)
		{
			xr.fx = pa[j].fx - 1.0f;
			xr.fy = pa[j].fy - 1.0f;
			xr.fw = 2.0f;
			xr.fh = 2.0f;

			if (compare_text(shape, -1, _T("ellipse"), -1, 1) == 0)
				(*pif->pf_draw_ellipse)(pif->ctx, &xp_dot, &xb_dot, &xr);
			else if (compare_text(shape, -1, _T("rect"), -1, 1) == 0)
				(*pif->pf_draw_rect)(pif->ctx, &xp_dot, &xb_dot, &xr);
			else if (compare_text(shape, -1, _T("round"), -1, 1) == 0)
				(*pif->pf_draw_round)(pif->ctx, &xp_dot, &xb_dot, &xr);
			else if (compare_text(shape, -1, _T("left-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_LEFT);
			else if (compare_text(shape, -1, _T("right-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_RIGHT);
			else if (compare_text(shape, -1, _T("top-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_TOP);
			else if (compare_text(shape, -1, _T("bottom-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->ctx, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_BOTTOM);
			else
				(*pif->pf_draw_ellipse)(pif->ctx, &xp_dot, &xb_dot, &xr);
		}
	}

	xmem_free(pa);

	xscpy(xb.opacity, _T("50"));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	for (i = 0; i < x_ruler; i++)
	{
		color = get_string_ptr(plt->x_colors, i);

		if (!is_null(color))
		{
			xscpy(xb.color, color);

			xr.fx = zerox + i * dx;
			xr.fw = dx;
			xr.fy = zeroy - y_ruler * dy;
			xr.fh = y_ruler * dy;
			(*pif->pf_draw_rect)(pif->ctx, NULL, &xb, &xr);
		}

		pt1.fx = zerox + (i + 1) * dx;
		pt1.fy = zeroy;
		pt2.fx = zerox + (i + 1) * dx;
		pt2.fy = zeroy + 2.0f;
		(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

		label = get_string_ptr(plt->x_labels, i);

		xr.fx = zerox + i * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);
	}
}

static void _plot_pantogram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pa[4] = { 0 };
	xrect_t xr, xr_pi;
	xsize_t xs;

	int i, j;
	float dd, rr, fr;
	float middy, middx;

	const tchar_t* label;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double nxtd, pred;
	double dbl;
	double total = 0.0;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.style, xb.style);
	xscpy(xb_dot.gradient, xb.gradient);

	rr = (pbox->fw < pbox->fh) ? (pbox->fw / 2) : (pbox->fh / 2);
	middx = pbox->fx + pbox->fw / 2;
	middy = pbox->fy + pbox->fh / 2;

	//ruler span
	dd = rr / 12;
	if (dd < 4.0f)
		dd = 4.0f;

	pt.fx = middx;
	pt.fy = middy;

	xr_pi.fx = middx - (rr - 2 * dd);
	xr_pi.fy = middy - (rr - 2 * dd);
	xr_pi.fw = 2 * (rr - 2 * dd);
	xr_pi.fh = 2 * (rr - 2 * dd);

	for (i = 0; i < 1; i ++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			total += matrix_get_value(pmt, i, j);
		}
	}

	if (IS_ZERO_DOUBLE(total))
	{
		(*pif->pf_draw_pie)(pif->ctx, &xp_dot, NULL, &xr_pi, 0, 3 * XPI / 2);
	}

	nxtd = 0.0;
	for (i = 0; i < 1; i++)
	{
		total = 0;
		for (j = 0; j < pmt->cols; j++)
		{
			total += matrix_get_value(pmt, i, j);
		}

		for (j = 0; j < pmt->cols; j++)
		{
			color = get_string_ptr(plt->y_colors, j);
			
			if (!is_null(color))
				xscpy(xb_dot.color, color);
			else
				xscpy(xb_dot.color, xb.color);

			pred = nxtd;

			dbl = matrix_get_value(pmt, i, j);

			if (IS_VALID_DOUBLE(dbl) && !IS_ZERO_DOUBLE(total))
				fr = (float)(dbl / total);
			else
				fr = 0.0f;

			nxtd = pred + (float)(2 * XPI *  fr);
			if (pred != nxtd)
			{
				(*pif->pf_draw_pie)(pif->ctx, &xp_dot, &xb_dot, &xr_pi, pred, nxtd);

				xs.fw = rr - dd;
				xs.fh = dd;
				(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, (pred + nxtd) / 2, (pred + nxtd) / 2, pa, 4);

				(*pif->pf_draw_line)(pif->ctx, &xp_dot, &pa[0], &pa[2]);

				if (pa[0].fx > pt.fx)
				{
					pa[1].fx = pa[0].fx + 2 * dd;
					pa[1].fy = pa[0].fy;

					pa[2].fx = pa[0].fx;
					pa[2].fy = pa[0].fy - dd;

					pa[3].fx = pa[0].fx;
					pa[3].fy = pa[0].fy;
				}
				else
				{
					pa[1].fx = pa[0].fx - 2 * dd;
					pa[1].fy = pa[0].fy;

					pa[2].fx = pa[0].fx - 2 * dd;
					pa[2].fy = pa[0].fy - dd;

					pa[3].fx = pa[0].fx - 2 * dd;
					pa[3].fy = pa[0].fy;
				}

				(*pif->pf_draw_line)(pif->ctx, &xp_dot, &pa[0], &pa[1]);

				xr.fx = pa[2].fx;
				xr.fw = 2 * dd;
				xr.fy = pa[2].fy;
				xr.fh = dd;
				xsprintf(numstr, _T("%.2f%"), fr * 100);
				(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, numstr, -1);

				label = get_string_ptr(plt->y_labels, j);

				xr.fx = pa[3].fx;
				xr.fw = 2 * dd;
				xr.fy = pa[3].fy;
				xr.fh = dd;
				(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);
			}
		}
	}
}


static void _plot_radargram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk;
	xpoint_t* pa;
	xrect_t xr;
	xsize_t xs;
	xspan_t xn;

	int i, j;
	float rr, dd, dx, dy, fr;
	float middx, middy;
	int qa;

	int y_ruler;
	int y_count;
	int x_count;

	const tchar_t* label;
	const tchar_t* color;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl;
	double y_base, y_step;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xb.color);

	y_count = pmt->cols;
	if (y_count < 3)
		y_count = 3;

	x_count = pmt->rows;
	
	//ruler count
	y_ruler = (plt->ruler) ? plt->ruler : 5;

	rr = (pbox->fw < pbox->fh) ? (pbox->fw / 2) : (pbox->fh / 2);
	middx = pbox->fx + pbox->fw / 2;
	middy = pbox->fy + pbox->fh / 2;

	//ruler span
	dd = rr / (y_ruler + 1);
	if (dd < 2.0f)
		dd = 2.0f;

	//x labels
	for (i = 0; i < x_count; i++)
	{
		color = get_string_ptr(plt->x_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
		}

		label = get_string_ptr(plt->x_labels, i);
	
		xr.fx = middx - dd * (y_ruler + 1);
		xr.fy = middy - dd * (y_ruler + 1) + (i + 1) * (dd / 2);
		pk.fx = xr.fx + dd / 2;
		pk.fy = xr.fy;
		(*pif->pf_draw_line)(pif->ctx, &xp_dot, RECTPOINT(&xr), &pk);

		xr.fx += (dd / 2 + 0.5f);
		xr.fy -= dd / 2;
		(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa_dot, &xr, label, -1);
		xr.fx -= (dd / 2 + 0.5f);
		xr.fy += dd / 2;
	}

	pt.fx = middx;
	pt.fy = middy;

	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	//equalgon
	for (i = y_ruler; i > 0; i--)
	{
		lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

		xn.fr = dd * i;
		(*pif->pf_draw_equalgon)(pif->ctx, &xp_dot, &xb_dot, &pt, &xn, y_count);
	}

	//vectex labels
	xn.fr = dd * y_ruler;
	pa = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)* (y_count + 1));
	(*pif->pf_calc_equalgon)(pif->ctx, &pt, &xn, y_count, pa);


	for (i = 0; i < y_count; i++)
	{
		(*pif->pf_draw_line)(pif->ctx, &xp, &pt, &(pa[i]));

		label = get_string_ptr(plt->y_labels, i);

		(*pif->pf_text_size)(pif->ctx, &xf_dot, label, -1, &xs);

		qa = ft_quadrant(&pt, &pa[i], 0);
		switch (qa)
		{
		case 1:
			pk.fx = pa[i].fx;
			pk.fy = pa[i].fy - 2 * xs.fh;
			break;
		case 2:
			pk.fx = pa[i].fx - xs.fw;
			pk.fy = pa[i].fy - 2 * xs.fh;
			break;
		case 3:
			pk.fx = pa[i].fx - xs.fw;
			pk.fy = pa[i].fy + xs.fh;
			break;
		case 4:
			pk.fx = pa[i].fx;
			pk.fy = pa[i].fy + xs.fh;
			break;
		}

		(*pif->pf_text_out)(pif->ctx, &xf_dot, &pk, label, -1);

		for (j = 1; j <= y_ruler; j++)
		{
			fr = (float)j / (float)y_ruler;
			dx = (pa[i].fx - pt.fx) * fr;
			dy = (pa[i].fy - pt.fy) * fr;
			pk.fx = pt.fx + dx;
			pk.fy = pt.fy + dy;

			y_base = get_numeric(plt->y_bases, i);
			y_step = get_numeric(plt->y_steps, i);

			if (j == y_ruler)
			{
				if (IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step))
					xsprintf(numstr, _T("%.f"), y_base + y_step * j);
				else
					xscpy(numstr, _T(""));

				(*pif->pf_text_size)(pif->ctx, &xf_dot, numstr, -1, &xs);

				switch (qa)
				{
				case 1:
					pk.fy -= xs.fh;
					break;
				case 2:
					pk.fx -= xs.fw;
					pk.fy -= xs.fh;
					break;
				case 3:
					pk.fx -= xs.fw;
					break;
				case 4:
					break;
				}

				(*pif->pf_text_out)(pif->ctx, &xf_dot, &pk, numstr, -1);
			}
		}
	}

	//radar line
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);
	for (i = 0; i < pmt->rows; i++)
	{
		color = get_string_ptr(plt->x_colors, i);
		if (!is_null(color))
		{
			xscpy(xp_dot.color, color);
			xscpy(xf_dot.color, color);
			xscpy(xb_dot.color, color);
			xscpy(xb_dot.opacity, _T("50"));
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			xscpy(xf_dot.color, xf.color);
			xscpy(xb_dot.color, xb.color);
			xscpy(xb_dot.opacity, _T("50"));
		}

		xn.fr = dd * y_ruler;
		(*pif->pf_calc_equalgon)(pif->ctx, &pt, &xn, y_count, pa);

		for (j = 0; j < pmt->cols; j++)
		{
			y_base = get_numeric(plt->y_bases, j);
			y_step = get_numeric(plt->y_steps, j);
			dbl = matrix_get_value(pmt, i, j);

			if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
				fr = (float)((dbl - y_base) / (y_step * y_ruler));
			else
				fr = 0;

			if (fr < 0)
				fr = 0;

			dx = (pa[j].fx - pt.fx) * fr;
			dy = (pa[j].fy - pt.fy) * fr;

			pa[j].fx = pt.fx + dx;
			pa[j].fy = pt.fy + dy;

			if (IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step))
				xsprintf(numstr, _T("%.f"), dbl);
			else
				xscpy(numstr, _T(""));

			(*pif->pf_text_size)(pif->ctx, &xf_dot, numstr, -1, &xs);

			qa = ft_quadrant(&pt, &pa[j], 0);

			switch (qa)
			{
			case 1:
				pk.fx = pa[j].fx;
				pk.fy = pa[j].fy - xs.fh;
				break;
			case 2:
				pk.fx = pa[j].fx - xs.fw;
				pk.fy = pa[j].fy - xs.fh;
				break;
			case 3:
				pk.fx = pa[j].fx - xs.fw;
				pk.fy = pa[j].fy;
				break;
			case 4:
				pk.fx = pa[j].fx;
				pk.fy = pa[j].fy;
				break;
			}

			(*pif->pf_text_out)(pif->ctx, &xf_dot, &pk, numstr, -1);
		}

		pa[j].fx = pa[0].fx;
		pa[j].fy = pa[0].fy;

		(*pif->pf_draw_polygon)(pif->ctx, &xp_dot, &xb_dot, pa, j + 1);
	}

	xmem_free(pa);
}

static void _plot_fuelgram(const if_drawing_t* pif, const plot_t* plt, matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt;
	xpoint_t pa[4];
	xrect_t xr;
	xsize_t xs;

	int i, j, k;
	float dd, rr;
	float middx, middy;
	double dr, fang, tang;

	int y_ruler;
	int y_count;

	const tchar_t* color;
	const tchar_t* label;
	tchar_t numstr[NUM_LEN + 1] = { 0 };

	const canvbox_t* pbox = (canvbox_t*)(&pif->rect);

	double dbl;
	double y_base, y_step;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	xsprintf(xf.size, _T("%s"), GDI_ATTR_FONT_SIZE_FOOTER);
	parse_xfont_from_style(&xf, plt->style);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);

	default_xface(&xa_dot);

	default_xfont(&xf_dot);
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 3);

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, xp.color);

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, xp.color);

	y_count = pmt->cols;
	if (!y_count)
		y_count = 1;

	rr = (pbox->fw > pbox->fh) ? (pbox->fw / 2) : (pbox->fh / 2);

	//ruler count
	y_ruler = 10;

	//ruler width
	dd = (2 * rr) / (y_ruler + 2);
	if (dd < 5.0f)
		dd = 5.0f;

	//arc unit
	dr = XPI / 5;

	middx = pbox->fx + pbox->fw / 2;
	middy = pbox->fy + pbox->fh / 2 + dd;

	pt.fx = middx;
	pt.fy = middy;

	//title at bottom
	xr.fx = middx - rr + dd;
	xr.fw = 2 * (rr - dd);
	xr.fy = middy - rr + dd;
	xr.fh = 2 * (rr - dd);

	xs.fw = xr.fw / 2;
	xs.fh = rr - dd;
	fang = XPI / 2;
	(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, fang, pa, 1);

	xr.fx = middx - rr;
	xr.fy = middy - rr;
	xr.fw = 2 * rr;
	xr.fh = 2 * rr;

	//fuel
	for (i = 0; i < 1; i++)
	{
		ft_expand_rect(&xr, -dd, -dd);

		for (j = 0; j < 7; j++)
		{
			color = get_string_ptr(plt->y_colors, j);
			if (!is_null(color))
			{
				xscpy(xb_dot.color, color);
			}
			else
			{
				xscpy(xp_dot.color, xp.color);
			}

			xs.fw = xr.fw / 2;
			xs.fh = dd;
			fang = 4 * XPI / 5 + j * dr;
			tang = fang + dr;
			(*pif->pf_draw_fan)(pif->ctx, &xp, &xb_dot, &pt, &xs, fang, tang);

			for (k = 0; k < 5; k++)
			{
				if (k < 4)
				{
					xs.fw = xr.fw / 2;
					xs.fh = dd / 2;
					(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					xsprintf(xp_dot.size, _T("%d"), (int)(dd / 5));

					(*pif->pf_draw_line)(pif->ctx, &xp_dot, &(pa[1]), &(pa[2]));
				}
				else
				{
					if (j < 1)
					{
						xs.fw = xr.fw / 2;
						xs.fh = 5 * dd / 4;
						(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					}
					else if (j < 2)
					{
						xs.fw = xr.fw / 2;
						xs.fh = 3 * dd / 2;
						(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					}
					else if (j < 4)
					{
						xs.fw = xr.fw / 2;
						xs.fh = 7 * dd / 4;
						(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					}
					else
					{
						xs.fw = xr.fw / 2;
						xs.fh = 2 * dd;
						(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					}

					xsprintf(xp_dot.size, _T("%d"), (int)(dd / 5) + 1);
				}
			}

			y_base = get_numeric(plt->y_bases, i);
			y_step = get_numeric(plt->y_steps, i);

			if (IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step))
				xsprintf(numstr, _T("%.f"), y_base + y_step * (j + 1));
			else
				xscpy(numstr, _T(""));
			(*pif->pf_text_out)(pif->ctx, &xf_dot, &(pa[2]), numstr, -1);
		}

		//pointer
		xscpy(xp.color, xb.color);
		lighten_xpen(&xp, DEF_HARD_LIGHTEN);

		ft_expand_rect(&xr, -2 * dd, -2 * dd);

		y_base = get_numeric(plt->y_bases, i);
		y_step = get_numeric(plt->y_steps, i);
		dbl = matrix_get_value(pmt, i, 0);

		xs.fw = xr.fw / 2;
		xs.fh = rr;

		if (IS_VALID_DOUBLE(dbl) && IS_VALID_DOUBLE(y_base) && IS_VALID_DOUBLE(y_step) && !IS_ZERO_DOUBLE(y_step))
			fang = 4 * XPI / 5 + ((dbl - y_base) / y_step) * dr;
		else
			fang = 4 * XPI / 5;

		(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, fang, pa, 1);

		ft_expand_rect(&xr, -2 * dd, -2 * dd);

		if (xr.fw < 6.0f)
		{
			xr.fx = middx - 3.0f;
			xr.fy = middy - 3.0f;
			xr.fw = 6.0f;
			xr.fh = 6.0f;
		}

		xs.fw = xr.fw / 2;
		xs.fh = dd;
		tang = (fang - XPI / 20);
		(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, tang, tang, pa + 1, 1);

		pa[2].fx = pt.fx;
		pa[2].fy = pt.fy;

		xs.fw = xr.fw / 2;
		xs.fh = dd;
		tang = fang + XPI / 20;
		(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, tang, tang, pa + 3, 1);

		(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pa, 4);

		xr.fx = middx - 2.0f;
		xr.fy = middy - 2.0f;
		xr.fw = 4.0f;
		xr.fh = 4.0f;
		(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

		if (IS_VALID_DOUBLE(dbl))
			xsprintf(numstr, _T("%04d"), (int)dbl);
		else
			xscpy(numstr, _T(""));

		for (k = -2; k < 2; k++)
		{
			xr.fx = middx + k * dd / 2;
			xr.fw = dd / 2;
			xr.fy = middy + dd;
			xr.fh = dd / 2;
			//(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

			(*pif->pf_draw_text)(pif->ctx, &xf_dot, &xa, &xr, (numstr + k + 2), 1);
		}

		label = get_string_ptr(plt->y_labels, i);

		xr.fx = middx - dd;
		xr.fw = 2 * dd;
		xr.fy = middy + 2 * dd;
		xr.fh = dd;
		(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, label, -1);
	}
}

static void _plot_topoggram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{

}


static void _plot_contourgram(const if_drawing_t* pif, const plot_t* plt, const matrix_t* pmt)
{

}
/********************************************************************************************************************/

void draw_plot(const if_drawing_t* pif, link_t_ptr ptr)
{
	plot_t po = { 0 };
	int rows, cols;
	matrix_t* pmt = { 0 };
	const tchar_t* type;
	tchar_t* matrix;
	int len;

	type = get_plot_type_ptr(ptr);

	len = get_plot_style(ptr, NULL, MAX_LONG);
	po.style = xsalloc(len + 1);
	get_plot_style(ptr, po.style, len);

	po.ruler = get_plot_ruler(ptr);

	po.y_grades = alloc_numeric_array();
	get_plot_y_grades(ptr, po.y_grades);

	po.y_stages = alloc_string_array();
	get_plot_y_stages(ptr, po.y_stages);

	po.y_bases = alloc_numeric_array();
	get_plot_y_bases(ptr, po.y_bases);

	po.y_steps = alloc_numeric_array();
	get_plot_y_steps(ptr, po.y_steps);

	po.y_labels = alloc_string_array();
	get_plot_y_labels(ptr, po.y_labels);

	po.y_colors = alloc_string_array();
	get_plot_y_colors(ptr, po.y_colors);

	po.y_shapes = alloc_string_array();
	get_plot_y_shapes(ptr, po.y_shapes);

	po.x_labels = alloc_string_array();
	get_plot_x_labels(ptr, po.x_labels);

	po.x_colors = alloc_string_array();
	get_plot_x_colors(ptr, po.x_colors);

	len = get_plot_matrix_data(ptr, NULL, MAX_LONG);
	matrix = xsalloc(len + 1);
	get_plot_matrix_data(ptr, matrix, len);

	rows = get_plot_matrix_rows(ptr);
	if (!rows) rows = 1;

	cols = get_plot_matrix_cols(ptr);
	if (!cols) cols = 1;

	pmt = matrix_alloc(rows, cols);
	matrix_parse(pmt, matrix, -1);
	xsfree(matrix);

	if (compare_text(type, -1, ATTR_PLOT_TYPE_CALENDAR, -1, 1) == 0)
		_plot_calendar(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_INDICATOR, -1, 1) == 0)
		_plot_indicator(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_THERMOMETER, -1, 1) == 0)
		_plot_thermometer(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_CONTRAGRAM, -1, 1) == 0)
		_plot_contragram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_BALANCEGRAM, -1, 1) == 0)
		_plot_balancegram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_BARGRAM, -1, 1) == 0)
		_plot_bargram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_KPIGRAM, -1, 1) == 0)
		_plot_kpigram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_TRENDGRAM, -1, 1) == 0)
		_plot_trendgram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_SCATTERGRAM, -1, 1) == 0)
		_plot_scattergram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_PANTOGRAM, -1, 1) == 0)
		_plot_pantogram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_HISTOGRAM, -1, 1) == 0)
		_plot_histogram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_MEDIANGRAM, -1, 1) == 0)
		_plot_mediangram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_RADARGRAM, -1, 1) == 0)
		_plot_radargram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_FUELGRAM, -1, 1) == 0)
		_plot_fuelgram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_TASKGRAM, -1, 1) == 0)
		_plot_taskgram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_TOPOGGGRAM, -1, 1) == 0)
		_plot_topoggram(pif, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_CONTOURGRAM, -1, 1) == 0)
		_plot_contourgram(pif, &po, pmt);

	matrix_free(pmt);

	xsfree(po.style);
	free_string_array(po.x_colors);
	free_string_array(po.x_labels);
	free_string_array(po.y_stages);
	free_string_array(po.y_colors);
	free_string_array(po.y_labels);
	free_string_array(po.y_shapes);
	free_numeric_array(po.y_bases);
	free_numeric_array(po.y_steps);
	free_numeric_array(po.y_grades);
}

#endif /*XDL_SUPPORT_VIEW*/