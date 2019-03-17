/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg document

	@module	svgdoc.c | svg document implement file

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

#include "svgdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_svg_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_SVG,-1);
	set_dom_node_xmlns(ptr, XMLNS, -1, XMLNS_SVG_INSTANCE,-1);
	set_dom_node_attr(ptr, XML_ATTR_VERSION, -1, _T("1.1"), -1);

	set_svg_width(ptr, DEF_PAPER_WIDTH);
	set_svg_height(ptr, DEF_PAPER_HEIGHT);

	return ptr;
}

void destroy_svg_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_svg_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

bool_t is_svg_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_SVG, -1, 0) == 0) ? 1 : 0;
}

bool_t is_svg_node(link_t_ptr ptr, link_t_ptr ilk)
{
	return is_dom_child_node(ptr, ilk);
}

link_t_ptr insert_svg_node(link_t_ptr nlk)
{
	return insert_dom_node(nlk,LINK_LAST);
}

void delete_svg_node(link_t_ptr nlk)
{
	delete_dom_node(nlk);
}

link_t_ptr get_svg_first_child_node(link_t_ptr nlk)
{
	return get_dom_first_child_node(nlk);
}

link_t_ptr get_svg_last_child_node(link_t_ptr nlk)
{
	return get_dom_last_child_node(nlk);
}

link_t_ptr get_svg_parent_node(link_t_ptr nlk)
{
	return get_dom_parent_node(nlk);
}

link_t_ptr get_svg_next_sibling_node(link_t_ptr nlk)
{
	return get_dom_next_sibling_node(nlk);
}

link_t_ptr get_svg_prev_sibling_node(link_t_ptr nlk)
{
	return get_dom_prev_sibling_node(nlk);
}

/******************************************************************************************************/
void set_svg_width(link_t_ptr ptr, float width)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	xsprintf(token, _T("%.1fmm"), width);

	set_dom_node_attr(ptr, SVG_ATTR_WIDTH, -1, token, -1);
}

float get_svg_width(link_t_ptr ptr)
{
	return (float)get_dom_node_attr_numeric(ptr, SVG_ATTR_WIDTH);
}

void set_svg_height(link_t_ptr ptr, float height)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	xsprintf(token, _T("%.1fmm"), height);

	set_dom_node_attr(ptr, SVG_ATTR_HEIGHT, -1, token, -1);
}

float get_svg_height(link_t_ptr ptr)
{
	return (float)get_dom_node_attr_numeric(ptr, SVG_ATTR_HEIGHT);
}

void set_svg_viewbox(link_t_ptr ptr, const xrect_t* pbox)
{
	tchar_t token[NUM_LEN * 4] = { 0 };

	xsprintf(token, _T("%d %d %d %d"), pbox->x, pbox->y, pbox->w, pbox->h);

	set_dom_node_attr(ptr, SVG_ATTR_VIEWBOX, -1, token, -1);
}

void get_svg_viewbox(link_t_ptr ptr, xrect_t* pbox)
{
	tchar_t* str;
	tchar_t* key;
	int klen;

	xmem_zero((void*)pbox, sizeof(xrect_t));

	str = (tchar_t*)get_dom_node_attr_ptr(ptr, SVG_ATTR_VIEWBOX, -1);
	if (is_null(str))
		return;

	str = (tchar_t*)parse_string_token(str, -1, _T(' '), &key, &klen);
	if (str)
	{
		pbox->x = xsntol(key, klen);
	}
	else
		return;

	str = (tchar_t*)parse_string_token(str, -1, _T(' '), &key, &klen);
	if (str)
	{
		pbox->y = xsntol(key, klen);
	}
	else
		return;

	str = (tchar_t*)parse_string_token(str, -1, _T(' '), &key, &klen);
	if (str)
	{
		pbox->w = xsntol(key, klen);
	}
	else
		return;

	str = (tchar_t*)parse_string_token(str, -1, _T(' '), &key, &klen);
	if (str)
	{
		pbox->h = xsntol(key, klen);
	}
}

void write_xpen_to_svg_node(link_t_ptr nlk, const xpen_t* pxp)
{
	tchar_t token[RES_LEN + 1] = { 0 };

	if (pxp)
	{
		if (!is_null(pxp->color))
			set_dom_node_attr(nlk, SVG_ATTR_STROKE_COLOR, -1, pxp->color, -1);
		else
			set_dom_node_attr(nlk, SVG_ATTR_STROKE_COLOR, -1, GDI_ATTR_RGB_GRAY, -1);
	}

	if (pxp && !is_null(pxp->opacity))
	{
		xsprintf(token, _T("%.2f"), xstonum(pxp->opacity) / 255.0);
		set_dom_node_attr(nlk, SVG_ATTR_STROKE_OPACITY, -1, token, -1);
	}

	if (pxp)
	{
		if(!is_null(pxp->size))
			set_dom_node_attr(nlk, SVG_ATTR_STROKE_WIDTH, -1, pxp->size, -1);
		else
			set_dom_node_attr(nlk, SVG_ATTR_STROKE_WIDTH, -1, _T("1"), -1);
	}
}

