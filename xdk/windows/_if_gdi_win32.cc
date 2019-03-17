/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi document

	@module	if_gdi_win.c | gdi windows implement file

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

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC

static LOGFONT lf_gdi = { 0 };

#ifdef WINCE
static long MulDiv(long a, long b, long c)
{
	return (long)((float)a * (float)b / (float)c);
}

static void DPtoLP(HDC hDC,POINT* pt,int n)
{
	int i;
	for(i = 0;i<n;i++)
	{
		pt[i].x = pt[i].x;
		pt[i].y = pt[i].y;
	}
}
#endif

static void _alphablend_rect(res_ctx_t rdc, const xbrush_t* pxb, const RECT* prt)
{
	HDC hDC = (HDC)rdc;

	HBITMAP hBmp = CreateCompatibleBitmap(hDC, prt->right - prt->left, prt->bottom - prt->top);
	HDC hComDC = CreateCompatibleDC(hDC);
	HBITMAP orgBmp = (HBITMAP)SelectObject(hComDC, hBmp);

	xcolor_t xc;

	parse_xcolor(&xc, pxb->color);

	HBRUSH hBrush = CreateSolidBrush(RGB(xc.r, xc.g, xc.b));
	FillRect(hComDC, prt, hBrush);
	DeleteObject(hBrush);

	BLENDFUNCTION bf = { 0 };

	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = (BYTE)_tstrtol(pxb->opacity);
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;// AC_SRC_ALPHA;

	AlphaBlend(hDC, prt->left, prt->top, prt->right - prt->left, prt->bottom - prt->top, hComDC, 0, 0, prt->right - prt->left, prt->bottom - prt->top, bf);

	hBmp = (HBITMAP)SelectObject(hComDC, orgBmp);
	DeleteDC(hComDC);

	DeleteObject(hBmp);
}

