﻿/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc region document

	@module	if_region.c | region xwindows implement file

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

#include "xdkiml.h"
#include "xdkattr.h"

#ifdef XDK_SUPPORT_CONTEXT_REGION

res_rgn_t _create_region(const tchar_t* shape, const xrect_t* pxr)
{
    return XCreateRegion();
}

void _delete_region(res_rgn_t rgn)
{
	XDestroyRegion(rgn);
}

bool_t _pt_in_region(res_rgn_t rgn, const xpoint_t* ppt)
{
	return (XPointInRegion(rgn, ppt->x, ppt->y)) ? 1 : 0;
}


#endif //XDK_SUPPORT_CONTEXT_REGION
