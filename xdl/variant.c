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
#include "xdlimp.h"

#include "xdlstd.h"


#define VARIANT_HDR_SIZE	8

/*********************************************************************************************************************************************/

static int _parse_string_array_count(const tchar_t* tokens)
{
	int count = 0;
	int len;

	len = xslen(tokens);
	while (len)
	{
		count++;
		tokens += (len + 1);
		len = xslen(tokens);
	}

	return count;
}

static int _parse_string_array(const tchar_t* tokens, tchar_t** sa, int n)
{
	int len, i = 0;

	len = xslen(tokens);
	while (len && i < n)
	{
		sa[i++] = xsclone(tokens);
		tokens += (len + 1);
		len = xslen(tokens);
	}

	return i;
}

static int _format_string_array(const tchar_t** sa, int n, tchar_t* tokens, int max)
{
	int len, i, total = 0;

	for (i = 0; i < n; i++)
	{
		len = xslen(sa[i]);
		if (total + len + 1 > max)
			break;

		if (tokens)
		{
			xsncpy(tokens, sa[i], len);
			tokens[len] = _T('\0');
			tokens += (len + 1);
		}
		total += (len + 1);
	}

	if (total + 1 > max)
		return total;

	if (tokens)
	{
		tokens[0] = _T('\0');
	}
	total++;

	return total;
}

/************************************************************************************************************************************************/

variant_t* variant_alloc(int encode)
{
	variant_t* pnew;

	pnew = (variant_t*)xmem_alloc(sizeof(variant_t));
	pnew->vv = VV_NULL;
	pnew->encode = encode;

	return pnew;
}

variant_t* variant_clone(const variant_t* pv)
{
	variant_t* pnew;

	pnew = (variant_t*)xmem_alloc(sizeof(variant_t));
	pnew->vv = VV_NULL;
	pnew->encode = DEF_MBS;

	if (pv)
	{
		variant_copy(pnew, pv);
	}

	return pnew;
}

void variant_free(variant_t* pv)
{
	variant_to_null(pv);

	xmem_free(pv);
}

void variant_to_null(variant_t* pv)
{
	int i;
	int encode;

	switch (pv->vv)
	{
	case VV_STRING:
		xmem_free(pv->string_one);
		break;

	case VV_BOOL_ARRAY:
		xmem_free(pv->bool_ptr);
		break;
	case VV_BYTE_ARRAY:
		xmem_free(pv->byte_ptr);
		break;
	case VV_SHORT_ARRAY:
		xmem_free(pv->short_ptr);
		break;
	case VV_INT_ARRAY:
		xmem_free(pv->int_ptr);
		break;
	case VV_LONG_ARRAY:
		xmem_free(pv->long_ptr);
		break;
	case VV_SCHAR_ARRAY:
		xmem_free(pv->schar_ptr);
		break;
	case VV_WCHAR_ARRAY:
		xmem_free(pv->wchar_ptr);
		break;
	case VV_FLOAT_ARRAY:
		xmem_free(pv->float_ptr);
		break;
	case VV_DOUBLE_ARRAY:
		xmem_free(pv->double_ptr);
		break;
	case VV_STRING_ARRAY:
		for (i = 0; i < pv->size; i++)
		{
			xmem_free(pv->string_ptr[i]);
		}
		xmem_free(pv->string_ptr);
		break;
	}

	encode = pv->encode;
	xmem_zero((void*)pv, sizeof(variant_t));
	pv->encode = encode;
}


bool_t variant_is_null(variant_t* pv)
{
	return (pv->vv == VV_NULL) ? 1 : 0;
}

