/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant string document

	@module	varstr.c | varstr implement file

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

#include "varstr.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#define VARSTR_INC	512

typedef struct _redstr_t{
	int size;
	int count;
	tchar_t* vbuf;
}redstr_t;

string_t varstr_alloc()
{
	redstr_t* pvs;

	pvs = (redstr_t*)xmem_alloc(sizeof(redstr_t));

	pvs->size = 0;
	pvs->vbuf = NULL;
	pvs->count = 0;

	return (string_t)pvs;
}

void varstr_free(string_t vs)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (pvs->vbuf)
		xmem_free(pvs->vbuf);

	xmem_free(pvs);
}

void varstr_incre(string_t vs,int len)
{
	redstr_t* pvs = (redstr_t*)vs;
	int org_size;

	org_size = pvs->size;
	while(len + pvs->count + 1 > org_size)
		org_size += VARSTR_INC;

	if (org_size > pvs->size)
	{
		pvs->size = org_size;
		pvs->vbuf = (tchar_t*)xmem_realloc(pvs->vbuf, pvs->size * sizeof(tchar_t));
	}
}

tchar_t* varstr_ensure_buf(string_t vs, int len)
{
	redstr_t* pvs = (redstr_t*)vs;

	pvs->count = 0;

	varstr_incre(pvs, len);

	pvs->count = len;

	return pvs->vbuf;
}

void varstr_attach_buf(string_t vs, tchar_t* buf, int size)
{
	redstr_t* pvs = (redstr_t*)vs;

	int len;

	if (pvs->vbuf)
		xmem_free(pvs->vbuf);

	len = xslen(buf);

	if (size < 0)
		size = (buf)? (len + 1) : 0;

	pvs->vbuf = buf;
	pvs->size = size;
	pvs->count = len;
}

tchar_t* varstr_detach_buf(string_t vs)
{
	redstr_t* pvs = (redstr_t*)vs;

	tchar_t* buf;

	buf = pvs->vbuf;

	pvs->vbuf = NULL;
	pvs->size = 0;
	pvs->count = 0;

	return buf;
}

int varstr_resize(string_t vs, int len)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (len < 0)
	{
		while (pvs->count)
		{
			if (pvs->vbuf[pvs->count - 1] == _T('\0'))
			{
				pvs->count--;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		if (pvs->count < len)
			varstr_incre(pvs, len - pvs->count);

		while (pvs->count < len)
		{
			pvs->vbuf[pvs->count] = _T(' ');
			pvs->count++;
		}

		if (pvs->vbuf)
		{
			pvs->vbuf[len] = _T('\0');
		}
		pvs->count = len;
	}

	return pvs->count;
}

int	varstr_cat(string_t vs, const tchar_t* str, int len)
{
	redstr_t* pvs = (redstr_t*)vs;

	int dlen;

	if (is_null(str))
		return pvs->count;

	dlen = (len < 0) ? xslen(str) : len;

	if (!dlen)
		return pvs->count;

	if (dlen + pvs->count + 1 > pvs->size)
	{
		varstr_incre(pvs, dlen);
	}

	if (pvs->vbuf)
	{
		xsncpy(pvs->vbuf + pvs->count, str, dlen);
	}

	pvs->count += dlen;

	return pvs->count;
}

int	varstr_cpy(string_t vs, const tchar_t* str, int len)
{
	redstr_t* pvs = (redstr_t*)vs;

	int dlen;

	if (len < 0)
		len = xslen(str);

	if (!len)
	{
		varstr_empty(pvs);
		return pvs->count;
	}

	dlen = (len < 0) ? xslen(str) : len;

	if (!dlen)
	{
		varstr_empty(pvs);
		return pvs->count;
	}

	if (dlen + 1 > pvs->size)
	{
		varstr_incre(pvs, dlen);
	}

	if (pvs->vbuf)
	{
		xsncpy(pvs->vbuf, str, dlen);
	}

	pvs->count = len;

	return pvs->count;
}

tchar_t varstr_get_char(string_t vs, int pos)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (pos < 0 || pos >= pvs->count)
		return 0;

	return pvs->vbuf[pos];
}

bool_t varstr_set_char(string_t vs, int pos, tchar_t ch)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (pos < 0 || pos >= pvs->count)
		return 0;

	pvs->vbuf[pos] = ch;

	return 1;
}

