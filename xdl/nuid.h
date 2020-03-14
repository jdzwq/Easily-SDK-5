/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc nuid document

	@module	nuid.h | interface file

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

#ifndef _NUID_H
#define _NUID_H

#include "xdldef.h"

typedef struct _nuid_t {
	unsigned long data1;  
	unsigned short data2;  
	unsigned short data3;  
	unsigned char data4[8];
} nuid_t;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API void nuid_zero(nuid_t* pu);

	EXP_API void nuid_from_timestamp(nuid_t* pu, lword_t tms);

	EXP_API lword_t nuid_to_timestamp(nuid_t* pu);

	EXP_API void nuid_from_md5(nuid_t* pu, byte_t buf[16]);

	EXP_API void nuid_to_md5(nuid_t* pu, byte_t buf[16]);

	EXP_API void nuid_parse_string(nuid_t* pu, const tchar_t* buf, int len);

	EXP_API int nuid_format_string(nuid_t* pu, tchar_t* buf, int max);

#if defined(_DEBUG) || defined(DEBUG)
	EXP_API void test_nuid(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_NUID_H*/
