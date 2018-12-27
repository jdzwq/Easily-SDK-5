/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdiplus document

	@module	if_gdiplus_win.c | gdiplus windows implement file

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

#if defined(XDK_SUPPORT_CONTEXT_GRAPHIC) && (defined(UNICODE) || defined(_UNICODE))

static LOGFONT lf_gdiplus = { 0 };

#ifndef ULONG_PTR
#define ULONG_PTR ULONG
#endif

#include "gdiplus\gdiplus.h"

#pragma comment(lib,"gdiplus.lib")

using namespace Gdiplus;

GdiplusStartupInput	g_input = NULL;
ULONG_PTR			g_token = NULL;


static void _adjust_rect(RECT* pRect, int src_width, int src_height, const tchar_t* horz_align, const tchar_t* vert_align)
{
	if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		pRect->right = (pRect->right - pRect->left < src_width) ? pRect->right : (pRect->left + src_width);
		pRect->bottom = (pRect->bottom - pRect->top < src_height) ? pRect->bottom : (pRect->top + src_height);
	}
	else if (_tstrcmp(horz_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0 && _tstrcmp(vert_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
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
static Pen* create_pen(const xpen_t* pxp)
{
	xcolor_t pen_color = {0};
	short sp;

	if (_tstrnull(pxp->color))
		parse_xcolor(&pen_color, GDI_ATTR_RGB_GRAY);
	else
		parse_xcolor(&pen_color,pxp->color);

	if (_tstrnull(pxp->size))
		sp = 1;
	else
		sp = _tstrtol(pxp->size);

	Pen* pp = new Pen(Color(pen_color.r,pen_color.g,pen_color.b),(REAL)sp);

	if(_tstrcmp(pxp->style,GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
		pp->SetDashStyle(DashStyleDot);
	else if(_tstrcmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
		pp->SetDashStyle(DashStyleDash);

	return pp;
}

static Brush* create_brush(const xbrush_t* pxb)
{
	xcolor_t brush_color = {0};
	short opacity;

	if (_tstrnull(pxb->color))
		parse_xcolor(&brush_color, GDI_ATTR_RGB_SOFTWHITE);
	else
		parse_xcolor(&brush_color,pxb->color);

	if (_tstrnull(pxb->opacity))
		opacity = 255;
	else
		opacity = _tstrtol(pxb->opacity);

	return new SolidBrush(Color((BYTE)opacity,brush_color.r,brush_color.g,brush_color.b));
}

static Font* create_font(const xfont_t* pxf)
{
	FontStyle fs;

	if(_tstrcmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		if (_tstrtol(pxf->weight) > 500)
			fs = FontStyleBoldItalic;
		else
			fs = FontStyleItalic;
	}else if(_tstrcmp(pxf->style,GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		fs = FontStyleUnderline;
	}else if(_tstrcmp(pxf->style,GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		fs = FontStyleStrikeout;
	}else
	{
		if (_tstrtol(pxf->weight) > 500)
			fs = FontStyleBold;
		else
			fs = FontStyleRegular;
	}

	tchar_t face[RES_LEN];

	if (_tstrnull(pxf->family))
	{
		_tstrcpy(face, lf_gdiplus.lfFaceName);
	}else
	{
		_tstrcpy(face, pxf->family);
	}

	FontFamily ff(face);
	BYTE fx = (BYTE)_tstrtol(pxf->size);

	return new Font(&ff,fx,fs,UnitPoint);
}

static StringFormat* create_face(const xface_t* pxa)
{
	StringFormat* psf = new StringFormat;

	if (_tstrcmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		psf->SetLineAlignment(StringAlignmentNear);
	}
	else if (_tstrcmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		psf->SetLineAlignment(StringAlignmentFar);
	}
	else
	{
		psf->SetLineAlignment(StringAlignmentCenter);
	}

	if (_tstrcmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		psf->SetAlignment(StringAlignmentCenter);
	}
	else if (_tstrcmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		psf->SetAlignment(StringAlignmentFar);
	}
	else
	{
		psf->SetAlignment(StringAlignmentNear);
	}

	if (_tstrnull(pxa->text_wrap))
		psf->SetFormatFlags(StringFormatFlagsNoWrap);

	return psf;
}

void _gdiplus_init(int osv)
{
	NONCLIENTMETRICS ncm = { 0 };

	ncm.cbSize = sizeof(ncm);

	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), (PVOID)&ncm, 0);

	CopyMemory((void*)&lf_gdiplus, (void*)&ncm.lfCaptionFont, sizeof(LOGFONT));

	if (!g_token)
	{
		GdiplusStartup(&g_token, &g_input, NULL);
	}
}

void _gdiplus_uninit(void)
{
	if (g_token)
	{
		GdiplusShutdown(g_token);
		g_token = NULL;
	}
}

void _gdiplus_draw_line(res_ctx_t rdc,const xpen_t* pxp, const xpoint_t*ppt1, const xpoint_t* ppt2)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(hDC,pt,2);

	Pen* pp = create_pen(pxp);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawLine(pp,pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	delete pp;
}

void _gdiplus_draw_3dline(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t*ppt1, const xpoint_t* ppt2)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(hDC,pt,2);

	xpen_t xp2;
	CopyMemory((void*)&xp2,(void*)pxp,sizeof(xpen_t));
	lighten_xpen(&xp2, -5);
	_tsprintf(xp2.size,_T("%d"),_tstrtol(xp2.size) + 1);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	Pen* pp = (Pen*)create_pen(&xp2);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawLine(pp,pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	delete pp;

	pp = (Pen*)create_pen(pxp);

	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawLine(pp,pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	delete pp;
}


void _gdiplus_draw_3drect(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	xpen_t xp2;
	CopyMemory((void*)&xp2, (void*)pxp, sizeof(xpen_t));
	lighten_xpen(&xp2, -5);
	_tsprintf(xp2.size, _T("%d"), _tstrtol(xp2.size) + 1);

	Pen* pp = (Pen*)create_pen(&xp2);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawRectangle(pp, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x - 1, pt[1].y - pt[0].y - 1));

	delete pp;

	pp = (Pen*)create_pen(pxp);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawRectangle(pp, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

	delete pp;
}

void _gdiplus_draw_polyline(res_ctx_t rdc,const xpen_t* pxp,const xpoint_t* ppt,int n)
{
	HDC hDC = (HDC)rdc;

	GraphicsPath path;	
	POINT pt[2];
	
	for(int i=0;i<n-1;i++)
	{
		pt[0].x = ppt[i].x;
		pt[0].y = ppt[i].y;
		pt[1].x = ppt[i+1].x;
		pt[1].y = ppt[i+1].y;

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	Pen* pp = create_pen(pxp);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	gh.DrawPath(pp,&path);

	delete pp;
}

void _gdiplus_draw_polygon(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xpoint_t* ppt,int n)
{
	HDC hDC = (HDC)rdc;

	GraphicsPath path;	
	POINT pt[2];
	
	for(int i=0;i<n-1;i++)
	{
		pt[0].x = ppt[i].x;
		pt[0].y = ppt[i].y;
		pt[1].x = ppt[i+1].x;
		pt[1].y = ppt[i+1].y;

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	if(n > 1)
	{
		pt[0].x = ppt[0].x;
		pt[0].y = ppt[0].y;
		pt[1].x = ppt[1].x;
		pt[1].y = ppt[1].y;

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb);
		gh.FillPath(pb, &path);

		delete pb;
	}

	if (!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);

		gh.DrawPath(pp, &path);
		delete pp;
	}
}

void _gdiplus_draw_rect(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC,pt,2);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = (Brush*)create_brush(pxb);
		gh.FillRectangle(pb, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		delete pb;
	}

	if (!is_null_xpen(pxp))
	{
		Pen* pp = (Pen*)create_pen(pxp);
		gh.SetSmoothingMode(SmoothingModeAntiAlias);
		gh.DrawRectangle(pp, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		delete pp;
	}
}

void _gdiplus_gradient_rect(res_ctx_t rdc, const xgradi_t* pxg, const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	xcolor_t xc1, xc2;
	parse_xcolor(&xc1, pxg->brim_color);
	parse_xcolor(&xc2, pxg->core_color);

	HBITMAP hBmp = _create_gradient_bitmap(rdc, &xc1, &xc2, pt[1].x - pt[0].x, pt[1].y - pt[0].y, pxg->type);
	Bitmap* pbm = new Bitmap(hBmp, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
	DeleteObject(hBmp);

	Brush* pb = new TextureBrush(pbm, Rect(0, 0, pt[1].x - pt[0].x, pt[1].y - pt[0].y));
	delete pbm;

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	gh.FillRectangle(pb, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

	delete pb;
}

void _gdiplus_alpha_rect(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity)
{
	HDC hDC = (HDC)rdc;
	xbrush_t xb;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	_tsprintf(xb.opacity, _T("%d"), opacity);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	Brush* pb = (Brush*)create_brush(&xb);
	gh.FillRectangle(pb, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

	delete pb;
}

void _gdiplus_draw_round(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;
	int r;

	r = (prt->w) / 10;
	if (r < 1)
		r = 1;
	else if (r > 10)
		r = 10;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC,pt,2);

	Rect rf(pt[0].x,pt[0].y,pt[1].x - pt[0].x,pt[1].y - pt[0].y);

	GraphicsPath path;

	//path.AddLine(rf.X,rf.Y + r,rf.X + r,rf.Y);
	path.AddArc(rf.X, rf.Y, 2 * r, 2 * r, 180, 90);
	path.AddLine(rf.X + r,rf.Y,rf.X + rf.Width - r,rf.Y);
	//path.AddLine(rf.X + rf.Width - r,rf.Y,rf.X + rf.Width,rf.Y + r);
	path.AddArc(rf.X + rf.Width - 2 * r, rf.Y, 2 * r, 2 * r, 270, 90);
	path.AddLine(rf.X + rf.Width,rf.Y + r,rf.X + rf.Width,rf.Y + rf.Height - r);
	//path.AddLine(rf.X + rf.Width,rf.Y + rf.Height - r,rf.X + rf.Width - r,rf.Y + rf.Height);
	path.AddArc(rf.X + rf.Width - 2 * r, rf.Y + rf.Height - 2 * r, 2 * r, 2 * r, 0, 90);
	path.AddLine(rf.X + rf.Width - r,rf.Y + rf.Height,rf.X + r,rf.Y + rf.Height);
	//path.AddLine(rf.X,rf.Y + rf.Height - r,rf.X,rf.Y + r);
	path.AddArc(rf.X, rf.Y + rf.Height - 2 * r, 2 * r, 2 * r, 90, 90);
	path.AddLine(rf.X, rf.Y + rf.Height - r, rf.X, rf.Y + r);
	
	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeHighQuality);

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb);
		gh.FillPath(pb, &path);
		delete pb;
	}

	if (!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);
		gh.DrawPath(pp, &path);

		delete pp;
	}
}

void _gdiplus_draw_ellipse(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC,pt,2);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	if(!is_null_xbrush(pxb))
	{
		Brush* pb = (Brush*)create_brush(pxb);
		gh.FillEllipse(pb, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		delete pb;
	}

	if(!is_null_xpen(pxp))
	{
		Pen* pp = (Pen*)create_pen(pxp);
		gh.SetSmoothingMode(SmoothingModeHighQuality);
		gh.DrawEllipse(pp, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		delete pp;
	}
}

void _gdiplus_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt, double fang, double tang)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	Rect rf(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	if(!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb);

		gh.FillPie(pb, rf, -(float)(fang / (2 * XPI) * 360), -(float)((tang - fang) / (2 * XPI) * 360));

		delete pb;
	}

	if(!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);

		gh.SetSmoothingMode(SmoothingModeHighQuality);
		gh.SetCompositingQuality(CompositingQualityGammaCorrected);
		gh.DrawPie(pp, rf, -(float)(fang / (2 * XPI) * 360), -(float)((tang - fang) / (2 * XPI) * 360));

		delete pp;
	}
}

void _gdiplus_draw_arc(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt, double fang, double tang)
{
	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	Rect rf(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	if (!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);

		gh.SetSmoothingMode(SmoothingModeHighQuality);
		gh.SetCompositingQuality(CompositingQualityGammaCorrected);
		gh.DrawArc(pp, rf, -(float)(fang / (2 * XPI) * 360), -(float)((tang - fang) / (2 * XPI) * 360));

		delete pp;
	}
}

void _gdiplus_draw_arrow(res_ctx_t rdc,const xpen_t* pxp,const xbrush_t* pxb,const xrect_t* prt,int alen,double arc)
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

	GraphicsPath path;

	path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	path.AddLine(pt[1].x,pt[1].y,pt[2].x,pt[2].y);
	path.AddLine(pt[2].x,pt[2].y,pt[3].x,pt[3].y);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	if(!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb);
		gh.FillPath(pb, &path);

		delete pb;
	}

	if(!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);
		gh.SetSmoothingMode(SmoothingModeAntiAlias);
		gh.DrawPath(pp, &path);

		delete pp;
	}
}

