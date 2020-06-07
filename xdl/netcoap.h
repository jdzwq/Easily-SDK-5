/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl coap document

	@module	netcoap.h | interface file

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

#ifndef _NETCOAP_H
#define _NETCOAP_H

#include "xdldef.h"
#include "httpattr.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_RAND)

#define DEF_COAP_PORT		5683

#define COAP_MAJOR_VERSION	1

#define COAP_PKG_SIZE		1472
#define COAP_HDR_SIZE		4
#define COAP_PDV_SIZE		1024
#define COAP_BLK_SIZE		1024
#define COAP_MAX_MTU		1152
#define COAP_MAX_TKL		8
#define COAP_MAX_OPT		1035
#define COAP_MAX_PDU		(8 * 1024 * 1024 + 256)

// message types
#define COAP_MESSAGE_CON       0 /* confirmable message (requires ACK/RST) */
#define COAP_MESSAGE_NON       1 /* non-confirmable message (one-shot message) */
#define COAP_MESSAGE_ACK       2 /* used to acknowledge confirmable messages */
#define COAP_MESSAGE_RST       3 /* indicates error in received messages */

// request methods
#define COAP_REQUEST_GET       1
#define COAP_REQUEST_POST      2
#define COAP_REQUEST_PUT       3
#define COAP_REQUEST_DELETE    4
#define COAP_REQUEST_FETCH     5 /* RFC 8132 */
#define COAP_REQUEST_PATCH     6 /* RFC 8132 */
#define COAP_REQUEST_IPATCH    7 /* RFC 8132 */

// CoAP option types
#define COAP_OPTION_IF_MATCH        1 /* C, opaque, 0-8 B, (none) */
#define COAP_OPTION_URI_HOST        3 /* C, String, 1-255 B, destination address */
#define COAP_OPTION_ETAG            4 /* E, opaque, 1-8 B, (none) */
#define COAP_OPTION_IF_NONE_MATCH   5 /* empty, 0 B, (none) */
#define COAP_OPTION_URI_PORT        7 /* C, uint, 0-2 B, destination port */
#define COAP_OPTION_LOCATION_PATH   8 /* E, String, 0-255 B, - */
#define COAP_OPTION_URI_PATH       11 /* C, String, 0-255 B, (none) */
#define COAP_OPTION_CONTENT_FORMAT 12 /* E, uint, 0-2 B, (none) */
#define COAP_OPTION_CONTENT_TYPE COAP_OPTION_CONTENT_FORMAT
#define COAP_OPTION_MAXAGE         14 /* E, uint, 0--4 B, 60 Seconds */
#define COAP_OPTION_URI_QUERY      15 /* C, String, 1-255 B, (none) */
#define COAP_OPTION_ACCEPT         17 /* C, uint,   0-2 B, (none) */
#define COAP_OPTION_LOCATION_QUERY 20 /* E, String,   0-255 B, (none) */
#define COAP_OPTION_PROXY_URI      35 /* C, String, 1-1034 B, (none) */
#define COAP_OPTION_PROXY_SCHEME   39 /* C, String, 1-255 B, (none) */
#define COAP_OPTION_SIZE1          60 /* E, uint, 0-4 B, (none) */
#define COAP_OPTION_SIZE2          28 /* E, uint, 0-4 B, (none) */

// option types from RFC 7641
#define COAP_OPTION_OBSERVE         6 /* E, empty/uint, 0 B/0-3 B, (none) */
#define COAP_OPTION_SUBSCRIPTION  COAP_OPTION_OBSERVE

// selected option types from RFC 7959 
#define COAP_OPTION_BLOCK2         23 /* C, uint, 0--3 B, (none) */
#define COAP_OPTION_BLOCK1         27 /* C, uint, 0--3 B, (none) */

// selected option types from RFC 7967
#define COAP_OPTION_NORESPONSE    258 /* N, uint, 0--1 B, 0 */

