/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdiplus document

	@module	if_gdiplus_win.c | windows implement file

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

#include "xduiml.h"
#include "xduutil.h"

#if defined(XDU_SUPPORT_CONTEXT_GDIPLUS)

static LOGFONT lf_gdiplus = { 0 };

#ifndef ULONG_PTR
#define ULONG_PTR ULONG
#endif

#include <gdiplus.h>

#pragma comment(lib,"gdiplus.lib")

using namespace Gdiplus;

GdiplusStartupInput	g_input = NULL;
ULONG_PTR			g_token = NULL;


static void _adjust_rect(RECT* pRect, int src_width, int src_height, const tchar_t* horz_align, const tchar_t* vert_align)
{
	xrect_t xr;

	xr.x = pRect->left;
	xr.y = pRect->top;
	xr.w = pRect->right - pRect->left;
	xr.h = pRect->bottom - pRect->top;

	pt_adjust_rect(&xr, src_width, src_height, horz_align, vert_align);

	pRect->left = xr.x;
	pRect->top = xr.y;
	pRect->right = xr.x + xr.w;
	pRect->bottom = xr.y + xr.h;
}


static void _calc_point(const POINT* pt, int r, double a, POINT* pp)
{
	pp->x = pt->x + (int)((float)r * cos(a));
	pp->y = pt->y + (int)((float)r * sin(a));
}

static void _calc_circle(int sflag, int lflag, const POINT* ppt1, const POINT* ppt2, int rr, POINT* ppt, int* arcf, int* arct)
{
	POINT pt;
	double len, arc1, arc2, arc;
	int n;

	if (lflag)
	{
		pt.x = ppt1->x - ppt2->x;
		pt.y = ppt1->y - ppt2->y;
	}
	else
	{
		pt.x = ppt2->x - ppt1->x;
		pt.y = ppt2->y - ppt1->y;
	}

	if (sflag)
	{
		if (pt.x <= 0 && pt.y >= 0)
			n = 1;
		else if (pt.x <= 0 && pt.y <= 0)
			n = 2;
		else if (pt.x >= 0 && pt.y <= 0)
			n = 3;
		else if (pt.x >= 0 && pt.y >= 0)
			n = 4;
	}
	else
	{

		if (pt.x <= 0 && pt.y <= 0)
			n = 1;
		else if (pt.x <= 0 && pt.y >= 0)
			n = 2;
		else if (pt.x >= 0 && pt.y >= 0)
			n = 3;
		else if (pt.x >= 0 && pt.y <= 0)
			n = 4;
	}

	if (pt.x < 0)
		pt.x = 0 - pt.x;
	if (pt.y < 0)
		pt.y = 0 - pt.y;

	len = sqrt(pow((double)(pt.x), 2) + pow((double)(pt.y), 2)) / 2;
	
	switch (n)
	{
	case 1:
		if (sflag)
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.x / (double)pt.y);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * sin(arc));
			pt.y = (int)((float)rr * cos(arc));

			ppt->x = -pt.x + ppt1->x;
			ppt->y = -pt.y + ppt1->y;
			*arcf = (int)((XPI / 2 - arc) * ((double)180 / XPI));
			*arct = (int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		else
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.x / (double)pt.y);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * sin(arc));
			pt.y = (int)((float)rr * cos(arc));

			ppt->x = -pt.x + ppt1->x;
			ppt->y = pt.y + ppt1->y;
			*arcf = (int)((3 * XPI / 2 + arc) * ((double)180 / XPI));
			*arct = -(int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		break;
	case 2:
		if (sflag)
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.y / (double)pt.x);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * cos(arc));
			pt.y = (int)((float)rr * sin(arc));

			ppt->x = pt.x + ppt1->x;
			ppt->y = -pt.y + ppt1->y;
			*arcf = (int)((XPI - arc) * ((double)180 / XPI));
			*arct = (int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		else
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.y / (double)pt.x);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * cos(arc));
			pt.y = (int)((float)rr * sin(arc));

			ppt->x = pt.x + ppt1->x;
			ppt->y = pt.y + ppt1->y;
			*arcf = (int)((2 * XPI - arc) * ((double)180 / XPI));
			*arct = -(int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		break;
	case 3:
		if (sflag)
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.x / (double)pt.y);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * sin(arc));
			pt.y = (int)((float)rr * cos(arc));

			ppt->x = pt.x + ppt1->x;
			ppt->y = pt.y + ppt1->y;
			*arcf = (int)((XPI * 3 / 2 - arc) * ((double)180 / XPI));
			*arct = (int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		else
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.x / (double)pt.y);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * sin(arc));
			pt.y = (int)((float)rr * cos(arc));

			ppt->x = pt.x + ppt1->x;
			ppt->y = -pt.y + ppt1->y;
			*arcf = (int)((3 * XPI / 2 - arc) * ((double)180 / XPI));
			*arct = -(int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		break;
	case 4:
		if (sflag)
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.y / (double)pt.x);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * cos(arc));
			pt.y = (int)((float)rr * sin(arc));

			ppt->x = -pt.x + ppt1->x;
			ppt->y = pt.y + ppt1->y;
			*arcf = (int)((2 * XPI - arc) * ((double)180 / XPI));
			*arct = (int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		else
		{
			arc1 = acos(len / rr);
			arc2 = atan((double)pt.y / (double)pt.x);
			arc = XPI - (arc1 + arc2);

			pt.x = (int)((float)rr * cos(arc));
			pt.y = (int)((float)rr * sin(arc));

			ppt->x = -pt.x + ppt1->x;
			ppt->y = -pt.y + ppt1->y;
			*arcf = (int)((arc)* ((double)180 / XPI));
			*arct = -(int)((XPI - (arc1 * 2)) * ((double)180 / XPI));
		}
		break;
	}

	if (lflag)
	{
		if (sflag)
		{
			*arcf = *arcf + *arct;
			if (*arcf >= 360)
				*arcf = (*arcf - 360);

			*arct = (360 - *arct);
		}
		else
		{
			*arcf = *arcf + *arct;
			if (*arcf < 0)
				*arcf = (360 + *arcf);

			*arct = -(360 + *arct);
		}

		ppt->x -= (ppt1->x - ppt2->x);
		ppt->y -= (ppt1->y - ppt2->y);
	}
}

/************************************************************************************************/

