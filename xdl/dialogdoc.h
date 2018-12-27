/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dialog document

	@module	dialogdoc.h | dialog document interface file

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

#ifndef _DIALOGDOC_H
#define _DIALOGDOC_H

#include "xdldef.h"
#include "docattr.h"

#ifdef XDL_SUPPORT_DOC

/*******************************************serial enable attributes********************************************/
#define set_dialog_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_dialog_name_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_dialog_id(ptr,token)						set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)

#define get_dialog_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_dialog_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define get_dialog_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define set_dialog_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_dialog_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define get_dialog_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)

#define set_dialog_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)

#define get_dialog_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)

#define set_dialog_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)

#define get_dialog_item_class_ptr(ilk)					get_dom_node_name_ptr(ilk)

#define set_dialog_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_dialog_item_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_dialog_item_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)

#define get_dialog_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

#define set_dialog_item_text(ilk,token,len)				set_dom_node_text(ilk,token,len)

#define get_dialog_item_text_ptr(ilk)					get_dom_node_text_ptr(ilk)

#define set_dialog_item_style(ilk,token)				set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)

#define get_dialog_item_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)

#define get_dialog_item_x(ilk)							get_dom_node_attr_float(ilk,ATTR_X)

#define set_dialog_item_x(ilk,xm)						set_dom_node_attr_float(ilk,ATTR_X,xm)

#define get_dialog_item_y(ilk)							get_dom_node_attr_float(ilk,ATTR_Y)

#define set_dialog_item_y(ilk,ym)						set_dom_node_attr_float(ilk,ATTR_Y,ym)

#define set_dialog_item_width(ilk,n)					set_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH,n)

#define get_dialog_item_width(ilk)						get_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH)

#define set_dialog_item_height(ilk,n)					set_dom_node_attr_float(ilk,ATTR_ITEM_HEIGHT,n)

#define get_dialog_item_height(ilk)						get_dom_node_attr_float(ilk,ATTR_ITEM_HEIGHT)

#define get_dialog_item_taborder(ilk)					get_dom_node_attr_integer(ilk,ATTR_TABORDER)

#define set_dialog_item_taborder(ilk,n)					set_dom_node_attr_integer(ilk,ATTR_TABORDER,n)

/**********************************************************************************************************/
#define set_dialog_item_delta(ilk,ul)					set_dom_node_delta(ilk,(var_long)ul)

#define get_dialog_item_delta(ilk)						get_dom_node_delta(ilk)

#define set_dialog_item_selected(flk,b)					set_dom_node_mask_check(flk,MSK_CHECKED,b)

#define get_dialog_item_selected(flk)					get_dom_node_mask_check(flk,MSK_CHECKED) 

/*********************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_dialog_doc(void);

XDL_API void destroy_dialog_doc(link_t_ptr ptr);

XDL_API void clear_dialog_doc(link_t_ptr ptr);

XDL_API bool_t is_dialog_doc(link_t_ptr ptr);

XDL_API bool_t is_dialog_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void reset_dialog_taborder(link_t_ptr ptr);

XDL_API link_t_ptr get_dialog_itemset(link_t_ptr ptr);

XDL_API link_t_ptr insert_dialog_item(link_t_ptr ptr,const tchar_t* sz_class);

XDL_API link_t_ptr get_dialog_next_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_dialog_prev_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_dialog_item(link_t_ptr ptr, const tchar_t* sz_name);

XDL_API int get_dialog_item_count(link_t_ptr ptr);

XDL_API int get_dialog_item_count_by_class(link_t_ptr ptr, const tchar_t* szClass);

XDL_API int get_dialog_item_selected_count(link_t_ptr ptr);

XDL_API void delete_dialog_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //DIALOGDOC_H