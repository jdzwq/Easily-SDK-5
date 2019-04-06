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

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_rich_name_ptr: get the rich name.
*/
#define get_rich_name_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_rich_name: set the rich name.
*/
#define set_rich_name(ptr,token)					set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET get_rich_id_ptr: get the rich identifier.
*/
#define get_rich_id_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_ID,-1)
/*
@PROPER id: string.
@SET set_rich_id: set the rich identifer.
*/
#define set_rich_id(ptr,token)						set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)
/*
@PROPER title: string.
@GET get_rich_title_ptr: get the rich title.
*/
#define get_rich_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_rich_title: set the rich title.
*/
#define set_rich_title(ptr,token)					set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER dynamic: boolean.
@SET set_rich_dynamic: set the rich is dynamic.
*/
#define set_rich_dynamic(nlk,b)						set_dom_node_attr_boolean(nlk,ATTR_DYNAMIC,b) 
/*
@PROPER dynamic: boolean.
@GET get_rich_dynamic: get the rich is dynamic.
*/
#define get_rich_dynamic(nlk)						get_dom_node_attr_boolean(nlk,ATTR_DYNAMIC)
/*
@PROPER width: numeric.
@GET get_rich_width: get the rich width in millimeter.
*/
#define get_rich_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER width: numeric.
@SET set_rich_width: set the rich width in millimeter.
*/
#define set_rich_width(ptr,n)						set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER height: numeric.
@GET get_rich_height: get the rich height in millimeter.
*/
#define get_rich_height(ptr)						get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: numeric.
@SET set_rich_height: set the rich height in millimeter.
*/
#define set_rich_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER name: string.
@GET get_rich_anch_name_ptr: get the rich anchor name.
*/
#define get_rich_anch_name_ptr(nlk)					get_dom_node_attr_ptr(nlk,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_rich_anch_name: set the rich anchor name.
*/
#define set_rich_anch_name(nlk,token)				set_dom_node_attr(nlk,ATTR_NAME,-1,token,-1)
/*
@PROPER title: string.
@GET get_rich_anch_title_ptr: get the rich anchor title.
*/
#define get_rich_anch_title_ptr(nlk)				get_dom_node_attr_ptr(nlk,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_rich_anch_title: set the rich anchor title.
*/
#define set_rich_anch_title(nlk,token)				set_dom_node_attr(nlk,ATTR_TITLE,-1,token,-1)
/*
@PROPER text: string.
@SET set_rich_anch_text: set the rich anchor text.
*/
#define set_rich_anch_text(nlk,token,len)			set_dom_node_text(nlk,token,len)
/*
@PROPER text: string.
@GET get_rich_anch_text_ptr: get the rich anchor text.
*/
#define get_rich_anch_text_ptr(nlk)					get_dom_node_text_ptr(nlk)
/*
@PROPER indent: numeric.
@SET set_rich_anch_indent: set the line indent.
*/
#define set_rich_anch_text_indent(ilk,n)			set_dom_node_attr_float(ilk,ATTR_TEXT_INDENT,n)
/*
@PROPER indent: numeric.
@GET get_rich_anch_indent: get the line indent.
*/
#define get_rich_anch_text_indent(ilk)				get_dom_node_attr_float(ilk,ATTR_TEXT_INDENT)
/*
@PROPER place: numeric.
@SET set_rich_anch_place: set the text tabed place.
*/
#define set_rich_anch_text_place(ilk,n)				set_dom_node_attr_float(ilk,ATTR_TEXT_PLACE,n)
/*
@PROPER place: numeric.
@GET get_rich_anch_place: get the text tabed place.
*/
#define get_rich_anch_text_place(ilk)				get_dom_node_attr_float(ilk,ATTR_TEXT_PLACE)
/*
@PROPER fixed: boolean.
@SET set_rich_anch_fixed: set the rich anchor is fixed.
*/
#define set_rich_anch_fixed(nlk,b)					set_dom_node_attr_boolean(nlk,ATTR_FIXED,b) 
/*
@PROPER fixed: boolean.
@GET get_rich_anch_fixed: get the rich anchor is fixed.
*/
#define get_rich_anch_fixed(nlk)					get_dom_node_attr_boolean(nlk,ATTR_FIXED)
/*
@PROPER lined: boolean.
@SET set_rich_anch_lined: set the rich anchor is lined.
*/
#define set_rich_anch_lined(nlk,b)					set_dom_node_attr_boolean(nlk,ATTR_LINED,b) 
/*
@PROPER lined: boolean.
@GET get_rich_anch_lined: get the rich anchor is lined.
*/
#define get_rich_anch_lined(nlk)					get_dom_node_attr_boolean(nlk,ATTR_LINED)

