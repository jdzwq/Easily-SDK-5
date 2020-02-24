/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml parse document

	@module	xmlobj.c | xml parse implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#include "xmlobj.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

typedef struct _XMLOBJECT{
	link_t_ptr doc;
	link_t_ptr nlk;

	link_t_ptr stack;
}XMLOBJECT;

void __callback _on_parse_xml_begin(void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	pop->stack = create_stack_table();
}

int __callback _on_parse_xml_head_attr(const tchar_t* key, int klen, const tchar_t* val, int vlen, void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	if (!pop->doc)
		return 1;

	if (!IS_XML_DOC(pop->doc))
		return 1;

	if (xsnicmp(XML_ATTR_ENCODING, key, klen) == 0)
		set_xml_encoding(pop->doc, val, vlen);
	else if (xsnicmp(XML_ATTR_VERSION, key, klen) == 0)
		set_xml_version(pop->doc, val, vlen);

	return 0;
}

int __callback _on_parse_xml_node_begin(void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	push_stack_node(pop->stack, (void*)pop->nlk);

	if (!pop->nlk)
		pop->nlk = IS_XML_DOC(pop->doc)? get_xml_dom_node(pop->doc) : pop->doc;
	else
		pop->nlk = insert_dom_node(pop->nlk, LINK_LAST);

	return 0;
}

int __callback _on_parse_xml_node_end(void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	pop->nlk = (pop->stack) ? (link_t_ptr)pop_stack_node(pop->stack) : NULL;

	if (!pop->nlk)
		return 1;

	return 0;
}

int __callback _on_parse_xml_node_name(const tchar_t* ns, int nslen, const tchar_t* na, int nalen, void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	if (!pop->nlk)
		return 1;

	if (is_null(ns) || !nslen)
	{
		set_dom_node_name(pop->nlk, na, nalen);
	}
	else
	{
		set_dom_node_ns(pop->nlk, ns, nslen);
		set_dom_node_name(pop->nlk, na, nalen);
	}

	return 0;
}

int __callback _on_parse_xml_node_attr(const tchar_t* key, int klen, const tchar_t* val, int vlen, void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	if (!pop->nlk)
		return 1;

	set_dom_node_attr(pop->nlk, key, klen, val, vlen);

	return 0;
}

int __callback _on_parse_xml_node_xmlns(const tchar_t* key, int klen, const tchar_t* val, int vlen, void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	if (!pop->nlk)
		return 1;

	set_dom_node_xmlns(pop->nlk, key, klen, val, vlen);

	return 0;
}

int __callback _on_parse_xml_node_text(bool_t b_cdata, const tchar_t* text, int len, void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	if (!pop->nlk)
		return 1;

	set_dom_node_text(pop->nlk, text, len);

	if (b_cdata)
	{
		set_dom_node_cdata(pop->nlk, 1);
	}

	return 0;
}

void __callback _on_parse_xml_end(void* pv, int code)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	if (pop->stack)
	{
		destroy_stack_table(pop->stack);
		pop->stack = NULL;
	}
}

bool_t __callback _on_parse_xml_has_node(void* pv)
{
	XMLOBJECT* pop = (XMLOBJECT*)pv;

	return (pop->nlk) ? 1 : 0;
}

bool_t parse_xml_doc_from_object(link_t_ptr ptr, OBJOPERATOR* pbo)
{
	XMLPARSER xp = { 0 };
	XMLOBJECT xo = { 0 };

	clear_xml_doc(ptr);
	xo.doc = ptr;

	xp.obj = (void*)&xo;
	xp.pf_on_begin = _on_parse_xml_begin;
	xp.pf_on_end = _on_parse_xml_end;
	xp.pf_on_head_parse_attr = _on_parse_xml_head_attr;
	xp.pf_on_node_parse_attr = _on_parse_xml_node_attr;
	xp.pf_on_node_parse_begin = _on_parse_xml_node_begin;
	xp.pf_on_node_parse_end = _on_parse_xml_node_end;
	xp.pf_on_node_parse_name = _on_parse_xml_node_name;
	xp.pf_on_node_parse_text = _on_parse_xml_node_text;
	xp.pf_on_node_parse_xmlns = _on_parse_xml_node_xmlns;
	xp.pf_on_has_node = _on_parse_xml_has_node;

	return parse_xml(&xp, pbo->encode, pbo);
}

