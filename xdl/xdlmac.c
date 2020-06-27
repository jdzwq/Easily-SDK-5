/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc macro function document

	@module	macro.c | implement file

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

/************************************************************
eg: func1(param1,func2(parm,...),param2,...)

macronode --> funcname + '(' + {funcparam + ','} ')'
funcparam --> macronode | variable | consttoken
funcname --> {a | b... | 1 | 2 ...}
variable --> {a | b | ... | 1 | 2 ...}
consttoken -> [stringtoken | numerictoken]
stringtoken --> '"' + {a | b... | 1 | 2 ...} + '"'
numerictoken --> {1 | 2 | ...}
*************************************************************/

#include "xdlmac.h"
#include "xdlimp.h"

#include "xdlstd.h"


/*define some terminated uchar_t*/
#define NILL			_T('\x02')

#define MACROTYPE		_T("macro_type")

#define MACRODATA		_T("macro_data")

#define MACRO_NULL		_T("0")

#define MACRO_NODE		_T("1")

#define MACRO_VAR		_T("2")

#define MACRO_STR		_T("3")

#define MACRO_NUM		_T("4")


/*define macro node type*/
typedef enum{lnNull = 0,lnNode = 1,lnVar = 2,lnString = 3,lnNumeric = 4}MacroType;

/*define blank uchar_t for skiping*/
static tchar_t MacroBlankChar[] = {_T(' '),_T('\t'),_T('\r'),_T('\n'),NILL};

/*define function name terminated uchar_t*/
static tchar_t MacroFuncNameTerm[] = {_T('('),_T('\0'),NILL};

/*define param terminated uchar_t*/
static tchar_t MacroParamTerm[] = {_T(','),_T(')'),_T('\0'),NILL};

/************************************************************
macro common function implement begin
*************************************************************/

tchar_t* macro_plus(tchar_t** pa,int size)
{
	double f = 0;
	int i;
	tchar_t* token;

	if(size < 2)
		return NULL;

	f = xstonum(pa[0]);

	for (i = 1; i < size; i++)
		f += xstonum(pa[i]);

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1) * sizeof(tchar_t));
	xsprintf(token,_T("%f"),f);
	return token;
}

tchar_t* macro_sub(tchar_t** pa,int size)
{
	double f ;
	int i;
	tchar_t* token;
	
	if(size < 2)
		return NULL;

	f = xstonum(pa[0]);

	for(i = 1;i<size;i++)
		f -= xstonum(pa[i]);

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1) * sizeof(tchar_t));
	xsprintf(token,_T("%f"),f);
	return token;
}

tchar_t* macro_div(tchar_t** pa,int size)
{
	double f ;
	int i;
	tchar_t* token;
	
	if(size < 2)
		return NULL;

	f = xstonum(pa[0]);

	for (i = 1; i < size; i++)
		f /= xstonum(pa[i]);

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1) * sizeof(tchar_t));
	xsprintf(token,_T("%f"),f);
	return token;
}

tchar_t* macro_mul(tchar_t** pa,int size)
{
	double f ;
	int i;
	tchar_t* token;
	
	if(size < 2)
		return NULL;

	f = xstonum(pa[0]);
	for (i = 1; i < size; i++)
		f *= xstonum(pa[i]);

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,_T("%f"),f);
	return token;
}

tchar_t* macro_avg(tchar_t** pa,int size)
{
	double f ;
	int i;
	tchar_t* token;
	
	if(size < 1)
		return NULL;

	f = xstonum(pa[0]);

	for (i = 1; i < size; i++)
		f += xstonum(pa[i]);
	f /= size;

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,_T("%f"),f);
	return token;
}

tchar_t* macro_min(tchar_t** pa,int size)
{
	double min,f ;
	int i;
	tchar_t* token;
	
	if(size < 1)
		return NULL;

	min = xstonum(pa[0]);
	for(i = 1;i<size;i++)
	{
		f = xstonum(pa[i]);
		if(f < min)
			min = f;
	}

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,_T("%f"),min);
	return token;
}

tchar_t* macro_max(tchar_t** pa,int size)
{
	double max,f ;
	int i;
	tchar_t* token;
	
	if(size < 1)
		return NULL;

	max = xstonum(pa[0]);
	for(i = 1;i<size;i++)
	{
		f = xstonum(pa[i]);
		if(f > max)
			max = f;
	}

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,_T("%f"),max);
	return token;
}

