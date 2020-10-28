/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc soap document

	@module	soapdoc.c | implement file

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

#include "soapdoc.h"
#include "domdoc.h"

#include "xdlimp.h"
#include "xdlstd.h"


#ifdef XDL_SUPPORT_DOC

link_t_ptr create_soap_doc()
{
	link_t_ptr ptr,nlk_head,nlk_body;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_SOAP,-1);

	nlk_head = insert_dom_node(ptr, LINK_LAST);
	set_dom_node_name(nlk_head, DOC_SOAP_HEAD, -1);

	nlk_body = insert_dom_node(ptr, LINK_LAST);
	set_dom_node_name(nlk_body, DOC_SOAP_BODY, -1);

	return ptr;
}

void destroy_soap_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_soap_doc(link_t_ptr ptr)
{
	return  (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_SOAP, -1, 1) == 0) ? 1 : 0;
}

void set_soap_default_xmlns(link_t_ptr ptr)
{
	link_t_ptr nlk_head, nlk_body;

	set_dom_node_ns(ptr, NSSOAP, -1);
	set_dom_node_xmlns(ptr, XMLNS_SOAP, -1, XMLNS_SOAP_INSTANCE, -1);
	set_dom_node_xmlns(ptr, XMLNS_XSI, -1, XMLNS_XSI_INSTANCE, -1);
	set_dom_node_xmlns(ptr, XMLNS_XSD, -1, XMLNS_XSD_INSTANCE, -1);

	set_dom_node_attr(ptr, SOAP_ATTR_ENCODINGSTYLE, -1, XMLNS_ENCODINGSTYLE_INSTANCE, -1);

	nlk_head = get_soap_head_node(ptr);
	if (nlk_head)
		set_dom_node_ns(nlk_head, NSSOAP, -1);

	nlk_body = get_soap_body_node(ptr);
	if (nlk_body)
		set_dom_node_ns(nlk_body, NSSOAP, -1);
}

