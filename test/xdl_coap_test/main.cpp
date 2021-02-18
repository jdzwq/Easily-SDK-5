
#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

#define URL_GET			_T("coap://127.0.0.1:5683/test/device/person")
//#define URL_GET			_T("coap://118.178.180.81:5683/test/device/person")
//#define URL_GET			_T("coap://49.234.135.113:5683/test/device/person")
//#define URL_GET			_T("coap://172.16.190.190:5683/test/device/person")
//#define URL_GET			_T("coap://172.16.190.200:5683/test/device/person")

void test_coap_put()
{
	xhand_t coap = xcoap_client(_T("PUT"), URL_GET);

	xcoap_set_blockwise(coap, 4);
	
	xcoap_connect(coap);

	byte_t data[2050] = { 0 };
	dword_t dw = 2050;

	if (xcoap_send(coap, data, &dw))
	{
		xcoap_flush(coap);
	}

	xcoap_close(coap);
}

void test_coap_get()
{
	xhand_t coap = xcoap_client(_T("GET"), URL_GET);

	xcoap_set_blockwise(coap, 4);

	xcoap_connect(coap);

	byte_t data[4096] = { 0 };
	dword_t dw = 4096;

	xcoap_recv(coap, data, &dw);
	
	xcoap_close(coap);
}

void test_coap_del()
{
	xhand_t coap = xcoap_client(_T("DELETE"), URL_GET);

	xcoap_connect(coap);

	xcoap_close(coap);
}

int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	//test_coap_put();

	//test_coap_get();

	test_coap_del();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS

	getch();
#endif

	return 0;
}

