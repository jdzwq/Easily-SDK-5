/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc cairo document

	@module	if_cairo_cairo.c | cairo cairo implement file

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

#include "xduiml.h"
#include "xduutil.h"

#ifdef XDU_SUPPORT_CONTEXT_CAIRO

static void DPtoLP(res_ctx_t rdc, XPoint* pt,int n)
{
	int i;
	for(i = 0;i<n;i++)
	{
		pt[i].x = pt[i].x;
		pt[i].y = pt[i].y;
	}
}

static void _adjust_rect(XRectangle* prt, int src_width, int src_height, const tchar_t* horz_align, const tchar_t* vert_align)
{
	if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && xscmp(vert_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		prt->width = (prt->width < src_width) ? prt->width : src_width;
		prt->height = (prt->height < src_height) ? prt->height : src_height;
	}
	else if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0 && xscmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		prt->x = (prt->width < src_width) ? prt->x : (prt->x + prt->width - src_width);
		prt->height = (prt->height < src_height) ? prt->height : src_height;
	}
	else if (xscmp(horz_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && xscmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		prt->width = (prt->width < src_width) ? prt->width : src_width;
		prt->y = (prt->height < src_height) ? prt->y : (prt->y + prt->height - src_height);
	}
	else if (xscmp(horz_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0 && xscmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		prt->x = (prt->width < src_width) ? prt->x : (prt->x + prt->width - src_width);
		prt->y = (prt->height < src_height) ? prt->y : (prt->y + prt->height - src_height);
	}
	else if (xscmp(horz_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0 && xscmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		if (prt->width > src_width)
		{
			prt->x = prt->x + (prt->width - src_width) / 2;
			prt->width = src_width;
		}
		if (prt->height > src_height)
		{
			prt->y = prt->y + (prt->height - src_height) / 2;
			prt->height = src_height;
		}
	}
	else if (xscmp(horz_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && xscmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		prt->width = (prt->width < src_width) ? prt->width : src_width;
		prt->y = (prt->height < src_height) ? prt->y : (prt->y + (prt->height - src_height) / 2);
		prt->height = (prt->height < src_height) ? prt->height : src_height;
	}
	else if (xscmp(horz_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0 && xscmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		prt->x = (prt->width < src_width) ? prt->x : (prt->x + prt->width - src_width);
		prt->y = (prt->height < src_height) ? prt->y : (prt->y + (prt->height - src_height) / 2);
		prt->height = (prt->height < src_height) ? prt->height : src_height;
	}
}

/************************************************************************************************/

void _cairo_init(int osv)
{

}

void _cairo_uninit(void)
{
	
}

void _cairo_draw_line(res_ctx_t rdc,const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
    X11_context_t* ctx = (X11_context_t*)rdc;
	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};
    
	XPoint pt[2];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(rdc,pt,2);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);
	}

	cairo_move_to(cai, pt[0].x, pt[0].y);
    cairo_line_to(cai, pt[1].x, pt[1].y);

    cairo_stroke(cai);

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_draw_polyline(res_ctx_t rdc,const xpen_t* pxp,const xpoint_t* ppt,int n)
{
	X11_context_t* ctx = (X11_context_t*)rdc;
	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};
    
	XPoint* pa;
	int i;
	
	if(!n) return;

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	pa = (XPoint*)calloc(n, sizeof(XPoint));
	for(i =0;i<n;i++)
	{
		pa[i].x = ppt[i].x;
		pa[i].y = ppt[i].y;
	}
	DPtoLP(rdc,pa,n);

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);
	}

	cairo_move_to(cai, pa[0].x, pa[0].y);

	for(i =1;i<n;i++)
	{
		cairo_line_to(cai, pa[i].x, pa[i].y);
		cairo_move_to(cai, pa[i].x, pa[i].y);
	}

	free(pa);

	cairo_stroke(cai);

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_draw_polygon(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int n)
{
    X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};

	XPoint* pa;
	int i;

	pa = (XPoint*)calloc(n + 1, sizeof(XPoint));
	for(i =0;i<n;i++)
	{
		pa[i].x = ppt[i].x;
		pa[i].y = ppt[i].y;
	}
	pa[n].x = ppt[0].x;
	pa[n].y = ppt[0].y;

	DPtoLP(rdc,pa,n + 1);

	cairo_new_path(cai);

	cairo_move_to(cai, pa[0].x, pa[0].y);

	for(i =1;i<n;i++)
	{
		cairo_line_to(cai, pa[i].x, pa[i].y);
		cairo_move_to(cai, pa[i].x, pa[i].y);
	}

	cairo_close_path(cai);

	free(pa);

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_fill(cai);
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);

		cairo_stroke(cai);
	}

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_draw_bezier(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	X11_context_t* ctx = (X11_context_t*)rdc;
	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};
    
	XPoint pt[4];
	
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;
	pt[2].x = ppt3->x;
	pt[2].y = ppt3->y;
	pt[3].x = ppt4->x;
	pt[3].y = ppt4->y;

	DPtoLP(rdc,pt,4);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);
	}

	cairo_move_to(cai, pt[0].x, pt[0].y);

	cairo_curve_to(cai, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);

	cairo_stroke(cai);

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_draw_curve(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
    
}

