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
#include "plotdoc.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdldoc.h"

typedef struct _plot_t{
	const tchar_t* title;
	const tchar_t* style;
	int ruler;

	const tchar_t* y_stages;
	const tchar_t* y_bases;
	const tchar_t* y_steps;
	const tchar_t* y_labels;
	const tchar_t* y_colors;
	const tchar_t* y_shapes;

	const tchar_t* x_labels;
	const tchar_t* x_colors;
}plot_t;

static void _plot_indicator(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xrect_t xr;

	int i, j, n;
	float dx, dy;
	float middy, zerox;

	int y_label_count;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));

	y_label_count = parse_string_token_count(plt->y_labels, -1, _T(','));
	if (!y_label_count)
		y_label_count = 1;

	//vert height
	dy = pbox->fh / 4;
	if (dy > 10.0f)
		dy = 10.0f;
	if (dy < 4.0f)
		dy = 4.0f;

	//horz bar width
	dx = pbox->fw / y_label_count;
	if (dx < 5.0f)
		dx = 5.0f;

	middy = pbox->fy + pbox->fh / 2;
	zerox = pbox->fx;

	//bar
	token = plt->y_labels;
	color = plt->y_colors;

	for (i = 0; i < y_label_count; i++)
	{
		color = parse_string_token(color, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
			xsncpy(xb_dot.color, key, klen);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		token = parse_string_token(token, -1, _T(','), &key, &klen);

		xr.fx = zerox + i * dx;
		xr.fw = dx;
		xr.fy = middy - dy;
		xr.fh = dy;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, key, klen);

		xr.fx = zerox + i * dx + 1.0f;
		xr.fw = dx - 2.0f;
		xr.fy = middy;
		xr.fh = dy;
		(*pif->pf_draw_rect)(pif->canvas, &xp_dot, &xb_dot, &xr);
	}
}

static void _plot_bargram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk, pt1, pt2;
	xrect_t xr;
	xsize_t xs;

	int i, j, n;
	float dd, dx, dy, dr;
	float zeroy, zerox;

	double* y_step;
	int y_step_count;
	double* y_base;
	int y_base_count;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	double dbl, dbr;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));

	y_base_count = parse_string_token_count(plt->y_bases, -1, _T(','));
	if (!y_base_count)
		y_base_count = 1;
	y_base = (double*)xmem_alloc(y_base_count * sizeof(double));
	i = 0;
	token = plt->y_bases;
	while (token && i < y_base_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_base[i] = (klen) ? xsntonum(key, klen) : 0.0f;
		i++;
	}

	y_step_count = parse_string_token_count(plt->y_steps, -1, _T(','));
	if (!y_step_count)
		y_step_count = 1;
	y_step = (double*)xmem_alloc(y_step_count * sizeof(double));
	i = 0;
	token = plt->y_steps;
	while (token && i < y_step_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_step[i] = (klen) ? xsntonum(key, klen) : 1.0f;
		i++;
	}

	//ruler height
	dy = pbox->fh / y_base_count;
	if (dy < 5.0f)
		dy = 5.0f;

	//bar height
	dd = dy / 2;
	dr = dd / 2;
	if (dd > 5.0f)
	{
		dd = 5.0f;
		dr = 2.0f;
	}
	else if (dd > 3.0f)
	{
		dd = 4.0f;
		dr = 1.5f;
	}
	else
	{
		dd = 2.5f;
		dr = 0.75f;
	}

	//horz bar width
	dx = pbox->fw;
	if (dx < 5.0f)
		dx = 5.0f;

	zeroy = pbox->fy;
	zerox = pbox->fx;

	//bar
	token = plt->y_labels;
	color = plt->y_colors;

	for (i = 0; i < pmt->rows; i++)
	{
		color = parse_string_token(color, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
			xsncpy(xb_dot.color, key, klen);
		}
		else
		{
			xscpy(xp_dot.color, xp.color);
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
			xscpy(xb_dot.color, xb.color);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		token = parse_string_token(token, -1, _T(','), &key, &klen);

		xr.fx = zerox + dr + dd;
		xr.fw = dx - 2 * dd - 2 * dr;
		xr.fy = zeroy + i * dy + dy / 2 - dd;
		xr.fh = 2 * dd;

		pk.fx = xr.fx;
		pk.fy = xr.fy - dd;
		(*pif->pf_text_out)(pif->canvas, &xf, &pk, key, klen);

		pt.fx = xr.fx;
		pt.fy = xr.fy + dd;
		(*pif->pf_draw_pie)(pif->canvas, &xp, &xb, &pt, dd, dd, XPI / 2, 3 * XPI / 2);

		(*pif->pf_draw_rect)(pif->canvas, &xp, &xb, &xr);

		pt.fx = xr.fx + xr.fw;
		pt.fy = xr.fy + dd;
		(*pif->pf_draw_pie)(pif->canvas, &xp, &xb, &pt, dd, dd, 3 * XPI / 2, 5 * XPI / 2);

		dbl = matrix_get_value(pmt, i, 0);
		if (y_step[i] == 0)
			dbr = 0;
		else
			dbr = ((dbl - y_base[i]) / y_step[i]);

		ft_expand_rect(&xr, -dr, -dr);
		xr.fw *= dbr;

		pt.fx = xr.fx;
		pt.fy = xr.fy + dd - dr;
		(*pif->pf_draw_pie)(pif->canvas, &xp_dot, &xb_dot, &pt, dd - dr, dd - dr, XPI / 2, 3 * XPI / 2);

		(*pif->pf_draw_rect)(pif->canvas, &xp_dot, &xb_dot, &xr);

		pt.fx = xr.fx + xr.fw;
		pt.fy = xr.fy + dd - dr;
		(*pif->pf_draw_pie)(pif->canvas, &xp_dot, &xb_dot, &pt, dd - dr, dd - dr, 3 * XPI / 2, 5 * XPI / 2);

		xsprintf(numstr, _T("%.2f%"), dbr * 100);
		xr.fx += (xr.fw + dd);
		xr.fw = dx - xr.fw;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);
	}

	xmem_free(y_step);
	xmem_free(y_base);
}

