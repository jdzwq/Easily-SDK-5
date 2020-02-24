/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	xdleditor.cc | editor control implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

/************************************************************
*************************************************************/

#include "xdleditor.h"
#include "hashtable.h"
#include "stringtable.h"
#include "xdlattr.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "domdoc.h"
#include "xdlutil.h"
#include "xdlgdi.h"
#include "xdlcanv.h"
#include "stringview.h"

#include "xdlnoti.h"
#include "windows\xdlwin32.h"

#define LISTBOX_MAXITEM		7

typedef struct tagListBoxlDelta{
	HWND hWnd;
	HWND hOwner;
	LINKPTR data;
	int nIndex;
	int nMetric;
	int nOffset;
}ListBoxDelta;

#define GETLISTBOXDELTA(hWnd) 	((ListBoxDelta*)GetWindowLong(hWnd,GWL_USERDATA))
#define SETLISTBOXDELTA(hWnd,ptd) SetWindowLong(hWnd,GWL_USERDATA,(LONG)ptd)

LRESULT CALLBACK ListBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM RegisterListBoxClass(HINSTANCE hInstance);

void ListBox_OnLButtonDown(HWND hWnd,UINT flag,short x,short y);
void ListBox_OnLButtonUp(HWND hWnd,UINT flag,short x,short y);
void ListBox_OnHScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll);
void ListBox_OnVScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll);
void ListBox_OnSize(HWND hWnd,UINT nType, int cx, int cy);
void ListBox_OnPaint(HWND hWnd,HDC hDC);
void ListBox_OnKeyDown(HWND hWnd,UINT nChar);

void ListBox_ResetScroll(HWND hWnd);
void ListBox_EnsureListVisible(HWND hWnd,int index);
void ListBox_ItemKillFocus(HWND hWnd);
void ListBox_ItemSetFocus(HWND hWnd,int index);
void ListBox_SetFocus(HWND hWnd);
void ListBox_TabSkip(HWND hWnd,int nf);
void ListBox_Layout(HWND hWnd);

void CalcListBoxItemRect(HWND hWnd,int index,RECT* pRect)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);

	GetClientRect(hWnd,pRect);
	pRect->top = (index - ptd->nOffset) * ptd->nMetric;
	pRect->bottom  = pRect->top + ptd->nMetric; 
}

int ListBoxPointHot(HWND hWnd,short xp,short yp)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	RECT rt;
	POINT pt;
	int i,count;

	pt.x = xp;
	pt.y = yp;

	GetClientRect(hWnd,&rt);
	rt.top = - ptd->nMetric * ptd->nOffset;
	rt.bottom = rt.top;

	count = get_hash_entity_count(ptd->data);
	for(i=0;i<count;i++)
	{		
		rt.top = rt.bottom;
		rt.bottom = rt.top + ptd->nMetric;

		if(PtInRect(&rt,pt))
		{
			return i;
		}
	}

	return -1;
}

ATOM RegisterListBoxClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)ListBoxProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= LISTCTRLCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK ListBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	PAINTSTRUCT ps;
	LPCREATESTRUCT lpcs;
	LINKPTR plk;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (ListBoxDelta*)xmem_alloc(sizeof(ListBoxDelta));
		memset((void*)ptd,0,sizeof(ListBoxDelta));
		SETLISTBOXDELTA(hWnd,ptd);

		ptd->hWnd = hWnd;
		ptd->hOwner = (HWND)lpcs->lpCreateParams;
		break;
	case WM_DESTROY:
		SETLISTBOXDELTA(hWnd,0);
		xmem_free(ptd);
		break;
	case WM_ATTACHDATA:
		assert(ptd->data == NULL);
		ptd->data = (LINKPTR)lParam;
		ptd->nIndex = -1;
		ListBox_Layout(hWnd);
		return 0;
	case WM_DETACHDATA:
		plk = ptd->data;
		ptd->data = NULL;
		ptd->nIndex = -1;
		return (LRESULT)plk;
	case WM_GETCURSEL:
		if(ptd->nIndex < 0)
			return 0;
		else
			return (LRESULT)get_hash_entity_at(ptd->data,ptd->nIndex);
	case WM_SETCURSEL:
		if(ptd->nIndex >= 0)
			ListBox_ItemKillFocus(hWnd);
		ptd->nIndex = get_hash_entity_index(ptd->data,(LINKPTR)lParam);
		if(ptd->nIndex >= 0)
			ListBox_ItemSetFocus(hWnd,ptd->nIndex);
		return 0;
	case WM_TABSKIP:
		if(ptd->data)
			ListBox_TabSkip(hWnd,(int)wParam);
		return 0;
	//case WM_KEYDOWN:
	//	if(ptd->data)
	//		ListBox_OnKeyDown(hWnd,(UINT)wParam);
	//	break;
	case WM_LBUTTONDOWN:
		if(ptd->data)
			ListBox_OnLButtonDown(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if(ptd->data)
			ListBox_OnLButtonUp(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_SIZE:
		if(ptd->data)
			ListBox_OnSize(hWnd,(UINT)wParam,(int)LOWORD(lParam),(int)HIWORD(lParam));
		break;
	case WM_VSCROLL:
		if(ptd->data)
			ListBox_OnVScroll(hWnd,(UINT)LOWORD(wParam),(short)HIWORD(wParam),(HWND)lParam);
		break;
	case WM_HSCROLL:
		if(ptd->data)
			ListBox_OnHScroll(hWnd,(UINT)LOWORD(wParam),(short)HIWORD(wParam),(HWND)lParam);
		break;
	case WM_PAINT:
		if(ptd && GetUpdateRect(hWnd,NULL,TRUE))
		{
			BeginPaint(hWnd,&ps);
			ListBox_OnPaint(hWnd,ps.hdc);		
			EndPaint(hWnd,&ps);
		}
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);	
}

void ListBox_ItemKillFocus(HWND hWnd)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	RECT rt;

	CalcListBoxItemRect(hWnd,ptd->nIndex,&rt);
	ptd->nIndex = -1;
	InvalidateRect(hWnd,&rt,FALSE);
}

void ListBox_ItemSetFocus(HWND hWnd,int index)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	RECT rt;

	ptd->nIndex = index;
	CalcListBoxItemRect(hWnd,ptd->nIndex,&rt);
	InvalidateRect(hWnd,&rt,FALSE);
}

