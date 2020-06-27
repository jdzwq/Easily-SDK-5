/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memo bio document

	@module	memobio.c | implement file

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

#include "memobio.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDL_SUPPORT_BIO)

int format_memo_doc(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr ilk;
	int ind, len, total = 0;

	ilk = get_memo_next_line(ptr, LINK_FIRST);
	while (ilk)
	{
		len = get_memo_line_indent(ilk);
		if (len + total > max)
			return total;

		if (buf)
		{
			ind = 0;
			while (ind < len)
			{
				buf[total + ind] = _T('\t');
				ind++;
			}
		}
		total += len;

		len = get_memo_line_text(ilk, NULL, MAX_LONG);
		if (len + total > max)
			return total;

		if (buf)
		{
			get_memo_line_text(ilk, buf + total, len);
		}
		total += len;

		len = 1;
		if (len + total > max)
			return total;

		if (buf)
		{
			buf[total] = _T('\n');
		}
		total += len;

		ilk = get_memo_next_line(ptr, ilk);
	}

	return total;
}

bool_t parse_memo_doc(link_t_ptr ptr, const tchar_t* buf, int len)
{
	link_t_ptr ilk;
	const tchar_t *tk_inde, *tk_line;
	int len_inde, len_line, total = 0;

	if (len < 0)
		len = xslen(buf);

	clear_memo_doc(ptr);

	while (total < len)
	{
		tk_inde = buf + total;
		len_inde = 0;
		while (*(tk_inde + len_inde) == _T('\t'))
		{
			len_inde++;
		}
		total += len_inde;

		tk_line = buf + total;
		len_line = 0;
		while (*(tk_line + len_line) != _T('\r') && *(tk_line + len_line) != _T('\n') && *(tk_line + len_line) != _T('\0') && total < len)
		{
			len_line++;
			total++;
		}

		ilk = insert_memo_line(ptr, LINK_LAST);
		if (len_inde)
		{
			set_memo_line_indent(ilk, len_inde);
		}
		set_memo_line_text(ilk, tk_line, len_line);

		if (*(tk_line + len_line) == _T('\r'))
		{
			len_line++;
			total++;
		}

		if (*(tk_line + len_line) == _T('\n'))
		{
			len_line++;
			total++;
		}

		if (*(buf + total) == _T('\0'))
			break;
	}

	return 1;
}

bool_t format_memo_doc_to_stream(link_t_ptr ptr, stream_t stm)
{
	link_t_ptr ilk;
	tchar_t ba[2] = { 0 };
	tchar_t* buf;
	int ind, len;

	stream_write_utfbom(stm, NULL);

	ilk = get_memo_next_line(ptr, LINK_FIRST);
	while (ilk)
	{
		len = get_memo_line_indent(ilk);
		ind = 0;
		while (ind < len)
		{
			ba[0] = _T('\t');
			stream_write_char(stm, ba, NULL);
			
			ind++;
		}

		len = get_memo_line_text(ilk, NULL, MAX_LONG);
		buf = xsalloc(len + 1);
		get_memo_line_text(ilk, buf, len);

		stream_write(stm, buf, len, NULL);

		xmem_free(buf);
		buf = NULL;

		ba[0] = _T('\n');
		stream_write_char(stm, ba, NULL);

		ilk = get_memo_next_line(ptr, ilk);
	}

	stream_write_line(stm, NULL, NULL);

	return (1);
}

bool_t parse_memo_doc_from_stream(link_t_ptr ptr, stream_t stm)
{
	link_t_ptr ilk;
	string_t vs;
	dword_t dw;
	bool_t rt = 0;
	tchar_t* pch;
	int len_inde, len_line;

	clear_memo_doc(ptr);

	stream_read_utfbom(stm, NULL);

	vs = string_alloc();

	while (1)
	{
		string_empty(vs);
		dw = 0;
		if (!(rt = stream_read_line(stm, vs, &dw)))
		{
			break;
		}

		if (string_len(vs) == 0)
			break;

		len_line = string_len(vs);
		pch = string_ptr(vs);

		len_inde = 0;
		while (*(pch + len_inde) == _T('\t'))
		{
			len_inde++;
		}
		
		len_line = 0;
		while (*(pch + len_line) != _T('\r') && *(pch + len_line) != _T('\n') && *(pch + len_line) != _T('\0'))
		{
			len_line++;
		}

		ilk = insert_memo_line(ptr, LINK_LAST);
		if (len_inde)
		{
			set_memo_line_indent(ilk, len_inde);
		}
		set_memo_line_text(ilk, (pch + len_inde), len_line);
	}

	return (1);
}

#endif //XDLDOC
