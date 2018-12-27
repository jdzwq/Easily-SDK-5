
#include <xdl.h>
#include <xds.h>
#include <radhex.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

#define ADDR		_T("172.16.190.190")
//#define ADDR		_T("127.0.0.1")
#define PORT		8882

void test_radhex()
{
	xhand_t tcp = xtcp_cli(PORT, ADDR);

	radhex_t* radhex = radhex_scu(tcp);

	variant_t var = { 0 };
	var.vv = VV_LONG;

	object_t val = object_alloc(DEF_MBS);

	Srand48(time(NULL));

	radhex_connect(radhex, _T("mydb"));

	int n = 1000;
	for (int i = 0; i < n;i++)
	{
		var.long_one = (Lrand48() % n);

		object_set_variant(val, var);

		if(radhex_set(radhex, var, val))
			printf("SET %d succeed\n", var.long_one);
		else
			printf("SET %d failed\n", var.long_one);

		object_empty(val);

		if(radhex_get(radhex, var, val))
			printf("GET %d succeed\n", var.long_one);
		else
			printf("GET %d failed\n", var.long_one);

		if (radhex_del(radhex, var))
			printf("DEL %d succeed\n", var.long_one);
		else
			printf("DEL %d failed\n", var.long_one);
	}

	variant_to_null(&var);
	object_free(val);

	radhex_close(radhex);

	xtcp_close(tcp);
}

int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	test_radhex();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

