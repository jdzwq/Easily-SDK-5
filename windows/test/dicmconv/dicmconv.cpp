// dicmconv.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <tchar.h>
#include <xdl.h>

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

#define SOPCLASS_DR		_T("1.2.840.10008.5.1.4.1.1.1")
#define SOPCLASS_CT		_T("1.2.840.10008.5.1.4.1.1.2")

tchar_t g_szSource[PATH_LEN] = { 0 };
tchar_t g_szFilter[PATH_LEN] = { 0 };
tchar_t g_szTarget[PATH_LEN] = { 0 };
tchar_t g_szPeriod[INT_LEN] = { 0 };

void _read_config()
{
	tchar_t sz_file[PATH_LEN] = { 0 };
	tchar_t sz_path[PATH_LEN] = { 0 };

	get_runpath(NULL, sz_path, PATH_LEN);

	xsprintf(sz_file, _T("%s\\dicmconv.ini"), sz_path);

	read_profile(sz_file, _T("CONFIG"), _T("SOURCE"), g_szSource, PATH_LEN);
	read_profile(sz_file, _T("CONFIG"), _T("FILTER"), g_szFilter, PATH_LEN);
	read_profile(sz_file, _T("CONFIG"), _T("TARGET"), g_szTarget, PATH_LEN);
	read_profile(sz_file, _T("CONFIG"), _T("PERIOD"), g_szPeriod, INT_LEN);

	xdate_t cur_date = { 0 };
	tchar_t sz_date[DATE_LEN] = { 0 };

	get_loc_date(&cur_date);
	plus_days(&cur_date, 0 - xstol(g_szPeriod));

	xsprintf(sz_date, _T("%d%02d%02d"), cur_date.year, cur_date.mon, cur_date.day);

	xscat(g_szSource, _T("/"));
	xscat(g_szSource, sz_date);

	if (!is_null(g_szTarget))
	{
		xscat(g_szTarget, _T("/"));
		xscat(g_szTarget, sz_date);
	}
}


int __callback _enum_file(link_t_ptr plk, void* pv)
{
	link_t_ptr nlk, ptr = (link_t_ptr)pv;
	tchar_t sz_src[PATH_LEN] = { 0 };
	tchar_t sz_dst[PATH_LEN] = { 0 };
	tchar_t *token;
	const tchar_t* nname;
	int len;

	XHANDLE xf;
	XOBJECT stm;
	tchar_t sz_query[1024];

	if (is_list_file_item(plk))
	{
		token = sz_dst + PATH_LEN - 1;
		*token = _T('\0');

		nname = get_list_item_file_name_ptr(plk);
		len = xslen(nname);
		token -= len;
		xsncpy(token, nname, len);

		nlk = get_dom_parent_node(plk);
		while (nlk && nlk != ptr)
		{
			token--;
			*token = _T('/');

			nname = get_list_item_file_name_ptr(nlk);
			len = xslen(nname);
			token -= len;
			memcpy((void*)token, (void*)nname, len * sizeof(tchar_t));

			nlk = get_dom_parent_node(nlk);
		}

		xscpy(sz_src, g_szSource);
		xscat(sz_src, _T("/"));
		xscat(sz_src, token);

		xscpy(sz_dst, g_szTarget);
		xscat(sz_dst, _T("/"));
		xscat(sz_dst, get_list_item_file_name_ptr(plk));

		xf = xfile_open(NULL, sz_src, FILE_OPEN_READ);
		if (xf)
		{
			stm = stream_alloc(xf);

			xsprintf(sz_query, _T("SOPClassUID=[%s,%s]"), SOPCLASS_DR, SOPCLASS_CT);

			if (query_dicm(sz_query, stm))
			{
				xfile_copy(NULL, sz_src, sz_dst, FILE_SINCE_NONE);
			}

			stream_free(stm);
			xfile_close(xf);
		}
	}

	return 0;
}

void _conv_data()
{
	link_t_ptr ptr = create_list_doc();

	xfile_tree(NULL, g_szSource, ptr);

	enum_dom_node(ptr, (PF_LINKENUM)_enum_file, (void*)ptr);

	destroy_list_doc(ptr);
}

int main(int argc, char* argv[])
{
	xdl_process_init(0);

	_read_config();

	_conv_data();

	xdl_process_uninit();

	return 0;
}

