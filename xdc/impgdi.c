/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi document

	@module	impgdi.c | implement file

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

#include "impgdi.h"
#include "xdcimp.h"
#include "xdcinit.h"

#if defined(XDU_SUPPORT_CONTEXT)

void draw_line_raw(res_ctx_t rdc,const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_line)(rdc, pxp, ppt1, ppt2);
}

void draw_polyline_raw(res_ctx_t rdc,const xpen_t* pxp,const xpoint_t* ppt,int n)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;
	
	(*pif->pf_gdi_draw_polyline)(rdc,pxp,ppt,n);
}

void draw_polygon_raw(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xpoint_t* ppt,int n)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_polygon)(rdc,pxp,pxb,ppt,n);
}

void draw_rect_raw(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* pxr)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_rect)(rdc,pxp,pxb,pxr);
}

void draw_path_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_path)(rdc, pxp, pxb, aa, pa);
}

void draw_round_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_round)(rdc, pxp, pxb, pxr);
}

void draw_bezier_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_bezier)(rdc, pxp, ppt1, ppt2, ppt3, ppt4);
}

void draw_curve_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_curve)(rdc, pxp, ppt, n);
}

void gradient_rect_raw(res_ctx_t rdc, const xcolor_t* xc_brim, const xcolor_t* xc_core, const tchar_t* gradient, const xrect_t* pxr)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_gradient_rect)(rdc, xc_brim, xc_core, gradient, pxr);
}

void alphablend_rect_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr, int opacity)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_alphablend_rect)(rdc, pxc, pxr, opacity);
}

void draw_ellipse_raw(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* pxr)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_ellipse)(rdc,pxp,pxb,pxr);
}

void draw_pie_raw(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xpoint_t* ppt, int rx, int ry, double fang,double tang)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_pie)(rdc,pxp,pxb,ppt,rx,ry,fang,tang);
}

void draw_arc_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int rx, int ry, double fang, double tang)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_arc)(rdc, pxp, ppt, rx, ry, fang, tang);
}

void draw_arrow_raw(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* pxr,int alen,double arc)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_arrow)(rdc,pxp,pxb,pxr,alen,arc);
}

void draw_text_raw(res_ctx_t rdc,const xfont_t* pxf,const xface_t* pxa,const xrect_t* pxr,const tchar_t* txt,int len)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_text)(rdc,pxf,pxa,pxr,txt,len);
}

void text_out_raw(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_text_out)(rdc, pxf, ppt, txt, len);
}

void color_out_raw(res_ctx_t rdc, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len)
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
	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));

	pre = rgbstr;
	while (nxt = parse_string_token(pre, len, _T(';'), &val, &vlen))
	{
		xsncpy(clr, val, vlen);
		parse_xcolor(&xc, clr);
		format_xcolor(&xc, xb.color);

		draw_rect_raw(rdc, NULL, &xb, &xr);

		len -= (nxt - pre);

		if (horz)
			xr.x += xr.w;
		else
			xr.y += xr.h;
		
		pre = nxt;
	}
}

void draw_bitmap_raw(res_ctx_t rdc, res_bmp_t bmp, const xpoint_t* ppt)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_draw_bitmap)(rdc, bmp, ppt);
}

void draw_icon_raw(res_ctx_t rdc, const tchar_t* iname, const xrect_t* prt)
{
	if_context_t *pif;
	res_bmp_t bmp;

	pif = PROCESS_CONTEXT_INTERFACE;

	bmp = load_bitmap_from_icon(rdc, iname);

	if (bmp)
	{
		(*pif->pf_gdi_draw_image)(rdc, bmp, NULL, prt);
		destroy_bitmap(bmp);
	}
}

void draw_thumb_raw(res_ctx_t rdc, const tchar_t* fname, const xrect_t* prt)
{
	if_context_t *pif;
	res_bmp_t bmp;

	pif = PROCESS_CONTEXT_INTERFACE;

	bmp = load_bitmap_from_thumb(rdc, fname);

	if (bmp)
	{
		(*pif->pf_gdi_draw_image)(rdc, bmp, NULL, prt);
		destroy_bitmap(bmp);
	}
}

void draw_image_raw(res_ctx_t rdc, const ximage_t* pxi, const xrect_t* pxr)
{
	if_context_t *pif;
	res_bmp_t bmp;

	pif = PROCESS_CONTEXT_INTERFACE;

	bmp = load_bitmap_from_ximage(rdc, (ximage_t*)pxi, pxr->w, pxr->h);

	if (bmp)
	{
		(*pif->pf_gdi_draw_image)(rdc, bmp, pxi->color, pxr);
		destroy_bitmap(bmp);
	}
}

void draw_code128_raw(res_ctx_t rdc, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	if_context_t *pif;

	link_t_ptr g, nlk;

	int black,span;
	dword_t i;
	int unit;
	xrect_t rt;
	xbrush_t xb;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

	pif = PROCESS_CONTEXT_INTERFACE;

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

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
	}

	unit = (*pif->pf_cast_mm_to_pt)(rdc, 0.3, 1);

	rt.x = prt->x + unit;
	rt.y = prt->y + unit;
	rt.w = unit;
	rt.h = prt->h - 2 * unit;

	black = 0;
	for (i = 0; i < bar_len; i++)
	{
		span = (bar_buf[i] - '0');
		rt.w = span * unit;

		black = (black) ? 0 : 1;

		if (black && pxc)
		{
			(*pif->pf_gdi_draw_rect)(rdc, NULL, &xb, &rt);
		}
		
		rt.x += rt.w;
	}

	xmem_free(bar_buf);

	prt->w = rt.x + unit - prt->x;
}

