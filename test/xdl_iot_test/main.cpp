
#include <xdl.h>
#include <xds.h>
#include <sms.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

#define SECRET_ID		_T("2a2f69763c897300efe63c0c5c08d1c7")
#define SECRET_KEY		_T("902a3a42fce117906909b889b90ebae6")

//#define SMS_URL		_T("https://118.178.180.81")
//#define SMS_URL		_T("http://127.0.0.1:8889")
#define SMS_URL		_T("http://www.biofolia.cn:8889")

#ifdef _OS_WINDOWS
#define SMS_MODULE		_T("sms_aliyun.dll")
#endif

#ifdef _OS_LINUX
#define SMS_MODULE		_T("libsms_aliyun.so")
#endif

#ifdef _OS_MACOS
#define SMS_MODULE		_T("sms_aliyun.dylib")
#endif

#define prod_iot_appId	_T("TrEMLE5k2UQAIhIiROHXPkICyt4a")
#define prod_iot_secret _T("oTKvBHx2HAIc3KZ9BrdEV7YOwCIa")
#define prod_iot_loginUrl _T("https://device.api.ct10649.com:8743/iocm/app/sec/v1.1.0/login")
#define prod_iot_postDataUrl _T("https://device.api.ct10649.com:8743/iocm/app/cmd/v1.4.0/deviceCommands")

#define URL_SESSION			_T("http://127.0.0.1:8889/iot/telecom/fcv.isp?action=session_key")
#define URL_COMMAND			_T("http://127.0.0.1:8889/iot/telecom/fcv.isp?action=command")

void test_baidu()
{
	xhand_t xhttp = xhttp_client(_T("GET"), _T("https://baidu.com"));

	if (!(xhttp))
	{
		printf("connect failed\n");
		return;
	}

	xhttp_set_request_default_header(xhttp);
	xhttp_set_request_header(xhttp, _T("Content-Type"), -1, _T("text/html"), -1);

	//send response
	if (!xhttp_send_request(xhttp))
	{
		printf("send request failed\n");
		xhttp_close(xhttp);
		return;
	}

	//recv request
	if (!xhttp_recv_response(xhttp))
	{
		printf("recv response failed\n");
		xhttp_close(xhttp);
		return;
	}

	tchar_t szBytes[NUM_LEN + 1] = { 0 };
	tchar_t szCode[INT_LEN + 1] = { 0 };
	xhttp_get_response_header(xhttp, _T("Content-Length"), -1, szBytes, NUM_LEN);
	xhttp_get_response_code(xhttp, szCode);

	dword_t len = xstol(szBytes);
	if (!len)
		len = 4 * 1024 * 1024;

	byte_t* buf = (byte_t*)xmem_alloc(len);

	xhttp_recv(xhttp, (byte_t*)buf, &len);

	printf("%s", (char*)buf);

	xhttp_close(xhttp);

	xmem_free(buf);
}

void set_ssl(xhand_t ssl)
{
	file_t xf;
	byte_t buf_crt[X509_CERT_SIZE] = { 0 };
	byte_t buf_rsa[RSA_KEY_SIZE] = { 0 };
	dword_t dw_crt, dw_rsa;

	xf = xfile_open(NULL, _T("./server.crt"), 0);
	if (xf)
	{
		dw_crt = X509_CERT_SIZE;
		xfile_read(xf, buf_crt, dw_crt);
		dw_crt = a_xslen((schar_t*)buf_crt);

		xfile_close(xf);
	}

	xf = xfile_open(NULL, _T("./server2.key"), 0);
	if (xf)
	{
		dw_rsa = RSA_KEY_SIZE;
		xfile_read(xf, buf_rsa, dw_rsa);
		dw_rsa = a_xslen((schar_t*)buf_rsa);

		xfile_close(xf);
	}

	xssl_set_cert(ssl, buf_crt, dw_crt, buf_rsa, dw_rsa, _T(""), -1);
}

