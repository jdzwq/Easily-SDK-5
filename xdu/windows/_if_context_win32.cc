/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc device context document

	@module	if_context_win.c | windows implement file

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

#ifndef WINCE
#include <Lm.h>
#pragma comment(lib,"Netapi32.lib")
#endif

int _context_version(void)
{
	int nVer;

#ifdef WINCE
	nVer = 4;
#else
	WKSTA_INFO_100 *pbuf = NULL;
	nVer = 5;
	if (NERR_Success == NetWkstaGetInfo(NULL, 100, (LPBYTE *)&pbuf))
	{
		nVer = pbuf->wki100_ver_major;
	}

	if (pbuf)
	{
		NetApiBufferFree(pbuf);
	}
#endif

	return nVer;
}

int _context_startup()
{
	int nVer;

#ifdef WINCE
	nVer = 4;
#else
	WKSTA_INFO_100 *pbuf = NULL;
	nVer = 5;
	if (NERR_Success == NetWkstaGetInfo(NULL, 100, (LPBYTE *)&pbuf))
	{
		nVer = pbuf->wki100_ver_major;
	}

	if (pbuf)
	{
		NetApiBufferFree(pbuf);
	}
#endif

#ifdef XDU_SUPPORT_CONTEXT_GDI
	_gdi_init(nVer);
#endif

#ifdef XDU_SUPPORT_CONTEXT_GDIPLUS
	_gdiplus_init(nVer);
#endif

#ifdef XDU_SUPPORT_CONTEXT_CAIRO
	_cairo_init(nVer);
#endif

	return nVer;
}

void _context_cleanup(void)
{
#ifdef XDU_SUPPORT_CONTEXT_GDI
	_gdi_uninit();
#endif

#ifdef XDU_SUPPORT_CONTEXT_GDIPLUS
	_gdiplus_uninit();
#endif

#ifdef XDU_SUPPORT_CONTEXT_CAIRO
	_cairo_uninit();
#endif
}

visual_t _create_display_context(res_win_t wt)
{
	win32_context_t* ctx = NULL;

	ctx = (win32_context_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(win32_context_t));
	ctx->head.tag = _VIEWING_DISPLAY;

	if (wt)
	{
		ctx->device.widget = wt;
		ctx->context = GetWindowDC(wt);
		ctx->type = CONTEXT_WIDGET;
	}
	else
	{
#ifdef WINCE
		ctx->context = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
#else
		ctx->context = CreateIC(_T("DISPLAY"), NULL, NULL, NULL);
#endif
		ctx->type = CONTEXT_SCREEN;
	}

	return (visual_t)&(ctx->head);
}

visual_t _create_compatible_context(visual_t rdc, int cx, int cy)
{
	win32_context_t* org = (win32_context_t*)rdc;
	win32_context_t* ctx = NULL;
	HBITMAP bmp;

	ctx = (win32_context_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(win32_context_t));

	ctx->context = CreateCompatibleDC(org->context);
	if (ctx->context)
		SetBkMode(ctx->context, TRANSPARENT);

	bmp = CreateCompatibleBitmap(org->context, cx, cy);

	ctx->device.bitmap = (HBITMAP)SelectObject(ctx->context, (HGDIOBJ)bmp);
	ctx->type = CONTEXT_MEMORY;

	return (visual_t)&(ctx->head);
}

void _destroy_context(visual_t rdc)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HBITMAP bmp;

	switch (ctx->type)
	{
	case CONTEXT_WIDGET:
		ReleaseDC(ctx->device.widget, ctx->context);
		break;
	case CONTEXT_MEMORY:
		bmp = (HBITMAP)SelectObject(ctx->context, (HGDIOBJ)ctx->device.bitmap);
		DeleteObject(bmp);

		DeleteDC(ctx->context);
		break;
	case CONTEXT_SCREEN:
		DeleteDC(ctx->context);
		break;
	case CONTEXT_PRINTER:
		DeleteDC(ctx->context);
		break;
	}

	HeapFree(GetProcessHeap(), 0, rdc);
}

void _get_device_caps(visual_t rdc, dev_cap_t* pcap)
{
	win32_context_t* ctx = (win32_context_t*)rdc;

	pcap->horz_res = GetDeviceCaps(ctx->context, HORZRES);
	pcap->vert_res = GetDeviceCaps(ctx->context, VERTRES);

	pcap->horz_pixels = GetDeviceCaps(ctx->context, LOGPIXELSX);
	pcap->vert_pixels = GetDeviceCaps(ctx->context, LOGPIXELSY);

	pcap->horz_size = GetDeviceCaps(ctx->context, PHYSICALWIDTH);
	pcap->vert_size = GetDeviceCaps(ctx->context, PHYSICALHEIGHT);

	pcap->horz_feed = GetDeviceCaps(ctx->context, PHYSICALOFFSETX);
	pcap->vert_feed = GetDeviceCaps(ctx->context, PHYSICALOFFSETY);
}

void _render_context(visual_t src, int srcx, int srcy, visual_t dst, int dstx, int dsty, int dstw, int dsth)
{
	win32_context_t* src_ctx = (win32_context_t*)src;
	win32_context_t* dst_ctx = (win32_context_t*)dst;

	BitBlt(dst_ctx->context, dstx, dsty, dstw, dsth, src_ctx->context, srcx, srcy, SRCCOPY);
}

/*******************************************************************************************************************/
#ifdef WINCE
static int MulDiv(int a, int b, int c)
{
	return (int)((float)a * (float)b / (float)c);
}
#endif

int _font_size(visual_t rdc, int height)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	TEXTMETRIC tm = { 0 };
	int size;

	GetTextMetrics(hDC, &tm);

	size = (int)((float)(height - tm.tmInternalLeading) * 72.0 / (float)GetDeviceCaps(hDC, LOGPIXELSY) + 0.5);

	if (!rdc)
		ReleaseDC(NULL, hDC);

	return size;
}

