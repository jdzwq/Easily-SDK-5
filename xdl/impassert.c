/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc assert document

	@module	impassert.c | assert implement file

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

#include "impassert.h"
#include "impjmp.h"
#include "imperr.h"
#include "xdlinit.h"


void xdl_assert(const char* _Expr, const char* _File, unsigned int _Line)
{
	if_error_t* pie;
#ifdef XDK_SUPPORT_THREAD
	if_thread_t* pit;
#endif

	pie = PROCESS_ERROR_INTERFACE;

#if defined(_DEBUG) || defined(DEBUG)
	if (pie)
	{
		(*pie->pf_error_print)(_Expr);
	}
#endif
#ifdef XDK_SUPPORT_THREAD
    
	pit = PROCESS_THREAD_INTERFACE;

	if (pit)
	{
		clear_jump();

		if ((*pit->pf_thread_get_id)() == g_xdl_mou.thread_id)
		{//the primary thread
			xdl_thread_uninit(-1);
			if(pie)
			{
				(*pie->pf_error_exit)();
			}
		}
		else
		{//the working thread
			xdl_thread_uninit(-1);
			if (pit)
			{
				(*pit->pf_thread_end)();
			}
		}
	}
#else
	pie = PROCESS_ERROR_INTERFACE;

	clear_jump();

	if (pie)
	{
		(*pie->pf_error_exit)();
	}
#endif
}

