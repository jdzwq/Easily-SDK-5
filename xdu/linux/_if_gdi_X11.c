/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi document

	@module	if_gdi_win.c | gdi windows implement file

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

#ifdef XDU_SUPPORT_CONTEXT_GRAPHIC

#define XRGB(ch) (unsigned short)((double)ch * 65535.0 / 256.0)

static void DPtoLP(res_ctx_t rdc, XPoint* pt,int n)
{
	int i;
	for(i = 0;i<n;i++)
	{
		pt[i].x = pt[i].x;
		pt[i].y = pt[i].y;
	}
}

static void _alphablend_rect(res_ctx_t rdc, const xbrush_t* pxb, const XRectangle* prt)
{

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

void _gdi_init(int osv)
{

}

void _gdi_uninit(void)
{
	
}

void _gdi_draw_line(res_ctx_t rdc,const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
    X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	unsigned long l_for;
	XColor ext, clr_pen = {0};
    
	XPoint pt[2];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(rdc,pt,2);

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		l_for = clr_pen.pixel;

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);
	}else
	{
		l_for = BlackPixel(g_display, DefaultScreen(g_display));
		l_w = 1;
		l_s = LineSolid;
	}

	XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapRound, JoinRound);

    XSetForeground(g_display, ctx->context, l_for);
    
    XDrawLine(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x, pt[1].y);

	if(clr_pen.pixel)
	{
		XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
	}
}

void _gdi_draw_polyline(res_ctx_t rdc,const xpen_t* pxp,const xpoint_t* ppt,int n)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	unsigned long l_p;
	XColor ext, clr_pen = {0};
    
	XPoint* pa;
	int i;
	
	if(!n) return;

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

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		l_p = clr_pen.pixel;

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);
	}else
	{
		l_p = BlackPixel(g_display, DefaultScreen(g_display));
		l_w = 1;
		l_s = LineSolid;
	}

	XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapRound, JoinRound);

    XSetForeground(g_display, ctx->context, l_p);
    
    XDrawLines(g_display, ctx->device, ctx->context, pa, n, CoordModePrevious);

	free(pa);

	if(clr_pen.pixel)
	{
		XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
	}
}

void _gdi_draw_polygon(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int n)
{
    X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_brush = {0}, clr_pen = {0};

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

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		clr_brush.red = XRGB(xc.r);
		clr_brush.green = XRGB(xc.g);
		clr_brush.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_brush);

		XSetForeground(g_display, ctx->context, clr_brush.pixel);

		XFillPolygon(g_display, ctx->device, ctx->context, pa, n + 1, Nonconvex, CoordModePrevious);

		if(clr_brush.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_brush.pixel), 1, 0);
		}
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);

		XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapNotLast, JoinMiter);

		XSetForeground(g_display, ctx->context, clr_pen.pixel);
		
		XDrawLines(g_display, ctx->device, ctx->context, pa, n + 1, CoordModePrevious);

		if(clr_pen.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
		}
	}

	free(pa);
}

void _gdi_draw_bezier(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
    
}

void _gdi_draw_curve(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
    
}

void _gdi_draw_path(res_ctx_t rdc, const xpen_t* pxp, const tchar_t* str, int len)
{
}

void _gdi_gradient_rect(res_ctx_t rdc, const xgradi_t* pxg, const xrect_t* prt)
{
	
}

void _gdi_alphablend_rect(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity)
{
	
}

void _gdi_draw_rect(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_brush = {0}, clr_pen = {0};

	XPoint pt[2];
    
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(rdc,pt,2);

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		clr_brush.red = XRGB(xc.r);
		clr_brush.green = XRGB(xc.g);
		clr_brush.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_brush);

		XSetForeground(g_display, ctx->context, clr_brush.pixel);

		XFillRectangle(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

		if(clr_brush.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_brush.pixel), 1, 0);
		}
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);

		XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapNotLast, JoinMiter);

		XSetForeground(g_display, ctx->context, clr_pen.pixel);
		
		XDrawRectangle(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

		if(clr_pen.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
		}
	}
}

void _gdi_draw_round(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_brush = {0}, clr_pen = {0};

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
	pa[4].x = prt->x;
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

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		clr_brush.red = XRGB(xc.r);
		clr_brush.green = XRGB(xc.g);
		clr_brush.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_brush);

		XSetForeground(g_display, ctx->context, clr_brush.pixel);

		XFillPolygon(g_display, ctx->device, ctx->context, pa, 9, Nonconvex, CoordModePrevious);

		if(clr_brush.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_brush.pixel), 1, 0);
		}
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);

		XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapNotLast, JoinMiter);

		XSetForeground(g_display, ctx->context, clr_pen.pixel);
		
		XDrawLines(g_display, ctx->device, ctx->context, pa, 9, CoordModePrevious);

		if(clr_pen.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
		}
	}
}

