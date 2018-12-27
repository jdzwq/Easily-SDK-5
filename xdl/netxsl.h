/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xsl document

	@module	netxsl.h | xsl interface file

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

#ifndef _NETXSL_H
#define _NETXSL_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_CRYPT)

#define XSL_MAJOR_VERSION_3             3
#define XSL_MINOR_VERSION_1             1

#define XSL_HS_HELLO_REQUEST            0
#define XSL_HS_CLIENT_HELLO             1
#define XSL_HS_SERVER_HELLO             2
#define XSL_HS_SERVER_EXCHANGE			3
#define XSL_HS_CLIENT_EXCHANGE			4
#define XSL_HS_FINISHED					5
#define XSL_HS_ERROR					-1

#define XSL_RSA_RC4_128_SHA             0
#define XSL_RSA_RC4_128_MD5             1

#define XSL_TAG_RANDOM				0x01
#define XSL_TAG_CIPHER				0x02
#define XSL_TAG_CERTIFICATE			0x03
#define XSL_TAG_DHMPARAM			0x04
#define XSL_TAG_RSACRYPT			0x05
#define XSL_TAG_RSASIGN				0x06
#define XSL_TAG_PROFILE				0x07
#define XSL_TAG_ERROR				0xFF

#define XSL_RSA_PUBLIC				0
#define XSL_RSA_PRIVATE				1

#define XSL_STR_SECRET				"SECRET"
#define XSL_STR_EXPANSION			"EXPANSION"
#define XSL_STR_FINISHED			"FINISHED"

typedef enum
{
	XSL_HANDSHAKE_ERROR = -1,
	XSL_HELLO_REQUEST = 0,
	XSL_HANDSHAKE_OVER = 1,

	XSL_CLIENT_HELLO = 10,

	XSL_SERVER_HELLO = 20,
	XSL_SERVER_EXCHANGE = 21,

	XSL_CLIENT_EXCHANGE = 30,
	XSL_CLIENT_FINISHED = 31,

	XSL_SERVER_FINISHED = 40
}xsl_states;

#define XSL_TYPE_CLIENT		0
#define XSL_TYPE_SERVER		1
#define XSL_TYPE_LISTEN		2

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xxsl_cli(unsigned short port, const tchar_t* addr);

XDL_API xhand_t xxsl_srv(res_file_t so);

XDL_API res_file_t xxsl_socket(xhand_t xsl);

XDL_API int  xxsl_type(xhand_t xsl);

XDL_API void  xxsl_close(xhand_t xsl);

XDL_API unsigned short xxsl_addr_port(xhand_t xsl, tchar_t* addr);

XDL_API unsigned short xxsl_peer_port(xhand_t xsl, tchar_t* addr);

XDL_API void xxsl_set_send_buff(xhand_t xsl, dword_t dw);

XDL_API void xxsl_set_recv_buff(xhand_t xsl, dword_t dw);

XDL_API void xxsl_set_send_timeout(xhand_t xsl, int s);

XDL_API void xxsl_set_recv_timeout(xhand_t xsl, int s);

XDL_API void xxsl_set_linger(xhand_t xsl, bool_t b_wait, int n_sec);

XDL_API bool_t xxsl_write(xhand_t xsl, const byte_t* data, dword_t* pb);

XDL_API bool_t xxsl_read(xhand_t xsl, byte_t* data, dword_t* pb);

XDL_API bool_t xxsl_set_ca(xhand_t xsl, const byte_t* sz_cert, dword_t clen);

XDL_API bool_t xxsl_set_cert(xhand_t xsl, const byte_t* sz_cert, dword_t clen);

XDL_API bool_t xxsl_set_rsa(xhand_t xsl, const byte_t* sz_rsa, dword_t rlen, const byte_t* sz_pwd, dword_t plen);

XDL_API bool_t xxsl_set_dhm(xhand_t xsl, const byte_t *dhm_P, const byte_t *dhm_G);

XDL_API bool_t xxsl_set_ciph(xhand_t xsl, int ciph);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/