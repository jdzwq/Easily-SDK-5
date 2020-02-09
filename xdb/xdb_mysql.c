/***********************************************************************
	Easily xdb mysql

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdb oci document

	@module	xdb_mysql.c | xdb mysql implement file

***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "xdbpro.h"

#include <mysql.h>

#ifdef _OS_WINDOWS
#pragma comment(lib,"mysqlclient.lib")
#endif

#define MYSQL_PARAM_TYPE_UNKNOWN    0
#define MYSQL_PARAM_INPUT           1
#define MYSQL_PARAM_INPUT_OUTPUT    2
#define MYSQL_RESULT_COL            3
#define MYSQL_PARAM_OUTPUT          4
#define MYSQL_RETURN_VALUE          5

#define SQL_BREAK   _T(";")

#define MYSQL_FIELD_SIZE(n)			(n / 3)

typedef struct _db_t{
	xdb_head head;

	int chs;
	MYSQL* ctx;
    MYSQL_STMT* stm;
    
	bool_t trans;
	int rows;
	tchar_t err_code[NUM_LEN + 1];
	tchar_t err_text[ERR_LEN + 1];
}db_t;

static void sqltodt(int type, tchar_t* dt)
{
	switch (type) {
	case MYSQL_TYPE_VARCHAR:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_GEOMETRY:
		xscpy(dt, ATTR_DATA_TYPE_STRING);
		break;
	case MYSQL_TYPE_TINY:
	case MYSQL_TYPE_SHORT:
	case MYSQL_TYPE_LONG:
		xscpy(dt, ATTR_DATA_TYPE_INTEGER);
		break;
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_FLOAT:
	case MYSQL_TYPE_DOUBLE:
		xscpy(dt, ATTR_DATA_TYPE_NUMERIC);
		break;
	case MYSQL_TYPE_LONGLONG:
	case MYSQL_TYPE_INT24:
		xscpy(dt, ATTR_DATA_TYPE_INTEGER);
		break;
	case MYSQL_TYPE_BIT:
	case MYSQL_TYPE_TINY_BLOB:
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:
		xscpy(dt, ATTR_DATA_TYPE_STRING);
		break;
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_NEWDATE:
		xscpy(dt, ATTR_DATA_TYPE_DATETIME);
		break;
	default:
		xscpy(dt, ATTR_DATA_TYPE_STRING);
		break;
	}
}

static int sqltolen(int type, int size)
{
	switch (type) 
	{
	case MYSQL_TYPE_VARCHAR:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_GEOMETRY:
		return size / 3;
	default:
		return size;
	}
}

static int split_semi(const tchar_t* token, int len)
{
	int tklen = 0, total = 0;
	bool_t glt = 0;
	const tchar_t* tkcur = token;

	if (len < 0)
		len = xslen(token);

	if (!len)
		return 0;

	while (*tkcur != _T(';') && *tkcur != _T('\0') && total < len)
	{
		if (*tkcur == _T('\'') || *tkcur == _T('\"'))
		{
			if (glt)
				glt = 0;
			else
				glt = 1;
		}

		tklen++;
		tkcur++;
		total++;

		if (glt)
		{
			while (*tkcur == _T(';'))
			{
				tklen++;
				tkcur++;
				total++;
			}
		}
	}

	return total;
}

static void _raise_ctx_error(MYSQL* ctx)
{
    tchar_t errcode[NUM_LEN + 1] = {0};
    tchar_t errtext[ERR_LEN + 1] = {0};
    
    const char* str;
    int len;
    
    str = mysql_error(ctx);
    len = a_xslen(str);
    
#ifdef _UNICODE
	utf8_to_ucs((byte_t*)str, len, errtext, ERR_LEN);
#else
	utf8_to_mbs((byte_t*)str, len, errtext, ERR_LEN);
#endif

	xsprintf(errcode, _T("%d"), mysql_errno(ctx));
    
    raise_user_error(errcode, errtext);
}

static void _raise_stm_error(MYSQL_STMT* stm)
{
    tchar_t err_code[NUM_LEN + 1] = { 0 };
    tchar_t err_text[ERR_LEN + 1] = { 0 };
    
    const char* str;
    int len;
    
    str = mysql_stmt_error(stm);
    len = a_xslen(str);

#ifdef _UNICODE
    utf8_to_ucs((byte_t*)str, len, err_text, ERR_LEN);
#else
    utf8_to_mbs((byte_t*)str, len, err_text, ERR_LEN);
#endif
    
    xsprintf(err_code, _T("%d"), mysql_stmt_errno(stm));
    
    raise_user_error(err_code, err_text);
}

static void _db_reset(db_t* pdb)
{
	xscpy(pdb->err_code, _T(""));
	xscpy(pdb->err_text, _T(""));

	pdb->rows = 0;
}

static void _db_tran(db_t* pdb)
{
	/*char sql[MIN_SQL_LEN] = { 0 };
	int len;
	
#ifdef _UNICODE
	len = ucs_to_utf8(_T("set autocommit=0;"), -1, (byte_t*)sql, MIN_SQL_LEN);
#else
	len = mbs_to_utf8(_T("set autocommit=0;"), -1, (byte_t*)sql, MIN_SQL_LEN);
#endif
	sql[len] = '\0';

	if (mysql_query(pdb->ctx, sql) != C_OK)
	{
		return;
	}

#ifdef _UNICODE
	len = ucs_to_utf8(_T("start transaction;"), -1, (byte_t*)sql, MIN_SQL_LEN);
#else
	len = mbs_to_utf8(_T("start transaction;"), -1, (byte_t*)sql, MIN_SQL_LEN);
#endif
	sql[len] = '\0';

	if (mysql_query(pdb->ctx, sql) != C_OK)
	{
		return;
	}*/
    
    if(!mysql_autocommit(pdb->ctx, 0))
        return;

	pdb->trans = 1;
}

static void _db_commit(db_t* pdb)
{
	/*char sql[MIN_SQL_LEN] = { 0 };
	int len;

	if (!pdb->trans)
		return;

#ifdef _UNICODE
	len = ucs_to_utf8(_T("commit;"), -1, (byte_t*)sql, 1024);
#else
	len = mbs_to_utf8(_T("commit;"), -1, (byte_t*)sql, 1024);
#endif
	sql[len] = '\0';

	mysql_query(pdb->ctx, sql);*/
    
    mysql_commit(pdb->ctx);
    
    mysql_autocommit(pdb->ctx, 1);

	pdb->trans = 0;
}

static void _db_rollback(db_t* pdb)
{
	/*char sql[MIN_SQL_LEN] = { 0 };
	int len;

	if (!pdb->trans)
		return;

#ifdef _UNICODE
	len = ucs_to_utf8(_T("rollback;"), -1, (byte_t*)sql, MIN_SQL_LEN);
#else
	len = mbs_to_utf8(_T("rollback;"), -1, (byte_t*)sql, MIN_SQL_LEN);
#endif
	sql[len] = '\0';

	mysql_query(pdb->ctx, sql);*/
    
    mysql_rollback(pdb->ctx);
    
    mysql_autocommit(pdb->ctx, 1);
    
	pdb->trans = 0;
}

bool_t STDCALL db_parse_dsn(const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len)
{
	LINKPTR d_ptr = NULL;

	TRY_CATCH;

	d_ptr = create_proper_doc();
	if (!load_proper_from_ini_file(d_ptr, NULL, dsnfile))
	{
		raise_user_error(_T("-1"), _T("parse dsn file failed"));
	}

	read_proper(d_ptr, _T("MYSQL"), -1, DSN_SERVER, -1, srv_buf, srv_len);
	read_proper(d_ptr, _T("MYSQL"), -1, DSN_DATABASE, -1, dbn_buf, dbn_len);
	read_proper(d_ptr, _T("MYSQL"), -1, DSN_UID, -1, uid_buf, uid_len);
	read_proper(d_ptr, _T("MYSQL"), -1, DSN_PWD, -1, pwd_buf, pwd_len);

	destroy_proper_doc(d_ptr);
	d_ptr = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_ptr)
		destroy_proper_doc(d_ptr);

	return 0;
}