#define rich_anch_text_set_chars(nlk,pos,pch,n)			dom_node_text_set_chars(nlk, pos, pch, n)

#define rich_anch_text_ins_chars(nlk,pos,pch,n)			dom_node_text_ins_chars(nlk, pos, pch, n)

#define rich_anch_text_del_chars(nlk,pos,n)				dom_node_text_del_chars(nlk, pos, n)

#define rich_anch_title_set_chars(nlk,pos,pch,n)		dom_node_attr_set_chars(nlk, ATTR_TITLE, pos, pch, n)

#define rich_anch_title_ins_chars(nlk,pos,pch,n)		dom_node_attr_ins_chars(nlk, ATTR_TITLE, pos, pch, n)

#define rich_anch_title_del_chars(nlk,pos,n)			dom_node_attr_del_chars(nlk, ATTR_TITLE, pos, n)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_rich_doc: create a rich document.
@RETURN link_t_ptr: return the rich document link component.
*/
XDL_API link_t_ptr create_rich_doc(void);

/*
@FUNCTION destroy_rich_doc: destroy a rich document.
@INPUT link_t_ptr ptr: the rich link component.
@RETURN void: none.
*/
XDL_API void destroy_rich_doc(link_t_ptr ptr);

/*
@FUNCTION get_rich_anchset: get rich anchor set.
@INPUT link_t_ptr ptr: the rich link component.
@RETURN link_t_ptr: the anchor set link component.
*/
XDL_API link_t_ptr get_rich_anchset(link_t_ptr ptr);

/*
@FUNCTION is_rich_doc: test is rich document.
@INPUT link_t_ptr ptr: the rich link component.
@RETURN bool_t: return nonzero for being a rich document, otherwise return zero.
*/
XDL_API bool_t is_rich_doc(link_t_ptr ptr);

/*
@FUNCTION is_rich_anch: test is rich anchor.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT link_t_ptr ilk: the anchor link component.
@RETURN bool_t: return nonzero for being a anchor, otherwise return zero.
*/
XDL_API bool_t is_rich_anch(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION clear_rich_doc: clear the rich document.
@INPUT link_t_ptr ptr: the rich link component.
@RETURN void: none.
*/
XDL_API void clear_rich_doc(link_t_ptr ptr);

/*
@FUNCTION reset_rich_doc: clear all of anchors text.
@INPUT link_t_ptr ptr: the rich link component.
@RETURN void: none.
*/
XDL_API void reset_rich_doc(link_t_ptr ptr);

/*
@FUNCTION get_rich_anch_count: counting the anchors in tag document.
@INPUT link_t_ptr ptr: the rich link component.
@RETURN int: return the number of anchors.
*/
XDL_API int get_rich_anch_count(link_t_ptr ptr);

/*
@FUNCTION insert_rich_anch: add a new anchor to rich document.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT link_t_ptr pos: the anchor link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new anchor link component.
*/
XDL_API link_t_ptr insert_rich_anch(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_rich_anch: delete the anchor.
@INPUT link_t_ptr ilk: the anchor link component.
@RETURN void: none.
*/
XDL_API void delete_rich_anch(link_t_ptr ilk);

/*
@FUNCTION get_rich_anch: find the anchor by name.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT const tchar_t* nname: the anchor name token.
@RETURN link_t_ptr: return the anchor link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_rich_anch(link_t_ptr ptr, const tchar_t* nname);

/*
@FUNCTION get_rich_next_anchor: get the next anchor.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT link_t_ptr pos: the anchor link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the anchor link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_rich_next_anch(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_rich_prev_anchor: get the previous anchor.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT link_t_ptr pos: the anchor link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the anchor link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_rich_prev_anch(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION is_rich_text_reserve: test is the rich reserve character.
@INPUT tchar_t ch: the character.
@RETURN bool_t: return nonzero for reserved, otherwise return zero.
*/
XDL_API bool_t is_rich_text_reserve(tchar_t ch);

/*
@FUNCTION format_rich_doc: format the rich document to string buffer.
@INPUT link_t_ptr ptr: the rich link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_rich_doc(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION parse_rich_doc: parse the rich document from string buffer.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT const tchar_t* buf: the string buffer.
@INPUT int len: the string buffer size in characters.
@RETURN int: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t parse_rich_doc(link_t_ptr ptr, const tchar_t* buf, int len);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif /*_RICHDOC_H*/
