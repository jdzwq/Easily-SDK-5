/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc sqldb bag document

	@module	gridsql.c | sqldb document implement file

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

#include "gridsql.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

bool_t get_grid_next_param(link_t_ptr ptr, tchar_t* pname, int max)
{
	const tchar_t *token;
	int tokenlen;
	bool_t rt = 0;

	token = get_grid_where_clause_ptr(ptr);

	if (is_null(token))
		return 0;

	if (is_null(pname))
		rt = 1;

	while (token = parse_param_name(token, -1, _T(':'), &tokenlen))
	{
		if (tokenlen == 0)
			continue;

		if (rt)
		{
			max = (max < tokenlen) ? max : tokenlen;
			xsncpy(pname, token, max);
			return 1;
		}

		if (compare_text(pname, -1, token, tokenlen, 0) == 0)
		{
			rt = 1;
		}
	}

	return 0;
}

void set_grid_sql_param(link_t_ptr ptr, const tchar_t* paramname, const tchar_t* paramval)
{
	if (is_null(paramname))
		return;

	set_dom_node_options_text(ptr, paramname, -1, paramval, -1);
}

const tchar_t* get_grid_sql_param_ptr(link_t_ptr ptr, const tchar_t* paramname, int len)
{
	link_t_ptr st;

	if (len < 0)
		len = xslen(paramname);

	if (!len)
		return NULL;

	st = get_dom_node_options_table(ptr);
	if (!st)
		return NULL;

	return get_string_entity_ptr(st, paramname, len);
}

int format_row_update_sql(link_t_ptr ptr,link_t_ptr rlk,tchar_t* buf,int max)
{
	link_t_ptr clk;
	int total = 0;
	int len,withand;
	const tchar_t* token;
	const tchar_t* prim;
	const tchar_t* update_table;
	const tchar_t* field_name;
	const tchar_t* table_name;
	const tchar_t* data_type;
	const tchar_t* field_type;
	const tchar_t* field_cast;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	xdate_t dt = { 0 };
	tchar_t bk[3] = { _T('\r'), _T('\n'), _T('\0') };

	update_table = get_grid_update_table_ptr(ptr);
	if(is_null(update_table))
		return 0;

	len = xslen(_T("UPDATE ")) + xslen(update_table) + xslen(_T(" SET ")); /*UPDATE szTable */
	if(total + len > max)
		return 0;

	if (buf)
	{
		xsprintf(buf + total, _T("UPDATE %s SET "), update_table);
	}
	total += len;

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		table_name = get_col_table_name_ptr(clk);
		if(!get_cell_dirty(rlk,clk) || !get_col_integrity(clk) || compare_text(table_name,-1,update_table,-1,1) != 0)
		{
			goto skipone;
		}
		
		field_name = get_col_field_name_ptr(clk);
		token = get_cell_text_ptr(rlk,clk);
		if(is_null(token))
		{
			len = xslen(field_name) + xslen(_T("=NULL,"));
			if(total + len > max)
				return total;

			if (buf)
			{
				xsprintf(buf + total, _T("%s=NULL,"), field_name);
			}
			total += len;
		}else
		{
			data_type = get_col_data_type_ptr(clk);
			if(compare_text(data_type,-1,ATTR_DATA_TYPE_STRING,-1,1) == 0)
			{
				len = xslen(field_name) + xslen(_T("='',")) + xslen(token);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s='%s',"), field_name, token);
				}
				total += len;
			}else if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 1) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 1) == 0)
			{
				field_cast = get_col_field_cast_ptr(clk);
				if (is_null(field_cast))
				{
					xscpy(sz_date, token);
				}
				else
				{
					parse_datetime(&dt, token);
					format_datetime_ex(&dt, field_cast, sz_date, DATE_LEN);
				}

				len = xslen(field_name) + xslen(_T("='',")) + xslen(sz_date);
				if (total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s='%s',"), field_name, sz_date);
				}
				total += len;
			}
			else
			{
				len = xslen(field_name) + xslen(_T("=,")) + xslen(token);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s=%s,"), field_name, token);
				}
				total += len;
			}
		}
