/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc UBB parse document

	@module	ubbparser.c | ubb parse implement file

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

#include "ubbparser.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

#define UBB_TAG_TEXT		_T("text")

//以下定义自动机的状态
typedef enum{
	UBB_FAILED	= -1,		//错误中断
	UBB_SUCCEED = 0,		//解析正常结束

	UBB_OPEN = 10,			//标记打开
	UBB_CLOSE = 11,			//标记关闭

	UBB_NS_OPEN = 100,			//元素打开
	UBB_NS_INNER_CLOSE = 110,	//元素内部中断
	UBB_NS_REVER_CLOSE = 120,	//元素预关闭
	UBB_NS_CLOSE = 130,			//元素关闭
	UBB_NS_NAME_BEGIN = 140,	//元素名开始
	UBB_NS_NAME_END = 150,		//元素名结束
	UBB_NS_KEY_BEGIN = 160,		//元素属性名开始
	UBB_NS_KEY_END = 170,		//元素属性名结束
	UBB_NS_ASIGN = 180,			//元素属性赋值
	UBB_NS_VAL_QUOT = 181,		//元素属性括号
	UBB_NS_VAL_BEGIN = 190,		//元素属性开始
	UBB_NS_VAL_END = 200,		//元素属性结束
	UBB_NS_TEXT_BEGIN = 210,	//元素文本开始
	UBB_NS_TEXT_END = 220,		//元素文本结束
	UBB_NS_REVER_BEGIN = 250,	//关闭标记中的元素名开始
	UBB_NS_REVER_END = 260,		//关闭标记中的元素名结束
}TAGSTATUS;

//以下定义读写头动作
typedef enum{
	PAUSE = 0,	//暂停于当前字符
	NEXT = 1,	//读取当前字符
	STOP = -1	//停机	
}TAGACTION;

//定义自动机
typedef struct _MATA{
	int ma;		//读写头动作
	int ms;		//当前状态
	tchar_t ch; //当前字符
	int bytes;	//扫描字节数
}TAGMATA;

//判断是否为空格符
#define _IsSpaceChar(ch) ((ch == _T(' ') || ch == _T('\t'))? 1 : 0)

//判断是否为掠过字符
//#define _IsSkipChar(ch) ((ch == _T('\n') || ch == _T('\r'))? 1 : 0)


#define idle		while(0 == 1)


