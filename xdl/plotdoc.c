/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc plot document

	@module	plotdoc.c | implement file

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

#include "plotdoc.h"
#include "domdoc.h"

#include "xdlimp.h"
#include "xdlstd.h"


#ifdef XDL_SUPPORT_DOC

link_t_ptr create_plot_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_PLOT, -1);

	set_plot_type(ptr, ATTR_PLOT_TYPE_BARGRAM, -1);

	return ptr;
}

void destroy_plot_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_plot_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_PLOT, -1, 0) == 0) ? 1 : 0;
}

void clear_plot_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

void merge_plot_doc(link_t_ptr dst, link_t_ptr src)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_WIDTH, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_WIDTH, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_HEIGHT, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_HEIGHT, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_TYPE, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_TYPE, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_RULER, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_RULER, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_STYLE, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_STYLE, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_Y_STAGES, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_Y_STAGES, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_Y_BASES, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_Y_BASES, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_Y_STEPS, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_Y_STEPS, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_Y_LABELS, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_Y_LABELS, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_Y_COLORS, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_Y_COLORS, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_Y_SHAPES, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_Y_SHAPES, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_X_LABELS, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_X_LABELS, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_PLOT_X_COLORS, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_PLOT_X_COLORS, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}

	nlk = find_dom_node_by_name(dst, 0, DOC_MATRIX, -1);
	if (!nlk)
	{
		nlk = find_dom_node_by_name(src, 0, DOC_MATRIX, -1);
		if (nlk)
		{
			nlk = detach_dom_node(src, nlk);
			attach_dom_node(dst, LINK_FIRST, nlk);
		}
	}
}

float	get_plot_width(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_WIDTH, -1);

	return (nlk) ? xstof(get_dom_node_text_ptr(nlk)) : 0.0f;
}

void set_plot_width(link_t_ptr ptr, float width)
{
	link_t_ptr nlk;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_WIDTH, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_WIDTH, -1);
	}

	ftoxs(width, num, NUM_LEN);
	set_dom_node_text(nlk, num, -1);
}

float	get_plot_height(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_HEIGHT, -1);

	return (nlk) ? xstof(get_dom_node_text_ptr(nlk)) : 0;
}

void set_plot_height(link_t_ptr ptr, float height)
{
	link_t_ptr nlk;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_HEIGHT, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_HEIGHT, -1);
	}

	ftoxs(height, num, NUM_LEN);
	set_dom_node_text(nlk, num, -1);
}

int	get_plot_ruler(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_RULER, -1);

	return (nlk) ? xstol(get_dom_node_text_ptr(nlk)) : 0;
}

void set_plot_ruler(link_t_ptr ptr, int n)
{
	link_t_ptr nlk;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_RULER, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_RULER, -1);
	}

	ltoxs(n, num, NUM_LEN);
	set_dom_node_text(nlk, num, -1);
}

int	get_plot_type(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_TYPE, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_type_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_TYPE, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_type(link_t_ptr ptr, const tchar_t* type, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_TYPE, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_PLOT_TYPE, -1);
	}

	set_dom_node_text(nlk, type, len);
}

int	get_plot_style(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_STYLE, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_style_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_STYLE, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_style(link_t_ptr ptr, const tchar_t* style, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_STYLE, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_STYLE, -1);
	}

	set_dom_node_text(nlk, style, len);
}

int	get_plot_y_stages(link_t_ptr ptr, double** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STAGES, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_STAGE, -1, 1) == 0)
		{
			insert_numeric(sa, n, xstonum(get_dom_node_text_ptr(sub)));

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_y_stages(link_t_ptr ptr, double** sa)
{
	link_t_ptr nlk, sub;
	int i, n;
	tchar_t numstr[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STAGES, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_STAGES, -1);
	}

	n = get_numeric_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_Y_STAGE, -1);

		numtoxs(get_numeric(sa, i), numstr, NUM_LEN);
		set_dom_node_text(sub, numstr, -1);
	}
}

int	get_plot_y_stages_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STAGES, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_STAGE, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_y_stages_token(link_t_ptr ptr, const tchar_t* y_stages, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STAGES, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_STAGES, -1);
	}

	if (len < 0)
		len = xslen(y_stages);

	while (n = parse_string_token((y_stages + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_Y_STAGE, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_y_bases(link_t_ptr ptr, double** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_BASES, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_BASE, -1, 1) == 0)
		{
			insert_numeric(sa, n, xstonum(get_dom_node_text_ptr(sub)));

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_y_bases(link_t_ptr ptr, double** sa)
{
	link_t_ptr nlk, sub;
	int i, n;
	tchar_t numstr[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_BASES, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_BASES, -1);
	}

	n = get_numeric_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_Y_BASE, -1);

		numtoxs(get_numeric(sa, i), numstr, NUM_LEN);
		set_dom_node_text(sub, numstr, -1);
	}
}

