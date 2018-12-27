/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc graph document

	@module	graphdoc.h | graph document interface file

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

#ifndef _GRAPHDOC_H
#define _GRAPHDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**********************************************serial enable attributes****************************************/
#define get_graph_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_graph_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_graph_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_graph_id(ptr,token)							set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)

#define get_graph_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define set_graph_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define get_graph_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_graph_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_graph_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_graph_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_graph_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_graph_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define get_graph_title_height(ptr)						get_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT)

#define set_graph_title_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT,n)

#define get_graph_yaxbar_height(ptr)					get_dom_node_attr_float(ptr,ATTR_YAXBAR_HEIGHT)

#define set_graph_yaxbar_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_YAXBAR_HEIGHT,n)

#define get_graph_yaxbar_width(ptr)						get_dom_node_attr_float(ptr,ATTR_YAXBAR_WIDTH)

#define set_graph_yaxbar_width(ptr,n)					set_dom_node_attr_float(ptr,ATTR_YAXBAR_WIDTH,n)

#define get_graph_xaxbar_width(ptr)						get_dom_node_attr_float(ptr,ATTR_XAXBAR_WIDTH)

#define set_graph_xaxbar_width(ptr,n)					set_dom_node_attr_float(ptr,ATTR_XAXBAR_WIDTH,n)

#define get_graph_xaxbar_height(ptr)					get_dom_node_attr_float(ptr,ATTR_XAXBAR_HEIGHT)

#define set_graph_xaxbar_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_XAXBAR_HEIGHT,n)

#define get_graph_showcheck(ptr)						get_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK)

#define set_graph_showcheck(ptr,b)						set_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK,b)

#define get_graph_showsum(ptr)							get_dom_node_attr_boolean(ptr,ATTR_SHOWSUM)

#define set_graph_showsum(ptr,b)						set_dom_node_attr_boolean(ptr,ATTR_SHOWSUM,b)

#define get_graph_xaxis_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_XAXIS,-1)

#define set_graph_xaxis(ptr,token)						set_dom_node_attr(ptr,ATTR_XAXIS,-1,token,-1)

#define get_graph_xaxdt_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_XAXDT,-1)

#define set_graph_xaxdt(ptr,token)						set_dom_node_attr(ptr,ATTR_XAXDT,-1,token,-1)

#define get_graph_xaxfmt_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_XAXFMT,-1)

#define set_graph_xaxfmt(ptr,token)						set_dom_node_attr(ptr,ATTR_XAXFMT,-1,token,-1)

#define get_graph_xaxfmt_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_XAXFMT,-1)

#define set_graph_xaxfmt(ptr,token)						set_dom_node_attr(ptr,ATTR_XAXFMT,-1,token,-1)

#define get_graph_xaxwrp(ptr)							get_dom_node_attr_boolean(ptr,ATTR_XAXWRP)

#define set_graph_xaxwrp(ptr,b)							set_dom_node_attr_boolean(ptr,ATTR_XAXWRP,b)

#define set_gax_name(glk,val)							set_dom_node_attr(glk,ATTR_NAME,-1,val,-1)

#define get_gax_name_ptr(glk)							get_dom_node_attr_ptr(glk,ATTR_NAME,-1)

#define set_gax_title(glk,val)							set_dom_node_attr(glk,ATTR_TITLE,-1,val,-1)

#define get_gax_title_ptr(glk)							get_dom_node_attr_ptr(glk,ATTR_TITLE,-1)

#define get_gax_graph_type_ptr(glk)						get_dom_node_attr_ptr(glk,ATTR_GRAPH_TYPE,-1)

#define set_gax_graph_type(glk,token)					set_dom_node_attr(glk,ATTR_GRAPH_TYPE,-1,token,-1)

#define get_gax_step(glk)								get_dom_node_attr_numeric(glk,ATTR_GAX_STEP)

#define set_gax_step(glk,n)								set_dom_node_attr_numeric(glk,ATTR_GAX_STEP,n)

#define get_gax_midd(glk)								get_dom_node_attr_numeric(glk,ATTR_GAX_MIDD)

#define set_gax_midd(glk,n)								set_dom_node_attr_numeric(glk,ATTR_GAX_MIDD,n)

#define set_yax_name(ylk,val)							set_dom_node_attr(ylk,ATTR_NAME,-1,val,-1)

#define get_yax_name_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_NAME,-1)

