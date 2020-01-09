/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xml view document

	@module	xmlview.c | xml view implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#include "xmlview.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdccanv.h"
#include "xdcdraw.h"
#include "xdcwidg.h"

#define DEF_NODE_HEIGHT		6

typedef struct _xml_delta_t{
	res_win_t owner;
	xhand_t canvas;

	link_t_ptr xml;
	link_t_ptr node;
	link_t_ptr hover;

	bool_t b_draw;
	bool_t b_scroll;
}xml_delta_t;


#define GETXMLDELTA(ph) 		(xml_delta_t*)widget_get_user_delta(ph)
#define SETXMLDELTA(ph,ptd)		widget_set_user_delta(ph,ptd)


/**********************************************************************************/

bool_t	noti_xml_node_changing(res_win_t widget);

void	noti_xml_node_changed(res_win_t widget, link_t_ptr ilk);

void	noti_xml_node_expand(res_win_t widget, link_t_ptr ilk);

void	noti_xml_node_collapse(res_win_t widget, link_t_ptr ilk);

void	noti_xml_reset_scroll(res_win_t widget);

void	noti_xml_reset_page(res_win_t widget);

/************************************************************************************/
void	hand_xml_create(res_win_t widget);

void	hand_xml_destroy(res_win_t widget);

void	hand_xml_paint(res_win_t widget, res_dc_t dc, const xrect_t* prt);

void	hand_xml_erase(res_win_t widget, res_dc_t dc, const xrect_t* prt);

void	hand_xml_size(res_win_t widget, const resize_t* prs);

void	hand_xml_lbutton_down(res_win_t widget, short x, short y);

void	hand_xml_lbutton_up(res_win_t widget, short x, short y);

void	hand_xml_lbutton_dbclick(res_win_t widget, short x, short y);

void	hand_xml_rbutton_down(res_win_t widget, short x, short y);

void	hand_xml_rbutton_up(res_win_t widget, short x, short y);

void	hand_xml_mouse_move(res_win_t widget, dword_t dw, short x, short y);

void	hand_xml_horz_wheel(res_win_t wt, short delta);

void	hand_xml_vert_wheel(res_win_t wt, short delta);

void	hand_xml_keydown(res_win_t widget, int nKey);

void	hand_xml_horz_scroll(res_win_t widget, const scroll_t* psc);

void	hand_xml_vert_scroll(res_win_t widget, const scroll_t* psc);

/************************************************control method********************************************/

long calc_xml_height(link_t_ptr xml)
{
	link_t_ptr ptr,ilk,st = NULL;
	long ih,th;

	ptr = get_xml_dom_node(xml);

	th = DEF_NODE_HEIGHT;
	ih = DEF_NODE_HEIGHT;

	ilk = get_dom_first_child_node(ptr);
	while(ilk)
	{
		th += ih;

		if(get_dom_node_mask_check(ilk,MSK_EXPANDED) && get_dom_child_node_count(ilk))
		{
			if(!st)
				st = create_stack_table();

			push_stack_item(st,(void*)ilk);

			ilk = get_dom_first_child_node(ilk);
			continue;

		}

		while(ilk)
		{
			ilk = get_dom_next_sibling_node(ilk);
			if(ilk)
				break;

			ilk = (st)? (link_t_ptr)pop_stack_item(st) : NULL;
			if (ilk)
				th += ih;
		}
	}

	if(st)
		destroy_stack_table(st);

	return th;
}

