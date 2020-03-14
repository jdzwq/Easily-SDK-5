/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc expression document

	@module	expr.h | interface file

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

#ifndef _XDLEXP_H
#define _XDLEXP_H

#include "xdldef.h"

/*data compare callback function*/
typedef int (*expr_compare_ptr)(const tchar_t* key,const tchar_t* sin,const tchar_t* val,void* parm);

#ifdef	__cplusplus
extern "C" {
#endif

/*parse expression token*/
EXP_API bool_t expr_parse(link_t_ptr ptr,const tchar_t* str);

/*format expression token size*/
EXP_API int expr_format_length(link_t_ptr ptr);

/*format expression token*/
EXP_API int expr_format(link_t_ptr ptr,tchar_t* buf,int max);

/*execute expression and return result*/
EXP_API bool_t expr_exec(link_t_ptr ptr,expr_compare_ptr pf,void* parm);

#ifdef	__cplusplus
}
#endif

#endif /*EXPR_H*/
