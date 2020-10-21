/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc bitmap document

	@module	oembmp.c | implement file

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

#include "oembmp.h"


/***************** not used, defined only for bitmap infowmation demo***********************************
typedef struct _bitmap_filehead_t
{
sword_t type;		//bitmap file type
dword_t size;		//bitmap file size
sword_t reserved1;
sword_t reserved2;
dword_t offset;		//bitmap data offset from file header
}bitmap_filehead_t;		//14 bytes

typedef struct _bitmap_infohead_t{
dword_t size;		//struct size
dword_t width;		//bitmap point width
dword_t height;		//bitmap point height
sword_t planes;		//number of planes for the target device, set to 1
sword_t bitcount;	//the number of bits-per-pixel. 1:is monochrome; 4:maximum of 16 colors; 8:maximum of 256 colors; 16:maximum of 2^16 colors; 24~; 32~;
dword_t compression; //type of compression.0: uncompressed format; 1: RLE format for bitmaps with 8 bpp; 2:RLE format for bitmaps with 4 bpp.
dword_t imagesize;	// the size, in bytes, of the image
dword_t horzpixels;	//the horizontal resolution, in pixels-per-meter
dword_t vertpixels;	//the vertical resolution, in pixels-per-meter
dword_t clrused;	// the number of color indexes in the color table  that are actually used by the bitmap
dword_t clrimportant;//the number of color indexes that are required for displaying the bitmap
}bitmap_infohead_t;			//40 bytes

typedef struct _bitmap_rgbquad_t{
byte_t blue;		//blue lighten(0-255)
byte_t green;		//green lighten(0-255)
byte_t red;			//red lighten(0-255)
byte_t reserved;	//set to zero
}bitmap_rgbquad_t;
******************************************************************************************************/

#define PUT_UC(buf,off,n)		(buf[off] = (unsigned char)((n) & 0xFF))
#define PUT_US(buf,off,n)		(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF))
#define PUT_UL(buf,off,n)		(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+2] = (unsigned char) (((n) >> 16) & 0xFF), buf[off+3] = (unsigned char) (((n) >> 24) & 0xFF))

#define GET_UC(buf,off)			((unsigned char)(buf[off] & 0xFF))
#define GET_US(buf,off)			(unsigned short)((buf[off] & 0x00FF) | ((buf[off + 1] << 8) & 0xFF00))
#define GET_UL(buf,off)			(unsigned long)((buf[off] & 0x000000FF) | ((buf[off + 1] << 8) & 0x0000FF00) | ((buf[off + 2] << 16) & 0x00FF0000) | ((buf[off + 3] << 24) & 0xFF000000))

typedef struct _bmp_quad_t{
	unsigned char blue;		//blue lighten(0-255)
	unsigned char green;		//green lighten(0-255)
	unsigned char red;			//red lighten(0-255)
	unsigned char reserved;	//set to zero
}bmp_quad_t;

static bmp_quad_t QUAD2[] = {
	{ 0x00, 0x00, 0x00, 0x00 },
	{ 0xFF, 0xFF, 0xFF, 0x00 },
};

static bmp_quad_t QUAD16[] = {
	{ 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x80, 0x00 },
	{ 0x00, 0x80, 0x00, 0x00 },
	{ 0x00, 0x80, 0x80, 0x00 },
	{ 0x80, 0x00, 0x00, 0x00 },
	{ 0x80, 0x00, 0x80, 0x00 },
	{ 0x80, 0x80, 0x00, 0x00 },
	{ 0x80, 0x80, 0x80, 0x00 },
	{ 0xC0, 0xC0, 0xC0, 0x00 },
	{ 0x00, 0x00, 0xFF, 0x00 },
	{ 0x00, 0xFF, 0x00, 0x00 },
	{ 0x00, 0xFF, 0xFF, 0x00 },
	{ 0xFF, 0x00, 0x00, 0x00 },
	{ 0xFF, 0x00, 0xFF, 0x00 },
	{ 0xFF, 0xFF, 0x00, 0x00 },
	{ 0xFF, 0xFF, 0xFF, 0x00 },
};
/******************************************************************************************************************/
static unsigned long _bmp_file_size(const bmp_info_t* pbi)
{
	unsigned short sPixBytes;
	unsigned long pixWidth;
	unsigned short sMapCount;

	if (pbi->clrbits == 1)
		sMapCount = 2;
	else if (pbi->clrbits == 4)
		sMapCount = 16;
	else if (pbi->clrbits == 8)
		sMapCount = 256;
	else
		sMapCount = 0;

	sPixBytes = ((pbi->width * pbi->clrbits + 31) & ~31) / 8;
	pixWidth = sPixBytes / (pbi->clrbits / 8);

	return BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + sMapCount * BMP_RGBQUAD_SIZE + sPixBytes * pbi->height;
}

