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
	mqtt_t* mqtt;

	tchar_t topic_name[PATH_LEN];

	radnet_t* radnet;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}mqtt_block_t;

void _invoke_error(const slots_block_t* pb, const mqtt_block_t* pd)
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

void _invoke_subcribe(const slots_block_t* pb, const mqtt_block_t* pd)
{
	variant_t key = { 0 };
	object_t val = NULL;
	int encode;
	byte_t* buf = NULL;
	dword_t len;
	dword_t total = 0;

	byte_t* msg_buf = NULL;
	dword_t msg_len;

	TRY_CATCH;

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	if (!radnet_get(pd->radnet, key, val))
	{
		raise_user_error(NULL, NULL);
	}

	encode = object_get_encode(val);

	len = object_get_bytes(val, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(len);

	object_get_bytes(val, buf, len);

	while (len)
	{
		msg_len = GET_DWORD_LOC(buf, total);
		pd->mqtt->packet_qos = (byte_t)GET_SWORD_LOC(buf, total + 4);
		pd->mqtt->packet_pid = GET_SWORD_LOC(buf, total + 6);
		msg_buf = buf + total + 8;

		if (!mqtt_push_message(pd->mqtt, msg_buf, msg_len - 4))
		{
			raise_user_error(NULL, NULL);
		}

		total += (4 + msg_len);
		len -= (4 + msg_len);
	}

	xmem_free(buf);
	buf = NULL;

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	END_CATCH;

	return;
ONERROR:

	XDL_TRACE_LAST;

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	return;
}

void _invoke_unsubcribe(const slots_block_t* pb, const mqtt_block_t* pd)
{
	variant_t key = { 0 };

	TRY_CATCH;

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	if (!radnet_del(pd->radnet, key))
	{
		raise_user_error(NULL, NULL);
	}

	variant_to_null(&key);

	END_CATCH;

	return;
ONERROR:

	XDL_TRACE_LAST;

	variant_to_null(&key);

	return;
}

int STDCALL slots_invoke(slots_block_t* pb)
{
	mqtt_block_t* pd = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t token[PATH_LEN] = { 0 };

	tchar_t addr[ADDR_LEN] = { 0 };
	tchar_t port[INT_LEN] = { 0 };
	tchar_t dbn[RES_LEN] = { 0 };

	xhand_t bio = NULL;

	TRY_CATCH;

	pd = (mqtt_block_t*)xmem_alloc(sizeof(mqtt_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(token, _T("%s/sub.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, token))
	{
		raise_user_error(NULL, NULL);
	}

	read_proper(ptr_prop, _T("RADNET"), -1, _T("SERVER"), -1, addr, ADDR_LEN);
	read_proper(ptr_prop, _T("RADNET"), -1, _T("PORT"), -1, port, INT_LEN);
	read_proper(ptr_prop, _T("RADNET"), -1, _T("DATABASE"), -1, dbn, RES_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	bio = xtcp_cli((unsigned short)xstol(port), addr);
	if (!bio)
	{
		raise_user_error(NULL, NULL);
	}

	pd->radnet = radnet_scu(bio);

	if (!radnet_connect(pd->radnet, dbn))
	{
		raise_user_error(NULL, NULL);
	}

	pd->mqtt = mqtt_scp(pb->slot, _MQTT_TYPE_SCP_SUB);
	if (!pd->mqtt)
	{
		raise_user_error(NULL, NULL);
	}

	if (!mqtt_accept(pd->mqtt))
	{
		raise_user_error(NULL, NULL);
	}

	if (!mqtt_poll_subscribe(pd->mqtt, pd->topic_name, PATH_LEN))
	{
		raise_user_error(NULL, NULL);
	}

	if (pd->mqtt->packet_qos)
		_invoke_subcribe(pb, pd);
	else
		_invoke_unsubcribe(pb, pd);

	radnet_close(pd->radnet);
	pd->radnet = NULL;

	xtcp_close(bio);
	bio = NULL;
	
	mqtt_close(pd->mqtt);
	pd->mqtt = NULL;

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return SLOTS_INVOKE_SUCCEED;

ONERROR:

	_invoke_error(pb, pd);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
	{
		if (pd->mqtt)
			mqtt_close(pd->mqtt);

		if (pd->radnet)
			radnet_close(pd->radnet);

		xmem_free(pd);
	}

	if (bio)
		xtcp_close(bio);

	return SLOTS_INVOKE_WITHINFO;
}

