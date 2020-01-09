/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xml parse document

	@module	xdlxml.c | xml parse implement file

	@devnote ����Ȩ 2003.01 - 2007.12	v3.0
	@devnote ����Ȩ 2008.01 - 2009.12	v3.5
	@devnote ����Ȩ 2012.01 - 2015.12	v4.0
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

#include "xdlxml.h"
#include "xmldoc.h"
#include "domdoc.h"
#include "stacktable.h"
#include "hashtable.h"
#include "stringtable.h"
#include "xdlstr.h"
#include "xdlmem.h"
#include "xdlvar.h"
#include "xdlcode.h"
#include "xdlerr.h"
#include "xdlutil.h"
#include "xdlstm.h"

//����ע���
#define A_CMTOKEN			"--"
#define W_CMTOKEN			L"--"
#define CMTOKEN				_T("--")
#define CMTOKEN_LEN			2

//�����η�ǰ׺
#define A_CDATA_HEAD		"[CDATA["
#define W_CDATA_HEAD		L"[CDATA["
#define CDATA_HEAD			_T("[CDATA[")
#define CDATA_HEAD_LEN		7
//�����η�β׺
#define A_CDATA_TAIL		"]]"
#define W_CDATA_TAIL		L"]]"
#define CDATA_TAIL			_T("]]")
#define CDATA_TAIL_LEN		2

#define A_XML_ATTR			"xml"
#define W_XML_ATTR			L"xml"
#define XML_ATTR			_T("xml")
#define XML_ATTR_LEN		3

#define NIL_CHAR			0

#define XMLNS				_T("xmlns")
#define XMLNS_LEN			5

//���¶����Զ�����״̬
typedef enum{
	NIL_FAILED	= -1,		//�����ж�
	NIL_SUCCEED = 0,		//������������
	NIL_SKIP = 1,			//�Թ�
	TG_OPEN = 10,			//��Ǵ�
	TG_CLOSE = 11,			//��ǹر�

	NS_OPEN = 100,			//Ԫ�ش�
	NS_INNER_CLOSE = 110,	//Ԫ���ڲ��ж�
	NS_REVER_CLOSE = 120,	//Ԫ��Ԥ�ر�
	NS_CLOSE = 130,			//Ԫ�عر�
	NS_NAME_BEGIN = 140,	//Ԫ������ʼ
	NS_NAME_END = 150,		//Ԫ��������
	NS_KEY_BEGIN = 160,		//Ԫ����������ʼ
	NS_KEY_END = 170,		//Ԫ������������
	NS_ASIGN = 180,			//Ԫ�����Ը�ֵ
	NS_VAL_QUOT = 181,		//Ԫ����������
	NS_VAL_BEGIN = 190,		//Ԫ�����Կ�ʼ
	NS_VAL_END = 200,		//Ԫ�����Խ���
	NS_TEXT_BEGIN = 210,	//Ԫ���ı���ʼ
	NS_TEXT_END = 220,		//Ԫ���ı�����
	NS_REVER_BEGIN = 250,	//�رձ���е�Ԫ������ʼ
	NS_REVER_END = 260,		//�رձ���е�Ԫ��������

	IS_OPEN = 300,			//ָ���
	IS_NAME_BEGIN = 310,	//ָ������ʼ
	IS_NAME_END = 320,		//ָ��������
	IS_KEY_BEGIN = 330,		//ָ�����ʼ
	IS_KEY_END = 340,		//ָ�������
	IS_ASIGN = 350,			//��ֵ��
	IS_VAL_QUOT = 351,		//����
	IS_VAL_BEGIN = 360,		//ָ��ֵ��ʼ
	IS_VAL_END = 370,		//ָ��ֵ����
	IS_CLOSE = 380,			//ָ��ر�
	DS_OPEN = 400,			//������
	DS_CLOSE = 410,			//�����ر�
	CM_BEGIN = 420,			//ע�Ϳ�ʼ
	CM_TEXT_BEGIN = 430,	//ע���ı���ʼ
	CM_TEXT_END = 440,		//ע���ı�����
	CM_END = 450,			//ע�ͽ���
	CD_BEGIN = 460,			//��ο�ʼ
	CD_TEXT_BEGIN = 470,	//����ı���ʼ
	CD_TEXT_END = 480,		//����ı�����
	CD_END = 490			//��ν���
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
	NEXT = 1,	//��ȡ��ǰ�ַ�
	RETAIN = 2, //������ǰ�ַ�
	SKIP_CDT_HEAD = 10,	//�ӹ�����ַ�
	SKIP_CDT_TAIL = 11,	//�ӹ�����ַ�
	SKIP_CMT_HEAD = 12,	//�ӹ�ע���ַ�
	SKIP_CMT_TAIL = 13,	//�ӹ�ע���ַ�
	STOP = -1	//ͣ��	
}MataAction;


//�����Զ���
typedef struct _MATA{
	int sa;		//ջ����
	int ma;		//��дͷ����
	int ms;		//��ǰ״̬

#ifdef _UNICODE
	ucode_t org[2]; //��һ�ַ�
	ucode_t ch[2]; //��ǰ�ַ�
#else
	schar_t org[4]; //��һ�ַ�
	schar_t ch[4]; //��ǰUTF8�ַ�
#endif

	int bytes;	//ɨ���ֽ���

	int enc;	//�����ʽ
	bool_t esc;	//�Ƿ�ת��
}MATA;


//�ж��Ƿ���Ч�������ַ�
#define _IsNamePre(ch) ((ch == _T('_') || (ch >= _T('A') && ch <= _T('Z')) || (ch >= _T('a') && ch <= _T('z')) || ch < 0)? 1: 0)

//�ж��Ƿ�Ϊ��Ч������
#define _IsNameChar(ch) ((ch == _T('_') || ch == _T('-') || ch == _T('.') || ch == _T(':') || (ch >= _T('0') && ch <= _T('9')) || (ch >= _T('A') && ch <= _T('Z')) || (ch >= _T('a') && ch <= _T('z')) || ch < 0)? 1: 0)

//�ж��Ƿ�Ϊ�ո��
#define _IsSpaceChar(ch) ((ch == _T(' ') || ch == _T('\t'))? 1 : 0)

//�ж��Ƿ�Ϊ�ӹ��ַ�
#define _IsSkipChar(ch) ((ch == _T('\n') || ch == _T('\r'))? 1 : 0)

//�ж��Ƿ���Ч�ı���
//#define _IsTokenChar(tchar_t ch) ((ch != '<' && ch != '>' && ch != '\"' && ch != '\'' && ch != '/')? 1 : 0)
#define _IsTokenChar(ch) ((ch != _T('<') && ch != _T('>') && ch != _T('\"') && ch != _T('\''))? 1 : 0)

#define _IsTextChar(ch) ((ch != _T('<') && ch != _T('>'))? 1 : 0)

//�ж��Ƿ���ת����ַ�
//#define _IsEscapeChar(ch) ((ch == _T('<') || ch == _T('>') || ch == _T('&') || ch == _T('\"') || ch == _T('\'') || ch == _T('/'))? 1 : 0)
#define _IsEscapeChar(ch) ((ch == _T('<') || ch == _T('>') || ch == _T('&') || ch == _T('\"') || ch == _T('\''))? 1 : 0)

#define idle		while(0 == 1)


int __callback _buffer_read_escape(void* p_obj, int max, int pos, int encode, tchar_t* pch)
{
	byte_t* buf = (byte_t*)p_obj + pos;

	if (pos >= max)
	{
		*pch = _T('\0');
		return 0;
	}

	switch (encode)
	{
	case _UTF16_LIT:
		return utf16lit_decode_escape(buf, pch);
	case _UTF16_BIG:
		return utf16big_decode_escape(buf, pch);
	case _GB2312:
		return gb2312_decode_escape(buf, pch);
	case _UTF8:
		return utf8_decode_escape(buf, pch);
	}

	return 0;
}

int __callback _buffer_write_escape(void* p_obj, int max, int pos, int encode, tchar_t ch)
{
	byte_t* buf = (p_obj)? ((byte_t*)p_obj + pos) : NULL;

	switch (encode)
	{
	case _UTF16_LIT:
		return utf16lit_encode_escape(ch, buf, max - pos);
	case _UTF16_BIG:
		return utf16big_encode_escape(ch, buf, max - pos);
	case _GB2312:
		return gb2312_encode_escape(ch, buf, max - pos);
	case _UTF8:
		return utf8_encode_escape(ch, buf, max - pos);
	}

	return 0;
}

