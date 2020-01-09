/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firedrop editor document

	@module	_if_firedrop_win.c | firedrop editor widnows implement file

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

typedef struct tagFireDropDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
	HWND hList;
	HWND hKey;
	LINKPTR data;
}FireDropDelta;

#define GETFIREDROPDELTA(hWnd) 	((FireDropDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREDROPDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireDropProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DropEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void	FireDrop_AttachData(HWND hWnd,LINKPTR ptr);
LINKPTR	FireDrop_DetachData(HWND hWnd);

void	FireDrop_SendNotify(HWND hWnd, int code, void* data);
void	FireDrop_OnNotice(HWND hWnd, NOTICE_STRING* pnt);
void	FireDrop_OnReset(HWND hWnd, int code);

void	FireDrop_RelayoutList(HWND hWnd);
void	FireDrop_RelayoutKey(HWND hWnd);

ATOM RegisterFireDropClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireDropProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIREDROPCLASS;

	return RegisterClass(&wcex);
}


LRESULT CALLBACK DropEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd,GWLP_USERDATA);

	switch(message)
	{
	case WM_MOUSEWHEEL:
		if(SendMessage(GetParent(hWnd),WM_MOUSEWHEEL,wParam,lParam))
			return 0;
		break;
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

LRESULT CALLBACK FireDropProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	RECT rt;
	HIMC hMic;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireDropDelta*)xmem_alloc(sizeof(FireDropDelta));
		memset((void*)ptd,0,sizeof(FireDropDelta));
		SETFIREDROPDELTA(hWnd,ptd);

		GetClientRect(hWnd,&rt);
		ptd->hEdit = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED | ES_AUTOHSCROLL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)DropEditProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit,TRUE);

		hMic = ImmGetContext(ptd->hEdit);
		ImmSetOpenStatus(hMic, FALSE);
		ImmReleaseContext(ptd->hEdit, hMic);

		if (XDC_TOUCHED)
		{
			ptd->hKey = CreateWindow(KEYCTRLCLASS, NULL, WS_POPUP | WS_BORDER, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
			FireDrop_RelayoutKey(hWnd);
		}

		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIREDROPDELTA(hWnd,0);

		if (IsWindow(ptd->hKey))
		{
			DestroyWindow(ptd->hKey);
		}

		if(IsWindow(ptd->hList))
		{
			DestroyWindow(ptd->hList);
		}

		if(IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit,FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if (ptd->data)
			destroy_string_doc(ptd->data);

		if(ptd->hFont)
			DeleteObject(ptd->hFont);

		xmem_free(ptd);
		break;
	case WM_COMMAND:
		if(HIWORD(wParam) == EN_CHANGE && (HWND)lParam == ptd->hEdit && IsWindow(ptd->hList))
		{
			int len = GetWindowTextLength(ptd->hEdit);
			tchar_t *token = xsalloc(len + 1);
			GetWindowText(ptd->hEdit, token, len + 1);
			string_view_filter(ptd->hList, token);
			xsfree(token);

			FireDrop_RelayoutList(hWnd);
		}else if(HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit && IsWindow(ptd->hList))
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
	case WM_MOUSEWHEEL:
		if(ptd->hList)
		{
			PostMessage(ptd->hList,WM_MOUSEWHEEL,wParam,lParam);
			return 1;
		}
		return 0;
	case FE_RESET:
		FireDrop_OnReset(hWnd, wParam);
		return 0;
	case FE_ONKEY:
		if(wParam == VK_DOWN)
		{
			string_view_tabskip(ptd->hList, NF_DOWN);
			return 1;
		}
		else if(wParam == VK_UP)
		{
			string_view_tabskip(ptd->hList, NF_UP);
			return 1;
		}
		else if(wParam == KEY_ENTER) //enter key
		{
			FireDrop_OnReset(hWnd, EC_COMMIT);
			return 1;
		}
		else if(wParam == KEY_ESC) //escape key
		{
			FireDrop_OnReset(hWnd, EC_ROLLBACK);
			return 1;
		}
		return 0;
	}

	return DefWindowProc(hWnd,message,wParam,lParam);		
}

void FireDrop_SendNotify(HWND hWnd, int code, void* data)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	NOTICE_EDITOR ne = { 0 };

	ne.widget = hWnd;
	ne.id = GetWindowLong(hWnd, GWL_ID);
	ne.code = code;
	ne.data = data;

	SendMessage(ptd->hOwner, FE_NOTICE, 0, (LPARAM)&ne);
}

