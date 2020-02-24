/***********************************************************************
	Easily sms

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc sms function document

	@module	smspro.h | sms interface file

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

#ifndef _SMSPRO_H
#define _SMSPRO_H

#include "smsdef.h"

extern sms_t STDCALL sms_open_isp(const tchar_t* ispfile);

extern void STDCALL sms_close(sms_t sms);

extern bool_t STDCALL sms_code(sms_t sms,const tchar_t* phone, const tchar_t* param, int len);

extern bool_t STDCALL sms_message(sms_t sms, const tchar_t* phone, const tchar_t* param, int len, tchar_t* msgid, int max);

extern bool_t STDCALL sms_receipt(sms_t sms, const tchar_t* phone, const tchar_t* msgid, int len, tchar_t* receipt, int max);

extern int STDCALL sms_error(sms_t sms,tchar_t* buf,int max);

/*********************************sms export def***************************************************/

typedef bool_t(STDCALL *PF_SMS_PARSE_ISP)(const tchar_t*, tchar_t*, int, tchar_t*, int, tchar_t*, int);
typedef sms_t(STDCALL *PF_SMS_OPEN_ISP)(const tchar_t*);
typedef void(STDCALL *PF_SMS_CLOSE)(sms_t);
typedef bool_t(STDCALL *PF_SMS_CODE)(sms_t, const tchar_t*, const tchar_t*, int);
typedef bool_t(STDCALL *PF_SMS_MESSAGE)(sms_t, const tchar_t*, const tchar_t*, int, tchar_t*, int);
typedef bool_t(STDCALL *PF_SMS_RECEIPT)(sms_t, const tchar_t*, const tchar_t*, int, tchar_t*, int);
typedef int(STDCALL *PF_SMS_ERROR)(sms_t, tchar_t*, int);


#endif