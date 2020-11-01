/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc utility document

	@module	xdsutil.c | implement file

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

#include "xdsutil.h"
#include "gdiattr.h"
#include "xdsstr.h"
#include "strutil.h"
#include "xdscolor.h"

void rgb_to_hsl(unsigned char r, unsigned char g, unsigned char b, short* ph, short* ps, short* pl)
{
	float fr, fg, fb;
	float fh, fs, fl;
	float fmax, fmin;

	fr = (float)((float)r / 255.0);
	fg = (float)((float)g / 255.0);
	fb = (float)((float)b / 255.0);

	fmax = max(fr, max(fg, fb));
	fmin = min(fr, min(fg, fb));

	if (fmax == fmin)
	{
		fh = 0.0;
	}
	else
	{
		if (fr == fmax)
		{
			if (fg > fb)
				fh = (float)(60.0 * (fg - fb) / (fmax - fmin));
			else
				fh = (float)(60.0 * (fg - fb) / (fmax - fmin) + 360.0);
		}
		else if (fg == fmax)
		{
			fh = (float)(60.0 * (fb - fr) / (fmax - fmin) + 120.0);
		}
		else
		{
			fh = (float)(60.0 * (fr - fg) / (fmax - fmin) + 240.0);
		}

		fh = (fh > 360) ? 360 : ((fh < 0) ? 0 : fh);
	}

	fl = (float)((fmax + fmin) / 2.0);
	if (fl == 0 || fmax == fmin)
		fs = 0;
	else if (fl < 0.5)
		fs = (float)((fmax - fmin) / (fmax + fmin));
	else
		fs = (float)((fmax - fmin) / (2.0 - fmax - fmin));

	fs = (fs > 100) ? 100 : ((fs < 0) ? 0 : fs);
	fs *= 100;

	fl = (fl > 100) ? 100 : ((fl < 0) ? 0 : fl);
	fl *= 100;

	*ph = (short)fh;
	*ps = (short)fs;
	*pl = (short)fl;
}

void hsl_to_rgb(short h, short s, short l, unsigned char* pr, unsigned char* pg, unsigned char* pb)
{
	float fh, fs, fl;
	float fr, fg, fb;
	float p, q;
	float f[3];
	int i;

	fh = (float)((float)h / 360.0);
	fs = (float)((float)s / 100.0);
	fl = (float)((float)l / 100.0);

	if (fs == 0)
	{
		fr = fg = fb = (float)(fl * 255.0);
	}
	else
	{
		if (fl < 0.5)
			q = (float)(fl * (fs + 1.0));
		else
			q = fl + fs - (float)(fl * fs);

		p = (float)(fl * 2.0) - q;

		f[0] = fh + (float)(1.0 / 3.0);
		f[1] = fh;
		f[2] = fh - (float)(1.0 / 3.0);
		for (i = 0; i< 3; i++)
		{
			if (f[i] < 0)
				f[i] += 1.0;
			if (f[i] > 1)
				f[i] -= 1.0;

			if (f[i] * 6 < 1)
				f[i] = p + (float)((q - p) * 6.0 * f[i]);
			else if (f[i] * 2 < 1)
				f[i] = q;
			else if (f[i] * 3 < 2)
				f[i] = p + (float)((q - p) * ((2.0 / 3.0) - f[i]) * 6.0);
			else
				f[i] = p;
		}

		fr = (float)(f[0] * 255.0);
		fg = (float)(f[1] * 255.0);
		fb = (float)(f[2] * 255.0);
	}

	fr = (fr > 255) ? 255 : ((fr < 0) ? 0 : fr);
	fg = (fg > 255) ? 255 : ((fg < 0) ? 0 : fg);
	fb = (fb > 255) ? 255 : ((fb < 0) ? 0 : fb);

	*pr = (unsigned char)fr;
	*pg = (unsigned char)fg;
	*pb = (unsigned char)fb;
}

void lighten_xcolor(xcolor_t* clr, int n)
{
	short h, s, l;

	rgb_to_hsl(clr->r, clr->g, clr->b, &h, &s, &l);

	l += n;

	if (l > 100)
		l = 100;
	else if (l < -100)
		l = -100;

	hsl_to_rgb(h, s, l, &clr->r, &clr->g, &clr->b);
}

bool_t is_whiteness_xcolor(const xcolor_t* pxc)
{
	return (pxc->r == 255 && pxc->g == 255 && pxc->b == 255) ? 1 : 0;
}

bool_t is_blackness_xcolor(const xcolor_t* pxc)
{
	return (pxc->r == 0 && pxc->g == 0 && pxc->b == 0) ? 1 : 0;
}