skipone:
		clk = get_next_col(ptr,clk);
		if(clk == NULL && buf)
			buf[total - 1] = L' '; /*replace last ','*/
	}

	/*format where clause*/
	len = xslen(_T("WHERE "));
	if(total + len > max)
		return total;

	if (buf)
	{
		xsprintf(buf + total, _T("%s"), _T("WHERE "));
	}
	total += len;

	withand = 0;
	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		field_type = get_col_field_type_ptr(clk);
		field_name = get_col_field_name_ptr(clk);
		table_name = get_col_table_name_ptr(clk);
		data_type = get_col_data_type_ptr(clk);

		if(is_null(field_name) || compare_text(field_type,-1,ATTR_FIELD_TYPE_PRIM,-1,1) != 0 || compare_text(table_name,-1,update_table,-1,1) != 0)
		{
			goto skiptwo;
		}
		withand = 1;
		if(get_cell_dirty(rlk,clk))
			prim = get_cell_prim_ptr(rlk, clk);
		else
			prim = get_cell_text_ptr(rlk,clk);
			

		if(is_null(prim))
		{
			len = xslen(field_name) + xslen(_T(" IS NULL AND "));
			if(total + len > max)
				return total;

			if (buf)
			{
				xsprintf(buf + total, _T("%s IS NULL AND "), field_name);
			}
			total += len;
		}else
		{
			if(compare_text(data_type,-1,ATTR_DATA_TYPE_STRING,-1,1) == 0)
			{
				len = xslen(field_name) + xslen(_T("='' AND ")) + xslen(prim);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s='%s' AND "), field_name, prim);
				}
				total += len;
			}
			else if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 1) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 1) == 0)
			{
				field_cast = get_col_field_cast_ptr(clk);
				if (is_null(field_cast))
				{
					xscpy(sz_date, prim);
				}
				else
				{
					parse_datetime(&dt, prim);
					format_datetime_ex(&dt, field_cast, sz_date, DATE_LEN);
				}

				len = xslen(field_name) + xslen(_T("='' AND ")) + xslen(sz_date);
				if (total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s='%s' AND "), field_name, sz_date);
				}
				total += len;
			}
			else
			{
				len = xslen(field_name) + xslen(_T("= AND ")) + xslen(prim);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s=%s AND "), field_name, prim);
				}
				total += len;
			}
		}
skiptwo:
		clk = get_next_col(ptr,clk);
		if(clk == NULL && buf && withand)/*remove last AND*/
			buf[total - 1] = buf[total - 2] = buf[total - 3] = buf[total - 4] = _T(' ');
	}

	/*include last \r\n*/
	if (total + 2 > max)
		return total;

	if (buf)
	{
		xsncpy(buf + total, bk, 2);
	}
	total += 2;

	return total;
}

