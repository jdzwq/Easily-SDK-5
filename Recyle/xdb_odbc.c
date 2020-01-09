/***********************************************************************
	Easily xdb odbc

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdb odbc document

	@module	xdb_odbc.c | xdb odbc implement file

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

#include <sql.h>
#include <sqlext.h>

#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbccp32.lib")

static tchar_t odbc_esc[] = { _T('%'), _T('\t'), _T('\r'), _T('\n'), _T('\0') };

typedef struct _db_t{
	xhand_head head;

	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stm;

	bool_t tran;

	int rows;
	tchar_t err_code[NUM_LEN + 1];
	tchar_t err_text[MAX_ERR + 1];

	XHANDLE log;
}db_t;

typedef struct _bindguid_t{
	SQLLEN ind;
	SQLLEN len;
	void* buf;
	void* ref;
}bindguid_t;

typedef struct _lob_t{
	xhand_head head;
	SQLHSTMT stmt;
	SQLSMALLINT type;
	SQLUSMALLINT col;
}lob_t;

static bool_t _lob_write(xhand_t io, const byte_t* data, dword_t* pb)
{
	lob_t* plob = (lob_t*)io;
	SQLRETURN rt;

	rt = SQLPutData(plob->stmt, (SQLPOINTER)(data), (SQLINTEGER)(*pb));

	return (rt == SQL_ERROR) ? 0 : 1;
}

static bool_t _lob_read(xhand_t io, const byte_t* data, dword_t* pb)
{
	lob_t* plob = (lob_t*)io;
	SQLRETURN rt;
	SQLLEN len = 0;

	rt = SQLGetData(plob->stmt, plob->col, plob->type, (SQLPOINTER)data, (SQLINTEGER)(*pb), &len);
	if (rt == SQL_NO_DATA || rt == SQL_ERROR)
	{
		*pb = 0;
	}
	if (len == SQL_NULL_DATA)
	{
		*pb = 0;
	}

	return (rt == SQL_ERROR) ? 0 : 1;
}

static void odbctodt(SQLSMALLINT type,tchar_t* dt)
{
	switch(type)
	{
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
	case SQL_WCHAR :
	case SQL_WVARCHAR:
	case SQL_WLONGVARCHAR:
		xscpy(dt,ATTR_DATA_TYPE_STRING);
		break;
	case SQL_BIT:
		xscpy(dt, ATTR_DATA_TYPE_BOOLEAN);
		break;
	case SQL_TINYINT:
	case SQL_SMALLINT:
	case SQL_INTEGER :
	case SQL_BIGINT:
		xscpy(dt,ATTR_DATA_TYPE_INTEGER);
		break ;
	case SQL_DECIMAL:
	case SQL_REAL :
	case SQL_FLOAT :
	case SQL_DOUBLE:
	case SQL_NUMERIC:
		xscpy(dt,ATTR_DATA_TYPE_NUMERIC);
		break;
	case SQL_TYPE_DATE:
		xscpy(dt, ATTR_DATA_TYPE_DATE);
		break;
	case SQL_TYPE_TIME:
	case SQL_TYPE_TIMESTAMP:
	case SQL_DATETIME:
		xscpy(dt,ATTR_DATA_TYPE_DATETIME);
		break;
	//case SQL_BINARY:
	//case SQL_VARBINARY:
	//case SQL_LONGVARBINARY:
		//xscpy(dt, ATTR_DATA_TYPE_BINARY);
		//break;
	default:
		xscpy(dt,ATTR_DATA_TYPE_STRING);
		break;
	}
}

static void _raise_dbc_error(SQLHDBC dbc)
{
	tchar_t err_code[NUM_LEN + 1] = { 0 };
	tchar_t err_text[MAX_ERR + 1] = { 0 };

	SQLINTEGER ne = 0;
	SQLSMALLINT si = 0;

	SQLGetDiagRec(SQL_HANDLE_DBC, dbc, 1, (SQLTCHAR*)err_code, &ne, (SQLTCHAR*)err_text, MAX_ERR, &si);

	raise_user_error(err_code, err_text);
}

static void _raise_stmt_error(HSTMT stmt)
{
	tchar_t err_code[NUM_LEN + 1] = { 0 };
	tchar_t err_text[MAX_ERR + 1] = { 0 };

	SQLINTEGER ne = 0;
	SQLSMALLINT si = 0;

	SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, (SQLTCHAR*)err_code, &ne, (SQLTCHAR*)err_text, MAX_ERR, &si);

	raise_user_error(err_code, err_text);
}

static void _db_reset(db_t* pdb)
{
	xscpy(pdb->err_code, _T(""));
	xscpy(pdb->err_text, _T(""));

	pdb->rows = 0;
}

bool_t __stdcall db_parse_dsn(const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len)
{
	link_t_ptr d_ptr = NULL;

	TRY_CATCH;

	d_ptr = create_proper_doc();
	if (!load_proper_doc_from_ini_file(d_ptr, NULL, dsnfile))
	{
		raise_user_error(_T("-1"), _T("parse dsn file failed"));
	}

	read_proper(d_ptr, _T("ODBC"), -1, DSN_SERVER, -1, srv_buf, srv_len);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_DATABASE, -1, dbn_buf, dbn_len);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_UID, -1, uid_buf, uid_len);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_PWD, -1, pwd_buf, pwd_len);

	destroy_proper_doc(d_ptr);
	d_ptr = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_ptr)
		destroy_proper_doc(d_ptr);

	return 0;
}

xhand_t __stdcall db_open_dsn(const tchar_t* dsnfile)
{
	db_t* pdb = NULL;

	SQLRETURN rt;
	SQLSMALLINT si;

	SQLHENV d_env = NULL;
	SQLHDBC d_dbc = NULL;

	tchar_t drv[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t srv[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t dbn[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t uid[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t pwd[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t tru[MAX_SQL_TOKEN + 1] = { 0 };
	tchar_t sql[MAX_SQL_NAME + 1] = { 0 };
	tchar_t sql2[MAX_SQL_NAME + 1] = { 0 };

	link_t_ptr d_ptr = NULL;

	TRY_CATCH;

	//for connecation pooling
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);

	d_ptr = create_proper_doc();
	if (!load_proper_doc_from_ini_file(d_ptr, NULL, dsnfile))
	{
		raise_user_error(_T("-1"), _T("parse dsn file failed"));
	}

	read_proper(d_ptr, _T("ODBC"), -1, DSN_DRIVER, -1, drv, MAX_SQL_TOKEN);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_SERVER, -1, srv, MAX_SQL_TOKEN);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_DATABASE, -1, dbn, MAX_SQL_TOKEN);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_UID, -1, uid, MAX_SQL_TOKEN);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_PWD, -1, pwd, MAX_SQL_TOKEN);
	read_proper(d_ptr, _T("ODBC"), -1, DSN_TRUSTED, -1, tru, MAX_SQL_TOKEN);

	destroy_proper_doc(d_ptr);
	d_ptr = NULL;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &d_env))
	{
		raise_user_error(_T("-1"), _T("Alloc enviroment handle failed"));
	}

	SQLSetEnvAttr(d_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	SQLSetEnvAttr(d_env, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, 0);
	SQLSetEnvAttr(d_env, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, 0);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, d_env, &d_dbc))
	{
		raise_user_error(_T("-1"), _T("Alloc connectoin handle failed"));
	}

	if (is_null(tru))
		xsprintf(sql, _T("DRIVER=%s;SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;"),drv, srv, dbn, uid, pwd);
	else
		xsprintf(sql, _T("DRIVER=%s;SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;Trusted_Connection=%s;"), drv, srv, dbn, uid, pwd, tru);

	rt = SQLDriverConnect(d_dbc, NULL, sql, SQL_NTS, sql2, MAX_SQL_NAME, &si, SQL_DRIVER_NOPROMPT);

	if (SQL_SUCCESS != rt && SQL_SUCCESS_WITH_INFO != rt)
	{
		_raise_dbc_error(d_dbc);
	}

	pdb = (db_t*)xmem_alloc(sizeof(db_t));
	pdb->head.tag = _HANDLE_ODBC;
	pdb->head.cbs = sizeof(db_t);
	pdb->env = d_env;
	pdb->dbc = d_dbc;

	END_CATCH;

	return (xhand_t)pdb;

ONERROR:

	if (d_ptr)
		destroy_proper_doc(d_ptr);
	if (d_dbc)
		SQLFreeHandle(SQL_HANDLE_DBC, d_dbc);
	if (d_env)
		SQLFreeHandle(SQL_HANDLE_ENV, d_env);

	//for connection pooling
	//CoUninitialize();

	return NULL;
}

xhand_t __stdcall db_open(const tchar_t* srv, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd)
{
	db_t* pdb = NULL;

	SQLRETURN rt;

	SQLHENV d_env = NULL;
	SQLHDBC d_dbc = NULL;

	TRY_CATCH;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &d_env))
	{
		raise_user_error(_T("-1"), _T("Alloc enviroment handle failed"));
	}

	SQLSetEnvAttr(d_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	SQLSetEnvAttr(d_env, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, 0);
	SQLSetEnvAttr(d_env, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, 0);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, d_env, &d_dbc))
	{
		raise_user_error(_T("-1"), _T("Alloc connectoin handle failed"));
	}

	rt = SQLConnect(d_dbc, (SQLTCHAR*)dbn, SQL_NTS, (SQLTCHAR*)uid, SQL_NTS, (SQLTCHAR*)pwd, SQL_NTS);

	if (SQL_SUCCESS != rt && SQL_SUCCESS_WITH_INFO != rt)
	{
		_raise_dbc_error(d_dbc);
	}

	pdb = (db_t*)xmem_alloc(sizeof(db_t));
	pdb->head.tag = _HANDLE_ODBC;
	pdb->head.cbs = sizeof(db_t);
	pdb->env = d_env;
	pdb->dbc = d_dbc;

	END_CATCH;

	return (xhand_t)pdb;

ONERROR:

	if (d_dbc)
		SQLFreeHandle(SQL_HANDLE_DBC, d_dbc);

	if (d_env)
		SQLFreeHandle(SQL_HANDLE_ENV, d_env);

	return NULL;
}

void __stdcall db_close(xhand_t db)
{
	db_t* pdb = (db_t*)db;
	
	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	SQLDisconnect(pdb->dbc);
	SQLFreeHandle(SQL_HANDLE_DBC,pdb->dbc);
	pdb->dbc = NULL;

	SQLFreeHandle(SQL_HANDLE_ENV,pdb->env);
	pdb->env = NULL;
	
	xmem_free(pdb);

	//for connection pooling
	//CoUninitialize();
}

void _db_tran(db_t* pdb)
{
	pdb->tran = 1;
	SQLSetConnectAttr(pdb->dbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
}

void _db_commit(db_t* pdb)
{
	if (pdb->tran)
	{
		SQLEndTran(SQL_HANDLE_DBC, pdb->dbc, SQL_COMMIT);
		pdb->tran = 0;
	}
	SQLSetConnectAttr(pdb->dbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_ON,0);
}

void _db_rollback(db_t* pdb)
{
	if (pdb->tran)
	{
		SQLEndTran(SQL_HANDLE_DBC, pdb->dbc, SQL_ROLLBACK);
		pdb->tran = 0;
	}
	SQLSetConnectAttr(pdb->dbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_ON,0);
}

bool_t _db_need_data_len(db_t* pdb)
{
	tchar_t token[3] = { 0 };
	SQLSMALLINT len = 0;

	SQLGetInfo(pdb->dbc, SQL_NEED_LONG_DATA_LEN, (SQLPOINTER)token, 3, &len);

	return (token[0] == _T('y') || token[0] == _T('Y')) ? 1 : 0;
}

bool_t __stdcall db_datetime(xhand_t db, tchar_t* sz_time)
{
	db_t* pdb = (db_t*)db;

	HSTMT d_hstmt = NULL;

	SQLLEN len;
	SQLRETURN rt;

	tchar_t sqlstr[] = _T("select convert(varchar(24),getdate(),120) as DT");

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	//SQLSetStmtOption(d_hstmt,SQL_CURSOR_TYPE,SQL_CURSOR_FORWARD_ONLY);
	SQLSetStmtAttr(d_hstmt, SQL_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);

	rt = SQLExecDirect(d_hstmt, (SQLTCHAR*)sqlstr, xslen(sqlstr));

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	rt = SQLFetch(d_hstmt);

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	rt = SQLGetData(d_hstmt, 1, SQL_C_TCHAR, (SQLPOINTER)sz_time, DATE_LEN * sizeof(tchar_t), &len);

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	SQLFreeStmt(d_hstmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	d_hstmt = NULL;

	pdb->rows = 1;

	END_CATCH;

	return 1;

ONERROR:

	if (d_hstmt)
	{
		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t __stdcall db_exec(xhand_t db, const tchar_t* sqlstr, int len)
{
	db_t* pdb = (db_t*)db;

	SQLRETURN rt;
	SQLLEN ne;

	HSTMT d_hstmt = NULL;

	tchar_t *tkpre,*tkcur;
	int tklen,total = 0;
	bool_t uni;
	bool_t glt = 0;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	if (len < 0)
		len = xslen(sqlstr);

	if (!len || is_null(sqlstr))
	{
		return 1;
	}

	TRY_CATCH;

	_db_reset(pdb);

	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT,pdb->dbc,&d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	_db_tran(pdb);

	total = 0;
	tkpre = (tchar_t*)sqlstr;
	while(total < len)
	{
		uni = 0;
		glt = 0;

		tklen = 0;
		tkcur = tkpre;
		while (*tkcur != _T('\r') && *tkcur != _T('\n') && total < len)
		{
			if (*tkcur == _T('\''))
			{
				if (glt)
					glt = 0;
				else
					glt = 1;
			}

			tklen ++;
			tkcur ++;
			total++;

			if (glt)
			{
				while (*tkcur == _T('\r') || *tkcur == _T('\n'))
				{
					tklen++;
					tkcur++;
					total++;
				}
			}
		}

		while (*tkcur == _T(' ') || *tkcur == _T('\t') || *tkcur == _T('\n') || *tkcur == _T('\r'))
		{
			if (*tkcur == _T('\r'))
				uni = 1;

			tkcur++;
			total++;
		}

		if(tklen == 0)
		{
			tkpre = tkcur;
			continue;
		}

		rt = SQLExecDirect(d_hstmt,(SQLTCHAR*)tkpre,tklen);

		if(rt != SQL_SUCCESS && rt != SQL_NO_DATA && rt != SQL_SUCCESS_WITH_INFO)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, tkpre, tklen, NULL);
			}

			_raise_stmt_error(d_hstmt);
		}

		ne = 0;
		SQLRowCount(d_hstmt,&ne);

		if (uni && ne != 1)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, tkpre, tklen, NULL);
			}

			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}

		pdb->rows += (int)ne;

		SQLFreeStmt(d_hstmt,SQL_CLOSE);
		tkpre = tkcur;
	}
	
	_db_commit(pdb);

	SQLFreeHandle(SQL_HANDLE_STMT,d_hstmt);
	d_hstmt = NULL;
	
	END_CATCH;

	return 1;

ONERROR:

	if (d_hstmt)
	{
		_db_rollback(pdb);

		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t __stdcall db_update(xhand_t db, link_t_ptr grid)
{
	db_t* pdb = (db_t*)db;

	HSTMT d_hstmt = NULL;
	tchar_t* d_sql = NULL;

	SQLRETURN rt;
	SQLLEN ne;

	link_t_ptr rlk;
	int len,rs;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	TRY_CATCH;

	_db_reset(pdb);

	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT,pdb->dbc,&d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	_db_tran(pdb);
	
	rt = SQL_SUCCESS;
	d_sql = NULL;

	rlk = get_next_row(grid,LINK_FIRST);
	while(rlk)
	{
		len = 0;
		rs = get_row_state(rlk);
		if(rs == dsDelete)
		{
			len = format_row_delete_sql(grid,rlk, NULL, MAX_LONG);
			if(len > 0)
			{
				d_sql = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
				format_row_delete_sql(grid,rlk,d_sql,len);
			}
		}else if(rs == dsNewDirty)
		{
			len = format_row_insert_sql(grid,rlk,NULL,MAX_LONG);
			if(len > 0)
			{
				d_sql = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
				format_row_insert_sql(grid,rlk,d_sql,len);
			}
		}else if(rs == dsDirty)
		{
			len = format_row_update_sql(grid,rlk,NULL,MAX_LONG);
			if(len > 0)
			{
				d_sql = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
				format_row_update_sql(grid,rlk,d_sql,len);
			}
		}
		else
		{
			d_sql = NULL;
			len = 0;
		}

		if (!len)
		{
			rlk = get_next_row(grid, rlk);
			continue;
		}

		rt = SQLExecDirect(d_hstmt, (SQLTCHAR*)d_sql, len);

		if (rt != SQL_SUCCESS && rt != SQL_NO_DATA && rt != SQL_SUCCESS_WITH_INFO)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, d_sql, len, NULL);
			}

			_raise_stmt_error(d_hstmt);
		}

		ne = 0;
		SQLRowCount(d_hstmt, &ne);
		if (!ne)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, d_sql, len, NULL);
			}

			raise_user_error(_T("-1"), ERR_TEXT_ROWCANCEL);
		}else if (ne > 1)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, d_sql, len, NULL);
			}

			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}
		else
		{
			pdb->rows += (int)ne;
		}

		xmem_free(d_sql);
		d_sql = NULL;

		SQLFreeStmt(d_hstmt, SQL_CLOSE);

		rlk = get_next_row(grid,rlk);
	}
	
	_db_commit(pdb);

	SQLFreeHandle(SQL_HANDLE_STMT,d_hstmt);
	d_hstmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_sql)
		xmem_free(d_sql);

	if (d_hstmt)
	{
		_db_rollback(pdb);

		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

int _db_fetch_row(db_t* pdb, HSTMT hstmt, link_t_ptr grid)
{
	SQLSMALLINT cols = 0;
	SQLSMALLINT namelen;
	SQLSMALLINT datatype;
	SQLSMALLINT scale;
	SQLSMALLINT nullable;
	SQLULEN colsize;
	SQLSMALLINT i;
	SQLRETURN rt;

	bindguid_t* pbind = NULL;

	link_t_ptr clk, rlk;
	tchar_t colname[MAX_SQL_NAME];
	const tchar_t* data_type;
	int len,rows = 0;
	bool_t b_able;

	b_able = get_grid_updateable(grid);

	rt = SQLNumResultCols(hstmt,&cols);

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
		return XDB_FAILED;

	pbind = (bindguid_t*)xmem_alloc(cols * sizeof(bindguid_t));

	for (i = 0; i < cols; i++)
	{
		colname[0] = _T('\0');
		SQLDescribeCol(hstmt, i + 1, (SQLTCHAR*)colname, MAX_SQL_NAME, &namelen, &datatype, &colsize, &scale, &nullable);

		clk = get_col(grid, colname);
		if (!clk)
		{
			continue;
		}

		data_type = get_col_data_type_ptr(clk);
		if (compare_text(data_type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
		{
			len = get_col_data_len(clk);
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			len = 2;
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			len = NUM_LEN;
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			len = DATE_LEN ;
		}
		else
		{
			len = 0;
		}

		if (!len)
		{
			len = MAX_SQL_DATA;
		}

		pbind[i].len = (len + 1) * sizeof(tchar_t);
		pbind[i].buf = (byte_t*)xmem_alloc(pbind[i].len);

		SQLBindCol(hstmt, i + 1, SQL_C_TCHAR, (SQLPOINTER)(pbind[i].buf), pbind[i].len, &(pbind[i].ind));
	}

	while(rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO)
	{
		rt = SQLFetch(hstmt);

		if(rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
			break;

		rlk = insert_row(grid,LINK_LAST);
		set_row_state(rlk,dsClean);
		rows ++;

		for(i=0;i<cols;i++)
		{
			colname[0] = _T('\0');
			SQLDescribeCol(hstmt, i + 1, (SQLTCHAR*)colname, MAX_SQL_NAME, &namelen, &datatype, &colsize, &scale, &nullable);

			clk = get_col(grid, colname);
			if (!clk)
			{
				continue;
			}

			if (pbind[i].ind != SQL_NULL_DATA && pbind[i].ind != SQL_NO_TOTAL)
			{
				set_cell_text(rlk, clk, (tchar_t*)(pbind[i].buf), -1);

				if (b_able && compare_text(get_col_field_type_ptr(clk), -1, ATTR_FIELD_TYPE_PRIM, -1, 0) == 0)
				{
					set_cell_prim(rlk, clk, (tchar_t*)(pbind[i].buf), -1);
				}
			}
		}
	}
	
	if (pbind)
	{
		for (i = 0; i < cols; i++)
			xmem_free(pbind[i].buf);

		xmem_free(pbind);
	}

	if(rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO)
		return XDB_PENDING;
	else if(rt == SQL_NO_DATA)
		return XDB_SUCCEED;
	else
		return XDB_FAILED;
}

bool_t __stdcall db_fetch(xhand_t db, link_t_ptr grid)
{
	db_t* pdb = (db_t*)db;

	HSTMT d_hstmt = NULL;
	tchar_t* d_sql = NULL;
	int nRet;

	SQLINTEGER len;
	SQLRETURN rt;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	TRY_CATCH;

	_db_reset(pdb);

	clear_grid_rows(grid);

	len = format_grid_select_sql(grid,NULL,MAX_LONG);
	if(len <= 0)
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	d_sql = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
	format_grid_select_sql(grid, d_sql, len);

	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT,pdb->dbc,&d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}
	
	//SQLSetStmtOption(d_hstmt,SQL_CURSOR_TYPE,SQL_CURSOR_FORWARD_ONLY);
	SQLSetStmtAttr(d_hstmt, SQL_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);

	rt = SQLExecDirect(d_hstmt,(SQLTCHAR*)d_sql,len);

	if(rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		if (pdb->log)
		{
			stream_write(pdb->log, d_sql, len, NULL);
		}

		_raise_stmt_error(d_hstmt);
	}

	xmem_free(d_sql);
	d_sql = NULL;

	nRet = XDB_PENDING;
	while (nRet == XDB_PENDING)
	{
		nRet = _db_fetch_row(pdb, d_hstmt, grid);
	}

	if (nRet == XDB_FAILED)
	{
		_raise_stmt_error(d_hstmt);
	}

	do {
		rt = SQLMoreResults(d_hstmt);
	} while (rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO);

	SQLFreeStmt(d_hstmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	d_hstmt = NULL;

	pdb->rows = get_row_count(grid);

	END_CATCH;

	return 1;

ONERROR:

	if (d_sql)
		xmem_free(d_sql);

	if (d_hstmt)
	{
		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t __stdcall db_select(xhand_t db, link_t_ptr grid, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	HSTMT d_hstmt = NULL;

	SQLSMALLINT cols;
	SQLSMALLINT namelen;
	SQLSMALLINT datatype;
	SQLSMALLINT scale;
	SQLSMALLINT nullable;
	SQLULEN colsize;
	SQLSMALLINT i;
	SQLRETURN rt;

	link_t_ptr clk;
	tchar_t colname[MAX_SQL_NAME];
	tchar_t token[MAX_SQL_NAME];
	int nRet;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	TRY_CATCH;
	
	_db_reset(pdb);

	clear_grid_rows(grid);
	clear_grid_cols(grid);

	if(is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT,pdb->dbc,&d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	//SQLSetStmtOption(d_hstmt,SQL_CURSOR_TYPE,SQL_CURSOR_FORWARD_ONLY);
	SQLSetStmtAttr(d_hstmt, SQL_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);

	rt = SQLExecDirect(d_hstmt,(SQLTCHAR*)sqlstr,xslen(sqlstr));

	if(rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		if (pdb->log)
		{
			stream_write(pdb->log, sqlstr, xslen(sqlstr), NULL);
		}

		_raise_stmt_error(d_hstmt);
	}
		
	cols = 0;
	rt = SQLNumResultCols(d_hstmt,&cols);
	
	if(rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}
	
	for(i=1;i<=cols;i++)
	{
		colname[0] = _T('\0');
		
		SQLDescribeCol(d_hstmt,i,(SQLTCHAR*)colname,MAX_SQL_NAME,&namelen,&datatype,&colsize,&scale,&nullable);	
		
		clk = insert_col(grid,LINK_LAST);

		set_col_name(clk,colname);
		set_col_title(clk,colname);

		odbctodt(datatype,token);
		set_col_data_type(clk,token);

		if (colsize > MAX_SQL_DATA)
			colsize = 0;
		set_col_data_len(clk,(long)colsize);

		if(scale < 0)
			scale = 0;
		if(scale > MAX_FLOAT_DIGI)
			scale = MAX_FLOAT_DIGI;
		set_col_data_dig(clk,scale);
	}
	
	nRet = XDB_PENDING;
	while (nRet == XDB_PENDING)
	{
		nRet = _db_fetch_row(pdb, d_hstmt, grid);
	}

	if (nRet == XDB_FAILED)
	{
		_raise_stmt_error(d_hstmt);
	}

	do {
		rt = SQLMoreResults(d_hstmt);
	} while (rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO);

	SQLFreeStmt(d_hstmt,SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT,d_hstmt);
	d_hstmt = NULL;

	pdb->rows = get_row_count(grid);

	END_CATCH;

	return 1;

ONERROR:

	if (d_hstmt)
	{
		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t __stdcall db_schema(xhand_t db, link_t_ptr grid, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	HSTMT d_hstmt = NULL;

	SQLSMALLINT si;
	SQLSMALLINT cols;
	SQLSMALLINT i;
	SQLRETURN rt;
	SQLLEN numattr;

	tchar_t strattr[MAX_ERR + 1] = { 0 };

	link_t_ptr clk;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	TRY_CATCH;

	_db_reset(pdb);

	clear_grid_rows(grid);
	clear_grid_cols(grid);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	SQLSetStmtAttr(d_hstmt, SQL_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, 0);

	rt = SQLExecDirect(d_hstmt, (SQLTCHAR*)sqlstr, xslen(sqlstr));

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	cols = 0;
	rt = SQLNumResultCols(d_hstmt, &cols);

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	for (i = 1; i <= cols; i++)
	{
		strattr[0] = _T('\0');
		numattr = 0;

		clk = insert_col(grid, LINK_LAST);

		SQLColAttribute(d_hstmt, i, SQL_DESC_BASE_TABLE_NAME, strattr, MAX_ERR, &si, NULL);
		set_col_table_name(clk, strattr);
		if (is_null(strattr))
			set_col_field_type(clk, ATTR_FIELD_TYPE_CALC);
		else
			set_col_field_type(clk, ATTR_FIELD_TYPE_DATA);

		SQLColAttribute(d_hstmt, i, SQL_DESC_BASE_COLUMN_NAME, strattr, MAX_ERR, &si, NULL);
		set_col_field_name(clk, strattr);

		SQLColAttribute(d_hstmt, i, SQL_DESC_NAME, strattr, MAX_ERR, &si, NULL);
		set_col_name(clk, strattr);
		set_col_title(clk, strattr);

		SQLColAttribute(d_hstmt, i, SQL_DESC_TYPE, NULL, 0, &si, &numattr);
		odbctodt((short)numattr, strattr);
		set_col_data_type(clk, strattr);

		SQLColAttribute(d_hstmt, i, SQL_DESC_LENGTH, NULL, 0, &si, &numattr);
		set_col_data_len(clk, (long)numattr);

		SQLColAttribute(d_hstmt, i, SQL_DESC_SCALE, NULL, 0, &si, &numattr);
		set_col_data_dig(clk, (long)numattr);

		SQLColAttribute(d_hstmt, i, SQL_DESC_NULLABLE, NULL, 0, &si, &numattr);
		set_col_nullable(clk, (bool_t)numattr);

		SQLColAttribute(d_hstmt, i, SQL_DESC_UPDATABLE, NULL, 0, &si, &numattr);
		set_col_updateable(clk, (bool_t)numattr);

	}

	do {
		rt = SQLMoreResults(d_hstmt);
	} while (rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO);

	SQLFreeStmt(d_hstmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	d_hstmt = NULL;

	pdb->rows = 0;

	END_CATCH;

	return 1;

ONERROR:

	if (d_hstmt)
	{
		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

int __cdecl _db_call_arg(db_t* pdb, const tchar_t* procname, const tchar_t* fmt, va_list* parg)
{
	HSTMT d_hstmt = NULL;
	SQLUINTEGER collen = 0;
	SQLSMALLINT colprec = 0;
	SQLSMALLINT iotype;
	SQLLEN ne = 0;
	SQLRETURN rt;

	bindguid_t* pbind = NULL;
	int i,ind = 0;
	long tk = -1;
	
	int sqllen;
	tchar_t* d_sql = NULL;
	tchar_t* token;
	tchar_t* ptr_str;
	long* ptr_long;
	double* ptr_double;

	TRY_CATCH;

	pdb->rows = 0;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	ind = 0;
	token = (tchar_t*)fmt;
	while (token && *token)
	{
		if (*token == '%')
			ind++;

		token++;
	}

	sqllen = xslen(_T("{? = call ()}")) + xslen(procname) + ind * 2;
	d_sql = (tchar_t*)xmem_alloc((sqllen + 1) * sizeof(tchar_t));

	xsprintf(d_sql, _T("{? = call %s("), procname);
	for (i = 0; i < ind; i++)
	{
		xscat(d_sql, _T("?,"));
	}

	sqllen = xslen(d_sql);
	if (d_sql[sqllen - 1] == _T('('))
	{
		d_sql[sqllen - 1] = _T('}');
		d_sql[sqllen] = _T('\0');
	}
	else
	{
		d_sql[sqllen - 1] = _T(')');
		d_sql[sqllen] = _T('}');
		d_sql[sqllen + 1] = _T('\0');
	}
	sqllen = xslen(d_sql);

	rt = SQLPrepare(d_hstmt, d_sql, sqllen);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	pbind = (bindguid_t*)xmem_alloc((ind + 1) * sizeof(bindguid_t));

	ind = 0;
	pbind[ind].len = sizeof(long);
	pbind[ind].buf = xmem_alloc(pbind[ind].len);
	pbind[ind].ind = 0;
	pbind[ind].ref = (void*)&tk;

	rt = SQLBindParameter(d_hstmt, ind + 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)(pbind[ind].buf), 0, &(pbind[ind].ind));
	ind++;

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

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
			iotype = SQL_PARAM_OUTPUT;
		}
		else if (*token == _T('+'))
		{
			token++;
			iotype = SQL_PARAM_INPUT_OUTPUT;
		}
		else
		{
			iotype = SQL_PARAM_INPUT;
		}

		collen = 0;
		colprec = 0;

		if (*token >= _T('0') && *token <= _T('9'))
			collen = (SQLUINTEGER)xstol(token);

		while (*token >= _T('0') && *token <= _T('9'))
			token++;

		if (*token == _T('.'))
		{
			token++;
			colprec = (SQLSMALLINT)xstol(token);
			while (*token >= _T('0') && *token <= _T('9'))
				token++;
		}

		rt = SQL_SUCCESS;

		switch (*token)
		{
		case _T('s'):
			ptr_str = va_arg(*parg, tchar_t*);
			if (!collen)
				collen = xslen((tchar_t*)ptr_str);

			pbind[ind].len = (collen + 1) * sizeof(tchar_t);
			pbind[ind].buf = xmem_alloc(pbind[ind].len);
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_INPUT)
			{
				xscpy((tchar_t*)(pbind[ind].buf), ptr_str);
				pbind[ind].ind = SQL_NTS;
			}
			else
			{
				pbind[ind].ind = 0;
			}
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_OUTPUT)
			{
				pbind[ind].ref = (void*)ptr_str;
			}
			rt = SQLBindParameter(d_hstmt, ind + 1, iotype, SQL_C_TCHAR, SQL_VARCHAR, collen, 0, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
			ind++;
			break;
		case _T('d'):
			ptr_long = va_arg(*parg, long*);
			pbind[ind].len = sizeof(long);
			pbind[ind].buf = xmem_alloc(pbind[ind].len);
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_INPUT)
			{
				xmem_copy(pbind[ind].buf, ptr_long, sizeof(long));
			}
			pbind[ind].ind = 0;
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_OUTPUT)
			{
				pbind[ind].ref = (void*)ptr_long;
			}
			rt = SQLBindParameter(d_hstmt, ind + 1, iotype, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
			ind++;
			break;
		case _T('f'):
			ptr_double = va_arg(*parg, double*);
			pbind[ind].len = sizeof(double);
			pbind[ind].buf = xmem_alloc(pbind[ind].len);
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_INPUT)
			{
				xmem_copy(pbind[ind].buf, ptr_long, sizeof(double));
			}
			pbind[ind].ind = 0;
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_OUTPUT)
			{
				pbind[ind].ref = (void*)ptr_double;
			}
			rt = SQLBindParameter(d_hstmt, ind + 1, iotype, SQL_C_DOUBLE, SQL_NUMERIC, collen, colprec, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
			ind++;
			break;
		}

		if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
			break;
	}

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	rt = SQLExecute(d_hstmt);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	xmem_free(d_sql);
	d_sql = NULL;

	for (i = 0; i < ind; i++)
	{
		if (pbind[i].ref)
		{
			xmem_copy(pbind[i].ref, pbind[i].buf, pbind[i].len);
		}
		xmem_free(pbind[i].buf);
	}

	xmem_free(pbind);
	pbind = NULL;

	SQLFreeStmt(d_hstmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	d_hstmt = NULL;

	END_CATCH;

	return tk;

ONERROR:

	if (d_sql)
		xmem_free(d_sql);

	if (pbind)
	{
		for (i = 0; i < ind; i++)
		{
			xmem_free(pbind[i].buf);
		}

		xmem_free(pbind);
	}

	if (d_hstmt)
	{
		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return -1;
}

int __cdecl db_call_arg(xhand_t db, const tchar_t* procname, const tchar_t* fmt, ...)
{
	db_t* pdb = (db_t*)db;

	va_list arg;
	int rt;

	va_start(arg,fmt);
	rt = _db_call_arg(pdb,procname,fmt,&arg);
	va_end(arg);

	return rt;
}

bool_t __stdcall db_call_fun(xhand_t db, link_t_ptr func)
{
	db_t* pdb = (db_t*)db;

	HSTMT d_hstmt = NULL;
	tchar_t* d_sql = NULL;

	SQLUINTEGER collen = 0;
	SQLSMALLINT colprec = 0;
	SQLSMALLINT iotype;
	SQLLEN ne = 0;
	SQLRETURN rt;

	link_t_ptr flk;

	bindguid_t* pbind = NULL;
	int ind = 0;
	int len;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &d_hstmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	ind = get_func_param_count(func);
	pbind = (bindguid_t*)xmem_alloc((ind + 1) * sizeof(bindguid_t));

	ind = 0;
	pbind[ind].ind = 0;
	pbind[ind].len = sizeof(long);
	pbind[ind].buf = xmem_alloc(pbind[ind].len);

	rt = SQLBindParameter(d_hstmt, ind + 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
	ind++;

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	flk = get_func_next_param(func, LINK_FIRST);
	while (flk)
	{
		if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_OUTPUT, -1, 0) == 0)
		{
			iotype = SQL_PARAM_OUTPUT;
		}
		else if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_INPUTOUTPUT, -1, 0) == 0)
		{
			iotype = SQL_PARAM_INPUT_OUTPUT;
		}else
		{
			iotype = SQL_PARAM_INPUT;
		}

		collen = (SQLUINTEGER)get_func_param_data_len(flk);
		colprec = (SQLSMALLINT)get_func_param_data_dig(flk);
		if (!colprec)
			colprec = MAX_FLOAT_DIGI;

		rt = SQL_SUCCESS;

		if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
		{
			pbind[ind].len = sizeof(long);
			pbind[ind].buf = xmem_alloc(pbind[ind].len);
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_INPUT)
			{
				*(long*)(pbind[ind].buf) = get_func_param_integer(flk);
			}
			pbind[ind].ind = pbind[ind].len;

			rt = SQLBindParameter(d_hstmt, ind + 1, iotype, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
			ind++;
		}
		else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			pbind[ind].len = sizeof(double);
			pbind[ind].buf = xmem_alloc(pbind[ind].len);
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_INPUT)
			{
				*(double*)(pbind[ind].buf) = get_func_param_numeric(flk);
			}
			pbind[ind].ind = pbind[ind].len;

			rt = SQLBindParameter(d_hstmt, ind + 1, iotype, SQL_C_DOUBLE, SQL_NUMERIC, collen, colprec, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
			ind++;
		}
		else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			collen = DATE_LEN;
			pbind[ind].len = collen * sizeof(tchar_t);
			pbind[ind].buf = xmem_alloc(pbind[ind].len);
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_INPUT)
			{
				xscpy((tchar_t*)pbind[ind].buf,get_func_param_text_ptr(flk));
				pbind[ind].ind = SQL_NTS;
			}else
				pbind[ind].ind = pbind[ind].len;

			rt = SQLBindParameter(d_hstmt, ind + 1, iotype, SQL_C_TCHAR, SQL_VARCHAR, pbind[ind].len, 0, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
			ind++;
		}
		else
		{
			if (!collen)
				collen = xslen(get_func_param_text_ptr(flk));

			pbind[ind].len = (collen + 1) * sizeof(tchar_t);
			pbind[ind].buf = xmem_alloc(pbind[ind].len);
			if (iotype == SQL_PARAM_INPUT_OUTPUT || iotype == SQL_PARAM_INPUT)
			{
				xscpy((tchar_t*)pbind[ind].buf, get_func_param_text_ptr(flk));
				pbind[ind].ind = SQL_NTS;
			}else
				pbind[ind].ind = pbind[ind].len;

			rt = SQLBindParameter(d_hstmt, ind + 1, iotype, SQL_C_TCHAR, SQL_VARCHAR, pbind[ind].len, 0, (SQLPOINTER)(pbind[ind].buf), pbind[ind].len, &(pbind[ind].ind));
			ind++;
		}

		if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
			break;

		flk = get_func_next_param(func, flk);
	}

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	ind--;
	len = xslen(_T("{? = call ()}")) + xslen(get_func_name_ptr(func)) + ind * 2;
	d_sql = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));

	xsprintf(d_sql, _T("{? = call %s("), get_func_name_ptr(func));
	while (ind--)
	{
		xscat(d_sql, _T("?,"));
	}

	len = xslen(d_sql);
	if (d_sql[len - 1] == _T('('))
	{
		d_sql[len - 1] = _T('}');
		d_sql[len] = _T('\0');
	}
	else
	{
		d_sql[len - 1] = _T(')');
		d_sql[len] = _T('}');
		d_sql[len + 1] = _T('\0');
	}
	len = xslen(d_sql);

	rt = SQLExecDirect(d_hstmt, d_sql, len);

	xmem_free(d_sql);
	d_sql = NULL;

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(d_hstmt);
	}

	SQLFreeStmt(d_hstmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	d_hstmt = NULL;

	ind = 0;
	set_func_data_type(func, ATTR_DATA_TYPE_INTEGER);
	set_func_return_integer(func, *((long*)(pbind[ind].buf)));
	xmem_free(pbind[ind].buf);
	pbind[ind].buf = NULL;

	ind++;

	flk = get_func_next_param(func, LINK_FIRST);
	while (flk)
	{
		if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_OUTPUT, -1, 0) == 0 || compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_INPUTOUTPUT, -1, 0) == 0)
		{
			if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
			{
				set_func_param_integer(flk, *(long*)(pbind[ind].buf));
			}
			else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
			{
				set_func_param_numeric(flk, *(double*)(pbind[ind].buf));
			}
			else
			{
				set_func_param_text(flk, (tchar_t*)(pbind[ind].buf), -1);
			}
		}

		xmem_free(pbind[ind].buf);
		pbind[ind].buf = NULL;
		
		ind++;

		flk = get_func_next_param(func, flk);
	}

	xmem_free(pbind);
	pbind = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_hstmt)
	{
		SQLFreeStmt(d_hstmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, d_hstmt);
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	if (d_sql)
		xmem_free(d_sql);

	if (pbind)
	{
		ind = get_func_param_count(func) + 1;
		while (ind--)
		{
			if (pbind[ind].buf)
			{
				xmem_free(pbind[ind].buf);
			}
		}
		xmem_free(pbind);
	}

	set_func_data_type(func, ATTR_DATA_TYPE_INTEGER);
	set_func_return_integer(func, -1);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
bool_t __stdcall db_prepare(xhand_t db, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	SQLSMALLINT cols;
	SQLLEN rows;
	SQLSMALLINT namelen;
	SQLSMALLINT coltype;
	SQLSMALLINT scale;
	SQLSMALLINT nullable;
	SQLULEN colsize;
	SQLSMALLINT i;
	SQLRETURN rt;

	tchar_t colname[MAX_SQL_NAME];
	tchar_t datatype[RES_LEN];
	dword_t pos;
	int len;
	bindguid_t* pbind = NULL;

	tchar_t feed[3] = { TXT_ITEMFEED, TXT_LINEFEED, _T('\n') };

	xhand_t vs = NULL;

	tchar_t* sz_esc = NULL;
	int len_esc = 0;
	int len_buf = 0;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	TRY_CATCH;

	_db_reset(pdb);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &pdb->stm))
	{
		_raise_dbc_error(pdb->dbc);
	}

	//SQLSetStmtOption(d_hstmt,SQL_CURSOR_TYPE,SQL_CURSOR_FORWARD_ONLY);
	SQLSetStmtAttr(pdb->stm, SQL_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);

	rt = SQLExecDirect(pdb->stm, (SQLTCHAR*)sqlstr, xslen(sqlstr));

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		if (pdb->log)
		{
			stream_write(pdb->log, sqlstr, xslen(sqlstr), NULL);
		}

		_raise_stmt_error(pdb->stm);
	}

	cols = 0;
	rt = SQLNumResultCols(pdb->stm, &cols);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(pdb->stm);
	}

	SQLRowCount(pdb->stm, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	END_CATCH;

	return 1;

ONERROR:

	if (pdb->stm)
	{
		SQLFreeStmt(pdb->stm, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, pdb->stm);
		pdb->stm = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t __stdcall db_export(xhand_t db, xhand_t stream, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLSMALLINT cols;
	SQLLEN rows;
	SQLSMALLINT namelen;
	SQLSMALLINT coltype;
	SQLSMALLINT scale;
	SQLSMALLINT nullable;
	SQLULEN colsize;
	SQLSMALLINT i;
	SQLRETURN rt;

	tchar_t colname[MAX_SQL_NAME];
	tchar_t datatype[RES_LEN];
	dword_t pos;
	int len;
	bindguid_t* pbind = NULL;

	tchar_t feed[3] = { TXT_ITEMFEED, TXT_LINEFEED, _T('\n') };

	xhand_t vs = NULL;

	tchar_t* sz_esc = NULL;
	int len_esc = 0;
	int len_buf = 0;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	//SQLSetStmtOption(d_hstmt,SQL_CURSOR_TYPE,SQL_CURSOR_FORWARD_ONLY);
	SQLSetStmtAttr(stmt, SQL_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);

	rt = SQLExecDirect(stmt, (SQLTCHAR*)sqlstr, xslen(sqlstr));

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		if (pdb->log)
		{
			stream_write(pdb->log, sqlstr, xslen(sqlstr), NULL);
		}

		_raise_stmt_error(stmt);
	}

	cols = 0;
	rt = SQLNumResultCols(stmt, &cols);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	SQLRowCount(stmt, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	stream_write_utfbom(stream, NULL);

	vs = varstr_alloc();

	pbind = (bindguid_t*)xmem_alloc(cols * sizeof(bindguid_t));

	pos = 0;
	for (i = 0; i < cols; i++)
	{
		colname[0] = _T('\0');

		SQLDescribeCol(stmt, i + 1, (SQLTCHAR*)colname, MAX_SQL_NAME, &namelen, &coltype, &colsize, &scale, &nullable);

		odbctodt(coltype, datatype);

		if (!colsize || colsize > MAX_SQL_DATA)
			colsize = MAX_SQL_DATA;

		if (compare_text(datatype, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
		{
			len = (int)colsize;
		}
		else if (compare_text(datatype, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			len = 2;
		}
		else if (compare_text(datatype, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0 || compare_text(datatype, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			len = NUM_LEN;
		}
		else if (compare_text(datatype, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(datatype, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			len = DATE_LEN;
		}
		else
		{
			len = MAX_SQL_NAME;
		}

		pbind[i].len = (len + 1) * sizeof(tchar_t);
		pbind[i].ind = 0;
		pbind[i].buf = (byte_t*)xmem_alloc(pbind[i].len);

		SQLBindCol(stmt, i + 1, SQL_C_TCHAR, pbind[i].buf, pbind[i].len, &pbind[i].ind);

		varstr_cat(vs, colname, -1);
		varstr_cat(vs, feed, 1);
	}
	varstr_cat(vs, feed + 1, 2);

	stream_write_line(stream, vs, &pos);

	varstr_empty(vs);

	while (rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO)
	{
		rt = SQLFetch(stmt);

		if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
			break;

		pos = 0;
		for (i = 0; i < cols; i++)
		{
			if (pbind[i].ind != SQL_NULL_DATA && pbind[i].ind != SQL_NO_TOTAL)
			{
				len_buf = xslen((tchar_t*)pbind[i].buf);
				len_esc = encode_escape(odbc_esc, pbind[i].buf, len_buf, NULL, MAX_LONG);
				if (len_esc != len_buf)
				{
					sz_esc = xsalloc(len_esc + 1);
					encode_escape(odbc_esc, pbind[i].buf, len_buf, sz_esc, len_esc);

					varstr_cat(vs, sz_esc, len_esc);
					xsfree(sz_esc);
				}
				else
				{
					varstr_cat(vs, (tchar_t*)(pbind[i].buf), len_buf);
				}
			}
			varstr_cat(vs, feed, 1);
		}

		varstr_cat(vs, feed + 1, 2);
		stream_write_line(stream, vs, &pos);

		varstr_empty(vs);
	}

	varstr_empty(vs);
	stream_write_line(stream, vs, &pos);

	varstr_free(vs);
	vs = NULL;

	if (rt == SQL_ERROR)
	{
		_raise_stmt_error(stmt);
	}

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	for (i = 0; i < cols; i++)
	{
		if (pbind[i].buf)
		{
			xmem_free(pbind[i].buf);
			pbind[i].buf = NULL;
			pbind[i].len = 0;
		}
	}
	xmem_free(pbind);

	END_CATCH;

	return 1;

ONERROR:

	if (stmt)
	{
		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	if (vs)
	{
		varstr_free(vs);
	}

	if (pbind)
	{
		for (i = 0; i < cols; i++)
		{
			if (pbind[i].buf)
			{
				xmem_free(pbind[i].buf);
				pbind[i].buf = NULL;
				pbind[i].len = 0;
			}
		}
		xmem_free(pbind);
	}

	return 0;
}

bool_t __stdcall db_import(xhand_t db, xhand_t stream, const tchar_t* table)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLLEN rows = 0;
	SQLPOINTER ind = 0;
	SQLRETURN rt;

	xhand_t vs = NULL;
	dword_t dw = 0;
	xhand_t vs_sql = NULL;
	const tchar_t* token;
	int tklen, i, cols = 0;

	bindguid_t* pbind = NULL;

	tchar_t* sz_esc = NULL;
	int len_esc = 0;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	stream_read_utfbom(stream, NULL);

	vs = varstr_alloc();

	if (!stream_read_line(stream, vs, &dw))
	{
		raise_user_error(_T("-1"), _T("read stream failed"));
	}

	vs_sql = varstr_alloc();

	varstr_printf(vs_sql, _T("insert into %s ("), table);

	cols = 0;
	token = varstr_ptr(vs);
	while (*token != TXT_LINEFEED && *token != _T('\0'))
	{
		tklen = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			tklen++;
		}

		varstr_cat(vs_sql, token - tklen, tklen);
		varstr_cat(vs_sql, _T(","), 1);

		if (*token == TXT_ITEMFEED)
			token++;

		cols++;
	}

	if (!cols)
	{
		raise_user_error(_T("-1"), _T("empty sql statement"));
	}

	tklen = varstr_len(vs_sql);
	varstr_set_char(vs_sql, tklen - 1, _T(')'));

	varstr_cat(vs_sql, _T(" values ("), -1);

	for (i = 0; i < cols; i++)
	{
		varstr_cat(vs_sql, _T("?,"), 2);
	}

	tklen = varstr_len(vs_sql);
	varstr_set_char(vs_sql, tklen - 1, _T(')'));

	rt = SQLPrepare(stmt, (SQLTCHAR*)varstr_ptr(vs_sql), SQL_NTS);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	varstr_free(vs_sql);
	vs_sql = NULL;

	pbind = (bindguid_t*)xmem_alloc(cols * sizeof(bindguid_t));

	varstr_empty(vs);

	while (1)
	{
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			raise_user_error(_T("-1"), _T("stream read line failed"));
		}

		if (!dw)
			break;

		i = 0;
		token = varstr_ptr(vs);
		while (*token != TXT_LINEFEED && *token != _T('\0'))
		{
			tklen = 0;
			while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
			{
				token++;
				tklen++;
			}

			len_esc = decode_escape(token - tklen, tklen, NULL, MAX_LONG);
			if (len_esc != tklen)
			{
				sz_esc = xsalloc(len_esc + 1);
				decode_escape(token - tklen, tklen, sz_esc, len_esc);

				pbind[i].len = (len_esc + 1) * sizeof(tchar_t);
				pbind[i].buf = (byte_t*)sz_esc;

				if (len_esc)
					pbind[i].ind = len_esc * sizeof(tchar_t);
				else
					pbind[i].ind = SQL_NULL_DATA;
			}
			else
			{
				pbind[i].len = (tklen + 1) * sizeof(tchar_t);
				pbind[i].buf = (byte_t*)xmem_alloc((tklen + 1) * sizeof(tchar_t));
				xmem_copy((void*)(pbind[i].buf), (void*)(token - tklen), tklen * sizeof(tchar_t));

				if (tklen)
					pbind[i].ind = tklen * sizeof(tchar_t);
				else
					pbind[i].ind = SQL_NULL_DATA;
			}

			if (*token == TXT_ITEMFEED)
				token++;

			if (++i == cols)
				break;
		}

		while (i < cols)
		{
			tklen = 0;
			pbind[i].len = (tklen + 1) * sizeof(tchar_t);
			pbind[i].buf = (byte_t*)xmem_alloc((tklen + 1) * sizeof(tchar_t));
			pbind[i].ind = SQL_NULL_DATA;

			if (++i == cols)
				break;
		}

		varstr_empty(vs);

		for (i = 0; i < cols; i++)
		{
			SQLBindParameter(stmt, i + 1, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, pbind[i].len, 0, (SQLPOINTER)(pbind[i].buf), pbind[i].len, &(pbind[i].ind));
		}

		rt = SQLExecute(stmt);

		if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
		{
			_raise_stmt_error(stmt);
		}

		for (i = 0; i < cols; i++)
		{
			xmem_free(pbind[i].buf);
			pbind[i].buf = NULL;
			pbind[i].len = 0;
		}

		SQLRowCount(stmt, &rows);
		pdb->rows += rows;
	}

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	for (i = 0; i < cols; i++)
	{
		if (pbind[i].buf)
			xmem_free(pbind[i].buf);
	}
	xmem_free(pbind);
	pbind = NULL;

	varstr_free(vs);
	vs = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stmt)
	{
		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	if (vs_sql)
		varstr_free(vs_sql);

	if (vs)
		varstr_free(vs);

	if (pbind)
	{
		for (i = 0; i < cols; i++)
		{
			if (pbind[i].buf)
				xmem_free(pbind[i].buf);
		}
		xmem_free(pbind);
	}

	return 0;
}

bool_t __stdcall db_batch(xhand_t db, xhand_t stream)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLRETURN rt;
	SQLLEN rows;

	dword_t dw;
	bool_t b_uni = 0;
	tchar_t *sql = NULL;
	int len;
	int encode = 0;

	xhand_t vs = NULL;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	_db_tran(pdb);

	stream_read_utfbom(stream, NULL);

	vs = varstr_alloc();

	while (1)
	{
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			raise_user_error(_T("-1"), _T("stream read line failed"));
		}

		if (!dw)
			break;

		if (compare_text(varstr_ptr(vs), xslen(SQL_GO), SQL_GO, -1, 1) == 0)
		{
			_db_commit(pdb);

			if (pdb->log)
			{
				stream_write(pdb->log, varstr_ptr(vs), varstr_len(vs), NULL);
			}

			varstr_empty(vs);

			_db_tran(pdb);

			continue;
		}

		b_uni = 0;
		len = varstr_len(vs);
		if (len > 1)
		{
			sql = (tchar_t*)varstr_ptr(vs);

			if (sql[len - 1] == _T('\r'))
			{
				varstr_set_char(vs, len - 1, _T('\0'));
				len--;
				b_uni = 1;
			}
			else if (sql[len - 2] == _T('\r'))
			{
				varstr_set_char(vs, len - 2, _T('\0'));
				len -= 2;
				b_uni = 1;
			}
		}

		rt = SQLExecDirect(stmt, (SQLTCHAR*)varstr_ptr(vs), len);

		if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, varstr_ptr(vs), varstr_len(vs), NULL);
			}

			_raise_stmt_error(stmt);
		}

		rows = 0;
		SQLRowCount(stmt, &rows);

		if (!rows && b_uni)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, varstr_ptr(vs), varstr_len(vs), NULL);
			}

			raise_user_error(_T("-1"), ERR_TEXT_ROWCANCEL);
		}
		else if (rows > 1 && b_uni)
		{
			if (pdb->log)
			{
				stream_write(pdb->log, varstr_ptr(vs), varstr_len(vs), NULL);
			}

			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}

		pdb->rows += (int)rows;

		varstr_empty(vs);

		SQLFreeStmt(stmt, SQL_CLOSE);
	}

	varstr_free(vs);
	vs = NULL;

	_db_commit(pdb);

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (vs)
		varstr_free(vs);

	if (stmt)
	{
		_db_rollback(pdb);

		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t _stdcall db_read_xdoc(xhand_t db, link_t_ptr dom, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLRETURN rt;
	SQLSMALLINT cols = 0;
	SQLLEN rows = 0;
	SQLLEN dlen = 0;

	lob_t ob = { 0 };
	xhand_t d_stream = NULL;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(dom != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	rt = SQLExecDirect(stmt, (SQLTCHAR*)sqlstr, xslen(sqlstr));

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	rt = SQLNumResultCols(stmt, &cols);
	if (!cols)
	{
		_raise_stmt_error(stmt);
	}

	rt = SQLFetch(stmt);

	if (rt == SQL_ERROR)
	{
		_raise_stmt_error(stmt);
	}

	if (rt != SQL_NO_DATA)
	{
		dlen = 0;
		rt = SQLGetData(stmt, 1, SQL_C_BINARY, (SQLPOINTER)(0x01), 0, &dlen);
		if (rt == SQL_ERROR)
		{
			_raise_stmt_error(stmt);
		}

		if (dlen == SQL_NULL_DATA)
		{
			dlen = 0;
		}
		else if (dlen == SQL_NO_TOTAL)
		{
			dlen = MAX_LONG;
		}

		if (dlen)
		{
			ob.col = 1;
			ob.type = SQL_C_BINARY;
			ob.stmt = stmt;

			d_stream = stream_alloc(NULL);
			stream_attach(d_stream, &ob.head, _lob_read, NULL);
			stream_set_size(d_stream, dlen);
			stream_set_encode(d_stream, _UTF16_LIT);

			if (!parse_dom_doc_from_stream(dom, d_stream))
			{
				raise_user_error(_T("-1"), _T("parse xml doc failed"));
			}

			stream_free(d_stream);
			d_stream = NULL;
		}
	}

	SQLRowCount(stmt, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_stream)
		stream_free(d_stream);

	if (stmt)
	{
		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t _stdcall db_write_xdoc(xhand_t db, link_t_ptr dom, const tchar_t* sqlfmt)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLLEN rows = 0;
	SQLPOINTER pid = 0;
	SQLLEN ne = 0;
	SQLRETURN rt;
	SQLUINTEGER size = 0;

	lob_t ob = { 0 };
	xhand_t d_stream = NULL;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(dom != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	rt = SQLPrepare(stmt, (SQLTCHAR*)sqlfmt, SQL_NTS);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	size = format_dom_doc_to_buffer(dom, NULL, MAX_LONG, _UTF16_LIT);
	ne = SQL_LEN_DATA_AT_EXEC(0);
	SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_WLONGVARCHAR, size, 0, (SQLPOINTER)1, 0, &ne);

	_db_tran(pdb);

	rt = SQLExecute(stmt);
	if (rt == SQL_ERROR)
	{
		_raise_stmt_error(stmt);
	}

	while (SQL_NEED_DATA == SQLParamData(stmt, &pid))
	{
		if ((int)pid == 1)
		{
			ob.col = 1;
			ob.type = SQL_C_BINARY;
			ob.stmt = stmt;

			d_stream = stream_alloc(NULL);
			stream_attach(d_stream, &ob.head, NULL, _lob_write);
			stream_set_size(d_stream, size);
			stream_set_encode(d_stream, _UTF16_LIT);

			if(!format_dom_doc_to_stream(dom, d_stream))
			{
				raise_user_error(_T("-1"), _T("format xml doc failed"));
			}

			stream_free(d_stream);
			d_stream = NULL;
		}
	}

	SQLRowCount(stmt, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	if (!rows)
	{
		_raise_stmt_error(stmt);
	}

	_db_commit(pdb);

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_stream)
		stream_free(d_stream);

	if (stmt)
	{
		_db_rollback(pdb);

		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t _stdcall db_read_blob(xhand_t db, xhand_t stream, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLRETURN rt;
	SQLSMALLINT cols = 0;
	SQLLEN rows = 0;
	SQLLEN dlen = 0;

	lob_t ob = { 0 };
	xhand_t d_stream = NULL;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	rt = SQLExecDirect(stmt, (SQLTCHAR*)sqlstr, xslen(sqlstr));
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	rt = SQLNumResultCols(stmt, &cols);
	if (!cols)
	{
		_raise_stmt_error(stmt);
	}

	rt = SQLFetch(stmt);

	if (rt == SQL_ERROR)
	{
		_raise_stmt_error(stmt);
	}

	if (rt != SQL_NO_DATA)
	{
		dlen = 0;
		rt = SQLGetData(stmt, 1, SQL_C_BINARY, (SQLPOINTER)(0x01), 0, &dlen);
		if (rt == SQL_ERROR)
		{
			_raise_stmt_error(stmt);
		}
		if (dlen == SQL_NULL_DATA)
		{
			dlen = 0;
		}
		else if (dlen == SQL_NO_TOTAL)
		{
			dlen = MAX_LONG;
		}

		if (dlen)
		{
			ob.col = 1;
			ob.type = SQL_C_BINARY;
			ob.stmt = stmt;

			d_stream = stream_alloc(NULL);
			stream_attach(d_stream, &ob.head, _lob_read, NULL);
			stream_set_size(d_stream, dlen);

			stream_copy(d_stream, stream);

			stream_free(d_stream);
			d_stream = NULL;
		}
	}

	SQLRowCount(stmt, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_stream)
		stream_free(d_stream);

	if (stmt)
	{
		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t _stdcall db_write_blob(xhand_t db, xhand_t stream, const tchar_t* sqlfmt)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLLEN ne = 0;
	SQLLEN rows = 0;
	SQLPOINTER pid = 0;
	SQLRETURN rt;

	lob_t ob = { 0 };
	xhand_t d_stream = NULL;
	dword_t size = 0;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	rt = SQLPrepare(stmt, (SQLTCHAR*)sqlfmt, SQL_NTS);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	size = stream_get_size(stream);
	if (!size)
	{
		size = MAX_LONG;
	}
	ne = SQL_LEN_DATA_AT_EXEC(0);
	SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, size, 0, (SQLPOINTER)1, 0, &ne);

	_db_tran(pdb);

	rt = SQLExecute(stmt);

	if (rt == SQL_ERROR)
	{
		_raise_stmt_error(stmt);
	}

	while (SQL_NEED_DATA == SQLParamData(stmt, &pid))
	{
		if ((int)pid == 1)
		{
			ob.col = 1;
			ob.type = SQL_C_BINARY;
			ob.stmt = stmt;

			d_stream = stream_alloc(NULL);
			stream_attach(d_stream, &ob.head, NULL, _lob_write);
			stream_set_size(d_stream, size);

			stream_copy(stream, d_stream);

			stream_free(d_stream);
			d_stream = NULL;
		}
	}

	SQLRowCount(stmt, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	if (!rows)
	{
		_raise_stmt_error(stmt);
	}

	_db_commit(pdb);

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (d_stream)
		stream_free(d_stream);

	if (stmt)
	{
		_db_rollback(pdb);

		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t _stdcall db_read_clob(xhand_t db, xhand_t varstr, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLRETURN rt;
	SQLSMALLINT cols = 0;
	SQLLEN rows = 0;
	SQLLEN dlen = 0;

	SQLSMALLINT namelen;
	SQLSMALLINT datatype;
	SQLSMALLINT scale;
	SQLSMALLINT nullable;
	SQLULEN colsize = 0;

	tchar_t colname[MAX_SQL_NAME] = { 0 };
	tchar_t coltype[INT_LEN + 1] = { 0 };

	dword_t size = 0;
	tchar_t* buf = NULL;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(varstr != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	rt = SQLExecDirect(stmt, (SQLTCHAR*)sqlstr, xslen(sqlstr));

	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	rt = SQLNumResultCols(stmt, &cols);
	if (!cols)
	{
		_raise_stmt_error(stmt);
	}

	rt = SQLFetch(stmt);

	if (rt == SQL_ERROR)
	{
		_raise_stmt_error(stmt);
	}

	if (rt != SQL_NO_DATA)
	{
		SQLDescribeCol(stmt, 1, (SQLTCHAR*)colname, MAX_SQL_NAME, &namelen, &datatype, &colsize, &scale, &nullable);

		odbctodt(datatype, coltype);

		size = 0;
		if (compare_text(coltype, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			size = 2;
		}
		else if (compare_text(coltype, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0 || compare_text(coltype, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			size = NUM_LEN;
		}
		else if (compare_text(coltype, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(coltype, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			size = DATE_LEN;
		}

		if (!size)
		{
			dlen = 0;
			rt = SQLGetData(stmt, 1, SQL_C_BINARY, (SQLPOINTER)(0x01), 0, &dlen);
			if (rt == SQL_ERROR)
			{
				_raise_stmt_error(stmt);
			}
			if (dlen == SQL_NULL_DATA)
			{
				size = 0;
			}
			else if (dlen == SQL_NO_TOTAL)
			{
				size = MAX_SQL_DATA;
			}
			else
			{
				size = dlen / sizeof(tchar_t);
			}
		}

		varstr_empty(varstr);

		buf = varstr_ensure_buf(varstr, size);

		rt = SQLGetData(stmt, 1, SQL_C_TCHAR, (SQLPOINTER)buf, (size + 1) * sizeof(tchar_t), &dlen);
		if (rt == SQL_ERROR)
		{
			_raise_stmt_error(stmt);
		}

		varstr_resize(varstr, -1);
	}

	SQLRowCount(stmt, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stmt)
	{
		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

bool_t _stdcall db_write_clob(xhand_t db, xhand_t varstr, const tchar_t* sqlfmt)
{
	db_t* pdb = (db_t*)db;

	HSTMT stmt = NULL;
	SQLLEN ne = 0;
	SQLLEN rows = 0;
	SQLPOINTER pid = 0;
	SQLRETURN rt;

	int size = 0;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	XDL_ASSERT(varstr != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, pdb->dbc, &stmt))
	{
		_raise_dbc_error(pdb->dbc);
	}

	rt = SQLPrepare(stmt, (SQLTCHAR*)sqlfmt, SQL_NTS);
	if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
	{
		_raise_stmt_error(stmt);
	}

	size = varstr_len(varstr) * sizeof(tchar_t);
	ne = SQL_LEN_DATA_AT_EXEC(0);
	SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_LONGVARCHAR, size, 0, (SQLPOINTER)1, 0, &ne);

	_db_tran(pdb);

	rt = SQLExecute(stmt);

	if (rt == SQL_ERROR)
	{
		_raise_stmt_error(stmt);
	}

	while (SQL_NEED_DATA == SQLParamData(stmt, &pid))
	{
		if ((int)pid == 1)
		{
			rt = SQLPutData(stmt, (SQLPOINTER)varstr_ptr(varstr), size);

			if (rt != SQL_SUCCESS && rt != SQL_SUCCESS_WITH_INFO)
			{
				_raise_stmt_error(stmt);
			}
		}
	}

	SQLRowCount(stmt, &rows);
	pdb->rows = (rows < 0) ? 0 : rows;

	if (!rows)
	{
		_raise_stmt_error(stmt);
	}

	_db_commit(pdb);

	SQLFreeStmt(stmt, SQL_CLOSE);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stmt)
	{
		_db_rollback(pdb);

		SQLFreeStmt(stmt, SQL_CLOSE);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		stmt = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, MAX_ERR);

	return 0;
}

int __stdcall db_rows(xhand_t db)
{
	db_t* pdb = (db_t*)db;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	return pdb->rows;
}

int __stdcall db_error(xhand_t db, tchar_t* buf, int max)
{
	db_t* pdb = (db_t*)db;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	max = (max < MAX_ERR) ? max : MAX_ERR;
	if (buf)
	{
		xsncpy(buf, pdb->err_text, max);
	}

	return -1;
}

void __stdcall db_trace(xhand_t db, XHANDLE stm)
{
	db_t* pdb = (db_t*)db;

	XDL_ASSERT(db && db->tag == _HANDLE_ODBC);

	pdb->log = stm;
}