void read_xpen_from_svg_node(link_t_ptr nlk, xpen_t* pxp)
{
	const tchar_t* token;

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_STROKE_COLOR, -1);
	if (!is_null(token))
	{
		xscpy(pxp->color, token);
	}

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_STROKE_OPACITY, -1);
	if (!is_null(token))
	{
		xsprintf(pxp->opacity, _T("%d"), (int)(xstof(token) * 255.0));
	}

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_STROKE_WIDTH, -1);
	if (!is_null(token))
	{
		xscpy(pxp->size, token);
	}
}

void write_xbrush_to_svg_node(link_t_ptr nlk, const xbrush_t* pxb)
{
	tchar_t token[RES_LEN + 1] = { 0 };

	if (pxb)
	{
		if (!is_null(pxb->color))
			set_dom_node_attr(nlk, SVG_ATTR_FILL_COLOR, -1, pxb->color, -1);
		else
			set_dom_node_attr(nlk, SVG_ATTR_FILL_COLOR, -1, GDI_ATTR_RGB_WHITE, -1);
	}
	else
	{
		set_dom_node_attr(nlk, SVG_ATTR_FILL_COLOR, -1, GDI_ATTR_RGB_WHITE, -1);
	}

	if (pxb && !is_null(pxb->opacity))
	{
		xsprintf(token, _T("%.2f"), xstof(pxb->opacity) / 255.0);
		set_dom_node_attr(nlk, SVG_ATTR_FILL_OPACITY, -1, token, -1);
	}
}

void read_xbrush_from_svg_node(link_t_ptr nlk, xbrush_t* pxb)
{
	const tchar_t* token;

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_FILL_COLOR, -1);
	if (!is_null(token))
	{
		xscpy(pxb->color, token);
	}
	
	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_FILL_OPACITY, -1);
	if (!is_null(token))
	{
		xsprintf(pxb->opacity, _T("%d"), (int)(xstof(token) * 255.0));
	}
}

void write_xfont_to_svg_node(link_t_ptr nlk, const xfont_t* pxf)
{
	if (pxf)
	{
		if (!is_null(pxf->size))
			set_dom_node_attr(nlk, SVG_ATTR_FONT_SIZE, -1, pxf->size, -1);
		else
			set_dom_node_attr(nlk, SVG_ATTR_FONT_SIZE, -1, GDI_ATTR_FONT_SIZE_TEXT, -1);
	}

	if (pxf)
	{
		if (!is_null(pxf->family))
			set_dom_node_attr(nlk, SVG_ATTR_FONT_FAMILY, -1, pxf->family, -1);
	}

	if (pxf)
	{
		if (!is_null(pxf->color))
			set_dom_node_attr(nlk, SVG_ATTR_TEXT_COLOR, -1, pxf->color, -1);
		else
			set_dom_node_attr(nlk, SVG_ATTR_TEXT_COLOR, -1, GDI_ATTR_RGB_BLACK, -1);
	}
}

void read_xfont_from_svg_node(link_t_ptr nlk, xfont_t* pxf)
{
	const tchar_t* token;

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_FONT_SIZE, -1);
	if (!is_null(token))
	{
		xscpy(pxf->size, token);
	}

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_FONT_FAMILY, -1);
	if (!is_null(token))
	{
		xscpy(pxf->family, token);
	}

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_TEXT_COLOR, -1);
	if (!is_null(token))
	{
		xscpy(pxf->color, token);
	}

}

void write_xface_to_svg_node(link_t_ptr nlk, const xface_t* pxa)
{
	tchar_t token[RES_LEN + 1] = { 0 };

	if (pxa)
	{
		if (!is_null(pxa->text_align))
		{
			if (compare_text(pxa->text_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
				xscpy(token, SVG_ATTR_TEXT_ALIGN_FAR);
			else if (compare_text(pxa->text_align, -1, GDI_ATTR_TEXT_ALIGN_CENTER, -1, 1) == 0)
				xscpy(token, SVG_ATTR_TEXT_ALIGN_CENTER);
			else
				xscpy(token, SVG_ATTR_TEXT_ALIGN_NEAR);
		}
		else
		{
			xscpy(token, SVG_ATTR_TEXT_ALIGN_NEAR);
		}
		set_dom_node_attr(nlk, SVG_ATTR_TEXT_ALIGN, -1, token, -1);
	}
}

void read_xface_from_svg_node(link_t_ptr nlk, xface_t* pxa)
{
	const tchar_t* token;

	token = get_dom_node_attr_ptr(nlk, SVG_ATTR_TEXT_ALIGN, -1);
	if (!is_null(token))
	{
		if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
			xscpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_FAR);
		else if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_CENTER, -1, 1) == 0)
			xscpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		else
			xscpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
	}
}

