/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc macro function document

	@module	macro.h | interface file

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

#ifndef _MACRO_H
#define _MACRO_H

#include "xdldef.h"

/*define some macro function*/
/*pluse(+) element in set eg: PLUS(1,val1,0.22,...)*/
#define FN_PLUS	_T("PLUS")

/*sub(-) sub element in set eg: SUB(10,2.9,val1,...)*/
#define FN_SUB		_T("SUB")

/*div(/) div element in set eg: DIV(100,val1,20,3.9,...)*/
#define FN_DIV		_T("DIV")

/*mul(*) mul element in set eg: MUL(3,9.23,val1,...)*/
#define FN_MUL		_T("MUL")

/*avg(sum/count) avg element in set eg: AVG(100,30,val1,30.40,...)*/
#define FN_AVG		_T("AVG")

/*find min numeric element in set eg: MIN(val1,30,100,43.98,...)*/
#define FN_MIN		_T("MIN")

/*find max numeric element in set eg: MAX(val1,30,100,43.98,...)*/
#define FN_MAX		_T("MAX")

/*round one numeric element by precision eg: ROUND(val,2) or ROUND(100.3456,2)*/
#define FN_ROUND	_T("ROUND")

/*remove one numeric negative sign eg: ABS(-100) or ABS(val)*/
#define FN_ABS		_T("ABS")

/*get one string element length eg: LEN("hello") or LEN(val)*/
#define FN_LEN		_T("LEN")

/*Returns a specified number of characters from a string element eg: MID("hello",1,3) or MID(val,1,3)*/
#define FN_MID		_T("MID")

/*cat string element in set eg: CAT("ab","cd",val,...)*/
#define FN_CAT		_T("CAT")

/*format numeric element to string by limited length and precision eg: FMT(100.2456,5,2)*/
#define FN_FMT		_T("FMT")

/*test string element is empty eg: EMPTY("")*/
#define FN_EMPTY	_T("EMPTY")

/*if function to test two element whick will be return eg: IF(val,"one","two")*/
#define FN_IF		_T("IF")

/*trim left string element eg: LTR(val)*/
#define FN_LTR		_T("LTR")

/*trim right string element eg: RTR(val)*/
#define FN_RTR		_T("RTR")

/*trim left and right string element eg: CTR*/
#define FN_CTR		_T("CTR")

/*compare two string element eg: SCMP("str1","str2")*/
#define FN_SCMP	_T("SCMP")

/*compare two numeric element eg: NCMP(100,30.20)*/
#define FN_NCMP	_T("NCMP")

/*test element is zero eg: ZERO(val)*/
#define FN_ZERO	_T("ZERO")

/*test element is less then and equal zero eg: LEZE(val)*/
#define FN_LEZE	_T("LEZE")

/*test element is grate then and equal zero eg: GRZE(val)*/
#define FN_GRZE	_T("GRZE")

/*fetch user variant value call back function*/
typedef tchar_t* (*macro_var_fetch)(const tchar_t* var,void* parm);

/*user self macro function*/
typedef tchar_t* (*macro_func_ptr)(tchar_t* pa[],int size);

#ifdef	__cplusplus
extern "C" {
#endif

/*parse macro function to multi tree*/
EXP_API bool_t macro_parse(link_t_ptr ptr,const tchar_t* str,int len);

/*format macro function to token*/
EXP_API int macro_format(link_t_ptr ptr,tchar_t* buf,int max);

/*format macro functin to token size*/
EXP_API int macro_format_size(link_t_ptr ptr);

/*run macro and return result token*/
EXP_API tchar_t* macro_calc(link_t_ptr ptr,macro_var_fetch vf,void* parm);

/*set macro self function*/
EXP_API void macro_set_func(link_t_ptr ptr,const tchar_t* funcname,macro_func_ptr pf);

/*get macro self function by name*/
EXP_API macro_func_ptr macro_get_func(link_t_ptr ptr,const tchar_t* funcname);

#ifdef	__cplusplus
}
#endif

#endif