void variant_copy(variant_t* pv1, const variant_t* pv2)
{
	int i;

	variant_to_null(pv1);

	if (!pv2) return;

	xmem_copy((void*)pv1, (void*)pv2, sizeof(variant_t));

	pv1->vv = (pv2->vv & 0x7F);

	switch (pv1->vv)
	{
	case VV_STRING:
		pv1->string_one = xsclone(pv2->string_one);
		break;
	case VV_BOOL_ARRAY:
		pv1->bool_ptr = (bool_t*)xmem_clone((void*)pv2->bool_ptr, pv2->size * sizeof(bool_t));
		break;
	case VV_BYTE_ARRAY:
		pv1->byte_ptr = (byte_t*)xmem_clone((void*)pv2->byte_ptr, pv2->size * sizeof(byte_t));
		break;
	case VV_SHORT_ARRAY:
		pv1->short_ptr = (short*)xmem_clone((void*)pv2->short_ptr, pv2->size * sizeof(short));
		break;
	case VV_INT_ARRAY:
		pv1->int_ptr = (int*)xmem_clone((void*)pv2->int_ptr, pv2->size * sizeof(int));
		break;
	case VV_LONG_ARRAY:
		pv1->long_ptr = (long long*)xmem_clone((void*)pv2->long_ptr, pv2->size * sizeof(long long));
		break;
	case VV_SCHAR_ARRAY:
		pv1->schar_ptr = (schar_t*)xmem_clone((void*)pv2->schar_ptr, pv2->size * sizeof(schar_t));
		break;
	case VV_WCHAR_ARRAY:
		pv1->wchar_ptr = (wchar_t*)xmem_clone((void*)pv2->wchar_ptr, pv2->size * sizeof(wchar_t));
		break;
	case VV_FLOAT_ARRAY:
		pv1->float_ptr = (float*)xmem_clone((void*)pv2->float_ptr, pv2->size * sizeof(float));
		break;
	case VV_DOUBLE_ARRAY:
		pv1->double_ptr = (double*)xmem_clone((void*)pv2->double_ptr, pv2->size * sizeof(double));
		break;
	case VV_STRING_ARRAY:
		pv1->string_ptr = (tchar_t**)xmem_alloc(pv2->size * sizeof(tchar_t*));
		for (i = 0; i < pv2->size; i++)
		{
			pv1->string_ptr[i] = xsclone(pv2->string_ptr[i]);
		}
		break;
	}
}

int variant_comp(const variant_t* pv1, const variant_t* pv2)
{
	int n1, n2, rt;

	if (pv1->vv == VV_NULL && pv2->vv == VV_NULL)
		return 0;
	else if (pv1->vv == VV_NULL)
		return -1;
	else if (pv2->vv == VV_NULL)
		return 1;

	if ((pv1->vv & 0x7F) != (pv2->vv & 0x7F))
		return (pv1->vv < pv2->vv) ? -1 : 1;

	switch ((pv2->vv & 0x7F))
	{
	case VV_BOOL:
		if (pv1->bool_one > pv2->bool_one)
			return 1;
		else if (pv1->bool_one < pv2->bool_one)
			return -1;
		else
			return 0;
	case VV_BYTE:
		if (pv1->byte_one > pv2->byte_one)
			return 1;
		else if (pv1->byte_one < pv2->byte_one)
			return -1;
		else
			return 0;
	case VV_SCHAR:
		if (pv1->schar_one > pv2->schar_one)
			return 1;
		else if (pv1->schar_one < pv2->schar_one)
			return -1;
		else
			return 0;
	case VV_WCHAR:
		if (pv1->wchar_one > pv2->wchar_one)
			return 1;
		else if (pv1->wchar_one < pv2->wchar_one)
			return -1;
		else
			return 0;
	case VV_SHORT:
		if (pv1->short_one > pv2->short_one)
			return 1;
		else if (pv1->short_one < pv2->short_one)
			return -1;
		else
			return 0;
	case VV_INT:
		if (pv1->int_one > pv2->int_one)
			return 1;
		else if (pv1->int_one < pv2->int_one)
			return -1;
		else
			return 0;
	case VV_LONG:
		if (pv1->long_one > pv2->long_one)
			return 1;
		else if (pv1->long_one < pv2->long_one)
			return -1;
		else
			return 0;
	case VV_FLOAT:
		if (pv1->float_one > pv2->float_one)
			return 1;
		else if (pv1->float_one < pv2->float_one)
			return -1;
		else
			return 0;
	case VV_DOUBLE:
		if (pv1->double_one > pv2->double_one)
			return 1;
		else if (pv1->double_one < pv2->double_one)
			return -1;
		else
			return 0;
	case VV_STRING:
		return compare_text(pv1->string_one, pv1->size, pv2->string_one, pv2->size, 0);
	case VV_BOOL_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->bool_ptr[n1] > pv2->bool_ptr[n2])
				return 1;
			else if (pv1->bool_ptr[n1] < pv2->bool_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_BYTE_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->byte_ptr[n1] > pv2->byte_ptr[n2])
				return 1;
			else if (pv1->byte_ptr[n1] < pv2->byte_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_SHORT_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->short_ptr[n1] > pv2->short_ptr[n2])
				return 1;
			else if (pv1->short_ptr[n1] < pv2->short_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_INT_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->int_ptr[n1] > pv2->int_ptr[n2])
				return 1;
			else if (pv1->int_ptr[n1] < pv2->int_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_LONG_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->long_ptr[n1] > pv2->long_ptr[n2])
				return 1;
			else if (pv1->long_ptr[n1] < pv2->long_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_SCHAR_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->schar_ptr[n1] > pv2->schar_ptr[n2])
				return 1;
			else if (pv1->schar_ptr[n1] < pv2->schar_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_WCHAR_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->wchar_ptr[n1] > pv2->wchar_ptr[n2])
				return 1;
			else if (pv1->wchar_ptr[n1] < pv2->wchar_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_FLOAT_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->float_ptr[n1] > pv2->float_ptr[n2])
				return 1;
			else if (pv1->float_ptr[n1] < pv2->float_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_DOUBLE_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			if (pv1->double_ptr[n1] > pv2->double_ptr[n2])
				return 1;
			else if (pv1->double_ptr[n1] < pv2->double_ptr[n2])
				return -1;

			n1++;
			n2++;
		}
		if (n1 < pv1->size)
			return 1;
		else if (n2 < pv2->size)
			return -1;
		else
			return 0;
	case VV_STRING_ARRAY:
		n1 = n2 = 0;
		while (n1 < pv1->size && n2 < pv2->size)
		{
			rt = compare_text(pv1->string_ptr[n1], -1, pv2->string_ptr[n2], -1, 0);
			if (rt)
				return rt;
			
			n1++;
			n2++;
		}
		return 0;
	}

	return 0;
}

