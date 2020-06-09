/***********************************************************************
	Easily coap web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc coap web api document

	@module	coap_api.cc | coap web api implement file

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

#include "coap_api.h"

typedef struct _coap_block_t{
	xhand_t coap;

	tchar_t loca[PATH_LEN];

	tchar_t code[NUM_LEN + 1];
	tchar_t text[ERR_LEN + 1];
}coap_block_t;

/*********************************************************************************/

static void split_topic(const tchar_t* topic, tchar_t* cid, tchar_t* did, tchar_t* pid)
{
	const tchar_t* token = topic;
	int len;

	len = 0;
	while (*token != _T('/') && *token != _T('\\') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	xsncpy(cid, token - len, len);

	if (*token != _T('\0'))
		token++;

	len = 0;
	while (*token != _T('/') && *token != _T('\\') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	xsncpy(did, token - len, len);

	if (*token != _T('\0'))
		token++;

	len = 0;
	while (*token != _T('/') && *token != _T('\\') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	xsncpy(pid, token - len, len);
}

static bool_t _invoke_get(const udps_block_t* pb, coap_block_t* pd)
{
	hex_obj_t hdb = NULL;
	hex_obj_t hkv = NULL;

	tchar_t path[PATH_LEN] = { 0 };
	tchar_t cid[UUID_LEN] = { 0 };
	tchar_t did[UUID_LEN] = { 0 };
	tchar_t pid[UUID_LEN] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;
	dword_t len;
	byte_t* buf = NULL;
	rad_hdr_t hdr = { 0 };

	TRY_CATCH;

	if (!xcoap_get_object(pd->coap, path, 1024))
	{
		raise_user_error(_T("_invoke_get"), _T("empty object"));
	}

	split_topic(path + 1, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_get"), _T("unknown kv database"));
	}
	
	xsprintf(path, _T("%s/%s"), pd->loca, cid);

	hdb = hexdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_get"), _T("open kv database failed"));
	}

	hkv = hexkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_get"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, MSG_CONFIG);
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	hexkv_destroy(hkv);
	hkv = NULL;

	hexdb_destroy(hdb);
	hdb = NULL;

	len = radobj_read(val, &hdr, NULL, MAX_LONG);
	
	buf = (byte_t*)xmem_alloc(len);

	radobj_read(val, &hdr, buf, len);

	variant_to_null(&key);

	object_free(val);
	val = NULL;

	if (!xcoap_send(pd->coap, buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;

	xcoap_flush(pd->coap);

	xscpy(pd->code, _T("_invoke_get"));
	xscpy(pd->text, _T("Succeeded"));

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(pd->code, pd->text, ERR_LEN);

	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		hexkv_destroy(hkv);

	if (hdb)
		hexdb_destroy(hdb);

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	return 0;
}

static bool_t _invoke_post(const udps_block_t* pb, coap_block_t* pd)
{
	hex_obj_t hdb = NULL;
	hex_obj_t hkv = NULL;

	tchar_t path[PATH_LEN] = { 0 };
	tchar_t cid[UUID_LEN] = { 0 };
	tchar_t did[UUID_LEN] = { 0 };
	tchar_t pid[UUID_LEN] = { 0 };

	rad_hdr_t hdr = { 0 };
	xdate_t dt = { 0 };
	byte_t payload[COAP_PDV_SIZE] = { 0 };
	dword_t dw;

	variant_t key = { 0 };
	object_t val = NULL;

	TRY_CATCH;

	if (!xcoap_get_object(pd->coap, path, 1024))
	{
		raise_user_error(_T("_invoke_post"), _T("empty object"));
	}

	dw = COAP_PDV_SIZE;
	if (!xcoap_recv(pd->coap, payload, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	split_topic(path + 1, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_get"), _T("unknown kv database"));
	}

	xsprintf(path, _T("%s/%s"), pd->loca, cid);

	hdb = hexdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_get"), _T("open kv database failed"));
	}

	hkv = hexkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_get"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, MSG_CONFIG);
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	hdr.mid = xcoap_msgid(pd->coap);
	xmem_copy((void*)hdr.ver, (void*)MSGVER_SENSOR, MSGVER_SIZE);
	get_utc_date(&dt);
	format_utctime(&dt, hdr.utc);

	radobj_write(val, &hdr, payload, dw);

	hexkv_attach(hkv, key, val);
	val = NULL;

	variant_to_null(&key);

	hexkv_destroy(hkv);
	hkv = NULL;

	hexdb_destroy(hdb);
	hdb = NULL;

	xcoap_abort(pd->coap, COAP_RESPONSE_200_CODE);

	xscpy(pd->code, _T("_invoke_post"));
	xscpy(pd->text, _T("Succeeded"));

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(pd->code, pd->text, ERR_LEN);

	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		hexkv_destroy(hkv);

	if (hdb)
		hexdb_destroy(hdb);

	variant_to_null(&key);

	if (val)
		object_free(val);

	return 0;
}

static bool_t _invoke_put(const udps_block_t* pb, coap_block_t* pd)
{
	hex_obj_t hdb = NULL;
	hex_obj_t hkv = NULL;

	tchar_t path[PATH_LEN] = { 0 };
	tchar_t cid[UUID_LEN] = { 0 };
	tchar_t did[UUID_LEN] = { 0 };
	tchar_t pid[UUID_LEN] = { 0 };

	xdate_t dt;
	rad_hdr_t hdr = { 0 };
	byte_t* payload = NULL;
	dword_t dw, total = 0;

	variant_t key = { 0 };
	object_t val = NULL;

	TRY_CATCH;

	if (!xcoap_get_object(pd->coap, path, 1024))
	{
		raise_user_error(_T("_invoke_put"), _T("empty object"));
	}

	split_topic(path + 1, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_put"), _T("unknown kv database"));
	}

	xsprintf(path, _T("%s/%s"), pd->loca, cid);

	hdb = hexdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_put"), _T("open kv database failed"));
	}

	hkv = hexkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_put"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, MSG_CONFIG);
	}

	total = 0;
	payload = (byte_t*)xmem_alloc(COAP_PDV_SIZE);

	while (1)
	{
		dw = COAP_PDV_SIZE;
		if (!xcoap_recv(pd->coap, payload + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}

		if (!dw)
			break;

		total += dw;
		payload = (byte_t*)xmem_realloc(payload, total + COAP_PDV_SIZE);
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	hdr.mid = xcoap_msgid(pd->coap);
	xmem_copy((void*)hdr.ver, (void*)MSGVER_SENSOR, MSGVER_SIZE);
	get_utc_date(&dt);
	format_utctime(&dt, hdr.utc);

	radobj_write(val, &hdr, payload, total);

	hexkv_attach(hkv, key, val);
	val = NULL;

	variant_to_null(&key);

	hexkv_destroy(hkv);
	hkv = NULL;

	hexdb_destroy(hdb);
	hdb = NULL;

	xscpy(pd->code, _T("_invoke_put"));
	xscpy(pd->text, _T("Succeeded"));

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(pd->code, pd->text, ERR_LEN);

	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		hexkv_destroy(hkv);

	if (hdb)
		hexdb_destroy(hdb);

	variant_to_null(&key);

	if (val)
		object_free(val);

	return 0;
}

static bool_t _invoke_delete(const udps_block_t* pb, coap_block_t* pd)
{
	hex_obj_t hdb = NULL;
	hex_obj_t hkv = NULL;

	tchar_t path[PATH_LEN] = { 0 };
	tchar_t cid[UUID_LEN] = { 0 };
	tchar_t did[UUID_LEN] = { 0 };
	tchar_t pid[UUID_LEN] = { 0 };

	variant_t key = { 0 };

	TRY_CATCH;

	if (!xcoap_get_object(pd->coap, path, 1024))
	{
		raise_user_error(_T("_invoke_delete"), _T("empty object"));
	}

	split_topic(path + 1, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_delete"), _T("unknown kv database"));
	}

	xsprintf(path, _T("%s/%s"), pd->loca, cid);

	hdb = hexdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_delete"), _T("open kv database failed"));
	}

	hkv = hexkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_delete"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, MSG_CONFIG);
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	hexkv_delete(hkv, key);

	variant_to_null(&key);

	hexkv_destroy(hkv);
	hkv = NULL;

	hexdb_destroy(hdb);
	hdb = NULL;

	xcoap_abort(pd->coap, COAP_RESPONSE_200_CODE);

	xscpy(pd->code, _T("_invoke_delete"));
	xscpy(pd->text, _T("Succeeded"));

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(pd->code, pd->text, ERR_LEN);

	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		hexkv_destroy(hkv);

	if (hdb)
		hexdb_destroy(hdb);

	variant_to_null(&key);

	return 0;
}

/****************************************************************************************************/

int STDCALL udps_invoke(const udps_block_t* pb)
{
	coap_block_t* pd = NULL;

	tchar_t file[PATH_LEN] = { 0 };
	tchar_t token[RES_LEN] = { 0 };

	link_t_ptr ptr_prop = NULL;
	bool_t rt = 1;

	TRY_CATCH;

	pd = (coap_block_t*)xmem_alloc(sizeof(coap_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/coap.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load coap config falied\n"));
	}

	read_proper(ptr_prop, _T("COAP"), -1, _T("LOCATION"), -1, file, PATH_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->loca, file);

	pd->coap = xcoap_server(pb->port, pb->addr, pb->pack, pb->size);
	if (!pd->coap)
	{
		raise_user_error(_T("coap_api"), _T("create coap service failed"));
	}

	if (pb->timo)
	{
		xudp_settmo(xcoap_bio(pd->coap), pb->timo);
	}

	xcoap_set_blockwise(pd->coap, 6);

	if (!xcoap_accept(pd->coap))
	{
		raise_user_error(NULL, NULL);
	}

	xcoap_method(pd->coap, token, INT_LEN);

	if (compare_text(token, -1, _T("GET"), -1, 1) == 0)
		rt = _invoke_get(pb, pd);
	else if(compare_text(token, -1, _T("PUT"), -1, 1) == 0)
		rt = _invoke_put(pb, pd);
	else if (compare_text(token, -1, _T("POST"), -1, 1) == 0)
		rt = _invoke_post(pb, pd);
	else if (compare_text(token, -1, _T("DELETE"), -1, 1) == 0)
		rt = _invoke_delete(pb, pd);
	else
	{
		raise_user_error(_T("coap_api"), _T("unknown coap method"));
	}

	xcoap_close(pd->coap);
	pd->coap = NULL;

	if (pb->pf_track_eror)
	{
		(*pb->pf_track_eror)(pb->hand, pd->code, pd->text);
	}

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? UDPS_INVOKE_SUCCEED : UDPS_INVOKE_WITHINFO;

ONERROR:
	XDL_TRACE_LAST;

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
	{
		if (pb->pf_track_eror)
		{
			(*pb->pf_track_eror)(pb->hand, pd->code, pd->text);
		}

		if (pd->coap)
			xcoap_close(pd->coap);

		xmem_free(pd);
	}

	return UDPS_INVOKE_WITHINFO;
}