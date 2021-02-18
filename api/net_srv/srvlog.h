/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc service log defination document

	@module	srvlog.h | service log definition interface file

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


#ifndef _SRVLOG_H
#define _SRVLOG_H

#include "srvdef.h"

void get_loged_interface(const tchar_t* unc, loged_interface* plog);

void get_event_interface(const tchar_t* url, event_interface* pev);

/**********************************************************************************************/

void xportm_log_info(const tchar_t* str, int len);

void xportm_log_error(const tchar_t* errcode, const tchar_t* errtext);

void xportm_log_data(const byte_t* data, dword_t size);

/**********************************************************************************************/

#endif //_SRVLOG_H