//解析TAG文本
bool_t parse_ubb_doc(link_t_ptr dom, const tchar_t* str, int len)
{
	link_t_ptr nlk = NULL;

	string_t vs_name, vs_key, vs_val, vs_text;

	TAGMATA ma = { 0 };

	delete_dom_child_nodes(dom);
	nlk = insert_dom_node(dom, LINK_LAST);
	set_dom_node_name(nlk, UBB_TAG_TEXT,-1);

	if (len < 0)
		len = xslen(str);

	vs_name = string_alloc();
	vs_key = string_alloc();
	vs_val = string_alloc();
	vs_text = string_alloc();
	
	//自动机初始化
	ma.ma = NEXT;
	ma.ms = UBB_NS_TEXT_BEGIN;
	ma.bytes = 0;

	while(ma.ma != STOP)
	{
		if (ma.ma == NEXT)
		{
			//读取下一字符
			ma.ch = str[ma.bytes];
			ma.bytes += 1;
		}

		//根据自动机状态跳转
		switch (ma.ms)
		{
		case UBB_OPEN: //标记打开
			if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_OPEN; ma.ma = NEXT; } //忽略空格
			else if (ma.ch == _T('/')){ ma.ms = UBB_NS_REVER_CLOSE; ma.ma = NEXT; } //准备关闭标记
			else { ma.ms = UBB_NS_OPEN; ma.ma = PAUSE; } //元素开始
			break;
		case UBB_NS_OPEN: //元素打开
			if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_NS_OPEN; ma.ma = NEXT; } //忽略空格
			else { ma.ms = UBB_NS_NAME_BEGIN; ma.ma = PAUSE; } //指令名开始
			break;
		case UBB_NS_NAME_BEGIN: //元素命名开始
			if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_NS_NAME_END; ma.ma = PAUSE; } //元素名结束
			else if (ma.ch == _T(']')){ ma.ms = UBB_NS_NAME_END; ma.ma = PAUSE; }//元素名结束
			else if (ma.ch == _T('/')){ ma.ms = UBB_NS_NAME_END; ma.ma = PAUSE; } //元素名结束
			else { ma.ms = UBB_NS_NAME_BEGIN; ma.ma = NEXT; } //元素名继续
			break;
		case UBB_NS_NAME_END: //元素命名结束
			if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_NS_NAME_END; ma.ma = NEXT; } //忽略空格
			else if (ma.ch == _T(']')){ ma.ms = UBB_NS_INNER_CLOSE; ma.ma = NEXT; } //元素名结束
			else if (ma.ch == _T('/')){ ma.ms = UBB_NS_REVER_CLOSE; ma.ma = NEXT; } //元素名结束
			else { ma.ms = UBB_NS_KEY_BEGIN; ma.ma = PAUSE; } //属性名开始
			break;
		case UBB_NS_KEY_BEGIN: //元素属性名开始
			if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_NS_KEY_END; ma.ma = PAUSE; } //属性名结束
			else if (ma.ch == _T('=')){ ma.ms = UBB_NS_KEY_END; ma.ma = PAUSE; } //属性赋值符
			else { ma.ms = UBB_NS_KEY_BEGIN; ma.ma = NEXT; } //继续属性名
			break;
		case UBB_NS_KEY_END: //元素属性名结束
			if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_NS_KEY_END; ma.ma = NEXT; } //忽略空格
			else if (ma.ch == _T('=')){ ma.ms = UBB_NS_ASIGN; ma.ma = NEXT; } //属性赋值符
			else { ma.ms = UBB_FAILED; ma.ma = STOP; } //无效字符
			break;
		case UBB_NS_ASIGN: //元素属性赋值
			if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_NS_ASIGN; ma.ma = NEXT; } //忽略空格
			else if (ma.ch == _T('\"') || ma.ch == _T('\'')){ ma.ms = UBB_NS_VAL_QUOT; ma.ma = NEXT; } //过渡到属性值开始,掠过“\"”
			else { ma.ms = UBB_FAILED; ma.ma = STOP; } //无效字符
			break;
		case UBB_NS_VAL_QUOT:
			if (ma.ch == _T('\"') || ma.ch == _T('\'')){ ma.ms = UBB_NS_VAL_BEGIN; ma.ma = PAUSE; } //过渡到属性值开始
			else { ma.ms = UBB_NS_VAL_BEGIN; ma.ma = PAUSE; } //开始属性值
			break;
		case UBB_NS_VAL_BEGIN: //元素属性值开始
			if (ma.ch == _T('\"') || ma.ch == _T('\'')){ ma.ms = UBB_NS_VAL_END; ma.ma = PAUSE; } //过渡到属性值结束
			else { ma.ms = UBB_NS_VAL_BEGIN; ma.ma = NEXT; } //继续属性值
			break;
		case UBB_NS_VAL_END: //元素属性值结束
			if (ma.ch == _T('\"') || ma.ch == _T('\'')){ ma.ms = UBB_NS_VAL_END; ma.ma = NEXT; } //属性值结束
			else if (_IsSpaceChar(ma.ch)){ ma.ms = UBB_NS_VAL_END; ma.ma = NEXT; } //忽略空格
			else if (ma.ch == _T('/')){ ma.ms = UBB_NS_REVER_CLOSE; ma.ma = NEXT; } //准备关闭标记
			else if (ma.ch == _T(']')){ ma.ms = UBB_NS_INNER_CLOSE; ma.ma = NEXT; } //标记中断
			else { ma.ms = UBB_NS_KEY_BEGIN; ma.ma = PAUSE; } //属性名开始
			break;
		case UBB_NS_INNER_CLOSE: //元素命名中断
			//if (ma.ch == _T('[')){ ma.ms = UBB_OPEN; ma.ma = NEXT; } //标记开始
			{ ma.ms = UBB_NS_TEXT_BEGIN; ma.ma = PAUSE; } //元素文本开始
			break;
		case UBB_NS_TEXT_BEGIN: //元素文本开始
			if (ma.ch == _T('[')){ ma.ms = UBB_NS_TEXT_END; ma.ma = PAUSE; } //过渡到元素文本结束
			else if (ma.ch == _T('\0')){ ma.ms = UBB_NS_TEXT_END; ma.ma = PAUSE; } //过渡到元素文本结束
			else { ma.ms = UBB_NS_TEXT_BEGIN; ma.ma = NEXT; } //继续元素文本
			break;
		case UBB_NS_TEXT_END: //元素文本结束
			if (ma.ch == _T('[')){ ma.ms = UBB_OPEN; ma.ma = NEXT; } //标记开始
			else if (ma.ch == _T('\0')){ ma.ms = UBB_SUCCEED; ma.ma = STOP; } //解析结束
			else { ma.ms = UBB_FAILED; ma.ma = STOP; } //无效字符
			break;
		case UBB_NS_REVER_CLOSE: //元素预关闭
			if (ma.ch == _T(']')){ ma.ms = UBB_NS_CLOSE; ma.ma = PAUSE; }//标记结束
			else { ma.ms = UBB_NS_REVER_BEGIN; ma.ma = PAUSE; } //关闭标记的元素名开始
			break;
		case UBB_NS_REVER_BEGIN: //元素关闭结构中的元素名开始
			if (ma.ch == _T(']')){ ma.ms = UBB_NS_REVER_END; ma.ma = PAUSE; } //关闭标记的元素名结束
			else { ma.ms = UBB_NS_REVER_BEGIN; ma.ma = NEXT; } //继续关闭标记的元素名
			break;
		case UBB_NS_REVER_END: //元素关闭结构中的元素名开始
			if (ma.ch == _T(']')){ ma.ms = UBB_NS_CLOSE; ma.ma = PAUSE; }//标记结束
			else { ma.ms = UBB_FAILED; ma.ma = STOP; } //无效字符
			break;
		case UBB_NS_CLOSE: //元素关闭
			{ma.ms = UBB_CLOSE; ma.ma = NEXT; } //过渡到标记关闭
			break;
		case UBB_CLOSE: //标记关闭
			{ma.ms = UBB_NS_TEXT_BEGIN; ma.ma = PAUSE; }
		break;
			default:
			{ma.ms = UBB_FAILED; ma.ma = STOP; } //无效字符
			break;
		}
	
		//根据自动机状态处置
		if(ma.ms == UBB_NS_OPEN)
		{
			idle;
		}else if(ma.ms == UBB_NS_NAME_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_name);
				break;
			case NEXT:
				string_ins_chars(vs_name, -1, &ma.ch, 1);
				break;
			}
		}else if(ma.ms == UBB_NS_NAME_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				set_dom_node_name(nlk, string_ptr(vs_name), -1);
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == UBB_NS_KEY_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_key);
				break;
			case NEXT:
				string_ins_chars(vs_key, -1, &ma.ch, 1);
				break;
			}
		}else if(ma.ma == UBB_NS_KEY_END)
		{
			idle;
		}else if(ma.ma == UBB_NS_ASIGN)
		{
			idle;
		}else if(ma.ms == UBB_NS_VAL_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_val);
				break;
			case NEXT:
				string_ins_chars(vs_val, -1, &ma.ch, 1);
				break;
			}
		}else if(ma.ms == UBB_NS_VAL_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				set_dom_node_attr(nlk, string_ptr(vs_key), -1, string_ptr(vs_val), -1);
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == UBB_NS_TEXT_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_text);
				break;
			case NEXT:
				string_ins_chars(vs_text, -1, &ma.ch, 1);
				break;
			}
		}else if(ma.ms == UBB_NS_TEXT_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				set_dom_node_text(nlk, string_ptr(vs_text),-1);
				break;
			case NEXT:
				break;
			}
		}
		else if (ma.ms == UBB_NS_REVER_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == UBB_NS_REVER_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if(!nlk)
				{
					ma.ms = UBB_FAILED;
				}
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == UBB_NS_CLOSE)
		{
			idle;
		}

		//根据自动机的状态决定是否退出
		if (ma.ms == UBB_FAILED || ma.ms == UBB_SUCCEED)
			break;

		if (ma.ms == UBB_NS_TEXT_END && ma.ma == PAUSE && ma.ch != _T('\0'))
		{
			nlk = insert_dom_node(dom, LINK_LAST);
			set_dom_node_name(nlk, UBB_TAG_TEXT,-1);

			string_empty(vs_name);
			string_empty(vs_key);
			string_empty(vs_val);
			string_empty(vs_text);
		}

	}

	string_free(vs_name);
	string_free(vs_key);
	string_free(vs_val);
	string_free(vs_text);

	//自动机的最终状态
	return (ma.ms == UBB_SUCCEED)? 1 : 0;
}