void _gdiplus_draw_text(res_ctx_t rdc,const xfont_t* pxf,const xface_t* pxa,const xrect_t* prt,const tchar_t* txt,int len)
{
	HDC hDC = (HDC)rdc;

	Font* pf = create_font(pxf);

	StringFormat* ps = create_face(pxa);

	xcolor_t text_color = {0};
	parse_xcolor(&text_color,pxf->color);

	Brush* pb = new SolidBrush(Color(text_color.r,text_color.g,text_color.b));

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC,pt,2);

	RectF rf((REAL)pt[0].x,(REAL)pt[0].y,(REAL)(pt[1].x - pt[0].x),(REAL)(pt[1].y - pt[0].y));

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	if (len < 0 && txt)
		len = _tstrlen(txt);

	gh.DrawString(txt,len,pf,rf,ps,pb);

	delete pb;
	delete pf;
	delete ps;
}

void _gdiplus_text_out(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	HFONT hFont, orgFont;
	COLORREF clr, orgClr;
	long fs;
	LOGFONT lf;
	xcolor_t xc;

	CopyMemory((void*)&lf, (void*)&lf_gdiplus, sizeof(LOGFONT));

	fs = _tstrtol(pxf->size);

	parse_xcolor(&xc, pxf->color);

	lf.lfHeight = -MulDiv(fs, GetDeviceCaps(rdc, LOGPIXELSY), 72);
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

	clr = RGB(xc.r, xc.g, xc.b);
	orgClr = SetTextColor(rdc, clr);

	if (len < 0 && txt)
		len = _tstrlen(txt);

	TextOut(rdc, ppt->x, ppt->y, txt, len);

	SetTextColor(rdc, orgClr);
	hFont = (HFONT)SelectObject(rdc, orgFont);
	DeleteObject(hFont);
}

