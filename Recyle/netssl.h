/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl document

	@module	netssl.h | ssl interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#ifndef _NETSSL_H
#define _NETSSL_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_SSL) && defined(XDL_SUPPORT_SOCK)

#define SSL_TYPE_CLIENT		0
#define SSL_TYPE_SERVER		1
#define SSL_TYPE_LISTEN		2

#define ERR_SSL_INIT		-1
#define ERR_SSL_OPEN		-2
#define ERR_SSL_CLOSE		-3
#define ERR_SSL_WRITE		-4
#define ERR_SSL_READ		-5

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xssl_cli(unsigned short port, const tchar_t* addr);

XDL_API xhand_t xssl_srv(res_sock_t so);

XDL_API res_sock_t xssl_socket(xhand_t ssl);

XDL_API int  xssl_type(xhand_t ssl);

XDL_API void  xssl_close(xhand_t ssl);

XDL_API bool_t  xssl_write(xhand_t ssl, const byte_t* data, dword_t* pb);

XDL_API bool_t  xssl_read(xhand_t ssl, byte_t* data, dword_t* pb);

XDL_API void xssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t len);

XDL_API void xssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t len);

XDL_API void xssl_set_rsa(xhand_t ssl, const byte_t* sz_rsa, dword_t rlen, const byte_t* sz_pwd, dword_t plen);

XDL_API void xssl_set_dhm(xhand_t ssl, schar_t *dhm_P, schar_t *dhm_G);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/