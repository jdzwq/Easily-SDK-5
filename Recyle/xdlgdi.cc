/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlgdi.h"
#include "xdlattr.h"
#include "xdlsvg.h"
#include "xdldef.h"
#include "xdlctx.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlutil.h"


#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif

#include "gdiplus\gdiplus.h"

using namespace Gdiplus;

GdiplusStartupInput	gdiplusStartupInput;
ULONG_PTR			gdiplusToken;
HMODULE				gdiEx = NULL;

typedef BOOL (WINAPI *GradientFillPtr)(HDC,PTRIVERTEX,ULONG,PVOID,ULONG,ULONG);
typedef BOOL (WINAPI *AlphaBlendPtr)(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
typedef BOOL (WINAPI *AlphaDIBBlendPtr)(HDC,int,int,int,int,CONST VOID *,CONST BITMAPINFO *,UINT,int,int,int,int,BLENDFUNCTION);
typedef BOOL (WINAPI *TransparentBltPtr)(HDC,int,int,int,int,HDC hDC,int,int,int,int,UINT);
typedef BOOL (WINAPI *TransparentDIBitsPtr)(HDC,int,int,int,int,CONST VOID *,CONST BITMAPINFO *,UINT,int,int,int,int,UINT);

void initial_context()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	gdiEx = LoadLibrary(L"msimg32.dll");
}

void uninitial_context()
{
	GdiplusShutdown(gdiplusToken);

	if(gdiEx)
	{
		FreeLibrary(gdiEx);
		gdiEx = NULL;
	}
}


Bitmap* _create_pattern_bitmap(const xcolor_t* pxc_front,const xcolor_t* pxc_back,const wchar_t* lay)
{
	HDC memDC;
	HBRUSH hBrush;
	HPEN newPen,orgPen;
	HBITMAP newBmp,orgBmp;
	RECT rt;

	HDC hDC = GetDC(NULL);
	newBmp = CreateCompatibleBitmap(hDC,4,4);
	memDC = CreateCompatibleDC(hDC);
	orgBmp = (HBITMAP)SelectObject(memDC,newBmp);
	ReleaseDC(NULL,hDC);

	rt.left = rt.top = 0;
	rt.right = 4;
	rt.bottom = 4;
	DPtoLP(memDC,(LPPOINT)&rt,2);

	hBrush = CreateSolidBrush(RGB(pxc_back->r,pxc_back->g,pxc_back->b));
	FillRect(memDC,&rt,hBrush);
	DeleteObject(hBrush);

	newPen = CreatePen(PS_SOLID,2,RGB(pxc_front->r,pxc_front->g,pxc_front->b));
	orgPen = (HPEN)SelectObject(memDC,newPen);

	POINT pt[2];
	if(xsicmp(lay,GDI_ATTR_FILL_MODE_VERT) == 0)
	{
		pt[0].x = 2;
		pt[0].y = 2;
		pt[1].x = 2;
		pt[1].y = 4;
		DPtoLP(memDC,pt,2);
	}else
	{
		pt[0].x = 0;
		pt[0].y = 2;
		pt[1].x = 4;
		pt[1].y = 2;
		DPtoLP(memDC,pt,2);
	}

	MoveToEx(memDC,pt[0].x,pt[0].y,NULL);
	LineTo(memDC,pt[1].x,pt[1].y);

	newPen = (HPEN)SelectObject(memDC,orgPen);
	DeleteObject(newPen);

	SelectObject(memDC,orgBmp);
	DeleteDC(memDC);

	Bitmap* pbm = new Bitmap(newBmp,(HPALETTE)GetStockObject(DEFAULT_PALETTE));
	DeleteObject(newBmp);

	return pbm;
}

