/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc proper document

	@module	properdoc.h | interface file

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

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_proper_name_ptr: get the proper name.
*/
#define get_proper_name_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_proper_name: set the proper name.
*/
#define set_proper_name(ptr,token)					set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER title: string.
@GET get_proper_title_ptr: get the proper title.
*/
#define get_proper_title_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_proper_title: set the proper title.
*/
#define set_proper_title(ptr,token)					set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER style: string.
@SET set_proper_style: set the proper style.
*/
#define set_proper_style(ptr,lay)					set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)
/*
@PROPER style: string.
@GET get_proper_style_ptr: get the proper style.
*/
#define get_proper_style_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER shape: string.
@GET get_proper_shape_ptr: get the proper shape.
*/
#define get_proper_shape_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_SHAPE,-1)
/*
@PROPER shape: string.
@SET set_proper_shape: set the proper shape.
*/
#define set_proper_shape(ptr,token)					set_dom_node_attr(ptr,ATTR_SHAPE,-1,token,-1)
/*
@PROPER iconSpan: numeric.
@SET set_proper_icon_span: set the proper icon span.
*/
#define set_proper_icon_span(ptr,n)					set_dom_node_attr_float(ptr,GDI_ATTR_GIZMO_SPAN,n)
/*
@PROPER iconSpan: numeric.
@GET get_proper_icon_span: get the proper icon span.
*/
#define get_proper_icon_span(ptr)					get_dom_node_attr_float(ptr,GDI_ATTR_GIZMO_SPAN)
/*
@PROPER itemSpan: numeric.
@SET set_proper_item_span: set the proper item span.
*/
#define set_proper_item_span(ptr,n)					set_dom_node_attr_float(ptr,ATTR_ITEM_SPAN,n)
/*
@PROPER itemSpan: numeric.
@GET get_proper_item_span: get the proper item span.
*/
#define get_proper_item_span(ptr)					get_dom_node_attr_float(ptr,ATTR_ITEM_SPAN)
/*
@PROPER itemHeight: numeric.
@SET set_proper_item_height: set the proper item height.
*/
#define set_proper_item_height(ptr,n)				set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)
/*
@PROPER itemHeight: numeric.
@GET get_proper_item_height: get the proper item height.
*/
#define get_proper_item_height(ptr)					get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)
/*
@PROPER name: string.
@SET set_section_name: set the section name.
*/
#define set_section_name(sec,val,len)				set_dom_node_attr(sec,ATTR_NAME,-1,val,len)
/*
@PROPER name: string.
@GET get_section_name_ptr: get the section name.
*/
#define get_section_name_ptr(sec)					get_dom_node_attr_ptr(sec,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_section_id: set the section identifier.
*/
#define set_section_id(elk,token)					set_dom_node_attr(elk,ATTR_ID,token)
/*
@PROPER id: string.
@GET get_section_id_ptr: get the section identifier.
*/
#define get_section_id_ptr(elk)						get_dom_node_attr_ptr(elk,ATTR_ID,-1)
/*
@PROPER icon: string.
@GET get_section_icon_ptr: get the section icon name.
*/
#define get_section_icon_ptr(slk)					get_dom_node_attr_ptr(slk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_section_icon: set the section icon name.
*/
#define set_section_icon(slk,token)				set_dom_node_attr(slk,ATTR_ICON,-1,token,-1)
/*
@PROPER id: string.
@SET set_entity_id: set the entity identifier.
*/
#define set_entity_id(elk,token)					set_dom_node_attr(elk,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_entity_id_ptr: get the entity identifier.
*/
#define get_entity_id_ptr(elk)						get_dom_node_attr_ptr(elk,ATTR_ID,-1)
/*
@PROPER name: string.
@SET set_entity_name: set the entity name.
*/
#define set_entity_name(ent,val,len)				set_dom_node_attr(ent,ATTR_NAME,-1,val,len)
/*
@PROPER name: string.
@GET get_entity_name_ptr: get the entity name.
*/
#define get_entity_name_ptr(ent)					get_dom_node_attr_ptr(ent,ATTR_NAME,-1)
/*
@PROPER value: string.
@SET set_entity_value: set the entity value.
*/
#define set_entity_value(ent,val,len)				set_dom_node_attr(ent,ATTR_TITLE,-1,val,len)
/*
@PROPER value: string.
@GET get_entity_value_ptr: get the entity value.
*/
#define get_entity_value_ptr(ent)					get_dom_node_attr_ptr(ent,ATTR_TITLE,-1)

