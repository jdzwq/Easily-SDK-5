/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memo document

	@module	memodoc.h | memo document interface file

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

#ifndef _MEMODOC_H
#define _MEMODOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#define get_memo_width(ptr)						get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_memo_width(ptr,n)					set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_memo_height(ptr)					get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_memo_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define set_memo_line_delta(ilk,ul)				set_dom_node_delta(ilk,(var_long)ul)

#define get_memo_line_delta(ilk)				get_dom_node_delta(ilk)

#define set_memo_line_text(ilk,token,len)		set_dom_node_text(ilk,token,len)

#define get_memo_line_text_ptr(ilk)				get_dom_node_text_ptr(ilk)

#define get_memo_line_text(ilk,buf,max)			get_dom_node_text(ilk,buf,max)

#define set_memo_line_indent(ilk,n)				set_dom_node_attr_integer(ilk,ATTR_ITEM_SPAN,n)

#define get_memo_line_indent(ilk)				get_dom_node_attr_integer(ilk,ATTR_ITEM_SPAN)

#define memo_line_text_set_chars(ilk,pos,pch,n)		dom_node_text_set_chars(ilk, pos, pch, n)

#define memo_line_text_ins_chars(ilk,pos,pch,n)		dom_node_text_ins_chars(ilk, pos, pch, n)

#define memo_line_text_del_chars(ilk,pos,n)			dom_node_text_del_chars(ilk, pos, n)

/*********************************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_memo_doc(void);

XDL_API void destroy_memo_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_memo_lineset(link_t_ptr ptr);

XDL_API void clear_memo_doc(link_t_ptr ptr);

XDL_API bool_t is_memo_doc(link_t_ptr ptr);

XDL_API bool_t is_memo_line(link_t_ptr ptr, link_t_ptr ilk);

XDL_API int get_memo_line_count(link_t_ptr ptr);

XDL_API link_t_ptr insert_memo_line(link_t_ptr ptr, link_t_ptr pos);

XDL_API void delete_memo_line(link_t_ptr tlk);

XDL_API link_t_ptr merge_memo_line(link_t_ptr nlk);

XDL_API link_t_ptr split_memo_line(link_t_ptr nlk, int pos);

XDL_API link_t_ptr get_memo_next_line(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_memo_prev_line(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_memo_line_at(link_t_ptr ptr, int index);

XDL_API int get_memo_line_index(link_t_ptr ptr, link_t_ptr tlk);

XDL_API int format_memo_doc(link_t_ptr ptr, tchar_t* buf, int max);

XDL_API bool_t parse_memo_doc(link_t_ptr ptr, const tchar_t* buf, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_MEMODOC_H*/