Bitmap* _create_gradient_bitmap(const xcolor_t* pxc_near,const xcolor_t* pxc_center,const wchar_t* lay,int x1,int y1,int x2,int y2)
{
	if(!gdiEx)
		return NULL;

	GradientFillPtr pf_GradientFill = (GradientFillPtr)GetProcAddress(gdiEx,"GradientFill");
	if(!pf_GradientFill)
		return NULL;

	HDC hDC = GetDC(NULL);

	RECT rt;
	rt.left = x1;
	rt.top = y1;
	rt.right = x2;
	rt.bottom = y2;
	DPtoLP(hDC,(LPPOINT)&rt,2);

	HBITMAP newBmp = CreateCompatibleBitmap(hDC,rt.right - rt.left,rt.bottom - rt.top);
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP orgBmp = (HBITMAP)SelectObject(memDC,newBmp);
	ReleaseDC(NULL,hDC);

	COLORREF clr_brush = RGB(pxc_near->r,pxc_near->g,pxc_near->b);
	COLORREF clr_linear = RGB(pxc_center->r,pxc_center->g,pxc_center->b);

	TRIVERTEX tv[4];
	GRADIENT_RECT gr[2];
	unsigned long mode = 0;

	if(xsicmp(lay,GDI_ATTR_FILL_MODE_VERT) == 0)
	{
		tv[0].x = rt.left;
		tv[0].y = rt.top;
		tv[0].Red = GetRValue(clr_brush) << 8;
		tv[0].Green = GetGValue(clr_brush) << 8;
		tv[0].Blue = GetBValue(clr_brush) << 8;
		tv[0].Alpha = 255;

		tv[1].x = rt.right;
		tv[1].y = (rt.top + rt.bottom) / 2;
		tv[1].Red = GetRValue(clr_linear) << 8;
		tv[1].Green = GetGValue(clr_linear) << 8;
		tv[1].Blue = GetBValue(clr_linear) << 8;
		tv[1].Alpha = 255;

		tv[2].x = rt.left;
		tv[2].y = (rt.top + rt.bottom) / 2;
		tv[2].Red = GetRValue(clr_linear) << 8;
		tv[2].Green = GetGValue(clr_linear) << 8;
		tv[2].Blue = GetBValue(clr_linear) << 8;
		tv[2].Alpha = 255;

		tv[3].x = rt.right;
		tv[3].y = rt.bottom;
		tv[3].Red = GetRValue(clr_brush) << 8;
		tv[3].Green = GetGValue(clr_brush) << 8;
		tv[3].Blue = GetBValue(clr_brush) << 8;
		tv[3].Alpha = 255;

		mode = GRADIENT_FILL_RECT_V;
	}else
	{
		tv[0].x = rt.left;
		tv[0].y = rt.top;
		tv[0].Red = GetRValue(clr_brush) << 8;
		tv[0].Green = GetGValue(clr_brush) << 8;
		tv[0].Blue = GetBValue(clr_brush) << 8;
		tv[0].Alpha = 255;
		
		tv[1].x = (rt.left + rt.right) / 2;
		tv[1].y = rt.bottom;
		tv[1].Red = GetRValue(clr_linear) << 8;
		tv[1].Green = GetGValue(clr_linear) << 8;
		tv[1].Blue = GetBValue(clr_linear) << 8;
		tv[1].Alpha = 255;
		
		tv[2].x = (rt.left + rt.right) / 2;
		tv[2].y = rt.top;
		tv[2].Red = GetRValue(clr_linear) << 8;
		tv[2].Green = GetGValue(clr_linear) << 8;
		tv[2].Blue = GetBValue(clr_linear) << 8;
		tv[2].Alpha = 255;
		
		tv[3].x = rt.right;
		tv[3].y = rt.bottom;
		tv[3].Red = GetRValue(clr_brush) << 8;
		tv[3].Green = GetGValue(clr_brush) << 8;
		tv[3].Blue = GetBValue(clr_brush) << 8;
		tv[3].Alpha = 255;

		mode = GRADIENT_FILL_RECT_H;
	}

	gr[0].UpperLeft = 0;
	gr[0].LowerRight = 1;
	gr[1].UpperLeft = 2;
	gr[1].LowerRight = 3;

	(*pf_GradientFill)(hDC,tv,4,&gr,2,mode);

	newBmp = (HBITMAP)SelectObject(memDC,orgBmp);
	DeleteDC(memDC);


	Bitmap* pbm = new Bitmap(newBmp,(HPALETTE)GetStockObject(DEFAULT_PALETTE));
	DeleteObject(newBmp);
	
	return pbm;
}

Pen* _create_pen(const xpen_t* pxp)
{
	xcolor_t xr = {0};
	parse_color(&xr,pxp->color);

	return new Pen(Color(xr.r,xr.g,xr.b),(REAL)xstos(pxp->size));
}

