/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget document
	
	@module	_if_widget_wnc.c | widget widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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

#include "../_if_xdk.h"
#include "../xdkutil.h"

#define WIDGET_TITLE_SPAN		(float)10 //mm
#define WIDGET_MENU_SPAN		(float)7.5 //mm
#define WIDGET_SCROLL_SPAN		(float)5//mm
#define WIDGET_ICON_SPAN		(float)3//mm
#define WIDGET_FRAME_EDGE		(float)1.5 //mm
#define WIDGET_CHILD_EDGE		(float)0.5 //mm

typedef enum{
	_SCROLL_HINT_NONE,
	_SCROLL_HINT_MIN,
	_SCROLL_HINT_MAX,
	_SCROLL_HINT_BAR
}SCROLL_HINT_CODE;

typedef enum{
	HINT_NOWHERE = 0,
	HINT_CLIENT = 1,
	HINT_MINIMIZE = 2,
	HINT_MAXIMIZE = 3,
	HINT_RESTORE = 4,
	HINT_CLOSE = 5,
	HINT_TITLE = 6,
	HINT_ICON = 7,
	HINT_LEFT = 8,
	HINT_RIGHT = 9,
	HINT_BOTTOM = 10,
	HINT_LEFTBOTTOM = 11,
	HINT_RIGHTBOTTOM = 12,
	HINT_BORDER = 13,
	HINT_HSCROLL = 14,
	HINT_VSCROLL = 15,
	HINT_MENUBAR = 16,
}NCHINT_CODE;

///////////////////////////////////////////////////////////////////////////////////
static void _CenterRect(RECT* prt, long cx, long cy)
{
	long lx, ly;

	lx = prt->right - prt->left;
	ly = prt->bottom - prt->top;

	if (lx > cx)
	{
		prt->left += (lx - cx) / 2;
		prt->right -= (lx - cx) / 2;
	}

	if (ly > cy)
	{
		prt->top += (ly - cy) / 2;
		prt->bottom -= (ly - cy) / 2;
	}
}

