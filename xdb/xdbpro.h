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

#ifdef	__cplusplus
extern "C" {
#endif
    
/*
@FUNCTION db_parse_dsn: parse database connect parameter from dsn file.
@INPUT const tchar_t*: the dsn file name.
@OUTPUT tchar_t* srv_buf: the string buffer for returning server name.
@INPUT int srv_len: the server name string buffer size in characters.
@OUTPUT tchar_t* dbn_buf: the string buffer for returning database name.
@INPUT int dbn_len: the database name string buffer size in characters.
@OUTPUT tchar_t* uid_buf: the string buffer for returning user name.
@INPUT int uid_len: the user name string buffer size in characters.
@OUTPUT tchar_t* pwd_buf: the string buffer for returning password.
@INPUT int pwd_len: the password string buffer size in characters.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_parse_dsn(const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len);

/*
@FUNCTION db_open_dsn: connect to database by dsn file.
@INPUT const tchar_t* dsnfile: the dsn file name. eg: 
local file connection: "d:\\somedir\\somedb.dsn", 
http file connection: "http://www.some.com/somedir/somedb.dsn", 
network file connection: "\\\\somehost\\sharedir\\somedb.dsn"
@RETURN xdb_t: if succeeds return xdb handle, fails return NULL.
*/
extern xdb_t STDCALL db_open_dsn(const tchar_t* dsnfile);

/*
@FUNCTION db_open: connect to database by parameters.
@INPUT const tchar_t* srv: the server name token.
@INPUT const tchar_t* dbn: the database name token.
@INPUT const tchar_t* uid: the user name token.
@INPUT const tchar_t* pwd: the password token.
@RETURN xdb_t: if succeeds return xdb handle, fails return NULL.
*/
extern xdb_t STDCALL db_open(const tchar_t* srv, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd);

/*
@FUNCTION db_close: close the database connection.
@INPUT xdb_t db: the xdb handle.
@RETURN void: none.
*/
extern void STDCALL db_close(xdb_t db);

/*
@FUNCTION db_exec: batch executing the sql statement.
@INPUT xdb_t db: the xdb handle.
@INPUT const tchar_t* sqlstr: the sqlstr statement.
@INPUT int len: the sql token length in characters.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_exec(xdb_t db,const tchar_t* sqlstr, int len);

/*
@FUNCTION db_select: generating grid col set and row set from database by sql statement.
@INPUT xdb_t db: the xdb handle.
@OUTPUT LINKPTR grid: the grid link component.
@INPUT const tchar_t* sqlstr: the select sqlstr statement.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_select(xdb_t db, LINKPTR grid, const tchar_t* sqlstr);

/*
@FUNCTION db_schema: generating grid col set from database by sql statement.
@INPUT xdb_t db: the xdb handle.
@OUTPUT LINKPTR grid: the grid link component.
@INPUT const tchar_t* sqlstr: the select sqlstr statement.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_schema(xdb_t db, LINKPTR grid, const tchar_t* sqlstr);

/*
@FUNCTION db_schema: generating grid row set from database by col set defination.
@INPUT xdb_t db: the xdb handle.
@OUTPUT LINKPTR grid: the grid link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_fetch(xdb_t db,LINKPTR grid);

/*
@FUNCTION db_update: commit grid row set update sql statement to database.
@INPUT xdb_t db: the xdb handle.
@OUTPUT LINKPTR grid: the grid link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_update(xdb_t db,LINKPTR grid);

/*
@FUNCTION db_datetime: get datetime from database.
@INPUT xdb_t db: the xdb handle.
@INPUT int diff: the diff day defination.
@OUTPUT tchar_t* sz_date: the string buffer for returning date token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_datetime(xdb_t db, int diff, tchar_t* sz_date);

/*
@FUNCTION db_rows: get rows affected.
@INPUT xdb_t db: the xdb handle.
@RETURN int: return the rows.
*/
extern int STDCALL db_rows(xdb_t db);

/*
@FUNCTION db_error: get rows affected.
@INPUT xdb_t db: the xdb handle.
@RETURN int: return the rows.
*/
extern int STDCALL db_error(xdb_t db,tchar_t* buf,int max);

/*
@FUNCTION db_call_argv: call database procedure using parameters.
@INPUT xdb_t db: the xdb handle.
@INPUT const tchar_t* procname: the procedure name.
@INPUT const tchar_t* fmt: the format token, eg: "%10s%-d%+10.2f".
@INPUT ...
@RETURN int: return the result, C_ERR(-1) indicate error raised.
*/
extern int db_call_argv(xdb_t db, const tchar_t* procname, const tchar_t* fmt, ...);

/*
@FUNCTION db_call_json: call database procedure using json document.
@INPUT xdb_t db: the xdb handle.
@INPUT const tchar_t* procname: the procedure name.
@INPUT LINKPTR json: the json document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_call_json(xdb_t db, const tchar_t* procname, LINKPTR json);

/*
@FUNCTION db_call_func: call database procedure using function document.
@INPUT xdb_t db: the xdb handle.
@INPUT const tchar_t* procname: the procedure name.
@INPUT LINKPTR func: the func document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_call_func(xdb_t db, LINKPTR func);

/*
@FUNCTION db_export: export database row set into stream.
@INPUT xdb_t db: the xdb handle.
@OUTPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlstr: the select sql statement.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_export(xdb_t db, stream_t stream, const tchar_t* sqlstr);

/*
@FUNCTION db_import: import database row set from stream.
@INPUT xdb_t db: the xdb handle.
@INPUT stream_t stream: the stream object.
@INPUT const tchar_t* table: the database table name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_import(xdb_t db, stream_t stream, const tchar_t* table);

/*
@FUNCTION db_batch: batch executing sql statement from stream.
@INPUT xdb_t db: the xdb handle.
@INPUT stream_t stream: the stream object.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_batch(xdb_t db, stream_t stream);

/*
@FUNCTION db_read_blob: select a blob object into stream.
@INPUT xdb_t db: the xdb handle.
@OUTPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlstr: the select sql statement. eg: "select blobfield from sometable where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_read_blob(xdb_t db, stream_t stream, const tchar_t* sqlstr);

/*
@FUNCTION db_write_blob: write a blob object into database from stream.
@INPUT xdb_t db: the xdb handle.
@INPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlfmt: the update sql statement. eg: "update sometable set blobfield = ? where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_write_blob(xdb_t db, stream_t stream, const tchar_t* sqlfmt);

/*
@FUNCTION db_read_clob: select a clob object into stream.
@INPUT xdb_t db: the xdb handle.
@OUTPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlstr: the select sql statement. eg: "select clobfield from sometable where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_read_clob(xdb_t db, string_t varstr, const tchar_t* sqlstr);

/*
@FUNCTION db_write_clob: write a clob object into database from stream.
@INPUT xdb_t db: the xdb handle.
@INPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlfmt: the update sql statement. eg: "update sometable set clobfield = ? where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_write_clob(xdb_t db, string_t varstr, const tchar_t* sqlfmt);

/*
@FUNCTION db_read_xdoc: select a xml document into document.
@INPUT xdb_t db: the xdb handle.
@OUTPUT LINKPTR domdoc: the dom document.
@INPUT const tchar_t* sqlstr: the select sql statement. eg: "select xmlfield from sometable where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_read_xdoc(xdb_t db, LINKPTR domdoc, const tchar_t* sqlstr);

/*
@FUNCTION db_write_xdoc: write a xml document into database from stream.
@INPUT xdb_t db: the xdb handle.
@INPUT LINKPTR domdoc: the dom document.
@INPUT const tchar_t* sqlfmt: the update sql statement. eg: "update sometable set xmlfield = ? where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
extern bool_t STDCALL db_write_xdoc(xdb_t db, LINKPTR domdoc, const tchar_t* sqlfmt);

/*
@FUNCTION db_trace: dump sql statement and error into stream.
@INPUT xdb_t db: the xdb handle.
@OUTPUT stream_t stm: the strem object.
@RETURN void: none.
*/
extern void STDCALL db_trace(xdb_t db, stream_t stm);

#ifdef	__cplusplus
}
#endif
        
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
typedef bool_t(STDCALL *PF_DB_DATETIME)(xdb_t, int, tchar_t*);
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
typedef bool_t(STDCALL *PF_DB_WRITE_CLOB)(xdb_t, string_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_CLOB)(xdb_t, string_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_WRITE_XDOC)(xdb_t, LINKPTR, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_XDOC)(xdb_t, LINKPTR, const tchar_t*);

#endif