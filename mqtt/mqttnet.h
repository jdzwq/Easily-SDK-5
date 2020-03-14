/***********************************************************************
	Easily MQTT v1.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl mqtt document

	@module	mqttnet.h | xdl mqtt interface file

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

#ifndef _MQTTNET_H
#define _MQTTNET_H

#include "mqttdef.h"

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
	_MQTT_TYPE_SCU_PUB = 1,
	_MQTT_TYPE_SCP_PUB = 2,
	_MQTT_TYPE_SCU_SUB = 3,
	_MQTT_TYPE_SCP_SUB = 4
}MQTT_TYPE;

typedef struct _mqtt_t{
	int type;
	xhand_t bio;

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

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API mqtt_t*	mqtt_scu(xhand_t bio, int scu);

	EXP_API mqtt_t*	mqtt_scp(xhand_t bio, int scp);

	EXP_API xhand_t	mqtt_bio(mqtt_t* mqtt);

	EXP_API void mqtt_close(mqtt_t* mqtt);

	EXP_API int mqtt_status(mqtt_t* mqtt);

	EXP_API bool_t mqtt_connect(mqtt_t* mqtt);

	EXP_API bool_t mqtt_accept(mqtt_t* mqtt);

	EXP_API bool_t mqtt_subcribe(mqtt_t* mqtt, const tchar_t* topic, int len);

	EXP_API bool_t mqtt_unsubcribe(mqtt_t* mqtt, const tchar_t* topic, int len);

	EXP_API bool_t mqtt_poll_subscribe(mqtt_t* mqtt, tchar_t* topic, int len);

	EXP_API bool_t mqtt_poll_message(mqtt_t* mqtt, byte_t** pbuf, dword_t* plen);

	EXP_API bool_t mqtt_publish(mqtt_t* mqtt, const tchar_t* topic, int len);

	EXP_API bool_t mqtt_push_message(mqtt_t* mqtt, const byte_t* buf, dword_t len);

#ifdef	__cplusplus
}
#endif


#endif /*MQTTNET_H*/