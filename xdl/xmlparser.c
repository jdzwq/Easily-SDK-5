/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml parse document

	@module	xmlparser.c | xml parse implement file

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

#include "xmlparser.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

#define A_XML_ATTR			"xml"
#define W_XML_ATTR			L"xml"
#define XML_ATTR			_T("xml")
#define XML_ATTR_LEN		3

#define NIL_CHAR			0

#define XMLNS				_T("xmlns")
#define XMLNS_LEN			5

//判断是否有效命名首字符
#define _IsNamePre(ch) ((ch == _T('_') || (ch >= _T('A') && ch <= _T('Z')) || (ch >= _T('a') && ch <= _T('z')) || ch < 0)? 1: 0)

//判断是否为有效命名符
#define _IsNameChar(ch) ((ch == _T('_') || ch == _T('-') || ch == _T('.') || ch == _T(':') || (ch >= _T('0') && ch <= _T('9')) || (ch >= _T('A') && ch <= _T('Z')) || (ch >= _T('a') && ch <= _T('z')) || ch < 0)? 1: 0)

//判断是否为空格符
#define _IsSpaceChar(ch) ((ch == _T(' ') || ch == _T('\t'))? 1 : 0)

//判断是否为掠过字符
#define _IsSkipChar(ch) ((ch == _T('\n') || ch == _T('\r') || ch == _T('\t'))? 1 : 0)

//判断是否有效文本符
//#define _IsTokenChar(tchar_t ch) ((ch != '<' && ch != '>' && ch != '\"' && ch != '\'' && ch != '/')? 1 : 0)
#define _IsTokenChar(ch) ((ch != _T('<') && ch != _T('>') && ch != _T('\"') && ch != _T('\''))? 1 : 0)

#define _IsTextChar(ch) ((ch != _T('<') && ch != _T('>'))? 1 : 0)

#define idle		while(0 == 1)

//以下定义自动机的状态
typedef enum{
	NIL_FAILED	= -1,		//错误中断
	NIL_SUCCEED = 0,		//解析正常结束
	NIL_SKIP = 1,			//略过
	NIL_BREAK = 2,			//用户中断

	TG_OPEN = 10,			//标记打开
	TG_CLOSE = 11,			//标记关闭

	NS_OPEN = 20,			//元素打开
	NS_INNER_CLOSE = 21,	//元素内部中断
	NS_REVER_CLOSE = 22,	//元素预关闭
	NS_CLOSE = 23,			//元素关闭
	NS_REVER_BEGIN = 24,	//关闭标记中的元素名开始
	NS_REVER_END = 25,		//关闭标记中的元素名结束
	NS_NAME_BEGIN = 30,		//元素名开始
	NS_NAME_END = 31,		//元素名结束
	NS_KEY_BEGIN = 32,		//元素属性名开始
	NS_KEY_END = 33,		//元素属性名结束
	NS_ASIGN = 34,			//元素属性赋值
	NS_VAL_QUOT = 35,		//元素属性括号
	NS_VAL_BEGIN = 36,		//元素属性开始
	NS_VAL_END = 37,		//元素属性结束
	NS_TEXT_BEGIN = 38,		//元素文本开始
	NS_TEXT_END = 39,		//元素文本结束

	IS_OPEN = 50,			//指令打开
	IS_NAME_BEGIN = 51,		//指令名开始
	IS_NAME_END = 52,		//指令名结束
	IS_KEY_BEGIN = 53,		//指令键开始
	IS_KEY_END = 54,		//指令键结束
	IS_ASIGN = 55,			//赋值符
	IS_VAL_QUOT = 56,		//括号
	IS_VAL_BEGIN = 57,		//指令值开始
	IS_VAL_END = 58,		//指令值结束
	IS_CLOSE = 59,			//指令关闭

	DS_OPEN = 60,			//声明打开
	DS_CLOSE = 61,			//声明关闭

	CM_BEGIN = 70,			//注释头开始
	CM_TEXT_BEGIN = 71,		//注释文本开始
	CM_TEXT_END = 72,		//注释文本结束
	CM_END = 73,			//注释尾结束

	CD_HEAD_BEGIN = 80,		//界段头开始
	CD_HEAD_END = 81,		//界段头结束
	CD_TEXT_BEGIN = 82,		//界段文本开始
	CD_TEXT_END = 83,		//界段文本结束
	CD_TAIL_BEGIN = 84,		//界段尾开始
	CD_TAIL_END = 85		//界段尾结束
}MataStatus;

