/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc grid document

	@module	griddoc.h | grid document interface file

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

#ifndef _GRIDDOC_H
#define _GRIDDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************serial enable attributes**************************************/
#define get_grid_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_grid_style(ptr,token)							set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_grid_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_grid_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_grid_id_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_grid_id(ptr,token)								set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)

#define get_grid_title_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define set_grid_title(ptr,token)							set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define get_grid_shape_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_SHAPE,-1)

#define set_grid_shape(ptr,token)							set_dom_node_attr(ptr,ATTR_SHAPE,-1,token,-1)

#define get_grid_showsum(ptr)								get_dom_node_attr_boolean(ptr,ATTR_SHOWSUM)

#define set_grid_showsum(ptr,b)								set_dom_node_attr_boolean(ptr,ATTR_SHOWSUM,b)

#define get_grid_showcheck(ptr)								get_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK)

#define set_grid_showcheck(ptr,b)							set_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK,b)

#define get_grid_stepdraw_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_STEPDRAW,-1)

#define set_grid_stepdraw(ptr,token)						set_dom_node_attr(ptr,ATTR_STEPDRAW,-1,token,-1)

#define get_grid_width(ptr)									get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_grid_width(ptr,n)								set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_grid_height(ptr)								get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_grid_height(ptr,n)								set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define get_grid_title_height(ptr)							get_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT)

#define set_grid_title_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT,n)

#define get_grid_rowbar_width(ptr)							get_dom_node_attr_float(ptr,ATTR_ROWBAR_WIDTH)

#define set_grid_rowbar_width(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ROWBAR_WIDTH,n)

#define get_grid_rowbar_height(ptr)							get_dom_node_attr_float(ptr,ATTR_ROWBAR_HEIGHT)

#define set_grid_rowbar_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ROWBAR_HEIGHT,n)

#define get_grid_colbar_height(ptr)							get_dom_node_attr_float(ptr,ATTR_COLBAR_HEIGHT)

#define set_grid_colbar_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_COLBAR_HEIGHT,n)

#define get_grid_subfield(ptr)								get_dom_node_attr_integer(ptr,ATTR_SUBFIELD)

#define set_grid_subfield(ptr,n)							set_dom_node_attr_integer(ptr,ATTR_SUBFIELD,n)

#define get_grid_update_table_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_UPDATE_TABLE,-1)

#define set_grid_update_table(ptr,token)					set_dom_node_attr(ptr,ATTR_UPDATE_TABLE,-1,token,-1)

#define get_grid_limit_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_LIMIT_CLAUSE,-1)

#define set_grid_limit_clause(ptr,token)					set_dom_node_attr(ptr,ATTR_LIMIT_CLAUSE,-1,token,-1)

#define get_grid_from_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_FROM_CLAUSE,-1)

#define set_grid_from_clause(ptr,token)						set_dom_node_attr(ptr,ATTR_FROM_CLAUSE,-1,token,-1)

#define get_grid_where_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_WHERE_CLAUSE,-1)

#define set_grid_where_clause(ptr,token)					set_dom_node_attr(ptr,ATTR_WHERE_CLAUSE,-1,token,-1)

#define get_grid_group_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_GROUP_CLAUSE,-1)

#define set_grid_group_clause(ptr,token)					set_dom_node_attr(ptr,ATTR_GROUP_CLAUSE,-1,token,-1)

#define set_col_id(clk,val)									set_dom_node_attr(clk,ATTR_ID,-1,val,-1)

#define get_col_id_ptr(clk)									get_dom_node_attr_ptr(clk,ATTR_ID,-1)

#define set_col_name(clk,val)								set_dom_node_attr(clk,ATTR_NAME,-1,val,-1)

#define get_col_name_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_NAME,-1)

#define set_col_title(clk,val)								set_dom_node_attr(clk,ATTR_TITLE,-1,val,-1)

#define get_col_title_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_TITLE,-1)

#define get_col_style_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_STYLE,-1)

#define set_col_style(clk,token)							set_dom_node_attr(clk,ATTR_STYLE,-1,token,-1)