int	get_plot_y_bases_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_BASES, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_BASE, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_y_bases_token(link_t_ptr ptr, const tchar_t* y_bases, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_BASES, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_BASES, -1);
	}

	if (len < 0)
		len = xslen(y_bases);

	while (n = parse_string_token((y_bases + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_Y_BASE, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_y_steps(link_t_ptr ptr, double** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STEPS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_STEP, -1, 1) == 0)
		{
			insert_numeric(sa, n, xstonum(get_dom_node_text_ptr(sub)));

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_y_steps(link_t_ptr ptr, double** sa)
{
	link_t_ptr nlk, sub;
	int i, n;
	tchar_t numstr[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STEPS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_STEPS, -1);
	}

	n = get_numeric_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_Y_STEP, -1);

		numtoxs(get_numeric(sa, i), numstr, NUM_LEN);
		set_dom_node_text(sub, numstr, -1);
	}
}

int	get_plot_y_steps_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STEPS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_STEP, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_y_steps_token(link_t_ptr ptr, const tchar_t* y_steps, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STEPS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_STEPS, -1);
	}

	if (len < 0)
		len = xslen(y_steps);

	while (n = parse_string_token((y_steps + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_Y_STEP, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_y_labels(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_LABELS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_LABEL, -1, 1) == 0)
		{
			insert_string(sa, n, get_dom_node_text_ptr(sub), -1);

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_y_labels(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int i, n;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_LABELS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_LABELS, -1);
	}

	n = get_string_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_Y_LABEL, -1);
		set_dom_node_text(sub, get_string_ptr(sa, i), -1);
	}
}

int	get_plot_y_labels_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_LABELS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_LABEL, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_y_labels_token(link_t_ptr ptr, const tchar_t* y_labels, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_LABELS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_LABELS, -1);
	}

	if (len < 0)
		len = xslen(y_labels);

	while (n = parse_string_token((y_labels + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_Y_LABEL, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_y_colors(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_COLORS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_COLOR, -1, 1) == 0)
		{
			insert_string(sa, n, get_dom_node_text_ptr(sub), -1);

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_y_colors(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int i, n;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_COLORS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_COLORS, -1);
	}

	n = get_string_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_Y_COLOR, -1);
		set_dom_node_text(sub, get_string_ptr(sa, i), -1);
	}
}

int	get_plot_y_colors_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_COLORS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_COLOR, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_y_colors_token(link_t_ptr ptr, const tchar_t* y_colors, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_COLORS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_COLORS, -1);
	}

	if (len < 0)
		len = xslen(y_colors);

	while (n = parse_string_token((y_colors + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_Y_COLOR, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_y_shapes(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_SHAPES, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_SHAPE, -1, 1) == 0)
		{
			insert_string(sa, n, get_dom_node_text_ptr(sub), -1);

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_y_shapes(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int i, n;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_SHAPES, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_SHAPES, -1);
	}

	n = get_string_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_Y_SHAPE, -1);
		set_dom_node_text(sub, get_string_ptr(sa, i), -1);
	}
}

int	get_plot_y_shapes_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_SHAPES, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_Y_SHAPE, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_y_shapes_token(link_t_ptr ptr, const tchar_t* y_shapes, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_SHAPES, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_SHAPES, -1);
	}

	if (len < 0)
		len = xslen(y_shapes);

	while (n = parse_string_token((y_shapes + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_Y_SHAPE, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_x_labels(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_LABELS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_X_LABEL, -1, 1) == 0)
		{
			insert_string(sa, n, get_dom_node_text_ptr(sub), -1);

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_x_labels(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int i, n;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_LABELS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_X_LABELS, -1);
	}

	n = get_string_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_X_LABEL, -1);
		set_dom_node_text(sub, get_string_ptr(sa, i), -1);
	}
}