void ListBox_SetFocus(HWND hWnd)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);

}

void ListBox_OnLButtonDown(HWND hWnd,UINT flag,short x,short y)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	int index;

	SetCapture(hWnd);

	index = ListBoxPointHot(hWnd,x,y);

	if(index != ptd->nIndex && ptd->nIndex >= 0)
		ListBox_ItemKillFocus(hWnd);

	if(index >= 0)
		ListBox_ItemSetFocus(hWnd,index);
	else
		ListBox_SetFocus(hWnd);
}

void ListBox_OnLButtonUp(HWND hWnd,UINT flag,short x,short y)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	
	ReleaseCapture();

	if(ptd->nIndex >= 0)
		PostMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(EC_COMMIT,0),(LPARAM)hWnd);
	else
		PostMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(EC_ROLLBACK,0),(LPARAM)hWnd);
}

void ListBox_TabSkip(HWND hWnd, int nSkip)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	int index = -1;

	if(ptd->nIndex >= 0)
	{
		index = ptd->nIndex;
		ListBox_ItemKillFocus(hWnd);
	}

	switch(nSkip)
	{
	case NF_LEFT:
	case NF_DOWN:
		index ++;
		break;
	case NF_RIGHT:
	case NF_UP:
		if(index < 0)
			index = get_hash_entity_count(ptd->data);
		index --;
		break;
	case NF_HOME:
		index = 0;
		break;
	case NF_END:
		index = get_hash_entity_count(ptd->data) - 1;
		break;
	}

	if(index < 0 || index >= get_hash_entity_count(ptd->data))
		index = -1;

	if(index != -1)
	{
		ListBox_EnsureListVisible(hWnd,index);
		ListBox_ItemSetFocus(hWnd,index);
	}else
	{
		ListBox_SetFocus(hWnd);
	}
}

void ListBox_OnKeyDown(HWND hWnd, UINT nChar)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);

	switch(nChar)
	{
	case VK_DOWN:
		PostMessage(hWnd,WM_TABSKIP,(WPARAM)NF_DOWN,0);
		break;
	case VK_UP:
		PostMessage(hWnd,WM_TABSKIP,(WPARAM)NF_UP,0);		
		break;
	case VK_HOME:
		PostMessage(hWnd,WM_TABSKIP,(WPARAM)NF_HOME,0);
		break;
	case VK_END:
		PostMessage(hWnd,WM_TABSKIP,(WPARAM)NF_END,0);
		break;
	case 13:
		PostMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(EC_COMMIT,0),(LPARAM)hWnd);
		break;
	}
}

void ListBox_OnSize(HWND hWnd,UINT type,int width,int height)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	
	ListBox_ResetScroll(hWnd);
	InvalidateRect(hWnd,NULL,FALSE);
}

void ListBox_OnVScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll) 
{
	// TODO: Add your message handler code here and/or call default
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	SCROLLINFO si;
	int nCur,nHeight;

	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo(hWnd,SB_VERT,&si);

	nHeight = get_hash_entity_count(ptd->data) - LISTBOX_MAXITEM;
	if(nHeight < 0)
		nHeight = 0;
	
	switch(nSBCode)
	{
	case SB_LINEUP:
		if(si.nPos == si.nMin)
			break;
		nCur = si.nPos - 1;
		if(nCur < 0)
			nCur = 0;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nHeight;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
		break;
	case SB_PAGEUP:
		if(si.nPos == si.nMin)
			break;
		nCur = si.nPos - 1;
		if(nCur < 0)
			nCur = 0;		
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nHeight;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
		break;
	case SB_LINEDOWN:
		if(si.nPos == si.nMax)
			break;
		nCur = si.nPos + 1;
		if(nCur > nHeight)
			nCur = nHeight;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nHeight;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
		break;
	case SB_PAGEDOWN:
		if(si.nPos == si.nMax)
			break;
		nCur = si.nPos + 1;
		if(nCur > nHeight)
			nCur = nHeight;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nHeight;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
		break;
	case SB_THUMBPOSITION:
		nCur = nPos;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nHeight;
		si.nMin = 0;
		si.nPos = nCur;
 		SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
		break;
	case SB_ENDSCROLL:
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd,SB_VERT,&si);
		ptd->nOffset = si.nPos;
		InvalidateRect(hWnd,NULL,FALSE);
		break;
	}
}

void ListBox_OnHScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);

}

void ListBox_OnPaint(HWND hWnd,HDC hDC)
{
	ListBoxDelta* ptd = GETLISTBOXDELTA(hWnd);
	
	LINKPTR plk;
	const wchar_t* token;
	int index;
	RECT rt;
	hash_enum_t he = {0};
	
	xfont_t xf = {0};
	xface_t xa = {0};
	xbrush_t xb = {0};
	xpen_t xp = {0};
	xbrush_t xb_focus = {0};
	xpen_t xp_focus = {0};
	
	parse_xfont_from_style(&xf,DEF_CTRL_FONT);
	parse_xface_from_style(&xa,DEF_CTRL_FACE);
	parse_xbrush_from_style(&xb,DEF_CTRL_BRUSH);
	parse_xpen_from_style(&xp,DEF_CTRL_PEN);
	//parse_xbrush_from_style(&xb_focus,DEF_FOCUS_BRUSH);
	//parse_xpen_from_style(&xp_focus,DEF_FOCUS_PEN);

	GetClientRect(hWnd,&rt);
	draw_rect_raw((res_dc_t)hDC,&xp,rt.left,rt.top,rt.right,rt.bottom);

	rt.bottom = rt.top;

	index = 0;
	plk = get_hash_first_entity(ptd->data,&he);
	while(plk)
	{
		rt.top = rt.bottom;
		rt.bottom = rt.top + ptd->nMetric;
		
		token = get_hash_entity_val_ptr(plk);
		if(!is_null(token))
			draw_text_raw((res_dc_t)hDC,&xf,&xa,rt.left,rt.top,rt.right,rt.bottom,token,-1);

		if(index++ == ptd->nIndex)
		{
			draw_rect_raw((res_dc_t)hDC,&xp_focus,rt.left,rt.top,rt.right,rt.bottom);
		}

		plk = get_hash_next_entity(ptd->data,&he);
	}
}

