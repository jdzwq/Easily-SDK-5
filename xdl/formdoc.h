/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc form document

	@module	formdoc.h | interface file

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

#ifndef _FORMDOC_H
#define _FORMDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_form_name_ptr: get the form name.
*/
#define get_form_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_form_name: set the form name.
*/
#define set_form_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET get_form_id_ptr: get the form identifier.
*/
#define get_form_id_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_ID,-1)
/*
@PROPER id: string.
@SET set_form_id: set the form identifier.
*/
#define set_form_id(ptr,token)								set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)
/*
@PROPER title: string.
@GET get_form_id_ptr: get the form title.
*/
#define get_form_title_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_form_title: set the form title.
*/
#define set_form_title(ptr,token)							set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER style: string.
@GET get_form_style_ptr: get the form drawing style.
*/
#define get_form_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER style: string.
@SET set_form_style: set the form drawing style.
*/
#define set_form_style(ptr,token)							set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)
/*
@PROPER width: float.
@GET get_form_width: get the form width in millimeter.
*/
#define get_form_width(ptr)									get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER width: float.
@SET set_form_width: set the form width in millimeter.
*/
#define set_form_width(ptr,n)								set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER height: float.
@GET get_form_height: get the form height in millimeter.
*/
#define get_form_height(ptr)								get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: float.
@SET set_form_height: set the form height in millimeter.
*/
#define set_form_height(ptr,n)								set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER name: string.
@SET set_field_name: set the field name.
*/
#define set_field_name(flk,val)								set_dom_node_attr(flk,ATTR_NAME,-1,val,-1)
/*
@PROPER name: string.
@GET get_field_name_ptr: get the field name.
*/
#define get_field_name_ptr(flk)								get_dom_node_attr_ptr(flk,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_field_id: set the field identifier.
*/
#define set_field_id(flk,val)								set_dom_node_attr(flk,ATTR_ID,-1,val,-1)
/*
@PROPER id: string.
@GET get_field_id_ptr: get the field identifier.
*/
#define get_field_id_ptr(flk)								get_dom_node_attr_ptr(flk,ATTR_ID,-1)
/*
@PROPER macro: string.
@SET set_field_macro: set the field macro token.
*/
#define set_field_macro(flk,val)							set_dom_node_attr(flk,ATTR_MACRO,-1,val,-1)
/*
@PROPER macro: string.
@GET get_field_macro_ptr: get the field macro token.
*/
#define get_field_macro_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_MACRO,-1)
/*
@PROPER editor: string.
@SET set_field_editor: set the field editor type.
*/
#define set_field_editor(flk,val)							set_dom_node_attr(flk,ATTR_EDITOR,-1,val,-1)
/*
@PROPER editor: string.
@GET get_field_editor_ptr: get the field editor type.
*/
#define get_field_editor_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_EDITOR,-1)
/*
@PROPER codebar: string.
@SET set_field_codebar: set the field code bar type.
*/
#define set_field_codebar(flk,val)							set_dom_node_attr(flk,ATTR_CODEBAR,-1,val,-1)
/*
@PROPER codebar: string.
@GET get_field_codebar_ptr: get the field code bar type.
*/
#define get_field_codebar_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_CODEBAR,-1)
/*
@PROPER dataType: string.
@SET set_field_data_type: set the field data type.
*/
#define set_field_data_type(flk,val)						set_dom_node_attr(flk,ATTR_DATA_TYPE,-1,val,-1)
/*
@PROPER dataType: string.
@GET get_field_data_type_ptr: get the field data type.
*/
#define get_field_data_type_ptr(flk)						get_dom_node_attr_ptr(flk,ATTR_DATA_TYPE,-1)
/*
@PROPER dataLen: integer.
@GET get_field_data_len: get the field data size in characters.
*/
#define get_field_data_len(flk)								get_dom_node_attr_integer(flk,ATTR_DATA_LEN)
/*
@PROPER dataLen: integer.
@SET set_field_data_len: set the field data size in characters.
*/
#define set_field_data_len(flk,n)							set_dom_node_attr_integer(flk,ATTR_DATA_LEN,n)
/*
@PROPER dataDig: integer.
@GET get_field_data_dig: get the field data decimal digits.
*/
#define get_field_data_dig(flk)								get_dom_node_attr_integer(flk,ATTR_DATA_DIG)
/*
@PROPER dataDig: integer.
@SET set_field_data_dig: set the field data decimal digits.
*/
#define set_field_data_dig(flk,n)							set_dom_node_attr_integer(flk,ATTR_DATA_DIG,n)
/*
@PROPER dataMin: string.
@SET set_field_data_min: set the field data minimize value.
*/
#define set_field_data_min(flk,val)							set_dom_node_attr(flk,ATTR_DATA_MIN,-1,val,-1)
/*
@PROPER dataMin: string.
@GET get_field_data_min_ptr: get the field data minimaize value.
*/
#define get_field_data_min_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_DATA_MIN,-1)
/*
@PROPER dataMax: string.
@SET set_field_data_max: set the field data maximized value.
*/
#define set_field_data_max(flk,val)							set_dom_node_attr(flk,ATTR_DATA_MAX,-1,val,-1)
/*
@PROPER dataMax: string.
@GET get_field_data_max_ptr: get the field data maximized value.
*/
#define get_field_data_max_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_DATA_MAX,-1)
/*
@PROPER zeroNull: boolean.
@GET get_field_zeronull: get the field data is zero as null.
*/
#define get_field_zeronull(flk)								get_dom_node_attr_boolean(flk,ATTR_ZERONULL)
/*
@PROPER zeroNull: boolean.
@SET set_field_zeronull: set the field data is zero as null.
*/
#define set_field_zeronull(flk,b)							set_dom_node_attr_boolean(flk,ATTR_ZERONULL,b)
/*
@PROPER format: string.
@SET get_field_format_ptr: set the field data format.
*/
#define get_field_format_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_FORMAT,-1)
/*
@PROPER format: string.
@SET set_field_format: set the field data format.
*/
#define set_field_format(flk,token)							set_dom_node_attr(flk,ATTR_FORMAT,-1,token,-1)
/*
@PROPER case: string.
@SET set_field_case: set the field data case casesensitive.
*/
#define set_field_case(flk,val)								set_dom_node_attr(flk,ATTR_CASE,-1,val,-1)
/*
@PROPER case: string.
@GET get_field_case_ptr: get the field data casesensitive.
*/
#define get_field_case_ptr(flk)								get_dom_node_attr_ptr(flk,ATTR_CASE,-1)
/*
@PROPER tabOrder: integer.
@GET get_field_taborder: get the field tab order.
*/
#define get_field_taborder(flk)								get_dom_node_attr_integer(flk,ATTR_TABORDER)
/*
@PROPER tabOrder: integer.
@SET set_field_taborder: set the field tab order.
*/
#define set_field_taborder(flk,n)							set_dom_node_attr_integer(flk,ATTR_TABORDER,n)
/*
@PROPER value: string.
@SET set_field_value: set the field checked value.
*/
#define set_field_value(flk,val)							set_dom_node_attr(flk,ATTR_VALUE,-1,val,-1)
/*
@PROPER value: string.
@GET get_field_value_ptr: get the field checked value.
*/
#define get_field_value_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_VALUE,-1)
/*
@PROPER group: integer.
@GET get_field_group: get the field group identifier.
*/
#define get_field_group(flk)								get_dom_node_attr_integer(flk,ATTR_GROUP)
/*
@PROPER group: integer.
@SET set_field_group: set the field group identifier.
*/
#define set_field_group(flk,n)								set_dom_node_attr_integer(flk,ATTR_GROUP,n)
/*
@PROPER visible: boolean.
@GET get_field_visible: get the field is visible at runtime.
*/
#define get_field_visible(flk)								get_dom_node_attr_boolean(flk,ATTR_VISIBLE)
/*
@PROPER visible: boolean.
@SET set_field_group: set the field is visible at runtime.
*/
#define set_field_visible(flk,b)							set_dom_node_attr_boolean(flk,ATTR_VISIBLE,b)
/*
@PROPER transparent: boolean.
@GET get_field_transparent: get the field is transparent drawing.
*/
#define get_field_transparent(flk)							get_dom_node_attr_boolean(flk,ATTR_TRANSPARENT)
/*
@PROPER transparent: boolean.
@SET set_field_transparent: set the field is transparent drawing.
*/
#define set_field_transparent(flk,b)						set_dom_node_attr_boolean(flk,ATTR_TRANSPARENT,b)
/*
@PROPER editable: boolean.
@GET get_field_editable: get the field is editable.
*/
#define get_field_editable(flk)								get_dom_node_attr_boolean(flk,ATTR_EDITABLE)
/*
@PROPER editable: boolean.
@SET set_field_editable: set the field is editable.
*/
#define set_field_editable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_EDITABLE,b)
/*
@PROPER fireable: boolean.
@GET get_field_fireable: get the field is fireable.
*/
#define get_field_fireable(flk)								get_dom_node_attr_boolean(flk,ATTR_FIREABLE)
/*
@PROPER fireable: boolean.
@SET set_field_fireable: set the field is fireable.
*/
#define set_field_fireable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_FIREABLE,b)
/*
@PROPER nullable: boolean.
@GET get_field_nullable: get the field is nullable.
*/
#define get_field_nullable(flk)								get_dom_node_attr_boolean(flk,ATTR_NULLABLE)
/*
@PROPER nullable: boolean.
@SET set_field_nullable: set the field is nullable.
*/
#define set_field_nullable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_NULLABLE,b)
/*
@PROPER printable: boolean.
@GET get_field_printable: get the field is printable.
*/
#define get_field_printable(flk)							get_dom_node_attr_boolean(flk,ATTR_PRINTABLE)
/*
@PROPER printable: boolean.
@SET set_field_printable: set the field is printable.
*/
#define set_field_printable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_PRINTABLE,b)
/*
@PROPER wrapable: boolean.
@GET get_field_wrapable: get the field is wrapable drawing.
*/
#define get_field_wrapable(flk)								get_dom_node_attr_boolean(flk,ATTR_WRAPABLE)
/*
@PROPER wrapable: boolean.
@SET set_field_wrapable: set the field is wrapable drawing.
*/
#define set_field_wrapable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_WRAPABLE,b)
/*
@PROPER delta: var_long.
@SET set_field_delta: set the field extract value.
*/
#define set_field_delta(flk,data)							set_dom_node_delta(flk,data)
/*
@PROPER delta: var_long.
@GET get_field_delta: get the field extract value.
*/
#define get_field_delta(flk)								get_dom_node_delta(flk)
/*
@PROPER options: string.
@SET set_field_options: set the field options token.
*/
#define set_field_options(flk,token,len)					set_dom_node_options(flk,token,len)

