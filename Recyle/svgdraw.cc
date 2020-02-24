/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc svg document

	@module	svgdoc.c | svg document implement file

	@devnote 张文权 2003.01 - 2007.12 v3.0
	@devnote 张文权 2008.01 - 2009.12 v4.0
*************************************************************/

/************************************************************
*************************************************************/
#include "svgdraw.h"
#include "xdlattr.h"
#include "xdlsvg.h"
#include "xdldom.h"
#include "xdlutil.h"
#include "xdldraw.h"
#include "xdlstr.h"
#include "xdlmem.h"

void write_line_to_svg_node(link_t_ptr glk,const xpoint_t* ppt1,const xpoint_t* ppt2)
{
	wchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_LINE,-1);

	xsprintf(token,UN("%fmm"),ppt1->x);
	set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);

	xsprintf(token,UN("%fmm"),ppt1->y);
	set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);

	xsprintf(token,UN("%fmm"),ppt2->x);
	set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);

	xsprintf(token,UN("%fmm"),ppt2->y);
	set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);
}

void read_line_from_svg_node(link_t_ptr glk,xpoint_t* ppt1,xpoint_t* ppt2)
{
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_LINE) != 0)
		return;

	ppt1->x = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_X1);
	ppt1->y = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_Y1);
	ppt2->x = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_X2);
	ppt2->y = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_Y2);
}

void write_polyline_to_svg_node(link_t_ptr glk,const xpoint_t* ppt,int n)
{
	wchar_t token[2 * NUM_LEN + 1];
	wchar_t* str;

	set_dom_node_name(glk,SVG_NODE_POLYLINE,-1);

	str = (wchar_t*)xmem_alloc(NUM_LEN * 2 * n * sizeof(wchar_t));
	for(int i=0;i<n;i++)
	{
		xsprintf(token,UN("%fmm,%fmm "),ppt[i].x,ppt[i].y);
		xscat(str,token);
	}
	set_dom_node_attr(glk,SVG_ATTR_POINTS,-1,str,-1);
	xmem_free(str);
}

int read_polyline_from_svg_node(link_t_ptr glk,xpoint_t* ppt,int n)
{
	const wchar_t* str;
	wchar_t* token;
	wchar_t* xpt;
	wchar_t* ypt;
	int len,xptlen,yptlen,count = 0;
	wchar_t tmp[NUM_LEN + 1];

	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_POLYLINE) != 0)
		return 0;

	str = get_dom_node_attr_ptr(glk,SVG_ATTR_POINTS,-1);
	if(is_null(str))
		return 0;

	len = xslen(str);

	token = (wchar_t*)str;
	while(*token && len)
	{
		xpt = token;
		while(*token != L',' && *token != L'\0')
		{
			token ++;
			len --;
		}
		xptlen = (int)(token - xpt);

		if(token == L'\0')
			break;

		token ++; //skip item feed
		len --;
		
		ypt = token;
		while(*token != L' ' && *token != L'\0')
		{
			token ++;
			len --;
		}
		yptlen = (int)(token - ypt);

		if(ppt && count < n)
		{
			xsncpy(tmp,xpt,xptlen);
			ppt[count].x = (float)xstof(tmp);

			xsncpy(tmp,ypt,yptlen);
			ppt[count].y = (float)xstof(tmp);
		}

		count ++;

		if(*token == L'\0')
			break;

		token ++; //kip line feed
		len --;
	}

	return count;
}

void write_polygon_to_svg_node(link_t_ptr glk,const xpoint_t* ppt,int n)
{
	wchar_t token[2 * NUM_LEN + 1];
	wchar_t* str;

	set_dom_node_name(glk,SVG_NODE_POLYGON,-1);

	str = (wchar_t*)xmem_alloc(NUM_LEN * 2 * n * sizeof(wchar_t));
	for(int i=0;i<n;i++)
	{
		xsprintf(token,UN("%fmm,%fmm "),ppt[i].x,ppt[i].y);
		xscat(str,token);
	}
	set_dom_node_attr(glk,SVG_ATTR_POINTS,-1,str,-1);
	xmem_free(str);
}

