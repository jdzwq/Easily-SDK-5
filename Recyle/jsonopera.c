/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc json operator document

	@module	jsonopera.c | json operator implement file

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

#include "jsonopera.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC


void call_write_json_begin(void* pv)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	pop->stack = create_stack_table();
}

bool_t call_write_json_node_begin(void* pv)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	push_stack_node(pop->stack, (void*)pop->nlk);

	if (!pop->nlk)
		pop->nlk = pop->doc;
	else
		pop->nlk = insert_dom_node(pop->nlk, LINK_LAST);

	return 1;
}

bool_t call_write_json_node_retain(void* pv)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;
	link_t_ptr plk;

	plk = (link_t_ptr)peek_stack_node(pop->stack, -1);
	if (!plk)
		plk = pop->doc;

	pop->nlk = insert_dom_node(plk, LINK_LAST);

	return 1;
}

bool_t call_write_json_node_end(void* pv)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	pop->nlk = (pop->stack) ? (link_t_ptr)pop_stack_node(pop->stack) : NULL;

	if (!pop->nlk)
		return 0;

	return 1;
}

link_t_ptr call_peek_json_node(void* pv)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	if (!pop->stack)
		return NULL;

	return peek_stack_node(pop->stack, -1);
}

bool_t call_write_json_node_name(void* pv, const tchar_t* na, int nalen)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	if (!pop->nlk)
		return 0;

	set_dom_node_name(pop->nlk, na, nalen);

	return 1;
}

bool_t call_write_json_node_text(void* pv, const tchar_t* text, int len)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	if (!pop->nlk)
		return 0;

	set_dom_node_text(pop->nlk, text, len);

	return 1;
}

bool_t call_write_json_node_mask(void* pv, dword_t mask, bool_t b_check)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	if (!pop->nlk)
		return 0;

	set_dom_node_mask_check(pop->nlk, mask, b_check);

	return 1;
}

bool_t call_peek_json_node_mask(void* pv, dword_t mask)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	if (!pop->nlk)
		return 0;

	return get_dom_node_mask_check(pop->nlk, mask);
}

const tchar_t* call_peek_json_node_name(void* pv)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	if (!pop->nlk)
		return NULL;

	return get_dom_node_name_ptr(pop->nlk);
}

void call_write_json_end(void* pv, int code)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	if (pop->stack)
	{
		destroy_stack_table(pop->stack);
		pop->stack = NULL;
	}
}

bool_t call_write_json_has_node(void* pv)
{
	JSONOBJECT* pop = (JSONOBJECT*)pv;

	return (pop->nlk) ? 1 : 0;
}

/************************************************************************************************************/

bool_t call_read_json_begin(void* pv)
{
	if_operator_t* pb = (if_operator_t*)pv;
	tchar_t pch[2] = { 0 };
	int pos;

	pch[0] = _T('{');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == C_ERR)
	{
		pb->pos = C_ERR;
		return 0;
	}
	pb->pos += pos;
}

bool_t call_read_json_node_begin(void* pv, int indent, int jarray, const tchar_t* nn)
{
	if_operator_t* pb = (if_operator_t*)pv;
	tchar_t pch[2] = { 0 };
	int pos = 0;

	if (pb->pf_write_carriage)
	{
		pos = (*pb->pf_write_carriage)(pb->obj, pb->max, pb->pos, pb->encode);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;
	}

	while (indent && pb->pf_write_indent)
	{
		pos = (*pb->pf_write_indent)(pb->obj, pb->max, pb->pos, pb->encode);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;

		indent--;
	}

	if (jarray != JSON_ARRAY_NEXT && jarray != JSON_ARRAY_LAST)
	{
		pch[0] = _T('\"');
		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, nn, -1);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("\" : "), -1);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;
	}

	if (jarray == JSON_ARRAY_FIRST || jarray == JSON_ARRAY_ONCE)
	{
		pch[0] = _T('[');
		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;
	}

	return 1;
}

bool_t call_read_dom_node_close(void* pv, int indent, bool_t b_parent)
{
	if_operator_t* pb = (if_operator_t*)pv;
	tchar_t pch[2] = { 0 };
	int pos = 0;

	pch[0] = _T('>');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == C_ERR)
	{
		pb->pos = C_ERR;
		return 0;
	}
	pb->pos += pos;

	return 1;
}

bool_t call_read_dom_node_text(void* pv, const tchar_t* text, int len)
{
	if_operator_t* pb = (if_operator_t*)pv;
	tchar_t pch[2] = { 0 };
	bool_t b_esc;
	int i, pos = 0;

	if (len < 0)
		len = xslen(text);

	pch[0] = _T('\"');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == C_ERR)
	{
		pb->pos = C_ERR;
		return 0;
	}
	pb->pos += pos;

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, text, len);
	if (pos == C_ERR)
	{
		pb->pos = C_ERR;
		return 0;
	}
	pb->pos += pos;

	pch[0] = _T('\"');
	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
	if (pos == C_ERR)
	{
		pb->pos = C_ERR;
		return 0;
	}
	pb->pos += pos;

	return 1;
}

bool_t call_read_dom_node_end(void* pv, int indent, bool_t b_parent, bool_t b_close, const tchar_t* ns, const tchar_t* nn)
{
	if_operator_t* pb = (if_operator_t*)pv;
	tchar_t pch[2] = { 0 };
	int pos = 0;
	bool_t b_ns = 0;

	if (b_close)
	{
		if (b_parent && pb->pf_write_carriage)
		{
			pos = (*pb->pf_write_carriage)(pb->obj, pb->max, pb->pos, pb->encode);
			if (pos == C_ERR)
			{
				pb->pos = C_ERR;
				return 0;
			}
			pb->pos += pos;
		}

		while (b_parent && indent && pb->pf_write_indent)
		{
			pos = (*pb->pf_write_indent)(pb->obj, pb->max, pb->pos, pb->encode);
			if (pos == C_ERR)
			{
				pb->pos = C_ERR;
				return 0;
			}
			pb->pos += pos;

			indent--;
		}

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("</"), -1);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;

		if (!is_null(ns))
			b_ns = 1;

		if (b_ns)
		{
			pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, ns, -1);
			if (pos == C_ERR)
			{
				pb->pos = C_ERR;
				return 0;
			}
			pb->pos += pos;

			pch[0] = _T(':');
			pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
			if (pos == C_ERR)
			{
				pb->pos = C_ERR;
				return 0;
			}
			pb->pos += pos;
		}

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, nn, -1);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;

		pch[0] = _T('>');
		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, pb->encode, pch);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;
	}
	else
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, pb->encode, _T("/>"), -1);
		if (pos == C_ERR)
		{
			pb->pos = C_ERR;
			return 0;
		}
		pb->pos += pos;
	}

	return 1;
}



#endif /*XDL_SUPPORT_DOC*/