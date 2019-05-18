
#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

void test_path()
{
	tchar_t pfmt[] = _T("$(ROOT)/bin");
	tchar_t *path;

	int len = printf_path(NULL, pfmt, _T("D:/root"));
	path = xsalloc(len + 1);
	printf_path(path, pfmt, _T("D:/root"));

	_tprintf(_T("%s\n"), path);
	xsfree(path);
}

void test_money()
{
	tchar_t token[NUM_LEN] = { 0 };

	format_money_chs(10.01, 0, token, NUM_LEN);
}

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//test_path();

	test_money();

	
	xdl_process_uninit();

	getch();

	return 0;
}

