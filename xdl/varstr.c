/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant string document

	@module	string.c | implement file

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

#define VARSTR_INC	512

typedef struct _string_context{
	memobj_head head;

	int count;
	int size;
	void* data;
}string_context;

/*******************************************************************************************/

string_t string_alloc()
{
	string_context* pvs;

	pvs = (string_context*)xmem_alloc(sizeof(string_context));
	pvs->head.tag = MEM_STRING;
	PUT_THREEBYTE_LOC(pvs->head.len, 0, (sizeof(string_context) - 4));

	pvs->count = 0;
	pvs->size = 0;
	pvs->data = NULL;

	return (string_t)&(pvs->head);
}

void string_free(string_t vs)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	if (pvs->data)
		xmem_free(pvs->data);

	xmem_free(pvs);
}

void string_attach(string_t var, tchar_t* data)
{
	string_context* pvs = TypePtrFromHead(string_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_STRING);

	if (pvs->data)
		xmem_free(pvs->data);

	pvs->data = (void*)data;
	pvs->count = xslen(data);
	pvs->size = pvs->count + 1;
}

tchar_t* string_detach(string_t var)
{
	string_context* pvs = TypePtrFromHead(string_context, var);
	void* d;

	XDL_ASSERT(var != NULL && var->tag == MEM_STRING);

	d = pvs->data;
	pvs->data = NULL;
	pvs->count = 0;
	pvs->size = 0;

	return (tchar_t*)d;
}

void string_incre(string_t vs,int len)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int s;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	s = pvs->size;
	while(len + pvs->count + 1 > s)
		s += VARSTR_INC;

	if (s > pvs->size)
	{
		pvs->data = xmem_realloc(pvs->data, s * sizeof(tchar_t));
		pvs->size = s;
	}
}

tchar_t* string_ensure_buf(string_t vs, int len)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	pvs->count = 0;

	string_incre(vs, len);

	pvs->count = len;

	return (tchar_t*)pvs->data;
}

int string_resize(string_t vs, int len)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	if (len < 0)
	{
		while (count)
		{
			if (vbuf[count - 1] == _T('\0'))
			{
				count--;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		if (count < len)
		{
			string_incre(vs, len - count);
			vbuf = (tchar_t*)pvs->data;
		}

		while (count < len)
		{
			vbuf[count] = _T(' ');
			count++;
		}

		if (vbuf)
		{
			vbuf[len] = _T('\0');
		}
		count = len;
	}

	pvs->count = count;

	return count;
}

int	string_cat(string_t vs, const tchar_t* str, int len)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count, dlen;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	if (is_null(str))
		return count;

	dlen = (len < 0) ? xslen(str) : len;

	if (!dlen)
		return count;

	if (dlen + count + 1 > pvs->size)
	{
		string_incre(vs, dlen);
		vbuf = (tchar_t*)pvs->data;
	}

	if (vbuf)
	{
		xsncpy(vbuf + count, str, dlen);
	}

	count += dlen;
	pvs->count = count;

	return count;
}

int	string_cpy(string_t vs, const tchar_t* str, int len)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count, dlen;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	dlen = (len < 0) ? xslen(str) : len;

	if (!dlen)
	{
		string_empty(vs);
		return 0;
	}

	if (dlen + 1 > pvs->size)
	{
		string_incre(vs, dlen);
		vbuf = (tchar_t*)pvs->data;
	}

	if (vbuf)
	{
		xsncpy(vbuf, str, dlen);
	}

	count = dlen;
	pvs->count = count;

	return count;
}

tchar_t string_get_char(string_t vs, int pos)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	XDL_ASSERT(pos >= 0 && pos < count);

	return vbuf[pos];
}

bool_t string_set_char(string_t vs, int pos, tchar_t ch)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	XDL_ASSERT(pos >= 0 && pos < count);

	vbuf[pos] = ch;

	return 1;
}

int string_get_chars(string_t vs, int pos, tchar_t* pch, int n)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	XDL_ASSERT(pos >= 0 && pos < count);

	if (pch)
	{
		xszero(pch, n + 1);
	}

	n = (n < count - pos) ? n : (count - pos);

	if (pch)
	{
		xsncpy(pch, vbuf + pos, n);
	}

	return n;
}

void string_set_chars(string_t vs,int pos,const tchar_t* pch,int n)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	XDL_ASSERT(pos <= count);

	if (pos < 0)
		pos = count;

	if (n < 0)
		n = xslen(pch);

	if (pos + n + 1 > pvs->size)
	{
		string_incre(vs, pos + n - pvs->size);
		vbuf = (tchar_t*)pvs->data;
	}

	xsnset(vbuf, pos, pch, n);

	count = (count > pos + n) ? count : (pos + n);
	pvs->count = count;
}

void string_ins_chars(string_t vs,int pos,const tchar_t* pch,int n)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	XDL_ASSERT(pos <= count);

	if(pos < 0)
		pos = count;

	if (n < 0)
		n = xslen(pch);

	if (count + n + 1 > pvs->size)
	{
		string_incre(vs, n);
		vbuf = (tchar_t*)pvs->data;
	}

	if (n)
	{
		xsnins(vbuf, pos, pch, n);
		count += n;
		pvs->count = count;
	}
}