static Pen* create_pen(const xpen_t* pxp)
{
	xcolor_t pen_color = {0};
	short sp;

	if (is_null(pxp->color))
		parse_xcolor(&pen_color, GDI_ATTR_RGB_GRAY);
	else
		parse_xcolor(&pen_color,pxp->color);

	if (is_null(pxp->size))
		sp = 1;
	else
		sp = xstol(pxp->size);

	Pen* pp = new Pen(Color(pen_color.r,pen_color.g,pen_color.b),(REAL)sp);

	if(xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DOTTED) == 0)
		pp->SetDashStyle(DashStyleDot);
	else if(xscmp(pxp->style,GDI_ATTR_STROKE_STYLE_DASHED) == 0)
		pp->SetDashStyle(DashStyleDash);

	return pp;
}

static Brush* create_brush(const xbrush_t* pxb, const xrect_t* pxr, GraphicsPath* pgp)
{
	xcolor_t brush_color = {0};
	xcolor_t linear_color = { 0 };
	short opacity;

	if (is_null(pxb->color))
		parse_xcolor(&brush_color, GDI_ATTR_RGB_SOFTWHITE);
	else
		parse_xcolor(&brush_color,pxb->color);

	if (is_null(pxb->opacity))
		opacity = 255;
	else
		opacity = xstol(pxb->opacity);

	if (xscmp(pxb->style, GDI_ATTR_FILL_STYLE_GRADIENT) == 0)
	{
		if (is_null(pxb->linear))
		{
			parse_xcolor(&linear_color, pxb->color);
			lighten_xcolor(&linear_color, 20);
		}
		else
		{
			parse_xcolor(&linear_color, pxb->linear);
		}

		if (pgp)
		{
			PathGradientBrush* pb = new PathGradientBrush(pgp);
			pb->SetCenterColor(Color(255, brush_color.r, brush_color.g, brush_color.b));
			Color clr(Color(255, linear_color.r, linear_color.g, linear_color.b));
			int n = 1;
			pb->SetSurroundColors(&clr, &n);

			return (Brush*)pb;
		}
		else if (pxr)
		{
			if (xscmp(pxb->gradient, GDI_ATTR_GRADIENT_HORZ) == 0)
				return 	new LinearGradientBrush(Rect(pxr->x, pxr->y, pxr->w, pxr->h), Color(255, brush_color.r, brush_color.g, brush_color.b), Color(255, linear_color.r, linear_color.g, linear_color.b), LinearGradientModeHorizontal);
			else if (xscmp(pxb->gradient, GDI_ATTR_GRADIENT_VERT) == 0)
				return 	new LinearGradientBrush(Rect(pxr->x, pxr->y, pxr->w, pxr->h), Color(255, brush_color.r, brush_color.g, brush_color.b), Color(255, linear_color.r, linear_color.g, linear_color.b), LinearGradientModeVertical);
			else
			{
				GraphicsPath gp;
				gp.AddRectangle(Rect(pxr->x, pxr->y, pxr->w, pxr->h));

				PathGradientBrush* pb = new PathGradientBrush(&gp);
				pb->SetCenterColor(Color(255, brush_color.r, brush_color.g, brush_color.b));
				Color clr(Color(255, linear_color.r, linear_color.g, linear_color.b));
				int n = 1;
				pb->SetSurroundColors(&clr, &n);

				return (Brush*)pb;
			}
		}
	}
	else if (xscmp(pxb->style, GDI_ATTR_FILL_STYLE_HATCH) == 0)
	{
		return new HatchBrush(HatchStyleCross, Color((BYTE)opacity, brush_color.r, brush_color.g, brush_color.b), Color(255, linear_color.r, linear_color.g, linear_color.b));
	}
	else
	{
		return new SolidBrush(Color((BYTE)opacity, brush_color.r, brush_color.g, brush_color.b));
	}

	return NULL;
}

static Font* create_font(const xfont_t* pxf)
{
	FontStyle fs;

	if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		if (xstol(pxf->weight) > 500)
			fs = FontStyleBoldItalic;
		else
			fs = FontStyleItalic;
	}else if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		fs = FontStyleUnderline;
	}else if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		fs = FontStyleStrikeout;
	}else
	{
		if (xstol(pxf->weight) > 500)
			fs = FontStyleBold;
		else
			fs = FontStyleRegular;
	}

	tchar_t face[32];

	if (is_null(pxf->family))
	{
		xscpy(face, lf_gdiplus.lfFaceName);
	}else
	{
		xscpy(face, pxf->family);
	}

	FontFamily ff(face);
	BYTE fx = (BYTE)xstol(pxf->size);

	return new Font(&ff,fx,fs,UnitPoint);
}

