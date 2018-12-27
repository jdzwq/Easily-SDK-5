/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc png document

	@module	oempng.c | png implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is xmem_free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "oempng.h"
#include "imperr.h"
#include "impmem.h"

#ifdef GPL_SUPPORT_PNG


#include <setjmp.h>
#include "../png/png.h"
#include "../bmp/bmplib.h"

#ifdef _OS_WINDOWS
#pragma comment(lib,"png.lib")
#pragma comment(lib,"bmp.lib")
#endif

typedef struct _io_buffer{
	png_bytep buf;
	png_size_t bytes;
	png_size_t pos;
}io_buffer;

void _png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	io_buffer* pio;

	pio = (io_buffer*)(png_get_io_ptr(png_ptr));

	if (pio->pos + length > pio->bytes)
	{
		png_error(png_ptr, "Read Data Failed");
	}

	memcpy((void*)data, (void*)(pio->buf + pio->pos), length);
	pio->pos += length;
}

void _png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	io_buffer* pio;

	pio = (io_buffer*)(png_get_io_ptr(png_ptr));

	if (pio->pos + length > pio->bytes)
	{
		png_error(png_ptr, "Write Data Failed");
	}

	memcpy((void*)(pio->buf + pio->pos), (void*)data, length);
	pio->pos += length;
}

dword_t xpng_decompress(const byte_t* png_buf, dword_t png_len, byte_t* bmp_buf, dword_t bmp_size)
{
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	double dGamma = 0;

	io_buffer io = { 0 };

	png_color_16* pbkg = NULL;
	png_color rgb;
	png_uint_32 pixWidth, pixHeight;
	int i, j, k, bitDepth, clrType;
	png_byte bChanel;
	png_size_t rowBytes;
	png_bytepp prow = NULL;
	png_bytep pdata = NULL;

	int sClrBits, sPixBytes;
	int sMapCount;
	dword_t dImageSize, dFileSize, dImageOffset, dTotal = 0;
	bmp_file_t bf = { 0 };
	bmp_info_t bi = { 0 };

	if (!png_buf || !png_len)
		return 0;

	if (png_sig_cmp(png_buf, 0, 8))
	{
		return 0;
	}

	png_ptr = png_create_read_struct(png_get_libpng_ver(NULL), NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
	if (!png_ptr)
	{
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		if (prow)
			xmem_free(prow);

		if (pdata)
			xmem_free(pdata);

		return 0;
	}

	io.buf = (png_bytep)(png_buf + 8);
	io.bytes = png_len;
	io.pos = 0;

	png_set_read_fn(png_ptr, (png_voidp)&io, (png_rw_ptr)_png_read_data);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &pixWidth, &pixHeight, &bitDepth, &clrType, NULL, NULL, NULL);

#ifdef PNG_READ_16_TO_8_SUPPORTED
	if (bitDepth == 16)
#  ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
		png_set_scale_16(png_ptr);
#  else
		png_set_strip_16(png_ptr);
#  endif
#endif

	if (clrType == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);

	if (bitDepth < 8)
		png_set_expand(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);

	if (clrType == PNG_COLOR_TYPE_GRAY || clrType == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	if (png_get_bKGD(png_ptr, info_ptr, &pbkg))
	{
		pbkg->red = 255;
		pbkg->green = 255;
		pbkg->blue = 255;

		png_set_background(png_ptr, pbkg, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);

		rgb.red = (unsigned char)pbkg->red;
		rgb.green = (unsigned char)pbkg->green;
		rgb.blue = (unsigned char)pbkg->blue;
	}
	else
	{
		rgb.red = 255;
		rgb.green = 255;
		rgb.blue = 255;
	}

	if (png_get_gAMA(png_ptr, info_ptr, &dGamma))
		png_set_gamma(png_ptr, (double) 2.2, dGamma);

	png_read_update_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &pixWidth, &pixHeight, &bitDepth, &clrType, NULL, NULL, NULL);

	bChanel = png_get_channels(png_ptr, info_ptr);

	sClrBits = bChanel * 8;

	if (sClrBits <= 8)
		sMapCount = (1 << sClrBits);
	else
		sMapCount = 0;

	sPixBytes = ((pixWidth * sClrBits + 31) & ~31) / 8;

	dImageSize = sPixBytes * pixHeight;
	dImageOffset = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + sMapCount * BMP_RGBQUAD_SIZE;
	dFileSize = dImageOffset + dImageSize;

	if (dFileSize > (dword_t)bmp_size)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		return 0;
	}

	if (!bmp_buf)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		return dFileSize;
	}

	rowBytes = png_get_rowbytes(png_ptr, info_ptr);

	prow = (png_bytepp)xmem_alloc(pixHeight * sizeof(png_bytep));
	pdata = (png_bytep)xmem_alloc(pixHeight * rowBytes * sizeof(png_byte));

	for (i = 0; i < (int)(pixHeight); i++)
	{
		prow[i] = pdata + i * rowBytes;
	}

	png_read_image(png_ptr, prow);

	png_read_end(png_ptr, NULL);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	bf.flag = BMP_FLAG;
	bf.fsize = dFileSize;
	bf.offset = dImageOffset;

	bi.isize = BMP_INFOHEADER_SIZE;
	bi.width = pixWidth;
	bi.height = pixHeight;
	bi.planes = 1;
	bi.clrbits = sClrBits;
	bi.bytes = dImageSize;
	bi.clrused = sMapCount;

	dTotal = 0;

	dTotal += bmp_set_header(&bf, &bi, bmp_buf, bmp_size);

	dTotal += bmp_fill_quad(&bi, bmp_buf, bmp_size);

	dTotal += sPixBytes * pixHeight;
	for (i = 0; i < (int)(pixHeight); i++)
	{
		dTotal -= sPixBytes;

		for (j = 0; j <= (int)(rowBytes - bChanel); j += bChanel)
		{
			for (k = 0; k < 3; k++)
			{//b,g,r,
				bmp_buf[dTotal + j + k] = prow[i][j + 3 - k - 1];
			}
			if (bChanel > 3)
			{
				bmp_buf[dTotal + j + 3] = prow[i][j + 3];
			}
		}
	}
	dTotal += sPixBytes * pixHeight;

	xmem_free(prow);

	xmem_free(pdata);

	return dTotal;
}

