/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc resource document

	@module	impres.c | resource implement file

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

#include "impres.h"
#include "xdlinit.h"
#include "impmem.h"
#include "imperr.h"

#ifdef XDK_SUPPORT_RES

void get_screen_size(xsize_t* pxs)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	(*pif->pf_get_screen_size)(pxs);
}

void get_desktop_size(xsize_t* pxs)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	(*pif->pf_get_desktop_size)(pxs);
}

void screen_size_to_pt(xsize_t* pxs)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	(*pif->pf_screen_size_to_pt)(pxs);
}

void screen_size_to_tm(xsize_t* pxs)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	(*pif->pf_screen_size_to_tm)(pxs);
}

#ifdef XDK_SUPPORT_RES_ACCEL

res_acl_t create_accel_table(const accel_t* pac, int n)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	return (*pif->pf_create_accel_table)(pac, n);
}

void destroy_accel_table(res_acl_t hac)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	(*pif->pf_destroy_accel_table)(hac);
}

#endif /*XDK_SUPPORT_RES_ACCEL*/

#ifdef XDK_SUPPORT_RES_REGION

res_rgn_t create_region(const tchar_t* shape, const xrect_t* pxr)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	return (*pif->pf_create_region)(shape, pxr);
}

void delete_region(res_rgn_t rgn)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	(*pif->pf_delete_region)(rgn);
}

bool_t pt_in_region(res_rgn_t rgn, const xpoint_t* ppt)
{
	if_resource_t* pif;

	pif = PROCESS_RESOURCE_INTERFACE;

	return (*pif->pf_pt_in_region)(rgn, ppt);
}

#endif /*XDK_SUPPORT_RES_REGION*/

#endif /*XDK_SUPPORT_RES*/
