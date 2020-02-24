/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	dropgrid.cc | drop grid control implement file

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

#include "griddoc.h"
#include "gridview.h"


typedef struct tagDropGridDelta{
	HWND hOwner;
	handle_t canvas;
	LINKPTR data;
	LINKPTR navCol;
	LINKPTR curCol;
	LINKPTR curRow;
	int orgX,orgY;
	int curX,curY;
	
	BOOL bDragCol;
	BOOL bSizeCol;
	BOOL bDraw;
}DropGridDelta;

#define GETDROPGRIDDELTA(hWnd) 	((DropGridDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETDROPGRIDDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK DropGridProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM RegisterDropGridClass(HINSTANCE hInstance);

void DropGrid_OnCreate(HWND hWnd,LPCREATESTRUCT lpcs);
void DropGrid_OnDestroy(HWND hWnd);
void DropGrid_OnLButtonDown(HWND hWnd,UINT flag,short x,short y);
void DropGrid_OnLButtonUp(HWND hWnd,UINT flag,short x,short y);
void DropGrid_OnMouseMove(HWND hWnd,UINT flag,short x,short y);
void DropGrid_OnLButtonDBClick(HWND hWnd);
void DropGrid_OnHScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll);
void DropGrid_OnVScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll);
void DropGrid_OnMouseWheel(HWND hWnd,UINT flag, short delta);
void DropGrid_OnSize(HWND hWnd,int nType,int cx,int cy);
void DropGrid_OnPaint(HWND hWnd);

//mouse moving event
void DropGrid_OnColSizing(HWND hWnd,short x,short y);
void DropGrid_OnColSized(HWND hWnd,short x, short y);
void DropGrid_OnColDrag(HWND hWnd,short x,short y);
void DropGrid_OnColDrop(HWND hWnd,short x, short y);

//input focus event
BOOL DropGrid_OnRowChanging(HWND hWnd);
void DropGrid_OnRowChanged(HWND hWnd,LINKPTR rlk);
void DropGrid_OnColChanging(HWND hWnd);
void DropGrid_OnColChanged(HWND hWnd, LINKPTR rlk);

void DropGrid_OnDrawForeground(HWND hWnd,HDC hDC);
void DropGrid_OnDrawBackground(HWND hWnd,HDC hDC);

void DropGrid_OnResetScroll(HWND hWnd);
void DropGrid_OnResetPage(HWND hWnd);

void DropGrid_AttachData(HWND hWnd,LINKPTR ptr);
LINKPTR DropGrid_DetachData(HWND hWnd);

void DropGrid_ReDraw(HWND hWnd);
void DropGrid_TabSkip(HWND hWnd,int bNext);

void DropGrid_EnsureVisible(HWND hWnd,LINKPTR rl,LINKPTR clk);
BOOL DropGrid_SetRowCol(HWND hWnd,LINKPTR rlk,LINKPTR clk);

void DropGrid_MoveFirstPage(HWND hWnd);
void DropGrid_MovePrevPage(HWND hWnd);
void DropGrid_MoveNextPage(HWND hWnd);
void DropGrid_MoveLastPage(HWND hWnd);
void DropGrid_MoveToPage(HWND hWnd,int page);

int _CalcCellRect(HWND hWnd,LINKPTR rlk,LINKPTR clk,RECT* pRect)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	xrect_t di;
	
	if(calc_grid_cell_rect(ptd->data,rlk,clk,&di))
	{
		pRect->left = mm_to_pt(ptd->canvas,di.x,1) ;
		pRect->top = mm_to_pt(ptd->canvas,di.y,0) ;
		pRect->right = mm_to_pt(ptd->canvas,di.x + di.w,1) ;
		pRect->bottom = mm_to_pt(ptd->canvas,di.y + di.h,0) ;
		return 1;
	}else
	{
		memset((void*)pRect,0,sizeof(RECT));
		return 0;
	}
}

