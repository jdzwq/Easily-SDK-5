/***********************************************************************
	Easily xdu v5.5

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

#include "xduiml.h"


#ifdef XDU_SUPPORT_CONTEXT
Display*     g_display = NULL;

int _context_startup(void)
{
    int nVer = 0;
    char* dname;
    
    dname = getenv("DISPLAY");
    
    g_display = XOpenDisplay(dname);

    if(!g_display) return (-1);

	return nVer;
}

void _context_cleanup(void)
{
    XCloseDisplay(g_display);
    g_display = NULL;
}

res_ctx_t _create_display_context(res_win_t wt)
{
    X11_suface_t* ctx = NULL;
    XGCValues gv = {0};
    
    ctx = (X11_suface_t*)calloc(1, sizeof(X11_suface_t));
    
    ctx->device = (wt)? wt : DefaultRootWindow(g_display);
    ctx->context = XCreateGC(g_display, ctx->device, 0, &gv);
    ctx->type = CONTEXT_WIDGET;
    ctx->color = DefaultColormap(g_display, DefaultScreen(g_display));
    
    return ctx;
}

res_ctx_t _create_compatible_context(res_ctx_t rdc, int cx, int cy)
{
    XGCValues gv = {0};
    Window r;
    int x,y;
    unsigned int w,h,b,d;
    X11_suface_t* ctx = NULL;
    
    XGetGeometry(g_display, rdc->device, &r, &x, &y, &w, &h, &b, &d);
    
    ctx = (X11_suface_t*)calloc(1, sizeof(X11_suface_t));

    ctx->device = XCreatePixmap (g_display, r, cx, cy, d);
    ctx->context = XCreateGC(g_display, rdc->device, 0, &gv);
    ctx->type = CONTEXT_MEMORY;
    ctx->color = DefaultColormap(g_display, DefaultScreen(g_display));
    
    return ctx;
}

void _destroy_context(res_ctx_t rdc)
{
    if(rdc->type == CONTEXT_MEMORY && rdc->device)
        XFreePixmap(g_display, rdc->device);
    
    if(rdc->context)
	    XFreeGC(g_display, rdc->context);
    
    free(rdc);
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

void _render_context(res_ctx_t src, int srcx, int srcy, res_ctx_t dst, int dstx, int dsty, int dstw, int dsth)
{
    XCopyArea(g_display, src->device, dst->device, src->context, srcx, srcy, dstw, dsth, dstx, dsty);
}

/*******************************************************************************************************************/

static tchar_t *x11_font_name[] = {_T("*")};
static tchar_t *x11_font_weight[] = {_T("regular"), _T("medium"), _T("bold")};
static tchar_t *x11_font_style[]  = {_T("r"), _T("i"), _T("o")};
static tchar_t x11_pattern[] = {_T("-*-%s-%s-%s-*--%d-*-*-*-*-*-*")};
    
static void _format_font_pattern(const xfont_t* pxf, tchar_t* buf)
{
    const tchar_t* fs_name = NULL;
    const tchar_t* fs_style = NULL;
    const tchar_t* fs_weight = NULL;
    int fs_size = 10;
    
    if(is_null((pxf->family)))
        fs_name = x11_font_name[0];
    else
        fs_name = pxf->family;
       
    if(xstol(pxf->weight) < 400)
        fs_weight = x11_font_weight[0];
    else if(xstol(pxf->weight) < 700)
        fs_weight = x11_font_weight[1];
    else
        fs_weight = x11_font_weight[2];
    
    if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
        fs_style = x11_font_style[1];
    else if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_OBLIQUE) == 0)
        fs_style = x11_font_style[2];
    else
        fs_style = x11_font_style[0];
    
     if(is_null((pxf->size)))
         fs_size = 10;
    else
        fs_size = xstol(pxf->size);
    
    xsprintf(buf, x11_pattern, fs_name, fs_weight, fs_style, fs_size);
}

float _pt_per_mm(res_ctx_t rdc, bool_t horz)
{
    int scrn;
    
    scrn = DefaultScreen(g_display);
    
    if(horz)
        return (float)((float)DisplayWidth(g_display, scrn) / (float)DisplayWidthMM(g_display, scrn));
    else
        return (float)((float)DisplayHeight(g_display, scrn) / (float)DisplayHeightMM(g_display, scrn));
}

int _font_size(res_ctx_t rdc, int height)
{
    float fh;
    int size;
    
    fh = _pt_per_mm(rdc, 0);
    
    size = (int)(((float)height / fh) * PDPERMM);
    
    return size;
}

void _text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, int* pcx, int* pcy)
{
    XFontStruct* pfs = NULL;
    tchar_t pattern[256] = {0};
    
    if(len < 0) len = xslen(txt);
    
    _format_font_pattern(pxf, pattern);
    
    pfs = XLoadQueryFont(g_display, pattern);
    if(!pfs)
        return;
    
    if(pcx) *pcx = XTextWidth(pfs, txt, len);
    
    if(pcy) *pcy = pfs->ascent + pfs->descent;
    
    XFreeFont(g_display, pfs);
}

void _text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, float* pfx, float* pfy)
{
    int w,h;
    
    _text_pt_size(rdc, pxf, txt, len, &w, &h);
    
    if(pfx) *pfx = (float)w / _pt_per_mm(rdc, 1);
    
    if(pfy) *pfy = (float)h / _pt_per_mm(rdc, 0);
}

void _text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, int* pcx, int* pcy)
{
    XFontStruct* pfs = NULL;
    tchar_t pattern[256] = {0};
    
    _format_font_pattern(pxf, pattern);
    
    pfs = XLoadQueryFont(g_display, pattern);
    if(!pfs)
        return;
    
    if(pcx) *pcx = (pfs->min_bounds.width + pfs->max_bounds.width) / 2;
    
    if(pcy) *pcy = pfs->ascent + pfs->descent;
}

void _text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, float* pfx, float* pfy)
{
    int w,h;
    
    _text_pt_metric(rdc, pxf, &w, &h);
    
    if(pfx) *pfx = (float)w / _pt_per_mm(rdc, 1);
    
    if(pfy) *pfy = (float)h / _pt_per_mm(rdc, 0);
}

float _cast_pt_to_mm(res_ctx_t rdc, int pt, bool_t horz)
{
    if(horz)
        return (float)pt / _pt_per_mm(rdc, 1);
    else
        return (float)pt / _pt_per_mm(rdc, 0);
}

int _cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz)
{
    if(horz)
        return (int)(mm * _pt_per_mm(rdc, 1));
    else
        return (int)(mm * _pt_per_mm(rdc, 0));
}

#endif //XDU_SUPPORT_CONTEXT
