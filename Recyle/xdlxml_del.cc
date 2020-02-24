/************************************************************
	Easily xdl v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	xdlxml.cc | xml utility implement file

	@devnote ����Ȩ 2003.01 - 2007.12	v3.0
	@devnote ����Ȩ 2008.01 - 2009.12	v3.5
	@devnote ����Ȩ 2012.01 -			v4.0
************************************************************/

#include "xdlxml.h"
#include "xmldoc.h"
#include "domdoc.h"
#include "stacktable.h"
#include "xdlstr.h"
#include "xdlmem.h"
#include "xdlvar.h"
#include "xdlcode.h"

	
//����ת���
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

//����ע���
#define A_CMTOKEN		"--"
#define W_CMTOKEN		L"--"
#define CMTOKEN_LEN		2

//�����η�ǰ׺
#define A_CDATA_HEAD		"[CDATA["
#define W_CDATA_HEAD		L"[CDATA["
#define CDATA_HEAD_LEN		7
//�����η�β׺
#define A_CDATA_TAIL		"]]"
#define W_CDATA_TAIL		L"]]"
#define CDATA_TAIL_LEN		2

//���¶����Զ�����״̬
typedef enum{
	NIL_FAILED	= -1,	//�����ж�
	NIL_SUCCEED = 0,	//������������
	NIL_SKIP = 1,		//�Թ�
	TG_OPEN = 20,		//��Ǵ�
	TG_CLOSE = 21,		//��ǹر�
	NS_OPEN = 25,		//Ԫ�ش�
	NS_INNER_CLOSE = 26,	//Ԫ���ڲ��ж�
	NS_REVER_CLOSE = 27,	//Ԫ��Ԥ�ر�
	NS_CLOSE = 28,		//Ԫ�عر�
	NS_NAME_BEGIN = 30,	//Ԫ������ʼ
	NS_NAME_END = 31,	//Ԫ��������
	NS_KEY_BEGIN = 32,	//Ԫ����������ʼ
	NS_KEY_END = 33,	//Ԫ������������
	NS_ASIGN = 34,		//Ԫ�����Ը�ֵ
	NS_VAL_BEGIN = 35,	//Ԫ�����Կ�ʼ
	NS_VAL_END = 36,	//Ԫ�����Խ���
	NS_TEXT_BEGIN = 37,	//Ԫ���ı���ʼ
	NS_TEXT_END = 38,	//Ԫ���ı�����
	NS_REVER_BEGIN = 39,//�رձ���е�Ԫ������ʼ
	NS_REVER_END = 300,	//�رձ���е�Ԫ������ʼ
	IS_OPEN = 40,		//ָ���
	IS_NAME_BEGIN = 41, //ָ������ʼ
	IS_NAME_END = 42,	//ָ��������
	IS_KEY_BEGIN = 43,	//ָ�����ʼ
	IS_KEY_END = 44,	//ָ�������
	IS_ASIGN = 45,		//��ֵ��
	IS_VAL_BEGIN = 46,	//ָ��ֵ��ʼ
	IS_VAL_END = 47,	//ָ��ֵ����
	IS_CLOSE = 48,		//ָ��ر�
	DS_OPEN = 50,		//������
	DS_SKIP = 51,		//��������
	DS_CLOSE = 52,		//�����ر�
	DS_NAME_BEGIN = 53,		//�������ƿ�ʼ
	DS_NAME_END = 54,		//�������ƽ���
	DS_TEXT_BEGIN = 55,	//�����ı���ʼ
	DS_TEXT_END = 56	//�����ı�����
}MataStatus;

//���¶���ջ����
typedef enum{
	NOP = 0,	//�޲���
	PUSH = 1,	//��ջ
	POP = 2		//��ջ
}StackOpera;

//���¶����дͷ����
typedef enum{
	PAUSE = 0,	//��ͣ�ڵ�ǰ�ַ�
	NEXT = 1,	//������һ�ַ�
	SKIP = 2,	//�Թ��ַ�
	STOP = -1	//ͣ��	
}MataAction;


//�����Զ���
typedef struct _Mata{
	int sa;		//ջ����
	int ma;		//��дͷ����
	int ms;		//��ǰ״̬
}Mata;

