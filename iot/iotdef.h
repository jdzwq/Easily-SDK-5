/***********************************************************************
	Easily iot

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	iotdef.h | definition interface file

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


#ifndef _IOTDEF_H
#define	_IOTDEF_H

#include <xdl.h>


#define IOT_PENDING		100
#define IOT_SUCCEED		1
#define IOT_FAILED		0

/*define iot type*/
typedef enum{
	_IOT_UNKNOWN = 0,
	_IOT_CMC = 1,
	_IOT_ALIYUN = 2,
}IOT_TYPE;

typedef struct _iot_head{
	sword_t dbt;
	sword_t cbs;
}iot_head, *iot_t;

typedef iot_t	IOT;

typedef struct _iot_command_t{
	tchar_t device_id[KEY_LEN];
	tchar_t session_key[KEY_LEN];
	tchar_t command_key[RES_LEN];
	tchar_t command_input[1024];
	tchar_t command_output[1024];
}iot_command_t;

#ifdef	__cplusplus
extern "C" {
#endif

	extern iot_t STDCALL iot_open_isp(const tchar_t* ispfile);

	extern void STDCALL iot_close(iot_t iot);

	extern bool_t STDCALL iot_session(iot_t iot, tchar_t* skey, int max);

	extern bool_t STDCALL iot_command(iot_t iot, iot_command_t* pio);

	extern int STDCALL iot_error(iot_t iot, tchar_t* buf, int max);

#ifdef	__cplusplus
}
#endif

/*********************************iot export def***************************************************/

typedef iot_t(STDCALL *PF_IOT_OPEN_ISP)(const tchar_t*);
typedef void(STDCALL *PF_IOT_CLOSE)(iot_t);
typedef bool_t(STDCALL *PF_IOT_SESSION)(iot_t, tchar_t*, int);
typedef bool_t(STDCALL *PF_IOT_COMMAND)(iot_t, iot_command_t*);
typedef int(STDCALL *PF_IOT_ERROR)(iot_t, tchar_t*, int);


#endif	/* _IOTDEF_H */