static void _plot_histogram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk, pt1, pt2;
	xrect_t xr;

	int i, j, n;
	float dd, dx, cx, dy, fr;
	float middy, zeroy, zerox;

	double* y_step;
	int y_step_count;
	double* y_base;
	int y_base_count;

	int x_label_count;
	int x_ruler_count;
	int y_label_count;
	int y_ruler_count;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	double dbl;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

	y_base_count = parse_string_token_count(plt->y_bases, -1, _T(','));
	if (!y_base_count)
		y_base_count = 1;
	y_base = (double*)xmem_alloc(y_base_count * sizeof(double));
	i = 0;
	token = plt->y_bases;
	while (token && i < y_base_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_base[i] = (klen) ? xsntonum(key, klen) : 0.0f;
		i++;
	}

	y_step_count = parse_string_token_count(plt->y_steps, -1, _T(','));
	if (!y_step_count)
		y_step_count = 1;
	y_step = (double*)xmem_alloc(y_step_count * sizeof(double));
	i = 0;
	token = plt->y_steps;
	while (token && i < y_step_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_step[i] = (klen) ? xsntonum(key, klen) : 1.0f;
		i++;
	}

	y_label_count = parse_string_token_count(plt->y_labels, -1, _T(','));
	if (!y_label_count)
		y_label_count = 1;

	x_label_count = parse_string_token_count(plt->x_labels, -1, _T(','));
	if (!x_label_count)
		x_label_count = 5;

	//y-ruler count
	y_ruler_count = 10;
	x_ruler_count = (x_label_count > 10) ? x_label_count : 5;

	//y-label width
	dd = 10.0f;

	//y-ruler height
	dy = pbox->fh / (y_ruler_count + 2);
	if (dy < 5.0f)
		dy = 5.0f;

	//x-ruler width
	dx = (pbox->fw - dd * y_label_count) / (x_ruler_count + 2);
	if (dx < 10.0f)
		dx = 10.0f;

	cx = dx / y_label_count;
	if (cx < 2.0f)
		cx = 2.0f;

	middy = pbox->fy + pbox->fh / 2;
	zeroy = pbox->fy + pbox->fh - dy;
	zerox = pbox->fx + dd * y_label_count;

	//title
	xr.fx = zerox;
	xr.fw = x_ruler_count * dx;
	xr.fy = zeroy - (y_ruler_count + 1) * dy;
	xr.fh = dy;
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, plt->title, -1);

	//y-axis
	pt1.fx = zerox;
	pt1.fy = zeroy - y_ruler_count * dy;
	pt2.fx = zerox;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//y ruler and labels
	token = plt->y_labels;

	for (i = 0; i<y_base_count; i++)
	{
		//y labels
		token = parse_string_token(token, -1, _T(','), &key, &klen);

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - y_ruler_count * dy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, key, klen);
		
		dbl = y_base[i];
		for (j = y_ruler_count / 2 + 1; j < y_ruler_count; j++)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}

			dbl += y_step[i];

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 2;
			xr.fh = dd / 2;
			xsprintf(numstr, _T("%.2f"), dbl);

			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);
		}

		dbl = y_base[i];
		for (j = y_ruler_count / 2; j > 0; j--)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 2;
			xr.fh = dd / 2;
			xsprintf(numstr, _T("%.2f"), dbl);
			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);

			dbl -= y_step[i];
		}
	}

	//vert line color
	token = plt->y_colors;

	for (i = 0; i < y_base_count; i++)
	{
		//y colors
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
			xsncpy(xb_dot.color, key, klen);
		}

		xr.fx = zerox - (i + 1) * dd + 0.5f;
		xr.fw = dy;
		xr.fy = zeroy - dy;
		xr.fh = dy;
		ft_center_rect(&xr, 3, 3);
		(*pif->pf_draw_rect)(pif->canvas, &xp_dot, &xb_dot, &xr);
	}

	//x axis
	pt1.fx = zerox;
	pt1.fy = zeroy;
	pt2.fx = zerox + x_ruler_count * dx;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//x middle axis
	pt1.fx = zerox;
	pt1.fy = middy;
	pt2.fx = zerox + x_ruler_count * dx;
	pt2.fy = middy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//x ruler and labels
	token = plt->x_labels;
	for (i = 1; i <= x_ruler_count; i++)
	{
		if (i < x_ruler_count)
		{
			pt1.fx = zerox + i * dx;
			pt1.fy = zeroy;
			pt2.fx = zerox + i * dx;
			pt2.fy = zeroy + 2.0f;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
		}

		token = parse_string_token(token, -1, _T(','), &key, &klen);
		xr.fx = zerox + (i - 1) * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa, &xr, key, klen);
	}

	//histo rect
	for (i = 0; i < pmt->rows; i++)
	{
		color = plt->y_colors;

		for (j = 0; j < pmt->cols; j++)
		{
			color = parse_string_token(color, -1, _T(','), &key, &klen);
			if (klen)
			{
				xsncpy(xp_dot.color, key, klen);
				xsncpy(xb_dot.color, key, klen);
			}

			dbl = matrix_get_value(pmt, i, j);

			fr = (dbl - y_base[j]) / (y_step[j]);

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

			(*pif->pf_draw_rect)(pif->canvas, &xp_dot, &xb_dot, &xr);

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

			xsprintf(numstr, _T("%f"), dbl);
			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa, &xr, numstr, -1);
		}
	}

	xmem_free(y_step);
	xmem_free(y_base);
}