void test_access()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_POST, prod_iot_loginUrl);

	set_ssl(xhttp_bio(xh));
	xssl_set_version(xhttp_bio(xh), TLSv11);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);
	xhttp_set_request_accept_charset(xh, CHARSET_UTF8, -1);

	xhttp_set_request_content_type(xh, _T("application/x-www-form-urlencoded"), -1);
	xhttp_set_request_content_type_charset(xh, CHARSET_UTF8, -1);

	xhttp_set_request_header(xh, _T("appId"), -1, prod_iot_appId, -1);
	xhttp_set_request_header(xh, _T("secret"), -1, prod_iot_secret, -1);

	xhttp_set_request_header(xh, _T("User-Agent"), -1, _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)"), -1);

	string_t vs = string_alloc();

	string_printf(vs, _T("%s=%s&%s=%s"), _T("appId"), prod_iot_appId, _T("secret"), prod_iot_secret);

	xhttp_send_string(xh, vs);

	string_free(vs);

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
/*
accessToken : dbf5a9caf2f51cde29b51278c19b26
tokenType : bearer
refreshToken : a026f6f0ad4834ec65b42b3bd1f545da
expiresIn : 3600
scope : default
*/

void test_send()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_POST, prod_iot_postDataUrl);

	set_ssl(xhttp_bio(xh));
	xssl_set_version(xhttp_bio(xh), TLSv11);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);
	xhttp_set_request_accept_charset(xh, CHARSET_UTF8, -1);

	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_request_header(xh, _T("app_key"), -1, prod_iot_appId, -1);
	xhttp_set_request_header(xh, _T("Authorization"), -1, _T("dbf5a9caf2f51cde29b51278c19b26"), -1);

	xhttp_set_request_header(xh, _T("User-Agent"), -1, _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)"), -1);

	char data[] = "{ \
		\"deviceId\" : \"a2664352-9505-45a5-9913-ae01a2660566\", \
		\"expireTime\" : 0,\
		\"command\" : { \
		 	\"serviceId\" : \"ServiceAPI\", \
			\"method\" : \"command\", \
			\"paras\" : { \
				\"payload\" : \"55aa00ff\" \
			} \
		} \
	}";

	dword_t dw = a_xslen(data);

	xhttp_send_full(xh, (byte_t*)data, dw);

	byte_t** pp = bytes_alloc();
	
	dw = 0;
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

//// 电信平台下发的数据格式
// $body='{"notifyType":"deviceDataChanged","deviceId":"a2664352-9505-45a5-9913-ae01a2660566","gatewayId":"a2664352-9505-45a5-9913-ae01a2660566","requestId":null,"service":{"serviceId":"ServiceAPI","serviceType":"ServiceAPI","data":{"payload":"d1086370303300050110000000000000000000000000000000ff92e9"},"eventTime":"20180806T093436Z"}}';

tchar_t session_key[KEY_LEN] = { 0 };

void test_iot_session_key()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_GET, URL_SESSION);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	xhttp_set_url_query_entity(xh, _T("action"), -1, _T("session_key"), -1);

	xhttp_send_request(xh);

	byte_t** pp = bytes_alloc();
	dword_t dw = 0;

	xhttp_recv_full(xh, pp, &dw);

	link_t_ptr ptr_json = create_json_doc();

	parse_json_doc_from_bytes(ptr_json, *pp, dw, _UTF8);

	link_t_ptr nlk_row = get_json_first_child_item(ptr_json);
	while (nlk_row)
	{
		LINKPTR nlk = get_json_first_child_item(nlk_row);
		while (nlk)
		{
			if (compare_text(get_json_item_name_ptr(nlk), -1, _T("session_key"), -1, 1) == 0)
				xscpy(session_key, get_dom_node_text_ptr(nlk));
			nlk = get_json_next_sibling_item(nlk);
		}

		nlk_row = get_json_next_sibling_item(nlk_row);
	}

	destroy_json_doc(ptr_json);

	bytes_free(pp);

	xhttp_close(xh);
}

//{"row" : {"session_key" : "1e68c5e526836452152f8ff06b93cf6"}}