static void _adjust_rect(RECT* pRect, int src_width, int src_height, const tchar_t* horz_align, const tchar_t* vert_align)
{
	if (_tstrcmp(horz_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && _tstrcmp(vert_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		pRect->right = (pRect->right - pRect->left < src_width) ? pRect->right : (pRect->left + src_width);
		pRect->bottom = (pRect->bottom - pRect->top < src_height) ? pRect->bottom : (pRect->top + src_height);
	}
	else if (_tstrcmp(horz_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		pRect->left = (pRect->right - pRect->left < src_width) ? pRect->left : (pRect->right - src_width);
		pRect->bottom = (pRect->bottom - pRect->top < src_height) ? pRect->bottom : (pRect->top + src_height);
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		pRect->right = (pRect->right - pRect->left < src_width) ? pRect->right : (pRect->left + src_width);
		pRect->top = (pRect->bottom - pRect->top < src_height) ? pRect->top : (pRect->bottom - src_height);
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		pRect->left = (pRect->right - pRect->left < src_width) ? pRect->left : (pRect->right - src_width);
		pRect->top = (pRect->bottom - pRect->top < src_height) ? pRect->top : (pRect->bottom - src_height);
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		if (pRect->right - pRect->left > src_width)
		{
			pRect->left = pRect->left + (pRect->right - pRect->left - src_width) / 2;
			pRect->right = pRect->left + src_width;
		}
		if (pRect->bottom - pRect->top > src_height)
		{
			pRect->top = pRect->top + (pRect->bottom - pRect->top - src_height) / 2;
			pRect->bottom = pRect->top + src_height;
		}
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		pRect->right = (pRect->right - pRect->left < src_width) ? pRect->right : (pRect->left + src_width);
		pRect->top = (pRect->bottom - pRect->top < src_height) ? pRect->top : (pRect->top + pRect->bottom - src_height) / 2;
		pRect->bottom = (pRect->bottom - pRect->top < src_height) ? pRect->bottom : (pRect->top + pRect->bottom + src_height) / 2;
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		pRect->left = (pRect->right - pRect->left < src_width) ? pRect->left : (pRect->right - src_width);
		pRect->top = (pRect->bottom - pRect->top < src_height) ? pRect->top : (pRect->top + pRect->bottom - src_height) / 2;
		pRect->bottom = (pRect->bottom - pRect->top < src_height) ? pRect->bottom : (pRect->top + pRect->bottom + src_height) / 2;
	}
}


/************************************************************************************************/

static HPEN create_pen(const xpen_t* pxp)
{
	long ps;
	xcolor_t xc = {0};

	parse_xcolor(&xc,pxp->color);

	if (_tstrcmp(pxp->style, GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
#ifdef WINCE
		ps = PS_DASH;
#else
		ps = PS_DOT;
#endif
	else if (_tstrcmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
		ps = PS_DASH;
	else
		ps = PS_SOLID;

	return CreatePen(ps, _tstrtol(pxp->size), RGB(xc.r, xc.g, xc.b));
}

static HBRUSH create_brush(const xbrush_t* pxb)
{
	xcolor_t xc = { 0 };

	parse_xcolor(&xc, pxb->color);

	return CreateSolidBrush(RGB(xc.r, xc.g, xc.b));
}

static HFONT create_font(res_ctx_t rdc, const xfont_t* pxf)
{
	LOGFONT lf;
	
	CopyMemory((void*)&lf, (void*)&lf_gdi, sizeof(LOGFONT));

	lf.lfHeight = -MulDiv(_tstrtol(pxf->size), GetDeviceCaps(rdc, LOGPIXELSY), 72);
	lf.lfWeight = _tstrtol(pxf->weight);

	if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!_tstrnull(pxf->family))
	{
		_tstrcpy(lf.lfFaceName, pxf->family);
	}

	return CreateFontIndirect(&lf);
}

void _gdi_init(int osv)
{
	NONCLIENTMETRICS ncm = { 0 };

	ncm.cbSize = sizeof(ncm);

	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), (PVOID)&ncm, 0);
	
	CopyMemory((void*)&lf_gdi, (void*)&ncm.lfCaptionFont, sizeof(LOGFONT));
}

void _gdi_uninit(void)
{
	
}

void _gdi_draw_line(res_ctx_t rdc,const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	HDC hDC = (HDC)rdc;
	
	POINT pt[2];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(hDC,pt,2);

	HPEN hPen, orgPen;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
	LineTo(hDC, pt[1].x, pt[1].y);

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}
}

void _gdi_draw_3dline(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(hDC,pt,2);

	HPEN hPen, orgPen;

	if(!is_null_xpen(pxp))
	{
		xpen_t xp2;
		CopyMemory((void*)&xp2, (void*)pxp, sizeof(xpen_t));
		lighten_xpen(&xp2, 15);
		_tsprintf(xp2.size, _T("%d"), _tstrtol(xp2.size) + 1);

		hPen = create_pen(&xp2);
		orgPen = (HPEN)SelectObject(hDC, hPen);

		MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		LineTo(hDC, pt[1].x, pt[1].y);

		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}
	
	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}
	
	MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
	LineTo(hDC, pt[1].x, pt[1].y);

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}
}

void _gdi_draw_3drect(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	HPEN hPen, orgPen;
	POINT pt[5];

	if(!is_null_xpen(pxp))
	{
		xpen_t xp2;
		CopyMemory((void*)&xp2, (void*)pxp, sizeof(xpen_t));
		lighten_xpen(&xp2, 15);
		_tsprintf(xp2.size, _T("%d"), _tstrtol(xp2.size) + 1);

		hPen = create_pen(&xp2);
		orgPen = (HPEN)SelectObject(hDC, hPen);

		pt[0].x = rt.left; pt[0].y = rt.top;
		pt[1].x = rt.right; pt[1].y = rt.top;
		pt[2].x = rt.right; pt[2].y = rt.bottom;
		pt[3].x = rt.left; pt[3].y = rt.bottom;
		pt[4].x = rt.left; pt[4].y = rt.top;
		Polyline(hDC, pt, 5);

		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	pt[0].x = rt.left; pt[0].y = rt.top;
	pt[1].x = rt.right; pt[1].y = rt.top;
	pt[2].x = rt.right; pt[2].y = rt.bottom;
	pt[3].x = rt.left; pt[3].y = rt.bottom;
	pt[4].x = rt.left; pt[4].y = rt.top;
	Polyline(hDC, pt, 5);

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}
}