tchar_t* macro_row_und(tchar_t** pa,int size)
{
	tchar_t fmt[10];
	tchar_t* token;

	if(size != 2)
		return NULL;

	if(is_null(pa[0]) || is_null(pa[1]))
		return NULL;

	xsprintf(fmt,_T("%c.%df"),_T('%'),xstol(pa[1]));
	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,fmt,xstonum(pa[0]));
	return token;
}

tchar_t* macro_abs(tchar_t** pa,int size)
{
	tchar_t* token;
	double f;

	if(size != 1)
		return NULL;

	f = xstonum(pa[0]);
	if(f < 0)
		f = 0 - f;
	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,_T("%f"),f);
	return token;
}

tchar_t* macro_fmt(tchar_t** pa,int size)
{
	tchar_t fmt[10];
	tchar_t* token;
	int len,i;

	if(size != 3)
		return NULL;

	if(is_null(pa[0]) || is_null(pa[1]) || is_null(pa[2]))
		return NULL;

	xsprintf(fmt,_T("%c%d.%df"),_T('%'),xstol(pa[1]),xstol(pa[2]));
	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,fmt,xstonum(pa[0]));
	len = xslen(token);
	for(i = 0;i<len;i ++)
	{
		if(token[i] == _T(' '))
			token[i] = _T('0');
		else
			break;
	}
	return token;
}

tchar_t* macro_len(tchar_t** pa,int size)
{
	tchar_t* token;

	if(size != 1)
		return NULL;

	if(is_null(pa[0]))
		return NULL;

	token = (tchar_t*)xmem_alloc((NUM_LEN + 1)*sizeof(tchar_t));
	xsprintf(token,_T("%d"),xslen(pa[0]));
	return token;
}

tchar_t* macro_mid(tchar_t** pa,int size)
{
	tchar_t* token;
	int len,n1,n2;

	if(size != 3)
		return NULL;

	if(is_null(pa[0]) || is_null(pa[1]) || is_null(pa[2]))
		return NULL;

	len = xslen(pa[0]);
	n1 = xstol(pa[1]);
	n2 = xstol(pa[2]);
	if(n1 >= len || n1 < 0 || n2 < 0)
	{
		return NULL;
	}

	if(n2 > len - n1)
		n2 = len - n1;
	token = (tchar_t*)xmem_alloc((n2 + 1) * sizeof(tchar_t));
	xsncpy(token,pa[0] + n1,n2);
	return token;
}

tchar_t* macro_cat(tchar_t** pa,int size)
{
	tchar_t* token;
	int len,i;

	if(size < 1)
		return NULL;

	len = 0;
	for(i=0;i<size;i++)
	{
		if(!is_null(pa[i]))
			len += xslen(pa[i]);
	}

	token = xsalloc(len + 1);
	for(i=0;i<size;i++)
	{
		if(!is_null(pa[i]))
			xscat(token,pa[i]);
	}

	return token;
}

tchar_t* macro_empty(tchar_t** pa,int size)
{
	tchar_t* token;

	if(size < 1)
		return NULL;

	token = (tchar_t*)xmem_alloc(2 * sizeof(tchar_t));

	if(is_null(pa[0]))
		token[0] = _T('0');
	else
		token[0] = _T('1');

	return token;
}

tchar_t* macro_scmp(tchar_t** pa,int size)
{
	tchar_t* token;
	int rt;
	
	if(size != 2)
		return NULL;

	rt = xscmp(pa[0],pa[1]);
	token = (tchar_t*)xmem_alloc(3 * sizeof(tchar_t));
	xsprintf(token,_T("%d"),rt);
	return token;
}

tchar_t* macro_if(tchar_t** pa,int size)
{
	tchar_t* token;
	int b,len;

	if(size != 3)
		return NULL;
	
	b = xstol(pa[0]);
	if(b)
	{
		if(is_null(pa[1]))
			return NULL;
		else
		{
			len = xslen(pa[1]);
			token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
			xsncpy(token,pa[1],len);
			return token;
		}
	}else
	{
		if(is_null(pa[2]))
			return NULL;
		else
		{
			len = xslen(pa[2]);
			token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
			xsncpy(token,pa[2],len);
			return token;
		}
	}
}

