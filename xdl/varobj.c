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
		byte_t[3] len; 8 + encoded bytes or compressed bytes
	}memobj_head;
	byte_t[1] if compressed
	byte_t[3] encoding type
	byte_t[4] encoded bytes
	byte_t[encoded bytes | conpressed bytes] data;
}object;
*************************************/

#define OBJECT_GET_TYPE(obj)	GET_BYTE((obj),0)
#define OBJECT_SET_TYPE(obj,n)	PUT_BYTE((obj),0,n)

#define OBJECT_SET_COMPRESS_SIZE(obj,n)	PUT_THREEBYTE_LOC((obj),1,(n + 8))
#define OBJECT_GET_COMPRESS_SIZE(obj)	(GET_THREEBYTE_LOC((obj),1) - 8)

#define OBJECT_GET_COMPRESS(obj)	(bool_t)GET_BYTE((obj), sizeof(memobj_head))
#define OBJECT_SET_COMPRESS(obj, b)	PUT_BYTE((obj),sizeof(memobj_head),(byte_t)b)

#define OBJECT_SET_ENCODE(obj,n)	PUT_ENCODE((obj),(sizeof(memobj_head) + 1),n)
#define OBJECT_GET_ENCODE(obj)		GET_ENCODE((obj),(sizeof(memobj_head) + 1))

#define OBJECT_SET_ENCODING_SIZE(obj,n)	PUT_DWORD_LOC((obj),(sizeof(memobj_head) + 4),n)
#define OBJECT_GET_ENCODING_SIZE(obj)	GET_DWORD_LOC((obj),(sizeof(memobj_head) + 4))

#define OBJECT_ENCODING_BUFFER(obj)		(obj + sizeof(memobj_head) + 8)
#define OBJECT_COMPRESS_BUFFER(obj)		(obj + sizeof(memobj_head) + 8)
#define OBJECT_ENCODING_HEAD_SIZE		(sizeof(memobj_head) + 8)
#define OBJECT_COMPRESS_HEAD_SIZE		(sizeof(memobj_head) + 8)

static void _object_compress(byte_t** pobj)
{
	byte_t type;
	bool_t compre;
	int encode;
	dword_t n, dw;
	byte_t* buf;

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	//encoding bytes
	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	//if compressed
	if (compre || dw < MIN_NEED_COMPRESS) return;

	switch ((type & 0x0F))
	{
	case MEM_BINARY:
		n = dw + dw / 10;
		if (n == dw) n++;

		buf = (byte_t*)xmem_alloc(n);
		if (xzlib_compress_bytes(OBJECT_ENCODING_BUFFER(*pobj), dw, buf, &n) == 0)
		{
			xmem_free(buf);
			return;
		}
		break;
	default:
		n = dw + dw / 10;
		if (n == dw) n++;

		buf = (byte_t*)xmem_alloc(n);
		if ((n = lzf_compress(OBJECT_ENCODING_BUFFER(*pobj), dw, buf, n)) == 0)
		{
			xmem_free(buf);
			return;
		}
		break;
	}

	bytes_realloc(pobj, OBJECT_COMPRESS_HEAD_SIZE + n);
	xmem_copy(OBJECT_COMPRESS_BUFFER(*pobj), buf, n);
	xmem_free(buf);

	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 1);
	//commpressed bytes
	OBJECT_SET_COMPRESS_SIZE(*pobj, n);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

static void _object_decompress(byte_t** pobj)
{
	byte_t type;
	bool_t compre;
	dword_t n, dw;
	byte_t* buf;
	int encode;

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	//encoding bytes
	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	//if not compressed
	if (!compre) return;

	//compressed bytes
	n = OBJECT_GET_COMPRESS_SIZE(*pobj);
	if (!n)
		return;

	buf = (byte_t*)xmem_alloc(n);
	xmem_copy(buf, OBJECT_COMPRESS_BUFFER(*pobj), n);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	switch ((type & 0x0F))
	{
	case MEM_BINARY:
		xzlib_uncompress_bytes(buf, n, OBJECT_ENCODING_BUFFER(*pobj), &dw);
		break;
	default:
		lzf_decompress(buf, n, OBJECT_ENCODING_BUFFER(*pobj), dw);
		break;
	}
	
	xmem_free(buf);

	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

/*********************************************************************************/
object_t object_alloc(int encode)
{
	byte_t** pobj;

	pobj = bytes_alloc();
	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE);

	//type
	OBJECT_SET_TYPE(*pobj, (MEM_BINARY | MEMENC_MASK));
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, 0);

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
	int encode;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE);

	//type
	OBJECT_SET_TYPE(*pobj, (MEM_BINARY | MEMENC_MASK));
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, 0);
}

