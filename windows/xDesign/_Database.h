/***********************************************************************
	Easily xDesign v3.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xDesign interface document

	@module	xDesign interface file

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

#pragma once

typedef struct _DBCTX{
	HMODULE hdb;

	XDB xdb;

	PF_DB_OPEN		pf_db_open;
	PF_DB_CLOSE		pf_db_close;
	PF_DB_EXEC		pf_db_exec;
	PF_DB_SELECT	pf_db_select;
	PF_DB_SCHEMA	pf_db_schema;
	PF_DB_FETCH		pf_db_fetch;
	PF_DB_UPDATE	pf_db_update;
	PF_DB_BATCH		pf_db_batch;
	PF_DB_ROWS		pf_db_rows;
	PF_DB_ERROR		pf_db_error;
	PF_DB_CALL_FUNC	pf_db_call_func;
	PF_DB_WRITE_BLOB	pf_db_write_blob;
	PF_DB_READ_BLOB		pf_db_read_blob;
	PF_DB_WRITE_CLOB	pf_db_write_clob;
	PF_DB_READ_CLOB		pf_db_read_clob;
	PF_DB_WRITE_XDOC	pf_db_write_xdoc;
	PF_DB_READ_XDOC		pf_db_read_xdoc;

	tchar_t szRDS[PATH_LEN];
	tchar_t szDBN[RES_LEN];
	tchar_t szSID[KEY_LEN];
	tchar_t szKEY[KEY_LEN];
}DBCTX;

DBCTX*		DBOpen(const tchar_t* conn);

void		DBClose(DBCTX* pctx);

bool_t		DBExec(DBCTX* pctx, const tchar_t* sql);

bool_t		DBBatch(DBCTX* pctx, stream_t stm);

bool_t		DBSelect(DBCTX* pctx, link_t_ptr grid, const tchar_t* sql);

bool_t		DBSchema(DBCTX* pctx, link_t_ptr grid, const tchar_t* sql);

bool_t		DBFetch(DBCTX* pctx, link_t_ptr grid);

bool_t		DBUpdate(DBCTX* pctx, link_t_ptr grid);

short		DBCall(DBCTX* pctx, link_t_ptr func);