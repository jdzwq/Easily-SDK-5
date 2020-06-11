
#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

//#define URL_GET			_T("tftp://127.0.0.1:69/demo.txt")
#define URL_GET			_T("tftp://118.178.180.81:69/demo.txt")
//#define URL_GET			_T("tftp://49.234.135.113:69/demo.txt")
//#define URL_GET			_T("tftp://172.16.190.190:69/demo.txt")
//#define URL_GET			_T("tftp://172.16.220.133:69/demo.txt")

void test_tftp_put()
{
	xhand_t tftp = xtftp_client(_T("PUT"), URL_GET);

	byte_t data[2050] = { 0 };
	dword_t dw = 2050;

	if (xtftp_send(tftp, data, &dw))
	{
		xtftp_flush(tftp);
	}

	xtftp_close(tftp);

}

void test_tftp_get()
{
	xhand_t tftp = xtftp_client(_T("GET"), URL_GET);

	byte_t data[4096] = { 0 };
	dword_t dw = 4096;

	xtftp_recv(tftp, data, &dw);
	
	xtftp_close(tftp);

}

void test_tftp_del()
{
	xhand_t tftp = xtftp_client(_T("DELETE"), URL_GET);

	xtftp_close(tftp);
}

int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	//test_tftp_put();

	test_tftp_get();

	//test_tftp_del();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS

	getch();
#endif

	return 0;
}

