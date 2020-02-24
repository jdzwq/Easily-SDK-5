/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml parser document

	@module	xmlparser.h | interface file

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

#ifndef _XMLPARSER_H
#define _XMLPARSER_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

//定义注解符
#define A_CMTOKEN			"--"
#define W_CMTOKEN			L"--"
#define CMTOKEN				_T("--")
#define CMTOKEN_LEN			2

//定义界段符前缀
#define A_CDATA_HEAD		"[CDATA["
#define W_CDATA_HEAD		L"[CDATA["
#define CDATA_HEAD			_T("[CDATA[")
#define CDATA_HEAD_LEN		7
//定义界段符尾缀
#define A_CDATA_TAIL		"]]"
#define W_CDATA_TAIL		L"]]"
#define CDATA_TAIL			_T("]]")
#define CDATA_TAIL_LEN		2


//判断是否需转义的字符
//#define _IsEscapeChar(ch) ((ch == _T('<') || ch == _T('>') || ch == _T('&') || ch == _T('\"') || ch == _T('\'') || ch == _T('/') || ch == _T('\f'))? 1 : 0)
#define _IsEscapeChar(ch) ((ch == _T('<') || ch == _T('>') || ch == _T('&') || ch == _T('\"') || ch == _T('\''))? 1 : 0)

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API bool_t parse_xml_doc_from_object(link_t_ptr ptr, if_operator_t* pbo);

	XDL_API bool_t parse_dom_doc_from_object(link_t_ptr ptr, if_operator_t* pbo);

	XDL_API bool_t format_xml_doc_to_object(link_t_ptr ptr, if_operator_t* pbo);

	XDL_API bool_t format_dom_doc_to_object(link_t_ptr ptr, if_operator_t* pbo);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif	/*_XMLPARSER_H*/