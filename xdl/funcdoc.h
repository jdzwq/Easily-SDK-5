/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc func document

	@module	funcdoc.h | func document interface file

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

#ifndef _FUNCDOC_H
#define _FUNCDOC_H

#include "xdldef.h"
#include "variant.h"

#ifdef XDL_SUPPORT_DOC

/**************************************serial enable attributes**********************************************/
#define set_func_name(ptr,val)								set_dom_node_attr(ptr,ATTR_NAME,-1,val,-1)

#define get_func_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_func_data_type(ptr,val)							set_dom_node_attr(ptr,ATTR_DATA_TYPE,-1,val,-1)

#define get_func_data_type_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_DATA_TYPE,-1)

#define set_func_return_text(ptr,val)						set_dom_node_attr(ptr,ATTR_RETURN,-1,val,-1)

#define get_func_return_text_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_RETURN,-1)

#define set_func_return_integer(ptr,val)					set_dom_node_attr_integer(ptr,ATTR_RETURN,val)

#define get_func_return_integer(ptr)						get_dom_node_attr_integer(ptr,ATTR_RETURN)

#define set_func_return_boolean(ptr,val)					set_dom_node_attr_boolean(ptr,ATTR_RETURN,val)

#define get_func_return_boolean(ptr)						get_dom_node_attr_boolean(ptr,ATTR_RETURN)

#define set_func_return_numeric(ptr,val)					set_dom_node_attr_numeric(ptr,ATTR_RETURN,val)

#define get_func_return_numeric(ptr)						get_dom_node_attr_numeric(ptr,ATTR_RETURN)

#define set_func_param_name(nlk,val)						set_dom_node_attr(nlk,ATTR_NAME,-1,val,-1)

#define get_func_param_name_ptr(nlk)						get_dom_node_attr_ptr(nlk,ATTR_NAME,-1)

#define set_func_param_type(nlk,val)						set_dom_node_attr(nlk,ATTR_PARAM_TYPE,-1,val,-1)

#define get_func_param_type_ptr(nlk)						get_dom_node_attr_ptr(nlk,ATTR_PARAM_TYPE,-1)

#define set_func_param_data_type(nlk,val)					set_dom_node_attr(nlk,ATTR_DATA_TYPE,-1,val,-1)

#define get_func_param_data_type_ptr(nlk)					get_dom_node_attr_ptr(nlk,ATTR_DATA_TYPE,-1)

#define get_func_param_data_len(nlk)						get_dom_node_attr_integer(nlk,ATTR_DATA_LEN)

#define set_func_param_data_len(nlk,n)						set_dom_node_attr_integer(nlk,ATTR_DATA_LEN,n)

#define get_func_param_data_dig(nlk)						get_dom_node_attr_integer(nlk,ATTR_DATA_DIG)

#define set_func_param_data_dig(nlk,n)						set_dom_node_attr_integer(nlk,ATTR_DATA_DIG,n)

#define get_func_param_text_ptr(nlk)						get_dom_node_text_ptr(nlk)

/******************************************runtime function**************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_func_doc(void);

XDL_API void destroy_func_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_func_paramset(link_t_ptr ptr);

XDL_API void clear_func_doc(link_t_ptr ptr);

XDL_API bool_t is_func_doc(link_t_ptr ptr);

XDL_API link_t_ptr	get_func_param(link_t_ptr ptr, const tchar_t* key, int keylen);

XDL_API link_t_ptr insert_func_param(link_t_ptr ptr,link_t_ptr pos);

XDL_API int get_func_param_count(link_t_ptr ptr);

XDL_API void delete_func_param(link_t_ptr nlk);

XDL_API link_t_ptr get_func_next_param(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_func_prev_param(link_t_ptr ptr, link_t_ptr pos);

XDL_API void set_func_param_text(link_t_ptr nlk, const tchar_t* token, int len);

XDL_API int get_func_param_text(link_t_ptr nlk, tchar_t* buf, int max);

XDL_API void set_func_param_boolean(link_t_ptr nlk, bool_t b);

XDL_API bool_t get_func_param_boolean(link_t_ptr nlk);

XDL_API void set_func_param_integer(link_t_ptr nlk, long n);

XDL_API long get_func_param_integer(link_t_ptr nlk);

XDL_API void set_func_param_numeric(link_t_ptr nlk, double n);

XDL_API double get_func_param_numeric(link_t_ptr nlk);

XDL_API void merge_func_return(link_t_ptr dst, link_t_ptr src);

XDL_API void import_func_param(link_t_ptr ptr, const variant_t* pv, int n);

XDL_API int export_func_param(link_t_ptr ptr, variant_t* pv, int n);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*FUNCDOC_H*/