void ListBox_ResetScroll(HWND hWnd)
{
	ListBoxDelta* ptd =GETLISTBOXDELTA(hWnd);
	int nHeight;
	SCROLLINFO si;

	nHeight = get_hash_entity_count(ptd->data) - LISTBOX_MAXITEM;
	if(nHeight < 0)
	{
		nHeight = 0;
	}

	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	GetScrollInfo(hWnd,SB_VERT,&si);

	if(si.nPos > nHeight)
		si.nPos = nHeight;
	si.fMask = SIF_RANGE | SIF_POS;
	si.nMax = nHeight;
	si.nMin = 0;
	SetScrollInfo(hWnd,SB_VERT,&si,TRUE);	
}

void ListBox_EnsureListVisible(HWND hWnd,int index)
{
	ListBoxDelta* ptd =GETLISTBOXDELTA(hWnd);
	RECT rt,rtCell;
	int nVertPos;
	SCROLLINFO si;

	GetClientRect(hWnd,&rt);
	CalcListBoxItemRect(hWnd,index,&rtCell);

	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;	
	GetScrollInfo(hWnd,SB_VERT,&si);
	nVertPos = si.nPos;

	if(rtCell.top < rt.top )
	{
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,nVertPos + rtCell.top - rt.top ),(LPARAM)0);
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_ENDSCROLL,0),(LPARAM)0);
	}
	if(rtCell.bottom > rt.bottom)
	{
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,nVertPos + rtCell.bottom - rt.bottom),(LPARAM)0);
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_ENDSCROLL,0),(LPARAM)0);
	}
}

void ListBox_Layout(HWND hWnd)
{
	ListBoxDelta* ptd =GETLISTBOXDELTA(hWnd);
	RECT rt,rtParent;
	int count,cy,cx;
	TEXTMETRIC tm;
	int avg_width,max_width = 0;
	hash_enum_t he = {0};
	HDC hDC;
	LINKPTR plk;

	GetWindowRect(ptd->hOwner,&rt);
	cx = rt.right - rt.left;
	cy = rt.bottom - rt.top;

	hDC = GetDC(hWnd);
	GetTextMetrics(hDC,&tm);
	ptd->nMetric = tm.tmHeight + tm.tmAscent;// + tm.tmDescent;
	avg_width = tm.tmAveCharWidth;
	ReleaseDC(hWnd,hDC);

	max_width = cx;
	count = 0;
	plk = get_hash_first_entity(ptd->data,&he);
	while(plk)
	{
		avg_width = xslen(get_hash_entity_val_ptr(plk)) * tm.tmAveCharWidth;
		max_width = (max_width > avg_width)? max_width : avg_width;
		count ++;
		plk = get_hash_next_entity(ptd->data,&he);
	}

	count = (count < LISTBOX_MAXITEM)? count : LISTBOX_MAXITEM;

	rt.left = rt.top = 0;
	rt.right = max_width;
	rt.bottom = count * ptd->nMetric;
	AdjustWindowRect(&rt,GetWindowLong(hWnd,GWL_STYLE),0);

	ClientToScreen(hWnd,(LPPOINT)&rt);
	ClientToScreen(hWnd,(LPPOINT)&rt + 1);

	/*GetWindowRect(GetDesktopWindow(),&rtParent);
	if(rt.top - rtParent.top < rtParent.bottom - rt.bottom)
	{
		rt.top = rt.bottom;
		rt.bottom = rt.top + cy;
		if(rt.bottom > rtParent.bottom)
			rt.bottom = rtParent.bottom;
	}else
	{
		rt.bottom = rt.top;
		rt.top = rt.bottom - cy;
		if(rt.top < rtParent.top)
			rt.top = rtParent.top;
	}*/

	MoveWindow(hWnd,rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,FALSE);
	ShowWindow(hWnd,SW_SHOW);
}

/*******************************************************************************************************/

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

void DropList_OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);
void DropList_OnDestroy(HWND hWnd);
void DropList_OnLButtonDown(HWND hWnd, UINT flag, short x, short y);
void DropList_OnLButtonUp(HWND hWnd, UINT flag, short x, short y);
void DropList_OnMouseWheel(HWND hWnd, UINT flag, short delta);
void DropList_OnPaint(HWND hWnd);

void DropList_OnItemChanging(HWND hWnd);
void DropList_OnItemChanged(HWND hWnd, LINKPTR elk);
void DropList_OnDrawBackground(HWND hWnd, HDC hDC);
void DropList_OnDrawForeground(HWND hWnd, HDC hDC);
void DropList_OnResetPage(HWND hWnd);

void DropList_TabSkip(HWND hWnd, int nf);
void DropList_SetCurSel(HWND hWnd, LINKPTR ent);
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

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)DropListProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = DROPLISTCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK DropListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	switch (message)
	{
	case WM_CREATE:
		DropList_OnCreate(hWnd, (LPCREATESTRUCT)lParam);
		break;
	case WM_DESTROY:
		DropList_OnDestroy(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if (ptd->data)
			DropList_OnLButtonDown(hWnd, wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if (ptd->data)
			DropList_OnLButtonUp(hWnd, wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		if (ptd->data)
			DropList_OnMouseWheel(hWnd, (UINT)LOWORD(wParam), (short)GET_WHEEL_DELTA_WPARAM(wParam));
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
		if (ptd)
		{
			ptd->data = (LINKPTR)lParam;
			ptd->entity = NULL;
			DropList_ReLayout(hWnd);
		}
		return 0;
	case WM_DETACHDATA:
		if (ptd)
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
		if (ptd->data)
			DropList_SetCurSel(hWnd, (LINKPTR)lParam);
		return 0;
	case WM_TABSKIP:
		if (ptd->data)
			DropList_TabSkip(hWnd, (int)wParam);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


void DropList_OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	DropListDelta* ptd;

	ptd = (DropListDelta*)xmem_alloc(sizeof(DropListDelta));
	memset((void*)ptd, 0, sizeof(DropListDelta));

	ptd->canvas = create_display_canvas(NULL);

	SETDROPLISTDELTA(hWnd, ptd);
}

void DropList_OnDestroy(HWND hWnd)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	destroy_display_canvas(ptd->canvas);

	SETDROPLISTDELTA(hWnd, 0);
	xmem_free(ptd);
}

void DropList_OnSize(HWND hWnd, UINT nType, int width, int height)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	if (nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED)
		DropList_ReDraw(hWnd);
}

void DropList_OnLButtonDown(HWND hWnd, UINT flag, short x, short y)
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

void DropList_OnLButtonUp(HWND hWnd, UINT flag, short x, short y)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);

	SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
}

