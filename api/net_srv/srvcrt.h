/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc service defination document

	@module	srvcert.h | service definition interface file

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


#ifndef _SRVCRT_H
#define _SRVCRT_H

#include "srvdef.h"

#define SSL_SSLCA_CRT		_T("sslca-crt")
#define SSL_SSLCA_KEY		_T("sslca-key")
#define SSL_SSLSRV_CRT		_T("sslsrv-crt")
#define SSL_SSLSRV_KEY		_T("sslsrv-key")
#define SSL_SSLCLI_CRT		_T("sslcli-crt")
#define SSL_SSLCLI_KEY		_T("sslcli-key")

#define SSH_SSHCA_CRT		_T("sshca-crt")
#define SSH_SSHCA_KEY		_T("sshca-key")
#define SSH_SSHSRV_CRT		_T("sshsrv-crt")
#define SSH_SSHSRV_KEY		_T("sshsrv-key")
#define SSH_SSHCLI_CRT		_T("sshcli-crt")
#define SSH_SSHCLI_KEY		_T("sshcli-key")

bool_t get_ssl_crt(const tchar_t* path, const tchar_t* name, byte_t* buf, dword_t* pb);

bool_t get_ssl_key(const tchar_t* path, const tchar_t* name, byte_t* buf, dword_t* pb);

bool_t get_ssh_key(const tchar_t* path, const tchar_t* name, byte_t* buf, dword_t* pb);


#endif //_SRVCERT_H