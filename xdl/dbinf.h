/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl db document

	@module	dbinf.h | interface file

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

#ifndef _DBINF_H
#define _DBINF_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API if_xdb_t* alloc_xdb_interface(const tchar_t* libfile);

EXP_API void free_xdb_interface(if_xdb_t* pdb);

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
EXP_API bool_t xdb_parse_dsn(if_xdb_t* pdb, const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len);

/*
@FUNCTION db_open_dsn: connect to database by dsn file.
@INPUT const tchar_t* dsnfile: the dsn file name. eg: 
local file connection: "d:\\somedir\\somedb.dsn", 
http file connection: "http://www.some.com/somedir/somedb.dsn", 
network file connection: "\\\\somehost\\sharedir\\somedb.dsn"
@RETURN bool_t: if succeeds return nonzero.
*/
EXP_API bool_t xdb_open_dsn(if_xdb_t* pdb, const tchar_t* dsnfile);

/*
@FUNCTION db_open: connect to database by parameters.
@INPUT const tchar_t* srv: the server name token.
@INPUT const tchar_t* dbn: the database name token.
@INPUT const tchar_t* uid: the user name token.
@INPUT const tchar_t* pwd: the password token.
@RETURN bool_t: if succeeds return nonzero.
*/
EXP_API bool_t xdb_open(if_xdb_t* pdb, const tchar_t* srv, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd);

/*
@FUNCTION db_close: close the database connection.
@INPUT if_xdb_t* pdb: the xdb interface.
@RETURN void: none.
*/
EXP_API void xdb_close(if_xdb_t* pdb);

/*
@FUNCTION db_exec: batch executing the sql statement.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT const tchar_t* sqlstr: the sqlstr statement.
@INPUT int len: the sql token length in characters.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_exec(if_xdb_t* pdb, const tchar_t* sqlstr, int len);

/*
@FUNCTION db_select: generating grid col set and row set from database by sql statement.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT link_t_ptr grid: the grid link component.
@INPUT const tchar_t* sqlstr: the select sqlstr statement.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_select(if_xdb_t* pdb, link_t_ptr grid, const tchar_t* sqlstr);

/*
@FUNCTION db_schema: generating grid col set from database by sql statement.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT link_t_ptr grid: the grid link component.
@INPUT const tchar_t* sqlstr: the select sqlstr statement.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_schema(if_xdb_t* pdb, link_t_ptr grid, const tchar_t* sqlstr);

/*
@FUNCTION db_schema: generating grid row set from database by col set defination.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT link_t_ptr grid: the grid link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_fetch(if_xdb_t* pdb, link_t_ptr grid);

/*
@FUNCTION db_update: commit grid row set update sql statement to database.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT link_t_ptr grid: the grid link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_update(if_xdb_t* pdb, link_t_ptr grid);

/*
@FUNCTION db_datetime: get datetime from database.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT int diff: the diff day defination.
@OUTPUT tchar_t* sz_date: the string buffer for returning date token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_datetime(if_xdb_t* pdb, int diff, tchar_t* sz_date);

/*
@FUNCTION db_rows: get rows affected.
@INPUT if_xdb_t* pdb: the xdb interface.
@RETURN int: return the rows.
*/
EXP_API int xdb_rows(if_xdb_t* pdb);

/*
@FUNCTION db_error: get rows affected.
@INPUT if_xdb_t* pdb: the xdb interface.
@RETURN int: return the rows.
*/
EXP_API int xdb_error(if_xdb_t* pdb, tchar_t* buf, int max);

/*
@FUNCTION db_call_json: call database procedure using json document.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT const tchar_t* procname: the procedure name.
@INPUT link_t_ptr json: the json document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_call_json(if_xdb_t* pdb, const tchar_t* procname, link_t_ptr json);

/*
@FUNCTION db_call_func: call database procedure using function document.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT const tchar_t* procname: the procedure name.
@INPUT link_t_ptr func: the func document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_call_func(if_xdb_t* pdb, link_t_ptr func);

/*
@FUNCTION db_export: export database row set into stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlstr: the select sql statement.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_export(if_xdb_t* pdb, stream_t stream, const tchar_t* sqlstr);

/*
@FUNCTION db_import: import database row set from stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT stream_t stream: the stream object.
@INPUT const tchar_t* table: the database table name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_import(if_xdb_t* pdb, stream_t stream, const tchar_t* table);

/*
@FUNCTION db_batch: batch executing sql statement from stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT stream_t stream: the stream object.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_batch(if_xdb_t* pdb, stream_t stream);

/*
@FUNCTION db_read_blob: select a blob object into stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlstr: the select sql statement. eg: "select blobfield from sometable where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_read_blob(if_xdb_t* pdb, stream_t stream, const tchar_t* sqlstr);

/*
@FUNCTION db_write_blob: write a blob object into database from stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlfmt: the update sql statement. eg: "update sometable set blobfield = ? where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_write_blob(if_xdb_t* pdb, stream_t stream, const tchar_t* sqlfmt);

/*
@FUNCTION db_read_clob: select a clob object into stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlstr: the select sql statement. eg: "select clobfield from sometable where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_read_clob(if_xdb_t* pdb, string_t varstr, const tchar_t* sqlstr);

/*
@FUNCTION db_write_clob: write a clob object into database from stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT stream_t stream: the stream object.
@INPUT const tchar_t* sqlfmt: the update sql statement. eg: "update sometable set clobfield = ? where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_write_clob(if_xdb_t* pdb, string_t varstr, const tchar_t* sqlfmt);

/*
@FUNCTION db_read_xdoc: select a xml document into document.
@INPUT if_xdb_t* pdb: the xdb interface.
@OUTPUT link_t_ptr domdoc: the dom document.
@INPUT const tchar_t* sqlstr: the select sql statement. eg: "select xmlfield from sometable where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_read_xdoc(if_xdb_t* pdb, link_t_ptr domdoc, const tchar_t* sqlstr);

/*
@FUNCTION db_write_xdoc: write a xml document into database from stream.
@INPUT if_xdb_t* pdb: the xdb interface.
@INPUT link_t_ptr domdoc: the dom document.
@INPUT const tchar_t* sqlfmt: the update sql statement. eg: "update sometable set xmlfield = ? where ...".
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xdb_write_xdoc(if_xdb_t* pdb, link_t_ptr domdoc, const tchar_t* sqlfmt);


#ifdef	__cplusplus
}
#endif


#endif /*DBINF_H*/