void string_del_chars(string_t vs,int pos,int n)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	XDL_ASSERT(pos >= 0 && pos < count);

	xsndel(vbuf, pos, n);
	
	count = (count > pos + n) ? (count - n) : pos;
	pvs->count = count;
}

const tchar_t* string_ptr(string_t vs)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	return (tchar_t*)pvs->data;
}

int string_len(string_t vs)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	return pvs->count;
}

void string_empty(string_t vs)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	if (pvs->size > VARSTR_INC)
	{
		xmem_free(pvs->data);
		pvs->size = 0;
	}
	else
	{
		xmem_zero((void*)pvs->data, pvs->size * sizeof(tchar_t));
	}

	pvs->count = 0;
}

bool_t string_is_empty(string_t vs)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	return (pvs->count) ? 0 : 1;
}

int string_append(string_t vs, const tchar_t* fmt, ...)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;
	int len;

	va_list arg;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	va_start(arg, fmt);
	len = xsprintf_arg(NULL, fmt, &arg);
	va_end(arg);

	if (!len)
		return count;

	if (count + len + 1 > pvs->size)
	{
		string_incre(vs, len);
		vbuf = (tchar_t*)pvs->data;
	}

	va_start(arg, fmt);
	xsprintf_arg(vbuf + count, fmt, &arg);
	va_end(arg);

	count += len;
	pvs->count = count;

	return count;
}

int string_printf(string_t vs,const tchar_t* fmt,...)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int len, count;
	tchar_t* vbuf;

	va_list arg;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	string_empty(vs);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	va_start(arg,fmt);
	len = xsprintf_arg(NULL,fmt,&arg);
	va_end(arg);

	if(!len)
		return count;

	if (count + len + 1 > pvs->size)
	{
		string_incre(vs, len);
		vbuf = (tchar_t*)pvs->data;
	}

	va_start(arg,fmt);
	xsprintf_arg(vbuf + count, fmt, &arg);
	va_end(arg);

	count += len;
	pvs->count = count;

	return count;
}

string_t string_clone(string_t vs)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);
	int count;
	tchar_t* vbuf;

	string_t pvs_new;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	count = pvs->count;
	vbuf = (tchar_t*)pvs->data;

	pvs_new = string_alloc();
	string_cpy(pvs_new, vbuf, count);

	return pvs_new;
}

dword_t string_encode(string_t vs, int encode, byte_t* buf, dword_t max)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	dword_t i;
	const tchar_t* str;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	if (encode == _UTF8)
	{
#ifdef _UNICODE
		return ucs_to_utf8(string_ptr(vs), string_len(vs), buf, max);
#else
		return mbs_to_utf8(string_ptr(vs), string_len(vs), buf, max);
#endif
	}
#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	else if (encode == _GB2312)
	{
#ifdef _UNICODE
		return ucs_to_gb2312(string_ptr(vs), string_len(vs), buf, max);
#else
		return mbs_to_gb2312(string_ptr(vs), string_len(vs), buf, max);
#endif
	}
#endif
	else if (encode == _UTF16_LIT)
	{
#ifdef _UNICODE
		return ucs_to_utf16lit(string_ptr(vs), string_len(vs), buf, max);
#else
		return mbs_to_utf16lit(string_ptr(vs), string_len(vs), buf, max);
#endif
	}
	else if (encode == _UTF16_BIG)
	{
#ifdef _UNICODE
		return ucs_to_utf16big(string_ptr(vs), string_len(vs), buf, max);
#else
		return mbs_to_utf16big(string_ptr(vs), string_len(vs), buf, max);
#endif
	}
	else
	{
#ifdef _UNICODE
		max = (max < string_len(vs) * sizeof(wchar_t)) ? max : (string_len(vs) * sizeof(wchar_t));
		if (buf)
		{
			str = string_ptr(vs);
			for (i = 0; i < max / 2; i++)
			{
				buf[i * 2] = GETLBYTE(str[i]);
				buf[i * 2 + 1] = GETHBYTE(str[i]);
			}
		}
		return max;
#else
		max = (max < string_len(vs)) ? max : string_len(vs);
		if (buf)
		{
			str = string_ptr(vs);
			xmem_copy((void*)buf, (void*)str, max);
		}
		return max;
#endif
	}
}

int string_decode(string_t vs, int encode, const byte_t* buf, dword_t size)
{
	string_context* pvs = TypePtrFromHead(string_context, vs);

	int i, len = 0;
	tchar_t* str = NULL;

	XDL_ASSERT(vs != NULL && vs->tag == MEM_STRING);

	if (encode == _UTF8)
	{
#ifdef _UNICODE
		len = utf8_to_ucs(buf, size, NULL, MAX_LONG);
#else
		len = utf8_to_mbs(buf, size, NULL, MAX_LONG);
#endif
	}
#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
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

	str = string_ensure_buf(vs, len);

	if (encode == _UTF8)
	{
#ifdef _UNICODE
		len = utf8_to_ucs(buf, size, str, len);
#else
		len = utf8_to_mbs(buf, size,  str, len);
#endif
	}
#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
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


