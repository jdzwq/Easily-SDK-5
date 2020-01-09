/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl file tree document

	@module	filetree.c | xdl file tree implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#include "filetree.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_file_tree()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_FILETREE_DIRECTORY, -1);

	return ptr;
}

void destroy_file_tree(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_file_tree(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

bool_t is_file_tree(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_FILETREE_DIRECTORY, -1, 0) == 0) ? 1 : 0;
}

bool_t is_file_item(link_t_ptr ptr, link_t_ptr ilk)
{
	if (!ilk)
		return 0;

	return is_dom_child_node(ptr, ilk) && (compare_text(get_dom_node_name_ptr(ilk),-1,DOC_FILETREE_FILE,-1,0) == 0);
}

bool_t is_directory_item(link_t_ptr ptr, link_t_ptr ilk)
{
	if (!ilk)
		return 0;

	return is_dom_child_node(ptr, ilk) && (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_FILETREE_DIRECTORY, -1, 0) == 0);
}

void sort_file_tree(link_t_ptr ptr,const tchar_t* attr, int desc)
{
	sort_dom_child_node_by_attr(ptr, attr, desc);
}

int get_file_item_count(link_t_ptr ptr)
{
	return get_dom_child_node_count(ptr);
}

link_t_ptr	get_file_item(link_t_ptr ptr, const tchar_t* key, int klen)
{
	return find_dom_node_by_attr(ptr, 0, ATTR_NAME, -1, key, klen);
}

void delete_file_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr get_file_next_item(link_t_ptr ptr, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_file_prev_item(link_t_ptr ptr, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

link_t_ptr insert_file_item(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr nlk;
	
	if (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_FILETREE_DIRECTORY, -1, 0) != 0)
		return NULL;

	nlk = insert_dom_node(ptr, pos);
	set_dom_node_name(nlk, DOC_FILETREE_FILE,-1);

	return nlk;
}

link_t_ptr insert_directory_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr nlk;

	if (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_FILETREE_DIRECTORY, -1, 0) != 0)
		return NULL;

	nlk = insert_dom_node(ptr, pos);
	set_dom_node_name(nlk, DOC_FILETREE_DIRECTORY,-1);

	return nlk;
}

void set_file_item_info(link_t_ptr ilk, const file_info_t* pfi)
{
	tchar_t fetag[NUM_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	set_dom_node_name(ilk, ((pfi->is_dir) ? DOC_FILETREE_DIRECTORY : DOC_FILETREE_FILE), -1);

	set_file_item_name(ilk, pfi->file_name);

	format_gmttime(&pfi->create_time, ftime);
	set_file_item_createtime(ilk, ftime);

	format_gmttime(&pfi->access_time, ftime);
	set_file_item_accesstime(ilk, ftime);

	format_gmttime(&pfi->write_time, ftime);
	set_file_item_writetime(ilk, ftime);

	if (!pfi->is_dir)
	{
		format_longlong(pfi->high_size, pfi->low_size, fsize);
		set_file_item_size(ilk, fsize);

		set_file_item_etag(ilk, pfi->file_etag);
	}
}

void get_file_item_info(link_t_ptr ilk, file_info_t* pfi)
{
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	pfi->is_dir = (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_FILETREE_DIRECTORY, -1, 0) == 0) ? 1 : 0;

	xsncpy(pfi->file_name, get_file_item_name_ptr(ilk), NAME_LEN);

	parse_gmttime(&pfi->write_time, get_file_item_writetime_ptr(ilk));
	parse_gmttime(&pfi->access_time, get_file_item_accesstime_ptr(ilk));
	parse_gmttime(&pfi->create_time, get_file_item_createtime_ptr(ilk));
	
	if (!pfi->is_dir)
	{
		xsncpy(pfi->file_etag, get_file_item_etag_ptr(ilk), RES_LEN);
		xsncpy(fsize, get_file_item_size_ptr(ilk), NUM_LEN);
		parse_longlong(&pfi->high_size, &pfi->low_size, fsize);
	}
}

#endif /*XDL_SUPPORT_DOC*/