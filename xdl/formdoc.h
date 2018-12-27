/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc form document

	@module	formdoc.h | form document interface file

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

/**************************************serial enable attributes**********************************************/
#define get_form_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_form_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_form_id_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_form_id(ptr,token)								set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)

#define get_form_title_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define set_form_title(ptr,token)							set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define get_form_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_form_style(ptr,token)							set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_form_width(ptr)									get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_form_width(ptr,n)								set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_form_height(ptr)								get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_form_height(ptr,n)								set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define set_field_name(flk,val)								set_dom_node_attr(flk,ATTR_NAME,-1,val,-1)

#define get_field_name_ptr(flk)								get_dom_node_attr_ptr(flk,ATTR_NAME,-1)

#define set_field_id(flk,val)								set_dom_node_attr(flk,ATTR_ID,-1,val,-1)

#define get_field_id_ptr(flk)								get_dom_node_attr_ptr(flk,ATTR_ID,-1)

#define set_field_macro(flk,val)							set_dom_node_attr(flk,ATTR_MACRO,-1,val,-1)

#define get_field_macro_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_MACRO,-1)

#define set_field_editor(flk,val)							set_dom_node_attr(flk,ATTR_EDITOR,-1,val,-1)

#define get_field_editor_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_EDITOR,-1)

#define set_field_codebar(flk,val)							set_dom_node_attr(flk,ATTR_CODEBAR,-1,val,-1)

#define get_field_codebar_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_CODEBAR,-1)

#define set_field_data_type(flk,val)						set_dom_node_attr(flk,ATTR_DATA_TYPE,-1,val,-1)

#define get_field_data_type_ptr(flk)						get_dom_node_attr_ptr(flk,ATTR_DATA_TYPE,-1)

#define get_field_data_len(flk)								get_dom_node_attr_integer(flk,ATTR_DATA_LEN)

#define set_field_data_len(flk,n)							set_dom_node_attr_integer(flk,ATTR_DATA_LEN,n)

#define get_field_data_dig(flk)								get_dom_node_attr_integer(flk,ATTR_DATA_DIG)

#define set_field_data_dig(flk,n)							set_dom_node_attr_integer(flk,ATTR_DATA_DIG,n)

#define set_field_data_min(flk,val)							set_dom_node_attr(flk,ATTR_DATA_MIN,-1,val,-1)

#define get_field_data_min_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_DATA_MIN,-1)

#define set_field_data_max(flk,val)							set_dom_node_attr(flk,ATTR_DATA_MAX,-1,val,-1)

#define get_field_data_max_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_DATA_MAX,-1)

#define get_field_zeronull(flk)								get_dom_node_attr_boolean(flk,ATTR_ZERONULL)

#define set_field_zeronull(flk,b)							set_dom_node_attr_boolean(flk,ATTR_ZERONULL,b)

#define set_field_case(flk,val)								set_dom_node_attr(flk,ATTR_CASE,-1,val,-1)

#define get_field_case_ptr(flk)								get_dom_node_attr_ptr(flk,ATTR_CASE,-1)

#define get_field_taborder(flk)								get_dom_node_attr_integer(flk,ATTR_TABORDER)

#define set_field_taborder(flk,n)							set_dom_node_attr_integer(flk,ATTR_TABORDER,n)

#define get_field_group(flk)								get_dom_node_attr_integer(flk,ATTR_GROUP)

#define set_field_group(flk,n)								set_dom_node_attr_integer(flk,ATTR_GROUP,n)

#define get_field_visible(flk)								get_dom_node_attr_boolean(flk,ATTR_VISIBLE)

#define set_field_visible(flk,b)							set_dom_node_attr_boolean(flk,ATTR_VISIBLE,b)

#define get_field_transparent(flk)							get_dom_node_attr_boolean(flk,ATTR_TRANSPARENT)

#define set_field_transparent(flk,b)						set_dom_node_attr_boolean(flk,ATTR_TRANSPARENT,b)

#define get_field_editable(flk)								get_dom_node_attr_boolean(flk,ATTR_EDITABLE)

#define set_field_editable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_EDITABLE,b)

#define get_field_fireable(flk)								get_dom_node_attr_boolean(flk,ATTR_FIREABLE)

#define set_field_fireable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_FIREABLE,b)

#define get_field_nullable(flk)								get_dom_node_attr_boolean(flk,ATTR_NULLABLE)

#define set_field_nullable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_NULLABLE,b)

#define get_field_printable(flk)							get_dom_node_attr_boolean(flk,ATTR_PRINTABLE)

#define set_field_printable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_PRINTABLE,b)

#define get_field_wrapable(flk)								get_dom_node_attr_boolean(flk,ATTR_WRAPABLE)

#define set_field_wrapable(flk,b)							set_dom_node_attr_boolean(flk,ATTR_WRAPABLE,b)

#define set_field_delta(flk,data)							set_dom_node_delta(flk,data)

#define get_field_delta(flk)								get_dom_node_delta(flk)

