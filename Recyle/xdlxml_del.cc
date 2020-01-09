/************************************************************
	Easily xdl v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	xdlxml.cc | xml utility implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
************************************************************/

#include "xdlxml.h"
#include "xmldoc.h"
#include "domdoc.h"
#include "stacktable.h"
#include "xdlstr.h"
#include "xdlmem.h"
#include "xdlvar.h"
#include "xdlcode.h"

	
//定义转义符
#define A_LT		"&lt;"	// < 
#define W_LT		L"&lt;"	// < 
#define LT_LEN	4

#define A_GT		"&gt;"	// > 
#define W_GT		L"&gt;"	// > 
#define GT_LEN	4

#define A_AMP		"&amp;"	// & 
#define W_AMP		L"&amp;"	// & 
#define AMP_LEN	5

#define A_APOS		"&apos;"	// ' 
#define W_APOS		L"&apos;"	// ' 
#define APOS_LEN	6

#define A_QUOT		"&quot;"	// " 
#define W_QUOT		L"&quot;"	// " 
#define QUOT_LEN	6

#define A_SPAC		"&nbsp;" //  
#define W_SPAC		L"&nbsp;" //  
#define SPAC_LEN	6		

#define A_QDIV		"&div;" // / 
#define W_QDIV		L"&div;" // / 
#define QDIV_LEN	5		

//定义注解符
#define A_CMTOKEN		"--"
#define W_CMTOKEN		L"--"
#define CMTOKEN_LEN		2

//定义界段符前缀
#define A_CDATA_HEAD		"[CDATA["
#define W_CDATA_HEAD		L"[CDATA["
#define CDATA_HEAD_LEN		7
//定义界段符尾缀
#define A_CDATA_TAIL		"]]"
#define W_CDATA_TAIL		L"]]"
#define CDATA_TAIL_LEN		2

//以下定义自动机的状态
typedef enum{
	NIL_FAILED	= -1,	//错误中断
	NIL_SUCCEED = 0,	//解析正常结束
	NIL_SKIP = 1,		//略过
	TG_OPEN = 20,		//标记打开
	TG_CLOSE = 21,		//标记关闭
	NS_OPEN = 25,		//元素打开
	NS_INNER_CLOSE = 26,	//元素内部中断
	NS_REVER_CLOSE = 27,	//元素预关闭
	NS_CLOSE = 28,		//元素关闭
	NS_NAME_BEGIN = 30,	//元素名开始
	NS_NAME_END = 31,	//元素名结束
	NS_KEY_BEGIN = 32,	//元素属性名开始
	NS_KEY_END = 33,	//元素属性名结束
	NS_ASIGN = 34,		//元素属性赋值
	NS_VAL_BEGIN = 35,	//元素属性开始
	NS_VAL_END = 36,	//元素属性结束
	NS_TEXT_BEGIN = 37,	//元素文本开始
	NS_TEXT_END = 38,	//元素文本结束
	NS_REVER_BEGIN = 39,//关闭标记中的元素名开始
	NS_REVER_END = 300,	//关闭标记中的元素名开始
	IS_OPEN = 40,		//指令打开
	IS_NAME_BEGIN = 41, //指令名开始
	IS_NAME_END = 42,	//指令名结束
	IS_KEY_BEGIN = 43,	//指令键开始
	IS_KEY_END = 44,	//指令键结束
	IS_ASIGN = 45,		//赋值符
	IS_VAL_BEGIN = 46,	//指令值开始
	IS_VAL_END = 47,	//指令值结束
	IS_CLOSE = 48,		//指令关闭
	DS_OPEN = 50,		//声明打开
	DS_SKIP = 51,		//忽略声明
	DS_CLOSE = 52,		//声明关闭
	DS_NAME_BEGIN = 53,		//申明名称开始
	DS_NAME_END = 54,		//申明名称结束
	DS_TEXT_BEGIN = 55,	//申明文本开始
	DS_TEXT_END = 56	//申明文本结束
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
	NEXT = 1,	//移至下一字符
	SKIP = 2,	//略过字符
	STOP = -1	//停机	
}MataAction;


//定义自动机
typedef struct _Mata{
	int sa;		//栈动作
	int ma;		//读写头动作
	int ms;		//当前状态
}Mata;