int read_polygon_from_svg_node(link_t_ptr glk,xpoint_t* ppt,int n)
{
	const wchar_t* str;
	wchar_t* token;
	wchar_t* xpt;
	wchar_t* ypt;
	int len,xptlen,yptlen,count = 0;
	wchar_t tmp[NUM_LEN + 1];

	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_POLYGON) != 0)
		return 0;

	str = get_dom_node_attr_ptr(glk,SVG_ATTR_POINTS,-1);
	if(is_null(str))
		return 0;

	len = xslen(str);

	token = (wchar_t*)str;
	while(*token && len)
	{
		xpt = token;
		while(*token != L',' && *token != L'\0')
		{
			token ++;
			len --;
		}
		xptlen = (int)(token - xpt);

		if(token == L'\0')
			break;

		token ++; //skip item feed
		len --;
		
		ypt = token;
		while(*token != L' ' && *token != L'\0')
		{
			token ++;
			len --;
		}
		yptlen = (int)(token - ypt);

		if(ppt && count < n)
		{
			xsncpy(tmp,xpt,xptlen);
			ppt[count].x = (float)xstof(tmp);
			xsncpy(tmp,ypt,yptlen);
			ppt[count].y = (float)xstof(tmp);
		}

		count ++;

		if(*token == L'\0')
			break;

		token ++; //kip line feed
		len --;
	}

	return count;
}

void write_rect_to_svg_node(link_t_ptr glk,const xrect_t* prt)
{
	wchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_RECT,-1);

	xsprintf(token,UN("%fmm"),prt->x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);
}

void read_rect_from_svg_node(link_t_ptr glk,xrect_t* prt)
{
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_RECT) != 0)
		return;

	prt->x = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_X);
	prt->y = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_Y);
	prt->w = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_WIDTH);
	prt->h = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_HEIGHT);
}


void write_ellipse_to_svg_node(link_t_ptr glk,const xrect_t* prt)
{
	wchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_CIRCLE,-1);

	xsprintf(token,UN("%fmm"),prt->x + prt->w / 2);
	set_dom_node_attr(glk,SVG_ATTR_CX,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->y + prt->h / 2);
	set_dom_node_attr(glk,SVG_ATTR_CY,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->w / 2);
	set_dom_node_attr(glk,SVG_ATTR_RX,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->h / 2);
	set_dom_node_attr(glk,SVG_ATTR_RY,-1,token,-1);
}

void read_ellipse_from_svg_node(link_t_ptr glk,xrect_t* prt)
{
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_CIRCLE) != 0)
		return;

	prt->x = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_CX) - (float)get_dom_node_attr_numeric(glk,SVG_ATTR_RX);
	prt->y = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_CY) - (float)get_dom_node_attr_numeric(glk,SVG_ATTR_RY);
	prt->w = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_RX) * 2;
	prt->h = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_RY) * 2;
}

void write_round_to_svg_node(link_t_ptr glk,const xrect_t* prt)
{
	wchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_RECT,-1);

	xsprintf(token,UN("%fmm"),prt->x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);

	xsprintf(token,UN("%fmm"),0.1);
	set_dom_node_attr(glk,SVG_ATTR_RX,-1,token,-1);

	xsprintf(token,UN("%fmm"),0.1);
	set_dom_node_attr(glk,SVG_ATTR_RY,-1,token,-1);
}

void read_round_from_svg_node(link_t_ptr glk,xrect_t* prt)
{
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_RECT) != 0)
		return;

	prt->x = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_X);
	prt->y = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_Y);
	prt->w = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_WIDTH);
	prt->h = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_HEIGHT);
}

void write_text_to_svg_node(link_t_ptr glk,const xrect_t* prt,const wchar_t* text,int len)
{
	wchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_TEXT,-1);

	xsprintf(token,UN("%fmm"),prt->x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);

	xsprintf(token,UN("%fmm"),prt->h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);

	set_dom_node_text(glk,text,len);
}

const wchar_t* read_text_from_svg_node(link_t_ptr glk,xrect_t* prt)
{
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_TEXT) != 0)
		return NULL;

	prt->x = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_X);
	prt->y = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_Y);
	prt->w = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_WIDTH);
	prt->h = (float)get_dom_node_attr_numeric(glk,SVG_ATTR_HEIGHT);

	return get_dom_node_text_ptr(glk);
}

