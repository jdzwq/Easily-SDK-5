
#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

//#define ADDR_PUB		_T("47.97.167.225")
//#define ADDR_PUB		_T("172.16.190.190")
//#define ADDR_PUB		_T("172.16.220.133")
//#define ADDR_PUB		_T("127.0.0.1")
#define ADDR_PUB		_T("118.178.180.81")
//#define ADDR_PUB		_T("49.234.135.113")
#define PORT_PUB		1833

//#define ADDR_SUB		_T("47.97.167.225")
//#define ADDR_SUB		_T("172.16.190.190")
//#define ADDR_SUB		_T("172.16.220.133")
//#define ADDR_SUB		_T("127.0.0.1")
//#define ADDR_SUB		_T("118.178.180.81")
#define ADDR_SUB		_T("49.234.135.113")
#define PORT_SUB		1833

void test_mqtt_pub()
{
	xhand_t tcp = xtcp_cli(PORT_PUB, ADDR_PUB);

	xhand_t mqtt = xmqtt_scu(tcp, _MQTT_TYPE_SCU_PUB);

	xmqtt_connect(mqtt);

	schar_t msg[NUM_LEN] = { 0 };
	int len;

	MQTT_PACKET_CTRL mc = { 0 };

	int n = 10;
	for (int i = 0; i < n; i++)
	{
		mc.packet_qos = i % 3;
		mc.packet_pid = i + 1;

		xmqtt_publish(mqtt, _T("test/device/person"), -1);

		len = a_xsprintf(msg, "msg%d", i);

		xmqtt_set_packet_ctrl(mqtt, &mc);

		if (!xmqtt_push_message(mqtt, (byte_t*)msg, len))
			break;

		printf("%s\n", msg);
	}

	xmqtt_close(mqtt);

	xtcp_close(tcp);
}

void test_mqtt_sub()
{
	xhand_t tcp = xtcp_cli(PORT_SUB, ADDR_SUB);

	xhand_t mqtt = xmqtt_scu(tcp, _MQTT_TYPE_SCU_SUB);

	xmqtt_connect(mqtt);

	tchar_t* msg;
	int len;

	byte_t* buf = NULL;
	dword_t n;

	//08:7c:be:58:37:f8-Gateway-NAME
	xmqtt_subcribe(mqtt, _T("test/device/person"), -1);

	while (xmqtt_status(mqtt) != _MQTT_STATUS_RELEASE)
	{
		if (!xmqtt_poll_message(mqtt, &buf, &n))
			break;

#ifdef _UNICODE
		len = utf8_to_ucs(buf, n, NULL, MAX_LONG);
#else
		len = utf8_to_mbs(buf, n, NULL, MAX_LONG);
#endif

		msg = xsalloc(len + 1);

#ifdef _UNICODE
		len = utf8_to_ucs(buf, n, msg, len);
#else
		len = utf8_to_mbs(buf, n, msg, len);
#endif

#ifdef _UNICODE
		wprintf(L"%s\n", msg);
#else
		printf("%s\n", msg);
#endif
		xsfree(msg);
	}

	xmqtt_close(mqtt);

	xtcp_close(tcp);
}

void test_mqtt_unsub()
{
	xhand_t tcp = xtcp_cli(PORT_SUB, ADDR_SUB);

	xhand_t mqtt = xmqtt_scu(tcp, _MQTT_TYPE_SCU_SUB);

	xmqtt_connect(mqtt);

	tchar_t* msg;
	int len;

	byte_t* buf = NULL;
	dword_t n;

	xmqtt_unsubcribe(mqtt, _T("test/device/person"), -1);

	xmqtt_close(mqtt);

	xtcp_close(tcp);
}


int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	test_mqtt_pub();

	//test_mqtt_sub();

	//test_mqtt_unsub();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS

	getch();
#endif

	return 0;
}

