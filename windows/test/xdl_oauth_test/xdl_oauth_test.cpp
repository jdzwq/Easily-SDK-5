// xdl_oauth_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define SECRET_ID		_T("2a2f69763c897300efe63c0c5c08d1c7")
#define SECRET_KEY		_T("902a3a42fce117906909b889b90ebae6")

#define AS_URL_AUTH_REQUEST		_T("http://172.16.190.190:8889/oau/auth_request?")
#define AS_URL_AUTH_ACCESS		_T("http://172.16.190.190:8889/oau/auth_access?")
#define AS_URL_AUTH_REFRESH		_T("http://127.0.0.1:8889/oau/auth_refresh?")

//#define AS_URL_AUTH_WEIAPP		_T("http://127.0.0.1:8889/oau/weiapp/fcv.isp")
//#define AS_URL_AUTH_WEIAPP		_T("https://118.178.180.81/oau/weiapp/fcv.isp")
#define AS_URL_AUTH_WEIAPP		_T("https://www.biofolia.cn:8888/oau/weiapp/fcv.isp")

void _test_implicit()
{
	xhand_t xh = xhttp_client(HTTP_METHOD_GET, AS_URL_AUTH_REQUEST);

	xhttp_set_url_query_entity(xh, OAUTH_RESPONSE_TYPE, -1, OAUTH_RESPONSE_TYPE_TOKEN, -1);
	xhttp_set_url_query_entity(xh, OAUTH_CLIENT_ID, -1, _T("111"), -1);
	xhttp_set_url_query_entity(xh, OAUTH_REDIRECT_URL, -1, _T("http://www.someone.com"), -1);
	xhttp_set_url_query_entity(xh, OAUTH_STATE, -1, _T("123"), -1);

	xhttp_send_request(xh);

	xhttp_recv_response(xh);

	tchar_t szCode[INT_LEN + 1] = { 0 };
	xhttp_get_response_code(xh, szCode);

	tchar_t szBytes[NUM_LEN + 1] = { 0 };
	tchar_t szLocation[PATH_LEN] = { 0 };
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, szBytes, NUM_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_LOCATION, -1, szLocation, PATH_LEN);

	wprintf(_T("%s"), szLocation);

	xhttp_close(xh);
}

void _test_explicit()
{
	xhand_t xh = xhttp_client(HTTP_METHOD_GET, AS_URL_AUTH_REQUEST);

	xhttp_set_url_query_entity(xh, OAUTH_RESPONSE_TYPE, -1, OAUTH_RESPONSE_TYPE_CODE, -1);
	xhttp_set_url_query_entity(xh, OAUTH_CLIENT_ID, -1, _T("111"), -1);
	xhttp_set_url_query_entity(xh, OAUTH_REDIRECT_URL, -1, _T("http://www.someone.com"), -1);
	xhttp_set_url_query_entity(xh, OAUTH_STATE, -1, _T("123"), -1);

	xhttp_send_request(xh);

	xhttp_recv_response(xh);

	tchar_t szCode[INT_LEN + 1] = { 0 };
	xhttp_get_response_code(xh, szCode);

	tchar_t szBytes[NUM_LEN + 1] = { 0 };
	tchar_t szLocation[PATH_LEN] = { 0 };
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, szBytes, NUM_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_LOCATION, -1, szLocation, PATH_LEN);

	wprintf(_T("%s\n"), szLocation);

	const tchar_t* token = szLocation;
	while (*token != _T('?') && *token != _T('\0'))
		token++;

	if (*token == _T('?'))
		token++;

	tchar_t oau_code[NUM_LEN] = { 0 };
	tchar_t oau_state[RES_LEN] = { 0 };

	tchar_t *key, *val;
	int klen, vlen;

	while (token = parse_options_token(token, -1, _T('='), _T('&'), &key, &klen, &val, &vlen))
	{
		if (compare_text(key, klen, OAUTH_CODE, -1, 1) == 0)
		{
			xsncpy(oau_code, val, vlen);
		}
		else if (compare_text(key, klen, OAUTH_STATE, -1, 1) == 0)
		{
			xsncpy(oau_state, val, vlen);
		}
	}

	xhttp_close(xh);

	xh = xhttp_client(HTTP_METHOD_GET, AS_URL_AUTH_ACCESS);

	xhttp_set_url_query_entity(xh, OAUTH_GRANT_TYPE, -1, OAUTH_GRANT_TYPE_AUTH, -1);
	xhttp_set_url_query_entity(xh, OAUTH_CODE, -1, oau_code,  -1);
	xhttp_set_url_query_entity(xh, OAUTH_REDIRECT_URL, -1, _T("http://www.someone.com"), -1);
	xhttp_set_url_query_entity(xh, OAUTH_CLIENT_ID, -1, _T("111"), -1);

	xhttp_send_request(xh);

	link_t_ptr ptr_json = create_json_doc();

	xhttp_recv_json(xh, ptr_json);

	link_t_ptr ilk = get_json_first_child_item(ptr_json);
	while (ilk)
	{
		wprintf(_T("%s : %s\n"), get_json_item_name_ptr(ilk), get_json_item_value_ptr(ilk));
		ilk = get_json_next_sibling_item(ilk);
	}

	destroy_json_doc(ptr_json);

	xhttp_close(xh);
}