int format_row_insert_sql(link_t_ptr ptr,link_t_ptr rlk,tchar_t* buf,int max)
{
	link_t_ptr clk;
	int total = 0;
	int len;
	const tchar_t* data_type;
	const tchar_t* token;
	const tchar_t* update_table;
	const tchar_t* field_name;
	const tchar_t* table_name;
	const tchar_t* field_cast;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	xdate_t dt = { 0 };
	tchar_t bk[3] = { _T('\r'), _T('\n'), _T('\0') };

	update_table = get_grid_update_table_ptr(ptr);
	if(is_null(update_table))
		return 0;

	len = xslen(_T("INSERT INTO ")) + xslen(update_table) + xslen(_T(" (")); 
	if(total + len > max)
		return total;

	if (buf)
	{
		xsprintf(buf + total, _T("INSERT INTO %s ("), update_table);
	}
	total += len;

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		table_name = get_col_table_name_ptr(clk);
		if(!get_col_integrity(clk) || compare_text(table_name,-1,update_table,-1,1) != 0)
		{
			goto skipone;
		}
		
		field_name = get_col_field_name_ptr(clk);
		len = xslen(field_name) + xslen(_T(","));
		if(total + len > max)
			return total;

		if (buf)
		{
			xsprintf(buf + total, _T("%s,"), field_name);
		}
		total += len;

skipone:
		clk = get_next_col(ptr,clk);
		
		if(clk == NULL && buf)
			buf[total - 1] = _T(')'); /*replace last ','*/
	}

	/*format where clause*/
	len = xslen(_T(" VALUES ("));
	if(total + len > max)
		return total;

	if (buf)
	{
		xsprintf(buf + total, _T("%s"), _T(" VALUES ("));
	}
	total += len;

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		table_name = get_col_table_name_ptr(clk);
		if(!get_col_integrity(clk)  || compare_text(table_name,-1,update_table,-1,1) != 0)
		{
			goto skiptwo;
		}

		token = get_cell_text_ptr(rlk,clk);
		if(is_null(token))
		{
			len = xslen(_T("NULL,"));
			if(total + len > max)
				return total;

			if (buf)
			{
				xsprintf(buf + total, _T("NULL,"));
			}
			total += len;
		}else
		{
			data_type = get_col_data_type_ptr(clk);
			if(compare_text(data_type,-1,ATTR_DATA_TYPE_STRING,-1,1) == 0)
			{
				len = xslen(_T("'',")) + xslen(token);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("'%s',"), token);
				}
				total += len;
			}
			else if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 1) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 1) == 0)
			{
				field_cast = get_col_field_cast_ptr(clk);
				if (is_null(field_cast))
				{
					xscpy(sz_date, token);
				}
				else
				{
					parse_datetime(&dt, token);
					format_datetime_ex(&dt, field_cast, sz_date, DATE_LEN);
				}

				len = xslen(_T("'',")) + xslen(sz_date);
				if (total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("'%s',"), sz_date);
				}
				total += len;
			}
			else
			{
				len = xslen(_T(",")) + xslen(token);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s,"), token);
				}
				total += len;
			}
		}
skiptwo:
		clk = get_next_col(ptr,clk);
		if(clk == NULL && buf)
			buf[total - 1] = _T(')'); /*replace last ','*/
	}

	/*include last \r\n*/
	if (total + 2 > max)
		return total;

	if (buf)
	{
		xsncpy(buf + total, bk, 2);
	}
	total += 2;

	return total;
}

int format_row_delete_sql(link_t_ptr ptr,link_t_ptr rlk,tchar_t* buf,int max)
{
	link_t_ptr clk;
	int total = 0;
	int len,withand;
	const tchar_t* data_type;
	const tchar_t* field_type;
	const tchar_t* prim;
	const tchar_t* update_table;
	const tchar_t* field_name;
	const tchar_t* table_name;
	const tchar_t* field_cast;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	xdate_t dt = { 0 };
	tchar_t bk[3] = { _T('\r'), _T('\n'), _T('\0') };

	update_table = get_grid_update_table_ptr(ptr);
	if(update_table == NULL)
		return 0;

	len = xslen(_T("DELETE ")) + xslen(update_table) + xslen(_T(" WHERE ")); /*UPDATE szTable */
	if(total + len > max)
		return total;

	if (buf)
	{
		xsprintf(buf + total, _T("DELETE %s WHERE "), update_table);
	}
	total += len;

	withand = 0;
	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		field_type = get_col_field_type_ptr(clk);
		field_name = get_col_field_name_ptr(clk);
		table_name = get_col_table_name_ptr(clk);

		if(is_null(field_name) || compare_text(field_type,-1,ATTR_FIELD_TYPE_PRIM,-1,1) != 0  || compare_text(table_name,-1,update_table,-1,1) != 0)
		{
			goto skip;
		}
		
		withand = 1;
		if(get_cell_dirty(rlk,clk))
			prim = get_cell_prim_ptr(rlk, clk);
		else
			prim = get_cell_text_ptr(rlk,clk);


		if(is_null(prim))
		{
			len = xslen(field_name) + xslen(_T(" IS NULL AND "));
			if(total + len > max)
				return total;

			if (buf)
			{
				xsprintf(buf + total, _T("%s IS NULL AND "), field_name);
			}
			total += len;
		}else
		{
			data_type = get_col_data_type_ptr(clk);
			if(compare_text(data_type,-1,ATTR_DATA_TYPE_STRING,-1,1) == 0)
			{
				len = xslen(field_name) + xslen(_T("='' AND ")) + xslen(prim);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s='%s' AND "), field_name, prim);
				}
				total += len;
			}
			else if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 1) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 1) == 0)
			{
				field_cast = get_col_field_cast_ptr(clk);
				if (is_null(field_cast))
				{
					xscpy(sz_date, prim);
				}
				else
				{
					parse_datetime(&dt, prim);
					format_datetime_ex(&dt, field_cast, sz_date, DATE_LEN);
				}

				len = xslen(field_name) + xslen(_T("='' AND ")) + xslen(sz_date);
				if (total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s='%s' AND "), field_name, sz_date);
				}
				total += len;
			}
			else
			{
				len = xslen(field_name) + xslen(_T("= AND ")) + xslen(prim);
				if(total + len > max)
					return total;

				if (buf)
				{
					xsprintf(buf + total, _T("%s=%s AND "), field_name, prim);
				}
				total += len;
			}
		}