static void _plot_scattergram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk, pt1, pt2;
	xrect_t xr;
	xsize_t xs;

	int i, j, n;
	float dd, dx, dy, fr;
	float middy, zeroy, zerox;

	double* y_step;
	int y_step_count;
	double* y_base;
	int y_base_count;

	int x_label_count;
	int x_ruler_count;
	int y_label_count;
	int y_ruler_count;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	double dbl;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

	y_base_count = parse_string_token_count(plt->y_bases, -1, _T(','));
	if (!y_base_count)
		y_base_count = 1;
	y_base = (double*)xmem_alloc(y_base_count * sizeof(double));
	i = 0;
	token = plt->y_bases;
	while (token && i < y_base_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_base[i] = (klen) ? xsntonum(key, klen) : 0.0f;
		i++;
	}

	y_step_count = parse_string_token_count(plt->y_steps, -1, _T(','));
	if (!y_step_count)
		y_step_count = 1;
	y_step = (double*)xmem_alloc(y_step_count * sizeof(double));
	i = 0;
	token = plt->y_steps;
	while (token && i < y_step_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_step[i] = (klen) ? xsntonum(key, klen) : 1.0f;
		i++;
	}

	y_label_count = parse_string_token_count(plt->y_labels, -1, _T(','));
	if (!y_label_count)
		y_label_count = 1;

	x_label_count = parse_string_token_count(plt->x_labels, -1, _T(','));
	if (!x_label_count)
		x_label_count = 5;

	//y-ruler count
	y_ruler_count = 10;
	x_ruler_count = (x_label_count > 10) ? x_label_count : 5;

	//y-label width
	dd = 10.0f;

	//y-ruler height
	dy = pbox->fh / (y_ruler_count + 2);
	if (dy < 5.0f)
		dy = 5.0f;

	//x-ruler width
	dx = (pbox->fw - dd * y_label_count) / (x_ruler_count + 2);
	if (dx < 10.0f)
		dx = 10.0f;

	middy = pbox->fy + pbox->fh / 2;
	zeroy = pbox->fy + pbox->fh - dy;
	zerox = pbox->fx + dd * y_label_count;

	//title
	xr.fx = zerox;
	xr.fw = x_ruler_count * dx;
	xr.fy = zeroy - (y_ruler_count + 1) * dy;
	xr.fh = dy;
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, plt->title, -1);

	//y-axis
	pt1.fx = zerox;
	pt1.fy = zeroy - y_ruler_count * dy;
	pt2.fx = zerox;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//y ruler and labels
	token = plt->y_labels;

	for (i = 0; i<y_base_count; i++)
	{
		//y labels
		token = parse_string_token(token, -1, _T(','), &key, &klen);

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - y_ruler_count * dy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, key, klen);

		dbl = y_base[i];
		for (j = y_ruler_count / 2 + 1; j < y_ruler_count; j++)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}

			dbl += y_step[i];

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 2;
			xr.fh = dd / 2;
			xsprintf(numstr, _T("%.2f"), dbl);

			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);
		}

		dbl = y_base[i];
		for (j = y_ruler_count / 2; j > 0; j--)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 2;
			xr.fh = dd / 2;
			xsprintf(numstr, _T("%.2f"), dbl);
			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);

			dbl -= y_step[i];
		}
	}

	//vert line color
	token = plt->y_colors;

	for (i = 0; i < y_base_count; i++)
	{
		//y colors
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
			xsncpy(xb_dot.color, key, klen);
		}

		xr.fx = zerox - (i + 1) * dd + 0.5f;
		xr.fw = dy;
		xr.fy = zeroy - dy;
		xr.fh = dy;
		ft_center_rect(&xr, 3, 3);
		(*pif->pf_draw_rect)(pif->canvas, &xp_dot, &xb_dot, &xr);
	}

	//x axis
	pt1.fx = zerox;
	pt1.fy = zeroy;
	pt2.fx = zerox + x_ruler_count * dx;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//x middle axis
	pt1.fx = zerox;
	pt1.fy = middy;
	pt2.fx = zerox + x_ruler_count * dx;
	pt2.fy = middy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//x ruler and labels
	token = plt->x_labels;
	for (i = 1; i <= x_ruler_count; i++)
	{
		if (i < x_ruler_count)
		{
			pt1.fx = zerox + i * dx;
			pt1.fy = zeroy;
			pt2.fx = zerox + i * dx;
			pt2.fy = zeroy + 2.0f;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
		}

		token = parse_string_token(token, -1, _T(','), &key, &klen);
		xr.fx = zerox + (i - 1) * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa, &xr, key, klen);
	}

	//scatter dot
	token = plt->y_colors;

	for (i = 0; i < pmt->cols; i++)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xb_dot.color, key, klen);
		}

		for (j = 0; j < pmt->rows; j++)
		{
			dbl = matrix_get_value(pmt, j, i);

			pt1.fx = zerox + (j + 1) * dx - dx / 2;
			pt1.fy = middy - (float)((dbl - y_base[i]) / y_step[i]) * dy;

			xr.fx = pt1.fx - 1.0f;
			xr.fy = pt1.fy - 1.0f;
			xr.fw = 2.0f;
			xr.fh = 2.0f;
			(*pif->pf_draw_ellipse)(pif->canvas, NULL, &xb_dot, &xr);
		}
	}

	xmem_free(y_step);
	xmem_free(y_base);
}

