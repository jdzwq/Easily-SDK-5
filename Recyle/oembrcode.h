/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc code brcode document

	@module	oembrcode.h | code brcode third party wrap interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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

#ifndef _OEMBRCODE_H
#define	_OEMBRCODE_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_BRCODE

XDL_API int pdf417_encode(const byte_t* token, int len, byte_t* buf, int max, int* prows, int* pcols);

XDL_API int code128_encode(const byte_t* token, int len, byte_t* buf, int max);

#endif //XDL_SUPPORT_BRCODE

#endif	// OEMBRCODE_H 