void write_ximage_to_svg_node(link_t_ptr glk, const ximage_t* pxi, const xrect_t* prt)
{
	tchar_t token[NUM_LEN + 1];
	
	tchar_t* data;
	int len;

	set_dom_node_name(glk, SVG_NODE_IMAGE, -1);

	xsprintf(token, _T("%d"), prt->x);
	set_dom_node_attr(glk, SVG_ATTR_X, -1, token, -1);

	xsprintf(token, _T("%d"), prt->y);
	set_dom_node_attr(glk, SVG_ATTR_Y, -1, token, -1);

	xsprintf(token, _T("%d"), prt->w);
	set_dom_node_attr(glk, SVG_ATTR_WIDTH, -1, token, -1);

	xsprintf(token, _T("%d"), prt->h);
	set_dom_node_attr(glk, SVG_ATTR_HEIGHT, -1, token, -1);

	len = format_ximage_to_source(pxi, NULL, MAX_LONG);
	data = xsalloc(len + 1);
	format_ximage_to_source(pxi, data, len);
	attach_dom_node_attr(glk, SVG_ATTR_IMAGE_HREF, data);
}

void read_ximage_from_svg_node(link_t_ptr glk, ximage_t* pxi, xrect_t* prt)
{
	if (xsicmp(get_dom_node_name_ptr(glk), SVG_NODE_IMAGE) != 0)
		return;

	prt->x = (get_dom_node_attr_integer(glk, SVG_ATTR_X));
	prt->y = (get_dom_node_attr_integer(glk, SVG_ATTR_Y));
	prt->w = (get_dom_node_attr_integer(glk, SVG_ATTR_WIDTH));
	prt->h = (get_dom_node_attr_integer(glk, SVG_ATTR_HEIGHT));

	parse_ximage_from_source(pxi, get_dom_node_attr_ptr(glk, SVG_ATTR_IMAGE_HREF, -1));
}

void write_line_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xpoint_t* ppt1,const xpoint_t* ppt2)
{
	tchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_LINE,-1);

	xsprintf(token,_T("%d"),ppt1->x);
	set_dom_node_attr(glk,SVG_ATTR_X1,-1,token,-1);

	xsprintf(token,_T("%d"),ppt1->y);
	set_dom_node_attr(glk,SVG_ATTR_Y1,-1,token,-1);

	xsprintf(token,_T("%d"),ppt2->x);
	set_dom_node_attr(glk,SVG_ATTR_X2,-1,token,-1);

	xsprintf(token,_T("%d"),ppt2->y);
	set_dom_node_attr(glk,SVG_ATTR_Y2,-1,token,-1);

	write_xpen_to_svg_node(glk, pxp);
}

void read_line_from_svg_node(link_t_ptr glk,xpen_t* pxp,xpoint_t* ppt1,xpoint_t* ppt2)
{
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_LINE) != 0)
		return;

	ppt1->x = (get_dom_node_attr_integer(glk, SVG_ATTR_X1));
	ppt1->y = (get_dom_node_attr_integer(glk, SVG_ATTR_Y1));
	ppt2->x = (get_dom_node_attr_integer(glk, SVG_ATTR_X2));
	ppt2->y = (get_dom_node_attr_integer(glk, SVG_ATTR_Y2));

	if (pxp)
		read_xpen_from_svg_node(glk, pxp);
}

void write_polyline_to_svg_node(link_t_ptr glk,const xpen_t* pxp, const xpoint_t* ppt,int n)
{
	tchar_t token[2 * NUM_LEN + 1];
	tchar_t* str;
	int i;

	set_dom_node_name(glk,SVG_NODE_POLYLINE,-1);

	str = xsalloc(NUM_LEN * 2 * n);
	for(i=0;i<n;i++)
	{
		xsprintf(token,_T("%d,%d "),ppt[i].x,ppt[i].y);
		xscat(str,token);
	}
	set_dom_node_attr(glk,SVG_ATTR_POINTS,-1,str,-1);
	xsfree(str);

	write_xpen_to_svg_node(glk, pxp);
}

