
#include <xdl.h>
#include <xds.h>
#include <mqtt.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

//#define ADDR_SUB		_T("47.97.167.225")
#define ADDR_SUB		_T("172.16.190.190")
//#define ADDR_SUB		_T("127.0.0.1")
#define PORT_SUB		8880

//#define ADDR_PUB		_T("47.97.167.225")
#define ADDR_PUB		_T("172.16.190.190")
//#define ADDR_PUB		_T("127.0.0.1")
#define PORT_PUB		8881

void test_mqtt_pub()
{
	xhand_t tcp = xtcp_cli(PORT_PUB, ADDR_PUB);

	mqtt_t* mqtt = mqtt_scu(tcp, _MQTT_TYPE_SCU_PUB);

	mqtt_connect(mqtt);

	schar_t msg[NUM_LEN] = { 0 };
	int len;

	int n = 100;
	for (int i = 0; i < n; i++)
	{
		mqtt->packet_qos = i % 3;
		mqtt->packet_pid = i + 1;
		mqtt_publish(mqtt, _T("test"), -1);

		len = a_xsprintf(msg, "msg%d", i);

		if (!mqtt_push_message(mqtt, (byte_t*)msg, len))
			break;

		printf("%s\n", msg);
	}

	mqtt_close(mqtt);

	xtcp_close(tcp);
}

void test_mqtt_sub()
{
	xhand_t tcp = xtcp_cli(PORT_SUB, ADDR_SUB);

	mqtt_t* mqtt = mqtt_scu(tcp, _MQTT_TYPE_SCU_SUB);

	mqtt_connect(mqtt);

	tchar_t* msg;
	int len;

	byte_t* buf = NULL;
	dword_t n;

	mqtt_subcribe(mqtt, _T("test"), -1);

	while (mqtt_status(mqtt) != _MQTT_STATUS_RELEASE)
	{
		if (!mqtt_poll_message(mqtt, &buf, &n))
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

	mqtt_close(mqtt);

	xtcp_close(tcp);
}

void test_mqtt_unsub()
{
	xhand_t tcp = xtcp_cli(PORT_SUB, ADDR_SUB);

	mqtt_t* mqtt = mqtt_scu(tcp, _MQTT_TYPE_SCU_SUB);

	mqtt_connect(mqtt);

	tchar_t* msg;
	int len;

	byte_t* buf = NULL;
	dword_t n;

	mqtt_unsubcribe(mqtt, _T("test"), -1);

	mqtt_close(mqtt);

	xtcp_close(tcp);
}


int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	//test_mqtt_pub();

	test_mqtt_sub();

	//test_mqtt_unsub();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