Brush* _create_brush(const xbrush_t* pxb,int x1,int y1,int x2,int y2)
{
	Brush* pb;
	Bitmap* pbm;

	xcolor_t brush_color = {0};
	xcolor_t linear_color = {0};

	short opacity;

	parse_color(&brush_color,pxb->brush_color);
	parse_color(&linear_color,pxb->linear_color);

	opacity = (short)(255 * xstof(pxb->opacity));

	if(xsicmp(pxb->style,GDI_ATTR_FILL_STYLE_HATCH) == 0)
	{
		if(xsicmp(pxb->mode,GDI_ATTR_FILL_MODE_VERT) == 0)
			pb = new HatchBrush(HatchStyleVertical,Color((BYTE)opacity,brush_color.r,brush_color.g,brush_color.b),Color((BYTE)opacity,linear_color.r,linear_color.g,linear_color.b));
		else
			pb = new HatchBrush(HatchStyleHorizontal,Color((BYTE)opacity,brush_color.r,brush_color.g,brush_color.b),Color((BYTE)opacity,linear_color.r,linear_color.g,linear_color.b));
	}else if(xsicmp(pxb->style,GDI_ATTR_FILL_STYLE_GRADIENT) == 0)
	{
		pbm = _create_gradient_bitmap(&brush_color,&linear_color,pxb->mode,x1,y1,x2,y2);
		pb = new TextureBrush(pbm,Rect(x1,y1,x2 - x1,y2 - y1));
		delete pbm;
	}else if(xsicmp(pxb->style,GDI_ATTR_FILL_STYLE_PATTERN) == 0)
	{
		pbm = _create_pattern_bitmap(&brush_color,&linear_color,pxb->mode);
		pb = new TextureBrush(pbm);
		delete pbm;
	}else
	{
		pb = new SolidBrush(Color((BYTE)opacity,brush_color.r,brush_color.g,brush_color.b));
	}

	return pb;
}

void draw_line_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2)
{
	HDC hDC = get_windows_dc(pct);

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	xcolor_t color = {0};
	parse_color(&color,pxp->color);

	Pen pen(Color(color.r,color.g,color.b),(REAL)xstos(pxp->size));

	Graphics gh(hDC);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawLine(&pen,pt[0].x,pt[0].y,pt[1].x,pt[1].y);
}

