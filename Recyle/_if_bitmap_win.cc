/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc bitmap document

	@module	if_bitmap_win.c | bitmap windows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#include "_if_xdc.h"

#ifdef WINCE
static long MulDiv(long a, long b, long c)
{
	return (long)((float)a * (float)b / (float)c);
}
#endif

static void _CenterRect(RECT* pRect, int src_width, int src_height)
{
	if (pRect->right - pRect->left > src_width)
	{
		pRect->left = pRect->left + (pRect->right - pRect->left - src_width) / 2;
		pRect->right = pRect->left + src_width;
	}
	if (pRect->bottom - pRect->top > src_height)
	{
		pRect->top = pRect->top + (pRect->bottom - pRect->top - src_height) / 2;
		pRect->bottom = pRect->top + src_height;
	}
}

res_bmp_t __cdecl _create_compatible_bitmap(res_dc_t rdc, long cx, long cy)
{
	return CreateCompatibleBitmap(rdc, cx, cy);
}

void	__cdecl _delete_bitmap(res_bmp_t bmp)
{
	DeleteObject(bmp);
}

res_bmp_t __cdecl _create_color_bitmap(res_dc_t rdc, const xcolor_t* pxc, long w, long h)
{
	HDC memDC;
	HBRUSH hBrush;
	HBITMAP newBmp, orgBmp;
	RECT rt;

	newBmp = CreateCompatibleBitmap(rdc, w, h);
	memDC = CreateCompatibleDC(rdc);
	orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	rt.left = rt.top = 0;
	rt.right = w;
	rt.bottom = h;

	hBrush = CreateSolidBrush(RGB(pxc->r, pxc->g, pxc->b));
	FillRect(memDC, &rt, hBrush);
	DeleteObject(hBrush);

	SelectObject(memDC, orgBmp);
	DeleteDC(memDC);

	return (res_bmp_t)newBmp;
}

res_bmp_t __cdecl _create_pattern_bitmap(res_dc_t rdc, const xcolor_t* pxc_front, const xcolor_t* pxc_back, long w, long h, const tchar_t* lay)
{
	HDC memDC;
	HBRUSH hBrush;
	HPEN newPen, orgPen;
	HBITMAP newBmp, orgBmp;
	RECT rt;

	newBmp = CreateCompatibleBitmap(rdc, w, h);
	memDC = CreateCompatibleDC(rdc);
	orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	rt.left = rt.top = 0;
	rt.right = w;
	rt.bottom = h;

	hBrush = CreateSolidBrush(RGB(pxc_back->r, pxc_back->g, pxc_back->b));
	FillRect(memDC, &rt, hBrush);
	DeleteObject(hBrush);

	newPen = CreatePen(PS_SOLID, 2, RGB(pxc_front->r, pxc_front->g, pxc_front->b));
	orgPen = (HPEN)SelectObject(memDC, newPen);

	POINT pt[2];
	if (xsicmp(lay, ATTR_LAYER_VERT) == 0)
	{
		pt[0].x = 2;
		pt[0].y = 2;
		pt[1].x = 2;
		pt[1].y = 4;
	}
	else
	{
		pt[0].x = 0;
		pt[0].y = 2;
		pt[1].x = 4;
		pt[1].y = 2;
	}

	MoveToEx(memDC, pt[0].x, pt[0].y, NULL);
	LineTo(memDC, pt[1].x, pt[1].y);

	newPen = (HPEN)SelectObject(memDC, orgPen);
	DeleteObject(newPen);

	SelectObject(memDC, orgBmp);
	DeleteDC(memDC);

	return (res_bmp_t)newBmp;
}