void _ParseColor(xcolor_t* pxc, const tchar_t* color)
{
	int len;
	tchar_t* token;
	tchar_t val[10];

	pxc->r = pxc->g = pxc->b = 0;
	if (_tstrnull(color))
		return;

	if (_tstrnicmp(color, _T("RGB("), 4) == 0)
		token = (tchar_t*)color + 4;
	else
		token = (tchar_t*)color;

	len = 0;
	while (*token != _T('.') && *token != _T(',') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	len = (len < 3) ? len : 3;
	if (len)
	{
		_tstrncpy(val, token - len, len);
		pxc->r = (unsigned char)_tstrtol(val);
	}

	if (*token == _T('.') || *token == _T(','))
		token++;

	len = 0;
	while (*token != _T('.') && *token != _T(',') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	len = (len < 3) ? len : 3;
	if (len)
	{
		_tstrncpy(val, token - len, len);
		pxc->g = (unsigned char)_tstrtol(val);
	}

	if (*token == _T('.') || *token == _T(','))
		token++;

	len = 0;
	while (*token != _T(')') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	len = (len < 3) ? len : 3;
	if (len)
	{
		_tstrncpy(val, token - len, len);
		pxc->b = (unsigned char)_tstrtol(val);
	}
}
////////////////////////////////////////////////////////////////////////////////////////
static HPEN _create_pen(const xpen_t* pxp)
{
	long ps;
	xcolor_t xc = { 0 };

	if (!pxp)
		return CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	_ParseColor(&xc, pxp->color);

	if (_tstricmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
#ifdef WINCE
		ps = PS_DASH;
#else
		ps = PS_DOT;
#endif
	else if (_tstricmp(pxp->style, GDI_ATTR_STROKE_STYLE_DASHED) == 0)
		ps = PS_DASH;
	else
		ps = PS_SOLID;

	return CreatePen(ps, _tstrtol(pxp->size), RGB(xc.r, xc.g, xc.b));
}

static HBRUSH _create_brush(const xbrush_t* pxb)
{
	xcolor_t xc = { 0 };

	if (!pxb)
		return CreateSolidBrush(RGB(255, 255, 255));

	_ParseColor(&xc, pxb->color);

	return CreateSolidBrush(RGB(xc.r, xc.g, xc.b));
}

static HFONT _create_font(const xfont_t* pxf)
{
	HDC hDC;
	LOGFONT lf;
	HFONT hf;
	long fs;

#ifdef WINCE
	hf = (HFONT)GetStockObject(SYSTEM_FONT);
#else
	hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
#endif
	GetObject(hf, sizeof(LOGFONT), (void*)&lf);

	if (!pxf)
		return CreateFontIndirect(&lf);

	hDC = GetDC(NULL);

	if (_tstrnull(pxf->size))
		fs = _tstrtol(GDI_ATTR_FONT_SIZE_SYSTEM);
	else
		fs = _tstrtol(pxf->size);

	lf.lfHeight = -MulDiv(fs, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	ReleaseDC(NULL, hDC);

	lf.lfWeight = _tstrtol(pxf->weight);

	if (_tstricmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (_tstricmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (_tstricmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!_tstrnull(pxf->family))
		_tstrcpy(lf.lfFaceName, pxf->family);
	else
		_tstrcpy(lf.lfFaceName, GDI_ATTR_FONT_FAMILY_ARIA);

	return CreateFontIndirect(&lf);
}

static HBITMAP _CreateBitmap(HDC rdc, const byte_t* pb, dword_t bytes)
{
	PBITMAPINFO pbmi;
	BITMAPFILEHEADER bfh;
	LPBYTE lpBits;

	if (!pb)
		return NULL;

	if ((DWORD)bytes < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
		return NULL;

	memcpy((void*)&bfh, (void*)pb, sizeof(BITMAPFILEHEADER));

	if (bfh.bfType != 0x4d42)
		return NULL;

	if ((DWORD)bytes < bfh.bfSize)
		return NULL;

	pbmi = (PBITMAPINFO)(pb + sizeof(BITMAPFILEHEADER));
	lpBits = (LPBYTE)(pb + bfh.bfOffBits);

	return  CreateDIBitmap(rdc, (BITMAPINFOHEADER*)pbmi, CBM_INIT, lpBits, pbmi, DIB_RGB_COLORS);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
static VOID _DrawBitmap(HDC hDC, HBITMAP hBmp, LPCRECT prt)
{
	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), (void*)&bmp);

	HDC hComDC = CreateCompatibleDC(hDC);
	HBITMAP orgBmp = (HBITMAP)SelectObject(hComDC, hBmp);

	RECT rt;

	memcpy((void*)&rt, (void*)prt, sizeof(RECT));
	_CenterRect(&rt, bmp.bmWidth, bmp.bmHeight);

	TransparentBlt(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hComDC, 0, 0, rt.right - rt.left, rt.bottom - rt.top, RGB(255, 255, 255));

	hBmp = (HBITMAP)SelectObject(hComDC, orgBmp);

	DeleteDC(hComDC);
}

static VOID _HorzGradient(HDC hDC, COLORREF clrNear, COLORREF clrCenter, LPCRECT prt)
{
	TRIVERTEX tv[4];
	GRADIENT_RECT gr[2];
	unsigned long mode = 0;

	tv[0].x = prt->left;
	tv[0].y = prt->top;
	tv[0].Red = GetRValue(clrNear) << 8;
	tv[0].Green = GetGValue(clrNear) << 8;
	tv[0].Blue = GetBValue(clrNear) << 8;
	tv[0].Alpha = 255;

	tv[1].x = (prt->left + prt->right) / 2;
	tv[1].y = prt->bottom;
	tv[1].Red = GetRValue(clrCenter) << 8;
	tv[1].Green = GetGValue(clrCenter) << 8;
	tv[1].Blue = GetBValue(clrCenter) << 8;
	tv[1].Alpha = 255;

	tv[2].x = (prt->left + prt->right) / 2;
	tv[2].y = prt->top;
	tv[2].Red = GetRValue(clrCenter) << 8;
	tv[2].Green = GetGValue(clrCenter) << 8;
	tv[2].Blue = GetBValue(clrCenter) << 8;
	tv[2].Alpha = 255;

	tv[3].x = prt->right;
	tv[3].y = prt->bottom;
	tv[3].Red = GetRValue(clrNear) << 8;
	tv[3].Green = GetGValue(clrNear) << 8;
	tv[3].Blue = GetBValue(clrNear) << 8;
	tv[3].Alpha = 255;

	mode = GRADIENT_FILL_RECT_H;

	gr[0].UpperLeft = 0;
	gr[0].LowerRight = 1;
	gr[1].UpperLeft = 2;
	gr[1].LowerRight = 3;

	GradientFill(hDC, tv, 4, &gr, 2, mode);
}

static VOID _VertGradient(HDC hDC, COLORREF clrNear, COLORREF clrCenter, LPCRECT prt)
{
	TRIVERTEX tv[4];
	GRADIENT_RECT gr[2];
	unsigned long mode = 0;

	tv[0].x = prt->left;
	tv[0].y = prt->top;
	tv[0].Red = GetRValue(clrNear) << 8;
	tv[0].Green = GetGValue(clrNear) << 8;
	tv[0].Blue = GetBValue(clrNear) << 8;
	tv[0].Alpha = 255;

	tv[1].x = prt->right;
	tv[1].y = (prt->top + prt->bottom) / 2;
	tv[1].Red = GetRValue(clrCenter) << 8;
	tv[1].Green = GetGValue(clrCenter) << 8;
	tv[1].Blue = GetBValue(clrCenter) << 8;
	tv[1].Alpha = 255;

	tv[2].x = prt->left;
	tv[2].y = (prt->top + prt->bottom) / 2;
	tv[2].Red = GetRValue(clrCenter) << 8;
	tv[2].Green = GetGValue(clrCenter) << 8;
	tv[2].Blue = GetBValue(clrCenter) << 8;
	tv[2].Alpha = 255;

	tv[3].x = prt->right;
	tv[3].y = prt->bottom;
	tv[3].Red = GetRValue(clrNear) << 8;
	tv[3].Green = GetGValue(clrNear) << 8;
	tv[3].Blue = GetBValue(clrNear) << 8;
	tv[3].Alpha = 255;

	mode = GRADIENT_FILL_RECT_V;

	gr[0].UpperLeft = 0;
	gr[0].LowerRight = 1;
	gr[1].UpperLeft = 2;
	gr[1].LowerRight = 3;

	GradientFill(hDC, tv, 4, &gr, 2, mode);
}
///////////////////////////////////////////////////////////////////////////////////

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

static void _WindowRectToScreen(HWND hWnd, RECT* prt)
{
	RECT rtWnd;

	GetWindowRect(hWnd, &rtWnd);

	prt->left += rtWnd.left;
	prt->right += rtWnd.left;
	prt->top += rtWnd.top;
	prt->bottom += rtWnd.top;
}

static void _WindowPointToScreen(HWND hWnd, POINT* ppt)
{
	RECT rtWnd;

	GetWindowRect(hWnd, &rtWnd);

	ppt->x += rtWnd.left;
	ppt->y += rtWnd.top;
}
///////////////////////////////////////////////////////////////////////////////////////

static void _WidgetCalcBorder(DWORD ws, long* edge, long* title, long* hscroll, long* vscroll, long* menu)
{
	*edge = *title = *hscroll = *vscroll = *menu = 0;

	xsize_t xs;

	if (ws & WD_STYLE_TITLE)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_TITLE_SPAN;
		_screen_size_to_pt(&xs);

		*title = xs.cy;
	}

	if (ws & WD_STYLE_BORDER)
	{
		xs.fx = ZERO_WIDTH;
		if (ws & WD_STYLE_CHILD)
			xs.fy = WIDGET_CHILD_EDGE;
		else
			xs.fy = WIDGET_FRAME_EDGE;
		_screen_size_to_pt(&xs);

		*edge = xs.cy;
	}

	if (ws & WD_STYLE_HSCROLL)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_SCROLL_SPAN;
		_screen_size_to_pt(&xs);

		*hscroll = xs.cy;
	}

	if (ws & WD_STYLE_VSCROLL)
	{
		xs.fx = WIDGET_SCROLL_SPAN;
		xs.fy = ZERO_HEIGHT;
		_screen_size_to_pt(&xs);

		*vscroll = xs.cx;
	}

	if (ws & WD_STYLE_MENUBAR)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_MENU_SPAN;
		_screen_size_to_pt(&xs);

		*menu = xs.cy;
	}
}

VOID _WidgetCalcHScroll(HWND hWnd, long* ppos, long* pmin, long* pmax)
{
	scroll_t sc;
	dword_t ws;

	*ppos = *pmin = *pmax = 0;

	ws = _widget_get_style(hWnd);

	if (ws & WD_STYLE_HSCROLL)
	{
		_widget_get_scroll(hWnd, 1, &sc);

		*ppos = sc.pos;
		*pmin = sc.min;
		*pmax = sc.max;
	}
}

VOID _WidgetCalcVScroll(HWND hWnd, long* ppos, long* pmin, long* pmax)
{
	scroll_t sc;
	dword_t ws;

	*ppos = *pmin = *pmax = 0;

	ws = _widget_get_style(hWnd);

	if (ws & WD_STYLE_VSCROLL)
	{
		_widget_get_scroll(hWnd, 0, &sc);

		*ppos = sc.pos;
		*pmin = sc.min;
		*pmax = sc.max;
	}
}

VOID _WidgetCalcHScrollRect(HWND hWnd, RECT* prt)
{
	long edge, title, hscr, vscr, menu;
	RECT rtWnd;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!hscr)
		return;

	GetWindowRect(hWnd, &rtWnd);
	_ScreenRectToWindow(hWnd, &rtWnd);

	prt->left = rtWnd.left + edge;
	prt->right = rtWnd.right - edge - vscr;
	prt->top = rtWnd.bottom - edge - hscr;
	prt->bottom = rtWnd.bottom - edge;
}

VOID _WidgetCalcVScrollRect(HWND hWnd, RECT* prt)
{
	long edge, title, hscr, vscr, menu;
	RECT rtWnd;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!vscr)
		return;

	GetWindowRect(hWnd, &rtWnd);
	_ScreenRectToWindow(hWnd, &rtWnd);

	prt->left = rtWnd.right - edge - vscr;
	prt->right = rtWnd.right - edge;
	prt->top = rtWnd.top + edge + title + menu;
	prt->bottom = rtWnd.bottom - edge - hscr;
}

VOID _WidgetCalcHScrollBarRect(HWND hWnd, RECT* prt)
{
	long edge, title, hscr, vscr, menu;
	long nMax, nMin, nPos;
	int ind;
	RECT rtWnd;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!hscr)
		return;

	_WidgetCalcHScroll(hWnd, &nPos, &nMin, &nMax);

	GetWindowRect(hWnd, &rtWnd);
	_ScreenRectToWindow(hWnd, &rtWnd);

	if (nMax < 0 || nPos == nMax)
	{
		prt->left = rtWnd.right - edge - vscr - hscr;
		prt->right = rtWnd.right - edge - vscr;
	}
	else
	{
		ind = (int)(((float)nPos / (float)nMax) * ((float)(rtWnd.right - rtWnd.left - 2 * edge - vscr) / (float)hscr));

		prt->left = rtWnd.left + edge + (ind)* hscr;
		prt->right = rtWnd.left + edge + (ind + 1) * hscr;
		prt->top = rtWnd.bottom - edge - hscr;
		prt->bottom = rtWnd.bottom - edge;
	}
}

VOID _WidgetCalcVScrollBarRect(HWND hWnd, RECT* prt)
{
	long edge, title, hscr, vscr, menu;
	long nMax, nMin, nPos;
	int ind;
	RECT rtWnd;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!vscr)
		return;

	_WidgetCalcVScroll(hWnd, &nPos, &nMin, &nMax);

	GetWindowRect(hWnd, &rtWnd);
	_ScreenRectToWindow(hWnd, &rtWnd);

	if (nMax < 0 || nPos == nMax)
	{
		prt->top = rtWnd.bottom - edge - hscr - vscr;
		prt->bottom = rtWnd.bottom - edge - hscr;
	}
	else
	{
		ind = (int)(((float)nPos / (float)nMax) * ((float)(rtWnd.bottom - rtWnd.top - title - menu - 2 * edge - hscr) / (float)vscr));

		prt->left = rtWnd.right - edge - vscr;
		prt->right = rtWnd.right - edge;
		prt->top = rtWnd.top + edge + title + menu + (ind)* vscr;
		prt->bottom = rtWnd.top + edge + title + menu + (ind + 1) * vscr;
	}
}

