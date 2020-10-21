/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc topog document

	@module	topogdoc.h | interface file

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

#ifndef _TOPOGDOC_H
#define _TOPOGDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_topog_name_ptr: get the topog name.
*/
#define get_topog_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_topog_name: set the topog name.
*/
#define set_topog_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET get_topog_id_ptr: get the topog identifier.
*/
#define get_topog_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_topog_id: set the topog identifier.
*/
#define set_topog_id(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER style: string.
@SET set_topog_style: set the topog drawing style.
*/
#define set_topog_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)
/*
@PROPER style: string.
@GET get_topog_id_ptr: get the topog drawing style.
*/
#define get_topog_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER width: numeric.
@SET set_topog_width: set the topog width.
*/
#define set_topog_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER width: numeric.
@GET get_topog_width: get the topog width.
*/
#define get_topog_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER height: numeric.
@GET get_topog_height: get the topog height.
*/
#define get_topog_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: numeric.
@SET set_topog_height: set the topog height.
*/
#define set_topog_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER rows: integer.
@GET get_topog_rows: get the topog rows.
*/
#define get_topog_rows(ptr)								get_dom_node_attr_integer(ptr,ATTR_ROWS)
/*
@PROPER rows: integer.
@SET set_topog_rows: set the topog rows.
*/
#define set_topog_rows(ptr,n)							set_dom_node_attr_integer(ptr,ATTR_ROWS,n)
/*
@PROPER cols: integer.
@GET get_topog_cols: get the topog cols.
*/
#define get_topog_cols(ptr)								get_dom_node_attr_integer(ptr,ATTR_COLS)
/*
@PROPER cols: integer.
@SET set_topog_cols: set the topog cols.
*/
#define set_topog_cols(ptr,n)							set_dom_node_attr_integer(ptr,ATTR_COLS,n)
/*
@PROPER rx: float.
@GET get_topog_rx: get the topog rx.
*/
#define get_topog_rx(ptr)								get_dom_node_attr_float(ptr,ATTR_RX)
/*
@PROPER rx: float.
@SET set_topog_rx: set the topog rx.
*/
#define set_topog_rx(ptr,n)								set_dom_node_attr_float(ptr,ATTR_RX,n)
/*
@PROPER ry: float.
@GET get_topog_ry: get the topog ry.
*/
#define get_topog_ry(ptr)								get_dom_node_attr_float(ptr,ATTR_RY)
/*
@PROPER ry: float.
@SET set_topog_ry: set the topog ry.
*/
#define set_topog_ry(ptr,n)								set_dom_node_attr_float(ptr,ATTR_RY,n)
/*
@PROPER matrix: string.
@SET set_topog_matrix: set the topog matrix.
*/
#define set_topog_matrix(ptr,token,len)					set_dom_node_attr(ptr,ATTR_MATRIX,-1,token,len)
/*
@PROPER matrix: string.
@GET get_topog_matrix_ptr: get the topog matrix.
*/
#define get_topog_matrix_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_MATRIX,-1)
/*
@PROPER name: string.
@GET get_topog_spot_name_ptr: get the spot name.
*/
#define get_topog_spot_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_topog_spot_name: set the spot name.
*/
#define set_topog_spot_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET get_topog_spot_id_ptr: get the spot identifier.
*/
#define get_topog_spot_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_topog_spot_id: set the spot identifier.
*/
#define set_topog_spot_id(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER type: string.
@GET get_topog_spot_type_ptr: get the spot type.
*/
#define get_topog_spot_type_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TYPE,-1)
/*
@PROPER type: string.
@SET set_topog_spot_type: set the spot type.
*/
#define set_topog_spot_type(ilk,token)					set_dom_node_attr(ilk,ATTR_TYPE,-1,token,-1)
/*
@PROPER title: string.
@GET get_topog_spot_title_ptr: get the spot title.
*/
#define get_topog_spot_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_topog_spot_title: set the spot title.
*/
#define set_topog_spot_title(ilk,token)					set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)
/*
@PROPER style: string.
@SET set_topog_spot_style: set the spot style.
*/
#define set_topog_spot_style(ilk,token)					set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)
/*
@PROPER style: string.
@GET get_topog_spot_style_ptr: get the spot style.
*/
#define get_topog_spot_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)
/*
@PROPER col: integer.
@GET get_topog_spot_col: get the spot col.
*/
#define get_topog_spot_col(ilk)							get_dom_node_attr_integer(ilk,ATTR_COL)
/*
@PROPER col: integer.
@SET set_topog_spot_col: set the spot col.
*/
#define set_topog_spot_col(ilk,col)						set_dom_node_attr_integer(ilk,ATTR_COL,col)
/*
@PROPER row: integer.
@GET get_topog_spot_row: get the spot row.
*/
#define get_topog_spot_row(ilk)							get_dom_node_attr_integer(ilk,ATTR_ROW)
/*
@PROPER row: integer.
@SET set_topog_spot_row: set the spot row.
*/
#define set_topog_spot_row(ilk,row)						set_dom_node_attr_integer(ilk,ATTR_ROW,row)

