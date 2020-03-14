/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl images document

	@module	imagesdoc.c | implement file

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

#include "imagesdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC


link_t_ptr create_images_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_IMAGES, -1);

	set_images_item_width(ptr, 18);
	set_images_item_height(ptr, 24);
	set_images_icon_span(ptr, DEF_GDI_ATTR_GIZMO_SPAN);

	return ptr;
}

void destroy_images_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_images_itemset(link_t_ptr ptr)
{
	return ptr;
}

bool_t is_images_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_IMAGES, -1, 0) == 0) ? 1 : 0;
}

bool_t is_images_item(link_t_ptr ptr, link_t_ptr ilk)
{
	if (!is_dom_child_node(ptr, ilk))
		return 0;

	return (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_IMAGES_IMAGE, -1, 0) == 0) ? 1 : 0;
}

void clear_images_doc(link_t_ptr ptr)
{
	ptr = get_images_itemset(ptr);

	delete_dom_child_nodes(ptr);
}

void sort_images_doc(link_t_ptr ptr,bool_t desc)
{
	ptr = get_images_itemset(ptr);

	sort_dom_child_node_by_name(ptr, desc);
}

int get_images_item_count(link_t_ptr ptr)
{
	ptr = get_images_itemset(ptr);

	return get_dom_child_node_count(ptr);
}

link_t_ptr	get_images_item(link_t_ptr ptr, const tchar_t* key, int keylen)
{
	ptr = get_images_itemset(ptr);

	return find_dom_node_by_attr(ptr, 0, ATTR_ALT, -1, key, keylen);
}

link_t_ptr insert_images_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr nlk;

	ptr = get_images_itemset(ptr);

	nlk = insert_dom_node(ptr, pos);
	set_dom_node_name(nlk, DOC_IMAGES_IMAGE, -1);
	
	return nlk;
}

void delete_images_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr get_images_next_item(link_t_ptr ptr, link_t_ptr pos)
{
	ptr = get_images_itemset(ptr);

	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_images_prev_item(link_t_ptr ptr, link_t_ptr pos)
{
	ptr = get_images_itemset(ptr);

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

bool_t get_ximage(link_t_ptr ptr, const tchar_t* alt, ximage_t* pxi)
{
	link_t_ptr plk;
	const tchar_t* src;
	const tchar_t* token;

	plk = get_images_item(ptr, alt, -1);
	if (!plk)
	{
		xscpy(pxi->type, _T(""));
		pxi->source = NULL;
		return 0;
	}

	src = get_images_item_src_ptr(plk);

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


#endif //XDLDOC