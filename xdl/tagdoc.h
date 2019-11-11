/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tag text doc document

	@module	tagdoc.h | interface file

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

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_tag_joint_name_ptr: get the tag joint name.
*/
#define get_tag_joint_name_ptr(nlk)				get_dom_node_name_ptr(nlk)
/*
@PROPER name: string.
@SET set_tag_joint_name: set the tag joint name.
*/
#define set_tag_joint_name(nlk,token)			set_dom_node_name(nlk,token,-1)

/*
@PROPER text: string.
@GET set_tag_joint_text: get the tag joint text.
*/
#define set_tag_joint_text(nlk,token,len)		set_dom_node_text(nlk,token,len)
/*
@PROPER text: string.
@SET get_tag_joint_text_ptr: set the tag joint text.
*/
#define get_tag_joint_text_ptr(nlk)				get_dom_node_text_ptr(nlk)

#define is_tag_text_joint(nlk)					((compare_text(get_tag_joint_name_ptr(nlk),-1,DOC_TAG_TEXT,-1,0) == 0)? 1 : 0)

#define tag_joint_text_set_chars(nlk,pos,pch,n)			dom_node_text_set_chars(nlk, pos, pch,n)

#define tag_joint_text_ins_chars(nlk,pos,pch,n)			dom_node_text_ins_chars(nlk, pos, pch,n)

#define tag_joint_text_del_chars(nlk,pos,n)				dom_node_text_del_chars(nlk, pos,n)


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Fonctions***************************************************************************/

/*
@FUNCTION create_tag_doc: create a tag document.
@RETURN link_t_ptr: return the tag document link component.
*/
XDL_API link_t_ptr create_tag_doc(void);

/*
@FUNCTION destroy_tag_doc: destroy a tag document.
@INPUT link_t_ptr ptr: the tag link component.
@RETURN void: none.
*/
XDL_API void destroy_tag_doc(link_t_ptr ptr);

/*
@FUNCTION get_tag_jointset: get tag node set.
@INPUT link_t_ptr ptr: the tag link component.
@RETURN link_t_ptr: the joint set link component.
*/
XDL_API link_t_ptr get_tag_jointset(link_t_ptr ptr);

/*
@FUNCTION is_tag_doc: test is tag document.
@INPUT link_t_ptr ptr: the tag link component.
@RETURN bool_t: return nonzero for being a tag document, otherwise return zero.
*/
XDL_API bool_t is_tag_doc(link_t_ptr ptr);

/*
@FUNCTION is_tag_joint: test is tag line node.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT link_t_ptr ilk: the joint link component.
@RETURN bool_t: return nonzero for being a node, otherwise return zero.
*/
XDL_API bool_t is_tag_joint(link_t_ptr ptr, link_t_ptr nlk);

/*
@FUNCTION clear_tag_doc: clear the tag document.
@INPUT link_t_ptr ptr: the tag link component.
@RETURN void: none.
*/
XDL_API void clear_tag_doc(link_t_ptr ptr);

/*
@FUNCTION get_tag_joint_count: counting the nodes in tag document.
@INPUT link_t_ptr ptr: the tag link component.
@RETURN int: return the number of nodes.
*/
XDL_API int get_tag_joint_count(link_t_ptr ptr);

/*
@FUNCTION insert_tag_joint: add a new node to tag document.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT link_t_ptr pos: the joint link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new node link component.
*/
XDL_API link_t_ptr insert_tag_joint(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_tag_joint: delete the node.
@INPUT link_t_ptr ilk: the joint link component.
@RETURN void: none.
*/
XDL_API void delete_tag_joint(link_t_ptr ilk);

/*
@FUNCTION merge_tag_joint: merge the node text to previous node.
@INPUT link_t_ptr ilk: the joint link component.
@RETURN link_t_ptr: return the joint link component merged.
*/
XDL_API link_t_ptr merge_tag_joint(link_t_ptr ilk);

/*
@FUNCTION get_tag_joint: find the node by name.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT const tchar_t* nname: the joint name token.
@RETURN link_t_ptr: return the joint link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_tag_joint(link_t_ptr ptr, const tchar_t* nname);

/*
@FUNCTION get_tag_next_joint: get the next node.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT link_t_ptr pos: the joint link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the joint link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_tag_next_joint(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_tag_prev_joint: get the previous node.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT link_t_ptr pos: the joint link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the joint link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_tag_prev_joint(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION clear_tag_joint_tag: clear the node tag.
@INPUT link_t_ptr ilk: the joint link component.
@RETURN void: none.
*/
XDL_API void clear_tag_joint_tag(link_t_ptr ilk);

/*
@FUNCTION is_tag_text_reserve: test is the tag reserve character.
@INPUT tchar_t ch: the character.
@RETURN bool_t: return nonzero for reserved, otherwise return zero.
*/
XDL_API bool_t is_tag_text_reserve(tchar_t ch);

/*
@FUNCTION format_tag_doc: format the tag document to string buffer.
@INPUT link_t_ptr ptr: the tag link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_tag_doc(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION parse_tag_doc: parse the tag document from string buffer.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT const tchar_t* buf: the string buffer.
@INPUT int len: the string buffer size in characters.
@RETURN int: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t parse_tag_doc(link_t_ptr ptr, const tchar_t* buf, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_TAGDOC_H*/
