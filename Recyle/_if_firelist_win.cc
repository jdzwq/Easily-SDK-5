/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firelist editor document

	@module	_if_firelist_win.c | firelist editor widnows implement file

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
#include "stringctrl.h"

typedef enum{
	_FIRELIST_HINT_NONE = 0,
	_FIRELIST_HINT_ITEM = 1
}_FIRELIST_HINT;

typedef struct tagFireListDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
	HWND hList;
	HWND hKey;
	LINKPTR data;
}FireListDelta;

#define GETFIRELISTDELTA(hWnd) 	((FireListDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIRELISTDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void	FireList_SendNotify(HWND hWnd, int code, void* data);
void	FireList_RelayoutList(HWND hWnd);
void	FireList_RelayoutKey(HWND hWnd);
void	FireList_OnNotice(HWND hWnd, NOTICE_STRING* pnt);
void	FireList_OnReset(HWND hWnd, int code);


ATOM RegisterFireListClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireListProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIRELISTCLASS;

	return RegisterClass(&wcex);
}


LRESULT CALLBACK ListEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK FireListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	RECT rt;
	HIMC hMic;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireListDelta*)xmem_alloc(sizeof(FireListDelta));
		memset((void*)ptd,0,sizeof(FireListDelta));
		SETFIRELISTDELTA(hWnd,ptd);

		GetClientRect(hWnd,&rt);
		ptd->hEdit = CreateWindow(_T("EDIT"),NULL,WS_CHILD | WS_VISIBLE | WS_DISABLED,rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,hWnd,NULL,NULL,NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)ListEditProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit,TRUE);

		hMic = ImmGetContext(ptd->hEdit);
		ImmSetOpenStatus(hMic, FALSE);
		ImmReleaseContext(ptd->hEdit, hMic);

		if (XDC_TOUCHED)
		{
			ptd->hKey = CreateWindow(KEYCTRLCLASS, NULL, WS_POPUP | WS_BORDER, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
			FireList_RelayoutKey(hWnd);
		}

		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIRELISTDELTA(hWnd,0);

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
			string_view_find(ptd->hList, LINK_FIRST, token);

			xsfree(token);
		}else if(HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit && IsWindow(ptd->hList))
		{
			PostMessage(ptd->hEdit, WM_KEYDOWN, KEY_ENTER, 0);
		}
		break;
	case FE_RESET:
		FireList_OnReset(hWnd, wParam);
		return 0;
	case WM_MOUSEWHEEL:
		if(ptd->hList)
		{
			PostMessage(ptd->hList,WM_MOUSEWHEEL,wParam,lParam);
			return 1;
		}
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
			FireList_OnReset(hWnd, EC_COMMIT);
			return 1;
		}
		else if(wParam == KEY_ESC) //escape key
		{
			FireList_OnReset(hWnd, EC_ROLLBACK);
			return 1;
		}
		else if(wParam == KEY_SPACE) //blank key
		{
			FireList_OnReset(hWnd, EC_CLEAN);
			return 1;
		}
		return 0;
	}

	return DefWindowProc(hWnd,message,wParam,lParam);		
}

void FireList_SendNotify(HWND hWnd, int code, void* data)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	NOTICE_EDITOR ne = { 0 };

	ne.widget = hWnd;
	ne.id = GetWindowLong(hWnd, GWL_ID);
	ne.code = code;
	ne.data = data;

	SendMessage(ptd->hOwner, FE_NOTICE, 0, (LPARAM)&ne);
}

void FireList_RelayoutKey(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
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

void FireList_RelayoutList(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	link_t_ptr ptr, plk;
	int count;
	long w, h, cy, cx;
	RECT rt, rtDesk;
	xrect_t xr;
	xsize_t xs;

	WINDOWINFO wi = { 0 };

	wi.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(ptd->hList, &wi);

	cx = wi.cxWindowBorders * 2;
	cy = wi.cyWindowBorders * 2;

	ptr = string_view_fetch(ptd->hList);

	w = get_string_item_width(ptr);
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

void FireList_OnReset(HWND hWnd, int code)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	link_t_ptr elk;

	elk = string_view_get_focus_item(ptd->hList);

	if (code == EC_COMMIT)
	{
		if (elk)
			FireList_SendNotify(hWnd, EC_COMMIT, (void*)get_string_item_name_ptr(elk));
		else
			FireList_SendNotify(hWnd, EC_COMMIT, NULL);
	}
	else if (code == EC_ROLLBACK)
	{
		FireList_SendNotify(hWnd, EC_ROLLBACK, NULL);
	}
	else if (code == EC_CLEAN)
	{
		FireList_SendNotify(hWnd, EC_COMMIT, NULL);
	}
}

/*********************************************************************************************************/
res_win_t firelist_create(res_win_t widget, const xrect_t* pxr)
{
	RECT rt;
	HWND hEditor;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	MapWindowPoints(widget, HWND_DESKTOP, (LPPOINT)&rt, 2);

	hEditor = CreateWindow(FIRELISTCLASS, NULL, WS_POPUP | WS_BORDER | WS_VISIBLE, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, widget, NULL, NULL, NULL);
	return (res_win_t)hEditor;
}

void firelist_set_font(res_win_t fw, const xfont_t* pxf)
{
	FireListDelta* ptd = GETFIRELISTDELTA(fw);
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

void firelist_set_options(res_win_t fw, const tchar_t* token, int len)
{
	FireListDelta* ptd = GETFIRELISTDELTA(fw);
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

	FireList_RelayoutList(fw);
}

void firelist_set_text(res_win_t fw, const tchar_t* token)
{
	FireListDelta* ptd = GETFIRELISTDELTA(fw);

	SetWindowText(ptd->hEdit, token);
	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
}

int firelist_get_text(res_win_t fw, tchar_t* buf, int max)
{
	FireListDelta* ptd = GETFIRELISTDELTA(fw);

	return GetWindowText(fw, buf, max);
}

void firelist_set_owner(res_win_t fw, res_win_t owner)
{
	FireListDelta* ptd = GETFIRELISTDELTA(fw);

	ptd->hOwner = owner;
}

res_win_t firelist_get_owner(res_win_t fw)
{
	FireListDelta* ptd = GETFIRELISTDELTA(fw);

	return ptd->hOwner;
}