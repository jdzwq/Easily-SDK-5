/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc label document

	@module	labeldoc.h | label document interface file

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

#ifndef _LABELDOC_H
#define _LABELDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/****************************************serial enable attributes******************************************/
#define get_label_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_label_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define set_label_style(ptr,lay)							set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)

#define get_label_style_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define get_label_width(ptr)								get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_label_width(ptr,n)								set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_label_height(ptr)								get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_label_height(ptr,n)								set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define get_label_item_width(ptr)							get_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH)

#define set_label_item_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH,n)

#define get_label_item_height(ptr)							get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)

#define set_label_item_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)

#define get_label_item_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_label_item_name(ilk,token)						set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_label_item_type_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TEXT_TYPE,-1)

#define set_label_item_type(ilk,token)						set_dom_node_attr(ilk,ATTR_TEXT_TYPE,-1,token,-1)

#define get_label_item_image_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_IMAGE,-1)

#define set_label_item_image(ilk,token)						set_dom_node_attr(ilk,ATTR_IMAGE,-1,token,-1)

#define get_label_item_title_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)

#define set_label_item_title(ilk,token)						set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

#define get_label_item_text_ptr(ilk)						get_dom_node_text_ptr(ilk)

#define set_label_item_text(ilk,token,len)					set_dom_node_text(ilk,token,len)

/***********************************************runtime attributes*********************************************/
#define set_label_images(ptr,images)						set_dom_node_images(ptr,images)

#define get_label_images(ptr)								get_dom_node_images(ptr)
/**********************************************runtime function*****************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_label_doc(void);

XDL_API void destroy_label_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_label_itemset(link_t_ptr ptr);

XDL_API void clear_label_doc(link_t_ptr ptr);

XDL_API bool_t is_label_doc(link_t_ptr ptr);

XDL_API bool_t is_label_item(link_t_ptr ptr, link_t_ptr plk);

XDL_API link_t_ptr label_doc_from_item(link_t_ptr ilk);

XDL_API link_t_ptr insert_label_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_label_next_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_label_prev_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_label_item_at(link_t_ptr ptr,int index);

XDL_API void delete_label_item(link_t_ptr ilk);

XDL_API int get_label_item_count(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*LABELDOC_H*/