#define set_yax_id(ylk,val)								set_dom_node_attr(ylk,ATTR_ID,-1,val,-1)

#define get_yax_id_ptr(ylk)								get_dom_node_attr_ptr(ylk,ATTR_ID,-1)

#define set_yax_title(ylk,val)							set_dom_node_attr(ylk,ATTR_TITLE,-1,val,-1)

#define get_yax_title_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_TITLE,-1)

#define set_yax_group(ylk,val)							set_dom_node_attr(ylk,ATTR_GROUP,-1,val,-1)

#define get_yax_group_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_GROUP,-1)

#define get_yax_line_cap_ptr(ylk)						get_dom_node_attr_ptr(ylk,ATTR_LINE_CAP,-1)

#define set_yax_line_cap(ylk,token)						set_dom_node_attr(ylk,ATTR_LINE_CAP,-1,token,-1)

#define get_yax_data_len(ylk)							get_dom_node_attr_integer(ylk,ATTR_DATA_LEN)

#define set_yax_data_len(ylk,n)							set_dom_node_attr_integer(ylk,ATTR_DATA_LEN,n)

#define get_yax_data_dig(ylk)							get_dom_node_attr_integer(ylk,ATTR_DATA_DIG)

#define set_yax_data_dig(ylk,n)							set_dom_node_attr_integer(ylk,ATTR_DATA_DIG,n)

#define get_yax_zeronull(ylk)							get_dom_node_attr_boolean(ylk,ATTR_ZERONULL)

#define set_yax_zeronull(ylk,b)							set_dom_node_attr_boolean(ylk,ATTR_ZERONULL,b)

#define get_yax_sortable(ylk)							get_dom_node_attr_boolean(ylk,ATTR_SORTABLE)

#define set_yax_sortable(ylk,b)							set_dom_node_attr_boolean(ylk,ATTR_SORTABLE,b)

#define set_yax_color(ylk,val)							set_dom_node_attr(ylk,ATTR_COLOR,-1,val,-1)

#define get_yax_color_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_COLOR,-1)

#define get_yax_image_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_IMAGE,-1)

#define set_yax_image(ylk,token)						set_dom_node_attr(ylk,ATTR_IMAGE,-1,token,-1)

#define get_xax_attr_table(xlk)							get_dom_node_attr_table(xlk)

#define set_xax_text(xlk,val)							set_dom_node_text(xlk,val,-1)

#define get_xax_text_ptr(xlk)							get_dom_node_text_ptr(xlk)

#define set_xax_name(xlk,val)							set_dom_node_attr(xlk,ATTR_NAME,-1,val,-1)

#define get_xax_name_ptr(xlk)							get_dom_node_attr_ptr(xlk,ATTR_NAME,-1)

/**********************************************runtime enable attributes***********************************************/
#define set_graph_images(ptr,images)					set_dom_node_images(ptr,images)

#define get_graph_images(ptr)							get_dom_node_images(ptr)

#define set_graph_design(ptr,b)							set_dom_node_mask_check(ptr,MSK_DESIGN,b) 

#define graph_is_design(ptr)							get_dom_node_mask_check(ptr,MSK_DESIGN)

#define set_xax_checked(xlk,check)						set_dom_node_mask_check(xlk,MSK_CHECKED,check)

#define get_xax_checked(xlk)							get_dom_node_mask_check(xlk,MSK_CHECKED)

#define set_xax_locked(xlk,c)							set_dom_node_mask_check(xlk,MSK_LOCKED,c) 

#define get_xax_locked(xlk)								get_dom_node_mask_check(xlk,MSK_LOCKED)

#define set_yax_selected(ylk,check)						set_dom_node_mask_check(ylk,MSK_CHECKED,check)

#define get_yax_selected(ylk)							get_dom_node_mask_check(ylk,MSK_CHECKED)

#define set_xax_state(xlk,c)							set_dom_node_mask(xlk,(get_dom_node_mask(xlk) & 0xFFFFFFF0) | c)

#define get_xax_state(xlk)								(get_dom_node_mask(xlk) & 0x0000000F)