void _gdi_draw_ellipse(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_brush = {0}, clr_pen = {0};

	XPoint pt[2];
    
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(rdc,pt,2);

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		clr_brush.red = XRGB(xc.r);
		clr_brush.green = XRGB(xc.g);
		clr_brush.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_brush);

		XSetForeground(g_display, ctx->context, clr_brush.pixel);

		XFillArc(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y, 0, 360 * 64);

		if(clr_brush.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_brush.pixel), 1, 0);
		}
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);

		XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapNotLast, JoinMiter);

		XSetForeground(g_display, ctx->context, clr_pen.pixel);
		
		XDrawArc(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y, 0, 360 * 64);

		if(clr_pen.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
		}
	}
}

void _gdi_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_brush = {0}, clr_pen = {0};

	XPoint pt[2];

	int fdeg, tdeg;
    
	pt[0].x = ppt->x - rx;
	pt[0].y = ppt->y - ry;
	pt[1].x = ppt->x + rx;
	pt[1].y = ppt->y + ry;

	DPtoLP(rdc,pt,2);

	fdeg = (int)(fang / XPI * 180 * 64);
	tdeg = (int)((tang - fang) / XPI * 180 * 64);

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		clr_brush.red = XRGB(xc.r);
		clr_brush.green = XRGB(xc.g);
		clr_brush.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_brush);

		XSetForeground(g_display, ctx->context, clr_brush.pixel);

		XFillArc(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y, fdeg, tdeg);

		if(clr_brush.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_brush.pixel), 1, 0);
		}
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);

		XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapNotLast, JoinMiter);

		XSetForeground(g_display, ctx->context, clr_pen.pixel);
		
		XDrawArc(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y, fdeg, tdeg);

		if(clr_pen.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
		}
	}
}

void _gdi_draw_arc(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t * ppt, int rx, int ry, double fang, double tang)
{
    X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_brush = {0}, clr_pen = {0};
	unsigned long l_p;

	XPoint pt[2];

	int fdeg, tdeg;
    
	pt[0].x = ppt->x - rx;
	pt[0].y = ppt->y - ry;
	pt[1].x = ppt->x + rx;
	pt[1].y = ppt->y + ry;

	DPtoLP(rdc,pt,2);

	fdeg = (int)(fang / XPI * 180 * 64);
	tdeg = (int)((tang - fang) / XPI * 180 * 64);

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);
		l_p = clr_pen.pixel;
	}else
	{
		l_s = LineSolid;
		l_w = 1;
		l_p = BlackPixel(g_display, DefaultScreen(g_display));
	}
	
	XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapNotLast, JoinMiter);

	XSetForeground(g_display, ctx->context, l_p);
	
	XDrawArc(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y, fdeg, tdeg);

	if(clr_pen.pixel)
	{
		XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
	}
}

