/***********************************************************************
	Easily oss web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc oss function document

	@module	ossdef.h | oss interface file

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

#ifndef _OSSDEF_H
#define _OSSDEF_H

#include <xdl.h>

typedef struct _oss_head{
	sword_t dbt;
	sword_t cbs;
}oss_head, *oss_t;

typedef oss_t	OSS;


#ifdef	__cplusplus
extern "C" {
#endif

	extern oss_t STDCALL oss_open_isp(const tchar_t* ispfile);

	extern void STDCALL oss_close(oss_t oss);

	extern bool_t STDCALL oss_ioctl(oss_t oss, const tchar_t* method, const tchar_t* object, void* inbuf, dword_t inlen, void* outbuf, dword_t* outlen);

	extern int STDCALL oss_error(oss_t oss, tchar_t* buf, int max);

#ifdef	__cplusplus
}
#endif
/*********************************oss export def***************************************************/

typedef oss_t(STDCALL *PF_OSS_OPEN_ISP)(const tchar_t*);
typedef void(STDCALL *PF_OSS_CLOSE)(oss_t);
typedef bool_t(STDCALL *PF_OSS_IOCTL)(oss_t, const tchar_t*, const tchar_t*, void*, dword_t, void*, dword_t*);
typedef int(STDCALL *PF_OSS_ERROR)(oss_t, tchar_t*, int);


#endif