bool_t calc_xml_node_rect(link_t_ptr xml,link_t_ptr plk,xrect_t* pxr)
{
	link_t_ptr ptr,ilk;
	long ic,th,ih,pw,offy;
	link_t_ptr st = NULL;
	bool_t b_find = 0;

	ptr = get_xml_dom_node(xml);

	th = DEF_NODE_HEIGHT;
	ih = DEF_NODE_HEIGHT;
	ic = DEF_NODE_HEIGHT / 2;

	pw = get_dom_page_width(ptr);
	offy = get_dom_page_yoff(ptr);

	pxr->x = 0;
	pxr->w = pw;
	pxr->y = 0;
	pxr->h = 0;

	th -= offy;

	ilk = get_dom_first_child_node(ptr);
	while(ilk)
	{
		if (ilk == plk)
		{
			b_find = 1;
			pxr->y = th;
			pxr->h = ih;
		}
		th += ih;

		if (get_dom_node_mask_check(ilk, MSK_EXPANDED) && get_dom_child_node_count(ilk))
		{
			pxr->x += ic / 2;

			if(!st)
				st = create_stack_table();

			push_stack_item(st,(void*)ilk);

			ilk = get_dom_first_child_node(ilk);
			continue;

		}

		while(ilk)
		{
			ilk = get_dom_next_sibling_node(ilk);
			if(ilk)
				break;
			ilk = (st)? (link_t_ptr)pop_stack_item(st) : NULL;
			if (ilk)
				pxr->x -= ic / 2;
		}

		if (ilk == plk)
		{
			pxr->h = th - pxr->y;
			break;
		}
	}

	if(st)
		destroy_stack_table(st);

	return b_find;
}

bool_t ensure_xml_node_visible(link_t_ptr xml,link_t_ptr ilk)
{
	bool_t b_tag = 0;
	xrect_t xr = {0};
	long th,ph,yoff;
	link_t_ptr ptr;

	ptr = get_xml_dom_node(xml);

	if(!ilk)
	{
		return b_tag;
	}

	th = DEF_NODE_HEIGHT;
	ph = get_dom_page_height(ptr);

	calc_xml_node_rect(ptr,ilk,&xr);
	
	if(xr.y < th)
	{
		yoff = get_dom_page_yoff(ptr) + (xr.y - th);
		set_dom_page_yoff(ptr,yoff);
		b_tag = 1;
	}else if(xr.y + xr.h > ph)
	{
		yoff = get_dom_page_yoff(ptr) + (xr.y + xr.h - ph);
		set_dom_page_yoff(ptr,yoff);
		b_tag = 1;
	}

	return b_tag;
}

int calc_xml_point_hint(link_t_ptr xml,const xpoint_t* ppt,link_t_ptr* pilk)
{
	link_t_ptr ptr,ilk,st = NULL;
	long x,y;
	long ic,ih,th,pw,offy;
	long total_h,total_i;
	int hint;

	*pilk = NULL;
	hint = XML_HINT_NONE;
	x = ppt->x;
	y = ppt->y;

	ptr = get_xml_dom_node(xml);

	pw = get_dom_page_width(ptr);
	offy = get_dom_page_yoff(ptr);

	ih = DEF_NODE_HEIGHT;
	th = DEF_NODE_HEIGHT;
	ic = DEF_NODE_HEIGHT / 2;

	if(y < th)
	{
		hint = XML_HINT_TITLE;

		return hint;
	}

	total_h = th - offy;
	total_i = 0;

	ilk = get_dom_first_child_node(ptr);
	while(ilk)
	{
		if(y < total_h + ih)
		{
			*pilk = ilk;
			if(x < total_i + ic)
			{
				hint = XML_HINT_EXPAND;
			}else
			{
				hint = XML_HINT_NODE;
			}
			break;
		}
	
		total_h += ih;

		if(get_dom_node_mask_check(ilk,MSK_EXPANDED))
		{
			if(!st)
				st = create_stack_table();

			push_stack_item(st,(void*)ilk);

			ilk = get_dom_first_child_node(ilk);
			total_i += ic / 2;
		}else
		{
			ilk = get_dom_next_sibling_node(ilk);
		}

		while(!ilk)
		{
			ilk = (st)? (link_t_ptr)pop_stack_item(st) : NULL;
			if(ilk)
			{
				ilk = get_dom_next_sibling_node(ilk);

			}else
				break;
			total_i -= ic / 2;
		}
	}

	if(st)
		destroy_stack_table(st);

	return hint;
}