static void _plot_pantogram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pa[4] = { 0 };
	xrect_t xr;

	int i, j, n;
	float dd, rr, fr;
	float middy, middx, zerox, zeroy;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	double nxtd, pred;
	double dbl;
	double total = 0.0;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

	rr = (pbox->fw < pbox->fh) ? (pbox->fw / 2) : (pbox->fh / 2);
	middx = pbox->fx + pbox->fw / 2;
	middy = pbox->fy + pbox->fh / 2;

	//ruler span
	dd = rr / 12;
	if (dd < 4.0f)
		dd = 4.0f;

	//title
	xr.fx = middx - rr;
	xr.fw = rr;
	xr.fy = middy - rr;
	xr.fh = dd;
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, plt->title, -1);

	pt.fx = middx;
	pt.fy = middy;

	for (i = 0; i < pmt->rows; i ++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			total += matrix_get_value(pmt, i, j);
		}
	}

	nxtd = 0.0;
	for (i = 0; i < pmt->rows; i++)
	{
		token = plt->x_labels;
		color = plt->x_colors;

		for (j = 0; j < pmt->cols; j++)
		{
			color = parse_string_token(color, -1, _T(','), &key, &klen);
			if (klen)
				xsncpy(xb_dot.color, key, klen);
			else
				lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

			pred = nxtd;

			dbl = matrix_get_value(pmt, i, j);

			if ((int)total == 0)
				fr = 0.0f;
			else
				fr = (float)(dbl / total);

			nxtd = pred + (float)(2 * XPI *  fr);
			if (pred != nxtd)
			{
				(*pif->pf_draw_pie)(pif->canvas, &xp_dot, &xb_dot, &pt, rr - 2 * dd, rr - 2 * dd, pred, nxtd);

				(*pif->pf_calc_fan)(pif->canvas, &pt, rr - dd, dd, (pred + nxtd) / 2, (pred + nxtd) / 2, pa, 4);

				(*pif->pf_draw_line)(pif->canvas, &xp_dot, &pa[0], &pa[2]);

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

				(*pif->pf_draw_line)(pif->canvas, &xp_dot, &pa[0], &pa[1]);

				xr.fx = pa[2].fx;
				xr.fw = 2 * dd;
				xr.fy = pa[2].fy;
				xr.fh = dd;
				xsprintf(numstr, _T("%.2f%"), fr * 100);
				(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);

				xr.fx = pa[3].fx;
				xr.fw = 2 * dd;
				xr.fy = pa[3].fy;
				xr.fh = dd;
				token = parse_string_token(token, -1, _T(','), &key, &klen);
				(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, key, klen);
			}
		}
	}
}

