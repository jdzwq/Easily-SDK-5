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

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_anno_name_ptr: get the annotation name.
*/
#define get_anno_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_anno_name: set the annotation name.
*/
#define set_anno_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET get_anno_id_ptr: get the annotation identifier.
*/
#define get_anno_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_anno_id: set the annotation identifier.
*/
#define set_anno_id(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER arti: string.
@GET get_anno_arti_name_ptr: get the article name.
*/
#define get_anno_arti_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER arti: string.
@SET set_anno_arti_name: set the article name.
*/
#define set_anno_arti_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET get_anno_arti_id_ptr: get the article identifier.
*/
#define get_anno_arti_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_anno_arti_id: set the article identifer.
*/
#define set_anno_arti_id(ilk,token)						set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER type: string.
@GET get_anno_arti_type_ptr: get the article type.
*/
#define get_anno_arti_type_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TYPE,-1)
/*
@PROPER type: string.
@SET set_anno_arti_type: set the article type.
*/
#define set_anno_arti_type(ilk,token)					set_dom_node_attr(ilk,ATTR_TYPE,-1,token,-1)
/*
@PROPER style: string.
@SET set_anno_arti_style: set the article style.
*/
#define set_anno_arti_style(ilk,token)					set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)
/*
@PROPER style: string.
@GET get_anno_arti_style_ptr: get the article style.
*/
#define get_anno_arti_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)
/*
@PROPER text: string.
@SET set_anno_arti_text: set the article text.
*/
#define set_anno_arti_text(ilk,token)					set_dom_node_text(ilk,token,-1)
/*
@PROPER text: string.
@GET get_anno_arti_text_ptr: get the article text.
*/
#define get_anno_arti_text_ptr(ilk)						get_dom_node_text_ptr(ilk)
/*
@PROPER points: string.
@SET set_anno_arti_points: set the article points.
*/
#define set_anno_arti_points(ilk,token)					set_dom_node_attr(ilk,ATTR_POINTS,-1,token,-1)
/*
@PROPER points: string.
@GET get_anno_arti_points_ptr: get the article points.
*/
#define get_anno_arti_points_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_POINTS,-1)

/*
@PROPER selected: boolean.
@SET set_anno_arti_selected: set the article is selected.
*/
#define set_anno_arti_selected(ilk,b)					set_dom_node_mask_check(ilk,MSK_CHECKED,b)
/*
@PROPER selected: boolean.
@GET get_anno_arti_selected: get the article is selected.
*/
#define get_anno_arti_selected(ilk)						get_dom_node_mask_check(ilk,MSK_CHECKED) 


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_anno_doc: create a annotation document.
@RETURN link_t_ptr: return the annotation document link component.
*/
XDL_API link_t_ptr create_anno_doc(void);

/*
@FUNCTION destroy_anno_doc: destroy a annotation document.
@INPUT link_t_ptr ptr: the annotation link component.
@RETURN void: none.
*/
XDL_API void destroy_anno_doc(link_t_ptr ptr);

/*
@FUNCTION clear_anno_doc: clear the annotation document.
@INPUT link_t_ptr ptr: the annotation link component.
@RETURN void: none.
*/
XDL_API void clear_anno_doc(link_t_ptr ptr);

/*
@FUNCTION get_anno_artiset: get annotation article set.
@INPUT link_t_ptr ptr: the annotation link component.
@RETURN link_t_ptr: the article set link component.
*/
XDL_API link_t_ptr get_anno_artiset(link_t_ptr ptr);

/*
@FUNCTION is_anno_doc: test is annotation document.
@INPUT link_t_ptr ptr: the annotation link component.
@RETURN bool_t: return nonzero for being a annotation document, otherwise return zero.
*/
XDL_API bool_t is_anno_doc(link_t_ptr ptr);

/*
@FUNCTION is_anno_arti: test is annotation article node.
@INPUT link_t_ptr ptr: the annotation link component.
@INPUT link_t_ptr ilk: the article link component.
@RETURN bool_t: return nonzero for being a article, otherwise return zero.
*/
XDL_API bool_t is_anno_arti(link_t_ptr ptr, link_t_ptr plk);

/*
@FUNCTION anno_doc_from_arti: trackback to annotation document from article node.
@INPUT link_t_ptr ilk: the article link component.
@RETURN link_t_ptr: return the annotation link component.
*/
XDL_API link_t_ptr anno_doc_from_arti(link_t_ptr ilk);

/*
@FUNCTION insert_anno_arti: add a new article to annotation document.
@INPUT link_t_ptr ptr: the annotation link component.
@INPUT link_t_ptr pos: the article link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new article link component.
*/
XDL_API link_t_ptr insert_anno_arti(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_anno_next_arti: get the next article.
@INPUT link_t_ptr ptr: the annotation link component.
@INPUT link_t_ptr pos: the article link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the article link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_anno_next_arti(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_anno_prev_arti: get the previous article.
@INPUT link_t_ptr ptr: the annotation link component.
@INPUT link_t_ptr pos: the article link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the article link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_anno_prev_arti(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_anno_arti_at: get the article at position.
@INPUT link_t_ptr ptr: the annotation link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the article link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_anno_arti_at(link_t_ptr ptr, int index);

/*
@FUNCTION delete_anno_arti: delete the article.
@INPUT link_t_ptr ilk: the article link component.
@RETURN void: none.
*/
XDL_API void delete_anno_arti(link_t_ptr ilk);

/*
@FUNCTION get_anno_arti_count: counting the articles in annotation document.
@INPUT link_t_ptr ptr: the annotation link component.
@RETURN int: return the number of nodes.
*/
XDL_API int get_anno_arti_count(link_t_ptr ptr);

/*
@FUNCTION get_anno_arti_selected_count: counting the selected articles in annotation document.
@INPUT link_t_ptr ptr: the annotation link component.
@RETURN int: return the number of nodes.
*/
XDL_API int get_anno_arti_selected_count(link_t_ptr ptr);

/*
@FUNCTION get_anno_arti_xpoint: get the article points.
@INPUT link_t_ptr ilk: the article link component.
@OUTPUT xpoint_t* ppt: the points buffer.
@INPUT int max: the points buffer size.
@RETURN int: return the number of pioints.
*/
XDL_API int get_anno_arti_xpoint(link_t_ptr ilk, xpoint_t* ppt, int max);

/*
@FUNCTION set_anno_arti_xpoint: set the article points.
@INPUT link_t_ptr ilk: the article link component.
@INPUT const xpoint_t* ppt: the points buffer.
@INPUT int n: the number of points.
@RETURN int: return the number of pioints.
*/
XDL_API void set_anno_arti_xpoint(link_t_ptr ilk, const xpoint_t* ppt, int n);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*ANNODOC_H*/