void draw_xml(xhand_t canv,link_t_ptr xml, const xpaint_t* pxn)
{
	link_t_ptr ptr,ilk;
	link_t_ptr st = NULL;
	long ic,ih,th,pw,offy,total_indent,total_height;
	xrect_t xr_text,xr_image;
	xbrush_t xb = {0};
	xpen_t xp = {0};
	xfont_t xf = {0};
	xface_t xa = {0};
	xcolor_t xc = {0};

	default_xpen(&xp);
	default_xbrush(&xb);
	default_xfont(&xf);
	default_xface(&xa);

	parse_xcolor(&xc,GDI_ATTR_RGB_DARKCYAN);

	ptr = get_xml_dom_node(xml);

	pw = get_dom_page_width(ptr);
	offy = get_dom_page_yoff(ptr);

	ih = DEF_NODE_HEIGHT;
	th = DEF_NODE_HEIGHT;
	ic = DEF_NODE_HEIGHT / 2;

	total_indent = 0;
	total_height = th - offy;

	ilk = get_dom_first_child_node(ptr);
	while(ilk)
	{
		xr_image.x = total_indent;
		xr_image.w = ic;
		xr_image.y = total_height;
		xr_image.h = ih;

		if (get_dom_node_mask_check(ilk, MSK_EXPANDED))
		{
			draw_expand(canv, &xc, &xr_image, _SMALL_BITMAP);
		}else
		{
			draw_expand(canv, &xc, &xr_image, _SMALL_BITMAP);
		}

		xr_text.x = total_indent + ic;
		xr_text.w = pw - xr_text.x;
		xr_text.y = total_height;
		xr_text.h = ih;

		draw_text(canv,&xf,&xa,&xr_text,get_dom_node_name_ptr(ilk),-1);

		total_height += ih;
		if (get_dom_node_mask_check(ilk, MSK_EXPANDED) && get_dom_child_node_count(ilk))
		{
			total_indent += ic / 2;

			if(!st)
				st = create_stack_table();

			push_stack_item(st,(void*)ilk);

			ilk = get_dom_first_child_node(ilk);
			continue;

		}

		while(ilk)
		{
			ilk = get_dom_next_sibling_node(ilk);
			if(ilk)
				break;
			ilk = (st)? (link_t_ptr)pop_stack_item(st) : NULL;
			total_indent -= ic / 2;
		}
	}

	if(st)
		destroy_stack_table(st);
}


/*********************************************control event**************************************/
int noti_xml_owner(res_win_t widget, unsigned long code, link_t_ptr xml, link_t_ptr ilk, void* data)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	NOTICE_XML nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.xml = xml;
	nf.node = ilk;

	widget_notice_owner(ptd->owner, (LPNOTICE)&nf);
	return nf.ret;
}

bool_t noti_xml_node_changing(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	link_t_ptr tlk;
	xrect_t xr;

	assert(ptd->node);

	if (noti_xml_owner(widget, NC_XMLNODECHANGING, ptd->xml, ptd->node, NULL))
		return 0;

	tlk = ptd->node;
	ptd->node = NULL;

	calc_xml_node_rect(ptd->xml, ptd->node, &xr);

	expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	rect_tm_to_pt(ptd->canvas, &xr);

	ptd->b_draw = 0;
	widget_invalid_rect(widget, &xr);

	return 1;
}

void noti_xml_node_changed(res_win_t widget, link_t_ptr ilk)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	xrect_t xr;
	bool_t bRe = 0;

	assert(!ptd->node);

	ptd->node = ilk;

	calc_xml_node_rect(ptd->xml, ptd->node, &xr);

	expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	rect_tm_to_pt(ptd->canvas, &xr);

	ptd->b_draw = 0;
	widget_invalid_rect(widget, &xr);

	noti_xml_owner(widget, NC_XMLNODECHANGED, ptd->xml, ptd->node, NULL);
}

