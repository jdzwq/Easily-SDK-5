/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl image utility document

	@module	imagesbag.c | implement file

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

#include "imagesbag.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDK_SUPPORT_FILE) 


link_t_ptr insert_images_item_from_url(link_t_ptr ptr, const tchar_t* iname, const tchar_t* url)
{
	link_t_ptr nlk;

	if (is_null(iname))
		return NULL;

	nlk = get_images_item(ptr, iname, -1);
	if (!nlk)
	{
		nlk = insert_images_item(ptr, LINK_LAST);
		set_images_item_alt(nlk, iname);
	}

	set_images_item_src(nlk, url);

	return nlk;
}

link_t_ptr insert_images_item_from_file(link_t_ptr ptr, const tchar_t* iname, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr nlk;
	dword_t size;
	byte_t *file_buf;
	int len,tlen;
	tchar_t* xbas_buf = NULL;
	tchar_t type[RES_LEN + 1] = { 0 };

	if (is_null(iname))
		return NULL;

	size = load_image_bytes_from_file(psd, fname, NULL, NULL, MAX_LONG);
	if (!size)
		return NULL;

	file_buf = (byte_t*)xmem_alloc(size);

	size = load_image_bytes_from_file(psd, fname, type, file_buf, size);
	if (!size)
	{
		xmem_free(file_buf);
		return NULL;
	}

	len = size;
	size = xbas_encode(file_buf, len, NULL, MAX_LONG);
	tlen = xslen(type);

	xbas_buf = xsalloc(tlen + size + 1);
	xsncpy(xbas_buf, type, tlen);

	xbas_encode(file_buf, len, xbas_buf + tlen, size);

	xmem_free(file_buf);

	nlk = get_images_item(ptr, iname, -1);
	if (!nlk)
	{
		nlk = insert_images_item(ptr, LINK_LAST);
		set_images_item_alt(nlk, iname);
	}

	attach_dom_node_attr(nlk, ATTR_SRC, xbas_buf);

	return nlk;
}

dword_t load_image_bytes_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* itype, byte_t* buf, dword_t max)
{
	dword_t size;
	file_t xf;
	int len;

	tchar_t fsize[NUM_LEN + 1] = { 0 };

	len = xslen(fname);
	if (len < 4)
		return 0;

	if (compare_text(fname + xslen(fname) - 4, -1, _T(".jpg"), -1, 1) == 0)
	{
		if (itype)
			xscpy(itype, GDI_ATTR_IMAGE_TYPE_JPG);
	}
	else if (compare_text(fname + xslen(fname) - 4, -1, _T(".png"), -1, 1) == 0)
	{
		if (itype)
			xscpy(itype, GDI_ATTR_IMAGE_TYPE_PNG);
	}
	else if (compare_text(fname + xslen(fname) - 4, -1, _T(".bmp"), -1, 1) == 0)
	{
		if (itype)
			xscpy(itype, GDI_ATTR_IMAGE_TYPE_BMP);
	}
	else
		return 0;

	if (!xfile_info(psd, fname, NULL, fsize, NULL, NULL))
	{
		return 0;
	}

	if (is_huge_size(fsize))
	{
		return 0;
	}

	size = xstol(fsize);

	if (buf && max >= (int)size)
	{
		xf = xfile_open(psd, fname, FILE_OPEN_READ);
		if (!xf)
			return 0;

		xfile_read(xf, buf, size);
		xfile_close(xf);
	}

	return size;
}

bool_t save_image_bytes_to_file(const secu_desc_t* psd, const tchar_t* fname, const byte_t* buf, dword_t len)
{
	bool_t rt = 0;
	file_t xf = NULL;
	dword_t size;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
	{
		return 0;
	}

	size = len;
	rt = xfile_write(xf, buf, size);

	xfile_close(xf);

	return rt;
}

int load_image_base64_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* buf, int max)
{
	int tlen, bsize;
	dword_t fsize;
	byte_t *file_buf;
	tchar_t itype[RES_LEN + 1] = { 0 };

	fsize = load_image_bytes_from_file(psd, fname, NULL, NULL, MAX_LONG);
	if (!fsize)
		return 0;

	file_buf = (byte_t*)xmem_alloc(fsize);
	fsize = load_image_bytes_from_file(psd, fname, itype, file_buf, fsize);
	if (!fsize)
	{
		xmem_free(file_buf);
		return 0;
	}

	bsize = xbas_encode(file_buf, fsize, NULL, MAX_LONG);
	if (!bsize)
	{
		xmem_free(file_buf);
		return 0;
	}

	tlen = xslen(itype);

	if (buf)
	{
		xscpy(buf, itype);

		xbas_encode(file_buf, fsize, buf + tlen, bsize);
	}

	xmem_free(file_buf);

	return tlen + bsize;
}

