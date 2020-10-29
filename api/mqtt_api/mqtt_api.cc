/***********************************************************************
	Easily mqtt web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc mqtt publish api document

	@module	mqtt_api.cc | mqtt publish api implement file
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

#include "mqtt_api.h"

typedef struct _mqtt_block_t{
	xhand_t mqtt;

	tchar_t topic_name[PATH_LEN + 1];

	byte_t msg_qos;
	sword_t msg_pid;
	dword_t msg_len;
	byte_t* msg_buf;

	secu_desc_t sd;
	tchar_t local[PATH_LEN + 1];

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

void _invoke_publish(const tcps_block_t* pb, mqtt_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	t_kb_t hkb = NULL;
	t_kv_t hkv = NULL;

	rad_hdr_t hdr = { 0 };

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	xdate_t dt;

	TRY_CATCH;

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_publish"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hkb = tkb_create(path, did);
	if (!hkb)
	{
		raise_user_error(_T("_invoke_publish"), _T("open kv database failed"));
	}

	hkv = tkv_create(hkb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_publish"), _T("create kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, MSG_CONFIG);
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	tkv_read(hkv, key, val);

	get_utc_date(&dt);

	xmem_copy((void*)hdr.ver, (void*)MSGVER_DECTOR, MSGVER_SIZE);
	hdr.qos = pd->msg_qos;
	format_utctime(&dt, hdr.utc);

	radobj_write(val, &hdr, pd->msg_buf, pd->msg_len);

	tkv_attach(hkv, key, val);
	val = NULL;

	variant_to_null(&key);

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hkb);
	hkb = NULL;

	END_CATCH;

	return;
ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		tkv_destroy(hkv);

	if (hkb)
		tkb_destroy(hkb);

	if (pb->ptt)
	{
		(*pb->ptt->pf_track_error)(pb->ptt->hand, sz_code, sz_error);
	}

	return;
}

void _invoke_subcribe(const tcps_block_t* pb, mqtt_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	t_kb_t hkb = NULL;
	t_kv_t hkv = NULL;

	MQTT_PACKET_CTRL mc = { 0 };

	xdate_t dt;
	rad_hdr_t hdr = { 0 };
	dword_t dw = 0;
	byte_t* buf = NULL;

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	TRY_CATCH;

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_publish"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hkb = tkb_create(path, did);
	if (!hkb)
	{
		raise_user_error(_T("_invoke_subcribe"), _T("open kv database failed"));
	}

	hkv = tkv_create(hkb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_subcribe"), _T("create tdb kv entity falied"));
	}

	if (is_null(pid))
	{
		xscpy(pid, MSG_CONFIG);
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	tkv_read(hkv, key, val);

	while ((dw = radobj_read(val, &hdr, NULL, MAX_LONG)))
	{
		buf = (byte_t*)xmem_alloc(dw);
		radobj_read(val, &hdr, buf, dw);

		xmem_set((void*)&mc, 0, sizeof(MQTT_PACKET_CTRL));

		if (hdr.ver[1] == 0x01)
		{
			mc.packet_qos = hdr.qos;
			mc.packet_pid = hdr.mid;
		}

		xmqtt_set_packet_ctrl(pd->mqtt, &mc);

		if (!xmqtt_push_message(pd->mqtt, buf, dw))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;

		parse_datetime(&dt, hdr.utc);
		plus_millseconds(&dt, 1);
		format_utctime(&dt, hdr.utc);
	}

	variant_to_null(&key);

	object_free(val);
	val = NULL;

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hkb);
	hkb = NULL;

	END_CATCH;

	return;
ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		tkv_destroy(hkv);

	if (hkb)
		tkb_destroy(hkb);

	if (pb->ptt)
	{
		(*pb->ptt->pf_track_error)(pb->ptt->hand, sz_code, sz_error);
	}

	return;
}

void _invoke_unsubcribe(const tcps_block_t* pb, mqtt_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };

	t_kb_t hkb = NULL;
	t_kv_t hkv = NULL;

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	TRY_CATCH;

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_unsubcribe"), _T("unknown kv database"));
	}

	if (is_null(pid))
	{
		raise_user_error(_T("_invoke_unsubcribe"), _T("unknown topic"));
	}

	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hkb = tkb_create(path, did);
	if (!hkb)
	{
		raise_user_error(_T("_invoke_unsubcribe"), _T("open kv database failed"));
	}

	hkv = tkv_create(hkb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_unsubcribe"), _T("create tdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	tkv_delete(hkv, key);

	variant_to_null(&key);

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hkb);
	hkb = NULL;

	END_CATCH;

	return;
ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	variant_to_null(&key);

	if (hkv)
		tkv_destroy(hkv);

	if (hkb)
		tkb_destroy(hkb);

	if (pb->ptt)
	{
		(*pb->ptt->pf_track_error)(pb->ptt->hand, sz_code, sz_error);
	}

	return;
}

/*******************************************************************************************************/
int STDCALL tcps_invoke(const tcps_block_t* pb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	mqtt_block_t* pd = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t file[PATH_LEN + 1] = { 0 };
	tchar_t token[RES_LEN + 1] = { 0 };

	int type;
	MQTT_PACKET_CTRL mc = { 0 };

	TRY_CATCH;

	pd = (mqtt_block_t*)xmem_alloc(sizeof(mqtt_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/mqtt.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(NULL, NULL);
	}

	xszero(file, PATH_LEN);

	read_proper(ptr_prop, _T("MQTT"), -1, _T("LOCATION"), -1, file, PATH_LEN);
	read_proper(ptr_prop, _T("MQTT"), -1, _T("PUBLICKEY"), -1, pd->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("MQTT"), -1, _T("PRIVATEKEY"), -1, pd->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->local, file);

	pd->mqtt = xmqtt_scp(pb->tcp, _MQTT_TYPE_SCP_UNK);

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

		type = xmqtt_type(pd->mqtt);

		if (type == _MQTT_TYPE_SCP_PUB)
		{
			if (!pd->msg_len)
				continue;

			xmqtt_poll_subscribe(pd->mqtt, pd->topic_name, PATH_LEN);

			xmem_set((void*)&mc, 0, sizeof(MQTT_PACKET_CTRL));
			xmqtt_get_packet_ctrl(pd->mqtt, &mc);
			pd->msg_qos = mc.packet_qos;
			pd->msg_pid = mc.packet_pid;

			_invoke_publish(pb, pd);
		}
		else if (type == _MQTT_TYPE_SCP_SUB)
		{
			xmqtt_poll_subscribe(pd->mqtt, pd->topic_name, PATH_LEN);

			xmqtt_get_packet_ctrl(pd->mqtt, &mc);
			if (mc.packet_qos)
				_invoke_subcribe(pb, pd);
			else
				_invoke_unsubcribe(pb, pd);

			break;
		}
		else
		{
			break;
		}
	}

	xmqtt_close(pd->mqtt);
	pd->mqtt = NULL;

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return TCPS_INVOKE_SUCCEED;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
	{
		if (pd->mqtt)
			xmqtt_close(pd->mqtt);

		xmem_free(pd);
	}

	if (pb->ptt)
	{
		(*pb->ptt->pf_track_error)(pb->ptt->hand, sz_code, sz_error);
	}

	return TCPS_INVOKE_WITHINFO;
}

