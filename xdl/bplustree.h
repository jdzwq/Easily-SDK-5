/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc B+ tree document

	@module	bplustree.h | bplus tree interface file

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

#ifndef _BPLUSTREE_H
#define _BPLUSTREE_H

#include "xdldef.h"
#include "variant.h"


#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_bplus_tree(void);

	XDL_API void destroy_bplus_tree(link_t_ptr ptr);

	XDL_API void clear_bplus_tree(link_t_ptr ptr);

	XDL_API bool_t is_bplus_tree(link_t_ptr ptr);

	XDL_API bool_t insert_bplus_entity(link_t_ptr ptr, variant_t var, object_t val);

	XDL_API bool_t delete_bplus_entity(link_t_ptr ptr, variant_t var);

	XDL_API bool_t find_bplus_entity(link_t_ptr ptr, variant_t var, object_t val);

	XDL_API void enum_bplus_entity(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

	XDL_API link_t_ptr attach_bplus_index_table(link_t_ptr ptr, link_t_ptr ft);

	XDL_API link_t_ptr attach_bplus_data_table(link_t_ptr ptr, link_t_ptr ft);

	XDL_API void update_bplus_index_table(link_t_ptr ptr, bool_t b_save);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_bplus_tree();
	XDL_API void test_bplus_tree_file_table(const tchar_t* iname, const tchar_t* dname);
#endif

#ifdef	__cplusplus
}
#endif


#endif /*_BPLUSTREE_H*/