int _CalcRowRect(HWND hWnd,LINKPTR rlk,RECT* pRect)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	xrect_t di;
	
	if(calc_grid_cell_rect(ptd->data,rlk,NULL,&di))
	{
		pRect->left = mm_to_pt(ptd->canvas,di.x,1) ;
		pRect->top = mm_to_pt(ptd->canvas,di.y,0) ;
		pRect->right = mm_to_pt(ptd->canvas,di.x + calc_grid_width(ptd->data),1);
		pRect->bottom = mm_to_pt(ptd->canvas,di.y + di.h,0) ;
		return 1;
	}else
	{
		memset((void*)pRect,0,sizeof(RECT));
		return 0;
	}
}

int _CalcRowRectNoBar(HWND hWnd,LINKPTR rlk,RECT* pRect)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	xrect_t di;
	
	if(calc_grid_cell_rect(ptd->data,rlk,NULL,&di))
	{
		pRect->left = mm_to_pt(ptd->canvas,di.x + di.w,1) ;
		pRect->top = mm_to_pt(ptd->canvas,di.y,0) ;
		pRect->right = mm_to_pt(ptd->canvas,di.x + calc_grid_width(ptd->data),1);
		pRect->bottom = mm_to_pt(ptd->canvas,di.y + di.h,0) ;
		return 1;
	}else
	{
		memset((void*)pRect,0,sizeof(RECT));
		return 0;
	}
}

void _CalcColRect(HWND hWnd,LINKPTR clk,RECT* pRect)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	xrect_t di;
	
	calc_grid_cell_rect(ptd->data,NULL,clk,&di);

	pRect->left = mm_to_pt(ptd->canvas,di.x,1) ;
	pRect->top = mm_to_pt(ptd->canvas,di.y,0) ;
	pRect->right = mm_to_pt(ptd->canvas,di.x + di.w,1) ;
	pRect->bottom = mm_to_pt(ptd->canvas,di.y + calc_grid_height(ptd->data),0) ;
}

