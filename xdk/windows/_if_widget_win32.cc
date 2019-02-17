/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget document

	@module	_if_widget_win.c | widget widnows implement file

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
#include "xdkutil.h"

#ifdef XDK_SUPPORT_WIDGET

#pragma comment(lib,"comctl32.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "Msimg32.lib")

#define WIDGET_TITLE_SPAN		(float)10	//mm
#define WIDGET_MENU_SPAN		(float)7.5	//mm
#define WIDGET_SCROLL_SPAN		(float)5	//mm
#define WIDGET_ICON_SPAN		(float)3	//mm
#define WIDGET_FRAME_EDGE		(float)1.5	//mm
#define WIDGET_CHILD_EDGE		(float)0.5	//mm

#define GETXDKDISPATCH(hWnd)		(if_event_t*)GetProp(hWnd, XDKDISPATCH)
#define SETXDKDISPATCH(hWnd, ev)	SetProp(hWnd, XDKDISPATCH, (HANDLE)ev)

#define GETXDKSUBPROC(hWnd)			(if_subproc_t*)GetProp(hWnd, XDKSUBPROC)
#define SETXDKSUBPROC(hWnd, lp)		SetProp(hWnd, XDKSUBPROC, (HANDLE)lp)

#define GETXDKCOREDELTA(hWnd)		(var_long)GetProp(hWnd, XDKCOREDELTA)
#define SETXDKCOREDELTA(hWnd, lp)	SetProp(hWnd, XDKCOREDELTA, (HANDLE)lp)

#define GETXDKUSERDELTA(hWnd)		(var_long)GetProp(hWnd, XDKUSERDELTA)
#define SETXDKUSERDELTA(hWnd, lp)	SetProp(hWnd, XDKUSERDELTA, (HANDLE)lp)

#define GETXDKSTYLE(hWnd)			(DWORD)GetProp(hWnd, XDKSTYLE)
#define SETXDKSTYLE(hWnd, dw)		SetProp(hWnd, XDKSTYLE, (HANDLE)dw)

#define GETXDKACCEL(hWnd)			(res_acl_t)GetProp(hWnd, XDKACCEL)
#define SETXDKACCEL(hWnd, acl)		SetProp(hWnd, XDKACCEL, (HANDLE)acl)

#define GETXDKOWNER(hWnd)			(res_win_t)GetProp(hWnd, XDKOWNER)
#define SETXDKOWNER(hWnd, win)		SetProp(hWnd, XDKOWNER, (HANDLE)win)

#define GETXDKUSER(hWnd)			(DWORD)GetProp(hWnd, XDKUSER)
#define SETXDKUSER(hWnd, dw)		SetProp(hWnd, XDKUSER, (HANDLE)dw)

#define GETXDKRESULT(hWnd)			(DWORD)GetProp(hWnd, XDKRESULT)
#define SETXDKRESULT(hWnd, dw)		SetProp(hWnd, XDKRESULT, (HANDLE)dw)

LRESULT CALLBACK XdcWidgetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static void _ClientRectToWindow(HWND hWnd, RECT* prt)
{
	RECT rtWnd, rtCli;

	GetClientRect(hWnd, &rtCli);

	ClientToScreen(hWnd, (LPPOINT)&rtCli);

	GetWindowRect(hWnd, &rtWnd);

	prt->left += (rtCli.left - rtWnd.left);
	prt->top += (rtCli.top - rtWnd.top);
	prt->right += (rtCli.left - rtWnd.left);
	prt->bottom += (rtCli.top - rtWnd.top);
}

static void _ClientPointToWindow(HWND hWnd, POINT* ppt)
{
	RECT rtWnd, rtCli;

	GetClientRect(hWnd, &rtCli);

	ClientToScreen(hWnd, (LPPOINT)&rtCli);

	GetWindowRect(hWnd, &rtWnd);

	ppt->x += (rtCli.left - rtWnd.left);
	ppt->y += (rtCli.top - rtWnd.top);
}

static void _ScreenRectToWindow(HWND hWnd, RECT* prt)
{
	RECT rtWnd;

	GetWindowRect(hWnd, &rtWnd);

	prt->left -=rtWnd.left;
	prt->right -= rtWnd.left;
	prt->top -= rtWnd.top;
	prt->bottom -= rtWnd.top;
}

static void _ScreenPointToWindow(HWND hWnd, POINT* ppt)
{
	RECT rtWnd;

	GetWindowRect(hWnd, &rtWnd);

	ppt->x -= rtWnd.left;
	ppt->y -= rtWnd.top;
}

static void _CenterRect(RECT* prt, long cx, long cy)
{
	if (prt->right - prt->left > cx)
	{
		prt->left += cx / 2;
		prt->right -= cx / 2;
	}

	if (prt->bottom - prt->top > cy)
	{
		prt->top += cy / 2;
		prt->bottom -= cy / 2;
	}
}

