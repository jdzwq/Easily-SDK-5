/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdl soap document

	@module	xdlsoap.c | xdl soap implement file

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

#include "xdlsoap.h"
#include "xdlinet.h"
#include "hashtable.h"
#include "xdlutil.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlval.h"
#include "xdlxml.h"
#include "domdoc.h"
#include "xmldoc.h"
#include "wsdldoc.h"
#include "schdoc.h"
#include "streambag.h"
#include "xdlfile.h"
#include "soapdoc.h"

typedef struct _xsoap_t{
	xhand_head head;		//head for xhand_t

	link_t_ptr sch_in;
	link_t_ptr sch_out;

	tchar_t sz_location[PATH_LEN];
	tchar_t sz_target[PATH_LEN];
	tchar_t sz_xmlns[RES_LEN];
	tchar_t sz_action[RES_LEN];

	tchar_t sz_input[RES_LEN];
	tchar_t sz_output[RES_LEN];
	tchar_t sz_fault[RES_LEN];

	tchar_t sz_fault_code[NUM_LEN + 1];
	tchar_t sz_fault_text[MAX_ERR + 1];
}xsoap_t;


xhand_t xsoap_create(const tchar_t* sz_wsdl, const tchar_t* sz_nsname, const tchar_t* sz_nsurl, const tchar_t* sz_service, const tchar_t* sz_method)
{
	xsoap_t* pso = NULL;
	int rt = 0;
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	xhand_t xh = NULL;
	byte_t* sz_buf = NULL;
	link_t_ptr wsdl = NULL;

	link_t_ptr xml, nlk_service, nlk_port, nlk_address, nlk_message, nlk_part, nlk_binding, nlk_porttype, nlk_operation;
	link_t_ptr nlk_child;
	bool_t b_chunk,b_fix;
	tchar_t* token;
	int pos,nlen,tklen;

	tchar_t sz_location[PATH_LEN] = { 0 };
	tchar_t sz_target[PATH_LEN] = { 0 };
	tchar_t sz_entry[RES_LEN] = { 0 };
	tchar_t sz_action[RES_LEN] = { 0 };
	tchar_t sz_input[RES_LEN] = { 0 };
	tchar_t sz_output[RES_LEN] = { 0 };
	tchar_t sz_fault[RES_LEN] = { 0 };
	tchar_t sz_fault_param[RES_LEN] = { 0 };

	link_t_ptr nlk_ele;

	xh = xhttp_open(NULL, _T("GET"), sz_wsdl);
	if (!xh)
		return NULL;

	pso = (xsoap_t*)xmem_alloc(sizeof(xsoap_t));
	pso->head.tag = _HANDLE_SOAP;
	pso->head.cbs = sizeof(xsoap_t);

	pso->sch_in = create_sch_doc();
	pso->sch_out = create_sch_doc();

	xscpy(pso->sz_location, sz_wsdl);
	xscpy(pso->sz_xmlns, sz_nsname);
	xscpy(pso->sz_target, sz_nsurl);
	if (is_null(sz_nsurl))
	{
		xsprintf(pso->sz_action, _T("%s"), sz_method);
	}
	else
	{
		tklen = xslen(sz_nsurl);
		if (sz_nsurl[tklen - 1] == L'/')
			xsprintf(pso->sz_action, _T("%s%s"), sz_nsurl, sz_method);
		else
			xsprintf(pso->sz_action, _T("%s/%s"), sz_nsurl, sz_method);
	}

	xscpy(pso->sz_input, sz_method);
	xsprintf(pso->sz_output, _T("%sResponse"), sz_method);
	xscpy(pso->sz_fault, _T("Fault"));

	if (is_null(xhttp_get_url_query_ptr(xh)))
	{
		goto _errret;
	}

	xhttp_set_request_default_header(xh);

	rt = xhttp_send(xh, NULL, 0);
	if (rt < 0)
	{
		goto _errret;
	}

	rt = xhttp_recv(xh, NULL, 0);
	if (rt < 0)
	{
		goto _errret;
	}

	xhttp_get_response_header(xh, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
	nlen = xstol(fsize);
	if (!nlen)
	{
		xhttp_get_response_header(xh, XHTTP_TRANSFER_ENCODING, -1, fsize, NUM_LEN);
		b_chunk = (compare_text(fsize, -1, XHTTP_TRANSFER_ENCODING_CHUNKED, -1, 1) == 0) ? 1 : 0;
		if (b_chunk)
			nlen = 0;
		else
			nlen = XHTTP_MAX_BUFFER;

		b_fix = 0;
	}
	else
	{
		b_chunk = 0;
		b_fix = 1;
	}

	if (!b_chunk)
	{
		pos = 0;
		do{
			sz_buf = (byte_t*)xmem_realloc(sz_buf, nlen + 1);

			rt = xhttp_recv(xh, sz_buf + pos, nlen - pos);
			if (rt <= 0)
				break;

			pos += rt;
			if (pos == nlen && b_fix)
				break;

			nlen += XHTTP_MAX_BUFFER;
		} while (1);
	}
	else
	{
		nlen = xhttp_chunked_size(xh);
		if (nlen < 0)
			goto _errret;

		pos = 0;
		do{
			sz_buf = (byte_t*)xmem_realloc(sz_buf, nlen + 1);

			rt = xhttp_chunked_recv(xh, sz_buf + pos, nlen - pos);
			if (rt <= 0)
				break;

			pos += rt;
			rt = xhttp_chunked_size(xh);
			if (rt <= 0)
				break;

			nlen += rt;
		} while (1);
	}
	nlen = pos;

	xhttp_close(xh);
	xh = NULL;

	wsdl = create_wsdl_doc();
	xml = upcast_dom_to_xml(wsdl);

	rt = parse_xml_doc(xml, sz_buf, nlen);

#ifdef _DEBUG
	save_xml_to_file(xml, _T("..\\..\\bin\\my.wsdl"));
#endif

	xmem_free(sz_buf);
	sz_buf = NULL;

	wsdl = downcast_xml_to_dom(xml);

	if (!rt)
	{
		goto _errret;
	}

	nlk_service = get_wsdl_service_node(wsdl, sz_service);
	if (!nlk_service)
	{
		goto _errret;
	}

	nlk_port = get_dom_first_child_node(nlk_service);
	while (nlk_port)
	{
		if (compare_text(get_dom_node_name_ptr(nlk_port), -1, DOC_WSDL_PORT, -1, 1) != 0)
		{
			nlk_port = get_dom_next_sibling_node(nlk_port);
			continue;
		}

		token = (tchar_t*)get_dom_node_attr_ptr(nlk_port, WSDL_ATTR_BINDING, -1);
		token = skip_xmlns(token, &tklen);
		
		nlk_binding = get_wsdl_binding_node(wsdl, token);
		if (!nlk_binding)
			break;

		nlk_operation = get_dom_first_child_node(nlk_binding);
		while (nlk_operation)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_operation), -1, DOC_WSDL_OPERATION, -1, 1) != 0)
			{
				nlk_operation = get_dom_next_sibling_node(nlk_operation);
				continue;
			}

			token = (tchar_t*)get_dom_node_attr_ptr(nlk_operation, WSDL_ATTR_NAME, -1);
			token = skip_xmlns(token, &tklen);

			if (compare_text(sz_method, -1, token, -1, 1) == 0)
			{
				nlk_child = get_dom_first_child_node(nlk_operation);
				while (nlk_child)
				{
					if (compare_text(get_dom_node_name_ptr(nlk_child), -1, DOC_WSDL_OPERATION, -1, 1) == 0)
					{
						get_dom_node_attr(nlk_child, WSDL_ATTR_SOAPACTION, -1, sz_action, RES_LEN);
						break;
					}

					nlk_child = get_dom_next_sibling_node(nlk_child);
				}

				token = (tchar_t*)get_dom_node_attr_ptr(nlk_binding, WSDL_ATTR_TYPE, -1);
				token = skip_xmlns(token, &tklen);

				xscpy(sz_entry, token);

				break;
			}

			nlk_operation = get_dom_next_sibling_node(nlk_operation);
		}

		if (!is_null(sz_entry))
		{
			nlk_address = get_dom_first_child_node(nlk_port);
			while (nlk_address)
			{
				if (compare_text(get_dom_node_name_ptr(nlk_address), -1, DOC_WSDL_ADDRESS, -1, 1) != 0)
				{
					nlk_address = get_dom_next_sibling_node(nlk_address);
					continue;
				}

				xscpy(sz_location, get_dom_node_attr_ptr(nlk_address, WSDL_ATTR_LOCATION, -1));

				nlk_address = get_dom_next_sibling_node(nlk_address);
			}
			break;
		}

		nlk_port = get_dom_next_sibling_node(nlk_port);
	}

	if (is_null(sz_entry))
	{
		goto _errret;
	}

	nlk_porttype = get_wsdl_porttype_node(wsdl, sz_entry);
	if (!nlk_porttype)
	{
		goto _errret;
	}

	nlk_operation = get_wsdl_porttype_operation_node(nlk_porttype, sz_method);
	if (!nlk_operation)
	{
		goto _errret;
	}

	nlk_child = get_dom_first_child_node(nlk_operation);
	while (nlk_child)
	{
		if (compare_text(get_dom_node_name_ptr(nlk_child), -1, DOC_WSDL_INPUT, -1, 1) == 0)
		{
			token = (tchar_t*)get_dom_node_attr_ptr(nlk_child, WSDL_ATTR_MESSAGE, -1);
			token = skip_xmlns(token, &tklen);
			xscpy(sz_input, token);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, DOC_WSDL_OUTPUT, -1, 1) == 0)
		{
			token = (tchar_t*)get_dom_node_attr_ptr(nlk_child, WSDL_ATTR_MESSAGE, -1);
			token = skip_xmlns(token, &tklen);
			xscpy(sz_output, token);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk_child), -1, DOC_WSDL_FAULT, -1, 1) == 0)
		{
			token = (tchar_t*)get_dom_node_attr_ptr(nlk_child, WSDL_ATTR_MESSAGE, -1);
			token = skip_xmlns(token, &tklen);
			xscpy(sz_fault, token);
		}

		nlk_child = get_dom_next_sibling_node(nlk_child);
	}

	nlk_message = get_wsdl_message_node(wsdl, sz_input);
	if (!nlk_message)
	{
		goto _errret;
	}

	nlk_part = get_dom_first_child_node(nlk_message);
	while (nlk_part)
	{
		if (compare_text(get_dom_node_name_ptr(nlk_part), -1, DOC_WSDL_PART, -1, 1) != 0)
		{
			nlk_part = get_dom_next_sibling_node(nlk_part);
			continue;
		}

		nlk_ele = insert_sch_node(pso->sch_in,LINK_LAST);

		token = (tchar_t*)get_dom_node_attr_ptr(nlk_part, WSDL_ATTR_NAME, -1);
		token = skip_xmlns(token, &tklen);
		set_sch_node_name(nlk_ele, token);

		set_sch_node_construct(nlk_ele, SCH_ATTR_CONSTRUCT_ELEMENT);

		token = (tchar_t*)get_dom_node_attr_ptr(nlk_part, WSDL_ATTR_TYPE, -1);
		token = skip_xmlns(token, &tklen);
		set_sch_node_type(nlk_ele, token);
		
		nlk_part = get_dom_next_sibling_node(nlk_part);
	}

	nlk_message = get_wsdl_message_node(wsdl, sz_output);
	if (nlk_message)
	{
		nlk_part = get_dom_first_child_node(nlk_message);
		while (nlk_part)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_part), -1, DOC_WSDL_PART, -1, 1) != 0)
			{
				nlk_part = get_dom_next_sibling_node(nlk_part);
				continue;
			}

			nlk_ele = insert_sch_node(pso->sch_out, LINK_LAST);

			token = (tchar_t*)get_dom_node_attr_ptr(nlk_part, WSDL_ATTR_NAME, -1);
			token = skip_xmlns(token, &tklen);
			set_sch_node_name(nlk_ele, token);

			set_sch_node_construct(nlk_ele, SCH_ATTR_CONSTRUCT_ELEMENT);

			token = (tchar_t*)get_dom_node_attr_ptr(nlk_part, WSDL_ATTR_TYPE, -1);
			token = skip_xmlns(token, &tklen);
			set_sch_node_type(nlk_ele, token);

			nlk_part = get_dom_next_sibling_node(nlk_part);
		}
	}

	nlk_message = get_wsdl_message_node(wsdl, sz_fault);
	if (nlk_message)
	{
		nlk_part = get_dom_first_child_node(nlk_message);
		while (nlk_part)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_part), -1, DOC_WSDL_PART, -1, 1) != 0)
			{
				nlk_part = get_dom_next_sibling_node(nlk_part);
				continue;
			}

			nlk_part = get_dom_next_sibling_node(nlk_part);
		}
	}

	destroy_wsdl_doc(wsdl);
	wsdl = NULL;

	if (!is_null(sz_location))
	{
		xscpy(pso->sz_location, sz_location);
	}

	if (!is_null(sz_target))
	{
		tklen = xslen(pso->sz_target);
		if (sz_nsurl[tklen - 1] == L'/')
			xsprintf(pso->sz_action, _T("%s%s"), sz_target, sz_method);
		else
			xsprintf(pso->sz_action, _T("%s/%s"), sz_target, sz_method);
	}

	if (!is_null(sz_input))
		xscpy(pso->sz_input, sz_input);

	if (!is_null(sz_output))
		xscpy(pso->sz_output, sz_output);

	if (!is_null(sz_fault))
		xscpy(pso->sz_fault, sz_fault);

