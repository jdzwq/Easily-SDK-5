/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc object document

	@module	object.c | object implement file

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
	byte_t[1] type & compressed;
	byte_t[3] encode;
	dword_t encode bytes;
	[dword_t compressed bytes;]
	byte_t[] data;
}radobj_t;
*************************************/

static void _object_compress(byte_t** pobj)
{
	byte_t type;
	bool_t compr;
	int encode;
	dword_t n, dw;
	byte_t* buf;

	type = GET_BYTE((*pobj), 0) & 0x7F;
	compr = GET_BYTE((*pobj), 0) & 0x80;
	encode = GET_ENCODE((*pobj), 1);
	dw = GET_DWORD_LOC((*pobj), 4);

	//if compressed
	if (compr) return;

	switch (type)
	{
	case _OBJECT_STRING:
		n = dw + dw / 10;
		if (n == dw) n++;

		buf = (byte_t*)xmem_alloc(n);
		n = lzf_compress((*pobj) + 8, dw, buf, n);
		break;
	default:
		n = dw;
		buf = (byte_t*)xmem_alloc(n);
		xzlib_compress_bytes((*pobj) + 8, dw, buf, &n);
		break;
	}

	varbuf_realloc(pobj, 12 + n);
	xmem_copy((*pobj) + 12, buf, n);
	xmem_free(buf);

	//set commpress bit
	PUT_BYTE((*pobj), 0, (type | 0x80));
	//encode
	PUT_ENCODE((*pobj), 1, encode);
	//encode bytes
	PUT_DWORD_LOC((*pobj), 4, dw);
	//set commpressed bytes
	PUT_DWORD_LOC((*pobj), 8, n);
}

static void _object_decompress(byte_t** pobj)
{
	byte_t type;
	bool_t compr;
	dword_t n, dw;
	byte_t* buf;
	int encode;

	type = GET_BYTE((*pobj), 0) & 0x7F;
	compr = GET_BYTE((*pobj), 0) & 0x80;
	encode = GET_ENCODE((*pobj), 1);
	dw = GET_DWORD_LOC((*pobj), 4);

	//if not compressed
	if (!compr) return;

	//compressed bytes
	n = GET_DWORD_LOC((*pobj), 8);
	if (!n)
		return;

	buf = (byte_t*)xmem_alloc(n);
	xmem_copy(buf, (*pobj) + 12, n);

	varbuf_realloc(pobj, 8 + dw);

	switch (type)
	{
	case _OBJECT_STRING:
		lzf_decompress(buf, n, (*pobj) + 8, dw);
		break;
	default:
		xzlib_uncompress_bytes(buf, n, (*pobj) + 8, &dw);
		break;
	}
	
	xmem_free(buf);

	//clear commpress bit
	PUT_BYTE((*pobj), 0, type);
	//encode
	PUT_ENCODE((*pobj), 1, encode);
	//encode bytes
	PUT_DWORD_LOC((*pobj), 4, dw);
}

/*********************************************************************************/
object_t object_alloc(int encode)
{
	byte_t** pobj;
	byte_t* buf;

	pobj = varbuf_alloc();
	buf = varbuf_realloc(pobj, 8);

	//type & compress
	PUT_BYTE(buf, 0, 0);
	//encode
	PUT_ENCODE(buf, 1, encode);
	//encode bytes
	PUT_DWORD_LOC(buf, 4, 0);

	return (object_t)pobj;
}

void object_free(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	XDL_ASSERT(obj != NULL);

	varbuf_free(pobj);
}

void object_empty(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t* buf;
	int encode;

	XDL_ASSERT(obj != NULL);

	encode = GET_ENCODE((*pobj), 1);

	buf = varbuf_realloc(pobj, 8);

	//type & compress
	PUT_BYTE(buf, 0, 0);
	//encode
	PUT_ENCODE(buf, 1, encode);
	//encode bytes
	PUT_DWORD_LOC(buf, 4, 0);
}

object_t object_clone(object_t obj)
{
	byte_t** pdst;
	byte_t** psrc = (byte_t**)obj;

	bool_t b;
	dword_t n;

	XDL_ASSERT(obj);

	pdst = object_alloc(DEF_MBS);

	if (!obj) return (object_t)pdst;

	b = GET_BYTE((*psrc), 0) & 0x80;
	if (b)
		n = GET_DWORD_LOC((*psrc), 8);
	else
		n = GET_DWORD_LOC((*psrc), 4);

	if (b)
		varbuf_realloc(pdst, 12 + n);
	else
		varbuf_realloc(pdst, 8 + n);

	if (b)
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), 12);
		xmem_copy((void*)(*pdst + 12), (void*)(*psrc + 12), n);
	}
	else
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), 8);
		xmem_copy((void*)(*pdst + 8), (void*)(*psrc + 8), n);
	}

	return (object_t)pdst;
}

