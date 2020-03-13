/***********************************************************************
	Easily RadNet 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc rednet defination document

	@module	radnet.h | rad network definition interface file

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


#ifndef _RADNET_H
#define	_RADNET_H

#include "raddef.h"

#define RADNET_HDR_SIZE			8
#define RADNET_COMMAND_HDR_SIZE	8
#define RADNET_DATASET_HDR_SIZE	8

/*RADNET Current Version*/
#define RADNET_VER				0x11
#define RADNET_PROTO_NAME		"RADNET"

/*RADNET Control Packet*/
#define RADNET_PDU_ERR			0
#define RADNET_PDU_CONNECT		1
#define RADNET_PDU_DISCONN		2
#define RADNET_PDU_COMMAND		3
#define RADNET_PDU_DATASET		4

/*RADNET Command Type*/
#define RADNET_COMMAND_INSERT	1
#define RADNET_COMMAND_UPDATE	2
#define RADNET_COMMAND_DELETE	3
#define RADNET_COMMAND_TRUNCA	4
#define RADNET_COMMAND_SELECT	5

/*RADNET Error Code*/
#define RADNET_ERROR_SUCCEED	0
#define RADNET_ERROR_SUCCEED_TEXT	_T("operation succeed")
#define RADNET_ERROR_OPEN		1
#define RADNET_ERROR_OPEN_TEXT		_T("open databse failed")
#define RADNET_ERROR_INSERT		2
#define RADNET_ERROR_INSERT_TEXT	_T("insert operation failed")
#define RADNET_ERROR_UPDATE		3
#define RADNET_ERROR_UPDATE_TEXT	_T("update operation failed")
#define RADNET_ERROR_TRUNCA		4
#define RADNET_ERROR_TRUNCA_TEXT	_T("truncate operation failed")
#define RADNET_ERROR_SELECT		5
#define RADNET_ERROR_SELECT_TEXT	_T("select operation failed")
#define RADNET_ERROR_DELETE		6
#define RADNET_ERROR_DELETE_TEXT	_T("delete operation failed")

/*RADNET Status*/
typedef enum{
	_RADNET_STATUS_ASSOCIATE = 0,
	_RADNET_STATUS_WAITING = 1,
	_RADNET_STATUS_RELEASE = 2
}RADNET_STATUS;

/*RADNET Type*/
typedef enum{
	_RADNET_TYPE_SCU = 1,
	_RADNET_TYPE_SCP = 2
}RADNET_TYPE;

typedef struct _radnet_t{
	int type;
	xhand_t bio;

	int encode;
	int status;

	tchar_t dbname[PATH_LEN];

	int errcode;
	tchar_t errtext[ERR_LEN];
}radnet_t;

typedef struct _radnet_pdv_head_t{
	dword_t size;
	byte_t type;
}radnet_pdv_head_t;

#ifdef	__cplusplus
EXP_API "C" {
#endif

EXP_API radnet_t* radnet_scu(xhand_t bio);

EXP_API radnet_t* radnet_scp(xhand_t bio);

EXP_API void radnet_close(radnet_t* radnet);

EXP_API void radnet_abort(radnet_t* radnet, int errcode);

EXP_API int radnet_status(radnet_t* radnet);

EXP_API bool_t radnet_connect(radnet_t* radnet, const tchar_t* dbname);

EXP_API bool_t radnet_accept(radnet_t* radnet);

EXP_API bool_t radnet_recv(radnet_t* radnet, radnet_pdv_head_t* pdv);

EXP_API bool_t radnet_send(radnet_t* radnet, const radnet_pdv_head_t* pdv);

EXP_API bool_t radnet_set(radnet_t* radnet, variant_t key, object_t val);

EXP_API bool_t radnet_get(radnet_t* radnet, variant_t key, object_t val);

EXP_API bool_t radnet_del(radnet_t* radnet, variant_t key);

#ifdef	__cplusplus
}
#endif


#endif	/* _RADNET_H */

