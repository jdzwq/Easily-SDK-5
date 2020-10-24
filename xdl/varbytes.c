/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant bytes document

	@module	varbytes.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is free software : you can radistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "varbytes.h"

#include "xdlimp.h"

typedef struct _bytes_t{
	byte_t* memo;
	dword_t size;
}bytes_t;

byte_t** bytes_alloc()
{
	bytes_t* pb;

	pb = (bytes_t*)xmem_alloc(sizeof(bytes_t));

	return &(pb->memo);
}

void bytes_free(byte_t** pp)
{
	bytes_t* pb;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	xmem_free(pb->memo);
	xmem_free(pp);
}

byte_t* bytes_realloc(byte_t** pp, dword_t size)
{
	bytes_t* pb;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	pb->memo = (byte_t*)xmem_realloc(pb->memo, size);
	pb->size = size;

	return pb->memo;
}

void bytes_attach(byte_t** pp, byte_t* p, dword_t size)
{
	bytes_t* pb;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	xmem_free(pb->memo);

	pb->memo = p;
	pb->size = size;
}

byte_t* bytes_detach(byte_t** pp)
{
	bytes_t* pb;
	byte_t* p;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	p = pb->memo;

	pb->memo = NULL;
	pb->size = 0;

	return p;
}

dword_t bytes_size(byte_t** pp)
{
	bytes_t* pb;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	return pb->size;
}

void bytes_copy(byte_t** pp, dword_t pos, byte_t* sub, dword_t* plen)
{
	bytes_t* pb;
	dword_t n;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	XDL_ASSERT(pos <= pb->size);

	n = (*plen < (pb->size - pos)) ? (*plen) : (pb->size - pos);
	xmem_copy((void*)sub, (void*)(pb->memo + pos), n);
	*plen = n;
}

void bytes_insert(byte_t** pp, dword_t pos, const byte_t* sub, dword_t len)
{
	bytes_t* pb;
	byte_t* p;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	XDL_ASSERT(pos <= pb->size);

	pb->memo = (byte_t*)xmem_realloc(pb->memo, pb->size + len);

	p = pb->memo + pos;
	xmem_move((void*)(p), pb->size - pos, len);
	xmem_copy((void*)(p), (void*)sub, len);
	pb->size += len;
}

void bytes_delete(byte_t** pp, dword_t pos, dword_t len)
{
	bytes_t* pb;
	byte_t* p;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	XDL_ASSERT(pos <= pb->size);

	len = (len < pb->size - pos) ? len : pb->size - pos;
	p = pb->memo + pos + len;
	xmem_move((void*)(p), pb->size - pos, -(int)len);
	p = pb->memo + pb->size - len;
	xmem_set((void*)(p), 0, len);
	pb->size -= len;
}

void bytes_replace(byte_t** pp, dword_t pos, dword_t num, const byte_t* sub, dword_t len)
{
	bytes_t* pb;
	byte_t* p;

	XDL_ASSERT(pp != NULL);

	pb = (bytes_t*)(pp);

	XDL_ASSERT(pos + num <= pb->size);

	if (len > num)
	{
		pb->memo = (byte_t*)xmem_realloc(pb->memo, pb->size + len - num);
	}

	p = pb->memo + pos + num;
	xmem_move((void*)(p), pb->size - (pos + num), (int)len - (int)num);
	if (len < num)
	{
		p = pb->memo + pb->size - (num - len);
		xmem_set((void*)(p), 0, num - len);
	}
	p = pb->memo + pos;
	xmem_copy((void*)(p), (void*)sub, len);
	pb->size += (len - num);
}