// response code
#define COAP_RESPONSE_200_CODE		(((200)/100 << 5) | (200)%100)  /* 2.00 OK */
#define COAP_RESPONSE_201_CODE		(((201)/100 << 5) | (201)%100)  /* 2.01 Created */
#define COAP_RESPONSE_203_CODE		(((203)/100 << 5) | (203)%100)  /* 2.03 Valid */
#define COAP_RESPONSE_204_CODE		(((204)/100 << 5) | (204)%100)  /* 2.04 Changed */
#define COAP_RESPONSE_205_CODE		(((205)/100 << 5) | (205)%100)  /* 2.05 Content */
#define COAP_RESPONSE_231_CODE		(((231)/100 << 5) | (231)%100)  /* 2.31 Continue */
#define COAP_RESPONSE_400_CODE		(((400)/100 << 5) | (400)%100)  /* 4.00 Bad Request */
#define COAP_RESPONSE_404_CODE		(((404)/100 << 5) | (404)%100)  /* 4.04 Not Found */
#define COAP_RESPONSE_405_CODE		(((405)/100 << 5) | (405)%100)  /* 4.05 Method Not Allowed */
#define COAP_RESPONSE_415_CODE		(((415)/100 << 5) | (415)%100)  /* 4.15 Unsupported Media Type */
#define COAP_RESPONSE_500_CODE		(((500)/100 << 5) | (500)%100)  /* 5.00 Internal Server Error */
#define COAP_RESPONSE_501_CODE		(((501)/100 << 5) | (501)%100)  /* 5.01 Not Implemented */
#define COAP_RESPONSE_503_CODE		(((503)/100 << 5) | (503)%100)  /* 5.03 Service Unavailable */
#define COAP_RESPONSE_504_CODE		(((504)/100 << 5) | (504)%100)  /* 5.04 Gateway Timeout */
#define COAP_RESPONSE_X_242_CODE    (((402)/100 << 5) | (402)%100)  /* Critical Option not supported */

#define COAP_IS_SUCCEED(code)		(((code >> 5) == 2)? 1 : 0)

// response message
#define COAP_RESPONSE_200_TEXT      _T("2.00 OK")
#define COAP_RESPONSE_201_TEXT      _T("2.01 Created")
#define COAP_RESPONSE_203_TEXT      _T("2.03 Valid")
#define COAP_RESPONSE_204_TEXT      _T("2.04 Changed")
#define COAP_RESPONSE_205_TEXT      _T("2.05 Changed")
#define COAP_RESPONSE_231_TEXT      _T("2.31 Continue")
#define COAP_RESPONSE_400_TEXT      _T("4.00 Bad Request")
#define COAP_RESPONSE_404_TEXT      _T("4.04 Not Found")
#define COAP_RESPONSE_405_TEXT      _T("4.05 Method Not Allowed")
#define COAP_RESPONSE_415_TEXT      _T("4.15 Unsupported Media Type")
#define COAP_RESPONSE_500_TEXT      _T("5.00 Internal Server Error")
#define COAP_RESPONSE_501_TEXT      _T("5.01 Not Implemented")
#define COAP_RESPONSE_503_TEXT      _T("5.03 Service Unavailable")
#define COAP_RESPONSE_504_TEXT      _T("5.04 Gateway Timeout")
#define COAP_RESPONSE_X_242_TEXT    _T("4.02 Critical Option not supported")

// signal message
#define COAP_SIGNALING_CODE(N) (((N)/100 << 5) | (N)%100)
#define COAP_SIGNALING_CSM_CODE     (((701)/100 << 5) | (701)%100)
#define COAP_SIGNALING_PING_CODE    (((702)/100 << 5) | (702)%100)
#define COAP_SIGNALING_PONG_CODE    (((703)/100 << 5) | (703)%100)
#define COAP_SIGNALING_RELEASE_CODE (((704)/100 << 5) | (704)%100)
#define COAP_SIGNALING_ABORT_CODE   (((705)/100 << 5) | (705)%100)

/* media type encoding */
#define COAP_MEDIATYPE_TEXT_PLAIN                 0 /* text/plain (UTF-8) */
#define COAP_MEDIATYPE_APPLICATION_LINK_FORMAT   40 /* application/link-format */
#define COAP_MEDIATYPE_APPLICATION_XML           41 /* application/xml */
#define COAP_MEDIATYPE_APPLICATION_OCTET_STREAM  42 /* application/octet-stream */
#define COAP_MEDIATYPE_APPLICATION_RDF_XML       43 /* application/rdf+xml */
#define COAP_MEDIATYPE_APPLICATION_EXI           47 /* application/exi  */
#define COAP_MEDIATYPE_APPLICATION_JSON          50 /* application/json  */
#define COAP_MEDIATYPE_APPLICATION_CBOR          60 /* application/cbor  */

