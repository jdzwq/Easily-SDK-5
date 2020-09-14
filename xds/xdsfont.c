/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc font document

	@module	xdsfont.c | implement file

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

#include "xdsfont.h"
#include "xdsattr.h"
#include "xdsstr.h"

typedef struct _font_map_t{
	tchar_t cn_font[16];
	float pt_font;
	float mm_font;
	float px_font;
}font_map_t;

static font_map_t font_map[] = {
	{ _T("一英寸"), 72.0f ,25.30f, 95.6f },
	{ _T("特大号"), 63.0f, 22.14f, 83.7f },
	{ _T("特号"), 54.0f, 18.97f, 71.7f },
	{ _T("初号"), 42.0f, 14.82f, 56.0f },
	{ _T("小初"), 36.0f, 12.70f, 48.0f },
	{ _T("一号"), 26.0f, 9.17f, 34.7f },
	{ _T("小一"), 24.0f, 8.47f, 32.0f },
	{ _T("小二"), 18.0f, 6.35f, 24.0f },
	{ _T("三号"), 16.0f, 5.64f, 21.3f },
	{ _T("小三"), 15.0f, 5.29f, 20.0f },
	{ _T("四号"), 14.0f, 4.94f, 18.7f },
	{ _T("小四"), 12.0f, 4.23f, 16.0f },
	{ _T("五号"), 10.5f, 3.70f, 14.0f },
	{ _T("小五"), 9.0f, 3.18f, 12.0f },
	{ _T("六号"), 7.5f, 2.56f, 10.0f },
	{ _T("小六"), 6.5f, 2.29f, 8.7f },
	{ _T("七号"), 5.5f, 1.94f, 7.3f },
	{ _T("八号"), 5.0f, 1.76f, 6.7f }
};

void font_metric_by_pt(float pt, float* pm, float* px)
{
	int i, n;
	
	n = sizeof(font_map) / sizeof(font_map_t);

	for (i = 0; i < n; i++)
	{
		if (pm) *pm = font_map[i].mm_font;
		
		if (px) *px = font_map[i].px_font;

		if (pt >= font_map[i].pt_font)
			break;
	}
}

void font_metric_by_px(float px, float* pt, float* pm)
{
	int i, n;

	n = sizeof(font_map) / sizeof(font_map_t);

	for (i = 0; i < n; i++)
	{
		if (pm) *pm = font_map[i].mm_font;

		if (pt) *pt = font_map[i].pt_font;

		if ((int)(px + 0.5) >= (int)(font_map[i].pt_font + 0.5))
			break;
	}
}

static tchar_t *x11_font_name[] = {_T("*")};
static tchar_t *x11_font_weight[] = {_T("regular"), _T("medium"), _T("bold")};
static tchar_t *x11_font_style[]  = {_T("r"), _T("i"), _T("o")};
static tchar_t x11_pattern[] = {_T("-*-%s-%s-%s-*--%d-*-*-*-*-*-*")};

 
int format_font_pattern(const xfont_t* pxf, tchar_t* buf)
{
    const tchar_t* fs_name = NULL;
    const tchar_t* fs_style = NULL;
    const tchar_t* fs_weight = NULL;
    int fs_size = 10;
    
    if(is_null((pxf->family)))
        fs_name = x11_font_name[0];
    else
        fs_name = pxf->family;
       
    if(xstol(pxf->weight) < 400)
        fs_weight = x11_font_weight[0];
    else if(xstol(pxf->weight) < 700)
        fs_weight = x11_font_weight[1];
    else
        fs_weight = x11_font_weight[2];
    
    if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
        fs_style = x11_font_style[1];
    else if(xscmp(pxf->style,GDI_ATTR_FONT_STYLE_OBLIQUE) == 0)
        fs_style = x11_font_style[2];
    else
        fs_style = x11_font_style[0];
    
     if(is_null((pxf->size)))
         fs_size = 10;
    else
        fs_size = xstol(pxf->size);
    
    return xsprintf(buf, x11_pattern, fs_name, fs_weight, fs_style, fs_size);
}