int _WidgetCalcHScrollHint(HWND hWnd, long x, long y, short* ppos)
{
	long edge, title, hscr, vscr, menu;
	
	int hint = _SCROLL_HINT_NONE;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!hscr)
		return hint;

	long nMax, nMin, nPos;
	RECT rtScr;

	_WidgetCalcHScrollRect(hWnd, &rtScr);

	_WidgetCalcHScroll(hWnd, &nPos, &nMin, &nMax);

	if (nMax > 0)
	{
		*ppos = (short)((float)(x - edge) / (float)(rtScr.right - rtScr.left) * (float)nMax);
	}
	else
	{
		*ppos = 0;
	}

	POINT pt;
	pt.x = x;
	pt.y = y;

	RECT rt;

	rt.left = rtScr.left;
	rt.right = rtScr.left + hscr;
	rt.top = rtScr.bottom - hscr;
	rt.bottom = rtScr.bottom;

	if (PtInRect(&rt, pt))
		return _SCROLL_HINT_MIN;

	rt.left = rtScr.right - hscr;
	rt.right = rtScr.right;
	rt.top = rtScr.bottom - hscr;
	rt.bottom = rtScr.bottom;

	if (PtInRect(&rt, pt))
		return _SCROLL_HINT_MAX;

	rt.left = rtScr.left + hscr;
	rt.right = rtScr.right - hscr;
	rt.top = rtScr.bottom - hscr;
	rt.bottom = rtScr.bottom;

	if (PtInRect(&rt, pt))
		return _SCROLL_HINT_BAR;

	return hint;
}

int _WidgetCalcVScrollHint(HWND hWnd, long x, long y, short* ppos)
{
	long edge, title, hscr, vscr, menu;

	int hint = _SCROLL_HINT_NONE;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!vscr)
		return hint;

	long nMax, nMin, nPos;
	RECT rtScr;

	_WidgetCalcVScrollRect(hWnd, &rtScr);

	_WidgetCalcVScroll(hWnd, &nPos, &nMin, &nMax);

	if (nMax > 0)
	{
		*ppos = (short)((float)(y - edge - title - menu) / (float)(rtScr.bottom - rtScr.top) * (float)nMax);
	}
	else
	{
		*ppos = 0;
	}

	POINT pt;
	pt.x = x;
	pt.y = y;

	RECT rt;

	rt.left = rtScr.right - vscr;
	rt.right = rtScr.right;
	rt.top = rtScr.top;
	rt.bottom = rtScr.top + vscr;

	if (PtInRect(&rt, pt))
		return _SCROLL_HINT_MIN;

	rt.left = rtScr.right - vscr;
	rt.right = rtScr.right;
	rt.top = rtScr.bottom - vscr;
	rt.bottom = rtScr.bottom;

	if (PtInRect(&rt, pt))
		return _SCROLL_HINT_MAX;

	rt.left = rtScr.right - vscr;
	rt.right = rtScr.right;
	rt.top = rtScr.top + vscr;
	rt.bottom = rtScr.bottom - vscr;

	if (PtInRect(&rt, pt))
		return _SCROLL_HINT_BAR;

	return hint;
}

VOID _WidgetCalcMenuBarRect(HWND hWnd, RECT* prt)
{
	long edge, title, hscr, vscr, menu;
	RECT rtWnd;

	DWORD ws = _widget_get_style(hWnd);

	GetWindowRect(hWnd, &rtWnd);
	_ScreenRectToWindow(hWnd, &rtWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!menu)
		return;

	prt->left = rtWnd.left + edge;
	prt->right = rtWnd.right - edge;
	prt->top = rtWnd.top + edge + title;
	prt->bottom = rtWnd.top + edge + title + menu;
}

static int _WidgetCalcMenuBarHint(HWND hWnd, long x, long y, LINKPTR* pilk)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return 0;

	long edge, title, vscr, hscr, menu;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!menu)
		return 0;

	RECT rtMenu;

	_WidgetCalcMenuBarRect(hWnd, &rtMenu);

	if (!pwt->menu)
		return 0;

	int count = get_menu_item_count(pwt->menu);

	RECT rtItem;

	rtItem.left = rtMenu.left;
	rtItem.right = rtMenu.left;
	rtItem.top = rtMenu.top;
	rtItem.bottom = rtMenu.bottom;

	POINT pt;
	pt.x = x;
	pt.y = y;

	HDC hDC = GetWindowDC(hWnd);
	SIZE se;
	LPCTSTR text;

	int hint = MENU_HINT_NONE;

	LINKPTR ilk = get_menu_next_item(pwt->menu, LINK_FIRST);
	while (ilk)
	{
		text = get_menu_item_title_ptr(ilk);
		GetTextExtentPoint32(hDC, text, _tstrlen(text), &se);

		rtItem.left = rtItem.right;
		rtItem.right = rtItem.left + menu + se.cx;

		if (PtInRect(&rtItem, pt))
		{
			*pilk = ilk;
			hint = MENU_HINT_ITEM;
			break;
		}

		ilk = get_menu_next_item(pwt->menu, ilk);
	}

	ReleaseDC(hWnd, hDC);

	return hint;
}

