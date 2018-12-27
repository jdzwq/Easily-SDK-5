/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc annotation document

	@module	annodoc.h | annotation document interface file

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

#ifndef _ANNODOC_H
#define _ANNODOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/****************************************serial enable attributes******************************************/
#define get_anno_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_anno_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_anno_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_anno_id(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_anno_spot_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_anno_spot_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_anno_spot_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_anno_spot_id(ilk,token)						set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_anno_spot_type_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TYPE,-1)

#define set_anno_spot_type(ilk,token)					set_dom_node_attr(ilk,ATTR_TYPE,-1,token,-1)

#define set_anno_spot_style(ilk,token)					set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)

#define get_anno_spot_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)

#define set_anno_spot_text(ilk,token)					set_dom_node_text(ilk,token,-1)

#define get_anno_spot_text_ptr(ilk)						get_dom_node_text_ptr(ilk)

#define set_anno_spot_points(ilk,token)					set_dom_node_attr(ilk,ATTR_POINTS,-1,token,-1)

#define get_anno_spot_points_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_POINTS,-1)

/***********************************************runtime attributes*********************************************/
#define set_anno_spot_selected(ilk,b)					set_dom_node_mask_check(ilk,MSK_CHECKED,b)

#define get_anno_spot_selected(ilk)						get_dom_node_mask_check(ilk,MSK_CHECKED) 
/**********************************************runtime function*************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_anno_doc();

XDL_API void destroy_anno_doc(link_t_ptr ptr);

XDL_API void clear_anno_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_anno_spotset(link_t_ptr ptr);

XDL_API bool_t is_anno_doc(link_t_ptr ptr);

XDL_API bool_t is_anno_spot(link_t_ptr ptr, link_t_ptr plk);

XDL_API link_t_ptr anno_doc_from_spot(link_t_ptr ilk);

XDL_API link_t_ptr insert_anno_spot(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_anno_next_spot(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_anno_prev_spot(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_anno_spot_at(link_t_ptr ptr,int index);

XDL_API void delete_anno_spot(link_t_ptr ilk);

XDL_API int get_anno_spot_count(link_t_ptr ptr);

XDL_API int get_anno_spot_selected_count(link_t_ptr ptr);

XDL_API int get_anno_spot_xpoint(link_t_ptr ilk, xpoint_t* ppt, int max);

XDL_API void set_anno_spot_xpoint(link_t_ptr ilk, const xpoint_t* ppt, int n);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*ANNODOC_H*/