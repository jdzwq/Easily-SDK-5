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


#ifndef _SRVCERT_H
#define _SRVCERT_H

#include "srvdef.h"

#define SSL_SSLCA_CRT		_T("sslca-crt")
#define SSL_SSLCA_KEY		_T("sslca-key")
#define SSL_SSLSRV_CRT		_T("sslsrv-crt")
#define SSL_SSLSRV_KEY		_T("sslsrv-key")
#define SSL_SSLCLI_CRT		_T("sslcli-crt")
#define SSL_SSLCLI_KEY		_T("sslcli-key")

#define XSL_XSLCA_CRT		_T("xslca-crt")
#define XSL_XSLCA_KEY		_T("xslca-key")
#define XSL_XSLSRV_CRT		_T("xslsrv-crt")
#define XSL_XSLSRV_KEY		_T("xslsrv-key")
#define XSL_XSLCLI_CRT		_T("xslcli-crt")
#define XSL_XSLCLI_KEY		_T("xslcli-key")

typedef struct _cert_t{
	xhand_t		srv_crt;
	xhand_t		srv_key;
	int			ca_count;
	xhand_t*	ca_chain;
}cert_t;

cert_t* alloc_certs(int secu, const tchar_t* sz_path);

void free_certs(cert_t* pcrt);

void set_certs(int secu, xhand_t bio);

#endif //_SRVCERT_H