void object_copy(object_t dst, object_t src)
{
	byte_t** pdst = (byte_t**)dst;
	byte_t** psrc = (byte_t**)src;

	bool_t b;
	dword_t n;

	XDL_ASSERT(dst && src);

	b = GET_BYTE((*psrc), 0) & 0x80;
	if (b)
		n = GET_DWORD_LOC((*psrc), 8);
	else
		n = GET_DWORD_LOC((*psrc), 4);

	if (b)
		varbuf_realloc(pdst, 12 + n);
	else
		varbuf_realloc(pdst, 8 + n);

	if (b)
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), 12);
		xmem_copy((void*)(*pdst + 12), (void*)(*psrc + 12), n);
	}
	else
	{
		xmem_copy((void*)(*pdst), (void*)(*psrc), 8);
		xmem_copy((void*)(*pdst + 8), (void*)(*psrc + 8), n);
	}
}

int object_get_encode(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	return GET_ENCODE((*pobj), 1);
}

int object_get_type(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	return (int)(GET_BYTE((*pobj), 0) & 0x7F);
}

bool_t object_get_commpress(object_t obj)
{
	byte_t** pobj = (byte_t**)obj;

	XDL_ASSERT(pobj);

	return (GET_BYTE((*pobj), 0) & 0x80) ? 1 : 0;
}

void object_set_commpress(object_t obj, bool_t b)
{
	byte_t** pobj = (byte_t**)obj;
	bool_t compr;

	XDL_ASSERT(pobj);

	compr = object_get_commpress(obj);

	if (compr && b)
		return;
	else if (!compr && !b)
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

	byte_t* buf;
	dword_t dw;
	
	if (len < 0)
		len = xslen(str);

	encode = GET_ENCODE((*pobj), 1);

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

	buf = varbuf_realloc(pobj, 8 + dw);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		dw = ucs_to_utf8(str, len, buf + 8, dw);
#else
		dw = mbs_to_utf8(str, len, buf + 8, dw);
#endif
		break;
	case _GB2312:
#ifdef _UNICODE
		dw = ucs_to_gb2312(str, len, buf + 8, dw);
#else
		dw = mbs_to_gb2312(str, len, buf + 8, dw);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		dw = ucs_to_utf16lit(str, len, buf + 8, dw);
#else
		dw = mbs_to_utf16lit(str, len, buf + 8, dw);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		dw = ucs_to_utf16big(str, len, buf + 8, dw);
#else
		dw = mbs_to_utf16big(str, len, buf + 8, dw);
#endif
		break;
		break;
	}

	type = _OBJECT_STRING & 0x7F;
	//type & compress
	PUT_BYTE(buf, 0, type);
	//encode
	PUT_ENCODE(buf, 1, encode);
	//encode bytes
	PUT_DWORD_LOC(buf, 4, dw);
}

int object_get_string(object_t obj, tchar_t* str, int max)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compr;
	int encode;

	byte_t* buf;
	dword_t dw;
	
	XDL_ASSERT(pobj);

	type = GET_BYTE((*pobj), 0) & 0x7F;
	compr = GET_BYTE((*pobj), 0) & 0x80;
	encode = GET_ENCODE((*pobj), 1);

	if (type != _OBJECT_STRING)
		return 0;

	if (compr)
	{
		//decompress first
		_object_decompress(obj);
	}

	buf = *pobj;
	dw = GET_DWORD_LOC(buf, 4);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		return utf8_to_ucs(buf + 8, dw, str, max);
#else
		return utf8_to_mbs(buf + 8, dw, str, max);
#endif
		break;
	case _GB2312:
#ifdef _UNICODE
		return gb2312_to_ucs(buf + 8, dw, str, max);
