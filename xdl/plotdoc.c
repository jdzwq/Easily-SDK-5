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
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_plot_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_PLOT, -1);

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
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_MATRIX, -1);

	if (nlk)
	{
		set_dom_node_text(nlk, NULL, 0);
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

int	get_plot_title(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_TITLE, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_title_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_TITLE, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_title(link_t_ptr ptr, const tchar_t* title, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_TITLE, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_TITLE, -1);
	}

	set_dom_node_text(nlk, title, len);
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

int	get_plot_y_bases(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_BASES, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_y_bases_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_BASES, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_y_bases(link_t_ptr ptr, const tchar_t* y_bases, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_BASES, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_BASES, -1);
	}

	set_dom_node_text(nlk, y_bases, len);
}


int	get_plot_y_steps(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STEPS, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_y_steps_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STEPS, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_y_steps(link_t_ptr ptr, const tchar_t* y_steps, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_STEPS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_STEPS, -1);
	}

	set_dom_node_text(nlk, y_steps, len);
}


int	get_plot_y_labels(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_LABELS, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_y_labels_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_LABELS, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_y_labels(link_t_ptr ptr, const tchar_t* y_labels, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_LABELS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_LABELS, -1);
	}

	set_dom_node_text(nlk, y_labels, len);
}


int	get_plot_y_colors(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_COLORS, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_y_colors_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_COLORS, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_y_colors(link_t_ptr ptr, const tchar_t* y_colors, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_Y_COLORS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_Y_COLORS, -1);
	}

	set_dom_node_text(nlk, y_colors, len);
}

int	get_plot_x_labels(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_LABELS, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_x_labels_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_LABELS, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_x_labels(link_t_ptr ptr, const tchar_t* x_labels, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_LABELS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_X_LABELS, -1);
	}

	set_dom_node_text(nlk, x_labels, len);
}

int	get_plot_x_colors(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_COLORS, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_x_colors_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_COLORS, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_x_colors(link_t_ptr ptr, const tchar_t* x_colors, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_X_COLORS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_X_COLORS, -1);
	}

	set_dom_node_text(nlk, x_colors, len);
}

int	get_plot_rows(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_ROWS, -1);

	return (nlk) ? xstol(get_dom_node_text_ptr(nlk)) : 0;
}

void set_plot_rows(link_t_ptr ptr, int rows)
{
	link_t_ptr nlk;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_ROWS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_ROWS, -1);
	}

	ltoxs(rows, num, NUM_LEN);
	set_dom_node_text(nlk, num, -1);
}

int	get_plot_cols(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_COLS, -1);

	return (nlk) ? xstol(get_dom_node_text_ptr(nlk)) : 0;
}

void set_plot_cols(link_t_ptr ptr, int cols)
{
	link_t_ptr nlk;
	tchar_t num[NUM_LEN] = { 0 };

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_COLS, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_COLS, -1);
	}

	ltoxs(cols, num, NUM_LEN);
	set_dom_node_text(nlk, num, -1);
}

int	get_plot_matrix(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_MATRIX, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_plot_matrix_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_MATRIX, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_plot_matrix(link_t_ptr ptr, const tchar_t* matrix, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_PLOT_MATRIX, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_LAST);
		set_dom_node_name(nlk, DOC_PLOT_MATRIX, -1);
	}

	set_dom_node_text(nlk, matrix, len);
}

#endif /*XDL_SUPPORT_DOC*/