//将转义串转换为正常串
int escape_decode_byte(const byte_t* str,wchar_t* buf)
{
	if(!a_xsncmp((schar_t*)str,A_LT,LT_LEN))
	{
		*buf = L'<';
		return LT_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_GT,GT_LEN))
	{
		*buf = L'>';
		return GT_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_AMP,AMP_LEN))
	{
		*buf = L'&';
		return AMP_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_APOS,APOS_LEN))
	{
		*buf = L'\'';
		return APOS_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_QUOT,QUOT_LEN))
	{
		*buf = L'\"';
		return QUOT_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_QDIV,QDIV_LEN))
	{
		*buf = L'/';
		return QDIV_LEN;
	}else
	{
		*buf = (wchar_t)(*str);
		return 1;
	}
}

int escape_decode_byte_length(const byte_t* str)
{
	if(!a_xsncmp((schar_t*)str,A_LT,LT_LEN))
	{
		return LT_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_GT,GT_LEN))
	{
		return GT_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_AMP,AMP_LEN))
	{
		return AMP_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_APOS,APOS_LEN))
	{
		return APOS_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_QUOT,QUOT_LEN))
	{
		return QUOT_LEN;
	}else if(!a_xsncmp((schar_t*)str,A_QDIV,QDIV_LEN))
	{
		return QDIV_LEN;
	}else
	{
		return 1;
	}
}

//将正常串转换为转义串
int escape_encode_byte(wchar_t ch,byte_t* buf)
{
	if(ch == L'<')
	{
		a_xscpy((schar_t*)buf,A_LT);
		return LT_LEN;
	}else if(ch == L'>')
	{
		a_xscpy((schar_t*)buf,A_GT);
		return GT_LEN;
	}else if(ch == L'&')
	{
		a_xscpy((schar_t*)buf,A_AMP);
		return AMP_LEN;
	}else if(ch == L'\'')
	{
		a_xscpy((schar_t*)buf,A_APOS);
		return APOS_LEN;
	}else if(ch == L'\"')
	{
		a_xscpy((schar_t*)buf,A_QUOT);
		return QUOT_LEN;
	}else if(ch == L'/')
	{
		a_xscpy((schar_t*)buf,A_QDIV);
		return QDIV_LEN;
	}else
	{
		buf[0] = (byte_t)ch;
		return 1;
	}
}

int escape_encode_byte_length(wchar_t ch)
{
	if(ch == L'<')
	{
		return LT_LEN;
	}else if(ch == L'>')
	{
		return GT_LEN;
	}else if(ch == L'&')
	{
		return AMP_LEN;
	}else if(ch == L'\'')
	{
		return APOS_LEN;
	}else if(ch == L'\"')
	{
		return QUOT_LEN;
	}else if(ch == L'/')
	{
		return QDIV_LEN;
	}else
	{
		return 1;
	}
}


//判断是否需转义的字符
#define _isEscapeChar(ch) ((ch == L'<' || ch == L'>' || ch == L'&' || ch == L'\"' || ch == L'\'' || ch == L'/')? 1 : 0)

//判断是否有效命名首字符
#define _IsNamePre(ch) ((ch == L'_' || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z') || ch < 0)? 1: 0)

//判断是否为有效命名符
#define _IsNameChar(ch) ((ch == L'_' || ch == L'-' || ch == L'.' || ch == L':' || (ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z') || ch < 0)? 1: 0)

//判断是否为空格符
#define _IsSpaceChar(ch) ((ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r')? 1 : 0)

//判断是否有效文本符
//#define _IsTokenChar(wchar_t ch) ((ch != L'<' && ch != L'>' && ch != L'\"' && ch != L'\'' && ch != L'/')? 1 : 0)
#define _IsTokenChar(ch) ((ch != L'<' && ch != L'>' && ch != L'\"' && ch != L'\'')? 1 : 0)

//判断是否需转义的字符
#define _IsEscapeChar(ch) ((ch == L'<' || ch == L'>' || ch == L'&' || ch == L'\"' || ch == L'\'' || ch == L'/')? 1 : 0)

#define idle		while(0 == 1)