ATOM RegisterDropGridClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
	wcex.lpfnWndProc	= (WNDPROC)DropGridProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= DROPGRIDCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK DropGridProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DropGridDelta* 	ptd = GETDROPGRIDDELTA(hWnd);
	int nCurPage,nMaxPage;

	switch(message)
	{
	case WM_CREATE:
		DropGrid_OnCreate(hWnd,(LPCREATESTRUCT)lParam);
		break;
	case WM_DESTROY:
		DropGrid_OnDestroy(hWnd);
		break;
	case WM_SETOWNER:
		if (ptd)
			ptd->hOwner = (HWND)lParam;
		return 0;
	case WM_GETOWNER:
		if (ptd)
			return (LRESULT)ptd->hOwner;
		else
			return 0;
	case WM_ATTACHDATA:
		if(ptd->data)
			DropGrid_DetachData(hWnd);
		DropGrid_AttachData(hWnd,(LINKPTR)lParam);
		return 0;
	case WM_FETCHDATA:
		return (LRESULT)ptd->data;
	case WM_DETACHDATA:
		return (LRESULT)DropGrid_DetachData(hWnd);
	case GM_GETCURROW:
		if(ptd->data)
			return (LRESULT)ptd->curRow;
		else
			return (LRESULT)NULL;
	case GM_SETROWCOL:
		if(ptd->data)
			DropGrid_SetRowCol(hWnd,(LINKPTR)wParam,(LINKPTR)lParam);
		return 0;
	case GM_GETCURCOL:
		if(ptd->data)
			return (LRESULT)ptd->curCol;
		else
			return (LRESULT)NULL;
	case GM_SETNAVCOL:
		if(ptd->data)
			ptd->navCol = (LINKPTR)lParam;
		return 0;
	case WM_SETCURPAGE:
		if(ptd->data)
		{
			nCurPage = get_grid_page_index(ptd->data);
			nMaxPage = calc_grid_pages(ptd->data);

			if(nCurPage != (int)wParam)
			{
				nCurPage = (int)wParam;
				if(nCurPage < 1)
					nCurPage = 1;
				if(nCurPage > nMaxPage)
					nCurPage = nMaxPage;

				ptd->bDraw = TRUE;
				InvalidateRect(hWnd,NULL,FALSE);
			}
		}
		return 0;
	case WM_GETCURPAGE:
		if(ptd->data)
			return (LRESULT)get_grid_page_index(ptd->data);
		else
			return 0;
	case WM_GETMAXPAGE:
		if(ptd->data)
			return (LRESULT)calc_grid_pages(ptd->data);
		else
			return 0;
	case WM_LBUTTONDBLCLK:
		if(ptd->data)
			DropGrid_OnLButtonDBClick(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if(ptd->data)
			DropGrid_OnLButtonDown(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if(ptd->data)
			DropGrid_OnLButtonUp(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		if(ptd->data)
			DropGrid_OnMouseMove(hWnd,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));
		break;
	case WM_SIZE:
		if(ptd->data)
		{
			DropGrid_OnSize(hWnd,(UINT)wParam,(int)LOWORD(lParam),(int)HIWORD(lParam));
		}
		break;
	case WM_VSCROLL:
		if(ptd->data)
			DropGrid_OnVScroll(hWnd,(UINT)LOWORD(wParam),(short)HIWORD(wParam),(HWND)lParam);
		break;
	case WM_HSCROLL:
		if(ptd->data)
			DropGrid_OnHScroll(hWnd,(UINT)LOWORD(wParam),(short)HIWORD(wParam),(HWND)lParam);
		break;
	case WM_MOUSEWHEEL:
		if(ptd->data)
			DropGrid_OnMouseWheel(hWnd,(UINT)LOWORD(wParam),(short)GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_PAINT:
		if(GetUpdateRect(hWnd,NULL,TRUE) && ptd->data)
			DropGrid_OnPaint(hWnd);
		break;
	case WM_TABSKIP:
		if(ptd->data)
			DropGrid_TabSkip(hWnd,(int)wParam);
		return 0;
	case WM_REDRAW:
		if(ptd->data)
			DropGrid_ReDraw(hWnd);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);	
}

void DropGrid_OnCreate(HWND hWnd,LPCREATESTRUCT lpcs)
{
	DropGridDelta* ptd;

	ptd = (DropGridDelta*)xmem_alloc(sizeof(DropGridDelta));

	ptd->canvas = create_display_canvas(NULL);

	SETDROPGRIDDELTA(hWnd,ptd);
}

void DropGrid_OnDestroy(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

	if(ptd->data)
		DropGrid_DetachData(hWnd);
	
	destroy_display_canvas(ptd->canvas);
	ptd->canvas = 0;

	SETDROPGRIDDELTA(hWnd,0);
	xmem_free(ptd);
}

void DropGrid_OnSize(HWND hWnd,int nType,int cx,int cy)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

	if(nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED)
		DropGrid_ReDraw(hWnd);
}

void DropGrid_OnPaint(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rt;

	BeginPaint(hWnd,&ps);

	GetClientRect(hWnd,&rt);

	hDC = (HDC)begin_display_paint(ptd->canvas,(res_dc_t)ps.hdc,rt.right - rt.left,rt.bottom - rt.top);

	DropGrid_OnDrawBackground(hWnd,hDC);

	draw_grid_page(ptd->canvas,ptd->data);

	DropGrid_OnDrawForeground(hWnd,hDC);

	end_display_paint(ptd->canvas,(res_dc_t)ps.hdc,ps.rcPaint.left,ps.rcPaint.top);

	EndPaint(hWnd,&ps);
}

void DropGrid_OnVScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll) 
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	SCROLLINFO si;
	int nCurPage,nMaxPage;

	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo(hWnd,SB_VERT,&si);

	nCurPage = get_grid_page_index(ptd->data);
	nMaxPage = calc_grid_pages(ptd->data);

	switch(nSBCode)
	{
	case SB_LINEUP:
		if(nCurPage == 1)
			break;
		DropGrid_MovePrevPage(hWnd);
		break;
	case SB_PAGEUP:
		if(nCurPage == 1)
			break;
		DropGrid_MovePrevPage(hWnd);
		break;
	case SB_LINEDOWN:
		if(nCurPage == nMaxPage)
			break;
		DropGrid_MoveNextPage(hWnd);
		break;
	case SB_PAGEDOWN:
		if(nCurPage == nMaxPage)
			break;
		DropGrid_MoveNextPage(hWnd);
		break;
	case SB_THUMBPOSITION:
		if(nCurPage == (int)si.nPos + 1)
			break;
		DropGrid_MoveToPage(hWnd,nPos + 1);
		break;
	}
}