res_bmp_t __cdecl _create_gradient_bitmap(res_dc_t rdc, const xcolor_t* pxc_near, const xcolor_t* pxc_center, long w, long h, const tchar_t* type)
{
	RECT rt;
	rt.left = 0;
	rt.top = 0;
	rt.right = w;
	rt.bottom = h;

	HBITMAP newBmp = CreateCompatibleBitmap(rdc, rt.right - rt.left, rt.bottom - rt.top);
	HDC memDC = CreateCompatibleDC(rdc);
	HBITMAP orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	COLORREF clr_brush = RGB(pxc_near->r, pxc_near->g, pxc_near->b);
	COLORREF clr_linear = RGB(pxc_center->r, pxc_center->g, pxc_center->b);

	TRIVERTEX tv[4];
	GRADIENT_RECT gr[2];
	unsigned long mode = 0;

	if (xsicmp(type, GDI_ATTR_GRADIENT_TYPE_VERT) == 0)
	{
		tv[0].x = rt.left;
		tv[0].y = rt.top;
		tv[0].Red = GetRValue(clr_brush) << 8;
		tv[0].Green = GetGValue(clr_brush) << 8;
		tv[0].Blue = GetBValue(clr_brush) << 8;
		tv[0].Alpha = 255;

		tv[1].x = rt.right;
		tv[1].y = (rt.top + rt.bottom) / 2;
		tv[1].Red = GetRValue(clr_linear) << 8;
		tv[1].Green = GetGValue(clr_linear) << 8;
		tv[1].Blue = GetBValue(clr_linear) << 8;
		tv[1].Alpha = 255;

		tv[2].x = rt.left;
		tv[2].y = (rt.top + rt.bottom) / 2;
		tv[2].Red = GetRValue(clr_linear) << 8;
		tv[2].Green = GetGValue(clr_linear) << 8;
		tv[2].Blue = GetBValue(clr_linear) << 8;
		tv[2].Alpha = 255;

		tv[3].x = rt.right;
		tv[3].y = rt.bottom;
		tv[3].Red = GetRValue(clr_brush) << 8;
		tv[3].Green = GetGValue(clr_brush) << 8;
		tv[3].Blue = GetBValue(clr_brush) << 8;
		tv[3].Alpha = 255;

		mode = GRADIENT_FILL_RECT_V;
	}
	else
	{
		tv[0].x = rt.left;
		tv[0].y = rt.top;
		tv[0].Red = GetRValue(clr_brush) << 8;
		tv[0].Green = GetGValue(clr_brush) << 8;
		tv[0].Blue = GetBValue(clr_brush) << 8;
		tv[0].Alpha = 255;

		tv[1].x = (rt.left + rt.right) / 2;
		tv[1].y = rt.bottom;
		tv[1].Red = GetRValue(clr_linear) << 8;
		tv[1].Green = GetGValue(clr_linear) << 8;
		tv[1].Blue = GetBValue(clr_linear) << 8;
		tv[1].Alpha = 255;

		tv[2].x = (rt.left + rt.right) / 2;
		tv[2].y = rt.top;
		tv[2].Red = GetRValue(clr_linear) << 8;
		tv[2].Green = GetGValue(clr_linear) << 8;
		tv[2].Blue = GetBValue(clr_linear) << 8;
		tv[2].Alpha = 255;

		tv[3].x = rt.right;
		tv[3].y = rt.bottom;
		tv[3].Red = GetRValue(clr_brush) << 8;
		tv[3].Green = GetGValue(clr_brush) << 8;
		tv[3].Blue = GetBValue(clr_brush) << 8;
		tv[3].Alpha = 255;

		mode = GRADIENT_FILL_RECT_H;
	}

	gr[0].UpperLeft = 0;
	gr[0].LowerRight = 1;
	gr[1].UpperLeft = 2;
	gr[1].LowerRight = 3;

	GradientFill(memDC, tv, 4, &gr, 2, mode);

	newBmp = (HBITMAP)SelectObject(memDC, orgBmp);
	DeleteDC(memDC);

	return (res_bmp_t)newBmp;
}