void draw_polyline_raw(const context_t* pct,const xpen_t* pxp,int* px,int* py,int n)
{
	HDC hDC = get_windows_dc(pct);

	GraphicsPath path;	
	POINT pt[2];
	
	for(int i=0;i<n-1;i++)
	{
		pt[0].x = px[i];
		pt[0].y = py[i];
		pt[1].x = px[i+1];
		pt[1].y = py[i+1];

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	Pen* pp = _create_pen(pxp);
	Graphics gh(hDC);
	gh.DrawPath(pp,&path);

	delete pp;
}

void draw_polygon_raw(const context_t* pct,const xpen_t* pxp,int* px,int* py,int n)
{
	HDC hDC = get_windows_dc(pct);

	GraphicsPath path;	
	POINT pt[2];
	
	for(int i=0;i<n-1;i++)
	{
		pt[0].x = px[i];
		pt[0].y = py[i];
		pt[1].x = px[i+1];
		pt[1].y = py[i+1];

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	if(n > 1)
	{
		pt[0].x = px[0];
		pt[0].y = py[0];
		pt[1].x = px[1];
		pt[1].y = py[1];

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	Graphics gh(hDC);
	Pen* pp = _create_pen(pxp);
	gh.DrawPath(pp,&path);

	delete pp;
}

void fill_polygon_raw(const context_t* pct,const xbrush_t* pxb,int* px,int* py,int n)
{
	HDC hDC = get_windows_dc(pct);

	GraphicsPath path;	
	POINT pt[2];
	
	for(int i=0;i<n-1;i++)
	{
		pt[0].x = px[i];
		pt[0].y = py[i];
		pt[1].x = px[i+1];
		pt[1].y = py[i+1];

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	if(n > 1)
	{
		pt[0].x = px[0];
		pt[0].y = py[0];
		pt[1].x = px[1];
		pt[1].y = py[1];

		DPtoLP(hDC,pt,2);

		path.AddLine(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
	}

	Graphics gh(hDC);
	Brush* pb = _create_brush(pxb,px[0],py[0],px[n-1],py[n-1]);
	gh.FillPath(pb,&path);

	delete pb;
}

void draw_rect_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2)
{
	HDC hDC = get_windows_dc(pct);

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	xcolor_t color = {0};
	parse_color(&color,pxp->color);

	Pen pen(Color(color.r,color.g,color.b),(REAL)xstos(pxp->size));

	Graphics gh(hDC);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawRectangle(&pen,Rect(pt[0].x,pt[0].y,pt[1].x - pt[0].x,pt[1].y - pt[0].y));
}

void fill_rect_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2)
{
	HDC hDC = get_windows_dc(pct);

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Graphics gh(hDC);

	Brush* pb = _create_brush(pxb,x1,y1,x2,y2);
	gh.FillRectangle(pb,Rect(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y));

	delete pb;

}

void draw_round_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2)
{
	HDC hDC = get_windows_dc(pct);
	int r;

	r = (x2 - x1) / 4;

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Rect rf(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y);

	GraphicsPath path;
	path.AddLine(rf.X,rf.Y + r,rf.X + r,rf.Y);
	path.AddLine(rf.X + r,rf.Y,rf.X + rf.Width - r,rf.Y);
	path.AddLine(rf.X + rf.Width - r,rf.Y,rf.X + rf.Width,rf.Y + r);
	path.AddLine(rf.X + rf.Width,rf.Y + r,rf.X + rf.Width,rf.Y + rf.Height - r);
	path.AddLine(rf.X + rf.Width,rf.Y + rf.Height - r,rf.X + rf.Width - r,rf.Y + rf.Height);
	path.AddLine(rf.X + rf.Width - r,rf.Y + rf.Height,rf.X + r,rf.Y + rf.Height);
	path.AddLine(rf.X,rf.Y + rf.Height - r,rf.X,rf.Y + r);
	
	Graphics gh(hDC);
	Pen* pp = _create_pen(pxp);
	gh.DrawPath(pp,&path);

	delete pp;
}

void fill_round_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2)
{
	HDC hDC = get_windows_dc(pct);
	int r;

	r = (x2 - x1) / 4;

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Rect rf(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y);

	GraphicsPath path;
	path.AddLine(rf.X,rf.Y + r,rf.X + r,rf.Y);
	path.AddLine(rf.X + r,rf.Y,rf.X + rf.Width - r,rf.Y);
	path.AddLine(rf.X + rf.Width - r,rf.Y,rf.X + rf.Width,rf.Y + r);
	path.AddLine(rf.X + rf.Width,rf.Y + r,rf.X + rf.Width,rf.Y + rf.Height - r);
	path.AddLine(rf.X + rf.Width,rf.Y + rf.Height - r,rf.X + rf.Width - r,rf.Y + rf.Height);
	path.AddLine(rf.X + rf.Width - r,rf.Y + rf.Height,rf.X + r,rf.Y + rf.Height);
	path.AddLine(rf.X,rf.Y + rf.Height - r,rf.X,rf.Y + r);
	
	Graphics gh(hDC);
	Brush* pb = _create_brush(pxb,x1,y1,x2,y2);
	gh.FillPath(pb,&path);

	delete pb;
}

void draw_ellipse_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2)
{
	HDC hDC = get_windows_dc(pct);

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	xcolor_t color = {0};
	parse_color(&color,pxp->color);

	Pen pen(Color(color.r,color.g,color.b),(REAL)xstos(pxp->size));

	Graphics gh(hDC);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.DrawEllipse(&pen,Rect(pt[0].x,pt[0].y,pt[1].x - pt[0].x,pt[1].y - pt[0].y));
}

void fill_ellipse_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2)
{
	HDC hDC = get_windows_dc(pct);

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Graphics gh(hDC);

	Brush* pb = _create_brush(pxb,x1,y1,x2,y2);
	gh.FillEllipse(pb,Rect(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y));

	delete pb;
}