void DropGrid_OnHScroll(HWND hWnd,UINT nSBCode, short nPos, HWND hWndScroll)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	SCROLLINFO si;
	float pw,gw;
	int nMax,nLine,nCur;
	
	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo(hWnd,SB_HORZ,&si);

	pw = get_grid_page_width(ptd->data);
	gw = calc_grid_width(ptd->data);

	nMax = (int)(gw * 10) - (int)(pw * 10);
	if(nMax < 0)
		nMax = 0;

	nLine = 100;

	switch(nSBCode)
	{
	case SB_LINELEFT:
		if(si.nPos == 0)
			break;
		nCur = si.nPos - nLine;
		if(nCur < 0)
			nCur = 0;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nMax;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_HORZ,&si,TRUE);
		break;
	case SB_PAGELEFT:
		if(si.nPos == 0)
			break;
		nCur = si.nPos - nLine * 2;
		if(nCur < 0)
			nCur = 0;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nMax;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_HORZ,&si,TRUE);
		break;
	case SB_LINERIGHT:
		if(si.nPos == nMax)
			break;
		nCur = si.nPos + nLine;
		if(nCur > nMax)
			nCur = nMax;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nMax;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_HORZ,&si,TRUE);
		break;
	case SB_PAGERIGHT:
		if(si.nPos == nMax)
			break;
		nCur = si.nPos + nLine * 2;
		if(nCur > nMax)
			nCur = nMax;
		si.fMask = SIF_RANGE | SIF_POS;
		si.nMax = nMax;
		si.nMin = 0;
		si.nPos = nCur;
		SetScrollInfo(hWnd,SB_HORZ,&si,TRUE);
		break;
	case SB_THUMBPOSITION:
		break;
	case SB_ENDSCROLL:
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd,SB_HORZ,&si);
		nCur = si.nPos;
		set_grid_page_xoff(ptd->data,(float)si.nPos / 10);
		ptd->bDraw = TRUE;
		InvalidateRect(hWnd,NULL,FALSE);
		break;
	}
}

void DropGrid_OnMouseWheel(HWND hWnd,UINT nFlags,short delta)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	int nCurPage,nMaxPage;

	nCurPage = get_grid_page_index(ptd->data);
	nMaxPage = calc_grid_pages(ptd->data);

	if(delta > 0)
	{
		if(nCurPage == nMaxPage)
			return;
		DropGrid_MoveNextPage(hWnd);
	}else
	{
		if(nCurPage == 1)
			return;
		DropGrid_MovePrevPage(hWnd);
	}
}

void DropGrid_OnLButtonDBClick(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

}

void DropGrid_OnMouseMove(HWND hWnd,UINT nFlags, short x,short y) 
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	LINKPTR row,col;
	int nHint;
	xpoint_t pt;

	if(ptd->bSizeCol || ptd->bDragCol)
	{
		ptd->curX = x;
		ptd->curY = y;
		//InvalidateRect(hWnd,NULL,FALSE);
		return;
	}

	pt.x = pt_to_mm(ptd->canvas,x,1);
	pt.y = pt_to_mm(ptd->canvas,y,0);
	
	nHint = calc_grid_point_hint(ptd->data,&pt,&row,&col);

	if(nHint == GRID_HINT_HORZ_SPLIT && row == ptd->curRow)
	{
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS)));
	}else if(nHint == GRID_HINT_VERT_SPLIT && col == ptd->curCol)
	{
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE)));
	}else
	{
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
	}
}

void DropGrid_OnLButtonDown(HWND hWnd,UINT nFlags, short x,short y) 
{
	// TODO: Add your message handler code here and/or call default
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	LINKPTR rlk,clk;
	int nHint;
	xpoint_t pt;
	BOOL bReCol,bReRow;

	SetCapture(hWnd);

	pt.x = pt_to_mm(ptd->canvas,x,1);
	pt.y = pt_to_mm(ptd->canvas,y,0);

	nHint = calc_grid_point_hint(ptd->data,&pt,&rlk,&clk);

	bReRow = (rlk == ptd->curRow)? TRUE : FALSE;
	bReCol = (clk == ptd->curCol)? TRUE : FALSE;

	if(!bReRow || !bReCol)
	{
		DropGrid_SetRowCol(hWnd,rlk,clk);
	}

	if (nHint == GRID_HINT_COLBAR)
	{
		if (bReCol)
			DropGrid_OnColDrag(hWnd, x, y);
	}

	if(nHint == GRID_HINT_VERT_SPLIT)
	{
		if(bReCol)
			DropGrid_OnColSizing(hWnd,x,y);
	}
}