bool_t is_grayness_xcolor(const xcolor_t* pxc)
{
	return (pxc->r == 169 && pxc->g == 169 && pxc->b == 169) ? 1 : 0;
}

void parse_xcolor(xcolor_t* pxc, const tchar_t* color)
{
	int len;
	tchar_t* token;
	tchar_t val[10];
	tchar_t clr[CLR_LEN + 1] = { 0 };

	pxc->r = pxc->g = pxc->b = 0;
	if (is_null(color))
		return;

	if (xsncmp(color, _T("RGB("), 4) == 0)
	{
		token = (tchar_t*)color + 4;
	}
	else if (*color == _T('#'))
	{
		token = (tchar_t*)color;
		len = xslen(token);
		
		if (len >= 2)
			pxc->r = (unsigned char)hexntol(token, 2);
		if (len >= 4)
			pxc->g = (unsigned char)hexntol((token + 2), 2);
		if (len >= 6)
			pxc->b = (unsigned char)hexntol((token + 4), 2);

		return;
	}
	else
	{
		find_color(color, NULL, clr, NULL);
		token = clr;
	}

	len = 0;
	while (*token != _T(' ') && *token != _T('.') && *token != _T(',') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	len = (len < 3) ? len : 3;
	if (len)
	{
		xsncpy(val, token - len, len);
		val[len] = _T('\0');
		pxc->r = (unsigned char)xstol(val);
	}

	if (*token == _T(' ') || *token == _T('.') || *token == _T(','))
		token++;

	len = 0;
	while (*token != _T(' ') && *token != _T('.') && *token != _T(',') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	len = (len < 3) ? len : 3;
	if (len)
	{
		xsncpy(val, token - len, len);
		val[len] = _T('\0');
		pxc->g = (unsigned char)xstol(val);
	}

	if (*token == _T(' ') || *token == _T('.') || *token == _T(','))
		token++;

	len = 0;
	while (*token != _T(')') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	len = (len < 3) ? len : 3;
	if (len)
	{
		xsncpy(val, token - len, len);
		val[len] = _T('\0');
		pxc->b = (unsigned char)xstol(val);
	}
}

void format_xcolor(const xcolor_t* pxc, tchar_t* buf)
{
	xsprintf(buf, _T("RGB(%d,%d,%d)"), pxc->r, pxc->g, pxc->b);
}

bool_t is_null_xpen(const xpen_t* pxp)
{
	if (!pxp)
		return 1;

	return (is_null(pxp->size)) ? 1 : 0;
}

bool_t is_null_xbrush(const xbrush_t* pxb)
{
	if (!pxb)
		return 1;

	return (is_null(pxb->color)) ? 1 : 0;
}

bool_t is_null_xfont(const xfont_t* pxf)
{
	if (!pxf)
		return 1;

	return (is_null(pxf->size)) ? 1 : 0;
}

bool_t is_null_xface(const xface_t* pxa)
{
	if (!pxa)
		return 1;

	return (is_null(pxa->line_align) && is_null(pxa->text_align) && is_null(pxa->text_wrap)) ? 1 : 0;
}

void default_xpen(xpen_t* pxp)
{
	a_xszero((schar_t*)pxp, sizeof(xpen_t));
	
	xscpy(pxp->style, GDI_ATTR_STROKE_STYLE_SOLID);
	xscpy(pxp->color, GDI_ATTR_RGB_GRAY);
	xscpy(pxp->size, _T("1"));
	xscpy(pxp->opacity, GDI_ATTR_OPACITY_SOFT);
}

void default_xbrush(xbrush_t* pxb)
{
	a_xszero((schar_t*)pxb, sizeof(xbrush_t));

	xscpy(pxb->style, GDI_ATTR_FILL_STYLE_SOLID);
	xscpy(pxb->color, GDI_ATTR_RGB_WHITE);
	xscpy(pxb->opacity, GDI_ATTR_OPACITY_SOFT);
}

void default_xfont(xfont_t* pxf)
{
	a_xszero((schar_t*)pxf, sizeof(xfont_t));

	xscpy(pxf->style, GDI_ATTR_FONT_STYLE_NORMAL);
	xscpy(pxf->size, GDI_ATTR_FONT_SIZE_SYSTEM);
	xscpy(pxf->weight, GDI_ATTR_FONT_WEIGHT_NORMAL);
	xscpy(pxf->color, GDI_ATTR_RGB_DARKBLACK);
	xscpy(pxf->family, _T(""));
}

void default_xface(xface_t* pxa)
{
	a_xszero((schar_t*)pxa, sizeof(xface_t));

	xscpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
	xscpy(pxa->line_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(pxa->line_height, DEF_GDI_TEXT_LINE_HEIGHT);
	xscpy(pxa->text_wrap, _T(""));
}

void merge_xpen(xpen_t* pxp_dst, const xpen_t* pxp_src)
{
	if (is_null(pxp_dst->style))
		xscpy(pxp_dst->style, pxp_src->style);
	if (is_null(pxp_dst->color))
		xscpy(pxp_dst->color, pxp_src->color);
	if (is_null(pxp_dst->size))
		xscpy(pxp_dst->size, pxp_src->size);
	if (is_null(pxp_dst->opacity))
		xscpy(pxp_dst->opacity, pxp_src->opacity);
}

void merge_xbrush(xbrush_t* pxb_dst, const xbrush_t* pxb_src)
{
	if (is_null(pxb_dst->style))
		xscpy(pxb_dst->style, pxb_src->style);
	if (is_null(pxb_dst->color))
		xscpy(pxb_dst->color, pxb_src->color);
	if (is_null(pxb_dst->opacity))
		xscpy(pxb_dst->opacity, pxb_src->opacity);
	if (is_null(pxb_dst->linear))
		xscpy(pxb_dst->linear, pxb_src->linear);
	if (is_null(pxb_dst->gradient))
		xscpy(pxb_dst->gradient, pxb_src->gradient);
}

void merge_xfont(xfont_t* pxf_dst, const xfont_t* pxf_src)
{
	if (is_null(pxf_dst->style))
		xscpy(pxf_dst->style, pxf_src->style);
	if (is_null(pxf_dst->size))
		xscpy(pxf_dst->size, pxf_src->size);
	if (is_null(pxf_dst->weight))
		xscpy(pxf_dst->weight, pxf_src->weight);
	if (is_null(pxf_dst->family))
		xscpy(pxf_dst->family, pxf_src->family);
	if (is_null(pxf_dst->color))
		xscpy(pxf_dst->color, pxf_src->color);
}

void merge_xface(xface_t* pxa_dst, const xface_t* pxa_src)
{
	if (is_null(pxa_dst->text_align))
		xscpy(pxa_dst->text_align, pxa_src->text_align);
	if (is_null(pxa_dst->line_align))
		xscpy(pxa_dst->line_align, pxa_src->line_align);
	if (is_null(pxa_dst->line_height))
		xscpy(pxa_dst->line_height, pxa_src->line_height);
	if (is_null(pxa_dst->text_wrap))
		xscpy(pxa_dst->text_wrap, pxa_src->text_wrap);
}

void lighten_xpen(xpen_t* pxp, int n)
{
	xcolor_t xc = { 0 };

	if (is_null(pxp->color))
		return;

	parse_xcolor(&xc, pxp->color);

	lighten_xcolor(&xc, n);

	format_xcolor(&xc, pxp->color);
}

void lighten_xbrush(xbrush_t* pxb, int n)
{
	xcolor_t xc = { 0 };

	if (is_null(pxb->color))
		return;

	parse_xcolor(&xc, pxb->color);

	lighten_xcolor(&xc, n);

	format_xcolor(&xc, pxb->color);
}

void lighten_xfont(xfont_t* pxf, int n)
{
	xcolor_t xc = { 0 };

	if (is_null(pxf->color))
		return;

	parse_xcolor(&xc, pxf->color);

	lighten_xcolor(&xc, n);

	format_xcolor(&xc, pxf->color);
}


void parse_xpen_from_style(xpen_t* pxp, const tchar_t* style)
{
	tchar_t *key, *val;
	int klen, vlen;
	int len, n, total = 0;

	len = xslen(style);
	while (n = parse_options_token((style + total), (len - total), CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen))
	{
		total += n;

		if (!klen)
			break;

		if (xsnicmp(GDI_ATTR_STROKE_STYLE, key, klen) == 0)
			xsncpy(pxp->style, val, vlen);
		else if (xsnicmp(GDI_ATTR_STROKE_COLOR, key, klen) == 0)
			xsncpy(pxp->color, val, vlen);
		else if (xsnicmp(GDI_ATTR_STROKE_WIDTH, key, klen) == 0)
			xsncpy(pxp->size, val, vlen);
		else if (xsnicmp(GDI_ATTR_STROKE_OPACITY, key, klen) == 0)
			xsncpy(pxp->opacity, val, vlen);
	}
}

int format_xpen_to_style(const xpen_t* pxp, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(pxp->style))
	{
		len = xslen(GDI_ATTR_STROKE_STYLE) + xslen(pxp->style) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_STYLE, CSS_ITEMFEED, pxp->style, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxp->size))
	{
		len = xslen(GDI_ATTR_STROKE_WIDTH) + xslen(pxp->size) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_WIDTH, CSS_ITEMFEED, pxp->size, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxp->color))
	{
		len = xslen(GDI_ATTR_STROKE_COLOR) + xslen(pxp->color) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_COLOR, CSS_ITEMFEED, pxp->color, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxp->opacity))
	{
		len = xslen(GDI_ATTR_STROKE_OPACITY) + xslen(pxp->opacity) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_OPACITY, CSS_ITEMFEED, pxp->opacity, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_xbrush_from_style(xbrush_t* pxb, const tchar_t* style)
{
	tchar_t *key, *val;
	int klen, vlen;
	int len, n, total = 0;

	len = xslen(style);
	while (n = parse_options_token((style + total), (len - total), CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen))
	{
		total += n;

		if (!klen)
			break;

		if (xsnicmp(GDI_ATTR_FILL_STYLE, key, klen) == 0)
			xsncpy(pxb->style, val, vlen);
		else if (xsnicmp(GDI_ATTR_FILL_COLOR, key, klen) == 0)
			xsncpy(pxb->color, val, vlen);
		else if (xsnicmp(GDI_ATTR_FILL_OPACITY, key, klen) == 0)
			xsncpy(pxb->opacity, val, vlen);
		else if (xsnicmp(GDI_ATTR_STOP_COLOR, key, klen) == 0)
			xsncpy(pxb->linear, val, vlen);
		else if (xsnicmp(GDI_ATTR_GRADIENT, key, klen) == 0)
			xsncpy(pxb->gradient, val, vlen);
	}
}

int format_xbrush_to_style(const xbrush_t* pxb, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(pxb->style))
	{
		len = xslen(GDI_ATTR_FILL_STYLE) + xslen(pxb->style) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FILL_STYLE, CSS_ITEMFEED, pxb->style, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxb->color))
	{
		len = xslen(GDI_ATTR_FILL_COLOR) + xslen(pxb->color) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FILL_COLOR, CSS_ITEMFEED, pxb->color, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxb->linear))
	{
		len = xslen(GDI_ATTR_STOP_COLOR) + xslen(pxb->linear) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STOP_COLOR, CSS_ITEMFEED, pxb->linear, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxb->gradient))
	{
		len = xslen(GDI_ATTR_GRADIENT) + xslen(pxb->color) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_GRADIENT, CSS_ITEMFEED, pxb->gradient, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxb->opacity))
	{
		len = xslen(GDI_ATTR_FILL_OPACITY) + xslen(pxb->opacity) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FILL_OPACITY, CSS_ITEMFEED, pxb->opacity, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_xfont_from_style(xfont_t* pxf, const tchar_t* style)
{
	tchar_t *key, *val;
	int klen, vlen;
	int len, n, total = 0;

	len = xslen(style);
	while (n = parse_options_token((style + total), (len - total), CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen))
	{
		total += n;
		if (!klen)
			break;

		if (xsnicmp(GDI_ATTR_FONT_STYLE, key, klen) == 0)
			xsncpy(pxf->style, val, vlen);
		else if (xsnicmp(GDI_ATTR_FONT_SIZE, key, klen) == 0)
			xsncpy(pxf->size, val, vlen);
		else if (xsnicmp(GDI_ATTR_FONT_WEIGHT, key, klen) == 0)
			xsncpy(pxf->weight, val, vlen);
		else if (xsnicmp(GDI_ATTR_FONT_FAMILY, key, klen) == 0)
			xsncpy(pxf->family, val, vlen);
		else if (xsnicmp(GDI_ATTR_FONT_COLOR, key, klen) == 0)
			xsncpy(pxf->color, val, vlen);
	}
}

int format_xfont_to_style(const xfont_t* pfont, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(pfont->family))
	{
		len = xslen(GDI_ATTR_FONT_FAMILY) + xslen(pfont->family) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_FAMILY, CSS_ITEMFEED, pfont->family, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->style))
	{
		len = xslen(GDI_ATTR_FONT_STYLE) + xslen(pfont->style) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_STYLE, CSS_ITEMFEED, pfont->style, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->size))
	{
		len = xslen(GDI_ATTR_FONT_SIZE) + xslen(pfont->size) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_SIZE, CSS_ITEMFEED, pfont->size, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->weight))
	{
		len = xslen(GDI_ATTR_FONT_WEIGHT) + xslen(pfont->weight) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_WEIGHT, CSS_ITEMFEED, pfont->weight, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->color))
	{
		len = xslen(GDI_ATTR_FONT_COLOR) + xslen(pfont->color) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_COLOR, CSS_ITEMFEED, pfont->color, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_xface_from_style(xface_t* ptt, const tchar_t* style)
{
	tchar_t *key, *val;
	int klen, vlen;
	int n, len, total = 0;

	len = xslen(style);
	while (n = parse_options_token((style + total), (len - total), CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen))
	{
		total += n;

		if (!klen)
			break;

		if (xsnicmp(GDI_ATTR_TEXT_ALIGN, key, klen) == 0)
			xsncpy(ptt->text_align, val, vlen);
		else if (xsnicmp(GDI_ATTR_LINE_ALIGN, key, klen) == 0)
			xsncpy(ptt->line_align, val, vlen);
		else if (xsnicmp(GDI_ATTR_TEXT_WRAP, key, klen) == 0)
			xsncpy(ptt->text_wrap, val, vlen);
		else if (xsnicmp(GDI_ATTR_LINE_HEIGHT, key, klen) == 0)
			xsncpy(ptt->line_height, val, vlen);
	}
}

int format_xface_to_style(const xface_t* ptt, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(ptt->text_align))
	{
		len = xslen(GDI_ATTR_TEXT_ALIGN) + xslen(ptt->text_align) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_TEXT_ALIGN, CSS_ITEMFEED, ptt->text_align, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(ptt->line_align))
	{
		len = xslen(GDI_ATTR_LINE_ALIGN) + xslen(ptt->line_align) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_LINE_ALIGN, CSS_ITEMFEED, ptt->line_align, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(ptt->text_wrap))
	{
		len = xslen(GDI_ATTR_TEXT_WRAP) + xslen(ptt->text_wrap) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_TEXT_WRAP, CSS_ITEMFEED, ptt->text_wrap, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(ptt->line_height))
	{
		len = xslen(GDI_ATTR_LINE_HEIGHT) + xslen(ptt->line_height) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_LINE_HEIGHT, CSS_ITEMFEED, ptt->line_height, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_ximage_from_source(ximage_t* pxi, const tchar_t* token)
{
	if (xsnicmp(token, GDI_ATTR_IMAGE_TYPE_JPG, xslen(GDI_ATTR_IMAGE_TYPE_JPG)) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_JPG);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_JPG);
	}
	else if (xsnicmp(token, GDI_ATTR_IMAGE_TYPE_PNG, xslen(GDI_ATTR_IMAGE_TYPE_PNG)) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_PNG);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_PNG);
	}
	else if (xsnicmp(token, GDI_ATTR_IMAGE_TYPE_BMP, xslen(GDI_ATTR_IMAGE_TYPE_BMP)) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_BMP);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_BMP);
	}
	else if (xsnicmp(token, GDI_ATTR_IMAGE_TYPE_URL, xslen(GDI_ATTR_IMAGE_TYPE_URL)) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_URL);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_URL);
	}
	else
	{
		xscpy(pxi->type, _T(""));
		pxi->source = NULL;
	}
}