//解析XML文本
bool_t parse_xml_doc(link_t_ptr doc,const byte_t* str,int len)
{
	wchar_t ch;
	bool_t b_utf8;
	byte_t* token;
	link_t_ptr nlk;
	int bytes;

	assert(doc);

	link_t_ptr st;
	handle_t v_node_name,v_node_text,v_attr_key,v_attr_val;

	Mata ma = {0};

	if(!str || !len)
		return 1;

	if(len < 0)
		len = MAX_LONG;

	clear_xml_doc(doc);

	//自动机初始化
	ma.ma = NEXT;
	ma.sa = NOP;
	ma.ms = NIL_SKIP;

	st = create_stack_table();
	nlk = NULL;

	v_node_name = w_varstr_alloc(256,256);
	v_node_text = w_varstr_alloc(1024,512);
	v_attr_key = w_varstr_alloc(256,256);
	v_attr_val = w_varstr_alloc(256,256);

	b_utf8 = 0;

	token = (byte_t*)str;
	bytes = 0;

	while(ma.ma != STOP)
	{
		//判断读写头的动作
		switch(ma.ma)
		{
		case NEXT:
		case SKIP:
			token += bytes;
			//处理转义符
			if(*token == '&')
			{
				bytes = escape_decode_byte(token,&ch);
			}else
			{
				if(b_utf8)
					bytes = utf8_decode_byte(token,&ch);
				else
					bytes = gb2312_decode_byte(token,&ch);
			}
			break;
		case PAUSE:
			break;
		}

		//根据自动机状态跳转
		switch(ma.ms) 
		{
		case NIL_SKIP: //空闲略过
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = NEXT;}	//忽略起始空格
			else if(ch == L'<'){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //遇到标记
			else if(ch == L'\0') {ma.sa = NOP; ma.ms = NIL_SUCCEED, ma.ma = STOP;}	//终结符
			else if(token == str + len) {ma.sa = NOP; ma.ms = NIL_SUCCEED, ma.ma = STOP;}	//串终止
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;}	//无效字符
			break;
		case TG_OPEN: //标记打开
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //忽略空格
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT;}	//遇到指令符
			else if(ch == L'!'){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //遇到声明符
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT;} //准备关闭标记
			else {ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = PAUSE;} //元素开始
			break;
		case IS_OPEN: //指令打开
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT;} //忽略空格
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = PAUSE;} //指令名开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_NAME_BEGIN: //指令名开始
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = NEXT;} //继续指令名
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE;} //指令名结束
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE;} //指令名结束
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_NAME_END: //指令名结束
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = NEXT;} //忽略空格
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT;} //指令名结束
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE;} //指令属性名开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_KEY_BEGIN: //指令属性名开始
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = NEXT;} //继续属性名
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE;} //属性名结束
			else if(ch == L'='){ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE;} //属性名结束
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_KEY_END: //指令属性名结束
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = NEXT;} //忽略空格
			else if(ch == L'='){ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT;} //属性赋值符
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_ASIGN: //指令属性赋值开始
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT;} //忽略空格
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = SKIP;} //过渡到属性赋值,掠过"\""
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_VAL_BEGIN: //指令属性值开始
			if(_IsTokenChar(ch)){ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = NEXT;} //继续属性值
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = PAUSE;} //过渡到属性值结束
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_VAL_END:  //指令属性值结束
			if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT;} //过渡到属性值结束
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT;} //忽略空格
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT;} //标记中断
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE;} //属性名开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case IS_CLOSE: //指令关闭
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT;} //忽略空格
			else if(ch == L'>'){ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT;} //标记关闭
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case DS_OPEN: //申明打开
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //忽略空格
			else if(ch == L'-'){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //遇到注释符
			else if(ch == L'['){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //遇到界段符
			else if(ch == L'>'){ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE;} //过渡到声明关闭
			else {ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //忽略声明
			break;
		case DS_CLOSE: //申明结束
			if(ch == L'>'){ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT;} //过渡到标记关闭
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_OPEN: //元素打开
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = NEXT;} //忽略空格
			else if(_IsNamePre(ch)){ma.sa = PUSH; ma.ms = NS_NAME_BEGIN; ma.ma = PAUSE;} //指令名开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_NAME_BEGIN: //元素命名开始
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = NS_NAME_BEGIN; ma.ma = NEXT;} //元素名继续
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE;} //元素名结束
			else if(ch == L'>'){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE;}//元素名结束
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE;} //元素名结束
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;}	//无效字符
			break;
		case NS_NAME_END: //元素命名结束
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = NEXT;} //忽略空格
			else if(ch == L'>'){ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT;} //元素名结束
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT;} //元素名结束
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE;} //属性名开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_KEY_BEGIN: //元素属性名开始
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = NEXT;} //继续属性名
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE;} //属性名结束
			else if(ch == L'='){ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE;} //属性赋值符
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_KEY_END: //元素属性名结束
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = NEXT;} //忽略空格
			else if(ch == L'='){ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT;} //属性赋值符
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_ASIGN: //元素属性赋值
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT;} //忽略空格
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = SKIP;} //过渡到属性值开始,掠过“\"”
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_VAL_BEGIN: //元素属性值开始
			if(_IsTokenChar(ch)){ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = NEXT;} //继续属性值
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = PAUSE;} //过渡到属性值结束
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_VAL_END: //元素属性值结束
			if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT;} //属性值结束
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT;} //忽略空格
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT;} //准备关闭标记
			else if(ch == L'>'){ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT;} //标记中断
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE;} //属性名开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_INNER_CLOSE: //元素命名中断
			if(ch == L'<'){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //标记开始
			else{ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = PAUSE;} //元素文本开始
			break;
		case NS_TEXT_BEGIN: //元素文本开始
			if(ch == L'<'){ma.sa = NOP; ma.ms = NS_TEXT_END; ma.ma = PAUSE;} //过渡到元素文本结束
			else if(ch == L'['){ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = SKIP;} //界段开始
			else if(_IsTokenChar(ch)){ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = NEXT;} //继续元素文本
			else{ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_TEXT_END: //元素文本结束
			if(ch == L'<'){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //标记开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_REVER_CLOSE: //元素预关闭
			if(ch == L'>'){ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE;}//标记结束
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = PAUSE;} //关闭标记的元素名开始
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_REVER_BEGIN: //元素关闭结构中的元素名开始
			if(ch == L'>'){ma.sa = NOP; ma.ms = NS_REVER_END; ma.ma = PAUSE;} //关闭标记的元素名结束
			else if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = NEXT;} //继续关闭标记的元素名
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_REVER_END: //元素关闭结构中的元素名开始
			if(ch == L'>'){ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE;}//标记结束
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
		case NS_CLOSE: //元素关闭
			{ma.sa = POP; ma.ms = TG_CLOSE; ma.ma = NEXT;} //过渡到标记关闭
			break;
		case TG_CLOSE: //标记关闭
			{ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = PAUSE;}
			break;
		default:
			{ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //无效字符
			break;
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
				w_varstr_empty(v_node_name);
				break;
			case NEXT:
				w_varstr_ins_char(v_node_name,-1,ch);
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
				w_varstr_empty(v_attr_key);
				break;
			case NEXT:
				w_varstr_ins_char(v_attr_key,-1,ch);
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
				break;
			case SKIP:
				w_varstr_empty(v_attr_val);
				break;
			case NEXT:
				w_varstr_ins_char(v_attr_val,-1,ch);
				break;
			}
		}else if(ma.ms == IS_VAL_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if(w_xsicmp(ATTR_XML,w_varstr_ptr(v_node_name)) == 0)
				{
					set_xml_attr(doc,w_varstr_ptr(v_attr_key),-1,w_varstr_ptr(v_attr_val),-1);

					if(w_xsicmp(ATTR_ENCODING,w_varstr_ptr(v_attr_key)) == 0)
					{
						if(w_xsicmp(ATTR_ENCODING_UTF,w_varstr_ptr(v_attr_val)) == 0)
							b_utf8 = 1;
						else
							b_utf8 = 0;
					}
				}
				w_varstr_empty(v_attr_key);
				w_varstr_empty(v_attr_val);
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == IS_CLOSE)
		{
			w_varstr_empty(v_node_name);
		}

		if(ma.ms == NS_OPEN)
		{
			idle;
		}else if(ma.ms == NS_NAME_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				w_varstr_empty(v_node_name);
				break;
			case NEXT:
				w_varstr_ins_char(v_node_name,-1,ch);
				break;
			}
		}else if(ma.ms == NS_NAME_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				set_dom_node_name(nlk,w_varstr_ptr(v_node_name),-1);
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == NS_KEY_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				w_varstr_empty(v_attr_key);
				break;
			case NEXT:
				w_varstr_ins_char(v_attr_key,-1,ch);
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
				break;
			case SKIP:
				w_varstr_empty(v_attr_val);
				break;
			case NEXT:
				w_varstr_ins_char(v_attr_val,-1,ch);
				break;
			}
		}else if(ma.ms == NS_VAL_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				set_dom_node_attr(nlk,w_varstr_ptr(v_attr_key),-1,w_varstr_ptr(v_attr_val),-1);
				w_varstr_empty(v_attr_key);
				w_varstr_empty(v_attr_val);
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == NS_TEXT_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				w_varstr_empty(v_node_text);
				break;
			case NEXT:
				w_varstr_ins_char(v_node_text,-1,ch);
				break;
			case SKIP:
				if(a_xsnicmp((schar_t*)token,A_CDATA_HEAD,CDATA_HEAD_LEN) == 0)
				{
					w_varstr_empty(v_node_text);

					bytes = CDATA_HEAD_LEN;

					while(1)
					{
						if(*(token + bytes) == ']')
						{
							if(a_xsnicmp((schar_t*)(token + bytes),A_CDATA_TAIL,CDATA_TAIL_LEN) == 0)
								break;
						}

						bytes ++;
					}

					bytes += CDATA_TAIL_LEN;

					ma.ma = NEXT;
					set_dom_node_cdata(nlk,(void*)((schar_t*)(token + CDATA_HEAD_LEN)),bytes - CDATA_HEAD_LEN - CDATA_TAIL_LEN);
				}else
				{
					w_varstr_ins_char(v_node_text,-1,ch);
					ma.ma = NEXT;
				}
				break;
			}
		}else if(ma.ms == NS_TEXT_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if(!get_dom_node_mask_check(nlk,MSK_CDATA))
					set_dom_node_text(nlk,w_varstr_ptr(v_node_text),-1);
				w_varstr_empty(v_node_text);
				break;
			case NEXT:
				break;
			}
		}else if(ma.ms == NS_REVER_BEGIN)
		{
			switch(ma.ma)
			{
			case PAUSE:
				w_varstr_empty(v_node_name);
				break;
			case NEXT:
				w_varstr_ins_char(v_node_name,-1,ch);
				break;
			}
		}else if(ma.ms == NS_REVER_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if(!nlk)
				{
					ma.ms = NIL_FAILED;
					break;
				}
				if(w_xsicmp(get_dom_node_name_ptr(nlk),w_varstr_ptr(v_node_name)) != 0)
				{
					ma.ms = NIL_FAILED;
					break;
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
			push_stack_item(st,(void*)nlk);
			if(!nlk)
				nlk = get_xml_dom_node(doc);
			else
				nlk = insert_dom_node(nlk,LINK_LAST);
		}else if(ma.sa == POP)
		{
			nlk = (link_t_ptr)pop_stack_item(st);
		}

		//根据自动机的状态决定是否退出
		if(ma.ms == NIL_FAILED || ma.ms == NIL_SUCCEED)
			break;
	}

	if(nlk != NULL)
	{
		ma.ms = NIL_FAILED;
	}

	destroy_stack_table(st);

	w_varstr_free(v_node_name);
	w_varstr_free(v_node_text);
	w_varstr_free(v_attr_key);
	w_varstr_free(v_attr_val);

	//自动机的最终状态
	return (ma.ms == NIL_SUCCEED)? 1 : 0;
}


