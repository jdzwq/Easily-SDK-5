/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl document

	@module	netssl.h | interface file

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

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_CRYPT)

#define SSL_MAJOR_VERSION_3             3
#define SSL_MINOR_VERSION_0             0 //SSL v3.0
#define SSL_MINOR_VERSION_1             1 //TLS v1.0
#define SSL_MINOR_VERSION_2             2 //TLS v1.1
#define SSL_MINOR_VERSION_3             3 //TLS v1.2

#define SSL_MSG_CHANGE_CIPHER_SPEC     20
#define SSL_MSG_ALERT                  21
#define SSL_MSG_HANDSHAKE              22
#define SSL_MSG_APPLICATION_DATA       23

#define SSL_ALERT_CLOSE_NOTIFY          0
#define SSL_ALERT_WARNING               1
#define SSL_ALERT_FATAL                 2
#define SSL_ALERT_NO_CERTIFICATE       41

#define SSL_HS_HELLO_REQUEST            0
#define SSL_HS_CLIENT_HELLO             1
#define SSL_HS_SERVER_HELLO             2
#define SSL_HS_CERTIFICATE             11
#define SSL_HS_SERVER_KEY_EXCHANGE     12
#define SSL_HS_CERTIFICATE_REQUEST     13
#define SSL_HS_SERVER_HELLO_DONE       14
#define SSL_HS_CERTIFICATE_VERIFY      15
#define SSL_HS_CLIENT_KEY_EXCHANGE     16
#define SSL_HS_FINISHED                20

#define SSL_RSA_RC4_128_MD5              4
#define SSL_RSA_RC4_128_SHA              5
#define SSL_RSA_DES_168_SHA             10
#define SSL_EDH_RSA_DES_168_SHA         22
#define SSL_RSA_AES_128_SHA             47
#define SSL_RSA_AES_256_SHA             53
#define SSL_EDH_RSA_AES_256_SHA         57

#define SSL_VERIFY_NONE                 0
#define SSL_VERIFY_OPTIONAL             1
#define SSL_VERIFY_REQUIRED             2

#define SSL_RSA_PUBLIC				0
#define SSL_RSA_PRIVATE				1

typedef enum
{
	SSL_HANDSHAKE_ERROR = -1,
	SSL_HELLO_REQUEST = 0,
	SSL_CLIENT_HELLO = 1,
	SSL_SERVER_HELLO = 2,
	SSL_SERVER_CERTIFICATE = 3,
	SSL_SERVER_KEY_EXCHANGE = 4,
	SSL_CERTIFICATE_REQUEST = 5,
	SSL_SERVER_HELLO_DONE = 6,
	SSL_CLIENT_CERTIFICATE = 7,
	SSL_CLIENT_KEY_EXCHANGE = 8,
	SSL_CERTIFICATE_VERIFY = 9,
	SSL_CLIENT_CHANGE_CIPHER_SPEC = 10,
	SSL_CLIENT_FINISHED = 11,
	SSL_SERVER_CHANGE_CIPHER_SPEC = 12,
	SSL_SERVER_FINISHED = 13,
	SSL_HANDSHAKE_OVER = 14
}handshake_states;

#define SSL_TYPE_CLIENT		0
#define SSL_TYPE_SERVER		1
#define SSL_TYPE_LISTEN		2

#define SSL_CTR_SIZE		8	//control byte size
#define SSL_HDR_SIZE		5	//message header size
#define SSL_HSH_SIZE		4	//handshake header size
#define SSL_RND_SIZE		32	//radom byte size
#define SSL_MST_SIZE		48	//master scret byte size
#define SSL_MAC_SIZE		20	//mac byte size
#define SSL_IVC_SIZE		16	//iv code size
#define SSL_SES_SIZE		32	//session id size
#define SSL_CTX_SIZE		128	//encrypt/decrypt context size
#define SSL_BLK_SIZE		256	//key block size

#define SSL_PKG_SIZE		16384
#define SSL_MAX_SIZE		(SSL_PKG_SIZE + 512)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xssl_cli: create a SSL client.
@INPUT unsigned short port: the network port to connect.
@INPUT const tchar_t* addr: the network address to connect.
@RETURN xhand_t: if succeeds return SSL client handle, fails return NULL.
*/
EXP_API xhand_t xssl_cli(unsigned short port, const tchar_t* addr);

/*
@FUNCTION xssl_srv: create a SSL server.
@INPUT res_file_t so: the network io resource handle, it must be a socket resource handle.
@RETURN xhand_t: if succeeds return SSL server handle, fails return NULL.
*/
EXP_API xhand_t xssl_srv(res_file_t so);

