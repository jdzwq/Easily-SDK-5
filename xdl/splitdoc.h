/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc split document

	@module	splitdoc.h | interface file

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

#ifndef _SPLITDOC_H
#define _SPLITDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER layer: string.
@GET get_split_item_layer_ptr: get the splitor layer.
*/
#define get_split_item_layer_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_LAYER,-1)
/*
@PROPER layer: string.
@SET set_split_item_layer: set the splitor layer.
*/
#define set_split_item_layer(ilk,token)					set_dom_node_attr(ilk,ATTR_LAYER,-1,token,-1)
/*
@PROPER ratio: string.
@SET set_split_item_ratio: set the splitor ratio.
*/
#define set_split_item_ratio(ilk,token)					set_dom_node_attr(ilk,ATTR_RATIO,-1,token,-1)
/*
@PROPER ratio: string.
@GET get_split_item_ratio_ptr: get the splitor ratio.
*/
#define get_split_item_ratio_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_RATIO,-1)
/*
@PROPER span: numeric.
@GET get_split_item_span: get the splitor span.
*/
#define get_split_item_span(ilk)						get_dom_node_attr_float(ilk, ATTR_ITEM_SPAN)
/*
@PROPER span: numeric.
@SET set_split_item_span: set the splitor span.
*/
#define set_split_item_span(ilk,n)						set_dom_node_attr_float(ilk,ATTR_ITEM_SPAN,n)
/*
@PROPER splited: boolean.
@SET set_split_item_splited: set the item is splited.
*/
#define set_split_item_splited(ilk,b)					set_dom_node_attr_boolean(ilk,ATTR_SPLITED,b) 
/*
@PROPER splited: boolean.
@GET get_split_item_splited: get the item is splited.
*/
#define get_split_item_splited(ilk)						get_dom_node_attr_boolean(ilk,ATTR_SPLITED)
/*
@PROPER fixed: boolean.
@SET set_split_item_fixed: set the item is fixed.
*/
#define set_split_item_fixed(ilk,b)						set_dom_node_attr_boolean(ilk,ATTR_FIXED,b) 
/*
@PROPER fixed: boolean.
@GET get_split_item_fixed: get the item is fixed.
*/
#define get_split_item_fixed(ilk)						get_dom_node_attr_boolean(ilk,ATTR_FIXED)

/*
@PROPER x: numeric.
@GET get_split_item_x: get the splitor x point.
*/
#define get_split_item_x(ilk)							get_dom_node_attr_float(ilk,ATTR_X)
/*
@PROPER x: nuneric.
@SET set_split_item_x: set the splitor x point.
*/
#define set_split_item_x(ilk,x)							set_dom_node_attr_float(ilk,ATTR_X,x)
/*
@PROPER y: numeric.
@GET get_split_item_y: get the splitor y point.
*/
#define get_split_item_y(ilk)							get_dom_node_attr_float(ilk,ATTR_Y)
/*
@PROPER y: nuneric.
@SET set_split_item_y: set the splitor y point.
*/
#define set_split_item_y(ilk,y)							set_dom_node_attr_float(ilk,ATTR_Y,y)
/*
@PROPER width: numeric.
@GET get_split_item_width: get the splitor width.
*/
#define get_split_item_width(ilk)						get_dom_node_attr_float(ilk,ATTR_WIDTH)
/*
@PROPER width: nuneric.
@SET set_split_item_width: set the splitor width.
*/
#define set_split_item_width(ilk,w)						set_dom_node_attr_float(ilk,ATTR_WIDTH,w)
/*
@PROPER height: numeric.
@GET get_split_item_height: get the splitor height.
*/
#define get_split_item_height(ilk)						get_dom_node_attr_float(ilk,ATTR_HEIGHT)
/*
@PROPER height: nuneric.
@SET set_split_item_height: set the splitor height.
*/
#define set_split_item_height(ilk,y)					set_dom_node_attr_float(ilk,ATTR_HEIGHT,y)
/*
@PROPER delta: var int.
@SET set_split_item_delta: set the splitor extract data.
*/
#define set_split_item_delta(ilk,ul)					set_dom_node_delta(ilk,(var_long)ul)
/*
@PROPER delta: var int.
@GET get_split_item_delta: get the splitor extract data.
*/
#define get_split_item_delta(ilk)						get_dom_node_delta(ilk)


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_split_doc: create a splitor document.
@RETURN link_t_ptr: return the splitor document link component.
*/
EXP_API link_t_ptr create_split_doc(void);

/*
@FUNCTION destroy_split_doc: destroy a splitor document.
@INPUT link_t_ptr ptr: the splitor link component.
@RETURN void: none.
*/
EXP_API void destroy_split_doc(link_t_ptr ptr);

/*
@FUNCTION is_split_doc: test is splitor document.
@INPUT link_t_ptr ptr: the splitor link component.
@RETURN bool_t: return nonzero for being a splitor document, otherwise return zero.
*/
EXP_API bool_t is_split_doc(link_t_ptr ptr);

/*
@FUNCTION clear_split_doc: clear the splitor document.
@INPUT link_t_ptr ptr: the splitor link component.
@RETURN void: none.
*/
EXP_API void clear_split_doc(link_t_ptr ptr);

/*
@FUNCTION is_split_item: test is splitor item node.
@INPUT link_t_ptr ptr: the splitor link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a item node, otherwise return zero.
*/
EXP_API bool_t is_split_item(link_t_ptr ptr,link_t_ptr ilk);

/*
@FUNCTION split_item: split the item.
@INPUT link_t_ptr ilk: the item link component.
@INPUT bool_t b_horz: nonzero for horizonal spliting, zero for vertical spliting.
@RETURN void: none.
*/
EXP_API void split_item(link_t_ptr ilk, bool_t b_horz);

/*
@FUNCTION unsplit_item: unsplit the item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
EXP_API void unsplit_item(link_t_ptr ilk);

/*
@FUNCTION enum_split_item: enm the items in splitor document.
@INPUT link_t_ptr ptr: the splitor link component.
@INPUT CALLBACK_ENUMLINK pf: the enumeration callback function, if the function return zero, enumeration will be breaked.
@INPUT void* param: the parameter translate into callback function.
@RETURN link_t_ptr: return the splitor item link component breaked at.
*/
EXP_API link_t_ptr enum_split_item(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

/*
@FUNCTION get_split_first_child_item: get the first child item.
@INPUT link_t_ptr ilk: the parent link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_split_first_child_item(link_t_ptr ilk);

/*
@FUNCTION get_split_last_child_item: get the last child item.
@INPUT link_t_ptr ilk: the parent link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_split_last_child_item(link_t_ptr ilk);

/*
@FUNCTION get_split_parent_item: get the parent item.
@INPUT link_t_ptr ilk: the child link component.
@RETURN link_t_ptr: return the parent item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_split_parent_item(link_t_ptr ilk);

/*
@FUNCTION get_split_next_sibling_item: get the next sibling item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_split_next_sibling_item(link_t_ptr ilk);

/*
@FUNCTION get_split_prev_sibling_item: get the previous sibling item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_split_prev_sibling_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_SPLITVIEW_H*/
