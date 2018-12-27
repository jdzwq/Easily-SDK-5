/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc topog document

	@module	topogdoc.h | topog document interface file

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

/****************************************serial enable attributes******************************************/
#define get_topog_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_topog_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_topog_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_topog_id(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define set_topog_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_topog_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define get_topog_rows(ptr)								get_dom_node_attr_integer(ptr,ATTR_ROWS)

#define set_topog_rows(ptr,n)							set_dom_node_attr_integer(ptr,ATTR_ROWS,n)

#define get_topog_cols(ptr)								get_dom_node_attr_integer(ptr,ATTR_COLS)

#define set_topog_cols(ptr,n)							set_dom_node_attr_integer(ptr,ATTR_COLS,n)

#define set_topog_matrix(ptr,token,len)					set_dom_node_attr(ptr,ATTR_MATRIX,-1,token,len)

#define get_topog_matrix_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_MATRIX,-1)

#define get_topog_rx(ptr)								get_dom_node_attr_float(ptr,ATTR_RX)

#define set_topog_rx(ptr,n)								set_dom_node_attr_float(ptr,ATTR_RX,n)

#define get_topog_ry(ptr)								get_dom_node_attr_float(ptr,ATTR_RY)

#define set_topog_ry(ptr,n)								set_dom_node_attr_float(ptr,ATTR_RY,n)

#define get_topog_spot_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_topog_spot_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_topog_spot_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_topog_spot_id(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_topog_spot_type_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TYPE,-1)

#define set_topog_spot_type(ilk,token)					set_dom_node_attr(ilk,ATTR_TYPE,-1,token,-1)

#define get_topog_spot_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)

#define set_topog_spot_title(ilk,token)					set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

#define set_topog_spot_style(ilk,token)					set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)

#define get_topog_spot_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)

#define get_topog_spot_col(ilk)							get_dom_node_attr_integer(ilk,ATTR_COL)

#define set_topog_spot_col(ilk,col)						set_dom_node_attr_integer(ilk,ATTR_COL,col)

#define get_topog_spot_row(ilk)							get_dom_node_attr_integer(ilk,ATTR_ROW)

#define set_topog_spot_row(ilk,row)						set_dom_node_attr_integer(ilk,ATTR_ROW,row)

/***********************************************runtime attributes*********************************************/
#define set_topog_design(ptr,b)							set_dom_node_mask_check(ptr,MSK_DESIGN,b)

#define topog_is_design(ptr)							get_dom_node_mask_check(ptr,MSK_DESIGN)

#define set_topog_spot_selected(ilk,b)					set_dom_node_mask_check(ilk,MSK_CHECKED,b)

#define get_topog_spot_selected(ilk)					get_dom_node_mask_check(ilk,MSK_CHECKED) 

/**********************************************runtime function*************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_topog_doc();

XDL_API void destroy_topog_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_topog_spotset(link_t_ptr ptr);

XDL_API void clear_topog_doc(link_t_ptr ptr);

XDL_API bool_t is_topog_doc(link_t_ptr ptr);

XDL_API bool_t is_topog_spot(link_t_ptr ptr, link_t_ptr plk);

XDL_API link_t_ptr topog_doc_from_spot(link_t_ptr ilk);

XDL_API link_t_ptr insert_topog_spot(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_topog_next_spot(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_topog_prev_spot(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_topog_spot(link_t_ptr ptr, const tchar_t* sname);

XDL_API link_t_ptr get_topog_spot_at(link_t_ptr ptr,int index);

XDL_API void delete_topog_spot(link_t_ptr ilk);

XDL_API int get_topog_spot_count(link_t_ptr ptr);

XDL_API int get_topog_spot_selected_count(link_t_ptr ptr);

XDL_API bool_t set_topog_matrix_value(link_t_ptr ptr, int row, int col, double v);

XDL_API double get_topog_matrix_value(link_t_ptr ptr, int row, int col);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*TOPOGDOC_H*/