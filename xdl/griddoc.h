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

/************************************************Properties***************************************************************************/

/*
@PROPER style: string.
@GET get_grid_style_ptr: get the grid style.
*/
#define get_grid_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER style: string.
@SET set_grid_style: set the grid style.
*/
#define set_grid_style(ptr,token)							set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)
/*
@PROPER name: string.
@GET get_grid_name_ptr: get the grid name.
*/
#define get_grid_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_grid_name: set the grid name.
*/
#define set_grid_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET get_grid_id_ptr: get the grid identifier.
*/
#define get_grid_id_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_ID,-1)
/*
@PROPER id: string.
@SET set_grid_id: set the grid identifier.
*/
#define set_grid_id(ptr,token)								set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)
/*
@PROPER title: string.
@GET get_grid_title_ptr: get the grid title.
*/
#define get_grid_title_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_grid_title: set the grid title.
*/
#define set_grid_title(ptr,token)							set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER shape: string.
@GET get_grid_shape_ptr: get the grid shape.
*/
#define get_grid_shape_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_SHAPE,-1)
/*
@PROPER shape: string.
@SET set_grid_shape: set the grid shape.
*/
#define set_grid_shape(ptr,token)							set_dom_node_attr(ptr,ATTR_SHAPE,-1,token,-1)
/*
@PROPER showSum: boolean.
@GET get_grid_shape_ptr: get the grid is show sumary bar.
*/
#define get_grid_showsum(ptr)								get_dom_node_attr_boolean(ptr,ATTR_SHOWSUM)
/*
@PROPER showSum: boolean.
@SET set_grid_shape: set the grid is show sumary bar.
*/
#define set_grid_showsum(ptr,b)								set_dom_node_attr_boolean(ptr,ATTR_SHOWSUM,b)
/*
@PROPER showCheck: boolean.
@GET get_grid_showcheck: get the grid is show check box.
*/
#define get_grid_showcheck(ptr)								get_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK)
/*
@PROPER showCheck: boolean.
@SET set_grid_showcheck: set the grid is show check box.
*/
#define set_grid_showcheck(ptr,b)							set_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK,b)
/*
@PROPER stepDraw: boolean.
@GET get_grid_stepdraw_ptr: get the grid is step drawing.
*/
#define get_grid_stepdraw_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_STEPDRAW,-1)
/*
@PROPER stepDraw: boolean.
@SET set_grid_stepdraw: set the grid is step drawing.
*/
#define set_grid_stepdraw(ptr,token)						set_dom_node_attr(ptr,ATTR_STEPDRAW,-1,token,-1)
/*
@PROPER width: float.
@GET get_grid_width: get the grid width in millimeter.
*/
#define get_grid_width(ptr)									get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER width: float.
@SET set_grid_width: set the grid width in millimeter.
*/
#define set_grid_width(ptr,n)								set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER height: float.
@GET get_grid_height: get the grid height in millimeter.
*/
#define get_grid_height(ptr)								get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: float.
@SET set_grid_height: set the grid height in millimeter.
*/
#define set_grid_height(ptr,n)								set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER titleHeight: float.
@GET get_grid_title_height: get the grid title bar height in millimeter.
*/
#define get_grid_title_height(ptr)							get_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT)
/*
@PROPER titleHeight: float.
@SET set_grid_title_height: set the grid title bar height in millimeter.
*/
#define set_grid_title_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT,n)
/*
@PROPER rowBarWidth: float.
@GET get_grid_rowbar_width: get the grid row bar width in millimeter.
*/
#define get_grid_rowbar_width(ptr)							get_dom_node_attr_float(ptr,ATTR_ROWBAR_WIDTH)
/*
@PROPER rowBarWidth: float.
@SET set_grid_rowbar_width: set the grid row bar width in millimeter.
*/
#define set_grid_rowbar_width(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ROWBAR_WIDTH,n)
/*
@PROPER rowBarHeight: float.
@GET get_grid_rowbar_height: get the grid row bar height in millimeter.
*/
#define get_grid_rowbar_height(ptr)							get_dom_node_attr_float(ptr,ATTR_ROWBAR_HEIGHT)
/*
@PROPER rowBarHeight: float.
@SET set_grid_rowbar_height: set the grid row bar height in millimeter.
*/
#define set_grid_rowbar_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ROWBAR_HEIGHT,n)
/*
@PROPER colBarHeight: float.
@GET get_grid_colbar_height: get the grid col bar height in millimeter.
*/
#define get_grid_colbar_height(ptr)							get_dom_node_attr_float(ptr,ATTR_COLBAR_HEIGHT)
/*
@PROPER colBarHeight: float.
@SET set_grid_colbar_height: set the grid col bar height in millimeter.
*/
#define set_grid_colbar_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_COLBAR_HEIGHT,n)
/*
@PROPER subfield: integer.
@GET get_grid_subfield: get the grid subfield number.
*/
#define get_grid_subfield(ptr)								get_dom_node_attr_integer(ptr,ATTR_SUBFIELD)
/*
@PROPER subfield: integer.
@SET set_grid_subfield: set the grid subfield number.
*/
#define set_grid_subfield(ptr,n)							set_dom_node_attr_integer(ptr,ATTR_SUBFIELD,n)
/*
@PROPER updateTable: string.
@GET get_grid_update_table_ptr: get the grid update table.
*/
#define get_grid_update_table_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_UPDATE_TABLE,-1)
/*
@PROPER updateTable: string.
@SET set_grid_update_table: set the grid update table.
*/
#define set_grid_update_table(ptr,token)					set_dom_node_attr(ptr,ATTR_UPDATE_TABLE,-1,token,-1)
/*
@PROPER limitClause: string.
@GET get_grid_limit_clause_ptr: get the grid sql limit clause.
*/
#define get_grid_limit_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_LIMIT_CLAUSE,-1)
/*
@PROPER limitClause: string.
@SET set_grid_limit_clause: set the grid sql limit clause.
*/
#define set_grid_limit_clause(ptr,token)					set_dom_node_attr(ptr,ATTR_LIMIT_CLAUSE,-1,token,-1)
/*
@PROPER fromClause: string.
@GET get_grid_from_clause_ptr: get the grid sql from clause.
*/
#define get_grid_from_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_FROM_CLAUSE,-1)
/*
@PROPER fromClause: string.
@SET set_grid_from_clause: set the grid sql from clause.
*/
#define set_grid_from_clause(ptr,token)						set_dom_node_attr(ptr,ATTR_FROM_CLAUSE,-1,token,-1)
/*
@PROPER whereClause: string.
@GET get_grid_where_clause_ptr: get the grid sql where clause.
*/
#define get_grid_where_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_WHERE_CLAUSE,-1)
/*
@PROPER whereClause: string.
@SET set_grid_where_clause: set the grid sql where clause.
*/
#define set_grid_where_clause(ptr,token)					set_dom_node_attr(ptr,ATTR_WHERE_CLAUSE,-1,token,-1)
/*
@PROPER groupClause: string.
@GET get_grid_group_clause_ptr: get the grid sql group clause.
*/
#define get_grid_group_clause_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_GROUP_CLAUSE,-1)
/*
@PROPER groupClause: string.
@SET set_grid_group_clause: set the grid sql group clause.
*/
#define set_grid_group_clause(ptr,token)					set_dom_node_attr(ptr,ATTR_GROUP_CLAUSE,-1,token,-1)
/*
@PROPER id: string.
@GET set_col_id: get the grid col identifier.
*/
#define set_col_id(clk,val)									set_dom_node_attr(clk,ATTR_ID,-1,val,-1)
/*
@PROPER id: string.
@SET get_col_id_ptr: set the col indentifier.
*/
#define get_col_id_ptr(clk)									get_dom_node_attr_ptr(clk,ATTR_ID,-1)
/*
@PROPER id: string.
@GET set_col_name: get the col name.
*/
#define set_col_name(clk,val)								set_dom_node_attr(clk,ATTR_NAME,-1,val,-1)
/*
@PROPER id: string.
@SET get_col_name_ptr: set the col name.
*/
#define get_col_name_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_NAME,-1)
/*
@PROPER title: string.
@SET set_col_title: set the col title.
*/
#define set_col_title(clk,val)								set_dom_node_attr(clk,ATTR_TITLE,-1,val,-1)
/*
@PROPER title: string.
@GET get_col_title_ptr: get the col title.
*/
#define get_col_title_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_TITLE,-1)
/*
@PROPER style: string.
@GET get_col_style_ptr: get the col style.
*/
#define get_col_style_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_STYLE,-1)
/*
@PROPER style: string.
@SET set_col_style: set the col style.
*/
#define set_col_style(clk,token)							set_dom_node_attr(clk,ATTR_STYLE,-1,token,-1)
/*
@PROPER width: float.
@GET get_col_width: get the col width.
*/
#define get_col_width(clk)									get_dom_node_attr_float(clk,ATTR_WIDTH)
/*
@PROPER width: float.
@SET set_col_width: set the col width.
*/
#define set_col_width(clk,n)								set_dom_node_attr_float(clk,ATTR_WIDTH,n)
/*
@PROPER image: string.
@GET get_col_image_ptr: get the col image name.
*/
#define get_col_image_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_IMAGE,-1)
/*
@PROPER image: string.
@SET set_col_image: set the col image name.
*/
#define set_col_image(clk,token)							set_dom_node_attr(clk,ATTR_IMAGE,-1,token,-1)
/*
@PROPER alignment: string.
@GET set_col_alignment: get the col alignment.
*/
#define set_col_alignment(clk,val)							set_dom_node_attr(clk,ATTR_ALIGNMENT,-1,val,-1)
/*
@PROPER alignment: string.
@GET get_col_alignment_ptr: get the col alignment.
*/
#define get_col_alignment_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_ALIGNMENT,-1)
/*
@PROPER editor: string.
@SET set_col_editor: set the col editor.
*/
#define set_col_editor(clk,val)								set_dom_node_attr(clk,ATTR_EDITOR,-1,val,-1)
/*
@PROPER editor: string.
@GET get_col_editor_ptr: get the col editor.
*/
#define get_col_editor_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_EDITOR,-1)
/*
@PROPER dataType: string.
@SET set_col_data_type: set the col data type.
*/
#define set_col_data_type(clk,val)							set_dom_node_attr(clk,ATTR_DATA_TYPE,-1,val,-1)
/*
@PROPER dataType: string.
@GET get_col_data_type_ptr: get the col data type.
*/
#define get_col_data_type_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_DATA_TYPE,-1)
/*
@PROPER dataLen: integer.
@GET get_col_data_len_ptr: get the col data length.
*/
#define get_col_data_len(clk)								get_dom_node_attr_integer(clk,ATTR_DATA_LEN)
/*
@PROPER dataLen: integer.
@SET set_col_data_len: set the col data length.
*/
#define set_col_data_len(clk,n)								set_dom_node_attr_integer(clk,ATTR_DATA_LEN,n)
/*
@PROPER dataLen: integer.
@GET get_col_data_dig: get the col data decimal digits.
*/
#define get_col_data_dig(clk)								get_dom_node_attr_integer(clk,ATTR_DATA_DIG)
/*
@PROPER dataDig: integer.
@SET set_col_data_dig: set the col data decimal digits.
*/
#define set_col_data_dig(clk,n)								set_dom_node_attr_integer(clk,ATTR_DATA_DIG,n)
/*
@PROPER dataMin: string.
@SET set_col_data_min: set the col data minimized value.
*/
#define set_col_data_min(clk,val)							set_dom_node_attr(clk,ATTR_DATA_MIN,-1,val,-1)
/*
@PROPER dataMin: string.
@GET get_col_data_min_ptr: get the col data minimized value.
*/
#define get_col_data_min_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_DATA_MIN,-1)
/*
@PROPER dataMax: string.
@SET set_col_data_max: set the col data maximized value.
*/
#define set_col_data_max(clk,val)							set_dom_node_attr(clk,ATTR_DATA_MAX,-1,val,-1)
/*
@PROPER dataMax: string.
@GET get_col_data_max_ptr: get the col data maximized value.
*/
#define get_col_data_max_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_DATA_MAX,-1)
/*
@PROPER case: string.
@SET set_col_case: set the col casesentive type.
*/
#define set_col_case(clk,val)								set_dom_node_attr(clk,ATTR_CASE,-1,val,-1)
/*
@PROPER case: string.
@GET get_col_case_ptr: get the col casesentive type.
*/
#define get_col_case_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_CASE,-1)
/*
@PROPER format: string.
@SET set_col_format: set the col format.
*/
#define set_col_format(clk,val)								set_dom_node_attr(clk,ATTR_FORMAT,-1,val,-1)
/*
@PROPER format: string.
@GET get_col_format_ptr: get the col format type.
*/
#define get_col_format_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_FORMAT,-1)
/*
@PROPER macro: string.
@SET set_col_macro: set the col macro expression.
*/
#define set_col_macro(clk,val)								set_dom_node_attr(clk,ATTR_MACRO,-1,val,-1)
/*
@PROPER macro: string.
@GET get_col_macro_ptr: get the col macro expression.
*/
#define get_col_macro_ptr(clk)								get_dom_node_attr_ptr(clk,ATTR_MACRO,-1)
/*
@PROPER visible: boolean.
@GET get_col_visible: get the col is visible.
*/
#define get_col_visible(clk)								get_dom_node_attr_boolean(clk,ATTR_VISIBLE)
/*
@PROPER macro: boolean.
@SET set_col_visible: set the col is visible.
*/
#define set_col_visible(clk,b)								set_dom_node_attr_boolean(clk,ATTR_VISIBLE,b)
/*
@PROPER printable: boolean.
@GET get_col_printable: get the col is printable.
*/
#define get_col_printable(clk)								get_dom_node_attr_boolean(clk,ATTR_PRINTABLE)
/*
@PROPER printable: boolean.
@SET set_col_printable: set the col is printable.
*/
#define set_col_printable(clk,b)							set_dom_node_attr_boolean(clk,ATTR_PRINTABLE,b)
/*
@PROPER editable: boolean.
@GET get_col_editable: get the col is editable.
*/
#define get_col_editable(clk)								get_dom_node_attr_boolean(clk,ATTR_EDITABLE)
/*
@PROPER editable: boolean.
@SET set_col_editable: set the col is editable.
*/
#define set_col_editable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_EDITABLE,b)
/*
@PROPER sortable: boolean.
@GET get_col_sortable: get the col is sortable.
*/
#define get_col_sortable(clk)								get_dom_node_attr_boolean(clk,ATTR_SORTABLE)
/*
@PROPER sortable: boolean.
@SET set_col_sortable: set the col is sortable.
*/
#define set_col_sortable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_SORTABLE,b)
/*
@PROPER fireable: boolean.
@GET get_col_fireable: get the col is fireable.
*/
#define get_col_fireable(clk)								get_dom_node_attr_boolean(clk,ATTR_FIREABLE)
/*
@PROPER fireable: boolean.
@SET set_col_fireable: set the col is fireable.
*/
#define set_col_fireable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_FIREABLE,b)
/*
@PROPER nullable: boolean.
@GET get_col_nullable: get the col is nullable.
*/
#define get_col_nullable(clk)								get_dom_node_attr_boolean(clk,ATTR_NULLABLE)
/*
@PROPER nullable: boolean.
@SET set_col_nullable: set the col is nullable.
*/
#define set_col_nullable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_NULLABLE,b)
/*
@PROPER wrapable: boolean.
@GET get_col_wrapable: get the col is wrapable text drawing.
*/
#define get_col_wrapable(clk)								get_dom_node_attr_boolean(clk,ATTR_WRAPABLE)
/*
@PROPER wrapable: boolean.
@SET set_col_wrapable: set the col is wrapable text drawing.
*/
#define set_col_wrapable(clk,b)								set_dom_node_attr_boolean(clk,ATTR_WRAPABLE,b)
/*
@PROPER zeronull: boolean.
@GET get_col_zeronull: get the col is zero value as null.
*/
#define get_col_zeronull(clk)								get_dom_node_attr_boolean(clk,ATTR_ZERONULL)
/*
@PROPER zeronull: boolean.
@SET set_col_zeronull: set the col is zero value as null.
*/
#define set_col_zeronull(clk,b)								set_dom_node_attr_boolean(clk,ATTR_ZERONULL,b)
/*
@PROPER sumable: boolean.
@GET get_col_sumable: get the col data can be sumared.
*/
#define get_col_sumable(clk)								(is_null(get_dom_node_attr_ptr(clk,ATTR_SUM_MODE))? 0 : 1)
/*
@PROPER sumMode: string.
@SET set_col_sum_mode: set the col sumary mode.
*/
#define set_col_sum_mode(clk,val)							set_dom_node_attr(clk,ATTR_SUM_MODE,-1,val,-1)
/*
@PROPER sumMode: string.
@GET get_col_sum_mode_ptr: get the col sumary mode.
*/
#define get_col_sum_mode_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_SUM_MODE,-1)
/*
@PROPER delta: var_long.
@SET set_col_delta: set the col extract value.
*/
#define set_col_delta(clk,data)								set_dom_node_delta(clk,data)
/*
@PROPER delta: var_long.
@GET get_col_delta: get the col extract value.
*/
#define get_col_delta(clk)									get_dom_node_delta(clk)
/*
@PROPER options: string.
@SET set_col_options: set the col options token.
*/
#define set_col_options(clk,token,len)						set_dom_node_options(clk,token,len)

