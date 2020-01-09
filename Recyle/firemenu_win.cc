/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	firemenu.cc | firemenu control implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

/************************************************************
*************************************************************/

#include "xdleditor.h"
#include "xdlcanv.h"
#include "xdlgdi.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlutil.h"
#include "xdlnoti.h"

#include "menudoc.h"
#include "menuview.h"

#include "domdoc.h"
#include "domview.h"

typedef struct tagFireMenulDelta{
	HWND hWnd;
	HWND hOwner;
	handle_t canvas;
	LINKPTR data;
	LINKPTR curItem;

	BOOL bDraw;
}FireMenuDelta;

#define GETFIREMENUDELTA(hWnd) 	((FireMenuDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREMENUDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM	RegisterFireMenuClass(HINSTANCE hInstance);

void FireMenu_OnMouseMove(HWND hWnd, UINT nFlags, short x, short y);
void FireMenu_OnLButtonDown(HWND hWnd,UINT flag,short x,short y);
void FireMenu_OnLButtonUp(HWND hWnd,UINT flag,short x,short y);
void FireMenu_OnMouseWheel(HWND hWnd,UINT flag, short delta);
void FireMenu_OnPaint(HWND hWnd);

void FireMenu_OnDrawBackground(HWND hWnd, HDC hDC);
void FireMenu_OnDrawForeground(HWND hWnd, HDC hDC);
void FireMenu_OnItemChanging(HWND hWnd);
void FireMenu_OnItemChanging(HWND hWnd,LINKPTR ilk);

void FireMenu_OnResetScroll(HWND hWnd);
void FireMenu_OnResetPage(HWND hWnd);

void FireMenu_AttachData(HWND hWnd, LINKPTR ptr);
LINKPTR FireMenu_DetachData(HWND hWnd);
void FireMenu_TabSkip(HWND hWnd, int nf);
void FireMenu_SetCurItem(HWND hWnd, LINKPTR ilk);
void FireMenu_ReDraw(HWND hWnd);
void FireMenu_ReLayout(HWND hWnd);

ATOM RegisterFireMenuClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireMenuProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIREMENUCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK FireMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	LPCREATESTRUCT lpcs;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireMenuDelta*)xmem_alloc(sizeof(FireMenuDelta));
		memset((void*)ptd,0,sizeof(FireMenuDelta));
		ptd->canvas = create_display_canvas(NULL);

		SETFIREMENUDELTA(hWnd,ptd);

		SetCapture(hWnd);
		break;
	case WM_DESTROY:
		ReleaseCapture();

		if (ptd)
		{
			destroy_display_canvas(ptd->canvas);
		}
		SETFIREMENUDELTA(hWnd,0);
		xmem_free(ptd);
		break;
	case WM_MOUSEMOVE:
		if (ptd && ptd->data)
			FireMenu_OnMouseMove(hWnd, wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		if (ptd && ptd->data)
			FireMenu_OnLButtonDown(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if (ptd && ptd->data)
			FireMenu_OnLButtonUp(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		if (ptd && ptd->data)
			FireMenu_OnMouseWheel(hWnd,(UINT)LOWORD(wParam),(short)GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_PAINT:
		if(ptd && GetUpdateRect(hWnd,NULL,TRUE))
		{
			FireMenu_OnPaint(hWnd);		
		}
		break;
	case WM_ATTACHDATA:
		if (ptd && ptd->data)
			FireMenu_DetachData(hWnd);
		FireMenu_AttachData(hWnd, (LINKPTR)lParam);
		return 0;
	case WM_DETACHDATA:
		return (LRESULT)FireMenu_DetachData(hWnd);
	case WM_FETCHDATA:
		if (ptd)
			return (LRESULT)ptd->data;
		else
			return 0;
	case WM_SETOWNER:
		if (ptd)
			ptd->hOwner = (HWND)lParam;
		return 0;
	case WM_GETOWNER:
		return (LRESULT)ptd->hOwner;
	case WM_TABSKIP:
		if (ptd && ptd->data)
			FireMenu_TabSkip(hWnd,(int)wParam);
		return 0;
	case WM_GETCURSEL:
		if (ptd && ptd->data)
			return (LRESULT)ptd->curItem;
		else
			return 0;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);	
}

void CalcMenuItemRect(HWND hWnd, LINKPTR plk, RECT* pRect)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	xrect_t xr;

	assert(plk);

	calc_menu_item_rect(ptd->data, plk, &xr);

	pRect->left = mm_to_pt(ptd->canvas, xr.x, 1);
	pRect->right = mm_to_pt(ptd->canvas, xr.x + xr.w, 1);
	pRect->top = mm_to_pt(ptd->canvas, xr.y, 0);
	pRect->bottom = mm_to_pt(ptd->canvas, xr.y + xr.h, 0);
}

void FireMenu_OnSize(HWND hWnd, UINT nType, int width, int height)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);

	if (nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED)
		FireMenu_ReDraw(hWnd);
}