void draw_pdf417_raw(res_ctx_t rdc, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	if_context_t *pif;

	link_t_ptr g, nlk;

	int black,span;
	int rows,cols;
	unsigned char b, c;
	int i,j;
	int unit;

	xrect_t rt;
	xbrush_t xb;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

	pif = PROCESS_CONTEXT_INTERFACE;

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

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
	}

	unit = (*pif->pf_cast_mm_to_pt)(rdc, 0.5, 1);

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.x = prt->x + unit;
		rt.w = unit;
		rt.y = prt->y + unit + i * 2 * unit;
		rt.h = 2 * unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.x += rt.w;

				black = (c & b) ? 0 : 1;

				if (black && pxc)
				{
					(*pif->pf_gdi_draw_rect)(rdc, NULL, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->w = rt.x + unit - prt->x;
	prt->h = rt.y + rt.h + unit - prt->y;
}

void draw_qrcode_raw(res_ctx_t rdc, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	if_context_t *pif;

	link_t_ptr g, nlk;

	int black,span;
	int rows,cols;
	unsigned char b, c;
	int i,j;

	float unit = 0.5;
	xrect_t rt;
	xbrush_t xb;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

	pif = PROCESS_CONTEXT_INTERFACE;

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

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
	}

	unit = (*pif->pf_cast_mm_to_pt)(rdc, 0.5, 1);

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.x = prt->x + unit;
		rt.w = unit;
		rt.y = prt->y + unit + i * unit;
		rt.h = unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.x += rt.w;

				black = (c & b) ? 1 : 0;

				if (black && pxc)
				{
					(*pif->pf_gdi_draw_rect)(rdc, NULL, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->w = rt.x + unit - prt->x;
	prt->h = rt.y + rt.h + unit - prt->y;
}


void image_size_raw(res_ctx_t rdc, const ximage_t* pxi, xsize_t* pxs)
{
	if_context_t *pif;
	res_bmp_t bmp;

	pif = PROCESS_CONTEXT_INTERFACE;

	bmp = load_bitmap_from_ximage(rdc, (ximage_t*)pxi,0, 0);
	if (bmp)
	{
		get_bitmap_size(bmp, &pxs->cx, &pxs->cy);
		destroy_bitmap(bmp);
	}
}

void fill_region_raw(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_fill_region)(rdc, pxb, rgn);
}


void exclip_rect_raw(res_ctx_t rdc, const xrect_t* pxr)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_exclip_rect)(rdc, pxr);
}

void inclip_rect_raw(res_ctx_t rdc, const xrect_t* pxr)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_inclip_rect)(rdc, pxr);
}

void text_size_raw(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_text_size)(rdc, pxf, txt, len, pxs);
}

void text_metric_raw(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_text_metric)(rdc, pxf, pxs);
}

void draw_gizmo_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* iname)
{
	PF_GIZMO_MAKE pf;

	pf = find_gizmo_maker(iname);
	if (pf)
	{
		(*pf)(rdc, pxc, pxr);
	}
}

void draw_select_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	draw_rect_raw(rdc, &xp,NULL, prt);
}

void draw_focus_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);

	draw_rect_raw(rdc, &xp,NULL, prt);
}