static DWORD _WindowStyle(u32_t wstyle)
{
	DWORD dw = 0;

	if (wstyle & WD_STYLE_OWNERNC)
	{
		if (wstyle & WD_STYLE_CHILD)
			dw |= (WS_CHILD | WS_CLIPSIBLINGS);
		else
			dw |= (WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		if (wstyle & WD_STYLE_TITLE)
			dw |= WS_CAPTION;
	}
	else
	{
		if (wstyle & WD_STYLE_CHILD)
			dw |= WS_CHILD;
		else
			dw |= WS_POPUP;

		if (wstyle & WD_STYLE_BORDER)
			dw |= WS_BORDER;

		if (wstyle & WD_STYLE_HSCROLL)
			dw |= WS_HSCROLL;

		if (wstyle & WD_STYLE_VSCROLL)
			dw |= WS_VSCROLL;

		if (wstyle & WD_STYLE_TITLE)
		{
			if (wstyle & WD_STYLE_SIZEBOX)
			{
#ifdef WINCE
				dw |= (WS_POPUP | WS_CAPTION);
#else
				dw = WS_OVERLAPPEDWINDOW;
#endif
			}
			else
			{
#ifdef WINCE
				dw |= (WS_POPUP | WS_CAPTION);
#else
				dw |= (WS_POPUPWINDOW | WS_CAPTION);
#endif
			}
		}
	}

	return dw;
}

/*******************************************************************************************/

ATOM RegisterXdcWidgetClass(HINSTANCE hInstance)
{
	WNDCLASS wcex = { 0 };

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)XdcWidgetProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = XDKWIDGET;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK XdcWidgetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPCREATESTRUCT lpcs;
	if_event_t* pev;
	DWORD ds;

	switch (message)
	{
#ifndef WINCE
	case WM_NCPAINT:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_paint)
		{
			HDC hDC;
			if (wParam == 1)
				hDC = GetWindowDC(hWnd);
			else
				hDC = GetDCEx(hWnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);

			SetBkMode(hDC, TRANSPARENT);

			(*pev->pf_on_nc_paint)(hWnd, hDC);

			ReleaseDC(hWnd, hDC);

			return 0;
		}
		break;
	case WM_NCCALCSIZE:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_calcsize)
		{
			xrect_t xrCli = { 0 };

			if ((BOOL)wParam)
			{
				LPNCCALCSIZE_PARAMS lpcp = (LPNCCALCSIZE_PARAMS)lParam;

				xrCli.x = lpcp->rgrc[0].left;
				xrCli.y = lpcp->rgrc[0].top;
				xrCli.w = lpcp->rgrc[0].right - lpcp->rgrc[0].left;
				xrCli.h = lpcp->rgrc[0].bottom - lpcp->rgrc[0].top;

				(*pev->pf_on_nc_calcsize)(hWnd, &xrCli);

				lpcp->rgrc[2].left = lpcp->rgrc[1].left;
				lpcp->rgrc[2].top = lpcp->rgrc[1].top;
				lpcp->rgrc[2].right = lpcp->rgrc[1].right;
				lpcp->rgrc[2].bottom = lpcp->rgrc[1].bottom;

				lpcp->rgrc[1].left = lpcp->rgrc[0].left;
				lpcp->rgrc[1].top = lpcp->rgrc[0].top;
				lpcp->rgrc[1].right = lpcp->rgrc[0].right;
				lpcp->rgrc[1].bottom = lpcp->rgrc[0].bottom;

				lpcp->rgrc[0].left = xrCli.x;
				lpcp->rgrc[0].top = xrCli.y;
				lpcp->rgrc[0].right = xrCli.x + xrCli.w;
				lpcp->rgrc[0].bottom = xrCli.y + xrCli.h;
			}
			else
			{
				LPRECT lprt = (LPRECT)lParam;

				xrCli.x = lprt->left;
				xrCli.y = lprt->top;
				xrCli.w = lprt->right - lprt->left;
				xrCli.h = lprt->bottom - lprt->top;

				(*pev->pf_on_nc_calcsize)(hWnd, &xrCli);

				lprt->left = xrCli.x;
				lprt->top = xrCli.y;
				lprt->right = xrCli.x + xrCli.w;
				lprt->bottom = xrCli.y + xrCli.h;
			}

			return 0;
		}
		break;
	case WM_NCHITTEST:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_hittest)
		{
			xpoint_t xp = { 0 };
			xp.x = GET_X_LPARAM(lParam);
			xp.y = GET_Y_LPARAM(lParam);

			int hit = (*pev->pf_on_nc_hittest)(hWnd, &xp);

			return (hit == HINT_MENUBAR || hit == HINT_ICON || hit == HINT_MINIMIZE || hit == HINT_MAXIMIZE || hit == HINT_RESTORE || hit == HINT_CLOSE || hit == HINT_HSCROLL || hit == HINT_VSCROLL || hit == HINT_PAGEUP || hit == HINT_PAGEDOWN) ? HINT_BORDER : hit;
		}
		break;
	case WM_NCLBUTTONUP:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_hittest)
		{
			xpoint_t xp = { 0 };
			xp.x = GET_X_LPARAM(lParam);
			xp.y = GET_Y_LPARAM(lParam);
			
			int hit = (*pev->pf_on_nc_hittest)(hWnd, &xp);

			RECT rt;
			GetWindowRect(hWnd, &rt);

			switch (hit)
			{
			case HINT_MINIMIZE:
				PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
				break;
			case HINT_MAXIMIZE:
				PostMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				break;
			case HINT_RESTORE:
				PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				break;
			case HINT_CLOSE:
				PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				break;
			case HINT_ICON:
				PostMessage(hWnd, WM_SYSCOMMAND, SC_CONTEXTHELP, MAKELPARAM(xp.x, xp.y));
				break;
			case HINT_HSCROLL:
				if (pev->pf_on_nc_calcscroll)
				{
					xp.x -= rt.left;
					xp.y -= rt.top;

					long pos = (*pev->pf_on_nc_calcscroll)(hWnd, 1, &xp);

					scroll_t scr = { 0 };
					_widget_get_scroll_info(hWnd, 1, &scr);

					if (pos != scr.pos)
					{
						PostMessage(hWnd, WM_SCROLL, (WPARAM)1, (LPARAM)(pos - scr.pos));
					}
				}
				break;
			case HINT_VSCROLL:
				if (pev->pf_on_nc_calcscroll)
				{
					xp.x -= rt.left;
					xp.y -= rt.top;

					long pos = (*pev->pf_on_nc_calcscroll)(hWnd, 0, &xp);

					scroll_t scr = { 0 };
					_widget_get_scroll_info(hWnd, 0, &scr);

					if (pos != scr.pos)
					{
						PostMessage(hWnd, WM_SCROLL, (WPARAM)0, (LPARAM)(pos - scr.pos));
					}
				}
				break;
			case HINT_PAGEUP:
				PostMessage(hWnd, WM_KEYDOWN, (WPARAM)KEY_PAGEUP, (LPARAM)1);
				SendMessage(hWnd, WM_KEYUP, (WPARAM)KEY_PAGEUP, (LPARAM)1);
				break;
			case HINT_PAGEDOWN:
				PostMessage(hWnd, WM_KEYDOWN, (WPARAM)KEY_PAGEDOWN, (LPARAM)1);
				SendMessage(hWnd, WM_KEYUP, (WPARAM)KEY_PAGEDOWN, (LPARAM)1);
				break;
			}
		}
		break;
	case WM_NCRBUTTONUP:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		break;
	case WM_NCACTIVATE:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		if (wParam)
		{
			SendMessage(hWnd, WM_NCPAINT, 1, 0);
			return 0;
		}
		else
		{
			return 1;
		}
		break;
#endif

	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		if (lpcs->lpCreateParams)
		{
			if_event_t* pv = (if_event_t*)lpcs->lpCreateParams;
			if (pv)
			{
				pev = (if_event_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(if_event_t));
				CopyMemory((void*)pev, (void*)pv, sizeof(if_event_t));
				SETXDKDISPATCH(hWnd, pev);
			}
		}

		pev = GETXDKDISPATCH(hWnd);

		if (pev && pev->pf_on_create)
		{
			if ((*pev->pf_on_create)(hWnd, pev->param))
				return -1;
		}
		break;
	case WM_DESTROY:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_destroy)
		{
			(*pev->pf_on_destroy)(hWnd);

			RemoveProp(hWnd, XDKDISPATCH);
			HeapFree(GetProcessHeap(), 0, pev);
		}
		break;
	case WM_CLOSE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_close)
		{
			if ((*pev->pf_on_close)(hWnd))
				return 0;
		}
		break;
	case WM_LBUTTONDBLCLK:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_lbutton_dbclick)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_lbutton_dbclick)(hWnd, &xp);
		}
		break;
	case WM_LBUTTONDOWN:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_lbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_lbutton_down)(hWnd, &xp);
		}
		break;
	case WM_LBUTTONUP:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_lbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_lbutton_up)(hWnd, &xp);
		}
		break;
	case WM_RBUTTONDOWN:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_rbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_rbutton_down)(hWnd, &xp);
		}
		break;
	case WM_RBUTTONUP:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_rbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_rbutton_up)(hWnd, &xp);
		}
		break;
	case WM_MOUSEMOVE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_mouse_move)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_mouse_move)(hWnd, (u32_t)wParam, &xp);
		}
		break;
	case WM_MOUSEHOVER:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_mouse_hover)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_mouse_hover)(hWnd, (u32_t)wParam, &xp);
		}
		break;
	case WM_MOUSELEAVE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_mouse_leave)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_mouse_leave)(hWnd, (u32_t)wParam, &xp);
		}
		break;
	case WM_MOVE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_move)
		{
			xpoint_t xp;

			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_move)(hWnd, &xp);
			return 0;
		}
		break;
	case WM_SIZE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_size)
		{
			xsize_t xs;
			
			xs.cx = (long)(short)LOWORD(lParam);
			xs.cy = (long)(short)HIWORD(lParam);

			(*pev->pf_on_size)(hWnd, (int)wParam, &xs);

			return 0;
		}
		break;
	case WM_SHOWWINDOW:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_show && !lParam)
		{
			(*pev->pf_on_show)(hWnd, (bool_t)wParam);
			return 0;
		}
		break;
	case WM_HSCROLL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_scroll)
		{
			scroll_t scr = { 0 };
			_widget_get_scroll_info(hWnd, 1, &scr);
			
			switch (LOWORD(wParam))
			{
			case SB_LEFT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, -scr.pos);
				break;
			case SB_RIGHT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.max - scr.pos);
				break;
			case SB_LINELEFT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, -scr.min);
				break;
			case SB_LINERIGHT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.min);
				break;
			case SB_PAGELEFT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, -scr.page);
				break;
			case SB_PAGERIGHT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.page);
				break;
			case SB_THUMBPOSITION:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.track - scr.pos);
				break;
			case SB_ENDSCROLL:
				/*if (pev->pf_on_scrollinfo)
				{
					scroll_t sc = { 0 };
					(*pev->pf_on_scrollinfo)(hWnd, (bool_t)1, &sc);

					scr.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
					scr.nPos = sc.pos;
					scr.nMin = sc.min;
					scr.nMax = sc.max;
					scr.nPage = sc.page;
					SetScrollInfo(hWnd, SB_HORZ, &scr, TRUE);
				}*/
				break;
			}
			return 0;
		}
		break;
	case WM_VSCROLL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_scroll)
		{
			scroll_t scr = { 0 };
			_widget_get_scroll_info(hWnd, 0, &scr);

			switch (LOWORD(wParam))
			{
			case SB_TOP:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, -scr.pos);
				break;
			case SB_BOTTOM:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.max - scr.pos);
				break;
			case SB_LINEUP:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, -scr.min);
				break;
			case SB_LINEDOWN:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.min);
				break;
			case SB_PAGEUP:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, -scr.page);
				break;
			case SB_PAGEDOWN:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.page);
				break;
			case SB_THUMBPOSITION:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.track - scr.pos);
				break;
			case SB_ENDSCROLL:
				/*if (pev->pf_on_scrollinfo)
				{
					scroll_t sc = { 0 };
					(*pev->pf_on_scrollinfo)(hWnd, (bool_t)0, &sc);

					scr.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
					scr.nPos = sc.pos;
					scr.nMin = sc.min;
					scr.nMax = sc.max;
					scr.nPage = sc.page;
					SetScrollInfo(hWnd, SB_VERT, &scr, TRUE);
				}*/
				break;
			}
			return 0;
		}
		break;
	case WM_SCROLL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_scroll)
		{
			(*pev->pf_on_scroll)(hWnd, (bool_t)wParam, (long)lParam);
		}
		return 0;