/* Content formats from RFC 8152 */
#define COAP_MEDIATYPE_APPLICATION_COSE_SIGN     98 /* application/cose; cose-type="cose-sign"     */
#define COAP_MEDIATYPE_APPLICATION_COSE_SIGN1    18 /* application/cose; cose-type="cose-sign1"    */
#define COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT  96 /* application/cose; cose-type="cose-encrypt"  */
#define COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT0 16 /* application/cose; cose-type="cose-encrypt0" */
#define COAP_MEDIATYPE_APPLICATION_COSE_MAC      97 /* application/cose; cose-type="cose-mac"      */
#define COAP_MEDIATYPE_APPLICATION_COSE_MAC0     17 /* application/cose; cose-type="cose-mac0"     */

#define COAP_MEDIATYPE_APPLICATION_COSE_KEY     101 /* application/cose-key  */
#define COAP_MEDIATYPE_APPLICATION_COSE_KEY_SET 102 /* application/cose-key-set  */

/* Note that identifiers for registered media types are in the range 0-65535. We
* use an unallocated type here and hope for the best. */
#define COAP_MEDIATYPE_ANY                       0xff /* any media type */

// coap protocol
#define COAP_PROTO_NONE	  0
#define COAP_PROTO_UDP	  1
#define COAP_PROTO_DTLS	  2

typedef enum{
	_XCOAP_TYPE_LIS = 0,
	_XCOAP_TYPE_CLI = 1,
	_XCOAP_TYPE_SRV = 2
}XCOAP_TYPE;


#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xcoap_client: create a COAP client.
@INPUT const tchar_t* method: the COAP method, it can be "GET", "PUT", "HEAD", "DELETE".
@INPUT const tchar_t* url: the COAP service url.
@RETURN xhand_t: if succeeds return COAP client handle, fails return NULL.
*/
EXP_API xhand_t		xcoap_client(const tchar_t* method, const tchar_t* url);

/*
@FUNCTION xcoap_server: create a COAP server.
@INPUT unsigned short port: the network port from client.
@INPUT const tchar_t* addr: the network address from client.
@INPUT const byte_t* pack: the network package from client.
@INPUT dword_t size: the network package size in bytes.
@RETURN xhand_t: if succeeds return COAP server handle, fails return NULL.
*/
EXP_API xhand_t		xcoap_server(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size);

/*
@FUNCTION xcoap_close: close the COAP handle.
@INPUT xhand_t coap: the COAP handle.
@RETURN void: none.
*/
EXP_API void		xcoap_close(xhand_t coap);

/*
@FUNCTION xcoap_connect: client connect to server.
@INPUT xhand_t coap: the COAP handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero..
*/
EXP_API bool_t		xcoap_connect(xhand_t coap);

/*
@FUNCTION xcoap_accept: server accept from client.
@INPUT xhand_t coap: the COAP handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero..
*/
EXP_API bool_t		xcoap_accept(xhand_t coap);

/*
@FUNCTION xcoap_method: get a COAP method.
@INPUT xhand_t coap: the COAP handle.
@OUTPUT tchar_t* buf: the string buffer for returning method.
@INPUT int max: the buffer size in characters.
@RETURN int: return the method token length in characters.
*/
EXP_API int			xcoap_method(xhand_t coap, tchar_t* buf, int max);

/*
@FUNCTION xcoap_msgid: get coap message ID;
@INPUT xhand_t coap: the COAP handle.
@RETURN sword_t: the message ID.
*/
EXP_API sword_t		xcoap_msgid(xhand_t coap);

/*
@FUNCTION xcoap_get_object: get a COAP file name.
@INPUT xhand_t coap: the COAP handle.
@OUTPUT tchar_t* buf: the string buffer for returning file name.
@INPUT int max: the buffer size in characters.
@RETURN none: return the object token length in characters.
*/
EXP_API int			xcoap_get_object(xhand_t coap, tchar_t* buf, int max);

/*
@FUNCTION xcoap_set_object: set COAP file name.
@INPUT xhand_t coap: the COAP handle.
@OUTPUT const tchar_t* obj: the file name.
@INPUT int len: the string size in characters.
@RETURN none: 
*/
EXP_API void		xcoap_set_object(xhand_t coap, const tchar_t* obj, int len);

