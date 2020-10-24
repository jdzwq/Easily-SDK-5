/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc stringarray document

	@module	stringarray.c | implement file

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

#include "stringarray.h"

#include "xdlimp.h"


tchar_t** alloc_string_array(void)
{
	tchar_t** ptr;

	ptr = (tchar_t**)xmem_alloc(sizeof(tchar_t*) + sizeof(var_long));

	return ptr;
}

void free_string_array(tchar_t** sa)
{
	xsfree(*sa);

	xmem_free(sa);
}

void clear_string_array(tchar_t** sa)
{
	xsfree(*sa);

	xmem_zero((void*)sa, sizeof(tchar_t*) + sizeof(var_long));
}

int get_string_array_size(tchar_t** sa)
{
	tchar_t* token = *sa;
	int n = 0;

	while (*token != _T('\0'))
	{
		token += (xslen(token) + 1);
		n++;
	}

	return n;
}

const tchar_t* get_string_ptr(tchar_t** sa, int index)
{
	tchar_t* token = *sa;

	while (token && index)
	{
		token += (xslen(token) + 1);
		index--;

		if (*token == _T('\0'))
			break;
	}

	return token;
}

int get_string(tchar_t** sa, int index, tchar_t* buf, int max)
{
	tchar_t* token = *sa;
	int n;

	while (token && index)
	{
		token += (xslen(token) + 1);
		index--;

		if (*token == _T('\0'))
			break;
	}

	n = xslen(token);
	n = (n < max) ? n : max;
	
	if (buf)
	{
		xsncpy(buf, token, n);
	}

	return n;
}

void insert_string(tchar_t** sa, int index, const tchar_t* tk, int len)
{
	tchar_t* token = *sa;
	int k, n = 0;
	var_long size;

	if (len < 0)
		len = xslen(tk);

	if (!len)
		return;

	while (token && index)
	{
		k = xslen(token);
		token += (k + 1);
		index--;
		n += (k + 1);

		if (*token == _T('\0'))
			break;
	}

	size = *(var_long*)(sa + 1);
	k = (len + 1) * sizeof(tchar_t);

	*sa = xmem_realloc(*sa, (size + k + sizeof(tchar_t)));
	xmem_zero(((byte_t*)(*sa) + size + k), sizeof(tchar_t));

	xmem_move((void*)(*sa + n), (size - n * sizeof(tchar_t)), k);
	xmem_copy((void*)(*sa + n), (void*)tk, (k - sizeof(tchar_t)));

	*(var_long*)(sa + 1) = (size + k);
}

void delete_string(tchar_t** sa, int index)
{
	tchar_t* token = *sa;
	int k, n = 0;
	var_long size;

	if (!token)
		return;

	while (token && index)
	{
		k = xslen(token);
		token += (k + 1);
		index--;
		n += (k + 1);

		if (*token == _T('\0'))
			return;
	}

	size = *(var_long*)(sa + 1);
	k = (xslen(token) + 1) * sizeof(tchar_t);

	xmem_move((void*)(*sa + n + k / sizeof(tchar_t)), (size - n * sizeof(tchar_t) - k), -k);
	*sa = xmem_realloc((void*)(*sa), (size - k + sizeof(tchar_t)));
	xmem_zero(((byte_t*)(*sa) + size - k), sizeof(tchar_t));

	*(var_long*)(sa + 1) = (size - k);
}

#if defined(_DEBUG) || defined(DEBUG)

void test_string_array()
{
	tchar_t token[10];

	tchar_t** sa = alloc_string_array();

	for (int i = 0; i < 10; i++)
	{
		xsprintf(token, _T("token%d"), i);
		insert_string(sa, i, token, -1);
	}

	for (int i = 0; i < 10; i++)
	{
		_tprintf(_T("%s\n"), get_string_ptr(sa, i));
	}

	while (get_string_array_size(sa))
	{
		delete_string(sa, 0);
	}

	free_string_array(sa);
}

#endif