#define get_field_options_len(flk)							get_dom_node_options_len(flk)
/*
@PROPER options: string.
@GET get_field_options: get the field options token.
*/
#define get_field_options(flk,buf,max)						get_dom_node_options(flk,buf,max)

#define get_field_options_table(flk)						get_dom_node_options_table(flk)
/*
@PROPER class: string.
@GET get_field_class_ptr: get the field class.
*/
#define get_field_class_ptr(flk)							get_dom_node_name_ptr(flk)
/*
@PROPER x: float.
@GET get_field_x: get the field x point in millimeter.
*/
#define get_field_x(flk)									get_dom_node_attr_float(flk,ATTR_X)
/*
@PROPER x: float.
@SET set_field_x: set the field x point in millimeter.
*/
#define set_field_x(flk,xm)									set_dom_node_attr_float(flk,ATTR_X,xm)
/*
@PROPER y: float.
@GET get_field_y: get the field y point in millimeter.
*/
#define get_field_y(flk)									get_dom_node_attr_float(flk,ATTR_Y)
/*
@PROPER y: float.
@SET set_field_y: set the field y point in millimeter.
*/
#define set_field_y(flk,ym)									set_dom_node_attr_float(flk,ATTR_Y,ym)
/*
@PROPER width: float.
@GET get_field_width: get the field width in millimeter.
*/
#define get_field_width(flk)								get_dom_node_attr_float(flk,ATTR_WIDTH)
/*
@PROPER width: float.
@SET set_field_width: set the field width in millimeter.
*/
#define set_field_width(flk,wm)								set_dom_node_attr_float(flk,ATTR_WIDTH,wm)
/*
@PROPER height: float.
@GET get_field_height: get the field height in millimeter.
*/
#define get_field_height(flk)								get_dom_node_attr_float(flk,ATTR_HEIGHT)
/*
@PROPER height: float.
@SET set_field_height: set the field height in millimeter.
*/
#define set_field_height(flk,ym)							set_dom_node_attr_float(flk,ATTR_HEIGHT,ym)
/*
@PROPER ratio: float.
@GET get_field_ratio: get the field ratio in millimeter.
*/
#define get_field_ratio(flk)								get_dom_node_attr_float(flk,ATTR_RATIO)
/*
@PROPER ratio: float.
@SET set_field_ratio: set the field ratio in millimeter.
*/
#define set_field_ratio(flk,ym)								set_dom_node_attr_float(flk,ATTR_RATIO,ym)
/*
@PROPER shape: string.
@GET get_field_shape_ptr: get the field shape type.
*/
#define get_field_shape_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_SHAPE,-1)
/*
@PROPER shape: string.
@SET set_field_shape: set the field shape type.
*/
#define set_field_shape(flk,token)							set_dom_node_attr(flk,ATTR_SHAPE,-1,token,-1)
/*
@PROPER style: string.
@GET get_field_style_ptr: get the field drawing style.
*/
#define get_field_style_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_STYLE,-1)
/*
@PROPER style: string.
@SET set_field_style: set the field drawing style.
*/
#define set_field_style(flk,token)							set_dom_node_attr(flk,ATTR_STYLE,-1,token,-1)
/*
@PROPER design: boolean.
@SET set_form_design: set the form is in designing mode.
*/
#define set_form_design(ptr,b)								set_dom_node_mask_check(ptr,MSK_DESIGN,b)
/*
@PROPER design: boolean.
@GET form_is_design: get the form is in designing mode.
*/
#define form_is_design(ptr)									get_dom_node_mask_check(ptr,MSK_DESIGN)
/*
@PROPER selected: boolean.
@SET set_field_selected: set the field is selected.
*/
#define set_field_selected(flk,b)							set_dom_node_mask_check(flk,MSK_CHECKED,b)
/*
@PROPER selected: boolean.
@GET get_field_selected: get the field is selected.
*/
#define get_field_selected(flk)								get_dom_node_mask_check(flk,MSK_CHECKED) 
/*
@PROPER hidden: boolean.
@SET set_field_hidden: set the field is hidden.
*/
#define set_field_hidden(flk,c)								set_dom_node_mask_check(flk,MSK_HIDDEN,c) 
/*
@PROPER hidden: boolean.
@GET get_field_hidden: get the field is hidden.
*/
#define get_field_hidden(flk)								get_dom_node_mask_check(flk,MSK_HIDDEN)
/*
@PROPER password: boolean.
@GET get_field_password: get the field is password.
*/
#define get_field_password(flk)								get_dom_node_mask_check(flk,MSK_PASSWORD)
/*
@PROPER password: boolean.
@SET set_field_password: set the field is password.
*/
#define set_field_password(flk,c)							set_dom_node_mask_check(flk,MSK_PASSWORD,c) 
/*
@PROPER dirty: boolean.
@SET set_field_dirty: set the field is dirty.
*/
#define set_field_dirty(flk,c)								set_dom_node_mask(flk,(get_dom_node_mask(flk) & 0xFFFFFFF0) | c)
/*
@PROPER dirty: boolean.
@GET get_field_dirty: get the field is dirty.
*/
#define get_field_dirty(flk)								(get_dom_node_mask(flk) & 0x0000000F)


