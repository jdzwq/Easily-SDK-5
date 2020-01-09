// xdl_oauth_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define AS_URL_AUTH_REQUEST		_T("http://172.16.190.190:8889/oau/auth_request?")
#define AS_URL_AUTH_ACCESS		_T("http://172.16.190.190:8889/oau/auth_access?")
#define AS_URL_AUTH_REFRESH		_T("http://127.0.0.1:8889/oau/auth_refresh?")

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

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//_test_implicit();

	_test_explicit();

	//_test_refresh();

	xdl_process_uninit();

	getch();

	return 0;
}