int __callback _buffer_read_char(void* p_obj, int max, int pos, int encode, tchar_t* pch)
{
	byte_t* buf = (byte_t*)p_obj + pos;

	if (pos >= max)
	{
		*pch = _T('\0');
		return 0;
	}

	switch (encode)
	{
	case _UTF16_LIT:
		pos = utf16_sequence(*(buf));
#ifdef _UNICODE
		utf16lit_byte_to_ucs(buf, pch);
#else
		utf16lit_byte_to_mbs(buf, pch);
#endif
		break;
	case _UTF16_BIG:
		pos = utf16_sequence(*(buf));
#ifdef _UNICODE
		utf16big_byte_to_ucs(buf, pch);
#else
		utf16big_byte_to_mbs(buf, pch);
#endif
		break;
	case _GB2312:
		pos = gb2312_sequence(*(buf));
#ifdef _UNICODE
		gb2312_byte_to_ucs(buf, pch);
#else
		gb2312_byte_to_mbs(buf, pch);
#endif
		break;
	case _UTF8:
		pos = utf8_sequence(*(buf));
#ifdef _UNICODE
		utf8_byte_to_ucs(buf, pch);
#else
		utf8_byte_to_mbs(buf, pch);
#endif
		break;
	default: 
		pos = 0;
		break;
	}

	return pos;
}

int __callback _buffer_write_char(void* p_obj, int max, int pos, int encode, tchar_t ch)
{
	byte_t* buf = (p_obj)? ((byte_t*)p_obj + pos) : NULL;

	switch (encode)
	{
	case _UTF16_LIT:
#ifdef _UNICODE
		pos = ucs_byte_to_utf16lit(ch, buf);
#else
		pos = mbs_byte_to_utf16lit(ch, buf);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		pos = ucs_byte_to_utf16big(ch, buf);
#else
		pos = mbs_byte_to_utf16big(ch, buf);
#endif
		break;
	case _GB2312:
#ifdef _UNICODE
		pos = ucs_byte_to_gb2312(ch, buf);
#else
		pos = mbs_byte_to_gb2312(ch, buf);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		pos = ucs_byte_to_utf8(ch, buf);
#else
		pos = mbs_byte_to_utf8(ch, buf);
#endif
		break;
	default :
		pos = 0;
		break;
	}

	return pos;
}

int __callback _buffer_write_token(void* p_obj, int max, int pos, int encode, const tchar_t* pch, int len)
{
	byte_t* buf = (p_obj) ? ((byte_t*)p_obj + pos) : NULL;

	if (len < 0)
		len = xslen(pch);

	if (!len)
		return 0;

	switch (encode)
	{
	case _UTF16_LIT:
#ifdef _UNICODE
		pos = ucs_to_utf16lit(pch, len, buf, max - pos);
#else
		pos = mbs_to_utf16lit(pch, len, buf, max - pos);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		pos = ucs_to_utf16big(pch, len, buf, max - pos);
#else
		pos = mbs_to_utf16big(pch, len, buf, max - pos);
#endif
		break;
	case _GB2312:
#ifdef _UNICODE
		pos = ucs_to_gb2312(pch, len, buf, max - pos);
#else
		pos = mbs_to_gb2312(pch, len, buf, max - pos);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		pos = ucs_to_utf8(pch, len, buf, max - pos);
#else
		pos = mbs_to_utf8(pch, len, buf, max - pos);
#endif
		break;
	default:
		pos = 0;
		break;
	}

	return pos;
}

int __callback _stream_read_escape(void* p_obj, int max, int pos, int encode, tchar_t* pch)
{
	xhand_t stm = (xhand_t)p_obj;
	bool_t rt = 0;
	dword_t dw = 0;

	stream_set_encode(stm, encode);

	rt = stream_read_escape(stm, pch, &dw);
	return (rt) ? (int)dw : 0;
}

int __callback _stream_write_escape(void* p_obj, int max, int pos, int encode, tchar_t ch)
{
	xhand_t stm = (xhand_t)p_obj;
	bool_t rt = 0;
	dword_t dw = 0;

	stream_set_encode(stm, encode);

	rt = stream_write_escape(stm, ch, &dw);
	return (rt) ? (int)dw : 0;
}

int __callback _stream_read_char(void* p_obj, int max, int pos, int encode, tchar_t* pch)
{
	xhand_t stm = (xhand_t)p_obj;
	bool_t rt = 0;
	dword_t dw = 0;

	stream_set_encode(stm, encode);

	rt = stream_read_char(stm, pch, &dw);
	return (rt) ? (int)dw : -1;
}

int __callback _stream_write_char(void* p_obj, int max, int pos, int encode, tchar_t ch)
{
	xhand_t stm = (xhand_t)p_obj;
	bool_t rt = 0;
	dword_t dw = 0;

	stream_set_encode(stm, encode);

	rt = stream_write_char(stm, &ch, &dw);
	return (rt) ? (int)dw : -1;
}

