/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc sqldb bag document

	@module	gridsql.h | interface file

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

#ifndef _GRIDSQL_H
#define _GRIDSQL_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC


#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION get_grid_next_param: fetch next sql parameter in where clause.
@INPUT link_t_ptr ptr: the grid link component.
@OUTPUT tchar_t* pname: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN bool_t: return nonzero if exists, otherwise return zero.
*/
XDL_API bool_t get_grid_next_param(link_t_ptr ptr, tchar_t* pname, int max);

/*
@FUNCTION set_grid_sql_param: set sql parameter value in where clause.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* parmname: the sql parameter name token.
@INPUT const tchar_t*: parmval: the sql parameter value token.
@RETURN void: none.
*/
XDL_API void set_grid_sql_param(link_t_ptr ptr, const tchar_t* paramname, const tchar_t* paramval);

/*
@FUNCTION get_grid_sql_param_ptr: get sql parameter value in where clause.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const tchar_t* paramname: the parameter name token.
@INPUT int len: the parameter name length in characters.
@RETURN const tchar_t*: return the parameter value if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_grid_sql_param_ptr(link_t_ptr ptr, const tchar_t* paramname, int len);

/*
@FUNCTION format_grid_param_clause: format sql where clause.
@INPUT link_t_ptr ptr: the grid link component.
@INTPUT conat tchar_t* clause: the where clause format string token.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_grid_param_clause(link_t_ptr ptr, const tchar_t* clause, tchar_t* buf, int max);

/*
@FUNCTION format_row_update_sql: format sql update clause.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_row_update_sql(link_t_ptr ptr, link_t_ptr rlk, tchar_t* buf, int max);

/*
@FUNCTION format_row_insert_sql: format sql insert clause.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_row_insert_sql(link_t_ptr ptr, link_t_ptr rlk, tchar_t* buf, int max);

/*
@FUNCTION format_row_delete_sql: format sql delete clause.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_row_delete_sql(link_t_ptr ptr, link_t_ptr rlk, tchar_t* buf, int max);

/*
@FUNCTION format_grid_select_sql: format sql select clause.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_grid_select_sql(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION format_grid_exec_sql: format sql execute clause.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_grid_exec_sql(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION format_grid_exec_string: format sql execute clause to string object.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr rlk: the row link component.
@OUTPUT string_t vs: the string object.
@RETURN int: return the characters formated.
*/
XDL_API int format_grid_exec_string(link_t_ptr ptr, string_t vs);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*GRIDSQL_H*/