tchar_t* macro_ltr(tchar_t** pa,int size)
{
	tchar_t* token;
	int len,n1;
	
	if(size != 1)
		return NULL;
	
	if(is_null(pa[0]))
		return NULL;

	len = xslen(pa[0]);
	for(n1=0;n1<len;n1++)
	{
		if((pa[0])[n1] != _T(' '))
			break;
	}
	len -= n1;
	token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
	xsncpy(token,pa[0] + n1,len);
	return token;
}

tchar_t* macro_rtr(tchar_t** pa,int size)
{
	tchar_t* token;
	int len,n1;

	if(size != 1)
		return NULL;
	
	if(is_null(pa[0]))
		return NULL;

	len = xslen(pa[0]);
	for(n1=len-1;n1>=0;n1--)
	{
		if((pa[0])[n1] != _T(' '))
			break;
	}
	len = n1 + 1;
	token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
	xsncpy(token,pa[0],len);
	return token;
}

tchar_t* macro_ctr(tchar_t** pa,int size)
{
	tchar_t* token;
	int len,n1,n2;

	if(size != 1)
		return NULL;
	
	if(is_null(pa[0]))
		return NULL;

	len = xslen(pa[0]);
	for(n1=0;n1<len;n1++)
	{
		if((pa[0])[n1] != _T(' '))
			break;
	}
	for(n2=len-1;n2>n1;n2--)
	{
		if((pa[0])[n1] != _T(' '))
			break;
	}
	len = n2 - n1 + 1;
	token = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
	xsncpy(token,pa[0] + n1,len);
	return token;
}

tchar_t* macro_ncmp(tchar_t** pa,int size)
{
	tchar_t* token;
	double f,f1;

	if(size != 2)
		return NULL;
	
	token = (tchar_t*)xmem_alloc(3 * sizeof(tchar_t));
	f = xstonum(pa[0]);
	f1 = xstonum(pa[1]);
	if(f == f1)
		xscpy(token,_T("0"));
	else if(f > f1)
		xscpy(token,_T("1"));
	else 
		xscpy(token,_T("-1"));
	return token;
}

tchar_t* macro_zero(tchar_t** pa,int size)
{
	tchar_t* token;

	if(size != 1)
		return NULL;
	
	token = (tchar_t*)xmem_alloc(2 * sizeof(tchar_t));
	if(xstonum(pa[0]) == 0)
		token[0] = _T('1');
	else
		token[0] = _T('0');
	return token;
}

tchar_t* macro_leze(tchar_t** pa,int size)
{
	tchar_t* token;

	if(size != 1)
		return NULL;
	
	token = (tchar_t*)xmem_alloc(2 * sizeof(tchar_t));
	if(xstonum(pa[0]) < 0)
		token[0] = _T('1');
	else
		token[0] = _T('0');
	return token;
}

tchar_t* macro_grze(tchar_t** pa,int size)
{
	tchar_t* token;

	if(size != 1)
		return NULL;
	
	token = (tchar_t*)xmem_alloc(2 * sizeof(tchar_t));
	if(xstonum(pa[0]) > 0)
		token[0] = _T('1');
	else
		token[0] = _T('0');
	return token;
}

/************************************************************
macro common function implement end
*************************************************************/


/*test ch is blank uchar_t */

int IsMacroBlankChar(tchar_t ch)
{
	int i = 0;
	while(MacroBlankChar[i] != NILL)
	{
		if(ch == MacroBlankChar[i])
			return 1;
		i++;
	}
	return 0;
}

/*test ch is function name terminated uchar_t*/

int IsMacroFuncNameTerm(tchar_t ch)
{
	int i = 0;
	while(MacroFuncNameTerm[i] != NILL)
	{
		if(ch == MacroFuncNameTerm[i])
			return 1;
		i++;
	}
	return 0;
}

/*test ch is param terminated uchar_t*/

int IsMacroParamTerm(tchar_t ch)
{
	int i = 0;
	while(MacroParamTerm[i] != NILL)
	{
		if(ch == MacroParamTerm[i])
			return 1;
		i++;
	}
	return 0;
}

/*split function name*/

void SplitMacroFuncName(tchar_t* str,int* plen)
{
	tchar_t* token = str;
	*plen = 0;

	while(!IsMacroFuncNameTerm(*token))
	{
		token ++;
		*plen = *plen + 1;
	}

	if(*token != _T('('))	/*no functoin name finded*/
		*plen = 0;
}

/*skip blank uchar_t*/

