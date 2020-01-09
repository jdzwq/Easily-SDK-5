/***********************************************************************
	Easily sms aliyun

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc sms aliyun document

	@module	sms_aliyun.c | sms aliyun implement file

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

#include "smspro.h"

typedef struct _ali_code_t{
	tchar_t Location[PATH_LEN];

	tchar_t SignatureMethod[MAX_SMS_NAME];
	tchar_t SignatureNonce[MAX_SMS_NAME];
	tchar_t AccessKeyId[MAX_SMS_NAME];
	tchar_t AccessSecret[MAX_SMS_NAME];
	tchar_t SignatureVersion[MIN_SMS_NAME];
	tchar_t Timestamp[DATE_LEN];
	tchar_t Format[MIN_SMS_NAME];

	tchar_t Action[MIN_SMS_NAME];
	tchar_t Version[MIN_SMS_NAME];
	tchar_t RegionId[MIN_SMS_NAME];
	tchar_t SignName[MAX_SMS_NAME];
	tchar_t TemplateCode[MIN_SMS_NAME];
}ali_code_t;

typedef struct _ali_t{
	sms_head head;

	ali_code_t code;
  
	tchar_t err_code[NUM_LEN + 1];
	tchar_t err_text[ERR_LEN + 1];
}ali_t;

static void aliyun_encode(byte_t** pbuf)
{
	byte_t* p;
	dword_t i, n;

	p = *pbuf;
	n = bytes_size(pbuf);

	for (i = 0; i < n; i++)
	{
		if (p[i] == '+') //replace as "%20"
		{
			bytes_replace(pbuf, i, 1, (byte_t*)"%20", 3);
			n += 2;
			i += 2;
		}
		else if (p[i] == '*') //replace as "%2A"
		{
			bytes_replace(pbuf, i, 1, (byte_t*)"%2A", 3);
			n += 2;
			i += 2;
		}
		else if (p[i] == '%' && p[i + 1] == '7' && p[i + 2] == 'E') //replace as "~"
		{
			bytes_replace(pbuf, i, 3, (byte_t*)"~", 1);
			n -= 2;
		}
	}
}

static void _sms_reset(ali_t* pal)
{
	xscpy(pal->err_code, _T(""));
	xscpy(pal->err_text, _T(""));
}

sms_t STDCALL sms_open_isp(const tchar_t* ispfile)
{
	ali_t* pal = NULL;
	LINKPTR d_ptr = NULL;

	TRY_CATCH;

	d_ptr = create_proper_doc();

	if (!load_proper_from_ini_file(d_ptr, NULL, ispfile))
	{
		raise_user_error(NULL, NULL);
	}

	pal = (ali_t*)xmem_alloc(sizeof(ali_t));

	read_proper(d_ptr, _T("CODE"), -1, _T("Location"), -1, pal->code.Location, PATH_LEN);
	read_proper(d_ptr, _T("CODE"), -1, _T("SignatureMethod"), -1, pal->code.SignatureMethod, MAX_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("AccessKeyId"), -1, pal->code.AccessKeyId, MAX_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("AccessSecret"), -1, pal->code.AccessSecret, MAX_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("SignatureVersion"), -1, pal->code.SignatureVersion, MIN_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("Format"), -1, pal->code.Format, MIN_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("Action"), -1, pal->code.Action, MIN_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("Version"), -1, pal->code.Version, MIN_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("RegionId"), -1, pal->code.RegionId, MIN_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("SignName"), -1, pal->code.SignName, MAX_SMS_NAME);
	read_proper(d_ptr, _T("CODE"), -1, _T("TemplateCode"), -1, pal->code.TemplateCode, MIN_SMS_NAME);

	destroy_proper_doc(d_ptr);
	d_ptr = NULL;


	END_CATCH;

	return (sms_t)(&pal->head);

ONERROR:

	get_last_error(pal->err_code, pal->err_text, ERR_LEN);

	if (d_ptr)
		destroy_proper_doc(d_ptr);

	if (pal)
		xmem_free(pal);

	return NULL;
}

void STDCALL sms_close(sms_t sms)
{
    ali_t* pal = (ali_t*)sms;
	
	XDL_ASSERT(pal != NULL);

	xmem_free(pal);
}

//<?xml version='1.0' encoding='UTF-8'?><SendSmsResponse><Message>OK</Message><RequestId>0F5E97D1-6EF7-4FC4-AAD9-53B64C78F3EF</RequestId><BizId>607923175813302502^0</BizId><Code>OK</Code></SendSmsResponse>

bool_t STDCALL sms_code(sms_t sms, const tchar_t* phone, const tchar_t* param, int len)
{
	ali_t* pal = (ali_t*)sms;
	xhand_t http = NULL;

	XDL_ASSERT(pal != NULL);

	lword_t ts = 0;
	nuid_t ui = { 0 };
	tchar_t uuid[KEY_LEN] = { 0 };
	xdate_t dt = { 0 };
	tchar_t stamp[DATE_LEN] = { 0 };

	LINKPTR st = NULL;

	tchar_t* opt_buf = NULL;
	int opt_len;

	byte_t* qry_buf = NULL;
	dword_t qry_len;

	byte_t** pb = NULL;

	byte_t* sig_buf = NULL;
	dword_t sig_len;

	byte_t screct[KEY_LEN] = { 0 };
	dword_t dw;
	byte_t hmac[20] = { 0 };
	byte_t sign[RES_LEN] = { 0 };

	TRY_CATCH;
	
	_sms_reset(pal);

	ts = get_timestamp();
	nuid_from_timestamp(&ui, ts);
	nuid_format_string(&ui, uuid, 50);
	utc_date_from_timestamp(&dt, ts);
	format_utctime(&dt, stamp);

	//create sorted param table
	st = create_string_table(1);

	write_string_entity(st, _T("SignatureMethod"), -1, pal->code.SignatureMethod, -1);
	write_string_entity(st, _T("SignatureNonce"), -1, uuid, -1);
	write_string_entity(st, _T("AccessKeyId"), -1, pal->code.AccessKeyId, -1);
	write_string_entity(st, _T("SignatureVersion"), -1, pal->code.SignatureVersion, -1);
	write_string_entity(st, _T("Timestamp"), -1, stamp, -1);
	write_string_entity(st, _T("Format"), -1, pal->code.Format, -1);

	write_string_entity(st, _T("Action"), -1, pal->code.Action, -1);
	write_string_entity(st, _T("Version"), -1, pal->code.Version, -1);
	write_string_entity(st, _T("RegionId"), -1, pal->code.RegionId, -1);
	write_string_entity(st, _T("PhoneNumbers"), -1, phone, -1);
	write_string_entity(st, _T("SignName"), -1, pal->code.SignName, -1);
	write_string_entity(st, _T("TemplateParam"), -1, param, len);
	write_string_entity(st, _T("TemplateCode"), -1, pal->code.TemplateCode, -1);
	//write_string_entity(st, _T("OutId"), -1, _T(""), -1);

	opt_len = string_table_format_options(st, NULL, MAX_LONG, _T('='), _T('&'));
	opt_buf = xsalloc(opt_len + 1);
	string_table_format_options(st, opt_buf, opt_len, _T('='), _T('&'));
	if (opt_len)
	{
		opt_len--;
		opt_buf[opt_len] = _T('\0');
	}

	destroy_string_table(st);
	st = NULL;

	//build encoded query token
	qry_len = xhttp_url_encoding(opt_buf, opt_len, NULL, MAX_LONG);
	qry_buf = (byte_t*)xmem_alloc(qry_len + 1);
	xhttp_url_encoding(opt_buf, opt_len, qry_buf, qry_len);

	xmem_free(opt_buf);
	opt_buf = NULL;

	//aliyun special encoding
	pb = bytes_alloc();
	bytes_attach(pb, qry_buf, qry_len);
	aliyun_encode(pb);
	qry_len = bytes_size(pb);
	qry_buf = bytes_detach(pb);

	//build the signature token, include head: "GET&%2F&", 8bytes
	sig_len = 8 + url_byte_encode(qry_buf, qry_len, NULL, MAX_LONG);
	sig_buf = (byte_t*)xmem_alloc(sig_len + 1);
	xmem_copy((void*)sig_buf, (void*)"GET&%2F&", 8);
	url_byte_encode(qry_buf, qry_len, sig_buf + 8, sig_len - 8);

	bytes_attach(pb, sig_buf, sig_len);
	aliyun_encode(pb);
	sig_len = bytes_size(pb);
	sig_buf = bytes_detach(pb);

#ifdef _UNICODE
	dw = ucs_to_utf8(pal->code.AccessSecret, -1, screct, KEY_LEN);
#else
	dw = mbs_to_utf8(pal->code.AccessSecret, -1, screct, KEY_LEN);
#endif

	//add '&'
	screct[dw] = '&';
	dw++;

	//calc the signature
	sha1_hmac(screct, dw, sig_buf, sig_len, hmac);
	dw = a_xbas_encode(hmac, 20, (schar_t*)sign, RES_LEN);

	xmem_free(sig_buf);
	sig_buf = NULL;

	//encoding the signature token, include head: "Signature=" and tail: "&", total 11 bytes
	sig_len = 10 + url_byte_encode(sign, dw, NULL, MAX_LONG);
	sig_buf = (byte_t*)xmem_alloc(sig_len + 2); //with the tail: '&'
	xmem_copy((void*)sig_buf, (void*)"Signature=", 10);
	url_byte_encode(sign, dw, sig_buf + 10, sig_len - 10);
	sig_buf[sig_len] = '&';
	sig_len++;

	//aliyun special encoding
	bytes_attach(pb, sig_buf, sig_len);
	aliyun_encode(pb);
	sig_len = bytes_size(pb);
	sig_buf = bytes_detach(pb);

	//add the "Signature=**" into query token
	bytes_attach(pb, qry_buf, qry_len);
	bytes_insert(pb, 0, sig_buf, sig_len);
	qry_len = bytes_size(pb);
	qry_buf = bytes_detach(pb);

	xmem_free(sig_buf);
	sig_buf = NULL;

	bytes_free(pb);
	pb = NULL;

	http = xhttp_client(_T("GET"), pal->code.Location);
	if (!http)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(http);
	xhttp_set_encoded_query(http, qry_buf, qry_len);

	xmem_free(qry_buf);
	qry_buf = NULL;

	if (!xhttp_send_request(http))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(http))
	{
		raise_user_error(NULL, NULL);
	}

	dw = xhttp_get_response_content_length(http);

	qry_buf = (byte_t*)xmem_alloc(dw + 1);
	
	if (!xhttp_recv(http, qry_buf, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(http);
	http = NULL;

#ifdef _UNICODE
	utf8_to_ucs(qry_buf, dw, pal->err_text, ERR_LEN - 1);
#else
	utf8_to_mbs(qry_buf, dw, pal->err_text, ERR_LEN - 1);
#endif

	xmem_free(qry_buf);
	qry_buf = NULL;

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(pal->err_code, pal->err_text, ERR_LEN);

	if (opt_buf)
		xsfree(opt_buf);

	if (qry_buf)
		xmem_free(qry_buf);

	if (sig_buf)
		xmem_free(sig_buf);

	if (pb)
		bytes_free(pb);

	if (http)
		xhttp_close(http);

	return 0;
}

bool_t STDCALL sms_message(sms_t sms, const tchar_t* phone, const tchar_t* param, int len, tchar_t* msgid, int max)
{
	return 0;
}

bool_t STDCALL sms_receipt(sms_t sms, const tchar_t* phone, const tchar_t* msgid, int len, tchar_t* receipt, int max)
{
	return 0;
}

int STDCALL sms_error(sms_t sms, tchar_t* buf, int max)
{
	ali_t* pal = (ali_t*)sms;

	XDL_ASSERT(pal != NULL);

	max = (max < ERR_LEN) ? max : ERR_LEN;
	if (buf)
	{
		xsncpy(buf, pal->err_text, max);
	}

	return -1;
}
