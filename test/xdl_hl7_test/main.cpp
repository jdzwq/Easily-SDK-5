
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

#define SECRET_ID		_T("2a2f69763c897300efe63c0c5c08d1c7")
#define SECRET_KEY		_T("902a3a42fce117906909b889b90ebae6")

//#define HL7_URL		_T("http://172.16.190.190:8889")
#define HL7_URL		_T("http://127.0.0.1")
//#define HL7_URL		_T("http://118.178.180.81")
//#define HL7_URL		_T("https://www.biofolia.cn:8888")

void test_hl7_head()
{
	tchar_t errcode[NUM_LEN + 1] = {0};
	tchar_t errtext[ERR_LEN + 1] = {0};

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	TRY_CATCH;

	tchar_t url[1024] = { 0 };

	xsprintf(url, _T("%s/hl7/test/device/person"), HL7_URL);

	xhand_t xh = xhttp_client(_T("HEAD"), url);

	xhttp_set_request_default_header(xh);

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh))
	{
		raise_user_error(NULL, NULL);
	}

	tchar_t fsize[NUM_LEN] = {0};
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);

	_tprintf(_T("%s\n"),fsize);

	tchar_t ftime[DATE_LEN] = {0};
	xhttp_get_response_header(xh, HTTP_HEADER_LASTMODIFIED, -1, ftime, DATE_LEN);

	_tprintf(_T("%s\n"),ftime);
	
	xhttp_close(xh);

	END_CATCH;
ONERROR:
	get_last_error(errcode, errtext, ERR_LEN);
	
	_tprintf(_T("%s\n"),errtext);
}

void test_hl7_put()
{
	tchar_t errcode[NUM_LEN + 1] = {0};
	tchar_t errtext[ERR_LEN + 1] = {0};

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	TRY_CATCH;

	tchar_t url[1024] = { 0 };

	xsprintf(url, _T("%s/hl7/test/device/person"), HL7_URL);

	xhand_t xh = xhttp_client(_T("PUT"), url);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	LINKPTR ptr_xml = create_xml_doc();
	LINKPTR nlk_rowset = get_xml_dom_node(ptr_xml);
	set_dom_node_name(nlk_rowset, _T("MDED"), -1);

	if (!xhttp_send_xml(xh, ptr_xml))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_xml_doc(ptr_xml);

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh))
	{
		raise_user_error(NULL, NULL);
	}

	_tprintf(_T("%s\n"),_T("PUT Succeed"));
	
	xhttp_close(xh);

	END_CATCH;
ONERROR:
	get_last_error(errcode, errtext, ERR_LEN);
	
	_tprintf(_T("%s\n"),errtext);
}

void test_hl7_list()
{
	tchar_t errcode[NUM_LEN + 1] = {0};
	tchar_t errtext[ERR_LEN + 1] = {0};

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	TRY_CATCH;

	tchar_t url[1024] = { 0 };

	xsprintf(url, _T("%s/hl7/test/device/person"), HL7_URL);

	xhand_t xh = xhttp_client(_T("LIST"), url);

	xhttp_set_request_default_header(xh);
	//xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	dword_t n = xhttp_get_response_content_length(xh);

	byte_t** pb = bytes_alloc();

	xhttp_recv_full(xh, pb, &n);

	printf("%s\n",(char*)*pb);

	bytes_free(pb);
	
	xhttp_close(xh);

	END_CATCH;
ONERROR:
	get_last_error(errcode, errtext, ERR_LEN);
	
	_tprintf(_T("%s\n"),errtext);
}

void test_hl7_get()
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	TRY_CATCH;

	tchar_t url[1024] = { 0 };

	xsprintf(url, _T("%s/hl7/test/device/person"), HL7_URL);

	xhand_t xh = xhttp_client(_T("GET"), url);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	//xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	xdate_t dt;
	tchar_t ftime[DATE_LEN] = { 0 };
	
	get_utc_date(&dt);
	format_gmttime(&dt, ftime);
	xhttp_set_request_header(xh, HTTP_HEADER_IFMODIFIEDSINCE, -1, ftime, -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	dword_t n = xhttp_get_response_content_length(xh);

	byte_t** pb = bytes_alloc();

	xhttp_recv_full(xh, pb, &n);

	printf("%s\n", (char*)*pb);

	bytes_free(pb);

	xhttp_close(xh);

	END_CATCH;
ONERROR:
	get_last_error(errcode, errtext, ERR_LEN);

	_tprintf(_T("%s\n"), errtext);
}

void test_hl7_delete()
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	TRY_CATCH;

	tchar_t url[1024] = { 0 };

	xsprintf(url, _T("%s/hl7/test/device/person"), HL7_URL);

	xhand_t xh = xhttp_client(_T("DELETE"), url);

	xhttp_set_request_default_header(xh);

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(xh);

	printf("%s\n", "Deleted");

	END_CATCH;
ONERROR:
	get_last_error(errcode, errtext, ERR_LEN);

	_tprintf(_T("%s\n"), errtext);
}

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	//test_hl7_put();

	//test_hl7_head();

	//test_hl7_list();

	//test_hl7_get();

	//test_hl7_delete();

	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