int variant_to_string(variant_t* pv, tchar_t* buf, int max)
{
	int i, n, len;

	switch (pv->vv & 0x7F)
	{
	case VV_NULL:
		return 0;
	case VV_BOOL:
		if (buf)
		{
			buf[0] = (pv->bool_one) ? _T('1') : _T('0');
		}
		return 1;
	case VV_BYTE:
		if (buf)
		{
			buf[0] = pv->byte_one;
		}
		return 1;
	case VV_SCHAR:
		if (buf)
		{
			buf[0] = pv->schar_one;
		}
		return 1;
	case VV_WCHAR:
		if (buf)
		{
			buf[0] = pv->wchar_one;
		}
		return 1;
	case VV_SHORT:
		return stoxs(pv->short_one, buf, max);
	case VV_INT:
		return ltoxs(pv->int_one, buf, max);
	case VV_LONG:
		return lltoxs(pv->long_one, buf, max);
	case VV_FLOAT:
		return ftoxs(pv->float_one, buf, max);
	case VV_DOUBLE:
		return numtoxs(pv->double_one, buf, max);
	case VV_STRING:
		len = pv->size;
		if (len < 0)
			len = xslen(pv->string_one);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, pv->string_one, len);
		}
		return len;
	case VV_BOOL_ARRAY:
		len = 0;
		for (i = 0; i < pv->size; i++)
		{
			if (buf)
			{
				buf[len] = (pv->bool_ptr[i]) ? _T('1') : _T('0');
				buf[len + 1] = _T(' ');
			}
			len += 2;
		}
		return len;
	case VV_BYTE_ARRAY:
		len = 0;
		for (i = 0; i < pv->size; i++)
		{
			n = format_hexnum((unsigned int)(pv->byte_ptr[i]), ((buf)? buf + len : NULL), 2);
			if (buf)
			{
				buf[len + n] = _T(' ');
			}
			len += (n + 1);
		}
		return len;
	case VV_SCHAR_ARRAY:
#ifdef _UNICODE
		return mbs_to_ucs(pv->schar_ptr, pv->size, buf, max);
#else
		len = pv->size;
		if (len < 0)
			len = a_xslen(pv->schar_ptr);
		len = (len < max) ? len : max;
		if (buf)
		{
			a_xsncpy(buf, pv->schar_ptr, len);
		}
		return len;
#endif
	case VV_WCHAR_ARRAY:
		len = pv->size;
		if (len < 0)
			len = w_xslen(pv->wchar_ptr);
#ifdef _UNICODE
		len = (len < max) ? len : max;
		if (buf)
		{
			w_xsncpy(buf, pv->wchar_ptr, len);
		}
		return len;
#else
		return ucs_to_mbs(pv->wchar_ptr, len, buf, max);
