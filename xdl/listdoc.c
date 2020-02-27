/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc list document

	@module	listdoc.c | implement file

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

#include "listdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_list_attr(link_t_ptr ptr) 
{
	set_list_style(ptr, _T("font-size:9;text-align:center;line-align:center;"));

	set_list_item_height(ptr,18); 
	set_list_item_width(ptr,20); 
	set_list_icon_span(ptr, DEF_GDI_ICON_SPAN);
}

link_t_ptr create_list_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_LIST,-1);
	default_list_attr(ptr);

	return ptr;
}

void destroy_list_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_list_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

int get_list_child_item_count(link_t_ptr ilk)
{
	return get_dom_child_node_count(ilk);
}

link_t_ptr insert_list_item(link_t_ptr ilk,link_t_ptr pos)
{
	link_t_ptr blk;

	blk = insert_dom_node(ilk,pos);
	set_dom_node_name(blk,DOC_LIST_ITEM,-1);

	return blk;
}

void delete_list_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

int get_list_item_level(link_t_ptr ilk)
{
	return get_dom_node_level(ilk);
}

link_t_ptr get_list_parent_item(link_t_ptr ilk)
{
	return get_dom_parent_node(ilk);
}

link_t_ptr get_list_first_child_item(link_t_ptr ilk)
{
	return get_dom_first_child_node(ilk);
}

link_t_ptr get_list_last_child_item(link_t_ptr ilk)
{
	return get_dom_last_child_node(ilk);
}

link_t_ptr get_list_next_sibling_item(link_t_ptr ilk)
{
	return get_dom_next_sibling_node(ilk);
}

link_t_ptr get_list_prev_sibling_item(link_t_ptr ilk)
{
	return get_dom_prev_sibling_node(ilk);
}

link_t_ptr get_list_item_at(link_t_ptr ilk,int index)
{
	return get_dom_child_node_at(ilk,index);
}

bool_t is_list_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_LIST,-1,0) == 0)? 1 : 0;
}

bool_t is_list_item(link_t_ptr ptr,link_t_ptr plk)
{
	return is_dom_child_node(ptr,plk);
}

link_t_ptr list_doc_from_item(link_t_ptr ilk)
{
	link_t_ptr ptr;

	ptr = ilk;
	do{
		if(is_list_doc(ptr))
			break;
		ptr = get_list_parent_item(ptr);
	}while(ptr);

	return ptr;
}

bool_t is_list_file_item(link_t_ptr ilk)
{
	return (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_LIST_FILE, -1, 0) == 0) ? 1 : 0;
}

bool_t is_list_directory_item(link_t_ptr ilk)
{
	return (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_LIST_DIRECTORY, -1, 0) == 0) ? 1 : 0;
}

link_t_ptr insert_list_file_item(link_t_ptr ilk, link_t_ptr pos)
{
	link_t_ptr nlk;

	nlk = insert_list_item(ilk, LINK_LAST);
	set_dom_node_name(nlk, DOC_LIST_FILE, -1);

	return nlk;
}

link_t_ptr insert_list_directory_item(link_t_ptr ilk, link_t_ptr pos)
{
	link_t_ptr nlk;

	nlk = insert_list_item(ilk, LINK_LAST);
	set_dom_node_name(nlk, DOC_LIST_DIRECTORY, -1);

	return nlk;
}

void set_list_item_file_info(link_t_ptr ilk, const file_info_t* pfi)
{
	tchar_t fetag[NUM_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	set_dom_node_name(ilk, ((pfi->is_dir) ? DOC_LIST_DIRECTORY : DOC_LIST_FILE), -1);

	set_list_item_file_name(ilk, pfi->file_name);

	format_gmttime(&pfi->create_time, ftime);
	set_list_item_file_createtime(ilk, ftime);

	format_gmttime(&pfi->access_time, ftime);
	set_list_item_file_accesstime(ilk, ftime);

	format_gmttime(&pfi->write_time, ftime);
	set_list_item_file_writetime(ilk, ftime);

	if (!pfi->is_dir)
	{
		format_long(pfi->high_size, pfi->low_size, fsize);
		set_list_item_file_size(ilk, fsize);

		set_list_item_file_etag(ilk, pfi->file_etag);
	}
}

void get_list_item_file_info(link_t_ptr ilk, file_info_t* pfi)
{
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	pfi->is_dir = (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_LIST_DIRECTORY, -1, 0) == 0) ? 1 : 0;

	xsncpy(pfi->file_name, get_list_item_file_name_ptr(ilk), META_LEN);

	parse_gmttime(&pfi->write_time, get_list_item_file_writetime_ptr(ilk));
	parse_gmttime(&pfi->access_time, get_list_item_file_accesstime_ptr(ilk));
	parse_gmttime(&pfi->create_time, get_list_item_file_createtime_ptr(ilk));

	if (!pfi->is_dir)
	{
		xsncpy(pfi->file_etag, get_list_item_file_etag_ptr(ilk), RES_LEN);
		xsncpy(fsize, get_list_item_file_size_ptr(ilk), NUM_LEN);
		parse_long(&pfi->high_size, &pfi->low_size, fsize);
	}
}

#endif //XDLDOC