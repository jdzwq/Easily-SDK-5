/***********************************************************************
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
	mqtt_t* mqtt;

	tchar_t topic_name[PATH_LEN];

	byte_t msg_qos;
	sword_t msg_pid;
	dword_t msg_len;
	byte_t* msg_buf;

	hex_obj_t hdb;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];

	tchar_t code[NUM_LEN + 1];
	tchar_t text[ERR_LEN + 1];
}mqtt_block_t;

void _invoke_publish(const slots_block_t* pb, mqtt_block_t* pd)
{
	variant_t key = { 0 };
	object_t val = NULL;

	byte_t* buf = NULL;
	dword_t len;
	sword_t sw = 0;

	hex_obj_t hkv = NULL;

	TRY_CATCH;

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_publish"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	len = object_get_bytes(val, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(len + 8 + pd->msg_len);
	object_get_bytes(val, buf, len);

	sw = 0;
	sw |= pd->msg_qos;
	PUT_DWORD_LOC(buf, len, pd->msg_len + 4);
	PUT_SWORD_LOC(buf, len + 4, sw);
	PUT_SWORD_LOC(buf, len + 6, pd->msg_pid);
	xmem_copy((void*)(buf + len + 8), pd->msg_buf, pd->msg_len);
	len += (8 + pd->msg_len);

	object_set_bytes(val, _UTF8, buf, len);

	xmem_free(buf);
	buf = NULL;

	hexkv_attach(hkv, key, val);
	val = NULL;

	variant_to_null(&key);

	hexkv_destroy(hkv);
	hkv = NULL;

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

	return;
}

int STDCALL slots_invoke(const slots_block_t* pb)
{
	mqtt_block_t* pd = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t file[PATH_LEN] = { 0 };
	tchar_t token[RES_LEN] = { 0 };

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

	pd->hdb = hexdb_create(pd->local, token);
	if (!pd->hdb)
	{
		raise_user_error(_T("-1"), _T("open database failed"));
	}

	pd->mqtt = mqtt_scp(pb->slot, _MQTT_TYPE_SCP_PUB);
	if (!pd->mqtt)
	{
		raise_user_error(NULL, NULL);
	}

	if (!mqtt_accept(pd->mqtt))
	{
		raise_user_error(NULL, NULL);
	}

	while (mqtt_status(pd->mqtt) != _MQTT_STATUS_RELEASE)
	{
		pd->msg_buf = NULL;
		pd->msg_len = 0;

		if (!mqtt_poll_message(pd->mqtt, &pd->msg_buf, &pd->msg_len))
		{
			raise_user_error(NULL, NULL);
		}

		if (!pd->msg_len)
			continue;
		
		mqtt_poll_subscribe(pd->mqtt, pd->topic_name, PATH_LEN);
		pd->msg_qos = pd->mqtt->packet_qos;
		pd->msg_pid = pd->mqtt->packet_pid;

		_invoke_publish(pb, pd);
	}

	mqtt_close(pd->mqtt);
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
			mqtt_close(pd->mqtt);

		if (pd->hdb)
			hexdb_destroy(pd->hdb);

		xmem_free(pd);
	}

	return SLOTS_INVOKE_WITHINFO;
}