#define get_col_width(clk)									get_dom_node_attr_float(clk,ATTR_WIDTH)

#define set_col_width(clk,n)								set_dom_node_attr_float(clk,ATTR_WIDTH,n)

#define get_col_image_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_IMAGE,-1)

#define set_col_image(clk,token)							set_dom_node_attr(clk,ATTR_IMAGE,-1,token,-1)

#define set_col_alignment(clk,val)							set_dom_node_attr(clk,ATTR_ALIGNMENT,-1,val,-1)

#define get_col_alignment_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_ALIGNMENT,-1)

#define set_col_editor(clk,val)								set_dom_node_attr(clk,ATTR_EDITOR,-1,val,-1)

#define get_col_editor_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_EDITOR,-1)

#define set_col_data_type(clk,val)							set_dom_node_attr(clk,ATTR_DATA_TYPE,-1,val,-1)

#define get_col_data_type_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_DATA_TYPE,-1)

#define get_col_data_len(clk)								get_dom_node_attr_integer(clk,ATTR_DATA_LEN)

#define set_col_data_len(clk,n)								set_dom_node_attr_integer(clk,ATTR_DATA_LEN,n)

#define get_col_data_dig(clk)								get_dom_node_attr_integer(clk,ATTR_DATA_DIG)

#define set_col_data_dig(clk,n)								set_dom_node_attr_integer(clk,ATTR_DATA_DIG,n)

#define set_col_data_min(clk,val)							set_dom_node_attr(clk,ATTR_DATA_MIN,-1,val,-1)

#define get_col_data_min_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_DATA_MIN,-1)

#define set_col_data_max(clk,val)							set_dom_node_attr(clk,ATTR_DATA_MAX,-1,val,-1)

#define get_col_data_max_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_DATA_MAX,-1)

#define set_col_case(clk,val)								set_dom_node_attr(clk,ATTR_CASE,-1,val,-1)

#define get_col_case_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_CASE,-1)

#define set_col_format(clk,val)								set_dom_node_attr(clk,ATTR_FORMAT,-1,val,-1)

#define get_col_format_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_FORMAT,-1)

#define set_col_macro(clk,val)								set_dom_node_attr(clk,ATTR_MACRO,-1,val,-1)

#define get_col_macro_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_MACRO,-1)

#define get_col_visible(clk)								get_dom_node_attr_boolean(clk,ATTR_VISIBLE)

#define set_col_visible(clk,b)								set_dom_node_attr_boolean(clk,ATTR_VISIBLE,b)

#define get_col_printable(clk)								get_dom_node_attr_boolean(clk,ATTR_PRINTABLE)

#define set_col_printable(clk,b)							set_dom_node_attr_boolean(clk,ATTR_PRINTABLE,b)

#define get_col_editable(clk)								get_dom_node_attr_boolean(clk,ATTR_EDITABLE)

#define set_col_editable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_EDITABLE,b)

#define get_col_sortable(clk)								get_dom_node_attr_boolean(clk,ATTR_SORTABLE)

#define set_col_sortable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_SORTABLE,b)

#define get_col_fireable(clk)								get_dom_node_attr_boolean(clk,ATTR_FIREABLE)

#define set_col_fireable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_FIREABLE,b)

#define get_col_nullable(clk)								get_dom_node_attr_boolean(clk,ATTR_NULLABLE)

#define set_col_nullable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_NULLABLE,b)

#define get_col_wrapable(clk)								get_dom_node_attr_boolean(clk,ATTR_WRAPABLE)

#define set_col_wrapable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_WRAPABLE,b)

#define get_col_zeronull(clk)								get_dom_node_attr_boolean(clk,ATTR_ZERONULL)

#define set_col_zeronull(clk,b)								set_dom_node_attr_boolean(clk,ATTR_ZERONULL,b)

#define get_col_sumable(clk)								(is_null(get_dom_node_attr_ptr(clk,ATTR_SUM_MODE))? 0 : 1)

#define set_col_sum_mode(clk,val)							set_dom_node_attr(clk,ATTR_SUM_MODE,-1,val,-1)

#define get_col_sum_mode_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_SUM_MODE,-1)

