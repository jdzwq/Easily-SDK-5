/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc coding document

	@module	coding.c | implement file

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

#include "coding.h"

#include "xdlimp.h"
#include "xdlstd.h"

#if defined(XDL_SUPPORT_GDI)

void draw_code128(const if_drawing_t* pif, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black, span;
	dword_t i;
	float unit;
	xrect_t rt;
	xbrush_t xb;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = code128_encode(buf, buf_len, NULL, MAX_LONG);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = code128_encode(buf, buf_len, bar_buf, bar_len);

	xmem_free(buf);

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
	}

	unit = MMPERPD;

	rt.fx = prt->fx + unit;
	rt.fy = prt->fy + unit;
	rt.fw = unit;
	rt.fh = prt->fh - 2 * unit;

	black = 0;
	for (i = 0; i < bar_len; i++)
	{
		span = (bar_buf[i] - '0');
		rt.fw = span * unit;

		black = (black) ? 0 : 1;

		if (black && pxc)
		{
			(*pif->pf_draw_rect)(pif->ctx, NULL, &xb, &rt);
		}

		rt.fx += rt.fw;
	}

	xmem_free(bar_buf);

	prt->fw = rt.fx + unit - prt->fx;
}

void draw_pdf417(const if_drawing_t* pif, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black;
	int rows, cols;
	unsigned char b, c;
	int i, j;
	float unit;

	xrect_t rt;
	xbrush_t xb;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = pdf417_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = pdf417_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
	}

	unit = MMPERPD * 2;

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.fx = prt->fx + unit;
		rt.fw = unit;
		rt.fy = prt->fy + unit + i * 2 * unit;
		rt.fh = 2 * unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.fx += rt.fw;

				black = (c & b) ? 0 : 1;

				if (black && pxc)
				{
					(*pif->pf_draw_rect)(pif->ctx, NULL, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->fw = rt.fx + unit - prt->fx;
	prt->fh = rt.fy + rt.fh + unit - prt->fy;
}

void draw_qrcode(const if_drawing_t* pif, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black;
	int rows, cols;
	unsigned char b, c;
	int i, j;

	float unit;
	xrect_t rt;
	xbrush_t xb;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = qr_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = qr_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	default_xbrush(&xb);

	if (pxc)
		format_xcolor(pxc, xb.color);
	else
		xscpy(xb.color, GDI_ATTR_RGB_BLACK);

	unit = MMPERPD * 2;

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.fx = prt->fx + unit;
		rt.fw = unit;
		rt.fy = prt->fy + unit + i * unit;
		rt.fh = unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.fx += rt.fw;

				black = (c & b) ? 1 : 0;

				if (black && pxc)
				{
					(*pif->pf_draw_rect)(pif->ctx, NULL, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->fw = rt.fx + unit - prt->fx;
	prt->fh = rt.fy + rt.fh + unit - prt->fy;
}

#endif /*XDL_SUPPORT_VIEW*/