void _gdi_draw_polyline(res_ctx_t rdc,const xpen_t* pxp,const xpoint_t* ppt,int n)
{
	HDC hDC = (HDC)rdc;

	POINT* pt = (POINT*)LocalAlloc(LPTR, n * sizeof(POINT));
	
	for(int i=0;i<n;i++)
	{
		pt[i].x = ppt[i].x;
		pt[i].y = ppt[i].y;
	}

	DPtoLP(hDC, pt, n);

	HPEN hPen, orgPen;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	Polyline(hDC, pt, n);

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	LocalFree(pt);
}

void _gdi_draw_polygon(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xpoint_t* ppt,int n)
{
	HDC hDC = (HDC)rdc;

	POINT* pt = (POINT*)LocalAlloc(LPTR, n * sizeof(POINT));

	for (int i = 0; i<n; i++)
	{
		pt[i].x = ppt[i].x;
		pt[i].y = ppt[i].y;
	}

	DPtoLP(hDC, pt, n);

	HPEN hPen, orgPen;
	HBRUSH hBrush, orgBrush;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = create_brush(pxb);
		orgBrush = (HBRUSH)SelectObject(hDC, hBrush);
	}

	Polygon(hDC, pt, n);

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = (HBRUSH)SelectObject(hDC, orgBrush);
		DeleteObject(hBrush);
	}

	LocalFree(pt);
}

void _gdi_draw_bezier(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	HDC hDC = (HDC)rdc;

	POINT* pt = (POINT*)LocalAlloc(LPTR, 4 * sizeof(POINT));

	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;
	pt[2].x = ppt3->x;
	pt[2].y = ppt3->y;
	pt[3].x = ppt4->x;
	pt[3].y = ppt4->y;

	DPtoLP(hDC, pt, 4);

	HPEN hPen, orgPen;

	if (!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	PolyBezier(hDC, pt, 4);

	if (!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	LocalFree(pt);
}

void _gdi_draw_path(res_ctx_t rdc, const xpen_t* pxp, const tchar_t* str, int len)
{
	HDC hDC = (HDC)rdc;
}

void _gdi_gradient_rect(res_ctx_t rdc, const xgradi_t* pxg, const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	xcolor_t xc1, xc2;
	parse_xcolor(&xc1, pxg->brim_color);
	parse_xcolor(&xc2, pxg->core_color);

	HBITMAP hBmp = _create_gradient_bitmap(rdc, &xc1, &xc2, rt.right - rt.left, rt.bottom - rt.top, pxg->type);

	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), (void*)&bmp);

	HDC hComDC = CreateCompatibleDC(hDC);
	HBITMAP orgBmp = (HBITMAP)SelectObject(hComDC, hBmp);

	_adjust_rect(&rt, bmp.bmWidth, bmp.bmHeight, GDI_ATTR_TEXT_ALIGN_CENTER, GDI_ATTR_TEXT_ALIGN_CENTER);

	BitBlt(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hComDC, 0, 0, SRCCOPY);

	hBmp = (HBITMAP)SelectObject(hComDC, orgBmp);
	DeleteObject(hBmp);
	DeleteDC(hComDC);
}

void _gdi_alphablend_rect(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity)
{
	HDC hDC = (HDC)rdc;
	xbrush_t xb;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	_tsprintf(xb.opacity, _T("%d"), opacity);

	_alphablend_rect(rdc, &xb, &rt);
}