/*
@FUNCTION xcoap_get_query: get a COAP query token.
@INPUT xhand_t coap: the COAP handle.
@OUTPUT tchar_t* buf: the string buffer for returning query token.
@INPUT int max: the buffer size in characters.
@RETURN int: return the query token length in characters.
*/
EXP_API int			xcoap_get_query(xhand_t coap, tchar_t* buf, int max);

/*
@FUNCTION xcoap_set_query: set COAP query token.
@INPUT xhand_t coap: the COAP handle.
@OUTPUT const tchar_t* qry: the query token
@INPUT int len: the string size in characters.
@RETURN none:
*/
EXP_API void		xcoap_set_query(xhand_t coap, const tchar_t* qry, int len);

/*
@FUNCTION xcoap_set_content_type: set COAP media content type.
@INPUT xhand_t coap: the COAP handle.
@INPUT int type: the content type.
@RETURN none:
*/
EXP_API void		xcoap_set_content_type(xhand_t coap, int type);

/*
@FUNCTION xcoap_get_content_type: get COAP media content type.
@INPUT xhand_t coap: the COAP handle.
@RETURN int: return the content type.
*/
EXP_API int			xcoap_get_content_type(xhand_t coap);

/*
@FUNCTION xcoap_set_content_length: set COAP media content size.
@INPUT xhand_t coap: the COAP handle.
@INPUT dword_t len: the content size in bytes.
@RETURN none:
*/
EXP_API void		xcoap_set_content_length(xhand_t coap, dword_t len);

/*
@FUNCTION xcoap_get_content_length: get COAP media content size.
@INPUT xhand_t coap: the COAP handle.
@RETURN dword_t: return the content length in bytes.
*/
EXP_API dword_t		xcoap_get_content_length(xhand_t coap);

/*
@FUNCTION xcoap_get_option: get a COAP option token.
@INPUT xhand_t coap: the COAP handle.
@INPUT int opd: the option id.
@OUTPUT tchar_t* buf: the string buffer for returning option token.
@INPUT int max: the buffer size in characters.
@RETURN int: return the option token length in characters.
*/
EXP_API int			xcoap_get_option(xhand_t coap, int opd, tchar_t* buf, int max);

/*
@FUNCTION xcoap_set_option: set COAP option token.
@INPUT xhand_t coap: the COAP handle.
@INPUT int opd: the option id.
@OUTPUT const tchar_t* opt: the option token
@INPUT int len: the string size in characters.
@RETURN none:
*/
EXP_API void		xcoap_set_option(xhand_t coap, int opd, const tchar_t* opt, int len);

/*
@FUNCTION xcoap_rest_blockwise: initialize block sending.
@INPUT xhand_t coap: the COAP handle.
@INPUT int szx: the block size indicater, [0..6], means the block max size is 2 power of 4 to 2 power of 10 bytes.
@RETURN void: none.
*/
EXP_API void		xcoap_set_blockwise(xhand_t coap, int szx);

/*
@FUNCTION xcoap_send: send COAP data.
@INPUT xhand_t coap: the COAP handle.
@INPUT const byte_t* data: the data bytes buffer.
@INOUTPUT dword_t* pb: indicate the bytes to send and return the bytes sended.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xcoap_send(xhand_t coap, const byte_t* data, dword_t *pb);

/*
@FUNCTION xcoap_flush: ensure COAP data sending compeleted.
@INPUT xhand_t coap: the COAP handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xcoap_flush(xhand_t coap);

/*
@FUNCTION xcoap_recv: recv COAP data.
@INPUT xhand_t coap: the COAP handle.
@INPUT const byte_t* data: the bytes buffer for receiving.
@INOUTPUT dword_t* pb: indicate the bytes to read and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xcoap_recv(xhand_t coap, byte_t* buf, dword_t *pb);

/*
@FUNCTION xcoap_abort: abort a COAP operation, it must be called by COAP server.
@INPUT xhand_t coap: the COAP handle.
@INPUT int errcode: the COAP error code.
@RETURN void: none.
*/
EXP_API void		xcoap_abort(xhand_t coap, int errcode);


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*XDLINET_H*/