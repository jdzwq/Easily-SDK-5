/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc chart document

	@module	chartdoc.h | interface file

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

#ifndef _CHARTDOC_H
#define _CHARTDOC_H

#include "xdldef.h"
#include "docattr.h"

#ifdef XDL_SUPPORT_DOC

/*******************************************serial enable attributes********************************************/
#define set_chart_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_chart_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_chart_id(ptr,token)							set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)

#define get_chart_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_chart_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_chart_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define get_chart_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_chart_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_chart_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_chart_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define get_chart_table_class_ptr(ilk)					get_dom_node_name_ptr(ilk)

#define set_chart_table_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_chart_table_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_chart_table_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)

#define get_chart_table_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

#define set_chart_table_text(ilk,token,len)				set_dom_node_text(ilk,token,len)

#define get_chart_table_text_ptr(ilk)					get_dom_node_text_ptr(ilk)

#define set_chart_table_style(ilk,token)				set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)

#define get_chart_table_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)

#define get_chart_table_x(ilk)							get_dom_node_attr_float(ilk,ATTR_X)

#define set_chart_table_x(ilk,xm)						set_dom_node_attr_float(ilk,ATTR_X,xm)

#define get_chart_table_y(ilk)							get_dom_node_attr_float(ilk,ATTR_Y)

#define set_chart_table_y(ilk,ym)						set_dom_node_attr_float(ilk,ATTR_Y,ym)

#define set_chart_table_width(ilk,n)					set_dom_node_attr_float(ilk,ATTR_WIDTH,n)

#define get_chart_table_width(ilk)						get_dom_node_attr_float(ilk,ATTR_WIDTH)

#define set_chart_table_height(ilk,n)					set_dom_node_attr_float(ilk,ATTR_HEIGHT,n)

#define get_chart_table_height(ilk)						get_dom_node_attr_float(ilk,ATTR_HEIGHT)

#define get_chart_table_taborder(ilk)					get_dom_node_attr_integer(ilk,ATTR_TABORDER)

#define set_chart_table_taborder(ilk,n)					set_dom_node_attr_integer(ilk,ATTR_TABORDER,n)

#define set_chart_map_table_rows(ilk, n)				set_dom_node_attr_integer(ilk, ATTR_ROWS, n)

#define get_chart_map_table_rows(ilk)					get_dom_node_attr_integer(ilk, ATTR_ROWS)

#define set_chart_map_table_cols(ilk, n)				set_dom_node_attr_integer(ilk, ATTR_COLS, n)

#define get_chart_map_table_cols(ilk)					get_dom_node_attr_integer(ilk, ATTR_COLS)

#define get_chart_map_table_rx(ilk)						get_dom_node_attr_float(ilk,ATTR_RX)

#define set_chart_map_table_rx(ilk,n)					set_dom_node_attr_float(ilk,ATTR_RX,n)

#define get_chart_map_table_ry(ilk)						get_dom_node_attr_float(ilk,ATTR_RY)

#define set_chart_map_table_ry(ilk,n)					set_dom_node_attr_float(ilk,ATTR_RY,n)

#define set_chart_dot_table_rows(ilk, n)				set_dom_node_attr_integer(ilk, ATTR_ROWS, n)

#define get_chart_dot_table_rows(ilk)					get_dom_node_attr_integer(ilk, ATTR_ROWS)

#define set_chart_dot_table_cols(ilk, n)				set_dom_node_attr_integer(ilk, ATTR_COLS, n)

#define get_chart_dot_table_cols(ilk)					get_dom_node_attr_integer(ilk, ATTR_COLS)

#define get_chart_dot_table_rx(ilk)						get_dom_node_attr_float(ilk,ATTR_RX)

#define set_chart_dot_table_rx(ilk,n)					set_dom_node_attr_float(ilk,ATTR_RX,n)

#define get_chart_dot_table_ry(ilk)						get_dom_node_attr_float(ilk,ATTR_RY)