float _pt_per_mm(visual_t rdc, bool_t horz)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	float fp;

	if (horz)
		fp = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	else
		fp = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	if (!rdc)
		ReleaseDC(NULL, hDC);

	return fp;
}

void _text_mm_size(visual_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	LOGFONT lf;
	HFONT hFont, orgFont;
	SIZE si;
	float htpermm, vtpermm;

	if (!txt)
	{
		pxs->fw = ZERO_WIDTH;
		pxs->fh = ZERO_HEIGHT;
	}
	else
	{
#ifdef WINCE
		orgFont = (HFONT)GetStockObject(SYSTEM_FONT);
#else
		orgFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
#endif
		GetObject(orgFont, sizeof(LOGFONT), (void*)&lf);

		lf.lfHeight = lf.lfHeight = -MulDiv(_ttol(pxf->size), GetDeviceCaps(hDC, LOGPIXELSY), 72);
		lf.lfWeight = _ttol(pxf->weight);

		if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
		{
			lf.lfItalic = 1;
		}
		else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
		{
			lf.lfUnderline = 1;
		}
		else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
		{
			lf.lfStrikeOut = 1;
		}

		if (!is_null(pxf->family))
		{
			xscpy(lf.lfFaceName, pxf->family);
		}

		hFont = CreateFontIndirect(&lf);

		orgFont = (HFONT)SelectObject(hDC, hFont);

		if (len < 0)
			len = (int)xslen(txt);

		GetTextExtentPoint32(hDC, txt, len, &si);

		htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
		vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

		pxs->fw = (float)((float)si.cx / htpermm);
		pxs->fh = (float)((float)si.cy / vtpermm);

		hFont = (HFONT)SelectObject(hDC, orgFont);
		DeleteObject(hFont);
	}

	if (!rdc)
		ReleaseDC(NULL, hDC);
}

void _text_pt_size(visual_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	LOGFONT lf;
	HFONT hFont, orgFont;
	SIZE si;

#ifdef WINCE
	orgFont = (HFONT)GetStockObject(SYSTEM_FONT);
#else
	orgFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
#endif
	GetObject(orgFont, sizeof(LOGFONT), (void*)&lf);

	lf.lfHeight = lf.lfHeight = -MulDiv(_ttol(pxf->size), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = _ttol(pxf->weight);

	if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!is_null(pxf->family))
	{
		xscpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);

	orgFont = (HFONT)SelectObject(hDC, hFont);

	if (len < 0)
		len = (int)xslen(txt);

	GetTextExtentPoint32(hDC, txt, len, &si);

	pxs->w = si.cx;
	pxs->h = si.cy;

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);

	if (!rdc)
		ReleaseDC(NULL, hDC);
}

void _text_mm_metric(visual_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	LOGFONT lf;
	HFONT hFont, orgFont;
	TEXTMETRIC tm = { 0 };
	float htpermm, vtpermm;

#ifdef WINCE
	hFont = (HFONT)GetStockObject(SYSTEM_FONT);
#else
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
#endif
	GetObject(hFont, sizeof(LOGFONT), (void*)&lf);

	lf.lfHeight = lf.lfHeight = -MulDiv(_ttol(pxf->size), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = _ttol(pxf->weight);

	if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!is_null(pxf->family))
	{
		xscpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);
	orgFont = (HFONT)SelectObject(hDC, hFont);

	GetTextMetrics(hDC, &tm);

	htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	//*pfx = (float)((float)tm.tmAveCharWidth / htpermm);
	pxs->fw = (float)((float)tm.tmMaxCharWidth / htpermm);
	pxs->fh = (float)((float)tm.tmHeight / vtpermm);

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);

	if (!rdc)
		ReleaseDC(NULL, hDC);
}

void _text_pt_metric(visual_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	LOGFONT lf;
	HFONT hFont, orgFont;
	TEXTMETRIC tm = { 0 };

#ifdef WINCE
	hFont = (HFONT)GetStockObject(SYSTEM_FONT);
#else
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
#endif
	GetObject(hFont, sizeof(LOGFONT), (void*)&lf);

	lf.lfHeight = lf.lfHeight = -MulDiv(_ttol(pxf->size), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = _ttol(pxf->weight);

	if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!is_null(pxf->family))
	{
		xscpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);
	orgFont = (HFONT)SelectObject(hDC, hFont);

	GetTextMetrics(hDC, &tm);

	//*pcx = tm.tmAveCharWidth;
	pxs->w = tm.tmMaxCharWidth;
	pxs->h = tm.tmHeight;

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);

	if (!rdc)
		ReleaseDC(NULL, hDC);
}

void _cast_pt_to_mm(visual_t rdc, bool_t horz, xspan_t* pxn)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	float htpermm, vtpermm;
	float mm;

	htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	if (horz)
		mm = (float)((float)pxn->r / htpermm);
	else
		mm = (float)((float)pxn->r / vtpermm);

	if (!rdc)
		ReleaseDC(NULL, hDC);

	pxn->fr = mm;
}

void _cast_mm_to_pt(visual_t rdc, bool_t horz, xspan_t* pxn)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (rdc)? (HDC)(ctx->context) : GetDC(NULL);
	float htpermm, vtpermm;
	int pt;

	htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	if (horz)
		pt = (int)(pxn->fr * htpermm + 0.5);
	else
		pt = (int)(pxn->fr * vtpermm + 0.5);

	if (!rdc)
		ReleaseDC(NULL, hDC);

	pxn->r = pt;
}

#endif //XDU_SUPPORT_CONTEXT