static void _WidgetCalcMenuItemRect(HWND hWnd, LINKPTR plk, xrect_t* pxr)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	xmem_zero((void*)pxr,sizeof(xrect_t));

	if (!pwt)
		return ;

	long edge, title, vscr, hscr, menu;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!menu)
		return;

	RECT rtMenu;

	_WidgetCalcMenuBarRect(hWnd, &rtMenu);

	if (!pwt->menu)
		return;

	RECT rtItem;

	rtItem.left = rtMenu.left;
	rtItem.right = rtMenu.left;
	rtItem.top = rtMenu.top;
	rtItem.bottom = rtMenu.bottom;

	HDC hDC = GetWindowDC(hWnd);
	SIZE se;
	LPCTSTR text;

	LINKPTR ilk = get_menu_next_item(pwt->menu, LINK_FIRST);
	while (ilk)
	{
		text = get_menu_item_title_ptr(ilk);
		GetTextExtentPoint32(hDC, text, _tstrlen(text), &se);

		rtItem.left = rtItem.right;
		rtItem.right = rtItem.left + menu + se.cx;

		if (ilk == plk)
		{
			_WindowRectToScreen(hWnd, &rtItem);

			pxr->x = rtItem.left;
			pxr->y = rtItem.top;
			pxr->w = rtItem.right - rtItem.left;
			pxr->h = rtItem.bottom - rtItem.top;

			break;
		}

		ilk = get_menu_next_item(pwt->menu, ilk);
	}

	ReleaseDC(hWnd, hDC);
}

static int _WidgetCalcNcHint(HWND hWnd, long x, long y)
{
	long edge, title, span, vscr, hscr, menu;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!edge && !title && !vscr && !hscr && !menu)
		return HINT_CLIENT;
	
	RECT xr,rt;
	POINT pt;

	GetWindowRect(hWnd, &xr);

	pt.x = x;
	pt.y = y;

	if (menu)
	{
		rt.left = xr.left + edge;
		rt.right = xr.right - edge;
		rt.top = xr.top + edge + title;
		rt.bottom = rt.top + menu;

		if (PtInRect(&rt, pt))
			return HINT_MENUBAR;
	}

	if (hscr)
	{
		rt.left = xr.left + edge;
		rt.right = xr.right - edge;
		rt.top = xr.bottom - edge - hscr;
		rt.bottom = xr.bottom - edge;

		if (PtInRect(&rt, pt))
			return HINT_HSCROLL;
	}

	if (vscr)
	{
		rt.left = xr.right - edge - vscr;
		rt.right = rt.left + vscr;
		rt.top = xr.top + edge + title + menu;
		rt.bottom = xr.bottom - hscr;

		if (PtInRect(&rt, pt))
			return HINT_VSCROLL;
	}

	if (!edge && title)
	{
		rt.left = xr.left;
		rt.right = xr.right;
		rt.top = xr.top;
		rt.bottom = xr.top + title;

		if (PtInRect(&rt, pt))
			return HINT_BORDER;
		else
			return HINT_CLIENT;
	}

	rt.left = xr.left + edge;
	rt.right = xr.left + edge + title;
	rt.top = xr.top + edge;
	rt.bottom = xr.top + edge + title;
	if (PtInRect(&rt, pt))
	{
		return HINT_ICON;
	}

	span = 0;
	if (ws & WD_STYLE_CLOSEBOX)
		span += title / 2;
	if (ws & WD_STYLE_SIZEBOX)
		span += title;

	rt.left = xr.right - edge - (title / 2 * 3);
	rt.right = xr.right - edge - title;
	rt.top = xr.top + edge;
	rt.bottom = xr.top + edge + title;
	if (PtInRect(&rt, pt))
	{
		if (ws & WD_STYLE_SIZEBOX)
			return HINT_MINIMIZE;
		else
			return HINT_TITLE;
	}

	rt.left = xr.right - edge - title;
	rt.right = xr.right - edge - (title / 2);
	rt.top = xr.top + edge;
	rt.bottom = xr.top + edge + title;
	if (PtInRect(&rt, pt))
	{
		if (ws & WD_STYLE_SIZEBOX)
		{
			if (IsZoomed(hWnd))
				return HINT_RESTORE;
			else
				return HINT_MAXIMIZE;
		}
		else
			return HINT_TITLE;
	}

	rt.left = xr.right - edge - (title / 2);
	rt.right = xr.right - edge;
	rt.top = xr.top + edge;
	rt.bottom = xr.top + edge + title;
	if (PtInRect(&rt, pt))
	{
		if (ws & WD_STYLE_CLOSEBOX)
			return HINT_CLOSE;
		else
			return HINT_TITLE;
	}

	rt.left = xr.left + edge;
	rt.right = xr.right - edge;
	rt.top = xr.top + edge;
	rt.bottom = xr.top + edge + title;
	if (PtInRect(&rt, pt))
	{
		return HINT_TITLE;
	}

	rt.left = xr.left;
	rt.right = xr.right;
	rt.top = xr.top;
	rt.bottom = xr.top + edge + title;
	if (PtInRect(&rt, pt))
	{
		return HINT_BORDER;
	}

	rt.left = xr.right - edge;
	rt.right = xr.right;
	rt.top = xr.top + edge + title;
	rt.bottom = xr.bottom - edge;
	if (PtInRect(&rt, pt))
	{
		return HINT_RIGHT;
	}

	rt.left = xr.right - edge;
	rt.right = xr.right;
	rt.top = xr.bottom - edge;
	rt.bottom = xr.bottom;
	if (PtInRect(&rt, pt))
	{
		return HINT_RIGHTBOTTOM;
	}

	rt.left = xr.left + edge;
	rt.right = xr.right - edge;
	rt.top = xr.bottom - edge;
	rt.bottom = xr.bottom;
	if (PtInRect(&rt, pt))
	{
		return HINT_BOTTOM;
	}

	rt.left = xr.left;
	rt.right = xr.left + edge;
	rt.top = xr.bottom - edge;
	rt.bottom = xr.bottom;
	if (PtInRect(&rt, pt))
	{
		return HINT_LEFTBOTTOM;
	}

	rt.left = xr.left;
	rt.right = xr.left + edge;
	rt.top = xr.top + edge + title;
	rt.bottom = xr.bottom - edge;
	if (PtInRect(&rt, pt))
	{
		return HINT_LEFT;
	}

	rt.left = xr.left + edge;
	rt.right = xr.right - edge;
	rt.top = xr.top + edge + title;
	rt.bottom = xr.bottom - edge;
	if (PtInRect(&rt, pt))
	{
		return HINT_CLIENT;
	}

	return HINT_NOWHERE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void _WidgetDrawMenuBar(HWND hWnd, HDC hDC)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	long edge, title, vscr, hscr, menu;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!menu)
		return;

	RECT rtMenu;

	_WidgetCalcMenuBarRect(hWnd, &rtMenu);

	xbrush_t xb = { 0 };

	memcpy((void*)&xb, (void*)&pwt->xb, sizeof(xbrush_t));

	xcolor_t xc_near, xc_center;
	_ParseColor(&xc_near, xb.color);
	_ParseColor(&xc_center, xb.color);
	lighten_xcolor(&xc_center, DEF_SOFT_DARKEN);

	COLORREF clrNear = RGB(xc_near.r, xc_near.g, xc_near.b);
	COLORREF clrCenter = RGB(xc_center.r, xc_center.g, xc_center.b);

	_VertGradient(hDC, clrNear, clrCenter, &rtMenu);

	if (!pwt->menu)
		return;

	HFONT hNewFont, hOrgFont;
	COLORREF crNew, crOrg;

	xfont_t xf = { 0 };

	memcpy((void*)&xf, (void*)&pwt->xf, sizeof(xfont_t));
	if (!_tstrnull(xf.size))
	{
		_tsprintf(xf.size, _T("%d"), _tstrtol(xf.size) - 1);
	}

	hNewFont = (HFONT)_create_font(&xf);
	hOrgFont = (HFONT)SelectObject(hDC, hNewFont);

	xcolor_t xc;
	_ParseColor(&xc, pwt->xf.color);

	crNew = COLORREF(RGB(xc.r, xc.g, xc.b));
	crOrg = (COLORREF)SetTextColor(hDC, crNew);

	const tchar_t* iname;
	int len;
	byte_t* buf;
	HBITMAP hBmp;
	RECT rtImage;

	LINKPTR plk, imagelist = get_menu_images(pwt->menu);

	rtImage.left = rtMenu.left;
	rtImage.right = rtMenu.left;
	rtImage.top = rtMenu.top;
	rtImage.bottom = rtMenu.bottom;

	SIZE seText;
	LPCTSTR text;

	LINKPTR ilk = get_menu_next_item(pwt->menu, LINK_FIRST);
	while (ilk)
	{
		text = get_menu_item_title_ptr(ilk);
		GetTextExtentPoint32(hDC, text, _tstrlen(text), &seText);

		rtImage.left = rtImage.right;
		rtImage.right = rtImage.left + menu;

		iname = get_menu_item_image_ptr(ilk);
		if (!_tstrnull(iname))
		{
			plk = get_image_item(imagelist, iname, -1);
			if (plk)
			{
				len = get_image_item_bitmap(plk, NULL, MAX_LONG);
				buf = (byte_t*)xmem_alloc(len);
				get_image_item_bitmap(plk, buf, len);

				hBmp = _CreateBitmap(hDC, buf, len);

				xmem_free(buf);

				_DrawBitmap(hDC, hBmp, &rtImage);

				DeleteObject(hBmp);
			}
		}

		rtImage.left = rtImage.right;
		rtImage.right = rtImage.left + seText.cx;

		if (!_tstrnull(text))
		{
			DrawText(hDC, text, _tstrlen(text), &rtImage, DT_SINGLELINE | DT_VCENTER);
		}

		ilk = get_menu_next_item(pwt->menu, ilk);
	}

	SetTextColor(hDC, crOrg);
	hNewFont = (HFONT)SelectObject(hDC, hOrgFont);

	DeleteObject(hNewFont);
}