void noti_xml_node_expand(res_win_t widget, link_t_ptr ilk)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	assert(ilk);

	set_dom_node_mask_check(ilk, MSK_EXPANDED, 1);

	noti_xml_owner(widget, NC_XMLNODEEXPAND, ptd->xml, ilk, NULL);

	xml_view_redraw(widget);
}

void noti_xml_node_collapse(res_win_t widget, link_t_ptr ilk)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	assert(ilk);

	set_dom_node_mask_check(ilk, MSK_EXPANDED, 0);

	noti_xml_owner(widget, NC_XMLNODECOLLAPSE, ptd->xml, ilk, NULL);

	xml_view_redraw(widget);
}

void noti_xml_reset_scroll(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	short nCurLine, nMaxLine;
	scroll_t sc;

	if (!ptd->b_scroll)
		return;

	nMaxLine = (short)(calc_xml_height(ptd->xml) - get_xml_page_height(ptd->xml));
	if (nMaxLine < 0)
		nMaxLine = 0;

	nCurLine = (short)(get_xml_page_yoff(ptd->xml));
	if (nCurLine > nMaxLine)
		nCurLine = nMaxLine;

	sc.max = nMaxLine;
	sc.min = 0;
	sc.pos = nCurLine;
	widget_set_vert_scroll_info(widget, &sc);
}

void noti_xml_reset_page(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	long pw, ph;
	long nCurLine, nMaxLine;
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	pw = pt_to_tm(ptd->canvas, xr.w, 1) - pt_to_tm(ptd->canvas, 0, 1);
	ph = pt_to_tm(ptd->canvas, xr.h, 0) - pt_to_tm(ptd->canvas, 0, 0);
	set_xml_page_width(ptd->xml, pw);
	set_xml_page_height(ptd->xml, ph);

	nMaxLine = (long)(calc_xml_height(ptd->xml) - get_xml_page_height(ptd->xml));
	if (nMaxLine < 0)
		nMaxLine = 0;

	nCurLine = (long)(get_xml_page_yoff(ptd->xml));
	if (nCurLine > nMaxLine)
	{
		nCurLine = nMaxLine;
		set_xml_page_yoff(ptd->xml, nCurLine);
	}
}


/***********************************************************************************/
void hand_xml_create(res_win_t widget)
{
	xml_delta_t* ptd;
	res_dc_t dc;

	ptd = (xml_delta_t*)xmem_alloc(sizeof(xml_delta_t));
	memset((void*)ptd, 0, sizeof(xml_delta_t));

	dc = widget_reference_dc(widget);
	ptd->canvas = create_display_canvas(dc);
	widget_release_dc(widget, dc);

	SETXMLDELTA(widget, ptd);
}

void hand_xml_destroy(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->xml)
		xml_view_detach(widget);

	destroy_display_canvas(ptd->canvas);

	xmem_free(ptd);

	SETXMLDELTA(widget, 0);
}

void hand_xml_erase(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

}

void hand_xml_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	xpaint_t xn = { 0 };
	xbrush_t xb = { 0 };
	xrect_t xr = { 0 };

	if (!ptd)
		return;

	noti_xml_owner(widget, NC_XMLPAINT, ptd->xml, NULL, (void*)&xn);

	widget_get_client_rect(widget, &xr);
	begin_display_paint(ptd->canvas, dc, xr.w, xr.h);

	parse_xbrush_from_style(&xb, DEF_CTRL_BRUSH);
	if (!is_null(xn.brush_color))
		xscpy(xb.color, xn.brush_color);

	widget_get_client_rect(widget, &xr);
	rect_pt_to_tm(ptd->canvas, &xr);
	expand_rect(&xr, 1, 1);

	fill_shape(ptd->canvas, &xb, &xr, ATTR_SHAPE_RECT);

	draw_xml(ptd->canvas, ptd->xml, &xn);

	end_display_paint(ptd->canvas, dc, pxr);
}

void hand_xml_size(res_win_t widget, const resize_t* prs)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->xml || !ptd->canvas)
		return;

	if (prs->code == RESIZE_MAXIMIZED || prs->code == RESIZE_MINIMIZED || prs->code == RESIZE_RESTORE)
		xml_view_redraw(widget);
}

