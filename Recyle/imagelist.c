/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl image list document

	@module	imagelist.c | xdl image list implement file

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

#include "imagelist.h"
#include "xdlstd.h"

typedef struct _image_list_t{
	link_t lk;

	link_t_ptr image;
}image_list_t;

#define ImageListFromLink(p) TypePtrFromLink(image_list_t,p)

link_t_ptr create_image_list()
{
	image_list_t* pil;

	pil = (image_list_t*)xmem_alloc(sizeof(image_list_t));
	pil->lk.tag = lkImageList;

	pil->image = create_hash_table();

	return &pil->lk;
}

void destroy_image_list(link_t_ptr il)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	destroy_hash_table(pil->image);

	xmem_free(pil);
}

void clear_image_list(link_t_ptr il)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	clear_hash_table(pil->image);
}

link_t_ptr get_image_hash_table(link_t_ptr il)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	return pil->image;
}

int get_image_list_count(link_t_ptr il)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	return get_hash_entity_count(pil->image);
}

int get_image(link_t_ptr il, const tchar_t* alt, tchar_t* src, int max)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	return read_hash_attr(pil->image, alt, -1, src, max);
}

const tchar_t* get_image_ptr(link_t_ptr il, const tchar_t* alt)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	return get_hash_attr_ptr(pil->image, alt, -1);
}

int get_image_len(link_t_ptr il, const tchar_t* alt)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	return get_hash_attr_len(pil->image, alt, -1);
}

void insert_image(link_t_ptr il, const tchar_t* alt, const tchar_t* src, int len)
{
	image_list_t* pil = ImageListFromLink(il);

	XDL_ASSERT(il && il->tag == lkImageList);

	write_hash_attr(pil->image, alt, -1, src, len);
}

void delete_image(link_t_ptr il, const tchar_t* alt)
{
	image_list_t* pil = ImageListFromLink(il);
	link_t_ptr ent;

	XDL_ASSERT(il && il->tag == lkImageList);

	ent = get_hash_entity(pil->image, alt, -1);
	if (ent)
	{
		delete_hash_entity(ent);
	}
}

void attach_image(link_t_ptr il, const tchar_t* alt, tchar_t* src)
{
	image_list_t* pil = ImageListFromLink(il);
	link_t_ptr ent;

	XDL_ASSERT(il && il->tag == lkImageList);

	ent = get_hash_entity(pil->image, alt, -1);
	if (!ent)
	{
		ent = write_hash_attr(pil->image, alt, -1, NULL, 0);
	}

	attach_hash_entity_val(ent, src);
}

tchar_t* detach_image(link_t_ptr il, const tchar_t* alt)
{
	image_list_t* pil = ImageListFromLink(il);
	link_t_ptr ent;

	XDL_ASSERT(il && il->tag == lkImageList);

	ent = get_hash_entity(pil->image, alt, -1);

	return (ent) ? detach_hash_entity_val(ent) : NULL;
}

bool_t get_image_at(link_t_ptr il, int index, const tchar_t** palt, const tchar_t** psrc)
{
	image_list_t* pil = ImageListFromLink(il);
	link_t_ptr ent;

	XDL_ASSERT(il && il->tag == lkImageList);

	ent = get_hash_entity_at(pil->image, index);
	if (!ent)
		return 0;

	if (palt)
		*palt = get_hash_entity_key_ptr(ent);
	if (psrc)
		*psrc = get_hash_entity_val_ptr(ent);
	
	return 1;
}

bool_t get_ximage(link_t_ptr il, const tchar_t* alt, ximage_t* pxi)
{
	image_list_t* pil = ImageListFromLink(il);
	link_t_ptr ent;
	const tchar_t* src;
	const tchar_t* token;

	XDL_ASSERT(il && il->tag == lkImageList);

	ent = get_hash_entity(pil->image, alt, -1);
	if (!ent)
	{
		xscpy(pxi->type, _T(""));
		pxi->source = NULL;
		return 0;
	}

	src = get_hash_entity_val_ptr(ent);

	if (compare_text(src, xslen(GDI_ATTR_IMAGE_TYPE_JPG), GDI_ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_JPG);
		src += xslen(GDI_ATTR_IMAGE_TYPE_JPG);
	}
	else if (compare_text(src, xslen(GDI_ATTR_IMAGE_TYPE_PNG), GDI_ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_PNG);
		src += xslen(GDI_ATTR_IMAGE_TYPE_PNG);
	}
	else if (compare_text(src, xslen(GDI_ATTR_IMAGE_TYPE_BMP), GDI_ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_BMP);
		src += xslen(GDI_ATTR_IMAGE_TYPE_BMP);
	}
	else
	{
		token = xsstr(src, _T("base64,"));
		token += xslen(_T("base64,"));
		xsncpy(pxi->type, src, token - src);
		src = token;
	}

	pxi->source = src;

	return 1;
}

