/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc object document

	@module	object.c | implement file

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

#include "varobj.h"
#include "varbytes.h"
#include "varstr.h"
#include "variant.h"
#include "map.h"
#include "vector.h"
#include "matrix.h"
#include "message.h"
#include "queue.h"

#include "xmlbio.h"
#include "xdlimp.h"
#include "domdoc.h"

#define MIN_NEED_COMPRESS	1024

/***********************************
struct{
	struct{
		byte_t[1] tag;
		byte_t[3] len; 4 + encoded bytes or compressed bytes
	}memobj_head;
	byte_t[1]: if compressed
	byte_t[3]: origin bytes
	byte_t[encoded bytes | conpressed bytes] data;
}object;
*************************************/

#define OBJECT_HEAD_SIZE		(sizeof(memobj_head) + 4)

#define OBJECT_GET_TYPE(obj)	GET_BYTE((obj),0)
#define OBJECT_SET_TYPE(obj,n)	PUT_BYTE((obj),0,n)

#define OBJECT_SET_SIZE(obj,n)	PUT_THREEBYTE_LOC((obj),1,(n + 4))
#define OBJECT_GET_SIZE(obj)	(GET_THREEBYTE_LOC((obj),1) - 4)

#define OBJECT_GET_COMPRESS(obj)	GET_BYTE((obj), sizeof(memobj_head))
#define OBJECT_SET_COMPRESS(obj, b)	PUT_BYTE((obj),sizeof(memobj_head),(byte_t)b)

#define OBJECT_SET_UNCOMPRE_SIZE(obj,n)	PUT_THREEBYTE_LOC((obj),(sizeof(memobj_head) + 1),n)
#define OBJECT_GET_UNCOMPRE_SIZE(obj)	GET_THREEBYTE_LOC((obj),(sizeof(memobj_head) + 1))

#define OBJECT_DATA_BUFFER(obj)		(obj + sizeof(memobj_head) + 4)

static void _object_compress(byte_t** pobj)
{
	byte_t type;
	bool_t compre;
	dword_t cur, org;
	byte_t* buf;

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	//object bytes
	org = OBJECT_GET_SIZE(*pobj);
	cur = org;

	//if compressed
	if (compre || org < MIN_NEED_COMPRESS) return;

	switch ((type & 0x0F))
	{
	case MEM_BINARY:
		cur += (cur / 10);
		if (cur == org) cur++;

		buf = (byte_t*)xmem_alloc(cur);
		if (xzlib_compress_bytes(OBJECT_DATA_BUFFER(*pobj), org, buf, &cur) == 0)
		{
			xmem_free(buf);
			return;
		}
		break;
	default:
		cur += (cur / 10);
		if (cur == org) cur++;

		buf = (byte_t*)xmem_alloc(cur);
		if ((cur = lzf_compress(OBJECT_DATA_BUFFER(*pobj), org, buf, cur)) == 0)
		{
			xmem_free(buf);
			return;
		}
		break;
	}

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + cur);
	xmem_copy(OBJECT_DATA_BUFFER(*pobj), buf, cur);
	xmem_free(buf);

	//object bytes (compressed)
	OBJECT_SET_SIZE(*pobj, cur);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 1);
	//uncompressed bytes (uncompressed)
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, org);
}

static void _object_decompress(byte_t** pobj)
{
	byte_t type;
	bool_t compre;
	dword_t cur, org;
	byte_t* buf;

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	//if not compressed
	if (!compre) return;

	//object bytes
	org = OBJECT_GET_SIZE(*pobj);

	//uncompressed bytes
	cur = OBJECT_GET_UNCOMPRE_SIZE(*pobj);
	if (!cur)
		return;

	buf = (byte_t*)xmem_alloc(org);
	xmem_copy(buf, OBJECT_DATA_BUFFER(*pobj), org);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + cur);

	switch ((type & 0x0F))
	{
	case MEM_BINARY:
		if (!xzlib_uncompress_bytes(buf, org, OBJECT_DATA_BUFFER(*pobj), &cur))
		{
			xmem_free(buf);
			return;
		}
		break;
	default:
		if ((cur = lzf_decompress(buf, org, OBJECT_DATA_BUFFER(*pobj), cur)) == 0)
		{
			xmem_free(buf);
			return;
		}
		break;
	}
	
	xmem_free(buf);

	//object bytes (uncompressed)
	OBJECT_SET_SIZE(*pobj, cur);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//original bytes (uncompressed)
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, cur);
}

