/***********************************************************************
	Easily xdb stub

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdb stub document

	@module	xdb_stub.c | xdb stub implement file

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

typedef struct _db_stub_t{
	xdb_head head;

	tchar_t sz_srv[RES_LEN + 1];
	tchar_t sz_dbn[RES_LEN + 1];
	tchar_t sz_uid[KEY_LEN + 1];
	tchar_t sz_pwd[KEY_LEN + 1];

	tchar_t err_code[NUM_LEN + 1];
	tchar_t err_text[ERR_LEN + 1];

	int rows;
}db_stub_t;

static tchar_t stub_esc[] = { _T('%'), _T('\t'), _T('\r'), _T('\n'), _T('\0') };

static void _parse_db_range(tchar_t* sz_range, int* pfrom, int* pto, int* pall)
{
	int len;
	const tchar_t* token = sz_range;

	*pfrom = *pto = *pall = 0;

	while (*token != _T('\0'))
	{
		while ((*token < _T('0') || *token > _T('9')) && *token != _T('\0'))
			token++;

		len = 0;
		while (*token >= _T('0') && *token <= _T('9'))
		{
			token++;
			len++;
		}

		if (*token == _T('-'))
			*pfrom = xsntol(token - len, len);
		else if (*token == _T('/'))
			*pto = xsntol(token - len, len);
		else
			*pall = xsntol(token - len, len);

		if (*token != _T('\0'))
			token++;
	}
}

void parse_colset_from_line(link_t_ptr ptr, string_t vs)
{
	const tchar_t* token = NULL;
	tchar_t* colname = NULL;
	int pos;
	link_t_ptr clk;
	
	if (!string_len(vs))
		return;

	token = string_ptr(vs);

	while (*token != TXT_LINEFEED && *token != _T('\0'))
	{
		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}

		colname = xsalloc(pos + 1);
		xsncpy(colname, token - pos, pos);

		clk = insert_col(ptr, LINK_LAST);
		set_col_name(clk, colname);
		set_col_title(clk, colname);

		xsfree(colname);

		if (*token == TXT_ITEMFEED)
			token++;
	}
}

void parse_rowset_from_line(link_t_ptr ptr, string_t vs)
{
	const tchar_t* token = NULL;
	int pos;
	link_t_ptr clk,rlk;
	const tchar_t *data_type, *data_cast;
	xdate_t dt = { 0 };
	tchar_t sz_date[DATE_LEN + 1] = { 0 };

	tchar_t* sz_esc = NULL;
	int esc_len = 0;

	if (!string_len(vs))
		return;

	rlk = insert_row(ptr, LINK_LAST);

	token = string_ptr(vs);
	
	clk = get_next_col(ptr, LINK_FIRST);

	while (*token != TXT_LINEFEED && *token != _T('\0'))
	{
		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}

		if (pos && clk)
		{
			data_type = get_col_data_type_ptr(clk);
			if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
			{
				data_cast = get_col_field_cast_ptr(clk);
				if (!is_null(data_cast))
				{
					xsncpy(sz_date, token - pos, pos);
					parse_datetime_ex(&dt, data_cast, sz_date);
					format_datetime(&dt, sz_date);
					set_cell_text(rlk, clk, sz_date, -1);
				}
				else
				{
					set_cell_text(rlk, clk, token - pos, pos);
				}
			}
			else
			{
				esc_len = decode_escape(token - pos, pos, NULL, MAX_LONG);
				if (esc_len != pos)
				{
					sz_esc = xsalloc(esc_len + 1);
					decode_escape(token - pos, pos, sz_esc, esc_len);

					attach_cell_text(rlk, clk, sz_esc);
				}
				else
				{
					set_cell_text(rlk, clk, token - pos, pos);
				}
			}
		}

		clk = get_next_col(ptr, clk);

		if (*token == TXT_ITEMFEED)
			token++;
	}
}

bool_t STDCALL db_parse_dsn(const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len)
{
	link_t_ptr d_ptr = NULL;

	TRY_CATCH;

	d_ptr = create_proper_doc();
	if (!load_proper_doc_from_ini_file(d_ptr, NULL, dsnfile))
	{
		raise_user_error(NULL, NULL);
	}

	read_proper(d_ptr, _T("STUB"), -1, DSN_SERVER, -1, srv_buf, srv_len);
	read_proper(d_ptr, _T("STUB"), -1, DSN_DATABASE, -1, dbn_buf, dbn_len);
	read_proper(d_ptr, _T("STUB"), -1, DSN_UID, -1, uid_buf, uid_len);
	read_proper(d_ptr, _T("STUB"), -1, DSN_PWD, -1, pwd_buf, pwd_len);

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
	db_stub_t* pdb;

	pdb = (db_stub_t*)xmem_alloc(sizeof(db_stub_t));
	pdb->head.dbt = _DB_STUB;
	pdb->head.cbs = sizeof(db_stub_t);

	xsncpy(pdb->sz_srv, srv, PATH_LEN);
	xsncpy(pdb->sz_dbn, dbn, RES_LEN);
	xsncpy(pdb->sz_uid, uid, KEY_LEN);
	xsncpy(pdb->sz_pwd, pwd, KEY_LEN);
	
	return &pdb->head;
}

void STDCALL db_close(xdb_t db)
{
	db_stub_t* pdb = (db_stub_t*)db;
	
	XDL_ASSERT(db && db->dbt == _DB_STUB);
	
	xmem_free(pdb);
}


bool_t STDCALL db_datetime(xdb_t db, const tchar_t* sz_when, tchar_t* sz_date)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("GET"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (is_null(sz_when))
		xhttp_set_query_entity(xhttp, XDB_API_DBDATETIME, -1, XDB_DATE_TODAY, -1);
	else
		xhttp_set_query_entity(xhttp, XDB_API_DBDATETIME, -1, sz_when, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_LASTMODIFIED, -1, sz_date, DATE_LEN);

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_schema(xdb_t db, link_t_ptr grid, const tchar_t* sqlstr)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	link_t_ptr ptr_sch = NULL;
	link_t_ptr ptr_xml;

	link_t_ptr slk_row,slk,clk;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("HEAD"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBEXPORT, -1, sqlstr, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	xhttp_set_request_accept_charset(xhttp, CHARSET_UTF8, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	ptr_sch = create_schema_doc();

	ptr_xml = upcast_dom_to_xml(ptr_sch);

	rt = xhttp_recv_xml(xhttp, ptr_xml);

	import_grid_schema(grid, ptr_sch);

	slk_row = get_schema_element(ptr_sch, DOC_GRID_ROW);
	if (slk_row)
	{
		slk = get_schema_next_element(slk_row, LINK_FIRST);
		while (slk)
		{
			clk = get_col(grid, get_schema_element_name_ptr(slk));
			if (clk)
			{
				set_col_table_name(clk, get_dom_node_attr_ptr(slk, ATTR_TABLE_NAME,-1));
				set_col_field_name(clk, get_dom_node_attr_ptr(slk, ATTR_FIELD_NAME, -1));
				set_col_field_type(clk, get_dom_node_attr_ptr(slk, ATTR_FIELD_TYPE, -1));
				set_col_nullable(clk, get_dom_node_attr_boolean(slk, ATTR_NULLABLE));
				set_col_updateable(clk, get_dom_node_attr_boolean(slk, ATTR_UPDATEABLE));
			}

			slk = get_schema_next_element(slk_row, slk);
		}
	}

	clk = get_next_col(grid, LINK_FIRST);
	while (clk)
	{
		set_col_field_name(clk, get_col_name_ptr(clk));
		set_col_field_type(clk, ATTR_FIELD_TYPE_DATA);

		clk = get_next_col(grid, clk);
	}
	
	ptr_sch = downcast_xml_to_dom(ptr_xml);
	ptr_xml = NULL;

	destroy_schema_doc(ptr_sch);
	ptr_sch = NULL;

	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"),pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (ptr_sch)
		destroy_schema_doc(ptr_sch);

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_select(xdb_t db, link_t_ptr grid, const tchar_t* sqlstr)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	link_t_ptr ptr_xml = NULL;
	stream_t stream;
	string_t vs = NULL;
	dword_t size = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	clear_grid_rowset(grid);
	clear_grid_colset(grid);

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("GET"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBEXPORT, -1, sqlstr, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	xhttp_set_request_accept_charset(xhttp, CHARSET_UTF8, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	stream = xhttp_get_recv_stream(xhttp);

	stream_read_utfbom(stream, NULL);

	vs = string_alloc();

	if (!stream_read_line(stream, vs, &size))
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	parse_colset_from_line(grid, vs);
	string_empty(vs);

	while (1)
	{
		size = 0;
		if (!stream_read_line(stream, vs, &size))
		{
			get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

			raise_user_error(pdb->err_code, pdb->err_text);
		}

		if (!size)
			break;

		parse_rowset_from_line(grid, vs);

		string_empty(vs);
	}

	string_free(vs);
	vs = NULL;

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (vs)
		string_free(vs);

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_fetch(xdb_t db, link_t_ptr grid)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	tchar_t* sqlstr = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	link_t_ptr tmp_grid = NULL;
	stream_t stream;
	string_t vs = NULL;
	dword_t size;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	clear_grid_rowset(grid);

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("GET"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	size = format_grid_select_sql(grid, NULL, MAX_LONG);
	sqlstr = xsalloc(size + 1);
	format_grid_select_sql(grid,sqlstr,size);

	xhttp_set_query_entity(xhttp, XDB_API_DBEXPORT, -1, sqlstr, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	xhttp_set_request_accept_charset(xhttp, CHARSET_UTF8, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	stream = xhttp_get_recv_stream(xhttp);

	stream_read_utfbom(stream, NULL);

	vs = string_alloc();

	tmp_grid = create_grid_doc();

	if (!stream_read_line(stream, vs, &size))
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	parse_colset_from_line(tmp_grid, vs);
	string_empty(vs);

	copy_grid_colsch(tmp_grid, grid);

	while (1)
	{
		size = 0;
		if (!stream_read_line(stream, vs, &size))
		{
			get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

			raise_user_error(pdb->err_code, pdb->err_text);
		}

		if (!size)
			break;

		parse_rowset_from_line(tmp_grid, vs);

		string_empty(vs);
	}

	string_free(vs);
	vs = NULL;

	merge_grid_rowset(grid, tmp_grid);
	destroy_grid_doc(tmp_grid);
	tmp_grid = NULL;

	if (get_grid_updateable(grid))
	{
		refresh_grid_rowset(grid);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (sqlstr)
		xmem_free(sqlstr);

	if (vs)
		string_free(vs);

	if (tmp_grid)
		destroy_grid_doc(tmp_grid);

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_exec(xdb_t db, const tchar_t* sqlstr, int sqllen)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	byte_t sz_bom[4] = { 0 };
	int n_bom = 0;
	int n_from, n_to, n_all;
	bool_t b_continue = 0;

	bool_t glt = 0;
	dword_t dw;

	stream_t stm = NULL;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	if (sqllen < 0)
		sqllen = xslen(sqlstr);

	if (!sqllen || is_null(sqlstr))
	{
		return 1;
	}

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn?%s"), pdb->sz_srv, pdb->sz_dbn, XDB_API_DBBATCH);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_header(xhttp, HTTP_HEADER_EXPECT, -1, HTTP_HEADER_EXPECT_CONTINUE, -1);

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_UTF8, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

#ifdef _UNICODE
	dw = ucs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#else
	dw = mbs_to_utf8(sqlstr, sqllen, NULL, MAX_LONG);
#endif
	dw += format_utfbom(_UTF8, NULL);
	xhttp_set_request_content_length(xhttp, dw);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (rt)
	{
		xhttp_get_response_code(xhttp, sz_code);
		b_continue = IS_XHTTP_CONTINUE(sz_code);
		if (b_continue)
		{
			xhttp_reset_response(xhttp);
		}
	}
	else
	{
		b_continue = 1;
	}

	if (b_continue)
	{
		stm = xhttp_get_send_stream(xhttp);

		stream_write_utfbom(stm, NULL);

		if (!stream_write(stm, sqlstr, sqllen, &dw))
		{
			get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

			raise_user_error(pdb->err_code, pdb->err_text);
		}
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_update(xdb_t db, link_t_ptr grid)
{
	db_stub_t* pdb = (db_stub_t*)db;

	LINKPTR rlk;

	xhand_t xhttp = NULL;
	stream_t stream;

	string_t var = NULL;
	tchar_t* sqlstr;
	int rs, sqllen;

	dword_t len;
	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	tchar_t sz_code[NUM_LEN + 1] = { 0 };

	int n_from, n_to, n_all;
	bool_t b_continue = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn?%s"), pdb->sz_srv, pdb->sz_dbn, XDB_API_DBBATCH);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_header(xhttp, HTTP_HEADER_EXPECT, -1, HTTP_HEADER_EXPECT_CONTINUE, -1);
	xhttp_set_request_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1);
	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_UTF8, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (rt)
	{
		xhttp_get_response_code(xhttp, sz_code);
		b_continue = IS_XHTTP_CONTINUE(sz_code);
		if (b_continue)
		{
			xhttp_reset_response(xhttp);
		}
	}
	else
	{
		b_continue = 1;
	}

	if (b_continue)
	{
		stream = xhttp_get_send_stream(xhttp);

		stream_write_utfbom(stream, NULL);

		var = string_alloc();

		rlk = get_next_row(grid, LINK_FIRST);
		while (rlk)
		{
			rs = get_row_state(rlk);

			if (rs == dsDelete)
			{
				sqllen = format_row_delete_sql(grid, rlk, NULL, MAX_LONG);
				sqlstr = string_ensure_buf(var, sqllen);
				sqllen = format_row_delete_sql(grid, rlk, sqlstr, sqllen);
			}
			else if (rs == dsNewDirty)
			{
				sqllen = format_row_insert_sql(grid, rlk, NULL, MAX_LONG);
				sqlstr = string_ensure_buf(var, sqllen);
				sqllen = format_row_insert_sql(grid, rlk, sqlstr, sqllen);
			}
			else if (rs == dsDirty)
			{
				sqllen = format_row_update_sql(grid, rlk, NULL, MAX_LONG);
				sqlstr = string_ensure_buf(var, sqllen);
				sqllen = format_row_update_sql(grid, rlk, sqlstr, sqllen);
			}
			else
			{
				rlk = get_next_row(grid, rlk);
				continue;
			}

			stream_write_line(stream, var, &len);

			string_empty(var);

			rlk = get_next_row(grid, rlk);
		}

		string_empty(var);
		stream_write_line(stream, var, &len);

		string_free(var);
		var = NULL;
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:
	if (var)
		string_free(var);

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_batch(xdb_t db, stream_t stream)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;
	stream_t stm_http;
	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_enc[INT_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;
	bool_t b_continue = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn?%s"), pdb->sz_srv, pdb->sz_dbn, XDB_API_DBBATCH);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_header(xhttp, HTTP_HEADER_EXPECT, -1, HTTP_HEADER_EXPECT_CONTINUE, -1);
	xhttp_set_request_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1);

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	format_charset(stream_get_encode(stream), sz_enc);
	if (!is_null(sz_enc))
	{
		xhttp_set_request_content_type_charset(xhttp, sz_enc, -1);
	}

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (rt)
	{
		xhttp_get_response_code(xhttp, sz_code);
		b_continue = IS_XHTTP_CONTINUE(sz_code);
		if (b_continue)
		{
			xhttp_reset_response(xhttp);
		}
	}
	else
	{
		b_continue = 1;
	}

	if (b_continue)
	{
		stm_http = xhttp_get_send_stream(xhttp);

		stream_read_utfbom(stream, NULL);

		stream_write_utfbom(stm_http, NULL);

		stream_copy(stream, stm_http);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_import(xdb_t db, stream_t stream, const tchar_t* table)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;
	stream_t stm_http;
	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_enc[RES_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;
	bool_t b_continue = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBIMPORT, -1, table, -1);

	xhttp_set_request_default_header(xhttp);
	xhttp_set_request_header(xhttp, HTTP_HEADER_EXPECT, -1, HTTP_HEADER_EXPECT_CONTINUE, -1);
	xhttp_set_request_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1);

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	format_charset(stream_get_encode(stream), sz_enc);
	if (!is_null(sz_enc))
	{
		xhttp_set_request_content_type_charset(xhttp, sz_enc, -1);
	}

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (rt)
	{
		xhttp_get_response_code(xhttp, sz_code);
		b_continue = IS_XHTTP_CONTINUE(sz_code);
		if (b_continue)
		{
			xhttp_reset_response(xhttp);
		}
	}
	else
	{
		b_continue = 1;
	}

	if (b_continue)
	{
		stm_http = xhttp_get_send_stream(xhttp);

		stream_read_utfbom(stream, NULL);

		stream_write_utfbom(stm_http, NULL);

		stream_copy(stream, stm_http);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_export(xdb_t db, stream_t stream, const tchar_t* sqlstr)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;
	stream_t stm_http;
	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_enc[RES_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("GET"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBEXPORT, -1, sqlstr, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	format_charset(stream_get_encode(stream), sz_enc);
	if (!is_null(sz_enc))
	{
		xhttp_set_request_accept_charset(xhttp, sz_enc, -1);
	}

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	stm_http = xhttp_get_recv_stream(xhttp);

	stream_read_utfbom(stm_http, NULL);

	stream_write_utfbom(stream, NULL);

	stream_copy(stm_http, stream);

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_call_func(xdb_t db, link_t_ptr func)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	bool_t rt;

	link_t_ptr ptr_func = NULL;
	link_t_ptr ptr_xml = NULL;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn?%s"), pdb->sz_srv, pdb->sz_dbn, XDB_API_DBCALLFUNC);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	ptr_xml = upcast_dom_to_xml(func);

	rt = xhttp_send_xml(xhttp, ptr_xml);

	func = downcast_xml_to_dom(ptr_xml);
	ptr_xml = NULL;

	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	ptr_func = create_func_doc();
	ptr_xml = upcast_dom_to_xml(ptr_func);

	rt = xhttp_recv_xml(xhttp, ptr_xml);

	ptr_func = downcast_xml_to_dom(ptr_xml);
	ptr_xml = NULL;

	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	merge_func_return(func, ptr_func);

	destroy_func_doc(ptr_func);
	ptr_func = NULL;

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (ptr_func)
		destroy_func_doc(ptr_func);

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t STDCALL db_call_json(xdb_t db, const tchar_t* procname, link_t_ptr json)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	bool_t rt;

	stream_t stm;
	byte_t* buf = NULL;
	dword_t len;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBCALLJSON, -1, procname, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);
	xhttp_set_request_accept_charset(xhttp, CHARSET_UTF8, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	len = format_json_doc_to_bytes(json, NULL, MAX_LONG, _UTF8);
	buf = (byte_t*)xmem_alloc(len);
	format_json_doc_to_bytes(json, buf, len, _UTF8);

	rt = xhttp_send_full(xhttp, buf, len);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xmem_free(buf);
	buf = NULL;

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	clear_json_doc(json);

	stm = xhttp_get_recv_stream(xhttp);

	rt = parse_json_doc_from_stream(json, stm);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (buf)
		xmem_free(buf);

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t _stdcall db_write_blob(xdb_t db, stream_t stream, const tchar_t* sqlfmt)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	stream_t stm_http = NULL;
	bool_t b_continue = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBWRITEBLOB, -1, sqlfmt, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_header(xhttp, HTTP_HEADER_EXPECT, -1, HTTP_HEADER_EXPECT_CONTINUE, -1);
	xhttp_set_request_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (rt)
	{
		xhttp_get_response_code(xhttp, sz_code);
		b_continue = IS_XHTTP_CONTINUE(sz_code);
		if (b_continue)
		{
			xhttp_reset_response(xhttp);
		}
	}
	else
	{
		b_continue = 1;
	}

	if (b_continue)
	{
		stm_http = xhttp_get_send_stream(xhttp);

		stream_copy(stream, stm_http);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t _stdcall db_read_blob(xdb_t db, stream_t stream, const tchar_t* sqlstr)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	stream_t stm_http = NULL;
	bool_t b_continue = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("GET"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBREADBLOB, -1, sqlstr, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	stm_http = xhttp_get_recv_stream(xhttp);

	stream_copy(stm_http, stream);

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t _stdcall db_write_clob(xdb_t db, string_t varstr, const tchar_t* sqlfmt)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };

	byte_t sz_bom[4] = { 0 };
	int n_bom = 0;
	int n_from, n_to, n_all;

	bool_t b_continue = 0;
	dword_t n_size = 0;

	stream_t stm = NULL;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBWRITECLOB, -1, sqlfmt, -1);

	n_bom = format_utfbom(_UTF8, sz_bom);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_header(xhttp, HTTP_HEADER_EXPECT, -1, HTTP_HEADER_EXPECT_CONTINUE, -1);

	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_UTF8, -1);

	n_size = string_encode(varstr, _UTF8, NULL, MAX_LONG) + n_bom;
	xsprintf(fsize, _T("%d"), n_size);
	xhttp_set_request_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (rt)
	{
		xhttp_get_response_code(xhttp, sz_code);
		b_continue = IS_XHTTP_CONTINUE(sz_code);
		if (b_continue)
		{
			xhttp_reset_response(xhttp);
		}
	}
	else
	{
		b_continue = 1;
	}

	if (b_continue)
	{
		stm = xhttp_get_send_stream(xhttp);

		stream_write_utfbom(stm, NULL);

		rt = stream_write_line(stm, varstr, NULL);
		if (!rt)
		{
			get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

			raise_user_error(pdb->err_code, pdb->err_text);
		}
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t _stdcall db_read_clob(xdb_t db, string_t varstr, const tchar_t* sqlstr)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	dword_t n_size = 0;

	stream_t stm = NULL;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	string_empty(varstr);

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("GET"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBREADCLOB, -1, sqlstr, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	xhttp_set_request_accept_charset(xhttp, CHARSET_UTF8, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	stm = xhttp_get_recv_stream(xhttp);

	stream_read_utfbom(stm, NULL);

	rt = stream_read_line(stm, varstr, NULL);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t _stdcall db_write_xdoc(xdb_t db, link_t_ptr dom, const tchar_t* sqlfmt)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t fcode[INT_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	bool_t b_continue = 0;
	link_t_ptr ptr_xml = NULL;
	dword_t n_size = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("POST"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBWRITEXDOC, -1, sqlfmt, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_header(xhttp, HTTP_HEADER_EXPECT, -1, HTTP_HEADER_EXPECT_CONTINUE, -1);
	xhttp_set_request_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (rt)
	{
		xhttp_get_response_code(xhttp, fcode);
		b_continue = IS_XHTTP_CONTINUE(fcode);
		if (b_continue)
		{
			xhttp_reset_response(xhttp);
		}
	}
	else
	{
		b_continue = 1;
	}

	if (b_continue)
	{
		ptr_xml = upcast_dom_to_xml(dom);
		rt = xhttp_send_xml(xhttp, ptr_xml);
		dom = downcast_xml_to_dom(ptr_xml);

		if (!rt)
		{
			get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

			raise_user_error(pdb->err_code, pdb->err_text);
		}
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"), pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}

bool_t _stdcall db_read_xdoc(xdb_t db, link_t_ptr dom, const tchar_t* sqlstr)
{
	db_stub_t* pdb = (db_stub_t*)db;

	xhand_t xhttp = NULL;

	bool_t rt;

	tchar_t sz_url[PATH_LEN] = { 0 };
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	tchar_t fcode[INT_LEN + 1] = { 0 };
	tchar_t frange[META_LEN + 1] = { 0 };
	int n_from, n_to, n_all;

	link_t_ptr ptr_xml = NULL;
	bool_t b_continue = 0;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	TRY_CATCH;

	xsprintf(sz_url, _T("%s/%s.dsn"), pdb->sz_srv, pdb->sz_dbn);

	xhttp = xhttp_client(_T("GET"), sz_url);
	if (!xhttp)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_set_query_entity(xhttp, XDB_API_DBREADXDOC, -1, sqlstr, -1);

	xhttp_set_request_default_header(xhttp);

	xhttp_set_request_accept_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	xhttp_request_signature(xhttp, HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_pwd, sz_hmac, HMAC_LEN);
	xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pdb->sz_uid, sz_hmac);
	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);

	rt = xhttp_send_request(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	rt = xhttp_recv_response(xhttp);
	if (!rt)
	{
		get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

		raise_user_error(pdb->err_code, pdb->err_text);
	}

	if (!xhttp_get_response_state(xhttp))
	{
		xhttp_recv_error(xhttp, NULL, NULL, pdb->err_code, pdb->err_text, ERR_LEN);
		raise_user_error(pdb->err_code, pdb->err_text);
	}

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTRANGE, -1, frange, META_LEN);
	n_from = n_to = n_all = 0;
	_parse_db_range(frange, &n_from, &n_to, &n_all);
	pdb->rows = n_all;

	ptr_xml = upcast_dom_to_xml(dom);

	xhttp_recv_xml(xhttp, ptr_xml);

	dom = downcast_xml_to_dom(ptr_xml);

	xhttp_close(xhttp);
	xhttp = NULL;

	xscpy(pdb->err_code, _T("0"));
	xsprintf(pdb->err_text, _T("succeed, %d rows affected"),pdb->rows);

	END_CATCH;

	return 1;

ONERROR:

	if (xhttp)
		xhttp_close(xhttp);

	return 0;
}
int STDCALL db_rows(xdb_t db)
{
	db_stub_t* pdb = (db_stub_t*)db;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	return pdb->rows;
}

int STDCALL db_error(xdb_t db, tchar_t* buf, int max)
{
	db_stub_t* pdb = (db_stub_t*)db;

	XDL_ASSERT(db && db->dbt == _DB_STUB);

	max = (max < ERR_LEN) ? max : ERR_LEN;
	if (buf)
	{
		xsncpy(buf, pdb->err_text, max);
	}

	return -1;
}
