/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl bitmap utility document

	@module	bitmapbag.c | bitmap utility implement file

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
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_FILE) && defined(XDL_SUPPORT_CTX) && defined(XDL_SUPPORT_BMP)



/*********************************************************************************************/

/*
res_bmp_t load_bitmap_from_images(res_ctx_t rdc, link_t_ptr images, const tchar_t* iname)
{
	res_bmp_t bmp = NULL;
	link_t_ptr ilk;
	int len;
	byte_t* buf = NULL;
	byte_t* tmp = NULL;
	dword_t dw;
	tchar_t type[RES_LEN] = { 0 };

	const tchar_t *token, *sz_image = NULL;

	ilk = get_images_item(images, iname, -1);
	if (!ilk)
		return NULL;

	sz_image = get_images_item_src_ptr(ilk);

	if (compare_text(sz_image, xslen(GDI_ATTR_IMAGE_TYPE_JPG), GDI_ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
		xscpy(type, GDI_ATTR_IMAGE_TYPE_JPG);
		sz_image += xslen(GDI_ATTR_IMAGE_TYPE_JPG);
	}
	else if (compare_text(sz_image, xslen(GDI_ATTR_IMAGE_TYPE_PNG), GDI_ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
		xscpy(type, GDI_ATTR_IMAGE_TYPE_PNG);
		sz_image += xslen(GDI_ATTR_IMAGE_TYPE_PNG);
	}
	else if (compare_text(sz_image, xslen(GDI_ATTR_IMAGE_TYPE_BMP), GDI_ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		xscpy(type, GDI_ATTR_IMAGE_TYPE_PNG);
		sz_image += xslen(GDI_ATTR_IMAGE_TYPE_BMP);
	}
	else
	{
		token = xsstr(sz_image, _T("base64,"));
		if (!token)
			return 0;

		token += xslen(_T("base64,"));
		xsncpy(type, sz_image, token - sz_image);
		sz_image = token;
	}

	len = xbas_decode(sz_image, -1, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(len);
	xbas_decode(sz_image, -1, buf, len);

	if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
#ifdef XDL_SUPPORT_JPG
		dw = xjpg_decompress(buf, len, NULL, MAX_LONG);
		tmp = (byte_t*)xmem_alloc(dw);
		xjpg_decompress(buf, len, tmp, dw);

		bmp = load_bitmap_from_bytes(rdc, tmp, dw);

		xmem_free(buf);
		xmem_free(tmp);

		return bmp;
#else
		return 0;
#endif //XDL_SUPPORT_JPG
	}
	else if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
#ifdef XDL_SUPPORT_PNG
		dw = xpng_decompress(buf, len, NULL, MAX_LONG);
		tmp = (byte_t*)xmem_alloc(dw);
		xpng_decompress(buf, len, tmp, dw);

		bmp = load_bitmap_from_bytes(rdc, tmp, dw);

		xmem_free(buf);
		xmem_free(tmp);

		return bmp;
#else
		return 0;
#endif //XDL_SUPPORT_PNG
	}
	else if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		bmp = load_bitmap_from_bytes(rdc, buf, len);

		xmem_free(buf);

		return bmp;
	}
	else
	{
		xmem_free(buf);

		return NULL;
	}

	return NULL;
}

bool_t save_bitmap_to_images(res_ctx_t rdc, res_bmp_t bmp, link_t_ptr imagelist, const tchar_t* iname)
{
	byte_t* bmp_buf;
	dword_t bmp_len;

	byte_t* zip_buf;
	dword_t zip_len;

	tchar_t* base_buf;
	int base_len;

	tchar_t type[RES_LEN] = { 0 };
	int type_len;

	link_t_ptr nlk;

	bmp_len = save_bitmap_to_bytes(rdc, bmp, NULL, MAX_LONG);
	if (!bmp_len)
		return 0;

	bmp_buf = (byte_t*)xmem_alloc(bmp_len);
	save_bitmap_to_bytes(rdc, bmp, bmp_buf, bmp_len);

#ifdef XDL_SUPPORT_PNG
	zip_len = bmp_len;
	zip_buf = (byte_t*)xmem_alloc(zip_len);

	zip_len = xpng_compress(bmp_buf, bmp_len, zip_buf, zip_len);

	xmem_free(bmp_buf);

	bmp_buf = zip_buf;
	bmp_len = zip_len;

	xscpy(type, GDI_ATTR_IMAGE_TYPE_PNG);
#else
	xscpy(type, GDI_ATTR_IMAGE_TYPE_BMP);
#endif

	type_len = xslen(type);
	base_len = xbas_encode(bmp_buf, bmp_len, NULL, MAX_LONG);
	base_buf = xsalloc(type_len + base_len + 1);

	xsncpy(base_buf, type, type_len);
	xbas_encode(bmp_buf, bmp_len, base_buf + type_len, base_len);

	xmem_free(bmp_buf);

	nlk = get_images_item(imagelist, iname, -1);
	if (!nlk)
	{
		nlk = insert_images_item(imagelist, LINK_LAST);
		set_images_item_alt(nlk, iname);
	}

	attach_dom_node_attr(nlk, ATTR_SRC, base_buf);

	return 1;
}

res_bmp_t load_bitmap_from_image_list(res_ctx_t rdc, object_t il, const tchar_t* iname)
{
	res_bmp_t bmp = NULL;
	int len;
	byte_t* buf = NULL;
	byte_t* tmp = NULL;
	dword_t dw;
	tchar_t type[RES_LEN] = { 0 };

	const tchar_t *token, *sz_image = NULL;

	sz_image = get_image_ptr(il, iname);

	if (compare_text(sz_image, xslen(GDI_ATTR_IMAGE_TYPE_JPG), GDI_ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
		xscpy(type, GDI_ATTR_IMAGE_TYPE_JPG);
		sz_image += xslen(GDI_ATTR_IMAGE_TYPE_JPG);
	}
	else if (compare_text(sz_image, xslen(GDI_ATTR_IMAGE_TYPE_PNG), GDI_ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
		xscpy(type, GDI_ATTR_IMAGE_TYPE_PNG);
		sz_image += xslen(GDI_ATTR_IMAGE_TYPE_PNG);
	}
	else if (compare_text(sz_image, xslen(GDI_ATTR_IMAGE_TYPE_BMP), GDI_ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		xscpy(type, GDI_ATTR_IMAGE_TYPE_BMP);
		sz_image += xslen(GDI_ATTR_IMAGE_TYPE_BMP);
	}
	else
	{
		token = xsstr(sz_image, _T("base64,"));
		if (!token)
			return 0;

		token += xslen(_T("base64,"));
		xsncpy(type, sz_image, token - sz_image);
		sz_image = token;
	}

	len = xbas_decode(sz_image, -1, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(len);
	xbas_decode(sz_image, -1, buf, len);

	if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
#ifdef XDL_SUPPORT_JPG
		dw = xjpg_decompress(buf, len, NULL, MAX_LONG);
		tmp = (byte_t*)xmem_alloc(dw);
		xjpg_decompress(buf, len, tmp, dw);

		bmp = load_bitmap_from_bytes(rdc, tmp, dw);

		xmem_free(buf);
		xmem_free(tmp);

		return bmp;
#else
		return 0;
#endif //XDL_SUPPORT_JPG
	}
	else if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
#ifdef XDL_SUPPORT_PNG
		dw = xpng_decompress(buf, len, NULL, MAX_LONG);
		tmp = (byte_t*)xmem_alloc(dw);
		xpng_decompress(buf, len, tmp, dw);

		bmp = load_bitmap_from_bytes(rdc, tmp, dw);

		xmem_free(buf);
		xmem_free(tmp);

		return bmp;
#else
		return 0;
#endif //XDL_SUPPORT_PNG
	}
	else if (compare_text(type, -1, GDI_ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		bmp = load_bitmap_from_bytes(rdc, buf, len);

		xmem_free(buf);

		return bmp;
	}
	else
	{
		xmem_free(buf);

		return NULL;
	}

	return NULL;
}

bool_t save_bitmap_to_image_list(res_ctx_t rdc, res_bmp_t bmp, object_t il, const tchar_t* iname)
{
	byte_t* bmp_buf;
	dword_t bmp_len;

	byte_t* zip_buf;
	dword_t zip_len;

	tchar_t* base_buf;
	int base_len;

	tchar_t type[RES_LEN] = { 0 };
	int type_len;

	bmp_len = save_bitmap_to_bytes(rdc, bmp, NULL, MAX_LONG);
	if (!bmp_len)
		return 0;

	bmp_buf = (byte_t*)xmem_alloc(bmp_len);
	save_bitmap_to_bytes(rdc, bmp, bmp_buf, bmp_len);

#ifdef XDL_SUPPORT_PNG
	zip_len = bmp_len;
	zip_buf = (byte_t*)xmem_alloc(zip_len);

	zip_len = xpng_compress(bmp_buf, bmp_len, zip_buf, zip_len);

	xmem_free(bmp_buf);

	bmp_buf = zip_buf;
	bmp_len = zip_len;

	xscpy(type, GDI_ATTR_IMAGE_TYPE_PNG);
#else
	xscpy(type, GDI_ATTR_IMAGE_TYPE_BMP);
#endif

	type_len = xslen(type);
	base_len = xbas_encode(bmp_buf, bmp_len, NULL, MAX_LONG);
	base_buf = xsalloc(type_len + base_len + 1);

	xsncpy(base_buf, type, type_len);
	xbas_encode(bmp_buf, bmp_len, base_buf + type_len, base_len);

	xmem_free(bmp_buf);

	attach_image(il, iname, base_buf);

	return 1;
}*/

#endif /*XDL_SUPPORT_CTX*/