xdb_t STDCALL db_open_dsn(const tchar_t* dsnfile)
{
	tchar_t drv[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t srv[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t dbn[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t uid[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t pwd[MAX_SQL_TOKEN + 1] = { 0 };

	if (!db_parse_dsn(dsnfile, srv, MAX_SQL_TOKEN, dbn, MAX_SQL_TOKEN, uid, MAX_SQL_TOKEN, pwd, MAX_SQL_TOKEN))
		return NULL;

	return db_open(srv, dbn, uid, pwd);
}

xdb_t STDCALL db_open(const tchar_t* srv, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd)
{
	db_t* pdb = NULL;
	MYSQL *ctx = NULL;

	char sdrv[MAX_SQL_TOKEN + 1] = { 0 };
	char ssrv[MAX_SQL_TOKEN + 1] = { 0 };
	char sdbn[MAX_SQL_TOKEN + 1] = { 0 };
	char suid[MAX_SQL_TOKEN + 1] = { 0 };
	char spwd[MAX_SQL_TOKEN + 1] = { 0 };

	TRY_CATCH;

#ifdef _UNICODE
    ucs_to_utf8(srv, -1, (byte_t*)ssrv, MAX_SQL_TOKEN);
    ucs_to_utf8(dbn, -1, (byte_t*)sdbn, MAX_SQL_TOKEN);
    ucs_to_utf8(uid, -1, (byte_t*)suid, MAX_SQL_TOKEN);
    ucs_to_utf8(pwd, -1, (byte_t*)spwd, MAX_SQL_TOKEN);
#else
    mbs_to_utf8(srv, -1, (byte_t*)ssrv, MAX_SQL_TOKEN);
    mbs_to_utf8(dbn, -1, (byte_t*)sdbn, MAX_SQL_TOKEN);
    mbs_to_utf8(uid, -1, (byte_t*)suid, MAX_SQL_TOKEN);
    mbs_to_utf8(pwd, -1, (byte_t*)spwd, MAX_SQL_TOKEN);
#endif
    
	ctx = mysql_init(NULL);
	if (!ctx)
	{
		raise_user_error(_T("-1"), _T("Alloc context handle failed"));
	}

	if (!mysql_real_connect(ctx, ssrv, suid, spwd, sdbn, 0, NULL, 0))
	{
        _raise_ctx_error(ctx);
	}
    
	mysql_set_character_set(ctx, "utf8");

	pdb = (db_t*)xmem_alloc(sizeof(db_t));
	pdb->head.dbt = _DB_MYSQL;
	pdb->head.cbs = sizeof(db_t);

	pdb->ctx = ctx;

	END_CATCH;

	return (xdb_t)pdb;

ONERROR:

	if (ctx)
		mysql_close(ctx);

	return NULL;
}

void STDCALL db_close(xdb_t db)
{
    db_t* pdb = (db_t*)db;
	
	XDL_ASSERT(pdb != NULL);

	if (pdb->stm)
	{
        mysql_stmt_close(pdb->stm);
		pdb->stm = NULL;
	}

    mysql_close(pdb->ctx);
	
	xmem_free(pdb);
}


bool_t STDCALL db_datetime(xdb_t db, int diff, tchar_t* sz_time)
{
	db_t* pdb = (db_t*)db;

	MYSQL_STMT *stm = NULL;
    MYSQL_BIND bind[1] = {0};
    unsigned long len = 0;
	int rt = 0;
    
    char sqlstr[MID_SQL_LEN] = {0};
	int n;

	if (!diff)
    {
		a_xscpy(sqlstr, "select date_format(now(),'%Y-%m-%d %H:%i:%s') as DT");
    }
	else
    {
		a_xsprintf(sqlstr, "select date_format(date_add(now(),interval %d day),%s) as DT", diff,"'%Y-%m-%d %H:%i:%s'");
    }

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

    stm = mysql_stmt_init(pdb->ctx);
	if (!stm)
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	if(C_OK != mysql_stmt_prepare(stm, sqlstr, a_xslen(sqlstr)))
	{
		_raise_stm_error(stm);
	}
    
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = xmem_alloc(DATE_LEN + 1);
    bind[0].buffer_length = DATE_LEN;
    bind[0].length = &len;

	if (C_OK != mysql_stmt_bind_result(stm, bind))
	{
		_raise_stm_error(stm);
	}

	if(C_OK != mysql_stmt_execute(stm))
    {
        _raise_stm_error(stm);
    }

    mysql_stmt_store_result(stm);
    
    rt = mysql_stmt_fetch(stm);
    switch(rt)
    {
        case 0:
        case MYSQL_NO_DATA:
        case MYSQL_DATA_TRUNCATED:
            break;
        default:
            _raise_stm_error(stm);
    }
    
#ifdef _UNICODE
    n = utf8_to_ucs((byte_t*)bind[0].buffer, len, sz_time, DATE_LEN);
#else
    n = utf8_to_mbs((byte_t*)bind[0].buffer, len, sz_time, DATE_LEN);
#endif
	sz_time[n] = _T('\0');

    xmem_free(bind[0].buffer);
    
	pdb->rows = 1;
    
    mysql_stmt_close(stm);
    stm = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
		mysql_stmt_close(stm);
    
    if(bind[0].buffer)
        xmem_free(bind[0].buffer);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_exec(xdb_t db, const tchar_t* sqlstr, int sqllen)
{
	db_t* pdb = (db_t*)db;

	MYSQL_STMT *stm = NULL;
    my_ulonglong row;
    int rows;
    char* d_sql = NULL;
    int d_len;
    
	tchar_t *tkpre, *tkcur;
	int tklen, total;
	bool_t uni;

	if (sqllen < 0)
		sqllen = xslen(sqlstr);

	if (!sqllen)
	{
		return 1;
	}

	TRY_CATCH;

	XDL_ASSERT(pdb != NULL);
	
	_db_reset(pdb);

    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
    }

	_db_tran(pdb);

	rows = 0;
	total = 0;
	tkcur = (tchar_t*)sqlstr;
	while (*tkcur != _T('\0'))
	{
		tklen = split_line(tkcur, sqllen);

		tkpre = tkcur;
		tkcur += tklen;
		sqllen -= tklen;
		uni = 0;

		while (*tkcur == _T(' ') || *tkcur == _T('\t') || *tkcur == _T('\n') || *tkcur == _T('\r'))
		{
			if (*tkcur == _T('\r'))
				uni = 1;

			tkcur++;
			sqllen--;
		}

		if (!tklen)
		{
			continue;
		}
        
#ifdef _UNICODE
        d_len = ucs_to_utf8(tkpre, tklen, NULL, MAX_LONG);
#else
        d_len = mbs_to_utf8(tkpre, tklen, NULL, MAX_LONG);
#endif
        
        d_sql = (char*)xmem_alloc(d_len + 1);
        
#ifdef _UNICODE
        d_len = ucs_to_utf8(tkpre, tklen, d_sql, d_len);
#else
        d_len = mbs_to_utf8(tkpre, tklen, d_sql, d_len);
#endif
        
        if(C_OK != mysql_stmt_prepare(stm, d_sql, d_len))
        {
            _raise_stm_error(stm);
        }

        if(C_OK != mysql_stmt_execute(stm))
        {
            _raise_stm_error(stm);
        }
        
        xmem_free(d_sql);
        d_sql = NULL;
        
        mysql_stmt_store_result(stm);

        row = mysql_stmt_affected_rows(stm);

		if (uni && row != 1)
		{
			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}
        
        mysql_stmt_free_result(stm);
        
        mysql_stmt_reset(stm);

		rows += (int)row;
	}

	_db_commit(pdb);

    mysql_stmt_close(stm);
    stm = NULL;

	pdb->rows = rows;

	END_CATCH;

	return 1;

ONERROR:

    if(d_sql)
        xmem_free(d_sql);
    
	if (stm)
	{
		_db_rollback(pdb);

        mysql_stmt_close(stm);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_update(xdb_t db, LINKPTR grid)
{
	db_t* pdb = (db_t*)db;
	
    MYSQL_STMT *stm = NULL;
    my_ulonglong row;
    int rows;
    
	tchar_t *sqlstr;
	int sqllen;
    
    char* d_sql = NULL;
    int d_len;

	LINKPTR rlk;
    dword_t rs;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
    }
	
	_db_tran(pdb);

	rows = 0;
	rlk = get_next_row(grid,LINK_FIRST);
	while(rlk)
	{
		sqllen = 0;
		rs = get_row_state(rlk);
		if(rs == dsDelete)
		{
			sqllen = format_row_delete_sql(grid, rlk, NULL, MAX_LONG);
			if(sqllen > 0)
			{
				sqlstr = (tchar_t*)xmem_alloc((sqllen + 1) * sizeof(tchar_t));
				format_row_delete_sql(grid,rlk,sqlstr,sqllen);
			}
		}else if(rs == dsNewDirty)
		{
			sqllen = format_row_insert_sql(grid, rlk, NULL, MAX_LONG);
			if(sqllen > 0)
			{
				sqlstr = (tchar_t*)xmem_alloc((sqllen + 1) * sizeof(tchar_t));
				format_row_insert_sql(grid,rlk,sqlstr,sqllen);
			}
		}else if(rs == dsDirty)
		{
			sqllen = format_row_update_sql(grid, rlk, NULL, MAX_LONG);
			if(sqllen > 0)
			{
				sqlstr = (tchar_t*)xmem_alloc((sqllen + 1) * sizeof(tchar_t));
				format_row_update_sql(grid,rlk,sqlstr,sqllen);
			}
		}
		else
		{
			sqllen = 0;
		}

		if (!sqllen)
		{
			rlk = get_next_row(grid, rlk);
			continue;
		}

#ifdef _UNICODE
        d_len = ucs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#else
        d_len = mbs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#endif
        
        d_sql = (char*)xmem_alloc(d_len + 1);
        
#ifdef _UNICODE
        d_len = ucs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#else
        d_len = mbs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#endif
        
        xmem_free(sqlstr);
        sqlstr = NULL;
        
        if(C_OK != mysql_stmt_prepare(stm, d_sql, d_len))
        {
            _raise_stm_error(stm);
        }
        
        if(C_OK != mysql_stmt_execute(stm))
        {
            _raise_stm_error(stm);
        }

		xmem_free(d_sql);
		d_sql = NULL;

        row = mysql_stmt_affected_rows(stm);
        
        mysql_stmt_free_result(stm);
        
        mysql_stmt_reset(stm);
        
		if (row != 1)
		{
			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}
		else
		{
			rows += (int)row;
		}
        
		rlk = get_next_row(grid,rlk);
	}
	
	_db_commit(pdb);

    mysql_stmt_close(stm);
    stm = NULL;

	pdb->rows = rows;

	END_CATCH;

	return 1;

ONERROR:

	if (sqlstr)
		xmem_free(sqlstr);
    
    if(d_sql)
        xmem_free(d_sql);

	if (stm)
	{
		_db_rollback(pdb);

		mysql_stmt_close(stm);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

int STDCALL _db_fetch_row(db_t* pdb, MYSQL_STMT* stm, LINKPTR grid)
{
	LINKPTR clk,rlk;
	
	tchar_t colname[MAX_SQL_NAME] = { 0 };
	const tchar_t* data_type;
	const tchar_t* data_cast;
	int len, rows = 0;
	xdate_t dt = { 0 };

    MYSQL_RES  *meta = NULL;
    MYSQL_FIELD *field = NULL;
    MYSQL_BIND *bind = NULL;
    my_bool* pnull = NULL;
    my_bool* perr = NULL;
    unsigned long* plen = NULL;
    char** pbuf = NULL;

    int rt = 0;
    unsigned long i, cols;
    
    tchar_t* d_str = NULL;
    int d_len;

    meta = mysql_stmt_result_metadata(stm);
    if(!meta)
    {
        return XDB_FAILED;
    }
    
    cols = mysql_num_fields(meta);

	bind = (MYSQL_BIND*)xmem_alloc(cols * sizeof(MYSQL_BIND));
    
    pnull = (my_bool*)xmem_alloc(cols * sizeof(my_bool));
    perr = (my_bool*)xmem_alloc(cols * sizeof(my_bool));
    plen = (unsigned long*)xmem_alloc(cols * sizeof(unsigned long));
    pbuf = (char**)xmem_alloc(cols * sizeof(char*));
    
	for (i = 0; i < cols; i++)
	{
        field = mysql_fetch_field_direct(meta, i);

#ifdef _UNICODE
		len = utf8_to_ucs(field->name, field->name_length, colname, MAX_SQL_NAME);
#else
		len = utf8_to_mbs(field->name, field->name_length, colname, MAX_SQL_NAME);
#endif
		colname[len] = _T('\0');

		clk = get_col(grid, colname);
		if (!clk)
		{
			continue;
		}

		data_type = get_col_data_type_ptr(clk);
		if (compare_text(data_type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
		{
			plen[i] = get_col_data_len(clk);
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			plen[i] = 2;
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			plen[i] = NUM_LEN;
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			plen[i] = DATE_LEN;
		}
		else
		{
            plen[i] = (field->length)? sqltolen(field->type, field->length) : MAX_SQL_NAME;
		}
        
        if(!plen[i])
            plen[i] = MAX_SQL_DATA;
        
        pbuf[i] = (char *)xmem_alloc(plen[i] + 1);

        bind[i].buffer_type= MYSQL_TYPE_STRING;
        bind[i].buffer_length = plen[i];
        bind[i].buffer= pbuf[i];
        bind[i].length= &plen[i];
        bind[i].is_null= &pnull[i];
        bind[i].error= &perr[i];
	}
    
    mysql_stmt_bind_result(stm, bind);
  
    rows = 0;
	rt = 0;
	while (!rt)
	{
        rt = mysql_stmt_fetch(stm);
		if (rt != C_OK)
			break;

		rlk = insert_row(grid, LINK_LAST);
		set_row_state(rlk, dsClean);
		rows++;

		for (i = 0; i < cols; i++)
		{
			field = mysql_fetch_field_direct(meta, i);
#ifdef _UNICODE
			len = utf8_to_ucs(field->name, field->name_length, colname, MAX_SQL_NAME);
#else
			len = utf8_to_mbs(field->name, field->name_length, colname, MAX_SQL_NAME);
#endif
			colname[len] = _T('\0');

			clk = get_col(grid, colname);
			if (clk == NULL)
				continue;

			if (!pnull[i] && plen[i])
			{                
#ifdef _UNICODE
                d_len = utf8_to_ucs((byte_t*)pbuf[i], plen[i], NULL, MAX_LONG);
                d_str = xsalloc(d_len + 1);
                d_len = utf8_to_ucs((byte_t*)pbuf[i], plen[i], d_str, d_len);
                
#else
                d_len = utf8_to_mbs((byte_t*)pbuf[i], plen[i], NULL, MAX_LONG);
                d_str = xsalloc(d_len + 1);
                d_len = utf8_to_mbs((byte_t*)pbuf[i], plen[i], d_str, d_len);
#endif

				data_type = get_col_data_type_ptr(clk);
                
				if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
				{
					data_cast = get_col_field_cast_ptr(clk);
					if (!is_null(data_cast))
					{
						parse_datetime_ex(&dt, data_cast, d_str);
						format_datetime(&dt, d_str);
					}
				}
                
                set_cell_text(rlk, clk, d_str, d_len);
                
                xsfree(d_str);
                d_str = NULL;
			}
		}
	}

	for (i = 0; i < cols; i++)
	{
		xmem_free(pbuf[i]);
	}
    
    xmem_free(pbuf);
	xmem_free(pnull);
	xmem_free(perr);
    xmem_free(plen);
    
    xmem_free(bind);
    
    mysql_free_result(meta);

	if(rt == MYSQL_NO_DATA)
		return XDB_SUCCEED;
	else
		return XDB_FAILED;
}

bool_t STDCALL db_fetch(xdb_t db, LINKPTR grid)
{
	db_t* pdb = (db_t*)db;
	
	MYSQL_STMT *stm = NULL;
	int rt;

	tchar_t* sqlstr = NULL;
	int sqllen;
    
    char* d_sql = NULL;
    int d_len;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	clear_grid_rowset(grid);

	sqllen = format_grid_select_sql(grid, NULL, MAX_LONG);
	if (sqllen <= 0)
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	sqlstr = (tchar_t*)xmem_alloc((sqllen + 1) * sizeof(tchar_t));
	format_grid_select_sql(grid,sqlstr,sqllen);

    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
    }
    
#ifdef _UNICODE
    d_len = ucs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#else
    d_len = mbs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#endif
    
    d_sql = (char*)xmem_alloc(d_len + 1);
    
#ifdef _UNICODE
    d_len = ucs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#else
    d_len = mbs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#endif
    
    xmem_free(sqlstr);
    sqlstr = NULL;
    
    if(C_OK != mysql_stmt_prepare(stm, d_sql, sqllen))
    {
        _raise_stm_error(stm);
    }
    
    xmem_free(d_sql);
    d_sql = NULL;
    
    if(C_OK != mysql_stmt_execute(stm))
    {
        _raise_stm_error(stm);
    }
    
    mysql_stmt_store_result(stm);
	
	rt = XDB_PENDING;
	while (rt == XDB_PENDING)
	{
		rt = _db_fetch_row(pdb, stm, grid);
	}

	if (rt == XDB_FAILED)
	{
		_raise_stm_error(stm);
	}

    mysql_stmt_close(stm);
    stm = NULL;

	pdb->rows = get_row_count(grid);

	END_CATCH;

	return 1;

ONERROR:
	if (sqlstr)
		xmem_free(sqlstr);
    
    if(d_sql)
        xmem_free(d_sql);

    if(stm)
        mysql_stmt_close(stm);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_select(xdb_t db, LINKPTR grid, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;
    
	LINKPTR clk;
    int sqllen;
	int i,len,cols = 0;
	tchar_t coltype[MAX_SQL_NAME] = { 0 };
	tchar_t colname[MAX_SQL_NAME] = { 0 };

    MYSQL_STMT *stm = NULL;
	int rt;
    MYSQL_RES  *meta = NULL;
    MYSQL_FIELD *field = NULL;
    
    char* d_sql = NULL;
    int d_len;
    
	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;
	
	_db_reset(pdb);

	clear_grid_rowset(grid);
	clear_grid_colset(grid);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}
    sqllen = xslen(sqlstr);
    
    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
    }
    
#ifdef _UNICODE
    d_len = ucs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#else
    d_len = mbs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#endif
    
    d_sql = (char*)xmem_alloc(d_len + 1);
    
#ifdef _UNICODE
    d_len = ucs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#else
    d_len = mbs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#endif
    
    if(C_OK != mysql_stmt_prepare(stm, d_sql, d_len))
    {
        _raise_stm_error(stm);
    }
    
    xmem_free(d_sql);
    d_sql = NULL;
    
    if(C_OK != mysql_stmt_execute(stm))
    {
        _raise_stm_error(stm);
    }
    
    mysql_stmt_store_result(stm);
    
    meta = mysql_stmt_result_metadata(stm);
    if(!meta)
    {
        _raise_stm_error(stm);
    }
    
    cols = mysql_num_fields(meta);
    
	for (i = 0; i < cols; i++)
	{
        field = mysql_fetch_field_direct(meta, i);

		clk = insert_col(grid, LINK_LAST);

#ifdef _UNICODE
		len = utf8_to_ucs(field->name, field->name_length, colname, MAX_SQL_NAME);
#else
		len = utf8_to_mbs(field->name, field->name_length, colname, MAX_SQL_NAME);
#endif
		colname[len] = _T('\0');
        set_col_name(clk, colname);
        set_col_title(clk, colname);

#ifdef _UNICODE
		len = utf8_to_ucs(field->org_name, field->org_name_length, colname, MAX_SQL_NAME);
#else
		len = utf8_to_mbs(field->org_name, field->org_name_length, colname, MAX_SQL_NAME);
#endif
		colname[len] = _T('\0');
        set_col_field_name(clk, colname);

#ifdef _UNICODE
		len = utf8_to_ucs(field->org_table, field->org_table_length, colname, MAX_SQL_NAME);
#else
		len =  utf8_to_mbs(field->org_table, field->org_table_length, colname, MAX_SQL_NAME);
#endif 
		colname[len] = _T('\0');
        set_col_table_name(clk, colname);
        
        if(IS_PRI_KEY(field->flags))
            set_col_field_type(clk, ATTR_FIELD_TYPE_PRIM);
        else
            set_col_field_type(clk, ATTR_FIELD_TYPE_DATA);

        if(IS_NOT_NULL(field->flags))
            set_col_nullable(clk, 0);
        else
            set_col_nullable(clk, 1);

        sqltodt(field->type, coltype);
        set_col_data_type(clk, coltype);
        
		set_col_data_len(clk, sqltolen(field->type, field->length));

		if (field->decimals)
            set_col_data_dig(clk, field->decimals);

	}

	rt = XDB_PENDING;
	while (rt == XDB_PENDING)
	{
		rt = _db_fetch_row(pdb, stm, grid);
	}

	if (rt == XDB_FAILED)
	{
		_raise_stm_error(stm);
	}

    mysql_free_result(meta);
    meta = NULL;
    
    mysql_stmt_close(stm);
    stm = NULL;

	pdb->rows = get_row_count(grid);

	END_CATCH;

	return 1;

ONERROR:
    
    if(d_sql)
        xmem_free(d_sql);

    if(meta)
        mysql_free_result(meta);
    
	if (stm)
		mysql_stmt_close(stm);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_schema(xdb_t db, LINKPTR grid, const tchar_t* sqlstr)
{
    db_t* pdb = (db_t*)db;
    
    LINKPTR clk;
    int sqllen;
    int i,len,cols = 0;
    tchar_t coltype[MAX_SQL_NAME] = { 0 };
    tchar_t colname[MAX_SQL_NAME] = { 0 };
    
    MYSQL_STMT *stm = NULL;
    MYSQL_RES  *meta = NULL;
    MYSQL_FIELD *field = NULL;

	char* d_sql = NULL;
	int d_len;
    
    XDL_ASSERT(pdb != NULL);
    
    TRY_CATCH;
    
    _db_reset(pdb);
    
    clear_grid_rowset(grid);
    clear_grid_colset(grid);
    
    if (is_null(sqlstr))
    {
        raise_user_error(_T("-1"), _T("Empty sql statement"));
    }
    sqllen = xslen(sqlstr);
    
    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
    }
    
#ifdef _UNICODE
	d_len = ucs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#else
	d_len = mbs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#endif
	d_sql = (char*)xmem_alloc(d_len + 1);
#ifdef _UNICODE
	d_len = ucs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#else
	d_len = mbs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#endif

    if(C_OK != mysql_stmt_prepare(stm, d_sql, d_len))
    {
        _raise_stm_error(stm);
    }
    
	xmem_free(d_sql);
	d_sql = NULL;

    if(C_OK != mysql_stmt_execute(stm))
    {
        _raise_stm_error(stm);
    }
    
    mysql_stmt_store_result(stm);
    
    meta = mysql_stmt_result_metadata(stm);
    if(!meta)
    {
        _raise_stm_error(stm);
    }
    
    cols = mysql_num_fields(meta);
    
    for (i = 0; i < cols; i++)
    {
        field = mysql_fetch_field_direct(meta, i);
        
        clk = insert_col(grid, LINK_LAST);
        
#ifdef _UNICODE
		len = utf8_to_ucs(field->name, field->name_length, colname, MAX_SQL_NAME);
#else
		len = utf8_to_mbs(field->name, field->name_length, colname, MAX_SQL_NAME);
#endif
		colname[len] = _T('\0');
		set_col_name(clk, colname);
		set_col_title(clk, colname);

#ifdef _UNICODE
		len = utf8_to_ucs(field->org_name, field->org_name_length, colname, MAX_SQL_NAME);
#else
		len = utf8_to_mbs(field->org_name, field->org_name_length, colname, MAX_SQL_NAME);
#endif
		colname[len] = _T('\0');
		set_col_field_name(clk, colname);

#ifdef _UNICODE
		len = utf8_to_ucs(field->org_table, field->org_table_length, colname, MAX_SQL_NAME);
#else
		len = utf8_to_mbs(field->org_table, field->org_table_length, colname, MAX_SQL_NAME);
#endif 
		colname[len] = _T('\0');
		set_col_table_name(clk, colname);
        
        if(IS_PRI_KEY(field->flags))
            set_col_field_type(clk, ATTR_FIELD_TYPE_PRIM);
        else
            set_col_field_type(clk, ATTR_FIELD_TYPE_DATA);
        
        if(IS_NOT_NULL(field->flags))
            set_col_nullable(clk, 0);
        else
            set_col_nullable(clk, 1);
        
        sqltodt(field->type, coltype);
        set_col_data_type(clk, coltype);
        
        set_col_data_len(clk, sqltolen(field->type, field->length));
        
        if (field->decimals)
            set_col_data_dig(clk, field->decimals);
        
    }
    
    mysql_free_result(meta);
    meta = NULL;
    
    mysql_stmt_close(stm);
    stm = NULL;
    
    pdb->rows = get_row_count(grid);
    
    END_CATCH;
    
    return 1;
    
ONERROR:
    
	if (d_sql)
		xmem_free(d_sql);

    if(meta)
        mysql_free_result(meta);

    if (stm)
        mysql_stmt_close(stm);
    
    get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);
    
    return 0;
}

int _db_call_argv(db_t* pdb, const tchar_t* procname, const tchar_t* fmt, va_list* parg)
{
    MYSQL_STMT *stm = NULL;
    MYSQL_BIND *bind = NULL;
    my_bool* pnull = NULL;
    my_bool* perr = NULL;
    unsigned long* plen = NULL;
    char** pbuf = NULL;
    int* pinout = NULL;
    int rt = 0;
    int i, ind;
    
    tchar_t* token;
    tchar_t* ptr_str;
    int* ptr_int;
    double* ptr_double;

	char* d_sql = NULL;
	int d_len;
    
    TRY_CATCH;
    
    pdb->rows = 0;
    
    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
        
    }
    
    ind = 0;
    token = (tchar_t*)fmt;
    while (token && *token)
    {
        if (*token == '%')
            ind++;
        
        token++;
    }
    
#ifdef _UNICODE
	d_len = ucs_to_utf8(procname, -1, NULL, MAX_LONG);
#else
	d_len = mbs_to_utf8(procname, -1, NULL, MAX_LONG);
#endif
    d_len += a_xslen("call ()") + ind * 2;
    d_sql = (char*)xmem_alloc(d_len + 1);
    
    a_xsprintf(d_sql, "call %S(", procname);

    for (i = 0; i < ind; i++)
    {
        a_xscat(d_sql, "?,");
    }

	d_len = a_xslen(d_sql);
	if (d_sql[d_len - 1] == '(')
		d_len++;

	d_sql[d_len - 1] = ')';
	d_sql[d_len] = '\0';

    if(C_OK != mysql_stmt_prepare(stm, d_sql, d_len))
    {
        _raise_stm_error(stm);
    }
    
	xmem_free(d_sql);
	d_sql = NULL;

    bind = (MYSQL_BIND*)xmem_alloc((ind) * sizeof(MYSQL_BIND));
    pnull = (my_bool*)xmem_alloc((ind) * sizeof(my_bool));
    perr = (my_bool*)xmem_alloc((ind) * sizeof(my_bool));
    plen = (unsigned long*)xmem_alloc((ind) * sizeof(unsigned long));
    pbuf = (char**)xmem_alloc((ind) * sizeof(char*));
    pinout = (int*)xmem_alloc((ind) * sizeof(int));

    ind = 0;
    token = (tchar_t*)fmt;
    while (token && *token)
    {
        while (*token && *token != _T('%'))
            token++;
        
        if (!*token)
            break;
        
        if (*token == _T('%'))
            token++;
        
        if (*token == _T('-'))
        {
            token++;
            pinout[ind] = MYSQL_PARAM_OUTPUT;
        }
        else if (*token == _T('+'))
        {
            token++;
            pinout[ind] = MYSQL_PARAM_INPUT_OUTPUT;
        }
        else
        {
            pinout[ind] = MYSQL_PARAM_INPUT;
        }
        
        if (*token >= _T('0') && *token <= _T('9'))
            plen[ind] = xstol(token);
        
        while (*token >= _T('0') && *token <= _T('9'))
            token++;
        
        if (*token == _T('.'))
        {
            token++;
            while (*token >= _T('0') && *token <= _T('9'))
                token++;
        }
        
        rt = 0;
        switch (*token)
        {
            case _T('s'):
                ptr_str = va_arg(*parg, tchar_t*);
				if (!plen[ind])
				{
#ifdef _UNICODE
					plen[ind] = ucs_to_utf8(ptr_str, -1, NULL, MAX_LONG);
#else
					plen[ind] = ucs_to_mbs(ptr_str, -1, NULL, MAX_LONG);
#endif
				}

                pbuf[ind] = xmem_alloc(plen[ind] + 1);
                
                if (pinout[ind] == MYSQL_PARAM_INPUT_OUTPUT || pinout[ind] == MYSQL_PARAM_INPUT)
                {
#ifdef _UNICODE
					ucs_to_utf8(ptr_str, -1, pbuf[ind], plen[ind]);
#else
					mbs_to_utf8(ptr_str, -1, pbuf[ind], plen[ind]);
#endif
                }
                bind[ind].buffer_type= MYSQL_TYPE_STRING;
                break;
            case _T('d'):
                ptr_int = va_arg(*parg, int*);
                plen[ind] = sizeof(int);
                pbuf[ind] = xmem_alloc(plen[ind]);
                
                if (pinout[ind] == MYSQL_PARAM_INPUT_OUTPUT || pinout[ind] == MYSQL_PARAM_INPUT)
                {
                    xmem_copy(pbuf[ind], ptr_int, sizeof(int));
                }
                bind[ind].buffer_type= MYSQL_TYPE_LONG;
                break;
            case _T('f'):
                ptr_double = va_arg(*parg, double*);
                plen[ind] = sizeof(double);
                pbuf[ind] = xmem_alloc(plen[ind]);
                
                if (pinout[ind] == MYSQL_PARAM_INPUT_OUTPUT || pinout[ind] == MYSQL_PARAM_INPUT)
                {
                    xmem_copy(pbuf[ind], ptr_double, sizeof(double));
                }
                bind[ind].buffer_type= MYSQL_TYPE_DOUBLE;
                break;
        }
        
        bind[ind].buffer= pbuf[ind];
        bind[i].buffer_length = plen[i];
        bind[ind].length= &plen[ind];
        bind[ind].is_null= &pnull[ind];
        bind[ind].error= &perr[ind];
        ind ++;
    }
    
    if (C_OK != mysql_stmt_bind_param(stm, bind))
    {
        _raise_stm_error(stm);
    }
    
    if(C_OK != mysql_stmt_execute(stm))
    {
        _raise_stm_error(stm);
    }
    
    mysql_stmt_store_result(stm);
    
    xmem_free(d_sql);
    d_sql = NULL;
    
    for (i = 0; i < ind; i++)
    {
        xmem_free(pbuf[i]);
    }
    
    xmem_free(pbuf);
    xmem_free(pnull);
    xmem_free(perr);
    xmem_free(plen);
    xmem_free(pinout);
    
    xmem_free(bind);
    
    mysql_stmt_close(stm);
    stm = NULL;
    
    END_CATCH;
    
    return 0;
    
ONERROR:
    
    if (d_sql)
        xmem_free(d_sql);
    
    if(pbuf)
    {
        for (i = 0; i < ind; i++)
        {
            xmem_free(pbuf[i]);
        }
        xmem_free(pbuf);
    }
    
    if(pnull)
        xmem_free(pnull);
    if(perr)
        xmem_free(perr);
    if(plen)
        xmem_free(plen);
    if(pinout)
        xmem_free(pinout);
    
    if(bind)
        xmem_free(bind);
    
    if (pdb->stm)
        mysql_stmt_close(stm);
    
    get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);
    
    return -1;
}

int db_call_argv(xdb_t db, const tchar_t* procname, const tchar_t* fmt, ...)
{
    db_t* pdb = (db_t*)db;
    
    va_list arg;
    int rt;
    
    va_start(arg,fmt);
    rt = _db_call_argv(pdb,procname,fmt,&arg);
    va_end(arg);
    
    return rt;
}

bool_t STDCALL db_call_func(xdb_t db, LINKPTR func)
{
    db_t* pdb = (db_t*)db;
    
    MYSQL_STMT *stm = NULL;
    MYSQL_BIND *bind = NULL;
    my_bool* pnull = NULL;
    my_bool* perr = NULL;
    unsigned long* plen = NULL;
    char** pbuf = NULL;
    int* pinout = NULL;
    int rt = 0;
    int i, ind;
    
    char* d_sql = NULL;
	int d_len;
    
    LINKPTR flk;
    
    XDL_ASSERT(db && db->dbt == _DB_ODBC);
    
    TRY_CATCH;
    
    _db_reset(pdb);
    
    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
        
    }
    
    ind = get_func_param_count(func);
    
    bind = (MYSQL_BIND*)xmem_alloc((ind) * sizeof(MYSQL_BIND));
    pnull = (my_bool*)xmem_alloc((ind) * sizeof(my_bool));
    perr = (my_bool*)xmem_alloc((ind) * sizeof(my_bool));
    plen = (unsigned long*)xmem_alloc((ind) * sizeof(unsigned long));
    pbuf = (char**)xmem_alloc((ind) * sizeof(char*));
    pinout = (int*)xmem_alloc((ind) * sizeof(int));
    
    ind = 0;
    flk = get_func_next_param(func, LINK_FIRST);
    while (flk)
    {
        if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_OUTPUT, -1, 0) == 0)
        {
            pinout[ind] = MYSQL_PARAM_OUTPUT;
        }
        else if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_INPUTOUTPUT, -1, 0) == 0)
        {
            pinout[ind] = MYSQL_PARAM_INPUT_OUTPUT;
        }
        else
        {
            pinout[ind] = MYSQL_PARAM_INPUT;
        }
        
        plen[ind] = get_func_param_data_len(flk);
        
        rt = 0;
        if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
        {
            plen[ind] = sizeof(int);
            pbuf[ind] = xmem_alloc(plen[ind]);
            if (pinout[ind] == MYSQL_PARAM_INPUT_OUTPUT || pinout[ind] == MYSQL_PARAM_INPUT)
            {
                *(int*)(pbuf[ind]) = get_func_param_integer(flk);
            }
        }
        else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
        {
            plen[ind] = sizeof(double);
            pbuf[ind] = xmem_alloc(plen[ind]);
            if (pinout[ind] == MYSQL_PARAM_INPUT_OUTPUT || pinout[ind] == MYSQL_PARAM_INPUT)
            {
                *(double*)(pbuf[ind]) = get_func_param_numeric(flk);
            }
        }
        else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
        {
            plen[ind] = DATE_LEN;
            pbuf[ind] = xmem_alloc(plen[ind]);
           if (pinout[ind] == MYSQL_PARAM_INPUT_OUTPUT || pinout[ind] == MYSQL_PARAM_INPUT)
            {
#ifdef _UNICODE
				ucs_to_utf8(get_func_param_text_ptr(flk), -1, pbuf[ind], plen[ind]);
#else
				mbs_to_utf8(get_func_param_text_ptr(flk), -1, pbuf[ind], plen[ind]);
#endif
            }
        }
        else
        {
			if (!plen[ind])
			{
#ifdef _UNICODE
				plen[ind] = ucs_to_utf8(get_func_param_text_ptr(flk), -1, NULL, MAX_LONG);
#else
				plen[ind] = mbs_to_utf8(get_func_param_text_ptr(flk), -1, NULL, MAX_LONG);
#endif
			}
            pbuf[ind] = xmem_alloc(plen[ind] + 1);
            if (pinout[ind] == MYSQL_PARAM_INPUT_OUTPUT || pinout[ind] == MYSQL_PARAM_INPUT)
            {
#ifdef _UNICODE
				ucs_to_utf8(get_func_param_text_ptr(flk), -1, pbuf[ind], plen[ind]);
#else
				mbs_to_utf8(get_func_param_text_ptr(flk), -1, pbuf[ind], plen[ind]);
#endif
            }
        }
        
        bind[ind].buffer= pbuf[ind];
        bind[i].buffer_length = plen[i];
        bind[ind].length= &plen[ind];
        bind[ind].is_null= &pnull[ind];
        bind[ind].error= &perr[ind];
        
        ind ++;
        flk = get_func_next_param(func, flk);
    }