void DropGrid_OnLButtonUp(HWND hWnd,UINT nFlags, short x,short y) 
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

	ReleaseCapture();

	if(ptd->bSizeCol && ptd->curCol)
	{
		DropGrid_OnColSized(hWnd,x,y);
		return;
	}

	if(ptd->bDragCol && ptd->curCol)
	{
		DropGrid_OnColDrop(hWnd,x,y);
		return;
	}

	SendMessage(ptd->hOwner, WM_COMMAND, MAKEWPARAM(EC_COMMIT, 0), (LPARAM)hWnd);
}

/*****************************************control event*************************************/
void DropGrid_OnDrawBackground(HWND hWnd,HDC hDC)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	RECT rt;
	xbrush_t xb = {0};

	parse_xbrush_from_style(&xb,DEF_CTRL_BRUSH);

	GetClientRect(hWnd,&rt);

	fill_shape_raw((res_dc_t)hDC,&xb,rt.left,rt.top,rt.right,rt.bottom,ATTR_SHAPE_RECT);
}

void DropGrid_OnDrawForeground(HWND hWnd,HDC hDC)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	xcolor_t xc;
	RECT rt;

	if(ptd->curRow)
	{
		_CalcRowRectNoBar(hWnd,ptd->curRow,&rt);
		parse_xcolor(&xc,DEF_ALPHA_COLOR);
		draw_alpha_raw(hDC,&xc,rt.left,rt.top,rt.right,rt.bottom);
		return;
	}
}

void DropGrid_OnResetPage(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	RECT rt;
	float pw,ph;
	int nCurPage,nMaxPage;
	int nCurLine,nMaxLine;
	
	GetClientRect(hWnd,&rt);
	pw = pt_to_mm(ptd->canvas,rt.right,1) - pt_to_mm(ptd->canvas,rt.left,1);
	set_grid_page_width(ptd->data,pw);
	ph = pt_to_mm(ptd->canvas,rt.bottom,0) - pt_to_mm(ptd->canvas,rt.top,0);
	set_grid_page_height(ptd->data,ph);

	nMaxPage = calc_grid_pages(ptd->data);
	nCurPage = get_grid_page_index(ptd->data);

	if(nCurPage > nMaxPage)
	{
		nCurPage = nMaxPage;
		set_grid_page_index(ptd->data,nCurPage);
	}

	nMaxLine = (int)(calc_grid_width(ptd->data) * 10) - (int)(get_grid_page_width(ptd->data) * 10);
	if(nMaxLine < 0)
		nMaxLine = 0;

	nCurLine = (int)(get_grid_page_xoff(ptd->data) * 10);
	if(nCurLine > nMaxLine)
	{
		nCurLine = nMaxLine;
		set_grid_page_xoff(ptd->data,(float)nCurLine / 10);
	}
}

void DropGrid_OnResetScroll(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	SCROLLINFO si;
	int nCurLine,nMaxLine;
	int nCurPage,nMaxPage;
	
	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo(hWnd,SB_HORZ,&si);

	nMaxLine = (int)(calc_grid_width(ptd->data) * 10) - (int)(get_grid_page_width(ptd->data) * 10);
	if(nMaxLine < 0)
		nMaxLine = 0;

	nCurLine = (int)(get_grid_page_xoff(ptd->data) * 10);
	if(nCurLine > nMaxLine)
		nCurLine = nMaxLine;
	
	si.fMask = SIF_RANGE | SIF_POS;
	si.nMax = nMaxLine;
	si.nMin = 0;
	si.nPos = nCurLine;
	SetScrollInfo(hWnd,SB_HORZ,&si,TRUE);

	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	GetScrollInfo(hWnd,SB_VERT,&si);

	nMaxPage = calc_grid_pages(ptd->data);
	nCurPage = get_grid_page_index(ptd->data);
	if(nCurPage > nMaxPage)
		nCurPage = nMaxPage;

	si.fMask = SIF_RANGE | SIF_POS;
	si.nMax = nMaxPage - 1;
	si.nMin = 0;
	si.nPos = nCurPage - 1;
	SetScrollInfo(hWnd,SB_VERT,&si,TRUE);	
}

