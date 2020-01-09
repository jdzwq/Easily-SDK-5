/***********************************************************************
	Easily sms web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc sms web api document

	@module	sms_api.cc | sms web api implement file

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

#include "sms_cmc.h"
#include "sms_api.h"


#define SMS_SEND					_T("SMS_SEND")
#define SMS_SEND_TIME				_T("SMS_SEND_TIME")

bool_t sms_send
(const tchar_t* url,
const tchar_t* acc_id,
const tchar_t* acc_key,
const tchar_t* phone,
const tchar_t* in_data,
int in_size,
tchar_t* out_buf,
int* out_max)
{
	XHANDLE xhttp = NULL;
	LINKPTR soap_send = NULL;
	LINKPTR soap_recv = NULL;
	LINKPTR nlk_req, nlk_param, nlk_child;
	bool_t rt;
	tchar_t errcode[NUM_LEN + 1] = { 0 };

	TRY_CATCH;

	xhttp = xhttp_client(_T("POST"), url);
	if (!xhttp)
	{
		raise_user_error(NULL, NULL);
	}

	soap_send = create_soap_doc();
	set_soap_default_xmlns(soap_send);

	nlk_req = set_soap_request_node(soap_send, _T("SendMessage3"), NULL, _T("http://openmas.chinamobile.com/sms"));

	nlk_param = set_soap_request_param(nlk_req, _T("destinationAddresses"), ATTR_DATA_TYPE_UNKNOWN, NULL, -1);
	set_dom_node_xmlns(nlk_param, _T("xmlns:a"), -1, _T("http://schemas.microsoft.com/2003/10/Serialization/Arrays"), -1);
	nlk_child = insert_dom_node(nlk_param, LINK_LAST);
	set_dom_node_name(nlk_child, _T("string"), -1);
	set_dom_node_ns(nlk_child, _T("a"), -1);
	set_dom_node_text(nlk_child, phone, -1);

	nlk_param = set_soap_request_param(nlk_req, _T("message"), ATTR_DATA_TYPE_STRING, (void*)in_data, in_size);

	nlk_param = set_soap_request_param(nlk_req, _T("extendCode"), ATTR_DATA_TYPE_STRING, _T("2"), -1);

	nlk_param = set_soap_request_param(nlk_req, _T("applicationId"), ATTR_DATA_TYPE_STRING, (void*)acc_id, -1);

	nlk_param = set_soap_request_param(nlk_req, _T("password"), ATTR_DATA_TYPE_STRING, (void*)acc_key, -1);

	xhttp_set_request_default_header(xhttp);
	xhttp_set_request_header(xhttp, HTTP_HEADER_SOAPACTION, -1, _T("http://openmas.chinamobile.com/sms/ISms/SendMessage3"), -1);
	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTXML, -1);
	xhttp_set_request_content_type_charset(xhttp, _T("utf-8"), -1);
	xhttp_set_request_header(xhttp, HTTP_HEADER_CONNECTION, -1, HTTP_HEADER_CONNECTION_KEEPALIVE, -1);

	soap_recv = create_soap_doc();

	rt = xhttp_invoke_soap(xhttp, soap_send, soap_recv);
	if (!rt)
	{
		raise_user_error(NULL, NULL);
	}

	destroy_soap_doc(soap_send);
	soap_send = NULL;

	destroy_soap_doc(soap_recv);
	soap_recv = NULL;

	xhttp_close(xhttp);
	xhttp = NULL;

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(errcode, out_buf, ERR_LEN);

	if (soap_send)
		destroy_soap_doc(soap_send);

	if (soap_recv)
		destroy_soap_doc(soap_recv);

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t __stdcall sms_ioctl(
	const tchar_t* io_op,
	const tchar_t* url,
	const tchar_t* acc_id,
	const tchar_t* acc_key,
	const tchar_t* phone,
	const tchar_t* in_data,
	int in_size,
	tchar_t* out_buf,
	int* out_max
)
{
	if (compare_text(io_op, -1, SMS_SEND, -1, 0) == 0)
		return sms_send(url,acc_id,acc_key,phone,in_data,in_size,out_buf,out_max);

	return 0;
}