//以下定义栈动作
typedef enum{
	NOP = 0,	//无操作
	PUSH = 1,	//入栈
	POP = 2		//出栈
}StackOpera;

//以下定义读写头动作
typedef enum{
	PAUSE = 0,	//暂停于当前字符
	NEXT = 1,	//读取当前字符
	SKIP = 2, //掠过之后字符
	SKIP_CDT_HEAD = 10,	//掠过界段字符
	SKIP_CDT_TAIL = 11,	//掠过界段字符
	SKIP_CMT_HEAD = 12,	//掠过注释字符
	SKIP_CMT_TAIL = 13,	//掠过注释字符
	STOP = -1	//停机	
}MataAction;


//定义自动机
typedef struct _MATA{
	int sa;		//栈动作
	int ma;		//读写头动作
	int ms;		//当前状态

	tchar_t org[ESC_LEN + 1];//上一字
	tchar_t cur[ESC_LEN + 1];//当前字

	int bytes;	//扫描字节数

	int enc;	//编码格式
	bool_t esc;	//是否转义
	bool_t eof; //是否有终结符
}MATA;


/************************************************************************************************************************/
void _string_clean_head_tail(string_t vs)
{
	int len;
	
	while (_IsSkipChar(string_get_char(vs,0)))
	{
		string_del_chars(vs, 0, 1);
	}

	len = string_len(vs);
	while (len && _IsSkipChar(string_get_char(vs, len - 1)))
	{
		string_del_chars(vs, len - 1, 1);
		len--;
	}
}