dword_t xpng_compress(const byte_t* bmp_buf, dword_t bmp_len, byte_t* png_buf, dword_t png_size)
{
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	double dGamma = 0;

	io_buffer io = { 0 };

	png_uint_32 pixWidth, pixHeight;
	int i, j, k, bitDepth, clrType;
	png_byte bChanel;
	png_size_t rowBytes;
	png_bytepp prow = NULL;
	png_bytep pdata = NULL;

	int sClrBits, sPixBytes;
	int sMapCount;
	dword_t dImageSize, dFileSize, dImageOffset, dTotal = 0;
	bmp_file_t bf = { 0 };
	bmp_info_t bi = { 0 };

	if (bmp_len < BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE)
	{
		return 0;
	}

	bmp_get_header(&bf, &bi, bmp_buf, bmp_len);

	if (bf.flag != BMP_FLAG)
	{
		return 0;
	}

	dImageOffset = bf.offset;
	dFileSize = bf.fsize;

	if (bmp_len < dFileSize)
	{
		return 0;
	}

	sClrBits = bi.clrbits;
	dImageSize = bi.bytes;
	sMapCount = bi.clrused;

	pixWidth = bi.width;
	pixHeight = bi.height;
	bChanel = sClrBits / 8;

	if (bChanel == 4)
		clrType = PNG_COLOR_TYPE_RGB_ALPHA;
	else if (bChanel == 3 || bChanel == 2)
		clrType = PNG_COLOR_TYPE_RGB;
	else
		clrType = PNG_COLOR_TYPE_GRAY;

	bitDepth = 8;

	sPixBytes = ((pixWidth * sClrBits + 31) & ~31) / 8;
	rowBytes = pixWidth * sClrBits / 8;

	png_ptr = png_create_write_struct(png_get_libpng_ver(NULL), NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
	if (!png_ptr)
	{
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, NULL);
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);

		if (prow)
			xmem_free(prow);

		if (pdata)
			xmem_free(pdata);

		return 0;
	}

	io.buf = (png_bytep)(png_buf);
	io.bytes = png_size;
	io.pos = 0;

	png_set_write_fn(png_ptr, (png_voidp)&io, (png_rw_ptr)_png_write_data, NULL);

	png_set_IHDR(png_ptr, info_ptr, pixWidth, pixHeight, bitDepth, clrType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	png_set_bgr(png_ptr);

	prow = (png_bytepp)xmem_alloc(pixHeight * sizeof(png_bytep));
	pdata = (png_bytep)xmem_alloc(pixHeight * rowBytes * sizeof(png_byte));

	for (i = 0; i < (int)(pixHeight); i++)
	{
		prow[i] = pdata + i * rowBytes;
	}

	dTotal = dImageOffset;
	dTotal += sPixBytes * pixHeight;
	for (i = 0; i < (int)(pixHeight); i++)
	{
		dTotal -= sPixBytes;

		for (j = 0; j <= (int)(rowBytes - bChanel); j += bChanel)
		{
			for (k = 0; k < 3; k++)
			{//b,g,r
				prow[i][j + k] = bmp_buf[dTotal + j + 3 - k - 1];
			}
			if (bChanel > 3)
			{
				prow[i][j + 3] = bmp_buf[dTotal + j + 3];
			}
		}
	}

	png_write_image(png_ptr, prow);

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

	xmem_free(prow);

	xmem_free(pdata);

	return io.pos + sizeof(png_byte);
}

#endif