/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firerich editor document

	@module	_if_firerich_win.c | firerich editor widnows implement file

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

typedef struct tagFireRichDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
}FireRichDelta;

#define GETFIRERICHDELTA(hWnd) 	((FireRichDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIRERICHDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireRichProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SubRichProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


void	FireRich_SendNotify(HWND hWnd, int code, void* data);

void	FireRich_OnReset(HWND hWnd, int code);

ATOM RegisterFireRichClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireRichProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIRERICHCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubRichProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK FireRichProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(hWnd);;
	LPCREATESTRUCT lpcs;
	RECT rt;
	WNDPROC lpfOrgProc;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireRichDelta*)xmem_alloc(sizeof(FireRichDelta));
		memset((void*)ptd,0,sizeof(FireRichDelta));
		SETFIRERICHDELTA(hWnd,ptd);

		GetClientRect(hWnd,&rt);
		ptd->hEdit = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)SubRichProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);
		
		EnableWindow(ptd->hEdit,TRUE);
		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIRERICHDELTA(hWnd,0);

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
	case WM_COMMAND:
		if(ptd)
		{
			if(HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit)
			{
				PostMessage(ptd->hEdit, WM_KEYDOWN, KEY_ENTER, 0);
			}
		}
		break;
	case FE_RESET:
		FireRich_OnReset(hWnd, wParam);
		return 0;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

void FireRich_SendNotify(HWND hWnd, int code, void* data)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(hWnd);
	NOTICE_EDITOR ne = { 0 };

	ne.widget = hWnd;
	ne.id = GetWindowLong(hWnd, GWL_ID);
	ne.code = code;
	ne.data = data;

	SendMessage(ptd->hOwner, FE_NOTICE, 0, (LPARAM)&ne);
}

void FireRich_OnReset(HWND hWnd, int code)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(hWnd);

	if (code == EC_COMMIT)
	{
		int len = GetWindowTextLength(ptd->hEdit);
		tchar_t *token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
		GetWindowText(ptd->hEdit, token, len + 1);

		FireRich_SendNotify(hWnd, EC_COMMIT, token);

		xmem_free(token);
	}
	else if (code == EC_ROLLBACK)
	{
		FireRich_SendNotify(hWnd, EC_ROLLBACK, NULL);
	}
	else if (code == EC_CLEAN)
	{
		FireRich_SendNotify(hWnd, EC_COMMIT, NULL);
	}
}

/*********************************************************************************/
res_win_t firerich_create(res_win_t widget, const xrect_t* pxr)
{
	RECT rt;
	HWND hEditor;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	MapWindowPoints(widget, HWND_DESKTOP, (LPPOINT)&rt, 2);

	hEditor = CreateWindow(FIRERICHCLASS, NULL, WS_POPUP | WS_VISIBLE , rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, widget, NULL, NULL, NULL);
	return (res_win_t)hEditor;
}

void firerich_set_font(res_win_t fw, const xfont_t* pxf)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(fw);
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

void firerich_set_owner(res_win_t fw, res_win_t owner)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(fw);

	ptd->hOwner = owner;
}

res_win_t firerich_set_owner(res_win_t fw)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(fw);

	return ptd->hOwner;
}

void firerich_set_text(res_win_t fw, const tchar_t* token)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(fw);

	SetWindowText(ptd->hEdit, token);
	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
}

int firemeo_get_text(res_win_t fw, tchar_t* buf, int max)
{
	FireRichDelta* ptd = GETFIRERICHDELTA(fw);

	return GetWindowText(fw, buf, max);
}