void DropGrid_OnColSizing(HWND hWnd,short x,short y)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

	SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE)));

	ptd->bSizeCol = TRUE;
	ptd->orgX = x;
	ptd->orgY = y;
}

void DropGrid_OnColSized(HWND hWnd,short x, short y)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	float cx;
	float mw;

	cx = pt_to_mm(ptd->canvas,x,1) - pt_to_mm(ptd->canvas,ptd->orgX,1);

	mw = get_col_width(ptd->curCol);
	mw += cx;
	if(mw < 1)
		mw = 1;

	if(ptd->curCol)
		set_col_width(ptd->curCol,mw);
	else
		set_grid_rowbar_width(ptd->data,mw);

	DropGrid_OnResetPage(hWnd);
	DropGrid_OnResetScroll(hWnd);

	ptd->bDraw = TRUE;
	InvalidateRect(hWnd,NULL,FALSE);

	ptd->bSizeCol = FALSE;
}

void DropGrid_OnColDrag(HWND hWnd,short x,short y)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

	assert(ptd->curCol);

	SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND)));

	ptd->bDragCol = TRUE;
	ptd->orgX = x;
	ptd->orgY = y;
}

void DropGrid_OnColDrop(HWND hWnd,short x,short y)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	int nHint;
	LINKPTR rlk,clk;
	xpoint_t pt;

	assert(ptd->curCol);

	pt.x = pt_to_mm(ptd->canvas,x,1);
	pt.y = pt_to_mm(ptd->canvas,y,0);

	nHint = calc_grid_point_hint(ptd->data,&pt,&rlk,&clk);
	if(clk != ptd->curCol)
	{
		if(clk)
		{
			switch_link_before(clk,ptd->curCol);
		}else
		{
			if(x < ptd->orgX)
				switch_link_first(ptd->curCol);
			else
				switch_link_last(ptd->curCol);
		}
	}

	DropGrid_OnResetPage(hWnd);
	DropGrid_OnResetScroll(hWnd);

	ptd->bDraw = TRUE;
	InvalidateRect(hWnd,NULL,FALSE);

	ptd->bDragCol = FALSE;
}

BOOL DropGrid_OnRowChanging(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	RECT rt;

	assert(ptd->curRow);
	
	_CalcRowRect(hWnd,ptd->curRow,&rt);
	rt.left -= DEF_FOCUS_FEED;
	rt.right += DEF_FOCUS_FEED;
	rt.top -= DEF_FOCUS_FEED;
	rt.bottom += DEF_FOCUS_FEED;

	ptd->curRow = NULL;

	ptd->bDraw = FALSE;
	InvalidateRect(hWnd,&rt,FALSE);

	return TRUE;
}

void DropGrid_OnRowChanged(HWND hWnd,LINKPTR rlk)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	RECT rt;

	assert(rlk && !ptd->curRow);

	_CalcRowRect(hWnd,rlk,&rt);
	rt.left -= DEF_FOCUS_FEED;
	rt.right += DEF_FOCUS_FEED;
	rt.top -= DEF_FOCUS_FEED;
	rt.bottom += DEF_FOCUS_FEED;

	ptd->curRow = rlk;
	ptd->bDraw = FALSE;
	InvalidateRect(hWnd,&rt,FALSE);
}

void DropGrid_OnColChanging(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	RECT rt;

	assert(ptd->curCol);

	_CalcColRect(hWnd,ptd->curCol,&rt);
	rt.left -= DEF_FOCUS_FEED;
	rt.right += DEF_FOCUS_FEED;
	rt.top -= DEF_FOCUS_FEED;
	rt.bottom += DEF_FOCUS_FEED;

	ptd->curCol = NULL;

	ptd->bDraw = FALSE;
	InvalidateRect(hWnd,&rt,FALSE);
}

