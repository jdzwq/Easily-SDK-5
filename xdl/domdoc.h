/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dom document

	@module	domdoc.h | domdoc interface file

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

#ifndef _DOMDOC_H
#define _DOMDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**********************************************************************************************************************/
#define set_dom_node_attr(ptr,key,keylen,val,vallen)	write_hash_attr(get_dom_node_attr_table(ptr),key,keylen,val,vallen)

#define get_dom_node_attr(ptr,key,keylen,buf,max)		read_hash_attr(get_dom_node_attr_table(ptr),key,keylen,buf,max)

#define get_dom_node_attr_ptr(ptr,key,keylen)			get_hash_attr_ptr(get_dom_node_attr_table(ptr),key,keylen)

#define get_dom_node_attr_len(ptr,key,keylen)			get_hash_attr_len(get_dom_node_attr_table(ptr),key,keylen)

#define get_dom_node_attr_integer(ptr,key)				get_hash_attr_integer(get_dom_node_attr_table(ptr),key)

#define set_dom_node_attr_integer(ptr,key,n)			set_hash_attr_integer(get_dom_node_attr_table(ptr),key,n)

#define get_dom_node_attr_float(ptr,key)				get_hash_attr_float(get_dom_node_attr_table(ptr),key)

#define set_dom_node_attr_float(ptr,key,n)				set_hash_attr_float(get_dom_node_attr_table(ptr),key,n)

#define get_dom_node_attr_numeric(ptr,key)				get_hash_attr_numeric(get_dom_node_attr_table(ptr),key)

#define set_dom_node_attr_numeric(ptr,key,n)			set_hash_attr_numeric(get_dom_node_attr_table(ptr),key,n)

#define get_dom_node_attr_boolean(ptr,key)				((xscmp(get_dom_node_attr_ptr(ptr,key,-1),TF_TRUE) == 0)? 1 : 0)

#define set_dom_node_attr_boolean(ptr,key,b)			set_dom_node_attr(ptr,key,-1,((b)? TF_TRUE : TF_FALSE),-1)

#define set_dom_node_cdata(nlk,b)						set_dom_node_mask_check(nlk, MSK_CDATA, b)