res_bmp_t __cdecl _create_code128_bitmap(res_dc_t rdc, long w, long h, const tchar_t* text)
{
	HDC winDC,memDC;
	HBRUSH wBrush, bBrush;
	HBITMAP newBmp, orgBmp;
	HFONT newFont, orgFont;
	LOGFONT lf = { 0 };
	RECT rt;
	int unit;

	byte_t* buf;
	int len;

	byte_t* bar_buf;
	int bar_len;
	int i,black,span;

#ifdef _UNICODE
	len = ucs_to_utf8(text, -1, NULL, MAX_LONG);
#else
	len = mbs_to_utf8(text, -1, NULL, MAX_LONG);
#endif

	if (!len)
		return NULL;

	buf = (byte_t*)xmem_alloc(len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, -1, buf, len);
#else
	mbs_to_utf8(text, -1, buf, len);
#endif

	bar_len = code128_encode(buf, len, NULL, MAX_LONG);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return NULL;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = code128_encode(buf, len, bar_buf, bar_len);

	xmem_free(buf);
	
	winDC = GetDC(NULL);
	unit = GetDeviceCaps(rdc, LOGPIXELSX) / GetDeviceCaps(winDC, LOGPIXELSX);
	ReleaseDC(NULL, winDC);

	wBrush = CreateSolidBrush(RGB(255, 255, 255));
	bBrush = CreateSolidBrush(RGB(0, 0, 0));

	newBmp = CreateCompatibleBitmap(rdc, w, h);
	memDC = CreateCompatibleDC(rdc);

	SetBkMode(memDC, TRANSPARENT);

	orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	rt.left = 0;
	rt.right = w;
	rt.top = 0;
	rt.bottom = h;

	FillRect(memDC, &rt, wBrush);

	rt.left = unit;
	rt.right = unit;
	rt.top = unit;
	rt.bottom = h * 2 / 3 - unit;

	black = 0;
	for (i = 0; i < bar_len; i++)
	{
		span = (bar_buf[i] - '0');
		rt.right = rt.left + span * unit;

		black = (black) ? 0 : 1;

		if (black)
		{
			FillRect(memDC, &rt, bBrush);
		}
		
		rt.left = rt.right;
	}

	orgFont = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(orgFont, sizeof(LOGFONT), (void*)&lf);
	lf.lfHeight = -MulDiv(10, GetDeviceCaps(rdc, LOGPIXELSY), 72);

	newFont = CreateFontIndirect(&lf);
	orgFont = (HFONT)SelectObject(memDC, newFont);

	rt.left = 0;
	rt.top = h * 2 / 3;
	rt.bottom = h;

	DrawText(memDC, text, -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	newFont = (HFONT)SelectObject(memDC, orgFont);
	DeleteObject(newFont);

	SelectObject(memDC, orgBmp);
	DeleteDC(memDC);

	DeleteObject(wBrush);
	DeleteObject(bBrush);

	xmem_free(bar_buf);

	return newBmp;
}

res_bmp_t __cdecl _create_pdf417_bitmap(res_dc_t rdc, long w, long h, const tchar_t* text)
{
	HDC winDC, memDC;
	HBRUSH wBrush, bBrush;
	HBITMAP newBmp, orgBmp;
	RECT rt;
	int unit;

	byte_t* buf;
	int len;
	byte_t b, c;

	byte_t* bar_buf;
	int cols,rows,bar_len;
	int i,j, black;

#ifdef _UNICODE
	len = ucs_to_utf8(text, -1, NULL, MAX_LONG);
#else
	len = mbs_to_utf8(text, -1, NULL, MAX_LONG);
#endif

	if (!len)
		return NULL;

	buf = (byte_t*)xmem_alloc(len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, -1, buf, len);
#else
	mbs_to_utf8(text, -1, buf, len);
#endif

	bar_len = pdf417_encode(buf, len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return NULL;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = pdf417_encode(buf, len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	winDC = GetDC(NULL);
	unit = GetDeviceCaps(rdc, LOGPIXELSX) / GetDeviceCaps(winDC, LOGPIXELSX);
	if (unit < 2)
		unit = 2;
	ReleaseDC(NULL, winDC);

	wBrush = CreateSolidBrush(RGB(255, 255, 255));
	bBrush = CreateSolidBrush(RGB(0, 0, 0));

	newBmp = CreateCompatibleBitmap(rdc, w, h);
	memDC = CreateCompatibleDC(rdc);

	orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	rt.left = 0;
	rt.right = w;
	rt.top = 0;
	rt.bottom = h;

	FillRect(memDC, &rt, wBrush);

	rt.left = rt.right = unit;
	rt.top = rt.bottom = unit;

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.left = rt.right = unit;
		rt.top = rt.bottom;
		rt.bottom = rt.top + 2 * unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.left = rt.right;
				rt.right = rt.left + unit;

				black = (c & b) ? 0 : 1;

				if (black)
				{
					FillRect(memDC, &rt, bBrush);
				}

				b = b >> 1;
			}
		}
	}
	
	SelectObject(memDC, orgBmp);
	DeleteDC(memDC);

	DeleteObject(wBrush);
	DeleteObject(bBrush);

	xmem_free(bar_buf);

	return newBmp;
}