#define set_field_options(flk,token,len)					set_dom_node_options(flk,token,len)

#define get_field_options_len(flk)							get_dom_node_options_len(flk)

#define get_field_options(flk,buf,max)						get_dom_node_options(flk,buf,max)

#define get_field_options_table(flk)						get_dom_node_options_table(flk)

#define get_field_class_ptr(flk)							get_dom_node_name_ptr(flk)

#define get_field_x(flk)									get_dom_node_attr_float(flk,ATTR_X)

#define set_field_x(flk,xm)									set_dom_node_attr_float(flk,ATTR_X,xm)

#define get_field_y(flk)									get_dom_node_attr_float(flk,ATTR_Y)

#define set_field_y(flk,ym)									set_dom_node_attr_float(flk,ATTR_Y,ym)

#define get_field_width(flk)								get_dom_node_attr_float(flk,ATTR_WIDTH)

#define set_field_width(flk,wm)								set_dom_node_attr_float(flk,ATTR_WIDTH,wm)

#define get_field_height(flk)								get_dom_node_attr_float(flk,ATTR_HEIGHT)

#define set_field_height(flk,ym)							set_dom_node_attr_float(flk,ATTR_HEIGHT,ym)

#define get_field_ratio(flk)								get_dom_node_attr_float(flk,ATTR_RATIO)

#define set_field_ratio(flk,ym)								set_dom_node_attr_float(flk,ATTR_RATIO,ym)

#define get_field_shape_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_SHAPE,-1)

#define set_field_shape(flk,token)							set_dom_node_attr(flk,ATTR_SHAPE,-1,token,-1)

#define get_field_format_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_FORMAT,-1)

#define set_field_format(flk,token)							set_dom_node_attr(flk,ATTR_FORMAT,-1,token,-1)

#define get_field_style_ptr(flk)							get_dom_node_attr_ptr(flk,ATTR_STYLE,-1)

#define set_field_style(flk,token)							set_dom_node_attr(flk,ATTR_STYLE,-1,token,-1)

/*****************************************runtime enable attributes********************************************/
#define set_form_images(ptr,images)							set_dom_node_images(ptr,images)

#define get_form_images(ptr)								get_dom_node_images(ptr)

#define set_form_design(ptr,b)								set_dom_node_mask_check(ptr,MSK_DESIGN,b)

#define form_is_design(ptr)									get_dom_node_mask_check(ptr,MSK_DESIGN)

#define set_field_selected(flk,b)							set_dom_node_mask_check(flk,MSK_CHECKED,b)

#define get_field_selected(flk)								get_dom_node_mask_check(flk,MSK_CHECKED) 

#define set_field_hidden(flk,c)								set_dom_node_mask_check(flk,MSK_HIDDEN,c) 

#define get_field_hidden(flk)								get_dom_node_mask_check(flk,MSK_HIDDEN)

#define get_field_password(flk)								get_dom_node_mask_check(flk,MSK_PASSWORD)

#define set_field_password(flk,c)							set_dom_node_mask_check(flk,MSK_PASSWORD,c) 

#define set_field_dirty(flk,c)								set_dom_node_mask(flk,(get_dom_node_mask(flk) & 0xFFFFFFF0) | c)

#define get_field_dirty(flk)								(get_dom_node_mask(flk) & 0x0000000F)

/******************************************runtime function**************************************************************/

#define IS_STATIC_FIELD(token)	(compare_text(token,-1,DOC_FORM_SHAPE,-1,0) == 0 || compare_text(token,-1,DOC_FORM_LABEL,-1,0) == 0)
#define IS_DATA_FIELD(token)	(compare_text(token,-1,DOC_FORM_TEXT,-1,0) == 0 || compare_text(token,-1,DOC_FORM_CHECK,-1,0) == 0  ||  compare_text(token,-1,DOC_FORM_TAG,-1,0) == 0 || compare_text(token,-1,DOC_FORM_MEMO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_PHOTO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_HREF,-1,0) == 0 || compare_text(token,-1,DOC_FORM_TABLE,-1,0) == 0)
#define IS_EMBED_FIELD(token)	(compare_text(token,-1,DOC_FORM_FORM,-1,0) == 0 || compare_text(token,-1,DOC_FORM_GRID,-1,0) == 0 || compare_text(token,-1,DOC_FORM_GRAPH,-1,0) == 0  ||  compare_text(token,-1,DOC_FORM_IMAGES,-1,0) == 0 || compare_text(token,-1,DOC_FORM_RICH,-1,0) == 0 )
#define IS_PAGED_FIELD(token)	(compare_text(token,-1,DOC_FORM_GRID,-1,0) == 0 || compare_text(token,-1,DOC_FORM_GRAPH,-1,0) == 0  || compare_text(token,-1,DOC_FORM_RICH,-1,0) == 0 ||  compare_text(token,-1,DOC_FORM_MEMO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_FORM,-1,0) == 0)
#define IS_LARGE_FIELD(token)	(compare_text(token,-1,DOC_FORM_PHOTO,-1,0) == 0 || compare_text(token,-1,DOC_FORM_MEMO,-1,0) == 0)
#define IS_EDITOR_FIELD(token)	(compare_text(token,-1,DOC_FORM_SHAPE,-1,0) != 0 && compare_text(token,-1,DOC_FORM_LABEL,-1,0) != 0 && compare_text(token,-1,DOC_FORM_HREF,-1,0) != 0 && compare_text(token,-1,DOC_FORM_PHOTO,-1,0) != 0 && compare_text(token,-1,DOC_FORM_IMAGES,-1,0) != 0)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_form_doc(void);

