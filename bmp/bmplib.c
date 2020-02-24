/*
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MPL as stated above or under the terms of the GNU
 * Library General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Library general Public License for more
 * details.
 */

#include <stdlib.h>

#include "bmplib.h"

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

unsigned long bmp_set_header(const bmp_file_t* pbf, const bmp_info_t* pbi, unsigned char* buf, unsigned long max)
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

unsigned long bmp_get_header(bmp_file_t* pbf, bmp_info_t* pbi, const unsigned char* src, unsigned long len)
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

unsigned long bmp_fill_quad(const bmp_info_t* pbi, unsigned char* buf, unsigned long max)
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