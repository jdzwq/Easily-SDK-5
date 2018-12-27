/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tag text doc document

	@module	tagdoc.h | tagdoc interface file

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

#ifndef _TAGDOC_H
#define _TAGDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**************************************************************************************************************/
#define get_tag_node_name_ptr(nlk)				get_dom_node_name_ptr(nlk)

#define set_tag_node_name(nlk,token)			set_dom_node_name(nlk,token,-1)

#define set_tag_node_text(nlk,token,len)		set_dom_node_text(nlk,token,len)

#define get_tag_node_text_ptr(nlk)				get_dom_node_text_ptr(nlk)

#define is_tag_text_node(nlk)					((compare_text(get_tag_node_name_ptr(nlk),-1,DOC_TAG_TEXT,-1,0) == 0)? 1 : 0)

#define tag_node_text_set_chars(nlk,pos,pch,n)			dom_node_text_set_chars(nlk, pos, pch,n)

#define tag_node_text_ins_chars(nlk,pos,pch,n)			dom_node_text_ins_chars(nlk, pos, pch,n)

#define tag_node_text_del_chars(nlk,pos,n)				dom_node_text_del_chars(nlk, pos,n)
/**************************************************************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_tag_doc(void);

XDL_API void destroy_tag_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_tag_nodeset(link_t_ptr ptr);

XDL_API bool_t is_tag_doc(link_t_ptr ptr);

XDL_API bool_t is_tag_node(link_t_ptr ptr, link_t_ptr nlk);

XDL_API void clear_tag_doc(link_t_ptr ptr);

XDL_API int get_tag_node_count(link_t_ptr ptr);

XDL_API link_t_ptr insert_tag_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API void delete_tag_node(link_t_ptr nlk);

XDL_API link_t_ptr merge_tag_node(link_t_ptr nlk);

XDL_API link_t_ptr get_tag_node(link_t_ptr ptr, const tchar_t* nname);

XDL_API link_t_ptr get_tag_next_node(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_tag_prev_node(link_t_ptr ptr,link_t_ptr pos);

XDL_API void clear_tag_node_tag(link_t_ptr elk);

XDL_API bool_t is_tag_text_reserve(tchar_t ch);

XDL_API int format_tag_doc(link_t_ptr ptr, tchar_t* buf, int max);

XDL_API bool_t parse_tag_doc(link_t_ptr ptr, const tchar_t* buf, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_TAGDOC_H*/
