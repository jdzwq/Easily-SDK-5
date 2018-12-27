/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl commole document

	@module	impcomm.h | xdl commole interface file

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

#ifndef _IMPCOMM_H
#define _IMPCOMM_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_COMM

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void xcomm_default_mode(dev_com_t* pmod);

XDL_API bool_t xcomm_set_mode(xhand_t com, const dev_com_t* pmod);

XDL_API bool_t xcomm_get_mode(xhand_t com, dev_com_t* pmod);

XDL_API xhand_t xcomm_open(const tchar_t* fname, dword_t fmode);

XDL_API void xcomm_close(xhand_t com);

XDL_API dword_t xcomm_listen(xhand_t com, dword_t* pcb);

XDL_API bool_t xcomm_write(xhand_t com, const byte_t* buf, dword_t* pb);

XDL_API bool_t xcomm_flush(xhand_t com);

XDL_API bool_t xcomm_read(xhand_t com, byte_t* buf, dword_t* pb);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_COMM*/

#endif /*IMPCOMM_H*/