/*********************************************************************************/
object_t object_alloc(void)
{
	byte_t** pobj;

	pobj = bytes_alloc();
	bytes_realloc(pobj, OBJECT_HEAD_SIZE);

	//type
	OBJECT_SET_TYPE(*pobj, (MEM_BINARY | MEMENC_MASK));
	//object size
	OBJECT_SET_SIZE(*pobj, 0);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed size
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, 0);

	return (object_t)pobj;
}

void object_free(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	bytes_free(pobj);
}

void object_empty(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	bytes_realloc(pobj, OBJECT_HEAD_SIZE);

	//type
	OBJECT_SET_TYPE(*pobj, (MEM_BINARY | MEMENC_MASK));
	//object size
	OBJECT_SET_SIZE(*pobj, 0);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed size
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, 0);
}

dword_t object_size(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	dword_t size;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	//uncompressed size
	size = OBJECT_GET_UNCOMPRE_SIZE(*pobj);

	return size;
}

void object_copy(object_t dst, object_t src)
{
	byte_t** pdst = (byte_t**)dst;
	byte_t** psrc = (byte_t**)src;
	byte_t t1, t2;
	dword_t n;

	XDL_ASSERT(dst && src);

	t1 = OBJECT_GET_TYPE(*psrc);
	t2 = OBJECT_GET_TYPE(*pdst);

	XDL_ASSERT(IS_OBJECT_TYPE(t1) && IS_OBJECT_TYPE(t2));

	n = OBJECT_GET_SIZE(*psrc);

	bytes_realloc(pdst, OBJECT_HEAD_SIZE + n);

	xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_HEAD_SIZE);
	xmem_copy((void*)OBJECT_DATA_BUFFER(*pdst), (void*)OBJECT_DATA_BUFFER(*psrc), n);
}

object_t object_clone(object_t obj)
{
	byte_t** pdst;
	byte_t** psrc = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*psrc);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	pdst = (byte_t**)object_alloc();
	object_copy((object_t)pdst, obj);

	return (object_t)pdst;
}

int object_get_type(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(pobj);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	return (int)type;
}

bool_t object_get_commpress(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(pobj);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	return (OBJECT_GET_COMPRESS(*pobj)) ? 1 : 0;
}

void object_set_commpress(object_t obj, bool_t b)
{
	byte_t** pobj = (byte_t**)obj;
	bool_t compre;
	byte_t type;

	XDL_ASSERT(pobj);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	compre = object_get_commpress(obj);

	if (compre && b)
		return;
	else if (!compre && !b)
		return;
	else if (b)
		_object_compress(pobj);
	else
		_object_decompress(pobj);
}

bool_t object_get_message(object_t obj, message_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_MESSAGE)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	message_decode(val, OBJECT_DATA_BUFFER(*pobj));

	return 1;
}

void object_set_message(object_t obj, message_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = message_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);

	message_encode(val, OBJECT_DATA_BUFFER(*pobj), dw);

	type = (MEM_MESSAGE | MEMENC_MASK);
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

bool_t object_get_queue(object_t obj, queue_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_QUEUE)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	queue_decode(val, OBJECT_DATA_BUFFER(*pobj));

	return 1;
}

void object_set_queue(object_t obj, queue_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = queue_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);

	queue_encode(val, OBJECT_DATA_BUFFER(*pobj), dw);

	type = (MEM_QUEUE | MEMENC_MASK);
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

bool_t object_get_matrix(object_t obj, matrix_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_MATRIX)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	matrix_decode(val, OBJECT_DATA_BUFFER(*pobj));

	return 1;
}

void object_set_matrix(object_t obj, matrix_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = matrix_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);

	matrix_encode(val, OBJECT_DATA_BUFFER(*pobj), dw);

	type = (MEM_MATRIX | MEMENC_MASK);
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

bool_t object_get_vector(object_t obj, vector_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_VECTOR)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	vector_decode(val, OBJECT_DATA_BUFFER(*pobj));

	return 1;
}