_errret:

	if (sz_buf)
		xmem_free(sz_buf);

	if (xh)
		xhttp_close(xh);

	if (wsdl)
		destroy_wsdl_doc(wsdl);

	return &(pso->head);
}

void  xsoap_destroy(xhand_t soa)
{
	xsoap_t* pso = (xsoap_t*)soa;

	if (pso->sch_in)
		destroy_sch_doc(pso->sch_in);

	if (pso->sch_out)
		destroy_sch_doc(pso->sch_out);

	xmem_free(pso);
}

link_t_ptr xsoap_get_request_param(xhand_t soa, const tchar_t* pname)
{
	xsoap_t* pso = (xsoap_t*)soa;

	return get_sch_node(pso->sch_in, pname, -1);
}

void xsoap_set_response_param(xhand_t soa, const tchar_t* pname, const tchar_t* ptype)
{
	xsoap_t* pso = (xsoap_t*)soa;
	link_t_ptr nlk_ele;

	if (is_null(pname))
		return;

	nlk_ele = xsoap_get_response_param(soa, pname);
	if (!nlk_ele)
		nlk_ele = insert_sch_node(pso->sch_out, LINK_FIRST);

	set_sch_node_name(nlk_ele, pname);
	set_sch_node_construct(nlk_ele, SCH_ATTR_CONSTRUCT_ELEMENT);
	set_sch_node_type(nlk_ele, ptype);

}