void draw_sizing_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep, dword_t pos)
{
	xrect_t xr;
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

	if (pos & SIZING_TOPLEFT)
	{
		xr.x = prt->x;
		xr.y = prt->y;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	if (pos & SIZING_TOPCENTER)
	{
		xr.x = prt->x + prt->w / 2 - 2;
		xr.y = prt->y;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	if (pos & SIZING_TOPRIGHT)
	{
		xr.x = prt->x + prt->w - 4;
		xr.y = prt->y;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	if (pos & SIZING_BOTTOMLEFT)
	{
		xr.x = prt->x;
		xr.y = prt->y + prt->h - 2;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	if (pos & SIZING_BOTTOMCENTER)
	{
		xr.x = prt->x + prt->w / 2 - 2;
		xr.y = prt->y + prt->h - 4;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	if (pos & SIZING_BOTTOMRIGHT)
	{
		xr.x = prt->x + prt->w - 4;
		xr.y = prt->y + prt->h - 4;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	if (pos & SIZING_LEFTCENTER)
	{
		xr.x = prt->x;
		xr.y = prt->y + prt->h / 2 - 2;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	if (pos & SIZING_RIGHTCENTER)
	{
		xr.x = prt->x + prt->w - 4;
		xr.y = prt->y + prt->h / 2 - 2;
		xr.w = 4;
		xr.h = 4;

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}
}

void draw_feed_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpoint_t pt[2];
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xsprintf(xp.opacity, _T("%d"), deep);
	//xp.adorn.feed = 2;
	//xp.adorn.size = 2;

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + 5;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + 5;
	pt[1].y = prt->y;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w - 5;
	pt[1].y = prt->y + prt->h;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h - 5;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_shape_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const tchar_t* shape)
{
	PF_SHAPE_MAKE pf;

	pf = find_shape_maker(shape);
	if (pf)
	{
		(*pf)(rdc, pxp, pxb, prt);
	}
}

void multi_line_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr)
{
	float line_rati;
	int lh, th;
	int i, rows;
	xpoint_t pt1, pt2;
	xsize_t xs;

	if (is_null(pxa->line_height))
		line_rati = xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = xstof(pxa->line_height);

	if (line_rati < 1)
		line_rati = 1.0;

	text_metric_raw(rdc, pxf, &xs);

	th = xs.cy;
	lh = (int)((float)th * (line_rati - 1.0));

	rows = pxr->h / (th + lh);

	pt1.x = pxr->x;
	pt1.y = pxr->y + th + lh;
	pt2.x = pxr->x + pxr->w;
	pt2.y = pxr->y + th + lh;

	for (i = 0; i < rows; i++)
	{
		draw_line_raw(rdc, pxp, &pt1, &pt2);

		pt1.y += (th + lh);
		pt2.y += (th + lh);
	}
}

typedef struct _FIXTEXT_SCAN{
	xrect_t xr;
}FIXTEXT_SCAN;

static int _fix_text_calc_rect(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	FIXTEXT_SCAN* ptt = (FIXTEXT_SCAN*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->xr.w = ptm->cur_x + ptm->cur_w - ptm->min_x;
		ptt->xr.h = ptm->cur_y + ptm->cur_h - ptm->min_y;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void text_rect_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr)
{
	FIXTEXT_SCAN tt = { 0 };
	if_measure_t it = { 0 };

	xmem_copy((void*)&tt.xr, (void*)pxr, sizeof(xrect_t));

	it.ctx = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	if (sz_len < 0)
		sz_len = xslen(sz_text);
	if (sz_text)
		sz_len++;

	scan_fix_text((tchar_t*)sz_text, sz_len, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, MAX_LONG, 0, _fix_text_calc_rect, (void*)&tt);

	xmem_copy((void*)pxr, (void*)&tt.xr, sizeof(xrect_t));
}

typedef struct _VARTEXT_DRAW{
	res_ctx_t rdc;
	int page;
}VARTEXT_DRAW;

static int _var_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	VARTEXT_DRAW* ptt = (VARTEXT_DRAW*)pp;
	xpoint_t pt;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		text_out_raw(ptt->rdc, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_var_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t data)
{
	VARTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.rdc = rdc;
	tt.page = 0;

	it.ctx = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	scan_var_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 0, _var_text_calc_draw, (void*)&tt);
}

typedef struct _TAGTEXT_DRAW{
	res_ctx_t rdc;
	int page;
}TAGTEXT_DRAW;

static int _tag_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TAGTEXT_DRAW* ptt = (TAGTEXT_DRAW*)pp;
	xpoint_t pt;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		text_out_raw(ptt->rdc, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_tag_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	TAGTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.rdc = rdc;
	tt.page = page;

	it.ctx = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	scan_tag_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _tag_text_calc_draw, (void*)&tt);
}

typedef struct _TAGTEXT_PAGE{
	int pages;
}TAGTEXT_PAGE;

static int _tag_text_calc_pages(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TAGTEXT_PAGE* ptt = (TAGTEXT_PAGE*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->pages = page;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int calc_tag_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data)
{
	TAGTEXT_PAGE tt = { 0 };
	if_measure_t it = { 0 };

	tt.pages = 1;

	it.ctx = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	scan_tag_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _tag_text_calc_pages, (void*)&tt);

	return tt.pages;
}

typedef struct _MEMOTEXT_DRAW{
	res_ctx_t rdc;
	int page;
}MEMOTEXT_DRAW;

static int _memo_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	MEMOTEXT_DRAW* ptt = (MEMOTEXT_DRAW*)pp;
	xpoint_t pt;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		text_out_raw(ptt->rdc, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_memo_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	MEMOTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.rdc = rdc;
	tt.page = page;

	it.ctx = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	scan_memo_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _memo_text_calc_draw, (void*)&tt);
}

typedef struct _MEMOTEXT_PAGE{
	int pages;
}MEMOTEXT_PAGE;

static int _memo_text_calc_pages(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	MEMOTEXT_PAGE* ptt = (MEMOTEXT_PAGE*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->pages = page;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int calc_memo_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data)
{
	MEMOTEXT_PAGE tt = { 0 };
	if_measure_t it = { 0 };

	tt.pages = 1;

	it.ctx = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	scan_memo_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _memo_text_calc_pages, (void*)&tt);

	return tt.pages;
}

typedef struct _RICHTEXT_DRAW{
	res_ctx_t rdc;
	int page;
}RICHTEXT_DRAW;

static int _rich_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	RICHTEXT_DRAW* ptt = (RICHTEXT_DRAW*)pp;
	xpoint_t pt;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		if (cur_char && *cur_char == _T('\n'))
			break;

		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		text_out_raw(ptt->rdc, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_rich_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	RICHTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.rdc = rdc;
	tt.page = page;

	it.ctx = (void*)rdc;
	it.pf_mm_points = (PF_PT_PER_MM)pt_per_mm;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	scan_rich_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _rich_text_calc_draw, (void*)&tt);
}

typedef struct _RICHTEXT_PAGE{
	int pages;
}RICHTEXT_PAGE;

static int _rich_text_calc_pages(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	RICHTEXT_PAGE* ptt = (RICHTEXT_PAGE*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->pages = page;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int calc_rich_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data)
{
	RICHTEXT_PAGE tt = { 0 };
	if_measure_t it = { 0 };

	tt.pages = 1;

	it.ctx = (void*)rdc;
	it.pf_mm_points = (PF_PT_PER_MM)pt_per_mm;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	scan_rich_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _rich_text_calc_pages, (void*)&tt);

	return tt.pages;
}

/*******************************************************************************************************/

void draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	res_ctx_t rdc;
	xpoint_t pt1, pt2;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	rdc = get_canvas_ctx(canv);

	xmem_copy((void*)&pt1, (void*)ppt1, sizeof(xpoint_t));
	xmem_copy((void*)&pt2, (void*)ppt2, sizeof(xpoint_t));

	point_tm_to_pt(canv, &pt1);
	point_tm_to_pt(canv, &pt2);

	(*pif->pf_gdi_draw_line)(rdc, pxp, &pt1, &pt2);
}

void draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xpoint_t* lpt;
	int i;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	lpt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * n);
	for (i = 0; i < n; i++)
	{
		xmem_copy((void*)&lpt[i], (void*)&ppt[i], sizeof(xpoint_t));
		point_tm_to_pt(canv, &lpt[i]);
	}

	(*pif->pf_gdi_draw_polyline)(rdc, pxp, lpt, n);

	xmem_free(lpt);
}

void draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xpoint_t* lpt;
	int i;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	lpt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * n);
	for (i = 0; i < n; i++)
	{
		xmem_copy((void*)&lpt[i], (void*)&ppt[i], sizeof(xpoint_t));
		point_tm_to_pt(canv, &lpt[i]);
	}

	(*pif->pf_gdi_draw_polygon)(rdc, pxp, pxb, lpt, n);

	xmem_free(lpt);
}

void draw_bezier(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	res_ctx_t rdc;
	xpoint_t pt1, pt2, pt3, pt4;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	rdc = get_canvas_ctx(canv);

	xmem_copy((void*)&pt1, (void*)ppt1, sizeof(xpoint_t));
	xmem_copy((void*)&pt2, (void*)ppt2, sizeof(xpoint_t));
	xmem_copy((void*)&pt3, (void*)ppt3, sizeof(xpoint_t));
	xmem_copy((void*)&pt4, (void*)ppt4, sizeof(xpoint_t));

	point_tm_to_pt(canv, &pt1);
	point_tm_to_pt(canv, &pt2);
	point_tm_to_pt(canv, &pt3);
	point_tm_to_pt(canv, &pt4);

	(*pif->pf_gdi_draw_bezier)(rdc, pxp, &pt1, &pt2, &pt3, &pt4);
}

void draw_curve(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	res_ctx_t rdc;
	xpoint_t* pa;
	int i;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	rdc = get_canvas_ctx(canv);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	for (i = 0; i < n; i++)
	{
		xmem_copy((void*)&pa[i], (void*)&ppt[i], sizeof(xpoint_t));
		point_tm_to_pt(canv, &pa[i]);
	}

	(*pif->pf_gdi_draw_curve)(rdc, pxp, pa, n);

	xmem_free(pa);
}

void draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_rect)(rdc, pxp, pxb, &xr);
}

void draw_path(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xpoint_t* ppt;
	int i, j;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	if (is_null(aa))
		return;

	ppt = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	xmem_copy((void*)ppt, (void*)pa, n * sizeof(xpoint_t));

	i = j = 0;
	while (*(aa + j))
	{
		if (*(aa + j) == _T('M') || *(aa + j) == _T('m'))
		{
			point_tm_to_pt(canv, &ppt[i]);
			i += 1;
		}
		else if (*(aa + j) == _T('L') || *(aa + j) == _T('l'))
		{
			point_tm_to_pt(canv, &ppt[i]);
			i += 1;
		}
		else if (*(aa + j) == _T('Q') || *(aa + j) == _T('q'))
		{
			point_tm_to_pt(canv, &ppt[i]);
			point_tm_to_pt(canv, &ppt[i + 1]);
			i += 2;
		}
		else if (*(aa + j) == _T('T') || *(aa + j) == _T('t'))
		{
			point_tm_to_pt(canv, &ppt[i]);
			i += 1;
		}
		else if (*(aa + j) == _T('C') || *(aa + j) == _T('c'))
		{
			point_tm_to_pt(canv, &ppt[i]);
			point_tm_to_pt(canv, &ppt[i + 1]);
			point_tm_to_pt(canv, &ppt[i + 2]);
			i += 3;
		}
		else if (*(aa + j) == _T('S') || *(aa + j) == _T('s'))
		{
			point_tm_to_pt(canv, &ppt[i]);
			point_tm_to_pt(canv, &ppt[i + 1]);
			i += 2;
		}
		else if (*(aa + j) == _T('A') || *(aa + j) == _T('a'))
		{
			point_tm_to_pt(canv, &ppt[i]);
			point_tm_to_pt(canv, &ppt[i + 1]);
			i += 2;
		}
		else if (*(aa + j) == _T('Z') || *(aa + j) == _T('z'))
		{
			break;
		}

		j++;
	}

	(*pif->pf_gdi_draw_path)(rdc, pxp, pxb, aa, ppt);

	xmem_free(ppt);
}

void gradient_rect(canvas_t canv, const xcolor_t* xc_brim, const xcolor_t* xc_core, const tchar_t* gradient, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_gradient_rect)(rdc, xc_brim, xc_core, gradient, &xr);
}

void alphablend_rect(canvas_t canv, const xcolor_t* pxc, const xrect_t* pxr, int opa)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_alphablend_rect)(rdc, pxc, &xr, opa);
}

void draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_round)(rdc, pxp, pxb, &xr);
}

void draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	(*pif->pf_gdi_draw_ellipse)(rdc, pxp, pxb, &xr);
}

void draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float rx, float ry, double fang, double tang)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xr.fx = ppt->fx;
	xr.fy = ppt->fy;
	xr.fw = rx;
	xr.fh = ry;
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_pie)(rdc, pxp, pxb, RECTPOINT(&xr), xr.w, xr.h, fang, tang);
}