int __callback _stream_write_token(void* p_obj, int max, int pos, int encode, const tchar_t* pch, int len)
{
	xhand_t stm = (xhand_t)p_obj;
	bool_t rt = 0;
	dword_t dw = 0;

	stream_set_encode(stm, encode);

	rt = stream_write(stm, pch, len, &dw);
	return (rt) ? (int)dw : -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����XML�ı�
bool_t _parse_xml(link_t_ptr ptr, int encode, BUFOPERATOR* pbo)
{
	link_t_ptr root,nlk = NULL;
	link_t_ptr st = NULL;
	int pos;

	MATA ma = { 0 };

	tchar_t* nsstr;
	int nsat, nslen, vsat, vslen;

	tchar_t errtext[MAX_ERR + 1];

	xhand_t vs_name = varstr_alloc(512, 128);
	xhand_t vs_key = varstr_alloc(512, 128);
	xhand_t vs_val = varstr_alloc(1024, 512);
	xhand_t vs_text = varstr_alloc(1024, 512);

	if (IS_XML_DOC(ptr))
	{
		clear_xml_doc(ptr);
		root = get_xml_dom_node(ptr);
	}
	else if (IS_DOM_DOC(ptr))
	{
		delete_dom_child_node(ptr);
		root = ptr;
	}
	else
	{
		set_last_error(XDL_FAILED, _T("unknown document"), -1);
		return 0;
	}

	//�Զ�����ʼ��
	ma.ma = NEXT;
	ma.sa = NOP;
	ma.ms = NIL_SKIP;
	ma.esc = 1;
	ma.bytes = 0;
	ma.enc = (encode)? encode : _UTF8;

	pos = 0;
	while(ma.ma != STOP)
	{
		//�رպͻָ�ת�忪��
		switch (ma.ms)
		{
		case CM_BEGIN:
			ma.esc = 0;
			break;
		case CM_END:
			ma.esc = 1;
			break;
		case CD_BEGIN:
			ma.esc = 0;
			break;
		case CD_END:
			ma.esc = 1;
			break;
		}

		if (ma.ma == NEXT)
		{
			ma.bytes += pos;

			memset((void*)ma.org, 0, sizeof(ma.org));
			memset((void*)ma.ch, 0, sizeof(ma.ch));

			//��ȡ��һ�ַ�
			pos = (*pbo->pf_read_char)(pbo->obj, pbo->max, ma.bytes, ma.enc, ma.ch);
			if (pos < 0)
			{
				ma.ms = NIL_FAILED;
				break;
			}
			else if (!pos)
			{
				ma.ch[0] = _T('\0');
			}

			//����ת���ַ�
			if (ma.esc && ma.ch[0] == CH_ESC)
			{
				ma.bytes += pos;
				pos = (*pbo->pf_read_escape)(pbo->obj, pbo->max, ma.bytes, ma.enc, ma.org);
				if (!pos)
				{
					xscpy(ma.org, ma.ch);
				}
			}
			else
			{
				xscpy(ma.org, ma.ch);
			}

		}

		if (ma.ma != RETAIN)
		{
			//�����Զ���״̬��ת
			switch (ma.ms)
			{
			case NIL_SKIP: //�����Թ�
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = NEXT; }	//������ʼ�ո�
				else if (ma.ch[0] == _T('<')){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //�������
				else if (ma.ch[0] == _T('\0')) { ma.sa = NOP; ma.ms = NIL_SUCCEED, ma.ma = STOP; }	//�ս��
				//else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; }	//��Ч�ַ�
				else { ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = NEXT; }	//��Ч�ַ�
				break;
			case TG_OPEN: //��Ǵ�
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT; }	//����ָ���
				else if (ma.ch[0] == _T('!')){ ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT; } //����������
				else if (ma.ch[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT; } //׼���رձ��
				else { ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = PAUSE; } //Ԫ�ؿ�ʼ
				break;
			case IS_OPEN: //ָ���
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_OPEN; ma.ma = NEXT; } //���Կո�
				else if (_IsNamePre(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = PAUSE; } //ָ������ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_NAME_BEGIN: //ָ������ʼ
				if (_IsNameChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_NAME_BEGIN; ma.ma = NEXT; } //����ָ����
				else if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE; } //ָ��������
				else if (ma.ch[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = PAUSE; } //ָ��������
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_NAME_END: //ָ��������
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_NAME_END; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT; } //ָ��������
				else if (_IsNamePre(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE; } //ָ����������ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_KEY_BEGIN: //ָ����������ʼ
				if (_IsNameChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = NEXT; } //����������
				else if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE; } //����������
				else if (ma.ch[0] == _T('=')){ ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = PAUSE; } //����������
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_KEY_END: //ָ������������
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_KEY_END; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('=')){ ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT; } //���Ը�ֵ��
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_ASIGN: //ָ�����Ը�ֵ��ʼ
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_ASIGN; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('\"') || ma.ch[0] == _T('\'')){ ma.sa = NOP; ma.ms = IS_VAL_QUOT; ma.ma = NEXT; } //���ɵ����Ը�ֵ,�ӹ�"\""
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_VAL_QUOT: //����
				if (_IsTokenChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = PAUSE; } //��ʼ����ֵ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_VAL_BEGIN: //ָ������ֵ��ʼ
				if (ma.ch[0] == _T('\"') || ma.ch[0] == _T('\'')){ ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = PAUSE; } //���ɵ�����ֵ����
				else if (_IsTokenChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_VAL_BEGIN; ma.ma = NEXT; } //��������ֵ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_VAL_END:  //ָ������ֵ����
				if (ma.ch[0] == _T('\"') || ma.ch[0] == _T('\'')){ ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT; } //���ɵ�����ֵ����
				else if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_VAL_END; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('?')){ ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT; } //����ж�
				else if (_IsNamePre(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_KEY_BEGIN; ma.ma = PAUSE; } //��������ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case IS_CLOSE: //ָ��ر�
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = IS_CLOSE; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT; } //��ǹر�
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case DS_OPEN: //������
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_BEGIN; ma.ma = NEXT; } //����ע�ͷ�
				else if (ma.ch[0] == _T('[')){ ma.sa = NOP; ma.ms = CD_BEGIN; ma.ma = NEXT; } //������η�
				else if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE; } //���ɵ������ر�
				else { ma.sa = NOP; ma.ms = DS_OPEN; ma.ma = NEXT; } //��������
				break;
			case CM_BEGIN: //ע���ı���ʼ
				if (ma.ch[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_TEXT_BEGIN; ma.ma = PAUSE; } //���ɵ�ע���ı���ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case CM_TEXT_BEGIN: //ע���ı���ʼ
				if (ma.ch[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_TEXT_END; ma.ma = NEXT; } //����ע�ͽ���
				else { ma.sa = NOP; ma.ms = CM_TEXT_BEGIN; ma.ma = NEXT; } //����ע���ı�
				break;
			case CM_TEXT_END: //ע���ı�����
				if (ma.ch[0] == _T('-')){ ma.sa = NOP; ma.ms = CM_END; ma.ma = NEXT; } //���ɵ�ע�ͽ���
				else if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = CM_END; ma.ma = PAUSE; } //���ɵ�ע�ͽ���
				else { ma.sa = NOP; ma.ms = CM_TEXT_BEGIN; ma.ma = RETAIN; } //������һ�ַ�Ϊ��Ч�ַ�
				break;
			case CM_END: //ע�ͽ���
				if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE; } //���ɵ������ر�
				else { ma.sa = NOP; ma.ms = CM_END; ma.ma = NEXT; } //����
				break;
			case CD_BEGIN: //��ο�ʼ
				if (ma.ch[0] == _T('C')){ ma.sa = NOP; ma.ms = CD_BEGIN; ma.ma = NEXT; } //�ӹ����ͷ
				else if (ma.ch[0] == _T('D')){ ma.sa = NOP; ma.ms = CD_BEGIN; ma.ma = NEXT; } //�ӹ����ͷ
				else if (ma.ch[0] == _T('A')){ ma.sa = NOP; ma.ms = CD_BEGIN; ma.ma = NEXT; } //�ӹ����ͷ
				else if (ma.ch[0] == _T('T')){ ma.sa = NOP; ma.ms = CD_BEGIN; ma.ma = NEXT; } //�ӹ����ͷ
				else if (ma.ch[0] == _T('A')){ ma.sa = NOP; ma.ms = CD_BEGIN; ma.ma = NEXT; } //�ӹ����ͷ
				else if (ma.ch[0] == _T('[')){ ma.sa = NOP; ma.ms = CD_TEXT_BEGIN; ma.ma = PAUSE; } //���ɵ�����ı���ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case CD_TEXT_BEGIN: //����ı���ʼ
				if (ma.ch[0] == _T(']')){ ma.sa = NOP; ma.ms = CD_TEXT_END; ma.ma = NEXT; } //������ν���
				else { ma.sa = NOP; ma.ms = CD_TEXT_BEGIN; ma.ma = NEXT; } //��������ı�
				break;
			case CD_TEXT_END: //����ı�����
				if (ma.ch[0] == _T(']')){ ma.sa = NOP; ma.ms = CD_TEXT_END; ma.ma = NEXT; } //������ν���
				else if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = CD_END; ma.ma = PAUSE; } //���ɵ���ν���
				else { ma.sa = NOP; ma.ms = CD_TEXT_BEGIN; ma.ma = RETAIN; } //������һ�ַ�Ϊ��Ч�ַ�
				break;
			case CD_END: //��ν���
				if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = DS_CLOSE; ma.ma = PAUSE; } //���ɵ������ر�
				else { ma.sa = NOP; ma.ms = CM_END; ma.ma = NEXT; } //����
				break;
			case DS_CLOSE: //��������
				if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = TG_CLOSE; ma.ma = NEXT; } //���ɵ���ǹر�
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_OPEN: //Ԫ�ش�
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_OPEN; ma.ma = NEXT; } //���Կո�
				else if (_IsNamePre(ma.ch[0])){ ma.sa = PUSH; ma.ms = NS_NAME_BEGIN; ma.ma = PAUSE; } //ָ������ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_NAME_BEGIN: //Ԫ��������ʼ
				if (_IsNameChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_NAME_BEGIN; ma.ma = NEXT; } //Ԫ��������
				else if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE; } //Ԫ��������
				else if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE; }//Ԫ��������
				else if (ma.ch[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = PAUSE; } //Ԫ��������
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; }	//��Ч�ַ�
				break;
			case NS_NAME_END: //Ԫ����������
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_NAME_END; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT; } //Ԫ��������
				else if (ma.ch[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT; } //Ԫ��������
				else if (_IsNamePre(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE; } //��������ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_KEY_BEGIN: //Ԫ����������ʼ
				if (_IsNameChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = NEXT; } //����������
				else if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE; } //����������
				else if (ma.ch[0] == _T('=')){ ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = PAUSE; } //���Ը�ֵ��
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_KEY_END: //Ԫ������������
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_KEY_END; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('=')){ ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT; } //���Ը�ֵ��
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_ASIGN: //Ԫ�����Ը�ֵ
				if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_ASIGN; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('\"') || ma.ch[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_QUOT; ma.ma = NEXT; } //���ɵ�����ֵ��ʼ,�ӹ���\"��
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_VAL_QUOT:
				if (_IsTokenChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = PAUSE; } //��ʼ����ֵ
				else if (ma.ch[0] == _T('\"') || ma.ch[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = PAUSE; } //���ɵ�����ֵ��ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_VAL_BEGIN: //Ԫ������ֵ��ʼ
				if (_IsTokenChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_VAL_BEGIN; ma.ma = NEXT; } //��������ֵ
				else if (ma.ch[0] == _T('\"') || ma.ch[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = PAUSE; } //���ɵ�����ֵ����
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_VAL_END: //Ԫ������ֵ����
				if (ma.ch[0] == _T('\"') || ma.ch[0] == _T('\'')){ ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT; } //����ֵ����
				else if (_IsSpaceChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_VAL_END; ma.ma = NEXT; } //���Կո�
				else if (ma.ch[0] == _T('/')){ ma.sa = NOP; ma.ms = NS_REVER_CLOSE; ma.ma = NEXT; } //׼���رձ��
				else if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_INNER_CLOSE; ma.ma = NEXT; } //����ж�
				else if (_IsNamePre(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_KEY_BEGIN; ma.ma = PAUSE; } //��������ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_INNER_CLOSE: //Ԫ�������ж�
				if (ma.ch[0] == _T('<')){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //��ǿ�ʼ
				else{ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = PAUSE; } //Ԫ���ı���ʼ
				break;
			case NS_TEXT_BEGIN: //Ԫ���ı���ʼ
				//if (ma.ch[0] == _T('['){ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = SKIP_CDT_HEAD; } //��ο�ʼ
				//if (ma.ch[0] == _T(']'){ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = SKIP_CDT_TAIL; } //��ν���
				if (ma.ch[0] == _T('<')){ ma.sa = NOP; ma.ms = NS_TEXT_END; ma.ma = PAUSE; } //���ɵ�Ԫ���ı�����
				else if (_IsTextChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_TEXT_BEGIN; ma.ma = NEXT; } //����Ԫ���ı�
				else{ ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_TEXT_END: //Ԫ���ı�����
				if (ma.ch[0] == _T('<')){ ma.sa = NOP; ma.ms = TG_OPEN; ma.ma = NEXT; } //��ǿ�ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_REVER_CLOSE: //Ԫ��Ԥ�ر�
				if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE; }//��ǽ���
				else if (_IsNamePre(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = PAUSE; } //�رձ�ǵ�Ԫ������ʼ
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_REVER_BEGIN: //Ԫ�عرսṹ�е�Ԫ������ʼ
				if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_REVER_END; ma.ma = PAUSE; } //�رձ�ǵ�Ԫ��������
				else if (_IsNameChar(ma.ch[0])){ ma.sa = NOP; ma.ms = NS_REVER_BEGIN; ma.ma = NEXT; } //�����رձ�ǵ�Ԫ����
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_REVER_END: //Ԫ�عرսṹ�е�Ԫ������ʼ
				if (ma.ch[0] == _T('>')){ ma.sa = NOP; ma.ms = NS_CLOSE; ma.ma = PAUSE; }//��ǽ���
				else { ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			case NS_CLOSE: //Ԫ�عر�
				{ma.sa = POP; ma.ms = TG_CLOSE; ma.ma = NEXT; } //���ɵ���ǹر�
				break;
			case TG_CLOSE: //��ǹر�
				{ma.sa = NOP; ma.ms = NIL_SKIP; ma.ma = PAUSE; }
				break;
			default:
				{ma.sa = NOP; ma.ms = NIL_FAILED; ma.ma = STOP; } //��Ч�ַ�
				break;
			}
		}
		else
		{
			ma.ma = NEXT;
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
				varstr_empty(vs_name);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_name, ma.org, -1);
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
				varstr_empty(vs_key);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_key, ma.org, -1);
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
				varstr_empty(vs_val);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_val, ma.org, -1);
				break;
			}
		}else if(ma.ms == IS_VAL_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if(xsicmp(DOC_XML,varstr_ptr(vs_name)) == 0)
				{
					if(xsicmp(XML_ATTR_ENCODING,varstr_ptr(vs_key)) == 0)
					{
						if (xsicmp(XML_ATTR_ENCODING_UTF8, varstr_ptr(vs_val)) == 0)
						{
							ma.enc = _UTF8;
							set_xml_encode(ptr, ma.enc);
						}
						else if (xsicmp(XML_ATTR_ENCODING_GB2312, varstr_ptr(vs_val)) == 0)
						{
							ma.enc = _GB2312;
							set_xml_encode(ptr, ma.enc);
						}
					}

					if (xsicmp(XML_ATTR_VERSION, varstr_ptr(vs_key)) == 0)
					{
						set_xml_version(ptr, varstr_ptr(vs_val), -1);
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
				varstr_empty(vs_name);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_name, ma.org, -1);
				break;
			}
		}else if(ma.ms == NS_NAME_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				nsstr = (tchar_t*)varstr_ptr(vs_name);
				if (split_xmlns(nsstr, &nsat, &nslen, &vsat, &vslen))
				{
					set_dom_node_ns(nlk, nsstr + nsat, nslen);
					set_dom_node_name(nlk, nsstr + vsat, vslen);
				}
				else
				{
					set_dom_node_name(nlk, varstr_ptr(vs_name), -1);
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
				varstr_empty(vs_key);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_key, ma.org, -1);
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
				varstr_empty(vs_val);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_val, ma.org, -1);
				break;
			}
		}else if(ma.ms == NS_VAL_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				if (xsnicmp(XMLNS, varstr_ptr(vs_key), XMLNS_LEN) == 0)
				{
					set_dom_node_xmlns(nlk, varstr_ptr(vs_key), -1, varstr_ptr(vs_val), -1);
				}
				else
				{
					set_dom_node_attr(nlk, varstr_ptr(vs_key), -1, varstr_ptr(vs_val), -1);
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
				varstr_empty(vs_text);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_text, ma.org, -1);
				break;
			}
		}else if(ma.ms == NS_TEXT_END)
		{
			switch(ma.ma)
			{
			case PAUSE:
				set_dom_node_text(nlk, varstr_ptr(vs_text),-1);
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
				varstr_empty(vs_text);
				break;
			case NEXT:
				if (!_IsSkipChar(ma.org[0]))
					varstr_cat(vs_text, ma.org, -1);
				break;
			}
		}
		else if (ma.ms == CD_END)
		{
			switch (ma.ma)
			{
			case PAUSE:
				if (nlk)
				{
					set_dom_node_text(nlk, varstr_ptr(vs_text), -1);
					set_dom_node_mask_check(nlk, MSK_CDATA, 1);
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
				if(!nlk)
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
			if (!st)
				st = create_stack_table();

			push_stack_node(st,(void*)nlk);
			if(!nlk)
				nlk = root;
			else
				nlk = insert_dom_node(nlk,LINK_LAST);
		}else if(ma.sa == POP)
		{
			nlk = (st)? (link_t_ptr)pop_stack_node(st) : NULL;
		}

		//�����Զ�����״̬�����Ƿ��˳�
		if(ma.ms == NIL_FAILED || ma.ms == NIL_SUCCEED)
			break;
	}

	if(nlk != NULL)
	{
		ma.ms = NIL_FAILED;
	}

	if (st)
	{
		destroy_stack_table(st);
	}

	varstr_free(vs_name);
	varstr_free(vs_key);
	varstr_free(vs_val);
	varstr_free(vs_text);

	if (ma.ms != NIL_SUCCEED)
	{
		xsprintf(errtext, _T("xml parse error at bytes: %d\n"), ma.bytes);

		set_last_error(XDL_FAILED, errtext, -1);
	}

	//�Զ���������״̬
	return (ma.ms == NIL_SUCCEED)? 1 : 0;
}