int	get_plot_x_labels_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_LABELS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_X_LABEL, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_x_labels_token(link_t_ptr ptr, const tchar_t* x_labels, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_LABELS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_X_LABELS, -1);
	}

	if (len < 0)
		len = xslen(x_labels);

	while (n = parse_string_token((x_labels + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_X_LABEL, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_x_colors(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int n = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_COLORS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_X_COLOR, -1, 1) == 0)
		{
			insert_string(sa, n, get_dom_node_text_ptr(sub), -1);

			n++;
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return n;
}

void set_plot_x_colors(link_t_ptr ptr, tchar_t** sa)
{
	link_t_ptr nlk, sub;
	int i, n;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_COLORS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_X_COLORS, -1);
	}

	n = get_string_array_size(sa);
	for (i = 0; i < n; i++)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_PLOT_X_COLOR, -1);
		set_dom_node_text(sub, get_string_ptr(sa, i), -1);
	}
}

int	get_plot_x_colors_token(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;
	int total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_COLORS, -1);
	if (!nlk)
		return 0;

	sub = get_dom_first_child_node(nlk);
	while (sub)
	{
		if (compare_text(get_dom_node_name_ptr(sub), -1, DOC_PLOT_X_COLOR, -1, 1) == 0)
		{
			total += get_dom_node_text(sub, ((buf) ? (buf + total) : NULL), (max - total));

			if (!is_last_link(sub))
			{
				if (buf)
				{
					buf[total] = _T(',');
				}
				total++;
			}
		}

		sub = get_dom_next_sibling_node(sub);
	}

	return total;
}

void set_plot_x_colors_token(link_t_ptr ptr, const tchar_t* x_colors, int len)
{
	link_t_ptr nlk, sub;
	tchar_t* key;
	int klen;
	int n, total = 0;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_COLORS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_X_COLORS, -1);
	}

	if (len < 0)
		len = xslen(x_colors);

	while (n = parse_string_token((x_colors + total), (len - total), _T(','), &key, &klen))
	{
		total += n;

		if (klen)
		{
			sub = insert_dom_node(nlk, LINK_LAST);
			set_dom_node_name(sub, DOC_PLOT_X_COLOR, -1);
			set_dom_node_text(sub, key, klen);
		}
	}
}

int	get_plot_matrix_rows(link_t_ptr ptr)
{
	link_t_ptr nlk, sub;

	nlk = find_dom_node_by_name(ptr, 0, DOC_MATRIX, -1);
	if (!nlk)
		return 0;

	sub = find_dom_node_by_name(nlk, 0, DOC_MATRIX_ROWS, -1);
	if (!sub)
		return 0;

	return xstol(get_dom_node_text_ptr(sub));
}

void set_plot_matrix_rows(link_t_ptr ptr, int rows)
{
	link_t_ptr nlk, sub;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_MATRIX, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_MATRIX, -1);
	}

	sub = find_dom_node_by_name(nlk, 0, DOC_MATRIX_ROWS, -1);
	if (!sub)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_MATRIX_ROWS, -1);
	}

	ltoxs(rows, num, NUM_LEN);
	set_dom_node_text(sub, num, -1);
}

int	get_plot_matrix_cols(link_t_ptr ptr)
{
	link_t_ptr nlk, sub;

	nlk = find_dom_node_by_name(ptr, 0, DOC_MATRIX, -1);
	if (!nlk)
		return 0;

	sub = find_dom_node_by_name(nlk, 0, DOC_MATRIX_COLS, -1);
	if (!sub)
		return 0;

	return xstol(get_dom_node_text_ptr(sub));
}

void set_plot_matrix_cols(link_t_ptr ptr, int cols)
{
	link_t_ptr nlk, sub;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_MATRIX, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_MATRIX, -1);
	}

	sub = find_dom_node_by_name(nlk, 0, DOC_MATRIX_COLS, -1);
	if (!sub)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_MATRIX_COLS, -1);
	}

	ltoxs(cols, num, NUM_LEN);
	set_dom_node_text(sub, num, -1);
}

int	get_plot_matrix_data(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk, sub;

	nlk = find_dom_node_by_name(ptr, 0, DOC_MATRIX, -1);
	if (!nlk)
		return 0;

	sub = find_dom_node_by_name(nlk, 0, DOC_MATRIX_DATA, -1);
	if (!sub)
		return 0;

	return get_dom_node_text(sub, buf, max);
}

void set_plot_matrix_data(link_t_ptr ptr, const tchar_t* data, int len)
{
	link_t_ptr nlk, sub;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_MATRIX, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_MATRIX, -1);
	}

	sub = find_dom_node_by_name(nlk, 0, DOC_MATRIX_DATA, -1);
	if (!sub)
	{
		sub = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(sub, DOC_MATRIX_DATA, -1);
	}

	set_dom_node_text(sub, data, len);
}

#endif /*XDL_SUPPORT_DOC*/