#ifdef _UNICODE
	d_len = ucs_to_utf8(get_func_name_ptr(func), -1, NULL, MAX_LONG);
#else
	d_len = mbs_to_utf8(get_func_name_ptr(func), -1, NULL, MAX_LONG);
#endif
	d_len += a_xslen("call ()") + ind * 2;
	d_sql = (char*)xmem_alloc(d_len + 1);

	a_xsprintf(d_sql, "call %S(", get_func_name_ptr(func));

	for (i = 0; i < ind; i++)
	{
		a_xscat(d_sql, "?,");
	}

	d_len = a_xslen(d_sql);
	if (d_sql[d_len - 1] == '(')
		d_len++;
	
	d_sql[d_len - 1] = ')';
	d_sql[d_len] = '\0';
    
    if (C_OK != mysql_stmt_bind_param(stm, bind))
    {
        _raise_stm_error(stm);
    }
    
    if(C_OK != mysql_stmt_execute(stm))
    {
        _raise_stm_error(stm);
    }
    
    mysql_stmt_store_result(stm);
    
    xmem_free(d_sql);
    d_sql = NULL;
    
    mysql_stmt_close(stm);
    stm = NULL;
    
    ind = 0;
    flk = get_func_next_param(func, LINK_FIRST);
    while (flk)
    {
        if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_OUTPUT, -1, 0) == 0 || compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_INPUTOUTPUT, -1, 0) == 0)
        {
            if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
            {
                set_func_param_integer(flk, *(int*)(pbuf[ind]));
            }
            else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
            {
                set_func_param_numeric(flk, *(double*)(pbuf[ind]));
            }
            else
            {
                set_func_param_text(flk, (tchar_t*)(pbuf[ind]), -1);
            }
        }
        
        ind++;
        
        flk = get_func_next_param(func, flk);
    }
    
    for (i = 0; i < ind; i++)
    {
        xmem_free(pbuf[i]);
    }
    
    xmem_free(pbuf);
    xmem_free(pnull);
    xmem_free(perr);
    xmem_free(plen);
    
    xmem_free(bind);
    
    set_func_data_type(func, ATTR_DATA_TYPE_INTEGER);
    set_func_return_integer(func, 0);
    
    END_CATCH;
    
    return 1;
    
