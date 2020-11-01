/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdb document

	@module	dbinf.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#include "dbinf.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdlnet.h"
#include "xdldoc.h"

if_xdb_t* alloc_xdb_interface(const tchar_t* libfile)
{
	res_modu_t lib;
	if_xdb_t* pdb;

	lib = load_library(libfile);
	if (!lib)
		return NULL;

	pdb = (if_xdb_t*)xmem_alloc(sizeof(if_xdb_t));

	pdb->lib = lib;
	pdb->pf_db_parse_dsn = (PF_DB_PARSE_DSN)get_address(pdb->lib, "db_parse_dsn");
	pdb->pf_db_open_dsn = (PF_DB_OPEN_DSN)get_address(pdb->lib, "db_open_dsn");
	pdb->pf_db_close = (PF_DB_CLOSE)get_address(pdb->lib, "db_close");

	pdb->pf_db_select = (PF_DB_SELECT)get_address(pdb->lib, "db_select");
	pdb->pf_db_schema = (PF_DB_SCHEMA)get_address(pdb->lib, "db_schema");
	pdb->pf_db_update = (PF_DB_UPDATE)get_address(pdb->lib, "db_update");
	pdb->pf_db_fetch = (PF_DB_FETCH)get_address(pdb->lib, "db_fetch");
	pdb->pf_db_exec = (PF_DB_EXEC)get_address(pdb->lib, "db_exec");
	pdb->pf_db_call_func = (PF_DB_CALL_FUNC)get_address(pdb->lib, "db_call_func");
	pdb->pf_db_call_json = (PF_DB_CALL_JSON)get_address(pdb->lib, "db_call_json");

	pdb->pf_db_export = (PF_DB_EXPORT)get_address(pdb->lib, "db_export");
	pdb->pf_db_import = (PF_DB_IMPORT)get_address(pdb->lib, "db_import");
	pdb->pf_db_batch = (PF_DB_BATCH)get_address(pdb->lib, "db_batch");
	pdb->pf_db_write_blob = (PF_DB_WRITE_BLOB)get_address(pdb->lib, "db_write_blob");
	pdb->pf_db_read_blob = (PF_DB_READ_BLOB)get_address(pdb->lib, "db_read_blob");
	pdb->pf_db_write_clob = (PF_DB_WRITE_CLOB)get_address(pdb->lib, "db_write_clob");
	pdb->pf_db_read_clob = (PF_DB_READ_CLOB)get_address(pdb->lib, "db_read_clob");
	pdb->pf_db_write_xdoc = (PF_DB_WRITE_XDOC)get_address(pdb->lib, "db_write_xdoc");
	pdb->pf_db_read_xdoc = (PF_DB_READ_XDOC)get_address(pdb->lib, "db_read_xdoc");

	pdb->pf_db_datetime = (PF_DB_DATETIME)get_address(pdb->lib, "db_datetime");
	pdb->pf_db_rows = (PF_DB_ROWS)get_address(pdb->lib, "db_rows");
	pdb->pf_db_error = (PF_DB_ERROR)get_address(pdb->lib, "db_error");

	return pdb;
}

void free_xdb_interface(if_xdb_t* pdb)
{
	if (pdb->xdb)
	{
		(*pdb->pf_db_close)(pdb->xdb);
	}

	if (pdb->lib)
	{
		free_library(pdb->lib);
	}

	xmem_free(pdb);
}

bool_t xdb_parse_dsn(if_xdb_t* pdb, const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len)
{
	XDL_ASSERT(pdb && pdb->pf_db_parse_dsn);

	return (*pdb->pf_db_parse_dsn)(dsnfile, srv_buf, srv_len, dbn_buf, dbn_len, uid_buf, uid_len, pwd_buf, pwd_len);
}

bool_t xdb_open_dsn(if_xdb_t* pdb, const tchar_t* dsnfile)
{
	XDL_ASSERT(pdb && pdb->pf_db_open_dsn);

	if (pdb->xdb)
		return 1;

	pdb->xdb = (*pdb->pf_db_open_dsn)(dsnfile);

	return (pdb->xdb) ? 1 : 0;
}

bool_t xdb_open(if_xdb_t* pdb, const tchar_t* srv, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd)
{
	XDL_ASSERT(pdb && pdb->pf_db_open);

	if (pdb->xdb)
		return 1;

	pdb->xdb = (*pdb->pf_db_open)(srv, dbn, uid, pwd);

	return (pdb->xdb) ? 1 : 0;
}

void xdb_close(if_xdb_t* pdb)
{
	XDL_ASSERT(pdb && pdb->pf_db_close);

	if (!pdb->xdb)
		return;

	(*pdb->pf_db_close)(pdb->xdb);
	pdb->xdb = NULL;
}

