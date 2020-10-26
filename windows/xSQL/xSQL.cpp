// xSQL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

HMODULE g_lib = NULL;

xdb_t _open_dsn(const tchar_t* type, const tchar_t* dsnfile)
{
	if (compare_text(type, -1, _T("odbc"),-1,1) == 0)
		g_lib = LoadLibrary(_T("xdb_odbc.dll"));
	else if (compare_text(type, -1, _T("oci"), -1, 1) == 0)
		g_lib = LoadLibrary(_T("xdb_oci.dll"));
	else if (compare_text(type, -1, _T("stub"), -1, 1) == 0)
		g_lib = LoadLibrary(_T("xdb_stub.dll"));
	else
		g_lib = NULL;

	PF_DB_OPEN_DSN pf_db_open_dsn = (PF_DB_OPEN_DSN)GetProcAddress(g_lib, "db_open_dsn");
	if (!pf_db_open_dsn)
		return NULL;

	return (*pf_db_open_dsn)(dsnfile);
}

void _close_xdb(xdb_t xdb)
{
	if (!g_lib)
		return;

	PF_DB_CLOSE pf_db_close = (PF_DB_CLOSE)GetProcAddress(g_lib, "db_close");
	if (!pf_db_close)
		return;

	(*pf_db_close)(xdb);

	FreeLibrary(g_lib);
	g_lib = NULL;
}

bool_t _export_file(xdb_t xdb, const tchar_t* sql, const tchar_t* fname)
{
	PF_DB_EXPORT pf_db_export = (PF_DB_EXPORT)GetProcAddress(g_lib, "db_export");
	if (!pf_db_export)
		return 0;

	xhand_t con = NULL;
	if_bio_t bio = { 0 };
	if_fio_t* xf = NULL;
	stream_t xs = NULL;
	bool_t rt = 0;

	if (is_null(fname))
		return 0;

	if (compare_text(fname,4,_T("cons"),4,1) == 0)
	{
		//con = xcons_stdout();
		if (!con)
			return 0;

		get_bio_interface(con, &bio);

		xs = stream_alloc(&bio);
		if (!xs)
		{
			xcons_free(con);
			return 0;
		}

		stream_set_encode(xs, _GB2312);
		rt = (*pf_db_export)(xdb, xs, sql);

		stream_free(xs);
		xcons_free(con);

		return rt;
	}
	else
	{
		xf = xfile_open(NULL, fname, FILE_OPEN_CREATE);
		if (!xf)
			return 0;

		get_bio_interface(xf->fd, &bio);

		xs = stream_alloc(&bio);
		if (!xs)
		{
			xfile_close(xf);
			return 0;
		}

		stream_set_encode(xs, _UTF8);

		rt = (*pf_db_export)(xdb, NULL, sql);
		if (rt)
		{
			rt = (*pf_db_export)(xdb, xs, sql);
		}

		stream_free(xs);
		xfile_close(xf);

		return rt;
	}
}

bool_t _import_file(xdb_t xdb, const tchar_t* table, const tchar_t* fname)
{
	PF_DB_IMPORT pf_db_import = (PF_DB_IMPORT)GetProcAddress(g_lib, "db_import");
	if (!pf_db_import)
		return 0;

	if_fio_t* xf = NULL;
	stream_t xs = NULL;
	if_bio_t bio = { 0 };
	bool_t rt = 0;
	tchar_t fenc[INT_LEN + 1] = { 0 };
	int encode = 0;

	if (is_null(fname))
		return 0;

	if (!xfile_info(NULL, fname, NULL, NULL, NULL, fenc))
		return 0;

	encode = parse_encode(fenc);

	xf = xfile_open(NULL, fname, 0);
	if (!xf)
		return 0;

	get_bio_interface(xf->fd, &bio);

	xs = stream_alloc(&bio);
	if (!xs)
	{
		xfile_close(xf);
		return 0;
	}

	if (!encode)
		encode = _UTF8;

	stream_set_encode(xs, encode);
	stream_set_mode(xs, LINE_OPERA);
	stream_write_utfbom(xs, NULL);

	rt = (*pf_db_import)(xdb, xs, table);

	stream_free(xs);
	xfile_close(xf);

	return rt;
}

