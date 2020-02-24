/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc news document

	@module	newsdoc.h | news document interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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

#ifndef _NEWSDOC_H
#define _NEWSDOC_H

#include "xdldef.h"
#include "xdlattr.h"

/***********************************************************************************************************/
#define set_news_id(ptr,val)							set_dom_node_attr(ptr,ATTR_ID,-1,val,-1)

#define get_news_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_ID,-1)

#define set_news_time(ptr,val)							set_dom_node_attr(ptr,ATTR_TIME,-1,val,-1)

#define get_news_time_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_TIME,-1)

/*************************************************document node name****************************************/

XDL_API link_t_ptr create_news_doc();

XDL_API void destroy_news_doc(link_t_ptr ptr);

XDL_API bool_t is_news_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_news_title_node(link_t_ptr ptr);

XDL_API link_t_ptr get_news_text_node(link_t_ptr ptr);

XDL_API link_t_ptr get_news_attachment_node(link_t_ptr ptr);

#endif //_NEWSDOC_H