#ifndef WINCE
	case WM_MOUSEWHEEL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_wheel)
		{
			(*pev->pf_on_wheel)(hWnd, (bool_t)0, (long)GET_WHEEL_DELTA_WPARAM(wParam));
			return 0;
		}
		else if (pev && pev->pf_on_scroll)
		{
			scroll_t scr = { 0 };

			_widget_get_scroll_info(hWnd, 0, &scr);

			if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.min);
			else
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, -scr.min);

			return 0;
		}
		break;
	case WM_MOUSEHWHEEL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_wheel)
		{
			(*pev->pf_on_wheel)(hWnd, (bool_t)1, (long)GET_WHEEL_DELTA_WPARAM(wParam));
			return 0;
		}
		else if (pev && pev->pf_on_scroll)
		{
			scroll_t scr = { 0 };
			_widget_get_scroll_info(hWnd, 1, &scr);

			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.min);
			else
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, -scr.min);

			return 0;
		}
		break;
#endif
	case WM_ACTIVATE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_activate)
		{
			(*pev->pf_on_activate)(hWnd, (int)LOWORD(wParam));
		}
		break;
	case WM_MOUSEACTIVATE:
		ds = GETXDKSTYLE(hWnd);
		if (ds & WD_STYLE_NOACTIVE)
			return MA_NOACTIVATE;
		break;
	case WM_ERASEBKGND:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_erase)
		{
			(*pev->pf_on_erase)(hWnd, (res_ctx_t)wParam);

			return 0;
		}
		break;
	case WM_PAINT:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_paint)
		{
			PAINTSTRUCT ps;
			RECT rtFront;

			if (GetUpdateRect(hWnd, &rtFront, FALSE))
			{
				xrect_t xrFront;
				xrFront.x = rtFront.left;
				xrFront.y = rtFront.top;
				xrFront.w = rtFront.right - rtFront.left;
				xrFront.h = rtFront.bottom - rtFront.top;

				BeginPaint(hWnd, &ps);

				(*pev->pf_on_paint)(hWnd, ps.hdc, &xrFront);

				EndPaint(hWnd, &ps);
			}
		}
		break;
	case WM_KEYDOWN:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_keydown)
		{
			(*pev->pf_on_keydown)(hWnd, (int)wParam);
			return 0;
		}
		break;
	case WM_CHAR:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_char)
		{
			(*pev->pf_on_char)(hWnd, (tchar_t)wParam);
			return 0;
		}
		break;
	case WM_ENABLE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_enable)
		{
			(*pev->pf_on_enable)(hWnd, (bool_t)wParam);
			return 0;
		}
		break;
	case WM_SETFOCUS:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_set_focus)
		{
			(*pev->pf_on_set_focus)(hWnd, (res_win_t)wParam);
			return 0;
		}
		break;
	case WM_KILLFOCUS:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_kill_focus)
		{
			(*pev->pf_on_kill_focus)(hWnd, (res_win_t)wParam);
			return 0;
		}
		break;
	case WM_COMMAND:
		pev = GETXDKDISPATCH(hWnd);
		if (pev)
		{
			if (LOWORD(wParam) == IDC_PARENT)
			{
				if (HIWORD(wParam) == COMMAND_FIND)
				{
					if (pev->pf_on_command_find)
					{
						(*pev->pf_on_command_find)(hWnd, (str_find_t*)lParam);
						return 0;
					}
				}
				else if (HIWORD(wParam) == COMMAND_REPLACE)
				{
					if (pev->pf_on_command_replace)
					{
						(*pev->pf_on_command_replace)(hWnd, (str_replace_t*)lParam);
						return 0;
					}
				}
				else if (pev->pf_on_parent_command)
				{
					(*pev->pf_on_parent_command)(hWnd, (int)(short)HIWORD(wParam), (var_long)lParam);
					return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_CHILD)
			{
				if (pev->pf_on_child_command)
				{
					(*pev->pf_on_child_command)(hWnd, (int)(short)HIWORD(wParam), (var_long)lParam);
					return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_SELF)
			{
				if (pev->pf_on_self_command)
				{
					(*pev->pf_on_self_command)(hWnd, (int)(short)HIWORD(wParam), (var_long)lParam);
					return 0;
				}
			}
			else
			{
				if (pev->pf_on_menu_command)
				{
					(*pev->pf_on_menu_command)(hWnd, (int)(short)HIWORD(wParam), (uid_t)(short)LOWORD(wParam), (var_long)lParam);
					return 0;
				}
			}
		}
		break;
	case WM_SYSCOMMAND:
		pev = GETXDKDISPATCH(hWnd);
#ifndef WINCE
		if ((wParam & 0xFFF0) == SC_CONTEXTHELP && pev && pev->pf_on_syscmd_click)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);
			(*pev->pf_on_syscmd_click)(hWnd, &xp);
			return 0;
		}
#endif
		break;
	case WM_TIMER:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_timer)
		{
			(*pev->pf_on_timer)(hWnd, (var_long)(wParam));
			return 0;
		}
		break;
	case WM_COPY:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_copy)
		{
			(*pev->pf_on_copy)(hWnd);
			return 0;
		}
		break;
	case WM_CUT:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_cut)
		{
			(*pev->pf_on_cut)(hWnd);
			return 0;
		}
		break;
	case WM_PASTE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_paste)
		{
			(*pev->pf_on_paste)(hWnd);
			return 0;
		}
		break;
	case WM_UNDO:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_undo)
		{
			(*pev->pf_on_undo)(hWnd);
			return 0;
		}
		break;
	case WM_NOTICE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_notice)
		{
			(*pev->pf_on_notice)(hWnd, (NOTICE*)lParam);
		}
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

