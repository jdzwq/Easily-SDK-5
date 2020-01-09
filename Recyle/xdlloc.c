/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc local document

	@module	xdlloc.c | local implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#include "xdlloc.h"
#include "xdlinit.h"
#include "xdlcode.h"
#include "xdlstr.h"

int loc_to_gb2312(const byte_t* src, int len, byte_t* buf, int max)
{
	switch (g_xdl_mou.if_loc)
	{
	case _GB2312:
		len = (len < 0) ? a_xslen((schar_t*)src) : len;
		if (buf)
		{
			len = (len < max) ? len : max;
			a_xsncpy((schar_t*)buf, (schar_t*)src, len);
		}
		return len;
	case _UTF8:

	case _UTF16:
		return gb2312_encode((ucode_t*)src, len, buf, max);
	}

	return 0;
}
