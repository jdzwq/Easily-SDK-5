/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc schema document

	@module	schemadoc.h | schema document interface file

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

#ifndef _SCHEMADOC_H
#define _SCHEMADOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**********************************************Properties**********************************************/

/*
@PROPER name: string.
@SET set_schema_element_name: set the schema element node name.
*/
#define set_schema_element_name(nlk,val)						set_dom_node_attr(nlk,SCHEMA_ATTR_NAME,-1,val,-1)
/*
@PROPER name: string.
@GET get_schema_element_name_ptr: get the schema element node name.
*/
#define get_schema_element_name_ptr(nlk)						get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_schema_element_id: set the schema element node identifer.
*/
#define set_schema_element_id(nlk,val)							set_dom_node_attr(nlk,SCHEMA_ATTR_ID,-1,val,-1)
/*
@PROPER id: string.
@GET get_schema_element_id_ptr: get the schema element node identifer.
*/
#define get_schema_element_id_ptr(nlk)							get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_ID,-1)
/*
@PROPER type: string.
@SET set_schema_element_type: set the schema element node type.
*/
#define set_schema_element_type(nlk,val)						set_dom_node_attr(nlk,SCHEMA_ATTR_TYPE,-1,val,-1)
/*
@PROPER type: string.
@GET get_schema_element_type_ptr: get the schema element node type.
*/
#define get_schema_element_type_ptr(nlk)						get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_TYPE,-1)
/*
@PROPER ref: string.
@SET set_schema_element_ref: set the schema element node reference.
*/
#define set_schema_element_ref(nlk,val)							set_dom_node_attr(nlk,SCHEMA_ATTR_REF,-1,val,-1)
/*
@PROPER ref: string.
@GET get_schema_element_ref_ptr: get the schema element node reference.
*/
#define get_schema_element_ref_ptr(nlk)							get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_REF,-1)
/*
@PROPER minInclusive: string.
@SET set_schema_element_minInclusive: set the schema element node minInclusive.
*/
#define set_schema_element_minInclusive(nlk,val)				set_dom_node_attr(nlk,SCHEMA_ATTR_MININCLUSIVE,-1,val,-1)
/*
@PROPER minInclusive: string.
@GET get_schema_element_minInclusive_ptr: get the schema element node minInclusive.
*/
#define get_schema_element_minInclusive_ptr(nlk)				get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MININCLUSIVE,-1)
/*
@PROPER maxInclusive: string.
@SET set_schema_element_maxInclusive: set the schema element node maxInclusive.
*/
#define set_schema_element_maxInclusive(nlk,val)				set_dom_node_attr(nlk,SCHEMA_ATTR_MAXINCLUSIVE,-1,val,-1)
/*
@PROPER maxInclusive: string.
@GET get_schema_element_maxInclusive_ptr: get the schema element node maxInclusive.
*/
#define get_schema_element_maxInclusive_ptr(nlk)				get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MAXINCLUSIVE,-1)
/*
@PROPER minOccurs: string.
@SET set_schema_element_minOccurs: set the schema element node minOccurs.
*/
#define set_schema_element_minOccurs(nlk,val)					set_dom_node_attr(nlk,SCHEMA_ATTR_MINOCCURS,-1,val,-1)
/*
@PROPER minOccurs: string.
@SET get_schema_element_minOccurs_ptr: get the schema element node minOccurs.
*/
#define get_schema_element_minOccurs_ptr(nlk)					get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MINOCCURS,-1)
/*
@PROPER maxOccurs: string.
@SET set_schema_element_maxOccurs: set the schema element node maxOccurs.
*/
#define set_schema_element_maxOccurs(nlk,val)					set_dom_node_attr(nlk,SCHEMA_ATTR_MAXOCCURS,-1,val,-1)
/*
@PROPER maxOccurs: string.
@SET get_schema_element_maxOccurs_ptr: get the schema element node maxOccurs.
*/
#define get_schema_element_maxOccurs_ptr(nlk)					get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MAXOCCURS,-1)
/*
@PROPER precision: integer.
@SET set_schema_element_precision: set the schema element node precision.
*/
#define set_schema_element_precision(nlk,val)					set_dom_node_attr_integer(nlk,SCHEMA_ATTR_PRECISION,val)
/*
@PROPER precision: integer.
@GET get_schema_element_precision: get the schema element node precision.
*/
#define get_schema_element_precision(nlk)						get_dom_node_attr_integer(nlk,SCHEMA_ATTR_PRECISION)
/*
@PROPER scale: integer.
@SET set_schema_element_scale: set the schema element node scale.
*/
#define set_schema_element_scale(nlk,val)						set_dom_node_attr_integer(nlk,SCHEMA_ATTR_SCALE,val)
/*
@PROPER scale: integer.
@GET get_schema_element_scale: get the schema element node scale.
*/
#define get_schema_element_scale(nlk)							get_dom_node_attr_integer(nlk,SCHEMA_ATTR_SCALE)