static StringFormat* create_face(const xface_t* pxa)
{
	StringFormat* psf = new StringFormat;

	if (xscmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		psf->SetLineAlignment(StringAlignmentNear);
	}
	else if (xscmp(pxa->line_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		psf->SetLineAlignment(StringAlignmentFar);
	}
	else
	{
		psf->SetLineAlignment(StringAlignmentCenter);
	}

	if (xscmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		psf->SetAlignment(StringAlignmentCenter);
	}
	else if (xscmp(pxa->text_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		psf->SetAlignment(StringAlignmentFar);
	}
	else
	{
		psf->SetAlignment(StringAlignmentNear);
	}

	if (is_null(pxa->text_wrap))
		psf->SetFormatFlags(StringFormatFlagsNoWrap);

	return psf;
}

static GraphicsPath* create_path(HDC hDC, const tchar_t* aa, const xpoint_t* pa)
{
	POINT pt_m = { 0 };
	POINT pt_p = { 0 };
	POINT pt_i = { 0 };
	POINT pt[4] = { 0 };
	int rx, ry;
	int sflag, lflag;
	RECT rt;
	int arcf, arct;
	POINT pk = { 0 };
	int n = 0;

	if (!aa)
		return NULL;

	GraphicsPath* path = new GraphicsPath;

	while (*aa)
	{
		if (*aa == _T('M') || *aa == _T('m'))
		{
			pt_m.x = pa[0].x;
			pt_m.y = pa[0].y;

			pt_p.x = pt_m.x;
			pt_p.y = pt_m.y;

			n = 1;
		}
		else if (*aa == _T('L'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pa[0].x;
			pt[1].y = pa[0].y;

			pt_p.x = pt[1].x;
			pt_p.y = pt[1].y;
			pt_i.x = 2 * pt[1].x - pt[0].x;
			pt_i.y = 2 * pt[1].y - pt[0].y;

			DPtoLP(hDC, pt, 2);
			path->AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);
			n = 1;
		}
		else if (*aa == _T('l'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_p.x + pa[0].x;
			pt[1].y = pt_p.y + pa[0].y;

			pt_p.x = pt[1].x;
			pt_p.y = pt[1].y;
			pt_i.x = 2 * pt[1].x - pt[0].x;
			pt_i.y = 2 * pt[1].y - pt[0].y;

			DPtoLP(hDC, pt, 2);
			path->AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);
			n = 1;
		}
		else if (*aa == _T('Q'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pa[0].x;
			pt[1].y = pa[0].y;
			pt[2].x = pa[1].x;
			pt[2].y = pa[1].y;

			pt_p.x = pt[2].x;
			pt_p.y = pt[2].y;
			pt_i.x = 2 * pt[2].x - pt[1].x;
			pt_i.y = 2 * pt[2].y - pt[1].y;

			DPtoLP(hDC, pt, 3);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
			n = 2;
		}
		else if (*aa == _T('q'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_m.x + pa[0].x;
			pt[1].y = pt_m.y + pa[0].y;
			pt[2].x = pt_m.x + pa[1].x;
			pt[2].y = pt_m.y + pa[1].y;

			pt_p.x = pt[2].x;
			pt_p.y = pt[2].y;
			pt_i.x = 2 * pt[2].x - pt[1].x;
			pt_i.y = 2 * pt[2].y - pt[1].y;

			DPtoLP(hDC, pt, 3);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
			n = 2;
		}
		else if (*aa == _T('T'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_i.x;
			pt[1].y = pt_i.y;
			pt[2].x = pa[0].x;
			pt[2].y = pa[0].y;

			pt_p.x = pt[2].x;
			pt_p.y = pt[2].y;
			pt_i.x = 2 * pt[2].x - pt[1].x;
			pt_i.y = 2 * pt[2].y - pt[1].y;

			DPtoLP(hDC, pt, 3);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
			n = 1;
		}
		else if (*aa == _T('t'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_i.x;
			pt[1].y = pt_i.y;
			pt[2].x = pt_p.x + pa[0].x;
			pt[2].y = pt_p.y + pa[0].y;

			pt_p.x = pt[2].x;
			pt_p.y = pt[2].y;
			pt_i.x = 2 * pt[2].x - pt[1].x;
			pt_i.y = 2 * pt[2].y - pt[1].y;

			DPtoLP(hDC, pt, 3);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
			n = 1;
		}
		else if (*aa == _T('C'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pa[0].x;
			pt[1].y = pa[0].y;
			pt[2].x = pa[1].x;
			pt[2].y = pa[1].y;
			pt[3].x = pa[2].x;
			pt[3].y = pa[2].y;

			pt_p.x = pt[3].x;
			pt_p.y = pt[3].y;
			pt_i.x = 2 * pt[3].x - pt[2].x;
			pt_i.y = 2 * pt[3].y - pt[2].y;

			DPtoLP(hDC, pt, 4);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);
			n = 3;
		}
		else if (*aa == _T('c'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_p.x + pa[0].x;
			pt[1].y = pt_p.y + pa[0].y;
			pt[2].x = pt_p.x + pa[1].x;
			pt[2].y = pt_p.y + pa[1].y;
			pt[3].x = pt_p.x + pa[2].x;
			pt[3].y = pt_p.y + pa[2].y;

			pt_p.x = pt[3].x;
			pt_p.y = pt[3].y;
			pt_i.x = 2 * pt[3].x - pt[2].x;
			pt_i.y = 2 * pt[3].y - pt[2].y;

			DPtoLP(hDC, pt, 4);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);
			n = 3;
		}
		else if (*aa == _T('S'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_i.x;
			pt[1].y = pt_i.y;
			pt[2].x = pa[0].x;
			pt[2].y = pa[0].y;
			pt[3].x = pa[1].x;
			pt[3].y = pa[1].y;

			pt_p.x = pt[3].x;
			pt_p.y = pt[3].y;
			pt_i.x = 2 * pt[2].x - pt[1].x;
			pt_i.y = 2 * pt[2].y - pt[1].y;

			DPtoLP(hDC, pt, 4);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);
			n = 2;
		}
		else if (*aa == _T('s'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_i.x;
			pt[1].y = pt_i.y;
			pt[2].x = pt_p.x + pa[0].x;
			pt[2].y = pt_p.y + pa[0].y;
			pt[3].x = pt_p.x + pa[1].x;
			pt[3].y = pt_p.y + pa[1].y;

			pt_p.x = pt[2].x;
			pt_p.y = pt[2].y;
			pt_i.x = 2 * pt[2].x - pt[1].x;
			pt_i.y = 2 * pt[2].y - pt[1].y;

			DPtoLP(hDC, pt, 4);
			path->AddBezier(pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);
			n = 2;
		}
		else if (*aa == _T('A'))
		{
			sflag = pa[0].x;
			lflag = pa[0].y;
			rx = pa[1].x;
			ry = pa[1].y;
			
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pa[2].x;
			pt[1].y = pa[2].y;

			pt_p.x = pt[1].x;
			pt_p.y = pt[1].y;
			pt_i.x = 2 * pt[1].x - pt[0].x;
			pt_i.y = 2 * pt[1].y - pt[0].y;

			_calc_circle(sflag, lflag, &pt[0], &pt[1], rx, &pk, &arcf, &arct);
			
			rt.left = pk.x - rx;
			rt.right = pk.x + rx;
			rt.top = pk.y - ry;
			rt.bottom = pk.y + ry;

			/*if (sflag)
			{
				if (pt[0].x <= pt[1].x && pt[0].y <= pt[1].y)
				{
					rt.left = pt[0].x - rx;
					rt.right = pt[1].x;
					rt.top = pt[0].y;
					rt.bottom = pt[1].y + ry;

					arcf = 270;
					arct = 90;
				}
				else if (pt[0].x <= pt[1].x && pt[0].y > pt[1].y)
				{
					rt.left = pt[0].x;
					rt.right = pt[1].x + rx;
					rt.top = pt[1].y;
					rt.bottom = pt[0].y + ry;

					arcf = 180;
					arct = 90;
				}
				else if (pt[0].x > pt[1].x && pt[0].y > pt[1].y)
				{
					rt.left = pt[1].x;
					rt.right = pt[0].x + rx;
					rt.top = pt[1].y - ry;
					rt.bottom = pt[0].y;

					arcf = 90;
					arct = 90;
				}
				else if (pt[0].x > pt[1].x && pt[0].y <= pt[1].y)
				{
					rt.left = pt[1].x - rx;
					rt.right = pt[0].x;
					rt.top = pt[0].y - ry;
					rt.bottom = pt[1].y;

					arcf = 0;
					arct = 90;
				}
			}
			else
			{
				if (pt[0].x > pt[1].x && pt[0].y > pt[1].y)
				{
					rt.left = pt[1].x - rx;
					rt.right = pt[0].x;
					rt.top = pt[1].y;
					rt.bottom = pt[0].y + ry;

					arcf = 270;
					arct = 90;
				}
				else if (pt[0].x > pt[1].x && pt[0].y < pt[1].y)
				{
					rt.left = pt[1].x;
					rt.right = pt[0].x + rx;
					rt.top = pt[0].y;
					rt.bottom = pt[1].y + ry;

					arcf = 180;
					arct = 90;
				}
				else if (pt[0].x < pt[1].x && pt[0].y < pt[1].y)
				{
					rt.left = pt[0].x;
					rt.right = pt[1].x + rx;
					rt.top = pt[0].y - ry;
					rt.bottom = pt[1].y;

					arcf = 90;
					arct = 90;
				}
				else if (pt[0].x < pt[1].x && pt[0].y > pt[1].y)
				{
					rt.left = pt[0].x - rx;
					rt.right = pt[1].x;
					rt.top = pt[1].y - ry;
					rt.bottom = pt[0].y;

					arcf = 0;
					arct = 90;
				}
			}*/

			DPtoLP(hDC, (LPPOINT)&rt, 2);
			path->AddArc(rt.left, rt.top, 2 * rx, 2 * ry, arcf, arct);
			n = 3;
		}
		else if (*aa == _T('Z') || *aa == _T('z'))
		{
			pt[0].x = pt_p.x;
			pt[0].y = pt_p.y;
			pt[1].x = pt_m.x;
			pt[1].y = pt_m.y;

			DPtoLP(hDC, pt, 2);
			path->AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);

			break;
		}

		aa++;
		pa += n;
	}

	return path;
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

void _gdiplus_draw_line(visual_t rdc,const xpen_t* pxp, const xpoint_t*ppt1, const xpoint_t* ppt2)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	POINT pt[2];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;

	DPtoLP(hDC,pt,2);

	Pen* pp = create_pen(pxp);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.SetSmoothingMode(SmoothingModeAntiAlias);
		gh.DrawLine(&pen, pt[0].x + pxp->adorn.feed, pt[0].y + pxp->adorn.feed, pt[1].x + pxp->adorn.feed, pt[1].y + pxp->adorn.feed);
	}

	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawLine(pp,pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	delete pp;
}


void _gdiplus_draw_arc(visual_t rdc, const xpen_t* pxp, const xpoint_t * ppt1, const xpoint_t* ppt2, const xsize_t* pxs, bool_t sflag, bool_t lflag)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	POINT pt[4] = { 0 };

	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;
	pt[2].x = pxs->w;
	pt[2].y = pxs->h;

	DPtoLP(hDC, pt, 3);

	int fang = 0;
	int tang = 0;

	_calc_circle(sflag, lflag, &pt[0], &pt[1], pt[2].x, &pt[3], &fang, &tang);

	Rect rf(pt[3].x - pt[2].x, pt[3].y - pt[2].y, 2 * pt[2].x, 2 * pt[2].y);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeHighQuality);

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		Rect rf2 = rf;
		rf2.X += pxp->adorn.feed;
		rf2.Y += pxp->adorn.feed;
		gh.SetCompositingQuality(CompositingQualityGammaCorrected);

		gh.DrawArc(&pen, rf2, (float)fang, (float)(tang));
	}

	if (!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);

		gh.SetCompositingQuality(CompositingQualityGammaCorrected);
		gh.DrawArc(pp, rf, (float)fang, (float)(tang));

		delete pp;
	}
}

void _gdiplus_draw_polyline(visual_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	GraphicsPath path;
	POINT pt[2];

	for (int i = 0; i<n - 1; i++)
	{
		pt[0].x = ppt[i].x;
		pt[0].y = ppt[i].y;
		pt[1].x = ppt[i + 1].x;
		pt[1].y = ppt[i + 1].y;

		DPtoLP(hDC, pt, 2);

		path.AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	}

	Pen* pp = create_pen(pxp);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		GraphicsPath* adron = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxp->adorn.feed, pxp->adorn.feed);

		adron->Transform(&M);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.DrawPath(&pen, adron);

		gh.ResetClip();

		delete adron;
	}

	gh.DrawPath(pp, &path);

	delete pp;
}