static unsigned long _bmp_image_size(const bmp_info_t* pbi)
{
	unsigned short sPixBytes;

	sPixBytes = ((pbi->width * pbi->clrbits + 31) & ~31) / 8;

	return sPixBytes * pbi->height;
}

static unsigned long _bmp_row_size(const bmp_info_t* pbi)
{
	unsigned long sPixBytes;

	sPixBytes = ((pbi->width * pbi->clrbits + 31) & ~31) / 8;

	return sPixBytes;
}

static unsigned long _bmp_image_offset(const bmp_info_t* pbi)
{
	unsigned short sMapCount;

	if (pbi->clrbits == 1)
		sMapCount = 2;
	else if (pbi->clrbits == 4)
		sMapCount = 16;
	else if (pbi->clrbits == 8)
		sMapCount = 256;
	else
		sMapCount = 0;

	return BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + sMapCount * BMP_RGBQUAD_SIZE;
}

static unsigned long _bmp_quad_size(const bmp_info_t* pbi)
{
	int iMapCount;

	if (pbi->clrbits == 1)
		iMapCount = 2;
	else if (pbi->clrbits == 4)
		iMapCount = 16;
	else if (pbi->clrbits == 8)
		iMapCount = 256;
	else
		iMapCount = 0;

	return iMapCount * BMP_RGBQUAD_SIZE;
}

static unsigned long _bmp_quad_count(const bmp_info_t* pbi)
{
	int iMapCount;

	if (pbi->clrbits == 1)
		iMapCount = 2;
	else if (pbi->clrbits == 4)
		iMapCount = 16;
	else if (pbi->clrbits == 8)
		iMapCount = 256;
	else
		iMapCount = 0;

	return iMapCount;
}

dword_t xbmp_set_header(const bmp_file_t* pbf, const bmp_info_t* pbi, unsigned char* buf, unsigned long max)
{
	unsigned char* pb;

	if (buf)
	{
		pb = BMPFILEHEADERPTR(buf);
		PUT_US(pb, 0, pbf->flag); //"B","M"     
		PUT_UL(pb, 2, pbf->fsize);
		PUT_US(pb, 6, 0);
		PUT_US(pb, 8, 0);
		PUT_UL(pb, 10, pbf->offset);

		pb = BMPINFOHEADERPTR(buf);
		PUT_UL(pb, 0, BMP_INFOHEADER_SIZE);
		PUT_UL(pb, 4, pbi->width);
		PUT_UL(pb, 8, pbi->height);
		PUT_US(pb, 12, pbi->planes);
		PUT_US(pb, 14, pbi->clrbits);
		PUT_UL(pb, 16, pbi->compress); // 0
		PUT_UL(pb, 20, pbi->bytes);
		PUT_UL(pb, 24, pbi->xpelsperm);
		PUT_UL(pb, 28, pbi->ypelsperm);
		PUT_UL(pb, 32, pbi->clrused);
		PUT_UL(pb, 36, pbi->clrimport);
	}

	return BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE;
}

dword_t xbmp_get_header(bmp_file_t* pbf, bmp_info_t* pbi, const unsigned char* src, unsigned long len)
{
	unsigned char* pb;

	if (!src || !len)
		return 0;

	pb = BMPFILEHEADERPTR(src);
	pbf->flag = GET_US(pb, 0); //"B","M"  
	if (pbf->flag != BMP_FLAG)
		return 0;

	pbf->fsize = GET_UL(pb, 2);
	//GET_US(pb, 6, 0);
	//GET_US(pb, 8, 0);
	pbf->offset = GET_UL(pb, 10);

	pb = BMPINFOHEADERPTR(src);
	pbi->isize = GET_UL(pb, 0);
	pbi->width = GET_UL(pb, 4);
	pbi->height = GET_UL(pb, 8);
	pbi->planes = GET_US(pb, 12);
	pbi->clrbits = GET_US(pb, 14);
	pbi->compress = GET_UL(pb, 16); // 0
	pbi->bytes = GET_UL(pb, 20);
	pbi->xpelsperm = GET_UL(pb, 24);
	pbi->ypelsperm = GET_UL(pb, 28);
	pbi->clrused = GET_UL(pb, 32);
	pbi->clrimport = GET_UL(pb, 36);

	return BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE;
}