void write_shape_to_svg_node(link_t_ptr glk,const xrect_t* prt,const wchar_t* shape)
{
	xpoint_t xp1,xp2;

	if(xsicmp(shape,ATTR_SHAPE_RECT) == 0)
	{
		write_rect_to_svg_node(glk,prt);
	}else if(xsicmp(shape,ATTR_SHAPE_ELLIPSE) == 0)
	{
		write_ellipse_to_svg_node(glk,prt);
	}else if(xsicmp(shape,ATTR_SHAPE_ROUND) == 0)
	{
		write_round_to_svg_node(glk,prt);
	}else if(xsicmp(shape,ATTR_SHAPE_LEFTLINE) == 0)
	{
		xp1.x = prt->x;
		xp1.y = prt->y;
		xp2.x = prt->x;
		xp2.y = prt->y + prt->h;
		
		write_line_to_svg_node(glk,&xp1,&xp2);
	}else if(xsicmp(shape,ATTR_SHAPE_TOPLINE) == 0)
	{
		xp1.x = prt->x;
		xp1.y = prt->y;
		xp2.x = prt->x + prt->w;
		xp2.y = prt->y;
		
		write_line_to_svg_node(glk,&xp1,&xp2);
	}else if(xsicmp(shape,ATTR_SHAPE_RIGHTLINE) == 0)
	{
		xp1.x = prt->x + prt->w;
		xp1.y = prt->y;
		xp2.x = prt->x + prt->w;
		xp2.y = prt->y + prt->h;
		
		write_line_to_svg_node(glk,&xp1,&xp2);
	}else if(xsicmp(shape,ATTR_SHAPE_BOTTOMLINE) == 0)
	{
		xp1.x = prt->x;
		xp1.y = prt->y + prt->h;
		xp2.x = prt->x + prt->w;
		xp2.y = prt->y + prt->h;
		
		write_line_to_svg_node(glk,&xp1,&xp2);
	}
}

void draw_svg_node(const context_t* pct,link_t_ptr glk)
{
	link_t_ptr nlk;
	const wchar_t* node_name;
	const wchar_t* style;
	xrect_t xr = {0};
	xpoint_t* ppt;
	int n;

	nlk = get_dom_child_node(glk);
	while(nlk)
	{
		node_name = get_dom_node_name_ptr(nlk);
		style = get_dom_node_attr_ptr(nlk,ATTR_STYLE,-1);

		if(xsicmp(node_name,SVG_NODE_LINE) == 0)
		{
			ppt = (xpoint_t*)xmem_alloc(2 * sizeof(xpoint_t));

			read_line_from_svg_node(nlk,&ppt[0],&ppt[1]);

			draw_line(pct,style,&ppt[0],&ppt[1]);

			xmem_free(ppt);

		}else if(xsicmp(node_name,SVG_NODE_RECT) == 0)
		{
			read_rect_from_svg_node(nlk,&xr);

			draw_rect(pct,style,&xr);
		}else if(xsicmp(node_name,SVG_NODE_CIRCLE) == 0)
		{
			read_ellipse_from_svg_node(nlk,&xr);
			draw_ellipse(pct,style,&xr);
		}else if(xsicmp(node_name,SVG_NODE_ELLIPSE) == 0)
		{
			read_ellipse_from_svg_node(nlk,&xr);
			draw_ellipse(pct,style,&xr);
		}else if(xsicmp(node_name,SVG_NODE_POLYLINE) == 0)
		{
			n = read_polyline_from_svg_node(nlk,NULL,MAX_LONG);
			ppt = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));

			read_polyline_from_svg_node(nlk,ppt,n);
			draw_polyline(pct,style,ppt,n);

			xmem_free(ppt);
		}else if(xsicmp(node_name,SVG_NODE_POLYGON) == 0)
		{
			n = read_polygon_from_svg_node(nlk,NULL,MAX_LONG);
			ppt = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));

			read_polygon_from_svg_node(nlk,ppt,n);
			draw_polygon(pct,style,ppt,n);

			xmem_free(ppt);
		}else if(xsicmp(node_name,SVG_NODE_TEXT) == 0)
		{
			read_rect_from_svg_node(nlk,&xr);
			draw_text(pct,style,&xr,get_dom_node_text_ptr(nlk),-1);
		}else if(xsicmp(node_name,SVG_NODE_IMAGE) == 0)
		{
		}

		nlk = get_dom_next_sibling_node(nlk);
	}
}

