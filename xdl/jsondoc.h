/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc json document set

	@module	jsondoc.h | document set interface file

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

#ifndef _JSONDOC_H
#define _JSONDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**********************************************************************************************************/
#define get_json_item_name_ptr(ilk)						get_dom_node_name_ptr(ilk)

#define set_json_item_name(ilk,token)					set_dom_node_name(ilk,token,-1)

#define get_json_item_value_ptr(ilk)					get_dom_node_text_ptr(ilk)

#define get_json_item_value(ilk,token,max)				set_dom_node_text(ilk,token,max)

#define set_json_item_value(ilk,token)					set_dom_node_text(ilk,token,-1)

/***********************************************************************************************************/
#define set_json_item_array(ilk,b)						set_dom_node_mask_check(ilk,MSK_ARRAY,b)

#define get_json_item_array(ilk)						get_dom_node_mask_check(ilk,MSK_ARRAY)
/***********************************************************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_json_doc(void);

	XDL_API void destroy_json_doc(link_t_ptr ptr);

	XDL_API void clear_json_doc(link_t_ptr ptr);

	XDL_API bool_t is_json_doc(link_t_ptr ptr);

	XDL_API bool_t is_json_item(link_t_ptr ptr, link_t_ptr ilk);

	XDL_API link_t_ptr get_json_item(link_t_ptr ptr, const tchar_t* iname);

	XDL_API link_t_ptr insert_json_item(link_t_ptr ptr, link_t_ptr pos);

	XDL_API void delete_json_item(link_t_ptr ilk);

	XDL_API int get_json_item_count(link_t_ptr ilk);

	XDL_API link_t_ptr get_json_first_child_item(link_t_ptr ilk);

	XDL_API link_t_ptr get_json_last_child_item(link_t_ptr ilk);

	XDL_API link_t_ptr get_json_parent_item(link_t_ptr ilk);

	XDL_API link_t_ptr get_json_next_sibling_item(link_t_ptr ilk);

	XDL_API link_t_ptr get_json_prev_sibling_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif	/*_JSONDOC_H*/