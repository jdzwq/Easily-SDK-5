/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg gdi document

	@module	svggdi.c | implement file

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

#include "svggdi.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_SVG)

void svg_text_metric_raw(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	float mm = 0.0f;

	font_metric_by_point(xstof(pxf->size), &mm, NULL);

	pxs->cx = svg_tm_to_pt(canv, mm, 1) - svg_tm_to_pt(canv, 0, 1);
	pxs->cy = svg_tm_to_pt(canv, mm, 0) - svg_tm_to_pt(canv, 0, 0);
}

void svg_text_size_raw(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	float mm = 0.0f;

	font_metric_by_point(xstof(pxf->size), &mm, NULL);

	if (len < 0) len = xslen(txt);

	pxs->cx = svg_tm_to_pt(canv, mm * len, 1) - svg_tm_to_pt(canv, 0, 1);
	pxs->cy = svg_tm_to_pt(canv, mm, 0) - svg_tm_to_pt(canv, 0, 0);
}

void svg_draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	link_t_ptr g, nlk;
	xpoint_t pt[2];

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	pt[0].fx = ppt1->fx;
	pt[0].fy = ppt1->fy;
	pt[1].fx = ppt2->fx;
	pt[1].fy = ppt2->fy;

	svg_point_tm_to_pt(canv, &pt[0]);
	svg_point_tm_to_pt(canv, &pt[1]);

	write_line_to_svg_node(nlk, pxp, &pt[0], &pt[1]);
}

void svg_draw_3dline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	link_t_ptr g, nlk;
	xpoint_t pt[2];

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	pt[0].fx = ppt1->fx;
	pt[0].fy = ppt1->fy;
	pt[1].fx = ppt2->fx;
	pt[1].fy = ppt2->fy;

	svg_point_tm_to_pt(canv, &pt[0]);
	svg_point_tm_to_pt(canv, &pt[1]);

	write_line_to_svg_node(nlk, pxp, &pt[0], &pt[1]);
}

void svg_draw_3drect(canvas_t canv, const xpen_t* pxp, const xrect_t* pxr)
{	
	link_t_ptr g, nlk;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	write_rect_to_svg_node(nlk, pxp, NULL, &xr);
}

void svg_draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr g, nlk;
	xpoint_t* pa;
	int i;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	for (i = 0; i < n; i++)
	{
		pa[i].fx = ppt[i].fx;
		pa[i].fy = ppt[i].fy;
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	write_polyline_to_svg_node(nlk, pxp, pa, n);

	xmem_free(pa);
}

void svg_draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n)
{
	link_t_ptr g, nlk;
	xpoint_t* pa;
	int i;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	for (i = 0; i < n; i++)
	{
		pa[i].fx = ppt[i].fx;
		pa[i].fy = ppt[i].fy;
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	write_polygon_to_svg_node(nlk, pxp, pxb, pa, n);

	xmem_free(pa);
}

void svg_draw_bezier(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	link_t_ptr g, nlk;
	xpoint_t pt[4];

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	pt[0].fx = ppt1->fx;
	pt[0].fy = ppt1->fy;
	pt[1].fx = ppt2->fx;
	pt[1].fy = ppt2->fy;
	pt[2].fx = ppt3->fx;
	pt[2].fy = ppt3->fy;
	pt[3].fx = ppt4->fx;
	pt[3].fy = ppt4->fy;

	svg_point_tm_to_pt(canv, &pt[0]);
	svg_point_tm_to_pt(canv, &pt[1]);
	svg_point_tm_to_pt(canv, &pt[2]);
	svg_point_tm_to_pt(canv, &pt[3]);

	write_bezier_to_svg_node(nlk, pxp, &pt[0], &pt[1], &pt[2], &pt[3]);
}

void svg_draw_curve(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr g, nlk;
	xpoint_t* pa;
	int i;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	
	for (i = 0; i < n; i++)
	{
		xmem_copy((void*)&pa[i], (void*)&ppt[i], sizeof(xpoint_t));
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	write_curve_to_svg_node(nlk, pxp, pa, n);

	xmem_free(pa);
}

void svg_draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g, nlk;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	write_rect_to_svg_node(nlk, pxp, pxb, &xr);
}

void svg_gradient_rect(canvas_t canv, const xgradi_t* pxg, const xrect_t* pxr)
{
	
}

void svg_alphablend_rect(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int opacity)
{

}

void svg_draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g, nlk;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	write_round_to_svg_node(nlk, pxp, pxb, &xr, xr.w / 10, xr.h / 10);
}

void svg_draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g, nlk;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	write_ellipse_to_svg_node(nlk, pxp, pxb, &xr);
}

