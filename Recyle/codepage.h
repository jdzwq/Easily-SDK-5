/***********************************************************************
	Easily xdl codepage

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc codepage document

	@module	codepage.h | codepage interface file

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

#ifndef _CODEPAGE_H
#define _CODEPAGE_H


extern int __stdcall unicode_gb2312_size(void);

extern void __stdcall unicode_gb2312_code(int index, unsigned short* code, unsigned short* val, unsigned short* key);

extern int __stdcall gb2312_unicode_size(void);

extern void __stdcall gb2312_unicode_code(int index, unsigned short* code, unsigned short* val, unsigned short* key);

#endif /*_CODEPAGE_H*/
