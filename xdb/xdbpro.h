/***********************************************************************
	Easily xdb

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdb function document

	@module	xdbpro.h | xdb interface file

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

#ifndef _XDBPRO_H
#define _XDBPRO_H

#include "xdbdef.h"

extern bool_t STDCALL db_parse_dsn(const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len);

extern xdb_t STDCALL db_open_dsn(const tchar_t* dsnfile);
//local file connection: "d:\\somedir\\somedb.dsn"
//http file connection: "http://www.some.com/somedir/somedb.dsn"
//network file connection: "\\\\somehost\\sharedir\\somedb.dsn"

extern xdb_t STDCALL db_open(const tchar_t* srv, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd);

extern void STDCALL db_close(xdb_t db);

extern bool_t STDCALL db_exec(xdb_t db,const tchar_t* sqlstr, int len);

extern bool_t STDCALL db_select(xdb_t db, LINKPTR grid, const tchar_t* sqlstr);

extern bool_t STDCALL db_schema(xdb_t db, LINKPTR grid, const tchar_t* sqlstr);

extern bool_t STDCALL db_fetch(xdb_t db,LINKPTR grid);

extern bool_t STDCALL db_update(xdb_t db,LINKPTR grid);

extern bool_t STDCALL db_datetime(xdb_t db, const tchar_t* sz_when, tchar_t* sz_date);

extern int STDCALL db_rows(xdb_t db);

extern int STDCALL db_error(xdb_t db,tchar_t* buf,int max);

extern int db_call_argv(xdb_t db, const tchar_t* procname, const tchar_t* fmt, ...);
//fmt like: "%10s%-d%+10.2f"

extern bool_t STDCALL db_call_json(xdb_t db, const tchar_t* procname, LINKPTR json);

extern bool_t STDCALL db_call_func(xdb_t db, LINKPTR func);

extern bool_t STDCALL db_export(xdb_t db, stream_t stream, const tchar_t* sqlstr);

extern bool_t STDCALL db_import(xdb_t db, stream_t stream, const tchar_t* table);

extern bool_t STDCALL db_batch(xdb_t db, stream_t stream);

extern bool_t STDCALL db_read_blob(xdb_t db, stream_t stream, const tchar_t* sqlstr);
//sqlstr like: select blobfield from sometable where ...

extern bool_t STDCALL db_write_blob(xdb_t db, stream_t stream, const tchar_t* sqlfmt);
//sqlfmt like: update sometable set blobfield = ? where ...

extern bool_t STDCALL db_read_clob(xdb_t db, string_t varstr, const tchar_t* sqlstr);
//sqlstr like: select clobfield from sometable where ...

extern bool_t STDCALL db_write_clob(xdb_t db, string_t varstr, const tchar_t* sqlfmt);
//sqlfmt like: update sometable set clobfield = ? where ...

extern bool_t STDCALL db_read_xdoc(xdb_t db, LINKPTR domdoc, const tchar_t* sqlstr);
//sqlstr like: select xmlfield from sometable where ...

extern bool_t STDCALL db_write_xdoc(xdb_t db, LINKPTR domdoc, const tchar_t* sqlfmt);
//sqlfmt like: update sometable set xmlfield = ? where ...

extern void STDCALL db_trace(xdb_t db, stream_t stm);

/*********************************xdb export def***************************************************/

typedef bool_t(STDCALL *PF_DB_PARSE_DSN)(const tchar_t*, tchar_t*, int, tchar_t*, int, tchar_t*, int, tchar_t*, int);
typedef xdb_t (STDCALL *PF_DB_OPEN)(const tchar_t*, const tchar_t*, const tchar_t*, const tchar_t*);
typedef xdb_t(STDCALL *PF_DB_OPEN_DSN)(const tchar_t*);
typedef void(STDCALL *PF_DB_CLOSE)(xdb_t);
typedef bool_t(STDCALL *PF_DB_EXEC)(xdb_t, const tchar_t*, int);
typedef bool_t(STDCALL *PF_DB_SELECT)(xdb_t, LINKPTR, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_SCHEMA)(xdb_t, LINKPTR, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_FETCH)(xdb_t, LINKPTR);
typedef bool_t(STDCALL *PF_DB_UPDATE)(xdb_t, LINKPTR);
typedef bool_t(STDCALL *PF_DB_DATETIME)(xdb_t, const tchar_t*, tchar_t*);
typedef int(STDCALL *PF_DB_ROWS)(xdb_t);
typedef int(STDCALL *PF_DB_ERROR)(xdb_t, tchar_t*, int);
typedef void(STDCALL *PF_DB_TRACE)(xdb_t, stream_t);
typedef bool_t(STDCALL *PF_DB_CALL_FUNC)(xdb_t, LINKPTR);
typedef bool_t(STDCALL *PF_DB_CALL_JSON)(xdb_t, const tchar_t*, LINKPTR);
typedef int (*PF_DB_CALL_ARGV)(xdb_t, const tchar_t*, const tchar_t*, ...);
typedef bool_t(STDCALL *PF_DB_EXPORT)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_IMPORT)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_BATCH)(xdb_t, stream_t);
typedef bool_t(STDCALL *PF_DB_WRITE_BLOB)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_BLOB)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_WRITE_CLOB)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_CLOB)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_WRITE_XDOC)(xdb_t, LINKPTR, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_XDOC)(xdb_t, LINKPTR, const tchar_t*);

#endif