#define IS_STATIC_FIELD(token)	(compare_text(token,-1,DOC_FORM_SHAPE,-1,0) == 0 || compare_text(token,-1,DOC_FORM_LABEL,-1,0) == 0)
#define IS_DATA_FIELD(token)	(compare_text(token,-1,DOC_FORM_TEXT,-1,0) == 0 || compare_text(token,-1,DOC_FORM_CHECK,-1,0) == 0  ||  compare_text(token,-1,DOC_FORM_TAG,-1,0) == 0 || compare_text(token,-1,DOC_FORM_MEMO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_PHOTO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_HREF,-1,0) == 0 || compare_text(token,-1,DOC_FORM_TABLE,-1,0) == 0)
#define IS_EMBED_FIELD(token)	(compare_text(token,-1,DOC_FORM_FORM,-1,0) == 0 || compare_text(token,-1,DOC_FORM_GRID,-1,0) == 0 || compare_text(token,-1,DOC_FORM_STATIS,-1,0) == 0  ||  compare_text(token,-1,DOC_FORM_IMAGES,-1,0) == 0 || compare_text(token,-1,DOC_FORM_RICH,-1,0) == 0 )
#define IS_PAGED_FIELD(token)	(compare_text(token,-1,DOC_FORM_GRID,-1,0) == 0 || compare_text(token,-1,DOC_FORM_STATIS,-1,0) == 0  || compare_text(token,-1,DOC_FORM_RICH,-1,0) == 0 ||  compare_text(token,-1,DOC_FORM_MEMO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_FORM,-1,0) == 0)
#define IS_LARGE_FIELD(token)	(compare_text(token,-1,DOC_FORM_PHOTO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_MEMO,-1,0) == 0)
#define IS_EDITOR_FIELD(token)	(compare_text(token,-1,DOC_FORM_SHAPE,-1,0) != 0 && compare_text(token,-1,DOC_FORM_LABEL,-1,0) != 0 && compare_text(token,-1,DOC_FORM_HREF,-1,0) != 0 && compare_text(token,-1,DOC_FORM_PHOTO,-1,0) != 0 && compare_text(token,-1,DOC_FORM_IMAGES,-1,0) != 0)
#define IS_AUTO_FIELD(token)	(compare_text(token,-1,DOC_FORM_FORM,-1,0) == 0 || compare_text(token,-1,DOC_FORM_GRID,-1,0) == 0 || compare_text(token,-1,DOC_FORM_STATIS,-1,0) == 0  || compare_text(token,-1,DOC_FORM_RICH,-1,0) == 0 ||  compare_text(token,-1,DOC_FORM_MEMO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_TAG,-1,0) == 0 || compare_text(token,-1,DOC_FORM_CHECK,-1,0) == 0)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_form_doc: create a form document.
@RETURN link_t_ptr: return the form document link component.
*/
XDL_API link_t_ptr create_form_doc(void);