//解析XML文本
bool_t parse_xml(xml_writer_t* pxp, int encode, if_operator_t* pbo)
{
	MATA ma = { 0 };
	int pos;
	bool_t ret;

	tchar_t* nsstr;
	int nesc, nsat, nslen, vsat, vslen;

	string_t vs_name, vs_key, vs_val, vs_text;

	tchar_t errtext[ERR_LEN + 1] = { 0 };

	if (pbo->isdom && !encode)
	{
		set_last_error(_T("-1"), _T("unknown dom encode"), -1);
		return 0;
	}

	vs_name = string_alloc();
	vs_key = string_alloc();
	vs_val = string_alloc();
	vs_text = string_alloc();

	if (pxp->pf_write_begin)
	{
		(*pxp->pf_write_begin)(pxp->obj);
	}

	ret = 1;

	//自动机初始化
	ma.ma = NEXT;
	ma.sa = NOP;
	ma.ms = NIL_SKIP;
	ma.esc = 1;
	ma.bytes = 0;
	ma.enc = (encode)? encode : _UTF8;
	ma.eof = (*pbo->pf_with_eof)(pbo->obj);

	pos = 0;
	while(ma.ma != STOP)
	{
		//关闭和恢复转义开关
		switch (ma.ms)
		{
		case CM_BEGIN:
			ma.esc = 0;
			break;
		case CM_END:
			ma.esc = 1;
			break;
		case CD_HEAD_BEGIN:
			ma.esc = 0;
			break;
		case CD_TAIL_END:
			ma.esc = 1;
			break;
		}

		//是否支持转义
		if (!(*pbo->pf_can_escape)(pbo->obj))
		{
			ma.esc = 0;
		}

		if (ma.ma == NEXT)
		{
			ma.bytes += pos;

			xmem_zero((void*)ma.org, sizeof(ma.org));
			xmem_zero((void*)ma.cur, sizeof(ma.cur));

			//读取下一字符
			pos = (*pbo->pf_read_char)(pbo->obj, pbo->max, ma.bytes, ma.enc, ma.cur);
			if (pos <= 0)
			{
				ma.cur[0] = _T('\0');
				pos = 0;
			}

			//处理转义字符
			if (ma.esc && ma.cur[0] == CH_ESC)
			{
				nesc = (*pbo->pf_read_escape)(pbo->obj, pbo->max, ma.bytes + pos, ma.enc, ma.org);
				if (nesc)
				{
					ma.bytes += pos;
					pos = nesc;
				}
				else
				{
					xscpy(ma.org, ma.cur);
				}
			}
			else
			{
				xscpy(ma.org, ma.cur);
			}

		}
		else if (ma.ma == SKIP)
		{
			while (ma.eof)
			{
				//读取下一字符
				ma.bytes += pos;
				pos = (*pbo->pf_read_char)(pbo->obj, pbo->max, ma.bytes, ma.enc, ma.cur);
				if (pos <= 0)
				{
					pos = 0;
					break;
				}
			}

			ma.cur[0] = _T('\0');
			ma.ms = NIL_SUCCEED;
			ma.ma = STOP;
		}

		if (ma.ma != STOP)
		{
			//根据自动机状态跳转
			switch (ma.ms)
			{
			case NIL_SKIP: //空闲略过
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = NEXT; }	//忽略起始空格
				else if (ma.cur[0] == _T('<')){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //遇到标记
				else if (ma.cur[0] == _T('\0')) { ma.sa = NOP; ma.ms = NIL_SUCCEED, ma.ma = STOP; }	//终结符
				//else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; }	//无效字符
				else { ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = NEXT; }	//无效字符
				break;
			case TG_OPEN: //标记打开
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT; }	//遇到指令符
				else if (ma.cur[0] == _T('!')){ ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT; } //遇到声明符
				else if (ma.cur[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT; } //准备关闭标记
				else { ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = PAUSE; } //元素开始
				break;
			case IS_OPEN: //指令打开
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT; } //忽略空格
				else if (_IsNamePre(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = PAUSE; } //指令名开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_NAME_BEGIN: //指令名开始
				if (_IsNameChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = NEXT; } //继续指令名
				else if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE; } //指令名结束
				else if (ma.cur[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE; } //指令名结束
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_NAME_END: //指令名结束
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT; } //指令名结束
				else if (_IsNamePre(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE; } //指令属性名开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_KEY_BEGIN: //指令属性名开始
				if (_IsNameChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = NEXT; } //继续属性名
				else if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE; } //属性名结束
				else if (ma.cur[0] == _T('=')){ ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE; } //属性名结束
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_KEY_END: //指令属性名结束
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('=')){ ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT; } //属性赋值符
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_ASIGN: //指令属性赋值开始
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('\"') || ma.cur[0] == _T('\'')){ ma.sa = NOP; ma.ms = IS_VAL_QUOT; ma.ma = NEXT; } //过渡到属性赋值,掠过"\""
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_VAL_QUOT: //括号
				if (_IsTokenChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = PAUSE; } //开始属性值
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_VAL_BEGIN: //指令属性值开始
				if (ma.cur[0] == _T('\"') || ma.cur[0] == _T('\'')){ ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = PAUSE; } //过渡到属性值结束
				else if (_IsTokenChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = NEXT; } //继续属性值
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_VAL_END:  //指令属性值结束
				if (ma.cur[0] == _T('\"') || ma.cur[0] == _T('\'')){ ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT; } //过渡到属性值结束
				else if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT; } //标记中断
				else if (_IsNamePre(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE; } //属性名开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case IS_CLOSE: //指令关闭
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT; } //标记关闭
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case DS_OPEN: //申明打开
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_BEGIN; ma.ma = NEXT; } //遇到注释符
				else if (ma.cur[0] == _T('[')){ ma.sa = NOP; ma.ms = CD_HEAD_BEGIN; ma.ma = NEXT; } //遇到界段符
				else if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE; } //过渡到声明关闭
				else { ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT; } //忽略声明
				break;
			case CM_BEGIN: //注释文本开始
				if (ma.cur[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_TEXT_BEGIN; ma.ma = NEXT; } //过渡到注释文本开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case CM_TEXT_BEGIN: //注释文本开始
				if (ma.cur[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_TEXT_END; ma.ma = NEXT; } //遇到注释结束
				else { ma.sa = NOP; ma.ms = CM_TEXT_BEGIN; ma.ma = NEXT; } //继续注释文本
				break;
			case CM_TEXT_END: //注释文本结束
				if (ma.cur[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_TEXT_END; ma.ma = NEXT; } //过渡到注释结束
				else if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = CM_END; ma.ma = PAUSE; } //过渡到注释结束
				else { ma.sa = NOP; ma.ms = CM_TEXT_BEGIN; ma.ma = NEXT; } //修正为有效字符
				break;
			case CM_END: //注释结束
				{ ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE; } //过渡到声明关闭
				break;
			case CD_HEAD_BEGIN: //界段头开始
				if (ma.cur[0] == _T('C')){ ma.sa = NOP; ma.ms = CD_HEAD_BEGIN; ma.ma = NEXT; } //掠过界段头
				else if (ma.cur[0] == _T('D')){ ma.sa = NOP; ma.ms = CD_HEAD_BEGIN; ma.ma = NEXT; } //掠过界段头
				else if (ma.cur[0] == _T('A')){ ma.sa = NOP; ma.ms = CD_HEAD_BEGIN; ma.ma = NEXT; } //掠过界段头
				else if (ma.cur[0] == _T('T')){ ma.sa = NOP; ma.ms = CD_HEAD_BEGIN; ma.ma = NEXT; } //掠过界段头
				else if (ma.cur[0] == _T('A')){ ma.sa = NOP; ma.ms = CD_HEAD_BEGIN; ma.ma = NEXT; } //掠过界段头
				else if (ma.cur[0] == _T('[')){ ma.sa = NOP; ma.ms = CD_HEAD_END; ma.ma = NEXT; } //掠过界段头
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case CD_HEAD_END: //界段头结束
				{ ma.sa = NOP; ma.ms = CD_TEXT_BEGIN; ma.ma = PAUSE; } //开始界段文本
				break;
			case CD_TEXT_BEGIN: //界段文本开始
				if (ma.cur[0] == _T(']')){ ma.sa = NOP; ma.ms = CD_TEXT_END; ma.ma = NEXT; } //遇到界段结束
				else { ma.sa = NOP; ma.ms = CD_TEXT_BEGIN; ma.ma = NEXT; } //继续界段文本
				break;
			case CD_TEXT_END: //界段文本结束
				if (ma.cur[0] == _T(']')){ ma.sa = NOP; ma.ms = CD_TAIL_BEGIN; ma.ma = NEXT; } //遇到界段结束
				else { ma.sa = NOP; ma.ms = CD_TEXT_BEGIN; ma.ma = NEXT; } //修正为有效字符
				break;
			case CD_TAIL_BEGIN:
				if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = CD_TAIL_END; ma.ma = PAUSE; } //过渡到界段结束
				else if (ma.cur[0] == _T(']')){ ma.sa = NOP; ma.ms = CD_TAIL_BEGIN; ma.ma = NEXT; }//继续界段结束
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case CD_TAIL_END: //界段结束
				{ ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE; } //过渡到声明关闭
				break;
			case DS_CLOSE: //申明结束
				if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT; } //过渡到标记关闭
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_OPEN: //元素打开
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = NEXT; } //忽略空格
				else if (_IsNamePre(ma.cur[0])){ ma.sa = PUSH; ma.ms = NS_NAME_BEGIN; ma.ma = PAUSE; } //指令名开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_NAME_BEGIN: //元素命名开始
				if (_IsNameChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_NAME_BEGIN; ma.ma = NEXT; } //元素名继续
				else if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE; } //元素名结束
				else if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE; }//元素名结束
				else if (ma.cur[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE; } //元素名结束
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; }	//无效字符
				break;
			case NS_NAME_END: //元素命名结束
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT; } //元素名结束
				else if (ma.cur[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT; } //元素名结束
				else if (_IsNamePre(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE; } //属性名开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_KEY_BEGIN: //元素属性名开始
				if (_IsNameChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = NEXT; } //继续属性名
				else if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE; } //属性名结束
				else if (ma.cur[0] == _T('=')){ ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE; } //属性赋值符
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_KEY_END: //元素属性名结束
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('=')){ ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT; } //属性赋值符
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_ASIGN: //元素属性赋值
				if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('\"') || ma.cur[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_QUOT; ma.ma = NEXT; } //过渡到属性值开始,掠过“\"”
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_VAL_QUOT:
				if (_IsTokenChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = PAUSE; } //开始属性值
				else if (ma.cur[0] == _T('\"') || ma.cur[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = PAUSE; } //过渡到属性值开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_VAL_BEGIN: //元素属性值开始
				if (_IsTokenChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = NEXT; } //继续属性值
				else if (ma.cur[0] == _T('\"') || ma.cur[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = PAUSE; } //过渡到属性值结束
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_VAL_END: //元素属性值结束
				if (ma.cur[0] == _T('\"') || ma.cur[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT; } //属性值结束
				else if (_IsSpaceChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT; } //忽略空格
				else if (ma.cur[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT; } //准备关闭标记
				else if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT; } //标记中断
				else if (_IsNamePre(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE; } //属性名开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_INNER_CLOSE: //元素命名中断
				if (ma.cur[0] == _T('<')){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //标记开始
				else{ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = PAUSE; } //元素文本开始
				break;
			case NS_TEXT_BEGIN: //元素文本开始
				//if (ma.cur[0] == _T('['){ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = SKIP_CDT_HEAD; } //界段开始
				//if (ma.cur[0] == _T(']'){ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = SKIP_CDT_TAIL; } //界段结束
				if (ma.cur[0] == _T('<')){ ma.sa = NOP; ma.ms = NS_TEXT_END; ma.ma = PAUSE; } //过渡到元素文本结束
				else if (_IsTextChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = NEXT; } //继续元素文本
				else{ ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_TEXT_END: //元素文本结束
				if (ma.cur[0] == _T('<')){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //标记开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_REVER_CLOSE: //元素预关闭
				if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE; }//标记结束
				else if (_IsNamePre(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = PAUSE; } //关闭标记的元素名开始
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_REVER_BEGIN: //元素关闭结构中的元素名开始
				if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_REVER_END; ma.ma = PAUSE; } //关闭标记的元素名结束
				else if (_IsNameChar(ma.cur[0])){ ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = NEXT; } //继续关闭标记的元素名
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_REVER_END: //元素关闭结构中的元素名开始
				if (ma.cur[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE; }//标记结束
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			case NS_CLOSE: //元素关闭
				{ma.sa = POP; ma.ms = TG_CLOSE; ma.ma = NEXT; } //过渡到标记关闭
				break;
			case TG_CLOSE: //标记关闭
				{ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = PAUSE; }
				break;
			default:
				{ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //无效字符
				break;
			}
		}
		else if (ma.ma != STOP)
		{
			ma.ma = NEXT;
		}

		//根据自动机状态处置
		if(ma.ms == IS_OPEN)
		{
			idle;
		}else if(ma.ms == IS_NAME_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_name);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					string_cat(vs_name, ma.org, -1);
				break;
			}
		}else if(ma.ms == IS_NAME_END)
		{
			idle;
		}else if(ma.ms == IS_KEY_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_key);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					string_cat(vs_key, ma.org, -1);
				break;
			}
		}else if(ma.ms == IS_KEY_END)
		{
			idle;
		}else if(ma.ms == IS_ASIGN)
		{
			idle;
		}else if(ma.ms == IS_VAL_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_val);
				break;
			case NEXT:
				string_cat(vs_val, ma.org, -1);
				break;
			}
		}else if(ma.ms == IS_VAL_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if(!pbo->isdom && xsicmp(DOC_XML,string_ptr(vs_name)) == 0)
				{
					if(xsicmp(XML_ATTR_ENCODING,string_ptr(vs_key)) == 0)
					{
						if (pxp->pf_head_write_attr)
						{
							ret = (*pxp->pf_head_write_attr)(pxp->obj, string_ptr(vs_key), string_len(vs_key), string_ptr(vs_val), string_len(vs_val));
						}

						if (xsicmp(CHARSET_UTF8, string_ptr(vs_val)) == 0)
						{
							ma.enc = _UTF8;
							if (pbo->pf_set_encode)
							{
								(*pbo->pf_set_encode)(pbo->obj, ma.enc);
							}
						}
						else if (xsicmp(CHARSET_GB2312, string_ptr(vs_val)) == 0)
						{
							ma.enc = _GB2312;
							if (pbo->pf_set_encode)
							{
								(*pbo->pf_set_encode)(pbo->obj, ma.enc);
							}
						}
					}
					else if (xsicmp(XML_ATTR_VERSION, string_ptr(vs_key)) == 0)
					{
						if (pxp->pf_head_write_attr)
						{
							ret = (*pxp->pf_head_write_attr)(pxp->obj, string_ptr(vs_key), string_len(vs_key), string_ptr(vs_val), string_len(vs_val));
						}
					}
				}
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == IS_CLOSE)
		{
			idle;
		}

		if(ma.ms == NS_OPEN)
		{
			idle;
		}else if(ma.ms == NS_NAME_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_name);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					string_cat(vs_name, ma.org, -1);
				break;
			}
		}else if(ma.ms == NS_NAME_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				nsstr = (tchar_t*)string_ptr(vs_name);
				if (split_xmlns(nsstr, &nsat, &nslen, &vsat, &vslen))
				{
					if (pxp->pf_node_write_name)
					{
						ret = (*pxp->pf_node_write_name)(pxp->obj, nsstr + nsat, nslen, nsstr + vsat, vslen);
					}
				}
				else
				{
					if (pxp->pf_node_write_name)
					{
						ret = (*pxp->pf_node_write_name)(pxp->obj, NULL, 0, string_ptr(vs_name), string_len(vs_name));
					}
				}
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == NS_KEY_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_key);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					string_cat(vs_key, ma.org, -1);
				break;
			}
		}else if(ma.ma == NS_KEY_END)
		{
			idle;
		}else if(ma.ma == NS_ASIGN)
		{
			idle;
		}else if(ma.ms == NS_VAL_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_val);
				break;
			case NEXT:
				string_cat(vs_val, ma.org, -1);
				break;
			}
		}else if(ma.ms == NS_VAL_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if (xsnicmp(XMLNS, string_ptr(vs_key), XMLNS_LEN) == 0)
				{
					if (pxp->pf_node_write_xmlns)
					{
						ret = (*pxp->pf_node_write_xmlns)(pxp->obj, string_ptr(vs_key), string_len(vs_key), string_ptr(vs_val), string_len(vs_val));
					}
				}
				else
				{
					if (pxp->pf_node_write_attr)
					{
						ret = (*pxp->pf_node_write_attr)(pxp->obj, string_ptr(vs_key), string_len(vs_key), string_ptr(vs_val), string_len(vs_val));
					}
				}
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == NS_TEXT_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				string_empty(vs_text);
				break;
			case NEXT:
				string_cat(vs_text, ma.org, -1);
				break;
			}
		}else if(ma.ms == NS_TEXT_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if (pxp->pf_node_write_text)
				{
					ret = (*pxp->pf_node_write_text)(pxp->obj, 0, string_ptr(vs_text), string_len(vs_text));
				}
				break;
			case NEXT:
				break;
			}
		}
		else if (ma.ms == CD_TEXT_BEGIN)
		{
			switch (ma.ma)
			{
			case PAUSE:
				string_empty(vs_text);
				break;
			case NEXT:
				string_cat(vs_text, ma.org, -1);
				break;
			}
		}
		else if (ma.ms == CD_TEXT_END)
		{
			switch (ma.ma)
			{
			case PAUSE:
				break;
			case NEXT:
				string_cat(vs_text, ma.org, -1);
				break;
			}
		}
		else if (ma.ms == CD_TAIL_END)
		{
			switch (ma.ma)
			{
			case PAUSE:
				if (!(*pxp->pf_has_node)(pxp->obj))
					break;

				string_del_chars(vs_text, string_len(vs_text)-1, 1);

				if (pxp->pf_node_write_text)
				{
					ret = (*pxp->pf_node_write_text)(pxp->obj, 1, string_ptr(vs_text), string_len(vs_text));
				}
				break;
			case NEXT:
				break;
			}
		}
		else if (ma.ms == NS_REVER_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == NS_REVER_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if (!(*pxp->pf_has_node)(pxp->obj))
				{
					ma.ms = NIL_FAILED;
				}
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == NS_CLOSE)
		{
			idle;
		}

		if(ma.sa == PUSH)
		{
			if (pxp->pf_node_write_begin)
			{
				ret = (*pxp->pf_node_write_begin)(pxp->obj);
			}

			ma.sa = NOP;
		}else if(ma.sa == POP)
		{
			if (pxp->pf_node_write_end)
			{
				ret = (*pxp->pf_node_write_end)(pxp->obj);
			}

			if (!ret)
			{
				ma.ms = NIL_SKIP;
				ma.ma = SKIP;
				ret = 1;
			}

			ma.sa = NOP;
		}

		if (!ret)
		{
			ma.ms = NIL_FAILED;
		}

		if (ma.ms != NIL_SKIP && ma.ma == NEXT && ma.cur[0] == _T('\0'))
		{
			ma.ms = NIL_FAILED;
		}

		//根据自动机的状态决定是否退出
		if (ma.ms == NIL_FAILED || ma.ms == NIL_BREAK || ma.ms == NIL_SUCCEED)
			break;
	}

	if (ma.ms == NIL_SUCCEED && (*pxp->pf_has_node)(pxp->obj))
	{
		ma.ms = NIL_FAILED;
	}

	if (pxp->pf_write_end)
	{
		(*pxp->pf_write_end)(pxp->obj, ma.ms);
	}

	string_free(vs_name);
	string_free(vs_key);
	string_free(vs_val);
	string_free(vs_text);

	if (ma.ms != NIL_SUCCEED)
	{
		xsprintf(errtext, _T("xml parse break at bytes: %d, the last byte is %s\n"), ma.bytes, ma.org);

		set_last_error(_T("-1"), errtext, -1);
	}

	//自动机的最终状态
	return (ma.ms == NIL_SUCCEED || ma.ms == NIL_BREAK) ? 1 : 0;
}