link_t_ptr get_soap_head_node(link_t_ptr ptr)
{
	link_t_ptr nlk;
	const tchar_t* name;

	name = get_dom_node_name_ptr(ptr);

	if(compare_text(name,-1,DOC_SOAP,-1,1) != 0)
		return NULL;

	nlk = get_dom_first_child_node(ptr);
	while(nlk)
	{
		name = get_dom_node_name_ptr(nlk);
		if(compare_text(name,-1,DOC_SOAP_HEAD,-1,1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return nlk;
}

link_t_ptr get_soap_body_node(link_t_ptr ptr)
{
	link_t_ptr nlk;
	const tchar_t* name;

	name = get_dom_node_name_ptr(ptr);

	if (compare_text(name, -1, DOC_SOAP, -1, 1) != 0)
		return NULL;

	nlk = get_dom_first_child_node(ptr);
	while (nlk)
	{
		name = get_dom_node_name_ptr(nlk);
		if (compare_text(name, -1, DOC_SOAP_BODY, -1, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return nlk;
}

link_t_ptr set_soap_request_node(link_t_ptr ptr, const tchar_t* sz_req, const tchar_t* sz_ns, const tchar_t* sz_url)
{
	link_t_ptr nlk_body,nlk_req;
	tchar_t sz_token[PATH_LEN + 1] = { 0 };

	nlk_body = get_soap_body_node(ptr);
	if(!nlk_body)
		return NULL;

	delete_dom_child_nodes(nlk_body);

	nlk_req = insert_dom_node(nlk_body, LINK_LAST);
	set_dom_node_name(nlk_req, sz_req, -1);

	if (is_null(sz_ns))
	{
		xscpy(sz_token, XMLNS);
	}
	else
	{
		set_dom_node_ns(nlk_req, sz_ns, -1);
		xsprintf(sz_token, _T("%s:%s"), XMLNS, sz_ns);
	}

	if (!is_null(sz_url))
	{
		set_dom_node_xmlns(nlk_req, sz_token, -1, sz_url, -1);
	}

	return nlk_req;
}

link_t_ptr get_soap_request_node(link_t_ptr ptr)
{
	link_t_ptr nlk_body;

	nlk_body = get_soap_body_node(ptr);
	if (!nlk_body)
		return NULL;

	return get_dom_first_child_node(nlk_body);
}

link_t_ptr set_soap_request_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int n)
{
	link_t_ptr nlk_param,nlk_item;
	tchar_t sz_num[NUM_LEN + 1];
	int j;

	nlk_param = find_dom_node_by_name(nlk, 1, pname, -1);
	if (!nlk_param)
	{
		nlk_param = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(nlk_param, pname, -1);
	}
	
	if (compare_text(ptype, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 1) == 0)
	{
		if (*(bool_t*)pdata)
			set_dom_node_text(nlk_param, _T("1"), -1);
		else
			set_dom_node_text(nlk_param, _T("0"), -1);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_INTEGER, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%d"), *(int*)pdata);
		set_dom_node_text(nlk_param, sz_num, -1);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_NUMERIC, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%f"), *(double*)pdata);
		set_dom_node_text(nlk_param, sz_num, -1);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_DATE, -1, 1) == 0)
	{
		set_dom_node_text(nlk_param, (tchar_t*)pdata, -1);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_DATETIME, -1, 1) == 0)
	{
		set_dom_node_text(nlk_param, (tchar_t*)pdata, -1);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_STRING, -1, 1) == 0)
	{
		set_dom_node_text(nlk_param, (tchar_t*)pdata, -1);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_BOOLEAN_ARRAY, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%s[%d]"), SOAP_ATTR_DATATYPE_BOOLEAN, n);

		set_dom_node_attr(nlk_param, SOAP_ATTR_ARRAYTYPE, -1, sz_num, -1);
		set_dom_node_attr(nlk_param, SOAP_ATTR_DATATYPE, -1,SOAP_ATTR_DATATYPE_ARRAY, -1);

		for (j = 0; j < n; j++)
		{
			nlk_item = insert_dom_node(nlk_param, LINK_LAST);
			set_dom_node_name(nlk_item, DOC_SOAP_ARRAY_ITEM, -1);
			set_dom_node_attr(nlk_item, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_BOOLEAN, -1);

			if (((bool_t*)pdata)[j])
				set_dom_node_text(nlk_item, _T("1"), -1);
			else
				set_dom_node_text(nlk_item, _T("0"), -1);
		}
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_INTEGER_ARRAY, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%s[%d]"), SOAP_ATTR_DATATYPE_INTEGER, n);

		set_dom_node_attr(nlk_param, SOAP_ATTR_ARRAYTYPE, -1, sz_num, -1);
		set_dom_node_attr(nlk_param, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_ARRAY, -1);

		for (j = 0; j < n; j++)
		{
			nlk_item = insert_dom_node(nlk_param, LINK_LAST);
			set_dom_node_name(nlk_item, DOC_SOAP_ARRAY_ITEM, -1);
			set_dom_node_attr(nlk_item, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_INTEGER, -1);

			xsprintf(sz_num, _T("%d"), ((int*)pdata)[j]);
			set_dom_node_text(nlk_item, sz_num, -1);
		}
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_NUMERIC_ARRAY, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%s[%d]"), SOAP_ATTR_DATATYPE_DECIMAL, n);

		set_dom_node_attr(nlk_param, SOAP_ATTR_ARRAYTYPE, -1, sz_num, -1);
		set_dom_node_attr(nlk_param, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_ARRAY, -1);

		for (j = 0; j < n; j++)
		{
			nlk_item = insert_dom_node(nlk_param, LINK_LAST);
			set_dom_node_name(nlk_item, DOC_SOAP_ARRAY_ITEM, -1);
			set_dom_node_attr(nlk_item, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_DECIMAL, -1);

			xsprintf(sz_num, _T("%f"), ((double*)pdata)[j]);
			set_dom_node_text(nlk_item, sz_num, -1);
		}
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_DATE_ARRAY, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%s[%d]"), SOAP_ATTR_DATATYPE_DATE, n);

		set_dom_node_attr(nlk_param, SOAP_ATTR_ARRAYTYPE, -1, sz_num, -1);
		set_dom_node_attr(nlk_param, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_ARRAY, -1);

		for (j = 0; j < n; j++)
		{
			nlk_item = insert_dom_node(nlk_param, LINK_LAST);
			set_dom_node_name(nlk_item, DOC_SOAP_ARRAY_ITEM, -1);
			set_dom_node_attr(nlk_item, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_DATE, -1);

			set_dom_node_text(nlk_item, ((tchar_t**)pdata)[j], -1);
		}
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_TIME_ARRAY, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%s[%d]"), SOAP_ATTR_DATATYPE_TIME, n);

		set_dom_node_attr(nlk_param, SOAP_ATTR_ARRAYTYPE, -1, sz_num, -1);
		set_dom_node_attr(nlk_param, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_ARRAY, -1);

		for (j = 0; j < n; j++)
		{
			nlk_item = insert_dom_node(nlk_param, LINK_LAST);
			set_dom_node_name(nlk_item, DOC_SOAP_ARRAY_ITEM, -1);
			set_dom_node_attr(nlk_item, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_TIME, -1);

			set_dom_node_text(nlk_item, ((tchar_t**)pdata)[j], -1);
		}
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_STRING_ARRAY, -1, 1) == 0)
	{
		xsprintf(sz_num, _T("%s[%d]"), SOAP_ATTR_DATATYPE_STRING, n);

		set_dom_node_attr(nlk_param, SOAP_ATTR_ARRAYTYPE, -1, sz_num, -1);
		set_dom_node_attr(nlk_param, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_ARRAY, -1);

		for (j = 0; j < n; j++)
		{
			nlk_item = insert_dom_node(nlk_param, LINK_LAST);
			set_dom_node_name(nlk_item, DOC_SOAP_ARRAY_ITEM, -1);
			set_dom_node_attr(nlk_item, SOAP_ATTR_DATATYPE, -1, SOAP_ATTR_DATATYPE_STRING, -1);

			set_dom_node_text(nlk_item, ((tchar_t**)pdata)[j], -1);
		}
	}
	
	return nlk_param;
}

link_t_ptr get_soap_response_node(link_t_ptr ptr)
{
	link_t_ptr nlk_body,nlk_child;

	nlk_body = get_soap_body_node(ptr);
	if (!nlk_body)
		return NULL;

	nlk_child = get_dom_first_child_node(nlk_body);
	if (compare_text(DOC_SOAP_FAULT, -1, get_dom_node_name_ptr(nlk_child), -1, 1) == 0)
		return NULL;

	return nlk_child;
}

link_t_ptr set_soap_response_node(link_t_ptr ptr, const tchar_t* sz_res, const tchar_t* sz_ns, const tchar_t* sz_url)
{
	link_t_ptr nlk_body, nlk_res;
	tchar_t sz_token[PATH_LEN + 1] = { 0 };

	nlk_body = get_soap_body_node(ptr);
	if (!nlk_body)
		return NULL;

	delete_dom_child_nodes(nlk_body);

	nlk_res = insert_dom_node(nlk_body, LINK_LAST);
	set_dom_node_name(nlk_res, sz_res, -1);

	if (is_null(sz_ns))
	{
		xscpy(sz_token, XMLNS);
	}
	else
	{
		set_dom_node_ns(nlk_res, sz_ns, -1);
		xsprintf(sz_token, _T("%s:%s"), XMLNS, sz_ns);
	}

	if (!is_null(sz_url))
	{
		set_dom_node_xmlns(nlk_res, sz_token, -1, sz_url, -1);
	}

	return nlk_res;
}

int get_soap_request_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int max)
{
	return get_soap_response_param(nlk, pname, ptype, pdata, max);
}