dword_t xbmp_fill_quad(const bmp_info_t* pbi, unsigned char* buf, unsigned long max)
{
	unsigned char* pb;
	unsigned long i;

	if (buf)
	{
		pb = BMPQUADHEADERPTR(buf);

		if (pbi->clrbits == 1)
		{
			for (i = 0; i < pbi->clrused; i++)
			{
				PUT_UC(pb, 0, QUAD2[i].blue);
				PUT_UC(pb, 1, QUAD2[i].green);
				PUT_UC(pb, 2, QUAD2[i].red);
				PUT_UC(pb, 3, 0);

				pb += BMP_RGBQUAD_SIZE;
			}
		}
		else if (pbi->clrbits == 4)
		{
			for (i = 0; i < pbi->clrused; i++)
			{
				PUT_UC(pb, 0, QUAD16[i].blue);
				PUT_UC(pb, 1, QUAD16[i].green);
				PUT_UC(pb, 2, QUAD16[i].red);
				PUT_UC(pb, 3, 0);

				pb += BMP_RGBQUAD_SIZE;
			}
		}if (pbi->clrbits == 8)
		{
			for (i = 0; i < pbi->clrused; i++)
			{
				PUT_UC(pb, 0, i);
				PUT_UC(pb, 1, i);
				PUT_UC(pb, 2, i);
				PUT_UC(pb, 3, 0);

				pb += BMP_RGBQUAD_SIZE;
			}
		}
	}

	return pbi->clrused * BMP_RGBQUAD_SIZE;
}

bool_t xbmp_get_size(xsize_t* ps, const unsigned char* src, unsigned long len)
{
	unsigned char* pb;

	if (!src || !len)
		return 0;

	pb = BMPFILEHEADERPTR(src);
	if (GET_US(pb, 0) != BMP_FLAG)
		return 0;

	pb = BMPINFOHEADERPTR(src);
	ps->w = GET_UL(pb, 4);
	ps->w = GET_UL(pb, 8);
	
	return 1;
}

bool_t xbmp_get_rgb(xcolor_t* pc, int row, int col, const unsigned char* src, unsigned long len)
{
	unsigned char* pb;
	int width, height, offset, clrbits, rowbytes;
	int a;

	byte_t *src_pbf;
	byte_t *src_pbq;
	byte_t* src_pbm;
	byte_t *rgb_buf;

	if (!src || !len)
		return 0;

	pb = BMPFILEHEADERPTR(src);
	if (GET_US(pb, 0) != BMP_FLAG)
		return 0;

	offset = GET_UL(pb, 10);

	pb = BMPINFOHEADERPTR(src);
	width = GET_UL(pb, 4);
	height = GET_UL(pb, 8);
	clrbits = GET_US(pb, 14);

	if(col >= width || row >= height)
		return 0;

	src_pbq = BMPQUADHEADERPTR(src);
	src_pbm = src + offset;

	rowbytes = ((width * clrbits + 31) & ~31) / 8;

	switch(clrbits)
	{
	case 1:
		rgb_buf = src_pbm + row * rowbytes + col / 4;

		if((col % 4) == 0 && rgb_buf[0] & 0x80)
		{
			pc->b = 0xFF;
			pc->g = 0xFF;
			pc->r = 0xFF;
		}
		else
		{
			pc->b = 0x00;
			pc->g = 0x00;
			pc->r = 0x00;
		}
		
		if((col % 4) == 1 && rgb_buf[0] & 0x40)
		{
			pc->b = 0xFF;
			pc->g = 0xFF;
			pc->r = 0xFF;
		}
		else
		{
			pc->b = 0x00;
			pc->g = 0x00;
			pc->r = 0x00;
		}

		if((col % 4) == 2 && rgb_buf[0] & 0x20)
		{
			pc->b = 0xFF;
			pc->g = 0xFF;
			pc->r = 0xFF;
		}
		else
		{
			pc->b = 0x00;
			pc->g = 0x00;
			pc->r = 0x00;
		}

		if((col % 4) == 3 && rgb_buf[0] & 0x10)
		{
			pc->b = 0xFF;
			pc->g = 0xFF;
			pc->r = 0xFF;
		}
		else
		{
			pc->b = 0x00;
			pc->g = 0x00;
			pc->r = 0x00;
		}

		break;
	case 4:
		rgb_buf = src_pbm + row * rowbytes + col / 2;

		if((col % 2) == 0)
			a = (rgb_buf[0] & 0xF0) >> 4;
		else
			a = (rgb_buf[0] & 0x0F);

		pc->b = GET_BYTE(src_pbq, a);
		pc->g = GET_BYTE(src_pbq, a + 1);
		pc->r = GET_BYTE(src_pbq, a + 2);

		break;
	case 8:
		rgb_buf = src_pbm + row * rowbytes + col;
		a = rgb_buf[0];

		pc->b = GET_BYTE(src_pbq, a);
		pc->g = GET_BYTE(src_pbq, a + 1);
		pc->r = GET_BYTE(src_pbq, a + 2);

		break;
	case 16:
		rgb_buf = src_pbm + row * rowbytes + col * 2;
		a = GET_SWORD_LOC(rgb_buf, 0);
		
		pc->b = (byte_t)(a & 0x001F);
		pc->g = (byte_t)((a & 0x03E0) >> 5);
		pc->r = (byte_t)((a & 0x7C00) >> 10);

		break;
	case 24:
		rgb_buf = src_pbm + row * rowbytes + col * 3;

		pc->b = rgb_buf[0];
		pc->g = rgb_buf[1];
		pc->r = rgb_buf[2];

		break;
	case 32:
		rgb_buf = src_pbm + row * rowbytes + col * 4;

		pc->b = rgb_buf[1];
		pc->g = rgb_buf[2];
		pc->r = rgb_buf[3];

		break;
	}

	return 1;
}