/***************************************************************************************************************/

bool_t parse_xml_doc_from_object(link_t_ptr ptr, if_operator_t* pbo)
{
	xml_writer_t xp = { 0 };
	XMLOBJECT xo = { 0 };

	clear_xml_doc(ptr);
	xo.doc = ptr;

	xp.obj = (void*)&xo;
	xp.pf_write_begin = call_write_xml_begin;
	xp.pf_write_end = call_write_xml_end;
	xp.pf_head_write_attr = call_write_xml_head_attr;
	xp.pf_node_write_attr = call_write_xml_node_attr;
	xp.pf_node_write_begin = call_write_xml_node_begin;
	xp.pf_node_write_end = call_write_xml_node_end;
	xp.pf_node_write_name = call_write_xml_node_name;
	xp.pf_node_write_text = call_write_xml_node_text;
	xp.pf_node_write_xmlns = call_write_xml_node_xmlns;
	xp.pf_has_node = call_write_xml_has_node;

	return parse_xml(&xp, pbo->encode, pbo);
}

bool_t parse_dom_doc_from_object(link_t_ptr ptr, if_operator_t* pbo)
{
	xml_writer_t xp = { 0 };
	XMLOBJECT xo = { 0 };

	delete_dom_child_nodes(ptr);
	xo.doc = ptr;

	xp.obj = (void*)&xo;
	xp.pf_write_begin = call_write_xml_begin;
	xp.pf_write_end = call_write_xml_end;
	xp.pf_node_write_attr = call_write_xml_node_attr;
	xp.pf_node_write_begin = call_write_xml_node_begin;
	xp.pf_node_write_end = call_write_xml_node_end;
	xp.pf_node_write_name = call_write_xml_node_name;
	xp.pf_node_write_text = call_write_xml_node_text;
	xp.pf_node_write_xmlns = call_write_xml_node_xmlns;
	xp.pf_has_node = call_write_xml_has_node;

	return parse_xml(&xp, pbo->encode, pbo);
}

