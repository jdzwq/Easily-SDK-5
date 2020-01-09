/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text view

	@module	textview.c | text view implement file

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
#include "textview.h"
#include "textdoc.h"
#include "xdllink.h"
#include "hashtable.h"
#include "stacktable.h"
#include "xmldoc.h"
#include "domdoc.h"
#include "xdlutil.h"
#include "xdlstr.h"
#include "xdlmem.h"

/************************************************************************************************/
long calc_text_height(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr)
{
	link_t_ptr ilk;
	long line_width, text_height, total_height = 0;
	float line_height;
	const tchar_t* sz_text;
	int i, len;
	xfont_t xf = { 0 };
	xsize_t se = { 0 };

	line_height = 1.5;
	line_width = 0;
	text_height = 0;

	ilk = get_dom_first_child_node(ptr);
	while (ilk)
	{
		sz_text = get_dom_node_text_ptr(ilk);
		len = xslen(sz_text);
		for (i = 0; i < len; i++)
		{
			(*pcanv->pf_text_size)(pcanv->canvas, &xf, sz_text + i, 1, &se);

			if (se.cy > text_height)
			{
				text_height = se.cy;
			}

			if (line_width + se.cx > pbox->width)
			{
				total_height += (long)(text_height * line_height);
				line_width = 0;
				text_height = 0;
			}
			else
			{
				line_width += se.cx;
			}
		}

		ilk = get_text_next_sibling_item(ilk);
	}

	return total_height;
}

void calc_text_hint(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk, int* row_at, int* col_at)
{
	link_t_ptr ilk;
	long line_width, text_height, total_height = 0;
	float line_height;
	const tchar_t* sz_text;
	int i, len;
	xfont_t xf = { 0 };
	xsize_t se = { 0 };

	line_height = 1.5;
	line_width = 0;
	text_height = 0;

	*row_at = *col_at = 0;
	*pilk = NULL;

	ilk = get_dom_first_child_node(ptr);
	while (ilk)
	{
		sz_text = get_dom_node_text_ptr(ilk);
		len = xslen(sz_text);
		for (i = 0; i < len; i++)
		{
			(*pcanv->pf_text_size)(pcanv->canvas, &xf, sz_text + i, 1, &se);

			if (se.cy > text_height)
			{
				text_height = se.cy;
			}

			if (line_width + se.cx > pbox->width)
			{
				total_height += (long)(text_height * line_height);
				line_width = 0;
				text_height = 0;

				*row_at = *row_at + 1;
				*col_at = 0;
			}
			else
			{
				line_width += se.cx;
				*col_at = *col_at + 1;
			}

			if (ppt->x > line_width - 10 && ppt->x < line_width + 10 && ppt->y > total_height - (long)(text_height * line_height) && ppt->y < total_height)
			{
				break;
			}
		}

		ilk = get_text_next_sibling_item(ilk);
	}
}

void calc_text_point(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr, int row, int col, xpoint_t* ppt1, xpoint_t* ppt2)
{
	link_t_ptr ilk;
	long line_width, text_height, total_height = 0;
	float line_height;
	const tchar_t* sz_text;
	int i, len, row_at, col_at;
	xfont_t xf = { 0 };
	xsize_t se = { 0 };

	line_height = 1.5;
	line_width = 0;
	text_height = 0;

	(*pcanv->pf_text_metric)(pcanv->canvas, &xf, &text_height, NULL, NULL, NULL);

	ppt1->x = 0;
	ppt1->y = 0;
	ppt2->x = 0;
	ppt2->y = text_height;

	row_at = col_at = 0;
	if (row_at == row && col_at == col)
		return;

	ilk = get_dom_first_child_node(ptr);
	while (ilk)
	{
		sz_text = get_dom_node_text_ptr(ilk);
		len = xslen(sz_text);
		for (i = 0; i < len; i++)
		{
			(*pcanv->pf_text_size)(pcanv->canvas, &xf, sz_text + i, 1, &se);

			if (se.cy > text_height)
			{
				text_height = se.cy;
			}

			if (line_width + se.cx > pbox->width)
			{
				total_height += (long)(text_height * line_height);
				line_width = 0;
				text_height = 0;

				row_at++;
				col_at = 0;
			}
			else
			{
				line_width += se.cx;
				col_at++;
			}

			if (row_at == row && col_at == col)
			{
				ppt1->x = line_width;
				ppt1->y = total_height - (long)(text_height * line_height);
				ppt2->x = line_width;
				ppt2->y = total_height;
				return;
			}
		}

		ilk = get_text_next_sibling_item(ilk);
	}
}

void calc_text_draw(const viewbox_t* pbox, const if_canvas_t* pcanv, link_t_ptr ptr)
{
	link_t_ptr ilk;
	long line_width, text_height, total_height = 0;
	float line_height;
	const tchar_t* sz_text;
	int i, len;
	xfont_t xf = { 0 };
	xcolor_t xc = { 0 };
	xface_t xa = { 0 };
	xsize_t se = { 0 };
	xpoint_t pt = { 0 };

	line_height = 1.5;
	line_width = 0;
	text_height = 0;

	ilk = get_dom_first_child_node(ptr);
	while (ilk)
	{
		sz_text = get_dom_node_text_ptr(ilk);
		len = xslen(sz_text);
		for (i = 0; i < len; i++)
		{
			(*pcanv->pf_text_size)(pcanv->canvas, &xf, sz_text + i, 1, &se);
			
			if (se.cy > text_height)
			{
				text_height = se.cy;
			}

			pt.x = line_width;
			pt.y = total_height;

			(*pcanv->pf_text_out)(pcanv->canvas, &xf, &xc, &pt, sz_text + i, 1);
			
			if (line_width + se.cx > pbox->width)
			{
				total_height += (long)(text_height * line_height);
				line_width = 0;
				text_height = 0;
			}
			else
			{
				line_width += se.cx;
			}
		}

		ilk = get_text_next_sibling_item(ilk);
	}
}