#define get_dom_node_cdata(nlk)							get_dom_node_mask_check(nlk, MSK_CDATA)
/**********************************************************************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_dom_doc(void);

XDL_API void destroy_dom_doc(link_t_ptr ptr);

XDL_API bool_t is_dom_child_node(link_t_ptr ilk,link_t_ptr plk);

XDL_API bool_t is_dom_doc_node(link_t_ptr ilk);

XDL_API bool_t is_dom_node_attr(link_t_ptr ilk,const tchar_t* key,int len);

XDL_API link_t_ptr get_dom_from_node(link_t_ptr ilk);

XDL_API void attach_dom_node(link_t_ptr ilk,link_t_ptr pos,link_t_ptr pnew);

XDL_API link_t_ptr detach_dom_node(link_t_ptr ilk,link_t_ptr plk);

XDL_API link_t_ptr enum_dom_node(link_t_ptr ptr,CALLBACK_ENUMLINK pf,void* param);

XDL_API link_t_ptr enum_dom_node_deep(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

XDL_API link_t_ptr insert_dom_node_before(link_t_ptr ilk, link_t_ptr pos);

XDL_API link_t_ptr insert_dom_node(link_t_ptr ilk,link_t_ptr pos);

XDL_API void delete_dom_node(link_t_ptr ilk);

XDL_API void delete_dom_child_nodes(link_t_ptr ilk);

XDL_API void merge_dom_child_nodes(link_t_ptr ilk, link_t_ptr src);

XDL_API link_t_ptr get_dom_child_node_root(link_t_ptr ilk);

XDL_API int get_dom_child_node_count(link_t_ptr ilk);

XDL_API void sort_dom_child_node_by_name(link_t_ptr ilk,int desc);

XDL_API void sort_dom_child_node_by_text(link_t_ptr ilk,int desc);

XDL_API void sort_dom_child_node_by_attr(link_t_ptr ilk,const tchar_t* attr,int desc);

XDL_API link_t_ptr get_dom_first_child_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_last_child_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_parent_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_next_sibling_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_prev_sibling_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_child_node_at(link_t_ptr ilk,int index);

XDL_API link_t_ptr find_dom_node_by_name(link_t_ptr ptr,bool_t deep,const tchar_t* sz_name,int len);

XDL_API link_t_ptr find_dom_node_by_text(link_t_ptr ptr, bool_t deep,const tchar_t* sz_text, int len);

XDL_API link_t_ptr find_dom_node_by_attr(link_t_ptr ptr, bool_t deep,const tchar_t* sz_key,int klen,const tchar_t* sz_val,int vlen);

XDL_API link_t_ptr get_dom_node_attr_table(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_node_options_table(link_t_ptr ilk);

XDL_API void set_dom_node_options(link_t_ptr ilk,const tchar_t* token,int len);

XDL_API int get_dom_node_options_len(link_t_ptr ilk);

XDL_API int get_dom_node_options(link_t_ptr ilk,tchar_t* buf,int max);

XDL_API const tchar_t* get_dom_node_options_text_ptr(link_t_ptr ilk,const tchar_t* szKey,int klen);

XDL_API void set_dom_node_options_text(link_t_ptr ilk,const tchar_t* szKey,int klen,const tchar_t* szVal,int vlen);

XDL_API void set_dom_node_name(link_t_ptr ilk,const tchar_t* sz_name,int len);

XDL_API const tchar_t* get_dom_node_name_ptr(link_t_ptr ilk);

XDL_API void set_dom_node_text(link_t_ptr ilk,const tchar_t* sz_text,int len);

XDL_API int get_dom_node_text(link_t_ptr ilk,tchar_t* buf,int max);

XDL_API const tchar_t* get_dom_node_text_ptr(link_t_ptr ilk);

XDL_API void set_dom_node_nsurl(link_t_ptr ilk, const tchar_t* sz_url, int len);

XDL_API const tchar_t* get_dom_node_nsurl_ptr(link_t_ptr ilk);

XDL_API void set_dom_node_ns(link_t_ptr ilk, const tchar_t* ns, int len);

XDL_API const tchar_t* get_dom_node_ns_ptr(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_node_name_table(link_t_ptr ilk);

XDL_API void set_dom_node_xmlns(link_t_ptr ilk, const tchar_t* nsname, int klen, const tchar_t* nsurl, int vlen);

XDL_API int get_dom_node_xmlns(link_t_ptr ilk, const tchar_t* nsname, int len, tchar_t* buf, int max);

XDL_API void attach_dom_node_text(link_t_ptr ilk, tchar_t* text);

XDL_API tchar_t* detach_dom_node_attr(link_t_ptr ilk, const tchar_t* key);

XDL_API void attach_dom_node_attr(link_t_ptr ilk, const tchar_t* key, tchar_t* val);

XDL_API tchar_t* detach_dom_node_text(link_t_ptr ilk);

XDL_API int dom_node_text_get_chars(link_t_ptr ilk, int pos, tchar_t* pch, int n);

XDL_API const tchar_t* dom_node_text_set_chars(link_t_ptr ilk, int pos, const tchar_t* pch, int n);

XDL_API const tchar_t* dom_node_text_ins_chars(link_t_ptr ilk, int pos, const tchar_t* pch, int n);

XDL_API const tchar_t* dom_node_text_del_chars(link_t_ptr ilk, int pos, int n);

XDL_API const tchar_t* dom_node_attr_set_chars(link_t_ptr ilk, const tchar_t* attr, int pos, const tchar_t* pch, int n);

XDL_API const tchar_t* dom_node_attr_ins_chars(link_t_ptr ilk, const tchar_t* attr, int pos, const tchar_t* pch, int n);

XDL_API const tchar_t* dom_node_attr_del_chars(link_t_ptr ilk, const tchar_t* attr, int pos, int n);

XDL_API link_t_ptr get_dom_node_images(link_t_ptr ilk);

XDL_API void set_dom_node_images(link_t_ptr ilk, link_t_ptr il);

XDL_API void copy_dom_node(link_t_ptr dst, link_t_ptr src);

/**********************************************runtime enable function***********************************/
XDL_API bool_t get_dom_node_page_cator(link_t_ptr ilk, int pos, page_cator_t* pca);

XDL_API void set_dom_node_page_cator(link_t_ptr ilk, int pos, const page_cator_t* pca);

XDL_API int get_dom_node_page_cator_count(link_t_ptr ilk);

XDL_API void del_dom_node_page_cator(link_t_ptr ilk, int page);

XDL_API bool_t get_dom_node_line_cator(link_t_ptr ilk, int point, long* pcx, long* pcy);

XDL_API void set_dom_node_line_cator(link_t_ptr ilk, int point, long cx, long cy);

XDL_API void ins_dom_node_line_cator(link_t_ptr ilk, int point, long cx, long cy);

XDL_API void del_dom_node_line_cator(link_t_ptr ilk, int point);

XDL_API void cut_dom_node_line_cator(link_t_ptr ilk, int point);;

XDL_API int get_dom_node_line_cator_count(link_t_ptr ilk);

XDL_API var_long get_dom_node_delta(link_t_ptr ilk);

XDL_API void set_dom_node_delta(link_t_ptr ilk,var_long delta);

XDL_API dword_t get_dom_node_mask(link_t_ptr ilk);

XDL_API void set_dom_node_mask(link_t_ptr ilk,dword_t ul);

XDL_API void set_dom_node_mask_check(link_t_ptr ilk,dword_t ul,bool_t b);

XDL_API bool_t get_dom_node_mask_check(link_t_ptr ilk,dword_t ul);

XDL_API int get_dom_child_node_mask_check_count(link_t_ptr ilk,dword_t ul);

XDL_API int set_dom_child_node_mask_check(link_t_ptr ilk,dword_t ul,bool_t b);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_DOMDOC_H*/