bool_t parse_dom_doc_from_object(link_t_ptr ptr, OBJOPERATOR* pbo)
{
	XMLPARSER xp = { 0 };
	XMLOBJECT xo = { 0 };

	delete_dom_child_nodes(ptr);
	xo.doc = ptr;

	xp.obj = (void*)&xo;
	xp.pf_on_begin = _on_parse_xml_begin;
	xp.pf_on_end = _on_parse_xml_end;
	xp.pf_on_node_parse_attr = _on_parse_xml_node_attr;
	xp.pf_on_node_parse_begin = _on_parse_xml_node_begin;
	xp.pf_on_node_parse_end = _on_parse_xml_node_end;
	xp.pf_on_node_parse_name = _on_parse_xml_node_name;
	xp.pf_on_node_parse_text = _on_parse_xml_node_text;
	xp.pf_on_node_parse_xmlns = _on_parse_xml_node_xmlns;
	xp.pf_on_has_node = _on_parse_xml_has_node;

	return parse_xml(&xp, pbo->encode, pbo);
}

/***************************************************************************************************************************/
int __callback	_on_format_dom_node_attr(const tchar_t* key, const tchar_t* val, void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	bool_t b_esc;
	tchar_t pch[2] = { 0 };
	int pos = 0;

	b_esc = (*pb->pf_can_escape)(pb->obj);

	pch[0] = _T(' ');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, key, -1);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	pch[0] = _T('=');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	pch[0] = _T('\"');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	while (val && *val)
	{
		if (b_esc && _IsEscapeChar(*val))
		{
			pch[0] = _T('&');
			pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
			if (pos == INTEGER_ERROR)
			{
				pb->pos = INTEGER_ERROR;
				return 1;
			}
			pb->pos += pos;

			pos = (*pb->pf_write_escape)(pb->obj, pb->max, pb->pos, pb->encode, *val);
			if (pos == INTEGER_ERROR)
			{
				pb->pos = INTEGER_ERROR;
				return 1;
			}
			pb->pos += pos;

			val++;
		}
		else
		{
			pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, val);
			if (pos == INTEGER_ERROR)
			{
				pb->pos = INTEGER_ERROR;
				return 1;
			}
			pb->pos += pos;
#ifdef _UNICODE
			val += ucs_sequence(*(val));
#else
			val += mbs_sequence(*(val));
#endif
		}
	}

	pch[0] = _T('\"');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_dom_node_begin(int indent, bool_t b_parent, const tchar_t* ns, const tchar_t* nn, void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	tchar_t pch[2] = { 0 };
	int pos = 0;
	bool_t b_ns = 0;

	if (pb->pf_write_carriage)
	{
		pos = (*pb->pf_write_carriage)(pb->obj, pb->max, pb->pos, pb->encode);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;
	}

	while (indent && pb->pf_write_indent)
	{
		pos = (*pb->pf_write_indent)(pb->obj, pb->max, pb->pos, pb->encode);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;

		indent--;
	}

	pch[0] = _T('<');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	if (!is_null(ns))
		b_ns = 1;

	if (b_ns)
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, ns, -1);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;

		pch[0] = _T(':');
		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;
	}

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, nn, -1);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_dom_node_close(int indent, bool_t b_parent, void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	tchar_t pch[2] = { 0 };
	int pos = 0;

	pch[0] = _T('>');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_dom_node_text(bool_t b_cdata, const tchar_t* text, int len, void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	tchar_t pch[2] = { 0 };
	bool_t b_esc;
	int i,pos = 0;

	if (len < 0)
		len = xslen(text);

	if (!len)
		return 0;

	if (b_cdata)
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("<!"), -1);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, CDATA_HEAD, -1);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;
	}

	if (b_cdata)
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, text, len);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, CDATA_TAIL, -1);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;

		pch[0] = _T('>');
		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;
	}
	else
	{
		b_esc = (*pb->pf_can_escape)(pb->obj);

		for (i = 0; i < len;)
		{
			if (b_esc && _IsEscapeChar(text[i]))
			{
				pch[0] = _T('&');
				pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
				if (pos == INTEGER_ERROR)
				{
					pb->pos = INTEGER_ERROR;
					return 1;
				}
				pb->pos += pos;

				pos = (*pb->pf_write_escape)(pb->obj, pb->max, pb->pos, pb->encode, text[i]);
				if (pos == INTEGER_ERROR)
				{
					pb->pos = INTEGER_ERROR;
					return 1;
				}
				i++;
				pb->pos += pos;
			}
			else
			{
				pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, text + i);
				if (pos == INTEGER_ERROR)
				{
					pb->pos = INTEGER_ERROR;
					return 1;
				}
#ifdef _UNICODE
				i += ucs_sequence(*(text + i));
#else
				i += mbs_sequence(*(text + i));
#endif
				pb->pos += pos;
			}
		}
	}

	return 0;
}