static void _WidgetDrawHScroll(HWND hWnd, HDC hDC)
{
	long edge, title, hscr, vscr, menu;
	long nMax, nPos;
	RECT rtWnd, rtScr;

	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!hscr)
		return;

	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };

	memcpy((void*)&xb, (void*)&pwt->xb, sizeof(xbrush_t));
	memcpy((void*)&xp, (void*)&pwt->xp, sizeof(xpen_t));

	nMax = pwt->cb.w - pwt->vb.w;
	nPos = pwt->vb.x;

	GetClientRect(hWnd, &rtScr);
	_ClientRectToWindow(hWnd, &rtScr);

	ExcludeClipRect(hDC, rtScr.left, rtScr.top, rtScr.right, rtScr.bottom);

	GetWindowRect(hWnd, &rtWnd);
	rtWnd.right -= rtWnd.left;
	rtWnd.bottom -= rtWnd.top;
	rtWnd.left = rtWnd.top = 0;

	SetBkMode(hDC, TRANSPARENT);

	xcolor_t xc_near, xc_center;
	_ParseColor(&xc_near, xb.color);
	_ParseColor(&xc_center, xb.color);
	lighten_xcolor(&xc_center, DEF_SOFT_LIGHTEN);

	rtScr.left = rtWnd.left + edge;
	rtScr.right = rtWnd.right - edge;
	rtScr.top = rtWnd.bottom - edge - hscr;
	rtScr.bottom = rtWnd.bottom - edge;

	COLORREF clrNear = RGB(xc_near.r, xc_near.g, xc_near.b);
	COLORREF clrCenter = RGB(xc_center.r, xc_center.g, xc_center.b);

	_VertGradient(hDC, clrNear, clrCenter, &rtScr);

	HPEN hNewPen, hOrgPen;
	HBRUSH hNewBrush, hOrgBrush;

	hNewPen = (HPEN)_create_pen(&xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	hNewBrush = (HBRUSH)_create_brush(&xb);
	hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	rtScr.left = rtWnd.left + edge;
	rtScr.right = rtWnd.left + edge + hscr;
	rtScr.top = rtWnd.bottom - edge - hscr;
	rtScr.bottom = rtWnd.bottom - edge;

	Rectangle(hDC, rtScr.left + 2, rtScr.top + 2, rtScr.right - 2, rtScr.bottom - 2);

	rtScr.left = rtWnd.right - edge - vscr - hscr;
	rtScr.right = rtWnd.right - edge - vscr;
	rtScr.top = rtWnd.bottom - edge - hscr;
	rtScr.bottom = rtWnd.bottom - edge;

	Rectangle(hDC, rtScr.left + 2, rtScr.top + 2, rtScr.right - 2, rtScr.bottom - 2);

	hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
	DeleteObject(hNewBrush);

	_WidgetCalcHScrollBarRect(hWnd, &rtScr);

	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	hNewBrush = (HBRUSH)_create_brush(&xb);
	hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	Rectangle(hDC, rtScr.left + 2, rtScr.top + 2, rtScr.right - 2, rtScr.bottom - 2);

	hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
	DeleteObject(hNewBrush);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);
}

static void _WidgetDrawVScroll(HWND hWnd, HDC hDC)
{
	long edge, title, hscr, vscr, menu;
	long nMax, nPos;
	RECT rtWnd, rtScr;

	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!vscr)
		return;

	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };

	memcpy((void*)&xb, (void*)&pwt->xb, sizeof(xbrush_t));
	memcpy((void*)&xp, (void*)&pwt->xp, sizeof(xpen_t));

	nMax = pwt->cb.h - pwt->vb.h;
	nPos = pwt->vb.y;

	GetClientRect(hWnd, &rtScr);
	_ClientRectToWindow(hWnd, &rtScr);

	ExcludeClipRect(hDC, rtScr.left, rtScr.top, rtScr.right, rtScr.bottom);

	GetWindowRect(hWnd, &rtWnd);
	rtWnd.right -= rtWnd.left;
	rtWnd.bottom -= rtWnd.top;
	rtWnd.left = rtWnd.top = 0;

	SetBkMode(hDC, TRANSPARENT);

	xcolor_t xc_near, xc_center;
	_ParseColor(&xc_near, xb.color);
	_ParseColor(&xc_center, xb.color);
	lighten_xcolor(&xc_center, DEF_SOFT_LIGHTEN);

	rtScr.left = rtWnd.right - edge - vscr;
	rtScr.right = rtWnd.right - edge;
	rtScr.top = rtWnd.top + edge + title + menu;
	rtScr.bottom = rtWnd.bottom - edge;

	COLORREF clrNear = RGB(xc_near.r, xc_near.g, xc_near.b);
	COLORREF clrCenter = RGB(xc_center.r, xc_center.g, xc_center.b);

	_HorzGradient(hDC, clrNear, clrCenter, &rtScr);

	HPEN hNewPen, hOrgPen;
	HBRUSH hNewBrush, hOrgBrush;

	hNewPen = (HPEN)_create_pen(&xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	hNewBrush = (HBRUSH)_create_brush(&xb);
	hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	rtScr.left = rtWnd.right - edge - vscr;
	rtScr.right = rtWnd.right - edge;
	rtScr.top = rtWnd.top + edge + title + menu;
	rtScr.bottom = rtWnd.top + edge + title + menu + vscr;

	Rectangle(hDC, rtScr.left + 2, rtScr.top + 2, rtScr.right - 2, rtScr.bottom - 2);

	rtScr.left = rtWnd.right - edge - vscr;
	rtScr.right = rtWnd.right - edge;
	rtScr.top = rtWnd.bottom - edge - hscr - vscr;
	rtScr.bottom = rtWnd.bottom - edge - hscr;

	Rectangle(hDC, rtScr.left + 2, rtScr.top + 2, rtScr.right - 2, rtScr.bottom - 2);

	hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
	DeleteObject(hNewBrush);

	_WidgetCalcVScrollBarRect(hWnd, &rtScr);

	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	hNewBrush = (HBRUSH)_create_brush(&xb);
	hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	Rectangle(hDC, rtScr.left + 2, rtScr.top + 2, rtScr.right - 2, rtScr.bottom - 2);

	hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
	DeleteObject(hNewBrush);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);
}