#define get_col_options_len(clk)							get_dom_node_options_len(clk)
/*
@PROPER options: string.
@GET get_col_options: get the col options token.
*/
#define get_col_options(clk,buf,max)						get_dom_node_options(clk,buf,max)

#define get_col_options_table(clk)							get_dom_node_options_table(clk)
/*
@PROPER updateable: boolean.
@GET get_col_updateable: get the col updateable.
*/
#define get_col_updateable(clk)								get_dom_node_attr_boolean(clk,ATTR_UPDATEABLE)
/*
@PROPER updateable: boolean.
@SET set_col_updateable: set the col updateable.
*/
#define set_col_updateable(clk,b)							set_dom_node_attr_boolean(clk,ATTR_UPDATEABLE,b)
/*
@PROPER fieldCast: string.
@SET set_col_field_cast: set the col field cast.
*/
#define set_col_field_cast(clk,val)							set_dom_node_attr(clk,ATTR_FIELD_CAST,-1,val,-1)
/*
@PROPER fieldCast: string.
@GET get_col_field_cast_ptr: get the col field cast.
*/
#define get_col_field_cast_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_FIELD_CAST,-1)
/*
@PROPER fieldType: string.
@SET set_col_field_type: set the col field type.
*/
#define set_col_field_type(clk,val)							set_dom_node_attr(clk,ATTR_FIELD_TYPE,-1,val,-1)
/*
@PROPER fieldType: string.
@GET get_col_field_type_ptr: get the col field type.
*/
#define get_col_field_type_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_FIELD_TYPE,-1)
/*
@PROPER fieldName: string.
@SET set_col_field_name: set the col field name.
*/
#define set_col_field_name(clk,val)							set_dom_node_attr(clk,ATTR_FIELD_NAME,-1,val,-1)
/*
@PROPER fieldName: string.
@GET get_col_field_name_ptr: get the col field name.
*/
#define get_col_field_name_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_FIELD_NAME,-1)
/*
@PROPER tableName: string.
@SET set_col_table_name: set the col table name.
*/
#define set_col_table_name(clk,val)							set_dom_node_attr(clk,ATTR_TABLE_NAME,-1,val,-1)
/*
@PROPER tableName: string.
@GET get_col_table_name_ptr: get the col table name.
*/
#define get_col_table_name_ptr(clk)							get_dom_node_attr_ptr(clk,ATTR_TABLE_NAME,-1)
/*
@PROPER delta: var_long.
@SET set_row_delta: set the col extract value.
*/
#define set_row_delta(rlk,data)								set_dom_node_delta(rlk,data)
/*
@PROPER delta: var_long.
@GET get_row_delta: get the col extract value.
*/
#define get_row_delta(rlk)									get_dom_node_delta(rlk)
/*
@PROPER visible: boolean.
@GET get_row_visible: get the row is visible.
*/
#define get_row_visible(rlk)								((get_row_hidden(rlk) || get_row_state(rlk) == dsDelete || get_row_state(rlk) == dsNewDelete)? 0 : 1)
/*
@PROPER image: string.
@SET set_row_image: set the row image.
*/
#define set_row_image(rlk,val)								set_dom_node_options_text(rlk,ATTR_IMAGE,val)
/*
@PROPER image: string.
@GET get_row_image_ptr: get the row image.
*/
#define get_row_image_ptr(rlk)								get_dom_node_options_text_ptr(rlk,ATTR_IMAGE,-1)
/*
@PROPER style: string.
@SET set_row_style: set the row style.
*/
#define set_row_style(rlk,val)								set_dom_node_options_text(rlk,ATTR_STYLE,-1,val,-1)
/*
@PROPER style: string.
@GET get_row_style_ptr: get the row style.
*/
#define get_row_style_ptr(rlk)								get_dom_node_options_text_ptr(rlk,ATTR_STYLE,-1)
/*
@PROPER text: string.
@SET set_row_text: set the row text.
*/
#define set_row_text(rlk,val)								set_dom_node_text(rlk,val,-1)
/*
@PROPER text: string.
@GET get_row_text_ptr: get the row text.
*/
#define get_row_text_ptr(rlk)								get_dom_node_text_ptr(rlk)

