// xdl_json_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define JSON_TOKEN _T("{\
	\"name\": \"中国\",\
		\"province\" : [{\
		\"name\": \"黑龙江\",\
			\"cities\" : {\
			\"city\": [\"哈尔滨\", \"大庆\"]\
		}\
	}, {\
		\"name\": \"广东\",\
		\"cities\" : {\
			\"city\": [\"广州\", \"深圳\", \"珠海\"]\
		}\
	}, {\
		\"name\": \"台湾\",\
		\"cities\" : {\
			\"city\": [\"台北\", \"高雄\"]\
		}\
	}, {\
		\"name\": \"新疆\",\
		\"cities\" : {\
			\"city\": [\"乌鲁木齐\"]\
		}\
	}]\
}")

/*{"name" : "中国","province" : [{"name" : "黑龙江","cities" : {"city" : ["哈尔滨","大庆"]}},{"name" : "广东","cities" : {"city" : ["广州","深圳","珠海"]}},{"name" : "台湾","cities" : {"city" : ["台北","高雄"]}},{"name" : "新疆","cities" : {"city" : ["乌鲁木齐"]}}]}
*/

void _test_string()
{
	link_t_ptr ptr = create_json_doc();

	string_t vs = varstr_alloc();

	varstr_cpy(vs, JSON_TOKEN, -1);

	parse_json_doc_from_string(ptr, vs);

	varstr_empty(vs);

	format_json_doc_to_string(ptr, vs);

	parse_json_doc_from_string(ptr, vs);

	save_dom_doc_to_file(ptr, NULL, _T("json.xml"));

	destroy_json_doc(ptr);

	wprintf(_T("%s\n"), varstr_ptr(vs));

	varstr_free(vs);
}

void _test_bytes()
{
	int len = xslen(JSON_TOKEN);

	int dw = ucs_to_mbs(JSON_TOKEN, len, NULL, MAX_LONG);
	schar_t* buf = (schar_t*)a_xsalloc(dw + 1);
	ucs_to_mbs(JSON_TOKEN, len, buf, dw);

	link_t_ptr ptr = create_json_doc();

	parse_json_doc_from_bytes(ptr, (byte_t*)buf, dw, _GB2312);

	save_dom_doc_to_file(ptr, NULL, _T("json.xml"));

	a_xszero(buf, dw + 1);

	format_json_doc_to_bytes(ptr, (byte_t*)buf, dw, _GB2312);

	destroy_json_doc(ptr);

	printf("%s\n", buf);
	a_xsfree(buf);
}

void _test_stream()
{
	link_t_ptr ptr = create_json_doc();

	string_t vs = varstr_alloc();

	varstr_cpy(vs, JSON_TOKEN, -1);

	parse_json_doc_from_string(ptr, vs);

	varstr_free(vs);

	file_t xf = xfile_open(NULL, _T("json.txt"), FILE_OPEN_CREATE);

	stream_t stm = stream_alloc(xfile_bio(xf));

	stream_set_encode(stm, _UTF8);
	stream_write_utfbom(stm, NULL);

	format_json_doc_to_stream(ptr, stm);

	stream_free(stm);

	xfile_close(xf);

	xf = xfile_open(NULL, _T("json.txt"), FILE_OPEN_READ);
	stm = stream_alloc(xfile_bio(xf));

	stream_set_encode(stm, _UTF8);
	stream_read_utfbom(stm, NULL);

	parse_json_doc_from_stream(ptr, stm);

	stream_free(stm);

	xfile_close(xf);

	save_dom_doc_to_file(ptr, NULL, _T("json.xml"));

	destroy_json_doc(ptr);
}

void _test_memo()
{
	link_t_ptr ptr = create_json_doc();

	string_t vs = varstr_alloc();

	varstr_cpy(vs, JSON_TOKEN, -1);

	parse_json_doc_from_string(ptr, vs);

	varstr_free(vs);

	link_t_ptr memo = create_memo_doc();

	format_json_doc_to_memo(ptr, memo);

	save_memo_doc_to_file(memo, NULL, _T("json.txt"));

	clear_memo_doc(memo);
	load_memo_doc_from_file(memo, NULL, _T("json.txt"));

	parse_json_doc_from_memo(ptr, memo);

	destroy_memo_doc(memo);

	save_dom_doc_to_file(ptr, NULL, _T("json.xml"));

	destroy_json_doc(ptr);
}

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	//_test_string();

	//_test_bytes();

	//_test_stream();

	_test_memo();

	xdl_process_uninit();

	getch();
	return 0;
}