XDL_API void destroy_form_doc(link_t_ptr ptr);

XDL_API void clear_form_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_form_fieldset(link_t_ptr ptr);

XDL_API bool_t is_form_doc(link_t_ptr ptr);

XDL_API bool_t is_form_field(link_t_ptr ptr,link_t_ptr flk);

XDL_API link_t_ptr form_from_field(link_t_ptr flk);

XDL_API void reset_form_doc(link_t_ptr ptr);

XDL_API void refresh_form_doc(link_t_ptr ptr);

XDL_API link_t_ptr insert_field(link_t_ptr ptr,const tchar_t* sz_class);

XDL_API void delete_field(link_t_ptr flk);

XDL_API int get_field_count(link_t_ptr ptr);

XDL_API int get_field_count_by_class(link_t_ptr ptr,const tchar_t* szClass);

XDL_API int get_field_selected_count(link_t_ptr ptr);

XDL_API int get_update_field_count(link_t_ptr ptr);

XDL_API int get_field_max_group(link_t_ptr ptr);

XDL_API link_t_ptr get_next_field(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_field(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_next_visible_field(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_visible_field(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_next_focusable_field(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_focusable_field(link_t_ptr ptr,link_t_ptr pos);

XDL_API bool_t get_field_focusable(link_t_ptr flk);

XDL_API link_t_ptr get_field(link_t_ptr ptr,const tchar_t* fname);

XDL_API link_t_ptr get_field_by_id(link_t_ptr ptr, const tchar_t* fid);

XDL_API void attach_field_text(link_t_ptr flk, tchar_t* val);

XDL_API tchar_t* detach_field_text(link_t_ptr flk);

XDL_API const tchar_t* get_field_text_ptr(link_t_ptr flk);

XDL_API int get_field_text(link_t_ptr flk, tchar_t* buf, int max);

XDL_API void set_field_text(link_t_ptr flk,const tchar_t* str,int len);

XDL_API void set_field_boolean(link_t_ptr flk,bool_t n);

XDL_API bool_t get_field_boolean(link_t_ptr flk);

XDL_API void set_field_integer(link_t_ptr flk,long n);

XDL_API long get_field_integer(link_t_ptr flk);

XDL_API void set_field_numeric(link_t_ptr flk,double dbl);

XDL_API double get_field_numeric(link_t_ptr flk);

XDL_API void set_field_date(link_t_ptr flk,const xdate_t* pdt);

XDL_API void get_field_date(link_t_ptr flk,xdate_t* pdt);

XDL_API void set_field_datetime(link_t_ptr flk,const xdate_t* pdt);

XDL_API void get_field_datetime(link_t_ptr flk,xdate_t* pdt);

XDL_API const tchar_t* get_field_options_text_ptr(link_t_ptr flk);

XDL_API link_t_ptr get_field_embed_rich(link_t_ptr flk);

XDL_API void set_field_embed_rich(link_t_ptr flk, link_t_ptr doc);

XDL_API link_t_ptr get_field_embed_grid(link_t_ptr flk);

XDL_API void set_field_embed_grid(link_t_ptr flk, link_t_ptr doc);

XDL_API link_t_ptr get_field_embed_graph(link_t_ptr flk);

XDL_API void set_field_embed_graph(link_t_ptr flk, link_t_ptr doc);

XDL_API link_t_ptr get_field_embed_images(link_t_ptr flk);

XDL_API void set_field_embed_images(link_t_ptr flk, link_t_ptr doc);

XDL_API link_t_ptr get_field_embed_form(link_t_ptr flk);

XDL_API void set_field_embed_form(link_t_ptr flk, link_t_ptr doc);

XDL_API void clear_field_embed(link_t_ptr flk);

XDL_API void clear_field_datadef(link_t_ptr flk);

XDL_API int calc_form_field(link_t_ptr ptr,link_t_ptr flk);

XDL_API int calc_form_doc(link_t_ptr ptr);

XDL_API int verify_form_field(link_t_ptr ptr, link_t_ptr flk);

XDL_API int verify_form_doc(link_t_ptr ptr,link_t_ptr* plk);

XDL_API int format_form_verify(link_t_ptr ptr, link_t_ptr flk, int code, tchar_t* buf, int max);

XDL_API void reset_form_taborder(link_t_ptr ptr);

#ifdef	__cplusplus
}
#endif

#endif //XDLVIEW

#endif //FORMDOC_H