#define get_entity_value(ent,buf,len)				get_dom_node_attr(ent,ATTR_TITLE,-1,buf,len)

/*
@PROPER options: string.
@SET set_entity_options: set the entity options.
*/
#define set_entity_options(ent,token,len)			set_dom_node_options(ent,token,len)

#define get_entity_options_len(ent)					get_dom_node_options_len(ent)

#define get_entity_options(ent,buf,max)				get_dom_node_options(ent,buf,max)

#define get_entity_options_table(ent)				get_dom_node_options_table(ent)
/*
@PROPER editable: boolean.
@GET get_entity_editable: get the entity editable.
*/
#define get_entity_editable(ent)					get_dom_node_attr_boolean(ent,ATTR_EDITABLE)
/*
@PROPER editable: boolean.
@SET set_entity_editable: set the entity editable.
*/
#define set_entity_editable(ent,b)					set_dom_node_attr_boolean(ent,ATTR_EDITABLE,b)
/*
@PROPER editor: string.
@SET set_entity_editor: set the entity editor.
*/
#define set_entity_editor(ent,token)				set_dom_node_attr(ent,ATTR_EDITOR,-1,token,-1)
/*
@PROPER editor: string.
@GET get_entity_editor_ptr: get the entity editor.
*/
#define get_entity_editor_ptr(ent)					get_dom_node_attr_ptr(ent,ATTR_EDITOR,-1)
/*
@PROPER icon: string.
@GET get_entity_icon_ptr: get the entity icon.
*/
#define get_entity_icon_ptr(elk)					get_dom_node_attr_ptr(elk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_entity_icon: set the entity icon.
*/
#define set_entity_icon(elk,token)					set_dom_node_attr(elk,ATTR_ICON,-1,token,-1)
/*
@PROPER collapsed: boolean.
@SET set_section_collapsed: set the section is collapsed.
*/
#define set_section_collapsed(slk,b)					set_dom_node_mask_check(slk,MSK_COLLAPSED,b) 
/*
@PROPER collapsed: boolean.
@GET get_section_collapsed: get the section is collapsed.
*/
#define get_section_collapsed(slk)					get_dom_node_mask_check(slk,MSK_COLLAPSED)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_proper_doc: create a proper document.
@RETURN link_t_ptr: return the proper document link component.
*/
EXP_API link_t_ptr create_proper_doc(void);

/*
@FUNCTION destroy_proper_doc: destroy a proper document.
@INPUT link_t_ptr ptr: the proper link component.
@RETURN void: none.
*/
EXP_API void destroy_proper_doc(link_t_ptr ptr);

/*
@FUNCTION clear_proper_doc: clear the proper document.
@INPUT link_t_ptr ptr: the proper link component.
@RETURN void: none.
*/
EXP_API void clear_proper_doc(link_t_ptr ptr);

/*
@FUNCTION is_proper_doc: test is proper document.
@INPUT link_t_ptr ptr: the proper link component.
@RETURN bool_t: return nonzero for being a proper document, otherwise return zero.
*/
EXP_API bool_t is_proper_doc(link_t_ptr ptr);

/*
@FUNCTION is_proper_section: test is proper section node.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT link_t_ptr slk: the section link component.
@RETURN bool_t: return nonzero for being a section, otherwise return zero.
*/
EXP_API bool_t is_proper_section(link_t_ptr ptr, link_t_ptr slk);

