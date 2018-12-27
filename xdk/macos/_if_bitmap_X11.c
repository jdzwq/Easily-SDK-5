/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc bitmap document

	@module	if_bitmap_win.c | bitmap windows implement file

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

#ifdef XDK_SUPPORT_CONTEXT_BITMAP


static void _CenterRect(XRectangle* pRect, int src_width, int src_height)
{
	if (pRect->width > (unsigned short)src_width)
	{
		pRect->x = pRect->x + (pRect->width - (unsigned short)src_width) / 2;
		pRect->width = (unsigned short)src_width;
	}
	if (pRect->height > (unsigned short)src_height)
	{
		pRect->y = pRect->y + (pRect->height - (unsigned short)src_height) / 2;
		pRect->height = (unsigned short)src_height;
	}
}

res_bmp_t _select_compatible_bitmap(res_ctx_t rdc, res_bmp_t bmp)
{
    return NULL;
}

res_bmp_t _create_compatible_bitmap(res_ctx_t rdc, long cx, long cy)
{
    X11_suface_t* ctx = (X11_suface_t*)rdc;
    int clr_bk;
    Pixmap pmp;
    XImage* pim;
    
    clr_bk = BlackPixel(g_display, DefaultScreen(g_display));
    
    XSetForeground(g_display, ctx->device, clr_bk);
    
	pmp = XCreatePixmap (g_display, ctx->device, cx, cy, 24);
    
    XFillRectangle(g_display, pmp, rdc, 0, 0, cx, cy);
    
    XFlush(g_display);
    
    pim = XGetImage(g_display, pmp, 0, 0, cx, cy, AllPlanes, ZPixmap);
    
    XFreePixmap(g_display, pmp);
    
    return pim;
}

void _destroy_bitmap(res_bmp_t bmp)
{
	XDestroyImage(bmp);
}

void _get_bitmap_size(res_bmp_t rb, long* pw, long* ph)
{
    XImage* pbi = (XImage*)rb;

    if(pw) *pw = pbi->width;
    
    if(ph) *ph = pbi->height;
}

res_bmp_t _create_color_bitmap(res_ctx_t rdc, const xcolor_t* pxc, long w, long h)
{
    XImage* pim;
    Visual *pvi;
    char* buf;
    int row,col;
    
    pvi = DefaultVisual(g_display, DefaultScreen(g_display));
    
    buf = calloc(1, w * h * 4);
    
    for(row = 0; row < h;row ++)
    {
        for(col = 0;col < 4 * w;col+=4)
        {
            buf[row * col] = (char)pxc->r;
            buf[row * col + 1] = (char)pxc->g;
            buf[row * col + 2] = (char)pxc->b;
        }
    }
    
    pim = XCreateImage(g_display, pvi, 24, ZPixmap, 0, buf, w, h, 32, w * sizeof(int));
    
    //free(buf);
    
    return pim;
}

res_bmp_t _create_pattern_bitmap(res_ctx_t rdc, const xcolor_t* pxc_front, const xcolor_t* pxc_back, long w, long h, const tchar_t* lay)
{
    return NULL;
}

res_bmp_t _create_gradient_bitmap(res_ctx_t rdc, const xcolor_t* pxc_near, const xcolor_t* pxc_center, long w, long h, const tchar_t* type)
{
	return NULL;
}

res_bmp_t _create_code128_bitmap(res_ctx_t rdc, long w, long h, const unsigned char* bar_buf, size_t bar_len, const tchar_t* text)
{
	return NULL;
}

res_bmp_t _create_pdf417_bitmap(res_ctx_t rdc, long w, long h, const unsigned char* bar_buf, size_t bar_len, int rows, int cols)
{
	return NULL;
}

res_bmp_t _create_qrcode_bitmap(res_ctx_t rdc, long w, long h, const unsigned char* bar_buf, size_t bar_len, int rows, int cols)
{
	return NULL;
}

res_bmp_t _create_storage_bitmap(res_ctx_t rdc, const tchar_t* filename)
{
	return NULL;
}

