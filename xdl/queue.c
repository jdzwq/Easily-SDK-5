/***********************************************************************
	Easily xdl v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc queue document

	@module	queue.c | queue document implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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

#include "queue.h"
#include "varbytes.h"
#include "message.h"
#include "miscella.h"

#include "xdlimp.h"

#include "xdlinit.h"

/*
struct{
	struct{
		byte[1]: tag
		byte[3]: len. total message block size
	}memobj_head
	message_block[]: n message block
}queue
*/

#define QUEUE_HEAD_SIZE		(sizeof(memobj_head))

#define QUEUE_GET_TYPE(que)		GET_BYTE((que),0)
#define QUEUE_SET_TYPE(que,n)	PUT_BYTE((que),0,n)

#define QUEUE_SET_SIZE(que,n)	PUT_THREEBYTE_LOC((que),1,n)
#define QUEUE_GET_SIZE(que)		(GET_THREEBYTE_LOC((que),1))

#define QUEUE_GET_BUF(que)		(que + QUEUE_HEAD_SIZE)

queue_t queue_alloc()
{
	byte_t** pq;

	pq = bytes_alloc();
	bytes_realloc(pq, QUEUE_HEAD_SIZE);

	QUEUE_SET_TYPE(*pq, MEM_QUEUE);
	QUEUE_SET_SIZE(*pq, 0);

	return (queue_t)pq;
}

void queue_free(queue_t que)
{
	byte_t** pq = (byte_t**)que;
	byte_t type;

	XDL_ASSERT(que != NULL);

	type = QUEUE_GET_TYPE(*pq);

	XDL_ASSERT(type == MEM_QUEUE);

	bytes_free(pq);
}

dword_t queue_size(queue_t que)
{
	byte_t** pq = (byte_t**)que;
	dword_t size;
	byte_t type;

	XDL_ASSERT(que != NULL);

	type = QUEUE_GET_TYPE(*pq);

	XDL_ASSERT(type == MEM_QUEUE);

	size = QUEUE_GET_SIZE(*pq);

	return size;
}

void queue_copy(queue_t dst, queue_t src)
{
	byte_t** psrc = (byte_t**)src;
	byte_t** pdst = (byte_t**)dst;

	byte_t t1, t2;

	t1 = QUEUE_GET_TYPE(*psrc);
	t2 = QUEUE_GET_TYPE(*pdst);

	XDL_ASSERT(src != NULL && t1 == MEM_QUEUE);
	XDL_ASSERT(dst != NULL && t2 == MEM_QUEUE);

	bytes_realloc(pdst, QUEUE_GET_SIZE(*psrc) + QUEUE_HEAD_SIZE);
	xmem_copy((void*)(*pdst), (void*)(*psrc), QUEUE_GET_SIZE(*psrc) + QUEUE_HEAD_SIZE);
}

dword_t queue_write(queue_t que, message_t msg)
{
	byte_t** pq = (byte_t**)que;
	msg_hdr_t hdr_org, hdr_cur = { 0 };
	dword_t len_msg, len_que, total;
	message_t msg_org;

	message_read(msg, &hdr_cur, NULL, MAX_LONG);

	if (!hdr_cur.utc)
		hdr_cur.utc = get_timestamp();
	
	len_msg = MEMOBJ_SIZE((*msg));
	len_que = MEMOBJ_SIZE((*que));

	bytes_realloc(pq, len_que + len_msg);

	msg_org = message_alloc();

	len_que -= QUEUE_HEAD_SIZE;
	total = 0;
	while (total < len_que)
	{
		message_borrow(msg_org, (QUEUE_GET_BUF(*pq) + total));

		if ((*msg_org)->tag != MEM_MESSAGE)
		{
			message_revert(msg_org);
			break;
		}

		message_read(msg_org, &hdr_org, NULL, MAX_LONG);

		if (hdr_cur.utc < hdr_org.utc)
		{
			message_revert(msg_org);
			break;
		}

		total += MEMOBJ_SIZE((*msg_org));
		message_revert(msg_org);
	}

	message_free(msg_org);

	if (total < len_que)
	{
		xmem_move((void*)(QUEUE_GET_BUF(*pq) + total), (len_que - total), len_msg);
		xmem_zero((void*)(QUEUE_GET_BUF(*pq) + total), len_msg);
	}

	xmem_copy((void*)(QUEUE_GET_BUF(*pq) + total), (void*)(*((byte_t**)msg)), len_msg);
	total += len_msg;

	QUEUE_SET_SIZE(*pq, total);

	return len_msg;
}