link_t_ptr xsoap_get_response_param(xhand_t soa, const tchar_t* pname)
{
	xsoap_t* pso = (xsoap_t*)soa;

	return get_sch_node(pso->sch_out, pname, -1);
}

void xsoap_set_request_param(xhand_t soa, const tchar_t* pname, const tchar_t* ptype)
{
	xsoap_t* pso = (xsoap_t*)soa;
	link_t_ptr nlk_ele;

	if (is_null(pname))
		return;

	nlk_ele = xsoap_get_request_param(soa, pname);
	if (!nlk_ele)
		nlk_ele = insert_sch_node(pso->sch_in, LINK_LAST);

	set_sch_node_name(nlk_ele, pname);
	set_sch_node_construct(nlk_ele, SCH_ATTR_CONSTRUCT_ELEMENT);
	set_sch_node_type(nlk_ele, ptype);
}

varval_t xsoap_invoke(xhand_t soa, varval_t* vs, int n)
{
	xsoap_t* pso = (xsoap_t*)soa;

	link_t_ptr xml,nlk_body,nlk_input, nlk_param, nlk_item, nlk_output, nlk_fault;
	link_t_ptr slk;

	link_t_ptr soap = NULL;
	xhand_t xh = NULL;
	int rt,pos,nlen = 0;
	byte_t* sz_buf = NULL;
	bool_t b_fix, b_chunk;

	const tchar_t* sz_result;
	const tchar_t* sz_type;
	const tchar_t* sz_return;
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_text[MAX_ERR + 1] = { 0 };
	tchar_t sz_token[RES_LEN + 1] = { 0 };
	int count;

	int i,j;
	varval_t v = { 0 };

	soap = create_soap_doc();

	set_soap_default_xmlns(soap);

	nlk_body = get_soap_body_node(soap);

	if (is_null(pso->sz_xmlns))
	{
		xscpy(sz_token, _T("xmlns"));
	}
	else
	{
		set_dom_node_ns(nlk_input, pso->sz_xmlns, -1);
		xsprintf(sz_token, _T("%s:%s"), _T("xmlns"), pso->sz_xmlns);
	}

	nlk_input = set_soap_request_node(soap, pso->sz_input, sz_token, pso->sz_target);
	
	i = 0;
	slk = get_sch_first_child_node(pso->sch_in);
	while (slk)
	{
		if (i == n)
			break;

		set_soap_request_param(nlk_input, get_sch_node_name_ptr(slk), get_sch_node_type_ptr(slk), (void*)&(vs->data), vs->size);
		
		i++;
		slk = get_sch_next_sibling_node(slk);
	}

	xh = xhttp_open(NULL, _T("POST"), pso->sz_location);
	if (!xh)
	{
		goto _errret;
	}

	xml = upcast_dom_to_xml(soap);
	nlen = format_xml_doc_size(xml);
	sz_buf = (byte_t*)xmem_alloc(nlen + 1);
	format_xml_doc(xml, sz_buf, nlen);
#ifdef _DEBUG
	save_xml_to_file(xml, _T("..\\..\\bin\\my_in.xml"));
#endif
	soap = downcast_xml_to_dom(xml);
	destroy_soap_doc(soap);
	soap = NULL;

	xsprintf(fsize, _T("%d"), nlen);
	xhttp_set_request_default_header(xh);
	xhttp_set_request_header(xh, XHTTP_HEADER_CONTENTTYPE, -1, _T("text/xml"), -1);
	xhttp_set_request_header(xh, XHTTP_HEADER_CONTENTTYPE_CHARSET, -1, _T("utf-8"), -1);
	xhttp_set_request_header(xh, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
	xhttp_set_request_header(xh, XHTTP_HEADER_SOAPACTION, -1, pso->sz_action, -1);

	rt = xhttp_send(xh, sz_buf, nlen);

	xmem_free(sz_buf);
	sz_buf = NULL;

	if (rt < nlen)
	{
		goto _errret;
	}

	xhttp_recv(xh, NULL, 0);

	xhttp_get_response_code(xh, sz_code);
	xhttp_get_response_message(xh, sz_text, MAX_ERR);
	xhttp_get_response_header(xh, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
	nlen = xstol(fsize);
	if (!nlen)
	{
		xhttp_get_response_header(xh, XHTTP_TRANSFER_ENCODING, -1, fsize, NUM_LEN);
		b_chunk = (compare_text(fsize, -1, XHTTP_TRANSFER_ENCODING_CHUNKED, -1, 1) == 0) ? 1 : 0;
		if (b_chunk)
			nlen = 0;
		else
			nlen = XHTTP_MAX_BUFFER;

		b_fix = 0;
	}
	else
	{
		b_chunk = 0;
		b_fix = 1;
	}

	if (!b_chunk)
	{
		pos = 0;
		do{
			sz_buf = (byte_t*)xmem_realloc(sz_buf, nlen + 1);

			rt = xhttp_recv(xh, sz_buf + pos, nlen - pos);
			if (rt <= 0)
				break;

			pos += rt;
			if (pos == nlen && b_fix)
				break;

			nlen += XHTTP_MAX_BUFFER;
		} while (1);
	}
	else
	{
		nlen = xhttp_chunked_size(xh);
		if (nlen < 0)
			goto _errret;

		pos = 0;
		do{
			sz_buf = (byte_t*)xmem_realloc(sz_buf, nlen + 1);

			rt = xhttp_chunked_recv(xh, sz_buf + pos, nlen - pos);
			if (rt <= 0)
				break;

			pos += rt;
			rt = xhttp_chunked_size(xh);
			if (rt <= 0)
				break;

			nlen += rt;
		} while (1);
	}
	nlen = pos;

	xhttp_close(xh);
	xh = NULL;

	soap = create_soap_doc();
	xml = upcast_dom_to_xml(soap);

	rt = parse_xml_doc(xml, sz_buf, nlen);

#ifdef _DEBUG
	save_xml_to_file(xml, _T("..\\..\\bin\\my_out.xml"));
#endif

	xmem_free(sz_buf);
	sz_buf = NULL;
		
	soap = downcast_xml_to_dom(xml);
	
	if (!rt)
	{
		xscpy(pso->sz_fault_code, sz_code);
		xscpy(pso->sz_fault_text, sz_text);

		goto _errret;
	}

	if (XHTTP_SUCCEED(sz_code))
	{
		nlk_output = get_soap_response_node(soap, pso->sz_output);
		if (!nlk_output)
		{
			goto _errret;
		}

		if (pso->sch_out)
		{
			slk = get_sch_first_child_node(pso->sch_out);
			sz_result = (slk) ? get_sch_node_name_ptr(slk) : NULL;
			sz_type = (slk) ? get_sch_node_type_ptr(slk) : NULL;
		}
		else
		{
			sz_result = NULL;
			sz_type = NULL;
		}

		slk = (pso->sch_out) ? get_sch_first_child_node(pso->sch_out) : NULL;
		while (slk)
		{
			sz_result = get_sch_node_name_ptr(slk);
			sz_type = get_sch_node_type_ptr(slk);

			get_soap_response_param(nlk_output, sz_result, sz_type, )
			slk = get_sch_next_sibling_node(slk);
		}
		if (pso->sch_out)
		{

			get_soap_response_param(nlk_output, sz_result, sz_type, )
		}

		nlk_param = get_dom_first_child_node(nlk_output);
		while (nlk_param)
		{
			if (is_null(sz_result) || compare_text(sz_result, -1, get_dom_node_name_ptr(nlk_param), -1, 1) == 0)
			{
				if (compare_text(sz_type, -1, SCH_ATTR_TYPE_BOOLEAN, -1, 1) == 0)
				{
					sz_return = get_dom_node_text_ptr(nlk_param);
					v.vv = VV_BOOL;
					v.bool_one = (bool_t)xstos(sz_return);
				}else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_INTEGER, -1, 1) == 0)
				{
					sz_return = get_dom_node_text_ptr(nlk_param);
					v.vv = VV_LONG;
					v.long_one = (bool_t)xstol(sz_return);
				}else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_DECIMAL, -1, 1) == 0)
				{
					sz_return = get_dom_node_text_ptr(nlk_param);
					v.vv = VV_NUMERIC;
					v.numeric_one = (bool_t)xstof(sz_return);
				}else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_TIME, -1, 1) == 0)
				{
					sz_return = get_dom_node_text_ptr(nlk_param);
					v.vv = VV_CHAR_PTR;
					v.char_ptr = xsalloc(xslen(sz_return) + 1);
					xscpy(v.char_ptr, sz_return);
				}else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_DATE, -1, 1) == 0)
				{
					sz_return = get_dom_node_text_ptr(nlk_param);
					v.vv = VV_CHAR_PTR;
					v.char_ptr = xsalloc(xslen(sz_return) + 1);
					xscpy(v.char_ptr, sz_return);
				}else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_STRING, -1, 1) == 0)
				{
					sz_return = get_dom_node_text_ptr(nlk_param);
					v.vv = VV_CHAR_PTR;
					v.char_ptr = xsalloc(xslen(sz_return) + 1);
					xscpy(v.char_ptr, sz_return);
				}else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_BOOLEAN_ARRAY, -1, 1) == 0)
				{
					i = get_dom_child_node_count(nlk_param);
					if (i)
					{
						v.vv = VV_BOOL_PTR;
						v.bool_ptr = (bool_t*)xmem_alloc(i * sizeof(bool_t));
						j = 0;
						nlk_item = get_dom_first_child_node(nlk_param);
						while (nlk_item)
						{
							sz_return = get_dom_node_text_ptr(nlk_item);
							v.bool_ptr[j++] = (bool_t)xstos(sz_return);

							nlk_item = get_dom_next_sibling_node(nlk_item);
						}
					}
				}
				else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_INTEGER_ARRAY, -1, 1) == 0)
				{
					i = get_dom_child_node_count(nlk_param);
					if (i)
					{
						v.vv = VV_LONG_PTR;
						v.long_ptr = (long*)xmem_alloc(i * sizeof(long));
						j = 0;
						nlk_item = get_dom_first_child_node(nlk_param);
						while (nlk_item)
						{
							sz_return = get_dom_node_text_ptr(nlk_item);
							v.long_ptr[j++] = xstol(sz_return);

							nlk_item = get_dom_next_sibling_node(nlk_item);
						}
					}
				}
				else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_DECIMAL_ARRAY, -1, 1) == 0)
				{
					i = get_dom_child_node_count(nlk_param);
					if (i)
					{
						v.vv = VV_NUMERIC_PTR;
						v.numeric_ptr = (double*)xmem_alloc(i * sizeof(double));
						j = 0;
						nlk_item = get_dom_first_child_node(nlk_param);
						while (nlk_item)
						{
							sz_return = get_dom_node_text_ptr(nlk_item);
							v.numeric_ptr[j++] = xstof(sz_return);

							nlk_item = get_dom_next_sibling_node(nlk_item);
						}
					}
				}
				else if (compare_text(sz_type, -1, SCH_ATTR_TYPE_TIME_ARRAY, -1, 1) == 0 || compare_text(sz_type, -1, SCH_ATTR_TYPE_DATE_ARRAY, -1, 1) == 0 || compare_text(sz_type, -1, SCH_ATTR_TYPE_STRING_ARRAY, -1, 1) == 0)
				{
					i = get_dom_child_node_count(nlk_param);
					if (i)
					{
						v.vv = VV_CHAR_PTR_PTR;
						v.char_ptr_ptr = (tchar_t**)xmem_alloc(i * sizeof(tchar_t*));
						j = 0;
						nlk_item = get_dom_first_child_node(nlk_param);
						while (nlk_item)
						{
							sz_return = get_dom_node_text_ptr(nlk_item);
							nlen = xslen(sz_return);
							v.char_ptr_ptr[j] = xsalloc(nlen + 1);
							xscpy(v.char_ptr_ptr[j], sz_return);

							j++;
							nlk_item = get_dom_next_sibling_node(nlk_item);
						}
					}
				}
			}
			nlk_param = get_dom_next_sibling_node(nlk_param);
		}
	}
	else
	{
		nlk_fault = get_soap_user_node(soap, pso->sz_fault);
		if (!nlk_fault)
		{
			goto _errret;
		}

		nlk_param = get_dom_first_child_node(nlk_fault);
		while (nlk_param)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_param), -1, DOC_SOAP_FAULT_CODE, -1, 1) == 0)
				get_dom_node_text(nlk_param, pso->sz_fault_code, NUM_LEN);
			else if (compare_text(get_dom_node_name_ptr(nlk_param), -1, DOC_SOAP_FAULT_STRING, -1, 1) == 0)
				get_dom_node_text(nlk_param, pso->sz_fault_text, MAX_ERR);

			nlk_param = get_dom_next_sibling_node(nlk_param);
		}
	}

_errret:

	if (sz_buf)
		xmem_free(sz_buf);

	if (soap)
		destroy_soap_doc(soap);

	if (xh)
		xhttp_close(xh);

	return v;
}

void xsoap_get_fault_code(xhand_t soa, tchar_t* code)
{
	xsoap_t* pso = (xsoap_t*)soa;

	xscpy(code, pso->sz_fault_code);
}

void xsoap_get_fault_string(xhand_t soa, tchar_t* text, int max)
{
	xsoap_t* pso = (xsoap_t*)soa;
	int len;

	len = xslen(pso->sz_fault_text);
	len = (len < max) ? len : max;

	xsncpy(text, pso->sz_fault_text, len);
}