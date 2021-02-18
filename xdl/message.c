/***********************************************************************
	Easily xdl v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc message document

	@module	varmsg.c | message document implement file

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

#include "message.h"
#include "varbytes.h"
#include "miscella.h"

#include "xdlimp.h"

#include "xdlinit.h"

/*
struct{
	struct{
		byte[1]: tag
		byte[3]: len = 12 + msglen
	}memobj_head
	byte[4]: ver
	byte[1]: qos
	byte[3]: seq
	byte[8]: utc
	byte[msglen]: msg
}message
*/

#define MESSAGE_HEAD_SIZE		(sizeof(memobj_head) + 16)

#define MESSAGE_GET_TYPE(msg)	GET_BYTE((msg),0)
#define MESSAGE_SET_TYPE(msg,n)	PUT_BYTE((msg),0,n)

#define MESSAGE_SET_SIZE(msg,n)	PUT_THREEBYTE_LOC((msg),1,(n + 16))
#define MESSAGE_GET_SIZE(msg)	(GET_THREEBYTE_LOC((msg),1) - 16)

#define MESSAGE_SET_VER(msg,n)	PUT_DWORD_LOC((msg),(sizeof(memobj_head)),n)
#define MESSAGE_GET_VER(msg)	GET_DWORD_LOC((msg),(sizeof(memobj_head)))

#define MESSAGE_GET_QOS(msg)	GET_BYTE((msg), (sizeof(memobj_head) + 4))
#define MESSAGE_SET_QOS(msg, b)	PUT_BYTE((msg),(sizeof(memobj_head) + 4),(byte_t)b)

#define MESSAGE_SET_SEQ(msg,n)	PUT_THREEBYTE_LOC((msg),(sizeof(memobj_head) + 5),n)
#define MESSAGE_GET_SEQ(msg)	GET_THREEBYTE_LOC((msg),(sizeof(memobj_head) + 5))

#define MESSAGE_SET_UTC(msg,n)	PUT_LWORD_LOC((msg),(sizeof(memobj_head) + 8),n)
#define MESSAGE_GET_UTC(msg)	GET_LWORD_LOC((msg),(sizeof(memobj_head) + 8))

#define MESSAGE_GET_BUF(msg)		(msg + MESSAGE_HEAD_SIZE)

message_t message_alloc()
{
	byte_t** pmsg;

	pmsg = bytes_alloc();
	bytes_realloc(pmsg, MESSAGE_HEAD_SIZE);

	MESSAGE_SET_TYPE(*pmsg, MEM_MESSAGE);
	MESSAGE_SET_SIZE(*pmsg, 0);
	MESSAGE_SET_VER(*pmsg, 0);
	MESSAGE_SET_QOS(*pmsg, 0);
	MESSAGE_SET_SEQ(*pmsg, 0);
	MESSAGE_SET_UTC(*pmsg, 0);

	return (message_t)pmsg;
}

void message_free(message_t msg)
{
	byte_t** pmsg = (byte_t**)msg;
	byte_t type;

	XDL_ASSERT(msg != NULL);

	type = MESSAGE_GET_TYPE(*pmsg);

	XDL_ASSERT(type == MEM_MESSAGE);

	bytes_free(pmsg);
}

dword_t message_size(message_t msg)
{
	byte_t** pmsg = (byte_t**)msg;
	dword_t size;
	byte_t type;

	XDL_ASSERT(msg != NULL);

	type = MESSAGE_GET_TYPE(*pmsg);

	XDL_ASSERT(type == MEM_MESSAGE);

	size = MESSAGE_GET_SIZE(*pmsg);

	return size;
}

void message_copy(message_t dst, message_t src)
{
	byte_t** psrc = (byte_t**)src;
	byte_t** pdst = (byte_t**)dst;

	byte_t t1,t2;

	t1 = MESSAGE_GET_TYPE(*psrc);
	t2 = MESSAGE_GET_TYPE(*pdst);

	XDL_ASSERT(src != NULL && t1 == MEM_MESSAGE);
	XDL_ASSERT(dst != NULL && t2 == MEM_MESSAGE);

	bytes_realloc(pdst, MESSAGE_GET_SIZE(*psrc) + MESSAGE_HEAD_SIZE);
	xmem_copy((void*)(*pdst), (void*)(*psrc), MESSAGE_GET_SIZE(*psrc) + MESSAGE_HEAD_SIZE);
}

void message_borrow(message_t msg, byte_t* buf)
{
	byte_t** pmsg = (byte_t**)msg;
	byte_t type;

	XDL_ASSERT(msg != NULL);

	type = MESSAGE_GET_TYPE(*pmsg);

	XDL_ASSERT(type == MEM_MESSAGE);

	bytes_attach(pmsg, buf, 0);
}

