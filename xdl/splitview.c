/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc split view

	@module	splitview.c | implement file

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
#include "splitview.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdldoc.h"

#ifdef XDL_SUPPORT_VIEW

void calc_split_item_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr)
{
	pxr->fx = get_split_item_x(cur);
	pxr->fy = get_split_item_y(cur);
	pxr->fw = get_split_item_width(cur);
	pxr->fh = get_split_item_height(cur);
}

void calc_split_span_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr)
{
	float fx, fy, fw, fh, m, span;
	float r;

	fx = get_split_item_x(cur);
	fy = get_split_item_y(cur);
	fw = get_split_item_width(cur);
	fh = get_split_item_height(cur);

	if (get_split_item_splited(cur))
	{
		r = (float)parse_numeric(get_split_item_ratio_ptr(cur),-1);

		if (compare_text(get_split_item_layer_ptr(cur), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
		{
			span = get_split_item_span(cur);

			if (r > 0)
			{
				if (r < 1)
					m = (float)(fh * r);
				else
					m = (r < fh) ? r : fh;
			}
			else
			{
				if (r > -1)
					m = (float)(fh * (1 + r));
				else
					m = (r + fh > 0) ? (r + fh) : 0;
			}

			fy += (m - span);
			fh = 2 * span;
		}
		else
		{
			span = get_split_item_span(cur);

			if (r > 0)
			{
				if (r < 1)
					m = (float)(fw * r);
				else
					m = (r < fw) ? r : fw;
			}
			else
			{
				if (r > -1)
					m = (float)(fw * (1 + r));
				else
					m = (r + fw > 0) ? (r + fw) : 0;
			}

			fx += (m - span);
			fw = 2 * span;
		}

		pxr->fx = fx;
		pxr->fy = fy;
		pxr->fw = fw;
		pxr->fh = fh;
	}
	else
	{
		xmem_zero((void*)pxr, sizeof(xrect_t));
	}
}

void resize_split_item(link_t_ptr ilk)
{
	link_t_ptr plk;
	float fx, fy, fw, fh, m, span;
	float r;
	link_t_ptr st = NULL;

	while (ilk)
	{
		if (get_split_item_splited(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_split_first_child_item(ilk);

			plk = get_split_parent_item(ilk);

			fx = get_split_item_x(plk);
			fy = get_split_item_y(plk);
			fw = get_split_item_width(plk);
			fh = get_split_item_height(plk);

			r = (float)parse_numeric(get_split_item_ratio_ptr(plk),-1);

			if (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				span = get_split_item_span(plk);

				if (r > 0)
				{
					if (r < 1)
						m = (float)(fh * r);
					else
						m = (r < fh) ? r : fh;
				}
				else
				{
					if (r > -1)
						m = (float)(fh * (1 + r));
					else
						m = (r + fh > 0) ? (r + fh) : 0;
				}

				fh = m - span / 2;
			}
			else
			{
				span = get_split_item_span(plk);

				if (r > 0)
				{
					if (r < 1)
						m = (float)(fw * r);
					else
						m = (r < fw) ? r : fw;
				}
				else
				{
					if (r > -1)
						m = (float)(fw * (1 + r));
					else
						m = (r + fw > 0) ? (r + fw) : 0;
				}

				fw = m - span / 2;
			}

			set_split_item_x(ilk, fx);
			set_split_item_y(ilk, fy);
			set_split_item_width(ilk, fw);
			set_split_item_height(ilk, fh);
			continue;
		}

		while (ilk)
		{
			ilk = get_split_next_sibling_item(ilk);
			if (ilk)
			{
				plk = get_split_parent_item(ilk);

				fx = get_split_item_x(plk);
				fy = get_split_item_y(plk);
				fw = get_split_item_width(plk);
				fh = get_split_item_height(plk);

				r = (float)parse_numeric(get_split_item_ratio_ptr(plk),-1);
				
				if (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
				{
					span = get_split_item_span(plk);

					if (r > 0)
					{
						if (r < 1)
							m = (float)(fh * r);
						else
							m = (r < fh) ? r : fh;
					}
					else
					{
						if (r > -1)
							m = (float)(fh * (1 + r));
						else
							m = (r + fh > 0) ? (r + fh) : 0;
					}

					fy += (m + span);
					fh -= (m + span);
				}
				else
				{
					span = get_split_item_span(plk);

					if (r > 0)
					{
						if (r < 1)
							m = (float)(fw * r);
						else
							m = (r < fw) ? r : fw;
					}
					else
					{
						if (r > -1)
							m = (float)(fw * (1 + r));
						else
							m = (r + fw > 0) ? (r + fw) : 0;
					}

					fx += (m + span);
					fw -= (m + span);
				}
				set_split_item_x(ilk, fx);
				set_split_item_y(ilk, fy);
				set_split_item_width(ilk, fw);
				set_split_item_height(ilk, fh);
				break;
			}

			ilk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);
}

void adjust_split_item(link_t_ptr ilk, float off)
{
	float w, h, m;
	float r;
	tchar_t token[NUM_LEN];

	if (!get_split_item_splited(ilk))
		return;

	if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
	{
		h = get_split_item_height(ilk);
		r = (float)parse_numeric(get_split_item_ratio_ptr(ilk),-1);
		
		if (r > 0)
		{
			if (r < 1)
				m = (float)(h * r);
			else
				m = (r < h) ? r : h;
		}
		else
		{
			if (r > -1)
				m = (float)(h * (1 + r));
			else
				m = (r + h > 0) ? (r + h) : 0;
		}

		m += off;
		if (m > h)
			m = h;

		xsprintf(token, _T("%.2f%c"), m / h * 100, _T('%'));
		set_split_item_ratio(ilk, token);
	}
	else
	{
		w = get_split_item_width(ilk);
		r = (float)parse_numeric(get_split_item_ratio_ptr(ilk),-1);

		if (r > 0)
		{
			if (r < 1)
				m = (float)(w * r);
			else
				m = (r < w) ? r : w;
		}
		else
		{
			if (r > -1)
				m = (float)(w * (1 + r));
			else
				m = (r + w > 0) ? (r + w) : 0;
		}

		m += off;
		if (m > w)
			m = w;

		xsprintf(token, _T("%.2f%c"), m / w * 100, _T('%'));
		set_split_item_ratio(ilk, token);
	}

	resize_split_item(ilk);
}

int calc_split_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr ilk;
	link_t_ptr st = NULL;
	float x, y, w, h, m, span;
	float r;
	int hint;

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
			r = (float)parse_numeric(get_split_item_ratio_ptr(ilk),-1);

			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				span = get_split_item_span(ilk);

				if (r > 0)
				{
					if (r < 1)
						m = (float)(h * r);
					else
						m = (r < h) ? r : h;
				}
				else
				{
					if (r > -1)
						m = (float)(h * (1 + r));
					else
						m = (r + h > 0) ? (r + h) : 0;
				}

				y += (m - span);
				h = 2 * span;
			}
			else
			{
				span = get_split_item_span(ilk);

				if (r > 0)
				{
					if (r < 1)
						m = (float)(w * r);
					else
						m = (r < w) ? r : w;
				}
				else
				{
					if (r > -1)
						m = (float)(w * (1 + r));
					else
						m = (r + w > 0) ? (r + w) : 0;
				}

				x += (m - span);
				w = 2 * span;
			}

			if (ft_inside(ppt->fx, ppt->fy, x, y, x + w, y + h))
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

	return hint;
}

#endif //XDLVIEW