ONERROR:
    
    if (d_sql)
        xmem_free(d_sql);
    
    if(pbuf)
    {
        for (i = 0; i < ind; i++)
        {
            xmem_free(pbuf[i]);
        }
        xmem_free(pbuf);
    }
    
    if(pnull)
        xmem_free(pnull);
    if(perr)
        xmem_free(perr);
    if(plen)
        xmem_free(plen);
    
    if(bind)
        xmem_free(bind);
    
    if (pdb->stm)
        mysql_stmt_close(stm);
    
    set_func_data_type(func, ATTR_DATA_TYPE_INTEGER);
    set_func_return_integer(func, -1);
    
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////
bool_t STDCALL _db_prepare(db_t* pdb, const tchar_t* sqlstr)
{
    int sqllen;
	char* d_sql = NULL;
	int d_len;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}
	sqllen = xslen(sqlstr);

    pdb->stm = mysql_stmt_init(pdb->ctx);
    if (!pdb->stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
    }
    
#ifdef _UNICODE
	d_len = ucs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#else
	d_len = mbs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#endif
    
	d_sql = (char*)xmem_alloc(d_len + 1);
    
#ifdef _UNICODE
	d_len = ucs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#else
	d_len = mbs_to_utf8(sqlstr, sqllen, d_sql, d_len);
