/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc buffer document

	@module	buffer.c | buffer implement file

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

#include "varbuf.h"
#include "xdlimp.h"

byte_t** varbuf_alloc()
{
	return (byte_t**)xmem_alloc(sizeof(byte_t*));
}

void varbuf_free(byte_t** pp)
{
	xmem_free(*pp);
	xmem_free(pp);
}

byte_t* varbuf_realloc(byte_t** pp, dword_t size)
{
	*pp = xmem_realloc(*pp, size);

	return *pp;
}

byte_t** varbuf_attach(byte_t* p)
{
	byte_t** pp;

	pp = xmem_alloc(sizeof(byte_t*));
	*pp = p;

	return (byte_t**)pp;
}

byte_t* varbuf_detach(byte_t** pp)
{
	byte_t* p;

	p = *pp;

	xmem_free(pp);

	return p;
}