static void _WidgetDrawLogo(HWND hWnd, HDC hDC, LPRECT prt)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	if (pwt->logo)
	{
		_DrawBitmap(hDC, pwt->logo, prt);
	}
	else
	{
		HPEN hNewPen, hOrgPen;
		HBRUSH hNewBrush, hOrgBrush;

		hNewPen = (HPEN)_create_pen(&pwt->xp);
		hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

		hNewBrush = (HBRUSH)_create_brush(&pwt->xb);
		hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

		xsize_t xs;

		xs.fx = WIDGET_ICON_SPAN;
		xs.fy = WIDGET_ICON_SPAN;

		_screen_size_to_pt(&xs);

		RECT rt;
		memcpy((void*)&rt, (void*)prt, sizeof(RECT));
		_CenterRect(&rt, xs.cx, xs.cy);

		Rectangle(hDC, rt.left, rt.top, rt.right, rt.bottom);

		hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
		DeleteObject(hNewBrush);

		hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
		DeleteObject(hNewPen);
	}
}

static void _WidgetDrawTitle(HWND hWnd, HDC hDC, LPRECT prt)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	HFONT hNewFont, hOrgFont;
	COLORREF crNew, crOrg;

	hNewFont = (HFONT)_create_font(&pwt->xf);
	hOrgFont = (HFONT)SelectObject(hDC, hNewFont);

	xcolor_t xc;
	_ParseColor(&xc, pwt->xf.color);

	crNew = COLORREF(RGB(xc.r, xc.g, xc.b));
	crOrg = (COLORREF)SetTextColor(hDC, crNew);

	tchar_t sz_title[RES_LEN + 1] = { 0 };

	GetWindowText(hWnd, sz_title, RES_LEN);

	DrawText(hDC, sz_title, -1, prt, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	SetTextColor(hDC, crOrg);
	hNewFont = (HFONT)SelectObject(hDC, hOrgFont);

	DeleteObject(hNewFont);
}

static void _WidgetDrawHelp(HWND hWnd, HDC hDC, LPRECT prt)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	xpen_t xp = { 0 };
	memcpy((void*)&xp, (void*)&pwt->xp, sizeof(xpen_t));
	_tstrcpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	HPEN hNewPen, hOrgPen;

	hNewPen = (HPEN)_create_pen(&xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	xsize_t xs;

	xs.fx = WIDGET_ICON_SPAN;
	xs.fy = WIDGET_ICON_SPAN;

	_screen_size_to_pt(&xs);

	RECT rt;
	memcpy((void*)&rt, (void*)prt, sizeof(RECT));
	_CenterRect(&rt, xs.cx, xs.cy);

	MoveToEx(hDC, rt.left, (prt->top + prt->bottom) / 2, NULL);
	LineTo(hDC, rt.right, (prt->top + prt->bottom) / 2);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);
}

static void _WidgetDrawMinBox(HWND hWnd, HDC hDC, LPRECT prt)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	HPEN hNewPen, hOrgPen;

	hNewPen = (HPEN)_create_pen(&pwt->xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	xbrush_t xb = { 0 };
	memcpy((void*)&xb, (void*)&pwt->xb, sizeof(xbrush_t));
	_tstrcpy(xb.color, pwt->xp.color);

	HBRUSH hNewBrush, hOrgBrush;

	hNewBrush = (HBRUSH)_create_brush(&xb);
	hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	xsize_t xs;

	xs.fx = WIDGET_ICON_SPAN;
	xs.fy = WIDGET_ICON_SPAN;

	_screen_size_to_pt(&xs);

	RECT rt;
	memcpy((void*)&rt, (void*)prt, sizeof(RECT));
	_CenterRect(&rt, xs.cx, xs.cy);

	Rectangle(hDC, rt.left, rt.bottom - 3, rt.right - 3, rt.bottom);

	hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
	DeleteObject(hNewBrush);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);
}

static void _WidgetDrawMaxBox(HWND hWnd, HDC hDC, LPRECT prt)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	xpen_t xp = { 0 };
	memcpy((void*)&xp, (void*)&pwt->xp, sizeof(xpen_t));
	_tstrcpy(xp.size, _T("2"));

	HPEN hNewPen, hOrgPen;

	hNewPen = (HPEN)_create_pen(&xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	HBRUSH hNewBrush, hOrgBrush;

	hNewBrush = (HBRUSH)_create_brush(&pwt->xb);
	hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	xsize_t xs;

	xs.fx = WIDGET_ICON_SPAN;
	xs.fy = WIDGET_ICON_SPAN;

	_screen_size_to_pt(&xs);

	RECT rt;
	memcpy((void*)&rt, (void*)prt, sizeof(RECT));
	_CenterRect(&rt, xs.cx, xs.cy);

	Rectangle(hDC, rt.left, rt.top, rt.right, rt.bottom);

	hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
	DeleteObject(hNewBrush);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);
}

static void _WidgetDrawZoomBox(HWND hWnd, HDC hDC, LPRECT prt)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	xpen_t xp = { 0 };
	memcpy((void*)&xp, (void*)&pwt->xp, sizeof(xpen_t));
	_tstrcpy(xp.size, _T("2"));

	HPEN hNewPen, hOrgPen;

	hNewPen = (HPEN)_create_pen(&xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	HBRUSH hNewBrush, hOrgBrush;

	hNewBrush = (HBRUSH)_create_brush(&pwt->xb);
	hOrgBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	RECT rt;
	memcpy((void*)&rt, (void*)prt, sizeof(RECT));

	xsize_t xs;

	xs.fx = WIDGET_ICON_SPAN;
	xs.fy = WIDGET_ICON_SPAN;

	_screen_size_to_pt(&xs);

	_CenterRect(&rt, xs.cx, xs.cy);

	Rectangle(hDC, rt.left, rt.top, rt.right, rt.bottom);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);

	_tstrcpy(xp.size, _T("1"));

	hNewPen = (HPEN)_create_pen(&xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	rt.left += 3;
	rt.top += 3;
	rt.right -= 3;
	rt.bottom -= 3;

	Rectangle(hDC, rt.left, rt.top, rt.right, rt.bottom);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);

	hNewBrush = (HBRUSH)SelectObject(hDC, hOrgBrush);
	DeleteObject(hNewBrush);
}