bool_t parse_xml_doc_from_buffer(link_t_ptr ptr, const byte_t* str, int len)
{
	BUFOPERATOR bo = { 0 };

	int encode, bytes;

	if (!str || len <= 0)
		return 0;

	//utf-8
	if (str[0] == 0xEF && str[1] == 0xBB && str[2] == 0xBF)
	{
		bytes = 3;
		encode = _UTF8;
	}//LIT BOM
	else if (str[0] == 0xFF && str[1] == 0xFE)
	{
		bytes = 2;
		encode = _UTF16_LIT;
	}//BIG BOM
	else if (str[0] == 0xFE && str[1] == 0xFF)
	{
		bytes = 2;
		encode = _UTF16_BIG;
	}
	else
	{
		bytes = 0;
		encode = _UTF8;
	}

	bo.obj = (void*)str;
	bo.max = len;
	bo.pf_read_char = _buffer_read_char;
	bo.pf_read_escape = _buffer_read_escape;

	return _parse_xml(ptr, encode, &bo);
}

bool_t parse_xml_doc_from_stream(link_t_ptr ptr, xhand_t xs)
{
	BUFOPERATOR bo = { 0 };

	bo.obj = (void*)xs;
	bo.max = 0;
	bo.pf_read_char = _stream_read_char;
	bo.pf_read_escape = _stream_read_escape;

	return _parse_xml(ptr, stream_get_encode(xs), &bo);
}

bool_t parse_dom_doc_from_buffer(link_t_ptr ptr, const byte_t* str, int len, int encode)
{
	BUFOPERATOR bo = { 0 };

	bo.obj = (void*)str;
	bo.max = len;
	bo.pf_read_char = _buffer_read_char;
	bo.pf_read_escape = _buffer_read_escape;

	return _parse_xml(ptr, encode, &bo);
}