/*
@FUNCTION destroy_form_doc: destroy a form document.
@INPUT link_t_ptr ptr: the form document link component.
@RETURN void: none.
*/
XDL_API void destroy_form_doc(link_t_ptr ptr);

/*
@FUNCTION clear_form_doc: clear a form document, all of field nodes in form document will be emptied.
@INPUT link_t_ptr ptr: the form document link component.
@RETURN void: none.
*/
XDL_API void clear_form_doc(link_t_ptr ptr);

/*
@FUNCTION get_form_fieldset: get a form fields set link component
@INPUT link_t_ptr ptr: the form document link component.
@RETURN link_t_ptr: return form fields set link component.
*/
XDL_API link_t_ptr get_form_fieldset(link_t_ptr ptr);

/*
@FUNCTION is_form_doc: test is a form document.
@INPUT link_t_ptr ptr: the form document link component.
@RETURN bool_t: return nonzero for being a form document, otherwise return zero.
*/
XDL_API bool_t is_form_doc(link_t_ptr ptr);

/*
@FUNCTION is_form_field: test is a form field node.
@INPUT link_t_ptr ptr: the form document link component.
@INPUT link_t_ptr nlk: the field link component.
@RETURN bool_t: return nonzero for being a field node, otherwise return zero.
*/
XDL_API bool_t is_form_field(link_t_ptr ptr,link_t_ptr flk);

