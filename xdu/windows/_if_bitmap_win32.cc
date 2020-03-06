/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc bitmap document

	@module	if_bitmap_win.c | windows implement file

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

#ifdef XDU_SUPPORT_CONTEXT_BITMAP

#ifndef WINCE
#pragma comment(lib, "Msimg32.lib")
#endif

#ifdef WINCE
static int MulDiv(int a, int b, int c)
{
	return (int)((float)a * (float)b / (float)c);
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

void _destroy_bitmap(res_bmp_t bmp)
{
	DeleteObject(bmp);
}

void _get_bitmap_size(res_bmp_t rb, int* pw, int* ph)
{
	BITMAP bmp;

	GetObject(rb, sizeof(bmp), (void*)&bmp);

	*pw = bmp.bmWidth;
	*ph = bmp.bmHeight;
}

res_bmp_t _create_context_bitmap(res_ctx_t rdc)
{
	HDC hDC = (HDC)(rdc->context);
	HBITMAP hbmp;

	if (rdc->type != CONTEXT_MEMORY)
		return NULL;

	hbmp = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);
	if (!hbmp)
		return NULL;

	return (HBITMAP)CopyImage(hbmp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_COPYRETURNORG);
}

res_bmp_t _create_color_bitmap(res_ctx_t rdc, const xcolor_t* pxc, int w, int h)
{
	HDC hDC = (HDC)(rdc->context);
	HDC memDC;
	HBRUSH hBrush;
	HBITMAP newBmp, orgBmp;
	RECT rt;

	newBmp = CreateCompatibleBitmap(hDC, w, h);
	memDC = CreateCompatibleDC(hDC);
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

res_bmp_t _create_pattern_bitmap(res_ctx_t rdc, const xcolor_t* pxc_front, const xcolor_t* pxc_back, int w, int h, const tchar_t* lay)
{
	HDC hDC = (HDC)(rdc->context);
	HDC memDC;
	HBRUSH hBrush;
	HPEN newPen, orgPen;
	HBITMAP newBmp, orgBmp;
	RECT rt;
	POINT pt[2];

	newBmp = CreateCompatibleBitmap(hDC, w, h);
	memDC = CreateCompatibleDC(hDC);
	orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	rt.left = rt.top = 0;
	rt.right = w;
	rt.bottom = h;

	hBrush = CreateSolidBrush(RGB(pxc_back->r, pxc_back->g, pxc_back->b));
	FillRect(memDC, &rt, hBrush);
	DeleteObject(hBrush);

	newPen = CreatePen(PS_SOLID, 2, RGB(pxc_front->r, pxc_front->g, pxc_front->b));
	orgPen = (HPEN)SelectObject(memDC, newPen);

	if (lay &&_tcsicmp(lay, GDI_ATTR_LAYOUT_VERT) == 0)
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

res_bmp_t _create_gradient_bitmap(res_ctx_t rdc, const xcolor_t* pxc_near, const xcolor_t* pxc_center, int w, int h, const tchar_t* type)
{
	HDC hDC = (HDC)(rdc->context);
	RECT rt;
	HBITMAP newBmp, orgBmp;
	HDC memDC;
	COLORREF clr_brush, clr_linear;
	TRIVERTEX tv[4];
	GRADIENT_RECT gr[2];
	unsigned int mode = 0;

	rt.left = 0;
	rt.top = 0;
	rt.right = w;
	rt.bottom = h;

	newBmp = CreateCompatibleBitmap(hDC, rt.right - rt.left, rt.bottom - rt.top);
	memDC = CreateCompatibleDC(hDC);
	orgBmp = (HBITMAP)SelectObject(memDC, newBmp);

	clr_brush = RGB(pxc_near->r, pxc_near->g, pxc_near->b);
	clr_linear = RGB(pxc_center->r, pxc_center->g, pxc_center->b);

	if (type && _tcsicmp(type, GDI_ATTR_GRADIENT_TYPE_VERT) == 0)
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

res_bmp_t _create_code128_bitmap(res_ctx_t rdc, int w, int h, const unsigned char* bar_buf, dword_t bar_len, const tchar_t* text)
{
	HDC hDC = (HDC)(rdc->context);
	HDC winDC,memDC;
	HBRUSH wBrush, bBrush;
	HBITMAP newBmp, orgBmp;
	HFONT newFont, orgFont;
	LOGFONT lf = { 0 };
	RECT rt;
	int unit;
	DWORD i;
	int black,span;
	
	winDC = GetDC(NULL);
	unit = GetDeviceCaps(hDC, LOGPIXELSX) / GetDeviceCaps(winDC, LOGPIXELSX);
	ReleaseDC(NULL, winDC);

	wBrush = CreateSolidBrush(RGB(255, 255, 255));
	bBrush = CreateSolidBrush(RGB(0, 0, 0));

	newBmp = CreateCompatibleBitmap(hDC, w, h);
	memDC = CreateCompatibleDC(hDC);

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
	lf.lfHeight = -MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72);

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

	return newBmp;
}

res_bmp_t _create_pdf417_bitmap(res_ctx_t rdc, int w, int h, const unsigned char* bar_buf, dword_t bar_len, int rows, int cols)
{
	HDC hDC = (HDC)(rdc->context);
	HDC winDC, memDC;
	HBRUSH wBrush, bBrush;
	HBITMAP newBmp, orgBmp;
	RECT rt;
	int unit;

	unsigned char b, c;
	int len, i,j, black;

	winDC = GetDC(NULL);
	unit = GetDeviceCaps(hDC, LOGPIXELSX) / GetDeviceCaps(winDC, LOGPIXELSX);
	if (unit < 2)
		unit = 2;
	ReleaseDC(NULL, winDC);

	wBrush = CreateSolidBrush(RGB(255, 255, 255));
	bBrush = CreateSolidBrush(RGB(0, 0, 0));

	newBmp = CreateCompatibleBitmap(hDC, w, h);
	memDC = CreateCompatibleDC(hDC);

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

	return newBmp;
}

res_bmp_t _create_qrcode_bitmap(res_ctx_t rdc, int w, int h, const unsigned char* bar_buf, dword_t bar_len, int rows, int cols)
{
	HDC hDC = (HDC)(rdc->context);
	HDC winDC, memDC;
	HBRUSH wBrush, bBrush;
	HBITMAP newBmp, orgBmp;
	RECT rt;
	int unit;

	unsigned char b, c;
	int len, i,j, black;

	winDC = GetDC(NULL);
	unit = GetDeviceCaps(hDC, LOGPIXELSX) / GetDeviceCaps(winDC, LOGPIXELSX);
	if (unit < 2)
		unit = 2;
	ReleaseDC(NULL, winDC);

	wBrush = CreateSolidBrush(RGB(255, 255, 255));
	bBrush = CreateSolidBrush(RGB(0, 0, 0));

	newBmp = CreateCompatibleBitmap(hDC, w, h);
	memDC = CreateCompatibleDC(hDC);

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

	return newBmp;
}

res_bmp_t _create_storage_bitmap(res_ctx_t rdc, const tchar_t* filename)
{
	HDC hDC = (HDC)(rdc->context);
	HANDLE handle;
	IPicture* p = NULL;
	IStream* s = NULL;
	HGLOBAL hb = NULL;
	dword_t size = 0;
	void* buf = NULL;

	WIN32_FILE_ATTRIBUTE_DATA ad = { 0 };
	HANDLE hFile;
	DWORD dw = 0;
	
	if (is_null(filename))
		return 0;

	if (!GetFileAttributesEx(filename, GetFileExInfoStandard, &ad))
		return 0;

	if (!ad.nFileSizeLow || ad.nFileSizeHigh)
		return 0;

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ((HANDLE)hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	
	size = ad.nFileSizeLow;
	hb = GlobalAlloc(GHND, size);

	if (!hb)
		return 0;

	buf = GlobalLock(hb);

	if (!ReadFile(hFile, buf, (DWORD)size, &dw, NULL))
	{
		GlobalUnlock(hb);
		GlobalFree(hb);
		return 0;
	}

	CloseHandle(hFile);

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
/*******************************************************************************/

dword_t _get_bitmap_bytes(res_bmp_t rb)
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

res_bmp_t _load_bitmap_from_bytes(res_ctx_t rdc, const unsigned char* pb, dword_t bytes)
{
	HDC hDC = (HDC)(rdc->context);
	PBITMAPINFO pbmi;
	BITMAPFILEHEADER bfh;
	LPBYTE lpBits;

	if (!pb)
		return NULL;

	if ((DWORD)bytes < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
		return NULL;

	CopyMemory((void*)&bfh, (void*)pb, sizeof(BITMAPFILEHEADER));

	if (bfh.bfType != 0x4d42)
		return NULL;

	if ((DWORD)bytes < bfh.bfSize)
		return NULL;

	pbmi = (PBITMAPINFO)(pb + sizeof(BITMAPFILEHEADER));
	lpBits = (LPBYTE)(pb + bfh.bfOffBits);
#ifdef WINCE
	return CreateDIBSection(hDC, pbmi, DIB_RGB_COLORS, NULL, NULL, bfh.bfOffBits);
#else
	return  CreateDIBitmap(hDC, (BITMAPINFOHEADER*)pbmi, CBM_INIT, lpBits, pbmi, DIB_RGB_COLORS);
#endif
}

dword_t _save_bitmap_to_bytes(res_ctx_t rdc, res_bmp_t rb, unsigned char* buf, dword_t max)
{
	HDC hDC = (HDC)(rdc->context);
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
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (DWORD)(1 << cClrBits));
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

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
		LocalFree(pbmi);
		return 0;
	}

	dwTotal = 0;
	if (buf)
	{
		CopyMemory((void*)(buf + dwTotal), (void*)&bfh, sizeof(BITMAPFILEHEADER));
	}
	dwTotal += sizeof(BITMAPFILEHEADER);

	if (buf)
	{
		CopyMemory((void*)(buf + dwTotal), (void*)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD));
	}
	dwTotal += sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD);

	if (buf)
	{
		lpBits = (LPBYTE)(buf + dwTotal);
	}
	else
	{
		lpBits = NULL;
	}
	dwTotal += pbih->biSizeImage;

	if (buf)
	{
#ifdef WINCE
		pbuf = NULL;
		hSec = CreateDIBSection(hDC, pbmi, DIB_RGB_COLORS, &pbuf, NULL, bfh.bfOffBits);
		if(!hSec)
		{
			LocalFree(pbmi);
			return 0;
		}
		CopyMemory((void*)lpBits, (void*)pbuf, pbih->biSizeImage);
		DeleteObject(hSec);
#else
		if (!GetDIBits(hDC, rb, 0, (WORD)pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS))
		{
			LocalFree(pbmi);
			return 0;
		}
#endif
	}

	LocalFree(pbmi);

	return dwTotal;
}

#ifdef XDU_SUPPORT_SHELL
res_bmp_t _load_bitmap_from_icon(res_ctx_t rdc, const tchar_t* iname)
{
	HDC hDC = (HDC)(rdc->context);
	HICON hIcon;
	ICONINFO ico = { 0 };

	int w, h;
	RECT rt;
	HDC hdc;
	HBITMAP hbmp, horg;
	HBRUSH hBrush;

	if (_tcsicmp(iname, ICON_QUESTION) == 0)
		hIcon = LoadIcon(NULL, IDI_QUESTION);
	else if (_tcsicmp(iname, ICON_EXCLAMATION) == 0)
		hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
	else if (_tcsicmp(iname, ICON_INFORMATION) == 0)
		hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
	else if (_tcsicmp(iname, ICON_WARING) == 0)
		hIcon = LoadIcon(NULL, IDI_WARNING);
	else if (_tcsicmp(iname, ICON_ERROR) == 0)
		hIcon = LoadIcon(NULL, IDI_ERROR);
	else if (_tcsicmp(iname, ICON_HAND) == 0)
		hIcon = LoadIcon(NULL, IDI_HAND);
	else if (_tcsicmp(iname, ICON_ASTERISK) == 0)
		hIcon = LoadIcon(NULL, IDI_ASTERISK);
	else
		hIcon = LoadIcon(NULL, IDI_APPLICATION);

	GetIconInfo(hIcon, &ico);

	w = ico.xHotspot * 2;
	h = ico.yHotspot * 2;

	rt.top = rt.bottom = 0;
	rt.right = w;
	rt.bottom = h;

	hdc = CreateCompatibleDC(hDC);
	hbmp = CreateCompatibleBitmap(hDC, w, h);
	horg = (HBITMAP)SelectObject(hdc, hbmp);

	hBrush = CreateSolidBrush(RGB(250, 250, 250));

	FillRect(hdc, &rt, hBrush);

	DrawIcon(hdc, 0, 0, hIcon);

	DeleteObject(hBrush);
	hbmp = (HBITMAP)SelectObject(hdc, horg);
	DeleteDC(hdc);

	DeleteObject(ico.hbmColor);
	DeleteObject(ico.hbmMask);

	return hbmp;
}

res_bmp_t _load_bitmap_from_thumb(res_ctx_t rdc, const tchar_t* file)
{
	HDC hDC = (HDC)(rdc->context);
	SHFILEINFO shi = { 0 };
	ICONINFO ico = { 0 };

	RECT rt;
	int w, h;
	HDC hdc;
	HBITMAP hbmp, horg;
	HBRUSH hBrush;

	SHGetFileInfo(file, 0, &shi, sizeof(shi), SHGFI_ICON);

	if (!shi.hIcon)
		return NULL;

	GetIconInfo(shi.hIcon, &ico);

	w = ico.xHotspot * 2;
	h = ico.yHotspot * 2;

	rt.top = rt.bottom = 0;
	rt.right = w;
	rt.bottom = h;

	hdc = CreateCompatibleDC(hDC);
	hbmp = CreateCompatibleBitmap(hDC, w, h);
	horg = (HBITMAP)SelectObject(hdc, hbmp);

	hBrush = CreateSolidBrush(RGB(250, 250, 250));

	FillRect(hdc, &rt, hBrush);

	DrawIcon(hdc, 0, 0, shi.hIcon);

	DeleteObject(hBrush);
	hbmp = (HBITMAP)SelectObject(hdc, horg);
	DeleteDC(hdc);

	DeleteObject(ico.hbmColor);
	DeleteObject(ico.hbmMask);

	return hbmp;
}
#endif //XDU_SUPPORT_SHELL

#endif //XDU_SUPPORT_CONTEXT_BITMAP
