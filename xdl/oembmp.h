/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc bitmap document

	@module	oembmp.h | bitmap interface file

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

#ifndef _OEMBMP_H
#define _OEMBMP_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_BMP

typedef struct _bmp_file_t{
	/*file info header*/
	unsigned short flag; /*file flag*/
	unsigned long fsize; /*file size*/
	unsigned short rev1;
	unsigned short rev2;
	unsigned long offset; /*image bytes position*/
}bmp_file_t;

typedef struct _bmp_info_t{
	/*bmp info header*/
	unsigned long isize; /*bitmap info struct size, set to 40 bytes*/
	unsigned long width; /*bitmap cols*/
	unsigned long height; /*bitmap rows*/
	unsigned short planes; /*number of planes for the target device, set to 1*/
	unsigned short clrbits; /*number of bits-per-pixel*/
	unsigned long compress; /*type of compression, set to 0*/
	unsigned long bytes; /*image bytes*/
	unsigned long xpelsperm; /*the horizontal resolution, in pixels-per-meter*/
	unsigned long ypelsperm; /*the vertical resolution, in pixels-per-meter*/
	unsigned long clrused; /*the number of color indexes in the color table*/
	unsigned long clrimport; /*the number of color indexes that are required for displaying the bitmap*/
}bmp_info_t;

#define BMP_FLAG		0x4d42 /*"B","M"*/
#define BMP_FILEHEADER_SIZE		14
#define BMP_INFOHEADER_SIZE		40
#define BMP_RGBQUAD_SIZE		4

#define BMPFILEHEADERPTR(p)			((unsigned char*)p)
#define BMPINFOHEADERPTR(p)			((unsigned char*)p + BMP_FILEHEADER_SIZE)
#define BMPQUADHEADERPTR(p)			((unsigned char*)p + BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE)

#define BMP_FILL_QUAD(p, a, r, g, b) do{PUT_BYTE(p, 0, b);PUT_BYTE(p, 1, g);PUT_BYTE(p, 2, r);PUT_BYTE(p, 3, a);}while(0);
#define BMP_FILL_RGB(p, r, g, b) do{PUT_BYTE(p, 0, b);PUT_BYTE(p, 1, g);PUT_BYTE(p, 2, r);}while(0);

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API dword_t xbmp_set_header(const bmp_file_t* pbf, const bmp_info_t* pbi, unsigned char* buf, unsigned long max);

	XDL_API dword_t xbmp_get_header(bmp_file_t* pbf, bmp_info_t* pbi, const unsigned char* src, unsigned long len);

	XDL_API dword_t xbmp_fill_quad(const bmp_info_t* pbi, unsigned char* buf, unsigned long max);

	XDL_API	dword_t xbmp_convgray(const byte_t* src, dword_t len, byte_t* buf, dword_t max);

	XDL_API	dword_t xbmp_convbina(const byte_t* src, dword_t len, byte_t* buf, dword_t max);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_BMP*/

#endif /*OEMBMP_H*/