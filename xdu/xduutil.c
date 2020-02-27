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


#define IS_LETTER(ch)	((ch >= _T('A') && ch <= _T('Z')) || (ch >= _T('a') && ch <= _T('z')))
#define IS_NUMERIC(ch)	((ch >= _T('1') && ch <= _T('9')) || (ch == _T(' ') || ch == _T(',')))

tchar_t* next_draw_path(const tchar_t* script, int len, tchar_t* pname, xpoint_t* ppt, int* pn)
{
	tchar_t* token = (tchar_t*)script;
	int total = 0;
	tchar_t ch;

	if (is_null(script))
		return NULL;

	if (len < 0)
		len = xslen(script);

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
		xsscanf(token, _T("%d %d"), &(ppt[0].x), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('L'):
	case _T('l'):
		xsscanf(token, _T("%d %d"), &(ppt[0].x), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('H'):
	case _T('h'):
		xsscanf(token, _T("%d"), &(ppt[0].x));
		ppt[0].y = 0;

		*pname = ch;
		*pn = 1;
		break;
	case _T('V'):
	case _T('v'):
		ppt[0].x = 0;
		xsscanf(token, _T("%d"), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('Q'):
	case _T('q'):
		xsscanf(token, _T("%d %d, %d %d"), &(ppt[0].x), &(ppt[0].y), &(ppt[1].x), &(ppt[1].y));

		*pname = ch;
		*pn = 2;
		break;
	case _T('T'):
	case _T('t'):
		xsscanf(token, _T("%d %d"), &(ppt[0].x), &(ppt[0].y));

		*pname = ch;
		*pn = 1;
		break;
	case _T('C'):
	case _T('c'):
		xsscanf(token, _T("%d %d, %d %d, %d %d"), &(ppt[0].x), &(ppt[0].y), &(ppt[1].x), &(ppt[1].y), &(ppt[2].x), &(ppt[2].y));

		*pname = ch;
		*pn = 3;
		break;
	case _T('S'):
	case _T('s'):
		xsscanf(token, _T("%d %d, %d %d"), &(ppt[0].x), &(ppt[0].y), &(ppt[1].x), &(ppt[1].y));

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