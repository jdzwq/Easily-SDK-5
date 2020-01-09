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
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdloem.h"
#include "xdldoc.h"

/***********************************
typedef struct _radobj_t{
	object_head{
		dword_t tag;
		dword_t lru;
	}
	byte_t[1] compressed;
	byte_t[3] encode;
	dword_t encode bytes;
	[dword_t compressed bytes;]
	byte_t[] data;
}radobj_t;
*************************************/

#define OBJECT_GET_TYPE(obj)	GET_BYTE((obj), 0)
#define OBJECT_SET_TYPE(obj,n)	PUT_BYTE((obj),0,n)
#define OBJECT_GET_COMPRESS(obj)	(bool_t)GET_BYTE((obj), sizeof(object_head))
#define OBJECT_SET_COMPRESS(obj, b)	PUT_BYTE((obj),sizeof(object_head),(byte_t)b)
#define OBJECT_SET_ENCODE(obj,n)	PUT_ENCODE((obj),sizeof(object_head) + 1,n)
#define OBJECT_GET_ENCODE(obj)		GET_ENCODE((obj),sizeof(object_head) + 1)
#define OBJECT_SET_ENCODE_SIZE(obj,n)	PUT_DWORD_LOC((obj),sizeof(object_head) + 4,n)
#define OBJECT_GET_ENCODE_SIZE(obj)		GET_DWORD_LOC((obj),sizeof(object_head) + 4)
#define OBJECT_SET_COMPRESS_SIZE(obj,n)	PUT_DWORD_LOC((obj),sizeof(object_head) + 8,n)
#define OBJECT_GET_COMPRESS_SIZE(obj)	GET_DWORD_LOC((obj),sizeof(object_head) + 8)

#define OBJECT_ENCODE_BUFFER(obj)		(obj + sizeof(object_head) + 8)
#define OBJECT_COMPRESS_BUFFER(obj)		(obj + sizeof(object_head) + 12)
#define OBJECT_ENCODE_HEAD_SIZE			(sizeof(object_head) + 8)
#define OBJECT_COMPRESS_HEAD_SIZE		(sizeof(object_head) + 12)

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
	dw = OBJECT_GET_ENCODE_SIZE(*pobj);

	//if compressed
	if (compre) return;

	switch (type)
	{
	case _OBJECT_STRING:
		n = dw + dw / 10;
		if (n == dw) n++;

		buf = (byte_t*)xmem_alloc(n);
		n = lzf_compress(OBJECT_ENCODE_BUFFER(*pobj), dw, buf, n);
		break;
	default:
		n = dw;
		buf = (byte_t*)xmem_alloc(n);
		xzlib_compress_bytes(OBJECT_ENCODE_BUFFER(*pobj), dw, buf, &n);
		break;
	}

	bytes_realloc(pobj, OBJECT_COMPRESS_HEAD_SIZE + n);
	xmem_copy(OBJECT_COMPRESS_BUFFER(*pobj), buf, n);
	xmem_free(buf);

	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 1);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, dw);
	//commpressed bytes
	OBJECT_SET_COMPRESS_SIZE(*pobj, n);
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
	dw = OBJECT_GET_ENCODE_SIZE(*pobj);

	//if not compressed
	if (!compre) return;

	//compressed bytes
	n = OBJECT_GET_COMPRESS_SIZE(*pobj);
	if (!n)
		return;

	buf = (byte_t*)xmem_alloc(n);
	xmem_copy(buf, OBJECT_COMPRESS_BUFFER(*pobj), n);

	bytes_realloc(pobj, OBJECT_ENCODE_HEAD_SIZE + dw);

	switch (type)
	{
	case _OBJECT_STRING:
		lzf_decompress(buf, n, OBJECT_ENCODE_BUFFER(*pobj), dw);
		break;
	default:
		xzlib_uncompress_bytes(buf, n, OBJECT_ENCODE_BUFFER(*pobj), &dw);
		break;
	}
	
	xmem_free(buf);

	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, dw);
}

/*********************************************************************************/
object_t object_alloc(int encode)
{
	byte_t** pobj;

	pobj = bytes_alloc();
	bytes_realloc(pobj, OBJECT_ENCODE_HEAD_SIZE);

	//type
	OBJECT_SET_TYPE(*pobj, _OBJECT_UNKNOWN);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, 0);

	return (object_t)pobj;
}

void object_free(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	XDL_ASSERT(obj != NULL);

	bytes_free(pobj);
}

void object_empty(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	int encode;

	XDL_ASSERT(obj != NULL);

	encode = OBJECT_GET_ENCODE(*pobj);

	bytes_realloc(pobj, OBJECT_ENCODE_HEAD_SIZE);

	//type
	OBJECT_SET_TYPE(*pobj, _OBJECT_UNKNOWN);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, 0);
}