#define set_chart_dot_table_ry(ilk,n)					set_dom_node_attr_float(ilk,ATTR_RY,n)

#define set_chart_line_table_size(ilk, n)				set_dom_node_attr_integer(ilk, ATTR_SIZE, n)

#define get_chart_line_table_size(ilk)					get_dom_node_attr_integer(ilk, ATTR_SIZE)

#define get_chart_line_table_base(ilk)					get_dom_node_attr_float(ilk,ATTR_BASE)

#define set_chart_line_table_base(ilk,n)				set_dom_node_attr_float(ilk,ATTR_BASE,n)

#define get_chart_line_table_span(ilk)					get_dom_node_attr_float(ilk,ATTR_SPAN)

#define set_chart_line_table_span(ilk,n)				set_dom_node_attr_float(ilk,ATTR_SPAN,n)

#define set_chart_bar_table_size(ilk, n)				set_dom_node_attr_integer(ilk, ATTR_SIZE, n)

#define get_chart_bar_table_size(ilk)					get_dom_node_attr_integer(ilk, ATTR_SIZE)

#define get_chart_bar_table_base(ilk)					get_dom_node_attr_float(ilk,ATTR_BASE)

#define set_chart_bar_table_base(ilk,n)					set_dom_node_attr_float(ilk,ATTR_BASE,n)

#define get_chart_bar_table_span(ilk)					get_dom_node_attr_float(ilk,ATTR_SPAN)

#define set_chart_bar_table_span(ilk,n)					set_dom_node_attr_float(ilk,ATTR_SPAN,n)

#define set_chart_pie_table_size(ilk, n)				set_dom_node_attr_integer(ilk, ATTR_SIZE, n)

#define get_chart_pie_table_size(ilk)					get_dom_node_attr_integer(ilk, ATTR_SIZE)

#define set_chart_counter_table_layer(ilk,token)		set_dom_node_attr(ilk,ATTR_LAYER,-1,token,-1)

#define get_chart_counter_table_layer_ptr(ilk)			get_dom_node_attr_ptr(ilk,ATTR_LAYER,-1)

#define set_chart_counter_table_size(ilk, n)			set_dom_node_attr_integer(ilk, ATTR_SIZE, n)

#define get_chart_counter_table_size(ilk)				get_dom_node_attr_integer(ilk, ATTR_SIZE)
/**********************************************************************************************************/
#define set_chart_table_delta(ilk,ul)					set_dom_node_delta(ilk,(var_long)ul)

#define get_chart_table_delta(ilk)						get_dom_node_delta(ilk)

#define set_chart_table_selected(flk,b)					set_dom_node_mask_check(flk,MSK_CHECKED,b)

#define get_chart_table_selected(flk)					get_dom_node_mask_check(flk,MSK_CHECKED) 

/*********************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_chart_doc(void);

XDL_API void destroy_chart_doc(link_t_ptr ptr);

XDL_API void clear_chart_doc(link_t_ptr ptr);

XDL_API bool_t is_chart_doc(link_t_ptr ptr);

XDL_API bool_t is_chart_table(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void reset_chart_taborder(link_t_ptr ptr);

XDL_API link_t_ptr get_chart_tableset(link_t_ptr ptr);

XDL_API link_t_ptr insert_chart_table(link_t_ptr ptr,const tchar_t* sz_class);

XDL_API link_t_ptr get_chart_next_table(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_chart_prev_table(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_chart_table(link_t_ptr ptr, const tchar_t* sz_name);

XDL_API int get_chart_table_count(link_t_ptr ptr);

XDL_API int get_chart_table_count_by_class(link_t_ptr ptr, const tchar_t* szClass);

XDL_API int get_chart_table_selected_count(link_t_ptr ptr);

XDL_API void delete_chart_table(link_t_ptr ilk);


#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //CHARTDOC_H