skip:
		clk = get_next_col(ptr,clk);

		if(clk == NULL && buf && withand)//remove last AND
			buf[total - 1] = buf[total - 2] = buf[total - 3] = buf[total - 4] = _T(' ');
	}

	/*include last \r\n*/
	if (total + 2 > max)
		return total;

	if (buf)
	{
		xsncpy(buf + total, bk, 2);
	}
	total += 2;

	return total;
}

int format_grid_param_clause(link_t_ptr ptr,const tchar_t* clause, tchar_t* buf,int max)
{
	const tchar_t *token,*pre;
	const tchar_t *val;
	int tokenlen,len,total;

	if (is_null(clause))
		return 0;

	total = 0;
	pre = token = clause;

	while(token = parse_param_name(token,-1,_T(':'),&tokenlen))
	{
		if(tokenlen == 0)
			continue;

		val = get_grid_sql_param_ptr(ptr,token,tokenlen);
		
		len = (int)(token - pre) - 1;
		if (total + len > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, pre, len);
		}
		total += len;

		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, _T("'"), 1);
		}
		total++;

		len = xslen(val);
		if(total + len > max)
			return total;

		if(buf)
		{
			xsncpy(buf + total, val, len);
		}
		total += len;

		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, _T("'"), 1);
		}
		total++;
		
		pre = token + tokenlen;
	}

	len = xslen(pre); 
	if(total + len > max)
		return total;

	if (buf)
	{
		xsncpy(buf + total, pre, len);
	}
	total += len;

	/*include last balnk ' '*/
	if (total + 1 > max)
		return total;

	if (buf)
	{
		xsncpy(buf + total, _T(" "), 1);
	}
	total += 1;

	return total;		
}

