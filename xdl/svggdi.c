/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg gdi document

	@module	svggdi.c | svg gdi implement file

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
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDK_SUPPORT_CONTEXT)

void svg_text_metric_raw(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	res_ctx_t rdc;
	xsize_t xs;

	rdc = svg_get_canvas_ctx(canv);

	text_mm_metric(rdc, pxf, &xs);

	pxs->cx = svg_tm_to_pt(canv, xs.fx, 1) - svg_tm_to_pt(canv, 0, 1);
	pxs->cy = svg_tm_to_pt(canv, xs.fy, 0) - svg_tm_to_pt(canv, 0, 0);
}

void svg_text_size_raw(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	xsize_t xs;
	res_ctx_t rdc;

	rdc = svg_get_canvas_ctx(canv);

	text_mm_size(rdc, pxf, txt, len, &xs);

	pxs->cx = svg_tm_to_pt(canv, xs.fx, 1) - svg_tm_to_pt(canv, 0, 1);
	pxs->cy = svg_tm_to_pt(canv, xs.fy, 0) - svg_tm_to_pt(canv, 0, 0);
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

void svg_draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang)
{
	link_t_ptr g, nlk;
	xrect_t xr;
	tchar_t token[100] = { 0 };

	g = svg_get_canvas_doc(canv);

	nlk = insert_svg_node(g);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	write_pie_to_svg_node(nlk, pxp, pxb, &xr, fang, tang);
}

void svg_draw_arc(canvas_t canv, const xpen_t* pxp, const xrect_t* pxr, double fang, double tang)
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