tchar_t* SkipMacroBlank(tchar_t* str)
{
	tchar_t* token = str;

	while(IsMacroBlankChar(*token))
		token ++;
	
	if(*token == _T('\0'))
		return NULL;
	else
		return token;
}

/*skip macro one param*/

tchar_t* SkipMacroParam(tchar_t* str)
{
	tchar_t* token = str;
	int quate = 0;

	while(!IsMacroParamTerm(*token) || quate)
	{
		if(*token == _T('('))
			quate ++;	/*find one sub quate*/
		else if(*token == _T(')'))
			quate --;	/*skip one sub quate*/

		token ++;
		if(*token == _T('\0'))
			break;
	}

	if(quate || *token == _T('\0')) /*lost some quate*/
		return NULL;
	else
		return token;
}

/*test param type*/

int GetMacroParamType(tchar_t* str,int len)
{
	tchar_t* token = str;

	token = SkipMacroBlank(token);
	if(token == NULL)	/*empty token*/
		return lnNull;
	else if(token == str + len)
		return lnNull;	/*empty token*/

	if(*token == _T('\''))	/*param is const string token*/
		return lnString;

	if((*token >= _T('0') && *token <= _T('9')) || *token == _T('.')) /*param is const numeric token*/
		return lnNumeric;

	len -= (int)(token - str);
	while(len--)
	{
		if(*token == _T('('))	/*param is sub macro node*/
			return lnNode;
		token ++;
	}

	return lnVar;		/*param is variable token*/
}

/*trim left and right blank*/

void TrimMacroToken(tchar_t* str,int len,tchar_t** strat,int* plen)
{
	tchar_t* token;
	
	if (!str || len <= 0)
		return;

	token = str;
	while(IsMacroBlankChar(*token) && token != str + len)	/*skip left blank*/
		token ++;
	*strat = token;

	token = str + len - 1;
	while(IsMacroBlankChar(*token) && token != str)	/*count not blank tchar_t*/
		token --;

	*plen = (int)(token - *strat) + 1;
}

/*parse macro node*/

void MacroNodeParse(link_t_ptr multi,link_t_ptr node,tchar_t* str,int len)
{
	tchar_t* token = str;
	tchar_t* subtoken;
	tchar_t* nexttoken;
	int type,tokenlen,sublen;
	link_t_ptr child;

	if(is_null(str))
		return ;

	if(len == -1)
		len = xslen(str);

	/*parse function name*/
	SplitMacroFuncName(token,&tokenlen);
	if(tokenlen == 0)
		return ;

	TrimMacroToken(token,tokenlen,&subtoken,&sublen);	/*get function name*/
	/*new macro node*/
	set_multi_node_attr(node,MACROTYPE,-1,MACRO_NODE,-1);
	set_multi_node_attr(node,MACRODATA,-1,subtoken,sublen);

	/*continue to parse function params*/
	token = token + tokenlen;
	token ++;	/*skip '('*/

	while(*token != _T('\0'))
	{
		nexttoken = SkipMacroParam(token);
		if(nexttoken == NULL)	/*invalid macro node*/
		{
			delete_multi_node(node);
			return ;
		}

		tokenlen = (int)(nexttoken - token);
		type = GetMacroParamType(token,tokenlen);
		switch(type)
		{
		case lnNull:
			child = insert_multi_node(node,LINK_LAST);
			set_multi_node_attr(child,MACROTYPE,-1,MACRO_NULL,-1);
			set_multi_node_attr(child,MACRODATA,-1,NULL,0);
			break;
		case lnString:
			child = insert_multi_node(node,LINK_LAST);
			set_multi_node_attr(child,MACROTYPE,-1,MACRO_STR,-1);
			TrimMacroToken(token,tokenlen,&subtoken,&sublen);
			subtoken ++;	/*not include first and last '\''*/
			sublen -= 2;
			set_multi_node_attr(child,MACRODATA,-1,subtoken,sublen);
			break;
		case lnNumeric:
			child = insert_multi_node(node,LINK_LAST);
			set_multi_node_attr(child,MACROTYPE,-1,MACRO_NUM,-1);
			TrimMacroToken(token,tokenlen,&subtoken,&sublen);
			set_multi_node_attr(child,MACRODATA,-1,subtoken,sublen);
			break;
		case lnVar:
			child = insert_multi_node(node,LINK_LAST);
			set_multi_node_attr(child,MACROTYPE,-1,MACRO_VAR,-1);
			TrimMacroToken(token,tokenlen,&subtoken,&sublen);
			set_multi_node_attr(child,MACRODATA,-1,subtoken,sublen);
			break;
		case lnNode:
			TrimMacroToken(token,tokenlen,&subtoken,&sublen);
			child = insert_multi_node(node,LINK_LAST);
			MacroNodeParse(multi,child,subtoken,sublen);
			break;
		}

		if(*nexttoken == _T(')'))	/*last param parsed*/
			break;

		token = nexttoken + 1;	/*skip ',' continue to parse next param*/
	}
}