int read_polyline_from_svg_node(link_t_ptr glk,xpen_t* pxp,xpoint_t* ppt,int n)
{
	const tchar_t* str;
	tchar_t* token;
	tchar_t* xpt;
	tchar_t* ypt;
	int len,xptlen,yptlen,count = 0;

	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_POLYLINE) != 0)
		return 0;

	str = get_dom_node_attr_ptr(glk,SVG_ATTR_POINTS,-1);
	if(is_null(str))
		return 0;

	len = xslen(str);

	token = (tchar_t*)str;
	while(*token && len)
	{
		xpt = token;
		while(*token != _T(',') && *token != _T('\0'))
		{
			token ++;
			len --;
		}
		xptlen = (int)(token - xpt);

		if(token == _T('\0'))
			break;

		token ++; //skip item feed
		len --;
		
		ypt = token;
		while(*token != _T(' ') && *token != _T('\0'))
		{
			token ++;
			len --;
		}
		yptlen = (int)(token - ypt);

		if(ppt && count < n)
		{
			ppt[count].x = xsntol(xpt, xptlen);
			ppt[count].y = xsntol(ypt, yptlen);
		}

		count ++;

		if(*token == _T('\0'))
			break;

		token ++; //kip line feed
		len --;
	}

	if (pxp)
		read_xpen_from_svg_node(glk, pxp);

	return count;
}

void write_polygon_to_svg_node(link_t_ptr glk,const xpen_t* pxp, const xbrush_t* pxb,const xpoint_t* ppt,int n)
{
	tchar_t token[2 * NUM_LEN + 1];
	tchar_t* str;
	int i;

	set_dom_node_name(glk,SVG_NODE_POLYGON,-1);

	str = xsalloc(NUM_LEN * 2 * n);
	for(i=0;i<n;i++)
	{
		xsprintf(token,_T("%d,%d "),ppt[i].x,ppt[i].y);
		xscat(str,token);
	}
	set_dom_node_attr(glk,SVG_ATTR_POINTS,-1,str,-1);
	xsfree(str);

	write_xpen_to_svg_node(glk, pxp);

	write_xbrush_to_svg_node(glk, pxb);
}

int read_polygon_from_svg_node(link_t_ptr glk,xpen_t* pxp,xbrush_t* pxb,xpoint_t* ppt,int n)
{
	const tchar_t* str;
	tchar_t* token;
	tchar_t* xpt;
	tchar_t* ypt;
	int len,xptlen,yptlen,count = 0;

	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_POLYGON) != 0)
		return 0;

	str = get_dom_node_attr_ptr(glk,SVG_ATTR_POINTS,-1);
	if(is_null(str))
		return 0;

	len = xslen(str);

	token = (tchar_t*)str;
	while(*token && len)
	{
		xpt = token;
		while(*token != _T(',') && *token != _T('\0'))
		{
			token ++;
			len --;
		}
		xptlen = (int)(token - xpt);

		if(token == _T('\0'))
			break;

		token ++; //skip item feed
		len --;
		
		ypt = token;
		while(*token != _T(' ') && *token != _T('\0'))
		{
			token ++;
			len --;
		}
		yptlen = (int)(token - ypt);

		if(ppt && count < n)
		{
			ppt[count].x = xsntol(xpt, xptlen);
			ppt[count].y = xsntol(ypt, yptlen);
		}

		count ++;

		if(*token == _T('\0'))
			break;

		token ++; //kip line feed
		len --;
	}

	if (pxp)
		read_xpen_from_svg_node(glk, pxp);

	if (pxb)
		read_xbrush_from_svg_node(glk, pxb);

	return count;
}

void write_bezier_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	tchar_t token[2 * 4 * NUM_LEN + 1];

	set_dom_node_name(glk, SVG_NODE_PATH, -1);

	xsprintf(token, _T("M%d,%d C%d,%d %d,%d %d,%d"), ppt1->x, ppt1->y, ppt2->x, ppt2->y, ppt3->x, ppt3->y, ppt4->x, ppt4->y);

	set_dom_node_attr(glk, SVG_ATTR_D, -1, token, -1);

	write_xpen_to_svg_node(glk, pxp);
}

