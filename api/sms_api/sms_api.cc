
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

#include "sms_api.h"

typedef struct _sms_block_t{
	hex_obj_t hdb;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];

	PF_SMS_OPEN_ISP pf_open_isp;
	PF_SMS_CLOSE pf_close;
	PF_SMS_CODE pf_code;
	PF_SMS_ERROR pf_error;

	tchar_t isp_file[PATH_LEN];
}sms_block_t;


bool_t _invoke_sms_code(const https_block_t* pb, sms_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	bool_t rt;
	SMS sms = NULL;

	tchar_t code[INT_LEN] = { 0 };
	tchar_t param[KEY_LEN] = { 0 };
	tchar_t phone[INT_LEN] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	byte_t* buf = NULL;
	dword_t len;
	dword_t total = 0;

	byte_t* msg_buf = NULL;
	dword_t msg_len;
	byte_t* han_buf = NULL;
	sword_t han_len;
	byte_t* hdr_buf = NULL;
	dword_t hdr_len;
	byte_t* sms_buf = NULL;
	dword_t sms_len;

	xdate_t dt;
	tchar_t sz_date[DATE_LEN] = { 0 };

	hex_obj_t hkv = NULL;

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, _T("Phone"), -1, phone, INT_LEN);

	if (is_null(phone))
	{
		raise_user_error(_T("0"), _T("phone empty!"));
	}

	sms = (*pxb->pf_open_isp)(pxb->isp_file);
	if (!sms)
	{
		raise_user_error(NULL, NULL);
	}

	Srand48(time(NULL));
	xsprintf(code, _T("%06d"), Lrand48());
	xsprintf(param, _T("{\"code\":\"%s\"}"), code);

	rt = (*pxb->pf_code)(sms, phone, param, -1);

	(*pxb->pf_error)(sms, sz_error, ERR_LEN);

	if (!rt)
	{
		raise_user_error(_T("0"), sz_error);
	}

	(*pxb->pf_close)(sms);
	sms = NULL;

	get_utc_date(&dt);
	format_utctime(&dt, sz_date);

	hkv = hexkv_create(pxb->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_sms_code"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, phone, -1);

	val = object_alloc(_UTF8);

	len = 4 + 2 + APPHAND_SIZE + 2 + SMSHEADER_SIZE + 4 + SMSMESSAGE_SIZE;
	buf = (byte_t*)xmem_alloc(len);
	
	//the message total size
	msg_buf = buf + 4;
	msg_len = (2 + APPHAND_SIZE + 2 + SMSHEADER_SIZE + 4 + SMSMESSAGE_SIZE);
	PUT_DWORD_NET((msg_buf - 4), 0, msg_len);
	//the message handler size
	han_buf = msg_buf + 2;
	han_len = APPHAND_SIZE;
	PUT_SWORD_NET((han_buf - 2), 0, han_len);
	//the message handler
	xmem_copy((void*)(han_buf), (void*)APPVER, 4);
	//the message header size
	hdr_buf = han_buf + APPHAND_SIZE + 2;
	hdr_len = SMSHEADER_SIZE;
	PUT_SWORD_NET((hdr_buf - 2), 0, hdr_len);
	//the message header
#if defined(_UNICODE) || defined(UNICODE)
	ucs_to_utf8(sz_date, -1, hdr_buf, TIMESTAMP_SIZE);
#else
	mbs_to_utf8(sz_date, -1, hdr_buf, TIMESTAMP_SIZE);
#endif
	//the message element size
	sms_buf = hdr_buf + SMSHEADER_SIZE + 4;
	sms_len = SMSMESSAGE_SIZE;
	PUT_DWORD_NET((sms_buf - 4), 0, sms_len);
	//the message element
#if defined(_UNICODE) || defined(UNICODE)
	ucs_to_utf8(code, -1, sms_buf, SMSMESSAGE_SIZE);
#else
	mbs_to_utf8(code, -1, sms_buf, SMSMESSAGE_SIZE);
#endif

	object_set_bytes(val, _UTF8, buf, len);

	xmem_free(buf);
	buf = NULL;

	hexkv_attach(hkv, key, val);
	val = NULL;

	variant_to_null(&key);

	hexkv_destroy(hkv);
	hkv = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_200, HTTP_CODE_200_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[SMS-Code]"), -1);

		xsprintf(sz_error, _T("SMS succeed"), 1);
		(*pb->pf_log_error)(pb->log, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (sms)
	{
		(*pxb->pf_close)(sms);
	}

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		hexkv_destroy(hkv);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[SMS-Code]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_sms_verify(const https_block_t* pb, sms_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	bool_t rt;

	tchar_t code[INT_LEN] = { 0 };
	tchar_t phone[INT_LEN] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	byte_t* buf = NULL;
	dword_t len;
	dword_t total = 0;

	byte_t* msg_buf = NULL;
	dword_t msg_len;
	byte_t* han_buf = NULL;
	sword_t han_len;
	byte_t* hdr_buf = NULL;
	dword_t hdr_len;
	byte_t* sms_buf = NULL;
	dword_t sms_len;
	byte_t ver[5] = { 0 };

	xdate_t dt_org, dt_cur;
	tchar_t org_date[DATE_LEN] = { 0 };
	tchar_t org_code[SMSMESSAGE_SIZE + 1] = { 0 };

	hex_obj_t hkv = NULL;

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, _T("Phone"), -1, phone, INT_LEN);
	xhttp_get_url_query_entity(pb->http, _T("Code"), -1, code, INT_LEN);

	if (is_null(phone))
	{
		raise_user_error(_T("0"), _T("Phone empty!"));
	}

	if (is_null(code))
	{
		raise_user_error(_T("0"), _T("Code empty!"));
	}

	hkv = hexkv_create(pxb->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, phone, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	len = object_get_bytes(val, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(len);

	object_get_bytes(val, buf, len);

	//the message total size
	msg_buf = buf + 4;
	msg_len = GET_DWORD_NET((msg_buf - 4), 0);
	if (msg_len > MAX_LONG)
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("invalid message total size"));
	}

	//the message handler
	han_buf = msg_buf + 2;
	han_len = GET_SWORD_NET((han_buf - 2), 0);
	if (han_len > MAX_SHORT)
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("invalid message handler size"));
	}
	xmem_copy((void*)ver, han_buf, 4);
	if (ver[0] != 'S' || ver[1] != 'M')
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("invalid message handler"));
	}

	//the message header
	hdr_buf = han_buf + han_len + 2;
	hdr_len = GET_SWORD_NET((hdr_buf - 2), 0);
	if (hdr_len > MAX_SHORT)
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("invalid message header size"));
	}
