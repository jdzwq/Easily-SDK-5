/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl imagesdoc document

	@module	imagesdoc.h | xdl images interface file

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

#ifndef _IMAGESDOC_H
#define _IMAGESDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/***************************************************************************************************************/
#define set_images_layer(ptr,lay)					set_dom_node_attr(ptr,ATTR_LAYER,-1,lay,-1)

#define get_images_layer_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_LAYER,-1)

#define set_images_item_width(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH,n)

#define get_images_item_width(ptr)						get_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH)

#define get_images_item_height(ptr)						get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)

#define set_images_item_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)

#define set_images_icon_span(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)

#define get_images_icon_span(ptr)						get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)

#define set_images_item_src(ilk,token)					set_dom_node_attr(ilk,ATTR_SRC,-1,token,-1)

#define get_images_item_src_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_SRC,-1)

#define set_images_item_alt(ilk,token)					set_dom_node_attr(ilk,ATTR_ALT,-1,token,-1)

#define get_images_item_alt_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ALT,-1)

#define set_images_item_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)

#define get_images_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

/**************************************************************************************************************/
#define set_images_item_checked(ilk,b)					set_dom_node_mask_check(ilk,MSK_CHECKED,b)

#define get_images_item_checked(ilk)						get_dom_node_mask_check(ilk,MSK_CHECKED)

/**************************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_images_doc(void);

XDL_API void destroy_images_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_images_itemset(link_t_ptr ptr);

XDL_API void clear_images_doc(link_t_ptr ptr);

XDL_API bool_t is_images_doc(link_t_ptr ptr);

XDL_API bool_t is_images_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void sort_images_doc(link_t_ptr ptr,int desc);

XDL_API int get_images_item_count(link_t_ptr ptr);

XDL_API link_t_ptr insert_images_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API void delete_images_item(link_t_ptr ilk);

XDL_API link_t_ptr get_images_next_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_images_prev_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_images_item(link_t_ptr ptr,const tchar_t* key,int keylen);

XDL_API void cache_images_doc(link_t_ptr ptr);

XDL_API void uncache_images_doc(link_t_ptr ptr);

XDL_API bool_t get_ximage(link_t_ptr ptr, const tchar_t* alt, ximage_t* pxi);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif /*_IMAGELIST_H*/
