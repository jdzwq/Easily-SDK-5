/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc str utility document

	@module	strutil.h | interface file

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

#ifndef _STRUTIL_H
#define _STRUTIL_H

#include "xdsdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API int w_parse_attrset_token(const wchar_t* attrset, int len, wchar_t** pkey, int* pkeylen, wchar_t** pval, int* pvallen);

EXP_API int a_parse_attrset_token(const schar_t* attrset, int len, schar_t** pkey, int* pkeylen, schar_t** pval, int* pvallen);

EXP_API int w_parse_attrset_token_count(const wchar_t* attrset, int len);

EXP_API int a_parse_attrset_token_count(const schar_t* attrset, int len);

EXP_API int a_parse_zero_token(const schar_t* tokens, schar_t** pkey, int* pkeylen);

EXP_API int w_parse_zero_token(const wchar_t* tokens, wchar_t** pkey, int* pkeylen);

EXP_API int w_parse_zero_token_count(const wchar_t* tokens);

EXP_API int a_parse_zero_token_count(const schar_t* tokens);

EXP_API int w_parse_options_token(const wchar_t* options,int len, wchar_t itemfeed, wchar_t linefeed, wchar_t** pkey, int* pkeylen, wchar_t** pval, int* pvallen);

EXP_API int a_parse_options_token(const schar_t* options, int len, schar_t itemfeed, schar_t linefeed, schar_t** pkey, int* pkeylen, schar_t** pval, int* pvallen);

EXP_API int w_parse_options_token_count(const wchar_t* options,int len,wchar_t itemfeed, wchar_t linefeed);

EXP_API int a_parse_options_token_count(const schar_t* options, int len, schar_t itemfeed, schar_t linefeed);


EXP_API int w_get_options_value(const wchar_t* options, int len, wchar_t itemfeed, wchar_t linefeed, const wchar_t* key, wchar_t* buf, int max);

EXP_API int a_get_options_value(const schar_t* options, int len, schar_t itemfeed, schar_t linefeed, const schar_t* key, schar_t* buf, int max);

EXP_API int w_parse_string_token(const wchar_t* tokens,int len, wchar_t itemfeed, wchar_t** pkey, int* pkeylen);

EXP_API int a_parse_string_token(const schar_t* tokens, int len, schar_t itemfeed, schar_t** pkey, int* pkeylen);

EXP_API int w_parse_string_token_count(const wchar_t* tokens,int len,wchar_t itemfeed);

EXP_API int a_parse_string_token_count(const schar_t* tokens, int len, schar_t itemfeed);

EXP_API int w_parse_param_name(const wchar_t* param, int len, wchar_t itemdot, wchar_t** pkey, int* plen);

EXP_API int a_parse_param_name(const schar_t* param, int len, schar_t itemdot, schar_t** pkey, int* plen);

EXP_API int w_parse_param_name_count(const wchar_t* param, int len, wchar_t itemdot);

EXP_API int a_parse_param_name_count(const schar_t* param, int len, schar_t itemdot);

#ifdef	__cplusplus
}
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define parse_attrset_token			w_parse_attrset_token
#define parse_attrset_token_count	w_parse_attrset_token_count
#define parse_options_token			w_parse_options_token
#define parse_options_token_count	w_parse_options_token_count
#define get_options_value			w_get_options_value
#define parse_string_token			w_parse_string_token
#define parse_string_token_count	w_parse_string_token_count
#define parse_zero_token			w_parse_zero_token
#define parse_zero_token_count		w_parse_zero_token_count
#define parse_param_name			w_parse_param_name
#define parse_param_name_count		w_parse_param_name_count
#else
#define parse_attrset_token			a_parse_attrset_token
#define parse_attrset_token_count	a_parse_attrset_token_count
#define parse_options_token			a_parse_options_token
#define parse_options_token_count	a_parse_options_token_count
#define get_options_value			a_get_options_value
#define parse_string_token			a_parse_string_token
#define parse_string_token_count	a_parse_string_token_count
#define parse_zero_token			a_parse_zero_token
#define parse_zero_token_count		a_parse_zero_token_count
#define parse_param_name			a_parse_param_name
#define parse_param_name_count		a_parse_param_name_count
#endif

#endif