link_t_ptr set_soap_response_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int n)
{
	return set_soap_request_param(nlk, pname, ptype, pdata, n);
}

int get_soap_response_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int max)
{
	link_t_ptr nlk_param,nlk_item;
	int j,n;
	const tchar_t* sz_text;

	nlk_param = find_dom_node_by_name(nlk, 1, pname, -1);
	if (!nlk_param)
		return 0;

	if (compare_text(ptype, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 1) == 0)
	{
		sz_text = get_dom_node_text_ptr(nlk_param);

		if (pdata)
		{
			if (compare_text(sz_text, -1, _T("0"), -1, 0) == 0)
				*(bool_t*)pdata = 0;
			else
				*(bool_t*)pdata = 0;
		}
		return 1;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_INTEGER, -1, 1) == 0)
	{
		sz_text = get_dom_node_text_ptr(nlk_param);
		if (pdata)
			*(int*)pdata = xstol(sz_text);
		return 1;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_NUMERIC, -1, 1) == 0)
	{
		sz_text = get_dom_node_text_ptr(nlk_param);
		if (pdata)
			*(double*)pdata = xstonum(sz_text);
		return 1;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_DATE, -1, 1) == 0)
	{
		return get_dom_node_text(nlk_param, (tchar_t*)pdata, max);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_DATETIME, -1, 1) == 0)
	{
		return get_dom_node_text(nlk_param, (tchar_t*)pdata, max);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_STRING, -1, 1) == 0)
	{
		return get_dom_node_text(nlk_param, (tchar_t*)pdata, max);
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_BOOLEAN_ARRAY, -1, 1) == 0)
	{
		n = get_dom_child_node_count(nlk_param);
		n = (n < max) ? n : max;
		
		if (pdata)
		{
			nlk_item = get_dom_first_child_node(nlk_param);
			for (j = 0; j < n; j++)
			{
				sz_text = get_dom_node_text_ptr(nlk_item);
				if (compare_text(sz_text, -1, _T("0"), -1, 0) == 0)
					((bool_t*)pdata)[j] = 0;
				else
					((bool_t*)pdata)[j] = 1;

				nlk_item = get_dom_next_sibling_node(nlk_item);
			}
		}
		return n;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_INTEGER_ARRAY, -1, 1) == 0)
	{
		n = get_dom_child_node_count(nlk_param);
		n = (n < max) ? n : max;

		if (pdata)
		{
			nlk_item = get_dom_first_child_node(nlk_param);
			for (j = 0; j < n; j++)
			{
				sz_text = get_dom_node_text_ptr(nlk_item);
				((int*)pdata)[j] = xstol(sz_text);
		
				nlk_item = get_dom_next_sibling_node(nlk_item);
			}
		}
		return n;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_NUMERIC_ARRAY, -1, 1) == 0)
	{
		n = get_dom_child_node_count(nlk_param);
		n = (n < max) ? n : max;

		if (pdata)
		{
			nlk_item = get_dom_first_child_node(nlk_param);
			for (j = 0; j < n; j++)
			{
				sz_text = get_dom_node_text_ptr(nlk_item);
				((double*)pdata)[j] = xstonum(sz_text);

				nlk_item = get_dom_next_sibling_node(nlk_item);
			}
		}
		return n;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_DATE_ARRAY, -1, 1) == 0)
	{
		n = get_dom_child_node_count(nlk_param);
		n = (n < max) ? n : max;

		if (pdata)
		{
			nlk_item = get_dom_first_child_node(nlk_param);
			for (j = 0; j < n; j++)
			{
				sz_text = get_dom_node_text_ptr(nlk_item);
				xscpy(((tchar_t**)pdata)[j],sz_text);

				nlk_item = get_dom_next_sibling_node(nlk_item);
			}
		}
		return n;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_DATE_ARRAY, -1, 1) == 0)
	{
		n = get_dom_child_node_count(nlk_param);
		n = (n < max) ? n : max;

		if (pdata)
		{
			nlk_item = get_dom_first_child_node(nlk_param);
			for (j = 0; j < n; j++)
			{
				sz_text = get_dom_node_text_ptr(nlk_item);
				xscpy(((tchar_t**)pdata)[j], sz_text);

				nlk_item = get_dom_next_sibling_node(nlk_item);
			}
		}
		return n;
	}
	else if (compare_text(ptype, -1, ATTR_DATA_TYPE_STRING_ARRAY, -1, 1) == 0)
	{
		n = get_dom_child_node_count(nlk_param);
		n = (n < max) ? n : max;

		if (pdata)
		{
			nlk_item = get_dom_first_child_node(nlk_param);
			for (j = 0; j < n; j++)
			{
				sz_text = get_dom_node_text_ptr(nlk_item);
				xscpy(((tchar_t**)pdata)[j], sz_text);

				nlk_item = get_dom_next_sibling_node(nlk_item);
			}
		}
		return n;
	}

	return 0;
}

