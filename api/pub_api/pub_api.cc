﻿/***********************************************************************
	Easily mqtt web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc mqtt publish api document

	@module	pub_api.cc | mqtt publish api implement file
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

#include "pub_api.h"

typedef struct _mqtt_block_t{
	xhand_t mqtt;

	tchar_t topic_name[PATH_LEN];

	byte_t msg_qos;
	sword_t msg_pid;
	dword_t msg_len;
	byte_t* msg_buf;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];

	tchar_t code[NUM_LEN + 1];
	tchar_t text[ERR_LEN + 1];
}mqtt_block_t;

static void split_topic(const tchar_t* topic, tchar_t* cid, tchar_t* did, tchar_t* pid)
{
	const tchar_t* token = topic;
	int len ;

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

void _invoke_publish(const slots_block_t* pb, mqtt_block_t* pd)
{
	variant_t key = { 0 };
	object_t val = NULL;

	sword_t sw = 0;

	byte_t* buf = NULL;
	dword_t total = 0;
	dword_t obj_len;
	byte_t* pub_buf;
	dword_t pub_len;
	byte_t* han_buf;
	sword_t han_len;
	byte_t* msg_buf;
	dword_t msg_len;

	hex_obj_t hdb = NULL;
	hex_obj_t hkv = NULL;

	tchar_t path[PATH_LEN] = { 0 };
	tchar_t cid[UUID_LEN] = { 0 };
	tchar_t did[UUID_LEN] = { 0 };
	tchar_t pid[UUID_LEN] = { 0 };

	xdate_t dt;
	tchar_t sz_date[UTC_LEN + 1] = { 0 };

	TRY_CATCH;

	split_topic(pd->topic_name, cid, did, pid);
	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_publish"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hdb = hexdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_publish"), _T("open kv database failed"));
	}

	hkv = hexkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_publish"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, PUB_TOPIC_CONFIG);
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	obj_len = object_get_bytes(val, NULL, MAX_LONG);

	sw = 0;
	sw |= pd->msg_qos;

	get_utc_date(&dt);
	format_utctime(&dt, sz_date);

	/* the PDU:
	struct object_list{
		origin object_list entities; obj_len bytes
		new object entity size; 4 bytes
		new object hander size; 2 bytes
		new object hander data; 8 bytes
		new object message size; 4 bytes
		new object message data; msg_len bytes
	}
	*/
	total = 0;
	buf = (byte_t*)xmem_alloc(obj_len + 4 + (2 + PUBHAN_SIZE) + (4 + pd->msg_len));
	//copy origin object_list
	object_get_bytes(val, buf, obj_len);
	total += obj_len;

	//the object total size
	pub_buf = buf + obj_len + 4;
	pub_len = ((2 + PUBHAN_SIZE) + (4 + pd->msg_len));
	PUT_DWORD_NET((pub_buf - 4), 0, pub_len);
	total += 4;

	//the object handler size
	han_buf = pub_buf + 2;
	han_len = PUBHAN_SIZE;
	PUT_SWORD_NET((han_buf - 2), 0, han_len);
	total += 2;
	//the object handler
	xmem_copy((void*)(han_buf), (void*)PUBVER, PUBVER_SIZE);
	PUT_SWORD_NET(han_buf, 4, sw);
	PUT_SWORD_NET(han_buf, 6, pd->msg_pid);
#if defined(_UNICODE) || defined(UNICODE)
	ucs_to_utf8(sz_date, UTC_LEN, (han_buf + 8), UTC_LEN);
#else
	mbs_to_utf8(sz_date, UTC_LEN, (han_buf + 8), UTC_LEN);
#endif
	total += han_len;

	//the object message size
	msg_buf = han_buf + PUBHAN_SIZE + 4;
	msg_len = pd->msg_len;
	PUT_DWORD_NET((msg_buf - 4), 0, msg_len);
	total += 4;
	//the object message
	xmem_copy((void*)(msg_buf), pd->msg_buf, pd->msg_len);
	total += pd->msg_len;

	//set new object list
	object_set_bytes(val, _UTF8, buf, total);

	xmem_free(buf);
	buf = NULL;

	hexkv_attach(hkv, key, val);
	val = NULL;

	variant_to_null(&key);

	hexkv_destroy(hkv);
	hkv = NULL;

	hexdb_destroy(hdb);
	hdb = NULL;

	xscpy(pd->code, _T("_invoke_publish"));
	xscpy(pd->text, _T("Succeeded"));

	END_CATCH;

	return;
ONERROR:

	get_last_error(pd->code, pd->text, ERR_LEN);

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		hexkv_destroy(hkv);

	if (hdb)
		hexdb_destroy(hdb);

	return;
}

int STDCALL slots_invoke(const slots_block_t* pb)
{
	mqtt_block_t* pd = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t file[PATH_LEN] = { 0 };
	tchar_t token[RES_LEN] = { 0 };

	MQTT_PACKET_CTRL mc = { 0 };

	TRY_CATCH;

	pd = (mqtt_block_t*)xmem_alloc(sizeof(mqtt_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/pub.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(NULL, NULL);
	}

	xszero(file, PATH_LEN);

	read_proper(ptr_prop, _T("PUB"), -1, _T("DATABASE"), -1, token, RES_LEN);
	read_proper(ptr_prop, _T("PUB"), -1, _T("LOCATION"), -1, file, PATH_LEN);
	read_proper(ptr_prop, _T("PUB"), -1, _T("PUBLICKEY"), -1, pd->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("PUB"), -1, _T("PRIVATEKEY"), -1, pd->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->local, file);

	pd->mqtt = xmqtt_scp(pb->slot, _MQTT_TYPE_SCP_PUB);
	if (!pd->mqtt)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xmqtt_accept(pd->mqtt))
	{
		raise_user_error(NULL, NULL);
	}

	while (xmqtt_status(pd->mqtt) != _MQTT_STATUS_RELEASE)
	{
		pd->msg_buf = NULL;
		pd->msg_len = 0;

		if (!xmqtt_poll_message(pd->mqtt, &pd->msg_buf, &pd->msg_len))
		{
			raise_user_error(NULL, NULL);
		}

		if (!pd->msg_len)
			continue;
		
		xmqtt_poll_subscribe(pd->mqtt, pd->topic_name, PATH_LEN);

		xmem_set((void*)&mc, 0, sizeof(MQTT_PACKET_CTRL));
		xmqtt_get_packet_ctrl(pd->mqtt, &mc);
		pd->msg_qos = mc.packet_qos;
		pd->msg_pid = mc.packet_pid;

		_invoke_publish(pb, pd);
	}

	xmqtt_close(pd->mqtt);
	pd->mqtt = NULL;

	if (pb->pf_track_eror)
	{
		(*pb->pf_track_eror)(pb->hand, pd->code, pd->text);
	}

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return SLOTS_INVOKE_SUCCEED;

ONERROR:

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
	{
		get_last_error(pd->code, pd->text, ERR_LEN);

		if (pb->pf_track_eror)
		{
			(*pb->pf_track_eror)(pb->hand, pd->code, pd->text);
		}

		if (pd->mqtt)
			xmqtt_close(pd->mqtt);

		xmem_free(pd);
	}

	return SLOTS_INVOKE_WITHINFO;
}

