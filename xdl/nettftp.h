/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl tftp document

	@module	nettftp.h | xdl tftp interface file

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

#ifndef _NETTFTP_H
#define _NETTFTP_H

#include "xdldef.h"
#include "httpattr.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_RAND)

typedef enum{
	_XTFTP_TYPE_LIS = 0,
	_XTFTP_TYPE_CLI = 1,
	_XTFTP_TYPE_SRV = 2
}XTFTP_TYPE;

#define DEF_TFTP_PORT		69

#define TFTP_PKG_SIZE      516
#define TFTP_HDR_SIZE		2
#define TFTP_PDU_SIZE      514
#define TFTP_PDV_SIZE      512

#define TFTP_PDU_HEAD		0
#define TFTP_PDU_RRQ		1
#define TFTP_PDU_WRQ		2
#define TFTP_PDU_DATA		3
#define TFTP_PDU_ACK		4
#define TFTP_PDU_ERR		5
#define TFTP_PDU_DEL		6

#define TFTP_METHOD_GET		_T("GET")
#define TFTP_METHOD_PUT		_T("PUT")
#define TFTP_METHOD_HEAD	_T("HEAD")
#define TFTP_METHOD_DELETE	_T("DELETE")

#define TFTP_CODE_NOTDEF		0
#define TFTP_CODE_NOTDEF_TEXT	_T("Not defined")

#define TFTP_CODE_NOTFIND		1
#define TFTP_CODE_NOTFIND_TEXT	_T("File not found")

#define TFTP_CODE_REJECT		2
#define TFTP_CODE_REJECT_TEXT	_T("Access violation")

#define TFTP_CODE_DSKFULL		3
#define TFTP_CODE_DSKFULL_TEXT	_T("Disk full or allocation exceeded")

#define TFTP_CODE_ILLEGAL	4
#define TFTP_CODE_ILLEGAL_TEXT	_T("Illegal TFTP operation")

#define TFTP_CODE_UNTID		5
#define TFTP_CODE_UNTID_TEXT	_T("Unknown transfer ID")

#define TFTP_CODE_EXISTS		6
#define TFTP_CODE_EXISTS_TEXT	_T("File already exists")

#define TFTP_CODE_NOUSER		7
#define TFTP_CODE_NOUSER_TEXT	_T("No such user")

/*TFTP Status*/
typedef enum{
	_TFTP_STATUS_ASSOCIATE = 0,
	_TFTP_STATUS_WAITING = 1,
	_TFTP_STATUS_PENDING = 2,
	_TFTP_STATUS_EOF = 3,
	_TFTP_STATUS_NOTIFY = 4,
	_TFTP_STATUS_RELEASE = 5
}TFTP_STATUS;

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API xhand_t		xtftp_client(const tchar_t* method, const tchar_t* url);

	XDL_API xhand_t		xtftp_server(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size);

	XDL_API int			xtftp_method(xhand_t tftp, tchar_t* buf, int max);

	XDL_API int			xtftp_object(xhand_t tftp, tchar_t* buf, int max);

	XDL_API void		xtftp_close(xhand_t tftp);

	XDL_API bool_t		xtftp_send(xhand_t tftp, const byte_t* data, dword_t *pb);

	XDL_API void		xtftp_flush(xhand_t tftp);

	XDL_API bool_t		xtftp_recv(xhand_t tftp, byte_t* buf, dword_t *pb);

	XDL_API void		xtftp_abort(xhand_t tftp, int errcode);
	
	XDL_API void		xtftp_set_isdir(xhand_t tftp, bool_t dir);

	XDL_API bool_t		xtftp_get_isdir(xhand_t tftp);

	XDL_API void		xtftp_set_filesize(xhand_t tftp, dword_t size);

	XDL_API dword_t		xtftp_get_filesize(xhand_t tftp);

	XDL_API void		xtftp_set_filetime(xhand_t tftp, const tchar_t* ftime);

	XDL_API void		xtftp_get_filetime(xhand_t tftp, tchar_t* ftime);

	XDL_API void		xtftp_set_filename(xhand_t tftp, const tchar_t* fname);

	XDL_API void		xtftp_get_filename(xhand_t tftp, tchar_t* fname);

	XDL_API bool_t		 xtftp_head(xhand_t tftp);

	XDL_API bool_t		 xtftp_fetch(xhand_t tftp);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*XDLINET_H*/