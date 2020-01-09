/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	droplist.cc | droplist control implement file

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

#include "domdoc.h"
#include "domview.h"

#include "stringtable.h"
#include "stringview.h"

typedef struct tagDropListlDelta{
	HWND hOwner;
	handle_t canvas;
	LINKPTR data;
	LINKPTR entity;
}DropListDelta;

#define GETDROPLISTDELTA(hWnd) 		((DropListDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETDROPLISTDELTA(hWnd,ptd)	SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK DropListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM	RegisterDropListClass(HINSTANCE hInstance);

void DropList_OnCreate(HWND hWnd,LPCREATESTRUCT lpcs);
void DropList_OnDestroy(HWND hWnd);
void DropList_OnLButtonDown(HWND hWnd,UINT flag,short x,short y);
void DropList_OnLButtonUp(HWND hWnd,UINT flag,short x,short y);
void DropList_OnMouseWheel(HWND hWnd,UINT flag, short delta);
void DropList_OnPaint(HWND hWnd);

void DropList_OnItemChanging(HWND hWnd);
void DropList_OnItemChanged(HWND hWnd,LINKPTR elk);
void DropList_OnDrawBackground(HWND hWnd, HDC hDC);
void DropList_OnDrawForeground(HWND hWnd, HDC hDC);
void DropList_OnResetPage(HWND hWnd);

void DropList_TabSkip(HWND hWnd,int nf);
void DropList_SetCurSel(HWND hWnd,LINKPTR ent);
void DropList_ReDraw(HWND hWnd);
void DropList_ReLayout(HWND hWnd);

void _CalcStringEntityRect(HWND hWnd, LINKPTR plk, RECT* pRect)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	xrect_t xr;

	assert(plk);
	calc_string_entity_rect(ptd->data, plk, &xr);

	pRect->left = mm_to_pt(ptd->canvas, xr.x, 1);
	pRect->right = mm_to_pt(ptd->canvas, xr.x + xr.w, 1);
	pRect->top = mm_to_pt(ptd->canvas, xr.y, 0);
	pRect->bottom = mm_to_pt(ptd->canvas, xr.y + xr.h, 0);
}

ATOM RegisterDropListClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)DropListProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= DROPLISTCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK DropListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	switch(message)
	{
	case WM_CREATE:
		DropList_OnCreate(hWnd,(LPCREATESTRUCT)lParam);
		break;
	case WM_DESTROY:
		DropList_OnDestroy(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if(ptd->data)
			DropList_OnLButtonDown(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if(ptd->data)
			DropList_OnLButtonUp(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		if(ptd->data)
			DropList_OnMouseWheel(hWnd,(UINT)LOWORD(wParam),(short)GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_PAINT:
		if (ptd && ptd->data && GetUpdateRect(hWnd, NULL, TRUE))
		{
			DropList_OnPaint(hWnd);
		}
		break;
	case WM_SETOWNER:
		if (ptd)
		{
			ptd->hOwner = (HWND)lParam;
		}
		return 0;
	case WM_GETOWNER:
		if (ptd)
			return (LRESULT)ptd->hOwner;
		else
			return 0;
	case WM_ATTACHDATA:
		if(ptd)
		{
			ptd->data = (LINKPTR)lParam;
			ptd->entity = NULL;
			DropList_ReLayout(hWnd);
		}
		return 0;
	case WM_DETACHDATA:
		if(ptd)
		{
			LINKPTR ptr = ptd->data;
			ptd->data = NULL;
			ptd->entity = NULL;
			return (LRESULT)ptr;
		}
		return 0;
	case WM_FETCHDATA:
		return (LRESULT)ptd->data;
	case WM_GETCURSEL:
		if (ptd->data)
			return (LRESULT)ptd->entity;
		return 0;
	case WM_SETCURSEL:
		if(ptd->data)
			DropList_SetCurSel(hWnd,(LINKPTR)lParam);
		return 0;
	case WM_TABSKIP:
		if (ptd->data)
			DropList_TabSkip(hWnd, (int)wParam);
		return 0;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);	
}


void DropList_OnCreate(HWND hWnd,LPCREATESTRUCT lpcs)
{
	DropListDelta* ptd;

	ptd = (DropListDelta*)xmem_alloc(sizeof(DropListDelta));
	memset((void*)ptd,0,sizeof(DropListDelta));

	ptd->canvas = create_display_canvas(NULL);

	SETDROPLISTDELTA(hWnd,ptd);
}

void DropList_OnDestroy(HWND hWnd)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	destroy_display_canvas(ptd->canvas);

	SETDROPLISTDELTA(hWnd,0);
	xmem_free(ptd);
}

void DropList_OnSize(HWND hWnd, UINT nType, int width, int height)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	if (nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED)
		DropList_ReDraw(hWnd);
}

void DropList_OnLButtonDown(HWND hWnd,UINT flag,short x,short y)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	xpoint_t xp;
	LINKPTR plk;
	int hint;

	xp.x = pt_to_mm(ptd->canvas, x, 1);
	xp.y = pt_to_mm(ptd->canvas, y, 0);

	hint = calc_string_point_hint(ptd->data, &xp, &plk);

	DropList_SetCurSel(hWnd, plk);
}

