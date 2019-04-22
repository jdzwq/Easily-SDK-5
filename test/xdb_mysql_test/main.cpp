
#include <xdl.h>
#include <xdb.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

PF_DB_OPEN_DSN pf_db_open_dsn;
PF_DB_OPEN pf_db_open;
PF_DB_CLOSE pf_db_close;
PF_DB_ERROR pf_db_error;
PF_DB_SCHEMA pf_db_schema;
PF_DB_ROWS pf_db_rows;
PF_DB_DATETIME pf_db_datetime;
PF_DB_SELECT pf_db_select;
PF_DB_EXEC pf_db_exec;
PF_DB_BATCH pf_db_batch;
PF_DB_WRITE_BLOB pf_db_write_blob;
PF_DB_READ_BLOB pf_db_read_blob;
PF_DB_WRITE_CLOB pf_db_write_clob;
PF_DB_READ_CLOB pf_db_read_clob;
PF_DB_WRITE_XDOC pf_db_write_xdoc;
PF_DB_READ_XDOC pf_db_read_xdoc;
PF_DB_IMPORT pf_db_import;
PF_DB_EXPORT pf_db_export;
PF_DB_CALL_FUNC pf_db_call_func;

//#define odbcdsn _T("./demo_odbc.dsn")
#define odbcdsn _T("./demo_stub.dsn")
//#define odbcdsn _T("./demo_mysql.dsn")
//#define odbcdsn _T("./demo_oci.dsn")

#if defined(_OS_WINDOWS)
//#define xdblib	_T("xdb_mysql.dll")
//#define xdblib	_T("xdb_oci.dll")
//#define xdblib	_T("xdb_odbc.dll")
#define xdblib	_T("xdb_stub.dll")
#elif defined(_OS_MACOS)
//#define xdblib	_T("libxdb_mysql.dylib")
#define xdblib	_T("libxdb_mysql.dylib")
#elif defined(_OS_LINUX)
//#define xdblib	_T("libxdb_oci.so")
#define xdblib	_T("libxdb_oci.so")
#endif


unsigned int STDCALL odbc_db_datetime(void* param)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	xdb_t xdb = NULL;

	bool_t rt;

	tchar_t sz_date[DATE_LEN + 1] = { 0 };

	xdl_thread_init();

	TRY_CATCH;

	xdb = (*pf_db_open_dsn)(odbcdsn);
	if (!xdb)
	{
		raise_user_error(_T("-1"), _T("open connection falied\n"));
	}

	rt = (*pf_db_datetime)(xdb, -1, sz_date);
	if (!rt)
	{
		(*pf_db_error)(xdb, errtext, ERR_LEN);

		raise_user_error(_T("-1"), errtext);
	}

#ifdef _UNICODE
	wprintf(L"%s\n", sz_date);
#else
    printf("%s\n", sz_date);
#endif

	rt = (*pf_db_datetime)(xdb, 0, sz_date);
	if (!rt)
	{
		(*pf_db_error)(xdb, errtext, ERR_LEN);

		raise_user_error(_T("-1"), errtext);
	}

#ifdef _UNICODE
	wprintf(L"%s\n", sz_date);
#else
   printf("%s\n", sz_date);
#endif

	rt = (*pf_db_datetime)(xdb, 1, sz_date);
	if (!rt)
	{
		(*pf_db_error)(xdb, errtext, ERR_LEN);

		raise_user_error(_T("-1"), errtext);
	}

#ifdef _UNICODE
    wprintf(L"%s\n", sz_date);
#else
    printf("%s\n", sz_date);
#endif

	(*pf_db_close)(xdb);
	xdb = NULL;

	END_CATCH;

	xdl_thread_uninit(0);

	return 0;

ONERROR:

	if (xdb)
		(*pf_db_close)(xdb);

	xdl_thread_uninit(0);

	return 0;
}