bool_t _batch_file(xdb_t xdb, const tchar_t* fname)
{
	PF_DB_BATCH pf_db_batch = (PF_DB_BATCH)GetProcAddress(g_lib, "db_batch");
	if (!pf_db_batch)
		return 0;

	if_fio_t* xf = NULL;
	stream_t xs = NULL;
	if_bio_t bio = { 0 };
	bool_t rt = 0;
	tchar_t fenc[INT_LEN + 1] = { 0 };
	int encode = 0;

	if (is_null(fname))
		return 0;

	if (!xfile_info(NULL, fname, NULL, NULL, NULL, fenc))
		return 0;

	encode = parse_encode(fenc);

	xf = xfile_open(NULL, fname, 0);
	if (!xf)
		return 0;

	get_bio_interface(xf->fd, &bio);

	xs = stream_alloc(&bio);
	if (!xs)
	{
		xfile_close(xf);
		return 0;
	}

	if (!encode)
		encode = _UTF8;

	stream_set_encode(xs, encode);
	stream_set_mode(xs, LINE_OPERA);

	rt = (*pf_db_batch)(xdb, xs);

	stream_free(xs);
	xfile_close(xf);

	return rt;
}

typedef enum {
	WAIT_METHOD = 0,
	WAIT_OPEN_TYPE = 1,
	WAIT_OPEN_FILE = 2,
	WAIT_CONNECT = 3,
	WAIT_DISCONNECT = 4,
	WAIT_SQLCMD = 5,

	WAIT_EXPORT_SQL = 6,
	WAIT_EXPORT_FILE = 7,
	WAIT_EXPORT = 8,

	WAIT_IMPORT_TABLE = 9,
	WAIT_IMPORT_FILE = 10,
	WAIT_IMPORT = 11,

	WAIT_BATCH_FILE = 12,
	WAIT_BATCH = 13,

	WAIT_EXIT = 100
}WAITSTEP;

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(0);

	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_verb[100] = { 0 };
	
	tchar_t sz_type[INT_LEN] = { 0 };
	tchar_t sz_file[PATH_LEN] = { 0 };
	tchar_t sz_cmd[RES_LEN] = { 0 };
	tchar_t sz_sql[8192] = { 0 };
	int len;
	int step;

	xdb_t xdb = NULL;

	_tprintf(_T("xSQL running..., ? for command help\n"));

	if (argc > 2)
	{
		xscpy(sz_type, argv[1]);
		xscpy(sz_file, argv[2]);
		step = WAIT_CONNECT;
	}
	else
	{
		step = WAIT_METHOD;
	}

	while (step != WAIT_EXIT)
	{
		xscpy(sz_verb, _T(""));

		switch (step)
		{
		case WAIT_METHOD:
			_tprintf(_T("xSQL need method: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[opendsn]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			if (compare_text(sz_verb, -1, _T("opendsn"), -1, 1) == 0)
			{
				step = WAIT_OPEN_TYPE;
			}

			break;
		case WAIT_OPEN_TYPE:
			_tprintf(_T("xSQL need open type: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[stub | odbc]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			if (compare_text(sz_verb, -1, _T("stub"), -1, 1) == 0 || compare_text(sz_verb, -1, _T("odbc"), -1, 1) == 0)
			{
				xscpy(sz_type, sz_verb);
				step = WAIT_OPEN_FILE;
			}

			break;
		case WAIT_OPEN_FILE:
			_tprintf(_T("xSQL need dsn file: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[somefile.dsn]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			if (!is_null(sz_verb))
			{
				xscpy(sz_file, sz_verb);
				step = WAIT_CONNECT;
			}
			break;
		case WAIT_CONNECT:
			xdb = _open_dsn(sz_type, sz_file);
			if (xdb)
			{
				_tprintf(_T("xSQL use %s connect to %s succeed!\n "), sz_type, sz_file);
				step = WAIT_SQLCMD;
			}
			else
			{
				_tprintf(_T("xSQL use %s connect to %s failed!\n "), sz_type, sz_file);
				step = WAIT_METHOD;
			}
			break;
		case WAIT_DISCONNECT:
			_close_xdb(xdb);
			xdb = NULL;

			_tprintf(_T("xSQL disconnected!\n "));

			step = WAIT_METHOD;
			break;
		case WAIT_SQLCMD:
			_tprintf(_T("xSQL need sql command> "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[export | import | batch | close]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			xscpy(sz_cmd, sz_verb);

			if (compare_text(sz_cmd, -1, _T("export"), -1, 1) == 0)
			{
				step = WAIT_EXPORT_SQL;
			}
			else if (compare_text(sz_cmd, -1, _T("import"), -1, 1) == 0)
			{
				step = WAIT_IMPORT_TABLE;
			}
			else if (compare_text(sz_cmd, -1, _T("batch"), -1, 1) == 0)
			{
				step = WAIT_BATCH_FILE;
			}
			else if (compare_text(sz_cmd, -1, _T("close"), -1, 1) == 0)
			{
				step = WAIT_DISCONNECT;
			}

			break;
		case WAIT_EXPORT_SQL:
			_tprintf(_T("xSQL need sql statement terminated by;> "));

			xscpy(sz_sql, _T(""));

			while (1)
			{
				xscpy(sz_verb, _T(""));
				_tscanf(_T("%s"), sz_verb);

				len = xslen(sz_verb);
				if (!len)
					break;
				else if (sz_verb[len - 1] == _T(';'))
				{
					xsncat(sz_sql, sz_verb, len - 1);
					break;
				}

				xscat(sz_sql, sz_verb);
				xscat(sz_sql, _T(" "));
			}

			if (!is_null(sz_sql))
			{
				step = WAIT_EXPORT_FILE;
			}
			break;
		case WAIT_EXPORT_FILE:
			_tprintf(_T("xSQL need export filename> "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[somefile.txt]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			if (!is_null(sz_verb))
			{
				xscpy(sz_file, sz_verb);
				step = WAIT_EXPORT;
			}

			break;
		case WAIT_EXPORT:
			if (_export_file(xdb, sz_sql, sz_file))
			{
				_tprintf(_T("export to %s succeed!\n"), sz_file);
			}
			else
			{
				get_last_error(NULL, sz_err, ERR_LEN);
				_tprintf(_T("%s\n"), sz_err);
			}

			step = WAIT_SQLCMD;
			break;
		case WAIT_IMPORT_TABLE:
			_tprintf(_T("xSQL need import tablename> "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[some database table]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			if (!is_null(sz_verb))
			{
				xscpy(sz_sql, sz_verb);
				step = WAIT_IMPORT_FILE;
			}

			break;
		case WAIT_IMPORT_FILE:
			_tprintf(_T("xSQL need import filename> "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[somefile.txt]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			if (!is_null(sz_verb))
			{
				xscpy(sz_file, sz_verb);
				step = WAIT_IMPORT;
			}

			break;
		case WAIT_IMPORT:
			if (_import_file(xdb, sz_sql, sz_file))
			{
				_tprintf(_T("import to %s succeed!\n"), sz_sql);
			}
			else
			{
				get_last_error(NULL, sz_err, ERR_LEN);
				_tprintf(_T("%s\n"), sz_err);
			}

			step = WAIT_SQLCMD;
			break;
		case WAIT_BATCH_FILE:
			_tprintf(_T("xSQL need batch filename> "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, 1, _T("?"), 1, 1) == 0)
			{
				_tprintf(_T("\t[somefile.sql]\n"));
				break;
			}
			else if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
				break;
			}

			if (!is_null(sz_verb))
			{
				xscpy(sz_file, sz_verb);
				step = WAIT_BATCH;
			}

			break;
		case WAIT_BATCH:
			if (_batch_file(xdb, sz_file))
			{
				_tprintf(_T("commit %s to database succeed!\n"), sz_file);
			}
			else
			{
				get_last_error(NULL, sz_err, ERR_LEN);
				_tprintf(_T("%s\n"), sz_err);
			}

			step = WAIT_SQLCMD;
			break;

		}
	}

	if (xdb)
		_close_xdb(xdb);

	xdl_process_uninit();

	return 0;
}