object_t object_clone(object_t obj)
{
	byte_t** pdst;
	byte_t** psrc = (byte_t**)obj;

	bool_t compre;
	dword_t n;

	XDL_ASSERT(obj != NULL);

	pdst = (byte_t**)object_alloc(DEF_MBS);

	compre = OBJECT_GET_COMPRESS(*psrc);
	if (compre)
		n = OBJECT_GET_COMPRESS_SIZE(*psrc);
	else
		n = OBJECT_GET_ENCODE_SIZE(*psrc);

	if (compre)
		bytes_realloc(pdst, OBJECT_COMPRESS_HEAD_SIZE + n);
	else
		bytes_realloc(pdst, OBJECT_ENCODE_HEAD_SIZE + n);

	if (compre)
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_COMPRESS_HEAD_SIZE);
		xmem_copy((void*)OBJECT_COMPRESS_BUFFER(*pdst), (void*)OBJECT_COMPRESS_BUFFER(*psrc), n);
	}
	else
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_ENCODE_HEAD_SIZE);
		xmem_copy((void*)OBJECT_ENCODE_BUFFER(*pdst), (void*)OBJECT_ENCODE_BUFFER(*psrc), n);
	}

	return (object_t)pdst;
}

void object_copy(object_t dst, object_t src)
{
	byte_t** pdst = (byte_t**)dst;
	byte_t** psrc = (byte_t**)src;

	bool_t compre;
	dword_t n;

	XDL_ASSERT(dst && src);

	compre = OBJECT_GET_COMPRESS(*psrc);
	if (compre)
		n = OBJECT_GET_COMPRESS_SIZE(*psrc);
	else
		n = OBJECT_GET_ENCODE_SIZE(*psrc);

	if (compre)
		bytes_realloc(pdst, OBJECT_COMPRESS_HEAD_SIZE + n);
	else
		bytes_realloc(pdst, OBJECT_ENCODE_HEAD_SIZE + n);

	if (compre)
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_COMPRESS_HEAD_SIZE);
		xmem_copy((void*)OBJECT_COMPRESS_BUFFER(*pdst), (void*)OBJECT_COMPRESS_BUFFER(*psrc), n);
	}
	else
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), OBJECT_ENCODE_HEAD_SIZE);
		xmem_copy((void*)OBJECT_ENCODE_BUFFER(*pdst), (void*)OBJECT_ENCODE_BUFFER(*psrc), n);
	}
}

int object_get_encode(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	XDL_ASSERT(pobj);

	return OBJECT_GET_ENCODE(*pobj);
}

int object_get_type(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	XDL_ASSERT(pobj);

	return OBJECT_GET_TYPE(*pobj);
}

bool_t object_get_commpress(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	XDL_ASSERT(pobj);

	return (OBJECT_GET_COMPRESS(*pobj)) ? 1 : 0;
}

void object_set_commpress(object_t obj, bool_t b)
{
	byte_t** pobj = (byte_t**)obj;
	bool_t compre;

	XDL_ASSERT(pobj);

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

void object_set_string(object_t obj, const tchar_t* str, int len)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;
	
	XDL_ASSERT(obj != NULL);

	if (len < 0)
		len = xslen(str);

	encode = OBJECT_GET_ENCODE(*pobj);

	if (!encode) encode = DEF_MBS;

	object_empty(obj);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		dw = ucs_to_utf8(str, len, NULL, MAX_LONG);
#else
		dw = mbs_to_utf8(str, len, NULL, MAX_LONG);
#endif
		break;
	case _GB2312:
#ifdef _UNICODE
		dw = ucs_to_gb2312(str, len, NULL, MAX_LONG);
#else
		dw = mbs_to_gb2312(str, len, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		dw = ucs_to_utf16lit(str, len, NULL, MAX_LONG);
#else
		dw = mbs_to_utf16lit(str, len, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		dw = ucs_to_utf16big(str, len, NULL, MAX_LONG);
#else
		dw = mbs_to_utf16big(str, len, NULL, MAX_LONG);
#endif
		break;
	}

	bytes_realloc(pobj, OBJECT_ENCODE_HEAD_SIZE + dw);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		dw = ucs_to_utf8(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#else
		dw = mbs_to_utf8(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#endif
		break;
	case _GB2312:
#ifdef _UNICODE
		dw = ucs_to_gb2312(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#else
		dw = mbs_to_gb2312(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		dw = ucs_to_utf16lit(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#else
		dw = mbs_to_utf16lit(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		dw = ucs_to_utf16big(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#else
		dw = mbs_to_utf16big(str, len, OBJECT_ENCODE_BUFFER(*pobj), dw);
#endif
		break;
		break;
	}

	type = _OBJECT_STRING;
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, dw);
}

int object_get_string(object_t obj, tchar_t* str, int max)
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

	if (type != _OBJECT_STRING)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODE_SIZE(*pobj);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		return utf8_to_ucs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#else
		return utf8_to_mbs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#endif
		break;
	case _GB2312:
#ifdef _UNICODE
		return gb2312_to_ucs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#else
		return gb2312_to_mbs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		return utf16lit_to_ucs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#else
		return utf16lit_to_mbs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		return utf16big_to_ucs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#else
		return utf16big_to_mbs(OBJECT_ENCODE_BUFFER(*pobj), dw, str, max);
#endif
		break;
	default:
		return 0;
	}
}

bool_t object_get_variant(object_t obj, variant_t* pval)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compre;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	variant_to_null(pval);

	type = OBJECT_GET_TYPE(*pobj);
	compre = OBJECT_GET_COMPRESS(*pobj);
	encode = OBJECT_GET_ENCODE(*pobj);

	if (type != _OBJECT_VARIANT)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODE_SIZE(*pobj);
	variant_decode(pval, (*pobj) + sizeof(object_head) + 8);

	return 1;
}

void object_set_variant(object_t obj, variant_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	encode = OBJECT_GET_ENCODE(*pobj);

	object_empty(obj);

	dw = variant_encode(&val, NULL, MAX_LONG);

	bytes_realloc(pobj, sizeof(object_head) + 8 + dw);
	variant_encode(&val, OBJECT_ENCODE_BUFFER(*pobj), dw);

	type = _OBJECT_VARIANT;
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, dw);
}

void object_set_domdoc(object_t obj, link_t_ptr dom)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	encode = OBJECT_GET_ENCODE(*pobj);

	if (!encode) encode = DEF_MBS;

	object_empty(obj);

	dw = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, encode);

	bytes_realloc(pobj, OBJECT_ENCODE_HEAD_SIZE + dw);
	format_dom_doc_to_bytes(dom, OBJECT_ENCODE_BUFFER(*pobj), dw, encode);

	type = _OBJECT_DOMDOC;
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, dw);
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

	if (type != _OBJECT_DOMDOC)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODE_SIZE(*pobj);

	return parse_dom_doc_from_bytes(dom, OBJECT_ENCODE_BUFFER(*pobj), dw, encode);
}