bool_t parse_doc_doc_from_stream(link_t_ptr ptr, xhand_t xs)
{
	BUFOPERATOR bo = { 0 };

	bo.obj = (void*)xs;
	bo.max = 0;
	bo.pf_read_char = _stream_read_char;
	bo.pf_read_escape = _stream_read_escape;

	return _parse_xml(ptr, stream_get_encode(xs), &bo);
}
/***************************************************************************************************************************/
int __callback	_on_format_node_attr(int encode, const tchar_t* key, const tchar_t* val, void* pv)
{
	BUFOPERATOR* pb = (BUFOPERATOR*)pv;
	int pos = 0;

	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T(' '));
	if (pos < 0)
		return 1;
	pb->pos += pos;

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, key, -1);
	if (pos < 0)
		return 1;
	pb->pos += pos;

	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('='));
	if (pos < 0)
		return 1;
	pb->pos += pos;

	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('\"'));
	if (pos < 0)
		return 1;
	pb->pos += pos;

	while (val && *val)
	{
		if (_IsEscapeChar(*val))
		{
			pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('&'));
			if (pos < 0)
				return 1;
			pb->pos += pos;

			pos = (*pb->pf_write_escape)(pb->obj, pb->max, pb->pos, encode, *(val++));
			if (pos < 0)
				return 1;
			pb->pos += pos;
		}
		else
		{
			pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, *(val++));
			if (pos < 0)
				return 1;
			pb->pos += pos;
		}
	}

	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('\"'));
	if (pos < 0)
		return 1;
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_node_begin(int encode, const tchar_t* ns, const tchar_t* nn, void* pv)
{
	BUFOPERATOR* pb = (BUFOPERATOR*)pv;
	int pos = 0;
	bool_t b_ns = 0;

	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('<'));
	if (pos < 0)
		return 1;
	pb->pos += pos;

	if (!is_null(ns))
		b_ns = 1;

	if (b_ns)
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, ns, -1);
		if (pos < 0)
			return 1;
		pb->pos += pos;

		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T(':'));
		if (pos < 0)
			return 1;
		pb->pos += pos;
	}

	pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, nn, -1);
	if (pos < 0)
		return 1;
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_node_close(int encode, void* pv)
{
	BUFOPERATOR* pb = (BUFOPERATOR*)pv;
	int pos = 0;

	pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('>'));
	if (pos < 0)
		return 1;
	pb->pos += pos;

	return 0;
}

int __callback	_on_format_node_text(int encode, bool_t b_cdata, const tchar_t* text, int len, void* pv)
{
	BUFOPERATOR* pb = (BUFOPERATOR*)pv;
	int i,pos = 0;

	if (len < 0)
		len = xslen(text);

	if (!len)
		return 0;

	if (b_cdata)
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, _T("<!"), -1);
		if (pos < 0)
			return 1;
		pb->pos += pos;

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, CDATA_HEAD, -1);
		if (pos < 0)
			return 1;
		pb->pos += pos;
	}

	if (b_cdata)
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, text, len);
		if (pos < 0)
			return 1;
		pb->pos += pos;

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, CDATA_TAIL, -1);
		if (pos < 0)
			return 1;
		pb->pos += pos;

		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('>'));
		if (pos < 0)
			return 1;
		pb->pos += pos;
	}
	else
	{
		for (i = 0; i < len;i++)
		{
			if (_IsEscapeChar(text[i]))
			{
				pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('&'));
				if (pos < 0)
					return 1;
				pb->pos += pos;

				pos = (*pb->pf_write_escape)(pb->obj, pb->max, pb->pos, encode, text[i]);
				if (pos < 0)
					return 1;
				pb->pos += pos;
			}
			else
			{
				pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, text[i]);
				if (pos < 0)
					return 1;
				pb->pos += pos;
			}
		}
	}

	return 0;
}

int __callback	_on_format_node_end(int encode, bool_t b_close, const tchar_t* ns, const tchar_t* nn, void* pv)
{
	BUFOPERATOR* pb = (BUFOPERATOR*)pv;
	int pos = 0;
	bool_t b_ns = 0;

	if (b_close)
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, _T("</"), -1);
		if (pos < 0)
			return 1;
		pb->pos += pos;

		if (!is_null(ns))
			b_ns = 1;

		if (b_ns)
		{
			pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, ns, -1);
			if (pos < 0)
				return 1;
			pb->pos += pos;

			pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T(':'));
			if (pos < 0)
				return 1;
			pb->pos += pos;
		}

		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, nn, -1);
		if (pos < 0)
			return 1;
		pb->pos += pos;

		pos = (*pb->pf_write_char)(pb->obj, pb->max, pb->pos, encode, _T('>'));
		if (pos < 0)
			return 1;
		pb->pos += pos;
	}
	else
	{
		pos = (*pb->pf_write_token)(pb->obj, pb->max, pb->pos, encode, _T(" />"), -1);
		if (pos < 0)
			return 1;
		pb->pos += pos;
	}

	return 0;
}

