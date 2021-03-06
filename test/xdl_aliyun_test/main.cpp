﻿
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif


void test_bytes()
{
	byte_t** pp = bytes_alloc();

	bytes_insert(pp, 0, (byte_t*)"123456", 6);
	dword_t dw = bytes_size(pp);

	bytes_insert(pp, dw, (byte_t*)"789", 3);
	dw = bytes_size(pp);
	bytes_insert(pp, 0, (byte_t*)"abc", 3);
	dw = bytes_size(pp);
	bytes_insert(pp, dw-1, (byte_t*)"efg", 3);
	dw = bytes_size(pp);
	bytes_delete(pp, dw-1,1);

	dw = bytes_size(pp);
	bytes_replace(pp, 0, 3, (byte_t*)"0", 1);

	dw = bytes_size(pp);
	bytes_replace(pp, dw-2, 1, (byte_t*)"123", 3);

	dw = bytes_size(pp);
	schar_t sz[20] = { 0 };
	a_xsncpy(sz, (schar_t*)(*pp), dw);

	bytes_free(pp);
}

/*
1.	paras.put("SignatureMethod", "HMAC-SHA1");
2.	paras.put("SignatureNonce", java.util.UUID.randomUUID().toString());
3.	paras.put("AccessKeyId", accessKeyId);
4.	paras.put("SignatureVersion", "1.0");
5.	paras.put("Timestamp", df.format(new java.util.Date()));
6.	paras.put("Format", "XML");

1.	paras.put("Action", "SendSms");
2.	paras.put("Version", "2017-05-25");
3.	paras.put("RegionId", "cn-hangzhou");
4.	paras.put("PhoneNumbers", "15300000001");
5.	paras.put("SignName", "短信模板名称");
6.	paras.put("TemplateParam", "{\"customer\":\"test\"}");
7.	paras.put("TemplateCode", "SMS_71390007");
8.	paras.put("OutId", "123");
*/

//replace("+", "%20").replace("*", "%2A").replace("%7E", "~");
//stringToSign.append("GET").append("&");
//stringToSign.append(specialUrlEncode("/")).append("&");
//stringToSign.append(specialUrlEncode(sortedQueryString));

