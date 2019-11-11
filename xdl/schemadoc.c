/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc schemaema document

	@module	schemaemadoc.c | implement file

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

#include "schemadoc.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_schema_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_SCHEMA, -1);

	return ptr;
}

void destroy_schema_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_schema_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_SCHEMA, -1, 0) == 0) ? 1 : 0;
}

bool_t is_empty_schema(link_t_ptr ptr)
{
	link_t_ptr slk_rowset, slk_row, slk_ref;

	if (!ptr)
		return 1;

	slk_rowset = get_schema_next_element(ptr, LINK_FIRST);
	while (slk_rowset)
	{
		if (is_schema_element(slk_rowset))
			break;

		slk_rowset = get_schema_next_element(ptr, slk_rowset);
	}

	if (!slk_rowset)
		return 1;

	slk_row = get_schema_next_element(slk_rowset, LINK_FIRST);
	while (slk_row)
	{
		if (is_null(get_schema_element_name_ptr(slk_row)))
		{
			slk_ref = get_schema_element(ptr, get_schema_element_ref_ptr(slk_row));
			if (slk_ref && is_schema_declare(slk_ref))
			{
				slk_row = slk_ref;
				break;
			}
		}
		else if (is_schema_element(slk_row))
		{
			break;
		}

		slk_row = get_schema_next_element(slk_rowset, slk_row);
	}

	if (!slk_row)
		return 1;

	return (get_schema_element_count(slk_row)) ? 0 : 1;
}

void clear_schema_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

int get_schema_element_count(link_t_ptr nlk)
{
	return get_dom_child_node_count(nlk);
}

bool_t is_schema_declare(link_t_ptr nlk)
{
	return (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_SCHEMA_DECLARE, -1, 0) == 0) ? 1 : 0;
}

bool_t is_schema_attribute(link_t_ptr nlk)
{
	return (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_SCHEMA_ATTRIBUTE, -1, 0) == 0) ? 1 : 0;
}

bool_t is_schema_element(link_t_ptr nlk)
{
	return (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_SCHEMA_ELEMENT, -1, 0) == 0) ? 1 : 0;
}

link_t_ptr insert_schema_element(link_t_ptr parent, link_t_ptr pos)
{
	link_t_ptr nlk;

	nlk = insert_dom_node(parent, pos);
	set_dom_node_name(nlk, DOC_SCHEMA_ELEMENT, -1);

	return nlk;
}

link_t_ptr insert_schema_attribute(link_t_ptr parent, link_t_ptr pos)
{
	link_t_ptr nlk;

	nlk = insert_dom_node(parent, pos);
	set_dom_node_name(nlk, DOC_SCHEMA_ATTRIBUTE, -1);

	return nlk;
}

link_t_ptr insert_schema_declare(link_t_ptr parent, link_t_ptr pos)
{
	link_t_ptr nlk;

	nlk = insert_dom_node(parent, pos);
	set_dom_node_name(nlk, DOC_SCHEMA_DECLARE, -1);

	return nlk;
}

void delete_schema_element(link_t_ptr nlk)
{
	delete_dom_node(nlk);
}

link_t_ptr get_schema_parent_element(link_t_ptr nlk)
{
	return get_dom_parent_node(nlk);
}

link_t_ptr get_schema_next_element(link_t_ptr parent, link_t_ptr nlk)
{
	if (nlk == LINK_FIRST)
		return get_dom_first_child_node(parent);
	else if (nlk == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(nlk);
}

link_t_ptr get_schema_prev_element(link_t_ptr parent, link_t_ptr nlk)
{
	if (nlk == LINK_FIRST)
		return NULL;
	else if (nlk == LINK_LAST)
		return get_dom_last_child_node(parent);
	else
		return get_dom_prev_sibling_node(nlk);
}

link_t_ptr get_schema_element(link_t_ptr nlk, const tchar_t* key)
{
	return find_dom_node_by_attr(nlk, 1, ATTR_NAME, -1, key, -1);
}

#endif /*XDL_SUPPORT_DOC*/