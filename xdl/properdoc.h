/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc proper document

	@module	properdoc.h | proper document interface file

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

#ifndef _PROPERDOC_H
#define _PROPERDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**************************************serial enable attributes*****************************************/
#define get_proper_name_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_proper_name(ptr,token)					set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_proper_title_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define set_proper_title(ptr,token)					set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define set_proper_style(ptr,lay)					set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)

#define get_proper_style_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define get_proper_shape_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_SHAPE,-1)

#define set_proper_shape(ptr,token)					set_dom_node_attr(ptr,ATTR_SHAPE,-1,token,-1)

#define set_proper_icon_span(ptr,n)					set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)

#define get_proper_icon_span(ptr)					get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)

#define set_proper_item_span(ptr,n)					set_dom_node_attr_float(ptr,ATTR_ITEM_SPAN,n)

#define get_proper_item_span(ptr)					get_dom_node_attr_float(ptr,ATTR_ITEM_SPAN)

#define set_proper_item_height(ptr,n)				set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)

#define get_proper_item_height(ptr)					get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)

#define set_section_name(sec,val,len)				set_dom_node_attr(sec,ATTR_NAME,-1,val,len)

#define get_section_name_ptr(sec)					get_dom_node_attr_ptr(sec,ATTR_NAME,-1)

#define set_section_id(elk,token)					set_dom_node_attr(elk,ATTR_ID,token)

#define get_section_id_ptr(elk)						get_dom_node_attr_ptr(elk,ATTR_ID,-1)

#define get_section_image_ptr(slk)					get_dom_node_attr_ptr(slk,ATTR_IMAGE,-1)

#define set_section_image(slk,token)				set_dom_node_attr(slk,ATTR_IMAGE,-1,token,-1)

#define set_entity_id(elk,token)					set_dom_node_attr(elk,ATTR_ID,-1,token,-1)

#define get_entity_id_ptr(elk)						get_dom_node_attr_ptr(elk,ATTR_ID,-1)

#define set_entity_name(ent,val,len)				set_dom_node_attr(ent,ATTR_NAME,-1,val,len)

#define get_entity_name_ptr(ent)					get_dom_node_attr_ptr(ent,ATTR_NAME,-1)

#define set_entity_value(ent,val,len)				set_dom_node_attr(ent,ATTR_TITLE,-1,val,len)

#define get_entity_value_ptr(ent)					get_dom_node_attr_ptr(ent,ATTR_TITLE,-1)

#define get_entity_value(ent,buf,len)				get_dom_node_attr(ent,ATTR_TITLE,-1,buf,len)

#define set_entity_options(ent,token,len)			set_dom_node_options(ent,token,len)

#define get_entity_options_len(ent)					get_dom_node_options_len(ent)

#define get_entity_options(ent,buf,max)				get_dom_node_options(ent,buf,max)

#define get_entity_options_table(ent)				get_dom_node_options_table(ent)

#define get_entity_editable(ent)					get_dom_node_attr_boolean(ent,ATTR_EDITABLE)

#define set_entity_editable(ent,b)					set_dom_node_attr_boolean(ent,ATTR_EDITABLE,b)

#define set_entity_editor(ent,token)				set_dom_node_attr(ent,ATTR_EDITOR,-1,token,-1)

#define get_entity_editor_ptr(ent)					get_dom_node_attr_ptr(ent,ATTR_EDITOR,-1)

#define get_entity_image_ptr(elk)					get_dom_node_attr_ptr(elk,ATTR_IMAGE,-1)

#define set_entity_image(elk,token)					set_dom_node_attr(elk,ATTR_IMAGE,-1,token,-1)

/*******************************************runtime enable attributes*************************************************/
#define set_proper_images(ptr,images)				set_dom_node_images(ptr,images)

#define get_proper_images(ptr)						get_dom_node_images(ptr)

#define set_section_expanded(slk,b)					set_dom_node_mask_check(slk,MSK_EXPANDED,b) 

#define get_section_expanded(slk)					get_dom_node_mask_check(slk,MSK_EXPANDED)
/*******************************************runtime function*********************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_proper_doc(void);

XDL_API void destroy_proper_doc(link_t_ptr ptr);

XDL_API void clear_proper_doc(link_t_ptr ptr);

XDL_API bool_t is_proper_doc(link_t_ptr ptr);

XDL_API bool_t is_proper_section(link_t_ptr ptr, link_t_ptr slk);

XDL_API bool_t is_proper_entity(link_t_ptr ptr, link_t_ptr ilk);

XDL_API link_t_ptr get_section(link_t_ptr ptr,const tchar_t* sz_name,int len);

XDL_API link_t_ptr get_next_section(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_section(link_t_ptr ptr,link_t_ptr pos);

XDL_API void delete_section(link_t_ptr slk);

XDL_API link_t_ptr get_proper_from_section(link_t_ptr slk);

XDL_API link_t_ptr get_entity(link_t_ptr slk,const tchar_t* sz_name,int len);

XDL_API link_t_ptr get_next_entity(link_t_ptr slk,link_t_ptr pos);

XDL_API link_t_ptr get_prev_entity(link_t_ptr slk,link_t_ptr pos);

XDL_API void delete_entity(link_t_ptr elk);

XDL_API link_t_ptr get_section_from_entity(link_t_ptr elk);

XDL_API	link_t_ptr write_proper(link_t_ptr ptr,const tchar_t* sec,int seclen,const tchar_t* ent,int entlen,const tchar_t* val,int vallen);

XDL_API int	read_proper(link_t_ptr ptr,const tchar_t* sec,int seclen,const tchar_t* ent,int entlen,tchar_t* buf,int max);

XDL_API const tchar_t* get_entity_options_text_ptr(link_t_ptr ent);

XDL_API const tchar_t* get_proper_ptr(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent);

XDL_API link_t_ptr set_proper_boolean(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, bool_t n);

XDL_API bool_t get_proper_boolean(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent);

XDL_API link_t_ptr set_proper_integer(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent,long n);

XDL_API long get_proper_integer(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent);

XDL_API link_t_ptr set_proper_float(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, float n);

XDL_API float get_proper_float(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent);

XDL_API link_t_ptr set_proper_numeric(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent,double n);

XDL_API double get_proper_numeric(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //PROPERDOC_H