#endif
    
    if(C_OK != mysql_stmt_prepare(pdb->stm, d_sql, d_len))
    {
        _raise_stm_error(pdb->stm);
    }
    
	xmem_free(d_sql);
	d_sql = NULL;

    if(C_OK != mysql_stmt_execute(pdb->stm))
    {
        _raise_stm_error(pdb->stm);
    }
    
    mysql_stmt_store_result(pdb->stm);

	pdb->rows = (int)mysql_stmt_num_rows(pdb->stm);

	END_CATCH;

	return 1;

ONERROR:

	if (d_sql)
		xmem_free(d_sql);

	if (pdb->stm)
        mysql_stmt_close(pdb->stm);

    pdb->stm = NULL;
    
	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_export(xdb_t db, stream_t stream, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;
    
    MYSQL_RES  *meta = NULL;
    MYSQL_FIELD *field = NULL;
    
    MYSQL_BIND *bind = NULL;
    my_bool* pnull = NULL;
    my_bool* perr = NULL;
    unsigned long* plen = NULL;
    tchar_t** pbuf = NULL;
    int rt;

    int i, len, cols;
    tchar_t datatype[MAX_SQL_NAME] = { 0 };
    tchar_t colname[MAX_SQL_NAME] = { 0 };

	string_t vs = NULL;

	tchar_t feed[3] = { TXT_ITEMFEED, TXT_LINEFEED, _T('\n') };

	tchar_t* sz_esc = NULL;
	int len_esc = 0;
    dword_t pos;

    tchar_t* d_str = NULL;
    int d_len;
    
	XDL_ASSERT(pdb != NULL);

	if (!pdb->stm)
	{
		_db_prepare(pdb, sqlstr);
	}

	if (!stream)
	{
		return (pdb->stm) ? 1 : 0;
	}

	TRY_CATCH;

	if (!pdb->stm)
	{
		raise_user_error(_T("-1"), ERR_TEXT_INVALIDSTMT);
	}

    meta = mysql_stmt_result_metadata(pdb->stm);
    if(!meta)
    {
        _raise_stm_error(pdb->stm);
    }
    cols = mysql_num_fields(meta);

	stream_write_utfbom(stream, NULL);

	vs = string_alloc();

    bind = (MYSQL_BIND*)xmem_alloc((cols) * sizeof(MYSQL_BIND));
    pnull = (my_bool*)xmem_alloc((cols) * sizeof(my_bool));
    perr = (my_bool*)xmem_alloc((cols) * sizeof(my_bool));
    plen = (unsigned long*)xmem_alloc((cols) * sizeof(unsigned long));
    pbuf = (tchar_t**)xmem_alloc((cols) * sizeof(tchar_t*));

	for (i = 0; i < cols; i++)
	{
		field = mysql_fetch_field_direct(meta, i);

#ifdef _UNICODE
		len = utf8_to_ucs(field->name, field->name_length, colname, MAX_SQL_NAME);
#else
		len = utf8_to_mbs(field->name, field->name_length, colname, MAX_SQL_NAME);
#endif
		colname[len] = _T('\0');
        
        sqltodt(field->type, datatype);
        
        if (compare_text(datatype, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
        {
            plen[i] = sqltolen(field->type, field->length);
        }
        else if (compare_text(datatype, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
        {
            plen[i] = 2;
        }
        else if (compare_text(datatype, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0 || compare_text(datatype, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
        {
            plen[i] = NUM_LEN;
        }
        else if (compare_text(datatype, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(datatype, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
        {
            plen[i] = DATE_LEN;
        }
        
        if(!plen[i]) plen[i] = MAX_SQL_DATA;
        
        pbuf[i] = (tchar_t *)xmem_alloc(plen[i] + 1);
        
        bind[i].buffer_type= MYSQL_TYPE_STRING;
        bind[i].buffer_length = plen[i];
        bind[i].buffer= pbuf[i];
        bind[i].length= &plen[i];
        bind[i].is_null= &pnull[i];
        bind[i].error= &perr[i];
		
		string_cat(vs, colname, -1);
		string_cat(vs, feed, 1);
	}
	string_cat(vs, feed + 1, 2);

	if (!stream_write_line(stream, vs, &pos))
	{
		raise_user_error(NULL, NULL);
	}

	string_empty(vs);

    if (C_OK != mysql_stmt_bind_result(pdb->stm, bind))
    {
        _raise_stm_error(pdb->stm);
    }
    
    rt = 0;
    while (!rt)
    {
        rt = mysql_stmt_fetch(pdb->stm);
        if (rt != C_OK)
            break;

		pos = 0;
		for (i = 0; i < cols; i++)
		{
			if (!pnull[i] && plen[i])
			{
#ifdef _UNICODE
                d_len = utf8_to_ucs((byte_t*)pbuf[i],plen[i],NULL, MAX_LONG);
#else
                d_len = utf8_to_mbs((byte_t*)pbuf[i],plen[i],NULL, MAX_LONG);
#endif
                d_str = xsalloc(d_len + 1);
#ifdef _UNICODE
                d_len = utf8_to_ucs((byte_t*)pbuf[i],plen[i],d_str, d_len);
#else
                d_len = utf8_to_mbs((byte_t*)pbuf[i],plen[i],d_str, d_len);
#endif

				len_esc = csv_char_encode(d_str, d_len, NULL, MAX_LONG);
				if (len_esc != d_len)
				{
					sz_esc = xsalloc(len_esc + 1);
					csv_char_encode(d_str, d_len, sz_esc, len_esc);

					string_cat(vs, sz_esc, len_esc);
					xsfree(sz_esc);
				}
				else
				{
					string_cat(vs, d_str, d_len);
				}
                
                xsfree(d_str);
                d_str = NULL;
			}
			string_cat(vs, feed, 1);
		}

		string_cat(vs, feed + 1, 2);

		if (!stream_write_line(stream, vs, &pos))
		{
			raise_user_error(NULL, NULL);
		}

		string_empty(vs);
	}
	
	string_empty(vs);

	if (!stream_write_line(stream, vs, &pos))
	{
		raise_user_error(NULL, NULL);
	}

	string_free(vs);
	vs = NULL;

	if (!stream_flush(stream))
	{
		raise_user_error(NULL, NULL);
	}
    
    mysql_free_result(meta);
    meta = NULL;

	mysql_stmt_close(pdb->stm);
	pdb->stm = NULL;

    for (i = 0; i < cols; i++)
    {
        xmem_free(pbuf[i]);
    }
    
    xmem_free(pbuf);
    xmem_free(pnull);
    xmem_free(perr);
    xmem_free(plen);
    
    xmem_free(bind);

	END_CATCH;

	return 1;

ONERROR:

    if(d_str)
        xsfree(d_str);
    
	if (vs)
		string_free(vs);

    if(pbuf)
    {
        for (i = 0; i < cols; i++)
        {
            xmem_free(pbuf[i]);
        }
        xmem_free(pbuf);
    }
    
    if(pnull)
        xmem_free(pnull);
    if(perr)
        xmem_free(perr);
    if(plen)
        xmem_free(plen);
    
    if(bind)
        xmem_free(bind);
    
    if(meta)
        mysql_free_result(meta);
    
    if (pdb->stm)
        mysql_stmt_close(pdb->stm);
    pdb->stm = NULL;

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_import(xdb_t db, stream_t stream, const tchar_t* table)
{
	db_t* pdb = (db_t*)db;

    MYSQL_STMT *stm = NULL;
    MYSQL_BIND *bind = NULL;
    my_bool* pnull = NULL;
    my_bool* perr = NULL;
    unsigned long* plen = NULL;
    char** pbuf = NULL;
    int i, cols;

	string_t vs = NULL;
	string_t vs_sql = NULL;
	const tchar_t* token;
    const tchar_t* tkpre;
	int tklen;
	int rows;
	dword_t dw;

	tchar_t* sz_esc = NULL;
	int len_esc = 0;
    
    char* d_sql = NULL;
    int d_len;

	XDL_ASSERT(pdb != NULL);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

    stm = mysql_stmt_init(pdb->ctx);
    if (!stm)
    {
        raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
    }

	_db_tran(pdb);

	stream_read_utfbom(stream, NULL);

	vs = string_alloc();

	dw = 0;
	if (!stream_read_line(stream, vs, &dw))
	{
		raise_user_error(_T("-1"), _T("read stream failed"));
	}

	vs_sql = string_alloc();

	string_printf(vs_sql, _T("insert into %s ("), table);

	cols = 0;
	token = string_ptr(vs);
	while (*token != TXT_LINEFEED && *token != _T('\0'))
	{
		tklen = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			tklen++;
		}

		string_cat(vs_sql, token - tklen, tklen);
		string_cat(vs_sql, _T(","), 1);

		if (*token == TXT_ITEMFEED)
			token++;

		cols++;
	}

	if (!cols)
	{
		raise_user_error(_T("-1"), _T("empty sql statement"));
	}
    
	tklen = string_len(vs_sql);
	string_set_char(vs_sql, tklen - 1, _T(')'));

	string_cat(vs_sql, _T(" values ("), -1);

	for (i = 0; i < cols; i++)
	{
		string_cat(vs_sql, _T("?,"), 2);
	}

	tklen = string_len(vs_sql);
	string_set_char(vs_sql, tklen - 1, _T(')'));

#ifdef _UNICODE
    d_len = ucs_to_utf8(string_ptr(vs_sql),string_len(vs_sql),NULL, MAX_LONG);
#else
    d_len = mbs_to_utf8(string_ptr(vs_sql),string_len(vs_sql),NULL, MAX_LONG);
#endif
    d_sql = (char*)xmem_alloc(d_len + 1);
#ifdef _UNICODE
    d_len = ucs_to_utf8(string_ptr(vs_sql),string_len(vs_sql),d_sql, d_len);
#else
    d_len = mbs_to_utf8(string_ptr(vs_sql),string_len(vs_sql),d_sql, d_len);
#endif
    
    string_free(vs_sql);
    vs_sql = NULL;
    
    if(C_OK != mysql_stmt_prepare(stm, d_sql, d_len))
    {
        _raise_stm_error(stm);
    }

    xmem_free(d_sql);
    d_sql = NULL;
    
    cols = mysql_stmt_param_count(stm);
    
    bind = (MYSQL_BIND*)xmem_alloc((cols) * sizeof(MYSQL_BIND));
    pnull = (my_bool*)xmem_alloc((cols) * sizeof(my_bool));
    perr = (my_bool*)xmem_alloc((cols) * sizeof(my_bool));
    plen = (unsigned long*)xmem_alloc((cols) * sizeof(unsigned long));
    pbuf = (char**)xmem_alloc((cols) * sizeof(char*));

	rows = 0;
	string_empty(vs);

	while (1)
	{
		string_empty(vs);
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			raise_user_error(_T("-1"), _T("stream read line failed"));
		}

		if (string_len(vs) == 0)
			break;

		i = 0;
		token = string_ptr(vs);
		while (*token != TXT_LINEFEED && *token != _T('\0'))
		{
			tklen = 0;
			while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
			{
				token++;
				tklen++;
			}

            if(tklen)
            {
                tkpre = token - tklen;
                len_esc = csv_char_decode(tkpre, tklen, NULL, MAX_LONG);
                if (len_esc != tklen)
                {
                    sz_esc = xsalloc(len_esc + 1);
					csv_char_decode(tkpre, tklen, sz_esc, len_esc);
                    
#ifdef _UNICODE
                    plen[i] = ucs_to_utf8(sz_esc, len_esc, NULL, MAX_LONG);
#else
                    plen[i] = mbs_to_utf8(sz_esc, len_esc, NULL, MAX_LONG);
#endif
                    pbuf[i] = xmem_alloc(plen[i] + 1);
#ifdef _UNICODE
                    plen[i] = ucs_to_utf8(sz_esc, len_esc, pbuf[i], plen[i]);
#else
                    plen[i] = mbs_to_utf8(sz_esc, len_esc, pbuf[i], plen[i]);
#endif
                    
                }
                else
                {
#ifdef _UNICODE
                    plen[i] = ucs_to_utf8(tkpre, tklen, NULL, MAX_LONG);
#else
                    plen[i] = mbs_to_utf8(tkpre, tklen, NULL, MAX_LONG);
#endif
                    pbuf[i] = xmem_alloc(plen[i] + 1);
#ifdef _UNICODE
                    plen[i] = ucs_to_utf8(tkpre, tklen, pbuf[i], plen[i]);
#else
                    plen[i] = mbs_to_utf8(tkpre, tklen, pbuf[i], plen[i]);
#endif
                }
                
                bind[i].buffer_type= MYSQL_TYPE_STRING;
                bind[i].buffer_length = plen[i];
                bind[i].buffer= pbuf[i];
                bind[i].length= &plen[i];
                bind[i].is_null= &pnull[i];
                bind[i].error= &perr[i];
            }else
            {
                plen[i] = 0;
                pbuf[i] = NULL;
                pnull[i] = 1;
                
                bind[i].buffer_type= MYSQL_TYPE_STRING;
                bind[i].buffer_length = plen[i];
                bind[i].buffer= pbuf[i];
                bind[i].length= &plen[i];
                bind[i].is_null= &pnull[i];
                bind[i].error= &perr[i];
            }

			if (*token == TXT_ITEMFEED)
				token++;
            
			if (++i == cols)
				break;
		}

		while (i < cols)
		{
			plen[i] = 0;
			pbuf[i] = NULL;
            pnull[i] = 1;
            
            bind[i].buffer_type= MYSQL_TYPE_STRING;
            bind[i].buffer_length = plen[i];
            bind[i].buffer= pbuf[i];
            bind[i].length= &plen[i];
            bind[i].is_null= &pnull[i];
            bind[i].error= &perr[i];
            
			if (++i == cols)
				break;
		}
        
        if (C_OK != mysql_stmt_bind_param(stm, bind))
        {
            _raise_stm_error(stm);
        }

        if(C_OK != mysql_stmt_execute(stm))
        {
            _raise_stm_error(stm);
        }

		for (i = 0; i < cols; i++)
		{
            if(pbuf[i])
                xmem_free(pbuf[i]);
            pbuf[i] = NULL;
			plen[i] = 0;
            pnull[i] = 0;
		}

		rows += (int)mysql_stmt_affected_rows(stm);
	}

	_db_commit(pdb);

    mysql_stmt_close(stm);
	stm = NULL;

    for (i = 0; i < cols; i++)
    {
        if(pbuf[i])
            xmem_free(pbuf[i]);
    }
    
    xmem_free(pbuf);
    xmem_free(pnull);
    xmem_free(perr);
    xmem_free(plen);
    
    xmem_free(bind);

	string_free(vs);
	vs = NULL;

	pdb->rows = rows;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
	{
		_db_rollback(pdb);

		mysql_stmt_close(stm);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

    if(d_sql)
        xmem_free(d_sql);
    
	if (vs_sql)
		string_free(vs_sql);

	if (vs)
		string_free(vs);

    if(pbuf)
    {
        for (i = 0; i < cols; i++)
        {
            if(pbuf[i])
                xmem_free(pbuf[i]);
        }
        xmem_free(pbuf);
    }
    
    if(pnull)
        xmem_free(pnull);
    if(perr)
        xmem_free(perr);
    if(plen)
        xmem_free(plen);
    if(bind)
        xmem_free(bind);

	return 0;
}

bool_t STDCALL db_batch(xdb_t db, stream_t stream)
{
	db_t* pdb = (db_t*)db;
    
    my_ulonglong rows;
    
	string_t vs = NULL;
	string_t vs_sql = NULL;
	dword_t dw;
    
    const tchar_t *tkcur, *tkpre;
    int tklen;
    
    char* d_sql = NULL;
    int d_len;

	MYSQL_RES* pres = NULL;

	XDL_ASSERT(pdb != NULL);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	stream_read_utfbom(stream, NULL);

	vs = string_alloc();
	vs_sql = string_alloc();

	while (1)
	{
		string_empty(vs);
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			raise_user_error(_T("-1"), _T("stream read line failed"));
		}

		if (string_len(vs) == 0)
		{
			dw = 0;

			if (string_len(vs_sql))
				goto EXECUTE;
			else
				break;
		}

        tkcur = string_ptr(vs);
        tklen = string_len(vs);

		while (*tkcur == _T(' ') || *tkcur == _T('\t') || *tkcur == _T('\r') || *tkcur == _T('\n'))
		{
			tkcur++;
			tklen--;
		}
       
		if (*tkcur == _T('-') && *(tkcur + 1) == _T('-'))
		{
			continue;
		}

		tklen = split_semi(tkcur, tklen);

		tkpre = tkcur;
		tkcur += tklen;

		string_cat(vs_sql, tkpre, tklen);

		if (*tkcur != _T(';'))
		{
			continue;
		}

		string_empty(vs);

EXECUTE:
		string_cat(vs_sql, _T(";"), 1);
		tkpre = string_ptr(vs_sql);
		tklen = string_len(vs_sql);

#ifdef _UNICODE
		d_len = ucs_to_utf8(tkpre, tklen, NULL, MAX_LONG);
#else
		d_len = mbs_to_utf8(tkpre, tklen, NULL, MAX_LONG);
#endif

		d_sql = (char*)xmem_alloc(d_len + 1);

#ifdef _UNICODE
		d_len = ucs_to_utf8(tkpre, tklen, d_sql, d_len);
#else
		d_len = mbs_to_utf8(tkpre, tklen, d_sql, d_len);
#endif

		string_empty(vs_sql);

		if (C_OK != mysql_real_query(pdb->ctx, d_sql, d_len))
		{
			_raise_ctx_error(pdb->ctx);
		}

		xmem_free(d_sql);
		d_sql = NULL;

		rows = mysql_affected_rows(pdb->ctx);

		pres = mysql_store_result(pdb->ctx);

		if (pres)
		{
			mysql_free_result(pres);
		}

		pdb->rows += (int)rows;

		if (!dw)
			break;
	}

	string_free(vs);
	vs = NULL;

	string_free(vs_sql);
	vs_sql = NULL;

	END_CATCH;

	return 1;

ONERROR:

    if(d_sql)
         xmem_free(d_sql);
    
	if (vs)
		string_free(vs);

	if (vs_sql)
		string_free(vs_sql);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

int STDCALL db_rows(xdb_t db)
{
	db_t* pdb = (db_t*)db;

	XDL_ASSERT(pdb != NULL);

	return pdb->rows;
}

int STDCALL db_error(xdb_t db, tchar_t* buf, int max)
{
	db_t* pdb = (db_t*)db;

	XDL_ASSERT(pdb != NULL);

	max = (max < ERR_LEN) ? max : ERR_LEN;
	if (buf)
	{
		xsncpy(buf, pdb->err_text, max);
	}

	return -1;
}