unsigned int STDCALL odbc_db_exec(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    string_t vs = NULL;
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    vs = string_alloc();
    
    string_append(vs, _T("insert into dogs (did,dname,dage,dprice) values ('%s','%s','%s','%s')\n"),
                  _T("1"),
                  _T("旺\n财"),
                  _T("2014-01-01"),
                  _T("100.25"));
    
    string_append(vs, _T("insert into dogs (did,dname,dage,dprice) values ('%s','%s','%s','%s')\n"),
                  _T("2"),
                  _T("\r\n来福"),
                  _T("2015-01-01"),
                  _T("200.50"));
    
    rt = (*pf_db_exec)(xdb, string_ptr(vs), string_len(vs));
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    string_free(vs);
    vs = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (vs)
        string_free(vs);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_schema(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    
    LINKPTR grid = NULL;
    LINKPTR clk;
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    grid = create_grid_doc();
    
    rt = (*pf_db_schema)(xdb, grid, _T("select * from dogs"));
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    clk = get_next_col(grid, LINK_FIRST);
    while (clk)
    {
#ifdef _UNICODE
        wprintf(L"%s %s %d\n", get_col_name_ptr(clk), get_col_data_type_ptr(clk), get_col_data_len(clk));
#else
        printf("%s %s %d\n", get_col_name_ptr(clk), get_col_data_type_ptr(clk), get_col_data_len(clk));
#endif
        clk = get_next_col(grid, clk);
    }
    
    destroy_grid_doc(grid);
    grid = NULL;
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    
    get_last_error(errcode, errtext, ERR_LEN);
    
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (grid)
        destroy_grid_doc(grid);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_select(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    
    LINKPTR grid = NULL;
    LINKPTR clk1, clk2, clk3, clk4, rlk;
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    grid = create_grid_doc();
    rt = (*pf_db_select)(xdb, grid, _T("SELECT * FROM dogs"));
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    clk1 = get_col(grid, _T("did"));
    clk2 = get_col(grid, _T("dname"));
    clk3 = get_col(grid, _T("dage"));
    clk4 = get_col(grid, _T("dprice"));
    
    rlk = get_next_row(grid, LINK_FIRST);
    while (rlk)
    {
#ifdef _UNICODE
        wprintf(L"%s\n", get_cell_text_ptr(rlk, clk1));
        wprintf(L"%s\n", get_cell_text_ptr(rlk, clk2));
        wprintf(L"%s\n", get_cell_text_ptr(rlk, clk3));
        wprintf(L"%s\n", get_cell_text_ptr(rlk, clk4));
#else
        printf("%s\n", get_cell_text_ptr(rlk, clk1));
        printf("%s\n", get_cell_text_ptr(rlk, clk2));
        printf("%s\n", get_cell_text_ptr(rlk, clk3));
        printf("%s\n", get_cell_text_ptr(rlk, clk4));

#endif
        
        rlk = get_next_row(grid, rlk);
    }
    
    destroy_grid_doc(grid);
    grid = NULL;
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (grid)
        destroy_grid_doc(grid);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_proc(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    
    LINKPTR func = NULL;
    LINKPTR nlk;
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    func = create_func_doc();
    set_func_name(func, _T("get_dog"));
    set_func_data_type(func, ATTR_DATA_TYPE_INTEGER);
    
    nlk = insert_func_param(func, LINK_LAST);
    set_func_param_name(nlk, _T("did"));
    set_func_param_data_type(nlk, ATTR_DATA_TYPE_STRING);
    set_func_param_data_len(nlk, 10);
    set_func_param_type(nlk, ATTR_PARAM_TYPE_INPUT);
    set_func_param_text(nlk, _T("2"), -1);
    
    nlk = insert_func_param(func, LINK_LAST);
    set_func_param_name(nlk, _T("dname"));
    set_func_param_data_type(nlk, ATTR_DATA_TYPE_STRING);
    set_func_param_data_len(nlk, 50);
    set_func_param_type(nlk, ATTR_PARAM_TYPE_OUTPUT);
    
    nlk = insert_func_param(func, LINK_LAST);
    set_func_param_name(nlk, _T("dage"));
    set_func_param_data_type(nlk, ATTR_DATA_TYPE_DATE);
    set_func_param_type(nlk, ATTR_PARAM_TYPE_OUTPUT);
    
    nlk = insert_func_param(func, LINK_LAST);
    set_func_param_name(nlk, _T("dprice"));
    set_func_param_data_type(nlk, ATTR_DATA_TYPE_NUMERIC);
    set_func_param_data_len(nlk, 10);
    set_func_param_data_dig(nlk, 2);
    set_func_param_type(nlk, ATTR_PARAM_TYPE_OUTPUT);
    
    rt = (*pf_db_call_func)(xdb, func);
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    nlk = get_func_param(func, _T("dname"), -1);
    if (nlk)
    {
#ifdef _UNICODE
        wprintf(L"%s\n", get_func_param_text_ptr(nlk));
#else
        printf("%s\n", get_func_param_text_ptr(nlk));
#endif
    }
    
    nlk = get_func_param(func, _T("dage"), -1);
    if (nlk)
    {
#ifdef _UNICODE
        wprintf(L"%s\n", get_func_param_text_ptr(nlk));
#else
        printf("%s\n", get_func_param_text_ptr(nlk));
#endif
    }
    
    nlk = get_func_param(func, _T("dprice"), -1);
    if (nlk)
    {
#ifdef _UNICODE
        wprintf(L"%f\n", get_func_param_numeric(nlk));
#else
        printf("%f\n", get_func_param_numeric(nlk));
#endif
    }
    
    destroy_func_doc(func);
    func = NULL;
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (func)
        destroy_func_doc(func);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_batch(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    tchar_t fsize[NUM_LEN + 1] = { 0 };
    file_t file = NULL;
    stream_t stream = NULL;
    dword_t size = 0;
    bool_t rt;
    tchar_t enc[INT_LEN] = { 0 };
    byte_t bom[4] = { 0 };
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    if (!xfile_info(NULL, _T("./dogs.sql"), NULL, fsize, NULL, enc))
    {
        raise_user_error(_T("-1"), _T("open file falied\n"));
    }
    
    file = xfile_open(NULL, _T("./dogs.sql"), FILE_OPEN_READ);
    if (!file)
    {
        raise_user_error(_T("-1"), _T("open file falied\n"));
    }
    
    size = xstol(fsize);
    
    stream = stream_alloc(xfile_bio(file));
    stream_set_encode(stream, parse_charset(enc));
    stream_set_mode(stream, LINE_OPERA);
    stream_set_size(stream, size);
    
    rt = (*pf_db_batch)(xdb, stream);
    
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    stream_free(stream);
    stream = NULL;
    
    xfile_close(file);
    file = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (stream)
        stream_free(stream);
    
    if (file)
        xfile_close(file);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_export(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    tchar_t fsize[NUM_LEN + 1] = { 0 };
    file_t file = NULL;
    stream_t stream = NULL;
    dword_t size = 0;
    bool_t rt;
    
    byte_t bom[4] = { 0 };
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    file = xfile_open(NULL, _T("./dogs2.txt"), FILE_OPEN_CREATE);
    if (!file)
    {
        raise_user_error(_T("-1"), _T("open file falied\n"));
    }
    
    stream = stream_alloc(xfile_bio(file));
    
    stream_set_encode(stream, _UTF8);
    
    rt = (*pf_db_export)(xdb, stream, _T("select * from dogs"));
    
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    stream_free(stream);
    stream = NULL;
    
    xfile_close(file);
    file = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (stream)
        stream_free(stream);
    
    if (file)
        xfile_close(file);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_import(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    tchar_t fsize[NUM_LEN + 1] = { 0 };
    tchar_t enc[INT_LEN] = { 0 };
    file_t file = NULL;
    stream_t stream = NULL;
    dword_t size = 0;
    bool_t rt;
    
    byte_t bom[4] = { 0 };
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    if (!xfile_info(NULL, _T("./dogs.txt"), NULL, fsize, NULL, enc))
    {
        raise_user_error(_T("-1"), _T("open file falied\n"));
    }
    
    file = xfile_open(NULL, _T("./dogs.txt"),FILE_OPEN_READ);
    if (!file)
    {
        raise_user_error(_T("-1"), _T("open file falied\n"));
    }
    
    stream = stream_alloc(xfile_bio(file));
    stream_set_encode(stream, parse_charset(enc));
    stream_set_mode(stream, LINE_OPERA);
    stream_set_size(stream, xstol(fsize));
    
    rt = (*pf_db_import)(xdb, stream, _T("dogs"));
    
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    stream_free(stream);
    stream = NULL;
    
    xfile_close(file);
    file = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (stream)
        stream_free(stream);
    
    if (file)
        xfile_close(file);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_write_blob(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    file_t file = NULL;
    stream_t stream = NULL;
    
    tchar_t fsize[NUM_LEN + 1] = { 0 };
    tchar_t ftime[DATE_LEN + 1] = { 0 };
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    if (!xfile_info(NULL, _T("pig.jpeg"), ftime, fsize, NULL, NULL))
    {
        raise_user_error(_T("-1"), _T("open pig.jpeg falied"));
    }
    
    file = xfile_open(NULL, _T("pig.jpeg"), 0);
    if (file == NULL)
    {
        raise_user_error(_T("-1"), _T("open pig.jpeg falied"));
    }
    
    stream = stream_alloc(xfile_bio(file));
    stream_set_size(stream, xstol(fsize));
    
    rt = (*pf_db_write_blob)(xdb, stream, _T("insert into pigs (pid,pdoc) values ('1',?)"));
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    stream_free(stream);
    stream = NULL;
    
    xfile_close(file);
    file = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (stream)
        stream_free(stream);
    
    if (file)
        xfile_close(file);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_read_blob(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    file_t file = NULL;
    stream_t stream = NULL;
    
    tchar_t fsize[NUM_LEN + 1] = { 0 };
    tchar_t ftime[DATE_LEN + 1] = { 0 };
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    file = xfile_open(NULL, _T("pig2.jpg"), FILE_OPEN_CREATE);
    if (file == NULL)
    {
        raise_user_error(_T("-1"), _T("open pig2.jpg falied"));
    }
    
    stream = stream_alloc(xfile_bio(file));
    
    rt = (*pf_db_read_blob)(xdb, stream, _T("select pdoc from pigs where pid = '1'"));
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    stream_free(stream);
    stream = NULL;
    
    xfile_close(file);
    file = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (stream)
        stream_free(stream);
    
    if (file)
        xfile_close(file);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_write_clob(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    file_t file = NULL;
    string_t vs = NULL;
    
    tchar_t fsize[NUM_LEN + 1] = { 0 };
    tchar_t ftime[DATE_LEN + 1] = { 0 };
    
    bool_t rt;
    
    dword_t base_size, file_size = 0;
    byte_t* file_buf = NULL;
    tchar_t* base_buf = NULL;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    if (!xfile_info(NULL, _T("cat.jpeg"), ftime, fsize, NULL, NULL))
    {
        raise_user_error(_T("-1"), _T("open cat.jpeg falied"));
    }
    
    file = xfile_open(NULL, _T("cat.jpeg"), 0);
    if (file == NULL)
    {
        raise_user_error(_T("-1"), _T("open cat.jpeg falied"));
    }
    
    file_size = xstol(fsize);
    file_buf = (byte_t*)xmem_alloc(file_size);
    
    xfile_read(file, file_buf, file_size);
    
    xfile_close(file);
    file = NULL;
    
    base_size = xbas_encode(file_buf, file_size, NULL, MAX_LONG);
    base_buf = xsalloc(base_size + 1);
    xbas_encode(file_buf, file_size, base_buf, base_size);
    
    xmem_free(file_buf);
    file_buf = NULL;
    
    vs = string_alloc();
    string_cat(vs, base_buf, base_size);
    
    xsfree(base_buf);
    base_buf = NULL;
    
    rt = (*pf_db_write_clob)(xdb, vs, _T("insert into cats (cid,cdoc) values ('1',?)"));
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    string_free(vs);
    vs = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (file_buf)
        xmem_free(file_buf);
    
    if (base_buf)
        xsfree(base_buf);
    
    if (vs)
        string_free(vs);
    
    if (file)
        xfile_close(file);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_read_clob(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    file_t file = NULL;
    string_t vs = NULL;
    
    tchar_t fsize[NUM_LEN + 1] = { 0 };
    tchar_t ftime[DATE_LEN + 1] = { 0 };
    
    dword_t file_size = 0;
    byte_t* file_buf = NULL;
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    vs = string_alloc();
    
    rt = (*pf_db_read_clob)(xdb, vs, _T("select cdoc from cats where cid = '1'"));
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    file_size = xbas_decode(string_ptr(vs), string_len(vs), NULL, MAX_LONG);
    file_buf = (byte_t*)xmem_alloc(file_size);
    xbas_decode(string_ptr(vs), string_len(vs), file_buf, file_size);
    
    string_free(vs);
    vs = NULL;
    
    file = xfile_open(NULL, _T("cat2.jpg"), FILE_OPEN_CREATE);
    if (file == NULL)
    {
        raise_user_error(_T("-1"), _T("open cat2.jpg falied"));
    }
    
    xfile_write(file, file_buf, file_size);
    
    xfile_close(file);
    file = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (file_buf)
        xmem_free(file_buf);
    
    if (vs)
        string_free(vs);
    
    if (file)
        xfile_close(file);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_write_xdoc(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    link_t_ptr nlk, dom, xml = NULL;
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    xml = create_xml_doc();
    if (!load_xml_doc_from_file(xml, NULL, _T("demo.xml")))
    {
        get_last_error(errcode, errtext, ERR_LEN);
        raise_user_error(_T("-1"), _T("open file falied"));
    }
    
    dom = downcast_xml_to_dom(xml);
    
    nlk = get_dom_first_child_node(dom);
    nlk = (nlk) ? get_dom_first_child_node(nlk) : NULL;
    
    if (nlk)
    {
        set_dom_node_cdata(nlk, 1);
        dom_node_text_ins_chars(nlk, -1, _T("\f"), 1);
    }
    
    rt = (*pf_db_write_xdoc)(xdb, dom, _T("insert into docs (xid,xdoc) values ('1',?)"));
    //rt = (*pf_db_write_xdoc)(xdb, dom, _T("update docs set xdoc = ? where xid= '1'\n"));
    xml = upcast_dom_to_xml(dom);
    
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    destroy_xml_doc(xml);
    xml = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (xml)
        destroy_xml_doc(xml);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}

unsigned int STDCALL odbc_db_read_xdoc(void* param)
{
    tchar_t errcode[NUM_LEN + 1] = { 0 };
    tchar_t errtext[ERR_LEN + 1] = { 0 };
    
    xdb_t xdb = NULL;
    int rows;
    
    link_t_ptr dom, xml = NULL;
    
    bool_t rt;
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    xdb = (*pf_db_open_dsn)(odbcdsn);
    if (!xdb)
    {
        raise_user_error(_T("-1"), _T("open connection falied\n"));
    }
    
    xml = create_xml_doc();
    dom = downcast_xml_to_dom(xml);
    
    rt = (*pf_db_read_xdoc)(xdb, dom, _T("select top 1 xdoc from docs"));
    xml = upcast_dom_to_xml(dom);
    
    if (!rt)
    {
        (*pf_db_error)(xdb, errtext, ERR_LEN);
        
        raise_user_error(_T("-1"), errtext);
    }
    
    save_xml_doc_to_file(xml, NULL, _T("demo2.xml"));
    
    destroy_xml_doc(xml);
    xml = NULL;
    
    rows = (*pf_db_rows)(xdb);
    
#ifdef _UNICODE
    wprintf(L"%d rows affected\n", rows);
#else
    printf("%d rows affected\n", rows);
#endif
    
    (*pf_db_close)(xdb);
    xdb = NULL;
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return 0;
    
ONERROR:
    get_last_error(errcode, errtext, ERR_LEN);
#ifdef _UNICODE
    wprintf(L"%s\n", errtext);
#else
    printf("%s\n", errtext);
#endif
    
    if (xml)
        destroy_xml_doc(xml);
    
    if (xdb)
        (*pf_db_close)(xdb);
    
    xdl_thread_uninit(0);
    
    return 0;
}


int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
    int maxt = 5000;

	res_modu_t lib = load_library(xdblib);

	pf_db_open_dsn = (PF_DB_OPEN_DSN)get_address(lib, "db_open_dsn");
	pf_db_close = (PF_DB_CLOSE)get_address(lib, "db_close");
	pf_db_error = (PF_DB_ERROR)get_address(lib, "db_error");
	pf_db_schema = (PF_DB_SCHEMA)get_address(lib, "db_schema");
	pf_db_select = (PF_DB_SELECT)get_address(lib, "db_select");
	pf_db_rows = (PF_DB_ROWS)get_address(lib, "db_rows");
	pf_db_datetime = (PF_DB_DATETIME)get_address(lib, "db_datetime");
	pf_db_exec = (PF_DB_EXEC)get_address(lib, "db_exec");
	pf_db_batch = (PF_DB_BATCH)get_address(lib, "db_batch");
	pf_db_write_blob = (PF_DB_WRITE_BLOB)get_address(lib, "db_write_blob");
	pf_db_read_blob = (PF_DB_READ_BLOB)get_address(lib, "db_read_blob");
	pf_db_write_clob = (PF_DB_WRITE_CLOB)get_address(lib, "db_write_clob");
	pf_db_read_clob = (PF_DB_READ_CLOB)get_address(lib, "db_read_clob");
	pf_db_write_xdoc = (PF_DB_WRITE_XDOC)get_address(lib, "db_write_xdoc");
	pf_db_read_xdoc = (PF_DB_READ_XDOC)get_address(lib, "db_read_xdoc");
	pf_db_import = (PF_DB_IMPORT)get_address(lib, "db_import");
	pf_db_export = (PF_DB_EXPORT)get_address(lib, "db_export");
	pf_db_call_func = (PF_DB_CALL_FUNC)get_address(lib, "db_call_func");

	res_hand_t* pth = (res_hand_t*)xmem_alloc(maxt * sizeof(res_hand_t));

    for (int i = 0; i < maxt; i++)
    {
        xthread_begin(&pth[i], (PF_THREADFUNC)odbc_db_datetime, (void*)0);
        
        //xthread_begin(&pth[i], (PF_THREADFUNC)odbc_db_schema, (void*)0);
                      
        //xthread_begin(&pth[i], (PF_THREADFUNC)odbc_db_exec, (void*)0);
        
        //xthread_begin(&pth[i], (PF_THREADFUNC)odbc_db_select, (void*)0);
        
        //xthread_begin(&pth[i], (PF_THREADFUNC)odbc_db_batch, (void*)0);
        
        //xthread_begin(&pth[i], (PF_THREADFUNC)odbc_db_export, (void*)0);
        
        //xthread_begin(&pth[i], (PF_THREADFUNC)odbc_db_import, (void*)0);
        
        xthread_sleep(10);
    }
    
	for (int i = 0; i < maxt; i++)
	{
		xthread_join(pth[i]);
	}

	xmem_free(pth);

	free_library(lib);

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