int format_ximage_to_source(const ximage_t* pxi, tchar_t* buf, int len)
{
	int size;

	size = xslen(pxi->type);

	if (size > len)
		return size;

	if (buf)
	{
		xscpy(buf, pxi->type);
	}

	size += xslen(pxi->source);

	if (size > len)
		return size;

	if (buf)
	{
		xscpy(buf + xslen(pxi->type), pxi->source);
	}

	return size;
}


/************************************************************************************************/

bool_t pt_inside(int x, int y, int x1, int y1, int x2, int y2)
{
	int minx, maxx, miny, maxy;

	minx = (x1 < x2) ? x1 : x2;
	maxx = (x1 > x2) ? x1 : x2;
	miny = (y1 < y2) ? y1 : y2;
	maxy = (y1 > y2) ? y1 : y2;

	return (x >= minx && x <= maxx && y >= miny && y <= maxy) ? 1 : 0;
}

bool_t ft_inside(float x, float y, float x1, float y1, float x2, float y2)
{
	float minx, maxx, miny, maxy;

	minx = (x1 < x2) ? x1 : x2;
	maxx = (x1 > x2) ? x1 : x2;
	miny = (y1 < y2) ? y1 : y2;
	maxy = (y1 > y2) ? y1 : y2;

	return (x >= minx && x <= maxx && y >= miny && y <= maxy) ? 1 : 0;
}

