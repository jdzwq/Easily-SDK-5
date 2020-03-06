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
    X11_suface_t* ctx = (X11_suface_t*)rdc;

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

		clr_pen.red = (unsigned short)((double)xc.r * 65535.0 / 256.0);
		clr_pen.green = (unsigned short)((double)xc.g * 65535.0 / 256.0);
		clr_pen.blue = (unsigned short)((double)xc.b * 65535.0 / 256.0);

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
	
}

void _gdi_draw_polygon(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int n)
{
    
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
	X11_suface_t* ctx = (X11_suface_t*)rdc;

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

		clr_brush.red = (unsigned short)((double)xc.r * 65535.0 / 256.0);
		clr_brush.green = (unsigned short)((double)xc.g * 65535.0 / 256.0);
		clr_brush.blue = (unsigned short)((double)xc.b * 65535.0 / 256.0);

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

		clr_pen.red = (unsigned short)((double)xc.r * 65535.0 / 256.0);
		clr_pen.green = (unsigned short)((double)xc.g * 65535.0 / 256.0);
		clr_pen.blue = (unsigned short)((double)xc.b * 65535.0 / 256.0);

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
	
}

void _gdi_draw_ellipse(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{

}

void _gdi_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang)
{

}

void _gdiplus_draw_arc(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t * ppt, int rx, int ry, double fang, double tang)
{
    
}

void _gdi_draw_arrow(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt,int alen,double arc)
{

}

void _gdi_draw_text(res_ctx_t rdc,const xfont_t* pxf,const xface_t* pxa,const xrect_t* prt,const tchar_t* txt,int len)
{
    /*f = XLoadFont(d, "fixed");
    XSetFont(d, gc, f);
    
    fontset = XCreateFontSet(dpy, "-*-*-*-*-*-*-16-*-*-*-*-*-*-*",
                             &missing_charsets, &num_missing_charsets,
                             &default_string);
    XFreeStringList(missing_charsets);
    
    (void) XmbTextExtents(fontset, str, num_bytes, &dummy, &boundingbox);
    
    originx = x + (width - boundingbox.width) / 2 - boundingbox.x;
    originy = y + (height - boundingbox.height) / 2 - boundingbox.y;
    
    XmbDrawString(dpy, w, fontset, gc, originx, originy, str, num_bytes);*/
}

void _gdi_text_out(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{

}

void _gdi_exclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	
}

void _gdi_inclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	
}

void _gdi_text_rect(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* prt)
{

}

void _gdi_text_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{

}

void _gdi_text_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{

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

