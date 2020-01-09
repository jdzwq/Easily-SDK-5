/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tls document

	@module	oemtls.h | tls interface file

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

#ifndef _OEMSSL_H
#define	_OEMSSL_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_SSL

typedef void*	ssl_sock_t;
typedef int(*pf_socket_write)(void* pso, unsigned char* buf, int len);
typedef int(*pf_socket_read)(void* pso, unsigned char* buf, int len);

XDL_API ssl_sock_t yssl_alloc(res_sock_t so, pf_socket_write pf_write, pf_socket_read pf_read, bool_t srv);

XDL_API void yssl_free(ssl_sock_t tls);

XDL_API int yssl_read(ssl_sock_t tls, byte_t* buf, dword_t max);

XDL_API int yssl_write(ssl_sock_t tls, const byte_t* buf, dword_t size);

XDL_API void yssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t len);

XDL_API void yssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t len);

XDL_API void yssl_set_rsa(xhand_t ssl, const byte_t* sz_rsa, dword_t rlen, const byte_t* sz_pwd, dword_t plen);

XDL_API void yssl_set_dhm(xhand_t ssl, schar_t *dhm_P, schar_t *dhm_G);

#endif //XDLOEM

#endif	/* OEMTLS_H */