void _cairo_draw_path(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa)
{

}

void _cairo_gradient_rect(res_ctx_t rdc, const xgradi_t* pxg, const xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;
	cairo_pattern_t* pat;

	xcolor_t xc_core, xc_brim;

	XPoint pt[2];
    
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(rdc,pt,2);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	parse_xcolor(&xc_core, pxg->core_color);
	parse_xcolor(&xc_brim, pxg->brim_color);

	pat = cairo_pattern_create_linear(pt[0].x, pt[0].y , pt[0].x , pt[1].y) ;

	cairo_pattern_add_color_stop_rgb (pat, 0.0, xc_brim.r / 255.0 , xc_brim.g / 255.0 , xc_brim.b / 255.0);
	cairo_pattern_add_color_stop_rgb (pat, 0.5, xc_core.r / 255.0 , xc_core.g / 255.0 , xc_core.b / 255.0) ;
	cairo_pattern_add_color_stop_rgb (pat, 1.0, xc_brim.r / 255.0 , xc_brim.g / 255.0 , xc_brim.b / 255.0);

	cairo_rectangle (cai, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y) ;

	cairo_set_source (cai, pat);

	cairo_fill (cai);

	cairo_pattern_destroy (pat);

	cairo_destroy(cai);

	cairo_surface_destroy(fac);
}

void _cairo_alphablend_rect(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	XPoint pt[2];
    
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(rdc,pt,2);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_set_source_rgba(cai, pxc->r / 255.0, pxc->g / 255.0, pxc->b / 255.0, opacity / 255.0);

	cairo_rectangle(cai, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	cairo_fill(cai);

	cairo_destroy(cai);

	cairo_surface_destroy(fac);
}

void _cairo_draw_rect(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};

	XPoint pt[2];
    
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(rdc,pt,2);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);

		cairo_rectangle(cai, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

		cairo_stroke(cai);
	}

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_rectangle(cai, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

		cairo_fill(cai);
	}

	cairo_destroy(cai);

	cairo_surface_destroy(fac);
}

void _cairo_draw_round(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};

	XPoint pa[9];
	int i;
	int r;

	r = (prt->w) / 10;
	if (r < 1)
		r = 1;
	else if (r > 6)
		r = 6;

	pa[0].x = prt->x;
	pa[0].y = prt->y + r;
	pa[1].x = prt->x + r;
	pa[1].y = prt->y;
	pa[2].x = prt->x + prt->w - r;
	pa[2].y = prt->y;
	pa[3].x = prt->x + prt->w;
	pa[3].y = prt->y + r;
	pa[4].x = prt->x + prt->w;
	pa[4].y = prt->y + prt->h - r;
	pa[5].x = prt->x + prt->w - r;
	pa[5].y = prt->y + prt->h;
	pa[6].x = prt->x + r;
	pa[6].y = prt->y + prt->h;
	pa[7].x = prt->x;
	pa[7].y = prt->y + prt->h - r;
	pa[8].x = prt->x;
	pa[8].y = prt->y + r;

	DPtoLP(rdc,pa,9);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_move_to(cai, pa[0].x, pa[0].y);

	for(i =1;i<9;i++)
	{
		cairo_line_to(cai, pa[i].x, pa[i].y);
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);

		cairo_stroke_preserve(cai);
	}

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_fill(cai);
	}

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_draw_ellipse(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};

	XPoint pt[2];
    
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(rdc,pt,2);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_arc (cai, (pt[0].x + pt[1].x) / 2, (pt[0].y + pt[1].y) / 2, (pt[1].x - pt[0].x) / 2, 0, 2 * XPI);

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_fill_preserve(cai);
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);

		cairo_stroke(cai);
	}

	cairo_destroy(cai);

	cairo_surface_destroy(fac);	
}