void draw_arc(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, float rx, float ry, double fang, double tang)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xr.fx = ppt->fx;
	xr.fy = ppt->fy;
	xr.fw = rx;
	xr.fh = ry;
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_arc)(rdc, pxp, RECTPOINT(&xr), xr.w, xr.h, fang, tang);
}

void draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;
	xsize_t xs;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	xs.fx = alen;
	xs.fy = alen;
	size_tm_to_pt(canv, &xs);

	(*pif->pf_gdi_draw_arrow)(rdc, pxp, pxb, &xr, xs.cx, arc);
}

void draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_text)(rdc, pxf, pxa, &xr, txt, len);
}

void text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xpoint_t pt;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&pt, (void*)ppt, sizeof(xpoint_t));
	point_tm_to_pt(canv, &pt);

	(*pif->pf_gdi_text_out)(rdc, pxf, &pt, txt, len);
}

void text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	res_ctx_t rdc = get_canvas_ctx(canv);

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_text_size)(rdc, pxf, txt, len, pxs);

	size_pt_to_tm(canv, pxs);
}

void text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	res_ctx_t rdc = get_canvas_ctx(canv);

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_gdi_text_metric)(rdc, pxf, pxs);

	size_pt_to_tm(canv, pxs);
}

void draw_pass(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	tchar_t sz_pass[INT_LEN + 1] = { 0 };

	len = format_password(txt, sz_pass, INT_LEN);

	draw_text(canv, pxf, pxa, pxr, sz_pass, len);
}

void draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap)
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
			text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		draw_text(canv, pxf, &xa, pxr, sz_format, -1);
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

		draw_text(canv, pxf, &xa, pxr, sz_format, -1);
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

		draw_text(canv, pxf, &xa, pxr, sz_format, -1);
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

		draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else 
	{
		if (autowrap && is_null(xa.text_wrap))
		{
			text_size(canv, pxf, data, len, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		draw_text(canv, pxf, &xa, pxr, data, len);
	}
}

void draw_icon(canvas_t canv, const tchar_t* iname, const xrect_t* prt)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)prt, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_icon_raw(rdc, iname, &xr);
}

void draw_thumb(canvas_t canv, const tchar_t* fname, const xrect_t* prt)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)prt, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_thumb_raw(rdc, fname, &xr);
}

void draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_image_raw(rdc, pxi, &xr);
}

void draw_code128(canvas_t canv, const xcolor_t* pxc, xrect_t* pxr, const tchar_t* text, int len)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_code128_raw(rdc, pxc, &xr, text, len);

	rect_pt_to_tm(canv, &xr);
	xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
}

void draw_pdf417(canvas_t canv, const xcolor_t* pxc, xrect_t* pxr, const tchar_t* text, int len)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_pdf417_raw(rdc, pxc, &xr, text, len);

	rect_pt_to_tm(canv, &xr);
	xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
}

void draw_qrcode(canvas_t canv, const xcolor_t* pxc, xrect_t* pxr, const tchar_t* text, int len)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_qrcode_raw(rdc, pxc, &xr, text, len);

	rect_pt_to_tm(canv, &xr);
	xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
}

void image_size(canvas_t canv, const ximage_t* pxi, xsize_t* pxs)
{
	res_ctx_t rdc = get_canvas_ctx(canv);

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	image_size_raw(rdc, pxi, pxs);

	size_pt_to_tm(canv, pxs);
}

void text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr = { 0 };

	text_rect_raw(rdc, pxf, pxa, txt, len, &xr);

	rect_pt_to_tm(canv, &xr);
	xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
}

void draw_bitmap(canvas_t canv, res_bmp_t bmp, const xpoint_t* ppt)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xpoint_t pt;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&pt, (void*)ppt, sizeof(xpoint_t));
	point_tm_to_pt(canv, &pt);

	(*pif->pf_gdi_draw_bitmap)(rdc, bmp, &pt);
}

void exclip_rect(canvas_t canv, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_exclip_rect)(rdc, &xr);
}

void inclip_rect(canvas_t canv, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	if_context_t *pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_inclip_rect)(rdc, &xr);
}

void draw_shape(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* shape)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_shape_raw(rdc, pxp, pxb, &xr, shape);
}

void multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	multi_line_raw(rdc, pxf, pxa, pxp, &xr);
}

void draw_gizmo(canvas_t canv, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* iname)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_gizmo_raw(rdc, pxc, &xr, iname);
}



void color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	color_out_raw(rdc, &xr, horz, rgbstr, len);
}

void draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_var_text_raw(rdc, pxf, pxa, &xr, var);
}

void draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag, int page)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_tag_text_raw(rdc, pxf, pxa, &xr, tag, page);
}

int calc_tag_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	return calc_tag_pages_raw(rdc, pxf, pxa, &xr, tag);
}

void draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich, int page)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_rich_text_raw(rdc, pxf, pxa, &xr, rich, page);
}

int calc_rich_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	return calc_rich_pages_raw(rdc, pxf, pxa, &xr, rich);
}

void draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo, int page)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	draw_memo_text_raw(rdc, pxf, pxa, &xr, memo, page);
}

int calc_memo_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	return calc_memo_pages_raw(rdc, pxf, pxa, &xr, memo);
}

void draw_select(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	draw_rect(canv, &xp, NULL, prt);
}

void draw_focus(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);

	draw_rect(canv, &xp, NULL, prt);
}

