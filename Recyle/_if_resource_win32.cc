/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc shell document

	@module	_if_shell_win.c | shell api widnows implement file

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

#ifdef XDK_SUPPORT_RES

#ifdef WINCE
static long MulDiv(long a, long b, long c)
{
	return (long)((float)a * (float)b / (float)c);
}
#endif

#ifdef XDK_SUPPORT_RES_SCREEN
void _get_screen_size(xsize_t* pxs)
{
	pxs->cx = GetSystemMetrics(SM_CXFULLSCREEN);
	pxs->cy = GetSystemMetrics(SM_CYFULLSCREEN);
}

void _get_desktop_size(xsize_t* pxs)
{
	RECT rt;

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	pxs->cx = rt.right - rt.left;
	pxs->cy = rt.bottom - rt.top;
}

void _screen_size_to_tm(xsize_t* pxs)
{
	HDC hDC;
	float htpermm, vtpermm;
	float cx, cy;

	hDC = GetDC(NULL);

	htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	cx = (float)((float)pxs->cx / htpermm);
	cy = (float)((float)pxs->cy / vtpermm);

	pxs->fx = cx;
	pxs->fy = cy;

	ReleaseDC(NULL, hDC);
}

void _screen_size_to_pt(xsize_t* pxs)
{
	HDC hDC;
	float htpermm, vtpermm;
	long cx, cy;

	hDC = GetDC(NULL);

	htpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSX) * INCHPERMM);
	vtpermm = (float)((float)GetDeviceCaps(hDC, LOGPIXELSY) * INCHPERMM);

	cx = (long)((float)pxs->fx * htpermm);
	cy = (long)((float)pxs->fy * vtpermm);

	pxs->cx = cx;
	pxs->cy = cy;

	ReleaseDC(NULL, hDC);
}
#endif

#ifdef XDK_SUPPORT_RES_ACCEL
res_acl_t _create_accel_table(const accel_t* pac, int n)
{
	HACCEL hac;
	ACCEL* pa;
	int i;

	pa = (ACCEL*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ACCEL) * n);
	for (i = 0; i < n; i++)
	{
		pa[i].fVirt = (BYTE)pac[i].vir;
		pa[i].key = (WORD)pac[i].key;
		pa[i].cmd = (WORD)pac[i].cmd;
	}

	hac = CreateAcceleratorTable(pa, n);
	HeapFree(GetProcessHeap(), 0, pa);

	return (res_acl_t)hac;
}

void _destroy_accel_table(res_acl_t hac)
{
	DestroyAcceleratorTable(hac);
}
#endif

#ifdef XDK_SUPPORT_RES_REGION
res_rgn_t _create_region(const tchar_t* shape, const xrect_t* pxr)
{
	RECT rt;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	if (_tstrcmp(shape, GDI_ATTR_REGION_RECT) == 0)
		return CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);
	else if (_tstrcmp(shape, GDI_ATTR_REGION_ROUND) == 0)
		return CreateRoundRectRgn(rt.left, rt.top, rt.right, rt.bottom, 10, 10);
	else if (_tstrcmp(shape, GDI_ATTR_REGION_ELLIPSE) == 0)
		return CreateEllipticRgn(rt.left, rt.top, rt.right, rt.bottom);
	else
		return NULL;
}

void _delete_region(res_rgn_t rgn)
{
	DeleteObject(rgn);
}

bool_t _pt_in_region(res_rgn_t rgn, const xpoint_t* ppt)
{
	return (PtInRegion(rgn, ppt->x, ppt->y)) ? 1 : 0;
}
#endif //XDK_SUPPORT_RES_REGION

#endif //XDK_SUPPORT_RES