#endif
	case VV_SHORT_ARRAY:
		len = 0;
		for (i = 0; i < pv->size; i++)
		{
			n = stoxs(pv->short_ptr[i], ((buf) ? buf + len : NULL), NUM_LEN);
			if (buf)
			{
				buf[len + n] = _T(' ');
			}
			len += (n + 1);
		}
		return len;
	case VV_INT_ARRAY:
		len = 0;
		for (i = 0; i < pv->size; i++)
		{
			n = ltoxs(pv->int_ptr[i], ((buf)? buf + len : NULL), NUM_LEN);
			if (buf)
			{
				buf[len + n] = _T(' ');
			}
			len += (n + 1);
		}
		return len;
	case VV_LONG_ARRAY:
		len = 0;
		for (i = 0; i < pv->size; i++)
		{
			n = format_long(GETHDWORD(pv->long_ptr[i]), GETLDWORD(pv->long_ptr[i]), ((buf) ? buf + len : NULL));
			if (buf)
			{
				buf[len + n] = _T(' ');
			}
			len += (n + 1);
		}
		return len;
	case VV_FLOAT_ARRAY:
		len = 0;
		for (i = 0; i < pv->size; i++)
		{
			n = ftoxs(pv->float_ptr[i], ((buf) ? buf + len : NULL), NUM_LEN);
			if (buf)
			{
				buf[len + n] = _T(' ');
			}
			len += (n + 1);
		}
		return len;
	case VV_DOUBLE_ARRAY:
		len = 0;
		for (i = 0; i < pv->size; i++)
		{
			n = numtoxs(pv->double_ptr[i], ((buf) ? buf + len : NULL), NUM_LEN);
			if (buf)
			{
				buf[len + n] = _T(' ');
			}
			len += (n + 1);
		}
		return len;
	case VV_STRING_ARRAY:
		len = _format_string_array(pv->string_ptr, pv->size, buf, max);
		return len;
	}

	return 0;
}

void variant_from_string(variant_t* pv, const tchar_t* buf, int len)
{
	int i, n, total = 0;
	tchar_t* key;

	switch (pv->vv)
	{
	case VV_NULL:
		break;
	case VV_BOOL:
		pv->bool_one = (buf[0] == _T('1')) ? 1 : 0;
		break;
	case VV_BYTE:
		pv->byte_one = (byte_t)(buf[0]);
		break;
	case VV_SCHAR:
#ifdef _UNICODE
		ucs_byte_to_mbs(buf[0], &(pv->schar_one));
#else
		pv->schar_one = buf[0];
#endif
		break;
	case VV_WCHAR:
#ifdef _UNICODE
		pv->wchar_one = buf[0];
#else
		mbs_byte_to_ucs(buf[0], &(pv->wchar_one));
#endif
		break;
	case VV_SHORT:
		pv->short_one = xsntos(buf, len);
		break;
	case VV_INT:
		pv->int_one = xsntol(buf, len);
		break;
	case VV_LONG:
		pv->long_one = xsntoll(buf, len);
		break;
	case VV_FLOAT:
		pv->float_one = xsntof(buf, len);
		break;
	case VV_DOUBLE:
		pv->double_one = xsntonum(buf, len);
		break;
	case VV_STRING:
		pv->size = (len < 0) ? xslen(buf) : len;
		pv->string_one = xsalloc(pv->size + 1);
		xsncpy(pv->string_one, buf, pv->size);
		break;
	case VV_BOOL_ARRAY:
		pv->size = parse_string_token_count(buf, len, _T(' '));
		pv->bool_ptr = (bool_t*)xmem_alloc(pv->size * sizeof(bool_t));
		i = 0;

		while (n = parse_string_token((buf + total), -1, _T(' '), &key, &n))
		{
			total += n;
			pv->bool_ptr[i] = (key[0] == _T('1')) ? 1 : 0;
		}
		break;
	case VV_BYTE_ARRAY:
		pv->size = parse_string_token_count(buf, len, _T(' '));
		pv->byte_ptr = (byte_t*)xmem_alloc(pv->size);
		i = 0;

		while (n = parse_string_token((buf + total), -1, _T(' '), &key, &n))
		{
			total += n;
			pv->byte_ptr[i] = (byte_t)parse_hexnum(key, 2);
		}
		break;
	case VV_SCHAR_ARRAY:
#ifdef _UNICODE
		pv->size = ucs_to_mbs(buf, len, NULL, MAX_LONG);
		pv->schar_ptr = a_xsalloc(pv->size + 1);
		ucs_to_mbs(buf, len, pv->schar_ptr, pv->size);
#else
		pv->size = (len < 0)? xslen(buf) : len;
		pv->schar_ptr = xsalloc(pv->size + 1);
		xsncpy(pv->schar_ptr, buf, pv->size);
#endif
		break;
	case VV_WCHAR_ARRAY:
#ifdef _UNICODE
		pv->size = (len < 0) ? xslen(buf) : len;
		pv->wchar_ptr = xsalloc(pv->size + 1);
		xsncpy(pv->wchar_ptr, buf, pv->size);
#else
		pv->size = mbs_to_ucs(buf, len, NULL, MAX_LONG);
		pv->wchar_ptr = w_xsalloc(pv->size + 1);
		mbs_to_ucs(buf, len, pv->wchar_ptr, pv->size);
#endif
		break;
	case VV_SHORT_ARRAY:
		pv->size = parse_string_token_count(buf, len, _T(' '));
		pv->short_ptr = (short*)xmem_alloc(pv->size * sizeof(short));
		i = 0;

		while (n = parse_string_token((buf + total), -1, _T(' '), &key, &n))
		{
			total += n;
			pv->short_ptr[i] = xsntos(key, n);
		}
		break;
	case VV_INT_ARRAY:
		pv->size = parse_string_token_count(buf, len, _T(' '));
		pv->int_ptr = (int*)xmem_alloc(pv->size * sizeof(int));
		i = 0;

		while (n = parse_string_token((buf + total), -1, _T(' '), &key, &n))
		{
			total += n;
			pv->int_ptr[i] = xsntol(key, n);
		}
		break;
	case VV_LONG_ARRAY:
		pv->size = parse_string_token_count(buf, len, _T(' '));
		pv->long_ptr = (long long*)xmem_alloc(pv->size * sizeof(long long));
		i = 0;

		while (n = parse_string_token((buf + total), -1, _T(' '), &key, &n))
		{
			total += n;
			pv->long_ptr[i] = xsntoll(key, n);
		}
		break;
	case VV_FLOAT_ARRAY:
		pv->size = parse_string_token_count(buf, len, _T(' '));
		pv->float_ptr = (float*)xmem_alloc(pv->size * sizeof(float));
		i = 0;

		while (n = parse_string_token((buf + total), -1, _T(' '), &key, &n))
		{
			total += n;
			pv->float_ptr[i] = xsntof(key, n);
		}
		break;
	case VV_DOUBLE_ARRAY:
		pv->size = parse_string_token_count(buf, len, _T(' '));
		pv->double_ptr = (double*)xmem_alloc(pv->size * sizeof(double));
		i = 0;

		while (n = parse_string_token((buf + total), -1, _T(' '), &key, &n))
		{
			total += n;
			pv->double_ptr[i] = xsntonum(key, n);
		}
		break;
	case VV_STRING_ARRAY:
		pv->size = _parse_string_array_count(buf);
		pv->string_ptr = (tchar_t**)xmem_alloc(pv->size * sizeof(tchar_t*));
		_parse_string_array(buf, pv->string_ptr, pv->size);
		break;
	}
}