void FireDrop_RelayoutKey(HWND hWnd)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	long cy, cx;
	RECT rt, rtDesk;

	cx = DEF_KEYCTRL_CX;
	cy = DEF_KEYCTRL_CY;

	GetWindowRect(GetDesktopWindow(), &rtDesk);

	if (cx > rtDesk.right - rtDesk.left)
		cx = rtDesk.right - rtDesk.left;
	if (cy > rtDesk.bottom - rtDesk.top)
		cy = rtDesk.bottom - rtDesk.top;

	GetWindowRect(ptd->hEdit, &rt);

	if (rt.top - cy < rtDesk.top)
	{
		cy = rt.top - rtDesk.top;
	}

	rt.bottom = rt.top;
	rt.top = rt.bottom - cy;

	if (rt.left + cx > rtDesk.right)
	{
		rt.right = rtDesk.right;
		rt.left = rt.right - cx;
	}
	else
	{
		rt.right = rt.left + cx;
	}

	SetWindowPos(ptd->hKey, HWND_TOP, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

void FireDrop_RelayoutList(HWND hWnd)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	link_t_ptr ptr,plk;
	int count;
	long w, h,cy,cx;
	RECT rt,rtDesk;
	xsize_t xs;
	xrect_t xr;
	WINDOWINFO wi = { 0 };

	wi.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(ptd->hList, &wi);

	cx = wi.cxWindowBorders * 2;
	cy = wi.cyWindowBorders * 2;

	ptr = string_view_fetch(ptd->hList);

	w = get_string_item_width(ptr);;
	h = 0;
	count = 0;

	plk = get_string_next_visible_item(ptr,LINK_FIRST);
	while (plk)
	{
		h += get_string_item_height(ptr);

		widget_text_size(ptd->hList, get_string_item_text_ptr(plk), -1, &xs);
		widget_size_to_tm(ptd->hList, &xs);

		if (w < xs.cx)
			w = xs.cx;

		count++;
		if (count > 7)
			break;

		plk = get_string_next_visible_item(ptr,plk);
	}

	xr.x = 0;
	xr.y = 0;
	xr.w = w;
	xr.h = h;
	widget_rect_to_pt(ptd->hList, &xr);

	cx += xr.w;
	cy += xr.h;

	GetWindowRect(GetDesktopWindow(), &rtDesk);

	GetWindowRect(ptd->hEdit, &rt);

	if (rt.bottom + cy > rtDesk.bottom)
	{
		cy = rtDesk.bottom - rt.bottom;
	}

	rt.top = rt.bottom;
	rt.bottom = rt.top + cy;

	if (rt.left + cx > rtDesk.right)
	{
		rt.right = rtDesk.right;
		rt.left = rt.right - cx;
	}
	else
	{
		rt.right = rt.left + cx;
	}

	SetWindowPos(ptd->hList, HWND_TOP, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

void FireDrop_OnReset(HWND hWnd, int code)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	link_t_ptr elk;

	elk = string_view_get_focus_item(ptd->hList);

	if (code == EC_COMMIT)
	{
		if (elk)
		{
			FireDrop_SendNotify(hWnd, EC_COMMIT, (void*)get_string_item_name_ptr(elk));
		}
		else
		{
			int len = GetWindowTextLength(ptd->hEdit);
			tchar_t *token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
			GetWindowText(ptd->hEdit, token, len + 1);

			FireDrop_SendNotify(hWnd, EC_COMMIT, (void*)token);

			xmem_free(token);
		}
	}
	else if (code == EC_ROLLBACK)
	{
		FireDrop_SendNotify(hWnd, EC_ROLLBACK, NULL);
	}
	else if (code == EC_CLEAN)
	{
		FireDrop_SendNotify(hWnd, EC_COMMIT, NULL);
	}
}

/*************************************************************************************************************************/
res_win_t firedrop_create(res_win_t widget, const xrect_t* pxr)
{
	RECT rt;
	HWND hEditor;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	MapWindowPoints(widget, HWND_DESKTOP, (LPPOINT)&rt, 2);

	hEditor = CreateWindow(FIREDROPCLASS, NULL, WS_POPUP | WS_BORDER | WS_VISIBLE, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, widget, NULL, NULL, NULL);
	return (res_win_t)hEditor;
}

void firedrop_set_options(res_win_t fw, const tchar_t* token, int len)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(fw);
	xrect_t xr;

	if (!ptd->data)
		ptd->data = create_string_doc();

	parse_string_doc_from_options(ptd->data, token, len);
	sort_string_doc(ptd->data, 0);

	widget_get_client_rect(fw, &xr);

	ptd->hList = string_view_create(NULL, WIDS_POPUP, &xr, fw);
	string_view_enable_fire(ptd->hList, 1);
	string_view_set_owner(ptd->hList, fw);

	string_view_attach(ptd->hList, ptd->data);
}

void firedrop_set_font(res_win_t fw, const xfont_t* pxf)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(fw);
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

void firedrop_set_text(res_win_t fw, const tchar_t* token)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(fw);

	SetWindowText(ptd->hEdit, token);
	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
}

int firedrop_get_text(res_win_t fw, tchar_t* buf, int max)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(fw);

	return GetWindowText(fw, buf, max);
}

void firedrop_set_owner(res_win_t fw, res_win_t owner)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(fw);

	ptd->hOwner = owner;
}

res_win_t firedrop_get_owner(res_win_t fw)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(fw);

	return ptd->hOwner;
}