void _gdiplus_draw_bezier(visual_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	POINT pt[4];
	pt[0].x = ppt1->x;
	pt[0].y = ppt1->y;
	pt[1].x = ppt2->x;
	pt[1].y = ppt2->y;
	pt[2].x = ppt3->x;
	pt[2].y = ppt3->y;
	pt[3].x = ppt4->x;
	pt[3].y = ppt4->y;

	DPtoLP(hDC, pt, 4);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);

	Pen* pp = (Pen*)create_pen(pxp);
	gh.SetCompositingQuality(CompositingQualityGammaCorrected);
	gh.DrawBezier(pp, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);

	delete pp;
}

void _gdiplus_draw_curve(visual_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	Point* pa = new Point[n];
	POINT pi;

	for (int i = 0; i < n; i++)
	{
		pi.x = ppt[i].x;
		pi.y = ppt[i].y;

		DPtoLP(hDC, &pi, 1);

		pa[i].X = pi.x;
		pa[i].Y = pi.y;
	}

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);

	Pen* pp = (Pen*)create_pen(pxp);
	gh.SetCompositingQuality(CompositingQualityGammaCorrected);
	gh.DrawCurve(pp, pa, n);

	delete[]pa;

	delete pp;
}

void _gdiplus_draw_rect(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		Region region(Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));
		gh.ExcludeClip(&region);

		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		LinearGradientBrush brush(Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b), LinearGradientModeForwardDiagonal);

		gh.FillRectangle(&brush, Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		gh.ResetClip();
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		Region region(Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));
		gh.ExcludeClip(&region);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.SetSmoothingMode(SmoothingModeAntiAlias);
		gh.DrawRectangle(&pen, Rect(pt[0].x + pxp->adorn.feed, pt[0].y + pxp->adorn.feed, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		gh.ResetClip();
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = (Brush*)create_brush(pxb, prt, NULL);
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

void _gdiplus_draw_triangle(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const tchar_t* orient)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	Rect rf(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	GraphicsPath path;

	if (xsicmp(orient, GDI_ATTR_ORIENT_LEFT) == 0)
	{
		path.AddLine(rf.X, rf.Y + rf.Height / 2, rf.X + rf.Width, rf.Y);
		path.AddLine(rf.X + rf.Width, rf.Y, rf.X + rf.Width, rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width, rf.Y + rf.Height, rf.X, rf.Y + rf.Height / 2);
	}
	else if (xsicmp(orient, GDI_ATTR_ORIENT_RIGHT) == 0)
	{
		path.AddLine(rf.X, rf.Y, rf.X + rf.Width, rf.Y + rf.Height / 2);
		path.AddLine(rf.X + rf.Width, rf.Y + rf.Height / 2, rf.X, rf.Y + rf.Height);
		path.AddLine(rf.X, rf.Y + rf.Height, rf.X, rf.Y);
	}
	else if (xsicmp(orient, GDI_ATTR_ORIENT_TOP) == 0)
	{
		path.AddLine(rf.X + rf.Width / 2, rf.Y, rf.X + rf.Width, rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width, rf.Y + rf.Height, rf.X, rf.Y + rf.Height);
		path.AddLine(rf.X, rf.Y + rf.Height, rf.X + rf.Width / 2, rf.Y);
	}
	else if (xsicmp(orient, GDI_ATTR_ORIENT_BOTTOM) == 0)
	{
		path.AddLine(rf.X, rf.Y, rf.X + rf.Width, rf.Y);
		path.AddLine(rf.X + rf.Width, rf.Y, rf.X + rf.Width / 2, rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width / 2, rf.Y + rf.Height, rf.X, rf.Y);
	}
	
	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeHighQuality);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		GraphicsPath* shadow = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxb->shadow.offx, pxb->shadow.offy);

		shadow->Transform(&M);

		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		LinearGradientBrush brush(Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b), LinearGradientModeForwardDiagonal);

		gh.FillPath(&brush, shadow);

		gh.ResetClip();

		delete shadow;
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		GraphicsPath* adron = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxp->adorn.feed, pxp->adorn.feed);

		adron->Transform(&M);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.SetSmoothingMode(SmoothingModeAntiAlias);
		gh.DrawPath(&pen, adron);

		gh.ResetClip();

		delete adron;
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb, prt, &path);
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

