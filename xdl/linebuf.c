/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc linear buffer document

	@module	linebuf.c | implement file

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

#include "linebuf.h"

#include "xdlimp.h"

#define BUFFERTAG_ROOT		0xFFFFFFFF

static linear_t_ptr _alloc_linear(sword_t size)
{
	linear_t_ptr pbuf;

	pbuf = (linear_t_ptr)xmem_alloc(sizeof(linear_t));
	pbuf->data = (byte_t*)xmem_alloc(size);
	pbuf->readed = 0;
	pbuf->writed = 0;

	return pbuf;
}

static void _free_linear(linear_t_ptr link)
{
	if (link->data)
		xmem_free(link->data);

	xmem_free(link);
}

void linear_init(linear_t_ptr root, sword_t frag)
{
	root->flag = BUFFERTAG_ROOT;
	root->frag = frag;
	root->next = root;
	root->prev = root;
}

linear_t_ptr linear_alloc(linear_t_ptr root)
{
	linear_t_ptr link;

	XDL_ASSERT(root->flag == BUFFERTAG_ROOT);

	link = _alloc_linear(root->frag);

	root->prev->next = link;
	link->prev = root->prev;
	root->prev = link;
	link->next = root;

	return link;
}

void linear_free(linear_t_ptr root, linear_t_ptr link)
{
	XDL_ASSERT(root->flag == BUFFERTAG_ROOT);

	link->prev->next = link->next;
	link->next->prev = link->prev;

	link->next = link->prev = NULL;
	_free_linear(link);
}

linear_t_ptr linear_get_first(linear_t_ptr root)
{
	linear_t_ptr pbuf;
	XDL_ASSERT(root->flag == BUFFERTAG_ROOT);

	pbuf = root->next;
	return (pbuf->flag == BUFFERTAG_ROOT) ? NULL : pbuf;
}

linear_t_ptr linear_get_last(linear_t_ptr root)
{
	linear_t_ptr pbuf;
	XDL_ASSERT(root->flag == BUFFERTAG_ROOT);

	pbuf = root->prev;
	return (pbuf->flag == BUFFERTAG_ROOT) ? NULL : pbuf;
}

linear_t_ptr linear_get_next(linear_t_ptr link)
{
	linear_t_ptr pbuf;
	XDL_ASSERT(link->flag != BUFFERTAG_ROOT);

	pbuf = link->next;
	return (pbuf->flag == BUFFERTAG_ROOT) ? NULL : pbuf;
}

linear_t_ptr linear_get_prev(linear_t_ptr link)
{
	linear_t_ptr pbuf;
	XDL_ASSERT(link->flag != BUFFERTAG_ROOT);

	pbuf = link->prev;
	return (pbuf->flag == BUFFERTAG_ROOT) ? NULL : pbuf;
}

dword_t linear_read(linear_t_ptr root, byte_t* buf, dword_t max)
{
	linear_t_ptr pbuf;
	dword_t pos = 0;
	sword_t blk;

	pbuf = linear_get_first(root);
	while (pbuf && pos < max)
	{
		blk = (pbuf->writed - pbuf->readed < max) ? (pbuf->writed - pbuf->readed) : max;
		if (blk)
		{
			xmem_copy((void*)(buf + pos), (void*)(pbuf->data + pbuf->readed), blk);
		}
		pos += blk;
		pbuf->readed += blk;

		if (pbuf->writed == pbuf->readed)
		{
			pbuf = linear_get_next(pbuf);
		}
	}

	return pos;
}

dword_t linear_write(linear_t_ptr root, const byte_t* buf, dword_t len)
{
	linear_t_ptr pbuf;
	dword_t pos = 0;
	sword_t blk;

	pbuf = linear_get_last(root);
	while (pos < len)
	{
		if (!pbuf)
		{
			pbuf = linear_alloc(root);
		}

		blk = (root->frag - pbuf->writed < len) ? (root->frag - pbuf->writed) : len;
		if (blk)
		{
			xmem_copy((void*)(pbuf->data + pbuf->writed), (void*)(buf + pos), blk);
		}
		pos += blk;
		pbuf->writed += blk;

		if (root->frag == pbuf->writed)
		{
			pbuf = NULL;
		}
	}

	return pos;
}