/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc mqtt document

	@module	mqttnet.c | mqtt network implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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

#include "netmqtt.h"
#include "bioinf.h"
#include "stream.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdlinit.h"

typedef struct _mqtt_t{
	xhand_head head;	/*head for xhand_t*/

	if_bio_t* pif;

	int type; /*connect type*/
	int status;	/*connect status*/
	byte_t packet; /*current pdu type*/

	/*session*/
	bool_t	b_username;
	bool_t	b_password;
	bool_t	b_will_retain;
	byte_t	n_will_qos;
	bool_t	b_will_flag;
	bool_t	b_clean_session;
	dword_t will_topic_length;
	byte_t*	will_topic;
	dword_t will_message_length;
	byte_t* will_message;
	dword_t username_length;
	byte_t* username;
	dword_t password_length;
	byte_t* password;

	sword_t session_ver;
	sword_t	session_liv;
	bool_t	session_new;
	byte_t	session_ret;
	dword_t session_len;
	byte_t*	session_cid;

	sword_t packet_pid;
	bool_t	packet_dup;
	int		packet_qos;
	bool_t	packet_ret;

	dword_t topic_size;
	byte_t* topic_name;
	byte_t	topic_qos;

	dword_t message_size;
	byte_t* message_data;
}mqtt_t;

static dword_t _encode_remain_length(byte_t* buf, int len)
{
	dword_t pos = 0;

	while (1)
	{
		if (buf) buf[pos] = len % 128;

		len /= 128;
		if (len)
		{
			if (buf) buf[pos] |= 128;
		}
		else
			break;

		pos++;
	}

	return (pos + 1);
}

static dword_t _decode_remain_length(byte_t* buf, int* plen)
{
	dword_t pos = 0;
	int len = 0;

	*plen = 0;

	while (1)
	{
		len = (buf[pos] & 127);
		len <<= pos;

		if (plen) *plen += len;

		if (!(buf[pos] & 128))
			break;

		pos++;
	}

	return (pos + 1);
}

static dword_t _mqtt_format_session(mqtt_t* mqtt, byte_t* pdv_data, dword_t pdv_size)
{
	dword_t pos;

	pos = 0;

	if (pos + 2 > pdv_size) return pos;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, pos, mqtt->session_len);
	}
	pos += 2;

	if (pos + mqtt->session_len > pdv_size) return pos;
	if (pdv_data)
	{
		xmem_copy((void*)(pdv_data + pos), mqtt->session_cid, mqtt->session_len);
	}
	pos += mqtt->session_len;

	if (mqtt->will_topic)
	{
		if (pos + 2 > pdv_size) return pos;
		if (pdv_data)
		{
			PUT_SWORD_NET(pdv_data, pos, mqtt->will_topic_length);
		}
		pos += 2;

		if (pos + mqtt->will_topic_length > pdv_size) return pos;
		if (pdv_data)
		{
			xmem_copy((void*)(pdv_data + pos), mqtt->will_topic, mqtt->will_topic_length);
		}
		pos += mqtt->will_topic_length;

		if (pos + 2 > pdv_size) return pos;
		if (pdv_data)
		{
			PUT_SWORD_NET(pdv_data, pos, mqtt->will_message_length);
		}
		pos += 2;

		if (pos + mqtt->will_message_length > pdv_size) return pos;
		if (pdv_data)
		{
			xmem_copy((void*)(pdv_data + pos), mqtt->will_message, mqtt->will_message_length);
		}
		pos += mqtt->will_message_length;
	}

	if (mqtt->b_username)
	{
		if (pos + 2 > pdv_size) return pos;
		if (pdv_data)
		{
			PUT_SWORD_NET(pdv_data, pos, mqtt->username_length);
		}
		pos += 2;

		if (pos + mqtt->username_length > pdv_size) return pos;
		if (pdv_data)
		{
			xmem_copy((void*)(pdv_data + pos), mqtt->username, mqtt->username_length);
		}
		pos += mqtt->username_length;
	}

	if (mqtt->b_password)
	{
		if (pos + 2 > pdv_size) return pos;
		if (pdv_data)
		{
			PUT_SWORD_NET(pdv_data, pos, mqtt->password_length);
		}
		pos += 2;

		if (pos + mqtt->password_length > pdv_size) return pos;
		if (pdv_data)
		{
			xmem_copy((void*)(pdv_data + pos), mqtt->password, mqtt->password_length);
		}
		pos += mqtt->password_length;
	}

	return pos;
}

static bool_t _mqtt_parse_session(mqtt_t* mqtt, const byte_t* pdv_data, dword_t pdv_size)
{
	lword_t tms;
	nuid_t nid;
	tchar_t buf[UUID_LEN + 1] = { 0 };

	dword_t pos = 0;

	if (pos + 2 > pdv_size) return 0;

	pos = 0;
	mqtt->session_len = GET_SWORD_NET(pdv_data, pos);
	pos += 2;

	if (pos + mqtt->session_len > pdv_size) return 0;

	mqtt->session_cid = (byte_t*)xmem_realloc(mqtt->session_cid, mqtt->session_len);
	xmem_copy((void*)(mqtt->session_cid), (void*)(pdv_data + pos), mqtt->session_len);
	pos += mqtt->session_len;

	if (mqtt->will_topic)
	{
		if (pos + 2 > pdv_size) return 0;

		mqtt->will_topic_length = GET_SWORD_NET(pdv_data, pos);
		pos += 2;

		if (pos + mqtt->will_topic_length > pdv_size) return 0;

		mqtt->will_topic = (byte_t*)xmem_realloc(mqtt->will_topic, mqtt->will_topic_length);
		xmem_copy((void*)(mqtt->will_topic), (void*)(pdv_data + pos), mqtt->will_topic_length);
		pos += mqtt->will_topic_length;

		if (pos + 2 > pdv_size) return 0;

		mqtt->will_message_length = GET_SWORD_NET(pdv_data, pos);
		pos += 2;

		if (pos + mqtt->will_message_length > pdv_size) return 0;

		mqtt->will_message = (byte_t*)xmem_realloc(mqtt->will_message, mqtt->will_message_length);
		xmem_copy((void*)(mqtt->will_message), (void*)(pdv_data + pos), mqtt->will_message_length);
		pos += mqtt->will_message_length;
	}

	if (mqtt->b_username)
	{
		if (pos + 2 > pdv_size) return 0;

		mqtt->username_length = GET_SWORD_NET(pdv_data, pos);
		pos += 2;

		if (pos + mqtt->username_length > pdv_size) return 0;

		mqtt->username = (byte_t*)xmem_realloc(mqtt->username, mqtt->username_length);
		xmem_copy((void*)(mqtt->username), (void*)(pdv_data + pos), mqtt->username_length);
		pos += mqtt->username_length;
	}

	if (mqtt->b_password)
	{
		if (pos + 2 > pdv_size) return 0;

		mqtt->password_length = GET_SWORD_NET(pdv_data, pos);
		pos += 2;

		if (pos + mqtt->password_length > pdv_size) return 0;

		mqtt->password = (byte_t*)xmem_realloc(mqtt->password, mqtt->password_length);
		xmem_copy((void*)(mqtt->password), (void*)(pdv_data + pos), mqtt->password_length);
		pos += mqtt->password_length;
	}

	if (!mqtt->b_clean_session && !mqtt->session_len)
	{
		tms = get_timestamp();
		nuid_from_timestamp(&nid, tms);
		nuid_format_string(&nid, buf, UUID_LEN);

		mqtt->session_cid = (byte_t*)xmem_realloc(mqtt->session_cid, UUID_LEN);

#ifdef _UNICODE
		mqtt->session_len = ucs_to_utf8(buf, -1, mqtt->session_cid, UUID_LEN);
#else
		mqtt->session_len = mbs_to_utf8(buf, -1, mqtt->session_cid, UUID_LEN);
#endif
	}

	mqtt->session_ret = MQTT_RET_ACCEPTED;
	return 1;
}

