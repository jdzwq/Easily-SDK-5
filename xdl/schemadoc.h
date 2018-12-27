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

/**************************************serial enable attributes**********************************************/

#define set_schema_element_name(nlk,val)						set_dom_node_attr(nlk,SCHEMA_ATTR_NAME,-1,val,-1)

#define get_schema_element_name_ptr(nlk)						get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_NAME,-1)

#define set_schema_element_id(nlk,val)							set_dom_node_attr(nlk,SCHEMA_ATTR_ID,-1,val,-1)

#define get_schema_element_id_ptr(nlk)							get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_ID,-1)

#define set_schema_element_type(nlk,val)						set_dom_node_attr(nlk,SCHEMA_ATTR_TYPE,-1,val,-1)

#define get_schema_element_type_ptr(nlk)						get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_TYPE,-1)

#define set_schema_element_ref(nlk,val)							set_dom_node_attr(nlk,SCHEMA_ATTR_REF,-1,val,-1)

#define get_schema_element_ref_ptr(nlk)							get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_REF,-1)

#define set_schema_element_ref(nlk,val)							set_dom_node_attr(nlk,SCHEMA_ATTR_REF,-1,val,-1)

#define get_schema_element_ref_ptr(nlk)							get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_REF,-1)

#define set_schema_element_minInclusive(nlk,val)				set_dom_node_attr(nlk,SCHEMA_ATTR_MININCLUSIVE,-1,val,-1)

#define get_schema_element_minInclusive_ptr(nlk)				get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MININCLUSIVE,-1)

#define set_schema_element_maxInclusive(nlk,val)				set_dom_node_attr(nlk,SCHEMA_ATTR_MAXINCLUSIVE,-1,val,-1)

#define get_schema_element_maxInclusive_ptr(nlk)				get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MAXINCLUSIVE,-1)

#define set_schema_element_minOccurs(nlk,val)					set_dom_node_attr(nlk,SCHEMA_ATTR_MINOCCURS,-1,val,-1)

#define get_schema_element_minOccurs_ptr(nlk)					get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MINOCCURS,-1)

#define set_schema_element_maxOccurs(nlk,val)					set_dom_node_attr(nlk,SCHEMA_ATTR_MAXOCCURS,-1,val,-1)

#define get_schema_element_maxOccurs_ptr(nlk)					get_dom_node_attr_ptr(nlk,SCHEMA_ATTR_MAXOCCURS,-1)

#define set_schema_element_precision(nlk,val)					set_dom_node_attr_integer(nlk,SCHEMA_ATTR_PRECISION,val)

#define get_schema_element_precision(nlk)						get_dom_node_attr_integer(nlk,SCHEMA_ATTR_PRECISION)

#define set_schema_element_scale(nlk,val)						set_dom_node_attr_integer(nlk,SCHEMA_ATTR_SCALE,val)

#define get_schema_element_scale(nlk)							get_dom_node_attr_integer(nlk,SCHEMA_ATTR_SCALE)
/******************************************runtime function**************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_schema_doc(void);

XDL_API void destroy_schema_doc(link_t_ptr ptr);

XDL_API bool_t is_schema_doc(link_t_ptr ptr);

XDL_API bool_t is_empty_schema(link_t_ptr ptr);

XDL_API void clear_schema_doc(link_t_ptr ptr);

XDL_API bool_t is_schema_declare(link_t_ptr nlk);

XDL_API bool_t is_schema_attribute(link_t_ptr nlk);

XDL_API bool_t is_schema_element(link_t_ptr nlk);

XDL_API link_t_ptr insert_schema_declare(link_t_ptr parent, link_t_ptr pos);

XDL_API link_t_ptr insert_schema_attribute(link_t_ptr parent, link_t_ptr pos);

XDL_API link_t_ptr insert_schema_element(link_t_ptr parent, link_t_ptr pos);

XDL_API void delete_schema_element(link_t_ptr nlk);

XDL_API int get_schema_element_count(link_t_ptr nlk);

XDL_API link_t_ptr get_schema_next_element(link_t_ptr parent,link_t_ptr nlk);

XDL_API link_t_ptr get_schema_prev_element(link_t_ptr parent,link_t_ptr nlk);

XDL_API link_t_ptr get_schema_parent_element(link_t_ptr nlk);

XDL_API link_t_ptr get_schema_element(link_t_ptr nlk,const tchar_t* ename);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*SCHEMADOC_H*/
