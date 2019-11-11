/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc grid bio document

	@module	gridbio.c | implement file

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

#include "gridbio.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlinit.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDL_SUPPORT_BIO)

int format_grid_colset_to_csv(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr clk;
	int len, size;
	const tchar_t* str;

	size = 0;
	if (size >= max)
		return size;

	clk = get_next_visible_col(ptr, LINK_FIRST);
	while (clk)
	{
		str = get_col_title_ptr(clk);
		len = xslen(str);
		if (size + len > max)
			return size;

		if (buf)
		{
			xsncpy(buf + size, str, len);
		}
		size += len;

		if (size + 1 > max)
			return size;

		if (buf)
		{
			buf[size] = TXT_ITEMFEED;
		}
		size += 1;

		clk = get_next_visible_col(ptr, clk);
	}

	if (buf)
	{
		if (size && buf[size - 1] == TXT_ITEMFEED)
			buf[size - 1] = TXT_LINEFEED;
	}

	if (size + 1 > max)
		return size;

	if (buf)
	{
		buf[size] = CSV_LINEFEED;
	}
	size += 1;

	return size;
}

int format_grid_rowset_to_csv(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr clk, rlk;
	int len, size;
	const tchar_t* str;

	size = 0;
	if (size >= max)
		return size;

	rlk = get_next_visible_row(ptr, LINK_FIRST);
	while (rlk)
	{
		clk = get_next_visible_col(ptr, LINK_FIRST);
		while (clk)
		{
			str = get_cell_text_ptr(rlk, clk);
			len = xslen(str);
			if (size + len > max)
				return size;

			if (buf)
			{
				get_cell_text(rlk, clk, buf + size, len);
			}
			size += len;

			if (size + 1 > max)
				return size;

			if (buf)
			{
				buf[size] = TXT_ITEMFEED;
			}
			size += 1;

			clk = get_next_visible_col(ptr, clk);
		}

		if (buf)
		{
			if (size && buf[size - 1] == TXT_ITEMFEED)
				buf[size - 1] = TXT_LINEFEED;
		}

		if (size + 1 > max)
			return size;

		if (buf)
		{
			buf[size] = CSV_LINEFEED;
		}
		size += 1;

		rlk = get_next_visible_row(ptr, rlk);
	}

	return size;
}

int parse_grid_colset_from_csv(link_t_ptr ptr, const tchar_t* flat, int len)
{
	link_t_ptr clk;
	tchar_t* token;
	tchar_t* prev;
	tchar_t* colname;
	int total = 0;

	if (len < 0)
		len = xslen(flat);

	clear_grid_rowset(ptr);
	clear_grid_colset(ptr);

	token = (tchar_t*)flat;
	while (total < len)
	{
		prev = token;
		while (*token != CSV_ITEMFEED && *token != CSV_LINEFEED && *token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0') && total < len)
		{
			token++;
			total++;
		}

		clk = insert_col(ptr, LINK_LAST);

		colname = xsnclone(prev, (int)(token - prev));
		set_col_name(clk, colname);
		set_col_title(clk, colname);
		xsfree(colname);

		if (*token == CSV_LINEFEED || *token == TXT_LINEFEED || *token == _T('\0') || total >= len)
			break;

		token++;
		total++;
	}

	while ((*token == CSV_LINEFEED || *token == TXT_LINEFEED) && total < len)
	{
		token++;
		total++;
	}

	return total;
}

int parse_grid_rowset_from_csv(link_t_ptr ptr, const tchar_t* flat, int len)
{
	link_t_ptr clk, rlk;
	tchar_t* token;
	tchar_t* prev;
	int total = 0;

	if (len <= 0)
		return 0;

	clear_grid_rowset(ptr);

	token = (tchar_t*)flat;
	while (total < len)
	{
		rlk = insert_row(ptr, LINK_LAST);

		clk = get_next_visible_col(ptr, LINK_FIRST);
		while (clk)
		{
			prev = token;
			while (*token != CSV_ITEMFEED && *token != CSV_LINEFEED && *token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0') && total < len)
			{
				token++;
				total++;
			}

			set_cell_text(rlk, clk, prev, (int)(token - prev));

			if (*token == CSV_LINEFEED || *token == TXT_LINEFEED || *token == _T('\0') || total >= len)
				break;

			token++;
			total++;

			clk = get_next_visible_col(ptr, clk);
		}

		//skip linefeed
		while (*token != CSV_LINEFEED && *token != TXT_LINEFEED && *token != _T('\0') && total < len)
		{
			token++;
			total++;
		}

		while ((*token == CSV_LINEFEED || *token == TXT_LINEFEED) && total < len)
		{
			token++;
			total++;
		}
	}

	return total;
}

bool_t save_grid_to_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname)
{
	int skip, len;
	tchar_t* str;
	dword_t size;

	file_t xf = NULL;
	byte_t* buf = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	if (head)
		len = format_grid_colset_to_csv(ptr, NULL, MAX_LONG) + format_grid_rowset_to_csv(ptr, NULL, MAX_LONG);
	else
		len = format_grid_rowset_to_csv(ptr, NULL, MAX_LONG);

	str = xsalloc(len + 1);

	if (head)
	{
		skip = format_grid_rowset_to_csv(ptr, str, len);
	}
	else
	{
		skip = 0;
	}

	format_grid_rowset_to_csv(ptr, str + skip, len - skip);

	skip = format_utfbom(DEF_MBS, NULL);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, NULL, MAX_LONG);
#else
	size = mbs_to_mbs(str, len, NULL, MAX_LONG);
#endif

	buf = (byte_t*)xmem_alloc(skip + size);

	format_utfbom(DEF_MBS, buf);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#else
	size = mbs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#endif

	xsfree(str);

	size += skip;
	if (!xfile_write(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	xmem_free(buf);
	buf = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (buf)
		xmem_free(buf);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return 0;
}

bool_t load_grid_from_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname)
{
	dword_t size;
	int skip, len;
	int encode = 0;
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	xhand_t xf = NULL;
	byte_t* buf = NULL;
	tchar_t* str = NULL;

	TRY_CATCH;

	if (!xfile_info(psd, fname, NULL, fsize, NULL, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize))
	{
		raise_user_error(_T("0"), _T("not support large file"));
	}

	xf = xfile_open(psd, fname, 0);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	size = xstol(fsize);
	buf = (byte_t*)xmem_alloc(size + 1);

	if (!xfile_read(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	encode = parse_utfbom(buf, size);
	skip = skip_utfbom(buf);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		len = utf8_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf8_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf16lit_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		len = utf16big_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf16big_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	default:
#ifdef _UNICODE
		len = mbs_to_ucs((schar_t*)(buf + skip), size - skip, NULL, MAX_LONG);
#else
		len = mbs_to_mbs((schar_t*)(buf + skip), size - skip, NULL, MAX_LONG);
#endif
		break;
	}

	str = xsalloc(len + 1);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		len = utf8_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf8_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf16lit_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		len = utf16big_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf16big_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	default:
#ifdef _UNICODE
		len = mbs_to_ucs((schar_t*)(buf + skip), size - skip, str, len);
#else
		len = mbs_to_mbs((schar_t*)(buf + skip), size - skip, str, len);
#endif
		break;
	}

	xmem_free(buf);
	buf = NULL;

	if (head)
		skip = parse_grid_colset_from_csv(ptr, str, len);
	else
		skip = 0;

	parse_grid_rowset_from_csv(ptr, str + skip, len - skip);

	xsfree(str);
	str = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (buf)
		xmem_free(buf);

	if (str)
		xsfree(str);

	return 0;
}
#endif //XDLDOC