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

#if defined(XDU_SUPPORT_CONTEXT_GRAPHIC)
#ifdef _UNICODE
	_gdiplus_init(nVer);
#else
	_gdi_init(nVer);
#endif
#endif

	return nVer;
}

void _context_cleanup(void)
{
#if defined(XDU_SUPPORT_CONTEXT_GRAPHIC)
#ifdef _UNICODE
	_gdiplus_uninit();
#else
	_gdi_uninit();
#endif
#endif
}

res_ctx_t _create_display_context(res_win_t wt)
{
	win32_context_t* ctx = NULL;

	ctx = (win32_context_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(win32_context_t));

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

	return ctx;
}

res_ctx_t _create_compatible_context(res_ctx_t rdc, int cx, int cy)
{
	win32_context_t* ctx = NULL;
	HBITMAP bmp = NULL;

	ctx = (win32_context_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(win32_context_t));

	ctx->context = CreateCompatibleDC(rdc->context);
	if (ctx->context)
		SetBkMode(ctx->context, TRANSPARENT);

	bmp = CreateCompatibleBitmap(rdc->context, cx, cy);
	ctx->device.bitmap = (HBITMAP)SelectObject(ctx->context, (HGDIOBJ)bmp);
	ctx->type = CONTEXT_MEMORY;

	return (res_ctx_t)ctx;
}

void _destroy_context(res_ctx_t rdc)
{
	HBITMAP bmp;

	switch (rdc->type)
	{
	case CONTEXT_WIDGET:
		ReleaseDC(rdc->device.widget, rdc->context);
		break;
	case CONTEXT_MEMORY:
		bmp = (HBITMAP)SelectObject(rdc->context, (HGDIOBJ)rdc->device.bitmap);
		DeleteObject(bmp);

		DeleteDC(rdc->context);
		break;
	case CONTEXT_SCREEN:
		DeleteDC(rdc->context);
		break;
	case CONTEXT_PRINTER:
		DeleteDC(rdc->context);
		break;
	}

	HeapFree(GetProcessHeap(), 0, rdc);
}

void _get_device_caps(res_ctx_t rdc, dev_cap_t* pcap)
{
	pcap->horz_res = GetDeviceCaps(rdc->context, HORZRES);
	pcap->vert_res = GetDeviceCaps(rdc->context, VERTRES);

	pcap->horz_pixels = GetDeviceCaps(rdc->context, LOGPIXELSX);
	pcap->vert_pixels = GetDeviceCaps(rdc->context, LOGPIXELSY);

	pcap->horz_size = GetDeviceCaps(rdc->context, PHYSICALWIDTH);
	pcap->vert_size = GetDeviceCaps(rdc->context, PHYSICALHEIGHT);

	pcap->horz_feed = GetDeviceCaps(rdc->context, PHYSICALOFFSETX);
	pcap->vert_feed = GetDeviceCaps(rdc->context, PHYSICALOFFSETY);
}

void _render_context(res_ctx_t src, int srcx, int srcy, res_ctx_t dst, int dstx, int dsty, int dstw, int dsth)
{
	BitBlt(dst->context, dstx, dsty, dstw, dsth, src->context, srcx, srcy, SRCCOPY);
}

/*******************************************************************************************************************/
#ifdef WINCE
static int MulDiv(int a, int b, int c)
{
	return (int)((float)a * (float)b / (float)c);
}
#endif

int _font_size(res_ctx_t rdc, int height)
{
	HDC hDC = (HDC)(rdc->context);
	TEXTMETRIC tm = { 0 };
	int size;

	GetTextMetrics(hDC, &tm);

	size = (int)((height - tm.tmInternalLeading) * 72 / GetDeviceCaps(hDC, LOGPIXELSY));

	return size;
}

float _pt_per_mm(res_ctx_t rdc, bool_t horz)
{
	HDC hDC = (HDC)(rdc->context);
	float fp;

	if (horz)
		fp = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	else
		fp = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	return fp;
}

void _text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, float* pfx, float* pfy)
{
	HDC hDC = (HDC)(rdc->context);
	LOGFONT lf;
	HFONT hFont, orgFont;
	SIZE si;
	float htpermm, vtpermm;

	if (!txt)
	{
		*pfx = ZERO_WIDTH;
		*pfy = ZERO_HEIGHT;
		return;
	}

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

	*pfx = (float)((float)si.cx / htpermm);
	*pfy = (float)((float)si.cy / vtpermm);

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);
}

void _text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, int* pcx, int* pcy)
{
	HDC hDC = (HDC)(rdc->context);
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

	*pcx = si.cx;
	*pcy = si.cy;

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);
}

void _text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, float* pfx, float* pfy)
{
	HDC hDC = (HDC)(rdc->context);
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
	*pfx = (float)((float)tm.tmMaxCharWidth / htpermm);
	*pfy = (float)((float)tm.tmHeight / vtpermm);

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);
}

void _text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, int* pcx, int* pcy)
{
	HDC hDC = (HDC)(rdc->context);
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
	*pcx = tm.tmMaxCharWidth;
	*pcy = tm.tmHeight;

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);
}

float _cast_pt_to_mm(res_ctx_t rdc, int pt, bool_t horz)
{
	HDC hDC = (HDC)(rdc->context);
	float htpermm, vtpermm;
	float mm;

	htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	if (horz)
		mm = (float)((float)pt / htpermm);
	else
		mm = (float)((float)pt / vtpermm);

	return mm;
}

int _cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz)
{
	HDC hDC = (HDC)(rdc->context);
	float htpermm, vtpermm;
	int pt;

	htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	if (horz)
		pt = (int)(mm * htpermm);
	else
		pt = (int)(mm * vtpermm);

	return pt;
}

#endif //XDU_SUPPORT_CONTEXT
