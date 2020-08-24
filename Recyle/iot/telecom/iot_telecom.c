/***********************************************************************
	Easily iot telecom

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc iot telecom document

	@module	iot_telecom.c | iot telecom implement file

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

#include "iotdef.h"

typedef struct _tel_t{
	iot_head head;

	tchar_t IspPath[PATH_LEN];

	tchar_t SessionUrl[PATH_LEN];
	tchar_t CommandUrl[PATH_LEN];
	tchar_t AppId[KEY_LEN];
	tchar_t Secret[KEY_LEN];
	tchar_t ServiceId[RES_LEN];
	tchar_t Certify[RES_LEN];
  
	tchar_t err_code[NUM_LEN + 1];
	tchar_t err_text[ERR_LEN + 1];
}tel_t;

static void _set_ssl(xhand_t ssl, tel_t* pal)
{
	file_t xf;
	tchar_t path[PATH_LEN];
	byte_t buf_crt[X509_CERT_SIZE] = { 0 };
	byte_t buf_rsa[RSA_KEY_SIZE] = { 0 };
	dword_t dw_crt, dw_rsa;

	xsprintf(path, _T("%s/%s.crt"), pal->IspPath, pal->Certify);

	xf = xfile_open(NULL, path, 0);
	if (xf)
	{
		dw_crt = X509_CERT_SIZE;
		xfile_read(xf, buf_crt, dw_crt);
		dw_crt = a_xslen((schar_t*)buf_crt);

		xfile_close(xf);
	}

	xsprintf(path, _T("%s/%s.key"), pal->IspPath, pal->Certify);

	xf = xfile_open(NULL, path, 0);
	if (xf)
	{
		dw_rsa = RSA_KEY_SIZE;
		xfile_read(xf, buf_rsa, dw_rsa);
		dw_rsa = a_xslen((schar_t*)buf_rsa);

		xfile_close(xf);
	}

	xssl_set_cert(ssl, buf_crt, dw_crt, buf_rsa, dw_rsa, _T(""), -1);
}

static void _iot_reset(tel_t* pal)
{
	xscpy(pal->err_code, _T(""));
	xscpy(pal->err_text, _T(""));
}

iot_t STDCALL iot_open_isp(const tchar_t* ispfile)
{
	tel_t* pal = NULL;
	LINKPTR d_ptr = NULL;

	TRY_CATCH;

	d_ptr = create_proper_doc();

	if (!load_proper_from_ini_file(d_ptr, NULL, ispfile))
	{
		raise_user_error(NULL, NULL);
	}

	pal = (tel_t*)xmem_alloc(sizeof(tel_t));

	split_path(ispfile, pal->IspPath, NULL, NULL);

	read_proper(d_ptr, _T("ISP"), -1, _T("SessionUrl"), -1, pal->SessionUrl, PATH_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("CommandUrl"), -1, pal->CommandUrl, PATH_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("AppId"), -1, pal->AppId, KEY_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("Secret"), -1, pal->Secret, KEY_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("ServiceId"), -1, pal->ServiceId, RES_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("Certify"), -1, pal->Certify, RES_LEN);

	destroy_proper_doc(d_ptr);
	d_ptr = NULL;


	END_CATCH;

	return (iot_t)(&pal->head);

ONERROR:

	if (d_ptr)
		destroy_proper_doc(d_ptr);

	if (pal)
		xmem_free(pal);

	return NULL;
}

void STDCALL iot_close(iot_t iot)
{
    tel_t* pal = (tel_t*)iot;
	
	XDL_ASSERT(pal != NULL);

	xmem_free(pal);
}

/*
accessToken: 9a9f1c666261daa9e2664b12c6355
tokenType : bearer
refreshToken : c04a6c09ce768dfad080c78a29863
expiresIn : 3600
scope : default
*/
/*
{"error_code":"100208","error_desc":"AppId or secret is not right."}
*/
bool_t STDCALL iot_session(iot_t iot, tchar_t* skey, int len)
{
	tel_t* pal = (tel_t*)iot;

	xhand_t xh = NULL;
	string_t vs = NULL;
	byte_t** pp = NULL;
	dword_t dw = 0;

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk;

	tchar_t sz_errcode[NUM_LEN + 1] = { 0 };
	tchar_t sz_errtext[ERR_LEN + 1] = { 0 };

	XDL_ASSERT(pal != NULL);

	TRY_CATCH;

	xh = xhttp_client(HTTP_METHOD_POST, pal->SessionUrl);
	if (!xh)
	{
		raise_user_error(NULL, NULL);
	}

	_set_ssl(xhttp_bio(xh), pal);

	xssl_set_version(xhttp_bio(xh), TLSv11);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);
	xhttp_set_request_accept_charset(xh, CHARSET_UTF8, -1);

	xhttp_set_request_content_type(xh, _T("application/x-www-form-urlencoded"), -1);
	xhttp_set_request_content_type_charset(xh, CHARSET_UTF8, -1);

	xhttp_set_request_header(xh, _T("appId"), -1, pal->AppId, -1);
	xhttp_set_request_header(xh, _T("secret"), -1, pal->Secret, -1);

	xhttp_set_request_header(xh, _T("User-Agent"), -1, _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)"), -1);

	vs = string_alloc();

	string_printf(vs, _T("%s=%s&%s=%s"), _T("appId"), pal->AppId, _T("secret"), pal->Secret);

	if (!xhttp_send_string(xh, vs))
	{
		raise_user_error(NULL, NULL);
	}

	string_free(vs);
	vs = NULL;

	pp = bytes_alloc();
	
	if (!xhttp_recv_full(xh, pp, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(xh);
	xh = NULL;

	ptr_json = create_json_doc();

	parse_json_doc_from_bytes(ptr_json, *pp, dw, _UTF8);

	bytes_free(pp);
	pp = NULL;

	xszero(skey, len);

	nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		if (compare_text(get_json_item_name_ptr(nlk), -1, _T("accessToken"), -1, 1) == 0)
		{
			xsncpy(skey, get_json_item_value_ptr(nlk), len);
		}
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("error_code"), -1, 1) == 0)
		{
			xsncpy(sz_errcode, get_json_item_value_ptr(nlk), NUM_LEN);
		}
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("error_desc"), -1, 1) == 0)
		{
			xsncpy(sz_errtext, get_json_item_value_ptr(nlk), ERR_LEN);
		}

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	if (is_null(skey))
	{
		raise_user_error(sz_errcode, sz_errtext);
	}

	END_CATCH;

	return 1;
