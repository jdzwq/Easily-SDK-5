/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc jpg document

	@module	oemjpg.c | implement file

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

#include "oemjpg.h"
#include "impmem.h"
#include "imperr.h"

#ifdef GPL_SUPPORT_JPG

#include <setjmp.h>
#include "../jpg/jpeglib.h"
#include "../bmp/bmplib.h"

#ifdef _OS_WINDOWS
#pragma comment(lib,"jpg.lib")
#pragma comment(lib,"bmp.lib")
#endif


struct xjpeg_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf jmp_buffer;
};

typedef struct xjpeg_error_mgr *xjpeg_error_mgr_ptr;

METHODDEF(void)
_jpeg_error_exit(j_common_ptr cinfo)
{
	xjpeg_error_mgr_ptr xjpeg_err = (xjpeg_error_mgr_ptr)cinfo->err;

	(*cinfo->err->output_message) (cinfo);

	longjmp(xjpeg_err->jmp_buffer, 1);
}

dword_t xjpg_decompress(const byte_t* jpg_buf, dword_t jpg_len, byte_t* bmp_buf, dword_t bmp_size)
{
	struct jpeg_decompress_struct cinfo;
	//struct jpeg_error_mgr jerr; //replaced by xjpeg_err_mgr
	struct xjpeg_error_mgr jerr = { 0 };
	JSAMPARRAY js_buffer;
	int i, j, bytes = 0;
	int row_count, col_bytes, alt_bytes;
	unsigned char *out_buffer, *out_pos;

	sword_t sClrBits;
	int iMapCount;
	dword_t dImageSize, dFileSize, dImageOffset, dTotal = 0;
	dword_t pixWidth, pixHeight;
	bmp_file_t bf = { 0 };
	bmp_info_t bi = { 0 };

	if (!jpg_buf || !jpg_len)
		return 0;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = _jpeg_error_exit;

	if (setjmp(jerr.jmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, (unsigned char*)jpg_buf, jpg_len);

	if (!jpeg_read_header(&cinfo, TRUE))
	{
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	if (!jpeg_start_decompress(&cinfo))
	{
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	sClrBits = cinfo.output_components * 8;

	if (sClrBits <= 8)
		iMapCount = (1 << sClrBits);
	else
		iMapCount = 0;

	pixWidth = cinfo.output_width;
	pixHeight = cinfo.output_height;
	dImageSize = ((pixWidth * sClrBits + 31) & ~31) / 8 * pixHeight;
	dImageOffset = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + iMapCount * BMP_RGBQUAD_SIZE;
	dFileSize = dImageOffset + dImageSize;

	if (dFileSize > (dword_t)bmp_size)
	{
		jpeg_destroy_decompress(&cinfo);

		return 0;
	}

	if (!bmp_buf)
	{
		jpeg_destroy_decompress(&cinfo);

		return dFileSize;
	}

	bf.flag = BMP_FLAG;
	bf.fsize = dFileSize;
	bf.offset = dImageOffset;

	bi.isize = BMP_INFOHEADER_SIZE;
	bi.width = pixWidth;
	bi.height = pixHeight;
	bi.planes = 1;
	bi.clrbits = sClrBits;
	bi.bytes = dImageSize;
	bi.xpelsperm = (dword_t)(cinfo.X_density * 100);
	bi.ypelsperm = (dword_t)(cinfo.Y_density * 100);
	bi.clrused = iMapCount;

	dTotal = 0;

	dTotal += bmp_set_header(&bf, &bi, bmp_buf, bmp_size);

	dTotal += bmp_fill_quad(&bi, bmp_buf, bmp_size);

	row_count = cinfo.output_height;
	col_bytes = cinfo.output_width * cinfo.output_components;

	out_buffer = (unsigned char*)xmem_alloc(col_bytes * row_count);

	js_buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, col_bytes, 1);

	out_pos = out_buffer;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, js_buffer, 1);

		memcpy((void*)out_pos, *js_buffer, col_bytes);
		out_pos += col_bytes;
	}

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	alt_bytes = ((cinfo.output_width * sClrBits + 31) & ~31) / 8;

	out_pos = out_buffer + col_bytes * (row_count - 1);
	for (i = 0; i < row_count; i++)
	{
		if (sClrBits >= 24)
		{
			for (j = 0; j <= col_bytes - 3; j += 3)
			{
				bmp_buf[dTotal + j + 0] = out_pos[j + 2];
				bmp_buf[dTotal + j + 1] = out_pos[j + 1];
				bmp_buf[dTotal + j + 2] = out_pos[j + 0];
			}
		}
		else
		{
			for (j = 0; j <= col_bytes - 1; j++)
			{
				bmp_buf[dTotal + j] = out_pos[j];
			}
		}
		out_pos -= col_bytes;
		dTotal += alt_bytes;
	}

	xmem_free(out_buffer);

	return dTotal;
}

dword_t xjpg_compress(const byte_t* bmp_buf, dword_t bmp_len, byte_t* jpg_buf, dword_t jpg_size)
{
	struct jpeg_compress_struct cinfo;
	//struct jpeg_error_mgr jerr; //replaced by xjpeg_err_mgr
	struct xjpeg_error_mgr jerr;
	JSAMPARRAY js_buffer;
	int col_bytes, alt_bytes;
	unsigned char *in_data, *in_pos;
	size_t rt_size;

	sword_t sClrBits;
	int iMapCount;
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
	iMapCount = bi.clrused;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = _jpeg_error_exit;
	if (setjmp(jerr.jmp_buffer))
	{
		jpeg_destroy_compress(&cinfo);
		return 0;
	}

	jpeg_create_compress(&cinfo);

	rt_size = jpg_size;
	jpeg_stdio_dest(&cinfo, jpg_buf, &rt_size);

	cinfo.image_width = bi.width;
	cinfo.image_height = bi.height;
	cinfo.input_components = sClrBits / 8;
	if (cinfo.input_components > 3)
		cinfo.input_components = 3;
	if (cinfo.input_components == 1)
		cinfo.in_color_space = JCS_GRAYSCALE;
	else
		cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 60, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	col_bytes = cinfo.image_width * cinfo.input_components;
	alt_bytes = ((cinfo.image_width * sClrBits + 31) & ~31) / 8;

	js_buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, col_bytes, 1);

	in_data = (unsigned char*)(bmp_buf + dImageOffset);
	in_pos = (unsigned char*)(in_data + alt_bytes * (cinfo.image_height - 1));
	while (cinfo.next_scanline < cinfo.image_height) {
		memcpy((void*)(*js_buffer), (void*)in_pos, col_bytes);
		jpeg_write_scanlines(&cinfo, js_buffer, 1);

		in_pos -= alt_bytes;
	}

	jpeg_finish_compress(&cinfo);

	jpeg_destroy_compress(&cinfo);

	return rt_size;
}

#endif