void FireMenu_OnMouseMove(HWND hWnd, UINT nFlags, short x, short y)
{
	// TODO: Add your message handler code here and/or call default
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	int nHint;
	LINKPTR plk;
	xpoint_t pt;

	pt.x = pt_to_mm(ptd->canvas, x, 1);
	pt.y = pt_to_mm(ptd->canvas, y, 0);
	plk = NULL;

	nHint = calc_menu_point_hint(ptd->data, &pt, &plk);

	if (plk != ptd->curItem)
		FireMenu_SetCurItem(hWnd, plk);
}

void FireMenu_OnLButtonDown(HWND hWnd,UINT flag,short x,short y)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);

}

void FireMenu_OnLButtonUp(HWND hWnd,UINT flag,short x,short y)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	UINT id;

	if (ptd->curItem)
	{
		id = get_menu_item_id(ptd->curItem);
		PostMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(id, 0), 0);
	}

	DestroyWindow(hWnd);
}

void FireMenu_OnMouseWheel(HWND hWnd,UINT nFlags,short delta)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	
}

void FireMenu_OnPaint(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rt;

	BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rt);

	hDC = (HDC)begin_display_paint(ptd->canvas, (res_dc_t)ps.hdc, rt.right - rt.left, rt.bottom - rt.top);

	FireMenu_OnDrawBackground(hWnd, hDC);

	draw_menu(ptd->canvas, ptd->data, ptd->curItem);

	FireMenu_OnDrawForeground(hWnd, hDC);

	end_display_paint(ptd->canvas, (res_dc_t)ps.hdc, ps.rcPaint.left, ps.rcPaint.top);

	EndPaint(hWnd, &ps);
}

/************************************************************************************************************/
void FireMenu_OnDrawBackground(HWND hWnd, HDC hDC)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	RECT rt;
	xbrush_t xb = { 0 };

	parse_xbrush_from_style(&xb, DEF_MENU_BRUSH);

	GetClientRect(hWnd, &rt);

	fill_shape_raw((res_dc_t)hDC, &xb, rt.left, rt.top, rt.right, rt.bottom, ATTR_SHAPE_RECT);
}

void FireMenu_OnDrawForeground(HWND hWnd, HDC hDC)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	/*RECT rt;
	xcolor_t xc;

	if(ptd->curItem)
	{
	parse_xcolor(&xc,DEF_FOCUS_COLOR);

	CalcMenuItemRect(hWnd,ptd->curItem,&rt);

	draw_focus_raw((res_dc_t)hDC,&xc,rt.left,rt.top,rt.right,rt.bottom);
	}*/
}

void FireMenu_OnItemChanging(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	RECT rt;

	assert(ptd->curItem);

	CalcMenuItemRect(hWnd, ptd->curItem, &rt);
	ptd->curItem = NULL;

	rt.left -= DEF_FOCUS_FEED;
	rt.top -= DEF_FOCUS_FEED;
	rt.right += DEF_FOCUS_FEED;
	rt.bottom += DEF_FOCUS_FEED;

	ptd->bDraw = FALSE;
	InvalidateRect(hWnd, &rt, FALSE);
}

void FireMenu_OnItemChanged(HWND hWnd, LINKPTR plk)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	RECT rt;

	assert(!ptd->curItem);

	ptd->curItem = plk;

	CalcMenuItemRect(hWnd, ptd->curItem, &rt);
	rt.left -= DEF_FOCUS_FEED;
	rt.top -= DEF_FOCUS_FEED;
	rt.right += DEF_FOCUS_FEED;
	rt.bottom += DEF_FOCUS_FEED;

	ptd->bDraw = FALSE;
	InvalidateRect(hWnd, &rt, FALSE);
}

