/***********************************************************************
	Easily xDesign v3.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xDesign document

	@module	xDesign implement file

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

#include "stdafx.h"
#include "_Database.h"
#include "_Module.h"
#include "_Project.h"


DBCTX*	 _CreateDBCtx(void);
void	 _DestroyDBCtx(DBCTX* pct);

bool_t	_DBOpenDSN(DBCTX* pct, const tchar_t* srv, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd);
void	_DBClose(DBCTX* pct);
bool_t	_DBExec(DBCTX* pct, const tchar_t* sql);
bool_t	_DBSelect(DBCTX* pct, link_t_ptr grid, const tchar_t* sql);
bool_t	_DBSchema(DBCTX* pct, link_t_ptr grid, const tchar_t* sql);
bool_t	_DBFetch(DBCTX* pct, link_t_ptr grid);
bool_t	_DBUpdate(DBCTX* pct, link_t_ptr grid);
int		_DBRows(DBCTX* pct);
short	_DBCallFun(DBCTX* pct, link_t_ptr func);
int		_DBError(DBCTX* pct, tchar_t* buf, int max);
void	_DBShowError(DBCTX* pct, HWND hWnd);

DBCTX* _CreateDBCtx(void)
{
	HMODULE lib;
	
	lib = load_library(_T("xdb_stub.dll"));

	if (!lib)
		return NULL;

	DBCTX* pct = (DBCTX*)xmem_alloc(sizeof(DBCTX));

	pct->pf_db_open = (PF_DB_OPEN)GetProcAddress(lib, "db_open");
	pct->pf_db_close = (PF_DB_CLOSE)GetProcAddress(lib, "db_close");
	pct->pf_db_exec = (PF_DB_EXEC)GetProcAddress(lib, "db_exec");
	pct->pf_db_select = (PF_DB_SELECT)GetProcAddress(lib, "db_select");
	pct->pf_db_schema = (PF_DB_SCHEMA)GetProcAddress(lib, "db_schema");
	pct->pf_db_fetch = (PF_DB_FETCH)GetProcAddress(lib, "db_fetch");
	pct->pf_db_update = (PF_DB_UPDATE)GetProcAddress(lib, "db_update");
	pct->pf_db_batch = (PF_DB_BATCH)GetProcAddress(lib, "db_batch");
	pct->pf_db_rows = (PF_DB_ROWS)GetProcAddress(lib, "db_rows");
	pct->pf_db_call_func = (PF_DB_CALL_FUNC)GetProcAddress(lib, "db_call_func");
	pct->pf_db_error = (PF_DB_ERROR)GetProcAddress(lib, "db_error");

	pct->hdb = lib;

	return pct;
}

void _DestroyDBCtx(DBCTX* pct)
{
	XDL_ASSERT(pct);

	if (pct->xdb)
		_DBClose(pct);

	if (pct->hdb)
		free_library(pct->hdb);

	xmem_free(pct);
}

bool_t _DBOpenDSN(DBCTX* pct)
{
	XDL_ASSERT(pct && pct->pf_db_open);

	if (pct->xdb)
		_DBClose(pct);

	widget_set_cursor(NULL, CURSOR_WAIT);

	pct->xdb = (*(pct->pf_db_open))(pct->szRDS, pct->szDBN, pct->szSID, pct->szKEY);

	widget_set_cursor(NULL, CURSOR_ARROW);

	return (pct->xdb) ? 1 : 0;
}

void _DBClose(DBCTX* pct)
{
	XDL_ASSERT(pct && pct->pf_db_close);

	if (pct->xdb)
		(*(pct->pf_db_close))(pct->xdb);
	pct->xdb = NULL;
}

bool_t _DBExec(DBCTX* pct, const tchar_t* sql)
{
	XDL_ASSERT(pct && pct->pf_db_exec);
	XDL_ASSERT(pct->xdb);

	widget_set_cursor(NULL, CURSOR_WAIT);

	bool_t rt = (*(pct->pf_db_exec))(pct->xdb, sql, -1);
	
	widget_set_cursor(NULL, CURSOR_ARROW);

	return rt;
}

bool_t _DBBatch(DBCTX* pct, stream_t stm)
{
	XDL_ASSERT(pct && pct->pf_db_batch);
	XDL_ASSERT(pct->xdb);

	widget_set_cursor(NULL, CURSOR_WAIT);

	bool_t rt = (*(pct->pf_db_batch))(pct->xdb, stm);

	widget_set_cursor(NULL, CURSOR_ARROW);

	return rt;
}

bool_t _DBSchema(DBCTX* pct, link_t_ptr grid, const tchar_t* sql)
{
	XDL_ASSERT(pct && pct->pf_db_schema);
	XDL_ASSERT(pct->xdb);

	widget_set_cursor(NULL, CURSOR_WAIT);

	bool_t rt = (*(pct->pf_db_schema))(pct->xdb, grid, sql);
	
	widget_set_cursor(NULL, CURSOR_ARROW);

	return rt;
}

bool_t _DBSelect(DBCTX* pct, link_t_ptr grid, const tchar_t* sql)
{
	XDL_ASSERT(pct && pct->pf_db_select);
	XDL_ASSERT(pct->xdb);

	widget_set_cursor(NULL, CURSOR_WAIT);

	bool_t rt = (*(pct->pf_db_select))(pct->xdb, grid, sql);
	
	widget_set_cursor(NULL, CURSOR_ARROW);

	return rt;
}

bool_t _DBFetch(DBCTX* pct, link_t_ptr grid)
{
	XDL_ASSERT(pct && pct->pf_db_fetch);
	XDL_ASSERT(pct->xdb);

	widget_set_cursor(NULL, CURSOR_WAIT);

	bool_t rt = (*(pct->pf_db_fetch))(pct->xdb, grid);
	
	widget_set_cursor(NULL, CURSOR_ARROW);

	return rt;
}

bool_t _DBUpdate(DBCTX* pct, link_t_ptr grid)
{
	XDL_ASSERT(pct && pct->pf_db_update);
	XDL_ASSERT(pct->xdb);

	widget_set_cursor(NULL, CURSOR_WAIT);

	bool_t rt = (*(pct->pf_db_update))(pct->xdb, grid);
	
	widget_set_cursor(NULL, CURSOR_ARROW);

	return rt;
}

int _DBRows(DBCTX* pct)
{
	XDL_ASSERT(pct && pct->pf_db_rows);
	XDL_ASSERT(pct->xdb);

	return (*(pct->pf_db_rows))(pct->xdb);
}

short _DBCall(DBCTX* pct, LINKPTR ptr_fun)
{
	XDL_ASSERT(pct && pct->pf_db_call_func);
	XDL_ASSERT(pct->xdb);

	widget_set_cursor(NULL, CURSOR_WAIT);

	short rt = (*(pct->pf_db_call_func))(pct->xdb, ptr_fun);
	
	widget_set_cursor(NULL, CURSOR_ARROW);

	return rt;
}

int _DBError(DBCTX* pct, tchar_t* buf, int max)
{
	XDL_ASSERT(pct && pct->pf_db_error);
	XDL_ASSERT(pct->xdb);

	return (*(pct->pf_db_error))(pct->xdb, buf, max);
}

void _DBShowError(DBCTX* pct, HWND hWnd)
{
	XDL_ASSERT(pct && pct->pf_db_error);
	XDL_ASSERT(pct->xdb);

	tchar_t szErr[ERR_LEN + 1] = { 0 };
	(*(pct->pf_db_error))(pct->xdb, szErr, ERR_LEN);

	ShowMsg(MSGICO_ERR, szErr);
}

void _DBConfig(const tchar_t* conn, tchar_t* szRDS, tchar_t* szDBN, tchar_t* szSID, tchar_t* szKEY)
{
	LINKPTR ptr_str = create_string_table(0);

	string_table_parse_attrset(ptr_str, conn, -1);

	LINKPTR slk = get_string_next_entity(ptr_str, LINK_FIRST);
	while (slk)
	{
		if (compare_text(get_string_entity_key_ptr(slk), -1, _T("SERVICE"), -1, 1) == 0)
		{
			get_string_entity_val(slk, szRDS, PATH_LEN);
		}
		else if (compare_text(get_string_entity_key_ptr(slk), -1, _T("DATABASE"), -1, 1) == 0)
		{
			get_string_entity_val(slk, szDBN, PATH_LEN);
		}
		else if (compare_text(get_string_entity_key_ptr(slk), -1, _T("SECRET-ID"), -1, 1) == 0)
		{
			get_string_entity_val(slk, szSID, KEY_LEN);
		}
		else if (compare_text(get_string_entity_key_ptr(slk), -1, _T("SECRET-KEY"), -1, 1) == 0)
		{
			get_string_entity_val(slk, szKEY, KEY_LEN);
		}

		slk = get_string_next_entity(ptr_str, slk);
	}

	destroy_string_table(ptr_str);
}

//////////////////////////////////////////////////////////////////////////////

DBCTX* DBOpen(const tchar_t* conn)
{
	DBCTX* pct = NULL;

	tchar_t szERR[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	pct = _CreateDBCtx();
	if (!pct)
	{
		raise_user_error(_T("-1"), _T("初始化数据源失败！"));
	}

	_DBConfig(conn, pct->szRDS, pct->szDBN, pct->szSID, pct->szKEY);

	END_CATCH;

	return pct;

ONERROR:

	if (pct)
		_DestroyDBCtx(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return NULL;
}

void DBClose(DBCTX* pct)
{
	_DestroyDBCtx(pct);
}

bool_t DBExec(DBCTX* pct, const tchar_t* sz_sql)
{
	tchar_t szERR[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	if (!_DBOpenDSN(pct))
	{
		raise_user_error(_T("-1"), _T("连接数据库失败！"));
	}

	if (!_DBExec(pct, sz_sql))
	{
		_DBError(pct, szERR, ERR_LEN);
		raise_user_error(_T("-1"), szERR);
	}

	_DBClose(pct);

	END_CATCH;

	return 1;

ONERROR:

	_DBClose(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return 0;
}

bool_t DBBatch(DBCTX* pct, stream_t stm)
{
	tchar_t szERR[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	if (!_DBOpenDSN(pct))
	{
		raise_user_error(_T("-1"), _T("连接数据库失败！"));
	}

	if (!_DBBatch(pct, stm))
	{
		_DBError(pct, szERR, ERR_LEN);
		raise_user_error(_T("-1"), szERR);
	}

	_DBClose(pct);

	END_CATCH;

	return 1;

ONERROR:

	_DBClose(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return 0;
}

bool_t DBSelect(DBCTX* pct, link_t_ptr grid, const tchar_t* sz_sql)
{
	tchar_t szERR[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	if (!_DBOpenDSN(pct))
	{
		raise_user_error(_T("-1"), _T("连接数据库失败！"));
	}

	if (!_DBSelect(pct, grid, sz_sql))
	{
		_DBError(pct, szERR, ERR_LEN);
		raise_user_error(_T("-1"), szERR);
	}

	_DBClose(pct);

	END_CATCH;

	return 1;

ONERROR:

	_DBClose(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return 0;
}

bool_t DBSchema(DBCTX* pct, link_t_ptr grid, const tchar_t* sz_sql)
{
	tchar_t szERR[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	if (!_DBOpenDSN(pct))
	{
		raise_user_error(_T("-1"), _T("连接数据库失败！"));
	}

	if (!_DBSchema(pct, grid, sz_sql))
	{
		_DBError(pct, szERR, ERR_LEN);
		raise_user_error(_T("-1"), szERR);
	}

	_DBClose(pct);

	END_CATCH;

	return 1;

ONERROR:

	_DBClose(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return 0;
}

bool_t DBFetch(DBCTX* pct, link_t_ptr grid)
{
	tchar_t szERR[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	if (!_DBOpenDSN(pct))
	{
		raise_user_error(_T("-1"), _T("连接数据库失败！"));
	}

	if (!_DBFetch(pct, grid))
	{
		_DBError(pct, szERR, ERR_LEN);
		raise_user_error(_T("-1"), szERR);
	}

	_DBClose(pct);

	END_CATCH;

	return 1;

ONERROR:

	_DBClose(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return 0;
}

bool_t DBUpdate(DBCTX* pct, link_t_ptr grid)
{
	tchar_t szERR[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	if (!_DBOpenDSN(pct))
	{
		raise_user_error(_T("-1"), _T("连接数据库失败！"));
	}

	if (!_DBUpdate(pct, grid))
	{
		_DBError(pct, szERR, ERR_LEN);
		raise_user_error(_T("-1"), szERR);
	}

	_DBClose(pct);

	END_CATCH;

	return 1;

ONERROR:

	_DBClose(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return 0;
}

short DBCall(DBCTX* pct, link_t_ptr func)
{
	tchar_t szERR[ERR_LEN + 1] = { 0 };

	short nRet = -1;

	TRY_CATCH;

	if (!_DBOpenDSN(pct))
	{
		raise_user_error(_T("-1"), _T("连接数据库失败！"));
	}

	nRet = _DBCall(pct, func);

	if (nRet < 0)
	{
		_DBError(pct, szERR, ERR_LEN);
		raise_user_error(_T("-1"), szERR);
	}

	_DBClose(pct);

	END_CATCH;

	return nRet;

ONERROR:

	_DBClose(pct);

	get_last_error(NULL, szERR, ERR_LEN);
	ShowMsg(MSGICO_ERR, szERR);

	return nRet;
}