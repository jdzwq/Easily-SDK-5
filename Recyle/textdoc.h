/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text document

	@module	textdoc.h | text document interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#ifndef _TEXTDOC_H
#define _TEXTDOC_H

#include "xdldef.h"
#include "xdlattr.h"


XDL_API link_t_ptr create_text_doc();

XDL_API void destroy_text_doc(link_t_ptr ptr);

XDL_API void clear_text_doc(link_t_ptr ptr);

XDL_API bool_t is_text_item(link_t_ptr ptr,link_t_ptr ilk);

XDL_API link_t_ptr insert_text_item(link_t_ptr ilk,const tchar_t* tag, link_t_ptr pos);

XDL_API void delete_text_item(link_t_ptr ilk);

XDL_API void delete_text_child_item(link_t_ptr ilk);

XDL_API int get_text_child_item_count(link_t_ptr ilk);

XDL_API link_t_ptr get_text_first_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_text_last_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_text_parent_item(link_t_ptr ilk);

XDL_API link_t_ptr get_text_next_sibling_item(link_t_ptr ilk);

XDL_API link_t_ptr get_text_prev_sibling_item(link_t_ptr ilk);

#endif /*_TEXTDOC_H*/