/**********************************************************************************************************/

dword_t xbmp_convgray(const byte_t* src, dword_t len, byte_t* buf, dword_t max)
{
	byte_t *src_pbf, *dst_pbf;
	byte_t *src_pbq, *dst_pbq;
	byte_t *src_pbm, *dst_pbm;
	bmp_file_t src_bf = { 0 };
	bmp_info_t src_bi = { 0 };
	bmp_file_t dst_bf = { 0 };
	bmp_info_t dst_bi = { 0 };
	dword_t srcRowBytes, dstRowBytes;
	sword_t dstClrBits;
	dword_t dstClrUsed;
	dword_t dstTotal;

	dword_t i, j;
	byte_t r, g, b;
	sword_t a;

	if (!src || !len)
		return 0;

	src_pbf = BMPFILEHEADERPTR(src);

	xbmp_get_header(&src_bf, &src_bi, src, len);

	if (src_bf.flag != BMP_FLAG)
		return 0;

	srcRowBytes = ((src_bi.width * src_bi.clrbits + 31) & ~31) / 8;

	src_pbq = BMPQUADHEADERPTR(src);
	src_pbm = src_pbf + src_bf.offset;

	dstClrBits = 8;
	dstClrUsed = (1 << dstClrBits);
	dstRowBytes = ((src_bi.width * dstClrBits + 31) & ~31) / 8;

	dst_bf.flag = BMP_FLAG;
	dst_bf.fsize = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + dstClrUsed * BMP_RGBQUAD_SIZE + dstRowBytes * src_bi.height;
	dst_bf.offset = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + dstClrUsed * BMP_RGBQUAD_SIZE;

	dst_bi.isize = BMP_INFOHEADER_SIZE;
	dst_bi.width = src_bi.width;
	dst_bi.height = src_bi.height;
	dst_bi.planes = 1;
	dst_bi.clrbits = dstClrBits;
	dst_bi.compress = 0;
	dst_bi.bytes = dstRowBytes * src_bi.height;
	dst_bi.xpelsperm = 0;
	dst_bi.ypelsperm = 0;
	dst_bi.clrused = dstClrUsed;
	dst_bi.clrimport = 0;

	if (!buf)
	{
		return dst_bf.fsize;
	}

	dst_pbf = BMPFILEHEADERPTR(buf);

	xbmp_set_header(&dst_bf, &dst_bi, dst_pbf, dst_bf.fsize);

	dst_pbq = BMPQUADHEADERPTR(buf);

	for (i = 0; i < dst_bi.clrused; i++)
	{
		PUT_BYTE(dst_pbq, 0, i);
		PUT_BYTE(dst_pbq, 1, i);
		PUT_BYTE(dst_pbq, 2, i);
		PUT_BYTE(dst_pbq, 3, 0);

		dst_pbq += BMP_RGBQUAD_SIZE;
	}

	dst_pbm = dst_pbf + dst_bf.offset;
	dstTotal = 0;

	for (i = 0; i < src_bi.height; i++)
	{
		for (j = 0; j < srcRowBytes;)
		{
			switch (src_bi.clrbits)
			{
			case 1:
				a = 0x80;
				while (a)
				{
					if (a & src_pbm[j])
						dst_pbm[dstTotal] = 0xFF;
					else
						dst_pbm[dstTotal] = 0x00;

					dstTotal++;
					if (dstTotal >= dstRowBytes)
						break;
					a = a >> 1;
				}
				j++;
				break;
			case 4:
				a = (src_pbm[j] & 0xF0) >> 4;
				b = GET_BYTE(src_pbq, a);
				g = GET_BYTE(src_pbq, a + 1);
				r = GET_BYTE(src_pbq, a + 2);

				a = (byte_t)RGB_GRAY(r, g, b);
				dst_pbm[dstTotal] = a;

				a = (src_pbm[j] & 0x0F);
				b = GET_BYTE(src_pbq, a);
				g = GET_BYTE(src_pbq, a + 1);
				r = GET_BYTE(src_pbq, a + 2);
				j++;

				a = (byte_t)RGB_GRAY(r, g, b);
				dst_pbm[dstTotal + 1] = a;
				dstTotal += 2;
				break;
			case 8:
				a = src_pbm[j];
				b = GET_BYTE(src_pbq, a);
				g = GET_BYTE(src_pbq, a + 1);
				r = GET_BYTE(src_pbq, a + 2);
				j++;

				a = (byte_t)RGB_GRAY(r, g, b);
				dst_pbm[dstTotal] = a;
				dstTotal++;
				break;
			case 16:
				memcpy((void*)&a, src_pbm + j, sizeof(sword_t));
				b = (byte_t)(a & 0x001F);
				g = (byte_t)((a & 0x03E0) >> 5);
				r = (byte_t)((a & 0x7C00) >> 10);
				j += 2;

				a = (byte_t)RGB_GRAY(r, g, b);
				dst_pbm[dstTotal] = a;
				dstTotal++;
				break;
			case 24:
				b = src_pbm[j];
				g = src_pbm[j + 1];
				r = src_pbm[j + 2];
				j += 3;

				a = (byte_t)RGB_GRAY(r, g, b);
				dst_pbm[dstTotal] = a;
				dstTotal++;
				break;
			case 32:
				b = src_pbm[j + 1];
				g = src_pbm[j + 2];
				r = src_pbm[j + 3];
				j += 4;

				a = (byte_t)RGB_GRAY(r, g, b);
				dst_pbm[dstTotal] = a;
				dstTotal++;
				break;
			default:
				j++;
				break;
			}
			if (dstTotal >= dstRowBytes)
				break;
		}
		src_pbm += srcRowBytes;
		dst_pbm += dstRowBytes;
		dstTotal = 0;
	}

	return dst_bf.fsize;
}