bool_t pt_in_rect(const xpoint_t* ppt, const xrect_t* pxr)
{
	return pt_inside(ppt->x, ppt->y, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}


bool_t ft_in_rect(const xpoint_t* ppt, const xrect_t* pxr)
{
	return ft_inside(ppt->fx, ppt->fy, pxr->fx, pxr->fy, pxr->fx + pxr->fw, pxr->fy + pxr->fh);
}

void ft_offset_point(xpoint_t* ppt, float cx, float cy)
{
	ppt->fx += cx;
	ppt->fy += cy;
}

void pt_offset_point(xpoint_t* ppt, int cx, int cy)
{
	ppt->x += cx;
	ppt->y += cy;
}

void ft_center_rect(xrect_t* pxr, float cx, float cy)
{
	pxr->fx += (pxr->fw - cx) / 2;
	pxr->fw = cx;
	pxr->fy += (pxr->fh - cy) / 2;
	pxr->fh = cy;
}

void pt_center_rect(xrect_t* pxr, int cx, int cy)
{
	pxr->x += (pxr->w - cx) / 2;
	pxr->w = cx;
	pxr->y += (pxr->h - cy) / 2;
	pxr->h = cy;
}

void pt_expand_rect(xrect_t* pxr, int cx, int cy)
{
	pxr->x -= cx;
	pxr->w += cx * 2;
	pxr->y -= cy;
	pxr->h += cy * 2;
}

void ft_expand_rect(xrect_t* pxr, float cx, float cy)
{
	pxr->fx -= cx;
	pxr->fw += cx * 2;
	pxr->fy -= cy;
	pxr->fh += cy * 2;
}

void pt_offset_rect(xrect_t* pxr, int cx, int cy)
{
	pxr->x += cx;
	pxr->y += cy;
}

void ft_offset_rect(xrect_t* pxr, float cx, float cy)
{
	pxr->fx += cx;
	pxr->fy += cy;
}

void ft_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt)
{
	float left, top, right, bottom;

	left = (pxr->fx < pxr_nxt->fx) ? pxr->fx : pxr_nxt->fx;
	top = (pxr->fy < pxr_nxt->fy) ? pxr->fy : pxr_nxt->fy;
	right = (pxr->fx + pxr->fw > pxr_nxt->fx + pxr_nxt->fw) ? (pxr->fx + pxr->fw) : (pxr_nxt->fx + pxr_nxt->fw);
	bottom = (pxr->fy + pxr->fh > pxr_nxt->fy + pxr_nxt->fh) ? (pxr->fy + pxr->fh) : (pxr_nxt->fy + pxr_nxt->fh);

	pxr->fx = left;
	pxr->fy = top;
	pxr->fw = right - left;
	pxr->fh = bottom - top;
}

