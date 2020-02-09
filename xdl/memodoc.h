/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memo document

	@module	memodoc.h | interface file

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

/************************************************Properties***************************************************************************/

/*
@PROPER width: float.
@GET get_memo_width: get the memo width.
*/
#define get_memo_width(ptr)						get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER width: float.
@SET set_memo_width: set the memo width.
*/
#define set_memo_width(ptr,n)					set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER height: float.
@GET get_memo_height: get the memo height.
*/
#define get_memo_height(ptr)					get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: float.
@SET set_memo_height: set the memo height.
*/
#define set_memo_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER delta: var_long.
@SET set_memo_line_delta: set the line extract data.
*/
#define set_memo_line_delta(ilk,ul)				set_dom_node_delta(ilk,(var_long)ul)
/*
@PROPER delta: var_long.
@GET get_memo_line_delta: get the memo extract data.
*/
#define get_memo_line_delta(ilk)				get_dom_node_delta(ilk)
/*
@PROPER text: string.
@SET set_memo_line_text: set the line text.
*/
#define set_memo_line_text(ilk,token,len)		set_dom_node_text(ilk,token,len)
/*
@PROPER text: string.
@GET get_memo_line_text_ptr: get the line text.
*/
#define get_memo_line_text_ptr(ilk)				get_dom_node_text_ptr(ilk)

#define get_memo_line_text(ilk,buf,max)			get_dom_node_text(ilk,buf,max)
/*
@PROPER indent: integer.
@SET set_memo_line_indent: set the line indent.
*/
#define set_memo_line_indent(ilk,n)				set_dom_node_attr_integer(ilk,ATTR_TEXT_INDENT,n)
/*
@PROPER indent: integer.
@GET get_memo_line_indent: get the line indent.
*/
#define get_memo_line_indent(ilk)				get_dom_node_attr_integer(ilk,ATTR_TEXT_INDENT)

#define memo_line_text_set_chars(ilk,pos,pch,n)		dom_node_text_set_chars(ilk, pos, pch, n)

#define memo_line_text_ins_chars(ilk,pos,pch,n)		dom_node_text_ins_chars(ilk, pos, pch, n)

#define memo_line_text_del_chars(ilk,pos,n)			dom_node_text_del_chars(ilk, pos, n)


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_memo_doc: create a memo document.
@RETURN link_t_ptr: return the memo document link component.
*/
XDL_API link_t_ptr create_memo_doc(void);

/*
@FUNCTION destroy_memo_doc: destroy a memo document.
@INPUT link_t_ptr ptr: the memo link component.
@RETURN void: none.
*/
XDL_API void destroy_memo_doc(link_t_ptr ptr);

/*
@FUNCTION get_memo_lineset: get memo line set.
@INPUT link_t_ptr ptr: the memo link component.
@RETURN link_t_ptr: the line set link component.
*/
XDL_API link_t_ptr get_memo_lineset(link_t_ptr ptr);

/*
@FUNCTION clear_memo_doc: clear the memo document.
@INPUT link_t_ptr ptr: the memo link component.
@RETURN void: none.
*/
XDL_API void clear_memo_doc(link_t_ptr ptr);

/*
@FUNCTION is_memo_doc: test is memo document.
@INPUT link_t_ptr ptr: the memo link component.
@RETURN bool_t: return nonzero for being a memo document, otherwise return zero.
*/
XDL_API bool_t is_memo_doc(link_t_ptr ptr);

/*
@FUNCTION is_memo_line: test is memo line node.
@INPUT link_t_ptr ptr: the memo link component.
@INPUT link_t_ptr ilk: the line link component.
@RETURN bool_t: return nonzero for being a line node, otherwise return zero.
*/
XDL_API bool_t is_memo_line(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION get_memo_line_count: counting the line nodes in memo document.
@INPUT link_t_ptr ptr: the memo link component.
@RETURN int: return the number of line nodes.
*/
XDL_API int get_memo_line_count(link_t_ptr ptr);

/*
@FUNCTION insert_memo_line: add a new line node to memo document.
@INPUT link_t_ptr ptr: the memo link component.
@INPUT link_t_ptr pos: the line link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new line link component.
*/
XDL_API link_t_ptr insert_memo_line(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_memo_line: delete the line node.
@INPUT link_t_ptr ilk: the line link component.
@RETURN void: none.
*/
XDL_API void delete_memo_line(link_t_ptr ilk);

/*
@FUNCTION merge_memo_line: merge the line text to previous node.
@INPUT link_t_ptr ilk: the line link component.
@RETURN link_t_ptr: return the line link component merged.
*/
XDL_API link_t_ptr merge_memo_line(link_t_ptr ilk);

/*
@FUNCTION split_memo_line: split the line text at position.
@INPUT link_t_ptr ilk: the line link component.
@INPUT int pos: the zero based position.
@RETURN link_t_ptr: return the new line link component splited.
*/
XDL_API link_t_ptr split_memo_line(link_t_ptr ilk, int pos);

/*
@FUNCTION get_memo_next_line: get the next line node.
@INPUT link_t_ptr ptr: the memo link component.
@INPUT link_t_ptr pos: the line link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the line link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_memo_next_line(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_memo_prev_line: get the previous line node.
@INPUT link_t_ptr ptr: the memo link component.
@INPUT link_t_ptr pos: the line link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the line link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_memo_prev_line(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_memo_line_at: find the line node at position.
@INPUT link_t_ptr ptr: the memo link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the line link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_memo_line_at(link_t_ptr ptr, int index);

/*
@FUNCTION get_memo_line_index: calc the line index in memo document.
@INPUT link_t_ptr ptr: the memo link component.
@INPUT link_t_ptr ilk: the line link component.
@RETURN int: return the zero based position.
*/
XDL_API int get_memo_line_index(link_t_ptr ptr, link_t_ptr ilk);


#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_MEMODOC_H*/