/*
@PROPER images: document.
@SET set_grid_images: set the grid images reference.
*/
#define set_grid_images(ptr,images)							set_dom_node_images(ptr,images)
/*
@PROPER images: document.
@GET get_grid_images: get the grid image reference.
*/
#define get_grid_images(ptr)								get_dom_node_images(ptr)
/*
@PROPER design: boolean.
@SET set_grid_images: set the grid is designing mode.
*/
#define set_grid_design(ptr,b)								set_dom_node_mask_check(ptr,MSK_DESIGN,b)
/*
@PROPER design: boolean.
@GET grid_is_design: get the grid is in designing mode.
*/
#define grid_is_design(ptr)									get_dom_node_mask_check(ptr,MSK_DESIGN)
/*
@PROPER selected: boolean.
@SET set_col_selected: set the col is selected.
*/
#define set_col_selected(clk,c)								set_dom_node_mask_check(clk,MSK_CHECKED,c) 
/*
@PROPER selected: boolean.
@GET get_col_selected: get the col is selected.
*/
#define get_col_selected(clk)								get_dom_node_mask_check(clk,MSK_CHECKED)
/*
@PROPER password: boolean.
@GET get_col_password: get the col is password.
*/
#define get_col_password(clk)								get_dom_node_mask_check(clk,MSK_PASSWORD)
/*
@PROPER password: boolean.
@SET set_col_password: set the col is password.
*/
#define set_col_password(clk,c)								set_dom_node_mask_check(clk,MSK_PASSWORD,c) 
/*
@PROPER state: integer.
@SET set_row_state: set the row state.
*/
#define set_row_state(rlk,c)								set_dom_node_mask(rlk,(get_dom_node_mask(rlk) & 0xFFFFFFF0) | c)
/*
@PROPER state: integer.
@GET get_row_state: get the row state.
*/
#define get_row_state(rlk)									(get_dom_node_mask(rlk) & 0x0000000F)
/*
@PROPER checked: boolean.
@SET set_row_checked: set the row checked.
*/
#define set_row_checked(rlk,c)								set_dom_node_mask_check(rlk,MSK_CHECKED,c) 
/*
@PROPER checked: boolean.
@GET get_row_checked: get the row checked.
*/
#define get_row_checked(rlk)								get_dom_node_mask_check(rlk,MSK_CHECKED)
/*
@PROPER locked: boolean.
@SET set_row_locked: set the row locked.
*/
#define set_row_locked(rlk,c)								set_dom_node_mask_check(rlk,MSK_LOCKED,c) 
/*
@PROPER locked: boolean.
@GET get_row_locked: get the row checked.
*/
#define get_row_locked(rlk)									get_dom_node_mask_check(rlk,MSK_LOCKED)
/*
@PROPER skipped: boolean.
@SET set_row_skipped: set the row skipped.
*/
#define set_row_skipped(rlk,c)								set_dom_node_mask_check(rlk,MSK_SKIPPED,c)
/*
@PROPER skipped: boolean.
@GET get_row_skipped: get the row skipped.
*/
#define get_row_skipped(rlk)								get_dom_node_mask_check(rlk,MSK_SKIPPED)
/*
@PROPER hidden: boolean.
@SET set_row_hidden: set the row hidden.
*/
#define set_row_hidden(rlk,c)								set_dom_node_mask_check(rlk,MSK_HIDDEN,c) 
/*
@PROPER hidden: boolean.
@GET get_row_hidden: get the row hidden.
*/
#define get_row_hidden(rlk)									get_dom_node_mask_check(rlk,MSK_HIDDEN)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_grid_doc: create a grid document.
@RETURN link_t_ptr: return the grid document link component.
*/
XDL_API link_t_ptr create_grid_doc(void);

