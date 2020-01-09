/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl filetree document

	@module	filetree.h | xdl file tree interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#ifndef _FILETREE_H
#define _FILETREE_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/*******************************************************************************************************************/
#define get_file_item_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_file_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_file_item_createtime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_CREATETIME,-1)

#define set_file_item_createtime(ilk,token)				set_dom_node_attr(ilk,ATTR_CREATETIME,-1,token,-1)

#define get_file_item_writetime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_WRITETIME,-1)

#define set_file_item_writetime(ilk,token)				set_dom_node_attr(ilk,ATTR_WRITETIME,-1,token,-1)

#define get_file_item_accesstime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_ACCESSTIME,-1)

#define set_file_item_accesstime(ilk,token)				set_dom_node_attr(ilk,ATTR_ACCESSTIME,-1,token,-1)

#define get_file_item_etag_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ETAG,-1)

#define set_file_item_etag(ilk,token)					set_dom_node_attr(ilk,ATTR_ETAG,-1,token,-1)

#define get_file_item_size_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_SIZE,-1)

#define set_file_item_size(ilk,token)					set_dom_node_attr(ilk,ATTR_SIZE,-1,token,-1)

#define get_file_item_archive_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ARCHIVE,-1)

#define set_file_item_archive(ilk,token)				set_dom_node_attr(ilk,ATTR_ARCHIVE,-1,token,-1)

/*********************************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_file_tree(void);

XDL_API void destroy_file_tree(link_t_ptr ptr);

XDL_API void clear_file_tree(link_t_ptr ptr);

XDL_API bool_t is_file_tree(link_t_ptr ptr);

XDL_API bool_t is_file_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API bool_t is_directory_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void sort_file_tree(link_t_ptr ilk,const tchar_t* attr, int desc);

XDL_API int get_file_item_count(link_t_ptr ilk);

XDL_API link_t_ptr get_file_item(link_t_ptr ilk,const tchar_t* key,int klen);

XDL_API link_t_ptr insert_file_item(link_t_ptr ilk, link_t_ptr pos);

XDL_API link_t_ptr insert_directory_item(link_t_ptr ilk, link_t_ptr pos);

XDL_API void delete_file_item(link_t_ptr ilk);

XDL_API link_t_ptr get_file_next_item(link_t_ptr ilk, link_t_ptr pos);

XDL_API link_t_ptr get_file_prev_item(link_t_ptr ilk, link_t_ptr pos);

XDL_API void set_file_item_info(link_t_ptr ilk, const file_info_t* pfi);

XDL_API void get_file_item_info(link_t_ptr ilk, file_info_t* pfi);

#ifdef	__cplusplus
}
#endif

#endif /*#XDL_SUPPORT_DOC*/

#endif /*_FILETREE_H*/