void _gdi_draw_arrow(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt,int alen,double arc)
{
	X11_context_t* ctx = (X11_context_t*)rdc;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_brush = {0}, clr_pen = {0};

	double a1;
	int x_line0, y_line0, x_line1, y_line1, x_line2, y_line2;
	int x1, x2, y1, y2;
	XPoint pa[4];

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

	if(pxb)
	{
		parse_xcolor(&xc,pxb->color);

		clr_brush.red = XRGB(xc.r);
		clr_brush.green = XRGB(xc.g);
		clr_brush.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_brush);

		XSetForeground(g_display, ctx->context, clr_brush.pixel);

		XFillPolygon(g_display, ctx->device, ctx->context, pa, 4, Nonconvex, CoordModePrevious);

		if(clr_brush.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_brush.pixel), 1, 0);
		}
	}

	if(pxp)
	{
		parse_xcolor(&xc,pxp->color);

		clr_pen.red = XRGB(xc.r);
		clr_pen.green = XRGB(xc.g);
		clr_pen.blue = XRGB(xc.b);

		XAllocColor(g_display, ctx->color, &clr_pen);

		if (xscmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
			l_s = LineOnOffDash;
		else if (xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
			l_s = LineDoubleDash;
		else
			l_s = LineSolid;
		
		l_w = xstol(pxp->size);

		XSetLineAttributes(g_display, ctx->context, l_w, l_s, CapNotLast, JoinMiter);

		XSetForeground(g_display, ctx->context, clr_pen.pixel);
		
		XDrawLines(g_display, ctx->device, ctx->context, pa, 4, CoordModePrevious);

		if(clr_pen.pixel)
		{
			XFreeColors(g_display, ctx->color, &(clr_pen.pixel), 1, 0);
		}
	}
}

static XFontStruct* _create_font(const xfont_t* pxf)
{
	XFontStruct* fs;
	char font_token[1024] = {0};
	
	if(is_null(pxf->family))
		xsappend(font_token, "*-%s", "*");
	else
		xsappend(font_token, "*-%s", pxf->family);
	
	if(xscmp(pxf->weight,GDI_ATTR_FONT_WEIGHT_BOLD) == 0)
		xsappend(font_token, "-%s-*", "bold");
	else
		xsappend(font_token, "-%s-*", "medium");
	
	if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
		xsappend(font_token, "-%s-*", "italic");
	else
		xsappend(font_token, "-%s-*", "normal");

	xsappend(font_token, "-%s-*", pxf->size);

	fs = XLoadQueryFont(g_display, font_token);
	if(!fs)
	{
		xsprintf(font_token, "*-*-*-%s-*", pxf->size);
		fs = XLoadQueryFont(g_display, font_token);
	}

	return fs;
}

void _gdi_draw_text(res_ctx_t rdc,const xfont_t* pxf,const xface_t* pxa,const xrect_t* prt,const tchar_t* txt,int len)
{
	XFontStruct* pfs = NULL;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_font = {0};

	XPoint pt[2];
    
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(rdc,pt,2);

	if(pxf)
	{
		pfs = _create_font(pxf);
	}

	if(pfs)
	{
		XSetFont(g_display, rdc->context, pfs->fid);
	}

	parse_xcolor(&xc,pxf->color);

	clr_font.red = XRGB(xc.r);
	clr_font.green = XRGB(xc.g);
	clr_font.blue = XRGB(xc.b);

	XAllocColor(g_display, rdc->color, &clr_font);

	XSetForeground(g_display, rdc->context, clr_font.pixel);

	if(len < 0) len = xslen(txt);

	XDrawString(g_display, rdc->device, rdc->context, pt[0].x, pt[0].y, txt, len);

	if(pfs)
	{
		XFreeFont(g_display, pfs);
	}

	if(clr_font.pixel)
	{
		XFreeColors(g_display, rdc->color, &(clr_font.pixel), 1, 0);
	}
}

void _gdi_text_out(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	XFontStruct* pfs = NULL;

	xcolor_t xc = {0};
	int l_w, l_s;
	XColor ext, clr_font = {0};

	XPoint pt[2];
    
	pt[0].x = ppt->x;
	pt[0].y = ppt->y;
	pt[1].x = ppt->x;
	pt[1].y = ppt->y;

	DPtoLP(rdc,pt,2);

	if(pxf)
	{
		pfs = _create_font(pxf);
	}

	if(pfs)
	{
		XSetFont(g_display, rdc->context, pfs->fid);
	}

	parse_xcolor(&xc,pxf->color);

	clr_font.red = XRGB(xc.r);
	clr_font.green = XRGB(xc.g);
	clr_font.blue = XRGB(xc.b);

	XAllocColor(g_display, rdc->color, &clr_font);

	XSetForeground(g_display, rdc->context, clr_font.pixel);

	if(len < 0) len = xslen(txt);

	XDrawString(g_display, rdc->device, rdc->context, pt[0].x, pt[0].y, txt, len);

	if(pfs)
	{
		XFreeFont(g_display, pfs);
	}

	if(clr_font.pixel)
	{
		XFreeColors(g_display, rdc->color, &(clr_font.pixel), 1, 0);
	}
}

void _gdi_exclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	
}

void _gdi_inclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	
}

void _gdi_text_rect(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* prt)
{
	XFontStruct* pfs = NULL;
	XCharStruct chs = {0};
	int direct = 0, ascent = 0, descent = 0;
	if(pxf)
	{
		pfs = _create_font(pxf);
	}

	if(!pfs) return;
	
	if(len < 0) len = xslen(txt);

	prt->w = XTextWidth(pfs, txt, len);

	XTextExtents(pfs, txt, len, &direct, &ascent, &descent, &chs);

	prt->h = ascent + descent;

	if(pfs)
	{
		XFreeFont(g_display, pfs);
	}
}

void _gdi_text_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	XFontStruct* pfs = NULL;
	XCharStruct chs = {0};
	int direct = 0, ascent = 0, descent = 0;
	if(pxf)
	{
		pfs = _create_font(pxf);
	}

	if(!pfs) return;
	
	if(len < 0) len = xslen(txt);

	XTextExtents(pfs, txt, len, &direct, &ascent, &descent, &chs);

	pxs->cx = chs.width;
	pxs->cy = ascent + descent;

	if(pfs)
	{
		XFreeFont(g_display, pfs);
	}
}

void _gdi_text_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	XFontStruct* pfs = NULL;
	XCharStruct chs = {0};
	int direct = 0, ascent = 0, descent = 0;
	if(pxf)
	{
		pfs = _create_font(pxf);
	}

	if(!pfs) return;
	
	XTextExtents(pfs, "aj", 2, &direct, &ascent, &descent, &chs);

	pxs->cx = chs.width / 2;
	pxs->cy = (ascent + descent) / 2;

	if(pfs)
	{
		XFreeFont(g_display, pfs);
	}
}

#ifdef XDU_SUPPORT_CONTEXT_BITMAP
void _gdi_draw_image(res_ctx_t rdc,res_bmp_t hbmp,const tchar_t* clr,const xrect_t* prt)
{
    
}

void _gdi_draw_bitmap(res_ctx_t rdc, res_bmp_t hbmp, const xrect_t* prt)
{
    
}
#endif

#ifdef XDU_SUPPORT_CONTEXT_REGION
void _gdi_fill_region(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn)
{
    
}
#endif

#endif //XDU_SUPPORT_CONTEXT_GRAPHIC

