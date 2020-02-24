/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc meta document

	@module	metadoc.h | interface file

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

#ifndef _METADOC_H
#define _METADOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_meta_doc: create a meta document.
@RETURN link_t_ptr: return the meta document link component.
*/
XDL_API link_t_ptr create_meta_doc();

/*
@FUNCTION destroy_meta_doc: destroy a meta document.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN void: none.
*/
XDL_API void destroy_meta_doc(link_t_ptr ptr);

/*
@FUNCTION clear_meta_doc: clear a meta document.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN void: none.
*/
XDL_API void clear_meta_doc(link_t_ptr ptr);

/*
@FUNCTION clear_meta_head_nodes: delete all of meta header child nodes.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN void: none.
*/
XDL_API void clear_meta_head_nodes(link_t_ptr ptr);

/*
@FUNCTION clear_meta_body_nodes: delete all of meta body child nodes.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN void: none.
*/
XDL_API void clear_meta_body_nodes(link_t_ptr ptr);

/*
@FUNCTION set_meta_doc_name: set meta document name.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT const tchar_t* sz_name: the name string token.
@RETURN void: none.
*/
XDL_API void set_meta_doc_name(link_t_ptr ptr, const tchar_t* sz_name);

/*
@FUNCTION get_meta_doc_name_ptr: get meta document name token.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN const tchar_t*: return the meta name token.
*/
XDL_API const tchar_t* get_meta_doc_name_ptr(link_t_ptr ptr);

/*
@FUNCTION get_meta_head_node: get meta header node.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN link_t_ptr: return the header node link component.
*/
XDL_API link_t_ptr get_meta_head_node(link_t_ptr ptr);

/*
@FUNCTION get_meta_body_node: get meta body node.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN link_t_ptr: return the header body link component.
*/
XDL_API link_t_ptr get_meta_body_node(link_t_ptr ptr);

/*
@FUNCTION attach_meta_body_node: attach a document as meta body node.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT link_t_ptr doc: the document link component.
@RETURN void: none.
*/
XDL_API void attach_meta_body_node(link_t_ptr ptr, link_t_ptr doc);

/*
@FUNCTION detach_meta_body_node: detach a document from meta body node.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN link_t_ptr: return the document link component detached if exists, otherwise return NULL.
*/
XDL_API link_t_ptr detach_meta_body_node(link_t_ptr ptr);

/*
@FUNCTION set_meta_head_title: set a document title.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT const tchar_t* sz_title: the title string token.
@INPUT int len: the length of title in characters.
@RETURN void: none.
*/
XDL_API void set_meta_head_title(link_t_ptr ptr, const tchar_t* sz_title, int len);

/*
@FUNCTION get_meta_head_title: copy document title.
@INPUT link_t_ptr ptr: the meta link component.
@OUTPUT tchar_t* sz_buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copyed.
*/
XDL_API int get_meta_head_title(link_t_ptr ptr, tchar_t* sz_buf, int max);

/*
@FUNCTION get_meta_head_title_ptr: get document title token.
@INPUT link_t_ptr ptr: the meta link component.
@RETURN const tchar_t*: return title token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_meta_head_title_ptr(link_t_ptr ptr);

/*
@FUNCTION set_meta_head_meta: add a document meta information.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT const tchar_t* sz_name: the name string token.
@INPUT int nlen: the name token length in characters.
@INPUT const tchar_t* sz_content: the content string token.
@INPUT int clen: the content token length in characters.
@RETURN void: none.
*/
XDL_API void set_meta_head_meta(link_t_ptr ptr, const tchar_t* sz_name, int nlen, const tchar_t* sz_content, int clen);

/*
@FUNCTION get_meta_head_meta: copy document meta content by name.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT const tchar_t* sz_name: the name string token.
@INPUT int nlen: the name token length in characters.
@OUTPUT tchar_t* sz_buf: the string buffer for returning content token.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters copyed.
*/
XDL_API int get_meta_head_meta(link_t_ptr ptr, const tchar_t* sz_name, int nlen, tchar_t* sz_buf, int max);

/*
@FUNCTION get_meta_head_meta_ptr: get document meta content token by name.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT const tchar_t* sz_name: the name string token.
@INPUT int nlen: the name token length in characters.
@RETURN const tchar_t*: return the content string token.
*/
XDL_API const tchar_t* get_meta_head_meta_ptr(link_t_ptr ptr, const tchar_t* sz_name, int nlen);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif //_METADOC_H