void object_set_bytes(object_t obj, int encode, const byte_t* buf, dword_t len)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;

	XDL_ASSERT(obj != NULL);

	object_empty(obj);

	bytes_realloc(pobj, sizeof(object_head) + 8 + len);

	xmem_copy((void*)OBJECT_ENCODE_BUFFER(*pobj), (void*)buf, len);

	type = _OBJECT_BINARY;
	//type
	OBJECT_SET_TYPE(*pobj, type);
	//set commpress bit
	OBJECT_SET_COMPRESS(*pobj, 0);
	//encode
	OBJECT_SET_ENCODE(*pobj, encode);
	//encodding bytes
	OBJECT_SET_ENCODE_SIZE(*pobj, len);
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

	if (type != _OBJECT_BINARY)
		return 0;

	if (compre)
	{
		//decompress first
		_object_decompress(pobj);
	}

	dw = OBJECT_GET_ENCODE_SIZE(*pobj);
	dw = (dw < max) ? dw : max;

	if (buf)
	{
		xmem_copy((void*)buf, (void*)((*pobj) + sizeof(object_head) + 8), dw);
	}

	return dw;
}

dword_t object_decode(object_t obj, const byte_t* data)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;
	dword_t dw;

	XDL_ASSERT(obj && data);

	type = OBJECT_GET_TYPE(*pobj);

	if (type != _OBJECT_BINARY && type != _OBJECT_STRING && type != _OBJECT_VARIANT && type != _OBJECT_DOMDOC)
		return 0;

	dw = OBJECT_ENCODE_HEAD_SIZE + GET_DWORD_LOC(data, sizeof(object_head) + 4);

	bytes_realloc(pobj, dw);
	xmem_copy((void*)(*pobj), (void*)data, dw);

	return dw;
}

dword_t object_encode(object_t obj, byte_t* buf, dword_t max)
{
	byte_t** pobj = (byte_t**)obj;
	bool_t compre;
	dword_t dw;

	XDL_ASSERT(obj != NULL);

	compre = OBJECT_GET_COMPRESS(*pobj);

	if (compre)
		_object_decompress(pobj);

	dw = sizeof(object_head) + 8 + GET_DWORD_LOC((*pobj), sizeof(object_head) + 4);
	dw = (dw < max) ? dw : max;

	if (buf)
	{
		xmem_copy(buf, (*pobj), dw);
	}

	return dw;
}

#if defined(_DEBUG) || defined(DEBUG)
void test_object(void)
{
	object_t obj = object_alloc(_UTF8);

	object_set_string(obj, _T("object test 字符对象测试"), -1);
	object_set_commpress(obj, 1);

	tchar_t* str;
	int len;
	len = object_get_string(obj, NULL, MAX_LONG);
	str = xsalloc(len + 1);
	object_get_string(obj, str, len);
	xsfree(str);

	variant_t v = { 0 };
	v.vv = VV_STRING;
	variant_from_string(&v, _T("variant object 变体对象测试"), -1);

	object_set_variant(obj, v);

	variant_to_null(&v);
	object_get_variant(obj, &v);
	variant_to_null(&v);

	byte_t* buf;
	dword_t dw;
	dw = object_encode(obj, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(dw);
	object_encode(obj, buf, dw);

	object_empty(obj);
	object_decode(obj, buf);
	xmem_free(buf);

	object_get_variant(obj, &v);

	variant_to_null(&v);
	object_free(obj);
}
#endif