/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc menu document

	@module	menudoc.h | menu document interface file

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

#ifndef _MENUDOC_H
#define _MENUDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/********************************************serial enable attributes*****************************************/
#define get_menu_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_menu_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define get_menu_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_menu_style(ptr,token)							set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_menu_show_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_SHOW,-1)

#define set_menu_show(ptr,token)							set_dom_node_attr(ptr,ATTR_SHOW,-1,token,-1)

#define set_menu_layer(ptr,lay)								set_dom_node_attr(ptr,ATTR_LAYER,-1,lay,-1)

#define get_menu_layer_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_LAYER,-1)

#define set_menu_icon_span(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)

#define get_menu_icon_span(ptr)								get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)

#define get_menu_item_name_ptr(ilk)							get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_menu_item_name(ilk,token)						set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_menu_item_iid(ilk)								get_dom_node_attr_integer(ilk,ATTR_ID)

#define set_menu_item_iid(ilk,n)							set_dom_node_attr_integer(ilk,ATTR_ID,n)

#define set_menu_item_title(ilk,token)						set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

#define get_menu_item_title_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)

#define get_menu_item_image_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_IMAGE,-1)

#define set_menu_item_image(ilk,token)						set_dom_node_attr(ilk,ATTR_IMAGE,-1,token,-1)

/********************************************runtime enable attributes******************************************/
#define set_menu_images(ptr,images)							set_dom_node_images(ptr,images)

#define get_menu_images(ptr)								get_dom_node_images(ptr)

#define set_menu_item_checked(ilk,n)						set_dom_node_mask_check(ilk,MSK_CHECKED,n)

#define get_menu_item_checked(ilk)							get_dom_node_mask_check(ilk,MSK_CHECKED)
/*******************************************runtime functin*****************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_menu_doc(void);

XDL_API void destroy_menu_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_menu_itemset(link_t_ptr ptr);

XDL_API void clear_menu_doc(link_t_ptr ptr);

XDL_API bool_t is_menu_doc(link_t_ptr ptr);

XDL_API bool_t is_menu_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API int get_menu_item_count(link_t_ptr ptr);

XDL_API link_t_ptr get_menu_next_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_menu_prev_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_menu_item(link_t_ptr ptr, const tchar_t* sz_name);

XDL_API link_t_ptr get_menu_item_by_iid(link_t_ptr ptr, int iid);

XDL_API link_t_ptr insert_menu_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API void delete_menu_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif	/*_MENUDOC_H*/