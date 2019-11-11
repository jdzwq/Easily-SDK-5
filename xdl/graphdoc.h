/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc graph document

	@module	graphdoc.h | interface file

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

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_graph_name: get the graph name.
*/
#define get_graph_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_graph_name_ptr: set the graph name.
*/
#define set_graph_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER id: string.
@GET set_graph_name: get the graph identifier.
*/
#define get_graph_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_ID,-1)
/*
@PROPER id: string.
@SET get_graph_name_ptr: set the graph identifier.
*/
#define set_graph_id(ptr,token)							set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)
/*
@PROPER title: string.
@GET get_graph_title_ptr: get the graph title.
*/
#define get_graph_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_graph_title: set the graph title.
*/
#define set_graph_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER style: string.
@GET get_graph_style_ptr: get the graph style.
*/
#define get_graph_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER style: string.
@SET set_graph_style: set the graph style.
*/
#define set_graph_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)
/*
@PROPER width: float.
@GET get_graph_width: get the graph width.
*/
#define get_graph_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER width: float.
@SET set_graph_width: set the graph width.
*/
#define set_graph_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER height: float.
@GET get_graph_height: get the graph height.
*/
#define get_graph_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: float.
@SET set_graph_height: set the graph height.
*/
#define set_graph_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER height: float.
@GET get_graph_title_height: get the graph title height.
*/
#define get_graph_title_height(ptr)						get_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT)
/*
@PROPER height: float.
@SET set_graph_title_height: set the graph title height.
*/
#define set_graph_title_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT,n)
/*
@PROPER yaxHeight: float.
@GET get_graph_yaxbar_height: get the graph yax bar height.
*/
#define get_graph_yaxbar_height(ptr)					get_dom_node_attr_float(ptr,ATTR_YAXBAR_HEIGHT)
/*
@PROPER yaxHeight: float.
@SET set_graph_yaxbar_height: set the graph yax bar height.
*/
#define set_graph_yaxbar_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_YAXBAR_HEIGHT,n)
/*
@PROPER yaxWidth: float.
@GET get_graph_yaxbar_width: get the graph yax bar width.
*/
#define get_graph_yaxbar_width(ptr)						get_dom_node_attr_float(ptr,ATTR_YAXBAR_WIDTH)
/*
@PROPER yaxWidth: float.
@SET set_graph_yaxbar_width: set the graph yax bar width.
*/
#define set_graph_yaxbar_width(ptr,n)					set_dom_node_attr_float(ptr,ATTR_YAXBAR_WIDTH,n)
/*
@PROPER xaxWidth: float.
@GET get_graph_xaxbar_width: get the graph xax bar width.
*/
#define get_graph_xaxbar_width(ptr)						get_dom_node_attr_float(ptr,ATTR_XAXBAR_WIDTH)
/*
@PROPER xaxWidth: float.
@SET set_graph_xaxbar_width: set the graph xax bar width.
*/
#define set_graph_xaxbar_width(ptr,n)					set_dom_node_attr_float(ptr,ATTR_XAXBAR_WIDTH,n)
/*
@PROPER xaxHeight: float.
@GET get_graph_xaxbar_height: get the graph xax bar height.
*/
#define get_graph_xaxbar_height(ptr)					get_dom_node_attr_float(ptr,ATTR_XAXBAR_HEIGHT)
/*
@PROPER xaxHeight: float.
@SET set_graph_xaxbar_height: set the graph xax bar height.
*/
#define set_graph_xaxbar_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_XAXBAR_HEIGHT,n)
/*
@PROPER showCheck: boolean.
@GET get_graph_showcheck: get the graph show check box.
*/
#define get_graph_showcheck(ptr)						get_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK)
/*
@PROPER showCheck: boolean.
@SET set_graph_showcheck: set the graph show check box.
*/
#define set_graph_showcheck(ptr,b)						set_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK,b)
/*
@PROPER showSummary: boolean.
@GET get_graph_showsum: get the graph show summary bar.
*/
#define get_graph_showsum(ptr)							get_dom_node_attr_boolean(ptr,ATTR_SHOWSUM)
/*
@PROPER showSummary: boolean.
@SET set_graph_showsum: set the graph show summary bar.
*/
#define set_graph_showsum(ptr,b)						set_dom_node_attr_boolean(ptr,ATTR_SHOWSUM,b)
/*
@PROPER xaxIs: boolean.
@GET get_graph_xaxis_ptr: get the graph xax is.
*/
#define get_graph_xaxis_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_XAXIS,-1)
/*
@PROPER xaxIs: boolean.
@SET set_graph_xaxis: set the graph xax is.
*/
#define set_graph_xaxis(ptr,token)						set_dom_node_attr(ptr,ATTR_XAXIS,-1,token,-1)
/*
@PROPER xaxDataType: boolean.
@GET get_graph_xaxdt_ptr: get the graph xax data type.
*/
#define get_graph_xaxdt_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_XAXDT,-1)
/*
@PROPER xaxDataType: boolean.
@SET set_graph_xaxdt: set the graph xax data type.
*/
#define set_graph_xaxdt(ptr,token)						set_dom_node_attr(ptr,ATTR_XAXDT,-1,token,-1)
/*
@PROPER xaxFormat: boolean.
@GET get_graph_xaxfmt_ptr: get the graph xax format.
*/
#define get_graph_xaxfmt_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_XAXFMT,-1)
/*
@PROPER xaxFormat: boolean.
@SET set_graph_xaxfmt: set the graph xax format.
*/
#define set_graph_xaxfmt(ptr,token)						set_dom_node_attr(ptr,ATTR_XAXFMT,-1,token,-1)
/*
@PROPER xaxWrap: boolean.
@GET get_graph_xaxwrp: get the graph xax wrap drawing.
*/
#define get_graph_xaxwrp(ptr)							get_dom_node_attr_boolean(ptr,ATTR_XAXWRP)
/*
@PROPER xaxWrap: boolean.
@SET set_graph_xaxwrp: set the graph xax wrap drawing.
*/
#define set_graph_xaxwrp(ptr,b)							set_dom_node_attr_boolean(ptr,ATTR_XAXWRP,b)
/*
@PROPER name: string.
@SET set_gax_name: set the gax name.
*/
#define set_gax_name(glk,val)							set_dom_node_attr(glk,ATTR_NAME,-1,val,-1)
/*
@PROPER name: string.
@GET get_gax_name_ptr: get the gax name.
*/
#define get_gax_name_ptr(glk)							get_dom_node_attr_ptr(glk,ATTR_NAME,-1)
/*
@PROPER title: string.
@SET set_gax_title: set the gax title.
*/
#define set_gax_title(glk,val)							set_dom_node_attr(glk,ATTR_TITLE,-1,val,-1)
/*
@PROPER title: string.
@GET get_gax_title_ptr: get the gax title.
*/
#define get_gax_title_ptr(glk)							get_dom_node_attr_ptr(glk,ATTR_TITLE,-1)
/*
@PROPER graphType: string.
@GET get_gax_graph_type_ptr: get the gax graph type.
*/
#define get_gax_graph_type_ptr(glk)						get_dom_node_attr_ptr(glk,ATTR_GRAPH_TYPE,-1)
/*
@PROPER graphType: string.
@SET set_gax_graph_type: set the gax graph type.
*/
#define set_gax_graph_type(glk,token)					set_dom_node_attr(glk,ATTR_GRAPH_TYPE,-1,token,-1)
/*
@PROPER step: numeric.
@GET get_gax_step: get the gax step.
*/
#define get_gax_step(glk)								get_dom_node_attr_numeric(glk,ATTR_GAX_STEP)
/*
@PROPER step: numeric.
@SET set_gax_step: set the gax step.
*/
#define set_gax_step(glk,n)								set_dom_node_attr_numeric(glk,ATTR_GAX_STEP,n)
/*
@PROPER middle: numeric.
@GET get_gax_midd: get the gax middle.
*/
#define get_gax_midd(glk)								get_dom_node_attr_numeric(glk,ATTR_GAX_MIDD)
/*
@PROPER middle: numeric.
@SET set_gax_midd: set the gax middle.
*/
#define set_gax_midd(glk,n)								set_dom_node_attr_numeric(glk,ATTR_GAX_MIDD,n)
/*
@PROPER name: string.
@SET set_yax_name: set the yax name.
*/
#define set_yax_name(ylk,val)							set_dom_node_attr(ylk,ATTR_NAME,-1,val,-1)
/*
@PROPER name: string.
@GET get_yax_name_ptr: get the yax name.
*/
#define get_yax_name_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_yax_id: set the yax identifier.
*/
#define set_yax_id(ylk,val)								set_dom_node_attr(ylk,ATTR_ID,-1,val,-1)
/*
@PROPER id: string.
@GET get_yax_id_ptr: get the yax identifier.
*/
#define get_yax_id_ptr(ylk)								get_dom_node_attr_ptr(ylk,ATTR_ID,-1)
/*
@PROPER title: string.
@SET set_yax_title: set the yax title.
*/
#define set_yax_title(ylk,val)							set_dom_node_attr(ylk,ATTR_TITLE,-1,val,-1)
/*
@PROPER title: string.
@GET get_yax_title_ptr: get the yax title.
*/
#define get_yax_title_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_TITLE,-1)
/*
@PROPER group: string.
@SET set_yax_group: set the yax group.
*/
#define set_yax_group(ylk,val)							set_dom_node_attr(ylk,ATTR_GROUP,-1,val,-1)
/*
@PROPER group: string.
@GET get_yax_group_ptr: get the yax group.
*/
#define get_yax_group_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_GROUP,-1)
/*
@PROPER lineCap: string.
@GET get_yax_line_cap_ptr: get the yax line cap.
*/
#define get_yax_line_cap_ptr(ylk)						get_dom_node_attr_ptr(ylk,ATTR_LINE_CAP,-1)
/*
@PROPER lineCap: string.
@SET set_yax_line_cap: set the yax line cap.
*/
#define set_yax_line_cap(ylk,token)						set_dom_node_attr(ylk,ATTR_LINE_CAP,-1,token,-1)
/*
@PROPER dataLen: integer.
@GET get_yax_data_len: get the yax line cap.
*/
#define get_yax_data_len(ylk)							get_dom_node_attr_integer(ylk,ATTR_DATA_LEN)
/*
@PROPER dataLen: integer.
@SET set_yax_data_len: set the yax line cap.
*/
#define set_yax_data_len(ylk,n)							set_dom_node_attr_integer(ylk,ATTR_DATA_LEN,n)
/*
@PROPER dataDig: integer.
@GET get_yax_data_dig: get the yax data digits.
*/
#define get_yax_data_dig(ylk)							get_dom_node_attr_integer(ylk,ATTR_DATA_DIG)
/*
@PROPER dataDig: integer.
@SET set_yax_data_dig: set the yax data digits.
*/
#define set_yax_data_dig(ylk,n)							set_dom_node_attr_integer(ylk,ATTR_DATA_DIG,n)
/*
@PROPER zeroNull: boolean.
@GET get_yax_zeronull: get the yax data digits.
*/
#define get_yax_zeronull(ylk)							get_dom_node_attr_boolean(ylk,ATTR_ZERONULL)
/*
@PROPER zeroNull: boolean.
@SET set_yax_zeronull: set the yax data digits.
*/
#define set_yax_zeronull(ylk,b)							set_dom_node_attr_boolean(ylk,ATTR_ZERONULL,b)
/*
@PROPER sortable: boolean.
@GET get_yax_sortable: get the yax sortable.
*/
#define get_yax_sortable(ylk)							get_dom_node_attr_boolean(ylk,ATTR_SORTABLE)
/*
@PROPER sortable: boolean.
@SET set_yax_sortable: set the yax sortable.
*/
#define set_yax_sortable(ylk,b)							set_dom_node_attr_boolean(ylk,ATTR_SORTABLE,b)
/*
@PROPER color: string.
@SET set_yax_color: set the yax color token.
*/
#define set_yax_color(ylk,val)							set_dom_node_attr(ylk,ATTR_COLOR,-1,val,-1)
/*
@PROPER color: string.
@GET get_yax_color_ptr: get the yax color token.
*/
#define get_yax_color_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_COLOR,-1)
/*
@PROPER image: string.
@GET get_yax_image_ptr: get the yax image name.
*/
#define get_yax_image_ptr(ylk)							get_dom_node_attr_ptr(ylk,ATTR_IMAGE,-1)
/*
@PROPER image: string.
@SET set_yax_image: set the yax image name.
*/
#define set_yax_image(ylk,token)						set_dom_node_attr(ylk,ATTR_IMAGE,-1,token,-1)

