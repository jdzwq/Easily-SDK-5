/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc diagram document

	@module	diagramdoc.h | interface file

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

#ifndef _DIAGRAMDOC_H
#define _DIAGRAMDOC_H

#include "xdldef.h"
#include "docattr.h"

#ifdef XDL_SUPPORT_DOC

/*******************************************serial enable attributes********************************************/
#define set_diagram_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_diagram_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_diagram_id(ptr,token)							set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)

#define get_diagram_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_diagram_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_diagram_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define get_diagram_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_diagram_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_diagram_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_diagram_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define set_diagram_printing(ptr,val)					set_dom_node_attr(ptr,ATTR_PRINTING,-1,val,-1)

#define get_diagram_printing_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_PRINTING,-1)

#define set_diagram_entity_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_diagram_entity_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define get_diagram_entity_class_ptr(ilk)					get_dom_node_name_ptr(ilk)

#define set_diagram_entity_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)

#define get_diagram_entity_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

#define set_diagram_entity_text(ilk,token,len)				set_dom_node_text(ilk,token,len)

#define get_diagram_entity_text_ptr(ilk)					get_dom_node_text_ptr(ilk)

#define set_diagram_entity_style(ilk,token)				set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)

#define get_diagram_entity_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)

#define get_diagram_entity_x(ilk)							get_dom_node_attr_float(ilk,ATTR_X)

#define set_diagram_entity_x(ilk,xm)						set_dom_node_attr_float(ilk,ATTR_X,xm)

#define get_diagram_entity_y(ilk)							get_dom_node_attr_float(ilk,ATTR_Y)

#define set_diagram_entity_y(ilk,ym)						set_dom_node_attr_float(ilk,ATTR_Y,ym)

#define set_diagram_entity_width(ilk,n)					set_dom_node_attr_float(ilk,ATTR_WIDTH,n)

#define get_diagram_entity_width(ilk)						get_dom_node_attr_float(ilk,ATTR_WIDTH)

#define set_diagram_entity_height(ilk,n)					set_dom_node_attr_float(ilk,ATTR_HEIGHT,n)

#define get_diagram_entity_height(ilk)						get_dom_node_attr_float(ilk,ATTR_HEIGHT)

#define get_diagram_entity_taborder(ilk)					get_dom_node_attr_integer(ilk,ATTR_TABORDER)

#define set_diagram_entity_taborder(ilk,n)					set_dom_node_attr_integer(ilk,ATTR_TABORDER,n)

/**********************************************************************************************************/
#define set_diagram_entity_delta(ilk,ul)					set_dom_node_delta(ilk,(vword_t)ul)

#define get_diagram_entity_delta(ilk)						get_dom_node_delta(ilk)

#define set_diagram_entity_selected(flk,b)					set_dom_node_mask_check(flk,MSK_CHECKED,b)

#define get_diagram_entity_selected(flk)					get_dom_node_mask_check(flk,MSK_CHECKED) 

/*********************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

EXP_API link_t_ptr create_diagram_doc(void);

EXP_API void destroy_diagram_doc(link_t_ptr ptr);

EXP_API void clear_diagram_doc(link_t_ptr ptr);

EXP_API bool_t is_diagram_doc(link_t_ptr ptr);

EXP_API bool_t is_diagram_entity(link_t_ptr ptr, link_t_ptr ilk);

EXP_API void reset_diagram_taborder(link_t_ptr ptr);

EXP_API link_t_ptr get_diagram_entityset(link_t_ptr ptr);

EXP_API link_t_ptr insert_diagram_entity(link_t_ptr ptr, const tchar_t* sz_class);

EXP_API link_t_ptr get_diagram_next_entity(link_t_ptr ptr,link_t_ptr pos);

EXP_API link_t_ptr get_diagram_prev_entity(link_t_ptr ptr,link_t_ptr pos);

EXP_API link_t_ptr get_diagram_entity(link_t_ptr ptr, const tchar_t* sz_name);

EXP_API int get_diagram_entity_count(link_t_ptr ptr);

EXP_API int get_diagram_entity_count_by_class(link_t_ptr ptr, const tchar_t* szClass);

EXP_API int get_diagram_entity_selected_count(link_t_ptr ptr);

EXP_API void delete_diagram_entity(link_t_ptr ilk);


#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //DIAGRAMDOC_H