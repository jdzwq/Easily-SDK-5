/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc split document

	@module	splitdoc.h | split document interface file

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

/**************************************serial enable attributes**************************************/
#define get_split_item_layer_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_LAYER,-1)

#define set_split_item_layer(ilk,token)					set_dom_node_attr(ilk,ATTR_LAYER,-1,token,-1)

#define set_split_item_ratio(ilk,token)					set_dom_node_attr(ilk,ATTR_RATIO,-1,token,-1)

#define get_split_item_ratio_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_RATIO,-1)

#define get_split_item_span(ilk)						get_dom_node_attr_float(ilk, ATTR_ITEM_SPAN)

#define set_split_item_span(ilk,n)						set_dom_node_attr_float(ilk,ATTR_ITEM_SPAN,n)

#define set_split_item_splited(ilk,b)					set_dom_node_attr_boolean(ilk,ATTR_SPLITED,b) 

#define get_split_item_splited(ilk)						get_dom_node_attr_boolean(ilk,ATTR_SPLITED)

#define set_split_item_fixed(ilk,b)						set_dom_node_attr_boolean(ilk,ATTR_FIXED,b) 

#define get_split_item_fixed(ilk)						get_dom_node_attr_boolean(ilk,ATTR_FIXED)
/***************************************runtime enable attributes************************************/

#define get_split_item_x(ilk)							get_dom_node_attr_float(ilk,ATTR_X)

#define set_split_item_x(ilk,x)							set_dom_node_attr_float(ilk,ATTR_X,x)

#define get_split_item_y(ilk)							get_dom_node_attr_float(ilk,ATTR_Y)

#define set_split_item_y(ilk,y)							set_dom_node_attr_float(ilk,ATTR_Y,y)

#define get_split_item_width(ilk)						get_dom_node_attr_float(ilk,ATTR_WIDTH)

#define set_split_item_width(ilk,w)						set_dom_node_attr_float(ilk,ATTR_WIDTH,w)

#define get_split_item_height(ilk)						get_dom_node_attr_float(ilk,ATTR_HEIGHT)

#define set_split_item_height(ilk,y)					set_dom_node_attr_float(ilk,ATTR_HEIGHT,y)

#define set_split_item_delta(ilk,ul)					set_dom_node_delta(ilk,(var_long)ul)

#define get_split_item_delta(ilk)						get_dom_node_delta(ilk)

/*******************************************runtime function********************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_split_doc();

XDL_API void destroy_split_doc(link_t_ptr ptr);

XDL_API bool_t is_split_doc(link_t_ptr ptr);

XDL_API void clear_split_doc(link_t_ptr ptr);

XDL_API bool_t is_split_item(link_t_ptr ptr,link_t_ptr ilk);

XDL_API void split_item(link_t_ptr ilk, bool_t b_horz);

XDL_API void unsplit_item(link_t_ptr ilk);

XDL_API link_t_ptr enum_split_item(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

XDL_API link_t_ptr get_split_first_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_split_last_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_split_parent_item(link_t_ptr ilk);

XDL_API link_t_ptr get_split_next_sibling_item(link_t_ptr ilk);

XDL_API link_t_ptr get_split_prev_sibling_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_SPLITVIEW_H*/