/*
@FUNCTION form_from_field: trackback to form from a field node.
@INPUT link_t_ptr flk: the field link component.
@RETURN link_t_ptr: return form link component.
*/
XDL_API link_t_ptr form_from_field(link_t_ptr flk);

/*
@FUNCTION reset_form_doc: clear all of field value in form document.
@INPUT link_t_ptr ptr: the form link component.
@RETURN void: none.
*/
XDL_API void reset_form_doc(link_t_ptr ptr);

/*
@FUNCTION refresh_form_doc: rest all of field status to clean in form document.
@INPUT link_t_ptr ptr: the form link component.
@RETURN void: none.
*/
XDL_API void refresh_form_doc(link_t_ptr ptr);

/*
@FUNCTION insert_field: add a new field to form document.
@INPUT link_t_ptr ptr: the form link component.
@INPUT const tchar_t* sz_class: the field class name.
@RETURN link_t_ptr: return the field link component.
*/
XDL_API link_t_ptr insert_field(link_t_ptr ptr,const tchar_t* sz_class);

/*
@FUNCTION delete_field: delete a field.
@INPUT link_t_ptr flk: the field link component.
@RETURN void: none.
*/
XDL_API void delete_field(link_t_ptr flk);

/*
@FUNCTION get_field_count: counting the fields in form document.
@INPUT link_t_ptr ptr: the form link component.
@RETURN int: return the number of fields.
*/
XDL_API int get_field_count(link_t_ptr ptr);