void svg_multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr)
{
	link_t_ptr g, nlk;

	float line_rati;
	long th, lh;
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
	lh = (long)((float)th * (line_rati - 1.0));

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

void svg_draw_password(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	tchar_t sz_pass[INT_LEN + 1] = { 0 };

	len = format_password(txt, sz_pass, INT_LEN);

	svg_draw_text(canv, pxf, pxa, pxr, sz_pass, len);
}

void svg_draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap)
{
	tchar_t sz_format[RES_LEN] = { 0 };
	xdate_t dt = { 0 };
	long lt;
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
			format_long_ex(lt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && pxa && is_null(xa.text_wrap))
		{
			text_size(canv, pxf, sz_format, -1, &xs);
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
			text_size(canv, pxf, sz_format, -1, &xs);
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
			text_size(canv, pxf, sz_format, -1, &xs);
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
			text_size(canv, pxf, sz_format, -1, &xs);
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
			text_size(canv, pxf, data, len, &xs);
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

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
	res_ctx_t rdc;
	res_bmp_t bmp;
	byte_t* bmp_buf;
	dword_t bmp_len;
	byte_t* png_buf;
	dword_t png_len;
	tchar_t* base_buf;
	int base_len;
	ximage_t xi = { 0 };
	xcolor_t xc;
#endif

	if (is_null(pxi->source))
		return;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
	if (compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_BMP, -1, 0) == 0)
	{
#ifdef GPL_SUPPORT_PNG
		bmp_len = xbas_decode(pxi->source, -1, NULL, MAX_LONG);
		bmp_buf = (byte_t*)xmem_alloc(bmp_len);
		xbas_decode(pxi->source, -1, bmp_buf, bmp_len);

		png_len = bmp_len;
		png_buf = (byte_t*)xmem_alloc(png_len);
		png_len = xpng_compress(bmp_buf, bmp_len, png_buf, png_len);

		xmem_free(bmp_buf);

		base_len = xbas_encode(png_buf, png_len, NULL, MAX_LONG);
		base_buf = xsalloc(base_len + 1);
		xbas_encode(png_buf, png_len, base_buf, base_len);

		xmem_free(png_buf);

		xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
		xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_PNG);
		xi.source = base_buf;

		nlk = insert_svg_node(g);

		write_ximage_to_svg_node(nlk, &xi, &xr);

		xsfree(base_buf);
#else
		nlk = insert_svg_node(g);
		write_ximage_to_svg_node(nlk, pxi, &xr);
#endif
	}
	else if (compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_COLOR, -1, 0) == 0)
	{
		parse_xcolor(&xc, pxi->source);

		rdc = svg_get_canvas_ctx(canv);
		bmp = create_color_bitmap(rdc, &xc, xr.w, xr.h);
		if (bmp)
		{
			bmp_len = get_bitmap_bytes(bmp);
			bmp_buf = (byte_t*)xmem_alloc(bmp_len);

			save_bitmap_to_bytes(rdc, bmp, bmp_buf, bmp_len);

			destroy_bitmap(bmp);

#ifdef GPL_SUPPORT_PNG
			png_len = bmp_len;
			png_buf = (byte_t*)xmem_alloc(png_len);
			png_len = xpng_compress(bmp_buf, bmp_len, png_buf, png_len);

			xmem_free(bmp_buf);

			base_len = xbas_encode(png_buf, png_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(png_buf, png_len, base_buf, base_len);

			xmem_free(png_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_PNG);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#else
			base_len = xbas_encode(bmp_buf, bmp_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(bmp_buf, bmp_len, base_buf, base_len);

			xmem_free(bmp_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_BMP);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#endif
		}
	}
#ifdef XDL_SUPPORT_BAR
	else if (compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_CODE128, -1, 0) == 0)
	{
		rdc = svg_get_canvas_ctx(canv);
		bmp = create_code128_bitmap(rdc, xr.w, xr.h, pxi->source);
		if (bmp)
		{
			bmp_len = get_bitmap_bytes(bmp);
			bmp_buf = (byte_t*)xmem_alloc(bmp_len);

			save_bitmap_to_bytes(rdc, bmp, bmp_buf, bmp_len);

			destroy_bitmap(bmp);

#ifdef GPL_SUPPORT_PNG
			png_len = bmp_len;
			png_buf = (byte_t*)xmem_alloc(png_len);
			png_len = xpng_compress(bmp_buf, bmp_len, png_buf, png_len);

			xmem_free(bmp_buf);

			base_len = xbas_encode(png_buf, png_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(png_buf, png_len, base_buf, base_len);

			xmem_free(png_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_PNG);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#else
			base_len = xbas_encode(bmp_buf, bmp_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(bmp_buf, bmp_len, base_buf, base_len);

			xmem_free(bmp_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_BMP);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#endif
		}
	}
	else if (compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_PDF417, -1, 0) == 0)
	{
		rdc = svg_get_canvas_ctx(canv);
		bmp = create_pdf417_bitmap(rdc, xr.w, xr.h, pxi->source);
		if (bmp)
		{
			bmp_len = get_bitmap_bytes(bmp);
			bmp_buf = (byte_t*)xmem_alloc(bmp_len);

			save_bitmap_to_bytes(rdc, bmp, bmp_buf, bmp_len);

			destroy_bitmap(bmp);

#ifdef GPL_SUPPORT_PNG
			png_len = bmp_len;
			png_buf = (byte_t*)xmem_alloc(png_len);
			png_len = xpng_compress(bmp_buf, bmp_len, png_buf, png_len);

			xmem_free(bmp_buf);

			base_len = xbas_encode(png_buf, png_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(png_buf, png_len, base_buf, base_len);

			xmem_free(png_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_PNG);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#else
			base_len = xbas_encode(bmp_buf, bmp_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(bmp_buf, bmp_len, base_buf, base_len);

			xmem_free(bmp_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_BMP);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#endif
		}
	}
	else if (compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_QRCODE, -1, 0) == 0)
	{
		rdc = svg_get_canvas_ctx(canv);
		bmp = create_qrcode_bitmap(rdc, xr.w, xr.h, pxi->source);
		if (bmp)
		{
			bmp_len = get_bitmap_bytes(bmp);
			bmp_buf = (byte_t*)xmem_alloc(bmp_len);

			save_bitmap_to_bytes(rdc, bmp, bmp_buf, bmp_len);

			destroy_bitmap(bmp);

#ifdef GPL_SUPPORT_PNG
			png_len = bmp_len;
			png_buf = (byte_t*)xmem_alloc(png_len);
			png_len = xpng_compress(bmp_buf, bmp_len, png_buf, png_len);

			xmem_free(bmp_buf);

			base_len = xbas_encode(png_buf, png_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(png_buf, png_len, base_buf, base_len);

			xmem_free(png_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_PNG);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#else
			base_len = xbas_encode(bmp_buf, bmp_len, NULL, MAX_LONG);
			base_buf = xsalloc(base_len + 1);
			xbas_encode(bmp_buf, bmp_len, base_buf, base_len);

			xmem_free(bmp_buf);

			xmem_copy((void*)&xi, (void*)pxi, sizeof(ximage_t));
			xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_BMP);
			xi.source = base_buf;

			nlk = insert_svg_node(g);

			write_ximage_to_svg_node(nlk, &xi, &xr);

			xsfree(base_buf);
#endif
		}
	}
#endif
	else if (compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_JPG, -1, 0) == 0 || compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_PNG, -1, 0) == 0)
	{
		nlk = insert_svg_node(g);
		write_ximage_to_svg_node(nlk, pxi, &xr);
	}
#else
	if (compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_JPG, -1, 0) == 0 || compare_text(pxi->type, -1, GDI_ATTR_IMAGE_TYPE_PNG, -1, 0) == 0)
	{
		nlk = insert_svg_node(g);
		write_ximage_to_svg_node(nlk, pxi, &xr);
	}