dword_t xbmp_convbina(const byte_t* src, dword_t len, byte_t* buf, dword_t max)
{
	byte_t *src_pbf, *dst_pbf;
	byte_t *src_pbq, *dst_pbq;
	byte_t *src_pbm, *dst_pbm;
	bmp_file_t src_bf = { 0 };
	bmp_info_t src_bi = { 0 };
	bmp_file_t dst_bf = { 0 };
	bmp_info_t dst_bi = { 0 };
	dword_t srcRowBytes, dstRowBytes;
	sword_t dstClrBits;
	dword_t dstClrUsed;
	dword_t dstTotal;

	dword_t i, j;
	byte_t r, g, b, q;
	sword_t a;

	if (!src || !len)
		return 0;

	src_pbf = BMPFILEHEADERPTR(src);

	xbmp_get_header(&dst_bf, &src_bi, src, len);

	if (src_bf.flag != BMP_FLAG)
		return 0;

	srcRowBytes = ((src_bi.width * src_bi.clrbits + 31) & ~31) / 8;

	src_pbq = BMPQUADHEADERPTR(src);
	src_pbm = src_pbf + src_bf.offset;

	dstClrBits = 1;
	dstClrUsed = (1 << dstClrBits);
	dstRowBytes = ((src_bi.width * dstClrBits + 31) & ~31) / 8;

	dst_bf.flag = BMP_FLAG;
	dst_bf.fsize = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + dstClrUsed * BMP_RGBQUAD_SIZE + dstRowBytes * src_bi.height;
	dst_bf.offset = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + dstClrUsed * BMP_RGBQUAD_SIZE;

	dst_bi.isize = BMP_INFOHEADER_SIZE;
	dst_bi.width = src_bi.width;
	dst_bi.height = src_bi.height;
	dst_bi.planes = 1;
	dst_bi.clrbits = dstClrBits;
	dst_bi.compress = 0;
	dst_bi.bytes = dstRowBytes * src_bi.height;
	dst_bi.xpelsperm = 0;
	dst_bi.ypelsperm = 0;
	dst_bi.clrused = dstClrUsed;
	dst_bi.clrimport = 0;

	if (!buf)
	{
		return dst_bf.fsize;
	}

	dst_pbf = BMPFILEHEADERPTR(buf);

	xbmp_set_header(&dst_bf, &dst_bi, dst_pbf, dst_bf.fsize);

	dst_pbq = BMPQUADHEADERPTR(buf);
	PUT_BYTE(dst_pbq, 0, 0);
	PUT_BYTE(dst_pbq, 1, 0);
	PUT_BYTE(dst_pbq, 2, 0);
	PUT_BYTE(dst_pbq, 3, 0);

	dst_pbq += BMP_RGBQUAD_SIZE;
	PUT_BYTE(dst_pbq, 0, 0xFF);
	PUT_BYTE(dst_pbq, 1, 0xFF);
	PUT_BYTE(dst_pbq, 2, 0xFF);
	PUT_BYTE(dst_pbq, 3, 0);

	dst_pbm = dst_pbf + dst_bf.offset;
	dstTotal = 0;

	for (i = 0; i < src_bi.height; i++)
	{
		q = 0x80;
		for (j = 0; j < srcRowBytes;)
		{
			switch (src_bi.clrbits)
			{
			case 1:
				dst_pbm[dstTotal] = src_pbm[j];
				j++;
				dstTotal++;
				break;
			case 4:
				a = (src_pbm[j] & 0xF0) >> 4;
				b = GET_BYTE(src_pbq, a);
				g = GET_BYTE(src_pbq, a + 1);
				r = GET_BYTE(src_pbq, a + 2);
				if (b + g + r)
				{
					dst_pbm[dstTotal] |= q;
				}
				q = q >> 1;

				a = (src_pbm[j] & 0x0F);
				b = GET_BYTE(src_pbq, a);
				g = GET_BYTE(src_pbq, a + 1);
				r = GET_BYTE(src_pbq, a + 2);
				if (b + g + r)
				{
					dst_pbm[dstTotal] |= q;
				}
				q = q >> 1;

				j++;
				if (!q || j >= srcRowBytes)
				{
					dstTotal++;
					q = 0x80;
				}
				break;
			case 8:
				a = src_pbm[j];
				b = GET_BYTE(src_pbq, a);
				g = GET_BYTE(src_pbq, a + 1);
				r = GET_BYTE(src_pbq, a + 2);
				if (b + g + r)
				{
					dst_pbm[dstTotal] |= q;
				}
				q = q >> 1;

				j++;
				if (!q || j >= srcRowBytes)
				{
					dstTotal++;
					q = 0x80;
				}
				break;
			case 16:
				memcpy((void*)&a, src_pbm + j, sizeof(sword_t));
				b = (byte_t)(a & 0x001F);
				g = (byte_t)((a & 0x03E0) >> 5);
				r = (byte_t)((a & 0x7C00) >> 10);
				if (b + g + r)
				{
					dst_pbm[dstTotal] |= q;
				}
				q = q >> 1;

				j += 2;
				if (!q || j >= srcRowBytes)
				{
					dstTotal++;
					q = 0x80;
				}
				break;
			case 24:
				b = src_pbm[j];
				g = src_pbm[j + 1];
				r = src_pbm[j + 2];
				if (b + g + r)
				{
					dst_pbm[dstTotal] |= q;
				}
				q = q >> 1;

				j += 3;
				if (!q || j >= srcRowBytes)
				{
					dstTotal++;
					q = 0x80;
				}
				break;
			case 32:
				b = src_pbm[j + 1];
				g = src_pbm[j + 2];
				r = src_pbm[j + 3];
				if (b + g + r)
				{
					dst_pbm[dstTotal] |= q;
				}
				q = q >> 1;

				j += 4;
				if (!q || j >= srcRowBytes)
				{
					dstTotal++;
					q = 0x80;
				}
				break;
			default:
				j++;
				break;
			}
			if (dstTotal >= dstRowBytes)
				break;
		}
		src_pbm += srcRowBytes;
		dst_pbm += dstRowBytes;
		dstTotal = 0;
	}

	return dst_bf.fsize;
}