int format_xml_doc_length(link_t_ptr ptr)
{
	return format_xml_doc(ptr,NULL,MAX_LONG);
}

int format_xml_doc(link_t_ptr ptr,byte_t* buf,int max)
{
	int len,size = 0;
	link_t_ptr nlk;
	link_t_ptr stack = NULL;
	bool_t b_cdata,b_utf8;
	const wchar_t *node_name,*node_text,*node_key,*node_val,*xml_key,*xml_val;
	int name_len,text_len,key_len,val_len,esc_len,enc_len,i;

	link_t_ptr slk,st;
	link_t_ptr ent,ht;
	hash_enum_t he = {0};

	b_utf8 = (xsicmp(get_xml_encoding_ptr(ptr),ATTR_ENCODING_UTF) == 0)? 1 : 0;

	len = a_xslen("<?xml ");
	if(size + len > max)
	{
		goto err_ret;
	}
	if(buf)
	{
		a_xsncpy((schar_t*)(buf + size),"<?xml ",len);
	}
	size += len;

	st = get_xml_attr_table(ptr);
	slk = get_string_next_entity(st,LINK_FIRST);
	while(slk)
	{
		xml_key = get_string_entity_key_ptr(slk);
		key_len = w_xslen(xml_key);

		if(b_utf8)
			enc_len = utf8_encode(xml_key,key_len,NULL,0);
		else
			enc_len = gb2312_encode(xml_key,key_len,NULL,0);

		//=
		len = enc_len + 1;
		if(size + len > max)
			goto err_ret;

		if(buf)
		{
			len = 0;

			if(b_utf8)
				len += utf8_encode(xml_key,key_len,(byte_t*)(buf + size + len),enc_len);
			else
				len += gb2312_encode(xml_key,key_len,(byte_t*)(buf + size + len),enc_len);

			a_xsncpy((schar_t*)(buf + size + len),"=",1);
			len ++;
		}
		size += len;

		xml_val = get_string_entity_val_ptr(slk);
		val_len = w_xslen(xml_val);

		if(b_utf8)
			enc_len = utf8_encode(xml_val,val_len,NULL,0);
		else
			enc_len = gb2312_encode(xml_val,val_len,NULL,0);

		//" "
		len = enc_len + 3;
		if(size + len > max)
			goto err_ret;

		if(buf)
		{
			len = 0;
			a_xsncpy((schar_t*)(buf + size + len),"\"",1);
			len ++;
			
			if(b_utf8)
				len += utf8_encode(xml_val,val_len,(byte_t*)(buf + size + len),enc_len);
			else
				len += gb2312_encode(xml_val,val_len,(byte_t*)(buf + size + len),enc_len);

			a_xsncpy((schar_t*)(buf + size + len),"\" ",2);
			len += 2;
		}
		size += len;

		slk = get_string_next_entity(st,slk);
	}
	
	//?>
	len = 2;
	if(size + len > max)
		goto err_ret;
	
	if(buf)
	{
		len = 0;
		a_xsncpy((schar_t*)(buf + size + len),"?>",2);
		len += 2;
	}
	size += len;

	stack = create_stack_table();

	nlk = get_xml_dom_node(ptr);
	while(nlk)
	{
		node_name = get_dom_node_name_ptr(nlk);
		name_len = xslen(node_name);

		//<node_name 
		if(b_utf8)
			enc_len = utf8_encode(node_name,name_len,NULL,0);
		else
			enc_len = gb2312_encode(node_name,name_len,NULL,0);

		//< 
		len = enc_len + 2;
		if(size + len > max)
		{
			goto err_ret;
		}

		if(buf)
		{
			len = 0;
			a_xsncpy((schar_t*)(buf + size + len),"<",1);
			len ++;

			if(b_utf8)
				len += utf8_encode(node_name,name_len,(byte_t*)(buf + size + len),enc_len);
			else
				len += gb2312_encode(node_name,name_len,(byte_t*)(buf + size + len),enc_len);

			a_xsncpy((schar_t*)(buf + size + len)," ",1);
			len ++;
		}
		size += len;

		ht = get_dom_node_attr_table(nlk);
		ent = get_hash_first_entity(ht,&he);
		while(ent)
		{
			node_key = get_hash_entity_key_ptr(ent);
			key_len = w_xslen(node_key);

			if(b_utf8)
				enc_len = utf8_encode(node_key,key_len,NULL,0);
			else
				enc_len = gb2312_encode(node_key,key_len,NULL,0);

			//=
			len = enc_len + 1;
			if(size + len > max)
				goto err_ret;

			if(buf)
			{
				len = 0;

				if(b_utf8)
					len += utf8_encode(node_key,key_len,(byte_t*)(buf + size + len),enc_len);
				else
					len += gb2312_encode(node_key,key_len,(byte_t*)(buf + size + len),enc_len);

				a_xsncpy((schar_t*)(buf + size + len),"=",1);
				len ++;
			}
			size += len;

			node_val = get_hash_entity_val_ptr(ent);
			val_len = w_xslen(node_val);

			esc_len = 0;
			for(i=0;i<val_len;i++)
			{
				esc_len += escape_encode_byte_length(node_val[i]);
			}
			esc_len -= val_len;

			if(b_utf8)
				enc_len = utf8_encode(node_val,val_len,NULL,0);
			else
				enc_len = gb2312_encode(node_val,val_len,NULL,0);

			//" "
			len = enc_len + esc_len + 3;
			if(size + len > max)
				goto err_ret;

			if(buf)
			{
				len = 0;
				a_xsncpy((schar_t*)(buf + size + len),"\"",1);
				len ++;

				for(i=0;i<val_len;i++)
				{
					if(escape_encode_byte_length(node_val[i]) > 1)
					{
						len += escape_encode_byte(node_val[i],(byte_t*)(buf + size + len));
					}else
					{
						if(b_utf8)
							len += utf8_encode_byte(node_val[i],(byte_t*)(buf + size + len));
						else
							len += gb2312_encode_byte(node_val[i],(byte_t*)(buf + size + len));
					}
				}

				a_xsncpy((schar_t*)(buf + size + len),"\" ",2);
				len += 2;
			}
			size += len;

			ent = get_hash_next_entity(ht,&he);
		}
		
		//>
		if(size + 1 > max)
		{
			goto err_ret;
		}

		if(buf)
		{
			a_xsncpy((schar_t*)(buf + size),">",1);
		}
		size += 1;

		node_text = get_dom_node_text_ptr(nlk);
		text_len = w_xslen(node_text);

		b_cdata = (get_dom_node_mask_check(nlk,MSK_CDATA))? 1 : 0;
		if(b_cdata)
		{
			len =  (CDATA_HEAD_LEN + CDATA_TAIL_LEN + text_len);
		}else
		{
			esc_len = 0;
			for(i=0;i<text_len;i++)
			{
				esc_len += escape_encode_byte_length(node_text[i]);
			}
			esc_len -= text_len;

			if(b_utf8)
				enc_len = utf8_encode(node_text,text_len,NULL,0);
			else
				enc_len = gb2312_encode(node_text,text_len,NULL,0);

			len = enc_len + esc_len;
		}

		if(size + len > max)
		{
			goto err_ret;
		}

		if(buf)
		{
			len = 0;

			if(b_cdata)
			{
				a_xsncpy((schar_t*)(buf + size + len),A_CDATA_HEAD,CDATA_HEAD_LEN);
				len += CDATA_HEAD_LEN;

				a_xsncpy((schar_t*)(buf + size + len),(schar_t*)node_text,text_len);
				len += text_len;

				a_xsncpy((schar_t*)(buf + size + len),A_CDATA_TAIL,CDATA_TAIL_LEN);
				len += CDATA_TAIL_LEN;
			}else
			{
				for(i=0;i<text_len;i++)
				{
					if(escape_encode_byte_length(node_text[i]) > 1)
					{
						len += escape_encode_byte(node_text[i],(byte_t*)(buf + size + len));
					}else
					{
						if(b_utf8)
							len += utf8_encode_byte(node_text[i],(byte_t*)(buf + size + len));
						else
							len += gb2312_encode_byte(node_text[i],(byte_t*)(buf + size + len));
					}
				}
			}
		}
		size += len;

		if(get_dom_first_child_node(nlk))
		{
			push_stack_item(stack,(void*)nlk);
			nlk = get_dom_first_child_node(nlk);
			continue;
		}

		while(nlk)
		{
			node_name = get_dom_node_name_ptr(nlk);
			name_len = w_xslen(node_name);
			//</node_name>
			if(b_utf8)
				enc_len = utf8_encode(node_name,name_len,NULL,0);
			else
				enc_len = gb2312_encode(node_name,name_len,NULL,0);

			//</>
			len = enc_len + 3;
			if(size + len > max)
			{
				goto err_ret;
			}

			if(buf)
			{
				len = 0;
				a_xsncpy((schar_t*)(buf + size + len),"</",2);
				len += 2;

				if(b_utf8)
					len += utf8_encode(node_name,name_len,(byte_t*)(buf + size + len),enc_len);
				else
					len += gb2312_encode(node_name,name_len,(byte_t*)(buf + size + len),enc_len);

				a_xsncpy((schar_t*)(buf + size + len ),">",1);
				len ++;
			}
			size += len;

			nlk = get_dom_next_sibling_node(nlk);
			if(nlk)
				break;
			nlk = (link_t_ptr)pop_stack_item(stack);
		}
	}
	
err_ret:
	if(ht)
		destroy_stack_table(stack);

	return size;
}