void _gdiplus_draw_round(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);
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

	DPtoLP(hDC, pt, 2);

	Rect rf(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	GraphicsPath path;

	path.AddArc(rf.X, rf.Y, 2 * r, 2 * r, 180, 90);
	path.AddLine(rf.X + r, rf.Y, rf.X + rf.Width - r, rf.Y);
	path.AddArc(rf.X + rf.Width - 2 * r, rf.Y, 2 * r, 2 * r, 270, 90);
	path.AddLine(rf.X + rf.Width, rf.Y + r, rf.X + rf.Width, rf.Y + rf.Height - r);
	path.AddArc(rf.X + rf.Width - 2 * r, rf.Y + rf.Height - 2 * r, 2 * r, 2 * r, 0, 90);
	path.AddLine(rf.X + rf.Width - r, rf.Y + rf.Height, rf.X + r, rf.Y + rf.Height);
	path.AddArc(rf.X, rf.Y + rf.Height - 2 * r, 2 * r, 2 * r, 90, 90);
	path.AddLine(rf.X, rf.Y + rf.Height - r, rf.X, rf.Y + r);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeHighQuality);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		GraphicsPath* shadow = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxb->shadow.offx, pxb->shadow.offy);

		shadow->Transform(&M);

		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		LinearGradientBrush brush(Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b), LinearGradientModeForwardDiagonal);

		gh.FillPath(&brush, shadow);

		gh.ResetClip();

		delete shadow;
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		GraphicsPath* adron = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxp->adorn.feed, pxp->adorn.feed);

		adron->Transform(&M);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.SetSmoothingMode(SmoothingModeAntiAlias);
		gh.DrawPath(&pen, adron);

		gh.ResetClip();

		delete adron;
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb, prt, &path);
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