void DropList_OnMouseWheel(HWND hWnd, UINT nFlags, short delta)
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

	InvalidateRect(hWnd, &rt, FALSE);
}

void DropList_OnEntityChanged(HWND hWnd, LINKPTR elk)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	RECT rt;

	assert(!ptd->entity);

	ptd->entity = elk;

	_CalcStringEntityRect(hWnd, ptd->entity, &rt);

	InvalidateRect(hWnd, &rt, FALSE);

	SendMessage(GetParent(hWnd), WM_STRPREVIEW, (WPARAM)-1, (LPARAM)get_string_entity_key_ptr(ptd->entity));
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
	RECT rt, rtParent;
	int count, cy, cx;

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

	GetWindowRect(hWnd, &rt);

	GetWindowRect(GetDesktopWindow(), &rtParent);

	if (rt.top - rtParent.top < rtParent.bottom - rt.bottom)
	{
		rt.top = rt.bottom;
		rt.bottom = rt.top + cy;
	}
	else
	{
		rt.bottom = rt.top;
		rt.top = rt.bottom - cy;
	}

	MoveWindow(hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, FALSE);

	DropList_ReDraw(hWnd);

	ShowWindow(hWnd, SW_SHOW);
}

void DropList_TabSkip(HWND hWnd, int nSkip)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	LINKPTR plk = NULL;

	switch (nSkip)
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

void DropList_FindText(HWND hWnd, const wchar_t* token)
{
	DropListDelta* ptd = GETDROPLISTDELTA(hWnd);
	LINKPTR ent = NULL;

	ent = get_string_next_entity(ptd->data, LINK_FIRST);
	while (ent)
	{
		if (compare_text(token, -1, get_string_entity_key_ptr(ent), -1, 1) == 0)
			break;
	}

	DropList_SetCurSel(hWnd, ent);
}

/******************************************************************************************/

typedef struct tagFireDateDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
}FireDateDelta;

#define GETFIREDATEDELTA(hWnd) 	((FireDateDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREDATEDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireDateProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int DE_GetEditorText(HWND hWnd, wchar_t* buf, int max);
void DE_SetEditorText(HWND hWnd, const wchar_t* text);
int DE_EditorTextSize(HWND hWnd);
void DE_GetDateTime(HWND hWnd, wchar_t* buf);
void DE_SetDateTime(HWND hWnd, const wchar_t* szDate);

ATOM RegisterFireDateClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)FireDateProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);/*(COLOR_WINDOW+1);*/
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = FIREDATECLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubDateProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_KEYDOWN:
		if (SendMessage(GetParent(hWnd), WM_KEYPREVIEW, wParam, lParam))
			return 0;
		break;
	default:
		break;
	}

	if (lpfOrgProc)
		return CallWindowProc(lpfOrgProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK FireDateProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	LPNMHDR lphdr;
	RECT rt;

	switch (message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireDateDelta*)xmem_alloc(sizeof(FireDateDelta));
		memset((void*)ptd, 0, sizeof(FireDateDelta));

		GetClientRect(hWnd, &rt);

		ptd->hEdit = CreateWindowEx(0, DATETIMEPICK_CLASS, NULL, lpcs->style | DTS_TIMEFORMAT | WS_DISABLED, lpcs->x, lpcs->y, lpcs->cx, lpcs->cy, hWnd, NULL, lpcs->hInstance, NULL);
		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)SubDateProc);
		SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit, TRUE);
		SetFocus(ptd->hEdit);

		SETFIREDATEDELTA(hWnd, ptd);
		break;
	case WM_DESTROY:
		SETFIREDATEDELTA(hWnd, 0);

		if (IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit, FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if (ptd->hFont)
			DeleteObject(ptd->hFont);

		xmem_free(ptd);
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
	case WM_NOTIFY:
		if (ptd)
		{
			lphdr = (LPNMHDR)lParam;
			if (lphdr->hwndFrom == ptd->hEdit && lphdr->code == DTN_CLOSEUP)
			{
				if (IsWindow(ptd->hOwner))
					SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
				else
					SetFocus(ptd->hEdit);
			}
			else if (lphdr->hwndFrom == ptd->hEdit && lphdr->code == NM_KILLFOCUS)
			{
				if (IsWindow(ptd->hOwner))
					SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
				else
					SetFocus(ptd->hEdit);
			}
		}
		break;
	case WM_SETFIREFONT:
		if (ptd->hFont)
			DeleteObject(ptd->hFont);
		ptd->hFont = (HFONT)XdcCreateFont((const xfont_t*)lParam);
		SendMessage(ptd->hEdit, WM_SETFONT, (WPARAM)ptd->hFont, MAKELPARAM(1, 0));
		return 0;
	case WM_RESETEDITOR:
		SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(wParam, 0), (LPARAM)hWnd);
		return 0;
	case WM_GETEDITORTEXT:
		return DE_GetEditorText(hWnd, (wchar_t*)lParam, (int)wParam);
	case WM_SETEDITORTEXT:
		DE_SetEditorText(hWnd, (const wchar_t*)lParam);
		return 0;
	case WM_EDITORTEXTSIZE:
		return DE_EditorTextSize(hWnd);
	case WM_KEYPREVIEW:
		if (wParam == 13 || wParam == 9) /*enter key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
			return 1;
		}
		else if (wParam == 27) /*escape key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_ROLLBACK, 0), (LPARAM)hWnd);
			return 1;
		}
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int DE_GetEditorText(HWND hWnd, wchar_t* buf, int max)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(hWnd);
	int len;
	wchar_t szDate[DATE_LEN + 1];

	assert(buf);
	DE_GetDateTime(hWnd, szDate);
	len = xslen(szDate);
	len = (len < max) ? len : max;
	xsncpy(buf, szDate, len);
	buf[len] = L'\0';
	return len;
}

void DE_SetEditorText(HWND hWnd, const wchar_t* text)
{
	DE_SetDateTime(hWnd, text);
}

int DE_EditorTextSize(HWND hWnd)
{
	FireDateDelta* ptd = GETFIREDATEDELTA(hWnd);
	return DATE_LEN;
}

void DE_GetDateTime(HWND hWnd, wchar_t* buf)
{
	FireDateDelta* ptd = NULL;
	ptd = GETFIREDATEDELTA(hWnd);

	SYSTEMTIME st;

	assert(buf);

	SendMessage(ptd->hEdit, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
	xsprintf(buf, L"%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void DE_SetDateTime(HWND hWnd, const wchar_t* szDate)
{
	FireDateDelta* ptd = NULL;

	ptd = GETFIREDATEDELTA(hWnd);

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

/******************************************************************************************************************/