int __callback	_on_format_dom_node_end(int indent, bool_t b_parent, bool_t b_close, const tchar_t* ns, const tchar_t* nn, void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	tchar_t pch[2] = { 0 };
	int pos = 0;
	bool_t b_ns = 0;

	if (b_close)
	{
		if (b_parent && pb->pf_write_carriage)
		{
			pos = (*pb->pf_write_carriage)(pb->obj, pb->max, pb->pos, pb->encode);
			if (pos == INTEGER_ERROR)
			{
				pb->pos = INTEGER_ERROR;
				return 1;
			}
			pb->pos += pos;
		}

		while (b_parent && indent && pb->pf_write_indent)
		{
			pos = (*pb->pf_write_indent)(pb->obj, pb->max, pb->pos, pb->encode);
			if (pos == INTEGER_ERROR)
			{
				pb->pos = INTEGER_ERROR;
				return 1;
			}
			pb->pos += pos;

			indent--;
		}

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("</"), -1);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;

		if (!is_null(ns))
			b_ns = 1;

		if (b_ns)
		{
			pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, ns, -1);
			if (pos == INTEGER_ERROR)
			{
				pb->pos = INTEGER_ERROR;
				return 1;
			}
			pb->pos += pos;

			pch[0] = _T(':');
			pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
			if (pos == INTEGER_ERROR)
			{
				pb->pos = INTEGER_ERROR;
				return 1;
			}
			pb->pos += pos;
		}

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, nn, -1);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;

		pch[0] = _T('>');
		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;
	}
	else
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("/>"), -1);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;
	}

	return 0;
}

int __callback	_on_format_xml_head_begin(void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	int pos = 0;
	bool_t b_ns = 0;

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("<?xml "), -1);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_xml_head_attr(const tchar_t* key, const tchar_t* val, void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	tchar_t pch[2] = { 0 };
	int pos = 0;

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, key, -1);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	pch[0] = _T('=');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	pch[0] = _T('\"');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	while (val && *val)
	{
		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, val);
		if (pos == INTEGER_ERROR)
		{
			pb->pos = INTEGER_ERROR;
			return 1;
		}
		pb->pos += pos;
		val++;
	}

	pch[0] = _T('\"');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	pch[0] = _T(' ');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_xml_head_end(void* pv)
{
	OBJOPERATOR* pb = (OBJOPERATOR*)pv;
	int pos = 0;

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("?>"), -1);
	if (pos == INTEGER_ERROR)
	{
		pb->pos = INTEGER_ERROR;
		return 1;
	}
	pb->pos += pos;

	return 0;
}

bool_t format_dom_doc_to_object(link_t_ptr dom, OBJOPERATOR* pbo)
{
	DOMREADER dr = { 0 };

	dr.pf_on_node_read_begin = _on_format_dom_node_begin;
	dr.pf_on_node_read_close = _on_format_dom_node_close;
	dr.pf_on_node_read_end = _on_format_dom_node_end;
	dr.pf_on_node_read_attr = _on_format_dom_node_attr;
	dr.pf_on_node_read_xmlns = _on_format_dom_node_attr;
	dr.pf_on_node_read_text = _on_format_dom_node_text;

	read_dom_doc(dom, &dr, (void*)pbo);

	return (pbo->pos < 0) ? 0 : 1;
}

bool_t format_xml_head_to_object(link_t_ptr dom, OBJOPERATOR* pbo)
{
	XMLREADER xr = { 0 };

	xr.pf_on_head_read_begin = _on_format_xml_head_begin;
	xr.pf_on_head_read_attr = _on_format_xml_head_attr;
	xr.pf_on_head_read_end = _on_format_xml_head_end;

	read_xml_head(dom, &xr, (void*)pbo);

	return (pbo->pos < 0) ? 0 : 1;
}

bool_t format_xml_doc_to_object(link_t_ptr ptr, OBJOPERATOR* pbo)
{
	if (!format_xml_head_to_object(ptr, pbo))
		return 0;

	if (!format_dom_doc_to_object(get_xml_dom_node(ptr), pbo))
		return 0;

	return 1;
}

#endif /*XDL_SUPPORT_DOC*/