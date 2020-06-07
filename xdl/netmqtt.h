/***********************************************************************
	Easilyxdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl mqtt document

	@module	mqttnet.h | xdl mqtt interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#ifndef _NETMQTT_H
#define _NETMQTT_H

#include "xdldef.h"

/*MQTT Current Version*/
#define MQTT_VER				0x0301
#define MQTT_PROTO_NAME			"MQIsdp"
#define MQTT_PROTO_LEVEL		0x03

/*MQTT Control Packet*/
#define MQTT_PDU_CONNECT		1
#define MQTT_PDU_CONNACK		2
#define MQTT_PDU_PUBLISH		3
#define MQTT_PDU_PUBACK			4
#define MQTT_PDU_PUBREC			5
#define MQTT_PDU_PUBREL			6
#define MQTT_PDU_PUBCOMP		7
#define MQTT_PDU_SUBSCRIBE		8
#define MQTT_PDU_SUBACK			9
#define MQTT_PDU_UNSUBSCRIBE	10
#define MQTT_PDU_UNSUBACK		11
#define MQTT_PDU_PINGREQ		12
#define MQTT_PDU_PINGRESP		13
#define MQTT_PDU_DISCONNECT		14

/*MQTT Message Flags*/
#define MQTT_MSK_USERNAME		0x80
#define MQTT_MSK_PASSWORD		0x40
#define MQTT_MSK_WILL_RETAIN	0x20
#define MQTT_MSK_WILL_QOS		(0x10 | 0x08)
#define MQTT_MSK_WILL_FLAG		0x04
#define MQTT_MSK_CLEAN_SESSION	0x02

#define MQTT_MSK_RETFLAG		0x01
#define MQTT_MSK_QOSFLAG		0x06
#define MQTT_MSK_DUPFLAG		0x08

/*MQTT Qos Type*/
#define MQTT_QOS_NONE			0
#define MQTT_QOS_ONEMORE		1
#define MQTT_QOS_ONCE			2

/*MQTT Config Options*/
#define MQTT_OPT_TITLE			1
#define MQTT_OPT_TOPIC			1

/*MQTT Return Code*/
#define MQTT_RET_ACCEPTED				0x00 //Connection Accepted
#define MQTT_RET_INVALID_PROTOCOL		0x01 //Connection Refused, unacceptable protocol version
#define MQTT_RET_INVALID_IDENTIFIER		0x02 //Connection Refused, identifier rejected
#define MQTT_RET_SERVER_UNAVALIABLE		0x03 //Connection Refused, Server unavailable
#define MQTT_RET_INVALID_USER			0x04 //Connection Refused, bad user name or password
#define MQTT_RET_UNAUTHROIZED			0x05 //Connection Refused, not authorized

/*MQTT Status*/
typedef enum{
	_MQTT_STATUS_ASSOCIATE = 0,
	_MQTT_STATUS_SLEEPING = 1,
	_MQTT_STATUS_WAITING = 2,
	_MQTT_STATUS_PENDING = 3,
	_MQTT_STATUS_RETAINED = 4,
	_MQTT_STATUS_COMPELETE = 5,
	_MQTT_STATUS_RELEASE = 6
}MQTT_STATUS;

/*MQTT Pcket Variable Size*/
#define MQTT_PROTOCOL_SIZE		6
#define MQTT_IDENTIFY_SIZE		23
#define MQTT_ALIVE_TIME			300000
#define MQTT_PDU_SIZE_MAXINUM	268435455 /*256M*/

/*MQTT type*/
typedef enum{
	_MQTT_TYPE_SCP_UNK = 0,
	_MQTT_TYPE_SCU_PUB = 1,
	_MQTT_TYPE_SCP_PUB = 2,
	_MQTT_TYPE_SCU_SUB = 3,
	_MQTT_TYPE_SCP_SUB = 4
}MQTT_TYPE;

/*MQTT packet info*/
typedef struct _MQTT_PACKET_CTRL{
	sword_t packet_pid;
	bool_t	packet_dup;
	int		packet_qos;
	bool_t	packet_ret;
}MQTT_PACKET_CTRL;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API xhand_t	xmqtt_scu(xhand_t bio, int scu);

	EXP_API xhand_t	xmqtt_scp(xhand_t bio, int scp);

	EXP_API int xmqtt_type(xhand_t mqtt);

	EXP_API xhand_t	xmqtt_bio(xhand_t mqtt);

	EXP_API void xmqtt_close(xhand_t mqtt);

	EXP_API int xmqtt_status(xhand_t mqtt);

	EXP_API bool_t xmqtt_connect(xhand_t mqtt);

	EXP_API bool_t xmqtt_accept(xhand_t mqtt);

	EXP_API bool_t xmqtt_subcribe(xhand_t mqtt, const tchar_t* topic, int len);

	EXP_API bool_t xmqtt_unsubcribe(xhand_t mqtt, const tchar_t* topic, int len);

	EXP_API bool_t xmqtt_poll_subscribe(xhand_t mqtt, tchar_t* topic, int len);

	EXP_API bool_t xmqtt_poll_message(xhand_t mqtt, byte_t** pbuf, dword_t* plen);

	EXP_API bool_t xmqtt_publish(xhand_t mqtt, const tchar_t* topic, int len);

	EXP_API bool_t xmqtt_push_message(xhand_t mqtt, const byte_t* buf, dword_t len);

	EXP_API void xmqtt_set_packet_ctrl(xhand_t mqtt, const MQTT_PACKET_CTRL* pmc);

	EXP_API void xmqtt_get_packet_ctrl(xhand_t mqtt, MQTT_PACKET_CTRL* pmc);

#ifdef	__cplusplus
}
#endif


#endif /*NETMQTT_H*/