void _gdi_draw_rect(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	HPEN hPen, orgPen;
	HBRUSH hBrush;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = create_brush(pxb);
	}

	if(!is_null_xbrush(pxb))
	{
		FillRect(hDC, &rt, hBrush);
	}

	if(!is_null_xpen(pxp))
	{
		POINT pt[5];
		pt[0].x = rt.left; pt[0].y = rt.top;
		pt[1].x = rt.right; pt[1].y = rt.top;
		pt[2].x = rt.right; pt[2].y = rt.bottom;
		pt[3].x = rt.left; pt[3].y = rt.bottom;
		pt[4].x = rt.left; pt[4].y = rt.top;
		Polyline(hDC, pt, 5);
	}

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		DeleteObject(hBrush);
	}
}

void _gdi_draw_round(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;
	int r;

	r = (prt->w) / 10;
	if (r < 1)
		r = 1;
	else if (r > 6)
		r = 6;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	HPEN hPen, orgPen;
	HBRUSH hBrush, orgBrush;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = create_brush(pxb);
		orgBrush = (HBRUSH)SelectObject(hDC, hBrush);
	}

#ifdef WINCE
	RoundRect(hDC, rt.left, rt.top, rt.right, rt.bottom, r,r);
#else

	BeginPath(hDC);

	MoveToEx(hDC, rt.left, rt.top + r, NULL);
	LineTo(hDC, rt.left + r, rt.top);

	MoveToEx(hDC, rt.left + r, rt.top, NULL);
	LineTo(hDC, rt.right - r, rt.top);

	MoveToEx(hDC, rt.right - r, rt.top, NULL);
	LineTo(hDC, rt.right, rt.top + r);

	MoveToEx(hDC, rt.right, rt.top + r, NULL);
	LineTo(hDC, rt.right, rt.bottom - r);

	MoveToEx(hDC, rt.right, rt.bottom - r, NULL);
	LineTo(hDC, rt.right - r, rt.bottom);

	MoveToEx(hDC, rt.right - r, rt.bottom, NULL);
	LineTo(hDC, rt.left + r, rt.bottom);

	MoveToEx(hDC, rt.left + r, rt.bottom, NULL);
	LineTo(hDC, rt.left, rt.bottom - r);

	MoveToEx(hDC, rt.left, rt.bottom - r, NULL);
	LineTo(hDC, rt.left, rt.top + r);

	EndPath(hDC);

	if (pxp && pxb)
		StrokeAndFillPath(hDC);
	else if(!is_null_xpen(pxp))
		StrokePath(hDC);
	else if(!is_null_xbrush(pxb))
		FillPath(hDC);
#endif

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = (HBRUSH)SelectObject(hDC, orgBrush);
		DeleteObject(hBrush);
	}
}

void _gdi_draw_ellipse(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	HPEN hPen, orgPen;
	HBRUSH hBrush, orgBrush;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = create_brush(pxb);
		orgBrush = (HBRUSH)SelectObject(hDC, hBrush);
	}

#ifdef WINCE
	Ellipse(hDC, rt.left, rt.top, rt.right, rt.bottom);
#else
	BeginPath(hDC);

	Arc(hDC, rt.left, rt.top, rt.right, rt.bottom, rt.left, (rt.top + rt.bottom) / 2, rt.left, (rt.top + rt.bottom) / 2);

	EndPath(hDC);

	if (pxp && pxb)
		StrokeAndFillPath(hDC);
	else if(!is_null_xpen(pxp))
		StrokePath(hDC);
	else if(!is_null_xbrush(pxb))
		FillPath(hDC);
#endif

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = (HBRUSH)SelectObject(hDC, orgBrush);
		DeleteObject(hBrush);
	}

}