dword_t variant_encode(variant_t* pv, byte_t* buf, dword_t max)
{
	dword_t n = 0;
	tchar_t* str;
	int len;
	short vv;
	int encode;

	len = variant_to_string(pv, NULL, MAX_LONG);
	str = xsalloc(len + 1);
	variant_to_string(pv, str, len);

	encode = (pv->encode) ? pv->encode : DEF_MBS;

	switch (encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = ucs_to_gb2312(str, len, ((buf) ? buf + VARIANT_HDR_SIZE : NULL), max - VARIANT_HDR_SIZE);
#else
		n = mbs_to_gb2312(str, len, ((buf)? buf + VARIANT_HDR_SIZE: NULL), max - VARIANT_HDR_SIZE);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = ucs_to_utf8(str, len, ((buf) ? buf + VARIANT_HDR_SIZE : NULL), max - VARIANT_HDR_SIZE);
#else
		n = mbs_to_utf8(str, len, ((buf)? buf + VARIANT_HDR_SIZE : NULL), max - VARIANT_HDR_SIZE);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = ucs_to_utf16lit(str, len, ((buf) ? buf + VARIANT_HDR_SIZE : NULL), max - VARIANT_HDR_SIZE);
#else
		n = mbs_to_utf16lit(str, len, ((buf)? buf + VARIANT_HDR_SIZE : NULL), max - VARIANT_HDR_SIZE);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = ucs_to_utf16big(str, len, ((buf) ? buf + VARIANT_HDR_SIZE : NULL), max - VARIANT_HDR_SIZE);
#else
		n = mbs_to_utf16big(str, len, ((buf)? buf + VARIANT_HDR_SIZE : NULL), max - VARIANT_HDR_SIZE);
#endif
		break;
	}

	xsfree(str);

	if (buf)
	{
		//variant HDR
		vv = (pv->vv & 0x7F);
		PUT_BYTE(buf, 0, vv);
		PUT_ENCODE(buf, 1, encode);
		PUT_DWORD_LOC(buf, 4, n);
	}

	return (n + VARIANT_HDR_SIZE);
}

