// xdl_ssl_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#define URL		_T("shttp://localhost:8889/loc/mymovi.mp4")
//#define URL		_T("http://localhost:8889/loc/mymovi.mp4")
#define URL		_T("ssh://localhost:8887/loc/mymovi.mp4")
//#define URL			_T("https://myssl.com:443/www.sspanda.com?status=q")
//#define URL			_T("https://www.baidu.com:443")

void set_ssl(xhand_t ssl)
{
	file_t xf;
	byte_t buf_crt[X509_CERT_SIZE] = { 0 };
	byte_t buf_rsa[RSA_KEY_SIZE] = { 0 };
	dword_t dw;

	xf = xfile_open(NULL, _T("sslcli.crt"), 0);
	if (xf)
	{
		dw = X509_CERT_SIZE;
		xfile_read(xf, buf_crt, dw);

		xssl_set_cert(ssl, buf_crt, a_xslen((schar_t*)buf_crt));
		xfile_close(xf);
	}

	xf = xfile_open(NULL, _T("sslcli.key"), 0);
	if (xf)
	{
		dw = RSA_KEY_SIZE;
		xfile_read(xf, buf_rsa, dw);

		xssl_set_rsa(ssl, buf_rsa, a_xslen((schar_t*)buf_rsa), NULL, 0);
		xfile_close(xf);
	}
}

void set_tls(xhand_t tls)
{
	file_t xf;
	byte_t buf_crt[X509_CERT_SIZE] = { 0 };
	byte_t buf_rsa[RSA_KEY_SIZE] = { 0 };
	dword_t dw;

	xf = xfile_open(NULL, _T("sslcli.crt"), 0);
	if (xf)
	{
		dw = X509_CERT_SIZE;
		xfile_read(xf, buf_crt, dw);

		xssl_set_cert(tls, buf_crt, a_xslen((schar_t*)buf_crt));
		xfile_close(xf);
	}

	xf = xfile_open(NULL, _T("sslcli.key"), 0);
	if (xf)
	{
		dw = RSA_KEY_SIZE;
		xfile_read(xf, buf_rsa, dw);

		xssl_set_rsa(tls, buf_rsa, a_xslen((schar_t*)buf_rsa), NULL, 0);
		xfile_close(xf);
	}
}

void test_https_get()
{
	XHANDLE xhttp = xhttp_client(L"GET", URL);

	//set_ssl(xhttp_bio(xhttp));
	//set_tls(xhttp_bio(xhttp));

	xhttp_set_request_default_header(xhttp);
	xhttp_set_request_header(xhttp, L"Content-Type", -1, L"text/html", -1);

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
	xhttp_get_response_header(xhttp, L"Content-Length", -1, szBytes, NUM_LEN);
	xhttp_get_response_code(xhttp, szCode);

	dword_t len = xstol(szBytes);
	if (!len)
		len = 4 * 1024 * 1024;

	byte_t* buf = (byte_t*)xmem_alloc(len);

	xhttp_recv(xhttp, (byte_t*)buf, &len);

	xhttp_close(xhttp);

	if (IS_XHTTP_SUCCEED(szCode))
	{
		file_t fh = xfile_open(NULL, _T("mymovi.mp4"), FILE_OPEN_CREATE);

		xfile_write(fh, buf, len);

		xfile_close(fh);
	}
	else
	{
		LINKPTR ptr_xml = create_xml_doc();

		bool_t rt = parse_xml_doc_from_bytes(ptr_xml, buf, len);

		destroy_xml_doc(ptr_xml);

		printf("%s", (char*)buf);
	}

	xmem_free(buf);
}

void test_ssh_get()
{
	XHANDLE xhttp = xhttp_client(L"GET", URL);

	//set_ssl(xhttp_bio(xhttp));
	//set_tls(xhttp_bio(xhttp));

	xhttp_set_request_default_header(xhttp);
	xhttp_set_request_header(xhttp, L"Content-Type", -1, L"text/html", -1);

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
	xhttp_get_response_header(xhttp, L"Content-Length", -1, szBytes, NUM_LEN);
	xhttp_get_response_code(xhttp, szCode);

	dword_t len = xstol(szBytes);
	if (!len)
		len = 4 * 1024 * 1024;

	byte_t* buf = (byte_t*)xmem_alloc(len);

	xhttp_recv(xhttp, (byte_t*)buf, &len);

	xhttp_close(xhttp);

	if (IS_XHTTP_SUCCEED(szCode))
	{
		file_t fh = xfile_open(NULL, _T("mymovi.mp4"), FILE_OPEN_CREATE);

		xfile_write(fh, buf, len);

		xfile_close(fh);
	}
	else
	{
		LINKPTR ptr_xml = create_xml_doc();

		bool_t rt = parse_xml_doc_from_bytes(ptr_xml, buf, len);

		destroy_xml_doc(ptr_xml);

		printf("%s", (char*)buf);
	}

	xmem_free(buf);
}

int main()
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//test_https_get();
	test_ssh_get();

	xdl_process_uninit();

	getch();

	return 0;
}