void pt_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt)
{
	int left, top, right, bottom;

	left = (pxr->x < pxr_nxt->x) ? pxr->x : pxr_nxt->x;
	top = (pxr->y < pxr_nxt->y) ? pxr->y : pxr_nxt->y;
	right = (pxr->x + pxr->w > pxr_nxt->x + pxr_nxt->w) ? (pxr->x + pxr->w) : (pxr_nxt->x + pxr_nxt->w);
	bottom = (pxr->y + pxr->h > pxr_nxt->y + pxr_nxt->h) ? (pxr->y + pxr->h) : (pxr_nxt->y + pxr_nxt->h);

	pxr->x = left;
	pxr->y = top;
	pxr->w = right - left;
	pxr->h = bottom - top;
}

bool_t ft_clip_rect(xrect_t* pxr, const xrect_t* pxrClp)
{
	if (pxr->fx == pxrClp->fx && pxr->fy == pxrClp->fy)
	{
		if (pxr->fh == pxrClp->fh && pxr->fw > pxrClp->fw)
		{
			pxr->fx += pxrClp->fw;
			return 1;
		}
		else if (pxr->fw == pxrClp->fw && pxr->fh > pxrClp->fh)
		{
			pxr->fy += pxrClp->fh;
			return 1;
		}
	}
	else if (pxr->fx + pxr->fw == pxrClp->fx + pxrClp->fw && pxr->fy + pxr->fh == pxrClp->fy + pxrClp->fh)
	{
		if (pxr->fh == pxrClp->fh && pxr->fw > pxrClp->fw)
		{
			pxr->fw -= pxrClp->fw;
			return 1;
		}
		else if (pxr->fw == pxrClp->fw && pxr->fh > pxrClp->fh)
		{
			pxr->fh -= pxrClp->fh;
			return 1;
		}
	}

	return 0;
}

