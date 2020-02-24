/***********************************************************************
	Easily xdb oci

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdb oci document

	@module	xdb_oci.c | xdb oci implement file

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

#include <oci.h>

#pragma comment(lib,"oci.lib")


typedef struct _db_t{
	xdb_head head;

	OCIEnv *env;
	OCIServer *srv;
	OCISvcCtx *ctx;
	OCISession *ses;
	OCIError *err;
	OCIStmt *stm;

	bool_t trans;
	int rows;
	tchar_t err_code[NUM_LEN + 1];
	tchar_t err_text[ERR_LEN + 1];
}db_t;

typedef struct _bindguid_t{
	OCIDefine* def;
	OCIBind* bin;
	sb2 io;
	tchar_t cp;
	void* src;
	sb2 ind;
	ub4 len;
	ub4 dot;
	void* buf;
}bindguid_t;

static void ocitodt(int type, tchar_t* dt)
{
	switch (type) {
	case SQLT_STR:
	case SQLT_VST:
	case SQLT_CHR:
	case SQLT_AFC:
	case SQLT_VCS:
	case SQLT_AVC:
	case SQLT_RDD:
	case SQLT_LNG:
#ifdef SQLT_INTERVAL_YM
	case SQLT_INTERVAL_YM:
#endif
#ifdef SQLT_INTERVAL_DS
	case SQLT_INTERVAL_DS:
#endif
		xscpy(dt, ATTR_DATA_TYPE_STRING);
		break;
	case SQLT_INT:
		xscpy(dt, ATTR_DATA_TYPE_INTEGER);
		break;
	case SQLT_FLT:
	case SQLT_NUM:
	case SQLT_VNU:
		xscpy(dt, ATTR_DATA_TYPE_NUMERIC);
		break;
	case SQLT_UIN:
		xscpy(dt, ATTR_DATA_TYPE_INTEGER);
		break;
	case SQLT_VBI:
	case SQLT_BIN:
	case SQLT_LBI:
	case SQLT_LVC:
	case SQLT_LVB:
	case SQLT_BLOB:
	case SQLT_CLOB:
	case SQLT_FILE:
	case SQLT_NTY:
	case SQLT_REF:
	case SQLT_RID:
		xscpy(dt, ATTR_DATA_TYPE_STRING);
		break;
	case SQLT_DAT:
	case SQLT_ODT:
#ifdef SQLT_TIMESTAMP
	case SQLT_TIMESTAMP:
	case SQLT_TIMESTAMP_TZ:
	case SQLT_TIMESTAMP_LTZ:
#endif
		xscpy(dt, ATTR_DATA_TYPE_DATETIME);
		break;
	default:
		xscpy(dt, ATTR_DATA_TYPE_STRING);
		break;
	}
}

static int split_semi(const tchar_t* token, int len)
{
	int tklen = 0, total = 0;
	bool_t glt = 0;
	const tchar_t* tkcur = token;

	if (len < 0)
		len = xslen(token);

	if (!len)
		return 0;

	while (*tkcur != _T(';') && *tkcur != _T('\0') && total < len)
	{
		if (*tkcur == _T('\'') || *tkcur == _T('\"'))
		{
			if (glt)
				glt = 0;
			else
				glt = 1;
		}

		tklen++;
		tkcur++;
		total++;

		if (glt)
		{
			while (*tkcur == _T(';'))
			{
				tklen++;
				tkcur++;
				total++;
			}
		}
	}

	return total;
}

static void _raise_oci_error(OCIError * oci_err)
{
	tchar_t err_code[NUM_LEN + 1] = { 0 };
	tchar_t err_text[ERR_LEN + 1] = { 0 };
	int errcode = 0;

	OCIErrorGet(oci_err, 1, (text*)NULL, &errcode, (text*)err_text, ERR_LEN, OCI_HTYPE_ERROR);

	xsprintf(err_code, _T("%d"), errcode);
	raise_user_error(err_code, err_text);
}

static void _db_reset(db_t* pdb)
{
	xscpy(pdb->err_code, _T(""));
	xscpy(pdb->err_text, _T(""));

	pdb->rows = 0;
}

void _db_tran(db_t* pdb)
{
	pdb->trans = 0;
	//OCITransStart(pdb->ctx, pdb->err, 90, OCI_TRANS_NEW);
}

void _db_commit(db_t* pdb)
{
	if (pdb->trans)
	{
		pdb->trans = 0;
		OCITransCommit(pdb->ctx, pdb->err, OCI_DEFAULT);
	}
}

void _db_rollback(db_t* pdb)
{
	if (pdb->trans)
	{
		pdb->trans = 0;
		OCITransRollback(pdb->ctx, pdb->err, OCI_DEFAULT);
	}
}

bool_t STDCALL db_parse_dsn(const tchar_t* dsnfile, tchar_t* srv_buf, int srv_len, tchar_t* dbn_buf, int dbn_len, tchar_t* uid_buf, int uid_len, tchar_t* pwd_buf, int pwd_len)
{
	LINKPTR d_ptr = NULL;

	TRY_CATCH;

	d_ptr = create_proper_doc();
	if (!load_proper_from_ini_file(d_ptr, NULL, dsnfile))
	{
		raise_user_error(_T("-1"), _T("parse dsn file failed"));
	}

	read_proper(d_ptr, _T("OCI"), -1, DSN_SERVER, -1, srv_buf, srv_len);
	read_proper(d_ptr, _T("OCI"), -1, DSN_DATABASE, -1, dbn_buf, dbn_len);
	read_proper(d_ptr, _T("OCI"), -1, DSN_UID, -1, uid_buf, uid_len);
	read_proper(d_ptr, _T("OCI"), -1, DSN_PWD, -1, pwd_buf, pwd_len);

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

xdb_t STDCALL db_open(const tchar_t* svc, const tchar_t* dbn, const tchar_t* uid, const tchar_t* pwd)
{
	db_t* pdb = NULL;

	OCIEnv *env = NULL;
	OCIServer *srv = NULL;
	OCISvcCtx *ctx = NULL;
	OCISession *ses = NULL;
	OCIError *err = NULL;

	TRY_CATCH;

	//OCIEnvInit((OCIEnv **)&env, OCI_DEFAULT, (size_t)0, (dvoid **)0);
#ifdef _UNICODE
	if (OCI_SUCCESS != OCIEnvNlsCreate(
		(OCIEnv **)&env,
		(ub4)0,
		(void *)0,
		(dvoid * (*)(dvoid *, size_t)) 0,
		(dvoid * (*)(dvoid *, dvoid *, size_t))0,
		(void(*)(dvoid *, dvoid *)) 0,
		(size_t)0,
		(void **)0,
		(ub2)OCI_UTF16ID,
		(ub2)OCI_UTF16ID))
	{
		raise_user_error(_T("-1"), _T("Alloc environment handle failed"));
	}
#else
	if (OCI_SUCCESS != OCIEnvCreate((OCIEnv **)&env,
		(ub4)OCI_DEFAULT, (dvoid *)0,
		(dvoid * (*)(dvoid *, size_t)) 0,
		(dvoid * (*)(dvoid *, dvoid *, size_t))0,
		(void(*)(dvoid *, dvoid *)) 0,
		(size_t)0, (dvoid **)0))
	{
		raise_user_error(_T("-1"), _T("Alloc environment handle failed"));
	}
#endif

	if (OCI_SUCCESS != OCIHandleAlloc((dvoid *)env, (dvoid **)&err,
		(ub4)OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0))
	{
		raise_user_error(_T("-1"), _T("Alloc error handle failed"));
	}

	if (OCI_SUCCESS != OCIHandleAlloc((dvoid *)env, (dvoid **)&srv,
		(ub4)OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0))
	{
		raise_user_error(_T("-1"), _T("Alloc server handle failed"));
	}

	if (OCI_SUCCESS != OCIHandleAlloc((dvoid *)env, (dvoid **)&ctx,
		(ub4)OCI_HTYPE_SVCCTX, (size_t)0, (dvoid **)0))
	{
		raise_user_error(_T("-1"), _T("Alloc context handle failed"));
	}

	if (OCI_SUCCESS != OCIHandleAlloc((dvoid *)env, (dvoid **)&ses,
		(ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid **)0))
	{
		raise_user_error(_T("-1"), _T("Alloc session handle failed"));
	}

	if (OCI_SUCCESS != OCILogon(
		env,
		err,
		&ctx,
		(text*)uid,
		xslen(uid) * sizeof(tchar_t),
		(text*)pwd,
		xslen(pwd) * sizeof(tchar_t),
		(text*)dbn,
		xslen(dbn) * sizeof(tchar_t)))
	{
		_raise_oci_error(err);
	}

	pdb = (db_t*)xmem_alloc(sizeof(db_t));
	pdb->head.dbt = _DB_OCI;
	pdb->head.cbs = sizeof(db_t);

	pdb->env = env;
	pdb->srv = srv;
	pdb->ctx = ctx;
	pdb->ses = ses;
	pdb->err = err;

	END_CATCH;

	return (xdb_t)pdb;

ONERROR:

	if (ses)
		OCIHandleFree((dvoid *)ses, OCI_HTYPE_SESSION);
	if (ctx)
		OCIHandleFree((dvoid *)ctx, OCI_HTYPE_SVCCTX);
	if (srv)
		OCIHandleFree((dvoid *)srv, OCI_HTYPE_SERVER);
	if (err)
		OCIHandleFree((dvoid *)err, OCI_HTYPE_ERROR);
	if (env)
		OCIHandleFree((dvoid *)env, OCI_HTYPE_ENV);

	return NULL;
}

void STDCALL db_close(xdb_t db)
{
	db_t* pdb = (db_t*)db;
	
	XDL_ASSERT(pdb != NULL);

	if (pdb->stm)
	{
		OCIHandleFree((dvoid *)pdb->stm, OCI_HTYPE_STMT);
		pdb->stm = NULL;
	}

	OCILogoff(pdb->ctx, pdb->err);

	OCIServerDetach(pdb->srv, pdb->err, OCI_DEFAULT);

	if (pdb->ses)
		OCIHandleFree((dvoid *)pdb->ses, OCI_HTYPE_SESSION);
	if (pdb->ctx)
		OCIHandleFree((dvoid *)pdb->ctx, OCI_HTYPE_SVCCTX);
	if (pdb->srv)
		OCIHandleFree((dvoid *)pdb->srv, OCI_HTYPE_SERVER);
	if (pdb->err)
		OCIHandleFree((dvoid *)pdb->err, OCI_HTYPE_ERROR);
	if (pdb->env)
		OCIHandleFree((dvoid *)pdb->env, OCI_HTYPE_ENV);
	
	xmem_free(pdb);
}


bool_t STDCALL db_datetime(xdb_t db, int diff, tchar_t* sz_time)
{
	db_t* pdb = (db_t*)db;

	OCIStmt *stm = NULL;
	OCIDefine* def = NULL;
	sb2 ind = 0;

	tchar_t sqlstr[MID_SQL_LEN] = { 0 };

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	if (!diff)
		xscpy(sqlstr, _T("select to_char(sysdate,'yyyy-mm-dd hh24:mi:ss') as DT from DUAL"));
	else
		xsprintf(sqlstr, _T("select to_char(sysdate + (%d),'yyyy-mm-dd hh24:mi:ss') as DT from DUAL"), diff);

	_db_reset(pdb);

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	if (OCI_ERROR == OCIStmtPrepare(stm, pdb->err, (text*)sqlstr, xslen(sqlstr) * sizeof(tchar_t), OCI_NTV_SYNTAX, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIStmtExecute(pdb->ctx, stm, pdb->err, 0, 0, 0, 0, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIDefineByPos(stm, &def, pdb->err, 1, (ub1*)sz_time, DATE_LEN * sizeof(tchar_t), SQLT_STR, &ind, 0, 0, OCI_DEFAULT))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIStmtFetch(stm, pdb->err, 1, OCI_FETCH_NEXT, OCI_DEFAULT))
	{
		_raise_oci_error(pdb->err);
	}

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	pdb->rows = 1;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_exec(xdb_t db, const tchar_t* sqlstr, int sqllen)
{
	db_t* pdb = (db_t*)db;

	OCIStmt *stm = NULL;
	ub4 row;
	int rt;

	tchar_t *tkpre, *tkcur;
	int tklen, rows = 0;
	bool_t uni = 0;

	if (sqllen < 0)
		sqllen = xslen(sqlstr);

	if (!sqllen || is_null(sqlstr))
	{
		return 1;
	}

	TRY_CATCH;

	XDL_ASSERT(pdb != NULL);
	
	_db_reset(pdb);

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	_db_tran(pdb);

	tkcur = (tchar_t*)sqlstr;
	while (sqllen)
	{
		tklen = split_line(tkcur, sqllen);

		tkpre = tkcur;
		tkcur += tklen;
		sqllen -= tklen;
		uni = 0;

		while (*tkcur == _T(' ') || *tkcur == _T('\t') || *tkcur == _T('\n') || *tkcur == _T('\r'))
		{
			if (*tkcur == _T('\r'))
				uni = 1;

			tkcur++;
			sqllen--;
		}

		if (!tklen)
		{
			continue;
		}

		rt = OCIStmtPrepare(stm, pdb->err, (text*)tkpre, tklen * sizeof(tchar_t), OCI_NTV_SYNTAX, OCI_DEFAULT);
		if (rt == OCI_ERROR)
		{
			_raise_oci_error(pdb->err);
		}

		rt = OCIStmtExecute(pdb->ctx, stm, pdb->err, 1, (ub4)0, NULL, NULL, (ub4)OCI_DEFAULT);
		if (rt == OCI_ERROR)
		{
			_raise_oci_error(pdb->err);
		}

		row = 0;
		OCIAttrGet(stm, OCI_HTYPE_STMT, &row, 0, OCI_ATTR_ROW_COUNT, pdb->err);

		if (uni && rows != 1)
		{
			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}

		rows += row;
	}

	_db_commit(pdb);

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	pdb->rows = rows;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
	{
		_db_rollback(pdb);

		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_update(xdb_t db, LINKPTR grid)
{
	db_t* pdb = (db_t*)db;
	
	OCIStmt *stm = NULL;
	ub4 row;
	tchar_t *tkcur;
	int rt, rows, tklen;

	LINKPTR rlk;
	int rs;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}
	
	_db_tran(pdb);

	rt = OCI_SUCCESS;
	tkcur = NULL;

	rows = 0;
	rlk = get_next_row(grid,LINK_FIRST);
	while(rlk)
	{
		tklen = 0;
		rs = get_row_state(rlk);
		if(rs == dsDelete)
		{
			tklen = format_row_delete_sql(grid, rlk, NULL, MAX_LONG);
			if(tklen > 0)
			{
				tkcur = (tchar_t*)xmem_alloc((tklen + 1) * sizeof(tchar_t));
				format_row_delete_sql(grid,rlk,tkcur,tklen);
			}
		}else if(rs == dsNewDirty)
		{
			tklen = format_row_insert_sql(grid, rlk, NULL, MAX_LONG);
			if(tklen > 0)
			{
				tkcur = (tchar_t*)xmem_alloc((tklen + 1) * sizeof(tchar_t));
				format_row_insert_sql(grid,rlk,tkcur,tklen);
			}
		}else if(rs == dsDirty)
		{
			tklen = format_row_update_sql(grid, rlk, NULL, MAX_LONG);
			if(tklen > 0)
			{
				tkcur = (tchar_t*)xmem_alloc((tklen + 1) * sizeof(tchar_t));
				format_row_update_sql(grid,rlk,tkcur,tklen);
			}
		}
		else
		{
			tklen = 0;
		}

		if (!tklen)
		{
			rlk = get_next_row(grid, rlk);
			continue;
		}

		rt = OCIStmtPrepare(stm, pdb->err, (text*)tkcur, tklen * sizeof(tchar_t), OCI_NTV_SYNTAX, OCI_DEFAULT);
		if (rt == OCI_ERROR)
		{
			_raise_oci_error(pdb->err);
		}

		rt = OCIStmtExecute(pdb->ctx, stm, pdb->err, 1, (ub4)0, NULL, NULL, (ub4)OCI_DEFAULT);
		if (rt == OCI_ERROR)
		{
			_raise_oci_error(pdb->err);
		}

		xmem_free(tkcur);
		tkcur = NULL;

		row = 0;
		OCIAttrGet(stm, OCI_HTYPE_STMT, &row, 0, OCI_ATTR_ROW_COUNT, pdb->err);

		if (row != 1)
		{
			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}
		else
		{
			rows += row;
		}

		rlk = get_next_row(grid,rlk);
	}
	
	_db_commit(pdb);

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	pdb->rows = rows;

	END_CATCH;

	return 1;

ONERROR:

	if (tkcur)
		xmem_free(tkcur);

	if (stm)
	{
		_db_rollback(pdb);

		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

int STDCALL _db_fetch_row(db_t* pdb, OCIStmt* stm, LINKPTR grid)
{
	LINKPTR clk,rlk;
	
	tchar_t colname[MAX_SQL_NAME] = { 0 };
	const tchar_t* data_type;
	const tchar_t* data_cast;
	int len,rows = 0;
	xdate_t dt = { 0 };

	OCIParam* cip;
	OCIDefine** pdef;
	text* cname;
	ub4 clen,i, cols;
	ub2 csize;
	ub2 rt = OCI_SUCCESS;

	byte_t** pbuf = NULL;
	sb2* pind = NULL;

	if (OCI_SUCCESS != OCIAttrGet(stm, OCI_HTYPE_STMT, &cols, 0, OCI_ATTR_PARAM_COUNT, pdb->err))
	{
		return XDB_FAILED;
	}

	pdef = (OCIDefine**)xmem_alloc(cols * sizeof(OCIDefine*));
	pind = (sb2*)xmem_alloc(cols * sizeof(sb2));
	pbuf = (byte_t**)xmem_alloc(cols * sizeof(byte_t*));

	for (i = 1; i <= cols; i++)
	{
		OCIParamGet(stm, OCI_HTYPE_STMT, pdb->err, (void**)&cip, i);

		xszero(colname, MAX_SQL_NAME);
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (text**)&cname, &clen, OCI_ATTR_NAME, pdb->err);
		if (clen > 0)
		{
			xmem_copy((void*)colname, (void*)cname, clen);
		}

		clk = get_col(grid, colname);
		if (!clk)
		{
			continue;
		}

		data_type = get_col_data_type_ptr(clk);
		if (compare_text(data_type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
		{
			len = get_col_data_len(clk);
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			len = 2;
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			len = NUM_LEN;
		}
		else if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			len = DATE_LEN;
		}
		else
		{
			len = MAX_SQL_NAME;
		}

		if (!len)
		{
			csize = 0;
			OCIAttrGet(cip, OCI_DTYPE_PARAM, &csize, 0, OCI_ATTR_DATA_SIZE, pdb->err);
			len = csize;
		}

		pbuf[i - 1] = (byte_t*)xmem_alloc((len + 1) * sizeof(tchar_t));

		OCIDefineByPos(stm, &pdef[i -1], pdb->err, i, (ub1*)pbuf[i -1], (len + 1) * sizeof(tchar_t), SQLT_STR, &pind[i -1], 0, 0, OCI_DEFAULT);
	}

	rows = 0;
	rt = OCI_SUCCESS;
	while (rt != OCI_ERROR)
	{
		rt = OCIStmtFetch(stm, pdb->err, 1, OCI_FETCH_NEXT, OCI_DEFAULT);

		if (rt == OCI_NO_DATA || rt == OCI_ERROR)
			break;

		rlk = insert_row(grid, LINK_LAST);
		set_row_state(rlk, dsClean);
		rows++;

		for (i = 1; i <= cols; i++)
		{
			OCIParamGet(stm, OCI_HTYPE_STMT, pdb->err, (void**)&cip, i);

			xszero(colname, MAX_SQL_NAME);
			OCIAttrGet(cip, OCI_DTYPE_PARAM, (text**)&cname, &clen, OCI_ATTR_NAME, pdb->err);
			if (clen > 0)
			{
				xmem_copy((void*)colname, (void*)cname, clen);
			}

			clk = get_col(grid, colname);
			if (clk == NULL)
				continue;

			if (!is_null((tchar_t*)(pbuf[i - 1])))
			{
				data_type = get_col_data_type_ptr(clk);
				if (compare_text(data_type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(data_type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
				{
					data_cast = get_col_field_cast_ptr(clk);
					if (!is_null(data_cast))
					{
						parse_datetime_ex(&dt, data_cast, (tchar_t*)(pbuf[i - 1]));
						format_datetime(&dt, (tchar_t*)(pbuf[i - 1]));
					}
				}

				set_cell_text(rlk, clk, (tchar_t*)(pbuf[i - 1]), -1);
			}
		}
	}

	for (i = 1; i <= cols; i++)
	{
		xmem_free(pbuf[i - 1]);
	}

	xmem_free(pbuf);
	xmem_free(pdef);
	xmem_free(pind);

	if (rt == OCI_SUCCESS || rt == OCI_SUCCESS_WITH_INFO)
		return XDB_PENDING;
	else if(rt == OCI_NO_DATA)
		return XDB_SUCCEED;
	else
		return XDB_FAILED;
}

bool_t STDCALL db_fetch(xdb_t db, LINKPTR grid)
{
	db_t* pdb = (db_t*)db;
	
	OCIStmt *stm = NULL;
	int rt;

	tchar_t* sqlstr = NULL;
	int sqllen;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	clear_grid_rowset(grid);

	sqllen = format_grid_select_sql(grid, NULL, MAX_LONG);
	if (sqllen <= 0)
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	sqlstr = (tchar_t*)xmem_alloc((sqllen + 1) * sizeof(tchar_t));
	format_grid_select_sql(grid,sqlstr,sqllen);

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	if (OCI_ERROR == OCIStmtPrepare(stm, pdb->err, (text*)sqlstr, xslen(sqlstr) * sizeof(tchar_t), OCI_NTV_SYNTAX, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIStmtExecute(pdb->ctx, stm, pdb->err, 0, 0, 0, 0, 0))
	{
		_raise_oci_error(pdb->err);
	}

	xmem_free(sqlstr);
	sqlstr = NULL;
	
	rt = XDB_PENDING;
	while (rt == XDB_PENDING)
	{
		rt = _db_fetch_row(pdb, stm, grid);
	}

	if (rt == XDB_FAILED)
	{
		_raise_oci_error(pdb->err);
	}

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	pdb->rows = get_row_count(grid);

	END_CATCH;

	return 1;

ONERROR:
	if (sqlstr)
		xmem_free(sqlstr);

	if (stm)
		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_select(xdb_t db, LINKPTR grid, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;
	LINKPTR clk;
	
	int rt;
	int rows = 0;
	tchar_t coltype[MAX_SQL_NAME] = { 0 };
	tchar_t colname[MAX_SQL_NAME] = { 0 };

	OCIStmt *stm = NULL;
	OCIParam* cip;
	text* cname;
	ub4 i,cols,ind;
	ub2 ctype,csize,cprec,cscale;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;
	
	_db_reset(pdb);

	clear_grid_rowset(grid);
	clear_grid_colset(grid);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	if (OCI_ERROR == OCIStmtPrepare(stm, pdb->err, (text*)sqlstr, xslen(sqlstr) * sizeof(tchar_t), OCI_NTV_SYNTAX, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIStmtExecute(pdb->ctx, stm, pdb->err, 0, 0, 0, 0, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIAttrGet(stm, OCI_HTYPE_STMT, (void*)&cols, 0, OCI_ATTR_PARAM_COUNT, pdb->err))
	{
		_raise_oci_error(pdb->err);
	}

	for (i = 1; i <= cols; i++)
	{
		OCIParamGet(stm, OCI_HTYPE_STMT, pdb->err, (void**)&cip, i);
		
		xmem_zero((void*)colname, MAX_SQL_NAME * sizeof(tchar_t));
		OCIAttrGet(cip, OCI_DTYPE_PARAM, &cname, &ind, OCI_ATTR_NAME, pdb->err);
		if (ind > 0)
			xmem_copy((void*)colname, cname, ind);

		ctype = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&ctype, 0, OCI_ATTR_DATA_TYPE, pdb->err);

		csize = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&csize, 0, OCI_ATTR_DATA_SIZE, pdb->err);

		cprec = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&cprec, 0, OCI_ATTR_PRECISION, pdb->err);

		cscale = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&cscale, 0, OCI_ATTR_SCALE, pdb->err);
		
		clk = insert_col(grid, LINK_LAST);

		set_col_name(clk, colname);
		set_col_title(clk, colname);

		ocitodt(ctype, coltype);
		set_col_data_type(clk, coltype);

		set_col_data_len(clk, csize);

		if (cscale < 0)
			cscale = 0;
		if (cscale > MAX_FLOAT_DIGI)
			cscale = MAX_FLOAT_DIGI;
		set_col_data_dig(clk, cscale);

	}

	rt = XDB_PENDING;
	while (rt == XDB_PENDING)
	{
		rt = _db_fetch_row(pdb, stm, grid);
	}

	if (rt == XDB_FAILED)
	{
		_raise_oci_error(pdb->err);
	}

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	pdb->rows = get_row_count(grid);

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_schema(xdb_t db, LINKPTR grid, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;
	LINKPTR clk;

	int rows = 0;
	tchar_t coltype[MAX_SQL_NAME] = { 0 };
	tchar_t colname[MAX_SQL_NAME] = { 0 };

	OCIStmt *stm = NULL;
	OCIParam* cip;
	text* cname;
	ub4 i, cols, ind;
	ub2 ctype, csize, cprec, cscale;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	clear_grid_rowset(grid);
	clear_grid_colset(grid);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	if (OCI_ERROR == OCIStmtPrepare(stm, pdb->err, (text*)sqlstr, xslen(sqlstr) * sizeof(tchar_t), OCI_NTV_SYNTAX, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIStmtExecute(pdb->ctx, stm, pdb->err, 0, 0, 0, 0, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIAttrGet(stm, OCI_HTYPE_STMT, (void*)&cols, 0, OCI_ATTR_PARAM_COUNT, pdb->err))
	{
		_raise_oci_error(pdb->err);
	}

	for (i = 1; i <= cols; i++)
	{
		OCIParamGet(stm, OCI_HTYPE_STMT, pdb->err, (void**)&cip, i);

		xmem_zero((void*)colname, MAX_SQL_NAME * sizeof(tchar_t));
		OCIAttrGet(cip, OCI_DTYPE_PARAM, &cname, &ind, OCI_ATTR_NAME, pdb->err);
		if (ind > 0)
			xmem_copy((void*)colname, cname, ind);

		ctype = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&ctype, 0, OCI_ATTR_DATA_TYPE, pdb->err);

		csize = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&csize, 0, OCI_ATTR_DATA_SIZE, pdb->err);

		cprec = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&cprec, 0, OCI_ATTR_PRECISION, pdb->err);

		cscale = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&cscale, 0, OCI_ATTR_SCALE, pdb->err);

		clk = insert_col(grid, LINK_LAST);

		set_col_name(clk, colname);
		set_col_title(clk, colname);

		ocitodt(ctype, coltype);
		set_col_data_type(clk, coltype);

		if (compare_text(coltype, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(coltype, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			set_col_data_len(clk, DATE_LEN);
		}
		else if (compare_text(coltype, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
		{
			set_col_data_len(clk, INT_LEN);
		}
		else if (compare_text(coltype, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			set_col_data_len(clk, cprec);
			set_col_data_dig(clk, cscale);
		}
		else
		{
			set_col_data_len(clk, csize);
		}
	}

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	pdb->rows = get_row_count(grid);

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

int _db_call_argv(db_t* pdb, const tchar_t* procname, const tchar_t* fmt, va_list* parg)
{
	OCIStmt *stm = NULL;
	sword rt;

	bindguid_t* pdg = NULL;

	tchar_t vname[10] = { 0 };

	tchar_t* sqlstr = NULL;
	int sqllen, pas, ind;
	const tchar_t* token;

	if (is_null(procname))
		return 0;

	TRY_CATCH;

	pdb->rows = 0;

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	pas = 0;
	token = fmt;
	while (token && *token)
	{
		if (*token == '%')
			pas++;

		token++;
	}

	sqllen = xslen(_T("begin (); end;")) + xslen(procname) + pas * 10;
	sqlstr = xsalloc(sqllen + 1);

	xsprintf(sqlstr, _T("begin %s("), procname);
	for (ind = 0; ind < pas; ind++)
	{
		xsprintf(vname, _T(":v%d,"), ind + 1);
		xscat(sqlstr, vname);
	}

	sqllen = xslen(sqlstr);
	if (sqlstr[sqllen - 1] == _T('('))
	{
		sqlstr[sqllen - 1] = _T(';');
	}
	else
	{
		sqlstr[sqllen - 1] = _T(')');
		sqlstr[sqllen] = _T(';');
		sqlstr[sqllen + 1] = _T('\0');
	}

	xscat(sqlstr, _T(" end;"));

	sqllen = xslen(sqlstr);
	rt = OCIStmtPrepare(stm, pdb->err, (text*)sqlstr, sqllen * sizeof(tchar_t), OCI_NTV_SYNTAX, OCI_DEFAULT);
	if (rt == OCI_ERROR)
	{
		_raise_oci_error(pdb->err);
	}

	xsfree(sqlstr);
	sqlstr = NULL;

	pdg = (bindguid_t*)xmem_alloc(pas * sizeof(bindguid_t));

	ind = 0;
	token = fmt;
	while (token && *token)
	{
		while (*token && *token != _T('%'))
			token++;

		if (!*token)
			break;

		if (*token == _T('%'))
			token++;
		if (*token == _T('-'))
		{
			token++;
			pdg[ind].io = 1;
		}
		else if (*token == _T('+'))
		{
			token++;
			pdg[ind].io = 2;
		}
		else
		{
			pdg[ind].io = 0;
		}

		if (*token >= _T('0') && *token <= _T('9'))
			pdg[ind].len = xstol(token);

		while (*token >= _T('0') && *token <= _T('9'))
			token++;

		if (*token == _T('.'))
		{
			token++;
			pdg[ind].dot = xstol(token);
			while (*token >= _T('0') && *token <= _T('9'))
				token++;
		}

		rt = OCI_SUCCESS;

		if (*token == _T('s'))
		{
			pdg[ind].src = va_arg(*parg, tchar_t*);
			pdg[ind].cp = _T('s');
			if (!pdg[ind].len)
				pdg[ind].len = (xslen((tchar_t*)pdg[ind].src) + 1) * sizeof(tchar_t);
			else
				pdg[ind].len = (pdg[ind].len + 1) * sizeof(tchar_t);
			pdg[ind].buf = xmem_alloc(pdg[ind].len);
			xscpy((tchar_t*)pdg[ind].buf, (tchar_t*)pdg[ind].src);
			if (is_null((tchar_t*)pdg[ind].src))
				pdg[ind].ind = -1;
			else
				pdg[ind].ind = pdg[ind].len;
			OCIBindByPos(stm, &(pdg[ind].bin), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_STR, &(pdg[ind].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
		}
		else if (*token == _T('d'))
		{
			pdg[ind].src = va_arg(*parg, int*);
			pdg[ind].cp = _T('d');
			pdg[ind].len = sizeof(int);
			pdg[ind].buf = xmem_alloc(sizeof(int));
			xmem_copy((void*)pdg[ind].buf, (void*)pdg[ind].src, sizeof(int));
			OCIBindByPos(stm, &(pdg[ind].bin), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_INT, &(pdg[ind].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
		}
		else if (*token == _T('f'))
		{
			pdg[ind].src = va_arg(*parg, double*);
			pdg[ind].cp = _T('f');
			pdg[ind].len = sizeof(OCINumber);
			pdg[ind].buf = xmem_alloc(sizeof(OCINumber));
			OCINumberFromReal(pdb->err, pdg[ind].src, sizeof(double), (OCINumber*)pdg[ind].buf);
			OCIBindByPos(stm, &(pdg[ind].bin), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_VNU, &(pdg[ind].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
		}
		else
		{
			break;
		}

		ind++;
	}

	for (ind = 0; ind < pas; ind++)
	{
		if (pdg[ind].io > 0)
		{
			switch (pdg[ind].cp)
			{
			case _T('s'):
				OCIDefineByPos(stm, &(pdg[ind].def), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_STR, &(pdg[ind].ind), NULL, NULL, (ub4)OCI_DEFAULT);
				break;
			case _T('d'):
				OCIDefineByPos(stm, &(pdg[ind].def), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_INT, &(pdg[ind].ind), NULL, NULL, (ub4)OCI_DEFAULT);
				break;
			case _T('f'):
				OCIDefineByPos(stm, &(pdg[ind].def), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_VNU, &(pdg[ind].ind), NULL, NULL, (ub4)OCI_DEFAULT);
				break;
			}
		}
	}

	rt = OCIStmtExecute(pdb->ctx, stm, pdb->err, 1, 0, NULL, NULL, OCI_DEFAULT | OCI_COMMIT_ON_SUCCESS);
	if (rt == OCI_ERROR)
	{
		_raise_oci_error(pdb->err);
	}

	for (ind = 0; ind < pas; ind++)
	{
		if (pdg[ind].io > 0)
		{
			switch (pdg[ind].cp)
			{
			case _T('s'):
				xscpy((tchar_t*)pdg[ind].src, (tchar_t*)pdg[ind].buf);
				break;
			case _T('d'):
				xmem_copy((void*)pdg[ind].src, (void*)pdg[ind].buf, sizeof(int));
				break;
			case _T('f'):
				OCINumberToReal(pdb->err, (OCINumber*)pdg[ind].buf, sizeof(double), pdg[ind].src);
				break;
			}
		}
	}

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	for (ind = 0; ind < pas; ind++)
	{
		if (pdg[ind].buf)
			xmem_free(pdg[ind].buf);
	}
	xmem_free(pdg);
	pdg = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
	{
		_db_rollback(pdb);

		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	if (sqlstr)
		xmem_free(sqlstr);

	if (pdg)
	{
		for (ind = 0; ind < pas; ind++)
		{
			if (pdg[ind].buf)
				xmem_free(pdg[ind].buf);
		}
		xmem_free(pdg);
	}

	return 0;
}

int db_call_argv(xdb_t db, const tchar_t* procname, const tchar_t* fmt, ...)
{
	db_t* pdb = (db_t*)db;
	va_list arg;
	int rt;

	va_start(arg, fmt);
	rt = _db_call_argv(pdb, procname, fmt, &arg);
	va_end(arg);

	return rt;
}

bool_t STDCALL db_call_func(xdb_t db, LINKPTR func)
{
	db_t* pdb = (db_t*)db;
	OCIStmt *stm = NULL;
	sword rt;

	bindguid_t* pdg = NULL;

	tchar_t vname[10] = { 0 };

	tchar_t* sqlstr = NULL;
	int sqllen, pas, ind;
	const tchar_t* procname;
	int na;
	double nf;
	const tchar_t* txt;

	LINKPTR flk;

	TRY_CATCH;

	pdb->rows = 0;

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	pas = get_func_param_count(func);
	procname = get_func_name_ptr(func);

	sqllen = xslen(_T("begin (); end;")) + xslen(procname) + pas * 10;
	sqlstr = xsalloc(sqllen + 1);

	xsprintf(sqlstr, _T("begin %s("), procname);
	for (ind = 0; ind < pas; ind++)
	{
		xsprintf(vname, _T(":v%d,"), ind + 1);
		xscat(sqlstr, vname);
	}

	sqllen = xslen(sqlstr);
	if (sqlstr[sqllen - 1] == _T('('))
	{
		sqlstr[sqllen - 1] = _T(';');
	}
	else
	{
		sqlstr[sqllen - 1] = _T(')');
		sqlstr[sqllen] = _T(';');
		sqlstr[sqllen + 1] = _T('\0');
	}

	xscat(sqlstr, _T(" end;"));

	sqllen = xslen(sqlstr);
	rt = OCIStmtPrepare(stm, pdb->err, (text*)sqlstr, sqllen * sizeof(tchar_t), OCI_NTV_SYNTAX, OCI_DEFAULT);
	if (rt == OCI_ERROR)
	{
		_raise_oci_error(pdb->err);
	}

	xsfree(sqlstr);
	sqlstr = NULL;

	pdg = (bindguid_t*)xmem_alloc(pas * sizeof(bindguid_t));

	ind = 0;
	flk = get_func_next_param(func, LINK_FIRST);
	while (flk)
	{
		if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_OUTPUT, -1, 0) == 0)
		{
			pdg[ind].io = 1;
		}
		else if (compare_text(get_func_param_type_ptr(flk), -1, ATTR_PARAM_TYPE_INPUTOUTPUT, -1, 0) == 0)
		{
			pdg[ind].io = 2;
		}
		else
		{
			pdg[ind].io = 0;
		}

		pdg[ind].len = get_func_param_data_len(flk);
		pdg[ind].dot = get_func_param_data_dig(flk);

		if (!pdg[ind].dot)
			pdg[ind].dot = MAX_FLOAT_DIGI;

		rt = OCI_SUCCESS;

		if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
		{
			pdg[ind].src = (void*)flk;
			pdg[ind].cp = _T('d');
			pdg[ind].len = sizeof(int);
			pdg[ind].buf = xmem_alloc(sizeof(int));
			na = get_func_param_integer(flk);
			xmem_copy((void*)pdg[ind].buf, (void*)&na, sizeof(int));
			OCIBindByPos(stm, &(pdg[ind].bin), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_INT, &(pdg[ind].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
		}
		else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			pdg[ind].src = (void*)flk;
			pdg[ind].cp = _T('f');
			pdg[ind].len = sizeof(OCINumber);
			pdg[ind].buf = xmem_alloc(sizeof(OCINumber));
			nf = get_func_param_numeric(flk);
			OCINumberFromReal(pdb->err, &nf, sizeof(double), (OCINumber*)pdg[ind].buf);
			OCIBindByPos(stm, &(pdg[ind].bin), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_VNU, &(pdg[ind].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
		}
		else if (compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(get_func_data_type_ptr(flk), -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			pdg[ind].src = (void*)flk;
			pdg[ind].cp = _T('s');
			pdg[ind].len = (DATE_LEN + 1) * sizeof(tchar_t);
			pdg[ind].buf = xmem_alloc(pdg[ind].len);
			txt = get_func_param_text_ptr(flk);
			xscpy((tchar_t*)pdg[ind].buf, txt);
			if (is_null(txt))
				pdg[ind].ind = -1;
			else
				pdg[ind].ind = pdg[ind].len;
			OCIBindByPos(stm, &(pdg[ind].bin), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_STR, &(pdg[ind].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
		}
		else
		{
			pdg[ind].src = (void*)flk;
			pdg[ind].cp = _T('s');
			txt = get_func_param_text_ptr(flk);
			if (!pdg[ind].len)
				pdg[ind].len = (xslen(txt) + 1) * sizeof(tchar_t);
			else
				pdg[ind].len = (pdg[ind].len + 1) * sizeof(tchar_t);
			pdg[ind].buf = xmem_alloc(pdg[ind].len);
			xscpy((tchar_t*)pdg[ind].buf, txt);
			if (is_null(txt))
				pdg[ind].ind = -1;
			else
				pdg[ind].ind = pdg[ind].len;
			OCIBindByPos(stm, &(pdg[ind].bin), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_STR, &(pdg[ind].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
		}

		ind++;
		flk = get_func_next_param(func, flk);
	}

	for (ind = 0; ind < pas; ind++)
	{
		if (pdg[ind].io > 0)
		{
			switch (pdg[ind].cp)
			{
			case _T('s'):
				OCIDefineByPos(stm, &(pdg[ind].def), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_STR, &(pdg[ind].ind), NULL, NULL, (ub4)OCI_DEFAULT);
				break;
			case _T('d'):
				OCIDefineByPos(stm, &(pdg[ind].def), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_INT, &(pdg[ind].ind), NULL, NULL, (ub4)OCI_DEFAULT);
				break;
			case _T('f'):
				OCIDefineByPos(stm, &(pdg[ind].def), pdb->err, ind + 1, pdg[ind].buf, pdg[ind].len, SQLT_VNU, &(pdg[ind].ind), NULL, NULL, (ub4)OCI_DEFAULT);
				break;
			}
		}
	}

	rt = OCIStmtExecute(pdb->ctx, stm, pdb->err, 1, 0, NULL, NULL, OCI_DEFAULT | OCI_COMMIT_ON_SUCCESS);
	if (rt == OCI_ERROR)
	{
		_raise_oci_error(pdb->err);
	}

	for (ind = 0; ind < pas; ind++)
	{
		if (pdg[ind].io > 0)
		{
			switch (pdg[ind].cp)
			{
			case _T('s'):
				flk = (LINKPTR)pdg[ind].src;
				set_func_param_text(flk, (tchar_t*)pdg[ind].buf, -1);
				break;
			case _T('d'):
				flk = (LINKPTR)pdg[ind].src;
				xmem_copy((void*)na, (void*)pdg[ind].buf, sizeof(int));
				set_func_param_integer(flk, na);
				break;
			case _T('f'):
				flk = (LINKPTR)pdg[ind].src;
				OCINumberToReal(pdb->err, (OCINumber*)pdg[ind].buf, sizeof(double), &nf);
				set_func_param_numeric(flk, nf);
				break;
			}
		}
	}

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	for (ind = 0; ind < pas; ind++)
	{
		if (pdg[ind].buf)
			xmem_free(pdg[ind].buf);
	}
	xmem_free(pdg);
	pdg = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
	{
		_db_rollback(pdb);

		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	if (sqlstr)
		xmem_free(sqlstr);

	if (pdg)
	{
		for (ind = 0; ind < pas; ind++)
		{
			if (pdg[ind].buf)
				xmem_free(pdg[ind].buf);
		}
		xmem_free(pdg);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
bool_t STDCALL _db_prepare(db_t* pdb, const tchar_t* sqlstr)
{
	ub4 rows;
	sword rt = OCI_SUCCESS;

	XDL_ASSERT(pdb != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (is_null(sqlstr))
	{
		raise_user_error(_T("-1"), _T("Empty sql statement"));
	}

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&pdb->stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	if (OCI_ERROR == OCIStmtPrepare(pdb->stm, pdb->err, (text*)sqlstr, xslen(sqlstr) * sizeof(tchar_t), OCI_NTV_SYNTAX, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIStmtExecute(pdb->ctx, pdb->stm, pdb->err, 0, 0, 0, 0, 0))
	{
		_raise_oci_error(pdb->err);
	}

	if (OCI_ERROR == OCIAttrGet(pdb->stm, OCI_HTYPE_STMT, (void*)&rows, 0, OCI_ATTR_ROW_COUNT, pdb->err))
	{
		_raise_oci_error(pdb->err);
	}

	pdb->rows = rows;

	END_CATCH;

	return 1;

ONERROR:

	if (pdb->stm)
	{
		OCIHandleFree((dvoid *)pdb->stm, OCI_HTYPE_STMT);
		pdb->stm = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_export(xdb_t db, stream_t stream, const tchar_t* sqlstr)
{
	db_t* pdb = (db_t*)db;

	tchar_t coltype[MAX_SQL_NAME] = { 0 };
	tchar_t colname[MAX_SQL_NAME] = { 0 };

	OCIParam* cip;
	text* cname;
	ub4 i, rows, cols, ind;
	ub2 ctype, csize, cprec, cscale;
	sword rt = OCI_SUCCESS;

	dword_t pos;
	int len;
	bindguid_t* pdg = NULL;

	string_t vs = NULL;

	tchar_t feed[3] = { TXT_ITEMFEED, TXT_LINEFEED, _T('\n') };

	tchar_t* sz_esc = NULL;
	int len_esc = 0;
	int len_buf = 0;

	XDL_ASSERT(pdb != NULL);

	if (!pdb->stm)
	{
		_db_prepare(pdb, sqlstr);
	}

	if (!stream)
	{
		return (pdb->stm) ? 1 : 0;
	}

	TRY_CATCH;

	if (!pdb->stm)
	{
		raise_user_error(_T("-1"), ERR_TEXT_INVALIDSTMT);
	}

	if (OCI_ERROR == OCIAttrGet(pdb->stm, OCI_HTYPE_STMT, (void*)&cols, 0, OCI_ATTR_PARAM_COUNT, pdb->err))
	{
		_raise_oci_error(pdb->err);
	}

	stream_write_utfbom(stream, NULL);

	vs = string_alloc();

	pdg = (bindguid_t*)xmem_alloc(cols * sizeof(bindguid_t));

	pos = 0;
	for (i = 0; i < cols; i++)
	{
		OCIParamGet(pdb->stm, OCI_HTYPE_STMT, pdb->err, (void**)&cip, i + 1);

		xmem_zero((void*)colname, MAX_SQL_NAME * sizeof(tchar_t));
		OCIAttrGet(cip, OCI_DTYPE_PARAM, &cname, &ind, OCI_ATTR_NAME, pdb->err);
		if (ind > 0)
			xmem_copy((void*)colname, cname, ind);

		ctype = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&ctype, 0, OCI_ATTR_DATA_TYPE, pdb->err);

		csize = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&csize, 0, OCI_ATTR_DATA_SIZE, pdb->err);

		cprec = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&cprec, 0, OCI_ATTR_PRECISION, pdb->err);

		cscale = 0;
		OCIAttrGet(cip, OCI_DTYPE_PARAM, (void*)&cscale, 0, OCI_ATTR_SCALE, pdb->err);

		ocitodt(ctype, coltype);

		if (cscale < 0)
			cscale = 0;
		if (cscale > MAX_FLOAT_DIGI)
			cscale = MAX_FLOAT_DIGI;

		if (compare_text(coltype, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
		{
			len = (int)csize;
		}
		else if (compare_text(coltype, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			len = 2;
		}
		else if (compare_text(coltype, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0 || compare_text(coltype, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			len = NUM_LEN;
		}
		else if (compare_text(coltype, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0 || compare_text(coltype, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			len = DATE_LEN;
		}
		else
		{
			len = MAX_SQL_DATA;
		}

		pdg[i].len = (len + 1) * sizeof(tchar_t);
		pdg[i].ind = 0;
		pdg[i].buf = (byte_t*)xmem_alloc(pdg[i].len);

		OCIDefineByPos(pdb->stm, &(pdg[i].def), pdb->err, i + 1, (ub1*)(pdg[i].buf), (pdg[i].len), SQLT_STR, &(pdg[i].ind), 0, 0, OCI_DEFAULT);
		
		string_cat(vs, colname, -1);
		string_cat(vs, feed, 1);
	}
	string_cat(vs, feed + 1, 2);

	if (!stream_write_line(stream, vs, &pos))
	{
		raise_user_error(NULL, NULL);
	}

	string_empty(vs);

	rows = 0;
	
	rt = OCI_SUCCESS;
	while (rt == OCI_SUCCESS || rt == OCI_SUCCESS_WITH_INFO)
	{
		rt = OCIStmtFetch(pdb->stm, pdb->err, 1, OCI_FETCH_NEXT, OCI_DEFAULT);

		if (rt != OCI_SUCCESS && rt != OCI_SUCCESS_WITH_INFO)
			break;

		rows++;

		pos = 0;
		for (i = 0; i < cols; i++)
		{
			if (pdg[i].ind >= 0)
			{
				len_buf = xslen((tchar_t*)pdg[i].buf);
				len_esc = csv_char_encode(pdg[i].buf, len_buf, NULL, MAX_LONG);
				if (len_esc != len_buf)
				{
					sz_esc = xsalloc(len_esc + 1);
					csv_char_decode(pdg[i].buf, len_buf, sz_esc, len_esc);

					string_cat(vs, sz_esc, len_esc);
					xsfree(sz_esc);
				}
				else
				{
					string_cat(vs, (tchar_t*)(pdg[i].buf), len_buf);
				}
			}
			string_cat(vs, feed, 1);
		}

		string_cat(vs, feed + 1, 2);

		if (!stream_write_line(stream, vs, &pos))
		{
			raise_user_error(NULL, NULL);
		}

		string_empty(vs);
	}
	
	string_empty(vs);

	string_free(vs);
	vs = NULL;

	stream_write_line(stream, NULL, NULL);

	if (!stream_flush(stream))
	{
		raise_user_error(NULL, NULL);
	}

	OCIHandleFree((dvoid *)pdb->stm, OCI_HTYPE_STMT);
	pdb->stm = NULL;

	if (rt == OCI_ERROR)
	{
		_raise_oci_error(pdb->err);
	}

	for (i = 0; i < cols; i++)
	{
		if (pdg[i].buf)
		{
			xmem_free(pdg[i].buf);
			pdg[i].buf = NULL;
			pdg[i].len = 0;
		}
	}
	xmem_free(pdg);

	pdb->rows = (int)rows;

	END_CATCH;

	return 1;

ONERROR:

	if (pdb->stm)
	{
		OCIHandleFree((dvoid *)pdb->stm, OCI_HTYPE_STMT);
		pdb->stm = NULL;
	}

	if (vs)
	{
		string_free(vs);
	}

	if (pdg)
	{
		for (i = 0; i < cols; i++)
		{
			if (pdg[i].buf)
			{
				xmem_free(pdg[i].buf);
				pdg[i].buf = NULL;
				pdg[i].len = 0;
			}
		}
		xmem_free(pdg);
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

bool_t STDCALL db_import(xdb_t db, stream_t stream, const tchar_t* table)
{
	db_t* pdb = (db_t*)db;

	OCIStmt *stm = NULL;
	ub4 i, ne, cols;
	sword rt = OCI_SUCCESS;

	bindguid_t* pdg = NULL;

	string_t vs = NULL;
	string_t vs_sql = NULL;
	const tchar_t* token;
	int tklen;
	int rows;
	dword_t dw;

	tchar_t* sz_esc = NULL;
	int len_esc = 0;

	XDL_ASSERT(pdb != NULL);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	_db_tran(pdb);

	stream_read_utfbom(stream, NULL);

	vs = string_alloc();

	dw = 0;
	if (!stream_read_line(stream, vs, &dw))
	{
		raise_user_error(_T("-1"), _T("read stream failed"));
	}

	vs_sql = string_alloc();

	string_printf(vs_sql, _T("insert into %s ("), table);

	cols = 0;
	token = string_ptr(vs);
	while (*token != TXT_LINEFEED && *token != _T('\0'))
	{
		tklen = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			tklen++;
		}

		string_cat(vs_sql, token - tklen, tklen);
		string_cat(vs_sql, _T(","), 1);

		if (*token == TXT_ITEMFEED)
			token++;

		cols++;
	}

	if (!cols)
	{
		raise_user_error(_T("-1"), _T("empty sql statement"));
	}

	tklen = string_len(vs_sql);
	string_set_char(vs_sql, tklen - 1, _T(')'));

	string_cat(vs_sql, _T(" values ("), -1);

	for (i = 0; i < cols; i++)
	{
		string_append(vs_sql, _T(":v%d,"), i + 1);
	}

	tklen = string_len(vs_sql);
	string_set_char(vs_sql, tklen - 1, _T(')'));

	if (OCI_ERROR == OCIStmtPrepare(stm, pdb->err, (text*)string_ptr(vs_sql), string_len(vs_sql) * sizeof(tchar_t), OCI_NTV_SYNTAX, 0))
	{
		_raise_oci_error(pdb->err);
	}

	string_free(vs_sql);
	vs_sql = NULL;

	pdg = (bindguid_t*)xmem_alloc(cols * sizeof(bindguid_t));

	rows = 0;
	string_empty(vs);

	while (1)
	{
		string_empty(vs);
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			raise_user_error(_T("-1"), _T("stream read line failed"));
		}

		if (string_len(vs) == 0)
			break;

		i = 0;
		token = string_ptr(vs);
		while (*token != TXT_LINEFEED && *token != _T('\0'))
		{
			tklen = 0;
			while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
			{
				token++;
				tklen++;
			}

			len_esc = csv_char_decode(token - tklen, tklen, NULL, MAX_LONG);
			if (len_esc != tklen)
			{
				sz_esc = xsalloc(len_esc + 1);
				csv_char_decode(token - tklen, tklen, sz_esc, len_esc);

				pdg[i].bin = NULL;
				pdg[i].len = (len_esc + 1) * sizeof(tchar_t);
				pdg[i].buf = (byte_t*)sz_esc;

				if (tklen)
					pdg[i].ind = (len_esc + 1) * sizeof(tchar_t);
				else
					pdg[i].ind = -1;
			}
			else
			{
				pdg[i].bin = NULL;
				pdg[i].len = (tklen + 1) * sizeof(tchar_t);
				pdg[i].buf = (byte_t*)xmem_alloc((tklen + 1) * sizeof(tchar_t));
				xmem_copy((void*)(pdg[i].buf), (void*)(token - tklen), tklen * sizeof(tchar_t));

				if (tklen)
					pdg[i].ind = (tklen + 1) * sizeof(tchar_t);
				else
					pdg[i].ind = -1;
			}

			if (*token == TXT_ITEMFEED)
				token++;

			if (++i == cols)
				break;
		}

		while (i < cols)
		{
			tklen = 0;
			pdg[i].bin = NULL;
			pdg[i].len = (tklen + 1) * sizeof(tchar_t);
			pdg[i].buf = (byte_t*)xmem_alloc((tklen + 1) * sizeof(tchar_t));
			pdg[i].ind = -1;

			if (++i == cols)
				break;
		}

		for (i = 0; i < cols; i++)
		{
			rt = OCIBindByPos(stm, &(pdg[i].bin), pdb->err, i + 1, (ub1*)(pdg[i].buf), (pdg[i].len), SQLT_STR, &(pdg[i].ind), NULL, NULL, 0, NULL, (ub4)OCI_DEFAULT);
			if (rt == OCI_ERROR)
			{
				_raise_oci_error(pdb->err);
			}
		}

		rt = OCIStmtExecute(pdb->ctx, stm, pdb->err, 1, (ub4)0, NULL, NULL, (ub4)OCI_DEFAULT);
		if (rt == OCI_ERROR)
		{
			_raise_oci_error(pdb->err);
		}

		for (i = 0; i < cols; i++)
		{
			xmem_free(pdg[i].buf);
			pdg[i].buf = NULL;
			pdg[i].len = 0;
		}

		ne = 0;
		OCIAttrGet(stm, OCI_HTYPE_STMT, &ne, 0, OCI_ATTR_ROW_COUNT, pdb->err);

		rows += ne;
	}

	_db_commit(pdb);

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	for (i = 0; i < cols; i++)
	{
		if (pdg[i].buf)
			xmem_free(pdg[i].buf);
	}
	xmem_free(pdg);
	pdg = NULL;

	string_free(vs);
	vs = NULL;

	pdb->rows = rows;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
	{
		_db_rollback(pdb);

		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
		stm = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	if (vs_sql)
		string_free(vs_sql);

	if (vs)
		string_free(vs);

	if (pdg)
	{
		for (i = 0; i < cols; i++)
		{
			if (pdg[i].buf)
				xmem_free(pdg[i].buf);
		}
		xmem_free(pdg);
	}

	return 0;
}

bool_t STDCALL db_batch(xdb_t db, stream_t stream)
{
	db_t* pdb = (db_t*)db;

	OCIStmt *stm = NULL;
	ub4 ne;
	sword rt = OCI_SUCCESS;

	bindguid_t* pdg = NULL;

	string_t vs = NULL;
	string_t vs_sql = NULL;
	const tchar_t *tkcur, *tkpre;
	int tklen;
	dword_t dw;
	bool_t b_uni;

	XDL_ASSERT(pdb != NULL);

	XDL_ASSERT(stream != NULL);

	TRY_CATCH;

	_db_reset(pdb);

	if (OCI_SUCCESS != OCIHandleAlloc((void *)pdb->env, (void **)&stm, OCI_HTYPE_STMT, 0, 0))
	{
		raise_user_error(_T("-1"), _T("Alloc stm handle failed"));
	}

	stream_read_utfbom(stream, NULL);

	vs = string_alloc();
	vs_sql = string_alloc();

	while (1)
	{
		string_empty(vs);
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			raise_user_error(_T("-1"), _T("stream read line failed"));
		}

		if (string_len(vs) == 0)
		{
			dw = 0;

			if (string_len(vs_sql))
				goto EXECUTE;
			else
				break;
		}

		tkcur = string_ptr(vs);
		tklen = string_len(vs);

		while (*tkcur == _T(' ') || *tkcur == _T('\t') || *tkcur == _T('\r') || *tkcur == _T('\n'))
		{
			tkcur++;
			tklen--;
		}

		if (*tkcur == _T('-') && *(tkcur + 1) == _T('-'))
		{
			continue;
		}

		tklen = split_semi(tkcur, tklen);

		tkpre = tkcur;
		tkcur += tklen;

		string_cat(vs_sql, tkpre, tklen);

		if (*tkcur != _T(';'))
		{
			continue;
		}

		string_empty(vs);

EXECUTE:
		tkpre = string_ptr(vs_sql);
		tklen = string_len(vs_sql);

		rt = OCIStmtPrepare(stm, pdb->err, (text*)tkpre, tklen * sizeof(tchar_t), OCI_NTV_SYNTAX, OCI_DEFAULT);
		if (rt == OCI_ERROR)
		{
			_raise_oci_error(pdb->err);
		}

		rt = OCIStmtExecute(pdb->ctx, stm, pdb->err, 1, (ub4)0, NULL, NULL, (ub4)OCI_DEFAULT);
		if (rt == OCI_ERROR)
		{
			_raise_oci_error(pdb->err);
		}

		string_empty(vs_sql);

		ne = 0;
		OCIAttrGet(stm, OCI_HTYPE_STMT, &ne, 0, OCI_ATTR_ROW_COUNT, pdb->err);

		if (b_uni && ne != 1)
		{
			raise_user_error(_T("-1"), ERR_TEXT_ROWCHANGED);
		}

		pdb->rows += (int)ne;

		if (!dw)
			break;
	}

	string_free(vs);
	vs = NULL;

	string_free(vs_sql);
	vs_sql = NULL;

	OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
	stm = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (vs)
		string_free(vs);

	if (vs_sql)
		string_free(vs_sql);

	if (stm)
	{
		OCIHandleFree((dvoid *)stm, OCI_HTYPE_STMT);
		stm = NULL;
	}

	get_last_error(pdb->err_code, pdb->err_text, ERR_LEN);

	return 0;
}

int STDCALL db_rows(xdb_t db)
{
	db_t* pdb = (db_t*)db;

	XDL_ASSERT(pdb != NULL);

	return pdb->rows;
}

int STDCALL db_error(xdb_t db, tchar_t* buf, int max)
{
	db_t* pdb = (db_t*)db;

	XDL_ASSERT(pdb != NULL);

	max = (max < ERR_LEN) ? max : ERR_LEN;
	if (buf)
	{
		xsncpy(buf, pdb->err_text, max);
	}

	return -1;
}