/*
@FUNCTION get_field_count_by_class: counting the fields in form document by class.
@INPUT link_t_ptr ptr: the form link component.
@INPUT const tchar_t* sz_class
@RETURN int: return the number of fields.
*/
XDL_API int get_field_count_by_class(link_t_ptr ptr,const tchar_t* sz_class);

/*
@FUNCTION get_field_selected_count: counting the selected fields in form document.
@INPUT link_t_ptr ptr: the form link component.
@RETURN int: return the number of fields.
*/
XDL_API int get_field_selected_count(link_t_ptr ptr);

/*
@FUNCTION get_update_field_count: counting the dirty fields in form document.
@INPUT link_t_ptr ptr: the form link component.
@RETURN int: return the number of fields.
*/
XDL_API int get_update_field_count(link_t_ptr ptr);

/*
@FUNCTION get_field_max_group: calcing the maximized group identifier.
@INPUT link_t_ptr ptr: the form link component.
@RETURN int: return the maximized group identifier.
*/
XDL_API int get_field_max_group(link_t_ptr ptr);

/*
@FUNCTION get_next_field: get the next field by position.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr pos: the field link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_field(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_field: get the previous field by position.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr pos: the field link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_field(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_next_visible_field: get the next visible field by position.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr pos: the field link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_visible_field(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_visible_field: get the previous visible field by position.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr pos: the field link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_visible_field(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_next_focusable_field: get the next focusable field by position.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr pos: the field link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_focusable_field(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_prev_focusable_field: get the previous focusable field by position.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr pos: the field link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_focusable_field(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_field_focusable: get the field is focusable.
@INPUT link_t_ptr flk: the field link component.
@RETURN bool_t: return nonzero if the field is focusable, otherwise return zero.
*/
XDL_API bool_t get_field_focusable(link_t_ptr flk);

/*
@FUNCTION get_field: find a field by name.
@INPUT link_t_ptr ptr: the form link component.
@INPUT const tchar_t* fname: the name string token.
@RETURN link_t_ptr: return field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_field(link_t_ptr ptr,const tchar_t* fname);

/*
@FUNCTION get_field: find a field by identifier.
@INPUT link_t_ptr ptr: the form link component.
@INPUT const tchar_t* fid: the indetifier string token.
@RETURN link_t_ptr: return field link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_field_by_id(link_t_ptr ptr, const tchar_t* fid);

/*
@FUNCTION attach_field_text: attach a string buffer to field.
@INPUT link_t_ptr flk: the field link component.
@INPUT const tchar_t* val: the string buffer.
@RETURN void: none.
*/
XDL_API void attach_field_text(link_t_ptr flk, tchar_t* val);

/*
@FUNCTION detach_field_text: detach the string buffer from field.
@INPUT link_t_ptr flk: the field link component.
@RETURN tchar_t*: return the string buffer if exists, otherwise return NULL.
*/
XDL_API tchar_t* detach_field_text(link_t_ptr flk);

