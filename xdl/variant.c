/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant value document

	@module	variant.c | implement file

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

#include "variant.h"
#include "miscella.h"

#include "xdlimp.h"


typedef struct _variant_context{
	memobj_head head;

	int type;
	int count;
	void* data;
}variant_context;


static int variant_realloc(variant_t var, byte_t type, int count)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	int sw = 0;

	switch ((type & 0x7F))
	{
	case VV_BOOL:
		sw = sizeof(bool_t);
		break;
	case VV_BYTE:
		sw = sizeof(byte_t);
		break;
	case VV_SCHAR:
		sw = sizeof(schar_t);
		break;
	case VV_WCHAR:
		sw = sizeof(wchar_t);
		break;
	case VV_SHORT:
		sw = sizeof(short);
		break;
	case VV_INT:
		sw = sizeof(int);
		break;
	case VV_LONG:
		sw = sizeof(long long);
		break;
	case VV_FLOAT:
		sw = sizeof(float);
		break;
	case VV_DOUBLE:
		sw = sizeof(double);
		break;
	case VV_STRING:
		sw = sizeof(tchar_t) * count;
		break;
	case VV_BOOL_ARRAY:
		sw = sizeof(bool_t) * count;
		break;
	case VV_BYTE_ARRAY:
		sw = sizeof(byte_t) * count;
		break;
	case VV_SHORT_ARRAY:
		sw = sizeof(short) * count;
		break;
	case VV_INT_ARRAY:
		sw = sizeof(int) * count;
		break;
	case VV_LONG_ARRAY:
		sw = sizeof(long long) * count;
		break;
	case VV_SCHAR_ARRAY:
		sw = sizeof(schar_t) * count;
		break;
	case VV_WCHAR_ARRAY:
		sw = sizeof(wchar_t) * count;
		break;
	case VV_FLOAT_ARRAY:
		sw = sizeof(float) * count;
		break;
	case VV_DOUBLE_ARRAY:
		sw = sizeof(double) * count;
		break;
	case VV_STRING_ARRAY:
		sw = sizeof(tchar_t) * count;
		break;
	default:
		type = VV_NULL;
		sw = 0;
		break;
	}

	pvar->data = xmem_realloc(pvar->data, sw);
	pvar->type = type;
	pvar->count = count;
	
	return sw;
}

/************************************************************************************************************************************************/

variant_t variant_alloc(int type)
{
	variant_context* pvar;

	pvar = (variant_context*)xmem_alloc(sizeof(variant_context));
	pvar->head.tag = MEM_VARIANT;
	PUT_THREEBYTE_LOC(pvar->head.len, 0, (sizeof(variant_context) - 4));

	variant_realloc((variant_t)&(pvar->head), type, 0);

	return (variant_t)&(pvar->head);
}

variant_t variant_clone(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	variant_context* pnew;
	int n;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	pnew = (variant_context*)variant_alloc(pvar->type);
	n = variant_realloc((variant_t)&(pnew->head), pvar->type, pvar->count);
	
	xmem_copy(pnew->data, pvar->data, n);

	return (variant_t)&(pnew->head);
}

void variant_free(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	if (pvar->data)
		xmem_free(pvar->data);

	xmem_free(pvar);
}

int variant_get_type(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	return pvar->type;
}

int variant_get_count(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	return pvar->count;
}

void* variant_data(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	return pvar->data;
}

void variant_attach(variant_t var, void* data)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	if (pvar->data)
		xmem_free(pvar->data);

	pvar->data = data;
}

void* variant_detach(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void* d;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	d = pvar->data;
	pvar->data = NULL;

	return d;
}

void variant_to_null(variant_t var, int type)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	variant_realloc(var, type, 0);
}

bool_t variant_is_null(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	return (pvar->type == VV_NULL) ? 1 : 0;
}

void variant_copy(variant_t dst, variant_t src)
{
	variant_context* psrc = TypePtrFromHead(variant_context, src);
	variant_context* pdst = TypePtrFromHead(variant_context, dst);
	
	int sw;

	XDL_ASSERT(src != NULL && src->tag == MEM_VARIANT && dst != NULL && dst->tag == MEM_VARIANT);

	sw = variant_realloc(dst, psrc->type, psrc->count);

	xmem_copy(pdst->data, psrc->data, sw);
}

