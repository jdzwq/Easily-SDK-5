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

#include "xportd.h"

void _write_log_title(file_t log, const tchar_t* sz_title, int len);

void _write_log_error(file_t log, const tchar_t* sz_code, const tchar_t* sz_error, int len);

void _write_log_data(file_t log, const byte_t* data, dword_t size);

void _write_log_xml(file_t log, link_t_ptr ptr_xml);

void _write_log_json(file_t log, link_t_ptr ptr_json);

/**********************************************************************************************/

void xportm_log_info(const tchar_t* str, int len);

void xportm_log_error(const tchar_t* errcode, const tchar_t* errtext);

void xportm_log_data(const byte_t* data, dword_t size);

/**********************************************************************************************/
bool_t get_param_item(const tchar_t* sz_param, const tchar_t* key, tchar_t* val, int max);

#endif //_SRVLOG_H