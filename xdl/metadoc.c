/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc meta document

	@module	metadoc.c | meta document implement file

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

#include "metadoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_meta_doc()
{
	link_t_ptr ptr,nlk_head;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_META,-1);

	nlk_head = insert_dom_node(ptr, LINK_LAST);
	set_dom_node_name(nlk_head, DOC_META_HEAD, -1);

	return ptr;
}

void destroy_meta_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_meta_doc(link_t_ptr ptr)
{
	clear_meta_head_nodes(ptr);
	clear_meta_body_nodes(ptr);
}

void clear_meta_head_nodes(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = get_meta_head_node(ptr);
	if (nlk)
	{
		delete_dom_child_nodes(ptr);
	}
}

void clear_meta_body_nodes(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = get_meta_body_node(ptr);
	if (nlk)
	{
		delete_dom_child_nodes(ptr);
	}
}

void set_meta_doc_name(link_t_ptr ptr, const tchar_t* sz_name)
{
	set_dom_node_name(ptr, sz_name, -1);
}

const tchar_t* get_meta_doc_name_ptr(link_t_ptr ptr)
{
	return get_dom_node_name_ptr(ptr);
}

link_t_ptr get_meta_head_node(link_t_ptr ptr)
{
	link_t_ptr nlk;
	const tchar_t* name;

	nlk = get_dom_first_child_node(ptr);
	while(nlk)
	{
		name = get_dom_node_name_ptr(nlk);
		if(compare_text(name,-1,DOC_META_HEAD,-1,1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return nlk;
}

link_t_ptr get_meta_body_node(link_t_ptr ptr)
{
	link_t_ptr nlk;
	const tchar_t* name;

	nlk = get_dom_first_child_node(ptr);
	while (nlk)
	{
		name = get_dom_node_name_ptr(nlk);
		if (compare_text(name, -1, DOC_META_BODY, -1, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return nlk;
}

void attach_meta_body_node(link_t_ptr ptr, link_t_ptr pnew)
{
	link_t_ptr nlk_body;

	nlk_body = get_meta_body_node(ptr);
	if (nlk_body)
	{
		detach_dom_node(ptr, nlk_body);
		destroy_dom_doc(nlk_body);
	}

	if (!pnew)
		return;

	set_meta_doc_name(ptr, get_dom_node_name_ptr(pnew));

	set_dom_node_name(pnew, DOC_META_BODY, -1);
	attach_dom_node(ptr, LINK_LAST, pnew);
}

link_t_ptr detach_meta_body_node(link_t_ptr ptr)
{
	link_t_ptr nlk_body;

	nlk_body = get_meta_body_node(ptr);
	if (!nlk_body)
		return NULL;

	nlk_body = detach_dom_node(ptr, nlk_body);
	set_dom_node_name(nlk_body, get_meta_doc_name_ptr(ptr), -1);

	return nlk_body;
}

void set_meta_head_title(link_t_ptr ptr, const tchar_t* sz_title, int len)
{
	link_t_ptr nlk_head, nlk;
	
	nlk_head = get_meta_head_node(ptr);
	if(!nlk_head)
		return;

	nlk = get_dom_first_child_node(nlk_head);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_META_TITLE, -1, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	if (!nlk)
	{
		nlk = insert_dom_node(nlk_head, LINK_FIRST);
		set_dom_node_name(nlk, DOC_META_TITLE, -1);
	}

	set_dom_node_text(nlk, sz_title, len);
}

const tchar_t* get_meta_head_title_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk_head, nlk;

	nlk_head = get_meta_head_node(ptr);
	if (!nlk_head)
		return 0;

	nlk = get_dom_first_child_node(nlk_head);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_META_TITLE, -1, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

int get_meta_head_title(link_t_ptr ptr, tchar_t* sz_buf, int max)
{
	link_t_ptr nlk_head, nlk;

	nlk_head = get_meta_head_node(ptr);
	if (!nlk_head)
		return 0;

	nlk = get_dom_first_child_node(nlk_head);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_META_TITLE, -1, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return (nlk) ? get_dom_node_text(nlk, sz_buf, max) : 0;
}

void set_meta_head_meta(link_t_ptr ptr, const tchar_t* sz_name, int nlen, const tchar_t* sz_content, int clen)
{
	link_t_ptr nlk_head, nlk;

	nlk_head = get_meta_head_node(ptr);
	if (!nlk_head)
		return;

	nlk = get_dom_first_child_node(nlk_head);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_META_META, -1, 1) == 0 && compare_text(get_dom_node_attr_ptr(nlk,ATTR_NAME,-1), -1, sz_name, nlen, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	if (!nlk)
	{
		nlk = insert_dom_node(nlk_head, LINK_LAST);
		set_dom_node_name(nlk, DOC_META_META, -1);
		set_dom_node_attr(nlk, ATTR_NAME, -1, sz_name, nlen);
	}

	set_dom_node_attr(nlk, ATTR_CONTENT, -1, sz_content, clen);
}

const tchar_t* get_meta_head_meta_ptr(link_t_ptr ptr, const tchar_t* sz_name, int nlen)
{
	link_t_ptr nlk_head, nlk;

	nlk_head = get_meta_head_node(ptr);
	if (!nlk_head)
		return 0;

	nlk = get_dom_first_child_node(nlk_head);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_META_META, -1, 1) == 0 && compare_text(get_dom_node_attr_ptr(nlk, ATTR_NAME, -1), -1, sz_name, nlen, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return (nlk) ? get_dom_node_attr_ptr(nlk, ATTR_CONTENT, -1) : 0;
}

int get_meta_head_meta(link_t_ptr ptr, const tchar_t* sz_name, int nlen, tchar_t* sz_buf, int max)
{
	link_t_ptr nlk_head, nlk;

	nlk_head = get_meta_head_node(ptr);
	if (!nlk_head)
		return 0;

	nlk = get_dom_first_child_node(nlk_head);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_META_META, -1, 1) == 0 && compare_text(get_dom_node_attr_ptr(nlk, ATTR_NAME, -1), -1, sz_name, nlen, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return (nlk) ? get_dom_node_attr(nlk, ATTR_CONTENT, -1, sz_buf, max) : 0;
}

#endif /*METADOC_H*/

