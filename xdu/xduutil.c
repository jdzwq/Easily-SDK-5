/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc utility document

	@module	xduutil.c | implement file

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

#include "xduutil.h"
#include "xduattr.h"

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

void parse_xcolor(xcolor_t* pxc, const tchar_t* color)
{
	int len;
	tchar_t* token;
	tchar_t val[10];

	pxc->r = pxc->g = pxc->b = 0;
	if (_tstrnull(color))
		return;

	if (_tstrncmp(color, _T("RGB("), 4) == 0)
		token = (tchar_t*)color + 4;
	else
		token = (tchar_t*)color;

	len = 0;
	while (*token != _T(' ') && *token != _T('.') && *token != _T(',') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	len = (len < 3) ? len : 3;
	if (len)
	{
		_tstrncpy(val, token - len, len);
		val[len] = _T('\0');
		pxc->r = (unsigned char)_tstrtol(val);
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
		_tstrncpy(val, token - len, len);
		val[len] = _T('\0');
		pxc->g = (unsigned char)_tstrtol(val);
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
		_tstrncpy(val, token - len, len);
		val[len] = _T('\0');
		pxc->b = (unsigned char)_tstrtol(val);
	}
}

void format_xcolor(const xcolor_t* pxc, tchar_t* buf)
{
	_tsprintf(buf, _T("RGB(%d,%d,%d)"), pxc->r, pxc->g, pxc->b);
}

bool_t is_null_xpen(const xpen_t* pxp)
{
	if (!pxp)
		return 1;

	return (_tstrnull(pxp->color)) ? 1 : 0;
}

bool_t is_null_xbrush(const xbrush_t* pxb)
{
	if (!pxb)
		return 1;

	return (_tstrnull(pxb->color)) ? 1 : 0;
}

bool_t is_null_xfont(const xfont_t* pxf)
{
	if (!pxf)
		return 1;

	return (_tstrnull(pxf->size)) ? 1 : 0;
}

bool_t is_null_xface(const xface_t* pxa)
{
	if (!pxa)
		return 1;

	return (_tstrnull(pxa->line_align) && _tstrnull(pxa->text_align) && _tstrnull(pxa->text_wrap)) ? 1 : 0;
}

void default_xpen(xpen_t* pxp)
{
	memset((void*)pxp, 0, sizeof(xpen_t));

	_tstrcpy(pxp->style, GDI_ATTR_STROKE_STYLE_SOLID);
	_tstrcpy(pxp->color, GDI_ATTR_RGB_LIGHTGRAY);
	_tstrcpy(pxp->size, _T("1"));
	_tstrcpy(pxp->opacity, GDI_ATTR_OPACITY_SOFT);
}

void default_xbrush(xbrush_t* pxb)
{
	memset((void*)pxb, 0, sizeof(xbrush_t));

	_tstrcpy(pxb->style, GDI_ATTR_FILL_STYLE_SOLID);
	_tstrcpy(pxb->color, GDI_ATTR_RGB_WHITE);
	_tstrcpy(pxb->opacity, GDI_ATTR_OPACITY_SOFT);
}

void default_xfont(xfont_t* pxf)
{
	memset((void*)pxf, 0, sizeof(xfont_t));

	_tstrcpy(pxf->style, GDI_ATTR_FONT_STYLE_NORMAL);
	_tstrcpy(pxf->size, GDI_ATTR_FONT_SIZE_SYSTEM);
	_tstrcpy(pxf->weight, GDI_ATTR_FONT_WEIGHT_NORMAL);
	_tstrcpy(pxf->color, GDI_ATTR_RGB_DARKBLACK);
	_tstrcpy(pxf->family, _T(""));
}

void default_xface(xface_t* pxa)
{
	memset((void*)pxa, 0, sizeof(xface_t));

	_tstrcpy(pxa->text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
	_tstrcpy(pxa->line_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	_tstrcpy(pxa->line_height, DEF_GDI_TEXT_LINE_HEIGHT);
	_tstrcpy(pxa->text_wrap, _T(""));
}

void default_xgradi(xgradi_t* pxg)
{
	memset((void*)pxg, 0, sizeof(xgradi_t));

	_tstrcpy(pxg->type, GDI_ATTR_GRADIENT_TYPE_VERT);
	_tstrcpy(pxg->opacity, _T("250"));
	_tstrcpy(pxg->brim_color, GDI_ATTR_RGB_SOFTWHITE);
	_tstrcpy(pxg->core_color, GDI_ATTR_RGB_DARKWHITE);
}

void merge_xpen(xpen_t* pxp_dst, const xpen_t* pxp_src)
{
	if (_tstrnull(pxp_dst->style))
		_tstrcpy(pxp_dst->style, pxp_src->style);
	if (_tstrnull(pxp_dst->color))
		_tstrcpy(pxp_dst->color, pxp_src->color);
	if (_tstrnull(pxp_dst->size))
		_tstrcpy(pxp_dst->size, pxp_src->size);
	if (_tstrnull(pxp_dst->opacity))
		_tstrcpy(pxp_dst->opacity, pxp_src->opacity);
}

void merge_xbrush(xbrush_t* pxb_dst, const xbrush_t* pxb_src)
{
	if (_tstrnull(pxb_dst->style))
		_tstrcpy(pxb_dst->style, pxb_src->style);
	if (_tstrnull(pxb_dst->color))
		_tstrcpy(pxb_dst->color, pxb_src->color);
	if (_tstrnull(pxb_dst->opacity))
		_tstrcpy(pxb_dst->opacity, pxb_src->opacity);
}

void merge_xfont(xfont_t* pxf_dst, const xfont_t* pxf_src)
{
	if (_tstrnull(pxf_dst->style))
		_tstrcpy(pxf_dst->style, pxf_src->style);
	if (_tstrnull(pxf_dst->size))
		_tstrcpy(pxf_dst->size, pxf_src->size);
	if (_tstrnull(pxf_dst->weight))
		_tstrcpy(pxf_dst->weight, pxf_src->weight);
	if (_tstrnull(pxf_dst->family))
		_tstrcpy(pxf_dst->family, pxf_src->family);
	if (_tstrnull(pxf_dst->color))
		_tstrcpy(pxf_dst->color, pxf_src->color);
}

void merge_xface(xface_t* pxa_dst, const xface_t* pxa_src)
{
	if (_tstrnull(pxa_dst->text_align))
		_tstrcpy(pxa_dst->text_align, pxa_src->text_align);
	if (_tstrnull(pxa_dst->line_align))
		_tstrcpy(pxa_dst->line_align, pxa_src->line_align);
	if (_tstrnull(pxa_dst->line_height))
		_tstrcpy(pxa_dst->line_height, pxa_src->line_height);
	if (_tstrnull(pxa_dst->text_wrap))
		_tstrcpy(pxa_dst->text_wrap, pxa_src->text_wrap);
}

void lighten_xpen(xpen_t* pxp, int n)
{
	xcolor_t xc = { 0 };

	if (_tstrnull(pxp->color))
		return;

	parse_xcolor(&xc, pxp->color);

	lighten_xcolor(&xc, n);

	format_xcolor(&xc, pxp->color);
}

void lighten_xbrush(xbrush_t* pxb, int n)
{
	xcolor_t xc = { 0 };

	if (_tstrnull(pxb->color))
		return;

	parse_xcolor(&xc, pxb->color);

	lighten_xcolor(&xc, n);

	format_xcolor(&xc, pxb->color);
}

void lighten_xfont(xfont_t* pxf, int n)
{
	xcolor_t xc = { 0 };

	if (_tstrnull(pxf->color))
		return;

	parse_xcolor(&xc, pxf->color);

	lighten_xcolor(&xc, n);

	format_xcolor(&xc, pxf->color);
}

void lighten_xgradi(xgradi_t* pxg, int n)
{
	xcolor_t xc = { 0 };

	parse_xcolor(&xc, pxg->core_color);
	lighten_xcolor(&xc, n);
	format_xcolor(&xc, pxg->core_color);
}

#define IS_LETTER(ch)	((ch >= _T('A') && ch <= _T('Z')) || (ch >= _T('a') && ch <= _T('z')))
#define IS_NUMERIC(ch)	((ch >= _T('1') && ch <= _T('9')) || (ch == _T(' ') || ch == _T(',')))

tchar_t* next_draw_path(const tchar_t* script, int len, tchar_t* pname, xpoint_t* ppt, int* pn)
{
	tchar_t* token = (tchar_t*)script;
	int total = 0;
	tchar_t ch;

	if (_tstrnull(script))
		return NULL;

	if (len < 0)
		len = _tstrlen(script);

	while (!IS_LETTER(*token) && *token && total < len)
	{
		token++;
		total++;
	}

	if (*token == _T('\0') || total == len)
		return NULL;

	ch = *token;

	token++;
	total++;

	switch (ch)
	{
	case _T('M'):
	case _T('m'):
		_tsscanf(token, _T("%d %d"), &(ppt[0].x), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('L'):
	case _T('l'):
		_tsscanf(token, _T("%d %d"), &(ppt[0].x), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('H'):
	case _T('h'):
		_tsscanf(token, _T("%d"), &(ppt[0].x));
		ppt[0].y = 0;

		*pname = ch;
		*pn = 1;
		break;
	case _T('V'):
	case _T('v'):
		ppt[0].x = 0;
		_tsscanf(token, _T("%d"), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('Q'):
	case _T('q'):
		_tsscanf(token, _T("%d %d, %d %d"), &(ppt[0].x), &(ppt[0].y), &(ppt[1].x), &(ppt[1].y));

		*pname = ch;
		*pn = 2;
		break;
	case _T('T'):
	case _T('t'):
		_tsscanf(token, _T("%d %d"), &(ppt[0].x), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('C'):
	case _T('c'):
		_tsscanf(token, _T("%d %d, %d %d, %d %d"), &(ppt[0].x), &(ppt[0].y), &(ppt[1].x), &(ppt[1].y), &(ppt[2].x), &(ppt[2].y));

		*pname = ch;
		*pn = 3;
		break;
	case _T('S'):
	case _T('s'):
		_tsscanf(token, _T("%d %d, %d %d"), &(ppt[0].x), &(ppt[0].y), &(ppt[1].x), &(ppt[1].y));

		*pname = ch;
		*pn = 2;
		break;
	case _T('Z'):
	case _T('z'):
		*pname = ch;
		*pn = 0;
		break;
	}

	while (IS_NUMERIC(*token) && total < len)
	{
		token++;
		total++;
	}

	return token;
}