int MacroNodeFormat(link_t_ptr multi,link_t_ptr node,tchar_t* buf,int max)
{
	int total,len;
	link_t_ptr child;
	const tchar_t *type,*data;

	total = 0;
	data = get_multi_node_attr_ptr(node,MACRODATA,-1);
	len = (data)? (xslen(data) + 1) : 1;	/*function name with '(' length*/
	if(total + len > max)
		return -1;
	if(buf)
		xsprintf(buf + total,_T("%s("),((data)? data : _T("")));
	total += len;

	/*format function params*/
	child = get_multi_first_child_node(node);
	while(child)
	{
		type = get_multi_node_attr_ptr(child,MACROTYPE,-1);
		data = get_multi_node_attr_ptr(child,MACRODATA,-1);
		switch(xstol(type))
		{
		case lnNull:
			len = 1;	/*null token with ','*/
			if(total + len > max)
				return -1;
			if(buf)
				xsprintf(buf + total,_T("%s"),_T(","));
			total += len;
			break;
		case lnString:
			len = (data)? (xslen(data) + 3) : 3;	/*string token with two '\'' and one ','*/
			if(total + len > max)
				return -1;
			if(buf)
				xsprintf(buf + total,_T("'%s',"),((data)? data : _T("")));
			total += len;
			break;
		case lnNumeric:
			len = (data)? (xslen(data) + 1) : 1;	/*numeric token with one ','*/
			if(total + len > max)
				return -1;
			if(buf)
				xsprintf(buf + total,_T("%s,"),((data)? data : _T("")));
			total += len;
			break;
		case lnVar:
			len = (data)? (xslen(data) + 1) : 1;	/*variable token with one ','*/
			if(total + len > max)
				return -1;
			if(buf)
				xsprintf(buf + total,_T("%s,"),((data)? data : _T("")));
			total += len;
			break;
		case lnNode:
			len = MacroNodeFormat(multi,child,((buf)? buf + total : NULL),max - total) + 1; /*sub node with ','*/
			if(len == 0 || total + len > max)
				return -1;
			if(buf)
				xsprintf(buf + total + len - 1,_T("%s"),_T(","));
			total += len;
			break;
		}

		child = get_multi_next_sibling_node(child);
	}

	if(buf)
		buf[total - 1] = _T(')');	/*replace last ',' with ')'*/

	return total;
}

tchar_t* MacroNodeCalc(link_t_ptr multi,link_t_ptr node,macro_var_fetch vf,void* vfparam)
{
	macro_func_ptr pf;
	link_t_ptr ht,elk,child;
	int size,dt;
	tchar_t** pa;
	tchar_t* token;
	const tchar_t* type;
	const tchar_t* data;
	
	ht = get_multi_node_attr_table(multi);
	/*get macro node func*/
	data = get_multi_node_attr_ptr(node,MACRODATA,-1);
	elk = get_hash_entity(ht,data,-1);	
	if(elk == NULL)
		return NULL;

	pf = (macro_func_ptr)get_hash_entity_delta(elk);
	if(pf == NULL)
		return NULL;

	size = get_multi_child_node_count(node);
	pa = (tchar_t**)xmem_alloc(size * sizeof(tchar_t*));

	child = get_multi_first_child_node(node);
	size = 0;
	while(child)
	{
		type = get_multi_node_attr_ptr(child,MACROTYPE,-1);
		data = get_multi_node_attr_ptr(child,MACRODATA,-1);
		switch(xstol(type))
		{
		case lnNull:
			pa[size ++] = NULL;
			break;
		case lnString:
			pa[size ++] = (tchar_t*)data;
			break;
		case lnNumeric:
			pa[size ++] = (tchar_t*)data;
			break;
		case lnVar:
			if(vf)
				pa[size ++] = (*vf)(data,vfparam);
			else 
				pa[size ++] = NULL;
			break;
		case lnNode:
			pa[size ++] = MacroNodeCalc(multi,child,vf,vfparam);
			break;
		}

		child = get_multi_next_sibling_node(child);
	}

	token = (*pf)(pa,size);

	size = 0;
	child = get_multi_first_child_node(node);
	while(child)
	{
		type = get_multi_node_attr_ptr(child,MACROTYPE,-1);
		dt = xstol(type);
		if((dt == lnNode || dt == lnVar) && pa[size])
			xmem_free(pa[size]);
		size ++;
		child = get_multi_next_sibling_node(child);
	}
	xmem_free(pa);

	return token;
}