dword_t object_size(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	dword_t size;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	size = OBJECT_GET_ENCODING_SIZE(*pobj);

	return size;
}

object_t object_clone(object_t obj)
{
	byte_t** pdst;
	byte_t** psrc = (byte_t**)obj;
	byte_t type;
	bool_t compre;
	dword_t n;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*psrc);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	pdst = (byte_t**)object_alloc(OBJECT_GET_ENCODE(*psrc));

	compre = OBJECT_GET_COMPRESS(*psrc);
	if (compre)
		n = OBJECT_GET_COMPRESS_SIZE(*psrc);
	else
		n = OBJECT_GET_ENCODING_SIZE(*psrc);

	if (compre)
		bytes_realloc(pdst, OBJECT_COMPRESS_HEAD_SIZE + n);
	else
		bytes_realloc(pdst, OBJECT_ENCODING_HEAD_SIZE + n);

	if (compre)
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_COMPRESS_HEAD_SIZE);
		xmem_copy((void*)OBJECT_COMPRESS_BUFFER(*pdst), (void*)OBJECT_COMPRESS_BUFFER(*psrc), n);
	}
	else
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_ENCODING_HEAD_SIZE);
		xmem_copy((void*)OBJECT_ENCODING_BUFFER(*pdst), (void*)OBJECT_ENCODING_BUFFER(*psrc), n);
	}

	return (object_t)pdst;
}

void object_copy(object_t dst, object_t src)
{
	byte_t** pdst = (byte_t**)dst;
	byte_t** psrc = (byte_t**)src;
	byte_t t1, t2;
	bool_t compre;
	dword_t n;

	XDL_ASSERT(dst && src);

	t1 = OBJECT_GET_TYPE(*psrc);
	t2 = OBJECT_GET_TYPE(*pdst);

	XDL_ASSERT(IS_OBJECT_TYPE(t1) && IS_OBJECT_TYPE(t2));

	compre = OBJECT_GET_COMPRESS(*psrc);
	if (compre)
		n = OBJECT_GET_COMPRESS_SIZE(*psrc);
	else
		n = OBJECT_GET_ENCODING_SIZE(*psrc);

	if (compre)
		bytes_realloc(pdst, OBJECT_COMPRESS_HEAD_SIZE + n);
	else
		bytes_realloc(pdst, OBJECT_ENCODING_HEAD_SIZE + n);

	if (compre)
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_COMPRESS_HEAD_SIZE);
		xmem_copy((void*)OBJECT_COMPRESS_BUFFER(*pdst), (void*)OBJECT_COMPRESS_BUFFER(*psrc), n);
	}
	else
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_ENCODING_HEAD_SIZE);
		xmem_copy((void*)OBJECT_ENCODING_BUFFER(*pdst), (void*)OBJECT_ENCODING_BUFFER(*psrc), n);
	}
}

int object_get_encode(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(pobj);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	return OBJECT_GET_ENCODE(*pobj);
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
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_MESSAGE)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	message_decode(val, OBJECT_ENCODING_BUFFER(*pobj));

	return 1;
}

void object_set_message(object_t obj, message_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = message_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	message_encode(val, OBJECT_ENCODING_BUFFER(*pobj), dw);

	type = (MEM_MESSAGE | MEMENC_MASK);
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

bool_t object_get_queue(object_t obj, queue_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_QUEUE)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	queue_decode(val, OBJECT_ENCODING_BUFFER(*pobj));

	return 1;
}

void object_set_queue(object_t obj, queue_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = queue_encode(val, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	queue_encode(val, OBJECT_ENCODING_BUFFER(*pobj), dw);

	type = (MEM_QUEUE | MEMENC_MASK);
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

bool_t object_get_matrix(object_t obj, matrix_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_MATRIX)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	matrix_decode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	return 1;
}

void object_set_matrix(object_t obj, matrix_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = matrix_encode(val, encode, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	matrix_encode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	type = (MEM_MATRIX | MEMENC_MASK);
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

bool_t object_get_vector(object_t obj, vector_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_VECTOR)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	vector_decode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	return 1;
}

void object_set_vector(object_t obj, vector_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = vector_encode(val, encode, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	vector_encode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	type = (MEM_VECTOR | MEMENC_MASK);
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

bool_t object_get_map(object_t obj, map_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_MAP)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	map_decode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	return 1;
}

void object_set_map(object_t obj, map_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = map_encode(val, encode, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	map_encode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	type = (MEM_MAP | MEMENC_MASK);
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

void object_set_string(object_t obj, string_t str)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	encode = OBJECT_GET_ENCODE(*pobj);

	if (!encode) encode = DEF_MBS;

	object_empty(obj);

	dw = string_encode(str, encode, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	string_encode(str, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	type = (MEM_STRING | MEMENC_MASK);
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

bool_t object_get_string(object_t obj, string_t str)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_STRING)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	string_decode(str, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	return 1;
}

bool_t object_get_variant(object_t obj, variant_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_VARIANT)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	variant_decode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	return 1;
}

void object_set_variant(object_t obj, variant_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	dw = variant_encode(val, encode, NULL, MAX_LONG);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);

	variant_encode(val, encode, OBJECT_ENCODING_BUFFER(*pobj), dw);

	type = (MEM_VARIANT | MEMENC_MASK);
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

#if defined(XDL_SUPPORT_DOC)

void object_set_domdoc(object_t obj, link_t_ptr dom)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if (!encode) encode = DEF_MBS;

	object_empty(obj);

	dw = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, encode);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + dw);
	format_dom_doc_to_bytes(dom, OBJECT_ENCODING_BUFFER(*pobj), dw, encode);

	type = MEM_DOMDOC | MEMENC_MASK;
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, dw);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, dw);
}

