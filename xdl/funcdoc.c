/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc func document

	@module	funcdoc.c | implement file

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

#include "funcdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_func_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_FUNC, -1);

	return ptr;
}

void destroy_func_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_func_paramset(link_t_ptr ptr)
{
	return ptr;
}

void clear_func_doc(link_t_ptr ptr)
{
	ptr = get_func_paramset(ptr);

	delete_dom_child_nodes(ptr);
}

bool_t is_func_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_FUNC, -1, 0) == 0) ? 1 : 0;
}

int get_func_param_count(link_t_ptr ptr)
{
	ptr = get_func_paramset(ptr);

	return get_dom_child_node_count(ptr);
}

link_t_ptr	get_func_param(link_t_ptr ptr,const tchar_t* key,int keylen)
{
	ptr = get_func_paramset(ptr);

	return find_dom_node_by_attr(ptr,0,ATTR_NAME,-1,key,keylen);
}

link_t_ptr insert_func_param(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr nlk;

	ptr = get_func_paramset(ptr);

	nlk = insert_dom_node(ptr, pos);
	set_dom_node_name(nlk, DOC_FUNC_PARAM,-1);

	return nlk;
}

void delete_func_param(link_t_ptr nlk)
{
	delete_dom_node(nlk);
}

link_t_ptr get_func_next_param(link_t_ptr ptr, link_t_ptr pos)
{
	ptr = get_func_paramset(ptr);

	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_func_prev_param(link_t_ptr ptr, link_t_ptr pos)
{
	ptr = get_func_paramset(ptr);

	if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else if (pos == LINK_FIRST)
		return NULL;
	else
		return get_dom_prev_sibling_node(pos);
}


void set_func_param_text(link_t_ptr nlk, const tchar_t* token, int len)
{
	int dlen;

	dlen = get_func_param_data_len(nlk);
	if (dlen <= 0)
		dlen = len;
	else
		dlen = (dlen < len) ? dlen : len;

	set_dom_node_text(nlk, token, dlen);
}

int get_func_param_text(link_t_ptr nlk, tchar_t* buf, int max)
{
	int dlen;

	dlen = get_func_param_data_len(nlk);
	if (dlen <= 0)
		dlen = max;
	else
		dlen = (dlen < max) ? dlen : max;

	return get_dom_node_text(nlk, buf, dlen);
}

void set_func_param_boolean(link_t_ptr nlk, bool_t b)
{
	set_dom_node_text(nlk, ((b)? _T("1") : _T("0")), -1);
}

bool_t get_func_param_boolean(link_t_ptr nlk)
{
	return (compare_text(get_dom_node_text_ptr(nlk), -1, _T("1"), -1, 0) == 0) ? 1 : 0;
}

void set_func_param_integer(link_t_ptr nlk, int n)
{
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	xsprintf(sz_num, _T("%d"), n);
	set_dom_node_text(nlk, sz_num, -1);
}

int get_func_param_integer(link_t_ptr nlk)
{
	return xstol(get_dom_node_text_ptr(nlk));
}

void set_func_param_numeric(link_t_ptr nlk, double n)
{
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	numtoxs_dig(n, get_func_param_data_dig(nlk), sz_num, NUM_LEN);
	set_dom_node_text(nlk, sz_num, -1);
}

double get_func_param_numeric(link_t_ptr nlk)
{
	return xstonum(get_dom_node_text_ptr(nlk));
}

void merge_func_return(link_t_ptr dst, link_t_ptr src)
{
	link_t_ptr flk_src, flk_dst;

	flk_src = get_func_next_param(src, LINK_FIRST);
	while (flk_src)
	{
		if (compare_text(get_func_param_type_ptr(flk_src), -1, ATTR_PARAM_TYPE_OUTPUT, -1, 0) == 0 || compare_text(get_func_param_type_ptr(flk_src), -1, ATTR_PARAM_TYPE_INPUTOUTPUT, -1, 0) == 0)
		{
			flk_dst = get_func_param(dst, get_func_param_name_ptr(flk_src), -1);
			if (!flk_dst)
			{
				flk_dst = insert_func_param(dst, LINK_LAST);
				set_func_param_name(flk_dst, get_func_param_name_ptr(flk_src));
				set_func_param_type(flk_dst, get_func_param_type_ptr(flk_src));
				set_func_param_data_type(flk_dst, get_func_param_data_type_ptr(flk_src));
			}
				
			set_func_param_text(flk_dst, get_func_param_text_ptr(flk_src), -1);
		}
		flk_src = get_func_next_param(src, flk_src);
	}

	set_func_return_text(dst, get_func_return_text_ptr(src));
}

void import_func_param(link_t_ptr ptr, const variant_t* pv, int n)
{
	link_t_ptr nlk;
	int i;

	clear_func_doc(ptr);

	for (i = 0; i < n; i++)
	{
		nlk = insert_func_param(ptr, LINK_LAST);

		switch (pv->vv)
		{
		case VV_STRING:
			set_func_param_data_type(nlk, ATTR_DATA_TYPE_STRING);
			set_func_param_text(nlk, pv->string_one, -1);
			break;
		case VV_BOOL:
			set_func_param_data_type(nlk, ATTR_DATA_TYPE_BOOLEAN);
			set_func_param_boolean(nlk, pv->bool_one);
			break;
		case VV_INT:
			set_func_param_data_type(nlk, ATTR_DATA_TYPE_INTEGER);
			set_func_param_integer(nlk, pv->int_one);
			break;
		case VV_FLOAT:
			set_func_param_data_type(nlk, ATTR_DATA_TYPE_NUMERIC);
			set_func_param_data_dig(nlk, pv->prec);
			set_func_param_numeric(nlk, pv->float_one);
			break;
		case VV_DOUBLE:
			set_func_param_data_type(nlk, ATTR_DATA_TYPE_NUMERIC);
			set_func_param_data_dig(nlk, pv->prec);
			set_func_param_numeric(nlk, pv->double_one);
			break;
		}

		switch (pv->io)
		{
		case IO_INOUT:
			set_func_param_type(nlk, ATTR_PARAM_TYPE_INPUTOUTPUT);
			break;
		case IO_OUT:
			set_func_param_type(nlk, ATTR_PARAM_TYPE_OUTPUT);
			break;
		default:
			set_func_param_type(nlk, ATTR_PARAM_TYPE_INPUT);
		}
	}
}

int export_func_param(link_t_ptr ptr, variant_t* pv, int n)
{
	int len,count = 0;
	link_t_ptr nlk;
	const tchar_t *data_type, *param_type;

	nlk = get_func_next_param(ptr, LINK_FIRST);
	while (nlk)
	{
		if (count > n)
			break;

		param_type = get_func_param_type_ptr(nlk);

		if (pv && (compare_text(param_type, -1, ATTR_PARAM_TYPE_OUTPUT, -1, 0) == 0 || compare_text(param_type, -1, ATTR_PARAM_TYPE_INPUTOUTPUT, -1, 0) == 0))
		{
			variant_to_null(&pv[count]);

			data_type = get_func_param_data_type_ptr(nlk);
			if (compare_text(data_type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
			{
				len = get_func_param_data_len(nlk);
				if (len <= 0)
				{
					pv[count].string_one = xsclone(get_func_param_text_ptr(nlk));
					pv[count].size = -1;
					pv[count].vv = VV_STRING;
				}
				else
				{
					pv[count].string_one = xsalloc(len + 1);
					pv[count].size = get_func_param_text(nlk, pv[count].string_one, len);
					pv[count].vv = VV_STRING;
				}
			}
			else if (compare_text(data_type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
			{
				pv[count].int_one = get_func_param_integer(nlk);
				pv[count].vv = VV_INT;
			}
			else if (compare_text(data_type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
			{
				pv[count].bool_one = get_func_param_boolean(nlk);
				pv[count].vv = VV_BOOL;
			}
			else if (compare_text(data_type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
			{
				pv[count].double_one = get_func_param_numeric(nlk);
				pv[count].vv = VV_DOUBLE;
			}
		}

		count++;
		nlk = get_func_next_param(ptr, nlk);
	}

	return count;
}

#endif /*XDL_SUPPORT_DOC*/