static void _plot_trendgram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk, pt1, pt2;
	xrect_t xr;

	int i, j, n;
	float dd, dx, dy, fr;
	float miny, maxy, middy, zeroy, zerox;

	double* y_step;
	int y_step_count;
	double* y_base;
	int y_base_count;
	double* y_stage;
	int y_stage_count;

	int x_label_count;
	int x_ruler_count;
	int y_label_count;
	int y_ruler_count;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* shape;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	double dbl;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	default_xbrush(&xb_dot);

	y_stage_count = parse_string_token_count(plt->y_stages, -1, _T(','));
	if (!y_stage_count)
		y_stage_count = 1;
	y_stage = (double*)xmem_alloc(y_stage_count * sizeof(double));
	i = 0;
	token = plt->y_stages;
	while (token && i < y_stage_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_stage[i] = (klen) ? xsntonum(key, klen) : 0.0f;
		i++;
	}

	y_base_count = parse_string_token_count(plt->y_bases, -1, _T(','));
	if (!y_base_count)
		y_base_count = 1;
	y_base = (double*)xmem_alloc(y_base_count * sizeof(double));
	i = 0;
	token = plt->y_bases;
	while (token && i < y_base_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_base[i] = (klen) ? xsntonum(key, klen) : 0.0f;
		i++;
	}

	y_step_count = parse_string_token_count(plt->y_steps, -1, _T(','));
	if (!y_step_count)
		y_step_count = 1;
	y_step = (double*)xmem_alloc(y_step_count * sizeof(double));
	i = 0;
	token = plt->y_steps;
	while (token && i < y_step_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_step[i] = (klen) ? xsntonum(key, klen) : 1.0f;
		i++;
	}

	y_label_count = parse_string_token_count(plt->y_labels, -1, _T(','));

	x_label_count = parse_string_token_count(plt->x_labels, -1, _T(','));

	//ruler count
	y_ruler_count = (plt->ruler) ? plt->ruler : 10;
	x_ruler_count = (x_label_count > 7) ? x_label_count : 7;

	//ruler height
	dy = pbox->fh / (y_ruler_count + 2);
	if (dy < 5.0f)
		dy = 5.0f;

	//ruler width
	dd = dy;
	if (dd > 10.0f)
		dd = 10.0f;
	if (dd < 8.0f)
		dd = 8.0f;

	//horz label width
	dx = (pbox->fw - dd * y_label_count) / (x_ruler_count);
	if (dx < 10.0f)
		dx = 10.0f;

	middy = pbox->fy + pbox->fh / 2;
	zeroy = pbox->fy + pbox->fh - dy;
	zerox = pbox->fx + dd * y_base_count;

	//title at bottom
	xr.fx = zerox;
	xr.fw = x_ruler_count * dx;
	xr.fy = zeroy + dy / 2;
	xr.fh = dy / 2;
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, plt->title, -1);

	//vert axis
	pt1.fx = zerox;
	pt1.fy = zeroy - y_ruler_count * dy;
	pt2.fx = zerox;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//vert ruler and labels
	token = plt->y_labels;

	for (i = 0; i<y_base_count; i++)
	{
		//y labels
		token = parse_string_token(token, -1, _T(','), &key, &klen);

		xr.fx = zerox - (i + 1) * dd;
		xr.fw = dd;
		xr.fy = zeroy - y_ruler_count * dy - dy / 2;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, key, klen);

		dbl = y_base[i];
		for (j = y_ruler_count / 2 + 1; j < y_ruler_count; j++)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}

			dbl += y_step[i];

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy -dd / 2;
			xr.fh = dd / 2;
			xsprintf(numstr, _T("%.2f"), dbl);
			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);
		}

		dbl = y_base[i];
		for (j = y_ruler_count / 2; j > 0; j--)
		{
			if (!i)
			{
				pt1.fx = zerox - 2.0f;
				pt1.fy = zeroy - j * dy;
				pt2.fx = zerox;
				pt2.fy = zeroy - j * dy;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}

			xr.fx = zerox - (i + 1) * dd;
			xr.fw = dd;
			xr.fy = zeroy - j * dy - dd / 2;
			xr.fh = dd / 2;
			xsprintf(numstr, _T("%.2f"), dbl);
			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);

			dbl -= y_step[i];
		}
	}

	//vert line color
	shape = plt->y_shapes;
	token = plt->y_colors;
	default_xpen(&xp_dot);
	default_xbrush(&xb_dot);
	for (i = 0; i < y_base_count; i++)
	{
		//y colors
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
			xsncpy(xb_dot.color, key, klen);
		}
		else
		{
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		xr.fx = zerox - (i + 1) * dd + 0.5f;
		xr.fw = dy;
		xr.fy = zeroy - dy;
		xr.fh = dy;
		ft_center_rect(&xr, 2, 2);

		shape = parse_string_token(shape, -1, _T(','), &key, &klen);
		if (compare_text(key, klen, _T("ellipse"),-1,1) == 0)
			(*pif->pf_draw_ellipse)(pif->canvas, &xp_dot, &xb_dot, &xr);
		else if (compare_text(key, klen, _T("rect"), -1, 1) == 0)
			(*pif->pf_draw_rect)(pif->canvas, &xp_dot, &xb_dot, &xr);
		else if (compare_text(key, klen, _T("round"), -1, 1) == 0)
			(*pif->pf_draw_round)(pif->canvas, &xp_dot, &xb_dot, &xr);
		else if (compare_text(key, klen, _T("left-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_LEFT);
		else if (compare_text(key, klen, _T("right-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_RIGHT);
		else if (compare_text(key, klen, _T("top-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_TOP);
		else if (compare_text(key, klen, _T("bottom-triangle"), -1, 1) == 0)
			(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_BOTTOM);
		else
			(*pif->pf_draw_ellipse)(pif->canvas, &xp_dot, &xb_dot, &xr);
	}

	//horz axis
	pt1.fx = zerox;
	pt1.fy = zeroy;
	pt2.fx = zerox + x_ruler_count * dx;
	pt2.fy = zeroy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//horz middle axis
	pt1.fx = zerox;
	pt1.fy = middy;
	pt2.fx = zerox + x_ruler_count * dx;
	pt2.fy = middy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//horz ruler and area
	miny = zeroy;
	maxy = zeroy - y_ruler_count * dy;

	for (i = 0; i < y_stage_count; i++)
	{
		pt1.fx = zerox;
		pt1.fy = middy - (float)((y_stage[i] - y_base[i]) / y_step[i]) * dy;
		pt2.fx = zerox + x_ruler_count * dx;
		pt2.fy = pt1.fy;
		(*pif->pf_draw_line)(pif->canvas, &xp_dot, &pt1, &pt2);

		if (miny > pt1.fy)
			miny = pt1.fy;
		if (maxy < pt1.fy)
			maxy = pt1.fy;
	}

	token = plt->x_labels;
	color = plt->x_colors;
	xscpy(xb.opacity, _T("128"));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	for (i = 1; i <= x_ruler_count; i++)
	{
		color = parse_string_token(color, -1, _T(','), &key, &klen);
		if (klen && maxy > miny)
		{
			xsncpy(xb.color, key, klen);

			xr.fx = zerox + (i - 1) * dx;
			xr.fw = dx;
			xr.fy = miny;
			xr.fh = maxy - miny;
			(*pif->pf_draw_rect)(pif->canvas, NULL, &xb, &xr);
		}

		if (i < x_ruler_count)
		{
			pt1.fx = zerox + i * dx;
			pt1.fy = zeroy;
			pt2.fx = zerox + i * dx;
			pt2.fy = zeroy + 2.0f;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
		}

		token = parse_string_token(token, -1, _T(','), &key, &klen);
		xr.fx = zerox + (i - 1) * dx;
		xr.fw = dx;
		xr.fy = zeroy;
		xr.fh = dy / 2;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, key, klen);
	}

	//horz stage axis
	miny = zeroy;
	maxy = zeroy - y_ruler_count * dy;

	default_xpen(&xp_dot);
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_FAR);
	color = plt->y_colors;
	token = plt->y_labels;
	for (i = 0; i < y_stage_count; i++)
	{
		color = parse_string_token(color, -1, _T(','), &key, &klen);
		if (klen)
			xsncpy(xp_dot.color, key, klen);
		else
			xscpy(xp_dot.color, xp.color);

		pt1.fx = zerox;
		pt1.fy = middy - (float)((y_stage[i] - y_base[i]) / y_step[i]) * dy;
		pt2.fx = zerox + x_ruler_count * dx;
		pt2.fy = pt1.fy;
		(*pif->pf_draw_line)(pif->canvas, &xp_dot, &pt1, &pt2);

		if (miny > pt1.fy)
			miny = pt1.fy;
		if (maxy < pt1.fy)
			maxy = pt1.fy;

		xr.fx = pt2.fx - 2 * dx;
		xr.fw = 2 * dx;
		xr.fy = pt2.fy - dy / 2;
		xr.fh = dy / 2;
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(numstr, key, klen);
			xsprintf((numstr + klen), _T(" %f"), y_stage[i]);
			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, numstr, -1);
		}
	}


	//trend lines
	token = plt->y_colors;
	shape = plt->y_shapes;
	default_xpen(&xp_dot);
	default_xbrush(&xb_dot);
	for (i = 0; i < pmt->cols; i++)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
			xsncpy(xb_dot.color, key, klen);
		}
		else
		{
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
			lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);
		}

		shape = parse_string_token(shape, -1, _T(','), &key, &klen);

		for (j = 0; j < pmt->rows; j++)
		{
			if (j)
			{
				pt2.fx = pt1.fx;
				pt2.fy = pt1.fy;
			}

			dbl = matrix_get_value(pmt, j, i);

			pt1.fx = zerox + (j + 1) * dx - dx / 2;
			pt1.fy = middy - (float)((dbl - y_base[i]) / y_step[i]) * dy;

			if (j)
			{
				(*pif->pf_draw_line)(pif->canvas, &xp_dot, &pt1, &pt2);
			}

			xr.fx = pt1.fx - 1.0f;
			xr.fy = pt1.fy - 1.0f;
			xr.fw = 2.0f;
			xr.fh = 2.0f;

			if (compare_text(key, klen, _T("ellipse"), -1, 1) == 0)
				(*pif->pf_draw_ellipse)(pif->canvas, &xp_dot, &xb_dot, &xr);
			else if (compare_text(key, klen, _T("rect"), -1, 1) == 0)
				(*pif->pf_draw_rect)(pif->canvas, &xp_dot, &xb_dot, &xr);
			else if (compare_text(key, klen, _T("round"), -1, 1) == 0)
				(*pif->pf_draw_round)(pif->canvas, &xp_dot, &xb_dot, &xr);
			else if (compare_text(key, klen, _T("left-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_LEFT);
			else if (compare_text(key, klen, _T("right-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_RIGHT);
			else if (compare_text(key, klen, _T("top-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_TOP);
			else if (compare_text(key, klen, _T("bottom-triangle"), -1, 1) == 0)
				(*pif->pf_draw_triangle)(pif->canvas, &xp_dot, &xb_dot, &xr, ATTR_ORITATION_BOTTOM);
			else
				(*pif->pf_draw_ellipse)(pif->canvas, &xp_dot, &xb_dot, &xr);
		}
	}

	xmem_free(y_step);
	xmem_free(y_base);
	xmem_free(y_stage);
}

static void _plot_radargram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk;
	xpoint_t* pa;
	xrect_t xr;

	int i, j, n;
	float rr, dd, dx, dy, fr;
	float middx, middy;

	double* y_step;
	int y_step_count;
	double* y_base;
	int y_base_count;
	int y_ruler_count;
	int y_label_count;
	int x_label_count;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	double dbl;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa_dot.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

	y_base_count = parse_string_token_count(plt->y_bases, -1, _T(','));
	if (!y_base_count)
		y_base_count = 5;
	y_base = (double*)xmem_alloc(y_base_count * sizeof(double));
	i = 0;
	token = plt->y_bases;
	while (token && i < y_base_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_base[i] = (klen) ? xsntonum(key, klen) : 0.0f;
		i++;
	}

	y_step_count = parse_string_token_count(plt->y_steps, -1, _T(','));
	if (!y_step_count)
		y_step_count = 5;
	y_step = (double*)xmem_alloc(y_step_count * sizeof(double));
	i = 0;
	token = plt->y_steps;
	while (token && i < y_step_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_step[i] = (klen) ? xsntonum(key, klen) : 1.0f;
		i++;
	}

	y_label_count = parse_string_token_count(plt->y_labels, -1, _T(','));
	if (!y_label_count)
		y_label_count = 5;

	x_label_count = parse_string_token_count(plt->x_labels, -1, _T(','));
	
	//ruler count
	y_ruler_count = (plt->ruler) ? plt->ruler : 5;

	rr = (pbox->fw < pbox->fh) ? (pbox->fw / 2) : (pbox->fh / 2);
	middx = pbox->fx + pbox->fw / 2;
	middy = pbox->fy + pbox->fh / 2;

	//ruler span
	dd = rr / (y_ruler_count + 1);
	if (dd < 2.0f)
		dd = 2.0f;

	//title at bottom
	xr.fx = middx - dd * y_ruler_count;
	xr.fw = 2 * dd * y_ruler_count;
	xr.fy = middy + dd * (y_ruler_count);
	xr.fh = dd;
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, plt->title, -1);

	//x labels
	token = plt->x_labels;
	color = plt->x_colors;
	for (i = 0; i < x_label_count; i++)
	{
		color = parse_string_token(color, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
		}
		else
		{
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}

		token = parse_string_token(token, -1, _T(','), &key, &klen);

		xr.fx = middx - dd * (y_ruler_count + 1);
		xr.fy = middy - dd * (y_ruler_count + 1) + (i + 1) * (dd / 2);
		pk.fx = xr.fx + dd / 2;
		pk.fy = xr.fy;
		(*pif->pf_draw_line)(pif->canvas, &xp_dot, RECTPOINT(&xr), &pk);

		xr.fx += (dd / 2 + 0.5f);
		xr.fy -= dd / 2;
		(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa_dot, &xr, key, klen);
		xr.fx -= (dd / 2 + 0.5f);
		xr.fy += dd / 2;
	}

	pt.fx = middx;
	pt.fy = middy;

	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	//equalgon
	for (i = y_ruler_count; i > 0; i--)
	{
		lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

		(*pif->pf_draw_equalgon)(pif->canvas, &xp_dot, &xb_dot, &pt, dd * i, y_label_count);
	}

	//vectex labels
	pa = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)* (y_label_count + 1));
	(*pif->pf_calc_equalgon)(pif->canvas, &pt, dd * y_ruler_count, y_label_count, pa);

	token = plt->y_labels;

	for (i = 0; i < y_label_count; i++)
	{
		(*pif->pf_draw_line)(pif->canvas, &xp, &pt, &(pa[i]));

		token = parse_string_token(token, -1, _T(','), &key, &klen);

		if (pa[i].fx >= pt.fx && pa[i].fy <= pt.fy)
		{
			pk.fx = pa[i].fx;
			pk.fy = pa[i].fy - 5.0f;
		}
		else if (pa[i].fx > pt.fx && pa[i].fy > pt.fy)
		{
			pk.fx = pa[i].fx;
			pk.fy = pa[i].fy + 5.0f;
		}
		else if (pa[i].fx < pt.fx && pa[i].fy > pt.fy)
		{
			pk.fx = pa[i].fx - 5.0f;
			pk.fy = pa[i].fy + 5.0f;
		}
		else
		{
			pk.fx = pa[i].fx - 5.0f;
			pk.fy = pa[i].fy - 5.0f;
		}

		(*pif->pf_text_out)(pif->canvas, &xf_dot, &pk, key, klen);

		for (j = 1; j <= y_ruler_count; j++)
		{
			fr = (float)j / (float)y_ruler_count;
			dx = (pa[i].fx - pt.fx) * fr;
			dy = (pa[i].fy - pt.fy) * fr;
			pk.fx = pt.fx + dx;
			pk.fy = pt.fy + dy;

			if (plt->ruler || j == y_ruler_count)
			{
				xsprintf(numstr, _T("%f"), y_base[i] + y_step[i] * j);
				(*pif->pf_text_out)(pif->canvas, &xf_dot, &pk, numstr, -1);
			}
		}
	}

	//radar line
	color = plt->x_colors;
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);
	for (i = 0; i < pmt->rows; i++)
	{
		(*pif->pf_calc_equalgon)(pif->canvas, &pt, dd * y_ruler_count, y_label_count, pa);

		for (j = 0; j < pmt->cols; j++)
		{
			dbl = matrix_get_value(pmt, i, j);

			if (y_step[j] != 0.0)
				fr = (float)((dbl - y_base[j]) / (y_step[j] * y_ruler_count));
			else
				fr = 0;

			if (fr < 0)
				fr = 0;

			dx = (pa[j].fx - pt.fx) * fr;
			dy = (pa[j].fy - pt.fy) * fr;

			pa[j].fx = pt.fx + dx;
			pa[j].fy = pt.fy + dy;
		}

		pa[j].fx = pa[0].fx;
		pa[j].fy = pa[0].fy;

		color = parse_string_token(color, -1, _T(','), &key, &klen);
		if (klen)
		{
			xsncpy(xp_dot.color, key, klen);
		}
		else
		{
			lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
		}

		(*pif->pf_draw_polyline)(pif->canvas, &xp_dot, pa, j + 1);
	}

	xmem_free(y_step);
	xmem_free(y_base);
	xmem_free(pa);
}

static void _plot_fuelgram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, matrix_t* pmt)
{
	xbrush_t xb, xb_dot;
	xpen_t xp, xp_dot;
	xfont_t xf, xf_dot;
	xface_t xa, xa_dot;

	xpoint_t pt, pk;
	xpoint_t pa[4];
	xrect_t xr;
	xsize_t xs;

	int i, j, k, n;
	float dd, dr, rr;
	float middx, middy;
	double fang, tang;

	double* y_step;
	int y_step_count;
	double* y_base;
	int y_base_count;
	int y_ruler_count;
	int y_label_count;

	const tchar_t* token;
	const tchar_t* color;
	const tchar_t* key;
	int klen;
	tchar_t numstr[NUM_LEN] = { 0 };

	double dbl;

	default_xface(&xa);
	parse_xface_from_style(&xa, plt->style);
	xmem_copy((void*)&xa_dot, (void*)&xa, sizeof(xface_t));
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xfont(&xf);
	parse_xfont_from_style(&xf, plt->style);
	xmem_copy((void*)&xf_dot, (void*)&xf, sizeof(xfont_t));
	xsprintf(xf_dot.size, _T("%d"), xstol(xf.size) - 2);

	default_xpen(&xp);
	parse_xpen_from_style(&xp, plt->style);
	xmem_copy((void*)&xp_dot, (void*)&xp, sizeof(xpen_t));
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, plt->style);
	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_dot, DEF_SOFT_DARKEN);

	y_base_count = parse_string_token_count(plt->y_bases, -1, _T(','));
	if (!y_base_count)
		y_base_count = 1;
	y_base = (double*)xmem_alloc(y_base_count * sizeof(double));
	i = 0;
	token = plt->y_bases;
	while (token && i < y_base_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_base[i] = (klen) ? xsntonum(key, klen) : 0.0f;
		i++;
	}

	y_step_count = parse_string_token_count(plt->y_steps, -1, _T(','));
	if (!y_step_count)
		y_step_count = 1;
	y_step = (double*)xmem_alloc(y_step_count * sizeof(double));
	i = 0;
	token = plt->y_steps;
	while (token && i < y_step_count)
	{
		token = parse_string_token(token, -1, _T(','), &key, &klen);
		y_step[i] = (klen) ? xsntonum(key, klen) : 1.0f;
		i++;
	}

	y_label_count = parse_string_token_count(plt->y_labels, -1, _T(','));
	if (!y_label_count)
		y_label_count = 1;

	rr = (pbox->fw > pbox->fh) ? (pbox->fw / 2) : (pbox->fh / 2);
	middx = pbox->fx + pbox->fw / 2;
	middy = pbox->fy + pbox->fh / 2;

	//ruler count
	y_ruler_count = 10;

	//ruler width
	dd = (2 * rr) / (y_ruler_count + 2);
	if (dd < 5.0f)
		dd = 5.0f;

	//arc unit
	dr = XPI / 5;

	pt.fx = middx;
	pt.fy = middy;

	//title at bottom
	xr.fx = middx - rr + dd;
	xr.fw = 2 * (rr - dd);
	xr.fy = middy - rr + dd;
	xr.fh = 2 * (rr - dd);

	fang = XPI / 2;
	(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, rr - dd, fang, fang, pa, 1);

	xr.fy = pa[0].fy - dd;
	xr.fh = dd;
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, plt->title, -1);

	xr.fx = middx - rr;
	xr.fy = middy - rr;
	xr.fw = 2 * rr;
	xr.fh = 2 * rr;

	//fuel
	for (i = 0; i < 1; i++)
	{
		ft_expand_rect(&xr, -dd, -dd);

		color = plt->y_colors;
		for (j = 0; j < 7; j++)
		{
			color = parse_string_token(color, -1, _T(','), &key, &klen);
			if (klen)
			{
				xsncpy(xb_dot.color, key, klen);
			}

			fang = 4 * XPI / 5 + j * dr;
			tang = fang + dr;
			(*pif->pf_draw_fan)(pif->canvas, &xp, &xb_dot, &pt, xr.fw / 2, dd, fang, tang);

			for (k = 0; k < 5; k++)
			{
				if (k < 4)
				{
					(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, dd / 2, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					xsprintf(xp_dot.size, _T("%d"), (int)(dd / 5));

					(*pif->pf_draw_line)(pif->canvas, &xp_dot, &(pa[1]), &(pa[2]));
				}
				else
				{
					if (j < 1)
						(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, 5 * dd / 4, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					else if (j < 2)
						(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, 3 * dd / 2, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					else if (j < 4)
						(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, 7 * dd / 4, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					else 
						(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, 2 * dd, fang + k * (dr / 5), fang + (k + 1) * (dr / 5), pa, 4);
					xsprintf(xp_dot.size, _T("%d"), (int)(dd / 5) + 1);
				}
			}

			xsprintf(numstr, _T("%f"), y_base[i] + y_step[i] * (j + 1));
			(*pif->pf_text_out)(pif->canvas, &xf_dot, &(pa[2]), numstr, -1);
		}

		//pointer
		ft_expand_rect(&xr, -2 * dd, -2 * dd);

		dbl = matrix_get_value(pmt, i, 0);

		if (y_step[i] != 0.0f)
			fang = 4 * XPI / 5 + ((dbl - y_base[i]) / y_step[i]) * dr;
		else
			fang = 4 * XPI / 5;
		(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, rr, fang, fang, pa, 1);

		ft_expand_rect(&xr, -2 * dd, -2 * dd);

		if (xr.fw < 6.0f)
		{
			xr.fx = middx - 3.0f;
			xr.fy = middy - 3.0f;
			xr.fw = 6.0f;
			xr.fh = 6.0f;
		}

		tang = (fang - XPI / 20);
		(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, dd, tang, tang, pa + 1, 1);

		pa[2].fx = pt.fx;
		pa[2].fy = pt.fy;

		tang = fang + XPI / 20;
		(*pif->pf_calc_fan)(pif->canvas, &pt, xr.fw / 2, dd, tang, tang, pa + 3, 1);

		(*pif->pf_draw_polygon)(pif->canvas, &xp, &xb, pa, 4);

		xr.fx = middx - 2.0f;
		xr.fy = middy - 2.0f;
		xr.fw = 4.0f;
		xr.fh = 4.0f;
		(*pif->pf_draw_ellipse)(pif->canvas, &xp, &xb, &xr);

		xsprintf(numstr, _T("%04d"), (int)dbl);

		for (k = -2; k < 2; k++)
		{
			xr.fx = middx + k * dd / 2;
			xr.fw = dd / 2;
			xr.fy = middy + dd;
			xr.fh = dd / 2;
			(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);

			(*pif->pf_draw_text)(pif->canvas, &xf_dot, &xa, &xr, (numstr + k + 2), 1);
		}
	}

	xmem_free(y_step);
	xmem_free(y_base);
}

static void _plot_topoggram(const if_canvas_t* pif, const canvbox_t* pbox, const plot_t* plt, const matrix_t* pmt)
{

}

/********************************************************************************************************************/

void draw_plot(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	plot_t po = { 0 };
	int rows, cols;
	matrix_t* pmt = { 0 };
	const tchar_t* type;
	const tchar_t* matrix;

	type = get_plot_type_ptr(ptr);

	po.title = get_plot_title_ptr(ptr);
	po.style = get_plot_style_ptr(ptr);
	po.ruler = get_plot_ruler(ptr);
	po.y_stages = get_plot_y_stages_ptr(ptr);
	po.y_bases = get_plot_y_bases_ptr(ptr);
	po.y_steps = get_plot_y_steps_ptr(ptr);
	po.y_labels = get_plot_y_labels_ptr(ptr);
	po.y_colors = get_plot_y_colors_ptr(ptr);
	po.y_shapes = get_plot_y_shapes_ptr(ptr);
	po.x_labels = get_plot_x_labels_ptr(ptr);
	po.x_colors = get_plot_x_colors_ptr(ptr);

	matrix = get_plot_matrix_ptr(ptr);

	rows = get_plot_rows(ptr);
	if (!rows) rows = 1;

	cols = get_plot_cols(ptr);
	if (!cols) cols = 1;

	pmt = matrix_alloc(rows, cols);
	matrix_parse(pmt, matrix, -1);

	if (compare_text(type, -1, ATTR_PLOT_TYPE_INDICATOR, -1, 1) == 0)
		_plot_indicator(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_BARGRAM, -1, 1) == 0)
		_plot_bargram(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_TOPOGGGRAM, -1, 1) == 0)
		_plot_topoggram(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_TRENDGRAM, -1, 1) == 0)
		_plot_trendgram(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_SCATTERGRAM, -1, 1) == 0)
		_plot_scattergram(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_PANTOGRAM, -1, 1) == 0)
		_plot_pantogram(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_HISTOGRAM, -1, 1) == 0)
		_plot_histogram(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_RADARGRAM, -1, 1) == 0)
		_plot_radargram(pif, pbox, &po, pmt);
	else if (compare_text(type, -1, ATTR_PLOT_TYPE_FUELGRAM, -1, 1) == 0)
		_plot_fuelgram(pif, pbox, &po, pmt);

	matrix_free(pmt);
}