int variant_comp(variant_t var1, variant_t var2)
{
	variant_context* pvar1 = TypePtrFromHead(variant_context, var1);
	variant_context* pvar2 = TypePtrFromHead(variant_context, var2);
	byte_t t1, t2;
	void *d1, *d2;
	int n1, n2, i;

	XDL_ASSERT(var1 != NULL && var1->tag == MEM_VARIANT && var2 != NULL && var2->tag == MEM_VARIANT);

	t1 = pvar1->type;
	t2 = pvar2->type;

	if (t1 == VV_NULL && t2 == VV_NULL)
		return 0;
	else if (t1 == VV_NULL)
		return -1;
	else if (t2 == VV_NULL)
		return 1;

	if ((t1 & 0x7F) != (t2 & 0x7F))
		return (t1 < t2) ? -1 : 1;

	n1 = pvar1->count;
	n2 = pvar2->count;
	d1 = pvar1->data;
	d2 = pvar2->data;

	switch ((t2 & 0x7F))
	{
	case VV_BOOL:
		if ((*(bool_t*)d1) >(*(bool_t*)d2))
			return 1;
		else if ((*(bool_t*)d1) < (*(bool_t*)d2))
			return -1;
		else
			return 0;
	case VV_BYTE:
		if ((*(byte_t*)d1) >(*(byte_t*)d2))
			return 1;
		else if ((*(byte_t*)d1) < (*(byte_t*)d2))
			return -1;
		else
			return 0;
	case VV_SCHAR:
		if ((*((schar_t*)d1)) > (*((schar_t*)d2)))
			return 1;
		else if ((*((schar_t*)d1)) < (*((schar_t*)d2)))
			return -1;
		else
			return 0;
	case VV_WCHAR:
		if ((*((wchar_t*)d1)) > (*((wchar_t*)d2)))
			return 1;
		else if ((*((wchar_t*)d1)) < (*((wchar_t*)d2)))
			return -1;
		else
			return 0;
	case VV_SHORT:
		if ((*((short*)d1)) > (*((short*)d2)))
			return 1;
		else if((*((short*)d1)) < (*((short*)d2)))
			return -1;
		else
			return 0;
	case VV_INT:
		if ((*((int*)d1)) > (*((int*)d2)))
			return 1;
		else if ((*((int*)d1)) < (*((int*)d2)))
			return -1;
		else
			return 0;
	case VV_LONG:
		if ((*((long*)d1)) > (*((long*)d2)))
			return 1;
		else if ((*((long*)d1)) < (*((long*)d2)))
			return -1;
		else
			return 0;
	case VV_FLOAT:
		if ((*((float*)d1)) > (*((float*)d2)))
			return 1;
		else if ((*((float*)d1)) < (*((float*)d2)))
			return -1;
		else
			return 0;
	case VV_DOUBLE:
		if ((*((double*)d1)) > (*((double*)d2)))
			return 1;
		else if ((*((double*)d1)) < (*((double*)d2)))
			return -1;
		else
			return 0;
	case VV_STRING:
		return compare_text((tchar_t*)d1, n1, (tchar_t*)d2, n2, 0);
	case VV_BOOL_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((bool_t*)d1)[i] > ((bool_t*)d2)[i])
				return 1;
			else if (((bool_t*)d1)[i] < ((bool_t*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_BYTE_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((byte_t*)d1)[i] >((byte_t*)d2)[i])
				return 1;
			else if (((byte_t*)d1)[i] < ((byte_t*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_SHORT_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((short*)d1)[i] >((short*)d2)[i])
				return 1;
			else if (((short*)d1)[i] < ((short*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_INT_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((int*)d1)[i] >((int*)d2)[i])
				return 1;
			else if (((int*)d1)[i] < ((int*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_LONG_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((long long*)d1)[i] >((long long*)d2)[i])
				return 1;
			else if (((long long*)d1)[i] < ((long long*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_SCHAR_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((schar_t*)d1)[i] >((schar_t*)d2)[i])
				return 1;
			else if (((schar_t*)d1)[i] < ((schar_t*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_WCHAR_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((wchar_t*)d1)[i] >((wchar_t*)d2)[i])
				return 1;
			else if (((wchar_t*)d1)[i] < ((wchar_t*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_FLOAT_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((float*)d1)[i] >((float*)d2)[i])
				return 1;
			else if (((float*)d1)[i] < ((float*)d2)[i])
				return -1;
			
			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_DOUBLE_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((double*)d1)[i] >((double*)d2)[i])
				return 1;
			else if (((double*)d1)[i] < ((double*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	case VV_STRING_ARRAY:
		i = 0;
		while (i < n1 && i < n2)
		{
			if (((tchar_t*)d1)[i] >((tchar_t*)d2)[i])
				return 1;
			else if (((tchar_t*)d1)[i] < ((tchar_t*)d2)[i])
				return -1;

			i++;
		}
		if (i < n1)
			return 1;
		else if (i < n2)
			return -1;
		else
			return 0;
	}

	return 0;
}

int variant_to_string(variant_t var, tchar_t* buf, int max)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t, i, n, k, j;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	n = pvar->count;
	d = pvar->data;

	switch (t & 0x7F)
	{
	case VV_NULL:
		if (buf)
		{
			buf[0] = _T('\0');
		}
		return 0;
	case VV_BOOL:
		if (buf)
		{
			buf[0] = (*((bool_t*)d)) ? _T('1') : _T('0');
			buf[1] = _T('\0');
		}
		return 1;
	case VV_BYTE:
		if (buf)
		{
			format_hexnum(*((byte_t*)d), buf, 2);
		}
		return 1;
	case VV_SCHAR:
		if (buf)
		{
			buf[0] = *((schar_t*)d);
			buf[1] = _T('\0');
		}
		return 1;
	case VV_WCHAR:
		if (buf)
		{
			buf[0] = *((wchar_t*)d);
			buf[1] = _T('\0');
		}
		return 1;
	case VV_SHORT:
		return stoxs(*((short*)d), buf, max);
	case VV_INT:
		return ltoxs(*((int*)d), buf, max);
	case VV_LONG:
		return lltoxs(*((long long*)d), buf, max);
	case VV_FLOAT:
		return ftoxs(*((float*)d), buf, max);
	case VV_DOUBLE:
		return numtoxs(*((double*)d), buf, max);
	case VV_STRING:
		n = (n < max) ? n : max;
		if (buf)
		{
			xmem_copy((void*)buf, d, n * sizeof(tchar_t));
			buf[n] = _T('\0');
		}
		return n;
	case VV_BOOL_ARRAY:
		for (i = 0; i < n; i++)
		{
			if (buf)
			{
				buf[i] = (((bool_t*)d)[i]) ? _T('1') : _T('0');
				buf[i + 1] = _T('\0');
			}
		}
		return (i);
	case VV_BYTE_ARRAY:
		for (i = 0; i < n; i++)
		{
			format_hexnum((unsigned int)(((byte_t*)d)[i]), ((buf) ? buf + i * 3 : NULL), 2);
		}
		return (i * 2);
	case VV_SCHAR_ARRAY:
#ifdef _UNICODE
		return mbs_to_ucs((schar_t*)(d), n, buf, max);
#else
		n = (n < max) ? n : max;
		if (buf)
		{
			xmem_copy((void*)buf, d, n * sizeof(schar_t));
			buf[n] = '\0';
		}
		return n;
#endif
	case VV_WCHAR_ARRAY:
#ifdef _UNICODE
		n = (n < max) ? n : max;
		if (buf)
		{
			xmem_copy((void*)buf, (void*)(d), n * sizeof(wchar_t));
			buf[n] = L'\0';
		}
		return n;
#else
		return ucs_to_mbs((wchar_t*)(d), n, buf, max);
#endif
	case VV_SHORT_ARRAY:
		k = 0;
		for (i = 0; i < n; i++)
		{
			j = stoxs(((short*)d)[i], ((buf) ? (buf + k) : NULL), NUM_LEN);
			k += j;

			if (buf)
			{
				buf[k] = _T(' ');
			}
			k++;
		}
		return k;
	case VV_INT_ARRAY:
		k = 0;
		for (i = 0; i < n; i++)
		{
			j = ltoxs(((int*)d)[i], ((buf) ? (buf + k) : NULL), NUM_LEN);
			k += j;

			if (buf)
			{
				buf[k] = _T(' ');
			}
			k++;
		}
		return k;
	case VV_LONG_ARRAY:
		k = 0;
		for (i = 0; i < n; i++)
		{
			j = lltoxs(((long long*)d)[i], ((buf) ? (buf + k) : NULL), NUM_LEN);
			k += j;

			if (buf)
			{
				buf[k] = _T(' ');
			}
			k++;
		}
		return k;
	case VV_FLOAT_ARRAY:
		k = 0;
		for (i = 0; i < n; i++)
		{
			j = ftoxs(((float*)d)[i], ((buf) ? (buf + k) : NULL), NUM_LEN);
			k += j;

			if (buf)
			{
				buf[k] = _T(' ');
			}
			k++;
		}
		return k;
	case VV_DOUBLE_ARRAY:
		k = 0;
		for (i = 0; i < n; i++)
		{
			j = numtoxs(((double*)d)[i], ((buf) ? (buf + k) : NULL), NUM_LEN);
			k += j;

			if (buf)
			{
				buf[k] = _T(' ');
			}
			k++;
		}
		return k;
	case VV_STRING_ARRAY:
		n = (n < max) ? n : max;
		if (buf)
		{
			xmem_copy((void*)buf, (void*)d, n * sizeof(tchar_t));
		}
		return n;
	}

	return 0;
}

void variant_from_string(variant_t var, const tchar_t* buf, int len)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t, i, n, k, j;
	tchar_t *key;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT); 

	if (len < 0)
		len = xslen(buf);

	t = pvar->type;
	n = pvar->count;
	d = pvar->data;

	switch (t & 0x7F)
	{
	case VV_NULL:
		variant_realloc(var, VV_STRING, len);
		d = pvar->data;
		xmem_copy(d, (void*)buf, len * sizeof(tchar_t));
		break;
	case VV_BOOL:
		*((bool_t*)d) = (buf[0] == _T('1')) ? 1 : 0;
		break;
	case VV_BYTE:
		*((byte_t*)d) = (byte_t)parse_hexnum(buf, 2);
		break;
	case VV_SCHAR:
#ifdef _UNICODE
		ucs_byte_to_mbs(buf[0], (schar_t*)d);
#else
		*((schar_t*)d) = buf[0];
#endif
		break;
	case VV_WCHAR:
#ifdef _UNICODE
		*((wchar_t*)d) = buf[0];
#else
		mbs_byte_to_ucs(buf[0], (wchar_t*)d);
#endif
		break;
	case VV_SHORT:
		*((short*)d) = xsntos(buf, len);
		break;
	case VV_INT:
		*((int*)d) = xsntol(buf, len);
		break;
	case VV_LONG:
		*((long long*)d) = xsntoll(buf, len);
		break;
	case VV_FLOAT:
		*((float*)d) = xsntof(buf, len);
		break;
	case VV_DOUBLE:
		*((double*)d) = xsntonum(buf, len);
		break;
	case VV_STRING:
		variant_realloc(var, VV_STRING, len);
		d = pvar->data;
		xmem_copy(d, (void*)buf, len * sizeof(tchar_t));
		break;
	case VV_BOOL_ARRAY:
		variant_realloc(var, VV_BOOL_ARRAY, len);
		d = pvar->data;
		for (i = 0; i < len;i++)
		{
			((bool_t*)d)[i] = (buf[i] == _T('1')) ? 1 : 0;
		}
		break;
	case VV_BYTE_ARRAY:
		variant_realloc(var, VV_BYTE_ARRAY, len / 2);
		d = pvar->data;
		for (i = 0; i < len / 2; i++)
		{
			((byte_t*)d)[i] = (byte_t)parse_hexnum(&(buf[i*2]), 2);
		}
		break;
	case VV_SCHAR_ARRAY:
#ifdef _UNICODE
		n = ucs_to_mbs(buf, len, NULL, MAX_LONG);
		variant_realloc(var, VV_SCHAR_ARRAY, n);
		d = pvar->data;
		ucs_to_mbs(buf, len, (schar_t*)d, n);
#else
		variant_realloc(var, VV_SCHAR_ARRAY, len);
		d = pvar->data;
		xmem_copy((void*)d, (void*)buf, len * sizeof(schar_t));
#endif
		break;
	case VV_WCHAR_ARRAY:
#ifdef _UNICODE
		variant_realloc(var, VV_WCHAR_ARRAY, len);
		d = pvar->data;
		xmem_copy((void*)d, (void*)buf, len * sizeof(wchar_t));
#else
		n = mbs_to_ucs(buf, len, NULL, MAX_LONG);
		variant_realloc(var, VV_WCHAR_ARRAY, n);
		d = pvar->data;
		mbs_to_ucs(buf, len, (wchar_t*)d, n);
#endif
		break;
	case VV_SHORT_ARRAY:
		n = parse_string_token_count(buf, len, _T(' '));
		variant_realloc(var, VV_SHORT_ARRAY, n);
		d = pvar->data;

		k = 0;
		i = 0;
		while (n = parse_string_token((buf + k), (len - k), _T(' '), &key, &j))
		{
			((short*)d)[i] = xsntos(key, j);
			k += n;
			i++;
		}
		break;
	case VV_INT_ARRAY:
		n = parse_string_token_count(buf, len, _T(' '));
		variant_realloc(var, VV_INT_ARRAY, n);
		d = pvar->data;

		k = 0;
		i = 0;
		while (n = parse_string_token((buf + k), (len - k), _T(' '), &key, &j))
		{
			((int*)d)[i] = xsntol(key, j);
			k += n;
			i++;
		}
		break;
	case VV_LONG_ARRAY:
		n = parse_string_token_count(buf, len, _T(' '));
		variant_realloc(var, VV_LONG_ARRAY, n);
		d = pvar->data;

		k = 0;
		i = 0;
		while (n = parse_string_token((buf + k), (len - k), _T(' '), &key, &j))
		{
			((long long*)d)[i] = xsntoll(key, j);
			k += n;
			i++;
		}
		break;
	case VV_FLOAT_ARRAY:
		n = parse_string_token_count(buf, len, _T(' '));
		variant_realloc(var, VV_FLOAT_ARRAY, n);
		d = pvar->data;

		k = 0;
		i = 0;
		while (n = parse_string_token((buf + k), (len - k), _T(' '), &key, &j))
		{
			((float*)d)[i] = xsntof(key, j);
			k += n;
			i++;
		}
		break;
	case VV_DOUBLE_ARRAY:
		n = parse_string_token_count(buf, len, _T(' '));
		variant_realloc(var, VV_DOUBLE_ARRAY, n);
		d = pvar->data;

		k = 0;
		i = 0;
		while (n = parse_string_token((buf + k), (len - k), _T(' '), &key, &j))
		{
			((double*)d)[i] = xsntonum(key, j);
			k += n;
			i++;
		}
		break;
	case VV_STRING_ARRAY:
		variant_realloc(var, VV_STRING_ARRAY, len);
		d = pvar->data;

		xmem_copy((void*)d, (void*)buf, len * sizeof(tchar_t));
		break;
	}
}

void variant_set_bool(variant_t var, bool_t b)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT); 

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_BOOL);

	*((bool_t*)d) = ((b)? 1 : 0);
}

bool_t variant_get_bool(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT); 

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_BOOL);

	return (*((bool_t*)d));
}

void variant_set_schar(variant_t var, schar_t c)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_SCHAR);

	*((schar_t*)d) = c;
}

schar_t variant_get_schar(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_SCHAR);

	return (*((schar_t*)d));
}

void variant_set_wchar(variant_t var, wchar_t w)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT); 

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_WCHAR);

	*((wchar_t*)d) = w;
}

wchar_t variant_get_wchar(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_WCHAR);

	return (*((wchar_t*)d));
}

