/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc rich text doc document

	@module	richdoc.h | richdoc interface file

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

#ifndef _RICHDOC_H
#define _RICHDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**************************************************************************************************************/
#define get_rich_name_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_rich_name(ptr,token)					set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_rich_id_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_rich_id(ptr,token)						set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)

#define get_rich_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define set_rich_title(ptr,token)					set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define set_rich_dynamic(nlk,b)						set_dom_node_attr_boolean(nlk,ATTR_DYNAMIC,b) 

#define get_rich_dynamic(nlk)						get_dom_node_attr_boolean(nlk,ATTR_DYNAMIC)

#define get_rich_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_rich_width(ptr,n)						set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_rich_height(ptr)						get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_rich_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define get_rich_node_name_ptr(nlk)					get_dom_node_attr_ptr(nlk,ATTR_NAME,-1)

#define set_rich_node_name(nlk,token)				set_dom_node_attr(nlk,ATTR_NAME,-1,token,-1)

#define get_rich_node_title_ptr(nlk)				get_dom_node_attr_ptr(nlk,ATTR_TITLE,-1)

#define set_rich_node_title(nlk,token)				set_dom_node_attr(nlk,ATTR_TITLE,-1,token,-1)

#define set_rich_node_text(nlk,token,len)			set_dom_node_text(nlk,token,len)

#define get_rich_node_text_ptr(nlk)					get_dom_node_text_ptr(nlk)

#define set_rich_node_fixed(nlk,b)					set_dom_node_attr_boolean(nlk,ATTR_FIXED,b) 

#define get_rich_node_fixed(nlk)					get_dom_node_attr_boolean(nlk,ATTR_FIXED)

#define set_rich_node_lined(nlk,b)					set_dom_node_attr_boolean(nlk,ATTR_LINED,b) 

#define get_rich_node_lined(nlk)					get_dom_node_attr_boolean(nlk,ATTR_LINED)

#define rich_node_text_set_chars(nlk,pos,pch,n)			dom_node_text_set_chars(nlk, pos, pch, n)

#define rich_node_text_ins_chars(nlk,pos,pch,n)			dom_node_text_ins_chars(nlk, pos, pch, n)

#define rich_node_text_del_chars(nlk,pos,n)				dom_node_text_del_chars(nlk, pos, n)

#define rich_node_title_set_chars(nlk,pos,pch,n)		dom_node_attr_set_chars(nlk, ATTR_TITLE, pos, pch, n)

#define rich_node_title_ins_chars(nlk,pos,pch,n)		dom_node_attr_ins_chars(nlk, ATTR_TITLE, pos, pch, n)

#define rich_node_title_del_chars(nlk,pos,n)			dom_node_attr_del_chars(nlk, ATTR_TITLE, pos, n)
/**************************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_rich_doc(void);

XDL_API void destroy_rich_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_rich_nodeset(link_t_ptr ptr);

XDL_API bool_t is_rich_doc(link_t_ptr ptr);

XDL_API bool_t is_rich_node(link_t_ptr ptr, link_t_ptr nlk);

XDL_API void clear_rich_doc(link_t_ptr ptr);

XDL_API void reset_rich_doc(link_t_ptr ptr);

XDL_API int get_rich_node_count(link_t_ptr ptr);

XDL_API link_t_ptr insert_rich_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API void delete_rich_node(link_t_ptr nlk);

XDL_API link_t_ptr get_rich_node(link_t_ptr ptr, const tchar_t* nname);

XDL_API link_t_ptr get_rich_next_node(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_rich_prev_node(link_t_ptr ptr,link_t_ptr pos);

XDL_API bool_t is_rich_text_reserve(tchar_t ch);

XDL_API int format_rich_doc(link_t_ptr ptr, tchar_t* buf, int max);

XDL_API bool_t parse_rich_doc(link_t_ptr ptr, const tchar_t* buf, int len);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif /*_RICHDOC_H*/