dword_t queue_read(queue_t que, message_t msg)
{
	byte_t** pq = (byte_t**)que;
	message_t org;
	dword_t len, total = 0;

	total = QUEUE_GET_SIZE(*pq);
	if (total)
	{
		org = message_alloc();

		message_borrow(org, QUEUE_GET_BUF(*pq));
		message_copy(msg, org);
		len = MEMOBJ_SIZE((*org));
		message_revert(org);

		xmem_move((void*)(QUEUE_GET_BUF(*pq) + len), (total - len), (0-len));
		total -= len;

		bytes_realloc(pq, total + QUEUE_HEAD_SIZE);

		QUEUE_SET_SIZE(*pq, total);

		message_free(org);
	}
	else
	{
		len = 0;
	}

	return len;
}

dword_t queue_peek(queue_t que, message_t msg)
{
	byte_t** pq = (byte_t**)que;
	message_t org;
	dword_t len, total = 0;

	total = QUEUE_GET_SIZE(*pq);
	if (total)
	{
		org = message_alloc();

		message_borrow(org, QUEUE_GET_BUF(*pq));
		message_copy(msg, org);
		len = MEMOBJ_SIZE((*org));
		message_revert(org);

		message_free(org);
	}
	else
	{
		len = 0;
	}

	return len;
}

dword_t queue_decode(queue_t que, const byte_t* data)
{
	byte_t** pq = (byte_t**)que;
	byte_t type;
	dword_t len;

	type = QUEUE_GET_TYPE(data);

	XDL_ASSERT(type == MEM_QUEUE);

	len = QUEUE_HEAD_SIZE + QUEUE_GET_SIZE(data);

	if (pq)
	{
		bytes_realloc(pq, len);
		xmem_copy((void*)(*pq), (void*)data, len);
	}

	return len;
}

dword_t queue_encode(queue_t que, byte_t* buf, dword_t max)
{
	byte_t** pq = (byte_t**)que;
	dword_t len;
	byte_t type;

	XDL_ASSERT(que != NULL);

	type = QUEUE_GET_TYPE(*pq);

	XDL_ASSERT(type == MEM_QUEUE);

	len = QUEUE_HEAD_SIZE + QUEUE_GET_SIZE(*pq);
	len = (len < max) ? len : max;

	if (buf)
	{
		xmem_copy((void*)buf, (void*)(*pq), len);
	}

	return len;
}

#if defined(XDL_SUPPORT_TEST)
void test_queue(void)
{
	byte_t buf[] = "hello world!";

	msg_hdr_t hdr = { 0 };

	message_t msg = message_alloc();

	hdr.ver = MSGVER_SENSOR;
	hdr.qos = 0x02;

	byte_t tmp[100] = { 0 };

	queue_t que = queue_alloc();

	for (int i = 0; i < 100; i++)
	{
		hdr.seq = i;
		hdr.utc = get_timestamp();

		message_write(msg, &hdr, buf, a_xslen((schar_t*)buf));

		queue_write(que, msg);

		printf("write: ver:0x%08x qos:%c seq:%d utc:%llu msg:%s\n", hdr.ver, hdr.qos, hdr.seq, hdr.utc, buf);
	}

	while (queue_read(que, msg))
	{
		message_read(msg, &hdr, tmp, 100);

		printf("read: ver:0x%08x qos:%c seq:%d utc:%llu msg:%s\n", hdr.ver, hdr.qos, hdr.seq, hdr.utc, tmp);
	}

	message_free(msg);

	queue_free(que);
}
#endif