void DropGrid_OnColChanged(HWND hWnd,LINKPTR clk)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	RECT rt;

	assert(clk && !ptd->curCol);

	_CalcColRect(hWnd,clk,&rt);
	rt.left -= DEF_FOCUS_FEED;
	rt.right += DEF_FOCUS_FEED;
	rt.top -= DEF_FOCUS_FEED;
	rt.bottom += DEF_FOCUS_FEED;

	ptd->curCol = clk;
	ptd->bDraw = FALSE;
	InvalidateRect(hWnd,&rt,FALSE);
}
/**************************************CONTROL METHOD*************************************/
void DropGrid_AttachData(HWND hWnd,LINKPTR ptr)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

	ptd->data = ptr;
	if(ptd->data == NULL)
		return;

	set_grid_page_index(ptd->data,1);
	set_grid_page_xoff(ptd->data,0);

	DropGrid_ReDraw(hWnd);
}

LINKPTR DropGrid_DetachData(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	LINKPTR data;

	data = ptd->data;
	ptd->data = NULL;

	InvalidateRect(hWnd,NULL,FALSE);

	return data;
}

void DropGrid_ReDraw(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	
	if (!is_grid_col(ptd->data, ptd->curCol))
		ptd->curCol = NULL;

	if (!is_grid_row(ptd->data, ptd->curRow))
		ptd->curRow = NULL;

	DropGrid_OnResetPage(hWnd);
	DropGrid_OnResetScroll(hWnd);
	
	ptd->bDraw = TRUE;
	InvalidateRect(hWnd,NULL,FALSE);
}

void DropGrid_TabSkip(HWND hWnd,int dir)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	LINKPTR rlk,clk;
	LINKPTR lastrow;

	rlk = ptd->curRow;
	clk = ptd->curCol;

	switch(dir)
	{
	case NF_LEFT:
		if(clk == NULL)
			clk = get_prev_focusable_col(ptd->data,LINK_LAST);
		else
			clk = get_prev_focusable_col(ptd->data,clk);
		DropGrid_SetRowCol(hWnd,rlk,clk);
		break;
	case NF_RIGHT:
		if(clk == NULL)
			clk = get_next_focusable_col(ptd->data,LINK_FIRST);
		else	
			clk = get_next_focusable_col(ptd->data,clk);
		DropGrid_SetRowCol(hWnd,rlk,clk);
		break;
	case NF_UP:
		if(rlk != NULL)
		{
			rlk = get_prev_visible_row(ptd->data,rlk);
			DropGrid_SetRowCol(hWnd,rlk,clk);
		}
		break;
	case NF_DOWN:
		if(rlk != NULL)
		{
			rlk = get_next_visible_row(ptd->data,rlk);
			if(rlk)
				DropGrid_SetRowCol(hWnd,rlk,clk);
		}else
		{
			calc_grid_row_scope(ptd->data,&rlk,&lastrow);
			if(rlk)
				DropGrid_SetRowCol(hWnd,rlk,clk);
		}
		break;
	case NF_HOME:
		DropGrid_MoveFirstPage(hWnd);
		break;
	case NF_END:
		DropGrid_MoveLastPage(hWnd);
		break;
	case NF_PAGEUP:
		DropGrid_MovePrevPage(hWnd);
		break;
	case NF_PAGEDOWN:
		DropGrid_MoveNextPage(hWnd);
		break;
	}
}

/*void DropGrid_FireNote(HWND hWnd,LINKPTR rlk,LINKPTR clk)
{
	DropGridDelta* ptd =GETDROPGRIDDELTA(hWnd);
	LINKPTR nextcol;
	int sumcols;

	assert(rlk && clk);

	if(get_col_fireable(clk))
	{
		DropGrid_ReDrawRow(hWnd,rlk,TRUE); //recalc row
	}else
	{
		DropGrid_ReDrawRow(hWnd,rlk,FALSE); //not calc row
	}
	
	sumcols = 0;
	if(get_grid_showsum(ptd->data))
	{
		if(get_col_sum_mode_ptr(clk))
		{
			sum_grid_col(ptd->data,clk);
			sumcols ++;
		}
		
		if(get_col_fireable(clk))
		{
			nextcol = get_next_col(ptd->data,LINK_FIRST);
			while(nextcol)
			{
				if(nextcol != clk && get_col_sum_mode_ptr(nextcol))
				{
					sum_grid_col(ptd->data,nextcol);
					sumcols ++;
							
					SendGridNotify(hWnd,NC_COLSUMED,clk,NULL,NULL);
				}
				nextcol = get_next_col(ptd->data,nextcol);
			}
		}
	}
	
	if(sumcols && get_grid_showsum(ptd->data))
	{
		DropGrid_ReDrawSumRow(hWnd);
	}
}*/