//��ת�崮ת��Ϊ������
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

//��������ת��Ϊת�崮
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


//�ж��Ƿ���ת����ַ�
#define _isEscapeChar(ch) ((ch == L'<' || ch == L'>' || ch == L'&' || ch == L'\"' || ch == L'\'' || ch == L'/')? 1 : 0)

//�ж��Ƿ���Ч�������ַ�
#define _IsNamePre(ch) ((ch == L'_' || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z') || ch < 0)? 1: 0)

//�ж��Ƿ�Ϊ��Ч������
#define _IsNameChar(ch) ((ch == L'_' || ch == L'-' || ch == L'.' || ch == L':' || (ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z') || ch < 0)? 1: 0)

//�ж��Ƿ�Ϊ�ո��
#define _IsSpaceChar(ch) ((ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r')? 1 : 0)

//�ж��Ƿ���Ч�ı���
//#define _IsTokenChar(wchar_t ch) ((ch != L'<' && ch != L'>' && ch != L'\"' && ch != L'\'' && ch != L'/')? 1 : 0)
#define _IsTokenChar(ch) ((ch != L'<' && ch != L'>' && ch != L'\"' && ch != L'\'')? 1 : 0)

//�ж��Ƿ���ת����ַ�
#define _IsEscapeChar(ch) ((ch == L'<' || ch == L'>' || ch == L'&' || ch == L'\"' || ch == L'\'' || ch == L'/')? 1 : 0)

#define idle		while(0 == 1)