void write_ellipse_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	tchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk, SVG_NODE_ELLIPSE, -1);

	xsprintf(token,_T("%d"),(prt->x + prt->w / 2));
	set_dom_node_attr(glk,SVG_ATTR_CX,-1,token,-1);

	xsprintf(token,_T("%d"),(prt->y + prt->h / 2));
	set_dom_node_attr(glk,SVG_ATTR_CY,-1,token,-1);

	xsprintf(token,_T("%d"),(prt->w / 2));
	set_dom_node_attr(glk,SVG_ATTR_RX,-1,token,-1);

	xsprintf(token,_T("%d"),(prt->h / 2));
	set_dom_node_attr(glk,SVG_ATTR_RY,-1,token,-1);

	write_xpen_to_svg_node(glk, pxp);

	write_xbrush_to_svg_node(glk, pxb);
}

void read_ellipse_from_svg_node(link_t_ptr glk,xpen_t* pxp,xbrush_t* pxb,xrect_t* prt)
{
	if (xsicmp(get_dom_node_name_ptr(glk), SVG_NODE_ELLIPSE) != 0)
		return;

	prt->x = (get_dom_node_attr_integer(glk, SVG_ATTR_CX) - get_dom_node_attr_integer(glk, SVG_ATTR_RX));
	prt->y = (get_dom_node_attr_integer(glk, SVG_ATTR_CY) - get_dom_node_attr_integer(glk, SVG_ATTR_RY));
	prt->w = (get_dom_node_attr_integer(glk, SVG_ATTR_RX) * 2);
	prt->h = (get_dom_node_attr_integer(glk, SVG_ATTR_RY) * 2);

	if (pxp)
		read_xpen_from_svg_node(glk, pxp);

	if (pxb)
		read_xbrush_from_svg_node(glk, pxb);
}

void write_rect_to_svg_node(link_t_ptr glk,const xpen_t* pxp, const xbrush_t* pxb,const xrect_t* prt)
{
	tchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_RECT,-1);

	xsprintf(token,_T("%d"),prt->x);
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);

	xsprintf(token,_T("%d"),prt->y);
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);

	xsprintf(token,_T("%d"),prt->w);
	set_dom_node_attr(glk,SVG_ATTR_WIDTH,-1,token,-1);

	xsprintf(token,_T("%d"),prt->h);
	set_dom_node_attr(glk,SVG_ATTR_HEIGHT,-1,token,-1);

	write_xpen_to_svg_node(glk, pxp);

	write_xbrush_to_svg_node(glk, pxb);
}

void read_rect_from_svg_node(link_t_ptr glk,xpen_t* pxp,xbrush_t* pxb,xrect_t* prt)
{
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_RECT) != 0)
		return;

	prt->x = (get_dom_node_attr_integer(glk, SVG_ATTR_X));
	prt->y = (get_dom_node_attr_integer(glk, SVG_ATTR_Y));
	prt->w = (get_dom_node_attr_integer(glk, SVG_ATTR_WIDTH));
	prt->h = (get_dom_node_attr_integer(glk, SVG_ATTR_HEIGHT));

	if (pxp)
		read_xpen_from_svg_node(glk, pxp);

	if (pxb)
		read_xbrush_from_svg_node(glk, pxb);
}

bool_t svg_node_is_round(link_t_ptr glk)
{
	return (compare_text(get_dom_node_name_ptr(glk),-1,SVG_NODE_RECT,-1,1) == 0 && is_dom_node_attr(glk, SVG_ATTR_RX, -1) && is_dom_node_attr(glk, SVG_ATTR_RY, -1));
}

void write_round_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, long rx, long ry)
{
	tchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk, SVG_NODE_RECT, -1);

	xsprintf(token, _T("%d"), prt->x);
	set_dom_node_attr(glk, SVG_ATTR_X, -1, token, -1);

	xsprintf(token, _T("%d"), prt->y);
	set_dom_node_attr(glk, SVG_ATTR_Y, -1, token, -1);

	xsprintf(token, _T("%d"), prt->w);
	set_dom_node_attr(glk, SVG_ATTR_WIDTH, -1, token, -1);

	xsprintf(token, _T("%d"), prt->h);
	set_dom_node_attr(glk, SVG_ATTR_HEIGHT, -1, token, -1);

	xsprintf(token, _T("%d"), rx);
	set_dom_node_attr(glk, SVG_ATTR_RX, -1, token, -1);

	xsprintf(token, _T("%d"), ry);
	set_dom_node_attr(glk, SVG_ATTR_RY, -1, token, -1);

	write_xpen_to_svg_node(glk, pxp);

	write_xbrush_to_svg_node(glk, pxb);
}