/*********************************************runtime functions********************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_graph_doc(void);

XDL_API void destroy_graph_doc(link_t_ptr ptr);

XDL_API void clear_graph_doc(link_t_ptr ptr);

XDL_API void clear_graph_xaxset(link_t_ptr ptr);

XDL_API void clear_graph_gaxset(link_t_ptr ptr);

XDL_API void clear_graph_yaxset(link_t_ptr ptr);

XDL_API void merge_graph_yaxset(link_t_ptr ptr_dest, link_t_ptr ptr_src);

XDL_API void merge_graph_xaxset(link_t_ptr ptr_dest, link_t_ptr ptr_src);

XDL_API bool_t is_graph_doc(link_t_ptr ptr);

XDL_API bool_t is_graph_xax(link_t_ptr ptr,link_t_ptr xlk);

XDL_API bool_t is_graph_yax(link_t_ptr ptr,link_t_ptr ylk);

XDL_API bool_t is_graph_gax(link_t_ptr ptr, link_t_ptr glk);

XDL_API link_t_ptr insert_gax(link_t_ptr ptr, link_t_ptr pos);

XDL_API void delete_gax(link_t_ptr ylk);

XDL_API link_t_ptr get_graph_gaxset(link_t_ptr ptr);

XDL_API link_t_ptr get_next_gax(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_prev_gax(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_gax(link_t_ptr ptr, const tchar_t* gname);

XDL_API int get_gax_count(link_t_ptr ptr);

XDL_API link_t_ptr insert_yax(link_t_ptr ptr,link_t_ptr pos);

XDL_API void delete_yax(link_t_ptr ylk);

XDL_API link_t_ptr get_graph_yaxset(link_t_ptr ptr);

XDL_API int get_yax_count(link_t_ptr ptr);

XDL_API int get_yax_selected_count(link_t_ptr ptr);

XDL_API link_t_ptr get_next_yax(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_yax(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_yax_at(link_t_ptr ptr, int index);

XDL_API link_t_ptr get_yax(link_t_ptr ptr,const tchar_t* yname);

XDL_API link_t_ptr get_yax_gax(link_t_ptr ptr, link_t_ptr ylk);

XDL_API link_t_ptr get_graph_xaxset(link_t_ptr ptr);

XDL_API link_t_ptr insert_xax(link_t_ptr ptr,link_t_ptr pos);

XDL_API int get_xax_count(link_t_ptr ptr);

XDL_API int get_xax_checked_count(link_t_ptr ptr);

XDL_API link_t_ptr get_xax_at(link_t_ptr ptr,int pos);

XDL_API void delete_xax(link_t_ptr ylk);

XDL_API link_t_ptr get_next_xax(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_prev_xax(link_t_ptr ptr,link_t_ptr ylk);

XDL_API link_t_ptr get_xax_at(link_t_ptr ptr,int nIndex);

XDL_API link_t_ptr get_xax(link_t_ptr ptr, const tchar_t* xname);

XDL_API link_t_ptr get_yax_by_id(link_t_ptr ptr, const tchar_t* cid);

XDL_API const tchar_t* get_xax_storage_ptr(link_t_ptr xlk, const tchar_t* key);

XDL_API void set_xax_storage(link_t_ptr xlk, const tchar_t* key, const tchar_t* val);

XDL_API int get_update_xax_count(link_t_ptr ptr);

XDL_API void refresh_graph_xaxset(link_t_ptr ptr);

XDL_API void set_coor_numeric(link_t_ptr xlk,link_t_ptr ylk,double dbl);

XDL_API double get_coor_numeric(link_t_ptr xlk,link_t_ptr ylk);

XDL_API void set_coor_text(link_t_ptr xlk,link_t_ptr ylk,const tchar_t* token, int len);

XDL_API const tchar_t* get_coor_text_ptr(link_t_ptr xlk,link_t_ptr ylk);

XDL_API int get_coor_text(link_t_ptr xlk,link_t_ptr ylk,tchar_t* buf,int max);

XDL_API const tchar_t* get_coor_storage_ptr(link_t_ptr xlk, link_t_ptr ylk, const tchar_t* key);

XDL_API void set_coor_storage(link_t_ptr xlk, link_t_ptr ylk, const tchar_t* key, const tchar_t* val);

XDL_API void set_coor_dirty(link_t_ptr rlk, link_t_ptr clk, bool_t b);

XDL_API bool_t get_coor_dirty(link_t_ptr rlk, link_t_ptr clk);

XDL_API void set_xax_clean(link_t_ptr rlk);

XDL_API void set_xax_dirty(link_t_ptr rlk);

XDL_API void calc_graph_gax_base(link_t_ptr ptr, link_t_ptr glk, float* pmidd, float* pstep);

#ifdef	__cplusplus
}
#endif

#endif //XDLVIEW

#endif //GRAPHDOC_H