#define get_xax_attr_table(xlk)							get_dom_node_attr_table(xlk)
/*
@PROPER text: string.
@SET set_xax_text: set the xax text.
*/
#define set_xax_text(xlk,val)							set_dom_node_text(xlk,val,-1)
/*
@PROPER text: string.
@GET get_xax_text_ptr: get the xax text.
*/
#define get_xax_text_ptr(xlk)							get_dom_node_text_ptr(xlk)
/*
@PROPER name: string.
@SET set_xax_name: set the xax name.
*/
#define set_xax_name(xlk,val)							set_dom_node_attr(xlk,ATTR_NAME,-1,val,-1)
/*
@PROPER name: string.
@GET get_xax_name_ptr: get the xax name.
*/
#define get_xax_name_ptr(xlk)							get_dom_node_attr_ptr(xlk,ATTR_NAME,-1)

/*
@PROPER images: document.
@SET set_graph_images: set the graph images reference.
*/
#define set_graph_images(ptr,images)					set_dom_node_images(ptr,images)
/*
@PROPER images: document.
@GET get_graph_images: get the graph image reference.
*/
#define get_graph_images(ptr)							get_dom_node_images(ptr)
/*
@PROPER design: boolean.
@SET set_graph_design: set the graph is design mode.
*/
#define set_graph_design(ptr,b)							set_dom_node_mask_check(ptr,MSK_DESIGN,b) 
/*
@PROPER design: boolean.
@GET graph_is_design: get the graph is design mode.
*/
#define graph_is_design(ptr)							get_dom_node_mask_check(ptr,MSK_DESIGN)
/*
@PROPER checked: boolean.
@SET set_xax_checked: set the xax is checked.
*/
#define set_xax_checked(xlk,check)						set_dom_node_mask_check(xlk,MSK_CHECKED,check)
/*
@PROPER checked: boolean.
@GET get_xax_checked: get the xsx is checked.
*/
#define get_xax_checked(xlk)							get_dom_node_mask_check(xlk,MSK_CHECKED)
/*
@PROPER locked: boolean.
@SET set_xax_locked: set the xax is locked.
*/
#define set_xax_locked(xlk,c)							set_dom_node_mask_check(xlk,MSK_LOCKED,c) 
/*
@PROPER locked: boolean.
@GET get_xax_locked: get the xsx is locked.
*/
#define get_xax_locked(xlk)								get_dom_node_mask_check(xlk,MSK_LOCKED)
/*
@PROPER selected: boolean.
@SET set_yax_selected: set the xax is selected.
*/
#define set_yax_selected(ylk,check)						set_dom_node_mask_check(ylk,MSK_CHECKED,check)
/*
@PROPER selected: boolean.
@GET get_yax_selected: get the xsx is selected.
*/
#define get_yax_selected(ylk)							get_dom_node_mask_check(ylk,MSK_CHECKED)
/*
@PROPER state: integer.
@SET set_xax_state: set the xax state.
*/
#define set_xax_state(xlk,c)							set_dom_node_mask(xlk,(get_dom_node_mask(xlk) & 0xFFFFFFF0) | c)
/*
@PROPER state: integer.
@GET get_xax_state: get the xsx state.
*/
#define get_xax_state(xlk)								(get_dom_node_mask(xlk) & 0x0000000F)


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_graph_doc: create a graph document.
@RETURN link_t_ptr: return the graph document link component.
*/
XDL_API link_t_ptr create_graph_doc(void);