bool_t xdb_exec(if_xdb_t* pdb, const tchar_t* sqlstr, int len)
{
	XDL_ASSERT(pdb && pdb->pf_db_exec);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_exec)(pdb->xdb, sqlstr, len);
}

bool_t xdb_select(if_xdb_t* pdb, link_t_ptr grid, const tchar_t* sqlstr)
{
	XDL_ASSERT(pdb && pdb->pf_db_select);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_select)(pdb->xdb, grid, sqlstr);
}

bool_t xdb_schema(if_xdb_t* pdb, link_t_ptr grid, const tchar_t* sqlstr)
{
	XDL_ASSERT(pdb && pdb->pf_db_schema);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_schema)(pdb->xdb, grid, sqlstr);
}

bool_t xdb_fetch(if_xdb_t* pdb, link_t_ptr grid)
{
	XDL_ASSERT(pdb && pdb->pf_db_fetch);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_fetch)(pdb->xdb, grid);
}

bool_t xdb_update(if_xdb_t* pdb, link_t_ptr grid)
{
	XDL_ASSERT(pdb && pdb->pf_db_update);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_update)(pdb->xdb, grid);
}

bool_t xdb_datetime(if_xdb_t* pdb, int diff, tchar_t* sz_date)
{
	XDL_ASSERT(pdb && pdb->pf_db_datetime);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_datetime)(pdb->xdb, diff, sz_date);
}

int xdb_rows(if_xdb_t* pdb)
{
	XDL_ASSERT(pdb && pdb->pf_db_rows);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_rows)(pdb->xdb);
}

int xdb_error(if_xdb_t* pdb, tchar_t* buf, int max)
{
	XDL_ASSERT(pdb && pdb->pf_db_error);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_error)(pdb->xdb, buf, max);
}

bool_t xdb_call_json(if_xdb_t* pdb, const tchar_t* procname, link_t_ptr json)
{
	XDL_ASSERT(pdb && pdb->pf_db_call_json);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_call_json)(pdb->xdb, procname, json);
}

bool_t xdb_call_func(if_xdb_t* pdb, link_t_ptr func)
{
	XDL_ASSERT(pdb && pdb->pf_db_call_func);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_call_func)(pdb->xdb, func);
}

bool_t xdb_export(if_xdb_t* pdb, stream_t stream, const tchar_t* sqlstr)
{
	XDL_ASSERT(pdb && pdb->pf_db_export);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_export)(pdb->xdb, stream, sqlstr);
}

bool_t xdb_import(if_xdb_t* pdb, stream_t stream, const tchar_t* table)
{
	XDL_ASSERT(pdb && pdb->pf_db_import);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_import)(pdb->xdb, stream, table);
}

bool_t xdb_batch(if_xdb_t* pdb, stream_t stream)
{
	XDL_ASSERT(pdb && pdb->pf_db_batch);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_batch)(pdb->xdb, stream);
}

bool_t xdb_read_blob(if_xdb_t* pdb, stream_t stream, const tchar_t* sqlstr)
{
	XDL_ASSERT(pdb && pdb->pf_db_read_blob);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_read_blob)(pdb->xdb, stream, sqlstr);
}

bool_t xdb_write_blob(if_xdb_t* pdb, stream_t stream, const tchar_t* sqlfmt)
{
	XDL_ASSERT(pdb && pdb->pf_db_write_blob);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_write_blob)(pdb->xdb, stream, sqlfmt);
}

bool_t xdb_read_clob(if_xdb_t* pdb, string_t varstr, const tchar_t* sqlstr)
{
	XDL_ASSERT(pdb && pdb->pf_db_read_clob);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_read_clob)(pdb->xdb, varstr, sqlstr);
}

bool_t xdb_write_clob(if_xdb_t* pdb, string_t varstr, const tchar_t* sqlfmt)
{
	XDL_ASSERT(pdb && pdb->pf_db_write_clob);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_write_clob)(pdb->xdb, varstr, sqlfmt);
}

bool_t xdb_read_xdoc(if_xdb_t* pdb, link_t_ptr domdoc, const tchar_t* sqlstr)
{
	XDL_ASSERT(pdb && pdb->pf_db_read_xdoc);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_read_xdoc)(pdb->xdb, domdoc, sqlstr);
}

bool_t xdb_write_xdoc(if_xdb_t* pdb, link_t_ptr domdoc, const tchar_t* sqlfmt)
{
	XDL_ASSERT(pdb && pdb->pf_db_write_xdoc);

	if (!pdb->xdb)
		return 0;

	return (*pdb->pf_db_write_xdoc)(pdb->xdb, domdoc, sqlfmt);
}