void object_set_vector(object_t obj, vector_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = vector_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);

	vector_encode(val, OBJECT_DATA_BUFFER(*pobj), dw);

	type = (MEM_VECTOR | MEMENC_MASK);
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

bool_t object_get_map(object_t obj, map_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_MAP)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	map_decode(val, OBJECT_DATA_BUFFER(*pobj));

	return 1;
}

void object_set_map(object_t obj, map_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = map_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);

	map_encode(val, OBJECT_DATA_BUFFER(*pobj), dw);

	type = (MEM_MAP | MEMENC_MASK);
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

void object_set_string(object_t obj, string_t str)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = string_encode(str, _UTF8, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);

	string_encode(str, _UTF8, OBJECT_DATA_BUFFER(*pobj), dw);

	type = (MEM_STRING | MEMENC_MASK);
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

bool_t object_get_string(object_t obj, string_t str)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_STRING)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_UNCOMPRE_SIZE(*pobj);

	string_decode(str, _UTF8, OBJECT_DATA_BUFFER(*pobj), dw);

	return 1;
}

bool_t object_get_variant(object_t obj, variant_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_VARIANT)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_UNCOMPRE_SIZE(*pobj);

	variant_decode(val, OBJECT_DATA_BUFFER(*pobj));

	return 1;
}

void object_set_variant(object_t obj, variant_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = variant_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);

	variant_encode(val, OBJECT_DATA_BUFFER(*pobj), dw);

	type = (MEM_VARIANT | MEMENC_MASK);
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

#if defined(XDL_SUPPORT_DOC)

void object_set_domdoc(object_t obj, link_t_ptr dom)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, _UTF8);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + dw);
	format_dom_doc_to_bytes(dom, OBJECT_DATA_BUFFER(*pobj), dw, _UTF8);

	type = MEM_DOMDOC | MEMENC_MASK;
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, dw);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, dw);
}

bool_t object_get_domdoc(object_t obj, link_t_ptr dom)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	delete_dom_child_nodes(dom);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_DOMDOC)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_UNCOMPRE_SIZE(*pobj);

	return parse_dom_doc_from_bytes(dom, OBJECT_DATA_BUFFER(*pobj), dw, _UTF8);
}

#endif

void object_set_bytes(object_t obj, const byte_t* buf, dword_t len)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + len);

	xmem_copy((void*)OBJECT_DATA_BUFFER(*pobj), (void*)buf, len);

	type = MEM_BINARY | MEMENC_MASK;
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, len);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, len);
}

void object_add_bytes(object_t obj, const byte_t* buf, dword_t len)
{
	byte_t** pobj = (byte_t**)obj;
	dword_t off;
	byte_t type;
	bool_t compre;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_BINARY)
		return ;

	if (compre)
	{
		_object_decompress(pobj);
	}

	off = OBJECT_GET_UNCOMPRE_SIZE(*pobj);

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + off + len);

	xmem_copy((void*)(OBJECT_DATA_BUFFER(*pobj) + off), (void*)buf, len);

	type = MEM_BINARY | MEMENC_MASK;
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, (off + len));
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, (off + len));
}

void object_del_bytes(object_t obj, dword_t off)
{
	byte_t** pobj = (byte_t**)obj;
	dword_t len;
	byte_t type;
	bool_t compre;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_BINARY)
		return;

	len = OBJECT_GET_UNCOMPRE_SIZE(*pobj);
	if (len <= off)
		return;

	if (compre)
	{
		_object_decompress(pobj);
	}

	bytes_realloc(pobj, OBJECT_HEAD_SIZE + off);

	type = MEM_BINARY | MEMENC_MASK;
	//object type
	OBJECT_SET_TYPE(*pobj, type);
	//object size
	OBJECT_SET_SIZE(*pobj, off);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//uncompressed bytes
	OBJECT_SET_UNCOMPRE_SIZE(*pobj, off);
}

dword_t object_get_bytes(object_t obj, byte_t* buf, dword_t max)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_BINARY)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_UNCOMPRE_SIZE(*pobj);
	dw = (dw < max) ? dw : max;

	if (buf)
	{
		xmem_copy((void*)buf, (void*)(OBJECT_DATA_BUFFER(*pobj)), dw);
	}

	return dw;
}

