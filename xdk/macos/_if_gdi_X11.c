/***********************************************************************
	Easily xdk v5.5

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

#include "xdkiml.h"
#include "xdkattr.h"
#include "xdkutil.h"

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC

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
	if (_tstrcmp(horz_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && _tstrcmp(vert_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		prt->width = (prt->width < src_width) ? prt->width : src_width;
		prt->height = (prt->height < src_height) ? prt->height : src_height;
	}
	else if (_tstrcmp(horz_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		prt->x = (prt->width < src_width) ? prt->x : (prt->x + prt->width - src_width);
		prt->height = (prt->height < src_height) ? prt->height : src_height;
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		prt->width = (prt->width < src_width) ? prt->width : src_width;
		prt->y = (prt->height < src_height) ? prt->y : (prt->y + prt->height - src_height);
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		prt->x = (prt->width < src_width) ? prt->x : (prt->x + prt->width - src_width);
		prt->y = (prt->height < src_height) ? prt->y : (prt->y + prt->height - src_height);
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
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
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		prt->width = (prt->width < src_width) ? prt->width : src_width;
		prt->y = (prt->height < src_height) ? prt->y : (prt->y + (prt->height - src_height) / 2);
		prt->height = (prt->height < src_height) ? prt->height : src_height;
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
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
	XPoint pt[2];
    
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(rdc,pt,2);

	//XSetLineAttributes(dpy, gc, 5, LineSolid, CapRound, JoinRound);

    XSetForeground(g_display, ctx->device, BlackPixel(g_display, DefaultScreen(g_display)));
    
    XDrawLine(g_display, ctx->device, ctx->context, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
}

void _gdi_draw_3dline(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	
}

void _gdi_draw_3drect(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	
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
	
}

void _gdi_draw_round(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	
}

void _gdi_draw_ellipse(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{

}

void _gdi_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt, double fang, double tang)
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

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
void _gdi_draw_image(res_ctx_t rdc,res_bmp_t hbmp,const tchar_t* clr,const xrect_t* prt)
{
    
}

void _gdi_draw_bitmap(res_ctx_t rdc, res_bmp_t hbmp, const xrect_t* prt)
{
    
}
#endif

#ifdef XDK_SUPPORT_CONTEXT_REGION
void _gdi_fill_region(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn)
{
    
}
#endif

#endif //XDK_SUPPORT_CONTEXT_GRAPHIC

