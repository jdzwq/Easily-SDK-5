/***********************************************************************
	Easily RadHex 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc redhex defination document

	@module	radnet.h | radhex network definition interface file

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

#define RADHEX_HDR_SIZE			8
#define RADHEX_COMMAND_HDR_SIZE	8
#define RADHEX_DATASET_HDR_SIZE	8

/*RADHEX Current Version*/
#define RADHEX_VER				0x11
#define RADHEX_PROTO_NAME		"RADHEX"

/*RADHEX Control Packet*/
#define RADHEX_PDU_ERR			0
#define RADHEX_PDU_CONNECT		1
#define RADHEX_PDU_DISCONN		2
#define RADHEX_PDU_COMMAND		3
#define RADHEX_PDU_DATASET		4

/*RADHEX Command Type*/
#define RADHEX_COMMAND_INSERT	1
#define RADHEX_COMMAND_UPDATE	2
#define RADHEX_COMMAND_DELETE	3
#define RADHEX_COMMAND_TRUNCA	4
#define RADHEX_COMMAND_SELECT	5

/*RADHEX Error Code*/
#define RADHEX_ERROR_SUCCEED	0
#define RADHEX_ERROR_SUCCEED_TEXT	_T("operation succeed")
#define RADHEX_ERROR_OPEN		1
#define RADHEX_ERROR_OPEN_TEXT		_T("open databse failed")
#define RADHEX_ERROR_INSERT		2
#define RADHEX_ERROR_INSERT_TEXT	_T("insert operation failed")
#define RADHEX_ERROR_UPDATE		3
#define RADHEX_ERROR_UPDATE_TEXT	_T("update operation failed")
#define RADHEX_ERROR_TRUNCA		4
#define RADHEX_ERROR_TRUNCA_TEXT	_T("truncate operation failed")
#define RADHEX_ERROR_SELECT		5
#define RADHEX_ERROR_SELECT_TEXT	_T("select operation failed")
#define RADHEX_ERROR_DELETE		6
#define RADHEX_ERROR_DELETE_TEXT	_T("delete operation failed")

/*RADHEX Status*/
typedef enum{
	_RADHEX_STATUS_ASSOCIATE = 0,
	_RADHEX_STATUS_WAITING = 1,
	_RADHEX_STATUS_RELEASE = 2
}RADHEX_STATUS;

/*RADHEX Type*/
typedef enum{
	_RADHEX_TYPE_SCU = 1,
	_RADHEX_TYPE_SCP = 2
}RADHEX_TYPE;

typedef struct _radhex_t{
	int type;
	xhand_t bio;

	int encode;
	int status;

	tchar_t dbname[PATH_LEN];

	int errcode;
	tchar_t errtext[ERR_LEN];
}radhex_t;

typedef struct _radhex_pdv_head_t{
	dword_t size;
	byte_t type;
}radhex_pdv_head_t;

#ifdef	__cplusplus
extern "C" {
#endif

	RAD_API radhex_t* radhex_scu(xhand_t bio);

	RAD_API radhex_t* radhex_scp(xhand_t bio);

	RAD_API void radhex_close(radhex_t* radhex);

	RAD_API void radhex_abort(radhex_t* radhex, int errcode);

	RAD_API int radhex_status(radhex_t* radhex);

	RAD_API bool_t radhex_connect(radhex_t* radhex, const tchar_t* dbname);

	RAD_API bool_t radhex_accept(radhex_t* radhex);

	RAD_API bool_t radhex_recv(radhex_t* radhex, radhex_pdv_head_t* pdv);

	RAD_API bool_t radhex_send(radhex_t* radhex, const radhex_pdv_head_t* pdv);

	RAD_API bool_t radhex_set(radhex_t* radhex, variant_t key, object_t val);

	RAD_API bool_t radhex_get(radhex_t* radhex, variant_t key, object_t val);

	RAD_API bool_t radhex_del(radhex_t* radhex, variant_t key);

#ifdef	__cplusplus
}
#endif


#endif	/* _RADNET_H */