res_bmp_t __cdecl _create_qrcode_bitmap(res_dc_t rdc, long w, long h, const tchar_t* text)
{
	HDC winDC, memDC;
	HBRUSH wBrush, bBrush;
	HBITMAP newBmp, orgBmp;
	RECT rt;
	int unit;

	byte_t* buf;
	int len;
	byte_t b, c;

	byte_t* bar_buf;
	int rows,cols, bar_len;
	int i,j, black;

#ifdef _UNICODE
	len = ucs_to_utf8(text, -1, NULL, MAX_LONG);
#else
	len = mbs_to_utf8(text, -1, NULL, MAX_LONG);
#endif

	if (!len)
		return NULL;

	buf = (byte_t*)xmem_alloc(len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, -1, buf, len);
#else
	mbs_to_utf8(text, -1, buf, len);
#endif

	bar_len = qr_encode(buf, len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return NULL;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = qr_encode(buf, len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	winDC = GetDC(NULL);
	unit = GetDeviceCaps(rdc, LOGPIXELSX) / GetDeviceCaps(winDC, LOGPIXELSX);
	if (unit < 2)
		unit = 2;
	ReleaseDC(NULL, winDC);

	wBrush = CreateSolidBrush(RGB(255, 255, 255));
	bBrush = CreateSolidBrush(RGB(0, 0, 0));

	newBmp = CreateCompatibleBitmap(rdc, w, h);
	memDC = CreateCompatibleDC(rdc);

	orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	rt.left = 0;
	rt.right = w;
	rt.top = 0;
	rt.bottom = h;

	FillRect(memDC, &rt, wBrush);

	rt.left = unit;
	rt.right = w;
	rt.top = unit;
	rt.bottom = h;

	rt.left = rt.right = unit;
	rt.top = rt.bottom = unit;

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.left = rt.right = unit;
		rt.top = rt.bottom;
		rt.bottom = rt.top + unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.left = rt.right;
				rt.right = rt.left + unit;

				black = (c & b) ? 1 : 0;

				if (black)
				{
					FillRect(memDC, &rt, bBrush);
				}

				b = b >> 1;
			}
		}
	}

	SelectObject(memDC, orgBmp);
	DeleteDC(memDC);

	DeleteObject(wBrush);
	DeleteObject(bBrush);

	xmem_free(bar_buf);

	return newBmp;
}

/*******************************************************************************/
void __cdecl _get_bitmap_size(res_bmp_t rb, long* pw, long* ph)
{
	BITMAP bmp;

	GetObject(rb, sizeof(bmp), (void*)&bmp);

	*pw = bmp.bmWidth;
	*ph = bmp.bmHeight;
}

dword_t __cdecl _get_bitmap_bytes(res_bmp_t rb)
{
	BITMAP bmp;
	WORD cClrBits;
	DWORD dwClrUsed;
	DWORD dwSizeImage;
	DWORD dwTotal;

	if (!GetObject(rb, sizeof(BITMAP), (LPSTR)&bmp))
		return 0;

	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else
		cClrBits = 32;

	if (cClrBits < 24)
		dwClrUsed = (1 << cClrBits);
	else
		dwClrUsed = 0;

	dwSizeImage = ((bmp.bmWidth * cClrBits + 31) & ~31) / 8 * bmp.bmHeight;

	dwTotal = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwClrUsed * sizeof(RGBQUAD) + dwSizeImage);

	return dwTotal;
}

