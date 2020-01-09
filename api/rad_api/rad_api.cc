/***********************************************************************
	Easily radhex web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc radhex web api document

	@module	rad_api.cc | radhex web api implement file
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

#include "rad_api.h"

typedef struct _radhex_block_t{
	radhex_t* radhex;
	dword_t request;
	dword_t response;

	hex_obj_t hkv;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}radhex_block_t;

void _invoke_error(const slots_block_t* pb, const radhex_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[ERROR]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

void _invoke_insert(const slots_block_t* pb, const radhex_block_t* pd)
{
	byte_t* buf = NULL;
	dword_t n, dw;

	variant_t var = { 0 };
	object_t val = NULL;

	stream_t stm = NULL;

	XDL_ASSERT(pd && pd->radhex);

	TRY_CATCH;

	stm = stream_alloc(pd->radhex->bio);

	dw = pd->request;
	buf = (byte_t*)xmem_alloc(dw);

	if (!stream_read_bytes(stm, buf, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	n = variant_decode(&var, buf);
	if (!n)
	{
		raise_user_error(_T("_invoke_insert"), _T("invalid key"));
	}

	val = object_alloc(DEF_MBS);

	n = object_decode(val, buf + n);
	if (!n)
	{
		raise_user_error(_T("_invoke_insert"), _T("invalid object"));
	}

	xmem_free(buf);
	buf = NULL;

	hexkv_attach(pd->hkv, var, val);
	val = NULL;

	variant_to_null(&var);

	radhex_abort(pd->radhex, RADHEX_ERROR_SUCCEED);

	END_CATCH;

	return;

ONERROR:

	XDL_TRACE_LAST;

	radhex_abort(pd->radhex, RADHEX_ERROR_INSERT);

	variant_to_null(&var);

	if (val)
		object_free(val);

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);
	
	return;
}

void _invoke_update(const slots_block_t* pb, const radhex_block_t* pd)
{
	byte_t* buf = NULL;
	dword_t n, dw;

	variant_t var = { 0 };
	object_t val = NULL;

	stream_t stm = NULL;

	XDL_ASSERT(pd && pd->radhex);

	TRY_CATCH;

	stm = stream_alloc(pd->radhex->bio);

	dw = pd->request;
	buf = (byte_t*)xmem_alloc(dw);

	if (!stream_read_bytes(stm, buf, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	n = variant_decode(&var, buf);
	if (!n)
	{
		raise_user_error(_T("_invoke_update"), _T("invalid key"));
	}

	val = object_alloc(DEF_MBS);

	n = object_decode(val, buf + n);
	if (!n)
	{
		raise_user_error(_T("_invoke_update"), _T("invalid object"));
	}

	xmem_free(buf);
	buf = NULL;

	if (!hexkv_update(pd->hkv, var, val))
	{
		raise_user_error(_T("_invoke_update"), _T("update failed"));
	}

	object_free(val);
	val = NULL;

	variant_to_null(&var);

	radhex_abort(pd->radhex, RADHEX_ERROR_SUCCEED);

	END_CATCH;

	return;

ONERROR:

	XDL_TRACE_LAST;

	radhex_abort(pd->radhex, RADHEX_ERROR_UPDATE);

	variant_to_null(&var);

	if (val)
		object_free(val);

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	return;
}

void _invoke_delete(const slots_block_t* pb, const radhex_block_t* pd)
{
	byte_t* buf = NULL;
	dword_t n, dw;

	variant_t var = { 0 };

	stream_t stm = NULL;

	XDL_ASSERT(pd && pd->radhex);

	TRY_CATCH;

	stm = stream_alloc(pd->radhex->bio);

	dw = pd->request;
	buf = (byte_t*)xmem_alloc(dw);

	if (!stream_read_bytes(stm, buf, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	n = variant_decode(&var, buf);
	if (!n)
	{
		raise_user_error(_T("_invoke_delete"), _T("invalid key"));
	}

	xmem_free(buf);
	buf = NULL;

	stream_free(stm);
	stm = NULL;

	hexkv_delete(pd->hkv, var);

	variant_to_null(&var);

	radhex_abort(pd->radhex, RADHEX_ERROR_SUCCEED);

	END_CATCH;

	return;

ONERROR:

	XDL_TRACE_LAST;

	radhex_abort(pd->radhex, RADHEX_ERROR_DELETE);

	variant_to_null(&var);

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	return;
}

void _invoke_trunca(const slots_block_t* pb, const radhex_block_t* pd)
{
	byte_t* buf = NULL;
	dword_t n, dw;

	variant_t var = { 0 };

	stream_t stm = NULL;

	XDL_ASSERT(pd && pd->radhex);

	TRY_CATCH;

	stm = stream_alloc(pd->radhex->bio);

	dw = pd->request;
	buf = (byte_t*)xmem_alloc(dw);

	if (!stream_read_bytes(stm, buf, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	n = variant_decode(&var, buf);
	if (!n)
	{
		raise_user_error(_T("_invoke_trunca"), _T("invalid key"));
	}

	xmem_free(buf);
	buf = NULL;

	hexkv_write(pd->hkv, var, NULL);

	variant_to_null(&var);

	radhex_abort(pd->radhex, RADHEX_ERROR_SUCCEED);

	END_CATCH;

	return;

ONERROR:

	XDL_TRACE_LAST;

	radhex_abort(pd->radhex, RADHEX_ERROR_TRUNCA);

	variant_to_null(&var);

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	return;
}

void _invoke_select(const slots_block_t* pb, const radhex_block_t* pd)
{
	radhex_pdv_head_t pdv = { 0 };
	byte_t* buf = NULL;
	dword_t n, dw;

	variant_t var = { 0 };
	object_t val = NULL;

	stream_t stm = NULL;
	bool_t snd = 0;

	XDL_ASSERT(pd && pd->radhex);

	TRY_CATCH;

	stm = stream_alloc(pd->radhex->bio);

	dw = pd->request;
	buf = (byte_t*)xmem_alloc(dw);

	if (!stream_read_bytes(stm, buf, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	n = variant_decode(&var, buf);
	if (!n)
	{
		raise_user_error(_T("_invoke_insert"), _T("invalid key"));
	}

	xmem_free(buf);
	buf = NULL;

	val = object_alloc(DEF_MBS);

	hexkv_read(pd->hkv, var, val);

	dw = variant_encode(&var, NULL, MAX_LONG) + object_encode(val, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(dw);

	n = variant_encode(&var, buf, MAX_LONG);
	n = object_encode(val, buf + n, MAX_LONG);

	variant_to_null(&var);

	object_free(val);
	val = NULL;

	pdv.type = 0;
	pdv.size = dw;

	snd = 1;

	if (!radhex_send(pd->radhex, &pdv))
	{
		raise_user_error(NULL, NULL);
	}

	if (!stream_write_bytes(stm, buf, dw))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return;

ONERROR:

	XDL_TRACE_LAST;

	if (!snd)
		radhex_abort(pd->radhex, RADHEX_ERROR_SELECT);

	variant_to_null(&var);

	if (val)
		object_free(val);

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	return;
}

int STDCALL slots_invoke(slots_block_t* pb)
{
	radhex_block_t* pd = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t token[PATH_LEN] = { 0 };

	hex_obj_t hdb = NULL;

	radhex_pdv_head_t pdv = { 0 };

	bool_t rt = 1;

	TRY_CATCH;

	pd = (radhex_block_t*)xmem_alloc(sizeof(radhex_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(token, _T("%s/rad.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, token))
	{
		raise_user_error(NULL, NULL);
	}

	read_proper(ptr_prop, _T("RAD"), -1, _T("LOCATION"), -1, token, PATH_LEN);
	read_proper(ptr_prop, _T("RAD"), -1, _T("PUBLICKEY"), -1, pd->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("RAD"), -1, _T("PRIVATEKEY"), -1, pd->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->local, token);

	pd->radhex = radhex_scp(pb->slot);
	if (!pd->radhex)
	{
		raise_user_error(NULL, NULL);
	}

	if (!radhex_accept(pd->radhex))
	{
		raise_user_error(NULL, NULL);
	}

	hdb = hexdb_create(pd->local, pd->radhex->dbname);
	if (!hdb)
	{
		radhex_abort(pd->radhex, RADHEX_ERROR_OPEN);

		raise_user_error(_T("-1"), _T("open database failed"));
	}

	pd->hkv = hexkv_create(hdb);

	while (radhex_status(pd->radhex) != _RADHEX_STATUS_RELEASE)
	{
		if (!radhex_recv(pd->radhex, &pdv))
		{
			raise_user_error(NULL, NULL);
		}

		if (!pdv.size)
			continue;
		
		pd->request = pdv.size;

		switch (pdv.type)
		{
		case RADHEX_COMMAND_INSERT:
			_invoke_insert(pb, pd);
			break;
		case RADHEX_COMMAND_UPDATE:
			_invoke_update(pb, pd);
			break;
		case RADHEX_COMMAND_DELETE:
			_invoke_delete(pb, pd);
			break;
		case RADHEX_COMMAND_TRUNCA:
			_invoke_trunca(pb, pd);
			break;
		case RADHEX_COMMAND_SELECT:
			_invoke_select(pb, pd);
			break;
		}
	}

	hexkv_flush(pd->hkv);

	hexkv_destroy(pd->hkv);
	pd->hkv = NULL;

	hexdb_destroy(hdb);
	hdb = NULL;

	radhex_close(pd->radhex);
	pd->radhex = NULL;

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? SLOTS_INVOKE_SUCCEED : SLOTS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pd);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
	{
		if (pd->radhex)
			radhex_close(pd->radhex);

		if (pd->hkv)
			hexkv_destroy(pd->hkv);

		xmem_free(pd);
	}

	if (hdb)
		hexdb_destroy(hdb);

	return SLOTS_INVOKE_WITHINFO;
}

