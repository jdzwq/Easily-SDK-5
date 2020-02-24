/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc resource document

	@module	impres.h | resource interface file

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

#ifndef _IMPRES_H
#define _IMPRES_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_RES

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void get_screen_size(xsize_t* pxs);

XDL_API void get_desktop_size(xsize_t* pxs);

XDL_API void screen_size_to_pt(xsize_t* pxs);

XDL_API void screen_size_to_tm(xsize_t* pxs);

#ifdef XDK_SUPPORT_RES_ACCEL
XDL_API res_acl_t create_accel_table(const accel_t* pac, int n);

XDL_API void destroy_accel_table(res_acl_t hac);
#endif

#ifdef XDK_SUPPORT_RES_REGION
XDL_API res_rgn_t create_region(const tchar_t* shape, const xrect_t* pxr);

XDL_API void delete_region(res_rgn_t rgn);

XDL_API bool_t pt_in_region(res_rgn_t rgn, const xpoint_t* ppt);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_RES*/

#endif /*IMPRES_H*/