void draw_feed(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpoint_t pt[2];
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);
	xp.adorn.feed = 2;
	xp.adorn.size = 2;

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx;
	pt[1].fy = (float)(prt->fy + 1.0);
	draw_line(canv, &xp, &pt[0], &pt[1]);

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = (float)(prt->fx + 1.0);
	pt[1].fy = prt->fy;
	draw_line(canv, &xp, &pt[0], &pt[1]);

	pt[0].fx = prt->fx + prt->fw;
	pt[0].fy = prt->fy + prt->fh;
	pt[1].fx = (float)(prt->fx + prt->fw - 1.0);
	pt[1].fy = prt->fy + prt->fh;
	draw_line(canv, &xp, &pt[0], &pt[1]);

	pt[0].fx = prt->fx + prt->fw;
	pt[0].fy = prt->fy + prt->fh;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = (float)(prt->fy + prt->fh - 1.0);
	draw_line(canv, &xp, &pt[0], &pt[1]);
}

void draw_progress(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int steps)
{
	int i,index;
	float iw;
	xrect_t xr;
	xpen_t xp;
	xbrush_t xb;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	//iw = prt->fw / 10;
	iw = DEF_SMALL_ICON * 2;
	
	index = steps % 10;
	if (!index && steps)
		index = 10;

	for (i = 0; i < 10; i++)
	{
		xr.fx = prt->fx + iw * i;
		xr.fw = iw;
		xr.fy = prt->fy;
		xr.fh = prt->fh;

		ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);

		if (index > i)
			draw_rect(canv, &xp, &xb, &xr);
		else
			draw_rect(canv, &xp, NULL, &xr);
	}
}