void DropList_OnLButtonUp(HWND hWnd,UINT flag,short x,short y)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
}

void DropList_OnMouseWheel(HWND hWnd,UINT nFlags,short delta)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	int nLine, nMax;

	nMax = (int)(calc_string_height(ptd->data) * 10) - (int)(get_string_page_height(ptd->data) * 10);
	if (nMax < 0)
		nMax = 0;

	nLine = (int)(get_string_page_yoff(ptd->data) * 10);

	if (delta < 0)
	{
		if (nLine == nMax)
			return;
		nLine += 100;
		if (nLine > nMax)
			nLine = nMax;
	}
	else
	{
		if (nLine < 0)
			return;
		nLine -= 100;
		if (nLine < 0)
			nLine = 0;
	}

	set_string_page_yoff(ptd->data, (float)nLine / 10);

	InvalidateRect(hWnd, NULL, FALSE);
}

void DropList_OnPaint(HWND hWnd)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rt;

	BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rt);

	hDC = (HDC)begin_display_paint(ptd->canvas, (res_dc_t)ps.hdc, rt.right - rt.left, rt.bottom - rt.top);

	DropList_OnDrawBackground(hWnd, hDC);

	draw_string(ptd->canvas, ptd->data);

	DropList_OnDrawForeground(hWnd, hDC);

	end_display_paint(ptd->canvas, (res_dc_t)ps.hdc, ps.rcPaint.left, ps.rcPaint.top);

	EndPaint(hWnd, &ps);
}

void DropList_OnDrawBackground(HWND hWnd, HDC hDC)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	RECT rt;
	xbrush_t xb = { 0 };

	parse_xbrush_from_style(&xb, DEF_CTRL_BRUSH);

	GetClientRect(hWnd, &rt);

	fill_shape_raw((res_dc_t)hDC, &xb, rt.left, rt.top, rt.right, rt.bottom, ATTR_SHAPE_RECT);
}

void DropList_OnDrawForeground(HWND hWnd, HDC hDC)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	RECT rt;
	xcolor_t xc;

	if (ptd->entity)
	{
		parse_xcolor(&xc, DEF_ALPHA_COLOR);

		_CalcStringEntityRect(hWnd, ptd->entity, &rt);

		draw_alpha_raw((res_dc_t)hDC, &xc, rt.left, rt.top, rt.right, rt.bottom);
	}
}

void DropList_OnResetPage(HWND hWnd)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	RECT rt;
	float pw, ph;
	int nCurLine, nMaxLine;

	GetClientRect(hWnd, &rt);
	pw = pt_to_mm(ptd->canvas, rt.right, 1) - pt_to_mm(ptd->canvas, rt.left, 1);
	set_string_page_width(ptd->data, pw);
	ph = pt_to_mm(ptd->canvas, rt.bottom, 0) - pt_to_mm(ptd->canvas, rt.top, 0);
	set_string_page_height(ptd->data, ph);

	nMaxLine = (int)(calc_string_height(ptd->data) * 10) - (int)(get_string_page_height(ptd->data) * 10);
	if (nMaxLine < 0)
		nMaxLine = 0;

	nCurLine = (int)(get_string_page_yoff(ptd->data) * 10);
	if (nCurLine > nMaxLine)
	{
		nCurLine = nMaxLine;
		set_string_page_yoff(ptd->data, (float)nCurLine / 10);
	}
}