void draw_svg_doc(const context_t* pct,link_t_ptr ptr)
{
	link_t_ptr glk;
	const wchar_t* node_name;

	glk = get_dom_child_node(ptr);
	while(glk)
	{
		node_name = get_dom_node_name_ptr(glk);

		if(xsicmp(node_name,SVG_NODE_G) == 0)
			draw_svg_node(pct,glk);
			
		glk = get_dom_next_sibling_node(glk);
	}
}

/*

void write_line_to_svg_node(link_t_ptr glk,const context_t* pct,const xpoint_t* prt1,const xpoint_t* prt2,const xpen_t* pxp)
{
	wchar_t token[NUM_LEN + 1];
	int x1,x2,y1,y2;

	x1 = mm_to_pt(pct,prt1->x,1);
	y1 = mm_to_pt(pct,prt1->y,0);
	x2 = mm_to_pt(pct,prt2->x,1);
	y2 = mm_to_pt(pct,prt2->y,0);

	xsprintf(token,UN("%d"),x1);
	set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);

	xsprintf(token,UN("%d"),y1);
	set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);

	xsprintf(token,UN("%d"),x2);
	set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);

	xsprintf(token,UN("%d"),y2);
	set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);

	if(pxp)
		write_pen_to_svg_node(glk,pct,pxp);
}

void read_line_from_svg_node(link_t_ptr glk,const context_t* pct,xpoint_t* prt1,xpoint_t* prt2,xpen_t* pxp)
{
	int x1,x2,y1,y2;

	x1 = get_dom_node_attr_long(glk,SVG_ATTR_X1);
	y1 = get_dom_node_attr_long(glk,SVG_ATTR_Y1);
	x2 = get_dom_node_attr_long(glk,SVG_ATTR_X2);
	y2 = get_dom_node_attr_long(glk,SVG_ATTR_Y2);

	prt1->x = pt_to_mm(pct,x1,1);
	prt1->y = pt_to_mm(pct,y1,0);
	prt2->x = pt_to_mm(pct,x2,1);
	prt2->y = pt_to_mm(pct,y2,0);

	if(pxp)
		read_pen_from_svg_node(glk,pct,pxp);
}

void write_polyline_to_svg_node(link_t_ptr glk,const context_t* pct,const xpoint_t* ppt,int n,const xpen_t* pxp)
{
	wchar_t token[2 * NUM_LEN + 1];
	wchar_t* str;
	int x,y;

	str = (wchar_t*)xmem_alloc(NUM_LEN * 2 * n * sizeof(wchar_t));
	for(int i=0;i<n;i++)
	{
		x = mm_to_pt(pct,ppt[i].x,1);
		y = mm_to_pt(pct,ppt[i].y,0);

		xsprintf(token,UN("%d,%d "),x,y);
		xscat(str,token);
	}
	set_dom_node_attr(glk,SVG_ATTR_POINTS,-1,str,-1);
	xmem_free(str);

	if(pxp)
		write_pen_to_svg_node(glk,pct,pxp);
}

int read_polyline_from_svg_node(link_t_ptr glk,const context_t* pct,xpoint_t* ppt,int n,xpen_t* pxp)
{
	const wchar_t* str;
	wchar_t* token;
	wchar_t* xpt;
	wchar_t* ypt;
	int len,xp,yp,xptlen,yptlen,count = 0;
	wchar_t tmp[NUM_LEN + 1];

	str = get_dom_node_attr_ptr(glk,SVG_ATTR_POINTS,-1);
	if(is_null(str))
		return 0;

	len = xslen(str);

	token = (wchar_t*)str;
	while(*token && len)
	{
		xpt = token;
		while(*token != L',' && *token != L'\0')
		{
			token ++;
			len --;
		}
		xptlen = (int)(token - xpt);

		if(token == L'\0')
			break;

		token ++; //skip item feed
		len --;
		
		ypt = token;
		while(*token != L' ' && *token != L'\0')
		{
			token ++;
			len --;
		}
		yptlen = (int)(token - ypt);

		if(ppt && count < n)
		{
			xsncpy(tmp,xpt,xptlen);
			xp = xstol(tmp);
			xsncpy(tmp,ypt,yptlen);
			yp = xstol(tmp);

			ppt[count].x = pt_to_mm(pct,xp,1);
			ppt[count].y = pt_to_mm(pct,yp,0);
		}

		count ++;

		if(*token == L'\0')
			break;

		token ++; //kip line feed
		len --;
	}

	if(pxp)
		read_pen_from_svg_node(glk,pct,pxp);

	return count;
}

void write_polygon_to_svg_node(link_t_ptr glk,const context_t* pct,const xpoint_t* ppt,int n,const xpen_t* pxp,const xbrush_t* pxb)
{
	wchar_t token[2 * NUM_LEN + 1];
	wchar_t* str;
	int x,y;

	str = (wchar_t*)xmem_alloc(NUM_LEN * 2 * n * sizeof(wchar_t));
	for(int i=0;i<n;i++)
	{
		x = mm_to_pt(pct,ppt[i].x,1);
		y = mm_to_pt(pct,ppt[i].y,0);

		xsprintf(token,UN("%d,%d "),x,y);
		xscat(str,token);
	}
	set_dom_node_attr(glk,SVG_ATTR_POINTS,-1,str,-1);
	xmem_free(str);

	if(pxp)
		write_pen_to_svg_node(glk,pct,pxp);

	if(pxb)
		write_brush_to_svg_node(glk,pct,pxb);
}

int read_polygon_from_svg_node(link_t_ptr glk,const context_t* pct,xpoint_t* ppt,int n,xpen_t* pxp,xbrush_t* pxb)
{
	const wchar_t* str;
	wchar_t* token;
	wchar_t* xpt;
	wchar_t* ypt;
	int len,xp,yp,xptlen,yptlen,count = 0;
	wchar_t tmp[NUM_LEN + 1];

	str = get_dom_node_attr_ptr(glk,SVG_ATTR_POINTS,-1);
	if(is_null(str))
		return 0;

	len = xslen(str);

	token = (wchar_t*)str;
	while(*token && len)
	{
		xpt = token;
		while(*token != L',' && *token != L'\0')
		{
			token ++;
			len --;
		}
		xptlen = (int)(token - xpt);

		if(token == L'\0')
			break;

		token ++; //skip item feed
		len --;
		
		ypt = token;
		while(*token != L' ' && *token != L'\0')
		{
			token ++;
			len --;
		}
		yptlen = (int)(token - ypt);

		if(ppt && count < n)
		{
			xsncpy(tmp,xpt,xptlen);
			xp = xstol(tmp);
			xsncpy(tmp,ypt,yptlen);
			yp = xstol(tmp);

			ppt[count].x = pt_to_mm(pct,xp,1);
			ppt[count].y = pt_to_mm(pct,yp,0);
		}

		count ++;

		if(*token == L'\0')
			break;

		token ++; //kip line feed
		len --;
	}

	if(pxp)
		read_pen_from_svg_node(glk,pct,pxp);

	if(pxb)
		read_brush_from_svg_node(glk,pct,pxb);

	return count;
}

void write_rect_to_svg_node(link_t_ptr glk,const context_t* pct,const xrect_t* prt,const xpen_t* pxp,const xbrush_t* pxb)
{
	wchar_t token[NUM_LEN + 1];
	int x,y,w,h;

	x = mm_to_pt(pct,prt->x,1);
	y = mm_to_pt(pct,prt->y,0);
	w = mm_to_pt(pct,prt->w,1);
	h = mm_to_pt(pct,prt->h,0);

	xsprintf(token,UN("%d"),x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);

	xsprintf(token,UN("%d"),y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);

	xsprintf(token,UN("%d"),w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);

	xsprintf(token,UN("%d"),h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);

	if(pxp)
		write_pen_to_svg_node(glk,pct,pxp);

	if(pxb)
		write_brush_to_svg_node(glk,pct,pxb);
}

void read_rect_from_svg_node(link_t_ptr glk,const context_t* pct,xrect_t* prt,xpen_t* pxp,xbrush_t* pxb)
{
	int x,y,w,h;

	x = get_dom_node_attr_long(glk,SVG_ATTR_X);
	y = get_dom_node_attr_long(glk,SVG_ATTR_Y);
	w = get_dom_node_attr_long(glk,SVG_ATTR_WIDTH);
	h = get_dom_node_attr_long(glk,SVG_ATTR_HEIGHT);

	prt->x = pt_to_mm(pct,x,1);
	prt->y = pt_to_mm(pct,y,0);
	prt->w = pt_to_mm(pct,w,1);
	prt->h = pt_to_mm(pct,h,0);

	if(pxp)
		read_pen_from_svg_node(glk,pct,pxp);

	if(pxb)
		read_brush_from_svg_node(glk,pct,pxb);
}


void write_ellipse_to_svg_node(link_t_ptr glk,const context_t* pct,const xrect_t* prt,const xpen_t* pxp,const xbrush_t* pxb)
{
	wchar_t token[NUM_LEN + 1];
	int cx,cy,rx,ry;

	cx = mm_to_pt(pct,prt->x + prt->w / 2,1);
	cy = mm_to_pt(pct,prt->y + prt->h / 2,0);
	rx = mm_to_pt(pct,prt->w / 2,1);
	ry = mm_to_pt(pct,prt->h / 2,0);

	xsprintf(token,UN("%d"),cx);
	set_dom_node_attr(glk,SVG_ATTR_CX,-1,token,-1);

	xsprintf(token,UN("%d"),cy);
	set_dom_node_attr(glk,SVG_ATTR_CY,-1,token,-1);

	xsprintf(token,UN("%d"),rx);
	set_dom_node_attr(glk,SVG_ATTR_RX,-1,token,-1);

	xsprintf(token,UN("%d"),ry);
	set_dom_node_attr(glk,SVG_ATTR_RY,-1,token,-1);

	if(pxp)
		write_pen_to_svg_node(glk,pct,pxp);

	if(pxb)
		write_brush_to_svg_node(glk,pct,pxb);
}

void read_ellipse_from_svg_node(link_t_ptr glk,const context_t* pct,xrect_t* prt,xpen_t* pxp,xbrush_t* pxb)
{
	int cx,cy,rx,ry;

	cx = get_dom_node_attr_long(glk,SVG_ATTR_CX);
	cy = get_dom_node_attr_long(glk,SVG_ATTR_CY);
	rx = get_dom_node_attr_long(glk,SVG_ATTR_RX);
	ry = get_dom_node_attr_long(glk,SVG_ATTR_RY);

	prt->x = pt_to_mm(pct,cx - rx,1);
	prt->y = pt_to_mm(pct,cy - ry,0);
	prt->w = pt_to_mm(pct,cx + rx,1);
	prt->h = pt_to_mm(pct,cx + ry,0);

	if(pxp)
		read_pen_from_svg_node(glk,pct,pxp);

	if(pxb)
		read_brush_from_svg_node(glk,pct,pxb);
}

void write_round_to_svg_node(link_t_ptr glk,const context_t* pct,const xrect_t* prt,const xpen_t* pxp,const xbrush_t* pxb)
{
	wchar_t token[NUM_LEN + 1];
	int x,y,rx,ry,w,h;

	x = mm_to_pt(pct,prt->x,1);
	y = mm_to_pt(pct,prt->y,0);
	w = mm_to_pt(pct,prt->w,1);
	h = mm_to_pt(pct,prt->h,0);
	rx = 10;
	ry = 10;

	xsprintf(token,UN("%d"),x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);

	xsprintf(token,UN("%d"),y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);

	xsprintf(token,UN("%d"),w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);

	xsprintf(token,UN("%d"),h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);

	xsprintf(token,UN("%d"),rx);
	set_dom_node_attr(glk,SVG_ATTR_RX,-1,token,-1);

	xsprintf(token,UN("%d"),ry);
	set_dom_node_attr(glk,SVG_ATTR_RY,-1,token,-1);

	if(pxp)
		write_pen_to_svg_node(glk,pct,pxp);

	if(pxb)
		write_brush_to_svg_node(glk,pct,pxb);
}

void read_round_from_svg_node(link_t_ptr glk,const context_t* pct,xrect_t* prt,xpen_t* pxp,xbrush_t* pxb)
{
	int x,y,rx,ry,w,h;

	x = get_dom_node_attr_long(glk,SVG_ATTR_X);
	y = get_dom_node_attr_long(glk,SVG_ATTR_Y);
	w = get_dom_node_attr_long(glk,SVG_ATTR_WIDTH);
	h = get_dom_node_attr_long(glk,SVG_ATTR_HEIGHT);
	rx = get_dom_node_attr_long(glk,SVG_ATTR_RX);
	ry = get_dom_node_attr_long(glk,SVG_ATTR_RY);

	prt->x = pt_to_mm(pct,x,1);
	prt->y = pt_to_mm(pct,y,0);
	prt->w = pt_to_mm(pct,w,1);
	prt->h = pt_to_mm(pct,h,0);

	if(pxp)
		read_pen_from_svg_node(glk,pct,pxp);

	if(pxb)
		read_brush_from_svg_node(glk,pct,pxb);
}
/*
void write_shape_to_svg_node(const wchar_t* shape,const xrect_t* pxr,link_t_ptr glk)
{
	wchar_t token[NUM_LEN + 1] = {0};
	if(compare_text(shape,-1,ATTR_SHAPE_LEFTLINE,-1,0) == 0)
	{
		set_dom_node_name(glk,SVG_NODE_LINE,-1);

		xsprintf(token,L"%.2f",pxr->x);
		set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y);
		set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->x);
		set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y + pxr->h);
		set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);
	}else if(compare_text(shape,-1,ATTR_SHAPE_TOPLINE,-1,0) == 0)
	{
		set_dom_node_name(glk,SVG_NODE_LINE,-1);

		xsprintf(token,L"%.2f",pxr->x);
		set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y);
		set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->x + pxr->w);
		set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y);
		set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);
	}else if(compare_text(shape,-1,ATTR_SHAPE_RIGHTLINE,-1,0) == 0)
	{
		set_dom_node_name(glk,SVG_NODE_LINE,-1);

		xsprintf(token,L"%.2f",pxr->x + pxr->w);
		set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y);
		set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->x + pxr->w);
		set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y + pxr->h);
		set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);	
	}else if(compare_text(shape,-1,ATTR_SHAPE_BOTTOMLINE,-1,0) == 0)
	{
		set_dom_node_name(glk,SVG_NODE_LINE,-1);

		xsprintf(token,L"%.2f",pxr->x);
		set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y + pxr->h);
		set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->x + pxr->w);
		set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y + pxr->h);
		set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);	
	}else if(compare_text(shape,-1,ATTR_SHAPE_RECT,-1,0) == 0)
	{
		set_dom_node_name(glk,SVG_NODE_RECT,-1);

		xsprintf(token,L"%.2f",pxr->x);
		set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y);
		set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->w);
		set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->h);
		set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);	
	}else if(compare_text(shape,-1,ATTR_SHAPE_ELLIPSE,-1,0) == 0)
	{
		set_dom_node_name(glk,SVG_NODE_ELLIPSE,-1);

		xsprintf(token,L"%.2f",pxr->x + pxr->w / 2);
		set_dom_node_attr(glk,SVG_ATTR_CX,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->y + pxr->h / 2);
		set_dom_node_attr(glk,SVG_ATTR_CY,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->w / 2);
		set_dom_node_attr(glk,SVG_ATTR_RX,-1,token,-1);
		xsprintf(token,L"%.2f",pxr->h / 2);
		set_dom_node_attr(glk,SVG_ATTR_RY,-1,token,-1);
	}else
	{
		set_dom_node_name(glk,SVG_NODE_NONE,-1);
	}
}

void write_brush_style_to_svg_node(link_t_ptr attr,link_t_ptr glk)
{
	set_dom_node_attr(glk,SVG_ATTR_FILL_STYLE,-1,get_hash_attr_ptr(attr,ATTR_BRUSH_STYLE,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_FILL_MODE,-1,get_hash_attr_ptr(attr,ATTR_BRUSH_MODE,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_FILL_COLOR,-1,get_hash_attr_ptr(attr,ATTR_BRUSH_COLOR,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_STOP_COLOR,-1,get_hash_attr_ptr(attr,ATTR_LINEAR_COLOR,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_FILL_OPACITY,-1,get_hash_attr_ptr(attr,ATTR_BRUSH_OPACITY,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_FILL_IMAGE,-1,get_hash_attr_ptr(attr,ATTR_BRUSH_IMAGE,-1),-1);
}


void write_font_style_to_svg_node(link_t_ptr attr,link_t_ptr glk)
{
	set_dom_node_attr(glk,SVG_ATTR_FONT_STYLE,-1,get_hash_attr_ptr(attr,ATTR_FONT_STYLE,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_FONT_SIZE,-1,get_hash_attr_ptr(attr,ATTR_FONT_SIZE,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_FONT_FAMILY,-1,get_hash_attr_ptr(attr,ATTR_FONT_FAMILY,-1),-1);
}



void write_line_style_to_svg_node(link_t_ptr attr,const xpoint_t* ptt_src,const xpoint_t* ptt_dest,link_t_ptr glk)
{
	wchar_t token[NUM_LEN + 1];
	
	assert(attr && attr->tag == lkHashTable);
	assert(glk && glk->tag == lkXmlNode);
	
	
	set_dom_node_name(glk,SVG_NODE_LINE,-1);
	
	xsprintf(token,_T("%.2f"),ptt_src->x);
	set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);
	xsprintf(token,_T("%.2f"),ptt_src->y);
	set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);
	xsprintf(token,_T("%.2f"),ptt_dest->x);
	set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);
	xsprintf(token,_T("%.2f"),ptt_dest->y);
	set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);

	write_pen_style_to_svg_node(attr,glk);
	set_dom_node_attr(glk,SVG_ATTR_STROKE_LINECAP,-1,get_hash_attr_ptr(attr,ATTR_START_CAP,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_STROKE_TAILCAP,-1,get_hash_attr_ptr(attr,ATTR_END_CAP,-1),-1);
}

void write_text_style_to_svg_node(link_t_ptr attr,const xrect_t* pxr,link_t_ptr glk)
{
	wchar_t token[NUM_LEN + 1];

	assert(attr && attr->tag == lkHashTable);
	assert(glk && glk->tag == lkXmlNode);
	
	set_dom_node_name(glk,SVG_NODE_TEXT,-1);
	
	xsprintf(token,_T("%.2f"),pxr->x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);
	xsprintf(token,_T("%.2f"),pxr->y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);
	xsprintf(token,_T("%.2f"),pxr->w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);
	xsprintf(token,_T("%.2f"),pxr->h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);	

	write_font_style_to_svg_node(attr,glk);

	set_dom_node_attr(glk,SVG_ATTR_TEXT_COLOR,-1,get_hash_attr_ptr(attr,ATTR_TEXT_COLOR,-1),-1);

	set_dom_node_attr(glk,SVG_ATTR_TEXT_ALIGN,-1,get_hash_attr_ptr(attr,ATTR_TEXT_ALIGN,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_LINE_ALIGN,-1,get_hash_attr_ptr(attr,ATTR_LINE_ALIGN,-1),-1);
}

void write_image_style_to_svg_node(link_t_ptr attr,const xrect_t* pxr,link_t_ptr glk)
{
	wchar_t token[NUM_LEN + 1];

	assert(attr && attr->tag == lkHashTable);
	assert(glk && glk->tag == lkXmlNode);
	
	set_dom_node_name(glk,SVG_NODE_IMAGE,-1);
	
	xsprintf(token,_T("%.2f"),pxr->x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);
	xsprintf(token,_T("%.2f"),pxr->y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);
	xsprintf(token,_T("%.2f"),pxr->w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);
	xsprintf(token,_T("%.2f"),pxr->h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);	

	set_dom_node_attr(glk,SVG_ATTR_IMAGE_STYLE,-1,get_hash_attr_ptr(attr,ATTR_IMAGE_STYLE,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_IMAGE_LOWCOLOR,-1,get_hash_attr_ptr(attr,ATTR_IMAGE_LOWCOLOR,-1),-1);
	set_dom_node_attr(glk,SVG_ATTR_IMAGE_HIGHCOLOR,-1,get_hash_attr_ptr(attr,ATTR_IMAGE_HIGHCOLOR,-1),-1);
}
*/