void hand_xml_lbutton_down(res_win_t widget, short x, short y)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	link_t_ptr tlk;
	int nHint;
	xpoint_t pt;
	bool_t bRe;

	if (!ptd)
		return;

	widget_set_focus(widget);
	widget_set_capture(widget,1);

	pt.x = pt_to_tm(ptd->canvas, x, 1);
	pt.y = pt_to_tm(ptd->canvas, y, 0);

	nHint = calc_xml_point_hint(ptd->xml, &pt, &tlk);

	bRe = (tlk == ptd->node) ? 1 : 0;

	if (nHint == XML_HINT_EXPAND)
	{
		noti_xml_node_expand(widget, tlk);
		return;
	}

	xml_view_set_focus_node(widget, tlk);
}

void hand_xml_lbutton_up(res_win_t widget, short x, short y)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	xpoint_t pt;

	if (!ptd)
		return;

	widget_set_capture(widget,0);

	pt.x = x;
	pt.y = y;
	widget_client_to_screen(widget, &pt);
	noti_xml_owner(widget, NC_XMLLBCLK,ptd->xml, ptd->node, (void*)&pt);
}

void hand_xml_lbutton_dbclick(res_win_t widget, short x, short y)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	xpoint_t pt;

	if (!ptd)
		return;

	pt.x = x;
	pt.y = y;
	widget_client_to_screen(widget, &pt);
	noti_xml_owner(widget, NC_XMLDBCLK, ptd->xml, ptd->node, (void*)&pt);
}

void hand_xml_rbutton_down(res_win_t widget, short x, short y)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	if (!ptd)
		return;
}

void hand_xml_rbutton_up(res_win_t widget, short x, short y)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	xpoint_t pt;

	if (!ptd)
		return;

	pt.x = x;
	pt.y = y;
	widget_client_to_screen(widget, &pt);
	noti_xml_owner(widget, NC_XMLRBCLK, ptd->xml, ptd->node, (void*)&pt);
}

void hand_xml_mouse_move(res_win_t widget, dword_t dw, short x, short y)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	xpoint_t pt;
	int nHint;
	link_t_ptr ilk;

	if (!ptd)
		return;

	pt.x = pt_to_tm(ptd->canvas, x, 1);
	pt.y = pt_to_tm(ptd->canvas, y, 0);

	nHint = calc_xml_point_hint(ptd->xml, &pt, &ilk);

	pt.x = x;
	pt.y = y;

	widget_client_to_screen(widget, &pt);

	if (ptd->hover && ptd->hover != ilk)
	{
		noti_xml_owner(widget, NC_XMLNODELEAVE, ptd->xml, ptd->hover, (void*)&pt);
		ptd->hover = NULL;
	}

	if (!ptd->hover && ilk)
	{
		ptd->hover = ilk;
		noti_xml_owner(widget, NC_XMLNODEENTER, ptd->xml, ptd->hover, (void*)&pt);
	}
}

void hand_xml_keydown(res_win_t widget, int nChar)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	if (!ptd)
		return;

	switch (nChar)
	{
	case 13:
		break;
	case VK_DOWN:
		xml_view_tabskip(widget, NF_DOWN);
		break;
	case VK_RIGHT:
		xml_view_tabskip(widget, NF_RIGHT);
		break;
		break;
	case VK_UP:
		xml_view_tabskip(widget, NF_UP);
		break;
	case VK_LEFT:
		xml_view_tabskip(widget, NF_LEFT);
		break;
	}
}