void variant_set_short(variant_t var, short c)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_SHORT);

	*((short*)d) = c;
}

short variant_get_short(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_SHORT);

	return (*((short*)d));
}

void variant_set_int(variant_t var, int c)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_INT);

	*((int*)d) = c;
}

int variant_get_int(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_INT);

	return (*((int*)d));
}

void variant_set_long(variant_t var, long long c)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_LONG);

	*((long long*)d) = c;
}

long long variant_get_long(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_LONG);

	return (*((long long*)d));
}

void variant_set_float(variant_t var, float c)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_FLOAT);

	*((float*)d) = c;
}

float variant_get_float(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_FLOAT);

	return (*((float*)d));
}

void variant_set_double(variant_t var, double c)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_DOUBLE);

	*((double*)d) = c;
}

double variant_get_double(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_DOUBLE);

	return (*((double*)d));
}

const tchar_t* variant_get_string_ptr(variant_t var)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	d = pvar->data;

	XDL_ASSERT(t == VV_STRING);

	return (tchar_t*)(d);
}


dword_t variant_encode(variant_t var, int encode, byte_t* buf, dword_t max)
{
	dword_t n = 0;
	tchar_t* str;
	int len;

	XDL_ASSERT(var != NULL);

	len = variant_to_string(var, NULL, MAX_LONG);
	str = xsalloc(len + 1);
	variant_to_string(var, str, len);

	switch (encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = ucs_to_gb2312(str, len, ((buf) ? (buf + 1) : NULL), max);
#else
		n = mbs_to_gb2312(str, len, ((buf)? (buf + 1): NULL), max);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = ucs_to_utf8(str, len, ((buf) ? (buf + 1) : NULL), max);
#else
		n = mbs_to_utf8(str, len, ((buf)? (buf + 1) : NULL), max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = ucs_to_utf16lit(str, len, ((buf) ? (buf + 1) : NULL), max);
#else
		n = mbs_to_utf16lit(str, len, ((buf)? (buf + 1) : NULL), max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = ucs_to_utf16big(str, len, ((buf) ? (buf + 1) : NULL), max);
#else
		n = mbs_to_utf16big(str, len, ((buf)? (buf + 1) : NULL), max);
#endif
		break;
	}

	xsfree(str);

	if (buf)
	{
		buf[0] = (byte_t)variant_get_type(var);
	}

	return (n + 1);
}

void variant_decode(variant_t var, int encode, const byte_t* buf, dword_t len)
{
	int n;
	tchar_t* str;
	byte_t type;

	if (!buf || !len)
	{
		if (var)
		{
			type = variant_get_type(var);
			variant_to_null(var, type);
		}
		return;
	}

	if (var)
	{
		type = buf[0];
		variant_realloc(var, type, 0);
	}

	switch (encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = gb2312_to_ucs((buf + 1), (len - 1), NULL, MAX_LONG);
#else
		n = gb2312_to_mbs((buf + 1), (len - 1), NULL, MAX_LONG);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = utf8_to_ucs((buf + 1), (len - 1), NULL, MAX_LONG);
#else
		n = utf8_to_mbs((buf + 1), (len - 1), NULL, MAX_LONG);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = utf16lit_to_ucs((buf + 1), (len - 1), NULL, MAX_LONG);
#else
		n = utf16lit_to_mbs((buf + 1), (len - 1), NULL, MAX_LONG);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = utf16big_to_ucs((buf + 1), (len - 1), NULL, MAX_LONG);
#else
		n = utf16big_to_mbs((buf + 1), (len - 1), NULL, MAX_LONG);
#endif
		break;
	}

	str = xsalloc(n + 1);

	switch (encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = gb2312_to_ucs((buf + 1), (len - 1), str, n);
#else
		n = gb2312_to_mbs((buf + 1), (len - 1), str, n);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = utf8_to_ucs((buf + 1), (len - 1), str, n);
#else
		n = utf8_to_mbs((buf + 1), (len - 1), str, n);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = utf16lit_to_ucs((buf + 1), (len - 1), str, n);
#else
		n = utf16lit_to_mbs((buf + 1), (len - 1), str, n);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = utf16big_to_ucs((buf + 1), (len - 1), str, n);
#else
		n = utf16big_to_mbs((buf + 1), (len - 1), str, n);
#endif
		break;
	}

	if (var)
	{
		variant_from_string(var, str, n);
	}

	xsfree(str);
}

void variant_hash32(variant_t var, key32_t* pkey)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t, n, sw;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	n = pvar->count;
	d = pvar->data;

	switch (t & 0x7F)
	{
	case VV_BOOL:
		sw = sizeof(bool_t);
		break;
	case VV_BYTE:
		sw = sizeof(byte_t);
		break;
	case VV_SCHAR:
		sw = sizeof(schar_t);
		break;
	case VV_WCHAR:
		sw = sizeof(wchar_t);
		break;
	case VV_SHORT:
		sw = sizeof(short);
		break;
	case VV_INT:
		sw = sizeof(int);
		break;
	case VV_LONG:
		sw = sizeof(long long);
		break;
	case VV_FLOAT:
		sw = sizeof(float);
		break;
	case VV_DOUBLE:
		sw = sizeof(double);
		break;
	case VV_STRING:
		sw = sizeof(tchar_t) * n;
		break;
	case VV_BOOL_ARRAY:
		sw = sizeof(bool_t) * n;
		break;
	case VV_BYTE_ARRAY:
		sw = sizeof(byte_t) * n;
		break;
	case VV_SHORT_ARRAY:
		sw = sizeof(short) * n;
		break;
	case VV_INT_ARRAY:
		sw = sizeof(int) * n;
		break;
	case VV_LONG_ARRAY:
		sw = sizeof(long long) * n;
		break;
	case VV_SCHAR_ARRAY:
		sw = sizeof(schar_t) * n;
		break;
	case VV_WCHAR_ARRAY:
		sw = sizeof(wchar_t) * n;
		break;
	case VV_FLOAT_ARRAY:
		sw = sizeof(float) * n;
		break;
	case VV_DOUBLE_ARRAY:
		sw = sizeof(double) * n;
		break;
	case VV_STRING_ARRAY:
		sw = sizeof(tchar_t) * n;
		break;
	default:
		sw = 0;
		break;
	}

	if (sw)
		murhash32((byte_t*)d, sw, (byte_t*)pkey);
	else
		xmem_zero((void*)pkey, sizeof(key32_t));
}

void variant_hash64(variant_t var, key64_t* pkey)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t, n, sw;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	n = pvar->count;
	d = pvar->data;

	switch (t & 0x7F)
	{
	case VV_BOOL:
		sw = sizeof(bool_t);
		break;
	case VV_BYTE:
		sw = sizeof(byte_t);
		break;
	case VV_SCHAR:
		sw = sizeof(schar_t);
		break;
	case VV_WCHAR:
		sw = sizeof(wchar_t);
		break;
	case VV_SHORT:
		sw = sizeof(short);
		break;
	case VV_INT:
		sw = sizeof(int);
		break;
	case VV_LONG:
		sw = sizeof(long long);
		break;
	case VV_FLOAT:
		sw = sizeof(float);
		break;
	case VV_DOUBLE:
		sw = sizeof(double);
		break;
	case VV_STRING:
		sw = sizeof(tchar_t) * n;
		break;
	case VV_BOOL_ARRAY:
		sw = sizeof(bool_t) * n;
		break;
	case VV_BYTE_ARRAY:
		sw = sizeof(byte_t) * n;
		break;
	case VV_SHORT_ARRAY:
		sw = sizeof(short) * n;
		break;
	case VV_INT_ARRAY:
		sw = sizeof(int) * n;
		break;
	case VV_LONG_ARRAY:
		sw = sizeof(long long) * n;
		break;
	case VV_SCHAR_ARRAY:
		sw = sizeof(schar_t) * n;
		break;
	case VV_WCHAR_ARRAY:
		sw = sizeof(wchar_t) * n;
		break;
	case VV_FLOAT_ARRAY:
		sw = sizeof(float) * n;
		break;
	case VV_DOUBLE_ARRAY:
		sw = sizeof(double) * n;
		break;
	case VV_STRING_ARRAY:
		sw = sizeof(tchar_t) * n;
		break;
	default:
		sw = 0;
		break;
	}

	if (sw)
		siphash64((byte_t*)d, sw, (byte_t*)pkey);
	else
		xmem_zero((void*)pkey, sizeof(key64_t));
}