void read_round_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* prt, long* prx, long* pry)
{
	if (xsicmp(get_dom_node_name_ptr(glk), SVG_NODE_RECT) != 0)
		return;

	prt->x = (get_dom_node_attr_integer(glk, SVG_ATTR_X));
	prt->y = (get_dom_node_attr_integer(glk, SVG_ATTR_Y));
	prt->w = (get_dom_node_attr_integer(glk, SVG_ATTR_WIDTH));
	prt->h = (get_dom_node_attr_integer(glk, SVG_ATTR_HEIGHT));

	*prx = (get_dom_node_attr_integer(glk, SVG_ATTR_RX));
	*pry = (get_dom_node_attr_integer(glk, SVG_ATTR_RY));

	if (pxp)
		read_xpen_from_svg_node(glk, pxp);

	if (pxb)
		read_xbrush_from_svg_node(glk, pxb);
}

void write_text_to_svg_node(link_t_ptr glk, const xfont_t* pxf, const xface_t* pxa, const xrect_t* prt,const tchar_t* text,int len)
{
	tchar_t token[NUM_LEN + 1];

	set_dom_node_name(glk,SVG_NODE_TEXT,-1);

	if (compare_text(pxa->text_align,-1,GDI_ATTR_TEXT_ALIGN_CENTER,-1,1) == 0)
		xsprintf(token,_T("%d"),(prt->x + prt->w / 2));
	else if (compare_text(pxa->text_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
		xsprintf(token, _T("%d"), (prt->x + prt->w));
	else
		xsprintf(token, _T("%d"), (prt->x));
	set_dom_node_attr(glk,SVG_ATTR_X,-1,token,-1);

	if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_NEAR, -1, 1) == 0)
		xsprintf(token,_T("%d"),(prt->y + prt->h / 2));
	else if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
		xsprintf(token, _T("%d"), (prt->y + prt->h));
	else
		xsprintf(token, _T("%d"), (prt->y + prt->h / 2));
	set_dom_node_attr(glk,SVG_ATTR_Y,-1,token,-1);

	if (prt->w)
	{
		xsprintf(token, _T("%d"), prt->w);
		set_dom_node_attr(glk, SVG_ATTR_WIDTH, -1, token, -1);
	}

	if (prt->h)
	{
		xsprintf(token, _T("%d"), prt->h);
		set_dom_node_attr(glk, SVG_ATTR_HEIGHT, -1, token, -1);
	}

	if (!is_null(pxf->size))
	{
		xsprintf(token, _T("%spt"), pxf->size);
		set_dom_node_attr(glk, SVG_ATTR_FONT_SIZE, -1, token, -1);
	}

	if (!is_null(pxf->family))
	{
		set_dom_node_attr(glk, SVG_ATTR_FONT_FAMILY, -1, pxf->family, -1);
	}

	if (!is_null(pxf->color))
	{
		set_dom_node_attr(glk, SVG_ATTR_TEXT_COLOR, -1, pxf->color, -1);
	}

	if (prt->w && prt->h)
	{
		if (compare_text(pxa->text_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
			xscpy(token, SVG_ATTR_TEXT_ALIGN_FAR);
		else if (compare_text(pxa->text_align, -1, GDI_ATTR_TEXT_ALIGN_CENTER, -1, 1) == 0)
			xscpy(token, SVG_ATTR_TEXT_ALIGN_CENTER);
		else
			xscpy(token, SVG_ATTR_TEXT_ALIGN_NEAR);

		set_dom_node_attr(glk, SVG_ATTR_TEXT_ALIGN, -1, token, -1);

		if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
			xscpy(token, SVG_ATTR_TEXT_ALIGN_FAR);
		else if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_NEAR, -1, 1) == 0)
			xscpy(token, SVG_ATTR_TEXT_ALIGN_NEAR);
		else
			xscpy(token, SVG_ATTR_TEXT_ALIGN_CENTER);

		set_dom_node_attr(glk, SVG_ATTR_LINE_ALIGN, -1, token, -1);
	}

	set_dom_node_text(glk,text,len);
}