void hand_xml_vert_scroll(res_win_t widget, const scroll_t* psc)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	short nMax, nLine, nCur;
	scroll_t sc;

	if (!ptd)
		return;

	nMax = (short)(calc_xml_height(ptd->xml) - get_xml_page_height(ptd->xml));
	if (nMax < 0)
		nMax = 0;

	nLine = 100;

	switch (psc->code)
	{
	case VERT_LINE_UP:
		if (psc->pos <= 0)
			return;

		nCur = psc->pos - nLine;
		if (nCur < 0)
			nCur = 0;

		sc.max = nMax;
		sc.min = 0;
		sc.pos = nCur;
		widget_set_vert_scroll_info(widget, &sc);
		break;
	case VERT_LINE_DOWN:
		if (psc->pos >= nMax)
			return;

		nCur = psc->pos + nLine;
		if (nCur > nMax)
			nCur = nMax;

		sc.max = nMax;
		sc.min = 0;
		sc.pos = nCur;
		widget_set_vert_scroll_info(widget, &sc);
		break;
	case VERT_PAGE_UP:
		if (psc->pos <= 0)
			return;

		nCur = psc->pos - nLine * 2;
		if (nCur < 0)
			nCur = 0;

		sc.max = nMax;
		sc.min = 0;
		sc.pos = nCur;
		widget_set_vert_scroll_info(widget, &sc);
		break;
	case VERT_PAGE_DOWN:
		if (psc->pos >= nMax)
			return;

		nCur = psc->pos + nLine * 2;
		if (nCur > nMax)
			nCur = nMax;

		sc.max = nMax;
		sc.min = 0;
		sc.pos = nCur;
		widget_set_vert_scroll_info(widget, &sc);
		break;
	case VERT_THUMB:
		if (psc->pos <= 0)
			nCur = 0;
		else if (psc->pos >= nMax)
			nCur = nMax;
		else
			nCur = psc->pos;

		sc.max = nMax;
		sc.min = 0;
		sc.pos = nCur;
		widget_set_vert_scroll_info(widget, &sc);
		break;
	case VERT_END:
		widget_get_vert_scroll_info(widget, &sc);

		set_xml_page_yoff(ptd->xml, sc.pos);

		ptd->b_draw = 1;
		widget_invalid_rect(widget, NULL);
		break;
	}
}

void hand_xml_horz_scroll(res_win_t widget, const scroll_t* psc)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	
	if (!ptd)
		return;
}

void hand_xml_vert_wheel(res_win_t widget, short delta)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	long nLine, nMax;

	if (!ptd)
		return;

	nMax = calc_xml_height(ptd->xml) - get_xml_page_height(ptd->xml);
	if (nMax < 0)
		nMax = 0;

	nLine = get_xml_page_yoff(ptd->xml);

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

	set_xml_page_yoff(ptd->xml, nLine);

	noti_xml_reset_scroll(widget);

	ptd->b_draw = 1;
	widget_invalid_rect(widget, NULL);
}

void hand_xml_horz_wheel(res_win_t widget, short delta)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	long nLine, nMax;

	if (!ptd)
		return;

}

/************************************************control method********************************************/
void xml_view_set_owner(res_win_t widget, res_win_t owner)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	ptd->owner = owner;
}

res_win_t xml_view_get_owner(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	return ptd->owner;
}

xhand_t xml_view_get_canvas(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	return ptd->canvas;
}

void xml_view_attach(res_win_t widget, link_t_ptr ptr)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	assert(ptd->xml == NULL);

	ptd->xml = ptr;
	if (ptd->xml)
	{
		xml_view_redraw(widget);
	}
}

link_t_ptr xml_view_detach(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	link_t_ptr ptr;

	ptr = ptd->xml;
	ptd->xml = NULL;

	widget_invalid_rect(widget, NULL);
	return ptr;
}

link_t_ptr xml_view_fetch(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	return ptd->xml;
}

void xml_view_redraw(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	assert(ptd->xml && ptd->canvas);

	if (ptd->node && !is_dom_child_node(get_xml_dom_node(ptd->xml), ptd->node))
	{
		ptd->node = NULL;
	}
	ptd->hover = NULL;

	noti_xml_reset_page(widget);
	noti_xml_reset_scroll(widget);

	ptd->b_draw = 1;
	widget_invalid_rect(widget, NULL);
}