bool_t pt_clip_rect(xrect_t* pxr, const xrect_t* pxrClp)
{
	if (pxr->x == pxrClp->x && pxr->y == pxrClp->y)
	{
		if (pxr->h == pxrClp->h && pxr->w > pxrClp->w)
		{
			pxr->x += pxrClp->w;
			return 1;
		}
		else if (pxr->w == pxrClp->w && pxr->h > pxrClp->h)
		{
			pxr->y += pxrClp->h;
			return 1;
		}
	}
	else if (pxr->x + pxr->w == pxrClp->x + pxrClp->w && pxr->y + pxr->h == pxrClp->y + pxrClp->h)
	{
		if (pxr->h == pxrClp->h && pxr->w > pxrClp->w)
		{
			pxr->w -= pxrClp->w;
			return 1;
		}
		else if (pxr->w == pxrClp->w && pxr->h > pxrClp->h)
		{
			pxr->h -= pxrClp->h;
			return 1;
		}
	}

	return 0;
}

void ft_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub)
{
	if (pxr->fx < pxr_sub->fx)
	{
		pxr->fx = pxr_sub->fx;
		pxr->fw -= (pxr_sub->fx - pxr->fx);
	}
	else
	{
		pxr->fw += (pxr_sub->fx - pxr->fx);
	}

	if (pxr->fy < pxr_sub->fy)
	{
		pxr->fy = pxr_sub->fy;
		pxr->fh -= (pxr_sub->fy - pxr->fy);
	}
	else
	{
		pxr->fh += (pxr_sub->fy - pxr->fy);
	}
}