void draw_corner(canvas_t canv, const xcolor_t* pxc, const xrect_t* pxr)
{
	res_ctx_t rdc = get_canvas_ctx(canv);
	xrect_t xr;
	xsize_t xs;
	int SPAN;
	xpoint_t pt1, pt2;
	xpen_t xp = { 0 };

	xs.fx = xs.fy = DEF_CORNER_FEED;
	size_tm_to_pt(canv, &xs);
	SPAN = xs.cx;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);
	pt_expand_rect(&xr, 3, 3);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_SOLID);
	xscpy(xp.size, _T("2"));
	xp.adorn.feed = 2;
	xp.adorn.size = 2;

	pt1.x = xr.x;
	pt1.y = xr.y;
	pt2.x = xr.x;
	pt2.y = xr.y - SPAN;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x;
	pt1.y = xr.y;
	pt2.x = xr.x - SPAN;
	pt2.y = xr.y;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	//right,top
	pt1.x = xr.x + xr.w;
	pt1.y = xr.y;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y - SPAN;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w;
	pt1.y = xr.y;
	pt2.x = xr.x + xr.w + SPAN;
	pt2.y = xr.y;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	//left,bottom
	pt1.x = xr.x;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x;
	pt2.y = xr.y + xr.h + SPAN;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x - SPAN;
	pt2.y = xr.y + xr.h;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	//right,bottom
	pt1.x = xr.x + xr.w;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h + SPAN;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w + SPAN;
	pt2.y = xr.y + xr.h;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_ruler(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt)
{
	float x1, x2, y1, y2;
	float fw, fh, fx, fy;
	float MAX_SPAN = 5.0;
	float MIN_SPAN = 1.0;
	xrect_t xr;
	xpoint_t pt1, pt2;
	int i, j;
	int vm, hm;
	xpen_t xp;
	xfont_t xf;
	xface_t xa;
	tchar_t sz[NUM_LEN + 1];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xfont(&xf);
	xscpy(xf.size, _T("7"));
	xscpy(xf.weight, _T("100"));
	xscpy(xf.color, xp.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	draw_rect(canv, &xp, NULL, prt);

	fx = prt->fx;
	fy = prt->fy;
	fh = prt->fh;
	fw = prt->fw;

	//left edge
	if ((int)fh % 10)
		vm = (int)fh / 10;
	else
		vm = (int)fh / 10 - 1;

	x1 = fx;
	x2 = x1 - MAX_SPAN;
	y1 = fy;
	y2 = y1 + 2 * MAX_SPAN;
	for (i = 1; i < vm; i++)
	{
		y1 = y2;
		y2 = y1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x2;
		pt2.fy = y1;
		draw_line(canv, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1;
				pt1.fy = y1 + j * MIN_SPAN;
				pt2.fx = x2 + 2 * MIN_SPAN;
				pt2.fy = y1 + j * MIN_SPAN;
				//draw_line(canv, &xp, x1, y1 + j * 10, x2 + 10, y1 + j * 10);
				draw_line(canv, &xp, &pt1, &pt2);
			}
			//else
			//{
				//draw_line(canv, &xp, x1, y1 + j * 10, x2 + 20, y1 + j * 10);
			//}
		}

		xsprintf(sz, _T("%d"), i);

		xr.fx = x2;
		xr.fy = y1;
		xr.fw = MAX_SPAN;
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		draw_text(canv, &xf, &xa, &xr, sz, -1);

		if (i == vm - 1)
		{
			pt1.fx = x1;
			pt1.fy = y2;
			pt2.fx = x2;
			pt2.fy = y2;
			draw_line(canv, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x2;
			xr.fy = y2;
			xr.fw = MAX_SPAN;
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
			draw_text(canv, &xf, &xa, &xr, sz, -1);
		}
	}

	//right edge
	if ((int)fh % 10)
		vm = (int)fh / 10;
	else
		vm = (int)fh / 10 - 1;

	x1 = fx + fw;
	x2 = x1 + MAX_SPAN;
	y1 = fy;
	y2 = y1 + 2 * MAX_SPAN;
	for (i = 1; i < vm; i++)
	{
		y1 = y2;
		y2 = y1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x2;
		pt2.fy = y1;
		draw_line(canv, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1;
				pt1.fy = y1 + j * MIN_SPAN;
				pt2.fx = x2 - 2 * MIN_SPAN;
				pt2.fy = y1 + j * MIN_SPAN;
				//draw_line(canv, &xp, x1, y1 + j * 10, x2 - 10, y1 + j * 10);
				draw_line(canv, &xp, &pt1, &pt2);
			}
			//else
			//{
				//draw_line(canv, &xp, x1, y1 + j * 10, x2 - 20, y1 + j * 10);
			//}
		}

		xsprintf(sz, _T("%d"), i);

		xr.fx = x1;
		xr.fy = y1;
		xr.fw = MAX_SPAN;
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);
		draw_text(canv, &xf, &xa, &xr, sz, -1);

		if (i == vm - 1)
		{
			pt1.fx = x1;
			pt1.fy = y2;
			pt2.fx = x2;
			pt2.fy = y2;
			draw_line(canv, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x1;
			xr.fy = y2;
			xr.fw = MAX_SPAN;
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);
			draw_text(canv, &xf, &xa, &xr, sz, -1);
		}
	}

	//top edge
	if ((int)fw % 10)
		hm = (int)fw / 10;
	else
		hm = (int)fw / 10 - 1;

	x1 = fx;
	x2 = x1 + 2 * MAX_SPAN;
	y1 = fy;
	y2 = y1 - MAX_SPAN;
	for (i = 1; i < hm; i++)
	{
		x1 = x2;
		x2 = x1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x1;
		pt2.fy = y2;
		draw_line(canv, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1 + j * MIN_SPAN;
				pt1.fy = y1;
				pt2.fx = x1 + j * MIN_SPAN;
				pt2.fy = y2 + 2 * MIN_SPAN;
				//draw_line(canv, &xp, x1 + j * 10, y1, x1 + j * 10, y2 + 10);
				draw_line(canv, &xp, &pt1, &pt2);
			}
			//else
			//{
				//draw_line(canv, &xp, x1 + j * 10, y1, x1 + j * 10, y2 + 20);
			//}
		}

		xsprintf(sz, _T("%d"), i);
		xr.fx = x1;
		xr.fy = y2;
		xr.fw = (float)(MAX_SPAN - 0.5);
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		draw_text(canv, &xf, &xa, &xr, sz, -1);

		if (i == hm - 1)
		{
			pt1.fx = x2;
			pt1.fy = y1;
			pt2.fx = x2;
			pt2.fy = y2;
			draw_line(canv, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x2;
			xr.fy = y2;
			xr.fw = (float)(MAX_SPAN - 0.5);
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
			draw_text(canv, &xf, &xa, &xr, sz, -1);
		}
	}

	//bottom edge
	if ((int)fw % 10)
		hm = (int)fw / 10;
	else
		hm = (int)fw / 10 - 1;

	x1 = fx;
	x2 = x1 + 2 * MAX_SPAN;
	y1 = fy + fh;
	y2 = y1 + MAX_SPAN;
	for (i = 1; i < hm; i++)
	{
		x1 = x2;
		x2 = x1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x1;
		pt2.fy = y2;
		draw_line(canv, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1 + j * MIN_SPAN;
				pt1.fy = y1;
				pt2.fx = x1 + j * MIN_SPAN;
				pt2.fy = y2 - 2 * MIN_SPAN;
				//draw_line(canv, &xp, x1 + j * 10, y1, x1 + j * 10, y2 - 10);
				draw_line(canv, &xp, &pt1, &pt2);
			}
			//else
			//{
				//draw_line(canv, &xp, x1 + j * 10, y1, x1 + j * 10, y2 - 20);
			//}
		}

		xsprintf(sz, _T("%d"), i);
		xr.fx = x1;
		xr.fy = (float)(y1 + MAX_SPAN / 2 - 0.5);
		xr.fw = MAX_SPAN;
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		draw_text(canv, &xf, &xa, &xr, sz, -1);

		if (i == hm - 1)
		{
			pt1.fx = x2;
			pt1.fy = y1;
			pt2.fx = x2;
			pt2.fy = y2;
			draw_line(canv, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x2;
			xr.fy = (float)(y1 + MAX_SPAN / 2 - 0.5);
			xr.fw = MAX_SPAN;
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
			draw_text(canv, &xf, &xa, &xr, sz, -1);
		}
	}
}

