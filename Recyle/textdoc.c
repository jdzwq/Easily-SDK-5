/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text document

	@module	textdoc.c | text document implement file

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

#include "textdoc.h"
#include "xdllink.h"
#include "hashtable.h"
#include "stacktable.h"
#include "xmldoc.h"
#include "domdoc.h"
#include "xdlutil.h"
#include "xdlstr.h"
#include "xdlmem.h"

link_t_ptr create_text_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,YBB_TAG_STYLE,-1);

	return ptr;
}

void destroy_text_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_text_doc(link_t_ptr ptr)
{
	delete_dom_child_node(ptr);
}

bool_t is_text_item(link_t_ptr ptr,link_t_ptr ilk)
{
	return is_dom_child_node(ptr,ilk);
}

link_t_ptr insert_text_item(link_t_ptr ilk, const tchar_t* tag, link_t_ptr pos)
{
	link_t_ptr tlk;

	tlk = insert_dom_node(ilk,pos);

	set_dom_node_name(tlk,tag,-1);

	return tlk;
}

void delete_text_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

void delete_text_child_item(link_t_ptr ilk)
{
	delete_dom_child_node(ilk);
}

int get_text_child_item_count(link_t_ptr ilk)
{
	return get_dom_child_node_count(ilk);
}

link_t_ptr get_text_first_child_item(link_t_ptr ilk)
{
	return get_dom_first_child_node(ilk);
}

link_t_ptr get_text_last_child_item(link_t_ptr ilk)
{
	return get_dom_last_child_node(ilk);
}

link_t_ptr get_text_parent_item(link_t_ptr ilk)
{
	return get_dom_parent_node(ilk);
}

link_t_ptr get_text_next_sibling_item(link_t_ptr ilk)
{
	return get_dom_next_sibling_node(ilk);
}

link_t_ptr get_text_prev_sibling_item(link_t_ptr ilk)
{
	return get_dom_prev_sibling_node(ilk);
}
