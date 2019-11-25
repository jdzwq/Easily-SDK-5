/***********************************************************************
	Easily xDesign v3.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xDesign document

	@module	xDesign implement file

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

#include "stdafx.h"
#include "_Define.h"
#include "_Project.h"

LINKPTR Project_Alloc()
{
	LINKPTR ptr_prj = create_meta_doc();

	set_meta_head_meta(ptr_prj, ATTR_PROJECT_RDS, -1, NULL, 0);
	set_meta_head_meta(ptr_prj, ATTR_PROJECT_SYN, -1, NULL, 0);

	LINKPTR body = create_dom_doc();
	set_dom_node_name(body, NODE_PROJECT, -1);

	attach_meta_body_node(ptr_prj, body);

	return ptr_prj;
}

void Project_Free(LINKPTR ptr_prj)
{
	destroy_meta_doc(ptr_prj);
}

bool_t Project_Save(LINKPTR ptr_prj, const tchar_t* szPath)
{
	return save_dom_doc_to_file(ptr_prj, NULL, szPath);
}

bool_t Project_Load(LINKPTR ptr_prj, const tchar_t* szPath)
{
	if (!load_dom_doc_from_file(ptr_prj, NULL, szPath))
	{
		return 0;
	}

	if (compare_text(get_dom_node_name_ptr(ptr_prj), -1, NODE_PROJECT, -1, 0) != 0)
	{
		return 0;
	}

	return 1;
}

bool_t Project_SetConfig(LINKPTR ptr_prj, const tchar_t* key, const tchar_t* val, int len)
{
	set_meta_head_meta(ptr_prj, key, -1, val, len);

	return 1;
}

bool_t Project_GetConfig(LINKPTR ptr_prj, const tchar_t* key, tchar_t* buf, int max)
{
	const tchar_t* token = get_meta_head_meta_ptr(ptr_prj, key, -1);

	xsncpy(buf, token, max);

	return 1;
}

bool_t Project_SetResource(LINKPTR ptr_prj, LINKPTR ptr_tree)
{
	LINKPTR body = get_meta_body_node(ptr_prj);

	if (!body)
		return 0;

	LINKPTR nlk_resource = find_dom_node_by_name(body, 0, NODE_PROJECT_RESOURCE, -1);
	if (!nlk_resource)
	{
		nlk_resource = insert_dom_node(body, LINK_LAST);
		set_dom_node_name(nlk_resource, NODE_PROJECT_RESOURCE, -1);
	}

	delete_dom_child_nodes(nlk_resource);

	LINKPTR tlk_parent = get_tree_first_child_item(ptr_tree);
	while (tlk_parent)
	{
		LINKPTR tlk_child = get_tree_first_child_item(tlk_parent);
		while (tlk_child)
		{
			LINKPTR nlk = insert_dom_node(nlk_resource, LINK_LAST);
			set_dom_node_name(nlk, get_tree_item_name_ptr(tlk_parent), -1);
			set_dom_node_text(nlk, get_tree_item_name_ptr(tlk_child), -1);

			tlk_child = get_tree_next_sibling_item(tlk_child);
		}
		tlk_parent = get_tree_next_sibling_item(tlk_parent);
	}

	return 1;
}

bool_t Project_GetResource(LINKPTR ptr_prj, LINKPTR ptr_tree)
{
	clear_tree_doc(ptr_tree);

	LINKPTR body = get_meta_body_node(ptr_prj);

	if (!body)
		return 0;

	LINKPTR nlk_resource = find_dom_node_by_name(body, 0, NODE_PROJECT_RESOURCE, -1);

	if (!nlk_resource)
		return 0;

	LINKPTR tlk_parent = NULL;
	tchar_t fname[RES_LEN] = { 0 };

	LINKPTR nlk = get_dom_first_child_node(nlk_resource);
	while (nlk)
	{
		tlk_parent = find_tree_item_by_name(ptr_tree, get_dom_node_name_ptr(nlk));
		if (!tlk_parent)
		{
			tlk_parent = insert_tree_item(ptr_tree, LINK_LAST);
			set_tree_item_name(tlk_parent, get_dom_node_name_ptr(nlk));
			set_tree_item_title(tlk_parent, get_dom_node_name_ptr(nlk));
			set_tree_item_icon(tlk_parent, ICON_NOTE);
		}

		LINKPTR tlk_child = find_tree_item_by_name(tlk_parent, get_dom_node_text_ptr(nlk));
		if (!tlk_child)
		{
			tlk_child = insert_tree_item(tlk_parent, LINK_LAST);
			set_tree_item_name(tlk_child, get_dom_node_text_ptr(nlk));
			split_path(get_dom_node_text_ptr(nlk), NULL, fname, NULL);
			set_tree_item_title(tlk_child, fname);
			set_tree_item_icon(tlk_child, ICON_SCHEMA);
		}

		nlk = get_dom_next_sibling_node(nlk);
	}

	return 1;
}

bool_t Project_SetDomain(LINKPTR ptr_prj, LINKPTR ptr_tree)
{
	LINKPTR body = get_meta_body_node(ptr_prj);

	if (!body)
		return 0;

	LINKPTR nlk_domain = find_dom_node_by_name(body, 0, NODE_PROJECT_DOMAIN, -1);

	if (!nlk_domain)
	{
		nlk_domain = insert_dom_node(body, LINK_LAST);
		set_dom_node_name(nlk_domain, NODE_PROJECT_DOMAIN, -1);
	}

	delete_dom_child_nodes(nlk_domain);

	LINKPTR tlk_parent = get_tree_first_child_item(ptr_tree);
	while (tlk_parent)
	{
		LINKPTR tlk_child = get_tree_first_child_item(tlk_parent);

		while (tlk_child)
		{
			LINKPTR nlk = insert_dom_node(nlk_domain, LINK_LAST);

			set_dom_node_name(nlk, get_tree_item_name_ptr(tlk_child), -1);
			set_dom_node_attr(nlk, ATTR_DATA_TYPE, -1, get_dom_node_attr_ptr(tlk_child, ATTR_DATA_TYPE, -1), -1);
			set_dom_node_attr(nlk, ATTR_DATA_LEN, -1, get_dom_node_attr_ptr(tlk_child, ATTR_DATA_LEN, -1), -1);
			set_dom_node_attr(nlk, ATTR_DATA_DIG, -1, get_dom_node_attr_ptr(tlk_child, ATTR_DATA_DIG, -1), -1);
			set_dom_node_attr(nlk, ATTR_CATEGORY, -1, get_tree_item_title_ptr(tlk_parent), -1);
			set_dom_node_text(nlk, get_dom_node_attr_ptr(tlk_child, ATTR_TITLE, -1), -1);

			tlk_child = get_tree_next_sibling_item(tlk_child);
		}

		tlk_parent = get_tree_next_sibling_item(tlk_parent);
	}

	return 1;
}

bool_t Project_GetDomain(LINKPTR ptr_prj, LINKPTR ptr_tree)
{
	clear_tree_doc(ptr_tree);

	LINKPTR body = get_meta_body_node(ptr_prj);

	if (!body)
		return 0;

	LINKPTR nlk_domain = find_dom_node_by_name(body, 0, NODE_PROJECT_DOMAIN, -1);

	if (!nlk_domain)
		return 0;

	tchar_t sz_title[250] = { 0 };

	LINKPTR nlk = get_dom_first_child_node(nlk_domain);
	while (nlk)
	{
		LINKPTR tlk_parent = find_tree_item_by_title(ptr_tree, get_dom_node_attr_ptr(nlk, ATTR_CATEGORY, -1));
		if (!tlk_parent)
		{
			tlk_parent = insert_tree_item(ptr_tree, LINK_LAST);
			set_tree_item_name(tlk_parent, ATTR_CATEGORY);
			set_tree_item_title(tlk_parent, get_dom_node_attr_ptr(nlk, ATTR_CATEGORY, -1));
		}

		LINKPTR tlk_child = insert_tree_item(tlk_parent, LINK_LAST);

		set_tree_item_name(tlk_child, get_dom_node_name_ptr(nlk));
		xsprintf(sz_title, _T("%s: %s"), get_dom_node_name_ptr(nlk), get_dom_node_text_ptr(nlk));
		set_tree_item_title(tlk_child,sz_title);
		set_dom_node_attr(tlk_child, ATTR_DATA_TYPE, -1, get_dom_node_attr_ptr(nlk, ATTR_DATA_TYPE, -1), -1);
		set_dom_node_attr(tlk_child, ATTR_DATA_LEN, -1, get_dom_node_attr_ptr(nlk, ATTR_DATA_LEN, -1), -1);
		set_dom_node_attr(tlk_child, ATTR_DATA_DIG, -1, get_dom_node_attr_ptr(nlk, ATTR_DATA_DIG, -1), -1);
		set_dom_node_attr(tlk_child, ATTR_TITLE, -1, get_dom_node_text_ptr(nlk), -1);

		nlk = get_dom_next_sibling_node(nlk);
	}

	return 1;
}