#endif
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
	res_ctx_t rdc;

	rdc = svg_get_canvas_ctx(canv);

	text_mm_size(rdc, pxf, txt, len, pxs);
}

void svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	res_ctx_t rdc;

	rdc = svg_get_canvas_ctx(canv);

	text_mm_metric(rdc, pxf, pxs);
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

////////////////////////////////////////////////////////////////////////////////////////////////
void _svg_draw_logo_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	
}

void _svg_draw_insert_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.fx = prt->fx;
	xr.fy = (float)(prt->fy + prt->fh / 2 - 0.25);
	xr.fw = prt->fw;
	xr.fh = 0.5;
	svg_draw_rect(canv, &xp, &xb, &xr);

	xr.fx = (float)(prt->fx + prt->fw / 2 - 0.25);
	xr.fy = prt->fy;
	xr.fw = 0.5;
	xr.fh = prt->fh;
	svg_draw_rect(canv, &xp, &xb, &xr);
}

void _svg_draw_delete_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.fx = prt->fx;
	xr.fy = (float)(prt->fy + prt->fh / 2 - 0.25);
	xr.fw = prt->fw;
	xr.fh = 0.5;
	svg_draw_rect(canv, &xp, &xb, &xr);
}

void _svg_draw_plus_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xr.fx = prt->fx;
	xr.fy = prt->fy;
	xr.fw = prt->fw / 2;
	xr.fh = prt->fh / 2;
	svg_draw_rect(canv, &xp, NULL, &xr);

	svg_draw_rect(canv, &xp, NULL, prt);
}

void _svg_draw_minus_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	pt1.fx = prt->fx;
	pt1.fy = prt->fy + prt->fh / 2;
	pt2.fx = prt->fx + prt->fw;
	pt2.fy = prt->fy + prt->fh / 2;
	svg_draw_line(canv, &xp, &pt1, &pt2);

	svg_draw_rect(canv, &xp, NULL, prt);
}

void _svg_draw_collapse_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	pt1.fx = prt->fx + prt->fw / 2;
	pt1.fy = prt->fy;
	pt2.fx = prt->fx + prt->fw;
	pt2.fy = prt->fy + prt->fh / 2;
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = prt->fx + prt->fw / 2;
	pt1.fy = prt->fy + prt->fh;
	pt2.fx = prt->fx + prt->fw;
	pt2.fy = prt->fy + prt->fh / 2;
	svg_draw_line(canv, &xp, &pt1, &pt2);
}

void _svg_draw_expand_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	pt1.fx = prt->fx;
	pt1.fy = prt->fy + prt->fh / 2;
	pt2.fx = prt->fx + prt->fw / 2;
	pt2.fy = prt->fy + prt->fh;
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = prt->fx + prt->fw;
	pt1.fy = prt->fy + prt->fh / 2;
	pt2.fx = prt->fx + prt->fw / 2;
	pt2.fy = prt->fy + prt->fh;
	svg_draw_line(canv, &xp, &pt1, &pt2);
}

void _svg_draw_home_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = (float)(prt->fx + prt->fw / 2);
	pt[0].fy = (float)(prt->fy + 0.25);
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy + prt->fh;
	pt[2].fx = prt->fx;
	pt[2].fy = prt->fy + prt->fh;
	svg_draw_polygon(canv, &xp, &xb, pt, 3);

	xr.fx = prt->fx;
	xr.fy = prt->fy;
	xr.fw = prt->fw;
	xr.fh = 0.5;
	svg_draw_rect(canv, &xp, &xb, &xr);
}

