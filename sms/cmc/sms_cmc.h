/***********************************************************************
	Easily sms web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc sms defination document

	@module	smsapi.h | sms web api definition interface file

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
#ifndef _SMSCMC_H
#define _SMSCMC_H

#include <xdl.h>

extern bool_t __stdcall sms_ioctl(
	const tchar_t* io_op,
	const tchar_t* url,
	const tchar_t* acc_id,
	const tchar_t* acc_key,
	const tchar_t* phone,
	const tchar_t* in_data, 
	int in_len, 

	tchar_t* out_buf,
	int* out_max
	);


#endif