void _gdiplus_calc_fan(visual_t rdc, const xpoint_t* ppt, const xsize_t* pxs, double fang, double tang, xpoint_t* pa, int n)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	xrect_t xr = { 0 };
	int r, s;

	r = pxs->w;
	s = pxs->h;

	xr.x = ppt->x - r;
	xr.y = ppt->y - r;
	xr.w = 2 * r;
	xr.h = 2 * r;

	POINT pt[2];
	pt[0].x = xr.x;
	pt[0].y = xr.y;
	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h;

	DPtoLP(hDC, pt, 2);

	Rect rf1(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	r -= s;
	pt_center_rect(&xr, 2 * r, 2 * r);

	pt[0].x = xr.x;
	pt[0].y = xr.y;
	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h;

	DPtoLP(hDC, pt, 2);

	Rect rf2(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	POINT p;
	p.x = (pt[0].x + pt[1].x) / 2;
	p.y = (pt[0].y + pt[1].y) / 2;

	_calc_point(&p, rf1.Width / 2, fang, &(pt[0]));
	_calc_point(&p, rf1.Width / 2, tang, &(pt[1]));

	LPtoDP(hDC, pt, 2);

	if (n > 0)
	{
		pa[0].x = pt[0].x;
		pa[0].y = pt[0].y;
	}
	if (n > 1)
	{
		pa[1].x = pt[1].x;
		pa[1].y = pt[1].y;
	}

	_calc_point(&p, rf2.Width / 2, tang, &(pt[0]));
	_calc_point(&p, rf2.Width / 2, fang, &(pt[1]));

	LPtoDP(hDC, pt, 2);

	if (n > 2)
	{
		pa[2].x = pt[0].x;
		pa[2].y = pt[0].y;
	}
	if (n > 3)
	{
		pa[3].x = pt[1].x;
		pa[3].y = pt[1].y;
	}
}

void _gdiplus_draw_fan(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, const xsize_t* pxs, double fang, double tang)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	xrect_t xr = { 0 };;
	int r, s;

	r = pxs->w;
	s = pxs->h;

	xr.x = ppt->x - r;
	xr.y = ppt->y - r;
	xr.w = 2 * r;
	xr.h = 2 * r;

	POINT pt[2];
	pt[0].x = xr.x;
	pt[0].y = xr.y;
	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h;

	DPtoLP(hDC, pt, 2);

	Rect rf1(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	r -= s;
	pt_center_rect(&xr, 2 * r, 2 * r);

	pt[0].x = xr.x;
	pt[0].y = xr.y;
	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h;

	DPtoLP(hDC, pt, 2);

	Rect rf2(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	GraphicsPath path;

	path.AddArc(rf1.X, rf1.Y, rf1.Width, rf1.Height, (fang / (2 * XPI) * 360), ((tang - fang) / (2 * XPI) * 360));

	POINT p;
	p.x = (pt[0].x + pt[1].x) / 2;
	p.y = (pt[0].y + pt[1].y) / 2;

	_calc_point(&p, rf1.Width / 2, tang, &(pt[0]));
	_calc_point(&p, rf2.Width / 2, tang, &(pt[1]));

	path.AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);

	path.AddArc(rf2.X, rf2.Y, rf2.Width, rf2.Height, (tang / (2 * XPI) * 360), ((fang - tang) / (2 * XPI) * 360));

	_calc_point(&p, rf2.Width / 2, fang, &(pt[0]));
	_calc_point(&p, rf1.Width / 2, fang, &(pt[1]));

	path.AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeHighQuality);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		GraphicsPath* shadow = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxb->shadow.offx, pxb->shadow.offy);

		shadow->Transform(&M);

		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		LinearGradientBrush brush(Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b), LinearGradientModeForwardDiagonal);

		gh.FillPath(&brush, shadow);

		gh.ResetClip();

		delete shadow;
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		GraphicsPath* adron = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxp->adorn.feed, pxp->adorn.feed);

		adron->Transform(&M);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.SetSmoothingMode(SmoothingModeAntiAlias);
		gh.DrawPath(&pen, adron);

		gh.ResetClip();

		delete adron;
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb, &xr, &path);
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

void _gdiplus_draw_ellipse(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		LinearGradientBrush brush(Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b), LinearGradientModeForwardDiagonal);

		gh.FillEllipse(&brush, Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y));
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.DrawEllipse(&pen, Rect(pt[0].x + pxp->adorn.feed, pt[0].y + pxp->adorn.feed, pt[1].x - pt[0].x, pt[1].y - pt[0].y));
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = (Brush*)create_brush(pxb, prt, NULL);
		gh.FillEllipse(pb, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		delete pb;
	}

	if (!is_null_xpen(pxp))
	{
		Pen* pp = (Pen*)create_pen(pxp);
		gh.DrawEllipse(pp, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

		delete pp;
	}
}

void _gdiplus_draw_pie(visual_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, const xsize_t* pxs, double fang, double tang)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	POINT pt[2];
	pt[0].x = ppt->x - pxs->w;
	pt[0].y = ppt->y - pxs->h;
	pt[1].x = ppt->x + pxs->w;
	pt[1].y = ppt->y + pxs->h;

	DPtoLP(hDC, pt, 2);

	Rect rf(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeHighQuality);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		LinearGradientBrush brush(Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b), LinearGradientModeForwardDiagonal);

		gh.FillPie(&brush, Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), (float)(fang / (2 * XPI) * 360), (float)((tang - fang) / (2 * XPI) * 360));
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.SetCompositingQuality(CompositingQualityGammaCorrected);
		gh.DrawPie(&pen, Rect(pt[0].x + pxp->adorn.feed, pt[0].y + pxp->adorn.feed, pt[1].x - pt[0].x, pt[1].y - pt[0].y), (float)(fang / (2 * XPI) * 360), (float)((tang - fang) / (2 * XPI) * 360));
	}

	if (!is_null_xbrush(pxb))
	{
		GraphicsPath gp;
		
		gp.AddPie(rf, (float)(fang / (2 * XPI) * 360), (float)((tang - fang) / (2 * XPI) * 360));

		Brush* pb = create_brush(pxb, NULL, &gp);

		gh.FillPie(pb, rf, (float)(fang / (2 * XPI) * 360), (float)((tang - fang) / (2 * XPI) * 360));

		delete pb;
	}

	if (!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);

		gh.SetCompositingQuality(CompositingQualityGammaCorrected);
		gh.DrawPie(pp, rf, (float)(fang / (2 * XPI) * 360), (float)((tang - fang) / (2 * XPI) * 360));

		delete pp;
	}
}

void _gdiplus_draw_arrow(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const xspan_t* pxn, double arc)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);
	double a1;
	int x_line0, y_line0, x_line1, y_line1, x_line2, y_line2;
	int x1, x2, y1, y2;
	POINT pt[4];

	int alen;

	alen = pxn->r;

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	x1 = pt[0].x;
	y1 = pt[0].y;
	x2 = pt[1].x;
	y2 = pt[1].y;

	pt[0].x = x2;
	pt[0].y = y2;

	a1 = atan2((float)(y2 - y1), (float)(x2 - x1));
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

	path.AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	path.AddLine(pt[1].x, pt[1].y, pt[2].x, pt[2].y);
	path.AddLine(pt[2].x, pt[2].y, pt[3].x, pt[3].y);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		GraphicsPath* shadow = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxb->shadow.offx, pxb->shadow.offy);

		shadow->Transform(&M);

		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		LinearGradientBrush brush(Rect(pt[0].x + pxb->shadow.offx, pt[0].y + pxb->shadow.offy, pt[1].x - pt[0].x, pt[1].y - pt[0].y), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b), LinearGradientModeVertical);

		gh.FillPath(&brush, shadow);

		gh.ResetClip();

		delete shadow;
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		GraphicsPath* adron = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxp->adorn.feed, pxp->adorn.feed);

		adron->Transform(&M);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.DrawPath(&pen, adron);

		gh.ResetClip();

		delete adron;
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb, prt, &path);
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