/*
@FUNCTION destroy_graph_doc: destroy a graph document.
@INPUT link_t_ptr ptr: the graph document link component.
@RETURN void: none.
*/
XDL_API void destroy_graph_doc(link_t_ptr ptr);

/*
@FUNCTION clear_graph_doc: clear a graph document.
@INPUT link_t_ptr ptr: the graph document link component.
@RETURN void: none.
*/
XDL_API void clear_graph_doc(link_t_ptr ptr);

/*
@FUNCTION clear_graph_xaxset: clear the graph xax set.
@INPUT link_t_ptr ptr: the graph document link component.
@RETURN void: none.
*/
XDL_API void clear_graph_xaxset(link_t_ptr ptr);

/*
@FUNCTION clear_graph_gaxset: clear the graph gax set.
@INPUT link_t_ptr ptr: the graph document link component.
@RETURN void: none.
*/
XDL_API void clear_graph_gaxset(link_t_ptr ptr);

/*
@FUNCTION clear_graph_yaxset: clear the graph yax set.
@INPUT link_t_ptr ptr: the graph document link component.
@RETURN void: none.
*/
XDL_API void clear_graph_yaxset(link_t_ptr ptr);

/*
@FUNCTION merge_graph_yaxset: merge source yax set to destination yax set.
@INPUT link_t_ptr ptr_dst: the destination yax link component.
@INPUT link_t_ptr ptr_src: the source yax link component.
@RETURN void: none.
*/
XDL_API void merge_graph_yaxset(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION merge_graph_xaxset: merge source xax set to destination xax set.
@INPUT link_t_ptr ptr_dst: the destination xax link component.
@INPUT link_t_ptr ptr_src: the source xax link component.
@RETURN void: none.
*/
XDL_API void merge_graph_xaxset(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION is_graph_doc: test is graph document.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN bool_t: return nonzero for being a graph document, otherwise return zero.
*/
XDL_API bool_t is_graph_doc(link_t_ptr ptr);

/*
@FUNCTION is_graph_xax: test is graph xax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr xlk: the xax link component.
@RETURN bool_t: return nonzero for being a xax of graph, otherwise return zero.
*/
XDL_API bool_t is_graph_xax(link_t_ptr ptr,link_t_ptr xlk);

/*
@FUNCTION is_graph_yax: test is graph yax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr ylk: the yax link component.
@RETURN bool_t: return nonzero for being a yax of graph, otherwise return zero.
*/
XDL_API bool_t is_graph_yax(link_t_ptr ptr,link_t_ptr ylk);

/*
@FUNCTION is_graph_gax: test is graph gax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr glk: the gax link component.
@RETURN bool_t: return nonzero for being a gax of graph, otherwise return zero.
*/
XDL_API bool_t is_graph_gax(link_t_ptr ptr, link_t_ptr glk);

/*
@FUNCTION insert_gax: add a new gax into graph.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr pos: the gax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new gax link component.
*/
XDL_API link_t_ptr insert_gax(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_gax: delete a gax.
@INPUT link_t_ptr glk: the gax link component.
@RETURN void: none.
*/
XDL_API void delete_gax(link_t_ptr glk);

/*
@FUNCTION get_graph_gaxset: get the graph gax set.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN link_t_ptr: return the gax set link component.
*/
XDL_API link_t_ptr get_graph_gaxset(link_t_ptr ptr);

/*
@FUNCTION get_next_gax: get the next gax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr pos: the gax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the gax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_gax(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_prev_gax: get the previous gax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr pos: the gax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the gax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_gax(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_gax: find the gax by name.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT const tchar_t* gname: the gax name token.
@RETURN link_t_ptr: return the gax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_gax(link_t_ptr ptr, const tchar_t* gname);

/*
@FUNCTION get_gax_count: counting the gaxs in graph.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN int: return the number of gaxs.
*/
XDL_API int get_gax_count(link_t_ptr ptr);

/*
@FUNCTION insert_yax: add a new yax into graph.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr pos: the yax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new yax link component.
*/
XDL_API link_t_ptr insert_yax(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_yax: delete a yax.
@INPUT link_t_ptr ylk: the yax link component.
@RETURN void: none.
*/
XDL_API void delete_yax(link_t_ptr ylk);

/*
@FUNCTION get_graph_yaxset: get graph yax set.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN link_t_ptr: return yax set link component.
*/
XDL_API link_t_ptr get_graph_yaxset(link_t_ptr ptr);

/*
@FUNCTION get_yax_count: counting yaxs in graph
@INPUT link_t_ptr ptr: the graph link component.
@RETURN int: return the number of yaxs.
*/
XDL_API int get_yax_count(link_t_ptr ptr);

/*
@FUNCTION get_yax_selected_count: counting selected yaxs in graph
@INPUT link_t_ptr ptr: the graph link component.
@RETURN int: return the number of yaxs.
*/
XDL_API int get_yax_selected_count(link_t_ptr ptr);

/*
@FUNCTION get_next_yax: get the next graph yax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr pos: the yax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return yax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_yax(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_prev_yax: get the previous graph yax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr pos: the yax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return yax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_yax(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_yax_at: get the graph yax at position.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return yax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_yax_at(link_t_ptr ptr, int index);

/*
@FUNCTION get_yax: find graph yax by name.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT const tchar_t* yname: the yax name token.
@RETURN link_t_ptr: return yax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_yax(link_t_ptr ptr, const tchar_t* yname);

/*
@FUNCTION get_xax: find graph xax by name.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT const tchar_t* xname: the xax name token.
@RETURN link_t_ptr: return xax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_yax_by_id(link_t_ptr ptr, const tchar_t* cid);

/*
@FUNCTION get_yax_gax: get gax bind to the yax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr ylk: the yax link component.
@RETURN link_t_ptr: return gax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_yax_gax(link_t_ptr ptr, link_t_ptr ylk);

/*
@FUNCTION get_graph_xaxset: get xax set.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN link_t_ptr: return gax set link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_graph_xaxset(link_t_ptr ptr);

/*
@FUNCTION insert_xax: add a xax to graph.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr pos: the xax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new xax link component.
*/
XDL_API link_t_ptr insert_xax(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_xax_count: counting the xaxs in graph.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN int: return the number of xaxs.
*/
XDL_API int get_xax_count(link_t_ptr ptr);

/*
@FUNCTION get_xax_checked_count: counting the checked xaxs in graph.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN int: return the number of xaxs.
*/
XDL_API int get_xax_checked_count(link_t_ptr ptr);

/*
@FUNCTION get_xax_at: get the xax at position.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT int pos: the zero based position.
@RETURN link_t_ptr: return the xax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_xax_at(link_t_ptr ptr, int pos);

/*
@FUNCTION delete_xax: delete the xax.
@INPUT link_t_ptr xlk: the xax link component.
@RETURN void: none.
*/
XDL_API void delete_xax(link_t_ptr xlk);

/*
@FUNCTION get_next_xax: get the next xax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr xlk: the xax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return xax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_xax(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_prev_xax: get the previous xax.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr xlk: the xax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return xax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_xax(link_t_ptr ptr, link_t_ptr xlk);

/*
@FUNCTION get_xax: find graph xax by name.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT const tchar_t* xname: the xax name token.
@RETURN link_t_ptr: return xax link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_xax(link_t_ptr ptr, const tchar_t* xname);

/*
@FUNCTION get_xax_storage_ptr: find the xax storage by key.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT const tchar_t* key: the key token.
@RETURN const tchar_t*: return the storage token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_xax_storage_ptr(link_t_ptr xlk, const tchar_t* key);

/*
@FUNCTION set_xax_storage: add a storage to xax.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT const tchar_t* key: the key token.
@INPUT const tchar_t* val: the value token.
@RETURN void: none.
*/
XDL_API void set_xax_storage(link_t_ptr xlk, const tchar_t* key, const tchar_t* val);

/*
@FUNCTION get_update_xax_count: counting the update state xaxs.
@INPUT link_t_ptr xlk: the xax link component.
@RETURN int: return the number of xaxs.
*/
XDL_API int get_update_xax_count(link_t_ptr ptr);

/*
@FUNCTION refresh_graph_xaxset: clear the xax set state.
@INPUT link_t_ptr ptr: the graph link component.
@RETURN void: none.
*/
XDL_API void refresh_graph_xaxset(link_t_ptr ptr);

/*
@FUNCTION set_coor_numeric: set the coor numeric.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@INPUT double dbl: the numeric.
@RETURN void: none.
*/
XDL_API void set_coor_numeric(link_t_ptr xlk, link_t_ptr ylk, double dbl);

/*
@FUNCTION get_coor_numeric: get the coor numeric.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@RETURN double: return the coor numeric.
*/
XDL_API double get_coor_numeric(link_t_ptr xlk,link_t_ptr ylk);

/*
@FUNCTION set_coor_text: set the coor text.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@INPUT const tchar_t* token: the text string token.
@INPUT int len: the length of text in characters.
@RETURN void: none.
*/
XDL_API void set_coor_text(link_t_ptr xlk, link_t_ptr ylk, const tchar_t* token, int len);

/*
@FUNCTION get_coor_text_ptr: get the coor text token.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@RETURN const tchar_t*: return the text token.
*/
XDL_API const tchar_t* get_coor_text_ptr(link_t_ptr xlk, link_t_ptr ylk);

/*
@FUNCTION get_coor_text: copy the coor text token.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@OUTPUT tchar_t* buf: the string buffer for returning coor text.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters copied.
*/
XDL_API int get_coor_text(link_t_ptr xlk, link_t_ptr ylk, tchar_t* buf, int max);

/*
@FUNCTION get_coor_storage_ptr: get the coor storage value by key.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@INPUT const tchar_t* key: the key token.
@RETURN const tchar_t*: return the storage value token.
*/
XDL_API const tchar_t* get_coor_storage_ptr(link_t_ptr xlk, link_t_ptr ylk, const tchar_t* key);

/*
@FUNCTION set_coor_storage: add a storage to coor.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@INPUT const tchar_t* key: the key token.
@INPUT const tchar_t* val: the value token.
@RETURN void: none.
*/
XDL_API void set_coor_storage(link_t_ptr xlk, link_t_ptr ylk, const tchar_t* key, const tchar_t* val);

/*
@FUNCTION set_coor_dirty: set the coor state.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@INPUT bool_t b: nonzero for dirty, zero for clean.
@RETURN void: none.
*/
XDL_API void set_coor_dirty(link_t_ptr rlk, link_t_ptr clk, bool_t b);

/*
@FUNCTION get_coor_dirty: get the coor state.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@RETURN bool_t: return nonzero for dirty, zero for clean..
*/
XDL_API bool_t get_coor_dirty(link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION set_xax_clean: set xax state to clean.
@INPUT link_t_ptr xlk: the xax link component.
@RETURN void: none.
*/
XDL_API void set_xax_clean(link_t_ptr rlk);

/*
@FUNCTION set_xax_dirty: set xax state to dirty.
@INPUT link_t_ptr xlk: the xax link component.
@RETURN void: none.
*/
XDL_API void set_xax_dirty(link_t_ptr rlk);

/*
@FUNCTION calc_graph_gax_base: calcing the graph gax base line.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr glk: the gax link component.
@OUTPUT float* pmidd: for return middle line value.
@OUTPUT float* pstep: for return step value.
@RETURN void: none.
*/
XDL_API void calc_graph_gax_base(link_t_ptr ptr, link_t_ptr glk, float* pmidd, float* pstep);

#ifdef	__cplusplus
}
#endif

#endif //XDLVIEW

#endif //GRAPHDOC_H