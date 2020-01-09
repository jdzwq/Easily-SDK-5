/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firedate editor document

	@module	_if_firedate_win.c | firedate editor widnows implement file

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


typedef struct tagFireDateDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
}FireDateDelta;

#define GETFIREDATEDELTA(hWnd) 	((FireDateDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREDATEDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireDateProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


void	FireDate_SendNotify(HWND hWnd, int code, void* data);
void	FireDate_OnReset(HWND hWnd, int code);

ATOM RegisterFireDateClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireDateProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIREDATECLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubDateProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK FireDateProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	LPNMHDR lphdr;
	RECT rt;
	HIMC hMic;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireDateDelta*)xmem_alloc(sizeof(FireDateDelta));
		memset((void*)ptd,0,sizeof(FireDateDelta));

		GetClientRect(hWnd,&rt);

		ptd->hEdit = CreateWindowEx(0, DATETIMEPICK_CLASS, NULL, lpcs->style | WS_DISABLED, lpcs->x, lpcs->y, lpcs->cx, lpcs->cy, hWnd, NULL, lpcs->hInstance, NULL);
		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)SubDateProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit,TRUE);

		hMic = ImmGetContext(ptd->hEdit);
		ImmSetOpenStatus(hMic, FALSE);
		ImmReleaseContext(ptd->hEdit, hMic);

		SetFocus(ptd->hEdit);

		SETFIREDATEDELTA(hWnd,ptd);
		break;
	case WM_DESTROY:
		SETFIREDATEDELTA(hWnd,0);

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
			if(lphdr->hwndFrom == ptd->hEdit && lphdr->code == DTN_CLOSEUP)
			{
				if (IsWindow(ptd->hOwner))
					PostMessage(ptd->hEdit, WM_KEYDOWN, KEY_ENTER, 0);
				else
					SetFocus(ptd->hEdit);
			}else if(lphdr->hwndFrom == ptd->hEdit && lphdr->code == NM_KILLFOCUS)
			{
				if(IsWindow(ptd->hOwner))
					PostMessage(ptd->hEdit, WM_KEYDOWN, KEY_ENTER, 0);
				else
					SetFocus(ptd->hEdit);
			}
		}
		break;
	case FE_RESET:
		FireDate_OnReset(hWnd, wParam);
		return 0;
	case FE_ONKEY:
		if(wParam == KEY_ENTER) //enter key
		{
			FireDate_OnReset(hWnd, EC_COMMIT);
			return 1;
		}
		else if(wParam == KEY_ESC) //escape key
		{
			FireDate_OnReset(hWnd, EC_ROLLBACK);
			return 1;
		}
		return 0;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);	
}

void FireDate_OnReset(HWND hWnd, int code)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(hWnd);
	tchar_t sz_date[DATE_LEN + 1];

	if (code == EC_COMMIT)
	{
		firedate_get_text(hWnd, sz_date);

		FireDate_SendNotify(hWnd, EC_COMMIT, sz_date);
	}
	else if (code == EC_ROLLBACK)
	{
		FireDate_SendNotify(hWnd, EC_ROLLBACK, NULL);
	}
	else if (code == EC_CLEAN)
	{
		FireDate_SendNotify(hWnd, EC_COMMIT, NULL);
	}
}

void FireDate_SendNotify(HWND hWnd, int code, void* data)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(hWnd);
	NOTICE_EDITOR ne = { 0 };

	ne.widget = hWnd;
	ne.id = GetWindowLong(hWnd, GWL_ID);
	ne.code = code;
	ne.data = data;

	SendMessage(ptd->hOwner, FE_NOTICE, 0, (LPARAM)&ne);
}

/*****************************************************************************************/
res_win_t firedate_create(res_win_t widget, const xrect_t* pxr)
{
	RECT rt;
	HWND hEditor;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	MapWindowPoints(widget, HWND_DESKTOP, (LPPOINT)&rt, 2);

	hEditor = CreateWindow(FIREDATECLASS, NULL, WS_POPUP | WS_BORDER | WS_VISIBLE, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, widget, NULL, NULL, NULL);
	return (res_win_t)hEditor;
}

void firedate_set_font(res_win_t widget, const xfont_t* pxf)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(widget);
	LOGFONT lf;
	HFONT hf;
	HDC hDC;

	assert(pxf);

	hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	hDC = GetDC(widget);

	GetObject(hf, sizeof(LOGFONT), (void*)&lf);
	lf.lfHeight = -MulDiv(xstos(pxf->size), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = xstos(pxf->weight);

	ReleaseDC(widget, hDC);

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

	SendMessage(widget, WM_SETFONT, (WPARAM)ptd->hFont, MAKELPARAM(1, 0));
}

void firedate_set_owner(res_win_t fw, res_win_t owner)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(fw);

	ptd->hOwner = owner;
}

res_win_t firedate_get_owner(res_win_t fw)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(fw);

	return ptd->hOwner;
}

void firedate_get_text(res_win_t fw, tchar_t* buf)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(fw);

	SYSTEMTIME st;

	assert(buf);

	SendMessage(ptd->hEdit, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
	xsprintf(buf, _T("%04d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
}

void firedate_set_text(res_win_t fw, const tchar_t* szDate)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(fw);

	SYSTEMTIME st;
	xdate_t md;

	if (is_null(szDate))
		return;

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
