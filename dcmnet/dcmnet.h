/***********************************************************************
	Easily DCM v1.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl dcm document

	@module	dcmnet.h | xdl dcm interface file

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

#ifndef _DCMNET_H
#define _DCMNET_H

#include "dcmdef.h"

/*dcm pdu type*/
#define DCM_PDU_ASSOCIATE_RQ	0x01
#define DCM_PDU_ASSOCIATE_AC	0x02
#define DCM_PDU_ASSOCIATE_RJ	0x03
#define DCM_PDU_DATA_TF 		0x04
#define DCM_PDU_RELEASE_RQ		0x05
#define DCM_PDU_RELEASE_RP		0x06
#define DCM_PDU_ABORT_RQ		0x07

#define DCM_CMD_C_ECHO_RQ		0x0030
#define DCM_CMD_C_ECHO_RSP		0x8030

#define DCM_PDV_SIZE_FROM_SET(dw)	(dw + 2)
#define DCM_PDU_SIZE_FROM_PDV(dw)	(dw + 4)

/*dcm options*/
#define DCM_OPT_AET_SCP			1
#define DCM_OPT_AET_SCU			2
#define DCM_OPT_SYNTAX_ABSTRACT	3
#define DCM_OPT_SYNTAX_TRANSFER	4
#define DCM_OPT_DATA_MAXINUM	5
#define DCM_OPT_CONTEXT_APPLICATION	6
#define DCM_OPT_CONTEXT_IMPLEMENT	7

#define DCM_USER_DATA_MAXINUM	16384

#define SOP_CONTEXT_IMPLEMENT	"1.2.276.0.7230010.3.0.3.6.2" 
#define SOP_CONTEXT_APPLICATION	"1.2.840.10008.3.1.1.1" 
#define SOP_SYNTAX_ABSTRACT		"1.2.840.10008.1.1"
#define SOP_SYNTAX_TRANSFER		"1.2.840.10008.1.2"

typedef enum{
	_DCM_STATUS_ASSOCIATE = 0,
	_DCM_STATUS_TRANSF = 1,
	_DCM_STATUS_PENDING = 2,
	_DCM_STATUS_RELEASE = 3
}DCM_STATUS;

typedef enum{
	_ASSOCIATE_ACCEPT = 0,
	_ASSOCIATE_USER_REJECT = 1,
	_ASSOCIATE_UNKNOWN = 2,
	_ASSOCIATE_ABSTRACT_SYNTAX_NOT_SUPPORT = 3,
	_ASSOCIATE_TRANSFER_SYNTAX_NOT_SUPPORT = 4
}DCM_ASSOCIATE_STATE;

typedef enum{
	_ASSOCIATE_SOURCE_SCU = 1,
	_ASSOCIATE_SOURCE_SCP_PDV = 2,
	_ASSOCIATE_SOURCE_SCP_PDU = 3
}DCM_ASSOCIATE_SOURCE;

typedef enum{
	_ASSOCIATE_REASON_NONE = 1,
	_ASSOCIATE_REASON_APPLICATION_CONTEXT_NAME_NOT_SUPPORT = 2,
	_ASSOCIATE_REASON_CALLING_AP_TITLE_NOT_RECONGNIZED = 3,
	_ASSOCIATE_REASON_CALLED_AP_TITLE_NOT_RECONGNIZED = 7
}DCM_ASSOCIATE_REASON;

typedef enum{
	_REJECTED_PERMANENT = 1,
	_REJECTED_TEMPORARY = 2
}DCM_REJECTED_STATE;


typedef enum{
	_DCM_TYPE_SCU = 1,
	_DCM_TYPE_SCP = 2
}DCM_TYPE;

typedef struct _dcm_pdv_head_t{
	dword_t size;
	byte_t did;
	byte_t ctl;
}dcm_pdv_head_t;

typedef struct _dcm_t{
	int type;
	xhand_t bio;

	bool_t status;		/*connect status*/
	dword_t n_request;	/*request pdu bytes*/
	dword_t n_response;	/*response pdu bytes*/

	sword_t ver;	/*version*/
	byte_t scp[16];	/*Called App title*/
	byte_t scu[16];	/*Calling Ap title*/
	byte_t ret[32];	/*retain*/

	byte_t ack[1];	/*associate ack*/
	byte_t src[1];	/*associate source*/
	byte_t dag[1];	/*associate diagnose*/

	dword_t udm;	/*User Data Maximum Length*/

	schar_t* iid;	/*Implement Context Item Text*/
	schar_t* uid;	/*Application Context Item Text*/
	schar_t* asn;	/*Abstract Syntax*/
	schar_t* tsn;	/*Transfer Syntax*/
}dcm_t;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API dcm_t*	dcm_scu(xhand_t bio);

	EXP_API dcm_t*	dcm_scp(xhand_t bio);

	EXP_API xhand_t	dcm_bio(dcm_t* dcm);

	EXP_API void dcm_close(dcm_t* dcm);

	EXP_API int dcm_status(dcm_t* dcm);

	EXP_API bool_t dcm_send(dcm_t* dcm, dcm_pdv_head_t* pdv);

	EXP_API bool_t dcm_recv(dcm_t* dcm, dcm_pdv_head_t* pdv);

	EXP_API void dcm_set_response_bytes(dcm_t* dcm, dword_t size);

	EXP_API dword_t dcm_get_response_bytes(dcm_t* dcm);

	EXP_API dword_t dcm_get_request_bytes(dcm_t* dcm);

	EXP_API void dcm_set_request_bytes(dcm_t* dcm, dword_t size);

	EXP_API dword_t dcm_pdv_group_size(dcm_pdv_head_t* pdv, int n);

	EXP_API dword_t dcm_get_options(dcm_t* dcm, int opt, void* buf, dword_t max);

	EXP_API void dcm_set_options(dcm_t* dcm, int opt, void* buf, dword_t len);

#ifdef	__cplusplus
}
#endif


#endif /*DCMNET_H*/