#define set_col_delta(clk,data)								set_dom_node_delta(clk,data)

#define get_col_delta(clk)									get_dom_node_delta(clk)

#define set_col_options(clk,token,len)						set_dom_node_options(clk,token,len)

#define get_col_options_len(clk)							get_dom_node_options_len(clk)

#define get_col_options(clk,buf,max)						get_dom_node_options(clk,buf,max)

#define get_col_options_table(clk)							get_dom_node_options_table(clk)

#define get_col_updateable(clk)								get_dom_node_attr_boolean(clk,ATTR_UPDATEABLE)

#define set_col_updateable(clk,b)							set_dom_node_attr_boolean(clk,ATTR_UPDATEABLE,b)

#define set_col_field_cast(clk,val)							set_dom_node_attr(clk,ATTR_FIELD_CAST,-1,val,-1)

#define get_col_field_cast_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_FIELD_CAST,-1)

#define set_col_field_type(clk,val)							set_dom_node_attr(clk,ATTR_FIELD_TYPE,-1,val,-1)

#define get_col_field_type_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_FIELD_TYPE,-1)

#define set_col_field_name(clk,val)							set_dom_node_attr(clk,ATTR_FIELD_NAME,-1,val,-1)

#define get_col_field_name_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_FIELD_NAME,-1)

#define set_col_table_name(clk,val)							set_dom_node_attr(clk,ATTR_TABLE_NAME,-1,val,-1)

#define get_col_table_name_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_TABLE_NAME,-1)

#define set_row_delta(rlk,data)								set_dom_node_delta(rlk,data)

#define get_row_delta(rlk)									get_dom_node_delta(rlk)

#define get_row_visible(rlk)								((get_row_hidden(rlk) || get_row_state(rlk) == dsDelete || get_row_state(rlk) == dsNewDelete)? 0 : 1)

#define set_row_image(rlk,val)								set_dom_node_options_text(rlk,ATTR_IMAGE,val)

#define get_row_image_ptr(rlk)								get_dom_node_options_text_ptr(rlk,ATTR_IMAGE,-1)

#define set_row_style(rlk,val)								set_dom_node_options_text(rlk,ATTR_STYLE,-1,val,-1)

#define get_row_style_ptr(rlk)								get_dom_node_options_text_ptr(rlk,ATTR_STYLE,-1)

#define set_row_text(rlk,val)								set_dom_node_text(rlk,val,-1)

#define get_row_text_ptr(rlk)								get_dom_node_text_ptr(rlk)

/*************************************************runtime enable attribtes************************************************/
#define set_grid_images(ptr,images)							set_dom_node_images(ptr,images)

#define get_grid_images(ptr)								get_dom_node_images(ptr)

#define set_grid_design(ptr,b)								set_dom_node_mask_check(ptr,MSK_DESIGN,b)

#define grid_is_design(ptr)									get_dom_node_mask_check(ptr,MSK_DESIGN)

#define set_col_selected(clk,c)								set_dom_node_mask_check(clk,MSK_CHECKED,c) 

#define get_col_selected(clk)								get_dom_node_mask_check(clk,MSK_CHECKED)

#define get_col_password(clk)								get_dom_node_mask_check(clk,MSK_PASSWORD)

#define set_col_password(clk,c)								set_dom_node_mask_check(clk,MSK_PASSWORD,c) 

#define set_row_state(rlk,c)								set_dom_node_mask(rlk,(get_dom_node_mask(rlk) & 0xFFFFFFF0) | c)

#define get_row_state(rlk)									(get_dom_node_mask(rlk) & 0x0000000F)

#define set_row_checked(rlk,c)								set_dom_node_mask_check(rlk,MSK_CHECKED,c) 

#define get_row_checked(rlk)								get_dom_node_mask_check(rlk,MSK_CHECKED)

#define set_row_locked(rlk,c)								set_dom_node_mask_check(rlk,MSK_LOCKED,c) 

#define get_row_locked(rlk)									get_dom_node_mask_check(rlk,MSK_LOCKED)

#define set_row_skipped(rlk,c)								set_dom_node_mask_check(rlk,MSK_SKIPPED,c)