/***********************************************************************/
bool_t macro_parse(link_t_ptr ptr,const tchar_t* str,int len)
{
	link_t_ptr root;
	link_t_ptr ht,elk;

	if (!ptr)
		return 0;

	if(is_null(str))
		return 0;

	ht = get_multi_node_attr_table(ptr);

	/*add some common macro function*/
	elk = write_hash_attr(ht, FN_PLUS, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_plus);

	elk = write_hash_attr(ht, FN_SUB, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_sub);

	elk = write_hash_attr(ht, FN_DIV, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_div);

	elk = write_hash_attr(ht, FN_MUL, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_mul);

	elk = write_hash_attr(ht, FN_AVG, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_avg);

	elk = write_hash_attr(ht, FN_MIN, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_min);

	elk = write_hash_attr(ht, FN_MAX, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_max);

	elk = write_hash_attr(ht, FN_ROUND, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_row_und);

	elk = write_hash_attr(ht, FN_ABS, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_abs);

	elk = write_hash_attr(ht, FN_LEN, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_len);

	elk = write_hash_attr(ht, FN_MID, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_mid);

	elk = write_hash_attr(ht, FN_CAT, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_cat);

	elk = write_hash_attr(ht, FN_FMT, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_fmt);

	elk = write_hash_attr(ht, FN_EMPTY, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_empty);

	elk = write_hash_attr(ht, FN_IF, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_if);

	elk = write_hash_attr(ht, FN_LTR, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_ltr);

	elk = write_hash_attr(ht, FN_RTR, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_rtr);

	elk = write_hash_attr(ht, FN_CTR, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_ctr);

	elk = write_hash_attr(ht, FN_SCMP, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_scmp);

	elk = write_hash_attr(ht, FN_NCMP, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_ncmp);

	elk = write_hash_attr(ht, FN_ZERO, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_zero);

	elk = write_hash_attr(ht, FN_LEZE, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_leze);

	elk = write_hash_attr(ht, FN_GRZE, -1, NULL, 0);
	set_hash_entity_delta(elk,(var_long)macro_grze);
	
	root = insert_multi_node(ptr,LINK_LAST);
	MacroNodeParse(ptr,root,(tchar_t*)str,len);

	return 1;
}

int macro_format(link_t_ptr ptr,tchar_t* buf,int max)
{
	link_t_ptr root;

	root = get_multi_first_child_node(ptr);
	if(root == NULL)
		return 0;
	else
		return MacroNodeFormat(ptr,root,buf,max);
}

int macro_format_size(link_t_ptr ptr)
{
	return macro_format(ptr, NULL, MAX_LONG);
}

tchar_t* macro_calc(link_t_ptr ptr,macro_var_fetch vf,void* vfparam)
{
	link_t_ptr root;

	root = get_multi_first_child_node(ptr);
	if(root == NULL)
		return NULL;
	else
		return MacroNodeCalc(ptr,root,vf,vfparam);
}

void macro_set_func(link_t_ptr ptr,const tchar_t* funcname,macro_func_ptr pf)
{
	link_t_ptr ht,elk;

	if(is_null(funcname) || pf == NULL)
		return ;

	ht = get_multi_node_attr_table(ptr);
	elk = write_hash_attr(ht,funcname,-1,NULL,0);
	set_hash_entity_delta(elk,(var_long)pf);
}

macro_func_ptr macro_get_func(link_t_ptr ptr,const tchar_t* funcname)
{
	link_t_ptr ht,elk;

	if(is_null(funcname))
		return NULL;

	ht = get_multi_node_attr_table(ptr);
	elk = get_hash_entity(ht,funcname,-1);
	if(elk == NULL)
		return NULL;
	else
		return (macro_func_ptr)get_hash_entity_delta(elk);
}

