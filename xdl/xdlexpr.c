/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc expression document

	@module	xdlexpr.c | expression implement file

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

#include "xdlexpr.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

/*define some exption operator reserve words function*/
#define OP_AND		_T("AND")

#define OP_OR		_T("OR")

#define OP_BR		_T("()")

#define SG_EQ		_T("=")

#define SG_AEQ		_T("==")

#define SG_PRE		_T("*=")

#define SG_SUF		_T("=*")

#define SG_GEQ		_T(">=")

#define SG_GRE		_T(">")

#define SG_LEQ		_T("<=")

#define SG_LES		_T("<")

#define SG_NEQ		_T("<>")

#define CT_NODE		_T("node")

#define CT_SUB		_T("sub")

#define CT_TAG		_T("tag")

typedef enum{ctErr = -1,ctNone = 0,ctNode = 1,ctSub = 2}expr_tag;

#define EXPR_KEY		_T("key")

#define EXPR_VAL		_T("val")

#define EXPR_SIN		_T("sin")

#define EXPR_TAG		_T("tag")

#define EXPR_OP		_T("op")

typedef struct _expr_node
{
	int tag;
	tchar_t* keyat;
	int keylen;
	tchar_t* sinat;
	int sinlen;
	tchar_t* valat;
	int vallen;
	tchar_t* opat;
	int oplen;
}expr_node;

#define is_blank(c) ((c == _T(' ') || c == _T('\t') || c == _T('\n') || c == _T('\r'))? 1 : 0)

#define is_keyterm(c) ((c == _T(' ') || c == _T('\t') || c == _T('\n') || c == _T('\r') || c == _T('\0'))? 1 : 0)

#define is_sinterm(c) ((c == _T(' ') || c == _T('\t') || c == _T('\n') || c == _T('\r') || c == _T(')') || c == _T('\0'))? 1 : 0)

#define is_valterm(c) ((c == _T(' ') || c == _T('\t') || c == _T('\n') || c == _T('\r') || c == _T(')') || c == _T('\0'))? 1 : 0)

#define is_subbegin(c) ((c == _T('('))? 1 : 0)

#define is_subterm(c) ((c == _T(')') || c == _T('\0'))? 1 : 0)

#define is_opterm(c) ((c == _T(' ') || c == _T('\t') || c == _T('\n') || c == _T('\r') || c == _T('(') || c == _T('\0'))? 1 : 0)

static tchar_t* _next_token(tchar_t* token,expr_node* pcn,int tklen)
{
	int count = 0;

	xmem_zero((void*)pcn, sizeof(expr_node));

	/*skip blank*/
	while(is_blank(*token) && count <= tklen)
	{
		token ++;
		count ++;
	}
	if(count == tklen)
		return token;

	/*test if sub token*/
	if(is_subbegin(*token))
	{
		pcn->tag = ctSub;
		pcn->keyat = ++token ; /*skip '('*/
		while(!is_subterm(*token) && count <= tklen)
		{
			token ++ ;
			count ++;
		}
		if(*token == _T('\0'))
		{
			pcn->tag = ctErr;
			return token;
		}
		pcn->keylen = (int)(token - pcn->keyat);
		token ++; /*skip ')'*/

		pcn->tag = ctSub;
	}else
	{
		/*parse key*/
		pcn->keyat = token;
		while(!is_keyterm(*token) && count <= tklen)
		{
			token ++;
			count ++;
		}
		if(count == tklen)
			return token;
		pcn->keylen = (int)(token - pcn->keyat);
		
		/*parse sign*/
		while(is_blank(*token) && count <= tklen)
		{
			token ++;
			count ++;
		}
		if(count == tklen)
			return token;
		pcn->sinat = token;
		while(!is_sinterm(*token) && count <= tklen)
		{
			token ++ ;
			count ++;
		}
		if(count == tklen)
			return token;
		pcn->sinlen = (int)(token - pcn->sinat);

		/*parse val*/
		while(is_blank(*token) && count <= tklen)
		{
			token ++;
			count ++;
		}
		pcn->valat = token;
		while(!is_valterm(*token) && count <= tklen)
		{
			token ++ ;
			count ++;
		}
		pcn->vallen = (int)(token - pcn->valat);

		pcn->tag = ctNode;

		if(count == tklen)
			return token;
	}

	/*parse opera*/
	while(is_blank(*token) && count <= tklen)
	{
		token ++;
		count ++;
	}
	pcn->opat = token;
	while(!is_opterm(*token) && count <= tklen)
	{
		token ++ ;
		count ++;
	}
	pcn->oplen = (int)(token - pcn->opat);	
	return token;
}