typedef struct tagFireDropDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
	wchar_t* szText;
	HWND hList;
}FireDropDelta;

#define GETFIREDROPDELTA(hWnd) 	((FireDropDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREDROPDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireDropProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DropEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void	FireDrop_AttachData(HWND hWnd, LINKPTR ptr);
LINKPTR	FireDrop_DetachData(HWND hWnd);

int		FireDrop_GetEditorText(HWND hWnd, wchar_t* buf, int max);
void	FireDrop_SetEditorText(HWND hWnd, const wchar_t* text);
int		FireDrop_EditorTextSize(HWND hWnd);

ATOM RegisterFireDropClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)FireDropProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = FIREDROPCLASS;

	return RegisterClass(&wcex);
}


LRESULT CALLBACK DropEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_MOUSEWHEEL:
		if (SendMessage(GetParent(hWnd), WM_MOUSEWHEEL, wParam, lParam))
			return 0;
		break;
	case WM_KEYDOWN:
		if (SendMessage(GetParent(hWnd), WM_KEYPREVIEW, wParam, lParam))
			return 0;
		break;
	default:
		break;
	}

	if (lpfOrgProc)
		return CallWindowProc(lpfOrgProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK FireDropProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	RECT rt;

	switch (message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireDropDelta*)xmem_alloc(sizeof(FireDropDelta));
		memset((void*)ptd, 0, sizeof(FireDropDelta));
		SETFIREDROPDELTA(hWnd, ptd);

		GetClientRect(hWnd, &rt);
		ptd->hEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)DropEditProc);
		SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit, TRUE);
		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIREDROPDELTA(hWnd, 0);

		if (IsWindow(ptd->hList))
		{
			DestroyWindow(ptd->hList);
		}

		if (IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit, FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if (ptd->hFont)
			DeleteObject(ptd->hFont);

		if (ptd->szText)
			xmem_free(ptd->szText);

		xmem_free(ptd);
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
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_UPDATE && (HWND)lParam == ptd->hEdit)
		{
			int len = GetWindowTextLength(ptd->hEdit);
			wchar_t *token = (wchar_t*)xmem_alloc((len + 1) * sizeof(wchar_t));
			GetWindowText(ptd->hEdit, token, len + 1);

			SendMessage(ptd->hList, WM_FINDTEXT, (WPARAM)len, (LPARAM)token);

			xmem_free(token);
		}
		else if (HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit && GetFocus() != ptd->hList && IsWindow(ptd->hList))
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
		}
		break;
	case WM_RESETEDITOR:
		SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(wParam, 0), (LPARAM)hWnd);
		return 0;
	case WM_GETEDITORTEXT:
		return FireDrop_GetEditorText(hWnd, (wchar_t*)lParam, (int)wParam);
	case WM_SETEDITORTEXT:
		FireDrop_SetEditorText(hWnd, (const wchar_t*)lParam);
		return 0;
	case WM_EDITORTEXTSIZE:
		return FireDrop_EditorTextSize(hWnd);
	case WM_ATTACHDATA:
		FireDrop_AttachData(hWnd, (LINKPTR)lParam);
		return 0;
	case WM_DETACHDATA:
		return (LRESULT)FireDrop_DetachData(hWnd);
	case WM_SETFIREFONT:
		if (ptd->hFont)
			DeleteObject(ptd->hFont);
		ptd->hFont = (HFONT)XdcCreateFont((const xfont_t*)lParam);
		SendMessage(ptd->hEdit, WM_SETFONT, (WPARAM)ptd->hFont, MAKELPARAM(1, 0));
		return 0;
	case WM_MOUSEWHEEL:
		if (ptd->hList)
		{
			PostMessage(ptd->hList, WM_MOUSEWHEEL, wParam, lParam);
			return 1;
		}
		return 0;
	case WM_KEYPREVIEW:
		if (wParam == VK_DOWN)
		{
			PostMessage(ptd->hList, WM_TABSKIP, (WPARAM)NF_DOWN, 0);
			return 1;
		}
		else if (wParam == VK_UP)
		{
			PostMessage(ptd->hList, WM_TABSKIP, (WPARAM)NF_UP, 0);
			return 1;
		}
		else if (wParam == 13) /*enter key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
			return 1;
		}
		else if (wParam == 27) /*escape key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_ROLLBACK, 0), (LPARAM)hWnd);
			return 1;
		}
		return 0;
	case WM_STRPREVIEW:
		SetWindowText(ptd->hEdit, (wchar_t*)lParam);
		SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


void FireDrop_AttachData(HWND hWnd, LINKPTR ptr)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	RECT rt;

	GetWindowRect(hWnd, &rt);

	ptd->hList = CreateWindow(DROPLISTCLASS, NULL, WS_POPUP, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);
	SendMessage(ptd->hList, WM_ATTACHDATA, 0, (LPARAM)ptr);

	SetFocus(ptd->hEdit);
}

LINKPTR FireDrop_DetachData(HWND hWnd)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);
	LINKPTR ptr;

	ptr = (LINKPTR)SendMessage(ptd->hList, WM_DETACHDATA, 0, 0);

	if (IsWindow(ptd->hList))
	{
		DestroyWindow(ptd->hList);
		ptd->hList = NULL;
	}

	return ptr;
}

int FireDrop_GetEditorText(HWND hWnd, wchar_t* buf, int max)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);

	return GetWindowText(ptd->hEdit, buf, max + 1);
}

void FireDrop_SetEditorText(HWND hWnd, const wchar_t* text)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);

	SetWindowText(ptd->hEdit, (LPCTSTR)text);
	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
}

int FireDrop_EditorTextSize(HWND hWnd)
{
	FireDropDelta* ptd = GETFIREDROPDELTA(hWnd);

	return GetWindowTextLength(ptd->hEdit);
}

/*******************************************************************************************************/

