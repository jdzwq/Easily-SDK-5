/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc asn1 document

	@module	oemasn.h | asn interface file

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

#ifndef _OEMASN_H
#define	_OEMASN_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_ASN

/*
* ASN1 DER TAG
*/
#define XASN_BOOLEAN                 0x01
#define XASN_INTEGER                 0x02
#define XASN_BIT_STRING              0x03
#define XASN_OCTET_STRING            0x04
#define XASN_NULL                    0x05
#define XASN_OID                     0x06
#define XASN_UTF8_STRING             0x0C
#define XASN_SEQUENCE                0x10
#define XASN_SET                     0x11
#define XASN_PRINTABLE_STRING        0x13
#define XASN_T61_STRING              0x14
#define XASN_IA5_STRING              0x16
#define XASN_UTC_TIME                0x17
#define XASN_UNIVERSAL_STRING        0x1C
#define XASN_BMP_STRING              0x1E
#define XASN_PRIMITIVE               0x00
#define XASN_CONSTRUCTED             0x20
#define XASN_CONTEXT_SPECIFIC        0x80

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API dword_t xasn_get_tag(byte_t *buf, int* ptag);

	XDL_API dword_t xasn_set_tag(byte_t* buf, int tag);

	XDL_API dword_t xasn_get_len(byte_t* buf, int *plen);

	XDL_API dword_t xasn_set_len(byte_t* buf, int len);

	XDL_API dword_t xasn_get_bool(byte_t* buf, bool_t *pval);

	XDL_API dword_t xasn_set_bool(byte_t* buf, bool_t val);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_ASN*/

#endif	/*OEMASN_H */