ONERROR:
	get_last_error(pal->err_code, pal->err_text, ERR_LEN);

	if (xh)
		xhttp_close(xh);

	if (vs)
		string_free(vs);

	if (pp)
		bytes_free(pp);

	if (ptr_json)
		destroy_json_doc(ptr_json);

	return 0;
}

//resultcode: 1010005
//resultmsg : Invalid access token or appId.
//{"error_code":"100418","error_desc":"The deviceData is not existed."}
bool_t STDCALL iot_command(iot_t iot, iot_command_t* pio)
{
	tel_t* pal = (tel_t*)iot;

	xhand_t xh = NULL;
	byte_t** pp = NULL;
	dword_t dw = 0;

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk_row, nlk, nlk_child;

	XDL_ASSERT(pal != NULL);

	TRY_CATCH;

	xh = xhttp_client(HTTP_METHOD_POST, pal->CommandUrl);
	if (!xh)
	{
		raise_user_error(NULL, NULL);
	}

	_set_ssl(xhttp_bio(xh), pal);

	xssl_set_version(xhttp_bio(xh), TLSv11);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);
	xhttp_set_request_accept_charset(xh, CHARSET_UTF8, -1);

	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_request_header(xh, _T("app_key"), -1, pal->AppId, -1);
	xhttp_set_request_header(xh, _T("Authorization"), -1, pio->session_key, -1);

	xhttp_set_request_header(xh, _T("User-Agent"), -1, _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)"), -1);

	ptr_json = create_json_doc();

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("deviceId"), -1);
	set_dom_node_text(nlk_row, pio->device_id, -1);

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("expireTime"), -1);
	set_dom_node_text(nlk_row, _T("0"), -1);

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("command"), -1);

	nlk = insert_json_item(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("serviceId"), -1);
	set_dom_node_text(nlk, pal->ServiceId, -1);

	nlk = insert_json_item(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("method"), -1);
	set_dom_node_text(nlk, _T("command"), -1);

	nlk = insert_json_item(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("paras"), -1);

	nlk_child = insert_json_item(nlk, LINK_LAST);
	set_dom_node_name(nlk_child, _T("payload"), -1);
	set_dom_node_text(nlk_child, pio->command_input, -1);

	if (!xhttp_send_json(xh, ptr_json))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	pp = bytes_alloc();

	if (!xhttp_recv_full(xh, pp, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(xh);
	xh = NULL;

	ptr_json = create_json_doc();

	parse_json_doc_from_bytes(ptr_json, *pp, dw, _UTF8);

	bytes_free(pp);
	pp = NULL;

	nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		if (compare_text(get_json_item_name_ptr(nlk), -1, _T("resultcode"), -1, 1) == 0 || compare_text(get_json_item_name_ptr(nlk), -1, _T("error_code"), -1, 1) == 0)
		{
			xsncpy(pal->err_code, get_json_item_value_ptr(nlk), NUM_LEN);
		}
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("resultmsg"), -1, 1) == 0 || compare_text(get_json_item_name_ptr(nlk), -1, _T("error_desc"), -1, 1) == 0)
		{
			xsncpy(pal->err_text, get_json_item_value_ptr(nlk), ERR_LEN);
		}

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	END_CATCH;

	return (xstol(pal->err_code) == 0) ? 1 : 0;
ONERROR:
	get_last_error(pal->err_code, pal->err_text, ERR_LEN);

	if (xh)
		xhttp_close(xh);

	if (pp)
		bytes_free(pp);

	if (ptr_json)
		destroy_json_doc(ptr_json);

	return 0;
}

int STDCALL iot_error(iot_t iot, tchar_t* buf, int max)
{
	tel_t* pal = (tel_t*)iot;

	XDL_ASSERT(pal != NULL);

	max = (max < ERR_LEN) ? max : ERR_LEN;
	if (buf)
	{
		xsncpy(buf, pal->err_text, max);
	}

	return -1;
}
