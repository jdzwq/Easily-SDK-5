/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc wsdl document

	@module	wsdldoc.c | wsdl document implement file

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

#include "wsdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_wsdl_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_WSDL,-1);

	return ptr;
}

void destroy_wsdl_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_wsdl_doc(link_t_ptr ptr)
{
	return  (compare_nons(get_dom_node_name_ptr(ptr), -1, DOC_WSDL, -1) == 0) ? 1 : 0;
}

void set_wsdl_default_xmlns(link_t_ptr ptr)
{
	set_dom_node_ns(ptr, NSWSDL, -1);
	set_dom_node_xmlns(ptr, XMLNS_WSDL, -1, XMLNS_WSDL_INSTANCE, -1);
	set_dom_node_xmlns(ptr, XMLNS_SOAP, -1, XMLNS_WSDL_SOAP, -1);
	set_dom_node_xmlns(ptr, XMLNS_XSD, -1, XMLNS_XSD_INSTANCE, -1);
}

link_t_ptr get_wsdl_next_message_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		ilk = get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		ilk = get_dom_next_sibling_node(pos);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_MESSAGE, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_next_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_prev_message_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		ilk = get_dom_last_child_node(ptr);
	else
		ilk = get_dom_prev_sibling_node(pos);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_MESSAGE, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_prev_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_next_porttype_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		ilk = get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		ilk = get_dom_next_sibling_node(pos);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_PORTTYPE, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_next_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_prev_porttype_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		ilk = get_dom_last_child_node(ptr);
	else
		ilk = get_dom_prev_sibling_node(pos);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_PORTTYPE, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_prev_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_next_binding_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		ilk = get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		ilk = get_dom_next_sibling_node(pos);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_BINDING, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_next_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_prev_binding_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		ilk = get_dom_last_child_node(ptr);
	else
		ilk = get_dom_prev_sibling_node(pos);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_BINDING, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_prev_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_next_service_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		ilk = get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		ilk = get_dom_next_sibling_node(pos);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_SERVICE, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_next_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_types_node(link_t_ptr ptr)
{
	link_t_ptr ilk;

	ilk = get_dom_first_child_node(ptr);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_TYPES, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_next_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr get_wsdl_service_node(link_t_ptr ptr)
{
	link_t_ptr ilk;

	ilk = get_dom_last_child_node(ptr);

	while (ilk)
	{
		if (compare_nons(get_dom_node_name_ptr(ilk), -1, DOC_WSDL_SERVICE, -1) == 0)
		{
			return ilk;
		}

		ilk = get_dom_prev_sibling_node(ilk);
	}

	return NULL;
}

link_t_ptr add_wsdl_types_node(link_t_ptr ptr)
{
	link_t_ptr ilk;
	
	ilk = get_wsdl_types_node(ptr);
	if (!ilk)
	{
		ilk = insert_dom_node(ptr, LINK_FIRST);
	}
	else
	{
		delete_dom_child_nodes(ilk);
	}

	set_dom_node_name(ilk, DOC_WSDL_TYPES, -1);
	set_dom_node_ns(ilk, NSWSDL, -1);

	return ilk;
}

link_t_ptr add_wsdl_types_schema(link_t_ptr ilk, const tchar_t* sz_tns)
{
	link_t_ptr plk;

	plk = insert_dom_node(ilk, LINK_LAST);
	set_dom_node_name(plk, DOC_SCHEMA, -1);
	set_dom_node_ns(plk, NSXSD, -1);
	if (!is_null(sz_tns))
	{
		set_dom_node_attr(plk, WSDL_ATTR_TARGETNAMESPACE, -1, sz_tns, -1);
		set_dom_node_attr(plk, _T("elementFormDefault"), -1, _T("unqualified"), -1);
	}

	return plk;
}

link_t_ptr add_wsdl_schema_element(link_t_ptr ilk, const tchar_t* sz_name, link_t_ptr sch)
{
	link_t_ptr plk_ele, plk_com, plk_seq, plk_ddt;
	link_t_ptr elk;
	tchar_t sz_withns[1024];

	plk_ele = insert_dom_node(ilk, LINK_LAST);
	set_dom_node_name(plk_ele, DOC_SCHEMA_ELEMENT, -1);
	set_dom_node_ns(plk_ele, NSXSD, -1);
	set_dom_node_attr(plk_ele, WSDL_ATTR_NAME, -1, sz_name, -1);

	plk_com = insert_dom_node(plk_ele, LINK_LAST);
	set_dom_node_name(plk_com, _T("complexType"), -1);
	set_dom_node_ns(plk_com, NSXSD, -1);

	plk_seq = insert_dom_node(plk_com, LINK_LAST);
	set_dom_node_name(plk_seq, _T("sequence"), -1);
	set_dom_node_ns(plk_seq, NSXSD, -1);

	elk = get_schema_next_element(sch, LINK_FIRST);
	while (elk)
	{
		plk_ddt = insert_dom_node(plk_seq, LINK_LAST);
		set_dom_node_name(plk_ddt, DOC_SCHEMA_ELEMENT, -1);
		set_dom_node_ns(plk_ddt, NSXSD, -1);
		set_dom_node_attr(plk_ddt, WSDL_ATTR_NAME, -1, get_schema_element_name_ptr(elk), -1);

		xsprintf(sz_withns, _T("%s:%s"), NSXSD, get_schema_element_type_ptr(elk));
		set_dom_node_attr(plk_ddt, WSDL_ATTR_TYPE, -1, sz_withns, -1);

		set_dom_node_attr(plk_ddt, _T("maxOccurs"), -1, _T("1"), -1);
		set_dom_node_attr(plk_ddt, _T("minOccurs"), -1, _T("0"), -1);

		elk = get_schema_next_element(sch, elk);
	}

	return plk_ele;
}

link_t_ptr add_wsdl_message_node(link_t_ptr ptr, const tchar_t* sz_name)
{
	link_t_ptr ilk, pos;

	pos = ilk = get_wsdl_next_message_node(ptr, LINK_FIRST);
	while (ilk)
	{
		if (compare_nons(get_dom_node_attr_ptr(ilk, WSDL_ATTR_NAME, -1), -1, sz_name, -1) == 0)
			break;

		pos = ilk;
		ilk = get_wsdl_next_message_node(ptr, ilk);
	}

	if (!ilk)
	{
		if (!pos)
			pos = LINK_LAST;
		ilk = insert_dom_node(ptr, pos);
	}
	else
	{
		delete_dom_child_nodes(ilk);
	}

	set_dom_node_name(ilk, DOC_WSDL_MESSAGE, -1);
	set_dom_node_ns(ilk, NSWSDL, -1);
	set_dom_node_attr(ilk, WSDL_ATTR_NAME, -1, sz_name, -1);

	return ilk;
}

link_t_ptr add_wsdl_message_part_param(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_type)
{
	link_t_ptr plk;
	tchar_t sz_withns[1024] = { 0 };

	plk = get_dom_first_child_node(ilk);
	while (plk)
	{
		if (compare_nons(get_dom_node_attr_ptr(plk, WSDL_ATTR_NAME, -1), -1, sz_name, -1) == 0)
			break;

		plk = get_dom_next_sibling_node(plk);
	}

	if (!plk)
	{
		plk = insert_dom_node(ilk, LINK_LAST);
	}
	else
	{
		delete_dom_child_nodes(plk);
	}

	set_dom_node_name(plk, WSDL_NODE_PART, -1);
	set_dom_node_ns(plk, NSWSDL, -1);
	set_dom_node_attr(plk, WSDL_ATTR_NAME, -1, sz_name, -1);
	xsprintf(sz_withns, _T("%s:%s"), NSXSD, sz_type);
	set_dom_node_attr(plk, WSDL_ATTR_TYPE, -1, sz_withns, -1);

	return plk;
}

link_t_ptr add_wsdl_message_part_element(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_element)
{
	link_t_ptr plk;
	tchar_t sz_withns[1024] = { 0 };

	plk = get_dom_first_child_node(ilk);
	while (plk)
	{
		if (compare_nons(get_dom_node_attr_ptr(plk, WSDL_ATTR_ELEMENT, -1), -1, sz_element, -1) == 0)
			break;

		plk = get_dom_next_sibling_node(plk);
	}

	if (!plk)
	{
		plk = insert_dom_node(ilk, LINK_LAST);
	}
	else
	{
		delete_dom_child_nodes(plk);
	}

	set_dom_node_name(plk, WSDL_NODE_PART, -1);
	set_dom_node_ns(plk, NSWSDL, -1);
	set_dom_node_attr(plk, WSDL_ATTR_NAME, -1, sz_name, -1);
	xsprintf(sz_withns, _T("%s:%s"), NSTNS, sz_element);
	set_dom_node_attr(plk, WSDL_ATTR_ELEMENT, -1, sz_withns, -1);

	return plk;
}

link_t_ptr add_wsdl_porttype_node(link_t_ptr ptr, const tchar_t* sz_name)
{
	link_t_ptr ilk,pos;

	pos = ilk = get_wsdl_next_porttype_node(ptr, LINK_FIRST);
	while (ilk)
	{
		if (compare_nons(get_dom_node_attr_ptr(ilk, WSDL_ATTR_NAME, -1), -1, sz_name, -1) == 0)
			break;

		pos = ilk;
		ilk = get_wsdl_next_porttype_node(ptr, ilk);
	}

	if (!ilk)
	{
		if (!pos)
			pos = LINK_LAST;
		ilk = insert_dom_node(ptr, pos);
	}
	else
	{
		delete_dom_child_nodes(ilk);
	}

	set_dom_node_name(ilk, DOC_WSDL_PORTTYPE, -1);
	set_dom_node_ns(ilk, NSWSDL, -1);
	set_dom_node_attr(ilk, WSDL_ATTR_NAME, -1, sz_name, -1);

	return ilk;
}

link_t_ptr add_wsdl_porttype_operation(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_inmsg, const tchar_t* sz_outmsg)
{
	link_t_ptr plk, nlk_wsdl;
	tchar_t sz_withns[1024] = { 0 };

	plk = get_dom_first_child_node(ilk);
	while (plk)
	{
		if (compare_nons(get_dom_node_attr_ptr(plk, WSDL_ATTR_NAME, -1), -1, sz_name, -1) == 0)
			break;

		plk = get_dom_next_sibling_node(plk);
	}

	if (!plk)
	{
		plk = insert_dom_node(ilk, LINK_LAST);
	}
	else
	{
		delete_dom_child_nodes(plk);
	}

	set_dom_node_name(plk, WSDL_NODE_OPERATION, -1);
	set_dom_node_ns(plk, NSWSDL, -1);
	set_dom_node_attr(plk, WSDL_ATTR_NAME, -1, sz_name, -1);

	nlk_wsdl = insert_dom_node(plk, LINK_LAST);
	set_dom_node_name(nlk_wsdl, WSDL_NODE_INPUT, -1);
	set_dom_node_ns(nlk_wsdl, NSWSDL, -1);
	xsprintf(sz_withns,_T("%s:%s"), NSTNS, sz_inmsg);
	set_dom_node_attr(nlk_wsdl, WSDL_ATTR_MESSAGE, -1, sz_withns, -1);

	nlk_wsdl = insert_dom_node(plk, LINK_LAST);
	set_dom_node_name(nlk_wsdl, WSDL_NODE_OUTPUT, -1);
	set_dom_node_ns(nlk_wsdl, NSWSDL, -1);
	xsprintf(sz_withns,_T("%s:%s"), NSTNS, sz_outmsg);
	set_dom_node_attr(nlk_wsdl, WSDL_ATTR_MESSAGE, -1, sz_withns, -1);

	return plk;
}

link_t_ptr add_wsdl_binding_node(link_t_ptr ptr, const tchar_t* sz_name, const tchar_t* sz_type)
{
	link_t_ptr ilk,pos;
	link_t_ptr plk;
	tchar_t sz_withns[1024];

	pos = ilk = get_wsdl_next_binding_node(ptr, LINK_FIRST);
	while (ilk)
	{
		if (compare_nons(get_dom_node_attr_ptr(ilk, WSDL_ATTR_NAME, -1), -1, sz_name, -1) == 0)
			break;

		pos = ilk;
		ilk = get_wsdl_next_binding_node(ptr, ilk);
	}

	if (!ilk)
	{
		if (!pos)
			pos = LINK_LAST;
		ilk = insert_dom_node(ptr, pos);
	}
	else
	{
		delete_dom_child_nodes(ilk);
	}

	set_dom_node_name(ilk, DOC_WSDL_BINDING, -1);
	set_dom_node_ns(ilk, NSWSDL, -1);
	set_dom_node_attr(ilk, WSDL_ATTR_NAME, -1, sz_name, -1);
	xsprintf(sz_withns, _T("%s:%s"), NSTNS, sz_type);
	set_dom_node_attr(ilk, WSDL_ATTR_TYPE, -1, sz_withns, -1);

	plk = insert_dom_node(ilk, LINK_FIRST);
	set_dom_node_name(plk, DOC_WSDL_BINDING, -1);
	set_dom_node_ns(plk, NSSOAP, -1);
	set_dom_node_attr(plk, WSDL_ATTR_TRANSPORT, -1, SOAP_TRANSPORT_HTTP, -1);

	return ilk;
}

link_t_ptr add_wsdl_binding_operation(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_action)
{
	link_t_ptr plk, nlk_wsdl, nlk_soap;

	plk = get_dom_first_child_node(ilk);
	while (plk)
	{
		if (compare_nons(get_dom_node_attr_ptr(plk, WSDL_ATTR_NAME, -1), -1, sz_name, -1) == 0)
			break;

		plk = get_dom_next_sibling_node(plk);
	}

	if (!plk)
	{
		plk = insert_dom_node(ilk, LINK_LAST);
	}
	else
	{
		delete_dom_child_nodes(plk);
	}

	set_dom_node_name(plk, WSDL_NODE_OPERATION, -1);
	set_dom_node_ns(plk, NSWSDL, -1);
	set_dom_node_attr(plk, WSDL_ATTR_NAME, -1, sz_name, -1);

	nlk_soap = insert_dom_node(plk, LINK_LAST);
	set_dom_node_name(nlk_soap, WSDL_NODE_OPERATION, -1);
	set_dom_node_ns(nlk_soap, NSSOAP, -1);
	set_dom_node_attr(nlk_soap, WSDL_ATTR_SOAPACTION, -1, sz_action, -1);
	set_dom_node_attr(nlk_soap, WSDL_ATTR_STYLE, -1, WSDL_ATTR_STYLE_DOCUMENT, -1);

	nlk_wsdl = insert_dom_node(plk, LINK_LAST);
	set_dom_node_name(nlk_wsdl, WSDL_NODE_INPUT, -1);
	set_dom_node_ns(nlk_wsdl, NSWSDL, -1);

	nlk_soap = insert_dom_node(nlk_wsdl, LINK_LAST);
	set_dom_node_name(nlk_soap, _T("body"), -1);
	set_dom_node_ns(nlk_soap, NSSOAP, -1);
	set_dom_node_attr(nlk_soap, _T("use"), -1, _T("literal"), -1);

	nlk_wsdl = insert_dom_node(plk, LINK_LAST);
	set_dom_node_name(nlk_wsdl, WSDL_NODE_OUTPUT, -1);
	set_dom_node_ns(nlk_wsdl, NSWSDL, -1);

	nlk_soap = insert_dom_node(nlk_wsdl, LINK_LAST);
	set_dom_node_name(nlk_soap, _T("body"), -1);
	set_dom_node_ns(nlk_soap, NSSOAP, -1);
	set_dom_node_attr(nlk_soap, _T("use"), -1, _T("literal"), -1);

	return plk;
}

link_t_ptr add_wsdl_service_node(link_t_ptr ptr, const tchar_t* sname)
{
	link_t_ptr ilk;

	ilk = get_wsdl_service_node(ptr);
	if (!ilk)
	{
		ilk = insert_dom_node(ptr, LINK_LAST);
	}
	else
	{
		delete_dom_child_nodes(ilk);
	}

	set_dom_node_name(ilk, DOC_WSDL_SERVICE, -1);
	set_dom_node_ns(ilk, NSWSDL, -1);
	set_dom_node_attr(ilk, WSDL_ATTR_NAME, -1, sname, -1);

	return ilk;
}

link_t_ptr add_wsdl_service_port(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_binding, const tchar_t* sz_location)
{
	link_t_ptr plk, nlk_soap;
	tchar_t sz_withns[1024];

	plk = get_dom_first_child_node(ilk);
	while (plk)
	{
		if (compare_nons(get_dom_node_attr_ptr(plk, WSDL_ATTR_NAME, -1), -1, sz_name, -1) == 0)
			break;

		plk = get_dom_next_sibling_node(plk);
	}

	if (!plk)
	{
		plk = insert_dom_node(ilk, LINK_LAST);
	}
	else
	{
		delete_dom_child_nodes(plk);
	}

	set_dom_node_name(plk, WSDL_NODE_PORT, -1);
	set_dom_node_ns(plk, NSWSDL, -1);
	set_dom_node_attr(plk, WSDL_ATTR_NAME, -1, sz_name, -1);
	xsprintf(sz_withns, _T("%s:%s"), NSTNS, sz_binding);
	set_dom_node_attr(plk, WSDL_ATTR_BINDING, -1, sz_withns, -1);

	nlk_soap = insert_dom_node(plk, LINK_LAST);
	set_dom_node_name(nlk_soap, WSDL_NODE_ADDRESS, -1);
	set_dom_node_ns(nlk_soap, NSSOAP, -1);
	set_dom_node_attr(nlk_soap, WSDL_ATTR_LOCATION, -1, sz_location, -1);

	return plk;
}

#endif /*XDL_SUPPORT_DOC*/