/*
@FUNCTION destroy_grid_doc: destroy a grid document.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN void: none.
*/
XDL_API void destroy_grid_doc(link_t_ptr ptr);

/*
@FUNCTION clear_grid_doc: clear the grid document.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN void: none.
*/
XDL_API void clear_grid_doc(link_t_ptr ptr);

/*
@FUNCTION is_grid_doc: test is the grid document.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN bool_t: return nonzero for grid document, otherwise return zero.
*/
XDL_API bool_t is_grid_doc(link_t_ptr ptr);

/*
@FUNCTION is_grid_col: test is a grid col node.
@INPUT link_t_ptr ptr: the grid document link component.
@INPUT link_t_ptr clk: the col node link component.
@RETURN bool_t: return nonzero for col node, otherwise return zero.
*/
XDL_API bool_t is_grid_col(link_t_ptr ptr,link_t_ptr clk);

/*
@FUNCTION is_grid_row: test is a grid row node.
@INPUT link_t_ptr ptr: the grid document link component.
@INPUT link_t_ptr rlk: the row node link component.
@RETURN bool_t: return nonzero for row node, otherwise return zero.
*/
XDL_API bool_t is_grid_row(link_t_ptr ptr, link_t_ptr rlk);

/*
@FUNCTION clear_grid_rowset: clear grid rowset.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN void: none.
*/
XDL_API void clear_grid_rowset(link_t_ptr ptr);