void DropGrid_EnsureVisible(HWND hWnd,LINKPTR rlk,LINKPTR clk)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);

	if(ensure_grid_cell_visible(ptd->data,rlk,clk))
	{
		DropGrid_OnResetScroll(hWnd);

		ptd->bDraw = TRUE;
		InvalidateRect(hWnd,NULL,FALSE);
	}
}

BOOL DropGrid_SetRowCol(HWND hWnd,LINKPTR rlk,LINKPTR clk)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	BOOL bReRow,bReCol;

	bReRow = (rlk == ptd->curRow)? TRUE : FALSE;
	bReCol = (clk == ptd->curCol)? TRUE : FALSE;

	if(bReRow && bReCol)
	{
		return TRUE;
	}

	if(!bReRow && ptd->curRow)
	{
		if(!DropGrid_OnRowChanging(hWnd))
			return FALSE;
	}

	if(!bReCol && ptd->curCol)
	{
		DropGrid_OnColChanging(hWnd);
	}

	DropGrid_EnsureVisible(hWnd,rlk,clk);

	if(!bReCol && clk)
		DropGrid_OnColChanged(hWnd,clk);

	if(!bReRow && rlk)
		DropGrid_OnRowChanged(hWnd,rlk);

	return TRUE;
}

void DropGrid_MoveFirstPage(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	int nCurPage;

	nCurPage = get_grid_page_index(ptd->data);

	if(nCurPage != 1)
	{
		nCurPage = 1;
		set_grid_page_index(ptd->data,nCurPage);

		DropGrid_OnResetScroll(hWnd);
		ptd->bDraw = TRUE;
		InvalidateRect(hWnd,NULL,FALSE);
	}
}

void DropGrid_MovePrevPage(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	int nCurPage;

	nCurPage = get_grid_page_index(ptd->data);

	if(nCurPage > 1)
	{
		nCurPage --;
		set_grid_page_index(ptd->data,nCurPage);

		DropGrid_OnResetScroll(hWnd);
		ptd->bDraw = TRUE;
		InvalidateRect(hWnd,NULL,FALSE);
	}
}

void DropGrid_MoveNextPage(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	int nCurPage,nMaxPage;

	nCurPage = get_grid_page_index(ptd->data);
	nMaxPage = calc_grid_pages(ptd->data);

	if(nCurPage < nMaxPage)
	{
		nCurPage ++;
		set_grid_page_index(ptd->data,nCurPage);

		DropGrid_OnResetScroll(hWnd);
		ptd->bDraw = TRUE;
		InvalidateRect(hWnd,NULL,FALSE);
	}
}

void DropGrid_MoveLastPage(HWND hWnd)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	int nCurPage,nMaxPage;

	nCurPage = get_grid_page_index(ptd->data);
	nMaxPage = calc_grid_pages(ptd->data);

	if(nCurPage != nMaxPage)
	{
		nCurPage = nMaxPage;
		set_grid_page_index(ptd->data,nCurPage);

		DropGrid_OnResetScroll(hWnd);
		ptd->bDraw = TRUE;
		InvalidateRect(hWnd,NULL,FALSE);
	}
}

void DropGrid_MoveToPage(HWND hWnd,int page)
{
	DropGridDelta* ptd = GETDROPGRIDDELTA(hWnd);
	int nCurPage,nMaxPage;

	nCurPage = get_grid_page_index(ptd->data);
	nMaxPage = calc_grid_pages(ptd->data);

	if(page > 0 && page != nCurPage && page <= nMaxPage)
	{
		nCurPage = page;
		set_grid_page_index(ptd->data,nCurPage);

		DropGrid_OnResetScroll(hWnd);
		ptd->bDraw = TRUE;
		InvalidateRect(hWnd,NULL,FALSE);
	}
}