bool_t object_get_domdoc(object_t obj, link_t_ptr dom)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	delete_dom_child_nodes(dom);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if ((type & 0x0F) != MEM_DOMDOC)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);

	return parse_dom_doc_from_bytes(dom, OBJECT_ENCODING_BUFFER(*pobj), dw, encode);
}

#endif

void object_set_bytes(object_t obj, int encode, const byte_t* buf, dword_t len)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	object_empty(obj);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + len);

	xmem_copy((void*)OBJECT_ENCODING_BUFFER(*pobj), (void*)buf, len);

	type = MEM_BINARY | MEMENC_MASK;
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, len);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, len);
}

void object_add_bytes(object_t obj, const byte_t* buf, dword_t len)
{
	byte_t** pobj = (byte_t**)obj;
	dword_t off;
	byte_t type;
	bool_t compre;
	int encode;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if (compre)
	{
		_object_decompress(pobj);
	}

	off = OBJECT_GET_ENCODING_SIZE(*pobj);

	bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + off + len);

	xmem_copy((void*)(OBJECT_ENCODING_BUFFER(*pobj) + off), (void*)buf, len);

	type = MEM_BINARY | MEMENC_MASK;
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	OBJECT_SET_COMPRESS_SIZE(*pobj, (len + off));
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODING_SIZE(*pobj, (len + off));

	if (compre)
	{
		_object_compress(pobj);
	}
}

void object_del_bytes(object_t obj, dword_t off)
{
	byte_t** pobj = (byte_t**)obj;
	dword_t len;
	byte_t type;
	bool_t compre;
	int encode;

	XDL_ASSERT(obj != NULL);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	XDL_ASSERT(IS_OBJECT_TYPE(type));

	if (compre)
	{
		_object_decompress(pobj);
	}

	len = OBJECT_GET_ENCODING_SIZE(*pobj);

	if (len > off)
	{
		bytes_realloc(pobj, OBJECT_ENCODING_HEAD_SIZE + off);

		type = MEM_BINARY | MEMENC_MASK;
		//type
		OBJECT_SET_TYPE(*pobj, type);
		//set commpress bit
		OBJECT_SET_COMPRESS(*pobj, 0);
		OBJECT_SET_COMPRESS_SIZE(*pobj, off);
		//encode
		OBJECT_SET_ENCODE(*pobj, encode);
		//encodding bytes
		OBJECT_SET_ENCODING_SIZE(*pobj, off);
	}

	if (compre)
	{
		_object_compress(pobj);
	}
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

	dw = OBJECT_GET_ENCODING_SIZE(*pobj);
	dw = (dw < max) ? dw : max;

	if (buf)
	{
		xmem_copy((void*)buf, (void*)(OBJECT_ENCODING_BUFFER(*pobj)), dw);
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

	dw = OBJECT_ENCODING_HEAD_SIZE + OBJECT_GET_COMPRESS_SIZE(*pobj);
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

	dw = OBJECT_ENCODING_HEAD_SIZE + OBJECT_GET_COMPRESS_SIZE(data);

	if (obj)
	{
		bytes_realloc(pobj, dw);
		xmem_copy((void*)(*pobj), (void*)data, dw);
	}

	return dw;
}


#if defined(XDL_SUPPORT_TEST)
void test_object(void)
{
	object_t obj = object_alloc(_UTF8);

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

	variant_t v = variant_alloc(VV_STRING);
	variant_from_string(v, string_ptr(s), string_len(s));

	object_set_variant(obj, v);
	object_set_commpress(obj, 1);
	len = object_size(obj);
	_tprintf(_T("variant object compressed:%d\n"), len);

	variant_to_null(v, VV_STRING);
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