void aliyun_encode(byte_t** pbuf)
{
	byte_t* p;
	dword_t i,n;

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

//AccessKeyId=testId
//&Action=SendSms
//&Format=XML
//&OutId=123
//&PhoneNumbers=15300000001
//&RegionId=cn-hangzhou
//&SignName=短信模板名称
//&SignatureMethod=HMAC-SHA1
//&SignatureNonce=45e25e9b-0a6f-4070-8c85-2956eda1b466
//&SignatureVersion=1.0
//&TemplateCode=SMS_71390007
//&TemplateParam={"customer":"test"}
//&Timestamp=2017-07-12T02:42:19Z
//&Version=2017-05-25


void test_encode()
{
	LINKPTR st = create_string_table(1);

	write_string_entity(st, _T("SignatureMethod"), -1, _T("HMAC-SHA1"), -1);
	write_string_entity(st, _T("SignatureNonce"), -1, _T("45e25e9b-0a6f-4070-8c85-2956eda1b466"), -1);
	write_string_entity(st, _T("AccessKeyId"), -1, _T("testId"), -1);
	write_string_entity(st, _T("SignatureVersion"), -1, _T("1.0"), -1);
	write_string_entity(st, _T("Timestamp"), -1, _T("2017-07-12T02:42:19Z"), -1);
	write_string_entity(st, _T("Format"), -1, _T("XML"), -1);

	write_string_entity(st, _T("Action"), -1, _T("SendSms"), -1);
	write_string_entity(st, _T("Version"), -1, _T("2017-05-25"), -1);
	write_string_entity(st, _T("RegionId"), -1, _T("cn-hangzhou"), -1);
	write_string_entity(st, _T("PhoneNumbers"), -1, _T("15300000001"), -1);
	write_string_entity(st, _T("SignName"), -1, _T("短信模板名称"), -1);
	write_string_entity(st, _T("TemplateParam"), -1, _T("{\"customer\":\"test\"}"), -1);
	write_string_entity(st, _T("TemplateCode"), -1, _T("SMS_71390007"), -1);
	write_string_entity(st, _T("OutId"), -1, _T("123"), -1);

	int len = string_table_format_options(st, NULL, MAX_LONG, _T('='), _T('&'));
	tchar_t* buf = xsalloc(len + 1);
	string_table_format_options(st, buf, len, _T('='), _T('&'));
	if (len)
	{
		len--;
		buf[len] = _T('\0');
	}

	destroy_string_table(st);

	byte_t** pbuf = bytes_alloc();
	byte_t* enc;
	dword_t enc_len;

	enc_len = xhttp_url_encoding(buf, len, NULL, MAX_LONG);
	enc = bytes_realloc(pbuf, enc_len);
	xhttp_url_encoding(buf, len, enc, enc_len);

	xmem_free(buf);

	aliyun_encode(pbuf);

	//hdr_len = 8; //"GET&%2F&"

	enc_len = 8 + url_byte_encode(*pbuf, bytes_size(pbuf), NULL, MAX_LONG);
	enc = (byte_t*)xmem_alloc(enc_len + 1);
	xmem_copy((void*)enc, (void*)"GET&%2F&", 8);
	url_byte_encode(*pbuf, bytes_size(pbuf), enc + 8, enc_len - 8);

	byte_t** pp = bytes_alloc();
	bytes_attach(pp, enc, enc_len);
	aliyun_encode(pp);

	byte_t key[] = "testSecret&";
	dword_t key_len = a_xslen((schar_t*)key);
	byte_t hmac[20] = { 0 };
	sha1_hmac(key, key_len, *pp, bytes_size(pp), hmac);

	byte_t sign[RES_LEN] = { 0 };
	dword_t sign_len;
	sign_len = a_xbas_encode(hmac, 20, (schar_t*)sign, RES_LEN);

	//Signature=
	enc_len = 10 + url_byte_encode(sign, sign_len, NULL, MAX_LONG);
	enc = (byte_t*)xmem_alloc(enc_len + 2); //&
	xmem_copy((void*)enc, (void*)"Signature=", 10);
	url_byte_encode(sign, sign_len, enc + 10, enc_len - 10);
	enc[enc_len] = '&';
	enc_len++;

	bytes_attach(pp, enc, enc_len);
	aliyun_encode(pp);

	bytes_insert(pbuf, 0, *pp, bytes_size(pp));

	bytes_free(pp);

	bytes_free(pbuf);
}

//验证码地址	dysmsapi.aliyuncs.com
//回执地址	dybaseapi.aliyuncs.com

//短信账户 fcv - user@1596062550886330.onaliyun.com
//AccessKey ID LTAI4Fwa4AL6JHMD338PomPF
//AccessKeySecret MMXrzSPCNHx89K91MFEdcjFor4jOXj
//TemplateCode SMS_176913456
//TemplateParam  _T("{\"code\":\"123456\"}
//SignName 居家健康照护

//<?xml version='1.0' encoding='UTF-8'?><SendSmsResponse><Message>OK</Message><RequestId>6BAB9EEC-6CCF-4DF4-91BC-E874B6FB5280</RequestId><BizId>392807084177959405^0</BizId><Code>OK</Code></SendSmsResponse>

#define URL_ALIYUN_SMS		_T("http://dysmsapi.aliyuncs.com")

void test_sms()
{
	lword_t ts = get_timestamp();

	nuid_t ui = { 0 };
	nuid_from_timestamp(&ui, ts);

	tchar_t uuid[50] = { 0 };
	nuid_format_string(&ui, uuid, 50);

	xdate_t dt = { 0 };
	utc_date_from_timestamp(&dt, ts);

	tchar_t stamp[50] = { 0 };
	format_utctime(&dt, stamp);

	get_utc_date(&dt);

	LINKPTR st = create_string_table(1);

	write_string_entity(st, _T("SignatureMethod"), -1, _T("HMAC-SHA1"), -1);
	write_string_entity(st, _T("SignatureNonce"), -1, uuid, -1);
	write_string_entity(st, _T("AccessKeyId"), -1, _T("LTAI4Fwa4AL6JHMD338PomPF"), -1);
	write_string_entity(st, _T("SignatureVersion"), -1, _T("1.0"), -1);
	write_string_entity(st, _T("Timestamp"), -1, stamp, -1);
	write_string_entity(st, _T("Format"), -1, _T("XML"), -1);

	write_string_entity(st, _T("Action"), -1, _T("SendSms"), -1);
	write_string_entity(st, _T("Version"), -1, _T("2017-05-25"), -1);
	write_string_entity(st, _T("RegionId"), -1, _T("cn-hangzhou"), -1);
	write_string_entity(st, _T("PhoneNumbers"), -1, _T("13588368696"), -1);
	write_string_entity(st, _T("SignName"), -1, _T("居家健康照护"), -1);
	write_string_entity(st, _T("TemplateParam"), -1, _T("{\"code\":\"123456\"}"), -1);
	write_string_entity(st, _T("TemplateCode"), -1, _T("SMS_176913456"), -1);
	//write_string_entity(st, _T("OutId"), -1, _T(""), -1);

	int len = string_table_format_options(st, NULL, MAX_LONG, _T('='), _T('&'));
	tchar_t* buf = xsalloc(len + 1);
	string_table_format_options(st, buf, len, _T('='), _T('&'));
	if (len)
	{
		len--;
		buf[len] = _T('\0');
	}

	destroy_string_table(st);

	byte_t** pbuf = bytes_alloc();
	byte_t* enc;
	dword_t enc_len;

	enc_len = xhttp_url_encoding(buf, len, NULL, MAX_LONG);
	enc = bytes_realloc(pbuf, enc_len);
	xhttp_url_encoding(buf, len, enc, enc_len);

	xmem_free(buf);

	aliyun_encode(pbuf);

	//hdr_len = 8; //"GET&%2F&"

	enc_len = 8 + url_byte_encode(*pbuf, bytes_size(pbuf), NULL, MAX_LONG);
	enc = (byte_t*)xmem_alloc(enc_len + 1);
	xmem_copy((void*)enc, (void*)"GET&%2F&", 8);
	url_byte_encode(*pbuf, bytes_size(pbuf), enc + 8, enc_len - 8);

	byte_t** pp = bytes_alloc();
	bytes_attach(pp, enc, enc_len);
	aliyun_encode(pp);

	byte_t key[] = "MMXrzSPCNHx89K91MFEdcjFor4jOXj&";
	dword_t key_len = a_xslen((schar_t*)key);
	byte_t hmac[20] = { 0 };
	sha1_hmac(key, key_len, *pp, bytes_size(pp), hmac);

	byte_t sign[RES_LEN] = { 0 };
	dword_t sign_len;
	sign_len = a_xbas_encode(hmac, 20, (schar_t*)sign, RES_LEN);

	//Signature=
	enc_len = 10 + url_byte_encode(sign, sign_len, NULL, MAX_LONG);
	enc = (byte_t*)xmem_alloc(enc_len + 2); //&
	xmem_copy((void*)enc, (void*)"Signature=", 10);
	url_byte_encode(sign, sign_len, enc + 10, enc_len - 10);
	enc[enc_len] = '&';
	enc_len++;

	bytes_attach(pp, enc, enc_len);
	aliyun_encode(pp);

	bytes_insert(pbuf, 0, *pp, bytes_size(pp));

	bytes_free(pp);

	xhand_t xh = xhttp_client(_T("GET"), URL_ALIYUN_SMS);

	xhttp_set_request_default_header(xh);
	xhttp_set_encoded_query(xh, *pbuf, bytes_size(pbuf));

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	bool_t rt = xhttp_get_response_state(xh);

	dword_t nlen = xhttp_get_response_content_length(xh);

	byte_t *pb = bytes_realloc(pbuf, nlen);

	if (!xhttp_recv(xh, *pbuf, &nlen))
	{
		raise_user_error(NULL, NULL);
	}

	tchar_t type[RES_LEN] = { 0 };
	xhttp_get_response_content_type_charset(xh, type, INT_LEN);

	xhttp_close(xh);

	LINKPTR ptr_xml = create_xml_doc();

	parse_xml_doc_from_bytes(ptr_xml, *pbuf, nlen);
	
	bytes_free(pbuf);
	pbuf = NULL;

	tchar_t message[INT_LEN] = { 0 };
	tchar_t request[ERR_LEN] = { 0 };

	LINKPTR dom = get_xml_dom_node(ptr_xml);
	LINKPTR nlk = get_dom_first_child_node(dom);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Message"), -1, 1) == 0)
			get_dom_node_text(nlk, message, INT_LEN);
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RequestId"), -1, 1) == 0)
			get_dom_node_text(nlk, request, ERR_LEN);

		nlk = get_dom_next_sibling_node(nlk);
	}

	destroy_xml_doc(ptr_xml);
	ptr_xml = NULL;
}



int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//test_bytes();

	//test_sms();


	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif
	return 0;
}

