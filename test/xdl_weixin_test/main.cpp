
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


int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);



	xdl_process_uninit();

	getch();

	return 0;
}

