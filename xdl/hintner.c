/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc text hintner document

	@module	hintner.c | implement file

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

#include "hintner.h"

#include "xdlimp.h"
#include "xdlstd.h"



#ifdef XDL_SUPPORT_VIEW

void hint_test(if_hint_t* pit, PF_HINT_CALLBACK pf, void* pp)
{
	bool_t b_focus = 0;
	bool_t b_drag = 0;
	bool_t b_sizew = 0;
	bool_t b_sizeh = 0;

	xrect_t xr;
	link_t_ptr xlk, ylk;
	int to = 0, ts = 0;
	
	typedef int(*PF_HINT_CALLBACK)(int state, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr, bool_t focus, bool_t drag, bool_t sizew, bool_t sizeh, void* pp);

	typedef int(*PF_HINT_NEXT)(void* param, link_t_ptr* p_xlk, link_t_ptr* p_ylk, xrect_t* p_rect, bool_t* p_focus, bool_t* p_drag, bool_t* p_sizew, bool_t* p_sizeh);

	to = (*pf)(_HINTNER_STATE_BEGIN, NULL, NULL, &xr, 0, 0, 0, 0, pp);

	if (to != _HINTNER_OPERA_STOP)
	{
		to = _HINTNER_OPERA_NEXT;
	}

	while (to != _HINTNER_OPERA_STOP)
	{
		switch (to)
		{
		case _HINTNER_OPERA_NEXT:

			(*pit->pf_hint_next)(pit->param, &xlk, &ylk, &xr, &b_focus, &b_drag, &b_sizew, &b_sizeh);
			ts = _HINTNER_STATE_OBJECT;
			break;
		}

		switch (ts)
		{
		case _HINTNER_STATE_OBJECT:
			to = (*pf)(_HINTNER_STATE_BEGIN, NULL, NULL, &xr, b_focus, b_drag, b_sizew, b_sizeh, pp);
			break;
		}

		if (to != _HINTNER_OPERA_NEXT)
			continue;
	}

}

#endif