typedef struct tagFireEditDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
}FireEditDelta;

#define GETFIREEDITDELTA(hWnd) 	((FireEditDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREEDITDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SubEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int		FireEdit_GetEditorText(HWND hWnd, wchar_t* buf, int max);
void	FireEdit_SetEditorText(HWND hWnd, const wchar_t* text);
int		FireEdit_EditorTextSize(HWND hWnd);

ATOM RegisterFireEditClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)FireEditProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = FIREEDITCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_KEYDOWN:
		if (SendMessage(GetParent(hWnd), WM_KEYPREVIEW, wParam, lParam))
			return 0;
		break;
	default:
		break;
	}

	if (lpfOrgProc)
		return CallWindowProc(lpfOrgProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK FireEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireEditDelta* ptd = GETFIREEDITDELTA(hWnd);;
	LPCREATESTRUCT lpcs;
	RECT rt;
	WNDPROC lpfOrgProc;

	switch (message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireEditDelta*)xmem_alloc(sizeof(FireEditDelta));
		memset((void*)ptd, 0, sizeof(FireEditDelta));
		SETFIREEDITDELTA(hWnd, ptd);

		GetClientRect(hWnd, &rt);
		ptd->hEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)SubEditProc);
		SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit, TRUE);
		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIREEDITDELTA(hWnd, 0);

		if (IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit, FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if (ptd->hFont)
			DeleteObject(ptd->hFont);

		xmem_free(ptd);
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
	case WM_COMMAND:
		if (ptd)
		{
			if (HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit)
			{
				SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
			}
		}
		break;
	case WM_SETFIREFONT:
		if (ptd->hFont)
			DeleteObject(ptd->hFont);
		ptd->hFont = (HFONT)XdcCreateFont((const xfont_t*)lParam);
		SendMessage(ptd->hEdit, WM_SETFONT, (WPARAM)ptd->hFont, MAKELPARAM(1, 0));
		return 0;
	case WM_RESETEDITOR:
		SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(wParam, 0), (LPARAM)hWnd);
		return 0;
	case WM_GETEDITORTEXT:
		return FireEdit_GetEditorText(hWnd, (wchar_t*)lParam, (int)wParam);
	case WM_SETEDITORTEXT:
		FireEdit_SetEditorText(hWnd, (const wchar_t*)lParam);
		return 0;
	case WM_EDITORTEXTSIZE:
		return FireEdit_EditorTextSize(hWnd);
	case WM_KEYPREVIEW:
		if (wParam == 13 || wParam == 9) /*enter key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
			return 1;
		}
		else if (wParam == 27) /*escape key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_ROLLBACK, 0), (LPARAM)hWnd);
			return 1;
		}
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int FireEdit_GetEditorText(HWND hWnd, wchar_t* buf, int max)
{
	FireEditDelta* ptd = GETFIREEDITDELTA(hWnd);

	return GetWindowText(ptd->hEdit, buf, max + 1);
}

void FireEdit_SetEditorText(HWND hWnd, const wchar_t* text)
{
	FireEditDelta* ptd = GETFIREEDITDELTA(hWnd);

	SetWindowText(ptd->hEdit, (LPCTSTR)text);

	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
}

int FireEdit_EditorTextSize(HWND hWnd)
{
	FireEditDelta* ptd = GETFIREEDITDELTA(hWnd);

	return GetWindowTextLength(ptd->hEdit);
}

/************************************************************************************************/

typedef struct tagFireListDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
	HWND hList;
}FireListDelta;

#define GETFIRELISTDELTA(hWnd) 	((FireListDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIRELISTDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void	FireList_AttachData(HWND hWnd, LINKPTR ptr);
LINKPTR	FireList_DetachData(HWND hWnd);

int		FireList_GetEditorText(HWND hWnd, wchar_t* buf, int max);
void	FireList_SetEditorText(HWND hWnd, const wchar_t* text);
int		FireList_EditorTextSize(HWND hWnd);

ATOM RegisterFireListClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)FireListProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = FIRELISTCLASS;

	return RegisterClass(&wcex);
}