void _gdi_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt, double fang, double tang)
{
#ifdef WINCE
	return;
#else
	HDC hDC = (HDC)rdc;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	int x1, y1, x2, y2;
	x1 = (int)((rt.left + rt.right) / 2 + (rt.right - rt.left) / 2 * cos(fang));
	y1 = (int)((rt.top + rt.bottom) / 2 - (rt.bottom - rt.top) / 2 * sin(fang));
	x2 = (int)((rt.left + rt.right) / 2 + (rt.right - rt.left) / 2 * cos(tang));
	y2 = (int)((rt.top + rt.bottom) / 2 - (rt.bottom - rt.top) / 2 * sin(tang));

	HPEN hPen, orgPen;
	HBRUSH hBrush, orgBrush;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = create_brush(pxb);
		orgBrush = (HBRUSH)SelectObject(hDC, hBrush);
	}

	Pie(hDC, rt.left, rt.top, rt.right, rt.bottom, x1, y1, x2, y2);

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = (HBRUSH)SelectObject(hDC, orgBrush);
		DeleteObject(hBrush);
	}
#endif
}

void _gdi_draw_arrow(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt,int alen,double arc)
{
	HDC hDC = (HDC)rdc;
	double a1;
	int x_line0,y_line0,x_line1,y_line1,x_line2,y_line2;
	long x1, x2, y1, y2;
	POINT pt[4];

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC,pt,2);

	x1 = pt[0].x;
	y1 = pt[0].y;
	x2 = pt[1].x;
	y2 = pt[1].y;

	pt[0].x = x2;
	pt[0].y = y2;

	a1 = atan2((float)(y2 - y1),(float)(x2 - x1));
	x_line0 = (int)((float)x2 - (float)alen * cos(a1));
	y_line0 = (int)((float)y2 - (float)alen * sin(a1));

	x_line1 = x2 + (int)((float)(x_line0 - x2) * cos(arc) - (float)(y_line0 - y2) * sin(arc));
	y_line1 = y2 + (int)((float)(x_line0 - x2) * sin(arc) + (float)(y_line0 - y2) * cos(arc));
	pt[1].x = x_line1;
	pt[1].y = y_line1;

	x_line2 = x2 + (int)((float)(x_line0 - x2) * cos(-arc) - (float)(y_line0 - y2) * sin(-arc));
	y_line2 = y2 + (int)((float)(x_line0 - x2) * sin(-arc) + (float)(y_line0 - y2) * cos(-arc));
	pt[2].x = x_line2;
	pt[2].y = y_line2;

	pt[3].x = x2;
	pt[3].y = y2;

	HPEN hPen, orgPen;
	HBRUSH hBrush, orgBrush;

	if(!is_null_xpen(pxp))
	{
		hPen = create_pen(pxp);
		orgPen = (HPEN)SelectObject(hDC, hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = create_brush(pxb);
		orgBrush = (HBRUSH)SelectObject(hDC, hBrush);
	}

	Polygon(hDC, pt, 4);

	if(!is_null_xpen(pxp))
	{
		hPen = (HPEN)SelectObject(hDC, orgPen);
		DeleteObject(hPen);
	}

	if(!is_null_xbrush(pxb))
	{
		hBrush = (HBRUSH)SelectObject(hDC, orgBrush);
		DeleteObject(hBrush);
	}
}

