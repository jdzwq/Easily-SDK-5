/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tools document

	@module	_if_tools_win.c | shell api widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#include "_if_xdc.h"

#pragma comment(lib,"comctl32.lib")

res_win_t __cdecl _create_tooltip(res_win_t wt)
{
	INITCOMMONCONTROLSEX icex;
	HWND hwnd = NULL;
	TOOLINFO ti = { 0 };

	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_BAR_CLASSES;

	if (!InitCommonControlsEx(&icex))
		return NULL;

	hwnd = CreateWindow(TOOLTIPS_CLASS, TEXT(""), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, NULL, NULL);
	if (!IsWindow(hwnd))
	{
		return NULL;
	}

	ti.cbSize = sizeof(TOOLINFO) - 4;
	ti.uFlags = TTF_TRACK | TTF_ABSOLUTE;
	ti.hwnd = wt;
	ti.uId = (UINT)IDC_TOOLTIP;
	ti.hinst = NULL;
	ti.lpszText = LPSTR_TEXTCALLBACK;
	ti.rect.left = ti.rect.top = ti.rect.bottom = ti.rect.right = 0;

	if (!SendMessage(hwnd, TTM_ADDTOOL, 0, (LPARAM)&ti))
	{
		DestroyWindow(hwnd);
		return NULL;
	}

	SendMessage(wt, TTM_TRACKACTIVATE, (WPARAM)1, (LPARAM)&ti);

	return hwnd;
}

void __cdecl _show_tooltip(res_win_t wt, bool_t b, const xpoint_t* pxp)
{
	TOOLINFO ti = { 0 };

	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd = wt;
	ti.uId = (UINT)IDC_TOOLTIP;

	SendMessage(wt, TTM_TRACKACTIVATE, (WPARAM)b, (LPARAM)&ti);
	SendMessage(wt, TTM_TRACKPOSITION, 0, (LPARAM)MAKELPARAM(pxp->x, pxp->y));
}