#ifdef WINCE
LRESULT CALLBACK XdcSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#else
LRESULT CALLBACK XdcSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
#endif
{
	if_subproc_t* pev;

	pev = GETXDKSUBPROC(hWnd);

#ifdef WINCE
	DWORD uIdSubclass;
	uIdSubclass = (pev)? pev->sid : 0;
#endif

	switch (message)
	{
	case WM_LBUTTONDBLCLK:
		if (pev && pev->sub_on_lbutton_dbclick)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_lbutton_dbclick)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_LBUTTONDOWN:
		if (pev && pev->sub_on_lbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_lbutton_down)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_LBUTTONUP:
		if (pev && pev->sub_on_lbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_lbutton_up)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_RBUTTONDOWN:
		if (pev && pev->sub_on_rbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_rbutton_down)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_RBUTTONUP:
		if (pev && pev->sub_on_rbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_rbutton_up)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOUSEMOVE:
		if (pev && pev->sub_on_mouse_move)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_mouse_move)(hWnd, (u32_t)wParam, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOUSEHOVER:
		if (pev && pev->sub_on_mouse_hover)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_mouse_hover)(hWnd, (u32_t)wParam, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOUSELEAVE:
		if (pev && pev->sub_on_mouse_leave)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_mouse_leave)(hWnd, (u32_t)wParam, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SIZE:
		if (pev && pev->sub_on_size)
		{
			xsize_t xs = { 0 };
			xs.cx = (long)(short)LOWORD(lParam);
			xs.cy = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_size)(hWnd, (int)wParam, &xs, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOVE:
		if (pev && pev->sub_on_move)
		{
			xpoint_t xp = { 0 };

			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_move)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SHOWWINDOW:
		if (pev && pev->sub_on_show)
		{
			if ((*pev->sub_on_show)(hWnd, (bool_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SCROLL:
		if (pev && pev->sub_on_scroll)
		{
			if ((*pev->sub_on_scroll)(hWnd, (bool_t)wParam, (long)lParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
#ifndef WINCE
	case WM_MOUSEWHEEL:
		if (pev && pev->sub_on_wheel)
		{
			if ((*pev->sub_on_wheel)(hWnd, (bool_t)0, (long)GET_WHEEL_DELTA_WPARAM(wParam), (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		else if (pev && pev->sub_on_scroll)
		{
			SCROLLINFO scr = { 0 };
			scr.cbSize = sizeof(scr);
			scr.fMask = SIF_POS;
			GetScrollInfo(hWnd, SB_VERT, &scr);

			if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				if ((*pev->sub_on_scroll)(hWnd, (bool_t)0, scr.nMin, (uid_t)uIdSubclass, pev->delta))
					return 0;
			}
			else
			{
				if ((*pev->sub_on_scroll)(hWnd, (bool_t)0, -scr.nMin, (uid_t)uIdSubclass, pev->delta))
					return 0;
			}
		}
		break;
	case WM_MOUSEHWHEEL:
		if (pev && pev->sub_on_wheel)
		{
			if ((*pev->sub_on_wheel)(hWnd, (bool_t)1, (long)GET_WHEEL_DELTA_WPARAM(wParam), (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		else if (pev && pev->sub_on_scroll)
		{
			SCROLLINFO scr = { 0 };
			scr.cbSize = sizeof(scr);
			scr.fMask = SIF_POS;
			GetScrollInfo(hWnd, SB_VERT, &scr);

			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				if ((*pev->sub_on_scroll)(hWnd, (bool_t)1, scr.nMin, (uid_t)uIdSubclass, pev->delta))
					return 0;
			}
			else
			{
				if ((*pev->sub_on_scroll)(hWnd, (bool_t)1, -scr.nMin, (uid_t)uIdSubclass, pev->delta))
					return 0;
			}
		}
		break;
#endif 
	case WM_ERASEBKGND:
		if (pev && pev->sub_on_erase)
		{
			if ((*pev->sub_on_erase)(hWnd, (res_ctx_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_PAINT:
		if (pev && pev->sub_on_paint)
		{
			PAINTSTRUCT ps;
			RECT rtFront;
			bool_t rt;

			if (GetUpdateRect(hWnd, &rtFront, FALSE))
			{
				xrect_t xrFront;
				xrFront.x = rtFront.left;
				xrFront.y = rtFront.top;
				xrFront.w = rtFront.right - rtFront.left;
				xrFront.h = rtFront.bottom - rtFront.top;

				BeginPaint(hWnd, &ps);

				rt = (*pev->sub_on_paint)(hWnd, ps.hdc, &xrFront, (uid_t)uIdSubclass, pev->delta);

				EndPaint(hWnd, &ps);

				if (rt)
					return 0;
			}
		}
		break;
	case WM_KEYDOWN:
		if (pev && pev->sub_on_keydown)
		{
			if ((*pev->sub_on_keydown)(hWnd, (int)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_CHAR:
		if (pev && pev->sub_on_char)
		{
			if ((*pev->sub_on_char)(hWnd, (tchar_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_ENABLE:
		if (pev && pev->sub_on_enable)
		{
			if ((*pev->sub_on_enable)(hWnd, (bool_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SETFOCUS:
		if (pev && pev->sub_on_set_focus)
		{
			if ((*pev->sub_on_set_focus)(hWnd, (res_win_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_KILLFOCUS:
		if (pev && pev->sub_on_kill_focus)
		{
			if ((*pev->sub_on_kill_focus)(hWnd, (res_win_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_NOTICE:
		if (pev && pev->sub_on_notice)
		{
			if ((*pev->sub_on_notice)(hWnd, (NOTICE*)lParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_COMMAND:
		if (pev)
		{
			if (LOWORD(wParam) == IDC_PARENT)
			{
				if (HIWORD(wParam) == COMMAND_FIND)
				{
					if (pev->sub_on_command_find)
					{
						if ((*pev->sub_on_command_find)(hWnd, (str_find_t*)lParam, (uid_t)uIdSubclass, pev->delta))
							return 0;
					}
				}
				else if (HIWORD(wParam) == COMMAND_REPLACE)
				{
					if (pev->sub_on_command_replace)
					{
						if ((*pev->sub_on_command_replace)(hWnd, (str_replace_t*)lParam, (uid_t)uIdSubclass, pev->delta))
							return 0;
					}
				}
				else if (pev->sub_on_parent_command)
				{
					if ((*pev->sub_on_parent_command)(hWnd, (int)HIWORD(wParam), (var_long)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_CHILD)
			{
				if (pev->sub_on_child_command)
				{
					if ((*pev->sub_on_child_command)(hWnd, (int)HIWORD(wParam), (var_long)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_SELF)
			{
				if (pev->sub_on_self_command)
				{
					if ((*pev->sub_on_self_command)(hWnd, (int)HIWORD(wParam), (var_long)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
			else 
			{
				if (pev->sub_on_menu_command)
				{
					if ((*pev->sub_on_menu_command)(hWnd, (int)HIWORD(wParam), (int)LOWORD(wParam), (var_long)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
		}
		break;
#ifndef WINCE
	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_MOUSEMENU && pev && pev->sub_on_sysclr_click)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);
			if ((*pev->sub_on_sysclr_click)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		else if ((wParam & 0xFFF0) == SC_CONTEXTHELP && pev && pev->sub_on_syslog_click)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);
			if ((*pev->sub_on_syslog_click)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
#endif 
	case WM_TIMER:
		if (pev && pev->sub_on_timer)
		{
			if ((*pev->sub_on_timer)(hWnd, (var_long)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_COPY:
		if (pev && pev->sub_on_copy)
		{
			if ((*pev->sub_on_copy)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_CUT:
		if (pev && pev->sub_on_cut)
		{
			if ((*pev->sub_on_cut)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_PASTE:
		if (pev && pev->sub_on_paste)
		{
			if ((*pev->sub_on_paste)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_UNDO:
		if (pev && pev->sub_on_undo)
		{
			if ((*pev->sub_on_undo)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_CLOSE:
		if (pev && pev->sub_on_close)
		{
			if ((*pev->sub_on_close)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 1;
		}
		break;
	case WM_DESTROY:
		if (pev && pev->sub_on_destroy)
		{
			(*pev->sub_on_destroy)(hWnd, (uid_t)uIdSubclass, pev->delta);
		}
		break;
	}

#ifdef WINCE
	if(pev && pev->proc)
		return CallWindowProc((WNDPROC)pev->proc,hWnd,message,wParam,lParam);
	else
		return DefWindowProc(hWnd,message,wParam,lParam);		
#else
	return DefSubclassProc(hWnd, message, wParam, lParam);
#endif
}

void _widget_startup(int ver)
{
	HINSTANCE hInst = GetModuleHandle(NULL);

	RegisterXdcWidgetClass(hInst);
}

void _widget_cleanup()
{
	HINSTANCE hInst = GetModuleHandle(NULL);

	UnregisterClass(XDKWIDGET, hInst);
}


res_win_t _widget_create(const tchar_t* wname, u32_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev)
{
	HWND hWnd = CreateWindow(XDKWIDGET, wname, _WindowStyle(wstyle), pxr->x, pxr->y, pxr->w, pxr->h, wparent, NULL, NULL, (void*)pev);

	if (!IsWindow(hWnd))
		return NULL;

	SETXDKSTYLE(hWnd, wstyle);

	if ((wstyle & WD_STYLE_VSCROLL) || (wstyle & WD_STYLE_VSCROLL) || (wstyle & WD_STYLE_HSCROLL) || (wstyle & WD_STYLE_MENUBAR))
	{
		_widget_update_window(hWnd);
	}

	return (res_win_t)hWnd;
}

void _widget_destroy(res_win_t wt)
{
	if (!IsWindow(wt))
		return;

	DestroyWindow((HWND)wt);
}

void _widget_close(res_win_t wt, int ret)
{
	SETXDKRESULT(wt, ret);

#ifdef WINCE
	if (GetWindowLong(wt, GWL_STYLE) & WS_CHILD)
#else
	if (GetWindowLongPtr(wt, GWL_STYLE) & WS_CHILD)
#endif
	{
		if (ret)
			PostMessage(wt, WM_CLOSE, 0, 0);
		else
			SendMessage(wt, WM_CLOSE, 0, 0);
	}
	else
	{
		if (ret)
			PostMessage(wt, WM_SYSCOMMAND, SC_CLOSE, 0);
		else
			SendMessage(wt, WM_SYSCOMMAND, SC_CLOSE, 0);
	}
}

if_subproc_t* _widget_get_subproc(res_win_t wt, uid_t sid)
{
	return GETXDKSUBPROC(wt);
}

bool_t _widget_set_subproc(res_win_t wt, uid_t sid, if_subproc_t* sub)
{
	BOOL b;
	DWORD_PTR p = 0;
	if_subproc_t* psub;
	xrect_t xr = { 0 };

	if (!sub)
		return 0;

	psub = GETXDKSUBPROC(wt);
	if (psub)
		return 0;

#ifdef WINCE
	psub->sid = sid;
	psub->proc = (void*)GetWindowLong(wt, GWL_WNDPROC);
	SetWindowLong(wt, GWL_WNDPROC, (LONG)XdcSubclassProc);

	psub = (if_subproc_t*)HeapAlloc(GetProcessHeap(NULL),sizeof(if_subproc_t));
	CopyMemory((void*)psub, (void*)sub, sizeof(if_subproc_t));
#else
	b = GetWindowSubclass(wt, (SUBCLASSPROC)XdcSubclassProc, (UINT_PTR)sid, &p);
	if (b)
		return 0;

	b = SetWindowSubclass(wt, (SUBCLASSPROC)XdcSubclassProc, (UINT_PTR)sid, (DWORD_PTR)0);
	if (!b)
		return 0;

	psub = (if_subproc_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(if_subproc_t));
	CopyMemory((void*)psub, (void*)sub, sizeof(if_subproc_t));
#endif

	SETXDKSUBPROC(wt, psub);

	if (psub->sub_on_subing)
	{
		(*psub->sub_on_subing)(wt, sid, psub->delta);
	}

	if (psub->sub_on_size)
	{
		_widget_get_client_rect(wt, &xr);

		(*psub->sub_on_size)(wt, 0, RECTSIZE(&xr), sid, psub->delta);
	}

	return 1;
}

void _widget_del_subproc(res_win_t wt, uid_t sid)
{
	if_subproc_t* psub;

	psub = GETXDKSUBPROC(wt);
	if (!psub)
		return;

#ifdef WINCE
	SetWindowLong(wt, GWL_WNDPROC, (LONG)psub->proc);
#else
	RemoveWindowSubclass(wt, (SUBCLASSPROC)XdcSubclassProc, (UINT_PTR)sid);
#endif

	HeapFree(GetProcessHeap(), 0, psub);

	SETXDKSUBPROC(wt, NULL);
}

bool_t _widget_set_subproc_delta(res_win_t wt, uid_t sid, var_long delta)
{
	if_subproc_t* psub;

	psub = GETXDKSUBPROC(wt);

	psub->delta = delta;
	return 1;
}

var_long _widget_get_subproc_delta(res_win_t wt, uid_t sid)
{
	if_subproc_t* psub;

	psub = GETXDKSUBPROC(wt);

	return psub->delta;
}

bool_t _widget_has_subproc(res_win_t wt)
{
	return (GETXDKSUBPROC(wt) == NULL) ? 0 : 1;
}

void _widget_set_style(res_win_t wt, u32_t ws)
{
	SETXDKSTYLE(wt, ws);
}

u32_t _widget_get_style(res_win_t wt)
{
	return GETXDKSTYLE(wt);
}

void _widget_set_accel(res_win_t wt, res_acl_t acl)
{
	SETXDKACCEL(wt, acl);
}

res_acl_t _widget_get_accel(res_win_t wt)
{
	return GETXDKACCEL(wt);
}

void _widget_set_owner(res_win_t wt, res_win_t win)
{
	SETXDKOWNER(wt, win);
}

res_win_t _widget_get_owner(res_win_t wt)
{
	return GETXDKOWNER(wt);
}

void _widget_set_core_delta(res_win_t wt, var_long pd)
{
	SETXDKCOREDELTA(wt, pd);
}

var_long _widget_get_core_delta(res_win_t wt)
{
	return GETXDKCOREDELTA(wt);
}

void _widget_set_user_delta(res_win_t wt, var_long pd)
{
	SETXDKUSERDELTA(wt, pd);
}

var_long _widget_get_user_delta(res_win_t wt)
{
	return GETXDKUSERDELTA(wt);
}

void _widget_set_user_id(res_win_t wt, uid_t uid)
{
#ifdef WINCE
	if (GetWindowLong(wt, GWL_STYLE) & WS_CHILD)
		SetWindowLong(wt, GWL_ID, (LONG)uid);
	else
		SETXDKUSER(wt, uid);
#else
	if (GetWindowLongPtr(wt, GWL_STYLE) & WS_CHILD)
		SetWindowLongPtr(wt, GWLP_ID, (LONG_PTR)uid);
	else
		SETXDKUSER(wt, uid);
#endif
}

uid_t _widget_get_user_id(res_win_t wt)
{
#ifdef WINCE
	if (GetWindowLong(wt, GWL_STYLE) & WS_CHILD)
		return (dword_t)GetWindowLong(wt, GWL_ID);
	else
		return GETXDKUSER(wt);
#else
	if (GetWindowLongPtr(wt, GWL_STYLE) & WS_CHILD)
		return (uid_t)GetWindowLongPtr(wt, GWLP_ID);
	else
		return GETXDKUSER(wt);
#endif
}

void _widget_set_user_result(res_win_t wt, int rt)
{
	SETXDKRESULT(wt, rt);
}

int _widget_get_user_result(res_win_t wt)
{
	return (int)GETXDKRESULT(wt);
}

res_win_t _widget_get_child(res_win_t wt, uid_t uid)
{
	return GetDlgItem(wt, uid);
}

res_win_t _widget_get_parent(res_win_t wt)
{
	return (res_win_t)GetParent(wt);
}

void _widget_set_user_prop(res_win_t wt, const tchar_t* pname, var_long pval)
{
	SetProp(wt, pname, (HANDLE)pval);
}

var_long _widget_get_user_prop(res_win_t wt, const tchar_t* pname)
{
	return (var_long)GetProp(wt, pname);
}

var_long _widget_del_user_prop(res_win_t wt, const tchar_t* pname)
{
	return (var_long)RemoveProp(wt, pname);
}

if_event_t* _widget_get_dispatch(res_win_t wt)
{
	return GETXDKDISPATCH(wt);
}

void _widget_calc_border(u32_t ws, border_t* pbd)
{
	xsize_t xs;

	pbd->edge = pbd->title = pbd->hscroll = pbd->vscroll = pbd->menu = pbd->icon = 0;

	if (ws & WD_STYLE_TITLE)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_TITLE_SPAN;
		_screen_size_to_pt(&xs);

		pbd->title = xs.cy;
	}

	if (ws & WD_STYLE_BORDER)
	{
		xs.fx = ZERO_WIDTH;
		if (ws & WD_STYLE_CHILD)
			xs.fy = WIDGET_CHILD_EDGE;
		else
			xs.fy = WIDGET_FRAME_EDGE;
		_screen_size_to_pt(&xs);

		pbd->edge = xs.cy;
	}

	if (ws & WD_STYLE_HSCROLL)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_SCROLL_SPAN;
		_screen_size_to_pt(&xs);

		pbd->hscroll = xs.cy;
	}

	if (ws & WD_STYLE_VSCROLL)
	{
		xs.fx = WIDGET_SCROLL_SPAN;
		xs.fy = ZERO_HEIGHT;
		_screen_size_to_pt(&xs);

		pbd->vscroll = xs.cx;
	}

	if (ws & WD_STYLE_MENUBAR)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_MENU_SPAN;
		_screen_size_to_pt(&xs);

		pbd->menu = xs.cy;
	}

	xs.fx = ZERO_WIDTH;
	xs.fy = WIDGET_ICON_SPAN;
	_screen_size_to_pt(&xs);
	pbd->icon = xs.cy;
}

void _widget_adjust_size(u32_t ws, xsize_t* pxs)
{
#ifdef WINCE
	RECT rt = { 0 };

	AdjustWindowRectEx(&rt, _WindowStyle(ws), 0, 0);

	pxs->cx = rt.right - rt.left;
	pxs->cy = rt.bottom - rt.top;
#else
	if (ws & WD_STYLE_OWNERNC)
	{
		border_t bd = { 0 };

		_widget_calc_border(ws, &bd);

		pxs->cx += (2 * bd.edge + bd.vscroll);
		pxs->cy += (2 * bd.edge + bd.title + bd.hscroll + bd.menu);
	}
	else
	{
		RECT rt = { 0 };
		rt.right = pxs->cx;
		rt.bottom = pxs->cy;

		AdjustWindowRect(&rt, _WindowStyle(ws), FALSE);

		pxs->cx = rt.right - rt.left;
		pxs->cy = rt.bottom - rt.top;
	}
#endif

	pxs->cx += 2;
	pxs->cy += 2;
}

bool_t _widget_is_maximized(res_win_t wt)
{
	return (IsZoomed((HWND)wt)) ? 1 : 0;
}

bool_t _widget_is_minimized(res_win_t wt)
{
	return (IsIconic((HWND)wt)) ? 1 : 0;
}

bool_t _widget_enum_child(res_win_t widget, PF_ENUM_WINDOW_PROC pf, var_long pv)
{
#ifdef WINCE
	HWND hWnd;

	hWnd = GetWindow(widget, GW_CHILD);
	while(hWnd)
	{
		(*pf)(hWnd,pv);

		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
	return 1;
#else
	return (bool_t)EnumChildWindows(widget, (WNDENUMPROC)pf, (LPARAM)pv);
#endif
}


res_ctx_t _widget_client_ctx(res_win_t wt)
{
	return GetDC(wt);
}

res_ctx_t _widget_window_ctx(res_win_t wt)
{
	return GetWindowDC(wt);
}

void _widget_release_ctx(res_win_t wt, res_ctx_t dc)
{
	ReleaseDC(wt, dc);
}

void _widget_get_client_rect(res_win_t wt, xrect_t* prt)
{
	RECT rt;

	GetClientRect(wt, &rt);

	prt->x = rt.left;
	prt->y = rt.top;
	prt->w = rt.right - rt.left;
	prt->h = rt.bottom - rt.top;
}

void _widget_get_window_rect(res_win_t wt, xrect_t* prt)
{
	RECT rt;

	GetWindowRect(wt, &rt);

	prt->x = rt.left;
	prt->y = rt.top;
	prt->w = rt.right - rt.left;
	prt->h = rt.bottom - rt.top;
}

void _widget_client_to_screen(res_win_t wt, xpoint_t* ppt)
{
	POINT pt;

	pt.x = ppt->x;
	pt.y = ppt->y;

	ClientToScreen((HWND)wt, &pt);

	ppt->x = pt.x;
	ppt->y = pt.y;
}

void _widget_screen_to_client(res_win_t wt, xpoint_t* ppt)
{
	POINT pt;

	pt.x = ppt->x;
	pt.y = ppt->y;

	ScreenToClient((HWND)wt, &pt);

	ppt->x = pt.x;
	ppt->y = pt.y;
}

void _widget_client_to_window(res_win_t wt, xpoint_t* ppt)
{
	POINT pt = { 0 };
	RECT rt;

	ClientToScreen(wt, &pt);
	GetWindowRect(wt, &rt);

	pt.x -= rt.left;
	pt.y -= rt.top;

	ppt->x += pt.x;
	ppt->y += pt.y;
}

void _widget_window_to_client(res_win_t wt, xpoint_t* ppt)
{
	POINT pt = { 0 };
	RECT rt;
	
	GetWindowRect(wt, &rt);
	pt.x = rt.left;
	pt.y = rt.top;
	ScreenToClient((HWND)wt, &pt);

	ppt->x += pt.x;
	ppt->y += pt.y;
}

void _widget_center_window(res_win_t wt, res_win_t owner)
{
	RECT rtChild, rtOwner;
	int cx, cy;
	BOOL bChild;

	owner = (owner) ? owner : GetDesktopWindow();

	bChild = GetWindowLong(wt, GWL_STYLE) & WS_CHILD;
	if (bChild)
	{
		GetClientRect(owner, &rtOwner);
	}
	else
	{
		GetWindowRect(owner, &rtOwner);
	}

	GetWindowRect(wt, &rtChild);
	cx = rtChild.right - rtChild.left;
	cy = rtChild.bottom - rtChild.top;

	rtChild.left = rtOwner.left + (rtOwner.right - rtOwner.left - cx) / 2;
	rtChild.right = rtOwner.right - (rtOwner.right - rtOwner.left - cx) / 2;
	rtChild.top = rtOwner.top + (rtOwner.bottom - rtOwner.top - cy) / 2;
	rtChild.bottom = rtOwner.bottom - (rtOwner.bottom - rtOwner.top - cy) / 2;

#ifdef WINCE
	SetWindowPos(wt, NULL, rtChild.left, rtChild.top, rtChild.right - rtChild.left, rtChild.bottom - rtChild.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
#else
	SetWindowPos(wt, NULL, rtChild.left, rtChild.top, rtChild.right - rtChild.left, rtChild.bottom - rtChild.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
#endif
}

void _widget_set_cursor(res_win_t wt, int curs)
{
	switch (curs)
	{
	case CURSOR_SIZENS:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		break;
	case CURSOR_SIZEWE:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		break;
	case CURSOR_SIZEALL:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
		break;
	case CURSOR_HAND:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		break;
	case CURSOR_HELP:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HELP)));
		break;
	case CURSOR_DRAG:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_APPSTARTING)));
		break;
	case CURSOR_ARROW:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		break;
	case CURSOR_IBEAM:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		break;
	default:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		break;
	}
}

void _widget_set_capture(res_win_t wt, bool_t b)
{
	if (b)
		SetCapture((HWND)wt);
	else
		ReleaseCapture();
}


var_long _widget_set_timer(res_win_t wt, int ms)
{
	return (var_long)SetTimer(wt, IDC_TIMER, ms, NULL);
}

void _widget_kill_timer(res_win_t wt, var_long tid)
{
	if (tid)
		KillTimer(wt, (UINT_PTR)tid);
	else
		KillTimer(wt, (UINT_PTR)IDC_TIMER);
}

void _widget_create_caret(res_win_t wt, int w, int h)
{
	CreateCaret(wt, NULL, w, h);
}

void _widget_destroy_caret(res_win_t wt)
{
	DestroyCaret();
}

void _widget_show_caret(res_win_t wt, int x, int y, bool_t b)
{
	SetCaretPos(x, y);

	if (b)
	{
		ShowCaret(wt);
	}
	else
	{
		HideCaret(wt);
	}
}

void _widget_set_imm(res_win_t wt, bool_t b)
{
	HIMC hMic;

	hMic = ImmGetContext(wt);
	ImmSetOpenStatus(hMic, b);
	ImmReleaseContext(wt, hMic);
}

bool_t _widget_get_imm(res_win_t wt)
{
	HIMC hMic;
	BOOL bOpen;

	hMic = ImmGetContext(wt);
	bOpen = ImmGetOpenStatus(hMic);
	ImmReleaseContext(wt, hMic);

	return (bOpen) ? 1 : 0;
}

void _widget_set_focus(res_win_t wt)
{
	SetFocus((HWND)wt);
}

bool_t _widget_key_state(res_win_t wt, int key)
{
	int ks;

	switch (key)
	{
	case KEY_SHIFT:
		ks = GetKeyState(VK_SHIFT);
		break;
	case KEY_CONTROL:
		ks = GetKeyState(VK_CONTROL);
		break;
	default:
		ks = 0;
	}

	return (ks & 0x8000) ? 1 : 0;
}

bool_t _widget_is_valid(res_win_t wt)
{
	return (IsWindow((HWND)wt)) ? 1 : 0;
}

bool_t _widget_is_focus(res_win_t wt)
{
	return (GetFocus() == wt) ? 1 : 0;
}

bool_t _widget_is_child(res_win_t wt)
{
#ifdef WINCE
	return (GetWindowLong(wt, GWL_STYLE) & WS_CHILD) ? 1 : 0;
#else
	return (GetWindowLongPtr(wt, GWL_STYLE) & WS_CHILD) ? 1 : 0;
#endif
}

bool_t _widget_is_ownc(res_win_t wt)
{
#ifdef WINCE
	return 0;
#else
	return (GETXDKSTYLE(wt) & WD_STYLE_OWNERNC) ? 1 : 0;
#endif
}

void _widget_post_char(res_win_t wt, tchar_t ch)
{
	INPUT input[2] = { 0 };

	if (IsWindow(wt))
	{
		PostMessage(wt, WM_CHAR, (WPARAM)ch, (LPARAM)1);
	}
	else
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = 0;
		input[0].ki.wScan = ch;
#ifdef WINCE
		input[0].ki.dwFlags = 0;
#else
		input[0].ki.dwFlags = KEYEVENTF_UNICODE;
#endif
		SendInput(1, input, sizeof(INPUT));

		input[1].type = INPUT_KEYBOARD;
		input[1].ki.wVk = 0;
		input[1].ki.wScan = ch;
#ifdef WINCE
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;
#else
		input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
#endif
		SendInput(1, input + 1, sizeof(INPUT));
	}
}

void _widget_post_key(res_win_t wt, int key)
{
	INPUT input[2] = { 0 };

	if (IsWindow(wt))
	{
		PostMessage((HWND)wt, WM_KEYDOWN, (WPARAM)key, (LPARAM)1);
		PostMessage((HWND)wt, WM_KEYUP, (WPARAM)key, (LPARAM)1);
	}
	else
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = key;
		input[0].ki.wScan = 1;
		input[0].ki.dwFlags = 0;
		SendInput(1, input, sizeof(INPUT));

		input[1].type = INPUT_KEYBOARD;
		input[1].ki.wVk = key;
		input[1].ki.wScan = 1;
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, input + 1, sizeof(INPUT));
	}
}

void _widget_move(res_win_t wt, const xpoint_t* ppt)
{
	RECT rt;

	GetWindowRect(wt, &rt);
	rt.left = ppt->x;
	rt.top = ppt->y;

#ifdef WINCE
	SetWindowPos(wt, NULL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
#else
	SetWindowPos(wt, NULL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
#endif
}

void _widget_size(res_win_t wt, const xsize_t* pxs)
{
	RECT rt;

	GetWindowRect(wt, &rt);
	rt.right = rt.left + pxs->cx;
	rt.bottom = rt.top + pxs->cy;

#ifdef WINCE
	SetWindowPos(wt, NULL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
#else
	SetWindowPos(wt, NULL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
#endif
}

void _widget_take(res_win_t wt, int zor)
{
#ifdef WINCE
	SetWindowPos(wt, (HWND)zor, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
#else
	SetWindowPos(wt, (HWND)zor, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOREDRAW);
#endif
}

void _widget_show(res_win_t wt, u32_t sw)
{
	RECT rt;
	DWORD dw = GETXDKSTYLE(wt);

	if (sw == WD_SHOW_HIDE)
	{
		ShowWindow(wt, SW_HIDE);
	}
	else if (sw == WD_SHOW_NORMAL)
	{
		if (dw & WD_STYLE_NOACTIVE)
		{
			ShowWindow(wt, SW_SHOWNA);
		}
		else
		{
			ShowWindow(wt, SW_SHOW);
		}
	}
	else if (sw == WD_SHOW_MAXIMIZE)
	{
		ShowWindow(wt, SW_MAXIMIZE);
	}
	else if (sw == WD_SHOW_MINIMIZE)
	{
		ShowWindow(wt, SW_MINIMIZE);
	}
	else if (sw == WD_SHOW_FULLSCREEN)
	{
		rt.left = rt.top = 0;
		rt.right = GetSystemMetrics(SM_CXFULLSCREEN);
		rt.bottom = GetSystemMetrics(SM_CYFULLSCREEN);

#ifdef WINCE
		AdjustWindowRectEx(&rt, (DWORD)GetWindowLong(wt, GWL_STYLE), 0, 0);
#else
		AdjustWindowRect(&rt, (DWORD)GetWindowLongPtr(wt, GWL_STYLE), 0);
#endif

		if (dw & WD_STYLE_NOACTIVE)
			SetWindowPos(wt, HWND_TOP, 0, 0, rt.right - rt.left, rt.bottom - rt.top, SWP_SHOWWINDOW | SWP_NOACTIVATE);
		else
			SetWindowPos(wt, HWND_TOP, 0, 0, rt.right - rt.left, rt.bottom - rt.top, SWP_SHOWWINDOW);
	}
	else if (sw == WD_SHOW_POPUPTOP)
	{
		if (dw & WD_STYLE_NOACTIVE)
			SetWindowPos(wt, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
		else
			SetWindowPos(wt, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
}

void _widget_update(res_win_t wt, const xrect_t* prt, bool_t b_erase)
{
	RECT rt;

	if (prt)
	{
		rt.left = prt->x;
		rt.top = prt->y;
		rt.right = prt->x + prt->w;
		rt.bottom = prt->y + prt->h;
		InvalidateRect(wt, &rt, (BOOL)b_erase);
	}
	else
	{
		InvalidateRect(wt, NULL, (BOOL)b_erase);
	}
}

void _widget_update_window(res_win_t wt)
{
	SetWindowPos(wt, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOCOPYBITS | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	PostMessage(wt, WM_NCPAINT, 1, 0);
}

void _widget_update_client(res_win_t wt)
{
	RECT rt;

	GetClientRect(wt, &rt);

	SendMessage(wt, WM_SIZE, 0, MAKELPARAM(rt.right - rt.left, rt.bottom - rt.top));
}

void _widget_enable(res_win_t wt, bool_t b)
{
	if (b)
		EnableWindow(wt, 1);
	else
		EnableWindow(wt, 0);
}

void _widget_post_message(res_win_t wt, int msg, var_long wp, var_long lp)
{
	PostMessage((HWND)wt, msg, (WPARAM)wp, (LPARAM)lp);
}

int _widget_send_message(res_win_t wt, int msg, var_long wp, var_long lp)
{
	return (int)SendMessage((HWND)wt, msg, (WPARAM)wp, (LPARAM)lp);
}

void _widget_post_command(res_win_t wt, int code, uid_t cid, var_long data)
{
	PostMessage((HWND)wt, WM_COMMAND, MAKEWPARAM(cid,code), (LPARAM)data);
}

int _widget_send_command(res_win_t wt, int code, uid_t cid, var_long data)
{
	return (int)SendMessage((HWND)wt, WM_COMMAND, MAKEWPARAM(cid, code), (LPARAM)data);
}

void _widget_set_title(res_win_t wt, const tchar_t* token)
{
	DWORD dw;
#ifdef WINCE
	dw = GetWindowLong(wt, GWL_STYLE);
#else
	dw = (DWORD)GetWindowLongPtr(wt, GWL_STYLE);
#endif

	if (dw & WS_CAPTION)
	{
		SetWindowText(wt, token);
	}
}

int _widget_get_title(res_win_t wt, tchar_t* buf, int max)
{
	DWORD dw;
	int len;

#ifdef WINCE
	dw = GetWindowLong(wt, GWL_STYLE);
#else
	dw = (DWORD)GetWindowLongPtr(wt, GWL_STYLE);
#endif

	if (dw & WS_CAPTION)
	{
		len = GetWindowTextLength(wt);
		if (buf)
		{
			len = (len < max) ? len : max;
			GetWindowText(wt, buf, len + 1);
		}
		return len;
	}

	return 0;
}

void _widget_get_scroll_info(res_win_t wt, bool_t horz, scroll_t* psl)
{
	SCROLLINFO si = { 0 };
	int bar;

	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;

	if (horz)
		bar = SB_HORZ;
	else
		bar = SB_VERT;

	GetScrollInfo(wt, bar, &si);

	psl->max = si.nMax;
	psl->min = si.nMin;
	psl->page = (long)(si.nPage);
	psl->pos = si.nPos;
	psl->track = si.nTrackPos;
}

void _widget_set_scroll_info(res_win_t wt, bool_t horz, const scroll_t* psl)
{
	SCROLLINFO si = { 0 };
	int bar;

	si.cbSize = sizeof(si);
	si.nMax = psl->max;
	si.nMin = psl->min;
	si.nPage = psl->page;
	si.nPos = psl->pos;

	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;

	if (horz)
		bar = SB_HORZ;
	else
		bar = SB_VERT;

	if (GETXDKSTYLE(wt) & WD_STYLE_OWNERNC)
		SetScrollInfo(wt, bar, &si, FALSE);
	else
		SetScrollInfo(wt, bar, &si, TRUE);
}

void _widget_copy(res_win_t wt)
{
	SendMessage(((wt)? wt : GetFocus()), WM_COPY, 0, 0);
}

void _widget_cut(res_win_t wt)
{
	SendMessage(((wt) ? wt : GetFocus()), WM_CUT, 0, 0);
}

void _widget_paste(res_win_t wt)
{
	SendMessage(((wt) ? wt : GetFocus()), WM_PASTE, 0, 0);
}

void _widget_undo(res_win_t wt)
{
	SendMessage(((wt) ? wt : GetFocus()), WM_UNDO, 0, 0);
}


//////////////////////////////////////////////////////////////////////////////////

int _widget_do_modal(res_win_t hWnd)
{
	MSG msg = { 0 };
	BOOL bShow = FALSE;
	BOOL bModal = TRUE;
	HACCEL hAcl = NULL;
	int nRet = 0;
	/*HWND hWndParent;

	hWndParent = GetParent(hWnd);

	if (hWndParent && hWndParent != GetDesktopWindow() && IsWindowEnabled(hWndParent))
	{
		EnableWindow(hWndParent, FALSE);
	}*/

#ifdef WINCE
	bShow = GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE;
#else
	bShow = GetWindowLongPtr(hWnd, GWL_STYLE) & WS_VISIBLE;
#endif

	if (!bShow)
	{
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
	}

	do{
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			if (msg.hwnd == hWnd && msg.message == WM_SYSCOMMAND && (msg.wParam & 0xFFF0) == SC_CLOSE)
			{
				nRet = _widget_get_user_result(hWnd);
			}

			if (!GetMessage(&msg, NULL, NULL, NULL))
			{
				PostQuitMessage(-1);
				bModal = FALSE;
				break;
			}

			if (msg.message == WM_KEYDOWN)
			{
				hAcl = _widget_get_accel(hWnd);

				if (hAcl && TranslateAccelerator(hWnd, hAcl, &msg))
					continue;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (!IsWindow(hWnd))
			{
				bModal = FALSE;
				break;
			}
		}
	} while (bModal);

	/*if (hWndParent)
	{
		SetForegroundWindow(hWndParent);

		if (!IsWindowEnabled(hWndParent))
		{
			EnableWindow(hWndParent, TRUE);
		}
		//SetWindowPos(hWndParent, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_DEFERERASE);
	}*/

	return nRet;
}

void _widget_do_trace(res_win_t hWnd)
{
	MSG msg = { 0 };
	BOOL bShow = FALSE;
	BOOL bTrace = TRUE;

#ifdef WINCE
	bShow = GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE;
#else
	bShow = GetWindowLongPtr(hWnd, GWL_STYLE) & WS_VISIBLE;
#endif
	if (!bShow)
	{
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
	}

	SetCapture(hWnd);

	do{
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			if (msg.message == WM_KEYDOWN && msg.wParam == KEY_ESC)
			{
				GetMessage(&msg, NULL, NULL, NULL);//remove esc

				DestroyWindow(hWnd);
			}

			if (!GetMessage(&msg, NULL, NULL, NULL))
			{
				PostQuitMessage(-1);
				bTrace = FALSE;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (!IsWindow(hWnd))
			{
				bTrace = FALSE;
				break;
			}
		}
	} while (bTrace);

	ReleaseCapture();
}

/*********************************************************************************************************/
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

#ifdef XDK_SUPPORT_WIDGET_EX
void _widget_track_mouse(res_win_t wt, u32_t mask)
{
	TRACKMOUSEEVENT te = { 0 };

	te.cbSize = sizeof(TRACKMOUSEEVENT);

	if (mask & TME_HOVER)
	{
		te.dwFlags |= TME_HOVER;
		te.dwHoverTime = HOVER_DEFAULT;
	}

	if (mask & TME_LEAVE)
	{
		te.dwFlags |= TME_LEAVE;
	}

	if (mask & TME_CANCEL)
	{
		te.dwFlags |= TME_CANCEL;
	}

	te.hwndTrack = wt;

	TrackMouseEvent(&te);
}

void _widget_set_region(res_win_t wt, res_rgn_t rgn)
{
	SetWindowRgn(wt, rgn, TRUE);
}

void _widget_set_alpha(res_win_t wt, unsigned char b)
{
	DWORD dw = GetWindowLong(wt, GWL_EXSTYLE);

	SetWindowLong(wt, GWL_EXSTYLE, dw | WS_EX_LAYERED);

	SetLayeredWindowAttributes(wt, 0, (BYTE)b, LWA_ALPHA);
}

unsigned char _widget_get_alpha(res_win_t wt)
{
	BYTE b = 0;

	GetLayeredWindowAttributes(wt, NULL, &b, NULL);

	return (unsigned char)b;
}
#endif /*XDK_SUPPORT_WIDGET_EX*/

#endif //XDK_SUPPORT_WIDGET