/***************************************************************************************************************************/

void format_xml(xml_reader_t* pxp, if_operator_t* pbo)
{
	link_t_ptr doc, nlk;
	link_t_ptr st, slk;
	link_t_ptr ht, elk;
	hash_enum_t he = { 0 };

	link_t_ptr stk = NULL;

	const tchar_t *node_ns, *node_name, *node_key, *node_val, *node_text;
	bool_t b_cdata, b_close, b_parent;
	int n_indent = 0;

	tchar_t encoding[RES_LEN], version[RES_LEN];

	doc = ((XMLOBJECT*)pxp->obj)->doc;

	if (IS_XML_DOC(doc))
	{
		get_xml_encoding(doc, encoding, RES_LEN);
		get_xml_version(doc, version, RES_LEN);

		if (pxp->pf_head_read_begin)
		{
			if (!(*pxp->pf_head_read_begin)(pbo))
				return;
		}

		if (pxp->pf_head_read_attr)
		{
			if (!(*pxp->pf_head_read_attr)(pbo, XML_ATTR_VERSION, version))
				return;

			if (!(*pxp->pf_head_read_attr)(pbo, XML_ATTR_ENCODING, encoding))
				return;
		}

		if (pxp->pf_head_read_end)
		{
			if (!(*pxp->pf_head_read_end)(pbo))
				return;
		}
	}

	if (IS_XML_DOC(doc))
	{
		doc = get_xml_dom_node(doc);
	}

	nlk = doc;
	while (nlk)
	{
		node_name = get_dom_node_name_ptr(nlk);
		node_ns = get_dom_node_ns_ptr(nlk);

		node_text = get_dom_node_text_ptr(nlk);
		b_cdata = get_dom_node_cdata(nlk);

		b_parent = (get_dom_first_child_node(nlk) == NULL) ? 0 : 1;

		b_close = (b_parent || !is_null(node_text)) ? 1 : 0;

		if (pxp->pf_node_read_begin)
		{
			if (!(*pxp->pf_node_read_begin)(pbo, n_indent, b_parent, node_ns, node_name))
				break;
		}

		//xmlns table
		st = get_dom_node_name_table(nlk);
		//attr table
		ht = get_dom_node_attr_table(nlk);

		//xmlns table
		slk = (st) ? get_string_next_entity(st, LINK_FIRST) : NULL;
		while (slk)
		{
			node_key = get_string_entity_key_ptr(slk);
			node_val = get_string_entity_val_ptr(slk);

			if (pxp->pf_node_read_xmlns)
			{
				if (!(*pxp->pf_node_read_xmlns)(pbo, node_key, node_val))
					break;
			}

			slk = get_string_next_entity(st, slk);
		}

		he.hash = ht;
		he.entity = LINK_FIRST;
		elk = (ht) ? get_hash_next_entity(&he) : NULL;
		while (elk)
		{
			node_key = get_hash_entity_key_ptr(elk);
			node_val = get_hash_entity_val_ptr(elk);

			if (pxp->pf_node_read_attr)
			{
				if (!(*pxp->pf_node_read_attr)(pbo, node_key, node_val))
					break;
			}

			elk = get_hash_next_entity(&he);
		}

		if (b_close)
		{
			if (pxp->pf_node_read_close)
			{
				if (!(*pxp->pf_node_read_close)(pbo, n_indent, b_parent))
					break;
			}
		}

		if (pxp->pf_node_read_text)
		{
			if (!(*pxp->pf_node_read_text)(pbo, b_cdata, node_text, -1))
				break;
		}

		if (get_dom_first_child_node(nlk))
		{
			if (!stk)
				stk = create_stack_table();
			push_stack_node(stk, (void*)nlk);

			n_indent++;
			nlk = get_dom_first_child_node(nlk);
			continue;
		}

		while (nlk)
		{
			if (pxp->pf_node_read_end)
			{
				node_name = get_dom_node_name_ptr(nlk);
				node_ns = get_dom_node_ns_ptr(nlk);

				b_parent = (get_dom_first_child_node(nlk) == NULL) ? 0 : 1;

				if (!(*pxp->pf_node_read_end)(pbo, n_indent, b_parent, b_close, node_ns, node_name))
					break;
			}

			nlk = (nlk == doc) ? NULL : get_dom_next_sibling_node(nlk);
			if (nlk)
				break;

			n_indent--;
			b_close = 1;
			nlk = (stk) ? (link_t_ptr)pop_stack_node(stk) : NULL;
		}
	}

	if (stk)
		destroy_stack_table(stk);
}