void test_iot_command()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_GET, URL_COMMAND);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	xhttp_set_url_query_entity(xh, _T("action"), -1, _T("command"), -1);

	link_t_ptr ptr_json = create_json_doc();

	LINKPTR nlk_row;

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("device_id"), -1);
	set_dom_node_text(nlk_row, _T("a2664352-9505-45a5-9913-ae01a2660566"), -1);

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("device_data"), -1);
	set_dom_node_text(nlk_row, _T("55aa00ff"), -1);

	xhttp_send_json(xh, ptr_json);

	destroy_json_doc(ptr_json);

	byte_t** pp = bytes_alloc();
	dword_t dw = 0;

	xhttp_recv_full(xh, pp, &dw);

	bytes_free(pp);

	xhttp_close(xh);
}

//#define URL_BP		_T("http://yundatatest.bioland.com.cn:8080/hdwdata/receive/api/data?data=5A25221060115A000253109076070141012120007C4_460027966329481_FFFFFFFFFFFFFFFFFFFFFFFFF")
#define URL_BP		_T("http://118.178.180.81/BP/bioland?data=5A26001010220D012560000000417141201120407B2_460047080504708_863082038053371FFFFFFFFFF")

void test_bp()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_GET, URL_BP);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	xhttp_set_request_accept_charset(xh, CHARSET_UTF8, -1);

	xhttp_set_request_header(xh, _T("User-Agent"), -1, _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)"), -1);

	xhttp_send_request(xh);

	byte_t** pp = bytes_alloc();
	dword_t dw = 0;

	xhttp_recv_full(xh, pp, &dw);

	printf("%s\n", (schar_t*)*pp);

	bytes_free(pp);

	xhttp_close(xh);
}


#define URL_DRUG			_T("http://118.178.180.81/drug/msense")

void test_drug_push()
{
	XHANDLE xh = xhttp_client(HTTP_METHOD_POST, URL_DRUG);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_content_type(xh, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, SECRET_KEY, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, SECRET_ID, sz_hmac);
	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	xhttp_set_url_query_entity(xh, _T("action"), -1, _T("command"), -1);

	link_t_ptr ptr_json = create_json_doc();

	// $body='{"notifyType":"deviceDataChanged","deviceId":"a2664352-9505-45a5-9913-ae01a2660566","gatewayId":"a2664352-9505-45a5-9913-ae01a2660566","requestId":null,"service":{"serviceId":"ServiceAPI","serviceType":"ServiceAPI","data":{"payload":"d1086370303300050110000000000000000000000000000000ff92e9"},"eventTime":"20180806T093436Z"}}';	


	LINKPTR nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("notifyType"), -1);
	set_dom_node_text(nlk_row, _T("deviceDataChanged"), -1);

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("deviceId"), -1);
	set_dom_node_text(nlk_row, _T("a2664352-9505-45a5-9913-ae01a2660566"), -1);

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("gatewayId"), -1);
	set_dom_node_text(nlk_row, _T("a2664352-9505-45a5-9913-ae01a2660566"), -1);

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("requestId"), -1);
	set_dom_node_text(nlk_row, _T(""), -1);

	nlk_row = insert_json_item(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("service"), -1);
	
	LINKPTR nlk = insert_json_item(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("serviceId"), -1);
	set_dom_node_text(nlk, _T("ServiceAPI"), -1);

	nlk = insert_json_item(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("data"), -1);
	
	LINKPTR nlk_child = insert_json_item(nlk, LINK_LAST);
	set_dom_node_name(nlk_child, _T("payload"), -1);
	set_dom_node_text(nlk_child, _T("d1086370303300050110000000000000000000000000000000ff92e9"), -1);

	nlk = insert_json_item(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("eventTime"), -1);
	set_dom_node_text(nlk, _T("20180806T093436Z"), -1);

	xhttp_send_json(xh, ptr_json);

	destroy_json_doc(ptr_json);

	byte_t** pp = bytes_alloc();
	dword_t dw = 0;

	xhttp_recv_full(xh, pp, &dw);

	printf("%s\n", (schar_t*)*pp);

	bytes_free(pp);

	xhttp_close(xh);
}

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);
	
	//test_baidu();

	//test_access();

	//test_send();

	//test_iot_session_key();

	//test_iot_command();

	test_drug_push();

	//test_bp();

	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif
	return 0;
}