void _cairo_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};

	XPoint pt[2];
    
	pt[0].x = ppt->x - rx;
	pt[0].y = ppt->y - ry;
	pt[1].x = ppt->x + rx;
	pt[1].y = ppt->y + ry;

	DPtoLP(rdc,pt,2);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_move_to(cai, (pt[0].x + pt[1].x) / 2, (pt[0].y + pt[1].y) / 2);

	cairo_arc(cai, (pt[0].x + pt[1].x) / 2, (pt[0].y + pt[1].y) / 2, (pt[1].x - pt[0].x) / 2, fang, tang);

	cairo_line_to(cai, (pt[0].x + pt[1].x) / 2, (pt[0].y + pt[1].y) / 2);

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_fill_preserve(cai);
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);

		cairo_stroke(cai);
	}

	cairo_destroy(cai);

	cairo_surface_destroy(fac);	
}

void _cairo_draw_arc(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t * ppt, int rx, int ry, double fang, double tang)
{
   X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};

	XPoint pt[2];
    
	pt[0].x = ppt->x - rx;
	pt[0].y = ppt->y - ry;
	pt[1].x = ppt->x + rx;
	pt[1].y = ppt->y + ry;

	DPtoLP(rdc,pt,2);

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_arc(cai, (pt[0].x + pt[1].x) / 2, (pt[0].y + pt[1].y) / 2, (pt[1].x - pt[0].x) / 2, fang, tang);

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);
	}

	cairo_stroke(cai);

	cairo_destroy(cai);

	cairo_surface_destroy(fac);
}

void _cairo_draw_arrow(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt,int alen,double arc)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int l_w;
	double dash_dotted[2] = {1.0, 1.0};
	double dash_dashed[2] = {2.0, 1.0};

	double a1;
	int x_line0, y_line0, x_line1, y_line1, x_line2, y_line2;
	int x1, x2, y1, y2;
	XPoint pa[4];
	int i;

	pa[0].x = prt->x;
	pa[0].y = prt->y;
	pa[1].x = prt->x + prt->w;
	pa[1].y = prt->y + prt->h;

	DPtoLP(rdc, pa, 2);

	x1 = pa[0].x;
	y1 = pa[0].y;
	x2 = pa[1].x;
	y2 = pa[1].y;

	pa[0].x = x2;
	pa[0].y = y2;

	a1 = atan2((float)(y2 - y1), (float)(x2 - x1));
	x_line0 = (int)((float)x2 - (float)alen * cos(a1));
	y_line0 = (int)((float)y2 - (float)alen * sin(a1));

	x_line1 = x2 + (int)((float)(x_line0 - x2) * cos(arc) - (float)(y_line0 - y2) * sin(arc));
	y_line1 = y2 + (int)((float)(x_line0 - x2) * sin(arc) + (float)(y_line0 - y2) * cos(arc));
	pa[1].x = x_line1;
	pa[1].y = y_line1;

	x_line2 = x2 + (int)((float)(x_line0 - x2) * cos(-arc) - (float)(y_line0 - y2) * sin(-arc));
	y_line2 = y2 + (int)((float)(x_line0 - x2) * sin(-arc) + (float)(y_line0 - y2) * cos(-arc));
	pa[2].x = x_line2;
	pa[2].y = y_line2;

	pa[3].x = x2;
	pa[3].y = y2;

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_move_to(cai, pa[0].x, pa[0].y);

	for(i =1;i<4;i++)
	{
		cairo_line_to(cai, pa[i].x, pa[i].y);
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			cairo_set_dash(cai, dash_dotted, 2, 0);
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			cairo_set_dash(cai, dash_dashed, 2, 0);

		l_w = xstol(pxp->size);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_set_line_width(cai, l_w);

		cairo_stroke_preserve(cai);
	}

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		cairo_set_source_rgb(cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

		cairo_fill(cai);
	}

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_draw_text(res_ctx_t rdc,const xfont_t* pxf,const xface_t* pxa,const xrect_t* prt,const tchar_t* txt,int len)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int n_weight, n_size;

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	n_weight = xstol(pxf->weight);
	n_size = xstol(pxf->size);

	parse_xcolor(&xc,pxf->color);

	cairo_set_source_rgb (cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

	if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		if(n_weight > 500)
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
		else
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
	}else
	{
		if(n_weight > 500)
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		else
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	}
 
	cairo_set_font_size (cai, n_size); 
	
	cairo_move_to (cai, prt->x, prt->y); 

	cairo_show_text (cai, txt); 

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_text_out(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	xcolor_t xc = {0};
	int n_weight, n_size;

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	n_weight = xstol(pxf->weight);
	n_size = xstol(pxf->size);

	parse_xcolor(&xc,pxf->color);

	cairo_set_source_rgb (cai, xc.r / 255.0, xc.g / 255.0, xc.b / 255.0);

	if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		if(n_weight > 500)
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
		else
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
	}else
	{
		if(n_weight > 500)
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		else
			cairo_select_font_face(cai, pxf->family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	}
 
	cairo_set_font_size (cai, n_size); 
	
	cairo_move_to(cai, ppt->x, ppt->y); 

	cairo_show_text(cai, txt); 

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_exclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	
}