/*******************************************************************************/
#pragma pack (2)
typedef struct _bitmap_filehead_t
{
    unsigned short type;		//bitmap file type
    unsigned long size;		//bitmap file size
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long offset;		//bitmap data offset from file header
}bitmap_filehead_t;		//14 bytes
#pragma pack ()

typedef struct _bitmap_infohead_t{
    unsigned long size;		//struct size
    unsigned long width;		//bitmap point width
    unsigned long height;		//bitmap point height
    unsigned short planes;		//number of planes for the target device, set to 1
    unsigned short bitcount;	//the number of bits-per-pixel. 1:is monochrome; 4:maximum of 16 colors; 8:maximum of 256 colors; 16:maximum of 2^16 colors; 24~; 32~;
    unsigned long compression; //type of compression.0: uncompressed format; 1: RLE format for bitmaps with 8 bpp; 2:RLE format for bitmaps with 4 bpp.
    unsigned long imagesize;	// the size, in bytes, of the image
    unsigned long horzpixels;	//the horizontal resolution, in pixels-per-meter
    unsigned long vertpixels;	//the vertical resolution, in pixels-per-meter
    unsigned long clrused;	// the number of color indexes in the color table  that are actually used by the bitmap
    unsigned long clrimportant;//the number of color indexes that are required for displaying the bitmap
}bitmap_infohead_t;			//40 bytes

typedef struct _bitmap_rgbquad_t{
    unsigned char blue;		//blue lighten(0-255)
    unsigned char green;		//green lighten(0-255)
    unsigned char red;			//red lighten(0-255)
    unsigned char reserved;	//set to zero
}bitmap_rgbquad_t;


size_t _get_bitmap_bytes(res_bmp_t rb)
{
    XImage* pim = (XImage*)rb;
    
	unsigned short cClrBits;
	unsigned long dwClrUsed;
	unsigned long dwSizeImage;
	unsigned long dwTotal;

    cClrBits = pim->bitmap_unit;

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

    dwSizeImage = ((pim->width * cClrBits + 31) & ~31) / 8 * pim->height;

	dwTotal = (unsigned long)(sizeof(bitmap_filehead_t) + sizeof(bitmap_infohead_t) + dwClrUsed * sizeof(bitmap_rgbquad_t) + dwSizeImage);

	return (size_t)dwTotal;
}

res_bmp_t _load_bitmap_from_bytes(res_ctx_t rdc, const unsigned char* pb, size_t bytes)
{
	bitmap_infohead_t* pbmi;
	bitmap_filehead_t bfh;
	char* lpBits;
    
    Visual* pvi;
    XImage* pim;

	if (!pb)
		return NULL;

	if ((unsigned long)bytes < sizeof(bitmap_filehead_t) + sizeof(bitmap_infohead_t))
		return NULL;

	memcpy((void*)&bfh, (void*)pb, sizeof(bitmap_filehead_t));

	if (bfh.type != 0x4d42)
		return NULL;

	if ((unsigned long)bytes < bfh.size)
		return NULL;

	pbmi = (bitmap_infohead_t*)(pb + sizeof(bitmap_filehead_t));
    
	lpBits = (char*)(pb + bfh.offset);
    
    pvi = DefaultVisual(g_display, DefaultScreen(g_display));
    
    pim = XCreateImage(g_display, pvi, 24, ZPixmap, 0, lpBits, pbmi->width, pbmi->height, 32, pbmi->width * sizeof(int));
    
    return pim;
}