int varstr_get_chars(string_t vs, int pos, tchar_t* pch, int n)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (pch)
	{
		xszero(pch, n + 1);
	}

	if (pos < 0 || pos >= pvs->count)
		return 0;

	n = (n < pvs->count - pos) ? n : (pvs->count - pos);

	if (pch)
	{
		xsncpy(pch, pvs->vbuf + pos, n);
	}

	return n;
}

void varstr_set_chars(string_t vs,int pos,const tchar_t* pch,int n)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (pos < 0)
		pos = pvs->count;

	if (pos > pvs->count)
		return;

	if (n < 0)
		n = xslen(pch);

	if (pos + n + 1 > pvs->size)
		varstr_incre(pvs, pos + n - pvs->size);

	xsnset(pvs->vbuf, pos, pch, n);

	pvs->count = (pvs->count > pos + n) ? pvs->count : (pos + n);
}

void varstr_ins_chars(string_t vs,int pos,const tchar_t* pch,int n)
{
	redstr_t* pvs = (redstr_t*)vs;

	if(pos < 0)
		pos = pvs->count;

	if (pos > pvs->count)
		return;

	if (n < 0)
		n = xslen(pch);

	if(pvs->count + n + 1 > pvs->size)
		varstr_incre(pvs,n);

	if (n)
	{
		xsnins(pvs->vbuf, pos, pch, n);
		pvs->count += n;
	}
}

void varstr_del_chars(string_t vs,int pos,int n)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (pos < 0 || pos >= pvs->count)
		return;

	xsndel(pvs->vbuf, pos, n);
	
	pvs->count = (pvs->count > pos + n) ? (pvs->count - n) : pos;
}

const tchar_t* varstr_ptr(string_t vs)
{
	redstr_t* pvs = (redstr_t*)vs;

	return pvs->vbuf;
}

int varstr_len(string_t vs)
{
	redstr_t* pvs = (redstr_t*)vs;

	return pvs->count;
}

void varstr_empty(string_t vs)
{
	redstr_t* pvs = (redstr_t*)vs;

	if (pvs->size > VARSTR_INC)
	{
		xmem_free(pvs->vbuf);

		pvs->size = 0;
		pvs->vbuf = NULL;
		pvs->count = 0;
	}
	else
	{
		xmem_zero((void*)pvs->vbuf, pvs->size * sizeof(tchar_t));
		pvs->count = 0;
	}
}

int varstr_append(string_t vs, const tchar_t* fmt, ...)
{
	redstr_t* pvs = (redstr_t*)vs;

	va_list arg;
	int len;

	va_start(arg, fmt);
	len = xsprintf_arg(NULL, fmt, &arg);
	va_end(arg);

	if (!len)
		return pvs->count;

	if (pvs->count + len + 1 > pvs->size)
		varstr_incre(pvs, len);

	va_start(arg, fmt);
	xsprintf_arg(pvs->vbuf + pvs->count, fmt, &arg);
	va_end(arg);

	pvs->count += len;

	return pvs->count;
}

int varstr_printf(string_t vs,const tchar_t* fmt,...)
{
	redstr_t* pvs = (redstr_t*)vs;

	va_list arg;
	int len;

	varstr_empty(pvs);

	va_start(arg,fmt);
	len = xsprintf_arg(NULL,fmt,&arg);
	va_end(arg);

	if(!len)
		return pvs->count;

	if(pvs->count + len + 1 > pvs->size)
		varstr_incre(pvs,len);

	va_start(arg,fmt);
	xsprintf_arg(pvs->vbuf + pvs->count, fmt, &arg);
	va_end(arg);

	pvs->count += len;

	return pvs->count;
}

string_t varstr_clone(string_t vs)
{
	redstr_t* pvs = (redstr_t*)vs;

	redstr_t* pvs_new;

	pvs_new = varstr_alloc();
	varstr_cpy(pvs_new, pvs->vbuf, pvs->count);

	return (string_t)pvs_new;
}