void FireMenu_OnResetScroll(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
}

void FireMenu_OnResetPage(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	RECT rt;
	float pw, ph;

	GetClientRect(hWnd, &rt);
	pw = pt_to_mm(ptd->canvas, rt.right, 1) - pt_to_mm(ptd->canvas, rt.left, 1);
	ph = pt_to_mm(ptd->canvas, rt.bottom, 0) - pt_to_mm(ptd->canvas, rt.top, 0);
	set_menu_page_width(ptd->data, pw);
	set_menu_page_height(ptd->data, ph);
}
/*******************************************************************************************************/
void FireMenu_AttachData(HWND hWnd, LINKPTR ptr)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);

	ptd->data = ptr;
	ptd->curItem = NULL;

	FireMenu_ReLayout(hWnd);

	if (ptd->data)
	{
		FireMenu_ReDraw(hWnd);
	}
}

LINKPTR FireMenu_DetachData(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	LINKPTR data;

	if (ptd->data)
	{
		data = ptd->data;
		ptd->data = NULL;
		ptd->curItem = NULL;
	}
	else
	{
		data = NULL;
	}
	return data;
}

void FireMenu_ReDraw(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);

	assert(ptd->data && ptd->canvas);

	if (!is_menu_item(ptd->data, ptd->curItem))
	{
		ptd->curItem = NULL;
	}

	FireMenu_OnResetPage(hWnd);
	FireMenu_OnResetScroll(hWnd);

	ptd->bDraw = TRUE;
	InvalidateRect(hWnd, NULL, FALSE);
}

void FireMenu_TabSkip(HWND hWnd, int nSkip)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	LINKPTR plk = ptd->curItem;

	switch (nSkip)
	{
	case NF_LEFT:
	case NF_DOWN:
		if (plk == NULL)
			plk = get_menu_first_child_item(ptd->data);
		else
			plk = get_menu_next_sibling_item(ptd->curItem);
		break;
	case NF_RIGHT:
	case NF_UP:
		if (plk == NULL)
			plk = get_menu_last_child_item(ptd->data);
		else
			plk = get_menu_prev_sibling_item(ptd->curItem);
		break;
	case NF_HOME:
		plk = get_menu_first_child_item(ptd->data);
		break;
	case NF_END:
		plk = get_menu_last_child_item(ptd->data);
		break;
	}

	FireMenu_SetCurItem(hWnd, plk);
}

void FireMenu_SetCurItem(HWND hWnd, LINKPTR ilk)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	BOOL bRe;

	bRe = (ilk == ptd->curItem) ? TRUE : FALSE;
	if (bRe)
		return;

	if (ptd->curItem && !bRe)
	{
		FireMenu_OnItemChanging(hWnd);
	}

	if (ilk && !bRe)
	{
		FireMenu_OnItemChanged(hWnd, ilk);
	}
}

void FireMenu_ReLayout(HWND hWnd)
{	
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	RECT rt;
	int cx, cy;
	int count,nWidth,nHeight;

	//client edge 
	GetWindowRect(hWnd,&rt);
	cy = rt.bottom - rt.top;
	cx = rt.right - rt.left;

	GetClientRect(hWnd,&rt);
	cy = cy - (rt.bottom - rt.top);
	cx = cx - (rt.right - rt.left);

	count = get_menu_child_item_count(ptd->data);
	if(count == 0)
	{
		return;
	}	

	if(count > 7)
		count = 7;

	nWidth = mm_to_pt(ptd->canvas, get_menu_item_width(ptd->data), 1) - mm_to_pt(ptd->canvas, 0, 1);
	nHeight = mm_to_pt(ptd->canvas, count * get_menu_item_height(ptd->data), 0) - mm_to_pt(ptd->canvas, 0, 0);

	GetWindowRect(hWnd, &rt);
	rt.right = rt.left + nWidth + cx;
	rt.bottom = rt.top + nHeight + cy;

	MoveWindow(hWnd,rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,FALSE);
	ShowWindow(hWnd,SW_SHOW);
}

void FireMenu_ItemEnsureVisible(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	
}