void DropList_OnEntityChanging(HWND hWnd)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	RECT rt;

	assert(ptd->entity);

	_CalcStringEntityRect(hWnd, ptd->entity, &rt);

	ptd->entity = NULL;

	InvalidateRect(hWnd,&rt,FALSE);
}

void DropList_OnEntityChanged(HWND hWnd,LINKPTR elk)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	RECT rt;
	
	assert(!ptd->entity);

	ptd->entity = elk;

	_CalcStringEntityRect(hWnd, ptd->entity, &rt);

	InvalidateRect(hWnd, &rt, FALSE);

	SendMessage(GetParent(hWnd),WM_STRPREVIEW,(WPARAM)-1,(LPARAM)get_string_entity_key_ptr(ptd->entity));
}

void DropList_ReDraw(HWND hWnd)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	assert(ptd->data && ptd->canvas);

	if (ptd->entity && !is_string_entity(ptd->data, ptd->entity))
	{
		ptd->entity = NULL;
	}

	DropList_OnResetPage(hWnd);

	InvalidateRect(hWnd, NULL, FALSE);
}

void DropList_SetCurSel(HWND hWnd, LINKPTR ent)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	BOOL bRe;

	bRe = (ent == ptd->entity) ? TRUE : FALSE;

	if (!bRe && ptd->entity)
		DropList_OnEntityChanging(hWnd);

	if (!bRe && ent)
	{
		if (ensure_string_entity_visible(ptd->data, ent))
		{
			InvalidateRect(hWnd, NULL, FALSE);
		}
		DropList_OnEntityChanged(hWnd, ent);
	}
}

void DropList_ReLayout(HWND hWnd)
{	
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	RECT rt,rtParent;
	int count,cy,cx;

	ShowWindow(hWnd, SW_SHOW);

	count = get_string_entity_count(ptd->data);
	if (count == 0)
	{
		return;
	}

	if (count > 7)
		count = 7;

	cy = mm_to_pt(ptd->canvas, count * get_string_entity_height(ptd->data), 0) - mm_to_pt(ptd->canvas, 0, 0);

	GetClientRect(hWnd, &rt);
	rt.bottom = rt.top + cy;

	AdjustWindowRect(&rt, GetWindowLong(hWnd, GWL_STYLE), FALSE);
	cx = rt.right - rt.left;
	cy = rt.bottom - rt.top;

	GetWindowRect(hWnd,&rt);

	GetWindowRect(GetDesktopWindow(),&rtParent);

	if(rt.top - rtParent.top < rtParent.bottom - rt.bottom)
	{
		rt.top = rt.bottom;
		rt.bottom =rt.top + cy;
	}else
	{
		rt.bottom = rt.top;
		rt.top = rt.bottom - cy;
	}

	MoveWindow(hWnd,rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,FALSE);

	DropList_ReDraw(hWnd);

	ShowWindow(hWnd, SW_SHOW);
}

void DropList_TabSkip(HWND hWnd, int nSkip)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	LINKPTR plk = NULL;

	switch(nSkip)
	{
	case NF_LEFT:
	case NF_DOWN:
		if (ptd->entity)
			plk = get_string_next_entity(ptd->data, ptd->entity);
		else
			plk = get_string_next_entity(ptd->data, LINK_FIRST);
		break;
	case NF_RIGHT:
	case NF_UP:
		if (ptd->entity)
			plk = get_string_prev_entity(ptd->data, ptd->entity);
		else
			plk = get_string_prev_entity(ptd->data, LINK_LAST);
		break;
	case NF_HOME:
		plk = get_string_next_entity(ptd->data, LINK_FIRST);
		break;
	case NF_END:
		plk = get_string_prev_entity(ptd->data, LINK_LAST);
		break;
	}

	DropList_SetCurSel(hWnd, plk);
}

void DropList_FindText(HWND hWnd,const wchar_t* token)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	LINKPTR ent = NULL;

	ent = get_string_next_entity(ptd->data, LINK_FIRST);
	while (ent)
	{
		if(compare_text(token,-1,get_string_entity_key_ptr(ent),-1,1) == 0)
			break;
	}

	DropList_SetCurSel(hWnd, ent);
}