#if defined(_UNICODE) || defined(UNICODE)
	utf8_to_ucs(hdr_buf, TIMESTAMP_SIZE, org_date, DATE_LEN);
#else
	utf8_to_mbs(hdr_buf, TIMESTAMP_SIZE, org_date, DATE_LEN);
#endif

	//the message element
	sms_buf = hdr_buf + hdr_len + 4;
	sms_len = GET_DWORD_NET((sms_buf - 4), 0);
	if (sms_len > MAX_LONG)
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("invalid message element size"));
	}
#if defined(_UNICODE) || defined(UNICODE)
	utf8_to_ucs(sms_buf, SMSMESSAGE_SIZE, org_code, SMSMESSAGE_SIZE);
#else
	utf8_to_mbs(sms_buf, SMSMESSAGE_SIZE, org_code, SMSMESSAGE_SIZE);
#endif

	xmem_free(buf);
	buf = NULL;

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	hexkv_destroy(hkv);
	hkv = NULL;

	if (compare_text(code, -1, org_code, -1, 1) != 0)
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("invalid sms code"));
	}

	parse_datetime(&dt_org, org_date);

	get_utc_date(&dt_cur);

	if (diff_mins(&dt_org, &dt_cur) > 5)
	{
		raise_user_error(_T("_invoke_sms_verify"), _T("sms code expired"));
	}

	xhttp_send_error(pb->http, HTTP_CODE_200, HTTP_CODE_200_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[SMS-Verify]"), -1);

		xsprintf(sz_error, _T("SMS succeed"), 1);
		(*pb->pf_log_error)(pb->log, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		hexkv_destroy(hkv);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[SMS-Verify]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, sms_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	d_recv = bytes_alloc();

	xhttp_recv_full(pb->http, d_recv, &n_size);

	bytes_free(d_recv);
	d_recv = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[SMS-Error]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

/****************************************************************************************************/

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	sms_block_t* pxb = NULL;

	tchar_t action[RES_LEN] = { 0 };
	int len;

	tchar_t token[RES_LEN] = { 0 };
	tchar_t file[PATH_LEN] = { 0 };
	LINKPTR ptr_prop = NULL;

	res_modu_t sms_lib = NULL;

	bool_t rt = 1;

	TRY_CATCH;

	pxb = (sms_block_t*)xmem_alloc(sizeof(sms_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/sms.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load sms config falied\n"));
	}

	xszero(file, PATH_LEN);
	read_proper(ptr_prop, _T("SMS"), -1, _T("DATABASE"), -1, token, RES_LEN);
	read_proper(ptr_prop, _T("SMS"), -1, _T("LOCATION"), -1, file, PATH_LEN);
	read_proper(ptr_prop, _T("SMS"), -1, _T("PUBLICKEY"), -1, pxb->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("SMS"), -1, _T("PRIVATEKEY"), -1, pxb->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pxb->local, file);

	if (xsnicmp(_T("aliyun"), pb->object + 1, xslen(_T("aliyun"))) == 0)
	{
#if defined(_OS_WINDOWS)
		sms_lib = load_library(_T("sms_aliyun.dll"));
#elif defined(_OS_MACOS)
		sms_lib = load_library(_T("libsms_aliyun.dylib"));
#elif defined(_OS_LINUX)
		sms_lib = load_library(_T("libsms_aliyun.so"));
#endif
		len += xslen(_T("aliyun"));
	}
	else
	{
		raise_user_error(_T("sms_api"), _T("unknown sms library"));
	}

	if (!sms_lib)
	{
		raise_user_error(_T("sms_api"), _T("load sms library falied\n"));
	}

	pxb->pf_open_isp = (PF_SMS_OPEN_ISP)get_address(sms_lib, "sms_open_isp");
	pxb->pf_close = (PF_SMS_CLOSE)get_address(sms_lib, "sms_close");
	pxb->pf_code = (PF_SMS_CODE)get_address(sms_lib, "sms_code");
	pxb->pf_error = (PF_SMS_ERROR)get_address(sms_lib, "sms_error");

	if (!pxb->pf_open_isp || !pxb->pf_close || !pxb->pf_code || !pxb->pf_error)
	{
		raise_user_error(_T("sms_api"), _T("get open/close functon falied"));
	}

	xsprintf(pxb->isp_file, _T("%s%s"), pb->path, pb->object);

	pxb->hdb = hexdb_create(pxb->local, token);
	if (!pxb->hdb)
	{
		raise_user_error(_T("-1"), _T("open database failed"));
	}

	xhttp_get_url_query_entity(pb->http, _T("Action"), -1, action, RES_LEN);

	if (compare_text(action, -1, SMS_ACTION_CODE, -1, 1) == 0)
	{
		rt = _invoke_sms_code(pb, pxb);
	}
	else if (compare_text(action, -1, SMS_ACTION_VERIFY, -1, 1) == 0)
	{
		rt = _invoke_sms_verify(pb, pxb);
	}
	else
	{
		raise_user_error(_T("sms_api"), _T("unknown sms function\n"));
	}

	hexdb_destroy(pxb->hdb);
	pxb->hdb = NULL;

	free_library(sms_lib);

	xmem_free(pxb);

	END_CATCH;

	return (rt)? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	if (pxb)
	{
		_invoke_error(pb, pxb);
	}

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (sms_lib)
		free_library(sms_lib);

	if (pxb)
	{
		if (pxb->hdb)
			hexdb_destroy(pxb->hdb);

		xmem_free(pxb);
	}

	return HTTPS_INVOKE_WITHINFO;
}
