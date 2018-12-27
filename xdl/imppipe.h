/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl pipe document

	@module	imppipe.h | xdl pipe interface file

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

#ifndef _IMPPIPE_H
#define _IMPPIPE_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_PIPE

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xpipe_srv(const tchar_t* pname, dword_t fmode);

XDL_API xhand_t xpipe_cli(const tchar_t* pname, dword_t fmode);

XDL_API bool_t xpipe_listen(xhand_t pip, int ms);

XDL_API void xpipe_stop(xhand_t pip);

XDL_API void xpipe_free(xhand_t pip);

XDL_API bool_t xpipe_write(xhand_t pip, const byte_t* data, dword_t* pb);

XDL_API bool_t xpipe_read(xhand_t pip, byte_t* buf, dword_t* pb);

XDL_API xhand_t xpipe_attach(res_file_t hp);

XDL_API res_file_t xpipe_detach(xhand_t pip);

XDL_API res_file_t xpipe_handle(xhand_t pip);

XDL_API bool_t xpipe_wait(const tchar_t* pname, int ms);

XDL_API bool_t xpipe_flush(xhand_t pip);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_PIPE*/

#endif /*IMPPIPE_H*/