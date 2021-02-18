/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc integerarray document

	@module	integerarray.c | implement file

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

#include "integerarray.h"

#include "xdlimp.h"



int** alloc_integer_array(void)
{
	int** sa;

	sa = (int**)xmem_alloc(2 * sizeof(int*));

	return sa;
}

void free_integer_array(int** sa)
{
	xmem_free(*sa);

	xmem_free(sa);
}

void clear_integer_array(int** sa)
{
	xmem_free(*sa);

	xmem_zero((void*)sa, 2 * sizeof(int*));
}

int get_integer_array_size(int** sa)
{
	return (int)(*(long*)(sa + 1));
}

int get_integer(int** sa, int index)
{
	int* pa = *sa;
	int size;

	size = (int)(*(long*)(sa + 1));

	if (index < 0 || index >= size)
		return MAX_LONG;

	return pa[index];
}

void insert_integer(int** sa, int index, int val)
{
	int size;

	size = (int)(*(long*)(sa + 1));

	XDL_ASSERT(index >= 0 && index <= size);

	*sa = xmem_realloc(*sa, (size + 1) * sizeof(int));

	xmem_move((void*)(*sa + index), ((size - index) * sizeof(int)), (int)sizeof(int));
	xmem_copy((void*)(*sa + index), (void*)&val, sizeof(int));

	*(long*)(sa + 1) = (size + 1);
}

void delete_integer(int** sa, int index)
{
	int size;

	size = (int)(*(long*)(sa + 1));

	XDL_ASSERT(index >= 0 && index < size);

	xmem_move((void*)(*sa + index + 1), ((size - index - 1) * sizeof(int)), -(int)sizeof(int));

	*sa = xmem_realloc(*sa, (size - 1) * sizeof(int));

	*(long*)(sa + 1) = (size - 1);
}


#if defined(XDL_SUPPORT_TEST)

void test_integer_array()
{
	int** sa = alloc_integer_array();
	int i;

	for (i = 0; i < 10; i++)
	{
		insert_integer(sa, i, i);
	}

	for (i = 0; i < 10; i++)
	{
		_tprintf(_T("%d\n"), get_integer(sa, i));
	}

	while (get_integer_array_size(sa))
	{
		delete_integer(sa, 0);
	}

	free_integer_array(sa);
}

#endif