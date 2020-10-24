/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc text bio document

	@module	textbio.c | implement file

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

#include "textbio.h"
#include "memobio.h"
#include "jsonbio.h"
#include "fileinf.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdldoc.h"
#include "xdlinit.h"

#if defined(XDL_SUPPORT_BIO)

int save_text_to_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* buf, int len)
{
	if_fio_t* xf = NULL;
	byte_t* data = NULL;
	dword_t size;
	int skip;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);

	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	skip = format_utfbom(DEF_MBS, NULL);

#ifdef _UNICODE
	size = ucs_to_mbs(buf, len, NULL, MAX_LONG);
#else
	size = mbs_to_mbs(buf, len, NULL, MAX_LONG);
#endif

	data = (byte_t*)xmem_alloc(skip + size);

	format_utfbom(DEF_MBS, data);

#ifdef _UNICODE
	size = ucs_to_mbs(buf, len, (schar_t*)(data + skip), size);
#else
	size = mbs_to_mbs(buf, len, (schar_t*)(data + skip), size);
#endif

	size += skip;

	if (!xfile_write(xf, data, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	xmem_free(data);
	data = NULL;

	END_CATCH;

	return len;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (data)
		xmem_free(data);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return C_ERR;
}

int load_text_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* buf, int max)
{
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	dword_t dw, size;
	int skip, encode;

	if_fio_t* xf = NULL;
	byte_t* data = NULL;

	TRY_CATCH;

	if (!xfile_info(psd, fname, NULL, fsize, NULL, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize))
	{
		raise_user_error(_T("0"), _T("not support huge size file"));
	}

	xf = xfile_open(psd, fname, FILE_OPEN_READ);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	size = xstol(fsize);
	data = (byte_t*)xmem_alloc(size);

	if (!xfile_read(xf, data, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	encode = parse_utfbom(data, size);
	skip = skip_utfbom(data);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		dw = utf8_to_ucs(data + skip, size - skip, buf, max);
#else
		dw = utf8_to_mbs(data + skip, size - skip, buf, max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		dw = utf16lit_to_ucs(data + skip, size - skip, buf, max);
#else
		dw = utf16lit_to_mbs(data + skip, size - skip, buf, max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		dw = utf16big_to_ucs(data + skip, size - skip, buf, max);
#else
		dw = utf16big_to_mbs(data + skip, size - skip, buf, max);
#endif
		break;
	default:
#ifdef _UNICODE
		dw = mbs_to_ucs((schar_t*)(data + skip), size - skip, buf, max);
#else
		dw = mbs_to_mbs((schar_t*)(data + skip), size - skip, buf, max);
#endif
		break;
	}

	xmem_free(data);
	data = NULL;

	END_CATCH;

	return (int)dw;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (data)
		xmem_free(data);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return C_ERR;
}

bool_t load_memo_from_text_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = load_text_from_file(psd, fname, NULL, MAX_LONG);
	if (len == C_ERR)
		return 0;

	buf = xsalloc(len + 1);

	load_text_from_file(psd, fname, buf, len);

	rt = parse_memo_doc(ptr, buf, len);

	xsfree(buf);

	return rt;
}

bool_t save_memo_to_text_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = format_memo_doc(ptr, NULL, MAX_LONG);

	buf = xsalloc(len + 1);

	format_memo_doc(ptr, buf, len);

	rt = save_text_to_file(psd, fname, buf, len);

	xsfree(buf);

	return rt;
}

bool_t load_tag_from_text_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = load_text_from_file(psd, fname, NULL, MAX_LONG);
	if (len == C_ERR)
		return 0;

	buf = xsalloc(len + 1);

	load_text_from_file(psd, fname, buf, len);

	rt = parse_tag_doc(ptr, buf, len);

	xsfree(buf);

	return rt;
}

bool_t save_tag_to_text_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = format_tag_doc(ptr, NULL, MAX_LONG);

	buf = xsalloc(len + 1);

	format_tag_doc(ptr, buf, len);

	rt = save_text_to_file(psd, fname, buf, len);

	xsfree(buf);

	return rt;
}

bool_t load_json_from_text_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;
	string_t vs = NULL;

	len = load_text_from_file(psd, fname, NULL, MAX_LONG);
	if (len == C_ERR)
		return 0;

	vs = string_alloc();
	buf = string_ensure_buf(vs, len);

	load_text_from_file(psd, fname, buf, len);

	rt = parse_json_doc_from_string(ptr, vs);
	
	string_free(vs);

	return rt;
}

bool_t save_json_to_text_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	bool_t rt;
	string_t vs = NULL;

	vs = string_alloc();
	format_json_doc_to_string(ptr, vs);

	rt = save_text_to_file(psd, fname, string_ptr(vs), string_len(vs));

	string_free(vs);

	return rt;
}

#endif //XDLDOC