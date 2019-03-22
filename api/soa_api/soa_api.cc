/***********************************************************************
	Easily loc web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc soa web api document

	@module	soa_api.cc | soa web api implement file

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

#include "soa_api.h"



void _invoke_soap_fault(xhand_t xh, const tchar_t* sz_code, const tchar_t* sz_error)
{
	link_t_ptr soap_fault, nlk_fault;
	link_t_ptr ptr_xml;
	dword_t n_size;
	tchar_t sz_size[NUM_LEN + 1];

	soap_fault = create_soap_doc();
	nlk_fault = set_soap_fault_node(soap_fault);

	set_soap_fault_code(nlk_fault, sz_code);
	set_soap_fault_string(nlk_fault, sz_error);

	ptr_xml = upcast_dom_to_xml(soap_fault);

	xhttp_set_response_code(xh, HTTP_CODE_200);
	xhttp_set_response_message(xh, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(xh, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_TEXTXML, -1);

	n_size = format_xml_doc_to_bytes(ptr_xml, NULL, MAX_LONG);
	xsprintf(sz_size, _T("%d"), n_size);
	xhttp_set_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, sz_size, -1);

	xhttp_send_xml(xh, ptr_xml);

	soap_fault = downcast_xml_to_dom(ptr_xml);

	destroy_soap_doc(soap_fault);
}

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	
	tchar_t token[PATH_LEN] = { 0 };
	tchar_t file[PATH_LEN] = { 0 };
	int k,len;

	HMODULE h_modu = NULL;
	soap_block_t* psb = NULL;
	PF_SOAP_INVOKE pf_invoke_soap = NULL;
	wsdl_block_t* pwb = NULL;
	PF_WSDL_INVOKE pf_invoke_wsdl = NULL;
	int n_status = 0;

	link_t_ptr ptr_xml;
	dword_t n_size = 0;
	int encode = 0;
	bool_t rt;
	bool_t b_wsdl = 0;

	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	if (compare_text(method, -1, _T("GET"), -1, 1) == 0)
	{
		b_wsdl = 1;
		pwb = (wsdl_block_t*)xmem_alloc(sizeof(wsdl_block_t));
	}
	else
	{
		b_wsdl = 0;
		psb = (soap_block_t*)xmem_alloc(sizeof(soap_block_t));
	}

	if (b_wsdl)
	{
		pwb->soap_wsdl = create_wsdl_doc();
		set_wsdl_default_xmlns(pwb->soap_wsdl);
		encode = _UTF8;
	}
	else
	{
		xhttp_get_request_header(pb->http, HTTP_HEADER_SOAPACTION, -1, token, PATH_LEN);
		if (token[0] == _T('\"'))
		{
			k = 0;
			do{
				token[k] = token[k + 1];

				if (token[k] == _T('\"'))
					break;

				k++;
			} while (token[k] != _T('\0'));

			token[k] = _T('\0');
		}

		len = xslen(pb->space);

		if (xsnicmp(pb->space, token, len) != 0)
		{
			xscat(token, _T(" namespace validated falied"));
			raise_user_error(_T("xhttp_invoke"), token);
		}

		if (token[len] == _T('/'))
			len++;

		xsncpy(psb->soap_action, token + len, PATH_LEN);

		psb->soap_request = create_soap_doc();

		ptr_xml = upcast_dom_to_xml(psb->soap_request);

		rt = xhttp_recv_xml(pb->http, ptr_xml);

		encode = get_xml_encode(ptr_xml);

		if (pb->log)
		{
			(*pb->pf_log_xml)(pb->log, ptr_xml);
		}

		psb->soap_request = downcast_xml_to_dom(ptr_xml);
		ptr_xml = NULL;

		if (!rt)
		{
			raise_user_error(NULL, NULL);
		}

		psb->soap_response = create_soap_doc();
		set_soap_default_xmlns(psb->soap_response);
	}

	split_path(pb->file, NULL, token, NULL);
	if (b_wsdl)
		xsprintf(pwb->soap_path, _T("%s/%s"), pb->path, token);
	else
		xsprintf(psb->soap_path, _T("%s/%s"), pb->path, token);

	xsprintf(file, _T("%s.dll"), token);
	h_modu = LoadLibrary(file);

	if (!h_modu)
	{
		raise_user_error(_T("-1"), _T("load soap module falied\n"));
	}

	if (b_wsdl)
	{
		pf_invoke_wsdl = (PF_WSDL_INVOKE)GetProcAddress(h_modu, "wsdl_invoke");

		if (!pf_invoke_wsdl)
		{
			raise_user_error(_T("-1"), _T("get wsdl invoke function falied\n"));
		}

		n_status = (*pf_invoke_wsdl)(pwb);
	}
	else
	{
		pf_invoke_soap = (PF_SOAP_INVOKE)GetProcAddress(h_modu, "soap_invoke");

		if (!pf_invoke_soap)
		{
			raise_user_error(_T("-1"), _T("get soap invoke function falied\n"));
		}

		n_status = (*pf_invoke_soap)(psb);
	}

	if (b_wsdl)
		ptr_xml = upcast_dom_to_xml(pwb->soap_wsdl);
	else
		ptr_xml = upcast_dom_to_xml(psb->soap_response);

	set_xml_encode(ptr_xml, encode);

	format_charset(encode, token);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_TEXTXML, -1);
	xhttp_set_response_content_type_charset(pb->http, token, -1);

	n_size = format_xml_doc_to_bytes(ptr_xml, NULL, MAX_LONG);
	xsprintf(sz_size, _T("%d"), n_size);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, sz_size, -1);

	xhttp_send_xml(pb->http, ptr_xml);

	if (pb->log)
	{
		(*pb->pf_log_xml)(pb->log, ptr_xml);
	}

	if (b_wsdl)
		pwb->soap_wsdl = downcast_xml_to_dom(ptr_xml);
	else
		psb->soap_response = downcast_xml_to_dom(ptr_xml);

	ptr_xml = NULL;

	FreeLibrary(h_modu);
	h_modu = NULL;

	if (b_wsdl)
	{
		destroy_wsdl_doc(pwb->soap_wsdl);
		pwb->soap_wsdl = NULL;

		xmem_free(pwb);
	}
	else
	{
		destroy_soap_doc(psb->soap_request);
		psb->soap_request = NULL;

		destroy_soap_doc(psb->soap_response);
		psb->soap_response = NULL;

		xmem_free(psb);
	}

	END_CATCH;

	return n_status;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[SOAP: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	if (h_modu)
	{
		FreeLibrary(h_modu);
	}

	if (pwb)
	{
		if (pwb->soap_wsdl)
			destroy_wsdl_doc(pwb->soap_wsdl);

		xmem_free(pwb);
	}

	if (psb)
	{
		if (psb->soap_request)
			destroy_soap_doc(psb->soap_request);

		if (psb->soap_response)
			destroy_soap_doc(psb->soap_response);

		xmem_free(psb);
	}

	if (b_wsdl)
	{
		xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);
	}
	else
	{
		_invoke_soap_fault(pb->http, sz_code, sz_error);
	}

	return HTTPS_INVOKE_WITHINFO;
}