void _gdi_draw_text(res_ctx_t rdc,const xfont_t* pxf,const xface_t* pxa,const xrect_t* prt,const tchar_t* txt,int len)
{
	HDC hDC = (HDC)rdc;
	
	if (_tstrnull(txt))
		return;

	if (len < 0)
		len = _tstrlen(txt);

	if (!len)
		return;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	DWORD dw = 0;
	COLORREF orgClr;
	if (pxa)
	{
		if (_tstrcmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
			dw |= DT_TOP;
		else if (_tstrcmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
			dw |= DT_BOTTOM;
		else
			dw |= DT_VCENTER;

		if (_tstrcmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
			dw |= DT_CENTER;
		else if (_tstrcmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
			dw |= DT_RIGHT;
		else
			dw |= DT_LEFT;

		if (!_tstrnull(pxa->text_wrap))
			dw |= DT_WORDBREAK;
		else
			dw |= DT_SINGLELINE;
	}
	else
	{
		dw = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	}

	HFONT hFont, orgFont;

	if (pxf)
	{
		hFont = create_font(rdc,pxf);
		orgFont = (HFONT)SelectObject(hDC, hFont);

		xcolor_t xc;
		parse_xcolor(&xc, pxf->color);
		orgClr = SetTextColor(hDC, RGB(xc.r, xc.g, xc.b));
	}

	rt.left += 1;
	rt.right -= 1;
	rt.top += 1;
	rt.bottom -= 1;
	DrawText(hDC, txt, len, &rt, dw);

	if (pxf)
	{
		hFont = (HFONT)SelectObject(hDC, orgFont);
		DeleteObject(hFont);

		SetTextColor(hDC, orgClr);
	}
}

void _gdi_text_out(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	HFONT hFont, orgFont;
	COLORREF clr, orgClr;
	xcolor_t xc;

	hFont = create_font(rdc, pxf);
	orgFont = (HFONT)SelectObject(rdc, hFont);

	if (pxf)
	{
		parse_xcolor(&xc, pxf->color);
		clr = RGB(xc.r, xc.g, xc.b);
		orgClr = SetTextColor(rdc, clr);
	}

	if (len < 0)
		len = _tstrlen(txt);

#ifdef WINCE
	ExtTextOut(rdc, ppt->x, ppt->y, 0, NULL, txt, len, NULL);
#else
	TextOut(rdc, ppt->x, ppt->y, txt, len);
#endif

	if (pxf)
	{
		SetTextColor(rdc, orgClr);
	}

	hFont = (HFONT)SelectObject(rdc, orgFont);
	DeleteObject(hFont);
}

void _gdi_draw_image(res_ctx_t rdc,res_bmp_t hBmp,const tchar_t* clr,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), (void*)&bmp);

	HDC hComDC = CreateCompatibleDC(hDC);
	HBITMAP orgBmp = (HBITMAP)SelectObject(hComDC, hBmp);

	_adjust_rect(&rt, bmp.bmWidth, bmp.bmHeight, GDI_ATTR_TEXT_ALIGN_CENTER, GDI_ATTR_TEXT_ALIGN_CENTER);

	//BitBlt(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hComDC, 0, 0, SRCAND);

	//BLENDFUNCTION bf = { 0 };
	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.SourceConstantAlpha = 128;
	//bf.AlphaFormat = 0;
	//AlphaBlend(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hComDC, 0, 0, rt.right - rt.left, rt.bottom - rt.top, bf);
	TransparentBlt(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hComDC, 0, 0, rt.right - rt.left, rt.bottom - rt.top, RGB(255,255,255));

	hBmp = (HBITMAP)SelectObject(hComDC, orgBmp);
	DeleteObject(hBmp);
	DeleteDC(hComDC);
}

void _gdi_draw_bitmap(res_ctx_t rdc, res_bmp_t hbmp, const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	BITMAP bmp;
	GetObject(hbmp, sizeof(BITMAP), (void*)&bmp);

	HDC hComDC = CreateCompatibleDC(hDC);
	HBITMAP orgBmp = (HBITMAP)SelectObject(hComDC, hbmp);

	//BitBlt(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hComDC, 0, 0, SRCAND);
	TransparentBlt(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hComDC, 0, 0, rt.right - rt.left, rt.bottom - rt.top, RGB(255, 255, 255));

	SelectObject(hComDC, orgBmp);
	DeleteDC(hComDC);
}

void _gdi_fill_region(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn)
{
	HBRUSH hBrush;

	hBrush = create_brush(pxb);

	FillRgn(rdc, rgn, hBrush);

	DeleteObject(hBrush);
}

void _gdi_exclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	ExcludeClipRect(rdc, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}

void _gdi_inclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	IntersectClipRect(rdc, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}

void _gdi_text_rect(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* prt)
{
	BOOL bRef = 0;

	if (!rdc)
	{
		bRef = 1;
		rdc = GetDC(NULL);
	}

	HDC hDC = (HDC)rdc;

	if (_tstrnull(txt))
		return;

	if (len < 0)
		len = _tstrlen(txt);

	if (!len)
		return;

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	DWORD dw = 0;
	COLORREF orgClr;
	if (pxa)
	{
		if (_tstrcmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
			dw |= DT_VCENTER;
		else if (_tstrcmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
			dw |= (DT_BOTTOM | DT_SINGLELINE);
		else
			dw |= DT_TOP;

		if (_tstrcmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
			dw |= DT_CENTER;
		else if (_tstrcmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
			dw |= DT_RIGHT;
		else
			dw |= DT_LEFT;

		if (_tstrcmp(pxa->text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK) == 0)
			dw |= DT_WORDBREAK;

		xcolor_t xc;
		parse_xcolor(&xc, pxf->color);
		orgClr = SetTextColor(hDC, RGB(xc.r, xc.g, xc.b));
	}

	HFONT hFont, orgFont;

	if (pxf)
	{
		hFont = create_font(rdc, pxf);
		orgFont = (HFONT)SelectObject(hDC, hFont);
	}

	dw |= DT_CALCRECT;
	DrawText(hDC, txt, len, &rt, dw);

	if (pxa)
	{
		SetTextColor(hDC, orgClr);
	}

	if (pxf)
	{
		hFont = (HFONT)SelectObject(hDC, orgFont);
		DeleteObject(hFont);
	}

	if (bRef)
		ReleaseDC(NULL, rdc);
}

void _gdi_text_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	BOOL bRef = 0;
	LOGFONT lf;
	HFONT hFont,orgFont;
	SIZE si;

	if (!rdc)
	{
		bRef = 1;
		rdc = GetDC(NULL);
	}

	CopyMemory((void*)&lf, (void*)&lf_gdi, sizeof(LOGFONT));

	lf.lfHeight = lf.lfHeight = -MulDiv(_tstrtol(pxf->size), GetDeviceCaps(rdc, LOGPIXELSY), 72);
	lf.lfWeight = _tstrtol(pxf->weight);

	if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!_tstrnull(pxf->family))
	{
		_tstrcpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);

	orgFont = (HFONT)SelectObject(rdc, hFont);

	if (len < 0)
		len = _tstrlen(txt);

	GetTextExtentPoint32(rdc, txt, len, &si);
	pxs->cx = si.cx;
	pxs->cy = si.cy;

	hFont = (HFONT)SelectObject(rdc, orgFont);
	DeleteObject(hFont);

	if (bRef)
		ReleaseDC(NULL, rdc);
}

void _gdi_text_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	BOOL bRef = 0;

	if (!rdc)
	{
		bRef = 1;
		rdc = GetDC(NULL);
	}

	LOGFONT lf;
	HFONT hFont, orgFont;
	TEXTMETRIC tm = { 0 };

	CopyMemory((void*)&lf, (void*)&lf_gdi, sizeof(LOGFONT));

	lf.lfHeight = lf.lfHeight = -MulDiv(_tstrtol(pxf->size), GetDeviceCaps(rdc, LOGPIXELSY), 72);
	lf.lfWeight = _tstrtol(pxf->weight);

	if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (_tstrcmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!_tstrnull(pxf->family))
	{
		_tstrcpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);
	orgFont = (HFONT)SelectObject(rdc, hFont);

	GetTextMetrics(rdc, &tm);

	hFont = (HFONT)SelectObject(rdc, orgFont);

	DeleteObject(hFont);

	if (bRef)
		ReleaseDC(NULL, rdc);

	pxs->cy = tm.tmHeight;
	pxs->cx = tm.tmMaxCharWidth;
	//pxs->cx = tm.tmAveCharWidth;
}

#endif //XDK_SUPPORT_CONTEXT_GRAPHIC