dword_t variant_decode(variant_t* pv, const byte_t* buf)
{
	dword_t len;
	int n;
	tchar_t* str;
	int encode;

	if (!buf) return 0;

	if (pv)
	{
		variant_to_null(pv);

		pv->vv = GET_BYTE(buf, 0);
		pv->encode = GET_ENCODE(buf, 1);
	}

	encode = GET_ENCODE(buf, 1);
	len = GET_DWORD_LOC(buf, 4);

	if (pv)
	{
		switch (encode)
		{
		case _GB2312:
#ifdef _UNICODE
			n = gb2312_to_ucs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#else
			n = gb2312_to_mbs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#endif
			break;
		case _UTF8:
#ifdef _UNICODE
			n = utf8_to_ucs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#else
			n = utf8_to_mbs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#endif
			break;
		case _UTF16_LIT:
#ifdef _UNICODE
			n = utf16lit_to_ucs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#else
			n = utf16lit_to_mbs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#endif
			break;
		case _UTF16_BIG:
#ifdef _UNICODE
			n = utf16big_to_ucs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#else
			n = utf16big_to_mbs(buf + VARIANT_HDR_SIZE, len, NULL, MAX_LONG);
#endif
			break;
		}

		str = xsalloc(n + 1);

		switch (encode)
		{
		case _GB2312:
#ifdef _UNICODE
			n = gb2312_to_ucs(buf + VARIANT_HDR_SIZE, len, str, n);
#else
			n = gb2312_to_mbs(buf + VARIANT_HDR_SIZE, len, str, n);
#endif
			break;
		case _UTF8:
#ifdef _UNICODE
			n = utf8_to_ucs(buf + VARIANT_HDR_SIZE, len, str, n);
#else
			n = utf8_to_mbs(buf + VARIANT_HDR_SIZE, len, str, n);
#endif
			break;
		case _UTF16_LIT:
#ifdef _UNICODE
			n = utf16lit_to_ucs(buf + VARIANT_HDR_SIZE, len, str, n);
#else
			n = utf16lit_to_mbs(buf + VARIANT_HDR_SIZE, len, str, n);
#endif
			break;
		case _UTF16_BIG:
#ifdef _UNICODE
			n = utf16big_to_ucs(buf + VARIANT_HDR_SIZE, len, str, n);
#else
			n = utf16big_to_mbs(buf + VARIANT_HDR_SIZE, len, str, n);
#endif
			break;
		}

		variant_from_string(pv, str, n);

		xsfree(str);
	}

	return (len + VARIANT_HDR_SIZE);
}