//����XML�ı�
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

	//�Զ�����ʼ��
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
		//�ж϶�дͷ�Ķ���
		switch(ma.ma)
		{
		case NEXT:
		case SKIP:
			token += bytes;
			//����ת���
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

		//�����Զ���״̬��ת
		switch(ma.ms) 
		{
		case NIL_SKIP: //�����Թ�
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = NEXT;}	//������ʼ�ո�
			else if(ch == L'<'){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //�������
			else if(ch == L'\0') {ma.sa = NOP; ma.ms = NIL_SUCCEED, ma.ma = STOP;}	//�ս��
			else if(token == str + len) {ma.sa = NOP; ma.ms = NIL_SUCCEED, ma.ma = STOP;}	//����ֹ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;}	//��Ч�ַ�
			break;
		case TG_OPEN: //��Ǵ�
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //���Կո�
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT;}	//����ָ���
			else if(ch == L'!'){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //����������
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT;} //׼���رձ��
			else {ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = PAUSE;} //Ԫ�ؿ�ʼ
			break;
		case IS_OPEN: //ָ���
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT;} //���Կո�
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = PAUSE;} //ָ������ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_NAME_BEGIN: //ָ������ʼ
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = NEXT;} //����ָ����
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE;} //ָ��������
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE;} //ָ��������
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_NAME_END: //ָ��������
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = NEXT;} //���Կո�
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT;} //ָ��������
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE;} //ָ����������ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_KEY_BEGIN: //ָ����������ʼ
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = NEXT;} //����������
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE;} //����������
			else if(ch == L'='){ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE;} //����������
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_KEY_END: //ָ������������
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = NEXT;} //���Կո�
			else if(ch == L'='){ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT;} //���Ը�ֵ��
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_ASIGN: //ָ�����Ը�ֵ��ʼ
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT;} //���Կո�
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = SKIP;} //���ɵ����Ը�ֵ,�ӹ�"\""
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_VAL_BEGIN: //ָ������ֵ��ʼ
			if(_IsTokenChar(ch)){ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = NEXT;} //��������ֵ
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = PAUSE;} //���ɵ�����ֵ����
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_VAL_END:  //ָ������ֵ����
			if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT;} //���ɵ�����ֵ����
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT;} //���Կո�
			else if(ch == L'?'){ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT;} //����ж�
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE;} //��������ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case IS_CLOSE: //ָ��ر�
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT;} //���Կո�
			else if(ch == L'>'){ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT;} //��ǹر�
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case DS_OPEN: //������
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //���Կո�
			else if(ch == L'-'){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //����ע�ͷ�
			else if(ch == L'['){ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //������η�
			else if(ch == L'>'){ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE;} //���ɵ������ر�
			else {ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT;} //��������
			break;
		case DS_CLOSE: //��������
			if(ch == L'>'){ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT;} //���ɵ���ǹر�
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_OPEN: //Ԫ�ش�
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = NEXT;} //���Կո�
			else if(_IsNamePre(ch)){ma.sa = PUSH; ma.ms = NS_NAME_BEGIN; ma.ma = PAUSE;} //ָ������ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_NAME_BEGIN: //Ԫ��������ʼ
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = NS_NAME_BEGIN; ma.ma = NEXT;} //Ԫ��������
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE;} //Ԫ��������
			else if(ch == L'>'){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE;}//Ԫ��������
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE;} //Ԫ��������
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;}	//��Ч�ַ�
			break;
		case NS_NAME_END: //Ԫ����������
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = NEXT;} //���Կո�
			else if(ch == L'>'){ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT;} //Ԫ��������
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT;} //Ԫ��������
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE;} //��������ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_KEY_BEGIN: //Ԫ����������ʼ
			if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = NEXT;} //����������
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE;} //����������
			else if(ch == L'='){ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE;} //���Ը�ֵ��
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_KEY_END: //Ԫ������������
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = NEXT;} //���Կո�
			else if(ch == L'='){ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT;} //���Ը�ֵ��
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_ASIGN: //Ԫ�����Ը�ֵ
			if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT;} //���Կո�
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = SKIP;} //���ɵ�����ֵ��ʼ,�ӹ���\"��
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_VAL_BEGIN: //Ԫ������ֵ��ʼ
			if(_IsTokenChar(ch)){ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = NEXT;} //��������ֵ
			else if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = PAUSE;} //���ɵ�����ֵ����
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_VAL_END: //Ԫ������ֵ����
			if(ch == L'\"' || ch == L'\''){ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT;} //����ֵ����
			else if(_IsSpaceChar(ch)){ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT;} //���Կո�
			else if(ch == L'/'){ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT;} //׼���رձ��
			else if(ch == L'>'){ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT;} //����ж�
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE;} //��������ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_INNER_CLOSE: //Ԫ�������ж�
			if(ch == L'<'){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //��ǿ�ʼ
			else{ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = PAUSE;} //Ԫ���ı���ʼ
			break;
		case NS_TEXT_BEGIN: //Ԫ���ı���ʼ
			if(ch == L'<'){ma.sa = NOP; ma.ms = NS_TEXT_END; ma.ma = PAUSE;} //���ɵ�Ԫ���ı�����
			else if(ch == L'['){ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = SKIP;} //��ο�ʼ
			else if(_IsTokenChar(ch)){ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = NEXT;} //����Ԫ���ı�
			else{ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_TEXT_END: //Ԫ���ı�����
			if(ch == L'<'){ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT;} //��ǿ�ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_REVER_CLOSE: //Ԫ��Ԥ�ر�
			if(ch == L'>'){ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE;}//��ǽ���
			else if(_IsNamePre(ch)){ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = PAUSE;} //�رձ�ǵ�Ԫ������ʼ
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_REVER_BEGIN: //Ԫ�عرսṹ�е�Ԫ������ʼ
			if(ch == L'>'){ma.sa = NOP; ma.ms = NS_REVER_END; ma.ma = PAUSE;} //�رձ�ǵ�Ԫ��������
			else if(_IsNameChar(ch)){ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = NEXT;} //�����رձ�ǵ�Ԫ����
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_REVER_END: //Ԫ�عرսṹ�е�Ԫ������ʼ
			if(ch == L'>'){ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE;}//��ǽ���
			else {ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		case NS_CLOSE: //Ԫ�عر�
			{ma.sa = POP; ma.ms = TG_CLOSE; ma.ma = NEXT;} //���ɵ���ǹر�
			break;
		case TG_CLOSE: //��ǹر�
			{ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = PAUSE;}
			break;
		default:
			{ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP;} //��Ч�ַ�
			break;
		}

		//�����Զ���״̬����
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

		//�����Զ�����״̬�����Ƿ��˳�
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

	//�Զ���������״̬
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
