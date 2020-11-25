
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

void test_intset()
{
	const tchar_t* str = _T("[1,2-5,7, 9-10, 12-20]");

	int n = parse_intset(str, -1, NULL, MAX_LONG);

	int* sa = (int*)xmem_alloc(n * sizeof(int));
	parse_intset(str, -1, sa, n);

	for (int i = 0; i < n; i++)
	{
		_tprintf(_T("%d\n"), sa[i]);
	}

	xmem_free(sa);
}

void test_words()
{
	const tchar_t* str = _T("这是ABC一段字体测试 文字");
	int n,total = 0;

	int len = xslen(str);

	while(n = next_word((str + total), (len - total)))
	{
		_tprintf(_T("%d\n"), n);
		total += n;
	}

	_tprintf(_T("len:%d total:%d\n"), len, total);
}

typedef struct _time_hint{
	int n_mon;
	int* p_mon;
	int n_week;
	int* p_week;
	int n_day;
	int* p_day;
	int n_hour;
	int* p_hour;
	int n_min;
	int* p_min;
	int n_sec;
	int* p_sec;
}time_hint;

void test_time_hint()
{
	time_hint th = { 0 };

	th.n_mon = parse_intset(_T("[1-12]"), -1, NULL, MAX_LONG);
	th.p_mon = (int*)xmem_alloc(th.n_mon * sizeof(int));
	parse_intset(_T("[1-12]"), -1, th.p_mon, th.n_mon);

	th.n_week = parse_intset(_T("[0,2,4]"), -1, NULL, MAX_LONG);
	th.p_week = (int*)xmem_alloc(th.n_week * sizeof(int));
	parse_intset(_T("[0,2,4]"), -1, th.p_week, th.n_week);

	th.n_day = parse_intset(_T("[1,2,3,4,5,6,7]"), -1, NULL, MAX_LONG);
	th.p_day = (int*)xmem_alloc(th.n_day * sizeof(int));
	parse_intset(_T("[1,2,3,4,5,6,7]"), -1, th.p_day, th.n_day);

	th.n_hour = parse_intset(_T("[10]"), -1, NULL, MAX_LONG);
	th.p_hour = (int*)xmem_alloc(th.n_hour * sizeof(int));
	parse_intset(_T("[10]"), -1, th.p_hour, th.n_hour);

	th.n_min = parse_intset(_T("[30]"), -1, NULL, MAX_LONG);
	th.p_min = (int*)xmem_alloc(th.n_min * sizeof(int));
	parse_intset(_T("[30]"), -1, th.p_min, th.n_min);

	th.n_sec = parse_intset(_T("[45]"), -1, NULL, MAX_LONG);
	th.p_sec = (int*)xmem_alloc(th.n_sec * sizeof(int));
	parse_intset(_T("[45]"), -1, th.p_sec, th.n_sec);

	xdate_t dt1 = { 0 }, dt2 = { 0 };
	parse_datetime(&dt1, _T("2020-01-01 00:00:00"));
	parse_datetime(&dt2, _T("2020-12-31 00:00:00"));

	tchar_t token[DATE_LEN] = { 0 };
	bool_t b = 0;
	int i;

	while (compare_datetime(&dt1, &dt2) < 0)
	{
		mak_loc_week(&dt1);

		b = 0;
		for (i = 0; i < th.n_mon; i++)
		{
			if (dt1.mon == th.p_mon[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && th.n_mon)
		{
			plus_months(&dt1, 1);
			dt1.day = 1;
			dt1.hour = 0;
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}
		
		b = 0;
		for (i = 0; i < th.n_week; i++)
		{
			if (dt1.wday == th.p_week[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && th.n_week)
		{
			plus_days(&dt1, 1);
			dt1.hour = 0;
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < th.n_day; i++)
		{
			if (dt1.day == th.p_day[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && th.n_day)
		{
			plus_days(&dt1, 1);
			dt1.hour = 0;
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < th.n_hour; i++)
		{
			if (dt1.hour == th.p_hour[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && th.n_hour)
		{
			plus_hours(&dt1, 1);
			dt1.min = 0;
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < th.n_min; i++)
		{
			if (dt1.min == th.p_min[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && th.n_min)
		{
			plus_minutes(&dt1, 1);
			dt1.sec = 0;
			continue;
		}

		b = 0;
		for (i = 0; i < th.n_sec; i++)
		{
			if (dt1.sec == th.p_sec[i])
			{
				b = 1;
				break;
			}
		}
		if (!b && th.n_sec)
		{
			plus_seconds(&dt1, 1);
			continue;
		}

		mak_loc_week(&dt1);
		format_datetime(&dt1, token);
		_tprintf(_T("%s W%d\n"), token, dt1.wday);

		plus_seconds(&dt1, 1);
	}

	xmem_free(th.p_mon);
	xmem_free(th.p_week);
	xmem_free(th.p_day);
	xmem_free(th.p_hour);
	xmem_free(th.p_min);
	xmem_free(th.p_sec);
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

	test_string_array();

	//test_integer_array();

	//test_numeric_array();

	//test_intset();

	//test_words();

	//test_time_hint();

	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