#else
		return gb2312_to_mbs(buf + 8, dw, str, max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		return utf16lit_to_ucs(buf + 8, dw, str, max);
#else
		return utf16lit_to_mbs(buf + 8, dw, str, max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		return utf16big_to_ucs(buf + 8, dw, str, max);
#else
		return utf16big_to_mbs(buf + 8, dw, str, max);
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
	bool_t compr;
	int encode;

	dword_t dw;

	XDL_ASSERT(pobj);

	variant_to_null(pval);

	type = GET_BYTE((*pobj), 0) & 0x7F;
	compr = GET_BYTE((*pobj), 0) & 0x80;
	encode = GET_ENCODE((*pobj), 1);

	if (type != _OBJECT_VARIANT)
		return 0;

	if (compr)
	{
		//decompress first
		_object_decompress(obj);
	}

	dw = GET_DWORD_LOC((*pobj), 4);
	variant_decode(pval, (*pobj) + 8);

	return 1;
}

void object_set_variant(object_t obj, variant_t val)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;

	byte_t* buf;
	dword_t dw;

	encode = GET_ENCODE((*pobj), 1);

	object_empty(obj);

	dw = variant_encode(&val, NULL, MAX_LONG);

	buf = varbuf_realloc(pobj, 8 + dw);
	variant_encode(&val, buf + 8, dw);

	type = _OBJECT_VARIANT & 0x7F;
	//type & compress
	PUT_BYTE(buf, 0, type);
	//encode
	PUT_ENCODE(buf, 1, encode);
	//encode bytes
	PUT_DWORD_LOC(buf, 4, dw);
}

void object_set_domdoc(object_t obj, link_t_ptr dom)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	int encode;

	byte_t* buf;
	dword_t dw;

	encode = GET_ENCODE((*pobj), 1);

	if (!encode) encode = DEF_MBS;

	object_empty(obj);

	dw = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, encode);

	buf = varbuf_realloc(pobj, 8 + dw);
	format_dom_doc_to_bytes(dom, buf + 8, dw, encode);

	type = _OBJECT_DOMDOC & 0x7F;
	//type & compress
	PUT_BYTE(buf, 0, type);
	//encode
	PUT_ENCODE(buf, 1, encode);
	//encode bytes
	PUT_DWORD_LOC(buf, 4, dw);
}

bool_t object_get_domdoc(object_t obj, link_t_ptr dom)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compr;
	int encode;

	byte_t* buf;
	dword_t dw;

	XDL_ASSERT(pobj);

	delete_dom_child_nodes(dom);

	type = GET_BYTE((*pobj), 0) & 0x7F;
	compr = GET_BYTE((*pobj), 0) & 0x80;
	encode = GET_ENCODE((*pobj), 1);

	if (type != _OBJECT_DOMDOC)
		return 0;

	if (compr)
	{
		//decompress first
		_object_decompress(obj);
	}

	buf = *pobj;
	dw = GET_DWORD_LOC(buf, 4);

	return parse_dom_doc_from_bytes(dom, buf + 8, dw, encode);
}

void object_set_bytes(object_t obj, int encode, const byte_t* buf, dword_t len)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t* p;
	byte_t type;

	object_empty(obj);

	p = varbuf_realloc(pobj, 8 + len);

	xmem_copy((void*)(p + 8), (void*)buf, len);

	type = _OBJECT_BINARY & 0x7F;
	//type & compress
	PUT_BYTE(p, 0, type);
	//encode
	PUT_ENCODE(p, 1, encode);
	//encode bytes
	PUT_DWORD_LOC(p, 4, len);
}

dword_t object_get_bytes(object_t obj, byte_t* buf, dword_t max)
{
	byte_t** pobj = (byte_t**)obj;

	byte_t type;
	bool_t compr;
	dword_t dw;

	XDL_ASSERT(pobj);

	type = GET_BYTE((*pobj), 0) & 0x7F;
	compr = GET_BYTE((*pobj), 0) & 0x80;

	if (type != _OBJECT_BINARY)
		return 0;

	if (compr)
	{
		//decompress first
		_object_decompress(obj);
	}

	dw = GET_DWORD_LOC((*pobj), 4);
	dw = (dw < max) ? dw : max;

	if (buf)
	{
		xmem_copy((void*)buf, (void*)((*pobj) + 8), dw);
	}

	return dw;
}

dword_t object_decode(object_t obj, const byte_t* data)
{
	byte_t** pobj = (byte_t**)obj;
	byte_t type;
	byte_t* buf;
	dword_t dw;

	XDL_ASSERT(data != NULL);

	type = GET_BYTE(data, 0) & 0x7F;
	if (type != _OBJECT_BINARY && type != _OBJECT_STRING && type != _OBJECT_VARIANT && type != _OBJECT_DOMDOC)
		return 0;

	dw = GET_DWORD_LOC(data, 4) + 8;

	buf = varbuf_realloc(pobj, dw);
	xmem_copy((void*)buf, (void*)data, dw);

	return dw;
}

dword_t object_encode(object_t obj, byte_t* buf, dword_t max)
{
	byte_t** pobj = (byte_t**)obj;
	bool_t compr;
	dword_t dw;

	XDL_ASSERT(pobj);

	compr = GET_BYTE((*pobj), 0) & 0x80;

	if (compr)
		_object_decompress(pobj);

	dw = GET_DWORD_LOC((*pobj), 4) + 8;
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