/*
@FUNCTION get_field_text_ptr: get the field text token.
@INPUT link_t_ptr flk: the field link component.
@RETURN tchar_t*: return the text token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_field_text_ptr(link_t_ptr flk);

/*
@FUNCTION get_field_text: copy the field text token.
@INPUT link_t_ptr flk: the field link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters copied.
*/
XDL_API int get_field_text(link_t_ptr flk, tchar_t* buf, int max);

/*
@FUNCTION set_field_text: set the field text value.
@INPUT link_t_ptr flk: the field link component.
@INPUT const tchar_t* str: the text value token.
@INPUT int len: the text value length in characters.
@RETURN void: none.
*/
XDL_API void set_field_text(link_t_ptr flk,const tchar_t* str,int len);

/*
@FUNCTION set_field_boolean: set the field boolean value.
@INPUT link_t_ptr flk: the field link component.
@INPUT bool_t b: the boolean value.
@RETURN void: none.
*/
XDL_API void set_field_boolean(link_t_ptr flk, bool_t n);

/*
@FUNCTION get_field_boolean: get the field boolean value.
@INPUT link_t_ptr flk: the field link component.
@RETURN bool_t: return field boolean value.
*/
XDL_API bool_t get_field_boolean(link_t_ptr flk);

/*
@FUNCTION set_field_integer: set the field integer value.
@INPUT link_t_ptr flk: the field link component.
@INPUT long n: the integer value.
@RETURN void: none.
*/
XDL_API void set_field_integer(link_t_ptr flk, long n);

/*
@FUNCTION get_field_integer: get the field integer value.
@INPUT link_t_ptr flk: the field link component.
@RETURN long: return field integer value.
*/
XDL_API long get_field_integer(link_t_ptr flk);

/*
@FUNCTION set_field_numeric: set the field numeric value.
@INPUT link_t_ptr flk: the field link component.
@INPUT double dbl: the numeric value.
@RETURN void: none.
*/
XDL_API void set_field_numeric(link_t_ptr flk, double dbl);

/*
@FUNCTION get_field_numeric: get the field numeric value.
@INPUT link_t_ptr flk: the field link component.
@RETURN double: return field numeric value.
*/
XDL_API double get_field_numeric(link_t_ptr flk);

/*
@FUNCTION set_field_date: set the field date value.
@INPUT link_t_ptr flk: the field link component.
@INPUT const xdate_t* pdt: the date struct.
@RETURN void: none.
*/
XDL_API void set_field_date(link_t_ptr flk, const xdate_t* pdt);

/*
@FUNCTION get_field_date: get the field date value.
@INPUT link_t_ptr flk: the field link component.
@OUTPUT xdate_t* pdt: the date struct buffer.
@RETURN void: none.
*/
XDL_API void get_field_date(link_t_ptr flk, xdate_t* pdt);

/*
@FUNCTION set_field_datetime: set the field datetime value.
@INPUT link_t_ptr flk: the field link component.
@INPUT const xdate_t* pdt: the date struct.
@RETURN void: none.
*/
XDL_API void set_field_datetime(link_t_ptr flk, const xdate_t* pdt);

/*
@FUNCTION get_field_datetime: get the field datetime value.
@INPUT link_t_ptr flk: the field link component.
@OUTPUT xdate_t* pdt: the date struct buffer.
@RETURN void: none.
*/
XDL_API void get_field_datetime(link_t_ptr flk,xdate_t* pdt);

/*
@FUNCTION get_field_options_text_ptr: get the field option text marching to field value.
@INPUT link_t_ptr flk: the field link component.
@RETURN const tchar_t*: return the option token if marched, otherwise return NULL.
*/
XDL_API const tchar_t* get_field_options_text_ptr(link_t_ptr flk);

/*
@FUNCTION get_field_embed_rich: get the field embed rich document.
@INPUT link_t_ptr flk: the field link component.
@RETURN link_t_ptr: return the rich link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_field_embed_rich(link_t_ptr flk);

/*
@FUNCTION set_field_embed_rich: set the field embed rich document.
@INPUT link_t_ptr flk: the field link component.
@INPUT link_t_ptr doc: the rich link componet.
@RETURN void: none.
*/
XDL_API void set_field_embed_rich(link_t_ptr flk, link_t_ptr doc);