void variant_hash128(variant_t var, key128_t* pkey)
{
	variant_context* pvar = TypePtrFromHead(variant_context, var);
	void *d;
	int t, n, sw;

	XDL_ASSERT(var != NULL && var->tag == MEM_VARIANT);

	t = pvar->type;
	n = pvar->count;
	d = pvar->data;

	switch (t & 0x7F)
	{
	case VV_BOOL:
		sw = sizeof(bool_t);
		break;
	case VV_BYTE:
		sw = sizeof(byte_t);
		break;
	case VV_SCHAR:
		sw = sizeof(schar_t);
		break;
	case VV_WCHAR:
		sw = sizeof(wchar_t);
		break;
	case VV_SHORT:
		sw = sizeof(short);
		break;
	case VV_INT:
		sw = sizeof(int);
		break;
	case VV_LONG:
		sw = sizeof(long long);
		break;
	case VV_FLOAT:
		sw = sizeof(float);
		break;
	case VV_DOUBLE:
		sw = sizeof(double);
		break;
	case VV_STRING:
		sw = sizeof(tchar_t) * n;
		break;
	case VV_BOOL_ARRAY:
		sw = sizeof(bool_t) * n;
		break;
	case VV_BYTE_ARRAY:
		sw = sizeof(byte_t) * n;
		break;
	case VV_SHORT_ARRAY:
		sw = sizeof(short) * n;
		break;
	case VV_INT_ARRAY:
		sw = sizeof(int) * n;
		break;
	case VV_LONG_ARRAY:
		sw = sizeof(long long) * n;
		break;
	case VV_SCHAR_ARRAY:
		sw = sizeof(schar_t) * n;
		break;
	case VV_WCHAR_ARRAY:
		sw = sizeof(wchar_t) * n;
		break;
	case VV_FLOAT_ARRAY:
		sw = sizeof(float) * n;
		break;
	case VV_DOUBLE_ARRAY:
		sw = sizeof(double) * n;
		break;
	case VV_STRING_ARRAY:
		sw = sizeof(tchar_t) * n;
		break;
	default:
		sw = 0;
		break;
	}

	if (sw)
		murhash128((byte_t*)d, sw, (byte_t*)pkey);
	else
		xmem_zero((void*)pkey, sizeof(key128_t));
}

#if defined(XDL_SUPPORT_TEST)
void test_variant(void)
{
	variant_t v1 = variant_alloc(VV_STRING);
	variant_t v2 = variant_alloc(VV_STRING);

	XDL_ASSERT(variant_comp(v1, v2) == 0);

	variant_from_string(v1, _T("test1"), -1);
	variant_from_string(v2, _T("test2"), -1);

	XDL_ASSERT(variant_comp(v1, v2) < 0);

	variant_free(v1);
	variant_free(v2);

	v1 = variant_alloc(VV_INT);
	v2 = variant_alloc(VV_INT);
	variant_from_string(v1, _T("123456789"), -1);

	tchar_t token[NUM_LEN + 1];
	variant_to_string(v1, token, NUM_LEN);

	variant_from_string(v2, token, -1);

	XDL_ASSERT(variant_comp(v1, v2) == 0);

	byte_t* buf;
	dword_t len;
	len = variant_encode(v1, _UTF8, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(len);
	variant_encode(v1, _UTF8, buf, len);

	variant_decode(v2, _UTF8, buf, len);

	XDL_ASSERT(variant_comp(v1, v2) == 0);

	xmem_free(buf);

	variant_free(v1);
	variant_free(v2);
}
#endif