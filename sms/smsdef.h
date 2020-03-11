/***********************************************************************
	Easily sms

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	smsdef.h | definition interface file

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


#ifndef _SMSDEF_H
#define	_SMSDEF_H

#include <xdl.h>

#define SMS_ACTION			_T("Action")
#define SMS_ACTION_CODE		_T("Code")
#define SMS_ACTION_VERIFY	_T("Verify")
#define SMS_ACTION_MESSAGE	_T("Message")
#define SMS_ACTION_RECEIPT	_T("Receipt")

#define MIN_SMS_NAME	64
#define MAX_SMS_NAME	256
#define MAX_SMS_DATA	1024

#define SMS_PENDING		100
#define SMS_SUCCEED		1
#define SMS_FAILED		0

/*define sms type*/
typedef enum{
	_SMS_UNKNOWN = 0,
	_SMS_CMC = 1,
	_SMS_ALIYUN = 2,
}SMS_TYPE;

typedef struct _sms_head{
	sword_t dbt;
	sword_t cbs;
}sms_head, *sms_t;

typedef sms_t	SMS;

#endif	/* _SMSDEF_H */