static void _mqtt_clean_session(mqtt_t* mqtt)
{
	if (mqtt->session_cid)
	{
		xmem_free(mqtt->session_cid);
		mqtt->session_len = 0;
	}

	if (mqtt->will_topic)
	{
		xmem_free(mqtt->will_topic);
		mqtt->will_message_length = 0;
	}

	if (mqtt->will_message)
	{
		xmem_free(mqtt->will_message);
		mqtt->will_message_length = 0;
	}

	if (mqtt->username)
	{
		xmem_free(mqtt->username);
		mqtt->username_length = 0;
	}

	if (mqtt->password)
	{
		xmem_free(mqtt->password);
		mqtt->password_length = 0;
	}

	if (mqtt->topic_name)
	{
		xmem_free(mqtt->topic_name);
		mqtt->topic_size = 0;
	}

	if (mqtt->message_data)
	{
		xmem_free(mqtt->message_data);
		mqtt->message_size = 0;
	}
}

static dword_t _mqtt_format_subcribe(mqtt_t* mqtt, byte_t* pdv_data, dword_t pdv_size)
{
	dword_t n, total = 0;

	n = 2;
	if (total + n > pdv_size) return total;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, total, mqtt->topic_size);
	}
	total += n;

	n = mqtt->topic_size;
	if (total + n > pdv_size) return total;
	if (pdv_data)
	{
		xmem_copy((void*)(pdv_data + total), (void*)mqtt->topic_name, mqtt->topic_size);
	}
	total += n;

	return total;
}

static bool_t _mqtt_parse_subcribe(mqtt_t* mqtt, const byte_t* pdv_data, dword_t pdv_size)
{
	dword_t total = 0;

	mqtt->topic_size = GET_SWORD_NET(pdv_data, total);
	total += 2;

	mqtt->topic_name = (byte_t*)xmem_realloc(mqtt->topic_name, mqtt->topic_size);

	xmem_copy((void*)mqtt->topic_name, (void*)(pdv_data + total), mqtt->topic_size);
	total += mqtt->topic_size;

	return 1;
}

static bool_t _mqtt_write_pdu(mqtt_t* mqtt, byte_t pdu_type, dword_t pdv_size)
{
	dword_t ctl_size = 0, var_size = 0, pdu_size = 0;
	dword_t n, total = 0;
	byte_t flags,c;
	byte_t ctl[INT_LEN] = { 0 };
	byte_t* buf = NULL;

	stream_t stm = NULL;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDU: {CONTROL | VARIABLE | PAYLOAD}

	TRY_CATCH;

	stm = stream_alloc(mqtt->pif);

	switch (pdu_type)
	{
	case MQTT_PDU_CONNECT:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE:	{proto {length | name | level} | connect flag {7-User Name Flag| 6-Password Flag| 5-Will Retain| 4-3-Will QoS| 2-Will Flag| 1-Clean Session| 0-Reserved} |  Keep Alive}

		//type:HB flags:LB
		n = 1;
		flags = 0; //CONNECT PDU FLAGS: none
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		var_size = sizeof(sword_t) + MQTT_PROTOCOL_SIZE + 1 + 1 + sizeof(short);
		pdu_size = var_size + pdv_size;
		if (pdu_size > MQTT_PDU_SIZE_MAXINUM)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//proto length:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, MQTT_PROTOCOL_SIZE);
		var_size += n;
		total += n;

		//proto name
		n = MQTT_PROTOCOL_SIZE;
		xmem_copy((void*)(buf + var_size), (void*)MQTT_PROTO_NAME, MQTT_PROTOCOL_SIZE);
		var_size += n;
		total += n;

		//proto level:BYTE
		n = 1;
		c = MQTT_PROTO_LEVEL;
		PUT_BYTE(buf, var_size, c);
		var_size += n;
		total += n;

		//connect flags:BYTE=BITS{7 - User Name Flag | 6 - Password Flag | 5 - Will Retain | 4 - 3 - Will QoS | 2 - Will Flag | 1 - Clean Session}
		n = 1;
		c = 0;
		if (mqtt->b_username)
			c |= MQTT_MSK_USERNAME;
		if (mqtt->b_password)
			c |= MQTT_MSK_PASSWORD;
		if (mqtt->b_will_retain)
			c |= MQTT_MSK_WILL_RETAIN;
		if (mqtt->n_will_qos)
			c |= (mqtt->n_will_qos << 3);
		if (mqtt->b_will_flag)
			c |= MQTT_MSK_WILL_FLAG;
		if (mqtt->b_clean_session)
			c |= MQTT_MSK_CLEAN_SESSION;
		PUT_BYTE(buf, var_size, c);
		var_size += n;
		total += n;

		//Keep Alive:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->session_liv);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;

		//must continue write payload
		total += pdv_size;
		break;
	case MQTT_PDU_CONNACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Connect Acknowledge Flags | Connect Return code}
		
		//type:HB flags:LB
		n = 1;
		flags = 0; //CONNACK PDU FLAGS: none
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = 1 + 1;
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Connect Acknowledge Flags:BYTE
		n = 1;
		if (mqtt->session_new)
			c = 0;
		else
			c = 1;
		PUT_BYTE(buf, var_size, c);
		var_size += n;
		total += n;

		//Connect Return code:BYTE
		n = 1;
		c = mqtt->session_ret;;
		PUT_BYTE(buf, var_size, c);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case MQTT_PDU_PUBLISH:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {topic {length | name} | Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = 0;
		if (mqtt->packet_dup)
			flags |= MQTT_MSK_DUPFLAG;
		if (mqtt->packet_qos)
			flags |= (mqtt->packet_qos << 1);
		if (mqtt->packet_ret)
			flags |= MQTT_MSK_RETFLAG;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		var_size = sizeof(short) + mqtt->topic_size + ((mqtt->packet_qos)? sizeof(short) : 0);
		pdu_size = var_size + pdv_size;
		if (pdu_size > MQTT_PDU_SIZE_MAXINUM)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//topic length:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->topic_size);
		var_size += n;
		total += n;

		//topic name
		n = mqtt->topic_size;
		xmem_copy((void*)(buf + var_size), (void*)mqtt->topic_name, n);
		var_size += n;
		total += n;

		if (mqtt->packet_qos)
		{
			//Packet Identifier:MSB LSB
			n = sizeof(short);
			PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
			var_size += n;
			total += n;
		}

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;

		//must continue write payload
		total += pdv_size;
		break;
	case MQTT_PDU_PUBACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case MQTT_PDU_PUBREC:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case MQTT_PDU_PUBREL:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = (MQTT_QOS_ONEMORE << 1);
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case MQTT_PDU_PUBCOMP:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier:MSB LSB
		n = sizeof(sword_t);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case MQTT_PDU_SUBSCRIBE:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = (MQTT_QOS_ONEMORE << 1);
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		if (pdu_size > MQTT_PDU_SIZE_MAXINUM)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;

		//must continue write payload
		total += pdv_size;
		break;
	case MQTT_PDU_SUBACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier
		n = sizeof(sword_t);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case MQTT_PDU_UNSUBSCRIBE:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = (MQTT_QOS_ONEMORE << 1);
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		if (pdu_size > MQTT_PDU_SIZE_MAXINUM)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;

		//must continue write payload
		total += pdv_size;
		break;
	case MQTT_PDU_UNSUBACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = sizeof(short);
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(var_size);
		var_size = 0;

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		PUT_SWORD_NET(buf, var_size, mqtt->packet_pid);
		var_size += n;
		total += n;

		if (!stream_write_bytes(stm, buf, var_size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case MQTT_PDU_PINGREQ:
		//CONTROL: header{ {type | flags} | remain length }

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = 0;
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}
		break;
	case MQTT_PDU_PINGRESP:
		//CONTROL: header{ {type | flags} | remain length }

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = 0;
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}
		break;
	case MQTT_PDU_DISCONNECT:
		//CONTROL: header{ {type | flags} | remain length }

		//type:HB flags:LB
		n = 1;
		flags = 0;
		c = (pdu_type << 4 | flags);
		ctl[ctl_size] = c;
		ctl_size += n;
		total += n;

		//remain length: 
		if (pdv_size)
		{
			raise_user_error(_T("write_pdu"), _T("invalid packet size"));
		}
		var_size = 0;
		pdu_size = var_size + pdv_size;
		n = _encode_remain_length(ctl + ctl_size, pdu_size);
		ctl_size += n;
		total += n;

		if (!stream_write_bytes(stm, ctl, ctl_size))
		{
			raise_user_error(NULL, NULL);
		}

		break;
	}

	XDL_ASSERT(total == ctl_size + var_size + pdv_size);

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (buf)
		buf = NULL;

	if (stm)
		stream_free(stm);

	return 0;
}