int varstr_encode(string_t vs, int encode, byte_t* buf, dword_t max)
{
	redstr_t* pvs = (redstr_t*)vs;

	dword_t i;
	const tchar_t* str;

	if (encode == _UTF8)
	{
#ifdef _UNICODE
		return ucs_to_utf8(varstr_ptr(pvs), varstr_len(pvs), buf, max);
#else
		return mbs_to_utf8(varstr_ptr(pvs), varstr_len(pvs), buf, max);
#endif
	}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	else if (encode == _GB2312)
	{
#ifdef _UNICODE
		return ucs_to_gb2312(varstr_ptr(pvs), varstr_len(pvs), buf, max);
#else
		return mbs_to_gb2312(varstr_ptr(pvs), varstr_len(pvs), buf, max);
#endif
	}
#endif
	else if (encode == _UTF16_LIT)
	{
#ifdef _UNICODE
		return ucs_to_utf16lit(varstr_ptr(pvs), varstr_len(pvs), buf, max);
#else
		return mbs_to_utf16lit(varstr_ptr(vs), varstr_len(vs), buf, max);
#endif
	}
	else if (encode == _UTF16_BIG)
	{
#ifdef _UNICODE
		return ucs_to_utf16big(varstr_ptr(pvs), varstr_len(pvs), buf, max);
#else
		return mbs_to_utf16big(varstr_ptr(pvs), varstr_len(pvs), buf, max);
#endif
	}
	else
	{
#ifdef _UNICODE
		max = (max < varstr_len(pvs) * sizeof(wchar_t)) ? max : (varstr_len(pvs) * sizeof(wchar_t));
		if (buf)
		{
			str = varstr_ptr(pvs);
			for (i = 0; i < max / 2; i++)
			{
				buf[i * 2] = GETLBYTE(str[i]);
				buf[i * 2 + 1] = GETHBYTE(str[i]);
			}
		}
		return max;
#else
		max = (max < varstr_len(vs)) ? max : varstr_len(pvs);
		if (buf)
		{
			str = varstr_ptr(pvs);
			xmem_copy((void*)buf, (void*)str, max);
		}
		return max;
#endif
	}
}

int varstr_decode(string_t vs, int encode, const byte_t* buf, dword_t size)
{
	redstr_t* pvs = (redstr_t*)vs;

	int i,len = 0;
	tchar_t* str = NULL;

	if (encode == _UTF8)
	{
#ifdef _UNICODE
		len = utf8_to_ucs(buf, size, NULL, MAX_LONG);
#else
		len = utf8_to_mbs(buf, size, NULL, MAX_LONG);
#endif
	}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	else if (encode == _GB2312)
	{
#ifdef _UNICODE
		len = gb2312_to_ucs(buf, size, NULL, MAX_LONG);
#else
		len = gb2312_to_mbs(buf, size, NULL, MAX_LONG);
#endif
	}
#endif
	else if (encode == _UTF16_LIT)
	{
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf, size, NULL, MAX_LONG);
#else
		len = utf16lit_to_mbs(buf, size, NULL, MAX_LONG);
#endif
	}
	else if (encode == _UTF16_BIG)
	{
#ifdef _UNICODE
		len = utf16big_to_ucs(buf, size, NULL, MAX_LONG);
#else
		len = utf16big_to_mbs(buf, size, NULL, MAX_LONG);
#endif
	}
	else
	{
#ifdef _UNICODE
		len = size / 2;
#else
		len = size;
#endif
	}

	str = varstr_ensure_buf(pvs, len);

	if (encode == _UTF8)
	{
#ifdef _UNICODE
		len = utf8_to_ucs(buf, size, str, len);
#else
		len = utf8_to_mbs(buf, size,  str, len);
#endif
	}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	else if (encode == _GB2312)
	{
#ifdef _UNICODE
		len = gb2312_to_ucs(buf, size, str, len);
#else
		len = gb2312_to_mbs(buf, size, str, len);
#endif
	}
#endif
	else if (encode == _UTF16_LIT)
	{
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf, size, str, len);
#else
		len = utf16lit_to_mbs(buf, size, str, len);
#endif
	}
	else if (encode == _UTF16_BIG)
	{
#ifdef _UNICODE
		len = utf16big_to_ucs(buf, size, str, len);
#else
		len = utf16big_to_mbs(buf, size, str, len);
#endif
	}
	else
	{
#ifdef _UNICODE
		for (i = 0; i < len; i++)
		{
			str[i] = MAKESWORD(buf[i * 2], buf[i * 2 + 1]);
		}
#else
		xmem_copy((void*)str,(void*)buf,len);
#endif
	}

	return len;
}


