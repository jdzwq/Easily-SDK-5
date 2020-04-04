
#include <xdl.h>
#include <xds.h>
#include <rad.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

//#define ADDR		_T("172.16.190.190")
//#define ADDR		_T("127.0.0.1")
//#define ADDR		_T("118.178.180.81")
#define ADDR		_T("49.234.135.113")
#define PORT		8880

void test_radnet()
{
	xhand_t tcp = xtcp_cli(PORT, ADDR);

	radnet_t* radnet = radnet_scu(tcp);

	variant_t var = { 0 };
	var.vv = VV_LONG;

	object_t val = object_alloc(DEF_MBS);

	Srand48(time(NULL));

	radnet_connect(radnet, _T("mydb"));

	int n = 10;
	for (int i = 0; i < n;i++)
	{
		var.long_one = (Lrand48() % n);

		object_set_variant(val, var);

		if(radnet_set(radnet, var, val))
			printf("SET %d succeed\n", var.long_one);
		else
			printf("SET %d failed\n", var.long_one);

		object_empty(val);

		if(radnet_get(radnet, var, val))
			printf("GET %d succeed\n", var.long_one);
		else
			printf("GET %d failed\n", var.long_one);

		if (radnet_del(radnet, var))
			printf("DEL %d succeed\n", var.long_one);
		else
			printf("DEL %d failed\n", var.long_one);
	}

	variant_to_null(&var);
	object_free(val);

	radnet_close(radnet);

	xtcp_close(tcp);
}

int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	test_radnet();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