void _gdiplus_draw_polygon(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	GraphicsPath path;
	POINT pt[2];

	for (int i = 0; i<n - 1; i++)
	{
		pt[0].x = ppt[i].x;
		pt[0].y = ppt[i].y;
		pt[1].x = ppt[i + 1].x;
		pt[1].y = ppt[i + 1].y;

		DPtoLP(hDC, pt, 2);

		path.AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	}

	if (n > 1)
	{
		pt[0].x = ppt[0].x;
		pt[0].y = ppt[0].y;
		pt[1].x = ppt[1].x;
		pt[1].y = ppt[1].y;

		DPtoLP(hDC, pt, 2);

		path.AddLine(pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	}

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		GraphicsPath* shadow = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxb->shadow.offx, pxb->shadow.offy);

		shadow->Transform(&M);

		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		PathGradientBrush brush(shadow);
		Color clr[3] = { Color(0, 0, 0, 0), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b) };
		REAL pos[3] = { 0.0F, 0.1F, 1.0F };
		brush.SetInterpolationColors(clr, pos, 3);

		gh.FillPath(&brush, shadow);

		gh.ResetClip();

		delete shadow;
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		GraphicsPath* adron = path.Clone();

		Region region(&path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxp->adorn.feed, pxp->adorn.feed);

		adron->Transform(&M);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.DrawPath(&pen, adron);

		gh.ResetClip();

		delete adron;
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb, NULL, &path);
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

void _gdiplus_draw_path(visual_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	GraphicsPath* path = create_path(hDC, aa, pa);

	if (!path)
		return;

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);

	if (pxb && (pxb->shadow.offx || pxb->shadow.offy))
	{
		GraphicsPath* shadow = path->Clone();

		Region region(path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxb->shadow.offx, pxb->shadow.offy);

		shadow->Transform(&M);

		xcolor_t xc_near, xc_far;
		parse_xcolor(&xc_near, pxb->color);
		memcpy((void*)&xc_far, (void*)&xc_near, sizeof(xcolor_t));
		lighten_xcolor(&xc_far, -10);

		PathGradientBrush brush(shadow);
		Color clr[3] = { Color(0, 0, 0, 0), Color(255, xc_near.r, xc_near.g, xc_near.b), Color(255, xc_far.r, xc_far.g, xc_far.b) };
		REAL pos[3] = { 0.0F, 0.1F, 1.0F };
		brush.SetInterpolationColors(clr, pos, 3);

		gh.FillPath(&brush, shadow);

		gh.ResetClip();

		delete shadow;
	}

	if (pxp && (pxp->adorn.feed || pxp->adorn.size))
	{
		GraphicsPath* adron = path->Clone();

		Region region(path);
		gh.ExcludeClip(&region);

		Matrix M;
		M.Translate(pxp->adorn.feed, pxp->adorn.feed);

		adron->Transform(&M);

		xcolor_t xc_gray;

		parse_xcolor(&xc_gray, pxp->color);
		lighten_xcolor(&xc_gray, -10);

		Pen pen(Color(xc_gray.r, xc_gray.g, xc_gray.b), (REAL)pxp->adorn.size);

		gh.DrawPath(&pen, adron);

		gh.ResetClip();

		delete adron;
	}

	if (!is_null_xbrush(pxb))
	{
		Brush* pb = create_brush(pxb, NULL, path);
		gh.FillPath(pb, path);

		delete pb;
	}

	if (!is_null_xpen(pxp))
	{
		Pen* pp = create_pen(pxp);
		gh.DrawPath(pp, path);

		delete pp;
	}

	delete path;
}

void _gdiplus_alphablend_rect(visual_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);
	xbrush_t xb;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	xsprintf(xb.opacity, _T("%d"), opacity);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	Brush* pb = (Brush*)create_brush(&xb, prt, NULL);
	gh.FillRectangle(pb, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

	delete pb;
}

void _gdiplus_gradient_rect(visual_t rdc, const xcolor_t* clr_brim, const xcolor_t* clr_core, const tchar_t* gradient, const xrect_t* prt)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);
	bitmap_t bmp;
	win32_bitmap_t* pwb;

	POINT pt[2];
	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	DPtoLP(hDC, pt, 2);

	bmp = _create_gradient_bitmap(rdc, clr_brim, clr_core, pt[1].x - pt[0].x, pt[1].y - pt[0].y, gradient);
	pwb = (win32_bitmap_t*)bmp;

	Bitmap* pbm = new Bitmap(pwb->bitmap, (HPALETTE)GetStockObject(DEFAULT_PALETTE));

	_destroy_bitmap(bmp);

	Brush* pb = new TextureBrush(pbm, Rect(0, 0, pt[1].x - pt[0].x, pt[1].y - pt[0].y));
	delete pbm;

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	gh.FillRectangle(pb, Rect(pt[0].x, pt[0].y, pt[1].x - pt[0].x, pt[1].y - pt[0].y));

	delete pb;
}

void _gdiplus_draw_text(visual_t rdc,const xfont_t* pxf,const xface_t* pxa,const xrect_t* prt,const tchar_t* txt,int len)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

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

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	if (len < 0 && txt)
		len = xslen(txt);

	gh.DrawString(txt,len,pf,rf,ps,pb);

	delete pb;
	delete pf;
	delete ps;
}

void _gdiplus_text_out(visual_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	HFONT hFont, orgFont;
	COLORREF clr, orgClr;
	int fs;
	LOGFONT lf;
	xcolor_t xc;

	CopyMemory((void*)&lf, (void*)&lf_gdiplus, sizeof(LOGFONT));

	fs = xstol(pxf->size);

	parse_xcolor(&xc, pxf->color);

	lf.lfHeight = -MulDiv(fs, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = xstol(pxf->weight);

	if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!is_null(pxf->family))
	{
		xscpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);

	orgFont = (HFONT)SelectObject(hDC, hFont);

	clr = RGB(xc.r, xc.g, xc.b);
	orgClr = SetTextColor(hDC, clr);

	if (len < 0 && txt)
		len = xslen(txt);

	TextOut(hDC, ppt->x, ppt->y, txt, len);

	SetTextColor(hDC, orgClr);
	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);
}

void _gdiplus_draw_image(visual_t rdc,bitmap_t bmp,const tchar_t* clr,const xrect_t* prt)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);
	win32_bitmap_t* pwb = (win32_bitmap_t*)bmp;

	Image* pi = new Bitmap((HBITMAP)(pwb->bitmap), (HPALETTE)GetStockObject(DEFAULT_PALETTE));
	if (!pi)
		return;

	int srcw = pi->GetWidth();
	int srch = pi->GetHeight();

	ImageAttributes iab;

	if (!is_null(clr))
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

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	gh.DrawImage(pi,Rect(rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top),0,0,srcw,srch,UnitPixel,&iab);

	delete pi;
}

