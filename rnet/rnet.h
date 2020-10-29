/***********************************************************************
	Easily RadNet 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc rednet defination document

	@module	rnet.h | rad network definition interface file

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


#ifndef _RNET_H
#define	_RNET_H

#include "rdef.h"

#define RNET_HDR_SIZE			8
#define RNET_COMMAND_HDR_SIZE	8
#define RNET_DATASET_HDR_SIZE	8

/*RNET Current Version*/
#define RNET_VER				0x11
#define RNET_PROTO_NAME		"RNET"

/*RNET Control Packet*/
#define RNET_PDU_ERR			0
#define RNET_PDU_CONNECT		1
#define RNET_PDU_DISCONN		2
#define RNET_PDU_COMMAND		3
#define RNET_PDU_DATASET		4

/*RNET Command Type*/
#define RNET_COMMAND_INSERT	1
#define RNET_COMMAND_UPDATE	2
#define RNET_COMMAND_DELETE	3
#define RNET_COMMAND_TRUNCA	4
#define RNET_COMMAND_SELECT	5

/*RNET Error Code*/
#define RNET_ERROR_SUCCEED	0
#define RNET_ERROR_SUCCEED_TEXT	_T("operation succeed")
#define RNET_ERROR_OPEN		1
#define RNET_ERROR_OPEN_TEXT		_T("open databse failed")
#define RNET_ERROR_INSERT		2
#define RNET_ERROR_INSERT_TEXT	_T("insert operation failed")
#define RNET_ERROR_UPDATE		3
#define RNET_ERROR_UPDATE_TEXT	_T("update operation failed")
#define RNET_ERROR_TRUNCA		4
#define RNET_ERROR_TRUNCA_TEXT	_T("truncate operation failed")
#define RNET_ERROR_SELECT		5
#define RNET_ERROR_SELECT_TEXT	_T("select operation failed")
#define RNET_ERROR_DELETE		6
#define RNET_ERROR_DELETE_TEXT	_T("delete operation failed")

/*RNET Status*/
typedef enum{
	_RNET_STATUS_ASSOCIATE = 0,
	_RNET_STATUS_WAITING = 1,
	_RNET_STATUS_RELEASE = 2
}RNET_STATUS;

/*RNET Type*/
typedef enum{
	_RNET_TYPE_SCU = 1,
	_RNET_TYPE_SCP = 2
}RNET_TYPE;

typedef struct _rnet_t{
	int type;

	if_bio_t* pbo;

	int encode;
	int status;

	tchar_t dbname[PATH_LEN + 1];

	int errcode;
	tchar_t errtext[ERR_LEN + 1];
}rnet_t;

typedef struct _rnet_pdv_head_t{
	dword_t size;
	byte_t type;
}rnet_pdv_head_t;

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API rnet_t* rnet_scu(xhand_t bio);

EXP_API rnet_t* rnet_scp(xhand_t bio);

EXP_API void rnet_close(rnet_t* rnet);

EXP_API void rnet_abort(rnet_t* rnet, int errcode);

EXP_API int rnet_status(rnet_t* rnet);

EXP_API bool_t rnet_connect(rnet_t* rnet, const tchar_t* dbname);

EXP_API bool_t rnet_accept(rnet_t* rnet);

EXP_API bool_t rnet_recv(rnet_t* rnet, rnet_pdv_head_t* pdv);

EXP_API bool_t rnet_send(rnet_t* rnet, const rnet_pdv_head_t* pdv);

EXP_API bool_t rnet_set(rnet_t* rnet, variant_t key, object_t val);

EXP_API bool_t rnet_get(rnet_t* rnet, variant_t key, object_t val);

EXP_API bool_t rnet_del(rnet_t* rnet, variant_t key);

#ifdef	__cplusplus
}
#endif


#endif	/* _RNET_H */