/*
@FUNCTION is_proper_entity: test is proper entity node.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT link_t_ptr ilk: the entity link component.
@RETURN bool_t: return nonzero for being a entity, otherwise return zero.
*/
EXP_API bool_t is_proper_entity(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION get_section: get the proper section by name.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sname: the section name token.
@INPUT int len: the name token length in characters.
@RETURN link_t_ptr: return the section link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_section(link_t_ptr ptr, const tchar_t* sname, int len);

/*
@FUNCTION get_next_section: get the next proper section.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT link_t_ptr pos: the section link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the section link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_next_section(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_section: get the previous proper section.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT link_t_ptr pos: the section link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the section link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_prev_section(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_section: delete the proper section.
@INPUT link_t_ptr slk: the section link component.
@RETURN void: none.
*/
EXP_API void delete_section(link_t_ptr slk);

/*
@FUNCTION proper_doc_from_section: trackback to proper document from section node.
@INPUT link_t_ptr ilk: the section link component.
@RETURN link_t_ptr: return the proper link component.
*/
EXP_API link_t_ptr proper_doc_from_section(link_t_ptr slk);

/*
@FUNCTION get_entity: get the entity by name.
@INPUT link_t_ptr slk: the section link component.
@INPUT const tchar_t* ename: the entity name token.
@INPUT int len: the name token length in characters.
@RETURN link_t_ptr: return the entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_entity(link_t_ptr slk, const tchar_t* ename, int len);

/*
@FUNCTION get_next_entity: get the next entity.
@INPUT link_t_ptr slk: the section link component.
@INPUT link_t_ptr pos: the entity link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_next_entity(link_t_ptr slk, link_t_ptr pos);

/*
@FUNCTION get_prev_entity: get the previous entity.
@INPUT link_t_ptr slk: the section link component.
@INPUT link_t_ptr pos: the entity link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_prev_entity(link_t_ptr slk, link_t_ptr pos);

/*
@FUNCTION delete_entity: delete the entity.
@INPUT link_t_ptr ilk: the entity link component.
@RETURN void: none.
*/
EXP_API void delete_entity(link_t_ptr elk);

/*
@FUNCTION section_from_entity: trackback to section from entity node.
@INPUT link_t_ptr ilk: the entity link component.
@RETURN link_t_ptr: return the section link component.
*/
EXP_API link_t_ptr section_from_entity(link_t_ptr elk);

/*
@FUNCTION write_proper: write content to proper document.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT int seclen: the section name length in characters.
@INPUT const tchar_t* ent: the entity name token.
@INPUT int entlen: the entity name length in characters.
@INPUT const tchar_t* val: the value token.
@INPUT int vallen: the value length in characters.
@RETURN link_t_ptr: return the entity link component.
*/
EXP_API	link_t_ptr write_proper(link_t_ptr ptr, const tchar_t* sec, int seclen, const tchar_t* ent, int entlen, const tchar_t* val, int vallen);

/*
@FUNCTION read_proper: read content from proper document.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT int seclen: the section name length in characters.
@INPUT const tchar_t* ent: the entity name token.
@INPUT int entlen: the entity name length in characters.
@OUTPUT tchar_t* buf: the string buffer for returning value.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters readed.
*/
EXP_API int	read_proper(link_t_ptr ptr, const tchar_t* sec, int seclen, const tchar_t* ent, int entlen, tchar_t* buf, int max);

/*
@FUNCTION get_entity_options_text_ptr: get entity options text releative to entity value.
@INPUT link_t_ptr ent: the proper link component.
@RETURN const tchar_t*: return options text token if exists, otherwise return NULL.
*/
EXP_API const tchar_t* get_entity_options_text_ptr(link_t_ptr ent);

/*
@FUNCTION get_proper_ptr: find entity and return entity value token.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@RETURN const tchar_t*: return entity value token if exists, otherwise return NULL.
*/
EXP_API const tchar_t* get_proper_ptr(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent);

/*
@FUNCTION set_proper_boolean: set entity boolean value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@INPUT bool_t b: the boolean value.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr set_proper_boolean(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, bool_t b);

/*
@FUNCTION get_proper_boolean: find entity and return entity boolean value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@RETURN bool_t: return entity boolean value if exists, otherwise return zero.
*/
EXP_API bool_t get_proper_boolean(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent);

/*
@FUNCTION set_proper_integer: set entity integer value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@INPUT int n: the integer value.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr set_proper_integer(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, int n);

/*
@FUNCTION get_proper_integer: find entity and return entity integer value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@RETURN integer: return entity integer value if exists, otherwise return zero.
*/
EXP_API int get_proper_integer(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent);

/*
@FUNCTION set_proper_float: set entity float value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@INPUT float n: the float value.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr set_proper_float(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, float n);

/*
@FUNCTION get_proper_float: find entity and return entity float value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@RETURN float: return entity float value if exists, otherwise return zero.
*/
EXP_API float get_proper_float(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent);

/*
@FUNCTION set_proper_numeric: set entity double value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@INPUT double n: the double value.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr set_proper_numeric(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, double n);

/*
@FUNCTION get_proper_numeric: find entity and return entity double value.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* sec: the section name token.
@INPUT const tchar_t* ent: the entity name token.
@RETURN double: return entity double value if exists, otherwise return zero.
*/
EXP_API double get_proper_numeric(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //PROPERDOC_H