/*
@FUNCTION clear_grid_colset: clear grid colset.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN void: none.
*/
XDL_API void clear_grid_colset(link_t_ptr ptr);

/*
@FUNCTION merge_grid_colset: merge source colset to destination colset.
@INPUT link_t_ptr ptr_dst: the destination grid document link component.
@INPUT link_t_ptr ptr_src: the source grid document link component.
@RETURN void: none.
*/
XDL_API void merge_grid_colset(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION merge_grid_rowset: merge source rowset to destination rowset.
@INPUT link_t_ptr ptr_dst: the destination grid document link component.
@INPUT link_t_ptr ptr_src: the source grid document link component.
@RETURN void: none.
*/
XDL_API void merge_grid_rowset(link_t_ptr ptr_dest, link_t_ptr ptr_src);

/*
@FUNCTION copy_grid_colsch: copy source colset schema to destination colset.
@INPUT link_t_ptr ptr_dst: the destination grid document link component.
@INPUT link_t_ptr ptr_src: the source grid document link component.
@RETURN void: none.
*/
XDL_API void copy_grid_colsch(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION get_col_count: counting the cols in grid.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN int: return the numner of cols.
*/
XDL_API int get_col_count(link_t_ptr ptr);

/*
@FUNCTION get_col_selected_count: counting the selected cols in grid.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN int: return the numner of cols.
*/
XDL_API int get_col_selected_count(link_t_ptr ptr);

/*
@FUNCTION get_grid_colset: get the colset link component.
@INPUT link_t_ptr ptr: the grid document link component.
@RETURN link_t_ptr: return the colset link component.
*/
XDL_API link_t_ptr get_grid_colset(link_t_ptr ptr);

/*
@FUNCTION insert_col: add a new col to grid at position.
@INPUT link_t_ptr ptr: the grid document link component.
@INPUT link_t_ptr pos: the col link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new col link component.
*/
XDL_API link_t_ptr insert_col(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_col: delete the col.
@INPUT link_t_ptr clk: the col link component.
@RETURN void: none.
*/
XDL_API void delete_col(link_t_ptr clk);

/*
@FUNCTION clear_col_datadef: clear the col data defination.
@INPUT link_t_ptr clk: the col link component.
@RETURN void: none.
*/
XDL_API void clear_col_datadef(link_t_ptr clk);

/*
@FUNCTION get_col: find the col by name.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* cname: the col name token.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_col(link_t_ptr ptr, const tchar_t* cname);

/*
@FUNCTION get_col_by_id: find the col by identifier.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* cid: the col identifier token.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_col_by_id(link_t_ptr ptr, const tchar_t* cid);

/*
@FUNCTION get_col_at: get the col at the position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_col_at(link_t_ptr ptr, int index);

/*
@FUNCTION get_next_col: get the next col at the position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the col link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_col(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_col: get the previous col at the position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the col link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_col(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_next_visible_col: get the next visible col at the position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the col link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_visible_col(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_visible_col: get the previous visible col at the position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the col link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_visible_col(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_next_focusable_col: get the next focusable col at the position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the col link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_focusable_col(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_prev_focusable_col: get the previous focusable col at the position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the col link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_focusable_col(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_col_focusable: get the col is focusable.
@INPUT link_t_ptr clk: the col link component.
@RETURN bool_t: return nonzero for being focusable col.
*/
XDL_API bool_t get_col_focusable(link_t_ptr clk);

/*
@FUNCTION get_grid_updateable: get the col is updateable.
@INPUT link_t_ptr clk: the col link component.
@RETURN bool_t: return nonzero for being updateable col.
*/
XDL_API bool_t get_grid_updateable(link_t_ptr ptr);

/*
@FUNCTION get_col_integrity: test the col is integrity.
@INPUT link_t_ptr clk: the col link component.
@RETURN bool_t: return nonzero for being integrity col.
*/
XDL_API bool_t get_col_integrity(link_t_ptr clk);

/*
@FUNCTION get_grid_rowset: get the grid rowset.
@INPUT link_t_ptr ptr: the grid link component.
@RETURN link_t_ptr: return grid rowset link component.
*/
XDL_API link_t_ptr get_grid_rowset(link_t_ptr ptr);

/*
@FUNCTION set_rowset_checked: set the rowset checked.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT bool_t b: nonzero for checked, zero for unchecked.
@RETURN void: return grid rowset link component.
*/
XDL_API void set_rowset_checked(link_t_ptr ptr, bool_t b);

/*
@FUNCTION get_rowset_checked: get the rowset checked.
@INPUT link_t_ptr ptr: the grid link component.
@RETURN bool_t: return nonzero for being rowset checked, zero for being rowset unchecked.
*/
XDL_API bool_t get_rowset_checked(link_t_ptr ptr);

/*
@FUNCTION insert_row: add a new row to grod at position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the row link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new row link component.
*/
XDL_API link_t_ptr insert_row(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_row: delete the row.
@INPUT link_t_ptr rlk: the row link component.
@RETURN void: none.
*/
XDL_API void delete_row(link_t_ptr rlk);

/*
@FUNCTION get_next_row: get the next row at position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the row link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the row link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_row(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_row: get the previous row at position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the row link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the row link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_row(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_next_visible_row: get the next visible row at position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the row link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the row link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_visible_row(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_visible_row: get the previsou visible row at position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr pos: the row link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the row link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_visible_row(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_row_count: counting the rows in grid.
@INPUT link_t_ptr ptr: the grid link component.
@RETURN int: return the number of rows.
*/
XDL_API int get_row_count(link_t_ptr ptr);

/*
@FUNCTION get_row_checked_count: counting the checked rows in grid.
@INPUT link_t_ptr ptr: the grid link component.
@RETURN int: return the number of rows.
*/
XDL_API int get_row_checked_count(link_t_ptr ptr);

/*
@FUNCTION get_row_storage_ptr: get the row storage token by key.
@INPUT link_t_ptr rlk: the row link component.
@INPUT const tchar_t* key: the key string token.
@RETURN const tchar_t*: return row storage value token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_row_storage_ptr(link_t_ptr rlk, const tchar_t* key);

/*
@FUNCTION set_row_storage: add a row storage.
@INPUT link_t_ptr rlk: the row link component.
@INPUT const tchar_t* key: the key string token.
@INPUT const tchar_t* val: the value string token.
@RETURN void: none.
*/
XDL_API void set_row_storage(link_t_ptr rlk, const tchar_t* key, const tchar_t* val);

/*
@FUNCTION get_cell_storage_ptr: get the cell storage token by key.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT const tchar_t* key: the key string token.
@RETURN const tchar_t*: return cell storage value token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_cell_storage_ptr(link_t_ptr rlk, link_t_ptr clk, const tchar_t* key);

/*
@FUNCTION set_cell_storage: add a cell storage.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT const tchar_t* key: the key string token.
@INPUT const tchar_t* val: the value string token.
@RETURN void: none.
*/
XDL_API void set_cell_storage(link_t_ptr rlk, link_t_ptr clk, const tchar_t* key, const tchar_t* val);

/*
@FUNCTION get_cell_prim_ptr: get the cell prim token.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN const tchar_t*: return cell prim token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_cell_prim_ptr(link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION set_cell_prim: set the cell prim value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT const tchar_t* str: the prim token.
@INPUT int len: the prim token length in characters.
@RETURN void: none.
*/
XDL_API void set_cell_prim(link_t_ptr rlk, link_t_ptr clk, const tchar_t* str, int len);

/*
@FUNCTION get_cell_options_text_ptr: get the cell options text.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN const tchar_t*: return the cell options text if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_cell_options_text_ptr(link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION attach_cell_text: attach a text buffer to cell.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT tchar_t* val: the value buffer.
@RETURN void: none.
*/
XDL_API void attach_cell_text(link_t_ptr rlk, link_t_ptr clk, tchar_t* val);

/*
@FUNCTION detach_cell_text: detach a text buffer from cell.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN tchar_t*: return the text buffer if exists, otherwise return NULL.
*/
XDL_API tchar_t* detach_cell_text(link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION get_cell_text_ptr: get the cell text token.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN tchar_t*: return the text token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_cell_text_ptr(link_t_ptr rlk,link_t_ptr clk);

/*
@FUNCTION get_cell_text: copy the cell text token.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copied.
*/
XDL_API int get_cell_text(link_t_ptr rlk, link_t_ptr clk, tchar_t* buf, int max);

/*
@FUNCTION set_cell_text: set the cell text token.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT const tchar_t* str: the text token.
@INPUT int len: the text length in characters
@RETURN void: none.
*/
XDL_API void set_cell_text(link_t_ptr rlk, link_t_ptr clk, const tchar_t* str, int len);

/*
@FUNCTION set_cell_boolean: set the cell boolean value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT bool_t b: the boolean value.
@RETURN void: none.
*/
XDL_API void set_cell_boolean(link_t_ptr rlk, link_t_ptr clk, bool_t b);

/*
@FUNCTION get_cell_boolean: get the cell boolean value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN bool_t: return cell boolean value.
*/
XDL_API bool_t get_cell_boolean(link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION set_cell_integer: set the cell integer value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT long n: the integer value.
@RETURN void: none.
*/
XDL_API void set_cell_integer(link_t_ptr rlk, link_t_ptr clk, long n);

/*
@FUNCTION get_cell_integer: get the cell integer value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN long: return cell integer value.
*/
XDL_API long get_cell_integer(link_t_ptr rlk,link_t_ptr clk);

/*
@FUNCTION set_cell_numeric: set the cell numeric value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT double dbl: the numeric value.
@RETURN void: none.
*/
XDL_API void set_cell_numeric(link_t_ptr rlk, link_t_ptr clk, double dbl);

/*
@FUNCTION get_cell_numeric: get the cell numeric value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN double: return cell numeric value.
*/
XDL_API double get_cell_numeric(link_t_ptr rlk,link_t_ptr clk);

/*
@FUNCTION set_cell_date: set the cell date value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT const xdate_t*: the date struct.
@RETURN void: none.
*/
XDL_API void set_cell_date(link_t_ptr rlk,link_t_ptr clk,const xdate_t* pdt);

/*
@FUNCTION get_cell_date: get the cell date value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@OUTPUT xdate_t*: the buffer of date struct.
@RETURN void: none.
*/
XDL_API void get_cell_date(link_t_ptr rlk,link_t_ptr clk,xdate_t* pdt);

/*
@FUNCTION set_cell_datetime: set the cell datetime value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT const xdate_t*: the date struct.
@RETURN void: none.
*/
XDL_API void set_cell_datetime(link_t_ptr rlk,link_t_ptr clk,const xdate_t* pdt);

/*
@FUNCTION get_cell_datetime: get the cell datetime value.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@OUTPUT xdate_t*: the buffer of date struct.
@RETURN void: none.
*/
XDL_API void get_cell_datetime(link_t_ptr rlk,link_t_ptr clk,xdate_t* pdt);

/*
@FUNCTION set_cell_dirty: set the cell state to dirty.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT bool_t b: nonzero for dirty state, zero for clean state.
@RETURN void: none.
*/
XDL_API void set_cell_dirty(link_t_ptr rlk, link_t_ptr clk, bool_t b);

/*
@FUNCTION get_cell_dirty: get the cell state.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN bool_t: return nonzero for dirty state, zero for clean state.
*/
XDL_API bool_t get_cell_dirty(link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION set_row_clean: set the row state to clean.
@INPUT link_t_ptr rlk: the row link component.
@RETURN void: none.
*/
XDL_API void set_row_clean(link_t_ptr rlk);

/*
@FUNCTION set_row_dirty: set the row state to dirty.
@INPUT link_t_ptr rlk: the row link component.
@RETURN void: none.
*/
XDL_API void set_row_dirty(link_t_ptr rlk);

/*
@FUNCTION set_row_delete: set the row state to delete.
@INPUT link_t_ptr rlk: the row link component.
@RETURN void: none.
*/
XDL_API void set_row_delete(link_t_ptr rlk);

/*
@FUNCTION get_visible_row_count: counting the visible rows.
@INPUT link_t_ptr ptr: the row link component.
@RETURN int: return the number of rows.
*/
XDL_API int get_visible_row_count(link_t_ptr ptr);

/*
@FUNCTION get_update_row_count: counting the update rows.
@INPUT link_t_ptr ptr: the row link component.
@RETURN int: return the number of rows.
*/
XDL_API int get_update_row_count(link_t_ptr ptr);

/*
@FUNCTION get_visible_row_at: get visible row at position.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the row link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_visible_row_at(link_t_ptr ptr, int index);

/*
@FUNCTION calc_grid_row: calcing the row.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@RETURN int: return the number of cells value changed.
*/
XDL_API int calc_grid_row(link_t_ptr ptr, link_t_ptr rlk);

/*
@FUNCTION calc_grid_rowset: calcing all of rows in grid.
@INPUT link_t_ptr ptr: the grid link component.
@RETURN int: return the number of rows changed.
*/
XDL_API int calc_grid_rowset(link_t_ptr ptr);

/*
@FUNCTION sum_grid_col: suming the col.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN int: return the number of rows sumed.
*/
XDL_API int sum_grid_col(link_t_ptr ptr, link_t_ptr clk);

/*
@FUNCTION sum_grid_colset: suming all of cols in grid.
@INPUT link_t_ptr ptr: the grid link component.
@RETURN int: return the number of cols sumed.
*/
XDL_API int sum_grid_colset(link_t_ptr ptr);

/*
@FUNCTION get_col_sum_text_ptr: get the col sumary text.
@INPUT link_t_ptr clk: the col link component.
@RETURN const tchar_t*: return the sumary text token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_col_sum_text_ptr(link_t_ptr clk);

/*
@FUNCTION set_col_sum_text: set the col sumary text.
@INPUT link_t_ptr clk: the col link component.
@INPUT const tchar_t* token: the col text token.
@RETURN void: none.
*/
XDL_API void set_col_sum_text(link_t_ptr clk,const tchar_t* token);

/*
@FUNCTION verify_grid_cell: verify the cell data,
the verify code returned can be veValid, veNull, veTruncate, veDatatype, veOverflow.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN int: return the verify code.
*/
XDL_API int verify_grid_cell(link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION verify_grid_doc: verify the grid data,
the verify code returned can be veValid, veNull, veTruncate, veDatatype, veOverflow.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr* prlk: link component buffer for returning row.
@INPUT link_t_ptr* pclk: link component buffer for returning col.
@RETURN int: return the verify code.
*/
XDL_API int verify_grid_doc(link_t_ptr ptr, link_t_ptr* prlk, link_t_ptr* pclk);

/*
@FUNCTION format_grid_verify: verify the cell data, and format the wraning text.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT int code: the verify code.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_grid_verify(link_t_ptr ptr, link_t_ptr rlk, link_t_ptr clk, int code, tchar_t* buf, int max);

/*
@FUNCTION sort_grid_col: sorting rowset according the col.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN void: none.
*/
XDL_API void sort_grid_col(link_t_ptr ptr, link_t_ptr clk);

/*
@FUNCTION refresh_grid_rowset: clear rowset dirty state.
@INPUT link_t_ptr ptr: the grid link component.
@RETURN void: none.
*/
XDL_API void refresh_grid_rowset(link_t_ptr ptr);

/*
@FUNCTION filter_grid_rowset: filter rowset by a expression.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* token: the expression token.
@RETURN void: none.
*/
XDL_API void filter_grid_rowset(link_t_ptr ptr, const tchar_t* token);

/*
@FUNCTION find_grid_row: find a row by expression.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* token: the expression token.
@INPUT link_t_ptr rlk: the start row link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the row link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr find_grid_row(link_t_ptr ptr, const tchar_t* token, link_t_ptr rlk, bool_t next);

/*
@FUNCTION copy_grid_row: copy source row content to destination row.
@INPUT link_t_ptr ptr_dst: the destination grid link component.
@INPUT link_t_ptr rlk_dst: the destination row link component.
@INPUT link_t_ptr ptr_src: the source grid link component.
@INPUT link_t_ptr rlk_src: the source row link component.
@RETURN void: none.
*/
XDL_API void copy_grid_row(link_t_ptr ptr_dst, link_t_ptr rlk_dst, link_t_ptr ptr_src, link_t_ptr rlk_src);

/*
@FUNCTION copy_grid_rowset: copy source rowset to destination rowset.
@INPUT link_t_ptr ptr_dst: the destination grid link component.
@INPUT link_t_ptr ptr_src: the source grid link component.
@RETURN void: none.
*/
XDL_API void copy_grid_rowset(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION update_grid_rowset: update source rowset to destination rowset.
@INPUT link_t_ptr ptr_dst: the destination grid link component.
@INPUT link_t_ptr ptr_src: the source grid link component.
@RETURN void: none.
*/
XDL_API void update_grid_rowset(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION make_grid_options: make grid options.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* keycol: the key col name.
@INPUT const tchar_t* valcol: the value col name.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int make_grid_options(link_t_ptr ptr, const tchar_t* keycol, const tchar_t*  valcol, tchar_t* buf, int max);

/*
@FUNCTION create_group_grid: create a group grid document.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* scol: the source col name.
@INPUT const tchar_t* dcol: the data title col name.
@INPUT const tchar_t* ncol: the data value col name.
@RETURN link_t_ptr: return the new grid link component.
*/
XDL_API link_t_ptr create_group_grid(link_t_ptr ptr, const tchar_t* scol, const tchar_t* dcol, const tchar_t* ncol);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*GRIDDOC_H*/