void pt_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub)
{
	if (pxr->x < pxr_sub->x)
	{
		pxr->x = pxr_sub->x;
		pxr->w -= (pxr_sub->x - pxr->x);
	}
	else
	{
		pxr->w += (pxr_sub->x - pxr->x);
	}

	if (pxr->y < pxr_sub->y)
	{
		pxr->y = pxr_sub->y;
		pxr->h -= (pxr_sub->y - pxr->y);
	}
	else
	{
		pxr->h += (pxr_sub->y - pxr->y);
	}
}

void ft_inter_square(xrect_t* pxr, const xrect_t* pxr_org)
{
	xpoint_t pt;
	float r;

	pt.fx = pxr_org->fx + pxr_org->fw / 2;
	pt.fy = pxr_org->fy + pxr_org->fh / 2;

	r = (pxr_org->fw < pxr_org->fh) ? pxr_org->fw / 2 : pxr_org->fh / 2;

	pxr->fx = pt.fx - r;
	pxr->fy = pt.fy - r;
	pxr->fw = 2 * r;
	pxr->fh = 2 * r;
}

void pt_inter_square(xrect_t* pxr, const xrect_t* pxr_org)
{
	xpoint_t pt;
	int r;

	pt.x = pxr_org->x + pxr_org->w / 2;
	pt.y = pxr_org->y + pxr_org->h / 2;

	r = (pxr_org->w < pxr_org->h) ? pxr_org->w / 2 : pxr_org->h / 2;

	pxr->x = pt.x - r;
	pxr->y = pt.y - r;
	pxr->w = 2 * r;
	pxr->h = 2 * r;
}

void ft_cell_rect(xrect_t* pxr, bool_t horz, int rows, int cols, int index)
{
	int row, col;

	if (horz)
	{
		row = index / cols;
		col = index % cols;
	}
	else
	{
		col = index / rows;
		row = index % rows;
	}

	pxr->fx += pxr->fw * col / cols;
	pxr->fw = pxr->fw / cols;
	pxr->fy += pxr->fh * row / rows;
	pxr->fh = pxr->fh / rows;
}

void pt_cell_rect(xrect_t* pxr, bool_t horz, int rows, int cols, int index)
{
	int row, col;

	if (horz)
	{
		row = index / cols;
		col = index % cols;
	}
	else
	{
		col = index / rows;
		row = index % rows;
	}

	pxr->x += pxr->w * col / cols;
	pxr->w = pxr->w / cols;
	pxr->y += pxr->h * row / rows;
	pxr->h = pxr->h / rows;
}

void empty_rect(xrect_t* pxr)
{
	pxr->fx = pxr->fy = pxr->fw = pxr->fh = 0;
}

bool_t rect_is_empty(const xrect_t* pxr)
{
	return (!pxr->w || !pxr->h) ? 1 : 0;
}