bool_t save_image_base64_to_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* buf, int len)
{
	byte_t* file_buf;
	dword_t file_len;
	bool_t rt;

	file_len = xbas_decode(buf, len, NULL, MAX_LONG);
	if (!file_len)
		return 0;

	file_buf = (byte_t*)xmem_alloc(file_len);
	xbas_decode(buf, len, file_buf, file_len);

	rt = save_image_bytes_to_file(psd, fname, file_buf, file_len);

	xmem_free(file_buf);

	return rt;
}

dword_t load_bitmap_bytes_from_file(const secu_desc_t* psd, const tchar_t* fname, byte_t* buf, dword_t max)
{
	tchar_t type[RES_LEN] = { 0 };

	byte_t* file_buf = NULL;
	dword_t file_len;

	byte_t* bmp_buf = NULL;
	dword_t bmp_len;

	file_len = load_image_bytes_from_file(NULL, fname, type, NULL, MAX_LONG);
	if (!file_len)
		return 0;

	file_buf = (byte_t*)xmem_alloc(file_len);
	load_image_bytes_from_file(NULL, fname, type, file_buf, file_len);

	if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
#ifdef GPL_SUPPORT_JPG
		bmp_len = xjpg_decompress(file_buf, file_len, NULL, MAX_LONG);
		if (!bmp_len)
		{
			xmem_free(file_buf);
			return 0;
		}

		bmp_buf = (byte_t*)xmem_alloc(bmp_len);

		xjpg_decompress(file_buf, file_len, bmp_buf, bmp_len);

		xmem_free(file_buf);
#else
		xmem_free(file_buf);
		return 0;
#endif //GPL_SUPPORT_JPG
	}
	else if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
#ifdef GPL_SUPPORT_PNG
		bmp_len = xpng_decompress(file_buf, file_len, NULL, MAX_LONG);
		if (!bmp_len)
		{
			xmem_free(file_buf);
			return 0;
		}

		bmp_buf = (byte_t*)xmem_alloc(bmp_len);

		xpng_decompress(file_buf, file_len, bmp_buf, bmp_len);

		xmem_free(file_buf);
#else
		xmem_free(file_buf);
		return 0;
#endif //GPL_SUPPORT_PNG
	}
	else if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		bmp_buf = file_buf;
		bmp_len = file_len;
	}

	if (!bmp_buf)
		return 0;

	if (buf)
	{
		max = (max < bmp_len) ? max : bmp_len;
		xmem_copy((void*)buf, (void*)bmp_buf, max);
	}
	else
	{
		max = bmp_len;
	}

	xmem_free(bmp_buf);

	return max;
}

bool_t save_bitmap_bytes_to_file(const secu_desc_t* psd, const tchar_t* fname, const byte_t* bmp_buf, dword_t bmp_len)
{
	byte_t* file_buf = NULL;
	dword_t file_len = 0;

	bool_t rt;

	if (compare_text(fname + xslen(fname) - 4, -1, _T(".jpg"), -1, 1) == 0)
	{
#ifdef GPL_SUPPORT_JPG
		file_len = bmp_len;
		file_buf = (byte_t*)xmem_alloc(file_len);
		file_len = xjpg_compress(bmp_buf, bmp_len, file_buf, file_len);
#else
		return 0;
#endif
	}
	else if (compare_text(fname + xslen(fname) - 4, -1, _T(".png"), -1, 1) == 0)
	{
#ifdef GPL_SUPPORT_PNG
		file_len = bmp_len;
		file_buf = (byte_t*)xmem_alloc(file_len);
		file_len = xpng_compress(bmp_buf, bmp_len, file_buf, file_len);
#else
		return 0;
#endif
	}
	else if (compare_text(fname + xslen(fname) - 4, -1, _T(".bmp"), -1, 1) == 0)
	{
		file_buf = (byte_t*)bmp_buf;
		file_len = bmp_len;
	}

	if (!file_len)
	{
		xmem_free(file_buf);
		return 0;
	}

	rt = save_image_bytes_to_file(NULL, fname, file_buf, file_len);

	xmem_free(file_buf);

	return rt;
}


#endif /*XDL_SUPPORT_DOC*/