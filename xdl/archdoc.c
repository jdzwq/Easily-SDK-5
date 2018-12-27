/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc document set

	@module	archdoc.c | archive document implement file

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

#include "archdoc.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_arch_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_ARCH, -1);

	return ptr;
}

void destroy_arch_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_arch_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

bool_t is_arch_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_ARCH, -1, 0) == 0) ? 1 : 0;
}

bool_t is_arch_catalog(link_t_ptr ptr, link_t_ptr ilk)
{
	if (!ilk)
		return 0;

	return is_dom_child_node(ptr, ilk) && (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_ARCH_CATALOG, -1, 0) == 0);
}

bool_t is_arch_document(link_t_ptr ptr, link_t_ptr ilk)
{
	if (!ilk)
		return 0;

	return is_dom_child_node(ptr, ilk) && (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_ARCH_DOCUMENT, -1, 0) == 0);
}

int get_arch_item_count(link_t_ptr ptr)
{
	return get_dom_child_node_count(ptr);
}

link_t_ptr	get_arch_item(link_t_ptr ptr, const tchar_t* iname)
{
	return find_dom_node_by_attr(ptr, 0, ATTR_NAME, -1, iname, -1);
}

void delete_arch_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr insert_arch_catalog(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr nlk;

	nlk = insert_dom_node(ptr, pos);
	set_dom_node_name(nlk, DOC_ARCH_CATALOG, -1);

	return nlk;
}

link_t_ptr insert_arch_document(link_t_ptr ptr, link_t_ptr pos, link_t_ptr doc)
{
	link_t_ptr nlk;

	nlk = insert_dom_node(ptr, pos);
	set_dom_node_name(nlk, DOC_ARCH_DOCUMENT, -1);

	attach_dom_node(nlk, LINK_LAST, doc);

	return nlk;
}

link_t_ptr attach_arch_document(link_t_ptr ilk, link_t_ptr doc)
{
	link_t_ptr nlk;

	nlk = get_dom_first_child_node(ilk);
	if (nlk)
	{
		detach_dom_node(ilk, nlk);
	}
	attach_dom_node(ilk, LINK_LAST, doc);

	return nlk;
}

link_t_ptr detach_arch_document(link_t_ptr ilk)
{
	link_t_ptr nlk;

	nlk = get_dom_first_child_node(ilk);
	if (nlk)
	{
		detach_dom_node(ilk, nlk);
	}

	return nlk;
}

link_t_ptr fetch_arch_document(link_t_ptr ilk)
{
	return get_dom_first_child_node(ilk);
}

link_t_ptr get_arch_first_child_item(link_t_ptr ilk)
{
	if(compare_text(get_dom_node_name_ptr(ilk), -1, DOC_ARCH_DOCUMENT, -1, 0) == 0)
		return NULL;

	return get_dom_first_child_node(ilk);
}

link_t_ptr get_arch_last_child_item(link_t_ptr ilk)
{
	if (compare_text(get_dom_node_name_ptr(ilk), -1, DOC_ARCH_DOCUMENT, -1, 0) == 0)
		return NULL;

	return get_dom_last_child_node(ilk);
}

link_t_ptr get_arch_parent_item(link_t_ptr ilk)
{
	return get_dom_parent_node(ilk);
}

link_t_ptr get_arch_next_sibling_item(link_t_ptr ilk)
{
	return get_dom_next_sibling_node(ilk);
}

link_t_ptr get_arch_prev_sibling_item(link_t_ptr ilk)
{
	return get_dom_prev_sibling_node(ilk);
}


#endif //XDLDOC