void _cairo_inclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	
}

void _cairo_text_rect(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	cairo_text_extents_t te; 

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_text_extents (cai, txt, &te); 

	prt->w = te.width;
	prt->h = te.height;

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_text_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	cairo_text_extents_t te; 

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_text_extents (cai, txt, &te); 

	pxs->cx = te.width;
	pxs->cy = te.height;

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

void _cairo_text_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	cairo_t* cai;
	cairo_surface_t* fac;

	cairo_font_extents_t te; 

	fac = cairo_xlib_surface_create(g_display, ctx->device, ctx->visual, ctx->width, ctx->height);
	if(!fac)
		return;
	
	cai = cairo_create(fac);
	if(!cai)
	{
		cairo_surface_destroy(fac);
		return;
	}

	cairo_font_extents (cai, &te); 

	pxs->cx = te.max_x_advance;
	pxs->cy = te.max_y_advance;

	cairo_surface_destroy(fac);

	cairo_destroy(cai);
}

#ifdef XDU_SUPPORT_CONTEXT_BITMAP
void _cairo_draw_image(res_ctx_t rdc, res_bmp_t hbmp, const tchar_t* clr, const xrect_t* prt)
{
    X11_context_t* ctx = (X11_context_t*)rdc;
	XImage* pmi = (XImage*)hbmp;

	XRectangle xr;
	XPoint pt[2];

	xr.x = prt->x;
	xr.y = prt->y;
	xr.width = prt->w;
	xr.height = prt->h;

	_adjust_rect(&xr, pmi->width, pmi->height, GDI_ATTR_TEXT_ALIGN_CENTER, GDI_ATTR_TEXT_ALIGN_CENTER);
    
	pt[0].x = xr.x;
	pt[0].y = xr.y;
	pt[1].x = xr.x + xr.width;
	pt[1].y = xr.y + xr.height;

	DPtoLP(ctx,pt,2);

	XPutImage(g_display, rdc->device, rdc->context, pmi, 0, 0, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);
}

void _cairo_draw_bitmap(res_ctx_t rdc, res_bmp_t hbmp, const xpoint_t* ppt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	XImage* pmi = (XImage*)hbmp;

	XPoint pt[1];
    
	pt[0].x = ppt->x;
	pt[0].y = ppt->y;

	XPutImage(g_display, ctx->device, ctx->context, pmi, 0, 0, pt[0].x, pt[0].y, pmi->width, pmi->height);
}

#endif

#ifdef XDU_SUPPORT_CONTEXT_REGION
void _cairo_fill_region(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn)
{
    
}
#endif

#endif //XDU_SUPPORT_CONTEXT_GRAPHIC