const tchar_t* read_text_from_svg_node(link_t_ptr glk,xfont_t* pxf, xface_t* pxa, xrect_t* prt)
{
	const tchar_t* token;
	
	if(xsicmp(get_dom_node_name_ptr(glk),SVG_NODE_TEXT) != 0)
		return NULL;

	prt->x = (get_dom_node_attr_integer(glk, SVG_ATTR_X));
	prt->y = (get_dom_node_attr_integer(glk, SVG_ATTR_Y));
	prt->w = (get_dom_node_attr_integer(glk, SVG_ATTR_WIDTH));
	prt->h = (get_dom_node_attr_integer(glk, SVG_ATTR_HEIGHT));

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_TEXT_ALIGN, -1);
	if (!is_null(token))
	{
		if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_CENTER, -1, 1) == 0)
			prt->x -= prt->w / 2;
		else if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
			prt->x -= prt->w;
	}

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_LINE_ALIGN, -1);
	if (!is_null(token))
	{
		if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_NEAR, -1, 1) == 0)
			prt->y -= prt->h / 2;
		else if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_CENTER, -1, 1) == 0)
			prt->y -= prt->h / 2;
		else
			prt->y -= prt->h;
	}

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_FONT_SIZE, -1);
	if (!is_null(token))
	{
		xscpy(pxf->size, token);
	}

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_FONT_FAMILY, -1);
	if (!is_null(token))
	{
		xscpy(pxf->family, token);
	}

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_TEXT_COLOR, -1);
	if (!is_null(token))
	{
		xscpy(pxf->color, token);
	}

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_TEXT_ALIGN, -1);
	if (!is_null(token))
	{
		if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
			xscpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_FAR);
		else if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_CENTER, -1, 1) == 0)
			xscpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		else
			xscpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
	}

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_LINE_ALIGN, -1);
	if (!is_null(token))
	{
		if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_FAR, -1, 1) == 0)
			xscpy(pxa->line_align, GDI_ATTR_TEXT_ALIGN_FAR);
		else if (compare_text(token, -1, SVG_ATTR_TEXT_ALIGN_CENTER, -1, 1) == 0)
			xscpy(pxa->line_align, GDI_ATTR_TEXT_ALIGN_CENTER);
		else
			xscpy(pxa->line_align, GDI_ATTR_TEXT_ALIGN_NEAR);
	}

	return get_dom_node_text_ptr(glk);
}

void write_shape_to_svg_node(link_t_ptr glk,const xpen_t* pxp, const xbrush_t* pxb,const xrect_t* prt,const tchar_t* shape)
{
	xpoint_t pt[4] = { 0 };

	if(xsicmp(shape,ATTR_SHAPE_RECT) == 0)
	{
		write_rect_to_svg_node(glk,pxp,pxb,prt);
	}else if(xsicmp(shape,ATTR_SHAPE_ELLIPSE) == 0)
	{
		write_ellipse_to_svg_node(glk,pxp,pxb,prt);
	}else if(xsicmp(shape,ATTR_SHAPE_ROUND) == 0)
	{
		write_round_to_svg_node(glk,pxp,pxb,prt, prt->w / 10, prt->h / 10);
	}else if(xsicmp(shape,ATTR_SHAPE_LEFTLINE) == 0)
	{
		pt[0].x = prt->x;
		pt[0].y = prt->y;
		pt[1].x = prt->x;
		pt[1].y = prt->y + prt->h;
		
		write_line_to_svg_node(glk, pxp, &pt[0], &pt[1]);
	}else if(xsicmp(shape,ATTR_SHAPE_TOPLINE) == 0)
	{
		pt[0].x = prt->x;
		pt[0].y = prt->y;
		pt[1].x = prt->x + prt->w;
		pt[1].y = prt->y;
		
		write_line_to_svg_node(glk, pxp, &pt[0], &pt[1]);
	}else if(xsicmp(shape,ATTR_SHAPE_RIGHTLINE) == 0)
	{
		pt[0].x = prt->x + prt->w;
		pt[0].y = prt->y;
		pt[1].x = prt->x + prt->w;
		pt[1].y = prt->y + prt->h;
		
		write_line_to_svg_node(glk, pxp, &pt[0], &pt[1]);
	}else if(xsicmp(shape,ATTR_SHAPE_BOTTOMLINE) == 0)
	{
		pt[0].x = prt->x;
		pt[0].y = prt->y + prt->h;
		pt[1].x = prt->x + prt->w;
		pt[1].y = prt->y + prt->h;
		
		write_line_to_svg_node(glk, pxp, &pt[0], &pt[1]);
	}
	else if (xsicmp(shape, ATTR_SHAPE_TOPTRIANGLE) == 0)
	{
		pt[0].x = prt->x + prt->w / 2;
		pt[0].y = prt->y;
		pt[1].x = prt->x + prt->w;
		pt[1].y = prt->y + prt->h;
		pt[2].x = prt->x;
		pt[2].y = prt->y + prt->h;
		pt[3].x = prt->x + prt->w / 2;
		pt[3].y = prt->y;

		if (pxb)
			write_polygon_to_svg_node(glk, pxp, pxb, pt, 4);
		else
			write_polyline_to_svg_node(glk, pxp, pt, 4);
	}
	else if (xsicmp(shape, ATTR_SHAPE_BOTTOMTRIANGLE) == 0)
	{
		pt[0].x = prt->x + prt->w / 2;
		pt[0].y = prt->y + prt->h;
		pt[1].x = prt->x;
		pt[1].y = prt->y;
		pt[2].x = prt->x + prt->w;
		pt[2].y = prt->y;
		pt[3].x = prt->x + prt->w / 2;
		pt[3].y = prt->y + prt->h;

		if (pxb)
			write_polygon_to_svg_node(glk, pxp, pxb, pt, 4);
		else
			write_polyline_to_svg_node(glk, pxp, pt, 4);
	}
	else if (xsicmp(shape, ATTR_SHAPE_LEFTTRIANGLE) == 0)
	{
		pt[0].x = prt->x;
		pt[0].y = prt->y + prt->h / 2;
		pt[1].x = prt->x + prt->w;
		pt[1].y = prt->y;
		pt[2].x = prt->x + prt->w;
		pt[2].y = prt->y + prt->h;
		pt[3].x = prt->x;
		pt[3].y = prt->y + prt->h / 2;

		if (pxb)
			write_polygon_to_svg_node(glk, pxp, pxb, pt, 4);
		else
			write_polyline_to_svg_node(glk, pxp, pt, 4);
	}
	else if (xsicmp(shape, ATTR_SHAPE_RIGHTTRIANGLE) == 0)
	{
		pt[0].x = prt->x + prt->w;
		pt[0].y = prt->y + prt->h / 2;
		pt[1].x = prt->x;
		pt[1].y = prt->y + prt->h;
		pt[2].x = prt->x;
		pt[2].y = prt->y;
		pt[3].x = prt->x + prt->w;
		pt[3].y = prt->y + prt->h / 2;

		if (pxb)
			write_polygon_to_svg_node(glk, pxp, pxb, pt, 4);
		else
			write_polyline_to_svg_node(glk, pxp, pt, 4);
	}
}

