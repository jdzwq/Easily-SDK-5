/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc trie tree document

	@module	trietree.h | interface file

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

#ifndef _TRIETREE_H
#define _TRIETREE_H

#include "xdldef.h"

/**********************************************************************************************************************/

typedef bool_t(*PF_ENUM_TRIE_TREE)(const tchar_t* key, int len, var_long delta, void* p);

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_trie_tree(void);

	XDL_API void destroy_trie_tree(link_t_ptr trie);

	XDL_API link_t_ptr insert_trie_node(link_t_ptr trie, const tchar_t* key, int len, tchar_t kfeed);

	XDL_API void delete_trie_node(link_t_ptr trie, const tchar_t* key, int len, tchar_t kfeed);

	XDL_API link_t_ptr find_trie_node(link_t_ptr trie, const tchar_t* key, int len, tchar_t kfeed);

	XDL_API link_t_ptr get_trie_from_node(link_t_ptr node);

	XDL_API bool_t is_trie_leaf(link_t_ptr node);

	XDL_API link_t_ptr enum_trie_tree(link_t_ptr trie, tchar_t kfeed, PF_ENUM_TRIE_TREE pf, void* param);

	XDL_API var_long get_trie_node_delta(link_t_ptr node);

	XDL_API void set_trie_node_delta(link_t_ptr node, var_long delta);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_trie_tree();
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_TRIETREE_H*/