void draw_text_raw(const context_t* pct,const xfont_t* pxf,int x1,int y1,int x2,int y2,const wchar_t* txt,int len)
{
	HDC hDC = get_windows_dc(pct);

	FontStyle fs;

	if(xsicmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		fs = FontStyleItalic;
	}else if(xsicmp(pxf->style,GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		fs = FontStyleUnderline;
	}else if(xsicmp(pxf->style,GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		fs = FontStyleStrikeout;
	}else
	{
		fs = FontStyleRegular;
	}

	FontFamily ff(pxf->family);
	BYTE fx = (BYTE)pxf->size;
	Font font(&ff,fx,fs,UnitPoint);

	StringFormat sf;
	if(xsicmp(pxf->line_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		sf.SetLineAlignment(StringAlignmentCenter);
	}else if(xsicmp(pxf->line_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		sf.SetLineAlignment(StringAlignmentFar);
	}else
	{
		sf.SetLineAlignment(StringAlignmentNear);
	}

	if(xsicmp(pxf->text_align,GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		sf.SetAlignment(StringAlignmentCenter);
	}else if(xsicmp(pxf->text_align,GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		sf.SetAlignment(StringAlignmentFar);
	}else
	{
		sf.SetAlignment(StringAlignmentNear);
	}

	if(is_null(pxf->text_wrap))
		sf.SetFormatFlags(StringFormatFlagsNoWrap);

	xcolor_t text_color = {0};
	parse_color(&text_color,pxf->text_color);

	Brush* pb = new SolidBrush(Color(text_color.r,text_color.g,text_color.b));

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	RectF rf((REAL)pt[0].x,(REAL)pt[0].y,(REAL)(pt[1].x - pt[1].x),(REAL)(pt[1].y - pt[1].y));

	Graphics gh(hDC);
	gh.DrawString(txt,len,&font,rf,&sf,pb);
	delete pb;

}

void  draw_triangle_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2,const wchar_t* ori)
{
	HDC hDC = get_windows_dc(pct);

	Pen* pp = (pxp)? _create_pen(pxp) : (new Pen(Color(255,0,0,0),1));

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Rect rf(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y);
	
	GraphicsPath path;
	if(xsicmp(ori,GDI_ATTR_ORITATION_LEFT) == 0)
	{
		path.AddLine(rf.X,rf.Y + rf.Height / 2,rf.X + rf.Width,rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width,rf.Y + rf.Height,rf.X + rf.Width,rf.Y);
		path.AddLine(rf.X + rf.Width,rf.Y,rf.X,rf.Y + rf.Height / 2);
	}else if(xsicmp(ori,GDI_ATTR_ORITATION_TOP) == 0)
	{
		path.AddLine(rf.X,rf.Y,rf.X + rf.Width,rf.Y);
		path.AddLine(rf.X + rf.Width,rf.Y,rf.X + rf.Width / 2,rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width / 2,rf.Y + rf.Height,rf.X,rf.Y);
	}else if(xsicmp(ori,GDI_ATTR_ORITATION_RIGHT) == 0)
	{
		path.AddLine(rf.X,rf.Y,rf.X,rf.Y + rf.Height);
		path.AddLine(rf.X,rf.Y + rf.Height,rf.X + rf.Width,rf.Y + rf.Height / 2);
		path.AddLine(rf.X + rf.Width,rf.Y + rf.Height / 2,rf.X,rf.Y);
	}else if(xsicmp(ori,GDI_ATTR_ORITATION_BOTTOM) == 0)
	{
		path.AddLine(rf.X + rf.Width / 2,rf.Y,rf.X,rf.Y + rf.Height);
		path.AddLine(rf.X,rf.Y + rf.Height,rf.X + rf.Width,rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width,rf.Y + rf.Height,rf.X + rf.Width / 2,rf.Y);
	}
	
	Graphics gh(hDC);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.SetCompositingQuality(CompositingQualityGammaCorrected);
	gh.DrawPath(pp,&path);
		
	delete pp;
}

void  fill_triangle_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2,const wchar_t* ori)
{
	HDC hDC = get_windows_dc(pct);

	Brush* pb = _create_brush(pxb,x1,y1,x2,y2);

	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Rect rf(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y);
	
	GraphicsPath path;
	if(xsicmp(ori,GDI_ATTR_ORITATION_LEFT) == 0)
	{
		path.AddLine(rf.X,rf.Y + rf.Height / 2,rf.X + rf.Width,rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width,rf.Y + rf.Height,rf.X + rf.Width,rf.Y);
		path.AddLine(rf.X + rf.Width,rf.Y,rf.X,rf.Y + rf.Height / 2);
	}else if(xsicmp(ori,GDI_ATTR_ORITATION_TOP) == 0)
	{
		path.AddLine(rf.X,rf.Y,rf.X + rf.Width,rf.Y);
		path.AddLine(rf.X + rf.Width,rf.Y,rf.X + rf.Width / 2,rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width / 2,rf.Y + rf.Height,rf.X,rf.Y);
	}else if(xsicmp(ori,GDI_ATTR_ORITATION_RIGHT) == 0)
	{
		path.AddLine(rf.X,rf.Y,rf.X,rf.Y + rf.Height);
		path.AddLine(rf.X,rf.Y + rf.Height,rf.X + rf.Width,rf.Y + rf.Height / 2);
		path.AddLine(rf.X + rf.Width,rf.Y + rf.Height / 2,rf.X,rf.Y);
	}else if(xsicmp(ori,GDI_ATTR_ORITATION_BOTTOM) == 0)
	{
		path.AddLine(rf.X + rf.Width / 2,rf.Y,rf.X,rf.Y + rf.Height);
		path.AddLine(rf.X,rf.Y + rf.Height,rf.X + rf.Width,rf.Y + rf.Height);
		path.AddLine(rf.X + rf.Width,rf.Y + rf.Height,rf.X + rf.Width / 2,rf.Y);
	}
	
	Graphics gh(hDC);
	gh.FillPath(pb,&path);
		
	delete pb;
}

void  draw_pie_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2,double fang,double tang)
{
	HDC hDC = get_windows_dc(pct);
	
	Pen* pp = (pxp)? _create_pen(pxp) : (new Pen(Color(255,0,0,0),1));
	
	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Rect rf(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y);

	Graphics gh(hDC);
	gh.SetSmoothingMode(SmoothingModeAntiAlias);
	gh.SetCompositingQuality(CompositingQualityGammaCorrected);
	gh.DrawPie(pp,rf,-(float)(fang / (2 * XPI) * 360),-(float)((tang - fang) / (2 * XPI) * 360));

	delete pp;
}

void  fill_pie_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2,double fang,double tang)
{	
	HDC hDC = get_windows_dc(pct);

	Brush* pb = _create_brush(pxb,x1,y1,x2,y2);	
	
	POINT pt[2];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;

	DPtoLP(hDC,pt,2);

	Rect rf(pt[0].x,pt[0].y,pt[1].x - pt[1].x,pt[1].y - pt[1].y);

	Graphics gh(hDC);
	gh.FillPie(pb,rf,-(float)(fang / (2 * XPI) * 360),-(float)((tang - fang) / (2 * XPI) * 360));
	delete pb;
}
/*
void draw_shape_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2,int shape)
{
	switch(shape)
	{
	case stLeftLine:
		draw_line_raw(pct,pxp,x1,y1,x1,y2);
		break;
	case stTopLine:
		draw_line_raw(pct,pxp,x1,y1,x2,y1);
		break;
	case stRightLine:
		draw_line_raw(pct,pxp,x2,y1,x2,y2);
		break;
	case stBottomLine:
		draw_line_raw(pct,pxp,x1,y2,x2,y2);
		break;
	case stRect:
		draw_rect_raw(pct,pxp,x1,y1,x2,y2);
		break;
	case stRound:
		draw_round_raw(pct,pxp,x1,y1,x2,y2);
		break;
	case stEllipse:
		draw_ellipse_raw(pct,pxp,x1,y1,x2,y2);
		break;
	}
}

void fill_shape_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2,int shape)
{
	switch(shape)
	{
	case stRect:
		fill_rect_raw(pct,pxb,x1,y1,x2,y2);
		break;
	case stRound:
		fill_round_raw(pct,pxb,x1,y1,x2,y2);
		break;
	case stEllipse:
		fill_ellipse_raw(pct,pxb,x1,y1,x2,y2);
		break;
	}
}
*/