#ifdef WINCE
res_bmp_t __cdecl _load_bitmap_from_file(res_dc_t rdc, const tchar_t* filename)
{
	HANDLE handle;
	byte_t* buf = NULL;
	dword_t size;
	XHANDLE fh;
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	if (is_null(filename))
		return 0;

	if (!xfile_info(NULL, filename, NULL, fsize, NULL, NULL))
		return 0;

	fh = xfile_open(NULL, filename, _T('r'));
	if (fh == NULL)
		return 0;

	if (is_huge_size(fsize))
	{
		xfile_close(fh);
		return 0;
	}

	if (is_zero_size(fsize))
	{
		xfile_close(fh);
		return 0;
	}

	size = xstol(fsize);
	buf = (byte_t*)xmem_alloc(size);

	xfile_read(fh, buf, &size);
	xfile_close(fh);

	handle = _load_bitmap_from_bytes(rdc, buf, size);
	xmem_free(buf);

	return (res_bmp_t)handle;
}
#else
res_bmp_t __cdecl _load_bitmap_from_file(res_dc_t rdc, const tchar_t* filename)
{
	HANDLE handle;
	IPicture* p = NULL;
	IStream* s = NULL;
	HGLOBAL hb = NULL;
	void* buf = NULL;
	dword_t size;
	XHANDLE fh;
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	if (is_null(filename))
		return 0;

	if (!xfile_info(NULL, filename, NULL, fsize, NULL, NULL))
		return 0;

	fh = xfile_open(NULL, filename, _T('r'));
	if (fh == NULL)
		return 0;

	if (is_huge_size(fsize))
	{
		xfile_close(fh);
		return 0;
	}

	if (is_zero_size(fsize))
	{
		xfile_close(fh);
		return 0;
	}

	size = xstol(fsize);
	
	hb = GlobalAlloc(GHND, size);
	buf = GlobalLock(hb);

	xfile_read(fh, (byte_t*)buf, &size);
	xfile_close(fh);

	if (!hb)
		return 0;

	CreateStreamOnHGlobal(hb, FALSE, &s);
	if (s == NULL)
	{
		GlobalUnlock(hb);
		GlobalFree(hb);
		return 0;
	}

	OleLoadPicture(s, 0, FALSE, IID_IPicture, (void**)&p);
	s->Release();
	GlobalUnlock(hb);
	GlobalFree(hb);

	if (p == NULL)
		return NULL;

	p->get_Handle((OLE_HANDLE *)&handle);

	handle = (HANDLE)CopyImage(handle, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_COPYRETURNORG);

	p->Release();

	return (res_bmp_t)handle;
}
#endif

res_bmp_t __cdecl _load_bitmap_from_bytes(res_dc_t rdc, const byte_t* pb, dword_t bytes)
{
	PBITMAPINFO pbmi;
	BITMAPFILEHEADER bfh;
	LPBYTE lpBits;

	if (!pb)
		return NULL;

	if ((DWORD)bytes < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
		return NULL;

	xmem_copy((void*)&bfh, (void*)pb, sizeof(BITMAPFILEHEADER));

	if (bfh.bfType != 0x4d42)
		return NULL;

	if ((DWORD)bytes < bfh.bfSize)
		return NULL;

	pbmi = (PBITMAPINFO)(pb + sizeof(BITMAPFILEHEADER));
	lpBits = (LPBYTE)(pb + bfh.bfOffBits);
#ifdef WINCE
	return CreateDIBSection(rdc, pbmi, DIB_RGB_COLORS, NULL, NULL, bfh.bfOffBits);
#else
	return  CreateDIBitmap(rdc, (BITMAPINFOHEADER*)pbmi, CBM_INIT, lpBits, pbmi, DIB_RGB_COLORS);
#endif
}

dword_t __cdecl _save_bitmap_to_bytes(res_dc_t rdc, res_bmp_t rb, byte_t* buf, dword_t max)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;
	BITMAPFILEHEADER bfh;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwTotal;
#ifdef WINCE
	HBITMAP hSec;
	void* pbuf;
#endif

	if (!GetObject(rb, sizeof(BITMAP), (LPSTR)&bmp))
		return 0;

	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else
		cClrBits = 32;

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)xmem_alloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (DWORD)(1 << cClrBits));
	else
		pbmi = (PBITMAPINFO)xmem_alloc(sizeof(BITMAPINFOHEADER));

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
	else
		pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * pbmi->bmiHeader.biHeight;
	pbmi->bmiHeader.biClrImportant = 0;

	pbih = (PBITMAPINFOHEADER)pbmi;

	bfh.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
	bfh.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD));

	if (pbih->biSizeImage > (DWORD)max)
	{
		xmem_free(pbmi);
		return 0;
	}

	dwTotal = 0;
	if (buf)
	{
		xmem_copy((void*)(buf + dwTotal), (void*)&bfh, sizeof(BITMAPFILEHEADER));
	}
	dwTotal += sizeof(BITMAPFILEHEADER);

	if (buf)
	{
		xmem_copy((void*)(buf + dwTotal), (void*)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD));
	}
	dwTotal += sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD);

	if (buf)
	{
		lpBits = (LPBYTE)(buf + dwTotal);
	}
	dwTotal += pbih->biSizeImage;

	if(!buf)
	{
		xmem_free(pbmi);

		return dwTotal;
	}