/*
@FUNCTION xssl_socket: get socket resource handle.
@INPUT xhand_t ssl: the SSL handle.
@RETURN res_file_t: return the socket resource handle.
*/
EXP_API res_file_t xssl_socket(xhand_t ssl);

/*
@FUNCTION xssl_type: get socket type, it can be _XSSL_TYPE_CLI, _XSSL_TYPE_SRV.
@INPUT xhand_t ssl: the SSL handle.
@RETURN int: return the socket type.
*/
EXP_API int  xssl_type(xhand_t ssl);

/*
@FUNCTION xssl_close: close SSL handle.
@INPUT xhand_t ssl: the SSL handle.
@RETURN void: none.
*/
EXP_API void  xssl_close(xhand_t ssl);

/*
@FUNCTION xssl_write: write SSL data.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* data: the data buffer.
@INOUTPUT dword_t* pb: indicate the bytes to write and return the bytes writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_write(xhand_t ssl, const byte_t* data, dword_t* pb);

/*
@FUNCTION xssl_flush: ensure write SSL data compeleted.
@INPUT xhand_t ssl: the SSL handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_flush(xhand_t ssl);

/*
@FUNCTION xssl_read: read SSL data.
@INPUT xhand_t ssl: the SSL handle.
@OUTPUT byte_t* data: the data buffer.
@INOUTPUT dword_t* pb: indicate the bytes to read and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_read(xhand_t ssl, byte_t* data, dword_t* pb);

/*
@FUNCTION xssl_setopt: set the socket options.
@INPUT xhand_t ssl: the SSL handle.
@INPUT int oid: the option id, eg: SOCKET_OPTION_SNDBUF, SOCKET_OPTION_RCVBUF, SOCKET_OPTION_NONBLK.
@INPUT void* opt: the option value pointer
@INPUT int len: the value length in bytes, string value must be a zero terminated token and set len to zero.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API void xssl_setopt(xhand_t ssl, int oid, void* opt, int len);

/*
@FUNCTION xssl_addr_port: get SSL local address and port.
@INPUT xhand_t ssl: the SSL handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the local port.
*/
EXP_API unsigned short xssl_addr_port(xhand_t ssl, tchar_t* addr);

/*
@FUNCTION xssl_peer_port: get SSL remote address and port.
@INPUT xhand_t ssl: the SSL handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the remote port.
*/
EXP_API unsigned short xssl_peer_port(xhand_t ssl, tchar_t* addr);

/*
@FUNCTION xssl_set_host: set SSL host name.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const tchar_t* host_cn: the host name.
@RETURN void: none.
*/
EXP_API void xssl_set_host(xhand_t ssl, const tchar_t* host_cn);

/*
@FUNCTION xssl_set_peer: set SSL peer name.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const tchar_t* host_cn: the peer name.
@RETURN void: none.
*/
EXP_API void xssl_set_peer(xhand_t ssl, const tchar_t* peer_cn);

/*
@FUNCTION xssl_set_auth: set SSL authorization mode.
@INPUT xhand_t ssl: the SSL handle.
@INPUT int authmode: the authorization mode, it can be SSL_VERIFY_NONE, SSL_VERIFY_OPTIONAL, SSL_VERIFY_REQUIRED.
@RETURN void: none.
*/
EXP_API void xssl_set_auth(xhand_t ssl, int authmode);

/*
@FUNCTION xssl_set_ca: set SSL root certificate.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* sz_cert: the cerificate bytes buffer.
@INPUT dword_t clen: the data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t clen);

/*
@FUNCTION xssl_set_cert: set SSL owner certificate.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* sz_cert: the cerificate bytes buffer.
@INPUT dword_t clen: the data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t clen);

/*
@FUNCTION xssl_set_rsa: set SSL private rsa key.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* sz_rsa: the rsa key bytes buffer.
@INPUT dword_t rlen: the rsa data size in bytes.
@INPUT const byte_t* sz_pwd: the password key bytes buffer.
@INPUT dword_t plen: the password data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_set_rsa(xhand_t ssl, const byte_t* sz_rsa, dword_t rlen, const tchar_t* sz_pwd, int plen);

/*
@FUNCTION xssl_set_dhm: set SSL dhm key.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* dhm_P: the dhm P value bytes buffer.
@INPUT const byte_t* dhm_G: the dhm G value bytes buffer.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_set_dhm(xhand_t ssl, const byte_t *dhm_P, const byte_t *dhm_G);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/