static bool_t _mqtt_read_pdu(mqtt_t* mqtt, byte_t* pdu_type, dword_t* pdv_size)
{
	dword_t pdu_size, var_size, n;
	byte_t flags;

	byte_t buf[INT_LEN] = { 0 };
	dword_t pos;

	stream_t stm = NULL;

	TRY_CATCH;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDU: {CONTROL | VARIABLE | PAYLOAD}

	stm = stream_alloc(mqtt->pif);

	//1: pdu type
	n = 1;
	if (!stream_read_bytes(stm, buf, &n))
	{
		raise_user_error(NULL, NULL);
	}
	*pdu_type = (buf[0] & 0xF0) >> 4;
	flags = buf[0] & 0x0F;

	//2.remain length
	pos = 0;
	while (1)
	{
		n = 1;
		if (!stream_read_bytes(stm, buf + pos, &n))
		{
			raise_user_error(NULL, NULL);
		}

		if (!(buf[pos] & 0x80))
			break;
		pos++;
	}
	_decode_remain_length(buf, (int*)&pdu_size);

	if (pdu_size > MQTT_PDU_SIZE_MAXINUM)
	{
		raise_user_error(_T("PDU"), _T("Invalid pdu size"));
	}

	switch (*pdu_type)
	{
	case MQTT_PDU_CONNECT:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE:	{proto {length | name | level} | connect flag {7-User Name Flag| 6-Password Flag| 5-Will Retain| 4-3-Will QoS| 2-Will Flag| 1-Clean Session| 0-Reserved} |  Keep Alive}

		if (flags != 0)
		{
			raise_user_error(_T("CONNECT"), _T("Invalid control flags"));
		}

		//proto length:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		pdu_size -= n;

		//remain length: 
		n = GET_SWORD_NET(buf, 0);
		var_size = sizeof(short) + n + 1 + 1 + sizeof(short);
		var_size -= sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("CONNECT"), _T("Invalid pakcet size"));
		}

		//proto name
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		pdu_size -= n;
		var_size -= n;

		//proto level:BYTE
		n = 1;
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		pdu_size -= n;
		var_size -= n;

		//connect flags:BYTE=BITS{7 - User Name Flag | 6 - Password Flag | 5 - Will Retain | 4 - 3 - Will QoS | 2 - Will Flag | 1 - Clean Session}
		n = 1;
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		if (buf[0] & MQTT_MSK_USERNAME) mqtt->b_username = 1;
		if (buf[0] & MQTT_MSK_PASSWORD) mqtt->b_password = 1;
		if (buf[0] & MQTT_MSK_WILL_RETAIN) mqtt->b_will_retain = 1;
		mqtt->n_will_qos = (buf[0] & MQTT_MSK_WILL_QOS) >> 3;
		if (buf[0] & MQTT_MSK_WILL_FLAG) mqtt->b_will_flag = 1;
		if (buf[0] & MQTT_MSK_CLEAN_SESSION) mqtt->b_clean_session = 1;
		pdu_size -= n;
		var_size -= n;

		//Keep Alive:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		mqtt->session_liv = GET_SWORD_NET(buf, 0);
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_CONNACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Connect Acknowledge Flags | Connect Return code}

		if (flags != 0)
		{
			raise_user_error(_T("CONNACK"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = 1 + 1;
		if (pdu_size < var_size)
		{
			raise_user_error(_T("CONNACK"), _T("Invalid pakcet size"));
		}

		//Connect Acknowledge Flags:BYTE
		n = 1;
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		mqtt->session_new = (buf[0]) ? 0 : 1;
		pdu_size -= n;
		var_size -= n;

		//Connect Return code:BYTE
		n = 1;
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		mqtt->session_ret = buf[0];
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_PUBLISH:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {topic {length | name} | Packet Identifier}

		if (flags & MQTT_MSK_DUPFLAG) mqtt->packet_dup = 1;
		mqtt->packet_qos = (flags & MQTT_MSK_QOSFLAG) >> 1;
		if (flags & MQTT_MSK_RETFLAG) mqtt->packet_ret = 1;
		
		//topic length:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		mqtt->topic_size = GET_SWORD_NET(buf, 0);
		//variable length: 
		var_size = sizeof(short) + mqtt->topic_size + ((mqtt->packet_qos)? sizeof(short) : 0);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("PUBLISH"), _T("Invalid pakcet size"));
		}
		pdu_size -= n;
		var_size -= n;

		//topic name
		n = mqtt->topic_size;
		mqtt->topic_name = (byte_t*)xmem_realloc(mqtt->topic_name, n + 1);
		if (!stream_read_bytes(stm, mqtt->topic_name, &n))
		{
			raise_user_error(NULL, NULL);
		}
		pdu_size -= n;
		var_size -= n;

		if (mqtt->packet_qos)
		{
			//Packet Identifier:MSB LSB
			n = sizeof(short);
			if (!stream_read_bytes(stm, buf, &n))
			{
				raise_user_error(NULL, NULL);
			}
			mqtt->packet_pid = GET_SWORD_NET(buf, 0);
			pdu_size -= n;
			var_size -= n;
		}

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_PUBACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		if (flags != 0)
		{
			raise_user_error(_T("PUBACK"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("PUBACK"), _T("Invalid pakcet size"));
		}

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		if (GET_SWORD_NET(buf, 0) != mqtt->packet_pid)
		{
			raise_user_error(_T("PUBACK"), _T("Invalid pakcet id"));
		}
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_PUBREC:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		if (flags != 0)
		{
			raise_user_error(_T("PUBREC"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("PUBREC"), _T("Invalid pakcet size"));
		}

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		if (GET_SWORD_NET(buf, 0) != mqtt->packet_pid)
		{
			raise_user_error(_T("PUBREC"), _T("Invalid pakcet id"));
		}
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_PUBREL:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		if (flags != (MQTT_QOS_ONEMORE << 1))
		{
			raise_user_error(_T("PUBREL"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("PUBREL"), _T("Invalid pakcet size"));
		}

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		if (GET_SWORD_NET(buf, 0) != mqtt->packet_pid)
		{
			raise_user_error(_T("PUBREL"), _T("Invalid pakcet id"));
		}
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_PUBCOMP:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		if (flags != 0)
		{
			raise_user_error(_T("PUBCOMP"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("PUBCOMP"), _T("Invalid pakcet size"));
		}

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		if (GET_SWORD_NET(buf, 0) != mqtt->packet_pid)
		{
			raise_user_error(_T("PUBCOMP"), _T("Invalid pakcet id"));
		}
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_SUBSCRIBE:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		if (flags != (MQTT_QOS_ONEMORE << 1))
		{
			raise_user_error(_T("SUBSCRIBE"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("SUBSCRIBE"), _T("Invalid pakcet size"));
		}

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		mqtt->packet_pid = GET_SWORD_NET(buf, 0);
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_SUBACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		if (flags != 0)
		{
			raise_user_error(_T("SUBACK"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("SUBACK"), _T("Invalid pakcet size"));
		}

		//Packet Identifier
		n = sizeof(sword_t);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		if (GET_SWORD_NET(buf, 0) != mqtt->packet_pid)
		{
			raise_user_error(_T("SUBACK"), _T("Invalid pakcet id"));
		}
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_UNSUBSCRIBE:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		flags = (MQTT_QOS_ONEMORE << 1);
		if (flags != (MQTT_QOS_ONEMORE << 1))
		{
			raise_user_error(_T("UNSUBSCRIBE"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("UNSUBSCRIBE"), _T("Invalid pakcet size"));
		}

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		mqtt->packet_pid = GET_SWORD_NET(buf, 0);
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_UNSUBACK:
		//CONTROL: header{ {type | flags} | remain length }
		//VARIABLE: {Packet Identifier}

		if (flags != 0)
		{
			raise_user_error(_T("UNSUBACK"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = sizeof(short);
		if (pdu_size < var_size)
		{
			raise_user_error(_T("UNSUBACK"), _T("Invalid pakcet size"));
		}

		//Packet Identifier:MSB LSB
		n = sizeof(short);
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}
		if (GET_SWORD_NET(buf, 0) != mqtt->packet_pid)
		{
			raise_user_error(_T("UNSUBACK"), _T("Invalid pakcet id"));
		}
		pdu_size -= n;
		var_size -= n;

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_PINGREQ:
		//CONTROL: header{ {type | flags} | remain length }

		if (flags != 0)
		{
			raise_user_error(_T("PINGREQ"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = 0;
		if (pdu_size < var_size)
		{
			raise_user_error(_T("PINGREQ"), _T("Invalid pakcet size"));
		}

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_PINGRESP:
		//CONTROL: header{ {type | flags} | remain length }

		if (flags != 0)
		{
			raise_user_error(_T("PINGRESP"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = 0;
		if (pdu_size < var_size)
		{
			raise_user_error(_T("PINGRESP"), _T("Invalid pakcet size"));
		}

		XDL_ASSERT(!var_size);
		break;
	case MQTT_PDU_DISCONNECT:
		//CONTROL: header{ {type | flags} | remain length }

		if (flags != 0)
		{
			raise_user_error(_T("DISCONNECT"), _T("Invalid control flags"));
		}

		//remain length: 
		var_size = 0;
		if (pdu_size < var_size)
		{
			raise_user_error(_T("DISCONNECT"), _T("Invalid pakcet size"));
		}

		XDL_ASSERT(!var_size);
		break;
	}

	if (pdv_size) *pdv_size = pdu_size;

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

/************************************************************************************************************/

xhand_t xmqtt_scu(xhand_t bio, int scu)
{
	mqtt_t* pmqtt = NULL;

	XDL_ASSERT(scu == _MQTT_TYPE_SCU_PUB || scu == _MQTT_TYPE_SCU_SUB);

	if (!bio)
		return NULL;

	pmqtt = (mqtt_t*)xmem_alloc(sizeof(mqtt_t));
	pmqtt->head.tag = _HANDLE_MQTT;
	pmqtt->type = scu;

	pmqtt->pif = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(bio, pmqtt->pif);

	pmqtt->session_ver = MQTT_VER;

	return (xhand_t)&(pmqtt->head);
}

xhand_t xmqtt_scp(xhand_t bio, int scp)
{
	mqtt_t* pmqtt = NULL;

	if (!bio)
		return NULL;

	pmqtt = (mqtt_t*)xmem_alloc(sizeof(mqtt_t));
	pmqtt->head.tag = _HANDLE_MQTT;
	pmqtt->type = scp;

	pmqtt->pif = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(bio, pmqtt->pif);

	pmqtt->session_ver = MQTT_VER;

	return (xhand_t)&(pmqtt->head);
}

xhand_t xmqtt_bio(xhand_t mqtt)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	return (pmqtt->pif)? pmqtt->pif->bio : NULL;
}

int xmqtt_type(xhand_t mqtt)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	return pmqtt->type;
}

int xmqtt_status(xhand_t mqtt)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	return pmqtt->status;
}

bool_t xmqtt_recv(xhand_t mqtt, dword_t* size)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	byte_t pdu_type = 0;
	dword_t pdv_size = 0;

	TRY_CATCH;

	if (pmqtt->type == _MQTT_TYPE_SCP_SUB || pmqtt->type == _MQTT_TYPE_SCP_UNK)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			//read CONNECT PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_CONNECT)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid connect pdu"));
			}
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("connect must has payload"));
			}
			pmqtt->packet = pdu_type;
			//continue read CONNECT payload
			break;
		case _MQTT_STATUS_SLEEPING:
			//read SUBCRIBE or UNSUBSCRIBE PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == MQTT_PDU_DISCONNECT)
			{
				pmqtt->status = _MQTT_STATUS_RELEASE;
				break;
			}

			//_MQTT_TYPE_SCP_UNK -> _MQTT_TYPE_SCP_SUB | _MQTT_TYPE_SCP_PUB
			if (pmqtt->type == _MQTT_TYPE_SCP_UNK && pdu_type == MQTT_PDU_PUBLISH)
			{
				if (!pdv_size)
				{
					raise_user_error(_T("mqtt_recv"), _T("publish must has payload"));
				}
				pmqtt->packet = pdu_type;
				//continue read PUBLISH payload
				pmqtt->status = (pmqtt->packet_qos == MQTT_QOS_NONE) ? _MQTT_STATUS_WAITING : _MQTT_STATUS_PENDING;

				pmqtt->type = _MQTT_TYPE_SCP_PUB;
				break;
			}
			else if (pmqtt->type == _MQTT_TYPE_SCP_UNK && (pdu_type == MQTT_PDU_SUBSCRIBE || pdu_type == MQTT_PDU_UNSUBSCRIBE))
			{
				pmqtt->type = _MQTT_TYPE_SCP_SUB;
			}

			if (pdu_type != MQTT_PDU_SUBSCRIBE && pdu_type != MQTT_PDU_UNSUBSCRIBE)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("subcribe or unscbscribe must has payload"));
			}
			pmqtt->packet_qos = (pdu_type == MQTT_PDU_SUBSCRIBE) ? MQTT_QOS_ONEMORE : MQTT_QOS_NONE;
			pmqtt->packet = pdu_type;
			//continue read SUBCRIBE payload
			break;
		case _MQTT_STATUS_WAITING:
			raise_user_error(_T("mqtt_recv"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_PENDING:
			//read PUBLISH ACK or REC
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == MQTT_PDU_DISCONNECT)
			{
				pmqtt->status = _MQTT_STATUS_RELEASE;
				break;
			}
			if (pdu_type != MQTT_PDU_PUBACK && pdu_type != MQTT_PDU_PUBREC)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			pmqtt->packet = pdu_type;
			//continue to publish
			pmqtt->status = (pdu_type == MQTT_PDU_PUBACK) ? _MQTT_STATUS_WAITING : _MQTT_STATUS_RETAINED;
			break;
		case _MQTT_STATUS_RETAINED:
			raise_user_error(_T("mqtt_recv"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_COMPELETE:
			//read PUBLISH COMP
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == MQTT_PDU_DISCONNECT)
			{
				pmqtt->status = _MQTT_STATUS_RELEASE;
				break;
			}
			if (pdu_type != MQTT_PDU_PUBCOMP)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			pmqtt->packet = pdu_type;
			//continue to publish
			pmqtt->status = _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_RELEASE:
			raise_user_error(_T("mqtt_recv"), _T("server released"));
			break;
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCP_PUB)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			//read CONNECT PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_CONNECT)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid connect pdu"));
			}
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("connect must has payload"));
			}
			pmqtt->packet = pdu_type;
			//continue read CONNECT payload
			break;
		case _MQTT_STATUS_WAITING:
			//read PUBLISH PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == MQTT_PDU_DISCONNECT)
			{
				pmqtt->packet = pdu_type;
				pmqtt->status = _MQTT_STATUS_RELEASE;
				break;
			}
			if (pdu_type != MQTT_PDU_PUBLISH)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("publish must has payload"));
			}
			pmqtt->packet = pdu_type;
			//continue read PUBLISH payload
			pmqtt->status = (pmqtt->packet_qos == MQTT_QOS_NONE) ? _MQTT_STATUS_WAITING : _MQTT_STATUS_PENDING;
			break;
		case _MQTT_STATUS_PENDING:
			raise_user_error(_T("mqtt_recv"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_RETAINED:
			//read PUBLISH PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == MQTT_PDU_DISCONNECT)
			{
				pmqtt->packet = pdu_type;
				pmqtt->status = _MQTT_STATUS_RELEASE;
				break;
			}
			if (pdu_type != MQTT_PDU_PUBREL)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			pmqtt->packet = pdu_type;
			//continue write PUBLISH COMP
			pmqtt->status = _MQTT_STATUS_COMPELETE;
			break;
		case _MQTT_STATUS_COMPELETE:
			raise_user_error(_T("mqtt_recv"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_RELEASE:
			raise_user_error(_T("mqtt_recv"), _T("server released"));
			break;
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCU_SUB)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			//read CONNACK PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_CONNACK)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid connack pdu"));
			}
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("connack has no payload"));
			}
			pmqtt->packet = pdu_type;
			//then to send SUBSCRIBE
			pmqtt->status = _MQTT_STATUS_SLEEPING;
			break;
		case _MQTT_STATUS_SLEEPING:
			//read SUBSCRIBE or UNSUBSCRIBE ACK
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_SUBACK && pdu_type != MQTT_PDU_UNSUBACK)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("subscribe or unsubscribe has no payload"));
			}
			pmqtt->packet = pdu_type;
			// then to read PUBLISH
			pmqtt->status = (pdu_type == MQTT_PDU_SUBACK) ? _MQTT_STATUS_WAITING : _MQTT_STATUS_RELEASE;
			break;
		case _MQTT_STATUS_WAITING:
			//read PUBLISH PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_PUBLISH)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			if (!pdv_size)
			{
				pmqtt->packet = pdu_type;
				pmqtt->status = _MQTT_STATUS_RELEASE;
				break;
			}
			pmqtt->packet = pdu_type;
			// continue to read PUBLISH payload
			pmqtt->status = (pmqtt->packet_qos == MQTT_QOS_NONE) ? _MQTT_STATUS_WAITING : _MQTT_STATUS_PENDING;
			break;
		case _MQTT_STATUS_PENDING:
			raise_user_error(_T("mqtt_recv"), _T("client released"));
			break;
		case _MQTT_STATUS_RETAINED:
			//read PUBLISH REL
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_PUBREL)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}

			pmqtt->packet = pdu_type;
			pmqtt->status = _MQTT_STATUS_COMPELETE;
			break;
		case _MQTT_STATUS_COMPELETE:
			raise_user_error(_T("mqtt_recv"), _T("client released"));
			break;
		case _MQTT_STATUS_RELEASE:
			raise_user_error(_T("mqtt_recv"), _T("client released"));
			break;
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCU_PUB)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			//read CONNACK PDU
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_CONNACK)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid connack pdu"));
			}
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("connack has no payload"));
			}
			pmqtt->packet = pdu_type;
			//then to send PUBLISH
			pmqtt->status = _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_WAITING:
			raise_user_error(_T("mqtt_recv"), _T("invalid client status"));
			break;
		case _MQTT_STATUS_PENDING:
			//read PUBLISH ACK or REC
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_PUBACK && pdu_type != MQTT_PDU_PUBREC)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_recv"), _T("PUBACK has no payload"));
			}
			pmqtt->packet = pdu_type;
			if (pmqtt->packet_qos == MQTT_QOS_ONCE)
				pmqtt->status = _MQTT_STATUS_RETAINED;
			else
				pmqtt->status = _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_RETAINED:
			raise_user_error(_T("mqtt_recv"), _T("invalid client status"));
			break;
		case _MQTT_STATUS_COMPELETE:
			//read PUBLISH COMP
			if (!_mqtt_read_pdu(pmqtt, &pdu_type, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != MQTT_PDU_PUBCOMP)
			{
				raise_user_error(_T("mqtt_recv"), _T("invalid message type"));
			}
			
			pmqtt->packet = pdu_type;
			pmqtt->status = _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_RELEASE:
			raise_user_error(_T("mqtt_recv"), _T("client released"));
			break;
		}
	}

	*size = pdv_size;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t xmqtt_send(xhand_t mqtt, dword_t pdv_size)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	byte_t pdu_type = 0;

	TRY_CATCH;

	if (pmqtt->type == _MQTT_TYPE_SCP_SUB || pmqtt->type == _MQTT_TYPE_SCP_UNK)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("CONNACK has no payload"));
			}
			//write CONNACK PDU
			pmqtt->packet = MQTT_PDU_CONNACK;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			pmqtt->status = _MQTT_STATUS_SLEEPING;
			break;
		case _MQTT_STATUS_SLEEPING:
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("SUBACK or UNSUBACK has no payload"));
			}
			//write SUBACK or UNSUBACK PDU
			if (pmqtt->packet == MQTT_PDU_SUBSCRIBE)
				pmqtt->packet = MQTT_PDU_SUBACK;
			else
				pmqtt->packet = MQTT_PDU_UNSUBACK;

			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			pmqtt->status = (pmqtt->packet == MQTT_PDU_SUBACK) ? _MQTT_STATUS_WAITING : _MQTT_STATUS_RELEASE;
			break;
		case _MQTT_STATUS_WAITING:
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("PUBLISH must has a payload"));
			}
			//write PUBLISH PDU
			pmqtt->packet = MQTT_PDU_PUBLISH;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to send publish payload
			pmqtt->status = (pmqtt->packet_qos) ? _MQTT_STATUS_PENDING : _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_PENDING:
			raise_user_error(_T("mqtt_recv"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_RETAINED:
			//write PUBLISH REL
			pmqtt->packet = MQTT_PDU_PUBREL;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to read publish comp
			pmqtt->status = _MQTT_STATUS_COMPELETE;
			break;
		case _MQTT_STATUS_COMPELETE:
			raise_user_error(_T("mqtt_recv"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_RELEASE:
			//raise_user_error(_T("mqtt_recv"), _T("server released"));
			break;
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCP_PUB)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("CONNACK has no payload"));
			}
			//write CONNACK PDU
			pmqtt->packet = MQTT_PDU_CONNACK;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			pmqtt->status = _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_WAITING:
			raise_user_error(_T("mqtt_send"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_PENDING:
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("PUBACK has no payload"));
			}
			//write PUBLISH ACK or REC
			if (pmqtt->packet_qos == MQTT_QOS_ONCE)
				pmqtt->packet = MQTT_PDU_PUBREC;
			else
				pmqtt->packet = MQTT_PDU_PUBACK;
			
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}

			if (pmqtt->packet_qos == MQTT_QOS_ONCE)
			{
				//waiting to read publish rel
				pmqtt->status = _MQTT_STATUS_RETAINED;
			}
			else
			{
				//continue to read publish
				pmqtt->status = _MQTT_STATUS_WAITING;
			}
			break;
		case _MQTT_STATUS_RETAINED:
			raise_user_error(_T("mqtt_send"), _T("invalid server status"));
			break;
		case _MQTT_STATUS_COMPELETE:
			pmqtt->packet = MQTT_PDU_PUBCOMP;

			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, 0))
			{
				raise_user_error(NULL, NULL);
			}

			//continue to read publish
			pmqtt->status = _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_RELEASE:
			//raise_user_error(_T("mqtt_send"), _T("server released"));
			break;
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCU_SUB)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("CONNECT must has payload"));
			}
			//write CONNECT PDU
			pmqtt->packet = MQTT_PDU_CONNECT;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to write payload
			break;
		case _MQTT_STATUS_SLEEPING:
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("SUBSCRIBE must has payload"));
			}
			//write SUBSCRIBE PDU or UNSUBSCRIBE
			pmqtt->packet = (pmqtt->packet_qos) ? MQTT_PDU_SUBSCRIBE : MQTT_PDU_UNSUBSCRIBE;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to send payload
			break;
		case _MQTT_STATUS_WAITING:
			raise_user_error(_T("mqtt_send"), _T("invalid client status"));
			break;
		case _MQTT_STATUS_PENDING:
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("PUBACK has no payload"));
			}
			//write PUBLISH ACK or REC
			pmqtt->packet = (pmqtt->packet_qos == MQTT_QOS_ONCE) ? MQTT_PDU_PUBREC : MQTT_PDU_PUBACK;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			pmqtt->status = (pmqtt->packet_qos == MQTT_QOS_ONCE) ? _MQTT_STATUS_RETAINED :  _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_RETAINED:
			raise_user_error(_T("mqtt_send"), _T("invalid client status"));
			break;
		case _MQTT_STATUS_COMPELETE:
			//write PUBLISH COMP
			pmqtt->packet = MQTT_PDU_PUBCOMP;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			pmqtt->status = _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_RELEASE:
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("DISCONNECT has no payload"));
			}
			//write DISCONNECT PDU
			pmqtt->packet = MQTT_PDU_DISCONNECT;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			break;
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCU_PUB)
	{
		switch (pmqtt->status)
		{
		case _MQTT_STATUS_ASSOCIATE:
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("CONNECT must has payload"));
			}
			//write CONNECT PDU
			pmqtt->packet = MQTT_PDU_CONNECT;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to write payload
			break;
		case _MQTT_STATUS_WAITING:
			if (!pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("PUBLISH must has payload"));
			}
			//write PUBLISH PDU
			pmqtt->packet = MQTT_PDU_PUBLISH;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to send payload
			pmqtt->status = (pmqtt->packet_qos) ? _MQTT_STATUS_PENDING : _MQTT_STATUS_WAITING;
			break;
		case _MQTT_STATUS_PENDING:
			raise_user_error(_T("mqtt_send"), _T("invalid client status"));
			break;
		case _MQTT_STATUS_RETAINED:
			//write PUBLISH REL
			pmqtt->packet = MQTT_PDU_PUBREL;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to read publish comp
			pmqtt->status = _MQTT_STATUS_COMPELETE;
			break;
		case _MQTT_STATUS_COMPELETE:
			raise_user_error(_T("mqtt_send"), _T("invalid client status"));
			break;
		case _MQTT_STATUS_RELEASE:
			if (pdv_size)
			{
				raise_user_error(_T("mqtt_send"), _T("DISCONNECT has no payload"));
			}
			//write DISCONNECT PDU
			pmqtt->packet = MQTT_PDU_DISCONNECT;
			if (!_mqtt_write_pdu(pmqtt, pmqtt->packet, pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
			break;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t xmqtt_accept(xhand_t mqtt)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCP_SUB || pmqtt->type == _MQTT_TYPE_SCP_PUB || pmqtt->type == _MQTT_TYPE_SCP_UNK);

	byte_t* pdv_buf = NULL;
	dword_t pdv_size;
	byte_t pdu_type = 0;

	stream_t stm = NULL;

	TRY_CATCH;

	//read CONNECT PDU
	if (!xmqtt_recv(mqtt, &pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	stm = stream_alloc(pmqtt->pif);

	//read CONNECT payload
	pdv_buf = (byte_t*)xmem_alloc(pdv_size);
	if (!stream_read_bytes(stm, pdv_buf, &pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	//initialize or restore session
	if (!_mqtt_parse_session(pmqtt, pdv_buf, pdv_size))
	{
		raise_user_error(_T("mqtt_accept"), _T("invalid session"));
	}

	xmem_free(pdv_buf);
	pdv_buf = NULL;

	//write CONNECT ACK
	if (!xmqtt_send(mqtt, 0))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (pdv_buf)
		xmem_free(pdv_buf);

	if (stm)
		stream_free(stm);

	return 0;
}

bool_t xmqtt_connect(xhand_t mqtt)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCU_SUB || pmqtt->type == _MQTT_TYPE_SCU_PUB);

	byte_t* pdv_buf = NULL;
	dword_t pdv_size;
	byte_t pdu_type = 0;

	stream_t stm = NULL;

	TRY_CATCH;

	pdv_size = _mqtt_format_session(pmqtt, NULL, MAX_LONG);
	pdv_buf = (byte_t*)xmem_alloc(pdv_size);
	_mqtt_format_session(pmqtt, pdv_buf, pdv_size);

	//write CONNECT PDU
	if (!xmqtt_send(mqtt, pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	stm = stream_alloc(pmqtt->pif);

	//write CONNECT payload 
	if (!stream_write_bytes(stm, pdv_buf, pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	xmem_free(pdv_buf);
	pdv_buf = NULL;

	//read CONNECT ACK
	if (!xmqtt_recv(mqtt, &pdv_size))
	{
		raise_user_error(NULL, NULL);
	}
	
	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (pdv_buf)
		xmem_free(pdv_buf);

	if (stm)
		stream_free(stm);

	return 0;
}

bool_t xmqtt_subcribe(xhand_t mqtt, const tchar_t* topic, int len)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCU_SUB);

	byte_t* pdv_buf = NULL;
	dword_t pdv_size;

	stream_t stm = NULL;

	TRY_CATCH;

#ifdef _UNICODE
	pmqtt->topic_size = ucs_to_utf8(topic, len, NULL, MAX_LONG);
#else
	pmqtt->topic_size = mbs_to_utf8(topic, len, NULL, MAX_LONG);
#endif
	pmqtt->topic_name = (byte_t*)xmem_realloc(pmqtt->topic_name, pmqtt->topic_size);
#ifdef _UNICODE
	pmqtt->topic_size = ucs_to_utf8(topic, len, pmqtt->topic_name, pmqtt->topic_size);
#else
	pmqtt->topic_size = mbs_to_utf8(topic, len, pmqtt->topic_name, pmqtt->topic_size);
#endif

	pdv_size = _mqtt_format_subcribe(pmqtt, NULL, MAX_LONG);
	pdv_buf = (byte_t*)xmem_alloc(pdv_size);
	_mqtt_format_subcribe(pmqtt, pdv_buf, pdv_size);

	//ensure send SUBSCRIBE
	pmqtt->packet_qos = 1;

	stm = stream_alloc(pmqtt->pif);

	//write SUBSCRIBE PDU
	if (!xmqtt_send(mqtt, pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	//write SUBSCRIBE payload
	if (!stream_write_bytes(stm, pdv_buf, pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	xmem_free(pdv_buf);
	pdv_buf = NULL;

	//read SUBSCRIBE ACK
	if (!xmqtt_recv(mqtt, &pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;
	
	return 1;
ONERROR:

	if (pdv_buf)
		xmem_free(pdv_buf);

	if (stm)
		stream_free(stm);

	return 0;
}

bool_t xmqtt_unsubcribe(xhand_t mqtt, const tchar_t* topic, int len)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCU_SUB);

	byte_t* pdv_buf = NULL;
	dword_t pdv_size;

	stream_t stm = NULL;

	TRY_CATCH;

#ifdef _UNICODE
	pmqtt->topic_size = ucs_to_utf8(topic, len, NULL, MAX_LONG);
#else
	pmqtt->topic_size = mbs_to_utf8(topic, len, NULL, MAX_LONG);
#endif
	pmqtt->topic_name = (byte_t*)xmem_realloc(pmqtt->topic_name, pmqtt->topic_size);
#ifdef _UNICODE
	pmqtt->topic_size = ucs_to_utf8(topic, len, pmqtt->topic_name, pmqtt->topic_size);
#else
	pmqtt->topic_size = mbs_to_utf8(topic, len, pmqtt->topic_name, pmqtt->topic_size);
#endif

	pdv_size = _mqtt_format_subcribe(pmqtt, NULL, MAX_LONG);
	pdv_buf = (byte_t*)xmem_alloc(pdv_size);
	_mqtt_format_subcribe(pmqtt, pdv_buf, pdv_size);

	//ensure send unsubcribe
	pmqtt->packet_qos = 0;

	//write UNSUBSCRIBE PDU
	if (!xmqtt_send(mqtt, pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	stm = stream_alloc(pmqtt->pif);

	//write UNSUBSCRIBE payload
	if (!stream_write_bytes(stm, pdv_buf, pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	xmem_free(pdv_buf);
	pdv_buf = NULL;

	//read UNSUBSCRIBE ACK
	if (!xmqtt_recv(mqtt, &pdv_size))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;
ONERROR:

	if (pdv_buf)
		xmem_free(pdv_buf);

	if (stm)
		stream_free(stm);

	return 0;
}

bool_t xmqtt_poll_subscribe(xhand_t mqtt, tchar_t* topic, int len)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCP_SUB || pmqtt->type == _MQTT_TYPE_SCP_PUB);

#ifdef _UNICODE
	utf8_to_ucs(pmqtt->topic_name, pmqtt->topic_size, topic, len);
#else
	utf8_to_mbs(pmqtt->topic_name, pmqtt->topic_size, topic, len);
#endif

	return 1;
}

bool_t xmqtt_poll_message(xhand_t mqtt, byte_t** pbuf, dword_t* plen)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCU_SUB || pmqtt->type == _MQTT_TYPE_SCP_PUB || pmqtt->type == _MQTT_TYPE_SCP_SUB || pmqtt->type == _MQTT_TYPE_SCP_UNK);

	stream_t stm = NULL;
	dword_t n;

	TRY_CATCH;

	*pbuf = NULL;
	*plen = 0;

	stm = stream_alloc(pmqtt->pif);

	if (pmqtt->type == _MQTT_TYPE_SCU_SUB)
	{
		//read PUBLISH PDU
		if (!xmqtt_recv(mqtt, &(pmqtt->message_size)))
		{
			raise_user_error(NULL, NULL);
		}

		pmqtt->message_data = (byte_t*)xmem_realloc(pmqtt->message_data, pmqtt->message_size);

		//read PUBLISH payload
		if (!stream_read_bytes(stm, pmqtt->message_data, &(pmqtt->message_size)))
		{
			raise_user_error(NULL, NULL);
		}

		if (pmqtt->packet_qos)
		{
			//write PUBLISH ACK
			if (!xmqtt_send(mqtt, 0))
			{
				raise_user_error(NULL, NULL);
			}
		}

		if (pmqtt->packet_qos == MQTT_QOS_ONCE)
		{
			//read PUBLISH REL
			if (!xmqtt_recv(mqtt, &n))
			{
				raise_user_error(NULL, NULL);
			}

			//write PUBLISH COMP
			if (!xmqtt_send(mqtt, 0))
			{
				raise_user_error(NULL, NULL);
			}
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCP_PUB)
	{
		//read PUBLISH PDU
		if (!xmqtt_recv(mqtt, &(pmqtt->message_size)))
		{
			raise_user_error(NULL, NULL);
		}

		pmqtt->message_data = (byte_t*)xmem_realloc(pmqtt->message_data, pmqtt->message_size);

		//read PUBLISH payload
		if (!stream_read_bytes(stm, pmqtt->message_data, &(pmqtt->message_size)))
		{
			raise_user_error(NULL, NULL);
		}

		if (pmqtt->packet_qos)
		{
			//write PUBLISH ACK
			if (!xmqtt_send(mqtt, 0))
			{
				raise_user_error(NULL, NULL);
			}
		}

		if (pmqtt->packet_qos == MQTT_QOS_ONCE)
		{
			//read PUBLISH REL
			if (!xmqtt_recv(mqtt, &n))
			{
				raise_user_error(NULL, NULL);
			}

			//write PUBLISH COMP
			if (!xmqtt_send(mqtt, 0))
			{
				raise_user_error(NULL, NULL);
			}
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCP_SUB || pmqtt->type == _MQTT_TYPE_SCP_UNK)
	{
		//read SUBSCRIBE or UNSUBSCRIBE PDU
		if (!xmqtt_recv(mqtt, &(pmqtt->message_size)))
		{
			raise_user_error(NULL, NULL);
		}

		pmqtt->message_data = (byte_t*)xmem_realloc(pmqtt->message_data, pmqtt->message_size);

		//read SUBSCRIBE or UNSUBSCRIBE payload
		if (!stream_read_bytes(stm, pmqtt->message_data, &(pmqtt->message_size)))
		{
			raise_user_error(NULL, NULL);
		}

		//_MQTT_TYPE_SCP_UNK->_MQTT_TYPE_SCP_PUB
		if (pmqtt->type == _MQTT_TYPE_SCP_PUB)
		{
			if (pmqtt->packet_qos)
			{
				//write PUBLISH ACK
				if (!xmqtt_send(mqtt, 0))
				{
					raise_user_error(NULL, NULL);
				}
			}

			if (pmqtt->packet_qos == MQTT_QOS_ONCE)
			{
				//read PUBLISH REL
				if (!xmqtt_recv(mqtt, &n))
				{
					raise_user_error(NULL, NULL);
				}

				//write PUBLISH COMP
				if (!xmqtt_send(mqtt, 0))
				{
					raise_user_error(NULL, NULL);
				}
			}
		}
		else
		{
			if (!_mqtt_parse_subcribe(pmqtt, pmqtt->message_data, pmqtt->message_size))
			{
				raise_user_error(NULL, NULL);
			}

			xmem_free(pmqtt->message_data);
			pmqtt->message_data = NULL;
			pmqtt->message_size = 0;

			//write SUBSCRIBE or UNSUBSCRIBE ACK
			if (!xmqtt_send(mqtt, 0))
			{
				raise_user_error(NULL, NULL);
			}
		}
	}

	stream_free(stm);
	stm = NULL;

	*pbuf = pmqtt->message_data;
	*plen = pmqtt->message_size;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (stm)
		stream_free(stm);

	return 0;
}

bool_t xmqtt_publish(xhand_t mqtt, const tchar_t* topic, int len)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCU_PUB || pmqtt->type == _MQTT_TYPE_SCP_SUB);

#ifdef _UNICODE
	pmqtt->topic_size = ucs_to_utf8(topic, len, NULL, MAX_LONG);
#else
	pmqtt->topic_size = mbs_to_utf8(topic, len, NULL, MAX_LONG);
#endif
	pmqtt->topic_name = (byte_t*)xmem_realloc(pmqtt->topic_name, pmqtt->topic_size);
#ifdef _UNICODE
	pmqtt->topic_size = ucs_to_utf8(topic, len, pmqtt->topic_name, pmqtt->topic_size);
#else
	pmqtt->topic_size = mbs_to_utf8(topic, len, pmqtt->topic_name, pmqtt->topic_size);
#endif

	return 1;
}

bool_t xmqtt_push_message(xhand_t mqtt, const byte_t* buf, dword_t len)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	XDL_ASSERT(pmqtt->type == _MQTT_TYPE_SCU_PUB || pmqtt->type == _MQTT_TYPE_SCP_SUB);

	dword_t pdv_size;

	stream_t stm = NULL;

	TRY_CATCH;

	stm = stream_alloc(pmqtt->pif);

	if (pmqtt->type == _MQTT_TYPE_SCU_PUB)
	{
		//write PUBLISH PDU
		if (!xmqtt_send(mqtt, len))
		{
			raise_user_error(NULL, NULL);
		}

		//write PUBLISH payload
		if (!stream_write_bytes(stm, buf, len))
		{
			raise_user_error(NULL, NULL);
		}

		if (pmqtt->packet_qos)
		{
			//read PUBLISH ACK
			if (!xmqtt_recv(mqtt, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
		}

		if (pmqtt->packet_qos == MQTT_QOS_ONCE)
		{
			//write PUBLISH REL
			if (!xmqtt_send(mqtt, 0))
			{
				raise_user_error(NULL, NULL);
			}
			//read PUBLISH COMP
			if (!xmqtt_recv(mqtt, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
		}
	}
	else if (pmqtt->type == _MQTT_TYPE_SCP_SUB)
	{
		//write PUBLISH PDU
		if (!xmqtt_send(mqtt, len))
		{
			raise_user_error(NULL, NULL);
		}

		//write PUBLISH payload
		if (!stream_write_bytes(stm, buf, len))
		{
			raise_user_error(NULL, NULL);
		}

		if (pmqtt->packet_qos)
		{
			//read PUBLISH ACK
			if (!xmqtt_recv(mqtt, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
		}

		if (pmqtt->packet_qos == MQTT_QOS_ONCE)
		{
			//write PUBLISH REL
			if (!xmqtt_send(mqtt, 0))
			{
				raise_user_error(NULL, NULL);
			}
			//read PUBLISH COMP
			if (!xmqtt_recv(mqtt, &pdv_size))
			{
				raise_user_error(NULL, NULL);
			}
		}
	}
	
	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (stm)
		stream_free(stm);

	return 0;
}

void xmqtt_close(xhand_t mqtt)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	if (pmqtt->type == _MQTT_TYPE_SCU_PUB && pmqtt->status == _MQTT_STATUS_WAITING)
	{
		//send DISCONNECT PDU
		pmqtt->status = _MQTT_STATUS_RELEASE;

		xmqtt_send(mqtt, 0);
	}

	_mqtt_clean_session(pmqtt);

	if (pmqtt->pif)
		xmem_free(pmqtt->pif);

	xmem_free(mqtt);
}

void xmqtt_set_packet_ctrl(xhand_t mqtt, const MQTT_PACKET_CTRL* pmc)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	pmqtt->packet_pid = pmc->packet_pid;
	pmqtt->packet_dup = pmc->packet_dup;
	pmqtt->packet_qos = pmc->packet_qos;
	pmqtt->packet_ret = pmc->packet_ret;
}

void xmqtt_get_packet_ctrl(xhand_t mqtt, MQTT_PACKET_CTRL* pmc)
{
	mqtt_t* pmqtt = TypePtrFromHead(mqtt_t, mqtt);

	XDL_ASSERT(mqtt && mqtt->tag == _HANDLE_MQTT);

	pmc->packet_pid = pmqtt->packet_pid;
	pmc->packet_dup = pmqtt->packet_dup;
	pmc->packet_qos = pmqtt->packet_qos;
	pmc->packet_ret = pmqtt->packet_ret;
}