#define get_row_skipped(rlk)								get_dom_node_mask_check(rlk,MSK_SKIPPED)

#define set_row_hidden(rlk,c)								set_dom_node_mask_check(rlk,MSK_HIDDEN,c) 

#define get_row_hidden(rlk)									get_dom_node_mask_check(rlk,MSK_HIDDEN)

/*************************************************runtime function**********************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_grid_doc(void);

XDL_API void destroy_grid_doc(link_t_ptr ptr);

XDL_API void clear_grid_doc(link_t_ptr ptr);

XDL_API bool_t is_grid_doc(link_t_ptr ptr);

XDL_API bool_t is_grid_col(link_t_ptr ptr,link_t_ptr clk);

XDL_API bool_t is_grid_row(link_t_ptr ptr, link_t_ptr rlk);

XDL_API void clear_grid_rowset(link_t_ptr ptr);

XDL_API void clear_grid_colset(link_t_ptr ptr);

XDL_API void merge_grid_colset(link_t_ptr ptr_dest,link_t_ptr ptr_src);

XDL_API void merge_grid_rowset(link_t_ptr ptr_dest,link_t_ptr ptr_src);

XDL_API void copy_grid_colsch(link_t_ptr ptr_desc, link_t_ptr ptr_src);

XDL_API int get_col_count(link_t_ptr ptr);

XDL_API int get_col_selected_count(link_t_ptr ptr);

XDL_API link_t_ptr get_grid_colset(link_t_ptr ptr);

XDL_API link_t_ptr insert_col(link_t_ptr ptr,link_t_ptr pos);

XDL_API void delete_col(link_t_ptr clk);

XDL_API void clear_col_datadef(link_t_ptr clk);

XDL_API link_t_ptr get_col(link_t_ptr ptr,const tchar_t* cname);

XDL_API link_t_ptr get_col_by_id(link_t_ptr ptr, const tchar_t* cid);

XDL_API link_t_ptr get_col_at(link_t_ptr ptr, int index);

XDL_API link_t_ptr get_next_col(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_col(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_next_visible_col(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_visible_col(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_next_focusable_col(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_focusable_col(link_t_ptr ptr,link_t_ptr pos);

XDL_API bool_t get_col_focusable(link_t_ptr clk);

XDL_API bool_t get_grid_updateable(link_t_ptr ptr);

XDL_API bool_t get_col_integrity(link_t_ptr clk);

XDL_API link_t_ptr get_grid_rowset(link_t_ptr ptr);

XDL_API void set_rowset_checked(link_t_ptr ptr, bool_t b);

XDL_API bool_t get_rowset_checked(link_t_ptr ptr);

XDL_API link_t_ptr insert_row(link_t_ptr ptr,link_t_ptr pos);

XDL_API void delete_row(link_t_ptr rlk);

XDL_API link_t_ptr get_next_row(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_row(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_next_visible_row(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_visible_row(link_t_ptr ptr,link_t_ptr pos);

XDL_API int get_row_count(link_t_ptr ptr);

XDL_API int get_row_checked_count(link_t_ptr ptr);

XDL_API const tchar_t* get_row_storage_ptr(link_t_ptr rlk, const tchar_t* key);

XDL_API void set_row_storage(link_t_ptr rlk, const tchar_t* key, const tchar_t* val);

XDL_API const tchar_t* get_cell_storage_ptr(link_t_ptr rlk, link_t_ptr clk, const tchar_t* key);

XDL_API void set_cell_storage(link_t_ptr rlk, link_t_ptr clk, const tchar_t* key, const tchar_t* val);

XDL_API const tchar_t* get_cell_prim_ptr(link_t_ptr rlk,link_t_ptr clk);

XDL_API void set_cell_prim(link_t_ptr rlk,link_t_ptr clk,const tchar_t* str, int len);

XDL_API const tchar_t* get_cell_options_text_ptr(link_t_ptr rlk,link_t_ptr clk);

XDL_API void attach_cell_text(link_t_ptr rlk, link_t_ptr clk, tchar_t* val);

XDL_API tchar_t* detach_cell_text(link_t_ptr rlk, link_t_ptr clk);

XDL_API const tchar_t* get_cell_text_ptr(link_t_ptr rlk,link_t_ptr clk);

XDL_API int get_cell_text(link_t_ptr rlk,link_t_ptr clk,tchar_t* buf,int max);

XDL_API void set_cell_text(link_t_ptr rlk,link_t_ptr clk,const tchar_t* str,int len);

XDL_API void set_cell_boolean(link_t_ptr rlk,link_t_ptr clk,bool_t n);

XDL_API bool_t get_cell_boolean(link_t_ptr rlk,link_t_ptr clk);

XDL_API void set_cell_integer(link_t_ptr rlk,link_t_ptr clk,long n);

XDL_API long get_cell_integer(link_t_ptr rlk,link_t_ptr clk);

XDL_API void set_cell_numeric(link_t_ptr rlk,link_t_ptr clk,double dbl);

XDL_API double get_cell_numeric(link_t_ptr rlk,link_t_ptr clk);

XDL_API void set_cell_date(link_t_ptr rlk,link_t_ptr clk,const xdate_t* pdt);

XDL_API void get_cell_date(link_t_ptr rlk,link_t_ptr clk,xdate_t* pdt);

XDL_API void set_cell_datetime(link_t_ptr rlk,link_t_ptr clk,const xdate_t* pdt);

XDL_API void get_cell_datetime(link_t_ptr rlk,link_t_ptr clk,xdate_t* pdt);

XDL_API void set_cell_dirty(link_t_ptr rlk,link_t_ptr clk,bool_t b);

XDL_API bool_t get_cell_dirty(link_t_ptr rlk,link_t_ptr clk);

XDL_API void set_row_clean(link_t_ptr rlk);

XDL_API void set_row_dirty(link_t_ptr rlk);

XDL_API void set_row_delete(link_t_ptr rlk);

XDL_API int get_visible_row_count(link_t_ptr ptr);

XDL_API int get_update_row_count(link_t_ptr ptr);

XDL_API link_t_ptr get_visible_row_at(link_t_ptr ptr,int index);

XDL_API int calc_grid_row(link_t_ptr ptr,link_t_ptr rlk);

XDL_API int calc_grid_rowset(link_t_ptr ptr);

XDL_API int sum_grid_col(link_t_ptr ptr,link_t_ptr clk);

XDL_API int sum_grid_colset(link_t_ptr ptr);

XDL_API const tchar_t* get_col_sum_text_ptr(link_t_ptr clk);

XDL_API void set_col_sum_text(link_t_ptr clk,const tchar_t* token);

XDL_API bool_t verify_grid_cell(link_t_ptr rlk,link_t_ptr clk);

XDL_API bool_t verify_grid_doc(link_t_ptr ptr,link_t_ptr* prlk,link_t_ptr* pclk);

XDL_API int format_grid_verify(link_t_ptr ptr, link_t_ptr rlk, link_t_ptr clk, int code, tchar_t* buf, int max);

XDL_API void sort_grid_col(link_t_ptr ptr,link_t_ptr clk);

XDL_API void refresh_grid_rowset(link_t_ptr ptr);

XDL_API void filter_grid_rowset(link_t_ptr ptr,const tchar_t* token);

XDL_API link_t_ptr find_grid_row(link_t_ptr ptr,const tchar_t* token,link_t_ptr rlk,bool_t next);

XDL_API void copy_grid_row(link_t_ptr ptr_dest,link_t_ptr rlk_dest,link_t_ptr ptr_src,link_t_ptr rlk_src);

XDL_API void copy_grid_rowset(link_t_ptr ptr_dest, link_t_ptr ptr_src);

XDL_API void update_grid_rowset(link_t_ptr ptr_dest, link_t_ptr rlk_dest);

XDL_API int make_grid_options(link_t_ptr ptr,const tchar_t* keycol,const tchar_t*  valcol,tchar_t* buf,int max);

XDL_API link_t_ptr create_group_grid(link_t_ptr ptr, const tchar_t* scol, const tchar_t* dcol, const tchar_t* ncol);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*GRIDDOC_H*/