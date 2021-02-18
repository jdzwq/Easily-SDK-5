/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc proper bio document

	@module	properbio.c | implement file

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

#include "properbio.h"
#include "fileinf.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdldoc.h"

#include "xdlinit.h"

#if defined(XDL_SUPPORT_BIO)

tchar_t* _parse_ini_line(tchar_t* str, tchar_t** pkey, int* pkeylen, tchar_t** pval, int* pvallen, int* prt)
{
	tchar_t* token = str;
	int sign = 0;

	*pkey = *pval = NULL;
	*pkeylen = *pvallen = 0;
	while ((*token == _T('\r') || *token == _T('\n') || *token == _T(' ') || *token == _T('\t')) && *token != _T('\0'))
		token++;
	if (*token == _T('\0'))
		return NULL;

	if (*token == _T(';'))
		*prt = 1;
	else if (*token == _T('['))
	{
		*prt = 2;
		token++;
		*pkey = token;
	}
	else
	{
		*prt = 3;
		*pkey = token;
	}

	while (*token != _T('\r') && *token != _T('\n') && *token != _T('\0'))
	{
		if (*token == _T('='))
		{
			sign = 1;
			token++;
			continue;
		}

		if (*prt == 2)
		{
			if (*token == _T(']'))
			{
				token++;
				break;
			}
			else
				*pkeylen = *pkeylen + 1;
		}

		else if (*prt == 3)
		{
			if (sign == 0)
				*pkeylen = *pkeylen + 1;
			else
			{
				if (*pval == NULL)
					*pval = token;
				*pvallen = *pvallen + 1;
			}
		}

		token++;
	}

	return token;
}

void parse_proper_from_ini(link_t_ptr ptr, const tchar_t* str, int len)
{
	tchar_t *val, *key, *sec, *tmp, *buf;
	int vallen, keylen, seclen;
	int rt;

	buf = (tchar_t*)str;
	sec = NULL;
	seclen = 0;
	while (buf = _parse_ini_line(buf, &key, &keylen, &val, &vallen, &rt))
	{
		if (rt == 2 && keylen)
		{
			sec = key;
			seclen = keylen;
			tmp = sec + keylen;
			while (*tmp == _T(' ') || *tmp == _T('\t'))
			{
				if (tmp == key)
					break;
				seclen--;
				tmp--;
			}
		}
		else if (rt == 3 && keylen)
		{
			tmp = key + keylen;
			while (*tmp == _T(' ') || *tmp == _T('\t'))
			{
				if (tmp == key)
					break;
				keylen--;
				tmp--;
			}

			if (val && vallen)
			{
				tmp = val + vallen;
				while (*tmp == _T(' ') || *tmp == _T('\t'))
				{
					if (tmp == val)
						break;
					vallen--;
					tmp--;
				}
			}

			write_proper(ptr, sec, seclen, key, keylen, val, vallen);
		}
	}
}

int format_proper_to_ini(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr slk, elk;
	int total;
	const tchar_t *sec, *ent, *val;
	int sec_len, ent_len, val_len;

	total = 0;

	sec = NULL;
	slk = get_next_section(ptr, LINK_FIRST);
	while (slk)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('[');
		}
		total += 1;

		//[sec]\r\n
		sec = get_dom_node_attr_ptr(slk, ATTR_NAME, -1);
		sec_len = xslen(sec);

		if (total + sec_len > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, sec, sec_len);
		}
		total += sec_len;

		if (total + 3 > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, _T("]\r\n"), 3);
		}
		total += 3;

		ent = val = NULL;
		elk = get_next_entity(slk, LINK_FIRST);
		while (elk)
		{
			//ent=val\r\n
			ent = get_entity_name_ptr(elk);
			ent_len = xslen(ent);

			if (total + ent_len > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, ent, ent_len);
			}
			total += ent_len;

			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = _T('=');
			}
			total += 1;

			val = get_entity_value_ptr(elk);
			val_len = xslen(val);

			if (total + val_len > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, val, val_len);
			}
			total += val_len;

			if (total + 2 > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, _T("\r\n"), 2);
			}
			total += 2;

			elk = get_next_entity(slk, elk);
		}

		slk = get_next_section(ptr, slk);
	}

	return total;
}

bool_t load_proper_from_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	dword_t size;
	int len;
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	int encode, skip;

	file_t xf = NULL;
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

	xf = xfile_open(psd, fname, FILE_OPEN_READ);
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

	parse_proper_from_ini(ptr, str, len);

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

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return 0;
}

bool_t save_proper_to_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	int len;
	dword_t size;
	int skip;

	file_t xf = NULL;
	byte_t* buf = NULL;
	tchar_t* str = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	len = format_proper_to_ini(ptr, NULL, MAX_LONG);

	str = xsalloc(len + 1);
	format_proper_to_ini(ptr, str, len);

	skip = format_utfbom(DEF_MBS, NULL);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, NULL, MAX_LONG);
#else
	size = mbs_to_mbs(str, len, NULL, MAX_LONG);
#endif

	buf = (byte_t*)xmem_alloc(skip + size);

	format_utfbom(_UTF8, buf);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#else
	size = mbs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#endif

	xsfree(str);
	str = NULL;

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

	if (str)
		xsfree(str);

	return 0;
}


#endif //XDLDOC