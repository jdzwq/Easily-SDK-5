/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firetime editor document

	@module	_if_firetime_win.c | firetime editor widnows implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#include "xdcinit.h"
#include "xdcutil.h"
#include "xdceditor.h"

typedef struct tagFireTimeDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
}FireTimeDelta;

#define GETFIRETIMEDELTA(hWnd) 	((FireTimeDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIRETIMEDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireTimeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


void	FireTime_SendNotify(HWND hWnd, int code, void* data);
void	FireTime_OnReset(HWND hWnd, int code);

ATOM RegisterFireTimeClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireTimeProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);/*(COLOR_WINDOW+1);*/
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIRETIMECLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubTimeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd,GWLP_USERDATA);

	switch(message)
	{
	case WM_KEYDOWN:
		if(SendMessage(GetParent(hWnd),FE_ONKEY,wParam,lParam))
			return 0;
		break;
	default:
		break;
	}

	if(lpfOrgProc)
		return CallWindowProc(lpfOrgProc,hWnd,message,wParam,lParam);
	else
		return DefWindowProc(hWnd,message,wParam,lParam);			
}

LRESULT CALLBACK FireTimeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	LPNMHDR lphdr;
	RECT rt;
	HIMC hMic;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireTimeDelta*)xmem_alloc(sizeof(FireTimeDelta));
		memset((void*)ptd,0,sizeof(FireTimeDelta));

		GetClientRect(hWnd,&rt);

		ptd->hEdit = CreateWindowEx(0, DATETIMEPICK_CLASS, NULL, lpcs->style | DTS_TIMEFORMAT | WS_DISABLED, lpcs->x, lpcs->y, lpcs->cx, lpcs->cy, hWnd, NULL, lpcs->hInstance, NULL);
		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)SubTimeProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit,TRUE);

		hMic = ImmGetContext(ptd->hEdit);
		ImmSetOpenStatus(hMic, FALSE);
		ImmReleaseContext(ptd->hEdit, hMic);

		SetFocus(ptd->hEdit);

		SETFIRETIMEDELTA(hWnd,ptd);
		break;
	case WM_DESTROY:
		SETFIRETIMEDELTA(hWnd,0);

		if(IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit,FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if(ptd->hFont)
			DeleteObject(ptd->hFont);

		xmem_free(ptd);
		break;
	case WM_NOTIFY:
		if(ptd)
		{
			lphdr = (LPNMHDR)lParam;
			if(lphdr->hwndFrom == ptd->hEdit && lphdr->code == NM_KILLFOCUS)
			{
				if(IsWindow(ptd->hOwner))
					PostMessage(ptd->hEdit, WM_KEYDOWN, KEY_ENTER, 0);
				else
					SetFocus(ptd->hEdit);
			}
		}
		break;
	case FE_RESET:
		FireTime_OnReset(hWnd, wParam);
		return 0;
	case FE_ONKEY:
		if(wParam == KEY_ENTER) //enter key
		{
			FireTime_OnReset(hWnd, EC_COMMIT);
			return 1;
		}
		else if(wParam == KEY_ESC) //escape key
		{
			FireTime_OnReset(hWnd, EC_ROLLBACK);
			return 1;
		}
		return 0;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);	
}

void FireTime_OnReset(HWND hWnd, int code)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(hWnd);
	tchar_t sz_time[DATE_LEN + 1];

	if (code == EC_COMMIT)
	{
		firetime_get_text(hWnd, sz_time);

		FireTime_SendNotify(hWnd, EC_COMMIT, sz_time);
	}
	else if (code == EC_ROLLBACK)
	{
		FireTime_SendNotify(hWnd, EC_ROLLBACK, NULL);
	}
	else if (code == EC_CLEAN)
	{
		FireTime_SendNotify(hWnd, EC_COMMIT, NULL);
	}
}

void FireTime_SendNotify(HWND hWnd, int code, void* data)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(hWnd);
	NOTICE_EDITOR ne = { 0 };

	ne.widget = hWnd;
	ne.id = GetWindowLong(hWnd, GWL_ID);
	ne.code = code;
	ne.data = data;

	SendMessage(ptd->hOwner, FE_NOTICE, 0, (LPARAM)&ne);
}

/*************************************************************************************/
res_win_t firetime_create(res_win_t widget, const xrect_t* pxr)
{
	RECT rt;
	HWND hEditor;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	MapWindowPoints(widget, HWND_DESKTOP, (LPPOINT)&rt, 2);

	hEditor = CreateWindow(FIRETIMECLASS, NULL, WS_POPUP | WS_BORDER | WS_VISIBLE, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, widget, NULL, NULL, NULL);
	return (res_win_t)hEditor;
}

void firetime_set_font(res_win_t fw, const xfont_t* pxf)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(fw);
	LOGFONT lf;
	HFONT hf;
	HDC hDC;

	assert(pxf);

	hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	hDC = GetDC(fw);

	GetObject(hf, sizeof(LOGFONT), (void*)&lf);
	lf.lfHeight = -MulDiv(xstos(pxf->size), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = xstos(pxf->weight);

	ReleaseDC(fw, hDC);

	if (xsicmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = (BYTE)TRUE;
	}
	else if (xsicmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = (BYTE)TRUE;
	}
	else if (xsicmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = (BYTE)TRUE;
	}
	xscpy(lf.lfFaceName, pxf->family);

	ptd->hFont = CreateFontIndirect(&lf);

	SendMessage(fw, WM_SETFONT, (WPARAM)ptd->hFont, MAKELPARAM(1, 0));
}

void firetime_get_text(res_win_t fw, tchar_t* buf)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(fw);

	SYSTEMTIME st;

	assert(buf);

	SendMessage(ptd->hEdit, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
	xsprintf(buf, _T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
}

void firetime_set_text(res_win_t fw, const tchar_t* szTime)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(fw);

	SYSTEMTIME st;
	xdate_t md;
	tchar_t szDate[DATE_LEN + 1] = { 0 };

	if (is_null(szTime))
		return;

	get_loc_date(&md);
	xsprintf(szDate, _T("%d-%02d-%02d %s"),md.year,md.mon,md.day,szTime);

	parse_datetime(&md, szDate);

	memset((void*)&st, 0, sizeof(SYSTEMTIME));
	st.wDay = md.day;
	st.wHour = md.hour;
	st.wMinute = md.min;
	st.wMonth = md.mon;
	st.wSecond = md.sec;
	st.wYear = md.year;
	SendMessage(ptd->hEdit, DTM_SETSYSTEMTIME, 0, (LPARAM)&st);
}

void firetime_set_owner(res_win_t fw, res_win_t owner)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(fw);

	ptd->hOwner = owner;
}

res_win_t firetime_get_owner(res_win_t fw)
{
	FireTimeDelta* ptd = GETFIRETIMEDELTA(fw);

	return ptd->hOwner;
}