void _svg_draw_up_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = (float)(prt->fx + prt->fw / 2);
	pt[0].fy = (float)(prt->fy + 0.25);
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy + prt->fh;
	pt[2].fx = prt->fx;
	pt[2].fy = prt->fy + prt->fh;
	svg_draw_polygon(canv, &xp, &xb, pt, 3);
}

void _svg_draw_down_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy;
	pt[2].fx = (float)(prt->fx + prt->fw / 2);
	pt[2].fy = (float)(prt->fy + prt->fh - 0.25);
	svg_draw_polygon(canv, &xp, &xb, pt, 3);
}

void _svg_draw_end_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy;
	pt[2].fx = (float)prt->fx + prt->fw / 2;
	pt[2].fy = (float)(prt->fy + prt->fh - 0.25);
	svg_draw_polygon(canv, &xp, &xb, pt, 3);

	xr.fx = prt->fx;
	xr.fy = (float)(prt->fy + prt->fh - 0.5);
	xr.fw = prt->fw;
	xr.fh = (float)0.5;
	svg_draw_rect(canv, &xp, &xb, &xr);
}

void _svg_draw_first_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = (float)(prt->fx + 0.25);
	pt[0].fy = (float)(prt->fy + prt->fh / 2);
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy;
	pt[2].fx = prt->fx + prt->fw;
	pt[2].fy = prt->fy + prt->fh;
	svg_draw_polygon(canv, &xp, &xb, pt, 3);

	xr.fx = prt->fx;
	xr.fy = prt->fy;
	xr.fw = 0.5;
	xr.fh = prt->fh;
	svg_draw_rect(canv, &xp, &xb, &xr);
}

void _svg_draw_prev_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = (float)(prt->fx + 0.25);
	pt[0].fy = (float)(prt->fy + prt->fh / 2);
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy;
	pt[2].fx = prt->fx + prt->fw;
	pt[2].fy = prt->fy + prt->fh;
	svg_draw_polygon(canv, &xp, &xb, pt, 3);
}

void _svg_draw_next_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = (float)(prt->fx + prt->fw - 0.25);
	pt[1].fy = (float)(prt->fy + prt->fh / 2);
	pt[2].fx = prt->fx;
	pt[2].fy = prt->fy + prt->fh;
	svg_draw_polygon(canv, &xp, &xb, pt, 3);
}

void _svg_draw_last_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = (float)(prt->fx + prt->fw - 0.25);
	pt[1].fy = (float)(prt->fy + prt->fh / 2);
	pt[2].fx = prt->fx;
	pt[2].fy = prt->fy + prt->fh;
	svg_draw_polygon(canv, &xp, &xb, pt, 3);

	xr.fx = (float)(prt->fx + prt->fw - 0.5);
	xr.fy = prt->fy;
	xr.fw = (float)0.5;
	xr.fh = prt->fh;
	svg_draw_rect(canv, &xp, &xb, &xr);
}

void _svg_draw_close_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	pt1.fx = prt->fx;
	pt1.fy = prt->fy;
	pt2.fx = prt->fx + prt->fw;
	pt2.fy = prt->fy + prt->fh;
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = prt->fx;
	pt1.fy = prt->fy + prt->fh;
	pt2.fx = prt->fx + prt->fw;
	pt2.fy = prt->fy;
	svg_draw_line(canv, &xp, &pt1, &pt2);
}

void _svg_draw_minimize_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.fx = prt->fx;
	xr.fy = (float)(prt->fy + prt->fh - 0.25);
	xr.fw = (float)1.0;
	xr.fh = (float)0.5;
	svg_draw_rect(canv, &xp, &xb, &xr);
}

void _svg_draw_maximize_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xr.fx = prt->fx;
	xr.fw = (float)(prt->fw - 0.5);
	xr.fy = prt->fy;
	xr.fh = (float)(prt->fh - 0.5);
	svg_draw_rect(canv, &xp, NULL, &xr);

	xr.fx = (float)(prt->fx + 0.5);
	xr.fw = (float)(prt->fw - 0.5);
	xr.fy = (float)(prt->fy + 0.5);
	xr.fh = (float)(prt->fh - 0.5);
	svg_draw_rect(canv, &xp, NULL, &xr);
}

void _svg_draw_restore_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	svg_draw_rect(canv, &xp, NULL, prt);
}