#ifdef WINCE
	pbuf = NULL;
	hSec = CreateDIBSection(rdc, pbmi, DIB_RGB_COLORS, &pbuf, NULL, bfh.bfOffBits);
	if(!hSec)
	{
		xmem_free(pbmi);
		return 0;
	}
	xmem_copy((void*)lpBits, (void*)pbuf, pbih->biSizeImage);
	DeleteObject(hSec);
#else
	if (!GetDIBits(rdc, rb, 0, (WORD)pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS))
	{
		xmem_free(pbmi);
		return 0;
	}
#endif

	xmem_free(pbmi);

	return dwTotal;
}

#ifdef XDC_SUPPORT_BITMAP_THUMB

res_bmp_t _load_bitmap_from_thumb(res_dc_t rdc, const tchar_t* file)
{
	SHFILEINFO shi = { 0 };

	SHGetFileInfo(file, 0, &shi, sizeof(shi), SHGFI_ICON);

	if (!shi.hIcon)
		return NULL;

	ICONINFO ico = { 0 };
	GetIconInfo(shi.hIcon, &ico);

	int w = ico.xHotspot * 2;
	int h = ico.yHotspot * 2;

	RECT rt;
	rt.top = rt.bottom = 0;
	rt.right = w;
	rt.bottom = h;

	HDC hdc = CreateCompatibleDC(rdc);
	HBITMAP hbmp = CreateCompatibleBitmap(rdc, w, h);
	HBITMAP horg = (HBITMAP)SelectObject(hdc, hbmp);

	HBRUSH hBrush = CreateSolidBrush(RGB(250, 250, 250));

	FillRect(hdc, &rt, hBrush);

	DrawIcon(hdc, 0, 0, shi.hIcon);

	DeleteObject(hBrush);
	hbmp = (HBITMAP)SelectObject(hdc, horg);
	DeleteDC(hdc);

	DeleteObject(ico.hbmColor);
	DeleteObject(ico.hbmMask);

	return hbmp;
}

res_bmp_t _load_bitmap_from_icon(res_dc_t rdc, const tchar_t* iname)
{
	HICON hIcon;

	if (compare_text(iname, -1, ICON_NAME_QUESTION, -1, 1) == 0)
		hIcon = LoadIcon(NULL, IDI_QUESTION);
	else if (compare_text(iname, -1, ICON_NAME_EXCLAMATION, -1, 1) == 0)
		hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
	else if (compare_text(iname, -1, ICON_NAME_INFORMATION, -1, 1) == 0)
		hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
	else if (compare_text(iname, -1, ICON_NAME_WARING, -1, 1) == 0)
		hIcon = LoadIcon(NULL, IDI_WARNING);
	else if (compare_text(iname, -1, ICON_NAME_ERROR, -1, 1) == 0)
		hIcon = LoadIcon(NULL, IDI_ERROR);
	else if (compare_text(iname, -1, ICON_NAME_HAND, -1, 1) == 0)
		hIcon = LoadIcon(NULL, IDI_HAND);
	else if (compare_text(iname, -1, ICON_NAME_ASTERISK, -1, 1) == 0)
		hIcon = LoadIcon(NULL, IDI_ASTERISK);
	else
		hIcon = LoadIcon(NULL, IDI_APPLICATION);

	ICONINFO ico = { 0 };
	GetIconInfo(hIcon, &ico);

	int w = ico.xHotspot * 2;
	int h = ico.yHotspot * 2;

	RECT rt;
	rt.top = rt.bottom = 0;
	rt.right = w;
	rt.bottom = h;

	HDC hdc = CreateCompatibleDC(rdc);
	HBITMAP hbmp = CreateCompatibleBitmap(rdc, w, h);
	HBITMAP horg = (HBITMAP)SelectObject(hdc, hbmp);

	HBRUSH hBrush = CreateSolidBrush(RGB(250, 250, 250));

	FillRect(hdc, &rt, hBrush);

	DrawIcon(hdc, 0, 0, hIcon);

	DeleteObject(hBrush);
	hbmp = (HBITMAP)SelectObject(hdc, horg);
	DeleteDC(hdc);

	DeleteObject(ico.hbmColor);
	DeleteObject(ico.hbmMask);

	return hbmp;
}

#endif //XDC_SUPPORT_BITMAP_THUMB