void _gdiplus_draw_bitmap(visual_t rdc, bitmap_t bmp, const xpoint_t* ppt)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);
	win32_bitmap_t* pwb = (win32_bitmap_t*)bmp;

	Image* pi = new Bitmap((HBITMAP)pwb->bitmap, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
	if (!pi)
		return;

	int srcw = pi->GetWidth();
	int srch = pi->GetHeight();

	ImageAttributes iab;
	iab.SetColorKey(Color(250, 250, 250), Color(255, 255, 255));

	POINT pt;
	pt.x = ppt->x;
	pt.y = ppt->y;

	DPtoLP(hDC, (LPPOINT)&pt, 1);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	gh.DrawImage(pi, pt.x, pt.y, 0, 0, srcw, srch, UnitPixel);

	delete pi;
}

void _gdiplus_draw_picture(visual_t rdc, const tchar_t* fname, const tchar_t* clr, const xrect_t* prt)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	Image* pi = new Image(fname);
	if (!pi)
		return;

	int srcw = pi->GetWidth();
	int srch = pi->GetHeight();

	ImageAttributes iab;

	if (!is_null(clr))
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

	DPtoLP(hDC, (LPPOINT)&rt, 2);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	gh.DrawImage(pi, Rect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top), 0, 0, srcw, srch, UnitPixel, &iab);

	delete pi;
}

/*void _gdiplus_draw_ico(visual_t rdc, res_ico_t icon, const xrect_t* prt)
{
	HDC hDC = (HDC)(ctx->context);

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

void _gdiplus_exclip_rect(visual_t rdc, const xrect_t* pxr)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	ExcludeClipRect(hDC, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}

void _gdiplus_inclip_rect(visual_t rdc, const xrect_t* pxr)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	IntersectClipRect(hDC, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}

void _gdiplus_fill_region(visual_t rdc, const xbrush_t* pxb, res_rgn_t rgn)
{
	win32_context_t* ctx = (win32_context_t*)rdc;
	HDC hDC = (HDC)(ctx->context);

	Region gn(rgn);

	Brush* pb = create_brush(pxb, NULL, NULL);

	Gdiplus::Graphics gh(hDC);

	gh.SetPageUnit(UnitPixel);

	gh.FillRegion(pb, &gn);

	delete pb;
}

void _gdiplus_text_rect(visual_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr)
{
	win32_context_t* ctx = (win32_context_t*)rdc;

	BOOL bRef = 0;
	HDC hDC;

	if (!rdc)
	{
		bRef = 1;
		hDC = GetDC(NULL);
	}
	else
	{
		hDC = (HDC)(ctx->context);
	}

	POINT pt[2];
	pt[0].x = pxr->x;
	pt[0].y = pxr->y;
	pt[1].x = pxr->x + pxr->w;
	pt[1].y = pxr->y + pxr->h;

	DPtoLP(hDC, pt, 2);

	RectF rf((REAL)pt[0].x, (REAL)pt[0].y, (REAL)(pt[1].x - pt[0].x), (REAL)(pt[1].y - pt[0].y));

	Gdiplus::Graphics gh(hDC);

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

	pt[0].x = (int)(rfOut.GetLeft());
	pt[0].y = (int)(rfOut.GetTop());
	pt[1].x = (int)(rfOut.GetRight());
	pt[1].y = (int)(rfOut.GetBottom());

	LPtoDP(hDC, pt, 2);

	pxr->x = pt[0].x;
	pxr->y = pt[0].y;
	pxr->w = pt[1].x - pt[0].x;
	pxr->h = pt[1].y - pt[0].y;

	delete pf;
	delete ps;

	if (bRef)
		ReleaseDC(NULL, hDC);
}

void _gdiplus_text_size(visual_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	win32_context_t* ctx = (win32_context_t*)rdc;

	BOOL bRef = 0;
	HDC hDC;

	if (!rdc)
	{
		bRef = 1;
		hDC = GetDC(NULL);
	}
	else
	{
		hDC = (HDC)(ctx->context);
	}

	LOGFONT lf;
	HFONT hFont, orgFont;
	SIZE si;
	int fs;

	CopyMemory((void*)&lf, (void*)&lf_gdiplus, sizeof(LOGFONT));

	fs = xstol(pxf->size);

	lf.lfHeight = -MulDiv(fs, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = xstol(pxf->weight);

	if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!is_null(pxf->family))
	{
		xscpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);

	orgFont = (HFONT)SelectObject(hDC, hFont);

	if (len < 0)
		len = (txt)? xslen(txt) : 0;

	GetTextExtentPoint32(hDC, txt, len, &si);

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);

	if (bRef)
		ReleaseDC(NULL, hDC);

	pxs->w = si.cx;
	pxs->h = si.cy;
}

void _gdiplus_text_metric(visual_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	win32_context_t* ctx = (win32_context_t*)rdc;

	BOOL bRef = 0;
	HDC hDC;

	if (!rdc)
	{
		bRef = 1;
		hDC = GetDC(NULL);
	}
	else
	{
		hDC = (HDC)(ctx->context);
	}

	LOGFONT lf;
	HFONT hFont, orgFont;
	int fs;
	TEXTMETRIC tm = { 0 };

	CopyMemory((void*)&lf, (void*)&lf_gdiplus, sizeof(LOGFONT));

	fs = xstol(pxf->size);

	lf.lfHeight = -MulDiv(fs, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = xstol(pxf->weight);

	if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = 1;
	}
	else if (xscmp(pxf->style, GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = 1;
	}

	if (!is_null(pxf->family))
	{
		xscpy(lf.lfFaceName, pxf->family);
	}

	hFont = CreateFontIndirect(&lf);
	orgFont = (HFONT)SelectObject(hDC, hFont);

	GetTextMetrics(hDC, &tm);

	hFont = (HFONT)SelectObject(hDC, orgFont);
	DeleteObject(hFont);

	if (bRef)
		ReleaseDC(NULL, hDC);

	//pxs->w = tm.tmAveCharWidth;
	pxs->h = tm.tmHeight;
	pxs->w = tm.tmMaxCharWidth;
}

#endif //XDU_SUPPORT_CONTEXT_GRAPHICPLUS