void variant_hash32(variant_t* pv, key32_t* pkey)
{
	switch (pv->vv & 0x7F)
	{
	case VV_NULL:
		xmem_zero((void*)pkey, sizeof(key32_t));
		break;
	case VV_BOOL:
		murhash32((byte_t*)&pv->bool_one, sizeof(bool_t), (byte_t*)pkey);
		break;
	case VV_BYTE:
		murhash32((byte_t*)&pv->byte_one, 1, (byte_t*)pkey);
		break;
	case VV_SCHAR:
		murhash32((byte_t*)&pv->schar_one, sizeof(schar_t), (byte_t*)pkey);
		break;
	case VV_WCHAR:
		murhash32((byte_t*)&pv->wchar_one, sizeof(wchar_t), (byte_t*)pkey);
		break;
	case VV_SHORT:
		murhash32((byte_t*)&pv->short_one, sizeof(short), (byte_t*)pkey);
		break;
	case VV_INT:
		murhash32((byte_t*)&pv->int_one, sizeof(int), (byte_t*)pkey);
		break;
	case VV_LONG:
		murhash32((byte_t*)&pv->long_one, sizeof(long long), (byte_t*)pkey);
		break;
	case VV_FLOAT:
		murhash32((byte_t*)&pv->float_one, sizeof(float), (byte_t*)pkey);
		break;
	case VV_DOUBLE:
		murhash32((byte_t*)&pv->double_one, sizeof(double), (byte_t*)pkey);
		break;
	case VV_STRING:
		murhash32((byte_t*)pv->string_one, pv->size * sizeof(tchar_t), (byte_t*)pkey);
		break;
	case VV_BOOL_ARRAY:
		murhash32((byte_t*)pv->bool_ptr, pv->size * sizeof(bool_t), (byte_t*)pkey);
		break;
	case VV_BYTE_ARRAY:
		murhash32((byte_t*)pv->byte_ptr, pv->size, (byte_t*)pkey);
		break;
	case VV_SHORT_ARRAY:
		murhash32((byte_t*)pv->short_ptr, pv->size * sizeof(short), (byte_t*)pkey);
		break;
	case VV_INT_ARRAY:
		murhash32((byte_t*)pv->int_ptr, pv->size * sizeof(int), (byte_t*)pkey);
		break;
	case VV_LONG_ARRAY:
		murhash32((byte_t*)pv->long_ptr, pv->size * sizeof(long long), (byte_t*)pkey);
		break;
	case VV_SCHAR_ARRAY:
		murhash32((byte_t*)pv->schar_ptr, pv->size * sizeof(schar_t), (byte_t*)pkey);
		break;
	case VV_WCHAR_ARRAY:
		murhash32((byte_t*)pv->wchar_ptr, pv->size * sizeof(wchar_t), (byte_t*)pkey);
		break;
	case VV_FLOAT_ARRAY:
		murhash32((byte_t*)pv->float_ptr, pv->size * sizeof(float), (byte_t*)pkey);
		break;
	case VV_DOUBLE_ARRAY:
		murhash32((byte_t*)pv->double_ptr, pv->size * sizeof(double), (byte_t*)pkey);
		break;
	}
}

void variant_hash64(variant_t* pv, key64_t* pkey)
{
	switch (pv->vv & 0x7F)
	{
	case VV_NULL:
		xmem_zero((void*)pkey, sizeof(key64_t));
		break;
	case VV_BOOL:
		siphash64((byte_t*)&pv->bool_one, sizeof(bool_t), (byte_t*)pkey);
		break;
	case VV_BYTE:
		siphash64((byte_t*)&pv->byte_one, 1, (byte_t*)pkey);
		break;
	case VV_SCHAR:
		siphash64((byte_t*)&pv->schar_one, sizeof(schar_t), (byte_t*)pkey);
		break;
	case VV_WCHAR:
		siphash64((byte_t*)&pv->wchar_one, sizeof(wchar_t), (byte_t*)pkey);
		break;
	case VV_SHORT:
		siphash64((byte_t*)&pv->short_one, sizeof(short), (byte_t*)pkey);
		break;
	case VV_INT:
		siphash64((byte_t*)&pv->int_one, sizeof(int), (byte_t*)pkey);
		break;
	case VV_LONG:
		siphash64((byte_t*)&pv->long_one, sizeof(long long), (byte_t*)pkey);
		break;
	case VV_FLOAT:
		siphash64((byte_t*)&pv->float_one, sizeof(float), (byte_t*)pkey);
		break;
	case VV_DOUBLE:
		siphash64((byte_t*)&pv->double_one, sizeof(double), (byte_t*)pkey);
		break;
	case VV_STRING:
		siphash64((byte_t*)pv->string_one, pv->size * sizeof(tchar_t), (byte_t*)pkey);
		break;
	case VV_BOOL_ARRAY:
		siphash64((byte_t*)pv->bool_ptr, pv->size * sizeof(bool_t), (byte_t*)pkey);
		break;
	case VV_BYTE_ARRAY:
		siphash64((byte_t*)pv->byte_ptr, pv->size, (byte_t*)pkey);
		break;
	case VV_INT_ARRAY:
		siphash64((byte_t*)pv->int_ptr, pv->size * sizeof(int), (byte_t*)pkey);
		break;
	case VV_LONG_ARRAY:
		siphash64((byte_t*)pv->long_ptr, pv->size * sizeof(long long), (byte_t*)pkey);
		break;
	case VV_SCHAR_ARRAY:
		siphash64((byte_t*)pv->schar_ptr, pv->size * sizeof(schar_t), (byte_t*)pkey);
		break;
	case VV_WCHAR_ARRAY:
		siphash64((byte_t*)pv->wchar_ptr, pv->size * sizeof(wchar_t), (byte_t*)pkey);
		break;
	case VV_FLOAT_ARRAY:
		siphash64((byte_t*)pv->float_ptr, pv->size * sizeof(float), (byte_t*)pkey);
		break;
	case VV_DOUBLE_ARRAY:
		siphash64((byte_t*)pv->double_ptr, pv->size * sizeof(double), (byte_t*)pkey);
		break;
	}
}