void _test_refresh()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_GET, AS_URL_AUTH_REFRESH);

	xhttp_set_url_query_entity(xh, OAUTH_GRANT_TYPE, -1, OAUTH_GRANT_TYPE_AUTH, -1);
	xhttp_set_url_query_entity(xh, OAUTH_REFRESH_TOKEN, -1, _T("1111"), -1);
	xhttp_set_url_query_entity(xh, OAUTH_SCOPE, -1, _T(""), -1);

	xhttp_send_request(xh);

	link_t_ptr ptr_json = create_json_doc();

	xhttp_recv_json(xh, ptr_json);

	link_t_ptr ilk = get_json_first_child_item(ptr_json);
	while (ilk)
	{
		wprintf(_T("%s : %s\n"), get_json_item_name_ptr(ilk), get_json_item_value_ptr(ilk));
		ilk = get_json_next_sibling_item(ilk);
	}

	destroy_json_doc(ptr_json);

	xhttp_close(xh);
}

//https://api.weixin.qq.com/sns/jscode2session?appid=APPID&secret=SECRET&js_code=JSCODE&grant_type=authorization_code
//https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code
void test_weixin_session()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_GET, _T("https://api.weixin.qq.com/sns/jscode2session"));

	xhttp_set_request_default_header(xh);
	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_url_query_entity(xh, _T("appid"), -1, _T("APPID"), -1);
	xhttp_set_url_query_entity(xh, _T("secret"), -1, _T("SECRET"), -1);
	//xhttp_set_url_query_entity(xh, _T("code"), -1, _T("CODE"), -1);
	xhttp_set_url_query_entity(xh, _T("js_code"), -1, _T("CODE"), -1);
	xhttp_set_url_query_entity(xh, _T("grant_type"), -1, _T("authorization_code"), -1);

	xhttp_send_request(xh);

	byte_t** pp = bytes_alloc();
	dword_t dw = 0;

	xhttp_recv_full(xh, pp, &dw);

	link_t_ptr ptr_json = create_json_doc();

	parse_json_doc_from_bytes(ptr_json, *pp, dw, _UTF8);

	link_t_ptr nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		wprintf(_T("%s : %s\n"), get_json_item_name_ptr(nlk), get_json_item_value_ptr(nlk));

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);

	bytes_free(pp);

	xhttp_close(xh);
}

void test_auth_weiapp_session()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_GET, AS_URL_AUTH_WEIAPP);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	xhttp_set_url_query_entity(xh, _T("action"), -1, _T("session"), -1);
	xhttp_set_url_query_entity(xh, _T("jscode"), -1, _T("CODE"), -1);

	xhttp_send_request(xh);

	byte_t** pp = bytes_alloc();
	dword_t dw = 0;

	xhttp_recv_full(xh, pp, &dw);

	link_t_ptr ptr_json = create_json_doc();

	parse_json_doc_from_bytes(ptr_json, *pp, dw, _UTF8);

	link_t_ptr nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		wprintf(_T("%s : %s\n"), get_json_item_name_ptr(nlk), get_json_item_value_ptr(nlk));

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);

	bytes_free(pp);

	xhttp_close(xh);
}

void test_auth_weiapp_access()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_GET, AS_URL_AUTH_WEIAPP);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	xhttp_set_url_query_entity(xh, _T("action"), -1, _T("access"), -1);

	xhttp_send_request(xh);

	byte_t** pp = bytes_alloc();
	dword_t dw = 0;

	xhttp_recv_full(xh, pp, &dw);

	link_t_ptr ptr_json = create_json_doc();

	parse_json_doc_from_bytes(ptr_json, *pp, dw, _UTF8);

	link_t_ptr nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		wprintf(_T("%s : %s\n"), get_json_item_name_ptr(nlk), get_json_item_value_ptr(nlk));

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);

	bytes_free(pp);

	xhttp_close(xh);
}

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//_test_implicit();

	//_test_explicit();

	//_test_refresh();

	//test_weixin_session();

	//test_auth_weiapp_session();

	test_auth_weiapp_access();

	xdl_process_uninit();

	getch();

	return 0;
}

