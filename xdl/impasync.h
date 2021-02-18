/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc async document

	@module	impasync.h | interface file

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

#ifndef _IMPASYNC_H
#define	_IMPASYNC_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_ASYNC

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION async_init: init async operation resource.
@INOUTPUT int type: the async mode, it can be ASYNC_BLOCK, ASYNC_EVENT, ASYNC_QUEUE.
@INPUT int ms: timeout in millisecond, -1 indicate INFINITE
@INPUT res_file_t fd: if type is ASYNC_QUEUE, this param is the input output handle for creating queue.
@RETURN  void: none.
*/
EXP_API void async_init(async_t* pas, int type, int ms, res_file_t fd);

/*
@FUNCTION async_uninit: uninit async operation resource.
@INPUT async_t* pas: the async struct for releasing background resource.
@RETURN void: none.
*/
EXP_API void async_uninit(async_t* pas);


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_ASYNC*/

#endif	/*_IMPASYNC_H*/