void xml_view_redraw_node(res_win_t widget, link_t_ptr ilk)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	xrect_t xr;

	assert(ilk);
	if (calc_xml_node_rect(ptd->xml, ilk, &xr))
	{
		expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

		rect_tm_to_pt(ptd->canvas, &xr);

		ptd->b_draw = 1;
		widget_invalid_rect(widget, &xr);
	}
}

bool_t xml_view_set_focus_node(res_win_t widget, link_t_ptr ilk)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	bool_t bRe;

	bRe = (ilk == ptd->node) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->node && !bRe)
	{
		if (!noti_xml_node_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		if (ensure_xml_node_visible(ptd->xml, ilk))
		{
			noti_xml_reset_scroll(widget);

			ptd->b_draw = 1;
			widget_invalid_rect(widget, NULL);
		}

		noti_xml_node_changed(widget, ilk);
	}

	return 1;
}

link_t_ptr xml_view_get_focus_node(res_win_t widget)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	return ptd->node;
}

void xml_view_tabskip(res_win_t widget, int nSkip)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);
	link_t_ptr plk = NULL;

	switch (nSkip)
	{
	case NF_RIGHT:
	case NF_DOWN:
		if (ptd->node)
		{
			plk = get_dom_next_sibling_node(ptd->node);
		}
		else
		{
			plk = get_dom_first_child_node(get_xml_dom_node(ptd->xml));
		}
		break;
	case NF_LEFT:
	case NF_UP:
		if (ptd->node)
		{
			plk = get_dom_prev_sibling_node(ptd->node);
		}
		else
		{
			plk = get_dom_last_child_node(get_xml_dom_node(ptd->xml));
		}
		break;
	case NF_HOME:
		plk = get_dom_first_child_node(get_xml_dom_node(ptd->xml));
		break;
	case NF_END:
		plk = get_dom_last_child_node(get_xml_dom_node(ptd->xml));
		break;
	}

	xml_view_set_focus_node(widget, plk);
}

void xml_view_get_node_rect(res_win_t widget, link_t_ptr nlk, xrect_t* prt)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	calc_xml_node_rect(ptd->xml, nlk, prt);
	rect_tm_to_pt(ptd->canvas, prt);
}

void xml_view_tm_to_pt(res_win_t widget, xpoint_t* ppt)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	ppt->x = tm_to_pt(ptd->canvas, ppt->x, 1);
	ppt->y = tm_to_pt(ptd->canvas, ppt->y, 0);
}

void xml_view_pt_to_tm(res_win_t widget, xpoint_t* ppt)
{
	xml_delta_t* ptd = GETXMLDELTA(widget);

	ppt->x = pt_to_tm(ptd->canvas, ppt->x, 1);
	ppt->y = pt_to_tm(ptd->canvas, ppt->y, 0);
}

res_win_t xml_view_create(const wchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_xml_create;
	ev.pf_on_destroy = hand_xml_destroy;

	ev.pf_on_erase = hand_xml_erase;
	ev.pf_on_paint = hand_xml_paint;

	ev.pf_on_size = hand_xml_size;

	ev.pf_on_horz_scroll = hand_xml_horz_scroll;
	ev.pf_on_horz_wheel = hand_xml_horz_wheel;
	ev.pf_on_vert_scroll = hand_xml_vert_scroll;
	ev.pf_on_vert_wheel = hand_xml_vert_wheel;

	ev.pf_on_keydown = hand_xml_keydown;

	ev.pf_on_mouse_move = hand_xml_mouse_move;
	ev.pf_on_lbutton_dbclick = hand_xml_lbutton_dbclick;
	ev.pf_on_lbutton_down = hand_xml_lbutton_down;
	ev.pf_on_lbutton_up = hand_xml_lbutton_up;
	ev.pf_on_rbutton_down = hand_xml_rbutton_down;
	ev.pf_on_rbutton_up = hand_xml_rbutton_up;

	return widget_create(wname, wstyle, pxr->x, pxr->y, pxr->w, pxr->h, wparent, (void*)&ev);
}