LRESULT CALLBACK ListEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_MOUSEWHEEL:
		if (SendMessage(GetParent(hWnd), WM_MOUSEWHEEL, wParam, lParam))
			return 0;
		break;
	case WM_KEYDOWN:
		if (SendMessage(GetParent(hWnd), WM_KEYPREVIEW, wParam, lParam))
			return 0;
		break;
	default:
		break;
	}

	if (lpfOrgProc)
		return CallWindowProc(lpfOrgProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK FireListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	RECT rt;

	switch (message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireListDelta*)xmem_alloc(sizeof(FireListDelta));
		memset((void*)ptd, 0, sizeof(FireListDelta));
		SETFIRELISTDELTA(hWnd, ptd);

		GetClientRect(hWnd, &rt);
		ptd->hEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)ListEditProc);
		SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit, TRUE);
		break;
	case WM_DESTROY:
		SETFIRELISTDELTA(hWnd, 0);

		if (IsWindow(ptd->hList))
		{
			DestroyWindow(ptd->hList);
		}

		if (IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit, FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if (ptd->hFont)
			DeleteObject(ptd->hFont);

		xmem_free(ptd);
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
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_UPDATE && (HWND)lParam == ptd->hEdit)
		{
			int len = GetWindowTextLength(ptd->hEdit);
			wchar_t *token = (wchar_t*)xmem_alloc((len + 1) * sizeof(wchar_t));
			GetWindowText(ptd->hEdit, token, len + 1);

			SendMessage(ptd->hList, WM_FINDTEXT, (WPARAM)len, (LPARAM)token);

			xmem_free(token);
		}
		else if (HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit && GetFocus() != ptd->hList && IsWindow(ptd->hList))
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
		}
		break;
	case WM_RESETEDITOR:
		SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(wParam, 0), (LPARAM)hWnd);
		return 0;
	case WM_GETEDITORTEXT:
		return FireList_GetEditorText(hWnd, (wchar_t*)lParam, (int)wParam);
	case WM_SETEDITORTEXT:
		FireList_SetEditorText(hWnd, (const wchar_t*)lParam);
		return 0;
	case WM_EDITORTEXTSIZE:
		return FireList_EditorTextSize(hWnd);
	case WM_ATTACHDATA:
		FireList_AttachData(hWnd, (LINKPTR)lParam);
		return 0;
	case WM_DETACHDATA:
		return (LRESULT)FireList_DetachData(hWnd);
	case WM_SETFIREFONT:
		if (ptd->hFont)
			DeleteObject(ptd->hFont);
		ptd->hFont = (HFONT)XdcCreateFont((const xfont_t*)lParam);
		SendMessage(ptd->hEdit, WM_SETFONT, (WPARAM)ptd->hFont, MAKELPARAM(1, 0));
		return 0;
	case WM_MOUSEWHEEL:
		if (ptd->hList)
		{
			PostMessage(ptd->hList, WM_MOUSEWHEEL, wParam, lParam);
			return 1;
		}
		return 0;
	case WM_KEYPREVIEW:
		if (wParam == VK_DOWN)
		{
			PostMessage(ptd->hList, WM_TABSKIP, (WPARAM)NF_DOWN, 0);
			return 1;
		}
		else if (wParam == VK_UP)
		{
			PostMessage(ptd->hList, WM_TABSKIP, (WPARAM)NF_UP, 0);
			return 1;
		}
		else if (wParam == 13) /*enter key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
			return 1;
		}
		else if (wParam == 27) /*escape key*/
		{
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_ROLLBACK, 0), (LPARAM)hWnd);
			return 1;
		}
		else if (wParam == 32) /*blank key*/
		{
			SendMessage(ptd->hList, WM_SETCURSEL, 0, 0);
			SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_CLEAN, 0), (LPARAM)hWnd);
			return 1;
		}
		return 0;
	case WM_STRPREVIEW:
		SetWindowText(ptd->hEdit, (wchar_t*)lParam);
		SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void FireList_AttachData(HWND hWnd, LINKPTR ptr)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	RECT rt;

	GetWindowRect(hWnd, &rt);

	ptd->hList = CreateWindow(DROPLISTCLASS, NULL, WS_POPUP | WS_BORDER, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);
	SendMessage(ptd->hList, WM_SETOWNER, 0, (LPARAM)ptd->hOwner);
	SendMessage(ptd->hList, WM_ATTACHDATA, 0, (LPARAM)ptr);

	SetFocus(ptd->hEdit);
}

LINKPTR FireList_DetachData(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	LINKPTR ptr;

	ptr = (LINKPTR)SendMessage(ptd->hList, WM_DETACHDATA, 0, 0);

	if (IsWindow(ptd->hList))
	{
		DestroyWindow(ptd->hList);
		ptd->hList = NULL;
	}

	return ptr;
}

int FireList_GetEditorText(HWND hWnd, wchar_t* buf, int max)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	LINKPTR ent;
	const wchar_t* key;
	int len;

	ent = (LINKPTR)SendMessage(ptd->hList, WM_GETCURSEL, 0, 0);
	if (ent)
	{
		key = get_string_entity_key_ptr(ent);
		len = xslen(key);
		len = (len < max) ? len : max;
		xsncpy(buf, key, len);
	}
	else
	{
		len = 0;
	}
	buf[len] = L'\0';

	return len;
}

void FireList_SetEditorText(HWND hWnd, const wchar_t* text)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	LINKPTR ptr, ent;

	SetWindowText(ptd->hEdit, (LPCTSTR)text);
	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);

	ptr = (LINKPTR)SendMessage(ptd->hList, WM_FETCHDATA, 0, 0);
	ent = get_string_entity(ptr, text, -1);
	SendMessage(ptd->hList, WM_SETCURSEL, 0, (LPARAM)ent);
}

int FireList_EditorTextSize(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	LINKPTR ent;
	const wchar_t* sz = NULL;

	ent = (LINKPTR)SendMessage(ptd->hList, WM_GETCURSEL, 0, 0);
	if (ent)
		sz = get_string_entity_key_ptr(ent);
	return xslen(sz);
}

/************************************************************************************/

typedef struct tagFireMemoDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
}FireMemoDelta;

#define GETFIREMEMODELTA(hWnd) 	((FireMemoDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIREMEMODELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireMemoProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int FireMemo_GetEditorText(HWND hWnd, wchar_t* buf, int max);
void FireMemo_SetEditorText(HWND hWnd, const wchar_t* text);
int FireMemo_EditorTextSize(HWND hWnd);