#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************Functions**********************************************/

/*
@FUNCTION create_schema_doc: create a schema document.
@RETURN link_t_ptr: return the schema document link component.
*/
XDL_API link_t_ptr create_schema_doc(void);

/*
@FUNCTION destroy_schema_doc: destroy a schema document.
@INPUT link_t_ptr ptr: the schema link component.
@RETURN void: none.
*/
XDL_API void destroy_schema_doc(link_t_ptr ptr);

/*
@FUNCTION is_schema_doc: test is a schema document.
@INPUT link_t_ptr ptr: the schema link component.
@RETURN bool_t: return nonzero for being a schema document, otherwise return zero.
*/
XDL_API bool_t is_schema_doc(link_t_ptr ptr);

/*
@FUNCTION is_empty_schema: test the schema document is empty.
@INPUT link_t_ptr ptr: the schema link component.
@RETURN bool_t: return nonzero for being empty document, otherwise return zero.
*/
XDL_API bool_t is_empty_schema(link_t_ptr ptr);

/*
@FUNCTION clear_schema_doc: clear the schema document, all of child nodes will be emptied.
@INPUT link_t_ptr ptr: the schema link component.
@RETURN void: none.
*/
XDL_API void clear_schema_doc(link_t_ptr ptr);

/*
@FUNCTION is_schema_declare: test is a declare schema node.
@INPUT link_t_ptr nlk: the declare node link component.
@RETURN bool_t: return nonzero for being declare node, otherwise return zero.
*/
XDL_API bool_t is_schema_declare(link_t_ptr nlk);

/*
@FUNCTION is_schema_attribute: test is a attribute schema node.
@INPUT link_t_ptr nlk: the attribute node link component.
@RETURN bool_t: return nonzero for being attribute node, otherwise return zero.
*/
XDL_API bool_t is_schema_attribute(link_t_ptr nlk);

/*
@FUNCTION is_schema_element: test is a element schema node.
@INPUT link_t_ptr nlk: the element node link component.
@RETURN bool_t: return nonzero for being element node, otherwise return zero.
*/
XDL_API bool_t is_schema_element(link_t_ptr nlk);

/*
@FUNCTION insert_schema_declare: insert a declare schema node.
@INPUT link_t_ptr parent: the parent node link component.
@INPUT link_t_ptr pos: the declare link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return declare link component.
*/
XDL_API link_t_ptr insert_schema_declare(link_t_ptr parent, link_t_ptr pos);

/*
@FUNCTION insert_schema_attribute: insert a attribute schema node.
@INPUT link_t_ptr parent: the parent node link component.
@INPUT link_t_ptr pos: the attribute link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return attribute link component.
*/
XDL_API link_t_ptr insert_schema_attribute(link_t_ptr parent, link_t_ptr pos);

/*
@FUNCTION insert_schema_element: insert a element schema node.
@INPUT link_t_ptr parent: the parent node link component.
@INPUT link_t_ptr pos: the element link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return element link component.
*/
XDL_API link_t_ptr insert_schema_element(link_t_ptr parent, link_t_ptr pos);

/*
@FUNCTION delete_schema_element: delete a element schema node.
@INPUT link_t_ptr nlk: the elementt link component.
@RETURN none: none.
*/
XDL_API void delete_schema_element(link_t_ptr nlk);

/*
@FUNCTION get_schema_element_count: counting the element child nodes.
@INPUT link_t_ptr nlk: the elementt link component.
@RETURN int: the number of element child nodes.
*/
XDL_API int get_schema_element_count(link_t_ptr nlk);

/*
@FUNCTION get_schema_next_element: get the next element node.
@INPUT link_t_ptr parent: the parent link component.
@INPUT link_t_ptr nlk: the element link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return element link component.
*/
XDL_API link_t_ptr get_schema_next_element(link_t_ptr parent,link_t_ptr nlk);

/*
@FUNCTION get_schema_prev_element: get the previous element node.
@INPUT link_t_ptr parent: the parent link component.
@INPUT link_t_ptr nlk: the element link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return element link component.
*/
XDL_API link_t_ptr get_schema_prev_element(link_t_ptr parent,link_t_ptr nlk);

/*
@FUNCTION get_schema_parent_element: get the parent element node.
@INPUT link_t_ptr nlk: the element link component.
@RETURN link_t_ptr: return parent link component.
*/
XDL_API link_t_ptr get_schema_parent_element(link_t_ptr nlk);

/*
@FUNCTION get_schema_element: find the parent element node by name.
@INPUT link_t_ptr nlk: the parent link component.
@INPUT const tchar_t* ename: the name string token.
@RETURN link_t_ptr: return element link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_schema_element(link_t_ptr nlk,const tchar_t* ename);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*SCHEMADOC_H*/