link_t_ptr set_soap_fault_node(link_t_ptr ptr)
{
	link_t_ptr nlk_body, nlk_child;

	nlk_body = get_soap_body_node(ptr);
	if (!nlk_body)
		return NULL;

	nlk_child = get_dom_first_child_node(nlk_body);
	while (nlk_child)
	{
		if (compare_text(DOC_SOAP_FAULT, -1, get_dom_node_name_ptr(nlk_child), -1, 1) == 0)
			return nlk_child;

		nlk_child = get_dom_next_sibling_node(nlk_child);
	}

	nlk_child = insert_dom_node(nlk_body, LINK_LAST);
	set_dom_node_name(nlk_child, DOC_SOAP_FAULT, -1);

	return nlk_child;
}

link_t_ptr get_soap_fault_node(link_t_ptr ptr)
{
	link_t_ptr nlk_body, nlk_child;

	nlk_body = get_soap_body_node(ptr);
	if (!nlk_body)
		return NULL;

	nlk_child = get_dom_first_child_node(nlk_body);
	while (nlk_child)
	{
		if (compare_text(DOC_SOAP_FAULT, -1, get_dom_node_name_ptr(nlk_child), -1, 1) == 0)
			return nlk_child;

		nlk_child = get_dom_next_sibling_node(nlk_child);
	}

	return NULL;
}

