/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc split view

	@module	splitview.c | split view implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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
#include "splitview.h"
#include "splitdoc.h"
#include "xdlstd.h"


#ifndef XDL_SUPPORT_CONTEXT

static float pt_per_mm(res_ctx_t rdc, bool_t horz)
{
	return (float)MMPERPT;
}

static float cast_pt_to_mm(res_ctx_t rdc, long pt, bool_t horz)
{
	return (float)(pt * MMPERPT);
}

static long cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz)
{
	return (long)(mm / MMPERPT);
}

#endif

#ifdef XDL_SUPPORT_VIEW

void calc_split_item_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr)
{
	pxr->x = get_split_item_x(cur);
	pxr->y = get_split_item_y(cur);
	pxr->w = get_split_item_width(cur);
	pxr->h = get_split_item_height(cur);
}

void calc_split_span_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr)
{
	long x, y, w, h, m, span;
	float r;
	res_ctx_t rdc;

#ifdef XDL_SUPPORT_CONTEXT
	rdc = create_display_context();
#else
	rdc = NULL;
#endif

	x = get_split_item_x(cur);
	y = get_split_item_y(cur);
	w = get_split_item_width(cur);
	h = get_split_item_height(cur);

	if (get_split_item_splited(cur))
	{
		r = (float)xstof(get_split_item_ratio_ptr(cur));

		if (compare_text(get_split_item_layer_ptr(cur), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
		{
			span = cast_mm_to_pt(rdc, get_split_item_span(cur), 0);

			if (r > 0)
			{
				if (r < 1)
					m = (long)((float)h * r);
				else
					m = (cast_mm_to_pt(rdc, r, 0) < h) ? cast_mm_to_pt(rdc, r,0) : h;
			}
			else
			{
				if (r > -1)
					m = (long)((float)h * (1 + r));
				else
					m = (cast_mm_to_pt(rdc, r, 0) + h > 0) ? (cast_mm_to_pt(rdc, r,0) + h) : 0;
			}

			y += (m - span);
			h = 2 * span;
		}
		else
		{
			span = cast_mm_to_pt(rdc, get_split_item_span(cur), 1);

			if (r > 0)
			{
				if (r < 1)
					m = (long)((float)w * r);
				else
					m = (cast_mm_to_pt(rdc, r, 1) < w) ? cast_mm_to_pt(rdc, r,1) : w;
			}
			else
			{
				if (r > -1)
					m = (long)((float)w * (1 + r));
				else
					m = (cast_mm_to_pt(rdc, r, 1) + w > 0) ? (cast_mm_to_pt(rdc, r,1) + w) : 0;
			}

			x += (m - span);
			w = 2 * span;
		}

		pxr->x = x;
		pxr->y = y;
		pxr->w = w;
		pxr->h = h;
	}
	else
	{
		xmem_zero((void*)pxr, sizeof(xrect_t));
	}

#ifdef XDL_SUPPORT_CONTEXT
	destroy_context(rdc);
#endif
}

void resize_split_item(link_t_ptr ilk)
{
	link_t_ptr plk;
	long x, y, w, h, m, span;
	float r;
	link_t_ptr st = NULL;
	res_ctx_t rdc;

#ifdef XDL_SUPPORT_CONTEXT
	rdc = create_display_context();
#else
	rdc = NULL;
#endif

	while (ilk)
	{
		if (get_split_item_splited(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_split_first_child_item(ilk);

			plk = get_split_parent_item(ilk);

			x = get_split_item_x(plk);
			y = get_split_item_y(plk);
			w = get_split_item_width(plk);
			h = get_split_item_height(plk);

			r = (float)xstof(get_split_item_ratio_ptr(plk));

			if (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				span = cast_mm_to_pt(rdc, get_split_item_span(plk),0);

				if (r > 0)
				{
					if (r < 1)
						m = (long)((float)h * r);
					else
						m = (cast_mm_to_pt(rdc, r, 0) < h) ? cast_mm_to_pt(rdc, r,0) : h;
				}
				else
				{
					if (r > -1)
						m = (long)((float)h * (1 + r));
					else
						m = (cast_mm_to_pt(rdc, r, 0) + h > 0) ? (cast_mm_to_pt(rdc, r,0) + h) : 0;
				}

				h = m - span / 2;
			}
			else
			{
				span = cast_mm_to_pt(rdc, get_split_item_span(plk), 1);

				if (r > 0)
				{
					if (r < 1)
						m = (long)((float)w * r);
					else
						m = (cast_mm_to_pt(rdc, r, 1) < w) ? cast_mm_to_pt(rdc, r,1) : w;
				}
				else
				{
					if (r > -1)
						m = (long)((float)w * (1 + r));
					else
						m = (cast_mm_to_pt(rdc, r, 1) + w > 0) ? (cast_mm_to_pt(rdc, r,1) + w) : 0;
				}

				w = m - span / 2;
			}

			set_split_item_x(ilk, x);
			set_split_item_y(ilk, y);
			set_split_item_width(ilk, w);
			set_split_item_height(ilk, h);
			continue;
		}

		while (ilk)
		{
			ilk = get_split_next_sibling_item(ilk);
			if (ilk)
			{
				plk = get_split_parent_item(ilk);

				x = get_split_item_x(plk);
				y = get_split_item_y(plk);
				w = get_split_item_width(plk);
				h = get_split_item_height(plk);

				r = (float)xstof(get_split_item_ratio_ptr(plk));
				
				if (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
				{
					span = cast_mm_to_pt(rdc, get_split_item_span(plk),0);

					if (r > 0)
					{
						if (r < 1)
							m = (long)((float)h * r);
						else
							m = (cast_mm_to_pt(rdc, r, 0) < h) ? cast_mm_to_pt(rdc, r, 0) : h;
					}
					else
					{
						if (r > -1)
							m = (long)((float)h * (1 + r));
						else
							m = (cast_mm_to_pt(rdc, r, 0) + h > 0) ? (cast_mm_to_pt(rdc, r, 0) + h) : 0;
					}

					y += (m + span);
					h -= (m + span);
				}
				else
				{
					span = cast_mm_to_pt(rdc, get_split_item_span(plk), 1);

					if (r > 0)
					{
						if (r < 1)
							m = (long)((float)w * r);
						else
							m = (cast_mm_to_pt(rdc, r, 1) < w) ? cast_mm_to_pt(rdc, r, 1) : w;
					}
					else
					{
						if (r > -1)
							m = (long)((float)w * (1 + r));
						else
							m = (cast_mm_to_pt(rdc, r, 1) + w > 0) ? (cast_mm_to_pt(rdc, r, 1) + w) : 0;
					}

					x += (m + span);
					w -= (m + span);
				}
				set_split_item_x(ilk, x);
				set_split_item_y(ilk, y);
				set_split_item_width(ilk, w);
				set_split_item_height(ilk, h);
				break;
			}

			ilk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);

#ifdef XDL_SUPPORT_CONTEXT
	destroy_context(rdc);
#endif
}

void adjust_split_item(link_t_ptr ilk, long off)
{
	long w, h, m;
	float r;
	tchar_t token[NUM_LEN];
	res_ctx_t rdc;

	if (!get_split_item_splited(ilk))
		return;

#ifdef XDL_SUPPORT_CONTEXT
	rdc = create_display_context();
#else
	rdc = NULL;
#endif

	if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
	{
		h = get_split_item_height(ilk);
		r = (float)xstof(get_split_item_ratio_ptr(ilk));
		
		if (r > 0)
		{
			if (r < 1)
				m = (long)((float)h * r);
			else
				m = (cast_mm_to_pt(rdc, r, 0) < h) ? cast_mm_to_pt(rdc, r, 0) : h;
		}
		else
		{
			if (r > -1)
				m = (long)((float)h * (1 + r));
			else
				m = (cast_mm_to_pt(rdc, r, 0) + h > 0) ? (cast_mm_to_pt(rdc, r, 0) + h) : 0;
		}

		m += off;
		if (m > h)
			m = h;

		xsprintf(token, _T("%.2f%c"), (float)m / (float)h * 100, _T('%'));
		set_split_item_ratio(ilk, token);
	}
	else
	{
		w = get_split_item_width(ilk);
		r = (float)xstof(get_split_item_ratio_ptr(ilk));

		if (r > 0)
		{
			if (r < 1)
				m = (long)((float)w * r);
			else
				m = (cast_mm_to_pt(rdc, r, 1) < w) ? cast_mm_to_pt(rdc, r, 1) : w;
		}
		else
		{
			if (r > -1)
				m = (long)((float)w * (1 + r));
			else
				m = (cast_mm_to_pt(rdc, r, 1) + w > 0) ? (cast_mm_to_pt(rdc, r, 1) + w) : 0;
		}

		m += off;
		if (m > w)
			m = w;

		xsprintf(token, _T("%.2f%c"), (float)m / (float)w * 100, _T('%'));
		set_split_item_ratio(ilk, token);
	}

	resize_split_item(ilk);

#ifdef XDL_SUPPORT_CONTEXT
	destroy_context(rdc);
#endif
}

int calc_split_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr ilk, st = NULL;
	long x, y, w, h, m, span;
	float r;
	int hint;
	res_ctx_t rdc;

#ifdef XDL_SUPPORT_CONTEXT
	rdc = create_display_context();
#else
	rdc = NULL;
#endif

	*pilk = NULL;
	hint = SPLIT_HINT_NONE;

	ilk = ptr;
	while (ilk)
	{
		x = get_split_item_x(ilk);
		y = get_split_item_y(ilk);
		w = get_split_item_width(ilk);
		h = get_split_item_height(ilk);

		if (get_split_item_splited(ilk))
		{
			r = (float)xstof(get_split_item_ratio_ptr(ilk));

			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				span = cast_mm_to_pt(rdc, get_split_item_span(ilk),0);

				if (r > 0)
				{
					if (r < 1)
						m = (long)((float)h * r);
					else
						m = (cast_mm_to_pt(rdc, r, 0) < h) ? cast_mm_to_pt(rdc, r, 0) : h;
				}
				else
				{
					if (r > -1)
						m = (long)((float)h * (1 + r));
					else
						m = (cast_mm_to_pt(rdc, r, 0) + h > 0) ? (cast_mm_to_pt(rdc, r, 0) + h) : 0;
				}

				y += (m - span);
				h = 2 * span;
			}
			else
			{
				span = cast_mm_to_pt(rdc, get_split_item_span(ilk), 1);

				if (r > 0)
				{
					if (r < 1)
						m = (long)((float)w * r);
					else
						m = (cast_mm_to_pt(rdc, r, 1) < w) ? cast_mm_to_pt(rdc, r, 1) : w;
				}
				else
				{
					if (r > -1)
						m = (long)((float)w * (1 + r));
					else
						m = (cast_mm_to_pt(rdc, r, 1) + w > 0) ? (cast_mm_to_pt(rdc, r, 1) + w) : 0;
				}

				x += (m - span);
				w = 2 * span;
			}

			if (pt_inside(ppt->x, ppt->y, x, y, x + w, y + h))
			{
				*pilk = ilk;
				hint = SPLIT_HINT_BAR;
				break;
			}
		}

		if (get_split_item_splited(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_split_first_child_item(ilk);
			continue;
		}

		while (ilk)
		{
			ilk = get_split_next_sibling_item(ilk);
			if (ilk)
			{
				break;
			}

			ilk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);

#ifdef XDL_SUPPORT_CONTEXT
	destroy_context(rdc);
#endif

	return hint;
}

#endif //XDLVIEW