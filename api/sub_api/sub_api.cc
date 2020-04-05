/***********************************************************************
	Easily mqtt web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc mqtt subcribe api document

	@module	sub_api.cc | mqtt subcribe api implement file
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

#include "sub_api.h"

typedef struct _mqtt_block_t{
	xhand_t mqtt;

	tchar_t topic_name[PATH_LEN];

	hex_obj_t hdb;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];

	tchar_t code[NUM_LEN + 1];
	tchar_t text[ERR_LEN + 1];
}mqtt_block_t;

void _invoke_subcribe(const slots_block_t* pb, mqtt_block_t* pd)
{
	variant_t key = { 0 };
	object_t val = NULL;
	int encode;
	byte_t* buf = NULL;
	dword_t len;
	dword_t total = 0;

	byte_t* han_buf;
	sword_t han_len;
	byte_t* hdr_buf;
	sword_t hdr_len;
	byte_t* msg_buf;
	dword_t msg_len;
	byte_t* sub_buf;
	dword_t sub_len;

	hex_obj_t hkv = NULL;

	byte_t ver[5] = { 0 };
	MQTT_PACKET_CTRL mc = { 0 };

	TRY_CATCH;

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_subcribe"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	encode = object_get_encode(val);

	len = object_get_bytes(val, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(len);

	object_get_bytes(val, buf, len);

	while (total < len)
	{
		//the message total size
		msg_buf = buf + total + 4;
		msg_len = GET_DWORD_NET((msg_buf - 4), 0);
		if (msg_len > MAX_LONG)
		{
			raise_user_error(_T("_invoke_subcribe"), _T("invalid message total size"));
		}

		xmem_set((void*)&mc, 0, sizeof(MQTT_PACKET_CTRL));

		//the message handler
		han_buf = msg_buf + 2;
		han_len = GET_SWORD_NET((han_buf - 2), 0);
		if (han_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_subcribe"), _T("invalid message handler size"));
		}
		xmem_copy((void*)ver, han_buf, 4);
		if (ver[0] == 'M' && ver[1] == 'Q')
		{
			mc.packet_qos = (byte_t)GET_SWORD_NET(han_buf, 4);
			mc.packet_pid = GET_SWORD_NET(han_buf, 6);
		}

		//the message header
		hdr_buf = han_buf + han_len + 2;
		hdr_len = GET_SWORD_NET((hdr_buf - 2), 0);
		if (hdr_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_subcribe"), _T("invalid message header size"));
		}

		//the message element
		sub_buf = hdr_buf + hdr_len + 4;
		sub_len = GET_DWORD_NET((sub_buf - 4), 0);
		if (sub_len > MAX_LONG)
		{
			raise_user_error(_T("_invoke_subcribe"), _T("invalid message element size"));
		}

		xmqtt_set_packet_ctrl(pd->mqtt, &mc);

		if (!xmqtt_push_message(pd->mqtt, sub_buf, sub_len))
		{
			raise_user_error(NULL, NULL);
		}

		total += (4 + msg_len);
	}

	xmem_free(buf);
	buf = NULL;

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	hexkv_destroy(hkv);
	hkv = NULL;

	xscpy(pd->code, _T("_invoke_subcribe"));
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

	return;
}

void _invoke_unsubcribe(const slots_block_t* pb, mqtt_block_t* pd)
{
	variant_t key = { 0 };

	hex_obj_t hkv = NULL;

	TRY_CATCH;

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_unsubcribe"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	hexkv_delete(hkv, key);

	variant_to_null(&key);

	hexkv_destroy(hkv);
	hkv = NULL;

	xscpy(pd->code, _T("_invoke_unsubcribe"));
	xscpy(pd->text, _T("Succeeded"));

	END_CATCH;

	return;
ONERROR:

	get_last_error(pd->code, pd->text, ERR_LEN);

	variant_to_null(&key);

	if (hkv)
		hexkv_destroy(hkv);

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

	xsprintf(token, _T("%s/sub.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, token))
	{
		raise_user_error(NULL, NULL);
	}

	xszero(file, PATH_LEN);

	read_proper(ptr_prop, _T("SUB"), -1, _T("DATABASE"), -1, token, RES_LEN);
	read_proper(ptr_prop, _T("SUB"), -1, _T("LOCATION"), -1, file, PATH_LEN);
	read_proper(ptr_prop, _T("SUB"), -1, _T("PUBLICKEY"), -1, pd->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("SUB"), -1, _T("PRIVATEKEY"), -1, pd->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->local, file);

	pd->hdb = hexdb_create(pd->local, token);
	if (!pd->hdb)
	{
		raise_user_error(_T("-1"), _T("open database failed"));
	}

	pd->mqtt = xmqtt_scp(pb->slot, _MQTT_TYPE_SCP_SUB);
	if (!pd->mqtt)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xmqtt_accept(pd->mqtt))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xmqtt_poll_subscribe(pd->mqtt, pd->topic_name, PATH_LEN))
	{
		raise_user_error(NULL, NULL);
	}

	xmqtt_get_packet_ctrl(pd->mqtt, &mc);
	if (mc.packet_qos)
		_invoke_subcribe(pb, pd);
	else
		_invoke_unsubcribe(pb, pd);
	
	xmqtt_close(pd->mqtt);
	pd->mqtt = NULL;

	hexdb_destroy(pd->hdb);
	pd->hdb = NULL;

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

		if (pd->hdb)
			hexdb_destroy(pd->hdb);

		xmem_free(pd);
	}

	return SLOTS_INVOKE_WITHINFO;
}