void set_soap_fault_code(link_t_ptr nlk, const tchar_t* sz_code)
{
	link_t_ptr nlk_child;

	nlk_child = get_dom_first_child_node(nlk);
	while (nlk_child)
	{
		if (compare_text(DOC_SOAP_FAULT_CODE, -1, get_dom_node_name_ptr(nlk_child), -1, 1) == 0)
			break;

		nlk_child = get_dom_next_sibling_node(nlk_child);
	}

	if (!nlk_child)
	{
		nlk_child = insert_dom_node(nlk, LINK_FIRST);
		set_dom_node_name(nlk_child, DOC_SOAP_FAULT_CODE, -1);
	}
	
	set_dom_node_text(nlk_child, sz_code, -1);
}

void set_soap_fault_string(link_t_ptr nlk, const tchar_t* sz_text)
{
	link_t_ptr nlk_child;

	nlk_child = get_dom_first_child_node(nlk);
	while (nlk_child)
	{
		if (compare_text(DOC_SOAP_FAULT_STRING, -1, get_dom_node_name_ptr(nlk_child), -1, 1) == 0)
			break;

		nlk_child = get_dom_next_sibling_node(nlk_child);
	}

	if (!nlk_child)
	{
		nlk_child = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(nlk_child, DOC_SOAP_FAULT_STRING, -1);
	}

	set_dom_node_text(nlk_child, sz_text, -1);
}

int get_soap_fault_code(link_t_ptr nlk, tchar_t* buf, int max)
{
	link_t_ptr nlk_child;

	nlk_child = get_dom_first_child_node(nlk);
	while (nlk_child)
	{
		if (compare_text(DOC_SOAP_FAULT_CODE, -1, get_dom_node_name_ptr(nlk_child), -1, 1) == 0)
			break;

		nlk_child = get_dom_next_sibling_node(nlk_child);
	}

	if (nlk_child)
		return get_dom_node_text(nlk_child, buf, max);
	else
		return 0;
}


int get_soap_fault_string(link_t_ptr nlk, tchar_t* buf, int max)
{
	link_t_ptr nlk_child;

	nlk_child = get_dom_first_child_node(nlk);
	while (nlk_child)
	{
		if (compare_text(DOC_SOAP_FAULT_STRING, -1, get_dom_node_name_ptr(nlk_child), -1, 1) == 0)
			break;

		nlk_child = get_dom_next_sibling_node(nlk_child);
	}

	if (nlk_child)
		return get_dom_node_text(nlk_child, buf, max);
	else
		return 0;
}

#endif /*SOAPDOC_H*/
