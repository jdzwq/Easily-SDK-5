
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

#define prod_iot_appId	_T("TrEMLE5k2UQAIhIiROHXPkICyt4a")
#define prod_iot_secret _T("oTKvBHx2HAIc3KZ9BrdEV7YOwCIa")
#define prod_iot_loginUrl _T("https://device.api.ct10649.com:8743/iocm/app/sec/v1.1.0/login")
#define prod_iot_postDataUrl _T("https://device.api.ct10649.com:8743/iocm/app/cmd/v1.4.0/deviceCommands")

//#define URL_SESSION			_T("http://127.0.0.1:8889/iot/telecom/msense.isp?action=session_key")
#define URL_SESSION			_T("http://www.biofolia.cn:8889/iot/telecom/msense.isp?action=session_key")

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


//#define URL_BP		_T("http://yundatatest.bioland.com.cn:8080/hdwdata/receive/api/data?data=5A25221060115A000253109076070141012120007C4_460027966329481_FFFFFFFFFFFFFFFFFFFFFFFFF")
//#define URL_BP		_T("http://118.178.180.81/BP/bioland?data=5A26001010220D012560000000417141201120407B2_460047080504708_863082038053371FFFFFFFFFF")
#define URL_BP		_T("http://www.biofolia.cn:8889/bp/bioland?data=5A26001010220D012560000000417141201120407B2_460047080504708_863082038053371FFFFFFFFFF")

void test_bp_code()
{
	/*
	服务平台返回数据示例：					
	服务器 IP: 42.121.236.135 (0x2A:0x79:0xEC:0x87).					
	服务器端口：8014 （0x1F4E）					
	服务器时间：14年7月8日10时39分					
	服务平台返回31字节数据：+IP2A79EC874E1F690E07080A272COK
	*/
	//Sum1:   IP，端口按字节HEX异或.如上Sum1＝2A ^ 79 ^ EC ^ 87 ^ 4E ^ 1F＝69H
	//Sum2：时间数据按字节HEX异或，如上Sum2＝ 0E ^ 07 ^ 08 ^ 0A ^ 27＝2CH

	tchar_t addr[] = _T("42.121.236.135");
	dword_t port = 8014;

	xdate_t dt = { 0 };
	dt.year = 2014;
	dt.mon = 7;
	dt.day = 8;
	dt.hour = 10;
	dt.min = 39;

	int len = xslen(addr);
	
	tchar_t* token;
	byte_t buf[32] = { 0 };
	dword_t n;
	byte_t sum;

	for (n = 0; n < len; n++)
	{
		if (*(addr + n) == _T('.'))
			*(addr + n) = _T('\0');
	}

	buf[0] = '+';
	buf[1] = 'I';
	buf[2] = 'P';
	n = 3;
	len = 3;

	sum = 0;

	token = addr;
	a_xsprintf((schar_t*)(buf + n), ("%02X"), xstol(token));
	n += 2;
	sum = (sum ^ ((byte_t)xstol(token)));

	token += (xslen(token) + 1);
	a_xsprintf((schar_t*)(buf + n), ("%02X"), xstol(token));
	n += 2;
	sum = (sum ^ ((byte_t)xstol(token)));

	token += (xslen(token) + 1);
	a_xsprintf((schar_t*)(buf + n), ("%02X"), xstol(token));
	n += 2;
	sum = (sum ^ ((byte_t)xstol(token)));

	token += (xslen(token) + 1);
	a_xsprintf((schar_t*)(buf + n), ("%02X"), xstol(token));
	n += 2;
	sum = (sum ^ ((byte_t)xstol(token)));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), (port & 0x00FF));
	n += 2;
	sum = (sum ^ (port & 0x00FF));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), (port >> 8));
	n += 2;
	sum = (sum ^ (port >> 8));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), sum);
	n += 2;
	len += 2;

	sum = 0;

	a_xsprintf((schar_t*)(buf + n), ("%02X"), (dt.year - 2000));
	n += 2;
	sum = (sum ^ ((byte)(dt.year - 2000)));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), dt.mon);
	n += 2;
	sum = (sum ^ ((byte)(dt.mon)));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), dt.day);
	n += 2;
	sum = (sum ^ ((byte)(dt.day)));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), dt.hour);
	n += 2;
	sum = (sum ^ ((byte)(dt.hour)));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), dt.min);
	n += 2;
	sum = (sum ^ ((byte)(dt.min)));

	a_xsprintf((schar_t*)(buf + n), ("%02X"), sum);
	n += 2;

	buf[n] = 'O';
	buf[n + 1] = 'K';
	n += 2;
}

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

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);
	
	//test_baidu();

	//test_access();

	//test_send();

	test_bp();

	//test_bp_code();

	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif
	return 0;
}

