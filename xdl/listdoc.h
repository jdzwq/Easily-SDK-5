/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc list document

	@module	listdoc.h | list document interface file

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

#ifndef _LISTDOC_H
#define _LISTDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/****************************************serial enable attributes******************************************/
#define get_list_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_list_style(ptr,lay)								set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)

#define get_list_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_list_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define set_list_layer(ptr,lay)								set_dom_node_attr(ptr,ATTR_LAYER,-1,lay,-1)

#define get_list_layer_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_LAYER,-1)

#define set_list_icon_span(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)

#define get_list_icon_span(ptr)								get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)

#define set_list_item_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH,n)

#define get_list_item_width(ptr)							get_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH)

#define get_list_item_height(ptr)							get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)

#define set_list_item_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)

#define get_list_showcheck(ptr)								get_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK)

#define set_list_showcheck(ptr,b)							set_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK,b)

#define set_list_item_id(ilk,token)							set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)

#define get_list_item_id_ptr(ilk)							get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

#define get_list_item_name_ptr(ilk)							get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_list_item_name(ilk,token)						set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_list_item_title_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)

#define set_list_item_title(ilk,token)						set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

#define get_list_item_image_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_IMAGE,-1)

#define set_list_item_image(ilk,token)						set_dom_node_attr(ilk,ATTR_IMAGE,-1,token,-1)
/***********************************************file attributes*************************************************/
#define get_list_item_file_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_list_item_file_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_list_item_file_createtime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_CREATETIME,-1)

#define set_list_item_file_createtime(ilk,token)			set_dom_node_attr(ilk,ATTR_CREATETIME,-1,token,-1)

#define get_list_item_file_writetime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_WRITETIME,-1)

#define set_list_item_file_writetime(ilk,token)				set_dom_node_attr(ilk,ATTR_WRITETIME,-1,token,-1)

#define get_list_item_file_accesstime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_ACCESSTIME,-1)

#define set_list_item_file_accesstime(ilk,token)			set_dom_node_attr(ilk,ATTR_ACCESSTIME,-1,token,-1)

#define get_list_item_file_etag_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ETAG,-1)

#define set_list_item_file_etag(ilk,token)					set_dom_node_attr(ilk,ATTR_ETAG,-1,token,-1)

#define get_list_item_file_size_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_SIZE,-1)

#define set_list_item_file_size(ilk,token)					set_dom_node_attr(ilk,ATTR_SIZE,-1,token,-1)

#define get_list_item_file_archive_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ARCHIVE,-1)

#define set_list_item_file_archive(ilk,token)				set_dom_node_attr(ilk,ATTR_ARCHIVE,-1,token,-1)

/***********************************************runtime attributes*********************************************/
#define set_list_images(ptr,images)							set_dom_node_images(ptr,images)

#define get_list_images(ptr)								get_dom_node_images(ptr)

#define set_list_item_locked(ilk,c)							set_dom_node_mask_check(ilk,MSK_LOCKED,c) 

#define get_list_item_locked(ilk)							get_dom_node_mask_check(ilk,MSK_LOCKED)

#define set_list_item_checked(ilk,b)						set_dom_node_mask_check(ilk,MSK_CHECKED,b)

#define get_list_item_checked(ilk)							get_dom_node_mask_check(ilk,MSK_CHECKED)

#define set_list_child_item_checked(ilk,b)					set_dom_child_node_mask_check(ilk,MSK_CHECKED,b)

#define get_list_item_checked_count(ilk)					get_dom_child_node_mask_check_count(ilk,MSK_CHECKED)

/**********************************************runtime function*****************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_list_doc(void);

XDL_API void destroy_list_doc(link_t_ptr ptr);

XDL_API void clear_list_doc(link_t_ptr ptr);

XDL_API int get_list_item_count(link_t_ptr ptr);

XDL_API bool_t is_list_doc(link_t_ptr ilk);

XDL_API bool_t is_list_item(link_t_ptr ptr, link_t_ptr plk);

XDL_API link_t_ptr list_doc_from_item(link_t_ptr ilk);

XDL_API link_t_ptr insert_list_item(link_t_ptr ilk,link_t_ptr pos);

XDL_API link_t_ptr get_list_first_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_list_last_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_list_item_at(link_t_ptr ilk,int index);

XDL_API link_t_ptr get_list_parent_item(link_t_ptr ilk);

XDL_API link_t_ptr get_list_next_sibling_item(link_t_ptr ilk);

XDL_API link_t_ptr get_list_prev_sibling_item(link_t_ptr ilk);

XDL_API void delete_list_item(link_t_ptr ilk);

XDL_API link_t_ptr insert_list_file_item(link_t_ptr ilk, link_t_ptr pos);

XDL_API link_t_ptr insert_list_directory_item(link_t_ptr ilk, link_t_ptr pos);

XDL_API bool_t is_list_file_item(link_t_ptr ilk);

XDL_API bool_t is_list_directory_item(link_t_ptr ilk);

XDL_API void set_list_item_file_info(link_t_ptr ilk, const file_info_t* pfi);

XDL_API void get_list_item_file_info(link_t_ptr ilk, file_info_t* pfi);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*LISTDOC_H*/