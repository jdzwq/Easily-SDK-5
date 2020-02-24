/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memo operator document

	@module	memoopera.c | implement file

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

#include "memoopera.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC


bool_t call_memo_can_escape(void* p_obj)
{
	return 0;
}

bool_t call_memo_with_eof(void* p_obj)
{
	return 1;
}

int call_memo_read_char(void* p_obj, int max, int pos, int encode, tchar_t* pch)
{
	MEMOOBJECT* ptt = (MEMOOBJECT*)p_obj;
	const tchar_t* sz_memo;
	int chs;

	if (ptt->eof)
		return 0;

	if (!ptt->nlk)
	{
		ptt->nlk = get_memo_next_line(ptt->txt, LINK_FIRST);
		if (!ptt->nlk)
		{
			ptt->eof = 1;
			return 0;
		}

		sz_memo = get_memo_line_text_ptr(ptt->nlk);
		ptt->len = xslen(sz_memo);
		ptt->pos = 0;
	}
	else
	{
		sz_memo = get_memo_line_text_ptr(ptt->nlk);
	}

	while (ptt->pos == ptt->len)
	{
		ptt->nlk = get_memo_next_line(ptt->txt, ptt->nlk);
		if (!ptt->nlk)
		{
			ptt->eof = 1;
			return 0;
		}

		sz_memo = get_memo_line_text_ptr(ptt->nlk);
		ptt->len = xslen(sz_memo);
		ptt->pos = 0;
	}

#ifdef _UNICODE
	chs = ucs_sequence(*(sz_memo + ptt->pos));
	xsncpy(pch, sz_memo + ptt->pos, chs);
#else
	chs = mbs_sequence(*(sz_memo + ptt->pos));
	xsncpy(pch, sz_memo + ptt->pos, chs);
#endif

	ptt->pos += chs;

	return chs;
}

int call_memo_read_token(void* p_obj, int max, int pos, int encode, tchar_t* pch, int len)
{
	int i,j = 0;

	while (i = call_memo_read_char(p_obj, max, pos, encode, pch + j))
	{
		pos += i;
#ifdef _UNICODE
		j += ucs_sequence(*(pch + j));
#else
		j += mbs_sequence(*(pch + j));
#endif
	}

	return j;
}

int call_memo_write_char(void* p_obj, int max, int pos, int encode, const tchar_t* pch)
{
	MEMOOBJECT* ptt = (MEMOOBJECT*)p_obj;
	int chs;

	if (!ptt->nlk || *pch == _T('\n'))
	{
		ptt->nlk = insert_memo_line(ptt->txt, LINK_LAST);
		ptt->len = ptt->pos = 0;
	}

	ptt->eof = 0;

	if (*pch == _T('\n'))
	{
		return 1;
	}

	if (!ptt->pos && *pch == _T('\t'))
	{
		set_memo_line_indent(ptt->nlk, get_memo_line_indent(ptt->nlk) + 1);
		return 1;
	}

#ifdef _UNICODE
	chs = ucs_sequence(*(pch));
#else
	chs = mbs_sequence(*(pch));
#endif

	memo_line_text_ins_chars(ptt->nlk, ptt->pos, pch, chs);
	ptt->len += chs;
	ptt->pos += chs;

	return chs;
}

int call_memo_write_indent(void* p_obj, int max, int pos, int encode)
{
	tchar_t pch[2] = { 0 };

	pch[0] = _T('\t');
	return call_memo_write_char(p_obj, max, pos, encode, pch);
}

int call_memo_write_carriage(void* p_obj, int max, int pos, int encode)
{
	tchar_t pch[2] = { 0 };

	pch[0] = _T('\n');
	return call_memo_write_char(p_obj, max, pos, encode, pch);
}

int call_memo_write_token(void* p_obj, int max, int pos, int encode, const tchar_t* pch, int len)
{
	int i, j = 0;

	if (len < 0)
		len = xslen(pch);

	for (j = 0; j < len;)
	{
		i = call_memo_write_char(p_obj, max, pos, encode, pch + j);
		pos += i;
#ifdef _UNICODE
		j += ucs_sequence(*(pch + j));
#else
		j += mbs_sequence(*(pch + j));
#endif
	}

	return j;
}

#endif //XDLDOC