/*
@PROPER design: boolean.
@SET set_topog_design: set the topog is designing mode.
*/
#define set_topog_design(ptr,b)							set_dom_node_mask_check(ptr,MSK_DESIGN,b)
/*
@PROPER design: boolean.
@GET topog_is_design: get the topog is designing mode.
*/
#define topog_is_design(ptr)							get_dom_node_mask_check(ptr,MSK_DESIGN)
/*
@PROPER selected: boolean.
@SET set_topog_spot_selected: set the spot is selected.
*/
#define set_topog_spot_selected(ilk,b)					set_dom_node_mask_check(ilk,MSK_CHECKED,b)
/*
@PROPER selected: boolean.
@GET get_topog_spot_selected: get the spot is selected.
*/
#define get_topog_spot_selected(ilk)					get_dom_node_mask_check(ilk,MSK_CHECKED) 


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_topog_doc: create a topog document.
@RETURN link_t_ptr: return the topog document link component.
*/
EXP_API link_t_ptr create_topog_doc(void);

/*
@FUNCTION destroy_topog_doc: destroy a topog document.
@INPUT link_t_ptr ptr: the topog link component.
@RETURN void: none.
*/
EXP_API void destroy_topog_doc(link_t_ptr ptr);

/*
@FUNCTION get_topog_spotset: get topog spot set.
@INPUT link_t_ptr ptr: the topog link component.
@RETURN link_t_ptr: the spot set link component.
*/
EXP_API link_t_ptr get_topog_spotset(link_t_ptr ptr);

/*
@FUNCTION clear_topog_doc: clear the topog document.
@INPUT link_t_ptr ptr: the topog link component.
@RETURN void: none.
*/
EXP_API void clear_topog_doc(link_t_ptr ptr);

/*
@FUNCTION is_topog_doc: test is topog document.
@INPUT link_t_ptr ptr: the topog link component.
@RETURN bool_t: return nonzero for being a topog document, otherwise return zero.
*/
EXP_API bool_t is_topog_doc(link_t_ptr ptr);

/*
@FUNCTION is_topog_spot: test is topog spot node.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT link_t_ptr ilk: the sopt link component.
@RETURN bool_t: return nonzero for being a spot node, otherwise return zero.
*/
EXP_API bool_t is_topog_spot(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION topog_doc_from_spot: trackback spot node to topog document.
@INPUT link_t_ptr ilk: the sopt link component.
@RETURN link_t_ptr: return topog link component.
*/
EXP_API link_t_ptr topog_doc_from_spot(link_t_ptr ilk);

/*
@FUNCTION insert_topog_spot: add a new spot node to topog document.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT link_t_ptr pos: the sopt link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new spot link component.
*/
EXP_API link_t_ptr insert_topog_spot(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_topog_next_spot: get the next spot node.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT link_t_ptr pos: the sopt link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the spot link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_topog_next_spot(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_topog_prev_spot: get the previous spot node.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT link_t_ptr pos: the sopt link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the spot link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_topog_prev_spot(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_topog_spot: find the spot node by name.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT const tchar_t* sname: the sopt name token.
@RETURN link_t_ptr: return the spot link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_topog_spot(link_t_ptr ptr, const tchar_t* sname);

/*
@FUNCTION get_topog_spot_at: find the spot node at position.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the spot link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_topog_spot_at(link_t_ptr ptr,int index);

/*
@FUNCTION delete_topog_spot: delete the spot node.
@INPUT link_t_ptr ilk: the spot link component.
@RETURN void: none.
*/
EXP_API void delete_topog_spot(link_t_ptr ilk);

/*
@FUNCTION get_topog_spot_count: counting the spot nodes in topog document.
@INPUT link_t_ptr ptr: the topog link component.
@RETURN int: return the number of spot nodes.
*/
EXP_API int get_topog_spot_count(link_t_ptr ptr);

/*
@FUNCTION get_topog_spot_selected_count: counting the selected spot nodes in topog document.
@INPUT link_t_ptr ptr: the topog link component.
@RETURN int: return the number of spot nodes.
*/
EXP_API int get_topog_spot_selected_count(link_t_ptr ptr);

/*
@FUNCTION set_topog_matrix_value: set the topog matrix value at row and col.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT int row: the zero based row position.
@INPUT int col: the zero based col position.
@INPUT double v: the value.
@RETURN bool_t: return nonzero if the row and col is valid, otherwise return zero.
*/
EXP_API bool_t set_topog_matrix_value(link_t_ptr ptr, int row, int col, double v);

/*
@FUNCTION get_topog_matrix_value: get the topog matrix value at row and col.
@INPUT link_t_ptr ptr: the topog link component.
@INPUT int row: the zero based row position.
@INPUT int col: the zero based col position.
@RETURN double: return the value if the row and col is valid, otherwise return zero.
*/
EXP_API double get_topog_matrix_value(link_t_ptr ptr, int row, int col);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*TOPOGDOC_H*/