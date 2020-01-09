/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firegrid editor document

	@module	_if_firegrid_win.c | firegrid editor widnows implement file

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
#include "gridctrl.h"

typedef struct tagFireGridDelta{
	HWND hOwner;
	HWND hEdit;
	HWND hGrid;
	HWND hKey;
}FireGridDelta;

#define GETFIREGRIDDELTA(hWnd) 	((FireGridDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREGRIDDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireGridProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM RegisterFireGridClass(HINSTANCE hInstance);

LRESULT CALLBACK FireGridEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


void	FireGrid_AttachData(HWND hWnd,LINKPTR ptr);
LINKPTR	FireGrid_DetachData(HWND hWnd);
void	FireGrid_SendNotify(HWND hWnd, int code, void* data);
void	FireGrid_OnReset(HWND hWnd, int code);
void	FireGrid_RelayoutList(HWND hWnd);
void	FireGrid_RelayoutKey(HWND hWnd);

ATOM RegisterFireGridClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireGridProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIREGRIDCLASS;

	return RegisterClass(&wcex);
}


LRESULT CALLBACK GridEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK FireGridProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	RECT rt;
	WNDPROC lpfOrgProc;
	HIMC hMic;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireGridDelta*)xmem_alloc(sizeof(FireGridDelta));
		memset((void*)ptd,0,sizeof(FireGridDelta));
		SETFIREGRIDDELTA(hWnd,ptd);

		GetClientRect(hWnd,&rt);
		ptd->hEdit = CreateWindow(_T("EDIT"),NULL,WS_CHILD | WS_VISIBLE | WS_DISABLED,rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,hWnd,NULL,NULL,NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)GridEditProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit,TRUE);

		hMic = ImmGetContext(ptd->hEdit);
		ImmSetOpenStatus(hMic, FALSE);
		ImmReleaseContext(ptd->hEdit, hMic);

		if (XDC_TOUCHED)
		{
			ptd->hKey = CreateWindow(KEYCTRLCLASS, NULL, WS_POPUP | WS_BORDER, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
			FireGrid_RelayoutKey(hWnd);
		}

		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIREGRIDDELTA(hWnd,0);

		if (IsWindow(ptd->hKey))
		{
			DestroyWindow(ptd->hKey);
		}

		if(IsWindow(ptd->hGrid))
		{
			DestroyWindow(ptd->hGrid);
		}

		if(IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit,FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}
		break;
	case FE_RESET:
		SendMessage(ptd->hOwner, WM_COMMAND, wParam, (LPARAM)hWnd);
		return 0;
	case WM_COMMAND:
		if(HIWORD(wParam) == EN_UPDATE && (HWND)lParam == ptd->hEdit)
		{
			link_t_ptr ptr = grid_view_fetch(ptd->hGrid);
			link_t_ptr clk = get_next_visible_col(ptr, LINK_FIRST);
			if (!clk)
				break;

			int clen = xslen(get_col_name_ptr(clk)) + 3;
			int tlen = GetWindowTextLength(ptd->hEdit);
			tchar_t *token = (tchar_t*)xmem_alloc((clen + tlen + 1) * sizeof(tchar_t));
			xscat(token, get_col_name_ptr(clk));
			xscat(token, _T(" = "));
			GetWindowText(ptd->hEdit,token + clen,tlen + 1);

			grid_view_filter(ptd->hGrid, token);

			xmem_free(token);
		}else if(HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit && GetFocus() != ptd->hGrid && IsWindow(ptd->hGrid))
		{
			FireGrid_OnReset(hWnd, EC_COMMIT);
		}
		break;
	case WM_MOUSEWHEEL:
		if(ptd->hGrid)
		{
			PostMessage(ptd->hGrid,WM_MOUSEWHEEL,wParam,lParam);
			return 1;
		}
		return 0;
	case FE_ONKEY:
		if(wParam == VK_DOWN)
		{
			grid_view_tabskip(ptd->hGrid, NF_DOWN);
			return 1;
		}
		else if(wParam == VK_UP)
		{
			grid_view_tabskip(ptd->hGrid, NF_UP);
			return 1;
		}
		else if(wParam == 13) /*enter key*/
		{
			FireGrid_OnReset(hWnd, EC_COMMIT);
			return 1;
		}
		else if(wParam == 27) /*escape key*/
		{
			FireGrid_OnReset(hWnd, EC_ROLLBACK);
			return 1;
		}
		return 0;
	}

	return DefWindowProc(hWnd,message,wParam,lParam);		
}