void _svg_draw_sum_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5] = { 0 };

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	pt[0].fx = prt->fx + prt->fw;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx;
	pt[1].fy = prt->fy;
	pt[2].fx = prt->fx + prt->fw / 2;
	pt[2].fy = prt->fy + prt->fh / 2;
	pt[3].fx = prt->fx;
	pt[3].fy = prt->fy + prt->fh;
	pt[4].fx = prt->fx + prt->fw;
	pt[4].fy = prt->fy + prt->fh;

	svg_draw_polyline(canv, &xp, pt, 5);
}

void _svg_draw_checkbox_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	svg_draw_rect(canv, &xp, NULL, prt);
}

void _svg_draw_checked_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	svg_draw_rect(canv, &xp, NULL, prt);

	pt1.fx = prt->fx;
	pt1.fy = (float)(prt->fy + prt->fh / 2);
	pt2.fx = (float)(prt->fx + 0.5);
	pt2.fy = (float)(prt->fy + prt->fh / 2);
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = (float)(prt->fx + 0.5);
	pt1.fy = (float)(prt->fy + prt->fh / 2);
	pt2.fx = (float)(prt->fx + prt->fw / 2);
	pt2.fy = (float)(prt->fy + prt->fh - 0.25);
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = (float)(prt->fx + prt->fw / 2);
	pt1.fy = (float)(prt->fy + prt->fh - 0.25);
	pt2.fx = (float)(prt->fx + prt->fw + 0.5);
	pt2.fy = (float)(prt->fy + 0.25);
	svg_draw_line(canv, &xp, &pt1, &pt2);
}

void _svg_draw_radiobox_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	svg_draw_ellipse(canv, &xp, NULL, prt);
}

void _svg_draw_radioed_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	svg_draw_ellipse(canv, &xp, &xb, prt);
}

void _svg_draw_selected_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	xscpy(xp.size, _T("1"));
	format_xcolor(pxc, xp.color);

	pt1.fx = prt->fx;
	pt1.fy = (float)(prt->fy + prt->fh / 2);
	pt2.fx = (float)(prt->fx + 0.5);
	pt2.fy = (float)(prt->fy + prt->fh / 2);
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = (float)(prt->fx + 0.5);
	pt1.fy = (float)(prt->fy + prt->fh / 2);
	pt2.fx = (float)(prt->fx + prt->fw / 2);
	pt2.fy = prt->fy + prt->fh;
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = (float)(prt->fx + prt->fw / 2);
	pt1.fy = prt->fy + prt->fh;
	pt2.fx = prt->fx + prt->fw;
	pt2.fy = prt->fy;
	svg_draw_line(canv, &xp, &pt1, &pt2);
}

void _svg_draw_folder_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;
	xpoint_t pt[4];

	xmem_copy((void*)&xr, (void*)prt, sizeof(xrect_t));

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	pt[0].fx = xr.fx;
	pt[0].fy = (float)(xr.fy + xr.fh / 4);

	pt[1].fx = xr.fx;
	pt[1].fy = xr.fy;

	pt[2].fx = (float)(xr.fx + xr.fw / 2);
	pt[2].fy = xr.fy;

	pt[3].fx = (float)(xr.fx + 3 * xr.fw / 4);
	pt[3].fy = xr.fy + xr.fh / 4;

	svg_draw_polyline(canv, &xp, pt, 4);

	xr.fy += (float)(xr.fh / 4);
	xr.fh -= (float)(xr.fh / 4);

	svg_draw_rect(canv, &xp, NULL, &xr);
}

void _svg_draw_guider_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[5];
	float feed;

	xmem_copy((void*)&xr, (void*)prt, sizeof(xrect_t));

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	feed = (float)(xr.fw / 3);
	if (feed < 1.0)
		feed = 1.0;

	pt[0].fx = xr.fx;
	pt[0].fy = xr.fy;

	pt[1].fx = (float)(xr.fx + xr.fw - feed);
	pt[1].fy = xr.fy;

	pt[2].fx = xr.fx + xr.fw;
	pt[2].fy = (float)(xr.fy + xr.fh / 2);

	pt[3].fx = (float)(xr.fx + xr.fw - feed);
	pt[3].fy = xr.fy + xr.fh;

	pt[4].fx = xr.fx;
	pt[4].fy = xr.fy + xr.fh;

	svg_draw_polygon(canv, &xp, &xb, pt, 5);
}