static void _WidgetDrawCloseBox(HWND hWnd, HDC hDC, LPRECT prt)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	xpen_t xp = { 0 };
	memcpy((void*)&xp, (void*)&pwt->xp, sizeof(xpen_t));
	_tstrcpy(xp.size, _T("2"));

	HPEN hNewPen, hOrgPen;

	hNewPen = (HPEN)_create_pen(&xp);
	hOrgPen = (HPEN)SelectObject(hDC, hNewPen);

	xsize_t xs;

	xs.fx = WIDGET_ICON_SPAN;
	xs.fy = WIDGET_ICON_SPAN;

	_screen_size_to_pt(&xs);

	RECT rt;
	memcpy((void*)&rt, (void*)prt, sizeof(RECT));
	_CenterRect(&rt, xs.cx, xs.cy);

	MoveToEx(hDC, rt.left, rt.top, NULL);
	LineTo(hDC, rt.right, rt.bottom);

	MoveToEx(hDC, rt.left, rt.bottom, NULL);
	LineTo(hDC, rt.right, rt.top);

	hNewPen = (HPEN)SelectObject(hDC, hOrgPen);
	DeleteObject(hNewPen);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int _WidgetNcHitTest(HWND hWnd, long x, long y)
{
	int hint;

	hint = _WidgetCalcNcHint(hWnd, x, y);

	return (hint == HINT_MENUBAR || hint == HINT_ICON || hint == HINT_MINIMIZE || hint == HINT_MAXIMIZE || hint == HINT_RESTORE || hint == HINT_CLOSE || hint == HINT_HSCROLL || hint == HINT_VSCROLL) ? HINT_BORDER : hint;
}

void _WidgetNcMouseMove(HWND hWnd, long x, long y)
{
	int hint;

	hint = _WidgetCalcNcHint(hWnd, x, y);
}

void _WidgetNcLButtonDown(HWND hWnd, long x, long y)
{
	int hint;

	hint = _WidgetCalcNcHint(hWnd, x, y);
}

void _WidgetNcLButtonUp(HWND hWnd, long x, long y)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return;

	int hint = _WidgetCalcNcHint(hWnd, x, y);

	POINT pt;
	pt.x = x;
	pt.y = y;
	_ScreenPointToWindow(hWnd, &pt);

	short pos = 0;

	int mint = 0;
	LINKPTR mlk = NULL;

	switch (hint)
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
		PostMessage(hWnd, WM_SYSCOMMAND, SC_CONTEXTHELP, MAKELPARAM(x, y));
		break;
	case HINT_HSCROLL:
		mint = _WidgetCalcHScrollHint(hWnd, pt.x, pt.y, &pos);
		if (mint == _SCROLL_HINT_MIN)
		{
			if (pwt->vb.x)
			{
				PostMessage(hWnd, WM_SCROLL, (WPARAM)1, (LPARAM)(-pwt->vb.x));
			}
			/*else
			{
				PostMessage(hWnd, WM_KEYDOWN, (WPARAM)KEY_PAGEUP, (LPARAM)1);
				PostMessage(hWnd, WM_KEYUP, (WPARAM)KEY_PAGEUP, (LPARAM)1);
			}*/
		}
		else if (mint == _SCROLL_HINT_MAX)
		{
			if (pwt->vb.x < pwt->cb.w - pwt->vb.w)
			{
				PostMessage(hWnd, WM_SCROLL, (WPARAM)1, (LPARAM)(pwt->cb.w - pwt->vb.w));
			}
			/*else
			{
				PostMessage(hWnd, WM_KEYDOWN, (WPARAM)KEY_PAGEDOWN, (LPARAM)1);
				PostMessage(hWnd, WM_KEYUP, (WPARAM)KEY_PAGEDOWN, (LPARAM)1);
			}*/
		}
		else if (mint == _SCROLL_HINT_BAR)
		{
			if (pos != pwt->vb.x)
			{
				PostMessage(hWnd, WM_SCROLL, (WPARAM)1, (LPARAM)(pos - pwt->vb.x));
			}
		}
		break;
	case HINT_VSCROLL:
		mint = _WidgetCalcVScrollHint(hWnd, pt.x, pt.y, &pos);
		if (mint == _SCROLL_HINT_MIN)
		{
			if (pwt->vb.y)
			{
				PostMessage(hWnd, WM_SCROLL, (WPARAM)0, (LPARAM)(-pwt->vb.y));
			}
			else
			{
				PostMessage(hWnd, WM_KEYDOWN, (WPARAM)KEY_PAGEUP, (LPARAM)1);
				PostMessage(hWnd, WM_KEYUP, (WPARAM)KEY_PAGEUP, (LPARAM)1);
			}
		}
		else if (mint == _SCROLL_HINT_MAX)
		{
			if (pwt->vb.y < pwt->cb.h - pwt->vb.h)
			{
				PostMessage(hWnd, WM_SCROLL, (WPARAM)0, (LPARAM)(pwt->cb.h - pwt->vb.h));
			}
			else
			{
				PostMessage(hWnd, WM_KEYDOWN, (WPARAM)KEY_PAGEDOWN, (LPARAM)1);
				PostMessage(hWnd, WM_KEYUP, (WPARAM)KEY_PAGEDOWN, (LPARAM)1);
			}
		}
		else if (mint == _SCROLL_HINT_BAR)
		{
			if (pos != pwt->vb.y)
			{
				PostMessage(hWnd, WM_SCROLL, (WPARAM)0, (LPARAM)(pos - pwt->vb.y));
			}
		}
		break;
	case HINT_MENUBAR:
		mint = _WidgetCalcMenuBarHint(hWnd, pt.x, pt.y, &mlk);
		if (mint == MENU_HINT_ITEM)
		{
			int iid = _widget_get_user_id(hWnd);
			int code = get_menu_item_iid(mlk);
			PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(iid,code), (LPARAM)NULL);
		}
		break;
	}
}

BOOL _WidgetNcPaint(HWND hWnd, HDC hDC)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	if (!pwt)
		return FALSE;

	long edge, title, hscr, vscr, menu;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	if (!edge && !title && !hscr && !vscr)
		return FALSE;
	
	xbrush_t xb;
	memcpy((void*)&xb, (void*)&pwt->xb, sizeof(xbrush_t));

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	RECT rtWnd, rtScr;

	GetClientRect(hWnd, &rtScr);
	_ClientRectToWindow(hWnd, &rtScr);

	ExcludeClipRect(hDC, rtScr.left, rtScr.top, rtScr.right, rtScr.bottom);

	GetWindowRect(hWnd, &rtWnd);
	rtWnd.right -= rtWnd.left;
	rtWnd.bottom -= rtWnd.top;
	rtWnd.left = rtWnd.top = 0;

	SetBkMode(hDC, TRANSPARENT);

	if (edge)
	{
		HBRUSH hNewBrush;

		hNewBrush = (HBRUSH)_create_brush(&xb);

		FillRect(hDC, &rtWnd, hNewBrush);

		DeleteObject(hNewBrush);
	}

	if (menu)
	{
		_WidgetDrawMenuBar(hWnd, hDC);
	}

	if (hscr)
	{
		_WidgetDrawHScroll(hWnd, hDC);
	}

	if (vscr)
	{
		_WidgetDrawVScroll(hWnd, hDC);
	}

	if (title)
	{
		xcolor_t xc_near, xc_center;

		_ParseColor(&xc_near, xb.color);
		_ParseColor(&xc_center, xb.color);;
		lighten_xcolor(&xc_center, DEF_SOFT_DARKEN);

		COLORREF clrNear = RGB(xc_near.r, xc_near.g, xc_near.b);
		COLORREF clrCenter = RGB(xc_center.r, xc_center.g, xc_center.b);

		rtScr.left = rtWnd.left + edge;
		rtScr.top = rtWnd.top + edge;
		rtScr.right = rtWnd.right - edge;
		rtScr.bottom = rtWnd.top + edge + title;

		_HorzGradient(hDC, clrNear, clrCenter, &rtScr);
	}

	if (title && edge)
	{
		rtScr.left = rtWnd.left + edge;
		rtScr.right = rtWnd.left + edge + title;
		rtScr.top = rtWnd.top + edge;
		rtScr.bottom = rtWnd.top + edge + title;
		_CenterRect(&rtScr, title / 2, title / 2);

		_WidgetDrawLogo(hWnd, hDC, &rtScr);

		rtScr.left = rtWnd.left + edge + title;
		rtScr.right = rtWnd.right - edge - 2 * title;
		rtScr.top = rtWnd.top + edge;
		rtScr.bottom = rtWnd.top + edge + title;

		_WidgetDrawTitle(hWnd, hDC, &rtScr);

		if (ws & WD_STYLE_SIZEBOX)
		{
			rtScr.left = rtWnd.right - edge - (title / 2 * 3);
			rtScr.right = rtScr.left + title / 2;
			rtScr.top = rtWnd.top + edge;
			rtScr.bottom = rtScr.top + title * 2 / 3;

			_WidgetDrawMinBox(hWnd, hDC, &rtScr);

			rtScr.left = rtWnd.right - edge - title;
			rtScr.right = rtScr.left + title / 2;
			rtScr.top = rtWnd.top + edge;
			rtScr.bottom = rtScr.top + title * 2 / 3;
			if (IsZoomed(hWnd))
				_WidgetDrawZoomBox(hWnd, hDC, &rtScr);
			else
				_WidgetDrawMaxBox(hWnd, hDC, &rtScr);
		}

		if (ws & WD_STYLE_CLOSEBOX)
		{
			rtScr.left = rtWnd.right - edge - (title / 2);
			rtScr.right = rtScr.left + title / 2;
			rtScr.top = rtWnd.top + edge;
			rtScr.bottom = rtScr.top + title * 2 / 3;
			_WidgetDrawCloseBox(hWnd, hDC, &rtScr);
		}
	}

	return TRUE;
}