byte_t* message_revert(message_t msg)
{
	byte_t** pmsg = (byte_t**)msg;
	byte_t type;
	byte_t* buf;

	XDL_ASSERT(msg != NULL);

	type = MESSAGE_GET_TYPE(*pmsg);

	XDL_ASSERT(type == MEM_MESSAGE);

	buf = bytes_detach(pmsg);

	bytes_realloc(pmsg, MESSAGE_HEAD_SIZE);

	MESSAGE_SET_TYPE(*pmsg, MEM_MESSAGE);
	MESSAGE_SET_SIZE(*pmsg, 0);
	MESSAGE_SET_VER(*pmsg, 0);
	MESSAGE_SET_QOS(*pmsg, 0);
	MESSAGE_SET_SEQ(*pmsg, 0);
	MESSAGE_SET_UTC(*pmsg, 0);

	return buf;
}

dword_t message_write(message_t msg, const msg_hdr_t* phr, const byte_t* buf, dword_t len)
{
	byte_t** pmsg = (byte_t**)msg;
	byte_t type;

	XDL_ASSERT(msg != NULL);

	type = MESSAGE_GET_TYPE(*pmsg);

	XDL_ASSERT(type == MEM_MESSAGE);

	bytes_realloc(pmsg, MESSAGE_HEAD_SIZE + len);

	MESSAGE_SET_TYPE(*pmsg, MEM_MESSAGE);
	MESSAGE_SET_SIZE(*pmsg, len);
	MESSAGE_SET_VER(*pmsg, phr->ver);
	MESSAGE_SET_QOS(*pmsg, phr->qos);
	MESSAGE_SET_SEQ(*pmsg, phr->seq);
	MESSAGE_SET_UTC(*pmsg, phr->utc);

	xmem_copy((void*)MESSAGE_GET_BUF(*pmsg), (void*)buf, len);

	return len;
}

dword_t message_read(message_t msg, msg_hdr_t* phr, byte_t* buf, dword_t max)
{
	byte_t** pmsg = (byte_t**)msg;
	byte_t type;
	dword_t len;

	XDL_ASSERT(msg != NULL);

	type = MESSAGE_GET_TYPE(*pmsg);

	XDL_ASSERT(type == MEM_MESSAGE);

	len = MESSAGE_GET_SIZE(*pmsg);
	if (phr)
	{
		phr->ver = MESSAGE_GET_VER(*pmsg);
		phr->qos = MESSAGE_GET_QOS(*pmsg);
		phr->seq = MESSAGE_GET_SEQ(*pmsg);
		phr->utc = MESSAGE_GET_UTC(*pmsg);
	}

	len = (len < max) ? len : max;

	if (buf)
	{
		xmem_copy((void*)(buf), (void*)MESSAGE_GET_BUF(*pmsg), len);
	}

	return len;
}

dword_t message_decode(message_t msg, const byte_t* data)
{
	byte_t** pmsg = (byte_t**)msg;
	byte_t type;
	dword_t len;

	type = MESSAGE_GET_TYPE(data);

	XDL_ASSERT(type == MEM_MESSAGE);

	len = MESSAGE_HEAD_SIZE + MESSAGE_GET_SIZE(data);

	if (pmsg)
	{
		bytes_realloc(pmsg, len);
		xmem_copy((void*)(*pmsg), (void*)data, len);
	}

	return len;
}

dword_t message_encode(message_t msg, byte_t* buf, dword_t max)
{
	byte_t** pmsg = (byte_t**)msg;
	dword_t len;
	byte_t type;

	XDL_ASSERT(msg != NULL);

	type = MESSAGE_GET_TYPE(*pmsg);

	XDL_ASSERT(type == MEM_MESSAGE);

	len = MESSAGE_HEAD_SIZE + MESSAGE_GET_SIZE(*pmsg);
	len = (len < max) ? len : max;

	if (buf)
	{
		xmem_copy((void*)buf, (void*)(*pmsg), len);
	}

	return len;
}

#if defined(XDL_SUPPORT_TEST)
void test_message(void)
{
	byte_t buf[] = "hello world!";

	msg_hdr_t hdr = { 0 };

	message_t msg = message_alloc();

	hdr.ver = MSGVER_SENSOR;
	hdr.qos = 0x02;
	
	byte_t tmp[100] = { 0 };

	for (int i = 0; i < 100; i++)
	{
		hdr.seq = i;
		hdr.utc = get_timestamp();

		message_write(msg, &hdr, buf, a_xslen((schar_t*)buf));

		message_read(msg, &hdr, tmp, 100);

		printf("ver:0x%08x qos:%c seq:%d utc:%llu msg:%s\n", hdr.ver, hdr.qos, hdr.seq, hdr.utc,tmp);
	}

	message_free(msg);
}
#endif