void pt_adjust_rect(xrect_t* pxr, int src_width, int src_height, const tchar_t* horz_align, const tchar_t* vert_align)
{
	if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		pxr->w = (pxr->w < src_width) ? pxr->w : src_width;
	}
	else if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		pxr->x = (pxr->w < src_width) ? pxr->x : (pxr->x + (pxr->w - src_width) / 2);
	}
	else if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		pxr->x = (pxr->w < src_width) ? pxr->x : (pxr->x + pxr->w - src_width);
	}

	if (xscmp(vert_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		pxr->h = (pxr->h < src_height) ? pxr->h : src_height;
	}
	else if(xscmp(vert_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		pxr->y = (pxr->h < src_height) ? pxr->y : (pxr->y + (pxr->h - src_height) / 2);
	}
	else if (xscmp(vert_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		pxr->y = (pxr->h < src_height) ? pxr->y : (pxr->y + pxr->h - src_height);
	}
}

void ft_adjust_rect(xrect_t* pxr, float src_width, float src_height, const tchar_t* horz_align, const tchar_t* vert_align)
{
	if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		pxr->fw = (pxr->fw < src_width) ? pxr->fw : src_width;
	}
	else if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		pxr->fx = (pxr->fw < src_width) ? pxr->fx : (pxr->fx + (pxr->fw - src_width) / 2);
	}
	else if (xscmp(horz_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		pxr->fx = (pxr->fw < src_width) ? pxr->fx : (pxr->fx + pxr->fw - src_width);
	}

	if (xscmp(vert_align, GDI_ATTR_TEXT_ALIGN_NEAR) == 0)
	{
		pxr->fh = (pxr->fh < src_height) ? pxr->fh : src_height;
	}
	else if (xscmp(vert_align, GDI_ATTR_TEXT_ALIGN_CENTER) == 0)
	{
		pxr->fy = (pxr->fh < src_height) ? pxr->fy : (pxr->fy + (pxr->fh - src_height) / 2);
	}
	else if (xscmp(vert_align, GDI_ATTR_TEXT_ALIGN_FAR) == 0)
	{
		pxr->fy = (pxr->fh < src_height) ? pxr->fy : (pxr->fy + pxr->fh - src_height);
	}
}

/*********************************************************************************************/

int mul_div_int(int m1, int m2, int d)
{
	return (int)((double)(m1 * m2) / (double)d);
}

short mul_div_short(short m1, short m2, short d)
{
	return (short)((float)(m1 * m2) / (float)d);
}

bool_t is_zero_float(float f)
{
	f -= (int)f;
	f *= 1000;

	return ((int)f) ? 0 : 1;
}

bool_t is_zero_double(double d)
{
	d -= (int)d;
	d *= 1000000;

	return ((int)d) ? 0 : 1;
}
/*********************************************************************************************/

void split_path(const tchar_t* pathfile, tchar_t* path, tchar_t* file, tchar_t* ext)
{
	const tchar_t* token;
	int n, extlen = 0;

	if (is_null(pathfile))
		return;

	token = pathfile + xslen(pathfile);
	while (token != pathfile && *token != _T('\\') && *token != _T('/'))
	{
		token--;

		if (*token == _T('.'))
		{
			extlen = xslen(token);
			if (ext)
			{
				n = (extlen - 1 < RES_LEN) ? (extlen - 1) : RES_LEN;
				xsncpy(ext, token + 1, n);
			}
			break;
		}
	}

	while (token != pathfile && *token != _T('\\') && *token != _T('/'))
	{
		token--;
	}

	if (file)
	{
		if (*token == _T('\\') || *token == _T('/'))
		{
			n = xslen(token + 1) - extlen;
			if (n > PATH_LEN) n = PATH_LEN;

			xsncpy(file, token + 1, n);
		}
		else
		{
			n = xslen(token) - extlen;
			if (n > PATH_LEN) n = PATH_LEN;

			xsncpy(file, token, n);
		}
	}

	if (path)
	{
		n = (int)(token - pathfile);
		if (n > PATH_LEN) n = PATH_LEN;

		xsncpy(path, pathfile, n);
	}
}

void split_file(const tchar_t* pathfile, tchar_t* path, tchar_t* file)
{
	const tchar_t* token;
	int n;

	if (is_null(pathfile))
		return;

	token = pathfile + xslen(pathfile);

	while (token != pathfile && *token != _T('\\') && *token != _T('/'))
	{
		token--;
	}

	if (file)
	{
		if (*token == _T('\\') || *token == _T('/'))
		{
			n = xslen(token + 1);
			if (n > PATH_LEN) n = PATH_LEN;

			xsncpy(file, token + 1, n);
		}
		else
		{
			n = xslen(token);
			if (n > PATH_LEN) n = PATH_LEN;

			xsncpy(file, token, n);
		}
	}

	if (path)
	{
		n = (int)(token - pathfile);
		if (n > PATH_LEN) n = PATH_LEN;

		xsncpy(path, pathfile, n);
	}
}
