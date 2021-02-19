
/***********************************************************************
	Easily xdb web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdb web api document

	@module	xdb_api.cc | xdb web api implement file

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

#include "xdb_api.h"

typedef struct _xdb_block_t{
	XDB xdb;

	PF_DB_EXEC	pf_db_exec;
	PF_DB_SCHEMA pf_db_schema;
	PF_DB_EXPORT pf_db_export;
	PF_DB_IMPORT pf_db_import;
	PF_DB_BATCH pf_db_batch;
	PF_DB_CALL_FUNC pf_db_call_func;
	PF_DB_CALL_JSON pf_db_call_json;
	PF_DB_DATETIME pf_db_datetime;
	PF_DB_ROWS pf_db_rows;
	PF_DB_ERROR pf_db_error;
	PF_DB_READ_BLOB pf_db_read_blob;
	PF_DB_WRITE_BLOB pf_db_write_blob;
	PF_DB_READ_CLOB pf_db_read_clob;
	PF_DB_WRITE_CLOB pf_db_write_clob;
	PF_DB_READ_XDOC pf_db_read_xdoc;
	PF_DB_WRITE_XDOC pf_db_write_xdoc;
}xdb_block_t;

static void _format_db_range(tchar_t* sz_range, int from, int to, int all)
{
	xsprintf(sz_range, _T("rows %d-%d/%d"), from, to, all);
}

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

bool_t _invoke_datetime(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	bool_t rt;
	tchar_t sz_diff[INT_LEN + 1] = { 0 };
	tchar_t sz_date[DATE_LEN + 1] = { 0 };

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, XDB_API_DBDATETIME, -1, sz_diff, INT_LEN);

	rt = (*pxb->pf_db_datetime)(pxb->xdb, xstol(sz_diff), sz_date);
	if (!rt)
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_LASTMODIFIED, -1, sz_date, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: DATETIME]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), 1);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: DATETIEM]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_schema(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_enc[INT_LEN + 1] = { 0 };
	int encode;

	tchar_t* d_sql = NULL;
	int n_rows,n_sql = 0;

	link_t_ptr ptr_xml;
	link_t_ptr d_ptr_grid = NULL;
	link_t_ptr d_ptr_sch = NULL;
	link_t_ptr slk_row,slk, clk;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBEXPORT, -1, NULL, MAX_LONG);

	if (!n_sql)
	{
		raise_user_error(_T("_invoke_schema"), _T("empty sql statement\n"));
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBEXPORT, -1, d_sql, n_sql);

	d_ptr_grid = create_grid_doc();

	if(!(*pxb->pf_db_schema)(pxb->xdb, d_ptr_grid, d_sql))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	d_ptr_sch = create_schema_doc();
	export_grid_schema(d_ptr_grid, d_ptr_sch);

	slk_row = get_schema_element(d_ptr_sch, DOC_GRID_ROW);
	if (slk_row)
	{
		slk = get_schema_next_element(slk_row, LINK_FIRST);
		while (slk)
		{
			clk = get_col(d_ptr_grid, get_schema_element_name_ptr(slk));
			if (clk)
			{
				set_dom_node_attr(slk, ATTR_TABLE_NAME, -1, get_col_table_name_ptr(clk), -1);
				set_dom_node_attr(slk, ATTR_FIELD_NAME, -1, get_col_field_name_ptr(clk), -1);
				set_dom_node_attr(slk, ATTR_FIELD_TYPE, -1, get_col_field_type_ptr(clk), -1);
				set_dom_node_attr_boolean(slk, ATTR_NULLABLE, get_col_nullable(clk));
				set_dom_node_attr_boolean(slk, ATTR_UPDATEABLE, get_col_updateable(clk));
			}

			slk = get_schema_next_element(slk_row, slk);
		}
	}

	destroy_grid_doc(d_ptr_grid);
	d_ptr_grid = NULL;

	ptr_xml = upcast_dom_to_xml(d_ptr_sch);

	xhttp_get_request_accept_charset(pb->http, sz_enc, RES_LEN);
	encode = parse_charset(sz_enc);
	if (encode)
	{
		set_xml_encode(ptr_xml, encode);
	}
	else
	{
		encode = get_xml_encode(ptr_xml);
	}
	format_charset(encode, sz_enc);
	
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	xhttp_set_response_content_type_charset(pb->http, sz_enc, -1);

	xhttp_send_xml(pb->http, ptr_xml);

	d_ptr_sch = downcast_xml_to_dom(ptr_xml);

	destroy_schema_doc(d_ptr_sch);
	d_ptr_sch = NULL;

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: SCHEMA]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (d_sql)
		xsfree(d_sql);

	if (d_ptr_grid)
		destroy_grid_doc(d_ptr_grid);

	if (d_ptr_sch)
		destroy_schema_doc(d_ptr_sch);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: SCHEMA]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_export(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_enc[RES_LEN + 1] = { 0 };
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_range[META_LEN + 1] = { 0 };
	int encode;

	dword_t n_size = 0;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	stream_t stream;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBEXPORT, -1, NULL, MAX_LONG);

	if (!n_sql)
	{
		raise_user_error(_T("_invoke_export"), _T("empty sql statement\n"));
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBEXPORT, -1, d_sql, n_sql);

	if (!(*pxb->pf_db_export)(pxb->xdb, NULL, d_sql))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}
	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xsfree(d_sql);
	d_sql = NULL;

	xhttp_get_request_accept_charset(pb->http, sz_enc, RES_LEN);
	encode = parse_charset(sz_enc);
	if (!encode)
	{
		encode = _UTF8;
	}
	format_charset(encode, sz_enc);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	xhttp_set_response_content_type_charset(pb->http, sz_enc, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	stream = xhttp_get_send_stream(pb->http);

	if (!(*pxb->pf_db_export)(pxb->xdb, stream, NULL))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: EXPORT]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (!xhttp_is_responsed(pb->http))
	{
		xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);
	}

	if (d_sql)
		xsfree(d_sql);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: EXPORT]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_import(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_range[META_LEN + 1] = { 0 };

	dword_t n_size = 0;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	stream_t stream;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBIMPORT, -1, NULL, MAX_LONG);

	if (!n_sql)
	{
		raise_user_error(_T("_invoke_import"), _T("empty table name\n"));
	}

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBIMPORT, -1, d_sql, n_sql);

	stream = xhttp_get_recv_stream(pb->http);

	if(!(*pxb->pf_db_import)(pxb->xdb, stream, d_sql))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: IMPORT]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (!xhttp_is_responsed(pb->http))
	{
		xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);
	}

	if (d_sql)
		xsfree(d_sql);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: IMPORT]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_execute(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_enc[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_range[META_LEN + 1] = { 0 };

	byte_t** pbuf = NULL;
	dword_t n_bom, n_size = 0;
	int bom;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	string_t vs_sql = NULL;
	string_t vs = NULL;

	stream_t stream;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	if (xhttp_is_lined_recv(pb->http))
	{
		stream = xhttp_get_recv_stream(pb->http);

		stream_read_utfbom(stream, NULL);

		vs_sql = string_alloc();
		vs = string_alloc();

		while(1){
			string_empty(vs);
			n_size = 0;

			if (!(stream_read_line(stream, vs, &n_size)))
			{
				raise_user_error(NULL, NULL);
			}

			if (string_len(vs) == 0)
				break;

			string_cat(vs_sql, string_ptr(vs), string_len(vs));
		}

		string_free(vs);
		vs = NULL;

		if (!(*pxb->pf_db_exec)(pxb->xdb, string_ptr(vs_sql), string_len(vs_sql)))
		{
			(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
			raise_user_error(_T("-1"), sz_error);
		}

		string_free(vs_sql);
		vs_sql = NULL;
	}
	else
	{
		pbuf = bytes_alloc();
		n_size = 0;

		if (!xhttp_recv_full(pb->http, pbuf, &n_size))
		{
			raise_user_error(NULL, NULL);
		}

		bom = parse_utfbom(*pbuf, n_size);
		if (bom)
			n_bom = format_utfbom(bom, NULL);
		else
			n_bom = 0;

		n_size -= n_bom;

#if defined(UNICODE) || defined(_UNICODE)
		n_sql = utf8_to_ucs((*pbuf + n_bom), n_size, NULL, MAX_LONG);
#else
		n_sql = utf8_to_mbs((*pbuf + n_bom), n_size, NULL, MAX_LONG);
#endif

		d_sql = xsalloc(n_sql + 1);

#if defined(UNICODE) || defined(_UNICODE)
		n_sql = utf8_to_ucs((*pbuf + n_bom), n_size, d_sql, n_sql);
#else
		n_sql = utf8_to_mbs((*pbuf + n_bom), n_size, d_sql, n_sql);
#endif

		bytes_free(pbuf);
		pbuf = NULL;

		if (!(*pxb->pf_db_exec)(pxb->xdb, d_sql, n_sql))
		{
			(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
			raise_user_error(_T("-1"), sz_error);
		}

		xsfree(d_sql);
		d_sql = NULL;
	}

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);

	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: EXECUTE]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (d_sql)
		xsfree(d_sql);

	if (pbuf)
		bytes_free(pbuf);

	if (vs)
		string_free(vs);

	if (vs_sql)
		string_free(vs_sql);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: EXECUTE]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_batch(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_range[META_LEN + 1] = { 0 };

	dword_t n_size = 0;
	int n_rows;
	stream_t stream;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	stream = xhttp_get_recv_stream(pb->http);

	if(!(*pxb->pf_db_batch)(pxb->xdb, stream))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: BATCH]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: BATCH]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_write_blob(const https_block_t* pb, xdb_block_t* pxb)
{	
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_enc[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_range[META_LEN + 1] = { 0 };

	dword_t n_size = 0;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	stream_t stream;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBWRITEBLOB, -1, NULL, MAX_LONG);
	if (!n_sql)
	{
		raise_user_error(_T("_invoke_write_blob"), _T("empty sql statement\n"));
	}

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBWRITEBLOB, -1, d_sql, n_sql);

	stream = xhttp_get_recv_stream(pb->http);

	if(!(*pxb->pf_db_write_blob)(pxb->xdb, stream, d_sql))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: BLOB]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (d_sql)
		xsfree(d_sql);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: BLOB]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_read_blob(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	stream_t stream;
	dword_t n_size = 0;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBREADBLOB, -1, NULL, MAX_LONG);
	if (!n_sql)
	{
		raise_user_error(_T("_invoke_read_blob"), _T("empty sql statement\n"));
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBREADBLOB, -1, d_sql, n_sql);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_TRANSFERENCODING, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	stream = xhttp_get_send_stream(pb->http);

	if (!(*pxb->pf_db_read_blob)(pxb->xdb, stream, d_sql))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: BLOB]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (!xhttp_is_responsed(pb->http))
	{
		xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);
	}

	if (d_sql)
		xsfree(d_sql);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: BLOB]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_write_clob(const https_block_t* pb, xdb_block_t* pxb)
{	
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_enc[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_range[META_LEN + 1] = { 0 };

	dword_t n_size = 0;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	stream_t stm = NULL;
	string_t vs = NULL;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBWRITECLOB, -1, NULL, MAX_LONG);

	if (!n_sql)
	{
		raise_user_error(_T("_invoke_write_blob"), _T("empty sql statement\n"));
	}

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBWRITECLOB, -1, d_sql, n_sql);

	stm = xhttp_get_recv_stream(pb->http);

	stream_read_utfbom(stm, NULL);

	vs = string_alloc();
	
	if(!stream_read_line(stm, vs, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	if(!(*pxb->pf_db_write_clob)(pxb->xdb, vs, d_sql))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	string_free(vs);
	vs = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);

	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: CLOB]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (d_sql)
		xsfree(d_sql);

	if (vs)
		string_free(vs);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: CLOB]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_read_clob(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t fcode[INT_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t frange[RES_LEN + 1] = { 0 }; 

	int encode = 0;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	stream_t stm = NULL;
	string_t vs = NULL;
	dword_t n_size = 0;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBREADCLOB, -1, NULL, MAX_LONG);

	if (!n_sql)
	{
		raise_user_error(_T("_invoke_read_blob"), _T("empty sql statement\n"));
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBREADCLOB, -1, d_sql, n_sql);

	vs = string_alloc();

	if(!(*pxb->pf_db_read_clob)(pxb->xdb, vs, d_sql))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_get_request_accept_charset(pb->http, fcode, INT_LEN);
	encode = parse_charset(fcode);
	if (!encode)
	{
		encode = _UTF8;
	}
	format_charset(encode, fcode);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	xhttp_set_response_content_type_charset(pb->http, fcode, -1);

	n_size = string_encode(vs, encode, NULL, MAX_LONG);

	xsprintf(fsize, _T("%d"), n_size);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
	xsprintf(frange, _T("rows /%d"), n_rows);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, frange, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	stm = xhttp_get_send_stream(pb->http);

	stream_write_utfbom(stm, NULL);

	if (!stream_write_line(stm,vs,NULL))
	{
		raise_user_error(NULL, NULL);
	}

	string_free(vs);
	vs = NULL;

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: CLOB]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (!xhttp_is_responsed(pb->http))
	{
		xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);
	}

	if (d_sql)
		xsfree(d_sql);

	if (vs)
		string_free(vs);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: CLOB]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_write_xdoc(const https_block_t* pb, xdb_block_t* pxb)
{	
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_enc[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_range[META_LEN + 1] = { 0 };

	dword_t n_size = 0;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	link_t_ptr dom,ptr_xml = NULL;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBWRITEXDOC, -1, NULL, MAX_LONG);

	if (!n_sql)
	{
		raise_user_error(_T("_invoke_write_xdoc"), _T("empty sql statement\n"));
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBWRITEXDOC, -1, d_sql, n_sql);

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	ptr_xml = create_xml_doc();

	if(!xhttp_recv_xml(pb->http, ptr_xml))
	{
		raise_user_error(NULL, NULL);
	}

	dom = downcast_xml_to_dom(ptr_xml);

	if(!(*pxb->pf_db_write_xdoc)(pxb->xdb, dom, d_sql))
	{
		ptr_xml = upcast_dom_to_xml(dom);

		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	ptr_xml = upcast_dom_to_xml(dom);
	destroy_xml_doc(ptr_xml);
	ptr_xml = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);

	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: XDOC]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (d_sql)
		xsfree(d_sql);

	if (ptr_xml)
		destroy_xml_doc(ptr_xml);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: XDOC]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_read_xdoc(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_enc[INT_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	int encode;

	tchar_t* d_sql = NULL;
	int n_rows, n_sql = 0;

	link_t_ptr dom,ptr_xml = NULL;
	dword_t n_size = 0;

	TRY_CATCH;

	n_sql = xhttp_get_url_query_entity(pb->http, XDB_API_DBREADXDOC, -1, NULL, MAX_LONG);

	if (!n_sql)
	{
		raise_user_error(_T("_invoke_read_xdoc"), _T("empty sql statement\n"));
	}

	d_sql = xsalloc(n_sql + 1);
	xhttp_get_url_query_entity(pb->http, XDB_API_DBREADXDOC, -1, d_sql, n_sql);

	ptr_xml = create_xml_doc();

	dom = downcast_xml_to_dom(ptr_xml);

	if(!(*pxb->pf_db_read_xdoc)(pxb->xdb, dom, d_sql))
	{
		ptr_xml = upcast_dom_to_xml(dom);

		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	xsfree(d_sql);
	d_sql = NULL;

	xhttp_get_request_accept_charset(pb->http, sz_enc, RES_LEN);
	encode = parse_charset(sz_enc);
	if (!encode)
	{
		encode = _UTF8;
	}
	format_charset(encode, sz_enc);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	xhttp_set_response_content_type_charset(pb->http, sz_enc, -1);

	if (!xhttp_send_xml(pb->http, ptr_xml))
	{
		raise_user_error(NULL, NULL);
	}

	ptr_xml = upcast_dom_to_xml(dom);
	destroy_xml_doc(ptr_xml);
	ptr_xml = NULL;

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: XDOC]"), -1);

		xsprintf(sz_error, _T("ODBC succeed, %d rows affected"), n_rows);
		(*pb->plg->pf_log_error)(pb->plg->unc, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (!xhttp_is_responsed(pb->http))
	{
		xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);
	}

	if (d_sql)
		xsfree(d_sql);

	if (ptr_xml)
		destroy_xml_doc(ptr_xml);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: XDOC]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_call_func(const https_block_t* pb, xdb_block_t* pxb)
{	
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_range[META_LEN + 1] = { 0 };
	tchar_t sz_enc[INT_LEN + 1] = { 0 };

	xhand_t xhttp = NULL;

	dword_t n_size = 0;
	int n_rows;

	link_t_ptr d_xml = NULL;
	link_t_ptr ptr_func;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	d_xml = create_xml_doc();

	if (!xhttp_recv_xml(pb->http, d_xml))
	{
		raise_user_error(NULL, NULL);
	}

	get_xml_encoding(d_xml, sz_enc, INT_LEN);

	ptr_func = downcast_xml_to_dom(d_xml);

	if (!is_func_doc(ptr_func))
	{
		d_xml = upcast_dom_to_xml(ptr_func);

		raise_user_error(_T("_invoke_proc"), _T("not function document"));
	}

	if (!(*pxb->pf_db_call_func)(pxb->xdb, ptr_func))
	{
		d_xml = upcast_dom_to_xml(ptr_func);

		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	d_xml = upcast_dom_to_xml(ptr_func);

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	set_xml_encoding(d_xml, sz_enc, -1);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPXML, -1);
	xhttp_set_response_content_type_charset(pb->http, sz_enc, -1);

	xhttp_send_xml(pb->http, d_xml);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: FUNC]"), -1);

		(*pb->plg->pf_log_xml)(pb->plg->unc, d_xml);
	}

	destroy_xml_doc(d_xml);
	d_xml = NULL;

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (d_xml)
		destroy_xml_doc(d_xml);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: FUNC]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_call_json(const https_block_t* pb, xdb_block_t* pxb)
{	
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	tchar_t sz_range[META_LEN + 1] = { 0 };
	tchar_t sz_enc[INT_LEN + 1] = { 0 };
	int encode;

	xhand_t xhttp = NULL;

	dword_t n_size = 0;
	int n_rows;

	stream_t ref_stm;
	link_t_ptr ptr_json = NULL;

	tchar_t sz_proc[MAX_SQL_NAME] = { 0 };
	byte_t* buf = NULL;
	dword_t len;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	xhttp_get_url_query_entity(pb->http, XDB_API_DBCALLJSON, -1, sz_proc, MAX_SQL_NAME);

	ptr_json = create_json_doc();

	ref_stm = xhttp_get_recv_stream(pb->http);

	if (!parse_json_doc_from_stream(ptr_json, ref_stm))
	{
		raise_user_error(NULL, NULL);
	}

	if (!(*pxb->pf_db_call_json)(pxb->xdb, sz_proc, ptr_json))
	{
		(*pxb->pf_db_error)(pxb->xdb, sz_error, ERR_LEN);
		raise_user_error(_T("-1"), sz_error);
	}

	n_rows = (*pxb->pf_db_rows)(pxb->xdb);

	xhttp_get_request_accept_charset(pb->http, sz_enc, RES_LEN);
	encode = parse_charset(sz_enc);
	if (!encode)
	{
		encode = _UTF8;
	}
	format_charset(encode, sz_enc);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xsprintf(sz_range, _T("rows /%d"), n_rows);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, _T("rows"), -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, sz_range, -1);

	xhttp_set_response_content_type(pb->http, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);
	xhttp_set_response_content_type_charset(pb->http, sz_enc, -1);

	len = format_json_doc_to_bytes(ptr_json, NULL, MAX_LONG, encode);
	buf = (byte_t*)xmem_alloc(len);
	format_json_doc_to_bytes(ptr_json, buf, len, encode);

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	if (!xhttp_send_full(pb->http, buf, len))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: JSON]"), -1);

		(*pb->plg->pf_log_data)(pb->plg->unc, buf, len);
	}

	xmem_free(buf);
	buf = NULL;

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (buf)
		xmem_free(buf);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: JSON]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

void _invoke_error(const https_block_t* pb, xdb_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	d_recv = bytes_alloc();

	xhttp_recv_full(pb->http, d_recv, &n_size);

	bytes_free(d_recv);
	d_recv = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[ODBC: ERROR]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;
}

/****************************************************************************************************/
int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	xdb_block_t* pxb = NULL;
	tchar_t token[PATH_LEN + 1] = { 0 };
	tchar_t action[RES_LEN + 1] = { 0 };
	int len;

	res_modu_t xdb_lib = NULL;

	PF_DB_OPEN_DSN pf_db_open_dsn = NULL;
	PF_DB_CLOSE pf_db_close = NULL;

	bool_t rt = 1;

	TRY_CATCH;


	pxb = (xdb_block_t*)xmem_alloc(sizeof(xdb_block_t));

	if (xsnicmp(pb->object + 1, DSN_TYPE_OCI, xslen(DSN_TYPE_OCI)) == 0)
	{
#if defined(_OS_WINDOWS)
		xdb_lib = load_library(_T("xdb_oci.dll"));
#elif defined(_OS_MACOS)
		xdb_lib = load_library(_T("libxdb_oci.dylib"));
#elif defined(_OS_LINUX)
		xdb_lib = load_library(_T("libxdb_oci.so"));
#endif
		len += xslen(DSN_TYPE_OCI);
	}
	else if (xsnicmp(pb->object + 1, DSN_TYPE_MYSQL, xslen(DSN_TYPE_MYSQL)) == 0)
	{
#if defined(_OS_WINDOWS)
		xdb_lib = load_library(_T("xdb_mysql.dll"));
#elif defined(_OS_MACOS)
		xdb_lib = load_library(_T("libxdb_mysql.dylib"));
#elif defined(_OS_LINUX)
		xdb_lib = load_library(_T("libxdb_mysql.so"));
#endif
		len += xslen(DSN_TYPE_MYSQL);
	}
	else if (xsnicmp(pb->object + 1, DSN_TYPE_ODBC, xslen(DSN_TYPE_ODBC)) == 0)
	{
#if defined(_OS_WINDOWS)
		xdb_lib = load_library(_T("xdb_odbc.dll"));
#elif defined(_OS_MACOS)
		xdb_lib = load_library(_T("libxdb_odbc.dylib"));
#elif defined(_OS_LINUX)
		xdb_lib = load_library(_T("libxdb_odbc.so"));
#endif
		len += xslen(DSN_TYPE_ODBC);
	}
	else
	{
		raise_user_error(_T("xdb_api"), _T("unknown xdb library"));
	}

	if (!xdb_lib)
	{
		raise_user_error(_T("xdb_api"), _T("load xdb library falied\n"));
	}

	pf_db_open_dsn = (PF_DB_OPEN_DSN)get_address(xdb_lib, "db_open_dsn");
	pf_db_close = (PF_DB_CLOSE)get_address(xdb_lib, "db_close");

	pxb->pf_db_exec = (PF_DB_EXEC)get_address(xdb_lib, "db_exec");
	pxb->pf_db_schema = (PF_DB_SCHEMA)get_address(xdb_lib, "db_schema");
	pxb->pf_db_export = (PF_DB_EXPORT)get_address(xdb_lib, "db_export");
	pxb->pf_db_import = (PF_DB_IMPORT)get_address(xdb_lib, "db_import");
	pxb->pf_db_batch = (PF_DB_BATCH)get_address(xdb_lib, "db_batch");
	pxb->pf_db_call_func = (PF_DB_CALL_FUNC)get_address(xdb_lib, "db_call_func");
	pxb->pf_db_call_json = (PF_DB_CALL_JSON)get_address(xdb_lib, "db_call_json");
	pxb->pf_db_rows = (PF_DB_ROWS)get_address(xdb_lib, "db_rows");
	pxb->pf_db_error = (PF_DB_ERROR)get_address(xdb_lib, "db_error");

	pxb->pf_db_write_blob = (PF_DB_WRITE_BLOB)get_address(xdb_lib, "db_write_blob");
	pxb->pf_db_read_blob = (PF_DB_READ_BLOB)get_address(xdb_lib, "db_read_blob");
	pxb->pf_db_write_clob = (PF_DB_WRITE_CLOB)get_address(xdb_lib, "db_write_clob");
	pxb->pf_db_read_clob = (PF_DB_READ_CLOB)get_address(xdb_lib, "db_read_clob");
	pxb->pf_db_write_xdoc = (PF_DB_WRITE_XDOC)get_address(xdb_lib, "db_write_xdoc");
	pxb->pf_db_read_xdoc = (PF_DB_READ_XDOC)get_address(xdb_lib, "db_read_xdoc");
	pxb->pf_db_datetime = (PF_DB_DATETIME)get_address(xdb_lib, "db_datetime");

	if (!pf_db_open_dsn || !pf_db_close)
	{
		raise_user_error(_T("xdb_api"), _T("get open/close functon falied"));
	}

	xsprintf(token, _T("%s%s"), pb->path, pb->object);

	pxb->xdb = (*pf_db_open_dsn)(token);
	if (!pxb->xdb)
	{
		raise_user_error(_T("xdb_api"), _T("open database connection failed"));
	}

	xhttp_get_url_method(pb->http, token, INT_LEN);
	xhttp_get_url_query(pb->http, action, RES_LEN);

	if (compare_text(action, xslen(XDB_API_DBEXPORT), XDB_API_DBEXPORT, -1, 1) == 0)
	{
		if (compare_text(method, -1, HTTP_METHOD_HEAD, -1, 1) == 0)
		{
			if (pxb->pf_db_schema == NULL || pxb->pf_db_error == NULL)
			{
				raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
			}
			rt = _invoke_schema(pb, pxb);
		}
		else
		{
			if (pxb->pf_db_export == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
			{
				raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
			}
			rt = _invoke_export(pb, pxb);
		}
	}
	else if (compare_text(action, xslen(XDB_API_DBIMPORT), XDB_API_DBIMPORT, -1, 1) == 0)
	{
		if (pxb->pf_db_import == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_import(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBEXECUTE), XDB_API_DBEXECUTE, -1, 1) == 0)
	{
		if (pxb->pf_db_exec == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_execute(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBBATCH), XDB_API_DBBATCH, -1, 1) == 0)
	{
		if (pxb->pf_db_batch == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_batch(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBCALLFUNC), XDB_API_DBCALLFUNC, -1, 1) == 0)
	{
		if (pxb->pf_db_call_func == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_call_func(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBCALLJSON), XDB_API_DBCALLJSON, -1, 1) == 0)
	{
		if (pxb->pf_db_call_json == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_call_json(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBWRITEBLOB), XDB_API_DBWRITEBLOB, -1, 1) == 0)
	{
		if (pxb->pf_db_write_blob == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_write_blob(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBREADBLOB), XDB_API_DBREADBLOB, -1, 1) == 0)
	{
		if (pxb->pf_db_read_blob == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_read_blob(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBWRITECLOB), XDB_API_DBWRITECLOB, -1, 1) == 0)
	{
		if (pxb->pf_db_write_clob == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_write_clob(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBREADCLOB), XDB_API_DBREADCLOB, -1, 1) == 0)
	{
		if (pxb->pf_db_read_clob == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_read_clob(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBWRITEXDOC), XDB_API_DBWRITEXDOC, -1, 1) == 0)
	{
		if (pxb->pf_db_write_xdoc == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_write_xdoc(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBREADXDOC), XDB_API_DBREADXDOC, -1, 1) == 0)
	{
		if (pxb->pf_db_read_xdoc == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_read_xdoc(pb, pxb);
	}
	else if (compare_text(action, xslen(XDB_API_DBDATETIME), XDB_API_DBDATETIME, -1, 1) == 0)
	{
		if (pxb->pf_db_datetime == NULL || pxb->pf_db_error == NULL || pxb->pf_db_rows == NULL)
		{
			raise_user_error(_T("xdb_api"), _T("load xdb function falied\n"));
		}
		rt = _invoke_datetime(pb, pxb);
	}
	else
	{
		raise_user_error(_T("xdb_api"), _T("unknown xdb function\n"));
	}

	if (pxb->xdb)
		(*pf_db_close)(pxb->xdb);

	if (xdb_lib)
		free_library(xdb_lib);

	xmem_free(pxb);

	END_CATCH;

	return (rt)? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pxb);

	if (pxb->xdb)
		(*pf_db_close)(pxb->xdb);

	if (xdb_lib)
		free_library(xdb_lib);

	xmem_free(pxb);

	return HTTPS_INVOKE_WITHINFO;
}