bool_t format_dom_doc_to_object(link_t_ptr dom, if_operator_t* pbo)
{
	xml_reader_t xr = { 0 };
	XMLOBJECT xo = { 0 };

	xo.doc = dom;
	xr.obj = (void*)&xo;

	xr.pf_node_read_begin = call_read_xml_node_begin;
	xr.pf_node_read_close = call_read_xml_node_close;
	xr.pf_node_read_end = call_read_xml_node_end;
	xr.pf_node_read_attr = call_read_xml_node_attr;
	xr.pf_node_read_xmlns = call_read_xml_node_attr;
	xr.pf_node_read_text = call_read_xml_node_text;

	format_xml(&xr, pbo);

	return (pbo->pos < 0) ? 0 : 1;
}

bool_t format_xml_doc_to_object(link_t_ptr dom, if_operator_t* pbo)
{
	xml_reader_t xr = { 0 };
	XMLOBJECT xo = { 0 };

	xo.doc = dom;
	xr.obj = (void*)&xo;

	xr.pf_head_read_begin = call_read_xml_head_begin;
	xr.pf_head_read_attr = call_read_xml_head_attr;
	xr.pf_head_read_end = call_read_xml_head_end;

	xr.pf_node_read_begin = call_read_xml_node_begin;
	xr.pf_node_read_close = call_read_xml_node_close;
	xr.pf_node_read_end = call_read_xml_node_end;
	xr.pf_node_read_attr = call_read_xml_node_attr;
	xr.pf_node_read_xmlns = call_read_xml_node_attr;
	xr.pf_node_read_text = call_read_xml_node_text;

	format_xml(&xr, pbo);

	return (pbo->pos < 0) ? 0 : 1;
}


#endif /*XDL_SUPPORT_DOC*/