/*
@FUNCTION get_field_embed_grid: get the field embed grid document.
@INPUT link_t_ptr flk: the field link component.
@RETURN link_t_ptr: return the grid link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_field_embed_grid(link_t_ptr flk);

/*
@FUNCTION set_field_embed_grid: set the field embed grid document.
@INPUT link_t_ptr flk: the field link component.
@INPUT link_t_ptr doc: the grid link componet.
@RETURN void: none.
*/
XDL_API void set_field_embed_grid(link_t_ptr flk, link_t_ptr doc);

/*
@FUNCTION get_field_embed_statis: get the field embed statis document.
@INPUT link_t_ptr flk: the field link component.
@RETURN link_t_ptr: return the statis link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_field_embed_statis(link_t_ptr flk);

/*
@FUNCTION set_field_embed_statis: set the field embed statis document.
@INPUT link_t_ptr flk: the field link component.
@INPUT link_t_ptr doc: the statis link componet.
@RETURN void: none.
*/
XDL_API void set_field_embed_statis(link_t_ptr flk, link_t_ptr doc);

/*
@FUNCTION get_field_embed_images: get the field embed images document.
@INPUT link_t_ptr flk: the field link component.
@RETURN link_t_ptr: return the images link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_field_embed_images(link_t_ptr flk);

/*
@FUNCTION set_field_embed_images: set the field embed images document.
@INPUT link_t_ptr flk: the field link component.
@INPUT link_t_ptr doc: the images link componet.
@RETURN void: none.
*/
XDL_API void set_field_embed_images(link_t_ptr flk, link_t_ptr doc);

/*
@FUNCTION get_field_embed_form: get the field embed form document.
@INPUT link_t_ptr flk: the field link component.
@RETURN link_t_ptr: return the form link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_field_embed_form(link_t_ptr flk);

/*
@FUNCTION set_field_embed_form: set the field embed form document.
@INPUT link_t_ptr flk: the field link component.
@INPUT link_t_ptr doc: the form link componet.
@RETURN void: none.
*/
XDL_API void set_field_embed_form(link_t_ptr flk, link_t_ptr doc);

/*
@FUNCTION clear_field_embed: clear the field embed document.
@INPUT link_t_ptr flk: the field link component.
@RETURN void: none.
*/
XDL_API void clear_field_embed(link_t_ptr flk);

/*
@FUNCTION clear_field_datadef: clear the field data defination.
@INPUT link_t_ptr flk: the field link component.
@RETURN void: none.
*/
XDL_API void clear_field_datadef(link_t_ptr flk);

/*
@FUNCTION calc_form_field: calcing the field data.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr flk: the field link component.
@RETURN int: return the numner of fields with data changed.
*/
XDL_API int calc_form_field(link_t_ptr ptr,link_t_ptr flk);

/*
@FUNCTION calc_form_doc: calcing all of fields data in form.
@INPUT link_t_ptr ptr: the form link component.
@RETURN void: none.
*/
XDL_API int calc_form_doc(link_t_ptr ptr);

/*
@FUNCTION verify_form_field: verify the field data, 
the verify code returned can be veValid, veNull, veTruncate, veDatatype, veOverflow.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr flk: the field link component.
@RETURN int: return the verify code.
*/
XDL_API int verify_form_field(link_t_ptr ptr, link_t_ptr flk);

/*
@FUNCTION verify_form_field: verify the fields data in form,
the verify code returned can be veValid, veNull, veTruncate, veDatatype, veOverflow.
@INPUT link_t_ptr ptr: the form link component.
@OUTPUT link_t_ptr* plk: the link component for return the invalid data field.
@RETURN int: return the verify code.
*/
XDL_API int verify_form_doc(link_t_ptr ptr, link_t_ptr* plk);

/*
@FUNCTION format_form_verify: verify the fields data in form, and format the wraning text.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr Flk: the field link component.
@INPUT int code: the verify code.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_form_verify(link_t_ptr ptr, link_t_ptr flk, int code, tchar_t* buf, int max);

/*
@FUNCTION reset_form_taborder: recalc the fields taborder in form.
@INPUT link_t_ptr ptr: the form link component.
@RETURN void: none.
*/
XDL_API void reset_form_taborder(link_t_ptr ptr);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //FORMDOC_H
