/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firenum editor document

	@module	_if_firenum_win.c | firenum editor widnows implement file

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
#include "xdcwidg.h"
#include "xdceditor.h"
#include "stringctrl.h"

typedef struct tagFireNumDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
	HWND hNum;
}FireNumDelta;

#define GETFIRENUMDELTA(hWnd) 	((FireNumDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIRENUMDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireNumProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DropEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void	FireNum_AttachData(HWND hWnd,LINKPTR ptr);
LINKPTR	FireNum_DetachData(HWND hWnd);

void	FireNum_SendNotify(HWND hWnd, int code, void* data);
void	FireNum_Relayout(HWND hWnd);
void	FireNum_OnNotice(HWND hWnd, NOTICE_STRING* pnt);
void	FireNum_OnReset(HWND hWnd, int code);

ATOM RegisterFireNumClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireNumProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIRENUMCLASS;

	return RegisterClass(&wcex);
}


static LRESULT CALLBACK EditSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd,GWLP_USERDATA);

	switch(message)
	{
	case WM_KEYDOWN:
		if(SendMessage(GetParent(hWnd),FE_ONKEY,wParam,lParam))
			return 0;
		break;
	case WM_CHAR:
		if ((wParam < _T('0') || wParam > _T('9')) && wParam != _T('.'))
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

LRESULT CALLBACK FireNumProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	RECT rt;
	HIMC hMic;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireNumDelta*)xmem_alloc(sizeof(FireNumDelta));
		memset((void*)ptd,0,sizeof(FireNumDelta));
		SETFIRENUMDELTA(hWnd,ptd);

		GetClientRect(hWnd,&rt);
		ptd->hEdit = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED | ES_AUTOHSCROLL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)EditSubProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit,TRUE);

		hMic = ImmGetContext(ptd->hEdit);
		ImmSetOpenStatus(hMic, FALSE);
		ImmReleaseContext(ptd->hEdit, hMic);

		if (XDC_TOUCHED)
		{
			ptd->hNum = CreateWindow(NUMCTRLCLASS, NULL, WS_POPUP | WS_BORDER, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
		}

		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIRENUMDELTA(hWnd,0);

		if(IsWindow(ptd->hNum))
		{
			DestroyWindow(ptd->hNum);
		}

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
		if(HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit && IsWindow(ptd->hNum))
		{
			PostMessage(ptd->hEdit, WM_KEYDOWN, 13, 0);
		}
		break;
	case WM_CHAR:
		if (ptd->hEdit)
		{
			SendMessage(ptd->hEdit, WM_CHAR, wParam, lParam);
		}
		return 0;
	case FE_RESET:
		FireNum_OnReset(hWnd, wParam);
		return 0;
	case FE_ONKEY:
		if(wParam == KEY_ENTER) //enter key
		{
			FireNum_OnReset(hWnd, EC_COMMIT);
			return 1;
		}
		else if(wParam == KEY_ESC) //escape key
		{
			FireNum_OnReset(hWnd, EC_ROLLBACK);
			return 1;
		}
		return 0;
	}

	return DefWindowProc(hWnd,message,wParam,lParam);		
}

void FireNum_SendNotify(HWND hWnd, int code, void* data)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(hWnd);
	NOTICE_EDITOR ne = { 0 };

	ne.widget = hWnd;
	ne.id = GetWindowLong(hWnd, GWL_ID);
	ne.code = code;
	ne.data = data;

	SendMessage(ptd->hOwner, FE_NOTICE, 0, (LPARAM)&ne);
}

void FireNum_Relayout(HWND hWnd)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(hWnd);
	long cy,cx;
	RECT rt,rtDesk;

	cx = DEF_NUMCTRL_CX;
	cy = DEF_NUMCTRL_CY;

	GetWindowRect(GetDesktopWindow(), &rtDesk);

	if (cx > rtDesk.right - rtDesk.left)
		cx = rtDesk.right - rtDesk.left;
	if (cy > rtDesk.bottom - rtDesk.top)
		cy = rtDesk.bottom - rtDesk.top;

	GetWindowRect(ptd->hEdit, &rt);

	if (rt.bottom + cy > rtDesk.bottom)
	{
		rt.bottom = rt.top;
		rt.top = rt.bottom - cy;
	}
	else
	{
		rt.top = rt.bottom;
		rt.bottom = rt.top + cy;
	}

	if (rt.left + cx > rtDesk.right)
	{
		rt.right = rtDesk.right;
		rt.left = rt.right - cx;
	}
	else
	{
		rt.right = rt.left + cx;
	}

	SetWindowPos(ptd->hNum, HWND_TOP, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

void FireNum_OnReset(HWND hWnd, int code)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(hWnd);

	if (code == EC_COMMIT)
	{
		int len = GetWindowTextLength(ptd->hEdit);
		tchar_t *token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
		GetWindowText(ptd->hEdit, token, len + 1);

		FireNum_SendNotify(hWnd, EC_COMMIT, token);

		xmem_free(token);
	}
	else if (code == EC_ROLLBACK)
	{
		FireNum_SendNotify(hWnd, EC_ROLLBACK, NULL);
	}
	else if (code == EC_CLEAN)
	{
		FireNum_SendNotify(hWnd, EC_COMMIT, NULL);
	}
}

/************************************************************************************************************************/
res_win_t firenum_create(res_win_t widget, const xrect_t* pxr)
{
	RECT rt;
	HWND hEditor;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	MapWindowPoints(widget, HWND_DESKTOP, (LPPOINT)&rt, 2);

	hEditor = CreateWindow(FIRENUMCLASS, NULL, WS_POPUP | WS_BORDER | WS_VISIBLE, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, widget, NULL, NULL, NULL);
	if (!IsWindow(hEditor))
		return NULL;
	
	FireNum_Relayout(hEditor);

	return (res_win_t)hEditor;
}

void firenum_set_font(res_win_t fw, const xfont_t* pxf)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(fw);
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

void firenum_set_text(res_win_t fw, const tchar_t* token)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(fw);

	SetWindowText(ptd->hEdit, token);
	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
}

int firenum_get_text(res_win_t fw, tchar_t* buf, int max)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(fw);

	return GetWindowText(fw, buf, max);
}

void firenum_set_owner(res_win_t fw, res_win_t owner)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(fw);

	ptd->hOwner = owner;
}

res_win_t firenum_get_owner(res_win_t fw)
{
	FireNumDelta* ptd = GETFIRENUMDELTA(fw);

	return ptd->hOwner;
}