int _format_node_attr_to_buffer(int encode, const tchar_t* key, int klen, const tchar_t* val, int vlen, byte_t* buf, int max)
{
	int i, esc_len, len = 0;
	tchar_t esc_token[MAX_ESC + 1];

	klen = (klen < 0) ? xslen(key) : klen;
	vlen = (vlen < 0) ? xslen(val) : vlen;

	//key
	if (encode == _UTF8)
#ifdef _UNICODE
		len += ucs_to_utf8(key, klen, ((buf)? buf + len : NULL), max - len);
#else
		len += mbs_to_utf8(key, klen, ((buf)? buf + len : NULL), max - len);
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		len += ucs_to_gb2312(key, klen, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_gb2312(key, klen, ((buf)? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		len += ucs_to_utf16lit(key, klen, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16lit(key, klen, ((buf)? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		len += ucs_to_utf16big(key, klen, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16big(key, klen, ((buf)? buf + len : NULL), max - len);
#endif
	
	if (len >= max)
		return len;

	//=
	if (encode == _UTF8)
#ifdef _UNICODE
		len += ucs_to_utf8(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf8(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		len += ucs_to_gb2312(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_gb2312(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		len += ucs_to_utf16lit(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16lit(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		len += ucs_to_utf16big(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16big(_T("=\""), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	
	if (len >= max)
		return len;

	for (i = 0; i < vlen;)
	{
#ifdef _UNICODE
		esc_len = ucs_encode_escape(val[i], esc_token + 1, MAX_ESC);
#else
		esc_len = mbs_encode_escape(val[i], esc_token + 1, MAX_ESC);
#endif
		if (!esc_len)
		{
#ifdef _UNICODE
			esc_len = 1;
			xsncpy(esc_token, val + i, esc_len);
			i += esc_len;
#else
			esc_len = mbs_sequence(val[i]);
			xsncpy(esc_token, val + i, esc_len);
			i += esc_len;
#endif
			
		}
		else
		{
			esc_token[0] = CH_ESC;
			esc_len++;
			i++;
		}

		if (encode == _UTF8)
#ifdef _UNICODE
			len += ucs_to_utf8(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#else
			len += mbs_to_utf8(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			len += ucs_to_gb2312(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#else
			len += mbs_to_gb2312(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			len += ucs_to_utf16lit(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#else
			len += mbs_to_utf16lit(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			len += ucs_to_utf16big(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#else
			len += mbs_to_utf16big(esc_token, esc_len, ((buf) ? buf + len : NULL), max - len);
#endif
		
		if (len >= max)
			return len;
	}

	if (encode == _UTF8)
#ifdef _UNICODE
		len += ucs_to_utf8(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf8(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		len += ucs_to_gb2312(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_gb2312(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		len += ucs_to_utf16lit(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16lit(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		len += ucs_to_utf16big(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16big(_T("\" "), 2, ((buf) ? buf + len : NULL), max - len);
#endif
	
	if (len >= max)
		return len;

	return len;
}

bool_t _format_node_attr_to_stream(int encode, const tchar_t* key, int klen, const tchar_t* val, int vlen, xhand_t xs)
{
	int i, esc_len;
	tchar_t esc_token[MAX_ESC + 1];

	klen = (klen < 0) ? xslen(key) : klen;
	vlen = (vlen < 0) ? xslen(val) : vlen;

	stream_set_encode(xs, encode);

	//key
	if (!stream_write(xs, key, klen, NULL))
		return 0;

	//=
	if (!stream_write(xs, _T("=\""), 2, NULL))
		return 0;

	for (i = 0; i < vlen;)
	{
#ifdef _UNICODE
		esc_len = ucs_encode_escape(val[i], esc_token + 1, MAX_ESC);
#else
		esc_len = mbs_encode_escape(val[i], esc_token + 1, MAX_ESC);
#endif
		if (!esc_len)
		{
#ifdef _UNICODE
			esc_len = 1;
			xsncpy(esc_token, val + i, esc_len);
			i += esc_len;
#else
			esc_len = mbs_sequence(val[i]);
			xsncpy(esc_token, val + i, esc_len);
			i += esc_len;
#endif

		}
		else
		{
			esc_token[0] = CH_ESC;
			esc_len++;
			i++;
		}

		stream_write(xs, esc_token, esc_len, NULL);
	}

	if (!stream_write(xs, _T("\" "), 2, NULL))
		return 0;

	return 1;
}


int _format_node_name_to_buffer(int encode, const tchar_t* ns, int klen, const tchar_t* nm, int vlen, byte_t* buf, int max)
{
	int len = 0;

	//ns
	if (!is_null(ns))
	{
		if (encode == _UTF8)
#ifdef _UNICODE
			len += ucs_to_utf8(ns, klen, ((buf)? buf + len : NULL), max - len);
#else
			len += mbs_to_utf8(ns, klen, ((buf) ? buf + len : NULL), max - len);
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			len += ucs_to_gb2312(ns, klen, ((buf) ? buf + len : NULL), max - len);
#else
			len += mbs_to_gb2312(ns, klen, ((buf) ? buf + len : NULL), max - len);
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			len += ucs_to_utf16lit(ns, klen, ((buf) ? buf + len : NULL), max - len);
#else
			len += mbs_to_utf16lit(ns, klen, ((buf) ? buf + len : NULL), max - len);
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			len += ucs_to_utf16big(ns, klen, ((buf) ? buf + len : NULL), max - len);
#else
			len += mbs_to_utf16big(ns, klen, ((buf) ? buf + len : NULL), max - len);
#endif

		if (len >= max)
			return len;
	}

	//:
	if (!is_null(ns))
	{
		if (encode == _UTF8)
#ifdef _UNICODE
			len += ucs_byte_to_utf8(_T(':'), ((buf) ? buf + len : NULL));
#else
			len += mbs_byte_to_utf8(_T(':'), ((buf) ? buf + len : NULL));
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			len += ucs_byte_to_gb2312(_T(':'), ((buf) ? buf + len : NULL));
#else
			len += mbs_byte_to_gb2312(_T(':'), ((buf) ? buf + len : NULL));
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			len += ucs_byte_to_utf16lit(_T(':'), ((buf) ? buf + len : NULL));
#else
			len += mbs_byte_to_utf16lit(_T(':'), ((buf) ? buf + len : NULL));
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			len += ucs_byte_to_utf16big(_T(':'), ((buf) ? buf + len : NULL));
#else
			len += mbs_byte_to_utf16big(_T(':'), ((buf) ? buf + len : NULL));
#endif
		
		if (len >= max)
			return len;
	}

	//name
	if (encode == _UTF8)
#ifdef _UNICODE
		len += ucs_to_utf8(nm, vlen, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf8(nm, vlen, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		len += ucs_to_gb2312(nm, vlen, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_gb2312(nm, vlen, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		len += ucs_to_utf16lit(nm, vlen, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16lit(nm, vlen, ((buf) ? buf + len : NULL), max - len);
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		len += ucs_to_utf16big(nm, vlen, ((buf) ? buf + len : NULL), max - len);
#else
		len += mbs_to_utf16big(nm, vlen, ((buf)? buf + len : NULL), max - len);
#endif
	
	if (len >= max)
		return len;

	return len;
}

bool_t _format_node_name_to_stream(int encode, const tchar_t* ns, int klen, const tchar_t* nm, int vlen, xhand_t xs)
{

	stream_set_encode(xs, encode);
	//ns
	if (!is_null(ns))
	{
		if (!stream_write(xs, ns, klen, NULL))
			return 0;
	}

	//:
	if (!is_null(ns))
	{
		if (!stream_write(xs, _T(":"), 1, NULL))
			return 0;
	}

	//name
	if(!stream_write(xs,nm,vlen, NULL))
		return 0;

	return 1;
}

int _format_xml_attr_to_buffer(link_t_ptr xml, byte_t* buf, int max)
{
	int total = 0;
	int encode;
	tchar_t encoding[RES_LEN], version[RES_LEN];

	get_xml_encoding(xml, encoding, RES_LEN);
	get_xml_version(xml, version, RES_LEN);

	encode = get_xml_encode(xml);

	if (encode == _UTF8)
#ifdef _UNICODE
		total += ucs_to_utf8(_T("<?xml "), -1, ((buf)? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(_T("<?xml "), -1, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		total += ucs_to_gb2312(_T("<?xml "), -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_gb2312(_T("<?xml "), -1, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		total += ucs_to_utf16lit(_T("<?xml "), -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf16lit(_T("<?xml "), -1, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		total += ucs_to_utf16big(_T("<?xml "), -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf16big(_T("<?xml "), -1, ((buf) ? buf + total : NULL), max - total);
#endif
	
	if (total >= max)
		return total;

	//version
	total += _format_node_attr_to_buffer(encode, XML_ATTR_VERSION, -1, version, -1, ((buf) ? buf + total : NULL), max - total);
	if (total >= max)
		return total;

	//encoding
	total += _format_node_attr_to_buffer(encode, XML_ATTR_ENCODING, -1, encoding, -1, ((buf) ? buf + total : NULL), max - total);
	if (total >= max)
		return total;

	//xml tail 
	if (encode == _UTF8)
#ifdef _UNICODE
		total += ucs_to_utf8(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		total += ucs_to_gb2312(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_gb2312(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		total += ucs_to_utf16lit(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf16lit(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		total += ucs_to_utf16big(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf16big(_T("?>"), -1, ((buf) ? buf + total : NULL), max - total);
#endif

	if (total >= max)
		return total;

	return total;
}

bool_t _format_xml_attr_to_stream(link_t_ptr xml, xhand_t xs)
{
	int encode;
	tchar_t encoding[RES_LEN], version[RES_LEN];

	get_xml_encoding(xml, encoding, RES_LEN);
	get_xml_version(xml, version, RES_LEN);

	encode = get_xml_encode(xml);

	stream_set_encode(xs, encode);

	if (!stream_write(xs, _T("<?xml "), -1, NULL))
		return 0;

	//version
	if (!_format_node_attr_to_stream(encode, XML_ATTR_VERSION, -1, version, -1, xs))
		return 0;

	//encoding
	if (!_format_node_attr_to_stream(encode, XML_ATTR_ENCODING, -1, encoding, -1, xs))
		return 0;

	//xml tail 
	if (!stream_write(xs, _T("?>"), -1, NULL))
		return 0;

	return 1;
}

int _format_dom_node_to_buffer(link_t_ptr nlk, int encode, byte_t* buf, int max)
{
	int total = 0;
	const tchar_t *node_ns, *node_name, *node_key, *node_val;
	int ns_len, name_len;

	link_t_ptr ent, ht;
	link_t_ptr slk, st;
	hash_enum_t he = { 0 };
	int attr_count = 0;

	//xmlns table
	st = get_dom_node_name_table(nlk);
	//attr table
	ht = get_dom_node_attr_table(nlk);

	attr_count = ((st) ? get_string_entity_count(st) : 0) + ((ht) ? get_hash_entity_count(ht) : 0);

	//<
	if (encode == _UTF8)
#ifdef _UNICODE
		total += ucs_byte_to_utf8(_T('<'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf8(_T('<'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		total += ucs_byte_to_gb2312(_T('<'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_gb2312(_T('<'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		total += ucs_byte_to_utf16lit(_T('<'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf16lit(_T('<'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		total += ucs_byte_to_utf16big(_T('<'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf16big(_T('<'), ((buf) ? buf + total : NULL));
#endif

	if (total >= max)
		return total;

	node_name = get_dom_node_name_ptr(nlk);
	name_len = xslen(node_name);

	node_ns = get_dom_node_ns_ptr(nlk);
	ns_len = xslen(node_ns);

	//name
	total += _format_node_name_to_buffer(encode, node_ns, ns_len, node_name, name_len, ((buf) ? buf + total : NULL), max - total);

	if (attr_count)
	{
		//_
		if (encode == _UTF8)
#ifdef _UNICODE
			total += ucs_byte_to_utf8(_T(' '), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf8(_T(' '), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			total += ucs_byte_to_gb2312(_T(' '), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_gb2312(_T(' '), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			total += ucs_byte_to_utf16lit(_T(' '), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16lit(_T(' '), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			total += ucs_byte_to_utf16big(_T(' '), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16big(_T(' '), ((buf) ? buf + total : NULL));
#endif

		if (total >= max)
			return total;
	}

	//xmlns table
	slk = (st) ? get_string_next_entity(st, LINK_FIRST) : NULL;
	while (slk)
	{
		node_key = get_string_entity_key_ptr(slk);
		node_val = get_string_entity_val_ptr(slk);

		total += _format_node_attr_to_buffer(encode, node_key, -1, node_val, -1, ((buf) ? buf + total : NULL), max - total);

		if (total >= max)
			return total;

		slk = get_string_next_entity(st, slk);
	}

	if (total >= max)
		return total;

	//attr table
	ent = (ht) ? get_hash_first_entity(ht, &he) : NULL;
	while (ent)
	{
		node_key = get_hash_entity_key_ptr(ent);
		node_val = get_hash_entity_val_ptr(ent);

		total += _format_node_attr_to_buffer(encode, node_key, -1, node_val, -1, ((buf) ? buf + total : NULL), max - total);

		if (total >= max)
			return total;

		ent = get_hash_next_entity(ht, &he);
	}

	if (total >= max)
		return total;

	//>
	if (encode == _UTF8)
#ifdef _UNICODE
		total += ucs_byte_to_utf8(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf8(_T('>'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		total += ucs_byte_to_gb2312(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_gb2312(_T('>'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		total += ucs_byte_to_utf16lit(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_gb2312(_T('>'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		total += ucs_byte_to_utf16big(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf16big(_T('>'), ((buf) ? buf + total : NULL));
#endif

	if (total >= max)
		return total;

	return total;
}

bool_t _format_dom_node_to_stream(link_t_ptr nlk, int encode, xhand_t xs)
{
	const tchar_t *node_ns, *node_name, *node_key, *node_val;
	int ns_len, name_len;

	link_t_ptr ent, ht;
	link_t_ptr slk, st;
	hash_enum_t he = { 0 };
	int attr_count = 0;

	stream_set_encode(xs, encode);

	//xmlns table
	st = get_dom_node_name_table(nlk);
	//attr table
	ht = get_dom_node_attr_table(nlk);

	attr_count = ((st) ? get_string_entity_count(st) : 0) + ((ht) ? get_hash_entity_count(ht) : 0);

	//<
	if (!stream_write(xs, _T("<"), 1, NULL))
		return 0;

	node_name = get_dom_node_name_ptr(nlk);
	name_len = xslen(node_name);

	node_ns = get_dom_node_ns_ptr(nlk);
	ns_len = xslen(node_ns);

	//name
	if (!_format_node_name_to_stream(encode, node_ns, ns_len, node_name, name_len, xs))
		return 0;

	if (attr_count > 0)
	{
		//_
		if (!stream_write(xs, _T(" "), 1, NULL))
			return 0;
	}

	//xmlns table
	slk = (st) ? get_string_next_entity(st, LINK_FIRST) : NULL;
	while (slk)
	{
		node_key = get_string_entity_key_ptr(slk);
		node_val = get_string_entity_val_ptr(slk);

		if (!_format_node_attr_to_stream(encode, node_key, -1, node_val, -1, xs))
			return 0;

		slk = get_string_next_entity(st, slk);
	}

	//attr table
	ent = (ht) ? get_hash_first_entity(ht, &he) : NULL;
	while (ent)
	{
		node_key = get_hash_entity_key_ptr(ent);
		node_val = get_hash_entity_val_ptr(ent);

		if (!_format_node_attr_to_stream(encode, node_key, -1, node_val, -1, xs))
			return 0;

		ent = get_hash_next_entity(ht, &he);
	}

	//>
	if (!stream_write(xs, _T(">"), 1, NULL))
		return 0;

	return 1;
}

int _format_dom_node_text_to_buffer(link_t_ptr nlk, int encode, byte_t* buf, int max)
{
	int total = 0;
	bool_t b_cdata;
	const tchar_t *node_text;
	int text_len;
	int esc_len, i;
	tchar_t esc_token[MAX_ESC + 1];

	b_cdata = (get_dom_node_mask_check(nlk, MSK_CDATA)) ? 1 : 0;

	if (b_cdata)
	{
		node_text = get_dom_node_text_ptr(nlk);
		text_len = xslen(node_text);
	}
	else
	{
		node_text = get_dom_node_text_ptr(nlk);
		text_len = xslen(node_text);
	}

	if (b_cdata)
	{
		if (encode == _UTF8)
#ifdef _UNICODE
			total += ucs_byte_to_utf8(_T('<'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf8(_T('<'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			total += ucs_byte_to_gb2312(_T('<'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_gb2312(_T('<'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			total += ucs_byte_to_utf16lit(_T('<'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16lit(_T('<'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			total += ucs_byte_to_utf16big(_T('<'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16big(_T('<'), ((buf) ? buf + total : NULL));
#endif

		if (total >= max)
			return total;

		if (encode == _UTF8)
#ifdef _UNICODE
			total += ucs_byte_to_utf8(_T('!'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf8(_T('!'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			total += ucs_byte_to_gb2312(_T('!'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_gb2312(_T('!'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			total += ucs_byte_to_utf16lit(_T('!'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16lit(_T('!'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			total += ucs_byte_to_utf16big(_T('!'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16big(_T('!'), ((buf) ? buf + total : NULL));
#endif

		if (total >= max)
			return total;

		if (encode == _UTF8)
#ifdef _UNICODE
			total += ucs_to_utf8(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf8(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			total += ucs_to_gb2312(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_gb2312(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			total += ucs_to_utf16lit(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf16lit(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			total += ucs_to_utf16big(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf16big(CDATA_HEAD, CDATA_HEAD_LEN, ((buf) ? buf + total : NULL), max - total);
#endif

		if (total >= max)
			return total;

	}

	for (i = 0; i < text_len;)
	{
		if (b_cdata)
		{
#ifdef _UNICODE
			esc_len = 1;
			xsncpy(esc_token, node_text + i, esc_len);
			i += esc_len;
#else
			esc_len = mbs_sequence(node_text[i]);
			xsncpy(esc_token, node_text + i, esc_len);
			i += esc_len;
#endif
		}
		else
		{
#ifdef _UNICODE
			esc_len = ucs_encode_escape(node_text[i], esc_token + 1, MAX_ESC);
#else
			esc_len = mbs_encode_escape(node_text[i], esc_token + 1, MAX_ESC);
#endif
			if (!esc_len)
			{
#ifdef _UNICODE
				esc_len = 1;
				xsncpy(esc_token, node_text + i, esc_len);
				i += esc_len;
#else
				esc_len = mbs_sequence(val[i]);
				xsncpy(esc_token, node_text + i, esc_len);
				i += esc_len;
#endif

			}
			else
			{
				esc_token[0] = CH_ESC;
				esc_len++;
				i++;
			}
		}

		if (encode == _UTF8)
#ifdef _UNICODE
			total += ucs_to_utf8(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf8(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			total += ucs_to_gb2312(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_gb2312(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			total += ucs_to_utf16lit(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf16lit(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			total += ucs_to_utf16big(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf16big(esc_token, esc_len, ((buf) ? buf + total : NULL), max - total);
#endif

		if (total >= max)
			return total;

	}

	if (total >= max)
		return total;

	if (b_cdata)
	{
		if (encode == _UTF8)
#ifdef _UNICODE
			total += ucs_to_utf8(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf8(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			total += ucs_to_gb2312(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_gb2312(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			total += ucs_to_utf16lit(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf16lit(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			total += ucs_to_utf16big(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#else
			total += mbs_to_utf16big(CDATA_TAIL, CDATA_TAIL_LEN, ((buf) ? buf + total : NULL), max - total);
#endif

		if (total >= max)
			return total;

		if (encode == _UTF8)
#ifdef _UNICODE
			total += ucs_byte_to_utf8(_T('>'), ((buf) ? buf + total : NULL));
#else
			total += ucs_byte_to_utf8(_T('>'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _GB2312)
#ifdef _UNICODE
			total += ucs_byte_to_gb2312(_T('>'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_gb2312(_T('>'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_LIT)
#ifdef _UNICODE
			total += ucs_byte_to_utf16lit(_T('>'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16lit(_T('>'), ((buf) ? buf + total : NULL));
#endif
		else if (encode == _UTF16_BIG)
#ifdef _UNICODE
			total += ucs_byte_to_utf16big(_T('>'), ((buf) ? buf + total : NULL));
#else
			total += mbs_byte_to_utf16big(_T('>'), ((buf) ? buf + total : NULL));
#endif

		if (total >= max)
			return total;
	}

	return total;
}

bool_t _format_dom_node_text_to_stream(link_t_ptr nlk, int encode, xhand_t xs)
{
	bool_t b_cdata;
	const tchar_t *node_text;
	int text_len;
	int esc_len, i;
	tchar_t esc_token[MAX_ESC + 1];

	stream_set_encode(xs, encode);

	b_cdata = (get_dom_node_mask_check(nlk, MSK_CDATA)) ? 1 : 0;

	if (b_cdata)
	{
		node_text = get_dom_node_text_ptr(nlk);
		text_len = xslen(node_text);
	}
	else
	{
		node_text = get_dom_node_text_ptr(nlk);
		text_len = xslen(node_text);
	}

	if (b_cdata)
	{
		if (!stream_write(xs, _T("<!"), 2, NULL))
			return 0;

		if(!stream_write(xs,CDATA_HEAD,CDATA_HEAD_LEN, NULL))
			return 0;
	}

	for (i = 0; i < text_len;)
	{
		if (b_cdata)
		{
#ifdef _UNICODE
			esc_len = 1;
			xsncpy(esc_token, node_text + i, esc_len);
			i += esc_len;
#else
			esc_len = mbs_sequence(val[i]);
			xsncpy(esc_token, node_text + i, esc_len);
			i += esc_len;
#endif
		}
		else
		{
#ifdef _UNICODE
			esc_len = ucs_encode_escape(node_text[i], esc_token + 1, MAX_ESC);
#else
			esc_len = mbs_encode_escape(node_text[i], esc_token + 1, MAX_ESC);
#endif
			if (!esc_len)
			{
#ifdef _UNICODE
				esc_len = 1;
				xsncpy(esc_token, node_text + i, esc_len);
				i += esc_len;
#else
				esc_len = mbs_sequence(val[i]);
				xsncpy(esc_token, node_text + i, esc_len);
				i += esc_len;
#endif

			}
			else
			{
				esc_token[0] = CH_ESC;
				esc_len++;
				i++;
			}
		}

		if (!stream_write(xs, esc_token, esc_len, NULL))
			return 0;
	}

	if (b_cdata)
	{
		if (!stream_write(xs, CDATA_TAIL, CDATA_TAIL_LEN, NULL))
			return 0;
		
		if (!stream_write(xs, _T(">"), 1, NULL))
			return 0;
	}

	return 1;
}

int _format_dom_node_rever_to_buffer(link_t_ptr nlk, int encode, byte_t* buf, int max)
{
	int total = 0;
	const tchar_t *node_ns, *node_name;
	int ns_len, name_len;
	
	//</
	if (encode == _UTF8)
#ifdef _UNICODE
		total += ucs_to_utf8(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		total += ucs_to_gb2312(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_gb2312(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		total += ucs_to_utf16lit(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf16lit(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		total += ucs_to_utf16big(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf16big(_T("</"), 2, ((buf) ? buf + total : NULL), max - total);
#endif

	if (total >= max)
		return total;

	node_name = get_dom_node_name_ptr(nlk);
	name_len = xslen(node_name);

	node_ns = get_dom_node_ns_ptr(nlk);
	ns_len = xslen(node_ns);

	//name
	total += _format_node_name_to_buffer(encode, node_ns, ns_len, node_name, name_len, ((buf) ? buf + total : NULL), max - total);

	//>
	if (encode == _UTF8)
#ifdef _UNICODE
		total += ucs_byte_to_utf8(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf8(_T('>'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _GB2312)
#ifdef _UNICODE
		total += ucs_byte_to_gb2312(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_gb2312(_T('>'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _UTF16_LIT)
#ifdef _UNICODE
		total += ucs_byte_to_utf16lit(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf16lit(_T('>'), ((buf) ? buf + total : NULL));
#endif
	else if (encode == _UTF16_BIG)
#ifdef _UNICODE
		total += ucs_byte_to_utf16big(_T('>'), ((buf) ? buf + total : NULL));
#else
		total += mbs_byte_to_utf16big(_T('>'), ((buf) ? buf + total : NULL));
#endif

	if (total >= max)
		return total;

	return total;
}

bool_t _format_dom_node_rever_to_stream(link_t_ptr nlk, int encode, xhand_t xs)
{
	const tchar_t *node_ns, *node_name;
	int ns_len, name_len;

	stream_set_encode(xs, encode);

	//</
	if (!stream_write(xs, _T("</"), 2, NULL))
		return 0;

	node_name = get_dom_node_name_ptr(nlk);
	name_len = xslen(node_name);

	node_ns = get_dom_node_ns_ptr(nlk);
	ns_len = xslen(node_ns);

	//name
	if (!_format_node_name_to_stream(encode, node_ns, ns_len, node_name, name_len, xs))
		return 0;

	//>
	if (!stream_write(xs, _T(">"), 1, NULL))
		return 0;

	return 1;
}

int format_dom_doc_to_buffer(link_t_ptr dom, byte_t* buf, int max, int encode)
{
	int total = 0;
	link_t_ptr nlk;
	link_t_ptr stack = NULL;

	nlk = dom;
	while (nlk)
	{
		total += _format_dom_node_to_buffer(nlk, encode, ((buf)? (buf + total): NULL), max - total);
		if (total >= max)
		{
			if (stack)
				destroy_stack_table(stack);

			return total;
		}

		total += _format_dom_node_text_to_buffer(nlk, encode, ((buf) ? (buf + total) : NULL), max - total);
		if (total >= max)
		{
			if (stack)
				destroy_stack_table(stack);

			return total;
		}

		if (get_dom_first_child_node(nlk))
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)nlk);
			nlk = get_dom_first_child_node(nlk);
			continue;
		}

		while (nlk)
		{
			total += _format_dom_node_rever_to_buffer(nlk, encode, ((buf) ? (buf + total) : NULL), max - total);
			if (total >= max)
			{
				if (stack)
					destroy_stack_table(stack);

				return total;
			}

			nlk = get_dom_next_sibling_node(nlk);
			if (nlk)
				break;
			nlk = (stack)? (link_t_ptr)pop_stack_node(stack) : NULL;
		}
	}

	if (stack)
		destroy_stack_table(stack);

	return total;
}

bool_t format_dom_doc_to_stream(link_t_ptr dom, xhand_t xs, int encode)
{
	link_t_ptr nlk;
	link_t_ptr stack = NULL;

	nlk = dom;
	while (nlk)
	{
		if(!_format_dom_node_to_stream(nlk, encode, xs))
		{
			if (stack)
				destroy_stack_table(stack);

			return 0;
		}

		if(!_format_dom_node_text_to_stream(nlk, encode, xs))
		{
			if (stack)
				destroy_stack_table(stack);

			return 0;
		}

		if (get_dom_first_child_node(nlk))
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)nlk);
			nlk = get_dom_first_child_node(nlk);
			continue;
		}

		while (nlk)
		{
			if(!_format_dom_node_rever_to_stream(nlk, encode, xs))
			{
				if (stack)
					destroy_stack_table(stack);

				return 0;
			}

			nlk = get_dom_next_sibling_node(nlk);
			if (nlk)
				break;
			nlk = (stack) ? (link_t_ptr)pop_stack_node(stack) : NULL;
		}
	}

	if (stack)
		destroy_stack_table(stack);

	return 1;
}


int format_xml_doc_to_buffer(link_t_ptr ptr, byte_t* buf, int max)
{
	int total = 0;
	int encode;
	tchar_t encoding[RES_LEN], version[RES_LEN];

	get_xml_encoding(ptr, encoding, RES_LEN);
	get_xml_version(ptr, version, RES_LEN);

	encode = get_xml_encode(ptr);

	total += _format_xml_attr_to_buffer(ptr, ((buf) ? buf + total : NULL), max - total);
	if (total >= max)
		return total;

	total += format_dom_doc_to_buffer(get_xml_dom_node(ptr), ((buf) ? buf + total : NULL), max - total, encode);
	if (total >= max)
		return total;

	return total;
}

bool_t format_xml_doc_to_stream(link_t_ptr ptr, xhand_t xs)
{
	int encode;
	tchar_t encoding[RES_LEN], version[RES_LEN];

	get_xml_encoding(ptr, encoding, RES_LEN);
	get_xml_version(ptr, version, RES_LEN);

	encode = get_xml_encode(ptr);

	if (!_format_xml_attr_to_stream(ptr, xs))
		return 0;

	if (!format_dom_doc_to_stream(get_xml_dom_node(ptr), xs, encode))
		return 0;

	return 1;
}