ATOM RegisterFireMemoClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)FireMemoProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = FIREMEMOCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubMemoProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_KEYDOWN:
		if (SendMessage(GetParent(hWnd), WM_KEYPREVIEW, wParam, lParam))
			return 0;
		break;
	default:
		break;
	}

	if (lpfOrgProc)
		return CallWindowProc(lpfOrgProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK FireMemoProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireMemoDelta* ptd = GETFIREMEMODELTA(hWnd);
	LPCREATESTRUCT lpcs;
	RECT rt;
	WNDPROC lpfOrgProc;

	switch (message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireMemoDelta*)xmem_alloc(sizeof(FireMemoDelta));
		memset((void*)ptd, 0, sizeof(FireMemoDelta));
		SETFIREMEMODELTA(hWnd, ptd);

		GetClientRect(hWnd, &rt);
		ptd->hEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hWnd, NULL, NULL, NULL);

		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)SubMemoProc);
		SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit, TRUE);
		SetFocus(ptd->hEdit);
		break;
	case WM_DESTROY:
		SETFIREMEMODELTA(hWnd, 0);

		if (IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit, FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit, GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit, GWLP_WNDPROC, (LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit, GWLP_USERDATA, (LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if (ptd->hFont)
			DeleteObject(ptd->hFont);

		xmem_free(ptd);

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
	case WM_SETFIREFONT:
		if (ptd->hFont)
			DeleteObject(ptd->hFont);
		ptd->hFont = (HFONT)XdcCreateFont((const xfont_t*)lParam);
		SendMessage(ptd->hEdit, WM_SETFONT, (WPARAM)ptd->hFont, MAKELPARAM(1, 0));
		return 0;
	case WM_COMMAND:
		if (ptd)
		{
			if (IsWindow(ptd->hOwner) && HIWORD(wParam) == EN_KILLFOCUS && (HWND)lParam == ptd->hEdit)
			{
				SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
			}
		}
		break;
	case WM_RESETEDITOR:
		SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(wParam, 0), (LPARAM)hWnd);
		return 0;
	case WM_GETEDITORTEXT:
		return FireMemo_GetEditorText(hWnd, (wchar_t*)lParam, (int)wParam);
	case WM_SETEDITORTEXT:
		FireMemo_SetEditorText(hWnd, (const wchar_t*)lParam);
		return 0;
	case WM_EDITORTEXTSIZE:
		return FireMemo_EditorTextSize(hWnd);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int FireMemo_GetEditorText(HWND hWnd, wchar_t* buf, int max)
{
	FireMemoDelta* ptd = GETFIREMEMODELTA(hWnd);

	return GetWindowText(ptd->hEdit, buf, max + 1);
}

void FireMemo_SetEditorText(HWND hWnd, const wchar_t* text)
{
	FireMemoDelta* ptd = GETFIREMEMODELTA(hWnd);

	SetWindowText(ptd->hEdit, (LPCTSTR)text);
	SendMessage(ptd->hEdit, EM_SETSEL, 0, (LPARAM)-1);
}

int FireMemo_EditorTextSize(HWND hWnd)
{
	FireMemoDelta* ptd = GETFIREMEMODELTA(hWnd);

	return GetWindowTextLength(ptd->hEdit);
}

/***************************************************************************************/

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
void FireMenu_OnLButtonDown(HWND hWnd, UINT flag, short x, short y);
void FireMenu_OnLButtonUp(HWND hWnd, UINT flag, short x, short y);
void FireMenu_OnMouseWheel(HWND hWnd, UINT flag, short delta);
void FireMenu_OnPaint(HWND hWnd);

void FireMenu_OnDrawBackground(HWND hWnd, HDC hDC);
void FireMenu_OnDrawForeground(HWND hWnd, HDC hDC);
void FireMenu_OnItemChanging(HWND hWnd);
void FireMenu_OnItemChanging(HWND hWnd, LINKPTR ilk);

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

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)FireMenuProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = FIREMENUCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK FireMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);
	LPCREATESTRUCT lpcs;

	switch (message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireMenuDelta*)xmem_alloc(sizeof(FireMenuDelta));
		memset((void*)ptd, 0, sizeof(FireMenuDelta));
		ptd->canvas = create_display_canvas(NULL);

		SETFIREMENUDELTA(hWnd, ptd);

		SetCapture(hWnd);
		break;
	case WM_DESTROY:
		ReleaseCapture();

		if (ptd)
		{
			destroy_display_canvas(ptd->canvas);
		}
		SETFIREMENUDELTA(hWnd, 0);
		xmem_free(ptd);
		break;
	case WM_MOUSEMOVE:
		if (ptd && ptd->data)
			FireMenu_OnMouseMove(hWnd, wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		if (ptd && ptd->data)
			FireMenu_OnLButtonDown(hWnd, wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if (ptd && ptd->data)
			FireMenu_OnLButtonUp(hWnd, wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		if (ptd && ptd->data)
			FireMenu_OnMouseWheel(hWnd, (UINT)LOWORD(wParam), (short)GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_PAINT:
		if (ptd && GetUpdateRect(hWnd, NULL, TRUE))
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
			FireMenu_TabSkip(hWnd, (int)wParam);
		return 0;
	case WM_GETCURSEL:
		if (ptd && ptd->data)
			return (LRESULT)ptd->curItem;
		else
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
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

void FireMenu_OnLButtonDown(HWND hWnd, UINT flag, short x, short y)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);

}

void FireMenu_OnLButtonUp(HWND hWnd, UINT flag, short x, short y)
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

void FireMenu_OnMouseWheel(HWND hWnd, UINT nFlags, short delta)
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

	hDC = (HDC)begin_display_paint(ptd->canvas, (res_t)ps.hdc, rt.right - rt.left, rt.bottom - rt.top);

	FireMenu_OnDrawBackground(hWnd, hDC);

	draw_menu(ptd->canvas, ptd->data, ptd->curItem);

	FireMenu_OnDrawForeground(hWnd, hDC);

	end_display_paint(ptd->canvas, (res_t)ps.hdc, ps.rcPaint.left, ps.rcPaint.top);

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

	fill_shape_raw((res_t)hDC, &xb, rt.left, rt.top, rt.right, rt.bottom, ATTR_SHAPE_RECT);
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

	draw_focus_raw((res_t)hDC,&xc,rt.left,rt.top,rt.right,rt.bottom);
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
	int count, nWidth, nHeight;

	//client edge 
	GetWindowRect(hWnd, &rt);
	cy = rt.bottom - rt.top;
	cx = rt.right - rt.left;

	GetClientRect(hWnd, &rt);
	cy = cy - (rt.bottom - rt.top);
	cx = cx - (rt.right - rt.left);

	count = get_menu_child_item_count(ptd->data);
	if (count == 0)
	{
		return;
	}

	if (count > 7)
		count = 7;

	nWidth = mm_to_pt(ptd->canvas, get_menu_item_width(ptd->data), 1) - mm_to_pt(ptd->canvas, 0, 1);
	nHeight = mm_to_pt(ptd->canvas, count * get_menu_item_height(ptd->data), 0) - mm_to_pt(ptd->canvas, 0, 0);

	GetWindowRect(hWnd, &rt);
	rt.right = rt.left + nWidth + cx;
	rt.bottom = rt.top + nHeight + cy;

	MoveWindow(hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, FALSE);
	ShowWindow(hWnd, SW_SHOW);
}

void FireMenu_ItemEnsureVisible(HWND hWnd)
{
	FireMenuDelta* ptd = GETFIREMENUDELTA(hWnd);

}

/************************************************************************************************************/
