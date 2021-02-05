
#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif


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

void test_decrypt()
{
	tchar_t encryptedData[] = _T("CiyLU1Aw2KjvrjMdj8YKliAjtP4gsMZMQmRzooG2xrDcvSnxIMXFufNstNGTyaGS9uT5geRa0W4oTOb1WT7fJlAC+oNPdbB+3hVbJSRgv+4lGOETKUQz6OYStslQ142dNCuabNPGBzlooOmB231qMM85d2/fV6ChevvXvQP8Hkue1poOFtnEtpyxVLW1zAo6/1Xx1COxFvrc2d7UL/lmHInNlxuacJXwu0fjpXfz/YqYzBIBzD6WUfTIF9GRHpOn/Hz7saL8xz+W//FRAUid1OksQaQx4CMs8LOddcQhULW4ucetDf96JcR3g0gfRK4PC7E/r7Z6xNrXd2UIeorGj5Ef7b1pJAYB6Y5anaHqZ9J6nKEBvB4DnNLIVWSgARns/8wR2SiRS7MNACwTyrGvt9ts8p12PKFdlqYTopNHR1Vf7XjfhQlVsAJdNiKdYmYVoKlaRv85IfVunYzO0IKXsyl7JCUjCpoG20f0a04COwfneQAGGwd5oa+T8yO5hzuyDb/XcxxmK01EpqOyuxINew==");
	tchar_t iv[] = _T("r7BXXKkLb8qrSNn05n0qiA==");
	tchar_t session_key[] = _T("tiihtNczf5v6AKRyjwEUhQ==");

	byte_t skey[16] = { 0 };
	xbas_decode(session_key, -1, skey, 16);

	byte_t ikey[16] = { 0 };
	xbas_decode(iv, -1, ikey, 16);

	byte_t data[4096] = { 0 };
	int n = xbas_decode(encryptedData, -1, data, 4096);

	byte_t buf[4096] = { 0 };

	aes_context aes = { 0 };

	aes_setkey_dec(&aes, skey, 128);
	aes_crypt_cbc(&aes, AES_DECRYPT, n, ikey, data, buf);

	printf("%s\n", (schar_t*)buf);

	tchar_t str[4096] = { 0 };

#if defined(_UNICODE) || defined(UNICODE)
	utf8_to_ucs(buf, n, str, 4096);
#else
	utf8_to_mbs(buf, n, str, 4096);
#endif

	link_t_ptr ptr_json = create_json_doc();

	parse_json_doc_from_bytes(ptr_json, buf, n, _UTF8);

	link_t_ptr nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		_tprintf(_T("%s : %s\n"), get_json_item_name_ptr(nlk), get_json_item_value_ptr(nlk));
		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);
}

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//test_weixin_session();

	test_decrypt();

	xdl_process_uninit();

	getch();

	return 0;
}