dword_t object_encode(object_t obj, byte_t* buf, dword_t max)
{
	byte_t** pobj = (byte_t**)obj;
	dword_t dw;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	dw = OBJECT_HEAD_SIZE + OBJECT_GET_SIZE(*pobj);
	dw = (dw < max) ? dw : max;

	if (buf)
	{
		xmem_copy((void*)buf, (void*)(*pobj), dw);
	}

	return dw;
}

dword_t object_decode(object_t obj, const byte_t* data)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;
	dword_t dw;

	if (!data)
		return 0;

	type = OBJECT_GET_TYPE(data);

	if (!IS_OBJECT_TYPE(type))
		return 0;

	dw = OBJECT_HEAD_SIZE + OBJECT_GET_SIZE(data);

	if (obj)
	{
		bytes_realloc(pobj, dw);
		xmem_copy((void*)(*pobj), (void*)data, dw);
	}

	return dw;
}

void object_hash32(object_t obj, key32_t* pkey)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;
	dword_t dw;
	byte_t* buf;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	dw = OBJECT_GET_SIZE(*pobj);
	buf = OBJECT_DATA_BUFFER(*pobj);

	if (dw)
		murhash32((byte_t*)buf, dw, (byte_t*)pkey);
	else
		xmem_zero((void*)pkey, sizeof(key32_t));
}

void object_hash64(object_t obj, key64_t* pkey)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;
	dword_t dw;
	byte_t* buf;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	dw = OBJECT_GET_SIZE(*pobj);
	buf = OBJECT_DATA_BUFFER(*pobj);

	if (dw)
		siphash64((byte_t*)buf, dw, (byte_t*)pkey);
	else
		xmem_zero((void*)pkey, sizeof(key64_t));
}

void object_hash128(object_t obj, key128_t* pkey)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;
	dword_t dw;
	byte_t* buf;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	dw = OBJECT_GET_SIZE(*pobj);
	buf = OBJECT_DATA_BUFFER(*pobj);

	if (dw)
		murhash128((byte_t*)buf, dw, (byte_t*)pkey);
	else
		xmem_zero((void*)pkey, sizeof(key128_t));
}

#if defined(XDL_SUPPORT_TEST)
void test_object(void)
{
	object_t obj = object_alloc();

	tchar_t* str;
	int len;

	string_t s = string_alloc();
	string_cpy(s, _T("object test 字符对象测试"), -1);
	len = string_len(s);
	str = string_ptr(s);
	_tprintf(_T("string object test: %s\n"), str);

	object_set_string(obj, s);
	object_set_commpress(obj, 1);
	len = object_size(obj);
	_tprintf(_T("string object compressed:%d\n"), len);

	string_empty(s);
	object_get_string(obj, s);
	len = string_len(s);
	str = string_ptr(s);
	_tprintf(_T("string object unconpressed: %s\n"), str);
	
	string_cpy(s, _T("object test 变体对象测试"), -1);
	len = string_len(s);
	str = string_ptr(s);
	_tprintf(_T("variant object test: %s\n"), str);

	variant_t v = variant_alloc(VV_STRING_UTF8);
	variant_from_string(v, string_ptr(s), string_len(s));

	object_set_variant(obj, v);
	object_set_commpress(obj, 1);
	len = object_size(obj);
	_tprintf(_T("variant object compressed:%d\n"), len);

	variant_to_null(v, VV_STRING_UTF8);
	object_get_variant(obj, v);
	len = variant_to_string(v, NULL, MAX_LONG);
	str = xsalloc(len + 1);
	variant_to_string(v, str, len);
	_tprintf(_T("variant object unconpressed: %s\n"), str);
	xsfree(str);

	byte_t* buf;
	dword_t dw;
	dw = object_encode(obj, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(dw);
	object_encode(obj, buf, dw);

	object_empty(obj);
	object_decode(obj, buf);
	xmem_free(buf);

	object_get_variant(obj, v);
	len = variant_to_string(v, NULL, MAX_LONG);
	str = xsalloc(len + 1);
	variant_to_string(v, str, len);
	xsfree(str);

	string_free(s);
	variant_free(v);
	object_free(obj);
}
#endif