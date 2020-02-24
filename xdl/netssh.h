/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssh document

	@module	netssh.h | interface file

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

#ifndef _NETSSH_H
#define _NETSSH_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_CRYPT)

#define SSH_VER_SERVER			"SSH-2.0-7.10 xService-5.5"
#define SSH_VER_CLIENT			"SSH-2.0-7.10 Easily-SDK-5.5"

#define SSH2_MSG_DISCONNECT 1
#define SSH2_MSG_IGNORE	 2
#define SSH2_MSG_UNIMPLEMENTED 3
#define SSH2_MSG_DEBUG	4
#define SSH2_MSG_SERVICE_REQUEST	5
#define SSH2_MSG_SERVICE_ACCEPT 6
#define SSH2_MSG_EXT_INFO 7

#define SSH2_MSG_KEXINIT	 20
#define SSH2_MSG_NEWKEYS 21

#define SSH2_MSG_KEXDH_INIT 30
#define SSH2_MSG_KEXDH_REPLY 31
#define SSH2_MSG_KEX_ECDH_INIT 30
#define SSH2_MSG_KEX_ECDH_REPLY 31
#define SSH2_MSG_ECMQV_INIT 30
#define SSH2_MSG_ECMQV_REPLY 31

#define SSH2_MSG_KEX_DH_GEX_REQUEST_OLD 30
#define SSH2_MSG_KEX_DH_GEX_GROUP 31
#define SSH2_MSG_KEX_DH_GEX_INIT 32
#define SSH2_MSG_KEX_DH_GEX_REPLY 33
#define SSH2_MSG_KEX_DH_GEX_REQUEST 34
#define SSH2_MSG_USERAUTH_REQUEST 50
#define SSH2_MSG_USERAUTH_FAILURE 51
#define SSH2_MSG_USERAUTH_SUCCESS 52
#define SSH2_MSG_USERAUTH_BANNER 53
#define SSH2_MSG_USERAUTH_PK_OK 60
#define SSH2_MSG_USERAUTH_PASSWD_CHANGEREQ 60
#define SSH2_MSG_USERAUTH_INFO_REQUEST	 60
#define SSH2_MSG_USERAUTH_GSSAPI_RESPONSE 60
#define SSH2_MSG_USERAUTH_INFO_RESPONSE 61
#define SSH2_MSG_USERAUTH_GSSAPI_TOKEN 61
#define SSH2_MSG_USERAUTH_GSSAPI_EXCHANGE_COMPLETE 63
#define SSH2_MSG_USERAUTH_GSSAPI_ERROR 64
#define SSH2_MSG_USERAUTH_GSSAPI_ERRTOK 65
#define SSH2_MSG_USERAUTH_GSSAPI_MIC 66

#define SSH2_MSG_GLOBAL_REQUEST 80
#define SSH2_MSG_REQUEST_SUCCESS 81
#define SSH2_MSG_REQUEST_FAILURE 82
#define SSH2_MSG_CHANNEL_OPEN 90
#define SSH2_MSG_CHANNEL_OPEN_CONFIRMATION 91
#define SSH2_MSG_CHANNEL_OPEN_FAILURE 92
#define SSH2_MSG_CHANNEL_WINDOW_ADJUST 93
#define SSH2_MSG_CHANNEL_DATA 94
#define SSH2_MSG_CHANNEL_EXTENDED_DATA 95
#define SSH2_MSG_CHANNEL_EOF	96
#define SSH2_MSG_CHANNEL_CLOSE 97
#define SSH2_MSG_CHANNEL_REQUEST 98
#define SSH2_MSG_CHANNEL_SUCCESS 99
#define SSH2_MSG_CHANNEL_FAILURE 100

#define SSH2_DISCONNECT_HOST_NOT_ALLOWED_TO_CONNECT 1
#define SSH2_DISCONNECT_PROTOCOL_ERROR 2
#define SSH2_DISCONNECT_KEY_EXCHANGE_FAILED 3
#define SSH2_DISCONNECT_HOST_AUTHENTICATION_FAILED 4
#define SSH2_DISCONNECT_RESERVED	 4
#define SSH2_DISCONNECT_MAC_ERROR 5
#define SSH2_DISCONNECT_COMPRESSION_ERROR 6
#define SSH2_DISCONNECT_SERVICE_NOT_AVAILABLE 7
#define SSH2_DISCONNECT_PROTOCOL_VERSION_NOT_SUPPORTED 8
#define SSH2_DISCONNECT_HOST_KEY_NOT_VERIFIABLE 9
#define SSH2_DISCONNECT_CONNECTION_LOST 10
#define SSH2_DISCONNECT_BY_APPLICATION 11
#define SSH2_DISCONNECT_TOO_MANY_CONNECTIONS 12
#define SSH2_DISCONNECT_AUTH_CANCELLED_BY_USER 13
#define SSH2_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE 14
#define SSH2_DISCONNECT_ILLEGAL_USER_NAME 15

#define SSH2_OPEN_ADMINISTRATIVELY_PROHIBITED		1
#define SSH2_OPEN_CONNECT_FAILED			2
#define SSH2_OPEN_UNKNOWN_CHANNEL_TYPE			3
#define SSH2_OPEN_RESOURCE_SHORTAGE			4

#define SSH2_EXTENDED_DATA_STDERR			1

#define SSH_TYPE_CLIENT		0
#define SSH_TYPE_SERVER		1
#define SSH_TYPE_LISTEN		2

#define SSH_MIN_SIZE        16
#define SSH_PDU_SIZE		35000
#define SSH_PDV_SIZE		32768
#define SSH_BANNER_SIZE		256
#define SSH_COOKIE_SIZE		16
#define SSH_MIN_PRIM		3

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xssh_cli(unsigned short port, const tchar_t* addr);

XDL_API xhand_t xssh_srv(res_file_t so);

XDL_API res_file_t xssh_socket(xhand_t ssh);

XDL_API int  xssh_type(xhand_t ssh);

XDL_API void  xssh_close(xhand_t ssh);

XDL_API bool_t xssh_write(xhand_t ssh, const byte_t* data, dword_t* pb);

XDL_API bool_t xssh_read(xhand_t ssh, byte_t* data, dword_t* pb);

XDL_API bool_t xssh_flush(xhand_t ssh);

XDL_API void xssh_setopt(xhand_t ssh, int oid, void* opt, int len);

XDL_API unsigned short xssh_addr_port(xhand_t ssh, tchar_t* addr);

XDL_API unsigned short xssh_peer_port(xhand_t ssh, tchar_t* addr);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/