void draw_svg(canvas_t canv, const xrect_t* pbox, link_t_ptr ptr)
{
	link_t_ptr ilk;
	int count;
	xrect_t xr = { 0 };
	xpoint_t pt[2];
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };
	const tchar_t* sz_name;
	const tchar_t* sz_text;
	double fang, tang;
	xpoint_t* ppt;
	int i;
	bool_t b_round;
	int rx, ry;
	canvas_t svgcanv;

	link_t_ptr st = NULL;

	int an, pn;
	tchar_t* aa = NULL;
	xpoint_t* pa = NULL;

	svgcanv = create_svg_canvas(ptr);

	ilk = get_svg_first_child_node(ptr);
	while (ilk)
	{
		sz_name = get_svg_node_name_ptr(ilk);

		if (compare_text(sz_name, -1, SVG_NODE_LINE, -1, 1) == 0)
		{
			xmem_zero((void*)&xp, sizeof(xpen_t));
			read_line_from_svg_node(ilk, &xp, &pt[0], &pt[1]);
			svg_point_pt_to_tm(svgcanv, &pt[0]);
			svg_point_pt_to_tm(svgcanv, &pt[1]);

			pt[0].fx += pbox->fx;
			pt[0].fy += pbox->fy;
			pt[1].fx += pbox->fx;
			pt[1].fy += pbox->fy;
			draw_line(canv, &xp, &pt[0], &pt[1]);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_RECT, -1, 1) == 0)
		{
			xmem_zero((void*)&xp, sizeof(xpen_t));
			xmem_zero((void*)&xb, sizeof(xbrush_t));
			b_round = svg_node_is_round(ilk);
			if (b_round)
				read_round_from_svg_node(ilk, &xp, &xb, &xr, &rx, &ry);
			else
				read_rect_from_svg_node(ilk, &xp, &xb, &xr);

			svg_rect_pt_to_tm(svgcanv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			if (b_round)
				draw_round(canv, &xp, &xb, &xr);
			else
				draw_rect(canv, &xp, &xb, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_ELLIPSE, -1, 1) == 0)
		{
			xmem_zero((void*)&xp, sizeof(xpen_t));
			xmem_zero((void*)&xb, sizeof(xbrush_t));
			read_ellipse_from_svg_node(ilk, &xp, &xb, &xr);
			svg_rect_pt_to_tm(svgcanv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			draw_ellipse(canv, &xp, &xb, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_POLYLINE, -1, 1) == 0)
		{
			count = read_polyline_from_svg_node(ilk, NULL, NULL, MAX_LONG);
			if (count)
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));

				ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * count);
				read_polyline_from_svg_node(ilk, &xp, ppt, count);
				for (i = 0; i < count; i++)
				{
					svg_point_pt_to_tm(svgcanv, &ppt[i]);

					ppt[i].fx += pbox->fx;
					ppt[i].fy += pbox->fy;
				}
				draw_polyline(canv, &xp, ppt, count);

				xmem_free(ppt);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_POLYGON, -1, 1) == 0)
		{
			count = read_polygon_from_svg_node(ilk, NULL, NULL, NULL, MAX_LONG);
			if (count)
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));

				ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * count);
				read_polygon_from_svg_node(ilk, &xp, &xb, ppt, count);
				for (i = 0; i < count; i++)
				{
					svg_point_pt_to_tm(svgcanv, &ppt[i]);

					ppt[i].fx += pbox->fx;
					ppt[i].fy += pbox->fy;
				}
				draw_polygon(canv, &xp, &xb, ppt, count);

				xmem_free(ppt);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_TEXT, -1, 1) == 0)
		{
			xmem_zero((void*)&xf, sizeof(xfont_t));
			xmem_zero((void*)&xa, sizeof(xface_t));
			sz_text = read_text_from_svg_node(ilk, &xf, &xa, &xr);
			svg_rect_pt_to_tm(svgcanv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			if (xr.h)
			{
				draw_text(canv, &xf, &xa, &xr, sz_text, -1);
			}
			else
			{
				pt[0].fx = xr.fx;
				pt[0].fy = xr.fy;
				text_out(canv, &xf, &pt[0], sz_text, -1);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_PATH, -1, 1) == 0)
		{
			if (svg_node_is_pie(ilk))
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));
				read_pie_from_svg_node(ilk, &xp, &xb, RECTPOINT(&xr),&xr.w, &xr.h, &fang, &tang);

				svg_rect_pt_to_tm(svgcanv, &xr);
				xr.fx += pbox->fx;
				xr.fy += pbox->fy;

				draw_pie(canv, &xp, &xb, RECTPOINT(&xr), xr.fw, xr.fh, fang, tang);
			}
			else if (svg_node_is_arc(ilk))
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				read_arc_from_svg_node(ilk, &xp, RECTPOINT(&xr), &xr.w, &xr.h, &fang, &tang);

				svg_rect_pt_to_tm(svgcanv, &xr);
				xr.fx += pbox->fx;
				xr.fy += pbox->fy;

				draw_arc(canv, &xp, RECTPOINT(&xr), xr.fw, xr.fh, fang, tang);
			}
			else
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));
				read_path_from_svg_node(ilk, &xp, &xb, NULL, &an, NULL, &pn);

				svg_rect_pt_to_tm(svgcanv, &xr);
				xr.fx += pbox->fx;
				xr.fy += pbox->fy;

				draw_path(canv, &xp, &xb, aa, pa, pn);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_IMAGE, -1, 1) == 0)
		{
			xmem_zero((void*)&xi, sizeof(ximage_t));
			read_ximage_from_svg_node(ilk, &xi, &xr);

			svg_rect_pt_to_tm(svgcanv, &xr);
			xr.fx += pbox->fx;
			xr.fy += pbox->fy;

			draw_image(canv, &xi, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_IMAGE, -1, 1) == 0)
		{
			xmem_zero((void*)&xi, sizeof(ximage_t));
			read_ximage_from_svg_node(ilk, &xi, &xr);

			svg_rect_pt_to_tm(svgcanv, &xr);
			xr.fx += pbox->fx;
			xr.fy += pbox->fy;

			draw_image(canv, &xi, &xr);
		}

		if (get_svg_first_child_node(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_svg_first_child_node(ilk);
			continue;

		}

		while (ilk)
		{
			ilk = get_svg_next_sibling_node(ilk);
			if (ilk)
				break;
			ilk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);

	destroy_svg_canvas(svgcanv);
}

#endif /*XDU_SUPPORT_CONTEXT*/