static VOID _WidgetNcCalcSize(HWND hWnd, RECT* prt)
{
	long edge, title, hscr, vscr, menu;

	DWORD ws = _widget_get_style(hWnd);

	_WidgetCalcBorder(ws, &edge, &title, &hscr, &vscr, &menu);

	prt->left += (edge);
	prt->right -= (edge + vscr);
	prt->top += (edge + title + menu);
	prt->bottom -= (edge + hscr);
}
/*******************************************************************************************/
result_t _widget_hand_nc_paint(res_win_t hWnd, uparam_t wParam, lparam_t lParam)
{
	HDC hDC = NULL;

	if (wParam == 1)
		hDC = GetWindowDC(hWnd);
	else
		hDC = GetDCEx(hWnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);

	BOOL b = _WidgetNcPaint(hWnd, hDC);

	ReleaseDC(hWnd, hDC);

	return (result_t)b;
}

result_t _widget_hand_nc_calcsize(res_win_t hWnd, uparam_t wParam, lparam_t lParam)
{
	RECT rtCli;

	if ((BOOL)wParam)
	{
		LPNCCALCSIZE_PARAMS lpcp = (LPNCCALCSIZE_PARAMS)lParam;

		rtCli.left = lpcp->rgrc[0].left;
		rtCli.top = lpcp->rgrc[0].top;
		rtCli.right = lpcp->rgrc[0].right;
		rtCli.bottom = lpcp->rgrc[0].bottom;

		_WidgetNcCalcSize(hWnd, &rtCli);

		lpcp->rgrc[2].left = lpcp->rgrc[1].left;
		lpcp->rgrc[2].top = lpcp->rgrc[1].top;
		lpcp->rgrc[2].right = lpcp->rgrc[1].right;
		lpcp->rgrc[2].bottom = lpcp->rgrc[1].bottom;

		lpcp->rgrc[1].left = lpcp->rgrc[0].left;
		lpcp->rgrc[1].top = lpcp->rgrc[0].top;
		lpcp->rgrc[1].right = lpcp->rgrc[0].right;
		lpcp->rgrc[1].bottom = lpcp->rgrc[0].bottom;

		lpcp->rgrc[0].left = rtCli.left;
		lpcp->rgrc[0].top = rtCli.top;
		lpcp->rgrc[0].right = rtCli.right;
		lpcp->rgrc[0].bottom = rtCli.bottom;
	}
	else
	{
		LPRECT lprt = (LPRECT)lParam;

		rtCli.left = lprt->left;
		rtCli.top = lprt->top;
		rtCli.right = lprt->right;
		rtCli.bottom = lprt->bottom;

		_WidgetNcCalcSize(hWnd, &rtCli);

		lprt->left = rtCli.left;
		lprt->top = rtCli.top;
		lprt->right = rtCli.right;
		lprt->bottom = rtCli.bottom;
	}

	return 0;
}

result_t _widget_hand_nc_hittest(res_win_t hWnd, uparam_t wParam, lparam_t lParam)
{
	int hit = _WidgetNcHitTest(hWnd, (long)(short)LOWORD(lParam), (long)(short)HIWORD(lParam));

	switch (hit)
	{
	case HINT_NOWHERE:
		return HTNOWHERE;
	case HINT_CLIENT:
		return HTCLIENT;
	case HINT_MINIMIZE:
		return HTMINBUTTON;
	case HINT_MAXIMIZE:
		return HTMAXBUTTON;
	case HINT_RESTORE:
		return HTSIZE;
	case HINT_CLOSE:
		return HTCLOSE;
	case HINT_TITLE:
		return HTCAPTION;
	case HINT_LEFT:
		return HTLEFT;
	case HINT_RIGHT:
		return HTRIGHT;
	case HINT_BOTTOM:
		return HTBOTTOM;
	case HINT_LEFTBOTTOM:
		return HTBOTTOMLEFT;
	case HINT_RIGHTBOTTOM:
		return HTBOTTOMRIGHT;
	case HINT_BORDER:
		return HTBORDER;
	case HINT_MENUBAR:
		return HTBORDER;
	case HINT_ICON:
		return HTBORDER;
	default:
		return HTNOWHERE;
	}
}

result_t _widget_hand_nc_lbutton_down(res_win_t hWnd, uparam_t wParam, lparam_t lParam)
{
	_WidgetNcLButtonDown(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}

result_t _widget_hand_nc_lbutton_up(res_win_t hWnd, uparam_t wParam, lparam_t lParam)
{
	_WidgetNcLButtonUp(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}

result_t _widget_hand_nc_mouse_move(res_win_t hWnd, uparam_t wParam, lparam_t lParam)
{
	_WidgetNcMouseMove(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}

void _widget_draw_nc_horz_scroll(res_win_t hWnd, res_ctx_t dc)
{
	_WidgetDrawHScroll(hWnd, dc);
}

void _widget_draw_nc_vert_scroll(res_win_t hWnd, res_ctx_t dc)
{
	_WidgetDrawVScroll(hWnd, dc);
}

void _widget_calc_nc_border(dword_t ws, border_t* pbd)
{
	_WidgetCalcBorder(ws, &pbd->edge, &pbd->title, &pbd->hscroll, &pbd->vscroll, &pbd->menu);
}

void _widget_calc_nc_menu_item(res_win_t hWnd, int iid, xrect_t* pxr)
{
	win_struct_t* pwt = _widget_get_struct(hWnd);

	xmem_zero((void*)pxr, sizeof(xrect_t));

	LINKPTR plk = (pwt->menu) ? get_menu_item_by_iid(pwt->menu, iid) : NULL;
	if (!plk)
		return;

	_WidgetCalcMenuItemRect(hWnd, plk, pxr);
}