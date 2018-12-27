
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

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//test_path();

	//test_rand();

	//test_prim();

	//test_set();

	//test_vector();

	//test_matrix();

	//test_uuid();

	//test_map();

	test_variant();

	//test_object();

	//test_dict_table();

	//test_hash_table();

	//test_ac_table();

	//test_trie_tree();

	//test_bina_tree();

	//test_file_table(_T("filetable"));

	//test_bplus_tree();

	//test_bplus_tree_file_table(_T("indextable"), _T("datatable"));

	xdl_process_uninit();

	getch();

	return 0;
}