int format_grid_select_sql(link_t_ptr ptr,tchar_t* buf,int max)
{
	link_t_ptr clk;
	int len,total = 0;
	const tchar_t* sz_limit;
	const tchar_t* field_type;
	const tchar_t* col_name;
	const tchar_t* field_name;
	const tchar_t* table_name;
	const tchar_t* sz_clause;

	len = xslen(_T("SELECT "));
	if(total + len > max)
		return total;

	if (buf)
	{
		xsprintf(buf + total, _T("SELECT "));
	}
	total += len;

	sz_limit = get_grid_limit_clause_ptr(ptr);
	len = xslen(sz_limit);
	if (total + len + 1> max)
		return total;

	if (buf)
	{
		xsncpy(buf + total, sz_limit, len);
		buf[total + len] = _T(' ');
	}
	total += (len + 1);

	if (total + 1 > max)
		return total;
	if (buf)
	{
		buf[total] = _T('\n');
	}
	total += 1;

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		col_name = get_col_name_ptr(clk);
		field_name = get_col_field_name_ptr(clk);
		table_name = get_col_table_name_ptr(clk);
		field_type = get_col_field_type_ptr(clk);

		if(is_null(col_name) || is_null(field_name))
		{
			goto skipone;
		}

		if (compare_text(field_type, -1, ATTR_FIELD_TYPE_CALC, -1, 1) != 0 && is_null(table_name))
		{
			goto skipone;
		}

		if(compare_text(field_type,-1,ATTR_FIELD_TYPE_CALC,-1,1) == 0) //calc field not include table name
		{
			len = xslen(field_name) + xslen(_T("\t AS ,\n")) + xslen(col_name);
			if(total + len > max)
				return total;

			if (buf)
			{
				xsprintf(buf + total, _T("\t%s AS %s,\n"), field_name, col_name);
			}
			total += len;

			goto skipone;
		}

		len = xslen(table_name) + xslen(field_name) + xslen(_T("\t. AS ,\n")) + xslen(col_name);
		if (total + len > max)
			return total;

		if (buf)
		{
			xsprintf(buf + total, _T("\t%s.%s AS %s,\n"), table_name, field_name, col_name);
		}
		total += len;

skipone:	

		clk = get_next_col(ptr,clk);

		if(!clk && buf && buf[total - 2] == _T(','))
			buf[total - 2] = _T(' '); //replace last ','
	}

	sz_clause = get_grid_from_clause_ptr(ptr);
	len = format_grid_param_clause(ptr, sz_clause, ((buf) ? (buf + total) : NULL), max - total);
	if (total + len > max)
		return total;
	total += len;

	if (len)
	{
		if (total + 1 > max)
			return total;
		if (buf)
		{
			buf[total] = _T('\n');
		}
		total += 1;
	}

	sz_clause = get_grid_where_clause_ptr(ptr);
	len = format_grid_param_clause(ptr, sz_clause, ((buf) ? (buf + total) : NULL), max - total);
	if (total + len > max)
		return total;
	total += len;

	if (len)
	{
		if (total + 1 > max)
			return total;
		if (buf)
		{
			buf[total] = _T('\n');
		}
		total += 1;
	}

	sz_clause = get_grid_group_clause_ptr(ptr);
	len = xslen(sz_clause);
	if (total + len > max)
		return total;

	if (buf)
	{
		xsncpy(buf + total, sz_clause, len);
	}
	total += len;

	return total;
}

int format_grid_exec_sql(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr rlk;
	int rs, total = 0;

	rlk = get_next_row(ptr, LINK_FIRST);
	while (rlk)
	{
		rs = get_row_state(rlk);

		if (rs == dsDelete)
		{
			total += format_row_delete_sql(ptr, rlk, ((buf)? (buf + total) : NULL), max - total);
		}
		else if (rs == dsNewDirty)
		{
			total += format_row_insert_sql(ptr, rlk, ((buf) ? (buf + total) : NULL), max - total);
		}
		else if (rs == dsDirty)
		{
			total += format_row_update_sql(ptr, rlk, ((buf) ? (buf + total) : NULL), max - total);
		}

		if (total >= max)
			return total;

		rlk = get_next_row(ptr, rlk);
	}

	return total;
}

int format_grid_exec_string(link_t_ptr ptr, string_t vs)
{
	link_t_ptr rlk;
	int rs, len, total = 0;
	tchar_t* buf;

	rlk = get_next_row(ptr, LINK_FIRST);
	while (rlk)
	{
		rs = get_row_state(rlk);

		if (rs == dsDelete)
		{
			len = format_row_delete_sql(ptr, rlk, NULL, MAX_LONG);
			buf = xsalloc(len + 1);
			format_row_delete_sql(ptr, rlk, buf, len);
		}
		else if (rs == dsNewDirty)
		{
			len = format_row_insert_sql(ptr, rlk, NULL, MAX_LONG);
			buf = xsalloc(len + 1);
			format_row_insert_sql(ptr, rlk, buf, len);
		}
		else if (rs == dsDirty)
		{
			len = format_row_update_sql(ptr, rlk, NULL, MAX_LONG);
			buf = xsalloc(len + 1);
			format_row_update_sql(ptr, rlk, buf, len);
		}
		else
		{
			buf = NULL;
			len = 0;
		}

		if (len)
			string_cat(vs, buf, len);

		if (buf)
			xsfree(buf);

		total += len;
		rlk = get_next_row(ptr, rlk);
	}

	return total;
}

#endif //XDLDOC