void svg_draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float rx, float ry, double fang, double tang)
{
	link_t_ptr g, nlk;
	xrect_t xr;
	tchar_t token[100] = { 0 };

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = ppt->fx;
	xr.fy = ppt->fy;
	xr.fw = rx;
	xr.fh = ry;

	svg_rect_tm_to_pt(canv, &xr);

	write_pie_to_svg_node(nlk, pxp, pxb, RECTPOINT(&xr), xr.w, xr.h, fang, tang);
}

void svg_draw_arc(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, float rx, float ry, double fang, double tang)
{

}

void svg_draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc)
{
	
}

void svg_draw_shape(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* shape)
{
	link_t_ptr g, nlk;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	write_shape_to_svg_node(nlk, pxp, pxb, &xr, shape);
}

void svg_multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr)
{
	link_t_ptr g, nlk;

	float line_rati;
	int th, lh;
	int i, rows;
	xpoint_t pt1, pt2;
	xrect_t xr;
	xsize_t xs;

	if (is_null(pxa->line_height))
		line_rati = xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = xstof(pxa->line_height);

	if (line_rati < 1)
		line_rati = 1.0;

	svg_text_metric_raw(canv, pxf, &xs);

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	svg_rect_tm_to_pt(canv, &xr);

	th = xs.cy;
	lh = (int)((float)th * (line_rati - 1.0));

	rows = xr.h / (th + lh);

	g = svg_get_canvas_doc(canv);

	pt1.x = xr.x;
	pt1.y = xr.y + th + lh;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + th + lh;

	for (i = 0; i < rows; i++)
	{
		nlk = insert_svg_node(g);

		write_line_to_svg_node(nlk, pxp, &pt1, &pt2);

		pt1.y += (th + lh);
		pt2.y += (th + lh);
	}
}

void svg_color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len)
{
	const tchar_t *pre, *nxt;
	xrect_t xr;
	xcolor_t xc;
	xbrush_t xb;
	tchar_t* val;
	int vlen;
	tchar_t clr[CLR_LEN + 1];

	if (len < 0)
		len = xslen(rgbstr);

	default_xbrush(&xb);

	pre = rgbstr;
	while (nxt = parse_string_token(pre, len, _T(';'), &val, &vlen))
	{
		xsncpy(clr, val, CLR_LEN);
		parse_xcolor(&xc, clr);
		format_xcolor(&xc, xb.color);

		svg_draw_rect(canv, NULL, &xb, &xr);

		len -= (nxt - pre);

		if (horz)
			xr.fx += xr.fw;
		else
			xr.fy += xr.fh;

		pre = nxt;
	}
}

void _svg_draw_single_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	link_t_ptr g, nlk;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	write_text_to_svg_node(nlk, pxf, pxa, &xr, txt, len);
}

void svg_draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	xrect_t xr = { 0 };

	if (pxa && !is_null(pxa->text_wrap))
	{
		if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 0) == 0)
		{ 
			xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
			svg_text_rect(canv, pxf, pxa, txt, len, &xr);

			xr.fy = pxr->fy + pxr->fh - xr.fh;
		}
		else if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_CENTER, -1, 0) == 0)
		{
			xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
			svg_text_rect(canv, pxf, pxa, txt, len, &xr);

			xr.fy = pxr->fy + (pxr->fh - xr.fh) / 2;
		}
		else
		{
			xr.fy = pxr->fy;
		}

		xr.fx = pxr->fx;
		xr.fw = pxr->fw;

		svg_draw_fix_text(canv, pxf, pxa, &xr, txt, len);
	}
	else
	{
		_svg_draw_single_text(canv, pxf, pxa, pxr, txt, len);
	}
}

void svg_draw_pass(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	tchar_t sz_pass[INT_LEN + 1] = { 0 };

	len = format_password(txt, sz_pass, INT_LEN);

	svg_draw_text(canv, pxf, pxa, pxr, sz_pass, len);
}