void _svg_draw_fixed_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;
	xpoint_t pt1, pt2;

	xmem_copy((void*)&xr, (void*)prt, sizeof(xrect_t));

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	pt1.fx = xr.fx;
	pt1.fy = (float)(xr.fy + xr.fh / 2);
	pt2.fx = (float)(xr.fx + 0.5);
	pt2.fy = (float)(xr.fy + xr.fh / 2);
	svg_draw_line(canv, &xp, &pt1, &pt2);

	pt1.fx = (float)(xr.fx + 0.5);
	pt1.fy = xr.fy;
	pt2.fx = (float)(xr.fx + 0.5);
	pt2.fy = xr.fy + xr.fh;
	svg_draw_line(canv, &xp, &pt1, &pt2);

	xr.fx += (float)0.5;
	xr.fw -= (float)0.5;
	xr.fy += (float)0.5;
	xr.fh -= (float)1.0;

	svg_draw_rect(canv, &xp, NULL, &xr);
}

void _svg_draw_point_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	_svg_draw_single_text(canv, &xf, &xa, prt, ICON_TEXT_POINT, -1);
}

void _svg_draw_cross_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	_svg_draw_single_text(canv, &xf, &xa, prt, ICON_TEXT_CROSS, -1);
}

void _svg_draw_rect_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	_svg_draw_single_text(canv, &xf, &xa, prt, ICON_TEXT_RECT, -1);
}

void _svg_draw_ellipse_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	_svg_draw_single_text(canv, &xf, &xa, prt, ICON_TEXT_ELLIPSE, -1);
}

void _svg_draw_star_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	_svg_draw_single_text(canv, &xf, &xa, prt, ICON_TEXT_STAR, -1);
}

void _svg_draw_diamond_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	_svg_draw_single_text(canv, &xf, &xa, prt, ICON_TEXT_DIAMOND, -1);
}

void svg_draw_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* iname)
{
	if (compare_text(iname, -1, ATTR_ICON_LOGO, -1, 0) == 0)
		_svg_draw_logo_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_COLLAPSE, -1, 0) == 0)
		_svg_draw_collapse_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_EXPAND, -1, 0) == 0)
		_svg_draw_expand_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_INSERT, -1, 0) == 0)
		_svg_draw_insert_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_DELETE, -1, 0) == 0)
		_svg_draw_delete_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_PLUS, -1, 0) == 0)
		_svg_draw_plus_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_MINUS, -1, 0) == 0)
		_svg_draw_minus_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_HOME, -1, 0) == 0)
		_svg_draw_home_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_UP, -1, 0) == 0)
		_svg_draw_up_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_DOWN, -1, 0) == 0)
		_svg_draw_down_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_END, -1, 0) == 0)
		_svg_draw_end_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_FIRST, -1, 0) == 0)
		_svg_draw_first_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_PREV, -1, 0) == 0)
		_svg_draw_prev_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_NEXT, -1, 0) == 0)
		_svg_draw_next_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_LAST, -1, 0) == 0)
		_svg_draw_last_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_GUIDER, -1, 0) == 0)
		_svg_draw_guider_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_FOLDER, -1, 0) == 0)
		_svg_draw_folder_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_CHECKBOX, -1, 0) == 0)
		_svg_draw_checkbox_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_CHECKED, -1, 0) == 0)
		_svg_draw_checked_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_RADIOBOX, -1, 0) == 0)
		_svg_draw_radiobox_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_RADIOED, -1, 0) == 0)
		_svg_draw_radioed_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_SELECTED, -1, 0) == 0)
		_svg_draw_selected_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_SUM, -1, 0) == 0)
		_svg_draw_sum_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_CLOSE, -1, 0) == 0)
		_svg_draw_close_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_MINIMIZE, -1, 0) == 0)
		_svg_draw_minimize_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_MAXIMIZE, -1, 0) == 0)
		_svg_draw_maximize_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_RESTORE, -1, 0) == 0)
		_svg_draw_restore_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_FIXED, -1, 0) == 0)
		_svg_draw_fixed_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_POINT, -1, 0) == 0)
		_svg_draw_point_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_RECT, -1, 0) == 0)
		_svg_draw_rect_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_ELLIPSE, -1, 0) == 0)
		_svg_draw_ellipse_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_CROSS, -1, 0) == 0)
		_svg_draw_cross_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_STAR, -1, 0) == 0)
		_svg_draw_star_icon(canv, pxc, pxr);
	else if (compare_text(iname, -1, ATTR_ICON_DIAMOND, -1, 0) == 0)
		_svg_draw_diamond_icon(canv, pxc, pxr);
}

#endif /*XDL_SUPPORT_VIEW*/