void variant_hash128(variant_t* pv, key128_t* pkey)
{
	switch (pv->vv & 0x7F)
	{
	case VV_NULL:
		xmem_zero((void*)pkey, sizeof(key128_t));
		break;
	case VV_BOOL:
		murhash128((byte_t*)&pv->bool_one, sizeof(bool_t), (byte_t*)pkey);
		break;
	case VV_BYTE:
		murhash128((byte_t*)&pv->byte_one, 1, (byte_t*)pkey);
		break;
	case VV_SCHAR:
		murhash128((byte_t*)&pv->schar_one, sizeof(schar_t), (byte_t*)pkey);
		break;
	case VV_WCHAR:
		murhash128((byte_t*)&pv->wchar_one, sizeof(wchar_t), (byte_t*)pkey);
		break;
	case VV_SHORT:
		murhash128((byte_t*)&pv->short_one, sizeof(short), (byte_t*)pkey);
		break;
	case VV_INT:
		murhash128((byte_t*)&pv->int_one, sizeof(int), (byte_t*)pkey);
		break;
	case VV_LONG:
		murhash128((byte_t*)&pv->long_one, sizeof(long long), (byte_t*)pkey);
		break;
	case VV_FLOAT:
		murhash128((byte_t*)&pv->float_one, sizeof(float), (byte_t*)pkey);
		break;
	case VV_DOUBLE:
		murhash128((byte_t*)&pv->double_one, sizeof(double), (byte_t*)pkey);
		break;
	case VV_STRING:
		murhash128((byte_t*)pv->string_one, pv->size * sizeof(tchar_t), (byte_t*)pkey);
		break;
	case VV_BOOL_ARRAY:
		murhash128((byte_t*)pv->bool_ptr, pv->size * sizeof(bool_t), (byte_t*)pkey);
		break;
	case VV_BYTE_ARRAY:
		murhash128((byte_t*)pv->byte_ptr, pv->size, (byte_t*)pkey);
		break;
	case VV_INT_ARRAY:
		murhash128((byte_t*)pv->int_ptr, pv->size * sizeof(int), (byte_t*)pkey);
		break;
	case VV_LONG_ARRAY:
		murhash128((byte_t*)pv->long_ptr, pv->size * sizeof(long long), (byte_t*)pkey);
		break;
	case VV_SCHAR_ARRAY:
		murhash128((byte_t*)pv->schar_ptr, pv->size * sizeof(schar_t), (byte_t*)pkey);
		break;
	case VV_WCHAR_ARRAY:
		murhash128((byte_t*)pv->wchar_ptr, pv->size * sizeof(wchar_t), (byte_t*)pkey);
		break;
	case VV_FLOAT_ARRAY:
		murhash128((byte_t*)pv->float_ptr, pv->size * sizeof(float), (byte_t*)pkey);
		break;
	case VV_DOUBLE_ARRAY:
		murhash128((byte_t*)pv->double_ptr, pv->size * sizeof(double), (byte_t*)pkey);
		break;
	}
}

#if defined(_DEBUG) || defined(DEBUG)
void test_variant(void)
{
	variant_t v1 = { 0 };
	variant_t v2 = { 0 };

	XDL_ASSERT(variant_comp(&v1, &v2) == 0);

	v1.vv = VV_SCHAR_ARRAY;
	v1.encode = DEF_MBS;
	variant_from_string(&v1, _T("test1"), -1);
	v2.vv = VV_SCHAR_ARRAY;
	v2.encode = DEF_MBS;
	variant_from_string(&v2, _T("test2"), -1);

	XDL_ASSERT(variant_comp(&v1, &v2) < 0);

	variant_to_null(&v1);
	variant_to_null(&v2);

	v1.vv = VV_INT;
	variant_from_string(&v1, _T("123456789"), -1);

	tchar_t token[NUM_LEN];
	variant_to_string(&v1, token, NUM_LEN);

	v2.vv = VV_INT;
	variant_from_string(&v2, token, -1);

	XDL_ASSERT(variant_comp(&v1, &v2) == 0);

	byte_t* buf;
	dword_t len;
	len = variant_encode(&v1, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(len);
	variant_encode(&v1, buf, len);

	len = variant_decode(NULL, buf);
	variant_decode(&v2, buf);

	XDL_ASSERT(variant_comp(&v1, &v2) == 0);

	xmem_free(buf);

	variant_to_null(&v1);
	variant_to_null(&v2);
}
#endif