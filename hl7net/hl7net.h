/***********************************************************************
	Easily HL7 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hl7 defination document

	@module	hl7net.h | hl7 network definition interface file

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


#ifndef _HL7NET_H
#define	_HL7NET_H

#include <xdl.h>

#if defined(_OS_WINDOWS) && defined(_USRDLL)
#define HL7NET_API __declspec(dllexport)
#else
#define HL7NET_API extern
#endif

/*HL7 Current Version*/
#define HL7NET_VER				0x0703

/*HL7 Control Packet*/
#define HL7NET_PDU_ERROR		0
#define HL7NET_PDU_CONNECT		1
#define HL7NET_PDU_CONNACK		2
#define HL7NET_PDU_PUBLISH		3
#define HL7NET_PDU_PUBACK		4
#define HL7NET_PDU_SUBSCRIBE	8
#define HL7NET_PDU_SUBACK		9
#define HL7NET_PDU_MESSAGE		10
#define HL7NET_PDU_MSGACK		11

/*HL7 Error Code*/
#define HL7NET_ERROR_SUCCEED	0
#define HL7NET_ERROR_SUCCEED_TEXT	_T("operation succeed")
#define HL7NET_ERROR_INVALIDUSER		1
#define HL7NET_ERROR_INVALIDUSER_TEXT	_T("invalid user name or password")
#define HL7NET_ERROR_INVALIDTOPIC		2
#define HL7NET_ERROR_INVALIDTOPIC_TEXT	_T("invalid topic")

/*HL7 Status*/
typedef enum{
	_HL7NET_STATUS_ASSOCIATE = 0,
	_HL7NET_STATUS_SLEEPING = 1,
	_HL7NET_STATUS_WAITING = 2,
	_HL7NET_STATUS_PENDING = 3,
	_HL7NET_STATUS_RELEASE = 4
}HL7NET_STATUS;

/*HL7 type*/
typedef enum{
	_HL7NET_TYPE_SCU_PUB = 1,
	_HL7NET_TYPE_SCP_PUB = 2,
	_HL7NET_TYPE_SCU_SUB = 3,
	_HL7NET_TYPE_SCP_SUB = 4
}HL7NET_TYPE;

#define HL7NET_PDU_HDR_SIZE		8
#define HL7NET_PDV_HDR_SIZE		8

typedef struct _hl7net_t{
	int type;
	xhand_t bio;

	int status;	/*connect status*/
	int encode; /*current pdu encode*/

	dword_t request_bytes;
	dword_t response_bytes;

	/*session*/
	sword_t session_ver;
	sword_t session_len;
	byte_t*	session_cid;

	int username_length;
	tchar_t* username;

	int password_length;
	tchar_t* password;

	int topic_length;
	tchar_t* topic_name;

	int error_code;
	int error_length;
	tchar_t* error_text;
}hl7net_t;

#define HL7NET_PDV_MSH		"MSH" //MessageHeader
#define HL7NET_PDV_MSA		"MSA" //MessageAcknowledgment
#define HL7NET_PDV_PID		"PID" //PatientIdentification
#define HL7NET_PDV_OBR		"OBR" //ObservationRequest
#define HL7NET_PDV_OBX		"OBX" //Observation
#define HL7NET_PDV_QRD		"QRD" //QueryDefinition
#define HL7NET_PDV_QRF		"QRF" //QueryFilter
#define HL7NET_PDV_ERR		"ERR" //Error
#define HL7NET_PDV_QAK		"QAK" //QueryAcknowledgment
#define HL7NET_PDV_DSP		"DSP" //DisplayData
#define HL7NET_PDV_DSC		"DSC" //ContinuationPointer
#define HL7NET_PDV_EVN		"EVN" //Event

typedef struct _hl7net_pdv_head_t{
	byte_t type[4];
	dword_t size;
}hl7net_pdv_head_t;

#ifdef	__cplusplus
extern "C" {
#endif

	HL7NET_API hl7net_t* hl7net_scu(xhand_t bio, int scu);

	HL7NET_API hl7net_t* hl7net_scp(xhand_t bio, int scp);

	HL7NET_API xhand_t	hl7net_bio(hl7net_t* hl7);

	HL7NET_API void hl7net_close(hl7net_t* hl7);

	HL7NET_API int hl7net_status(hl7net_t* hl7);

	HL7NET_API bool_t hl7net_connect(hl7net_t* hl7);

	HL7NET_API bool_t hl7net_accept(hl7net_t* hl7);

	HL7NET_API void hl7net_abort(hl7net_t* hl7, int errcode);

	HL7NET_API bool_t hl7net_subscribe(hl7net_t* hl7, const tchar_t* topic, int len);

	HL7NET_API bool_t hl7net_poll_subscribe(hl7net_t* hl7, tchar_t* topic, int len);

	HL7NET_API bool_t hl7net_publish(hl7net_t* hl7, const tchar_t* topic, int len);

	HL7NET_API bool_t hl7net_poll_publish(hl7net_t* hl7, tchar_t* topic, int len);

	HL7NET_API bool_t hl7net_poll_message(hl7net_t* hl7, hl7net_pdv_head_t* pdv);

	HL7NET_API bool_t hl7net_push_message(hl7net_t* hl7, const hl7net_pdv_head_t* pdv);

	HL7NET_API void hl7net_set_response_bytes(hl7net_t* hl7, dword_t size);

	HL7NET_API dword_t hl7net_get_response_bytes(hl7net_t* hl7);

	HL7NET_API dword_t hl7net_get_request_bytes(hl7net_t* hl7);

	HL7NET_API void hl7net_set_request_bytes(hl7net_t* hl7, dword_t size);

	HL7NET_API dword_t hl7net_pdv_group_size(hl7net_pdv_head_t* pdv, int n);

#ifdef	__cplusplus
}
#endif




#endif	/* _HL7NET_H */

