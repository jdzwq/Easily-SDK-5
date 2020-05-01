﻿/***********************************************************************
	Easily hl7 web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hl7 defination document

	@module	hl7api.h | hl7 web api definition interface file

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

#ifndef _HL7API_H
#define _HL7API_H

#include <xdl.h>
#include <hexdfs.h>

#define HL7_ELEMENT_TIMESTAMP	_T("timestamp")
#define HL7_ELEMENT_LONGITUDE	_T("longitude")
#define HL7_ELEMENT_LATITUDE	_T("latitude")
#define HL7_ELEMENT_ALTITUDE	_T("altitude")
#define HL7_ELEMENT_IDENTIFY	_T("identify")
#define HL7_ELEMENT_PACKAGE		_T("package")
#define HL7_ELEMENT_PACKSIZE	_T("packsize")

#define HL7VER	"HL11"
#define HL7VER_SIZE			4
#define HL7HANDER_SIZE		8
#define HL7HEADER_SIZE		(IDENTIFY_SIZE + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE)

#ifdef	__cplusplus
extern "C" {
#endif
    
extern int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb);

#ifdef	__cplusplus
}
#endif

#endif