bool_t svg_node_is_pie(link_t_ptr glk)
{
	const tchar_t* token;

	if (xsicmp(get_dom_node_name_ptr(glk), SVG_NODE_PATH) != 0)
		return 0;

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_D, -1);

	if (xsstr(token, _T("A")))
		return 1;
	else
		return 0;
}

void write_pie_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang)
{
	xpoint_t pt1, pt2;
	tchar_t token[10 * INT_LEN];
	int larg;

	set_dom_node_name(glk, SVG_NODE_PATH, -1);

	pt1.x = (long)((float)pxr->w / 2 * cos(fang)) + pxr->x + pxr->w / 2;
	pt1.y = -(long)((float)pxr->h / 2 * sin(fang)) + pxr->y + pxr->h / 2;

	pt2.x = (long)((float)pxr->w / 2 * cos(tang)) + pxr->x + pxr->w / 2;
	pt2.y = -(long)((float)pxr->h / 2 * sin(tang)) + pxr->y + pxr->h / 2;

	if (tang - fang > XPI)
		larg = 1;
	else
		larg = 0;

	xsprintf(token, _T("M%d %d L%d %d A%d %d 0 %d 0 %d %d Z"), pxr->x + pxr->w / 2, pxr->y + pxr->h / 2, pt1.x, pt1.y, pxr->w / 2, pxr->h / 2, larg, pt2.x, pt2.y);
	
	set_dom_node_attr(glk, SVG_ATTR_D, -1, token, -1);

	write_xpen_to_svg_node(glk, pxp);

	write_xbrush_to_svg_node(glk, pxb);
}

void read_pie_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* pxr, double* pfang, double* ptang)
{
	const tchar_t* token;
	long x1, y1, x2, y2, x3, y3, rx, ry;
	int tan, lar, ccw;

	if (xsicmp(get_dom_node_name_ptr(glk), SVG_NODE_PATH) != 0)
		return;

	token = get_dom_node_attr_ptr(glk, SVG_ATTR_D, -1);
	if (is_null(token))
		return;

	xscanf(token, _T("M%d %d L%d %d A%d %d %d %d %d %d %d Z"), &x1, &y1, &x2, &y2, &rx, &ry, &tan, &lar, &ccw, &x3, &y3);

	pxr->x = x1 - rx;
	pxr->y = y1 - ry;
	pxr->w = 2 * rx;
	pxr->h = 2 * ry;

	*pfang = acos((float)(x2 - x1) / (float)rx);
	if (y1 < y2)
		*pfang = 2 * XPI - *pfang;

	*ptang = acos((float)(x3 - x1) / (float)rx);
	if (y1 <= y3)
		*ptang = 2 * XPI - *ptang;

	if (pxp)
		read_xpen_from_svg_node(glk, pxp);

	if (pxb)
		read_xbrush_from_svg_node(glk, pxb);
}


#endif //XDLDLC