void _gdiplus_draw_image(res_ctx_t rdc,res_bmp_t bmp,const tchar_t* clr,const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	Image* pi = new Bitmap((HBITMAP)bmp, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
	if (!pi)
		return;

	int srcw = pi->GetWidth();
	int srch = pi->GetHeight();

	ImageAttributes iab;

	if (!_tstrnull(clr))
	{
		xcolor_t xc_high, xc_low;

		parse_xcolor(&xc_high, GDI_ATTR_RGB_WHITE);

		parse_xcolor(&xc_low, clr);

		iab.SetColorKey(Color(xc_low.r, xc_low.g, xc_low.b), Color(xc_high.r, xc_high.g, xc_high.b));
	}

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	_adjust_rect(&rt, srcw, srch, GDI_ATTR_TEXT_ALIGN_CENTER, GDI_ATTR_TEXT_ALIGN_CENTER);

	DPtoLP(hDC,(LPPOINT)&rt,2);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	gh.DrawImage(pi,Rect(rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top),0,0,srcw,srch,UnitPixel,&iab);

	delete pi;
}

void _gdiplus_draw_bitmap(res_ctx_t rdc, res_bmp_t bmp, const xrect_t* prt)
{
	HDC hDC = (HDC)rdc;

	Image* pi = new Bitmap((HBITMAP)bmp, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
	if (!pi)
		return;

	int srcw = pi->GetWidth();
	int srch = pi->GetHeight();

	ImageAttributes iab;
	iab.SetColorKey(Color(250, 250, 250), Color(255, 255, 255));

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	//_adjust_rect(&rt, srcw, srch, GDI_ATTR_TEXT_ALIGN_CENTER, GDI_ATTR_TEXT_ALIGN_CENTER);

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	//gh.DrawImage(pi, Rect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top), 0, 0, srcw, srch, UnitPixel, &iab);
	
	gh.DrawImage(pi, rt.left, rt.top, 0, 0, srcw, srch, UnitPixel);

	delete pi;
}

/*void _gdiplus_draw_ico(res_ctx_t rdc, res_ico_t icon, const xrect_t* prt)
{
XDC_ASSERT(rdc != NULL);

	HDC hDC = (HDC)rdc;

	Image* pi = new Bitmap((HICON)icon);
	if (!pi)
		return;

	int srcw = pi->GetWidth();
	int srch = pi->GetHeight();

	ImageAttributes iab;
	iab.SetColorKey(Color(250, 250, 250), Color(255, 255, 255));

	RECT rt;
	rt.left = prt->x;
	rt.top = prt->y;
	rt.right = prt->x + prt->w;
	rt.bottom = prt->y + prt->h;

	_adjust_rect(&rt, srcw, srch, GDI_ATTR_TEXT_ALIGN_CENTER, GDI_ATTR_TEXT_ALIGN_CENTER);

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	gh.DrawImage(pi, Rect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top), 0, 0, srcw, srch, UnitPixel, &iab);

	delete pi;
}*/

void _gdiplus_exclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	ExcludeClipRect(rdc, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}

void _gdiplus_inclip_rect(res_ctx_t rdc, const xrect_t* pxr)
{
	IntersectClipRect(rdc, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}

void _gdiplus_fill_region(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn)
{
	Region gn(rgn);

	Brush* pb = create_brush(pxb);

	Graphics gh(rdc);
	gh.SetPageUnit(UnitPixel);

	gh.FillRegion(pb, &gn);

	delete pb;
}

void _gdiplus_text_rect(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr)
{
	BOOL bRef = 0;

	if (!rdc)
	{
		bRef = 1;
		rdc = GetDC(NULL);
	}

	HDC hDC = (HDC)rdc;

	POINT pt[2];
	pt[0].x = pxr->x;
	pt[0].y = pxr->y;
	pt[1].x = pxr->x + pxr->w;
	pt[1].y = pxr->y + pxr->h;

	DPtoLP(hDC, pt, 2);

	RectF rf((REAL)pt[0].x, (REAL)pt[0].y, (REAL)(pt[1].x - pt[0].x), (REAL)(pt[1].y - pt[0].y));

	Graphics gh(hDC);
	gh.SetPageUnit(UnitPixel);

	Font* pf = create_font(pxf);
	StringFormat* ps = create_face(pxa);

	RectF rfOut;
	gh.MeasureString((wchar_t*)txt, len, pf, rf, ps, &rfOut);

	/*FontFamily ff;
	GraphicsPath path;

	pf->GetFamily(&ff);

	path.AddString(txt, len, &ff, pf->GetStyle(), pf->GetSize(), PointF(0, 0), ps);

	path.GetBounds(&rfOut);
	*/

	pt[0].x = (long)(rfOut.GetLeft());
	pt[0].y = (long)(rfOut.GetTop());
	pt[1].x = (long)(rfOut.GetRight());
	pt[1].y = (long)(rfOut.GetBottom());

	LPtoDP(hDC, pt, 2);

	pxr->x = pt[0].x;
	pxr->y = pt[0].y;
	pxr->w = pt[1].x - pt[0].x;
	pxr->h = pt[1].y - pt[0].y;

	delete pf;
	delete ps;

	if (bRef)
		ReleaseDC(NULL, rdc);
}

void _gdiplus_text_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	BOOL bRef = 0;

	if (!rdc)
	{
		bRef = 1;
		rdc = GetDC(NULL);
	}

	LOGFONT lf;
	HFONT hFont, orgFont;
	SIZE si;
	long fs;

	CopyMemory((void*)&lf, (void*)&lf_gdiplus, sizeof(LOGFONT));

	fs = _tstrtol(pxf->size);

	lf.lfHeight = -MulDiv(fs, GetDeviceCaps(rdc, LOGPIXELSY), 72);
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
		len = (txt)? _tstrlen(txt) : 0;

	GetTextExtentPoint32(rdc, txt, len, &si);

	hFont = (HFONT)SelectObject(rdc, orgFont);
	DeleteObject(hFont);

	if (bRef)
		ReleaseDC(NULL, rdc);

	pxs->cx = si.cx;
	pxs->cy = si.cy;
}

void _gdiplus_text_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	BOOL bRef = 0;

	if (!rdc)
	{
		bRef = 1;
		rdc = GetDC(NULL);
	}

	LOGFONT lf;
	HFONT hFont, orgFont;
	long fs;
	TEXTMETRIC tm = { 0 };

	CopyMemory((void*)&lf, (void*)&lf_gdiplus, sizeof(LOGFONT));

	fs = _tstrtol(pxf->size);

	lf.lfHeight = -MulDiv(fs, GetDeviceCaps(rdc, LOGPIXELSY), 72);
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

	//pxs->cx = tm.tmAveCharWidth;
	pxs->cy = tm.tmHeight;
	pxs->cx = tm.tmMaxCharWidth;
}

#endif //XDK_SUPPORT_CONTEXT_GRAPHICPLUS
