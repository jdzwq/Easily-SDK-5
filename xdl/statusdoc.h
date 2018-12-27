/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc status document

	@module	statusdoc.h | status document interface file

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

#ifndef _STATUSDOC_H
#define _STATUSDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/*******************************************serial enable attributes********************************************/
#define get_status_name_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_status_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define set_status_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define get_status_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define set_status_style(ptr,lay)						set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)

#define get_status_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_status_icon_span(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)

#define get_status_icon_span(ptr)						get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)

#define get_status_alignment_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_ALIGNMENT,-1)

#define set_status_alignment(ptr,token)					set_dom_node_attr(ptr,ATTR_ALIGNMENT,-1,token,-1)

#define set_status_item_delta(ilk,ul)					set_dom_node_delta(ilk,(var_long)ul)

#define get_status_item_delta(ilk)						get_dom_node_delta(ilk)

#define set_status_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_status_item_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_status_item_title(ilk,token)				set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

#define get_status_item_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)

#define set_status_item_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)

#define get_status_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

#define get_status_item_image_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_IMAGE,-1)

#define set_status_item_image(ilk,token)				set_dom_node_attr(ilk,ATTR_IMAGE,-1,token,-1)

#define get_status_item_image_position_ptr(ilk)			get_dom_node_attr_ptr(ilk,ATTR_IMAGE_POSITION,-1)

#define set_status_item_image_position(ilk,token)		set_dom_node_attr(ilk,ATTR_IMAGE_POSITION,-1,token,-1)

#define set_status_item_width(ilk,n)					set_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH,n)

#define get_status_item_width(ilk)						get_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH)

/***************************************************runtime attributes************************************/
#define set_status_images(ptr,images)					set_dom_node_images(ptr,images)

#define get_status_images(ptr)							get_dom_node_images(ptr)
/*********************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_status_doc(void);

XDL_API void destroy_status_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_status_itemset(link_t_ptr ptr);

XDL_API bool_t is_status_doc(link_t_ptr ptr);

XDL_API bool_t is_status_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void clear_status_doc(link_t_ptr ptr);

XDL_API link_t_ptr insert_status_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_status_next_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_status_prev_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_status_item(link_t_ptr ptr, const tchar_t* szName);

XDL_API int get_status_item_count(link_t_ptr ptr);

XDL_API void delete_status_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*STATUSDOC_H*/