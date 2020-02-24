/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml bio document

	@module	xmlbio.c | implement file

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

#include "xmlbio.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

bool_t parse_xml_doc_from_memo(link_t_ptr xml, link_t_ptr txt)
{
	if_operator_t bo = { 0 };
	MEMOOBJECT to = { 0 };

	to.txt = txt;
	to.nlk = NULL;
	to.len = to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.encode = DEF_UCS;
	bo.pf_read_char = call_memo_read_char;
	bo.pf_can_escape = call_memo_can_escape;
	bo.pf_with_eof = call_memo_with_eof;

	return parse_xml_doc_from_object(xml, &bo);
}

bool_t parse_dom_doc_from_memo(link_t_ptr dom, link_t_ptr txt)
{
	if_operator_t bo = { 0 };
	MEMOOBJECT to = { 0 };

	to.txt = txt;
	to.nlk = NULL;
	to.len = to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.isdom = 1;
	bo.encode = DEF_UCS;
	bo.pf_read_char = call_memo_read_char;
	bo.pf_can_escape = call_memo_can_escape;
	bo.pf_with_eof = call_memo_with_eof;

	return parse_dom_doc_from_object(dom, &bo);
}

bool_t format_dom_doc_to_memo(link_t_ptr dom, link_t_ptr txt)
{
	if_operator_t bo = { 0 };
	MEMOOBJECT to = { 0 };

	to.txt = txt;
	to.nlk = NULL;
	to.len = to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.pos = 0;
	bo.encode = DEF_UCS;
	bo.pf_write_char = call_memo_write_char;
	bo.pf_write_token = call_memo_write_token;
	bo.pf_write_carriage = call_memo_write_carriage;
	bo.pf_write_indent = call_memo_write_indent;
	bo.pf_can_escape = call_memo_can_escape;

	return format_dom_doc_to_object(dom, &bo);
}

bool_t format_xml_doc_to_memo(link_t_ptr xml, link_t_ptr txt)
{
	if_operator_t bo = { 0 };
	MEMOOBJECT to = { 0 };

	to.txt = txt;
	to.nlk = NULL;
	to.len = to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.pos = 0;
	bo.encode = DEF_UCS;
	bo.pf_write_char = call_memo_write_char;
	bo.pf_write_token = call_memo_write_token;
	bo.pf_write_carriage = call_memo_write_carriage;
	bo.pf_write_indent = call_memo_write_indent;
	bo.pf_can_escape = call_memo_can_escape;

	return format_xml_doc_to_object(xml, &bo);
}

bool_t parse_xml_doc_from_string(link_t_ptr xml, string_t vs)
{
	if_operator_t bo = { 0 };
	STRINGOBJECT to = { 0 };

	to.var = vs;
	to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.encode = DEF_UCS;
	bo.pf_read_char = call_string_read_char;
	bo.pf_can_escape = call_string_can_escape;
	bo.pf_with_eof = call_string_with_eof;

	return parse_xml_doc_from_object(xml, &bo);
}

bool_t parse_dom_doc_from_string(link_t_ptr dom, string_t vs)
{
	if_operator_t bo = { 0 };
	STRINGOBJECT to = { 0 };

	to.var = vs;
	to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.isdom = 1;
	bo.encode = DEF_UCS;
	bo.pf_read_char = call_string_read_char;
	bo.pf_can_escape = call_string_can_escape;
	bo.pf_with_eof = call_string_with_eof;

	return parse_dom_doc_from_object(dom, &bo);
}

bool_t format_dom_doc_to_string(link_t_ptr dom, string_t vs)
{
	if_operator_t bo = { 0 };
	STRINGOBJECT to = { 0 };

	to.var = vs;
	to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.pos = 0;
	bo.encode = DEF_UCS;
	bo.pf_write_char = call_string_write_char;
	bo.pf_write_token = call_string_write_token;
	bo.pf_can_escape = call_string_can_escape;

	return format_dom_doc_to_object(dom, &bo);
}

bool_t format_xml_doc_to_string(link_t_ptr xml, string_t vs)
{
	if_operator_t bo = { 0 };
	STRINGOBJECT to = { 0 };

	to.var = vs;
	to.pos = 0;

	bo.obj = (void*)&to;
	bo.max = 0;
	bo.pos = 0;
	bo.encode = DEF_UCS;
	bo.pf_write_char = call_string_write_char;
	bo.pf_write_token = call_string_write_token;
	bo.pf_can_escape = call_string_can_escape;

	return format_xml_doc_to_object(xml, &bo);
}

bool_t parse_xml_doc_from_stream(link_t_ptr xml, stream_t xs)
{
	if_operator_t bo = { 0 };
	int encode;

	encode = stream_get_encode(xs);

	if (encode == _UNKNOWN)
	{
		encode = _UTF8;
		stream_set_encode(xs, encode);
	}

	if (encode == _UTF16_LIT || encode == _UTF16_BIG)
	{
		stream_read_utfbom(xs, NULL);
	}

	bo.obj = (void*)xs;
	bo.max = 0;
	bo.encode = encode;
	bo.pf_read_char = call_stream_read_char;
	bo.pf_read_escape = call_stream_read_escape;
	bo.pf_can_escape = call_stream_can_escape;
	bo.pf_with_eof = call_stream_with_eof;
	bo.pf_set_encode = call_stream_set_encode;

	return parse_xml_doc_from_object(xml, &bo);
}

