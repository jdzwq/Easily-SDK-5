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

	bool_t share;
	tchar_t loca[PATH_LEN + 1];
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
	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	t_kb_t hdb = NULL;
	t_kv_t hkv = NULL;

	variant_t key = NULL;
	object_t val = NULL;
	dword_t len;
	byte_t* buf = NULL;
	queue_t que = NULL;
	message_t msg = NULL;
	msg_hdr_t hdr = { 0 };

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

	hdb = tkb_create(path, did, pd->share);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_get"), _T("open kv database failed"));
	}

	hkv = tkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_get"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, ZERO_NUID);
	}

	key = variant_alloc(VV_STRING);
	variant_from_string(key, pid, -1);

	val = object_alloc(_UTF8);

	tkv_read(hkv, key, val);

	que = queue_alloc();

	if (object_size(val) > 0)
	{
		if (!object_get_queue(val, que))
		{
			raise_user_error(_T("_invoke_get"), _T("get queue failed"));
		}
	}

	msg = message_alloc();
	queue_read(que, msg);

	object_set_queue(val, que);
	queue_free(que);
	que = NULL;

	object_set_commpress(val, 1);
	tkv_attach(hkv, key, val);
	val = NULL;

	variant_free(key);
	key = NULL;

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hdb);
	hdb = NULL;

	len = message_read(msg, &hdr, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(len + 1);
	message_read(msg, NULL, buf, len);

	message_free(msg);
	msg = NULL;

	if (!xcoap_send(pd->coap, buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;

	xcoap_flush(pd->coap);

	END_CATCH;

	return 1;

ONERROR:
	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		tkv_destroy(hkv);

	if (hdb)
		tkb_destroy(hdb);

	if (key)
		variant_free(key);

	if (val)
		object_free(val);

	if (que)
		queue_free(que);

	if (msg)
		message_free(msg);

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _invoke_post(const udps_block_t* pb, coap_block_t* pd)
{
	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	byte_t payload[COAP_PDV_SIZE] = { 0 };
	dword_t dw;

	t_kb_t hdb = NULL;
	t_kv_t hkv = NULL;

	variant_t key = NULL;
	object_t val = NULL;
	queue_t que = NULL;
	message_t msg = NULL;
	msg_hdr_t hdr = { 0 };

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

	hdb = tkb_create(path, did, pd->share);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_get"), _T("open kv database failed"));
	}

	hkv = tkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_get"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, ZERO_NUID);
	}

	hdr.ver = MSGVER_SENSOR;
	hdr.qos = 1;
	hdr.seq = xcoap_msgid(pd->coap);
	hdr.utc = get_timestamp();

	msg = message_alloc();
	message_write(msg, &hdr, payload, dw);

	que = queue_alloc();
	queue_write(que, msg);

	message_free(msg);
	msg = NULL;
	
	key = variant_alloc(VV_STRING);
	variant_from_string(key, pid, -1);

	val = object_alloc(_UTF8);
	object_set_queue(val, que);

	queue_free(que);
	que = NULL;

	object_set_commpress(val, 1);
	tkv_attach(hkv, key, val);
	val = NULL;

	variant_free(key);
	key = NULL;

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hdb);
	hdb = NULL;

	xcoap_abort(pd->coap, COAP_RESPONSE_200_CODE);

	END_CATCH;

	return 1;

ONERROR:

	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		tkv_destroy(hkv);

	if (hdb)
		tkb_destroy(hdb);

	if (key)
		variant_free(key);

	if (val)
		object_free(val);

	if (que)
		queue_free(que);

	if (msg)
		message_free(msg);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _invoke_put(const udps_block_t* pb, coap_block_t* pd)
{
	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	byte_t* payload = NULL;
	dword_t dw, total = 0;

	t_kb_t hdb = NULL;
	t_kv_t hkv = NULL;

	variant_t key = NULL;
	object_t val = NULL;
	queue_t que = NULL;
	message_t msg = NULL;
	msg_hdr_t hdr = { 0 };

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

	hdb = tkb_create(path, did, pd->share);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_put"), _T("open kv database failed"));
	}

	hkv = tkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_put"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, ZERO_NUID);
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

	key = variant_alloc(VV_STRING);
	variant_from_string(key, pid, -1);

	val = object_alloc(_UTF8);

	tkv_read(hkv, key, val);

	que = queue_alloc();

	if (object_size(val) > 0)
	{
		if (!object_get_queue(val, que))
		{
			raise_user_error(_T("_invoke_put"), _T("read queue falied"));
		}
	}

	hdr.ver = MSGVER_SENSOR;
	hdr.qos = 1;
	hdr.seq = xcoap_msgid(pd->coap);
	hdr.utc = get_timestamp();

	msg = message_alloc();
	message_write(msg, &hdr, payload, total);

	queue_write(que, msg);

	message_free(msg);
	msg = NULL;

	object_set_queue(val, que);
	queue_free(que);
	que = NULL;
	
	object_set_commpress(val, 1);
	tkv_attach(hkv, key, val);
	val = NULL;

	variant_free(key);
	key = NULL;

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hdb);
	hdb = NULL;

	END_CATCH;

	return 1;

ONERROR:

	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		tkv_destroy(hkv);

	if (hdb)
		tkb_destroy(hdb);

	if (key)
		variant_free(key);

	if (val)
		object_free(val);

	if (que)
		queue_free(que);

	if (msg)
		message_free(msg);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _invoke_delete(const udps_block_t* pb, coap_block_t* pd)
{
	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	t_kb_t hdb = NULL;
	t_kv_t hkv = NULL;

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

	hdb = tkb_create(path, did, pd->share);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_delete"), _T("open kv database failed"));
	}

	hkv = tkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_delete"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, ZERO_NUID);
	}

	key = variant_alloc(VV_STRING);
	variant_from_string(key, pid, -1);

	tkv_delete(hkv, key);

	variant_free(key);
	key = NULL;

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hdb);
	hdb = NULL;

	xcoap_abort(pd->coap, COAP_RESPONSE_200_CODE);

	END_CATCH;

	return 1;

ONERROR:

	xcoap_abort(pd->coap, COAP_RESPONSE_500_CODE);

	if (hkv)
		tkv_destroy(hkv);

	if (hdb)
		tkb_destroy(hdb);

	if (key)
		variant_free(key);

	XDL_TRACE_LAST;

	return 0;
}

/****************************************************************************************************/

int STDCALL udps_invoke(const udps_block_t* pb)
{
	coap_block_t* pd = NULL;

	tchar_t file[PATH_LEN + 1] = { 0 };
	tchar_t token[RES_LEN + 1] = { 0 };

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
	read_proper(ptr_prop, _T("COAP"), -1, _T("EXCLUSIVE"), -1, token, INT_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->loca, file);
	pd->share = (xstol(token)) ? 0 : 1;

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

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? UDPS_INVOKE_SUCCEED : UDPS_INVOKE_WITHINFO;

ONERROR:
	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
	{
		if (pd->coap)
			xcoap_close(pd->coap);

		xmem_free(pd);
	}

	XDL_TRACE_LAST;

	return UDPS_INVOKE_WITHINFO;
}