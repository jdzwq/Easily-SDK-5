
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

void test_stamp()
{
	xdate_t dt;
	tchar_t sz_date[UTC_LEN + 1] = { 0 };
	dword_t ms;

	ms = get_times();

	ms += 24 * 60 * 60;

	utc_date_from_times(&dt, ms);
	format_utctime(&dt, sz_date);
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

	//sprintf(buf,  "%c",  'W');

	//test_func(10, _T('0'), 10);

	xsprintf(buf, _T("%s%"),  _T("hello"));

	//printf(buf);
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

void test_conv()
{
	schar_t* mbs_token = "这是中文";
	byte_t utf_buf[100] = {0};
	schar_t mbs_buf[100] = {0};

	mbs_byte_to_utf8(mbs_token, utf_buf);
	utf8_byte_to_mbs(utf_buf, mbs_buf);

	//dword_t n = mbs_to_utf8(mbs_token, -1, utf_buf, 100);
	//utf8_to_mbs(utf_buf, n, mbs_buf, 100);

	link_t_ptr json = create_json_doc();
	link_t_ptr nlk = insert_json_item(json, LINK_LAST);
	set_json_item_name(nlk, _T("demo"));
	//set_json_item_value(nlk, mbs_token);

	dword_t n = format_json_doc_to_bytes(json, utf_buf, 100, _UTF8);

	destroy_json_doc(json);

	json = create_json_doc();
	parse_json_doc_from_bytes(json, utf_buf, n, _UTF8);
	format_json_doc_to_bytes(json, (byte_t*)mbs_buf, 100, _UTF8);
	destroy_json_doc(json);
}

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//test_mem();

	//test_utc();

	//test_stamp();

	//test_printf();

	//test_path2();

	//test_money();

	//test_vector();

	//test_matrix();

	//test_set();

	//test_object();

	//test_conv();

	//test_string_array();

	test_integer_array();

	test_numeric_array();

	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