bool_t parse_dom_doc_from_stream(link_t_ptr dom, stream_t xs)
{
	if_operator_t bo = { 0 };

	bo.obj = (void*)xs;
	bo.max = 0;
	bo.isdom = 1;
	bo.encode = stream_get_encode(xs);;
	bo.pf_read_char = call_stream_read_char;
	bo.pf_read_escape = call_stream_read_escape;
	bo.pf_can_escape = call_stream_can_escape;
	bo.pf_with_eof = call_stream_with_eof;

	return parse_dom_doc_from_object(dom, &bo);
}

bool_t format_dom_doc_to_stream(link_t_ptr dom, stream_t xs)
{
	if_operator_t bo = { 0 };

	bo.obj = (void*)xs;
	bo.max = 0;
	bo.pos = 0;
	bo.encode = stream_get_encode(xs);
	bo.pf_write_char = call_stream_write_char;
	bo.pf_write_escape = call_stream_write_escape;
	bo.pf_write_token = call_stream_write_token;
	bo.pf_can_escape = call_stream_can_escape;

	return format_dom_doc_to_object(dom, &bo);
}

bool_t format_xml_doc_to_stream(link_t_ptr xml, stream_t xs)
{
	if_operator_t bo = { 0 };

	int encode;

	encode = get_xml_encode(xml);
	stream_set_encode(xs, encode);

	if (encode == _UTF16_LIT || encode == _UTF16_BIG)
	{
		stream_write_utfbom(xs, NULL);
	}

	bo.obj = (void*)xs;
	bo.max = 0;
	bo.pos = 0;
	bo.encode = stream_get_encode(xs);
	bo.pf_write_char = call_stream_write_char;
	bo.pf_write_escape = call_stream_write_escape;
	bo.pf_write_token = call_stream_write_token;
	bo.pf_can_escape = call_stream_can_escape;

	return format_xml_doc_to_object(xml, &bo);
}

bool_t parse_xml_doc_from_bytes(link_t_ptr xml, const byte_t* str, dword_t len)
{
	if_operator_t bo = { 0 };

	int encode, bytes = 0;

	if (!str || !len)
		return 0;

	encode = parse_utfbom(str, 3);

	if (encode == _UNKNOWN)
	{
		encode = _UTF8;
	}
	else
	{
		bytes = skip_utfbom(str);
		str += bytes;
		len -= bytes;
	}

	bo.obj = (void*)str;
	bo.max = len;
	bo.encode = encode;
	bo.pf_read_char = call_buffer_read_char;
	bo.pf_read_escape = call_buffer_read_escape;
	bo.pf_can_escape = call_buffer_can_escape;
	bo.pf_with_eof = call_buffer_with_eof;

	return parse_xml_doc_from_object(xml, &bo);
}

bool_t parse_dom_doc_from_bytes(link_t_ptr dom, const byte_t* str, dword_t len, int encode)
{
	if_operator_t bo = { 0 };

	bo.obj = (void*)str;
	bo.max = len;
	bo.encode = encode;
	bo.isdom = 1;
	bo.pf_read_char = call_buffer_read_char;
	bo.pf_read_escape = call_buffer_read_escape;
	bo.pf_can_escape = call_buffer_can_escape;
	bo.pf_with_eof = call_buffer_with_eof;

	return parse_dom_doc_from_object(dom, &bo);
}

dword_t format_dom_doc_to_bytes(link_t_ptr dom, byte_t* buf, dword_t max, int encode)
{
	if_operator_t bo = { 0 };

	bo.obj = (void*)buf;
	bo.max = max;
	bo.pos = 0;
	bo.encode = encode;
	bo.pf_write_char = call_buffer_write_char;
	bo.pf_write_escape = call_buffer_write_escape;
	bo.pf_write_token = call_buffer_write_token;
	bo.pf_can_escape = call_buffer_can_escape;

	if (format_dom_doc_to_object(dom, &bo))
		return bo.pos;
	else
		return 0;
}

dword_t format_xml_doc_to_bytes(link_t_ptr xml, byte_t* buf, dword_t max)
{
	if_operator_t bo = { 0 };

	dword_t total = 0;
	int encode;

	encode = get_xml_encode(xml);

	if (encode == _UTF16_LIT || encode == _UTF16_BIG)
	{
		total += format_utfbom(encode, ((buf) ? (buf + total) : NULL));
		if (total >= max)
			return total;

		max -= total;
		if (buf)
		{
			buf += total;
		}
	}

	bo.obj = (void*)buf;
	bo.max = max;
	bo.pos = 0;
	bo.encode = encode;
	bo.pf_write_char = call_buffer_write_char;
	bo.pf_write_escape = call_buffer_write_escape;
	bo.pf_write_token = call_buffer_write_token;
	bo.pf_can_escape = call_buffer_can_escape;

	if (format_xml_doc_to_object(xml, &bo))
		return (bo.pos + total);
	else
		return 0;
}

#endif //XDLDOC