void svg_draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap)
{
	tchar_t sz_format[RES_LEN] = { 0 };
	xdate_t dt = { 0 };
	int lt;
	double db;

	xsize_t xs;
	xface_t xa;

	if (len < 0)
		len = xslen(data);

	xmem_copy((void*)&xa, (void*)pxa, sizeof(xface_t));

	if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
	{
		lt = xsntol(data, len);

		if (zeronull && !lt)
		{
			xscpy(sz_format, _T(""));
		}
		else if (!is_null(fmt))
		{
			format_integer_ex(lt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && pxa && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
	{
		db = xsntonum(data, len);

		if (zeronull && is_zero_numeric(db, dig))
		{
			xscpy(sz_format, _T(""));
		}
		else if (!is_null(fmt))
		{
			format_numeric(db, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
	{
		if (!is_null(fmt) && !is_null(data))
		{
			parse_datetime(&dt, data);
			format_datetime_ex(&dt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0)
	{
		if (!is_null(fmt) && !is_null(data))
		{
			parse_date(&dt, data);
			format_datetime_ex(&dt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
	{
		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, data, len, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, data, len);
	}
}

void svg_draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr)
{
	link_t_ptr g, nlk;
	xrect_t xr;

	if (is_null(pxi->source))
		return;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	nlk = insert_svg_node(g);
	write_ximage_to_svg_node(nlk, pxi, &xr);
}

void svg_draw_code128(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* text, int len)
{
	link_t_ptr g, nlk;

	int black,span;
	dword_t i;
	float unit = 0.3;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = code128_encode(buf, buf_len, NULL, MAX_LONG);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = code128_encode(buf, buf_len, bar_buf, bar_len);

	xmem_free(buf);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.fx = prt->fx + unit;
	rt.fy = prt->fy + unit;
	rt.fw = unit;
	rt.fh = prt->fh - 2 * unit;

	black = 0;
	for (i = 0; i < bar_len; i++)
	{
		span = (bar_buf[i] - '0');
		rt.fw = span * unit;

		black = (black) ? 0 : 1;

		if (black)
		{
			svg_draw_rect(canv, &xp, &xb, &rt);
		}
		
		rt.fx += rt.fw;
	}

	xmem_free(bar_buf);
}

void svg_draw_pdf417(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* text, int len)
{
	link_t_ptr g, nlk;

	int black,span;
	int rows,cols;
	unsigned char b, c;
	int i,j;

	float unit = 0.5;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = pdf417_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = pdf417_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.fx = prt->fx + unit;
		rt.fw = unit;
		rt.fy = prt->fy + unit + i * 2 * unit;
		rt.fh = 2 * unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.fx += rt.fw;

				black = (c & b) ? 0 : 1;

				if (black)
				{
					svg_draw_rect(canv, &xp, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);
}

#ifdef GPL_SUPPORT_QRCODE
void svg_draw_qrcode(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* text, int len)
{
	link_t_ptr g, nlk;

	int black,span;
	int rows,cols;
	unsigned char b, c;
	int i,j;

	float unit = 0.5;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = qr_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = qr_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.fx = prt->fx + unit;
		rt.fw = unit;
		rt.fy = prt->fy + unit + i * unit;
		rt.fh = unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.fx += rt.fw;

				black = (c & b) ? 1 : 0;

				if (black)
				{
					svg_draw_rect(canv, &xp, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);
}
#endif

void svg_draw_gizmo(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* gname)
{
}

void svg_draw_icon(canvas_t canv, const tchar_t* iname, const xrect_t* prt)
{
}

void svg_draw_thumb(canvas_t canv, const tchar_t* fname, const xrect_t* prt)
{
}

void svg_text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	xrect_t xr;
	xface_t xa;
	link_t_ptr g, nlk;

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = ppt->fx;
	xr.fy = ppt->fy;

	svg_text_size(canv, pxf, txt, len, RECTSIZE(&xr));

	default_xface(&xa);

	svg_rect_tm_to_pt(canv, &xr);

	write_text_to_svg_node(nlk, pxf, &xa, &xr, txt, len);
}

void svg_text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	float mm = 0.0f;

	font_metric_by_point(xstof(pxf->size), &mm, NULL);

	if (len < 0) len = xslen(txt);

	pxs->fx = mm * len;
	pxs->fy = mm;
}

void svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	float mm = 0.0f;

	font_metric_by_point(xstof(pxf->size), &mm, NULL);

	pxs->fx = mm;
	pxs->fy = mm;
}

typedef struct _FIXTEXT_SCAN{
	canvas_t canv;
	xrect_t xr;
}FIXTEXT_SCAN;

static int _fix_text_calc_rect(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	FIXTEXT_SCAN* ptt = (FIXTEXT_SCAN*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->xr.w = ptm->cur_x + ptm->cur_w - ptm->min_x + ptm->char_w;
		ptt->xr.h = ptm->cur_y + ptm->cur_h - ptm->min_y + ptm->line_h;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr)
{
	FIXTEXT_SCAN tt = { 0 };
	if_measure_t it = { 0 };

	tt.canv = canv;

	tt.xr.fx = pxr->fx;
	tt.xr.fy = pxr->fy;
	tt.xr.fw = pxr->fw;
	tt.xr.fh = PAPER_MAX_HEIGHT;
	svg_rect_tm_to_pt(canv, &tt.xr);

	it.ctx = (void*)canv;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	if (sz_len < 0)
		sz_len = xslen(sz_text);
	if (sz_text)
		sz_len++;

	scan_fix_text((tchar_t*)sz_text, sz_len, &it, pxf, pxa, tt.xr.x, tt.xr.y, tt.xr.w, tt.xr.h, 0, _fix_text_calc_rect, (void*)&tt);

	pxr->fw = svg_pt_to_tm(canv, tt.xr.w, 1) - svg_pt_to_tm(canv, 0, 1);
	pxr->fh = svg_pt_to_tm(canv, tt.xr.h, 0) - svg_pt_to_tm(canv, 0, 0);
}

typedef struct _FIXTEXT_DRAW{
	canvas_t canv;
	string_t vs;
}FIXTEXT_DRAW;

static int _fix_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	FIXTEXT_DRAW* ptt = (FIXTEXT_DRAW*)pp;
	xrect_t xr;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		svg_rect_pt_to_tm(ptt->canv, &xr);

		_svg_draw_single_text(ptt->canv, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_fix_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* text, int len)
{
	FIXTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };
	xrect_t xr;

	tt.canv = canv;
	tt.vs = string_alloc();

	it.ctx = (void*)canv;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = PAPER_MAX_HEIGHT;
	svg_rect_tm_to_pt(canv, &xr);

	if (len < 0)
		len = xslen(text);
	if (text)
		len++;

	scan_fix_text((tchar_t*)text, len, &it, pxf, pxa, xr.x, xr.y, xr.w, xr.h, 0, _fix_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

typedef struct _VARTEXT_DRAW{
	canvas_t canv;
	int page;
	string_t vs;
}VARTEXT_DRAW;

static int _var_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	VARTEXT_DRAW* ptt = (VARTEXT_DRAW*)pp;
	xrect_t xr;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		svg_rect_pt_to_tm(ptt->canv, &xr);

		_svg_draw_single_text(ptt->canv, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var)
{
	VARTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };
	xrect_t xr;

	tt.canv = canv;
	tt.page = 0;
	tt.vs = string_alloc();

	it.ctx = (void*)canv;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	scan_var_text(var, &it, pxf, pxa, xr.x, xr.y, xr.w, xr.h, 0, _var_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

typedef struct _TAGTEXT_DRAW{
	canvas_t canv;
	int page;
	string_t vs;
}TAGTEXT_DRAW;

static int _tag_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TAGTEXT_DRAW* ptt = (TAGTEXT_DRAW*)pp;
	xrect_t xr;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_END:
		xr.x = ptm->min_x + ptm->char_w;
		xr.y = ptm->cur_y;
		xr.w = ptm->max_x - ptm->min_x;
		xr.h = ptm->cur_h;

		svg_rect_pt_to_tm(ptt->canv, &xr);

		_svg_draw_single_text(ptt->canv, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	TAGTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };
	xrect_t xr;

	tt.canv = canv;
	tt.page = page;
	tt.vs = string_alloc();

	it.ctx = (void*)canv;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	scan_tag_text(data, &it, pxf, pxa, xr.x, xr.y, xr.w, xr.h, 1, _tag_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

typedef struct _MEMOTEXT_DRAW{
	canvas_t canv;
	int page;
	string_t vs;
}MEMOTEXT_DRAW;

static int _memo_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	MEMOTEXT_DRAW* ptt = (MEMOTEXT_DRAW*)pp;
	xrect_t xr;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		svg_rect_pt_to_tm(ptt->canv, &xr);

		_svg_draw_single_text(ptt->canv, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	MEMOTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };
	xrect_t xr;

	tt.canv = canv;
	tt.page = page;
	tt.vs = string_alloc();

	it.ctx = (void*)canv;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	scan_memo_text(data, &it, pxf, pxa, xr.x, xr.y, xr.w, xr.h, 1, _memo_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

typedef struct _RICHTEXT_DRAW{
	canvas_t canv;
	int page;
	string_t vs;
}RICHTEXT_DRAW;

static int _rich_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	RICHTEXT_DRAW* ptt = (RICHTEXT_DRAW*)pp;
	xrect_t xr;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		svg_rect_pt_to_tm(ptt->canv, &xr);

		_svg_draw_single_text(ptt->canv, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	RICHTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };
	xrect_t xr;

	tt.canv = canv;
	tt.page = page;
	tt.vs = string_alloc();

	it.ctx = (void*)canv;
	it.pf_mm_points = (PF_MM_POINTS)svg_pt_per_mm;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	scan_rich_text(data, &it, pxf, pxa, xr.x, xr.y, xr.w, xr.h, 1, _rich_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}


#endif /*XDL_SUPPORT_SVG*/

