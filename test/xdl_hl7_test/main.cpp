
#include <xdl.h>
#include <xds.h>
#include <hl7.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

//#define ADDR_SUB		_T("172.16.190.190")
#define ADDR_SUB		_T("127.0.0.1")
#define PORT_SUB		8883

//#define ADDR_PUB		_T("172.16.190.190")
#define ADDR_PUB		_T("127.0.0.1")
#define PORT_PUB		8884

void test_hl7net_pub()
{
	schar_t msg[NUM_LEN] = { 0 };
	int len;

	hl7net_pdv_head_t pdv = { 0 };

	xhand_t tcp = xtcp_cli(PORT_PUB, ADDR_PUB);

	hl7net_t* hl7net = hl7net_scu(tcp, _HL7NET_TYPE_SCU_PUB);

	hl7net_connect(hl7net);

	hl7net_publish(hl7net, _T("my"), -1);

	stream_t stm = stream_alloc(hl7net_bio(hl7net));

	int i = 2;
	while (i--)
	{
		len = a_xsprintf(msg, "msg%d", i);

		xmem_copy((void*)pdv.type, "MSH", 3);
		pdv.size = len;

		hl7net_push_message(hl7net, &pdv);

		stream_write_bytes(stm, (byte_t*)msg, len);
	}

	stream_free(stm);
	
	hl7net_close(hl7net);

	xtcp_close(tcp);
}

void test_hl7net_sub()
{
	schar_t msg[NUM_LEN] = { 0 };
	int len;

	hl7net_pdv_head_t pdv = { 0 };

	xhand_t tcp = xtcp_cli(PORT_SUB, ADDR_SUB);

	hl7net_t* hl7net = hl7net_scu(tcp, _HL7NET_TYPE_SCU_SUB);

	hl7net_connect(hl7net);

	hl7net_subscribe(hl7net, _T("my"), -1);

	stream_t stm = stream_alloc(hl7net_bio(hl7net));

	int i = 2;
	while (i--)
	{
		hl7net_poll_message(hl7net, &pdv);

		stream_read_bytes(stm, (byte_t*)msg, &pdv.size);
	}

	stream_free(stm);

	hl7net_close(hl7net);

	xtcp_close(tcp);
}

int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	//test_hl7net_pub();

	test_hl7net_sub();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