int format_ubb_doc(link_t_ptr dom, tchar_t* buf, int max)
{
	int len,total = 0;
	link_t_ptr nlk;
	link_t_ptr ht, ent;
	hash_enum_t he = { 0 };
	const tchar_t *sz_name, *sz_key, *sz_val, *sz_text;

	nlk = get_dom_first_child_node(dom);
	while (nlk)
	{
		sz_name = get_dom_node_name_ptr(nlk);
		ht = get_dom_node_attr_table(nlk);

		if (compare_text(sz_name, -1, UBB_TAG_TEXT, -1, 1) != 0)
		{
			len = xslen(sz_name);
			if (len)
			{
				if (get_hash_entity_count(ht))
					len += 2; //[space
				else
					len += 1; //[
			}
		}
		else
		{
			sz_name = NULL;
			len = 0;
			ht = NULL;
		}
		if (total + len > max)
			return total;

		if (buf)
		{
			if (!is_null(sz_name))
			{
				if (get_hash_entity_count(ht))
					xsprintf(buf + total, _T("[%s "), sz_name);
				else
					xsprintf(buf + total, _T("[%s"), sz_name);
			}
		}
		total += len;

		he.hash = ht;
		he.entity = LINK_FIRST;
		ent = (ht) ? get_hash_next_entity(&he) : NULL;
		while (ent)
		{
			sz_key = get_hash_entity_key_ptr(ent);
			sz_val = get_hash_entity_val_ptr(ent);

			len = xslen(sz_key) + xslen(sz_val) + 4; //key="val" space

			if (total + len >= max)
				return total;

			if (buf)
			{
				xsprintf(buf + total, _T("%s=\"%s\" "), sz_key, sz_val);
			}
			total += len;

			ent = get_hash_next_entity(&he);
		}

		if (!is_null(sz_name))
			len = 1; //]
		else
			len = 0;

		if (total + len > max)
			return total;

		if (buf)
		{
			if (!is_null(sz_name))
			{
				xsncpy(buf + total, _T("]"), 1);
			}
		}
		total += len;

		sz_text = get_dom_node_text_ptr(nlk);
		len = xslen(sz_text);

		if (total + len > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, sz_text, len);
		}
		total += len;

		if (!is_null(sz_name))
		{
			len = xslen(sz_name);
			len += 3; //[/name]
		}
		else
		{
			len = 0;
		}

		if (total + len > max)
			return total;

		if (buf)
		{
			if (!is_null(sz_name))
			{
				xsprintf(buf + total, _T("[/%s]"), sz_name);
			}
		}
		total += len;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return total;
}

#endif /*XDL_SUPPORT_DOC*/