
#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

void test_mem()
{
	byte_t b[20] = {0};

	for(int i = 0;i< 10;i++)
	{
		b[i] = '0' + i;
	}

	xmem_move(b, 10, 10);
	xmem_zero(b, 10);

	xmem_move(b + 10, 10, -10);
	xmem_zero(b + 10, 10);
}

void test_utc()
{
	xdate_t dt;
	tchar_t sz_date[UTC_LEN + 1] = { 0 };

	get_utc_date(&dt);
	format_utctime(&dt, sz_date);

	xdate_t dt2;
	parse_datetime(&dt2, sz_date);

	int rt = compare_datetime(&dt, &dt2);
}

void test_func(int a, ...)
{
	va_list args;

	va_start(args, a);

	char c = (char)va_arg(args, int);

	int b = va_arg(args, int);

	va_end(args);
}

void test_printf()
{
	char tmp[100] = { 0 };
	int len = a_xsprintf(tmp, "%d '%Y-%m-%d %H:%i') as DT", -1);

	tchar_t buf[20] = {0};

	sprintf(buf,  _T("%c"),  _T('W'));

	//test_func(10, _T('0'), 10);

	xsprintf(buf, _T("%s%"),  _T("hello"));

	printf(buf);
}

void test_path()
{
	tchar_t pfmt[] = _T("$(XSERVICE_ROOT)/api/%s");
	tchar_t *path;

	int len = printf_path(NULL, pfmt, _T("loc_api.dll"));
	path = xsalloc(len + 1);
	printf_path(path, pfmt, _T("loc_api.dll"));

	xsfree(path);
}

void test_path2()
{
	tchar_t pfmt[] = _T("$(XSERVICE_DATA)/api");
	tchar_t *path;

	int len = printf_path(NULL, pfmt);
	path = xsalloc(len + 1);
	printf_path(path, pfmt);

	xsfree(path);
}

void test_money()
{
	tchar_t token[NUM_LEN] = { 0 };

	format_money_chs(10.01, 0, token, NUM_LEN);
}

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//test_mem();

	//test_utc();

	test_printf();

	//test_path2();

	//test_money();

	//test_vector();

	//test_matrix();

	//test_set();

	//test_object();


	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

