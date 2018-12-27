/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc device context document

	@module	if_context_win.c | device context windows implement file

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

#ifdef XDK_SUPPORT_CONTEXT
Display*     g_display = NULL;

int _context_startup(void)
{
    int nVer = 0;
    char* dname;
    
    dname = getenv("DISPLAY");
    
    g_display = XOpenDisplay(dname);
    
	return nVer;
}

void _context_cleanup(void)
{
    XCloseDisplay(g_display);
    g_display = NULL;
}

res_ctx_t _create_display_context(void)
{
    X11_suface_t* ctx = NULL;
    XGCValues gv = {0};
    
    ctx = (X11_suface_t*)calloc(1, sizeof(X11_suface_t));
    
    ctx->device = DefaultRootWindow(g_display);
    ctx->context = XCreateGC(g_display, ctx->device, 0, &gv);
    ctx->memo = 0;
    
    return ctx;
}

res_ctx_t _create_compatible_context(res_ctx_t rdc)
{
    X11_suface_t* ctx = (X11_suface_t*)rdc;
    XGCValues gv = {0};
    
    ctx = (X11_suface_t*)calloc(1, sizeof(X11_suface_t));
    
    ctx->device = XCreatePixmap (g_display, DefaultRootWindow(g_display), 1, 1, 0);
    ctx->context = XCreateGC(g_display, ctx->device, 0, &gv);
    ctx->memo = 1;
    
    return ctx;
}

void _destroy_context(res_ctx_t rdc)
{
    X11_suface_t* ctx = (X11_suface_t*)rdc;
    
    if(ctx->memo)
        XFreePixmap(g_display, ctx->device);
    
	XFreeGC(g_display, ctx->context);
    
    free(ctx);
}

void _get_device_caps(res_ctx_t rdc, dev_cap_t* pcap)
{
    int scrn;
    
    scrn = DefaultScreen(g_display);

    pcap->horz_res = DisplayWidth(g_display, scrn);
    pcap->vert_res = DisplayHeight(g_display, scrn);
    
	pcap->horz_pixels = DisplayWidthMM(g_display, scrn);
	pcap->vert_pixels = DisplayHeightMM(g_display, scrn);

	pcap->horz_size = DisplayWidth(g_display, scrn);
	pcap->vert_size = DisplayHeight(g_display, scrn);

    pcap->horz_feed = 0;
    pcap->vert_feed = 0;
}

void _render_context(res_ctx_t src, long srcx, long srcy, res_ctx_t dst, long dstx, long dsty, long dstw, long dsth)
{
    XCopyArea(g_display, src, dst, src, srcx, srcy, dstw, dsth, dstx, dsty);
    
    /*cairo_surface_t *surface;
    Pixmap pixmap;
    
    pixmap = XCreatePixmap (GDK_WINDOW_XDISPLAY (window),
                            GDK_WINDOW_XID (window),
                            width, height,
                            gdk_visual_get_depth (visual));
    surface = cairo_xlib_surface_create (GDK_WINDOW_XDISPLAY (window),
                                         pixmap,
                                         GDK_VISUAL_XVISUAL (visual),
                                         width, height);
    attach_free_pixmap_handler (surface, GDK_WINDOW_DISPLAY (window), pixmap);*/
}

/*******************************************************************************************************************/

float _pt_per_mm(res_ctx_t rdc, bool_t horz)
{
    int scrn;
    
    scrn = DefaultScreen(g_display);
    
    if(horz)
        return (float)((float)DisplayWidth(g_display, scrn) / (float)DisplayWidthMM(g_display, scrn));
    else
        return (float)((float)DisplayHeight(g_display, scrn) / (float)DisplayHeightMM(g_display, scrn));
}

int _font_size(res_ctx_t rdc, long height)
{
    float fh;
    int size;
    
    fh = _pt_per_mm(rdc, 0);
    
    size = (int)(((float)height / fh) * PDPERMM);
    
    return size;
}

void _text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, long* pcx, long* pcy)
{
    XFontStruct* pfs = NULL;
    tchar_t pattern[1024] = {0};
    
    if(len < 0) len = _tstrlen(txt);
    
    _tsprintf(pattern, _T("*-%s?-*-%s-*-%s?-*-%d-*"), pxf->family, pxf->weight, pxf->style, _tstrtol(pxf->size));
    
    pfs = XLoadQueryFont(g_display, pattern);
    if(!pfs)
        return;
    
    if(pcx) *pcx = XTextWidth(pfs, txt, len);
    
    if(pcy) *pcy = pfs->ascent + pfs->descent;
    
    XFreeFont(g_display, pfs);
}

void _text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, float* pfx, float* pfy)
{
    long w,h;
    
    _text_pt_size(rdc, pxf, txt, len, &w, &h);
    
    if(pfx) *pfx = (float)w / _pt_per_mm(rdc, 1);
    
    if(pfy) *pfy = (float)h / _pt_per_mm(rdc, 0);
}

void _text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, long* pcx, long* pcy)
{
    XFontStruct* pfs = NULL;
    tchar_t pattern[1024] = {0};
    
    _tsprintf(pattern, _T("*-%d"), _tstrtol(pxf->size));
    
    pfs = XLoadQueryFont(g_display, pattern);
    if(!pfs)
        return;
    
    if(pcx) *pcx = (pfs->min_bounds.width + pfs->max_bounds.width) / 2;
    
    if(pcy) *pcy = pfs->ascent + pfs->descent;
}

void _text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, float* pfx, float* pfy)
{
    long w,h;
    
    _text_pt_metric(rdc, pxf, &w, &h);
    
    if(pfx) *pfx = (float)w / _pt_per_mm(rdc, 1);
    
    if(pfy) *pfy = (float)h / _pt_per_mm(rdc, 0);
}

float _cast_pt_to_mm(res_ctx_t rdc, long pt, bool_t horz)
{
    if(horz)
        return (float)pt / _pt_per_mm(rdc, 1);
    else
        return (float)pt / _pt_per_mm(rdc, 0);
}

long _cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz)
{
    if(horz)
        return (long)(mm * _pt_per_mm(rdc, 1));
    else
        return (long)(mm * _pt_per_mm(rdc, 0));
}

#endif //XDK_SUPPORT_CONTEXT