static void expr_parse_node(link_t_ptr multi,link_t_ptr parent,tchar_t* token,int len)
{
	expr_node cn;
	link_t_ptr tlk;
	tchar_t* next = token;
	int skip = 0;
	while(*token != _T('\0'))
	{
		next = _next_token(token,&cn,len - skip);
		skip = (int)(next - token);
		token = next;
		switch(cn.tag)
		{
		case ctSub:
			tlk = insert_multi_node(multi,LINK_LAST);
			set_multi_node_attr(tlk,EXPR_TAG,-1,CT_SUB,-1);
			set_multi_node_attr(tlk,EXPR_KEY,-1,cn.keyat,cn.keylen);
			set_multi_node_attr(tlk,EXPR_OP,-1,cn.opat,cn.oplen);

			expr_parse_node(multi,tlk,cn.keyat,cn.keylen);
			break;
		case ctNode:
			tlk = insert_multi_node(multi,LINK_LAST);
			set_multi_node_attr(tlk,EXPR_TAG,-1,CT_NODE,-1);
			set_multi_node_attr(tlk,EXPR_KEY,-1,cn.keyat,cn.keylen);
			set_multi_node_attr(tlk,EXPR_SIN,-1,cn.sinat,cn.sinlen);
			set_multi_node_attr(tlk,EXPR_VAL,-1,cn.valat,cn.vallen);
			set_multi_node_attr(tlk,EXPR_OP,-1,cn.opat,cn.oplen);
			break;
		}
		if(cn.tag == ctErr || cn.tag == ctNone)
			break;
	}
}

bool_t expr_parse(link_t_ptr ptr,const tchar_t* str)
{
	if(is_null(str))
		return 1;

	expr_parse_node(ptr,NULL,(tchar_t*)str,xslen(str));
	return 1;
}

int expr_format_length(link_t_ptr ptr)
{
	return expr_format(ptr,NULL,MAX_LONG);
}

int expr_format(link_t_ptr ptr,tchar_t* buf,int max)
{
	link_t_ptr node;
	const tchar_t *token,*key,*val,*sin,*op;
	int total = 0;
	int keylen,vallen,sinlen,oplen;

	node = get_multi_first_child_node(ptr);
	while(node)
	{
		token = get_multi_node_attr_ptr(node,EXPR_TAG,-1);
		if(xscmp(token,CT_SUB) == 0)
		{
			key = get_multi_node_attr_ptr(node,EXPR_KEY,-1);
			keylen = (key)? xslen(key) : 0;
			if(keylen + total + 4 > max) /*include two blank and two bracket*/
				return -1;
			if(buf)
				xsprintf(buf + total,_T(" (%s) "),((key)? key : _T("")));
			total += (keylen + 4);
		}else if(xscmp(token,CT_NODE) == 0)
		{
			key = get_multi_node_attr_ptr(node,EXPR_KEY,-1);
			sin = get_multi_node_attr_ptr(node,EXPR_SIN,-1);
			val = get_multi_node_attr_ptr(node,EXPR_VAL,-1);
			keylen = (key)? xslen(key) : 0;
			sinlen = (sin)? xslen(sin) : 0;
			vallen = (val)? xslen(val) : 0;
			if(keylen + sinlen + vallen + total + 4 > max) /*include four blank*/
				return -1;
			if(buf)
				xsprintf(buf + total,_T(" %s %s %s "),((key)? key : _T("")),((sin)? sin : _T("")),((val)? val : _T("")));
			total += (keylen + sinlen + vallen + 4);
		}

		op = get_multi_node_attr_ptr(node,EXPR_OP,-1);
		oplen = (op)? xslen(op) : 0;
		if(oplen + total + 2 > max) /*include two blank*/
			return -1;
		if(buf)
			xsprintf(buf + total,_T(" %s "),((op)? op : _T("")));
		total += (oplen + 2);

		node = get_multi_next_sibling_node(node);
	}
	return total;
}

int expr_exec_node(link_t_ptr tlk,expr_compare_ptr pf,void* parm)
{
	const tchar_t *tag,*key,*val,*sin,*op;
	int valid;

	tag = get_multi_node_attr_ptr(tlk,EXPR_TAG,-1);
	if(xscmp(tag,CT_SUB) == 0)
	{
		valid = expr_exec_node(get_multi_first_child_node(tlk),pf,parm);
	}else
	{
		key = get_multi_node_attr_ptr(tlk,EXPR_KEY,-1);
		sin = get_multi_node_attr_ptr(tlk,EXPR_SIN,-1);
		val = get_multi_node_attr_ptr(tlk,EXPR_VAL,-1);
		valid = (*pf)(key,sin,val,parm);
	}
	op = get_multi_node_attr_ptr(tlk,EXPR_OP,-1);
	if(xsicmp(op,OP_AND) == 0)
	{
		if(valid == 0)
			return 0;
		tlk = get_multi_next_sibling_node(tlk);
		if(tlk == NULL)
			return valid;
		else
			return expr_exec_node(tlk,pf,parm);
	}else if(xsicmp(op,OP_OR) == 0)
	{
		if(valid == 1)
			return 1;
		tlk = get_multi_next_sibling_node(tlk);
		if(tlk == NULL)
			return valid;
		else
			return expr_exec_node(tlk,pf,parm);
	}else
		return valid;
}

int expr_exec(link_t_ptr ptr,expr_compare_ptr pf,void* parm)
{
	link_t_ptr tlk;
	tlk = get_multi_first_child_node(ptr);
	if(tlk == NULL)
		return 0;
	return expr_exec_node(tlk,pf,parm);
}