void FireGrid_SendNotify(HWND hWnd, int code, void* data)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(hWnd);
	NOTICE_EDITOR ne = { 0 };

	ne.widget = hWnd;
	ne.id = GetWindowLong(hWnd, GWL_ID);
	ne.code = code;
	ne.data = data;

	SendMessage(ptd->hOwner, FE_NOTICE, 0, (LPARAM)&ne);
}

void FireGrid_OnReset(HWND hWnd, int code)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(hWnd);
	link_t_ptr rlk;

	if (code == EC_COMMIT)
	{
		rlk = grid_view_get_focus_row(ptd->hGrid);
		FireGrid_SendNotify(hWnd, EC_COMMIT, (void*)rlk);
	}
	else if (code == EC_ROLLBACK)
	{
		FireGrid_SendNotify(hWnd, EC_ROLLBACK, NULL);
	}
	else if (code == EC_CLEAN)
	{
		FireGrid_SendNotify(hWnd, EC_COMMIT, NULL);
	}
}

void FireGrid_RelayoutKey(HWND hWnd)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(hWnd);
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

void FireGrid_RelayoutList(HWND hWnd)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(hWnd);
	link_t_ptr ptr, plk;
	int count;
	long w, h, cy, cx;
	RECT rt, rtDesk;
	xpoint_t pt1, pt2;
	WINDOWINFO wi = { 0 };

	wi.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(ptd->hGrid, &wi);

	cx = wi.cxWindowBorders * 2;
	cy = wi.cyWindowBorders * 2;

	ptr = grid_view_fetch(ptd->hGrid);

	w = 0;// calc_grid_width(NULL, ptr);
	h = get_grid_title_height(ptr) + get_grid_colbar_height(ptr);
	count = 0;

	plk = get_next_visible_row(ptr, LINK_FIRST);
	while (plk)
	{
		h += get_grid_rowbar_height(ptr);

		count++;
		if (count > 7)
			break;

		plk = get_next_visible_row(ptr, plk);
	}

	pt1.x = w;
	pt1.y = h;
	grid_view_tm_to_pt(ptd->hGrid, &pt1);

	pt2.x = 0;
	pt2.y = 0;
	grid_view_tm_to_pt(ptd->hGrid, &pt2);

	cx += (pt1.x - pt2.x);
	cy += (pt1.y - pt2.y);

	GetWindowRect(GetDesktopWindow(), &rtDesk);

	GetWindowRect(ptd->hGrid, &rt);

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

	SetWindowPos(ptd->hGrid, HWND_TOP, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

/****************************************************************************************************/
res_win_t firegrid_create(res_win_t widget, const xrect_t* pxr)
{
	RECT rt;
	HWND hEditor;

	rt.left = pxr->x;
	rt.top = pxr->y;
	rt.right = pxr->x + pxr->w;
	rt.bottom = pxr->y + pxr->h;

	MapWindowPoints(widget, HWND_DESKTOP, (LPPOINT)&rt, 2);

	hEditor = CreateWindow(FIREGRIDCLASS, NULL, WS_POPUP | WS_BORDER | WS_VISIBLE, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, widget, NULL, NULL, NULL);
	return (res_win_t)hEditor;
}

void firegrid_attach(res_win_t fw, LINKPTR ptr)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(fw);
	xrect_t xr;

	widget_get_client_rect(fw, &xr);

	ptd->hGrid = grid_view_create(NULL, WIDS_POPUP, &xr, fw);
	grid_view_enable_fire(fw, 1);
	grid_view_set_owner(fw, fw);
	grid_view_attach(fw, ptr);

	FireGrid_RelayoutList(fw);

	SetFocus(ptd->hEdit);
}

LINKPTR firegrid_detach(res_win_t fw)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(fw);
	LINKPTR ptr;

	ptr = grid_view_detach(ptd->hGrid);

	if (IsWindow(ptd->hGrid))
	{
		DestroyWindow(ptd->hGrid);
		ptd->hGrid = NULL;
	}

	return ptr;
}

void firegrid_set_owner(res_win_t fw, res_win_t owner)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(fw);

	ptd->hOwner = owner;
}

res_win_t firegrid_set_owner(res_win_t fw)
{
	FireGridDelta* ptd = GETFIREGRIDDELTA(fw);

	return ptd->hOwner;
}