size_t _save_bitmap_to_bytes(res_ctx_t rdc, res_bmp_t rb, unsigned char* buf, size_t max)
{
	XImage* pim = (XImage*)rb;
    
	bitmap_infohead_t* pbmi;
	unsigned short    cClrBits;
	bitmap_filehead_t bfh;
	char* lpBits;
	unsigned long dwTotal;

	cClrBits = (unsigned short)(pim->bitmap_unit);

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
		pbmi = (bitmap_infohead_t*)calloc(1, sizeof(bitmap_infohead_t) + sizeof(bitmap_rgbquad_t) * (unsigned long)(1 << cClrBits));
	else
		pbmi = (bitmap_infohead_t*)calloc(1, sizeof(bitmap_infohead_t));

	pbmi->size = sizeof(bitmap_infohead_t);
	pbmi->width = pim->width;
	pbmi->height = pim->height;
	pbmi->planes = 1;
	pbmi->bitcount = pim->bits_per_pixel;
	if (cClrBits < 24)
		pbmi->clrused = (1 << cClrBits);
	else
		pbmi->clrused = 0;
	pbmi->compression = 0;
	pbmi->imagesize = ((pbmi->width * cClrBits + 31) & ~31) / 8 * pbmi->height;
	pbmi->clrimportant = 0;

	bfh.type = 0x4d42;        // 0x42 = "B" 0x4d = "M"
	bfh.size = (unsigned long)(sizeof(bitmap_filehead_t) + sizeof(bitmap_infohead_t) + pbmi->clrused * sizeof(bitmap_rgbquad_t) + pbmi->imagesize);
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.offset = (unsigned long)(sizeof(bitmap_filehead_t) + sizeof(bitmap_infohead_t) + pbmi->clrused * sizeof(bitmap_rgbquad_t));

	if (pbmi->imagesize > (unsigned long)max)
	{
		free(pbmi);
		return 0;
	}

	dwTotal = 0;
	if (buf)
	{
		memcpy((void*)(buf + dwTotal), (void*)&bfh, sizeof(bitmap_filehead_t));
	}
	dwTotal += sizeof(bitmap_filehead_t);

	if (buf)
	{
		memcpy((void*)(buf + dwTotal), (void*)pbmi, sizeof(bitmap_infohead_t) + pbmi->clrused * sizeof(bitmap_rgbquad_t));
	}
	dwTotal += sizeof(bitmap_infohead_t) + pbmi->clrused * sizeof(bitmap_rgbquad_t);

	if (buf)
	{
		lpBits = (char*)(buf + dwTotal);
	}
	else
	{
		lpBits = NULL;
	}
	dwTotal += pbmi->imagesize;

	if (buf)
	{
        memcpy(lpBits, pim->data, pbmi->imagesize);
	}

	free(pbmi);

	return (size_t)dwTotal;
}

#ifdef XDK_SUPPORT_SHELL
res_bmp_t _load_bitmap_from_icon(res_ctx_t rdc, const tchar_t* iname)
{
	/*HICON hIcon;
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

	hdc = CreateCompatibleDC(rdc);
	hbmp = CreateCompatibleBitmap(rdc, w, h);
	horg = (HBITMAP)SelectObject(hdc, hbmp);

	hBrush = CreateSolidBrush(RGB(250, 250, 250));

	FillRect(hdc, &rt, hBrush);

	DrawIcon(hdc, 0, 0, hIcon);

	DeleteObject(hBrush);
	hbmp = (HBITMAP)SelectObject(hdc, horg);
	DeleteDC(hdc);

	DeleteObject(ico.hbmColor);
	DeleteObject(ico.hbmMask);

	return hbmp;*/
    
    return NULL;
}

res_bmp_t _load_bitmap_from_thumb(res_ctx_t rdc, const tchar_t* file)
{
	/*SHFILEINFO shi = { 0 };
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

	hdc = CreateCompatibleDC(rdc);
	hbmp = CreateCompatibleBitmap(rdc, w, h);
	horg = (HBITMAP)SelectObject(hdc, hbmp);

	hBrush = CreateSolidBrush(RGB(250, 250, 250));

	FillRect(hdc, &rt, hBrush);

	DrawIcon(hdc, 0, 0, shi.hIcon);

	DeleteObject(hBrush);
	hbmp = (HBITMAP)SelectObject(hdc, horg);
	DeleteDC(hdc);

	DeleteObject(ico.hbmColor);
	DeleteObject(ico.hbmMask);

	return hbmp;*/
    
    return NULL;
}
#endif //XDK_SUPPORT_SHELL

#endif //XDK_SUPPORT_CONTEXT_BITMAP
