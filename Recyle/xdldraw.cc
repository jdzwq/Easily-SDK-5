/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "xdldraw.h"
#include "xdlgdi.h"
#include "xdlutil.h"
#include "xdlstr.h"
#include "xdlmem.h"

void draw_line(const context_t* pct,const wchar_t* style,const xpoint_t *ppt1,const xpoint_t *ppt2)
{
	int x1,y1,x2,y2;
	xpen_t xp = {0};

	x1 = mm_to_pt(pct,ppt1->x,1);
	y1 = mm_to_pt(pct,ppt1->y,0);

	x2 = mm_to_pt(pct,ppt2->x,1);
	y2 = mm_to_pt(pct,ppt2->y,0);

	parse_pen_from_style(&xp,style);

	draw_line_raw(pct,&xp,x1,y1,x2,y2);
}

void draw_polyline(const context_t* pct,const wchar_t* style,const xpoint_t *ppt,int n)
{
	int *px,*py;
	xpen_t xp = {0};

	px = (int*)xmem_alloc(n * sizeof(int));
	py = (int*)xmem_alloc(n * sizeof(int));

	for(int i=0;i<n;i++)
	{
		px[i] = mm_to_pt(pct,ppt[i].x,1);
		py[i] = mm_to_pt(pct,ppt[i].y,0);
	}

	parse_pen_from_style(&xp,style);

	draw_polyline_raw(pct,&xp,px,py,n);

	xmem_free(px);
	xmem_free(py);
}

void draw_polygon(const context_t* pct,const wchar_t* style,const xpoint_t *ppt,int n)
{
	int *px,*py;
	xpen_t xp = {0};
	xbrush_t xb = {0};

	px = (int*)xmem_alloc(n * sizeof(int));
	py = (int*)xmem_alloc(n * sizeof(int));

	for(int i=0;i<n;i++)
	{
		px[i] = mm_to_pt(pct,ppt[i].x,1);
		py[i] = mm_to_pt(pct,ppt[i].y,0);
	}

	parse_pen_from_style(&xp,style);
	if(!is_null_pen(&xp))
		draw_polygon_raw(pct,&xp,px,py,n);

	parse_brush_from_style(&xb,style);
	if(!is_null_brush(&xb))
		fill_polygon_raw(pct,&xb,px,py,n);

	xmem_free(px);
	xmem_free(py);
}

void draw_rect(const context_t* pct,const wchar_t* style,const xrect_t *prt)
{
	int x1,y1,x2,y2;
	xpen_t xp = {0};
	xbrush_t xb = {0};

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	parse_pen_from_style(&xp,style);
	if(!is_null_pen(&xp))
		draw_rect_raw(pct,&xp,x1,y1,x2,y2);

	parse_brush_from_style(&xb,style);
	if(!is_null_brush(&xb))
		fill_rect_raw(pct,&xb,x1,y1,x2,y2);
}

void draw_round(const context_t* pct,const wchar_t* style,const xrect_t *prt)
{
	int x1,y1,x2,y2;
	xpen_t xp = {0};
	xbrush_t xb = {0};

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	parse_pen_from_style(&xp,style);
	if(!is_null_pen(&xp))
		draw_round_raw(pct,&xp,x1,y1,x2,y2);

	parse_brush_from_style(&xb,style);
	if(!is_null_brush(&xb))
		fill_round_raw(pct,&xb,x1,y1,x2,y2);
}

void draw_ellipse(const context_t* pct,const wchar_t* style,const xrect_t *prt)
{
	int x1,y1,x2,y2;
	xpen_t xp = {0};
	xbrush_t xb = {0};

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	parse_pen_from_style(&xp,style);
	if(!is_null_pen(&xp))
		draw_ellipse_raw(pct,&xp,x1,y1,x2,y2);

	parse_brush_from_style(&xb,style);
	if(!is_null_brush(&xb))
		fill_ellipse_raw(pct,&xb,x1,y1,x2,y2);
}

void draw_text(const context_t* pct,const wchar_t* style,const xrect_t *prt,const wchar_t* txt,int len)
{
	int x1,y1,x2,y2;
	xfont_t xf = {0};

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	parse_font_from_style(&xf,style);
	draw_text_raw(pct,&xf,x1,y1,x2,y2,txt,len);
}

void  draw_triangle(const context_t* pct,const wchar_t* style,const xrect_t *prt,const wchar_t* ori)
{
	int x1,y1,x2,y2;
	xpen_t xp = {0};
	xbrush_t xb = {0};

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	parse_pen_from_style(&xp,style);
	if(!is_null_pen(&xp))
		draw_triangle_raw(pct,&xp,x1,y1,x2,y2,ori);

	parse_brush_from_style(&xb,style);
	if(!is_null_brush(&xb))
		fill_triangle_raw(pct,&xb,x1,y1,x2,y2,ori);
}

void  draw_pie(const context_t* pct,const wchar_t* style,const xrect_t *prt,double fang,double tang)
{
	int x1,y1,x2,y2;
	xpen_t xp = {0};
	xbrush_t xb = {0};

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	parse_pen_from_style(&xp,style);
	if(!is_null_pen(&xp))
		draw_pie_raw(pct,&xp,x1,y2,x2,y2,fang,tang);

	parse_brush_from_style(&xb,style);
	if(!is_null_brush(&xb))
		fill_pie_raw(pct,&xb,x1,y1,x2,y2,fang,tang);
}

/*
void draw_shape(const context_t* pct,const xpen_t* pxp,const xrect_t *prt,int shape)
{
	int x1,y1,x2,y2;

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	draw_shape_raw(pct,pxp,x1,y1,x2,y2,shape);
}

void fill_shape(const context_t* pct,const xbrush_t* pxb,const xrect_t *prt,int shape)
{
	int x1,y1,x2,y2;

	x1 = mm_to_pt(pct,prt->x,1);
	x2 = mm_to_pt(pct,prt->x + prt->w,1);
	y1 = mm_to_pt(pct,prt->y,0);
	y2 = mm_to_pt(pct,prt->y + prt->h,0);

	fill_shape_raw(pct,pxb,x1,y1,x2,y2,shape);
}
*/