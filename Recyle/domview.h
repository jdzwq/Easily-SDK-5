/*************************************************************
	Easily xdl v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@doc dom document

	@module	domdoc.h | dom document interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

#ifndef _DOMDOC_H
#define _DOMDOC_H

#include "xdllink.h"
#include "hashtable.h"

#ifdef __cplusplus
extern "C" {
#endif

#define set_dom_node_attr(ptr,key,keylen,val,vallen)	write_hash_attr(get_dom_node_attr_table(ptr),key,keylen,val,vallen)

#define get_dom_node_attr(ptr,key,keylen,buf,max)		read_hash_attr(get_dom_node_attr_table(ptr),key,keylen,buf,max)

#define get_dom_node_attr_ptr(ptr,key,keylen)			get_hash_attr_ptr(get_dom_node_attr_table(ptr),key,keylen)

#define get_dom_node_attr_len(ptr,key,keylen)			get_hash_attr_len(get_dom_node_attr_table(ptr),key,keylen)

#define get_dom_node_attr_long(ptr,key)					get_hash_attr_long(get_dom_node_attr_table(ptr),key)

#define set_dom_node_attr_long(ptr,key,n)				set_hash_attr_long(get_dom_node_attr_table(ptr),key,n)

#define get_dom_node_attr_short(ptr,key)				get_hash_attr_short(get_dom_node_attr_table(ptr),key)

#define set_dom_node_attr_short(ptr,key,n)				set_hash_attr_short(get_dom_node_attr_table(ptr),key,n)

#define get_dom_node_attr_numeric(ptr,key)				get_hash_attr_numeric(get_dom_node_attr_table(ptr),key)

#define set_dom_node_attr_numeric(ptr,key,n)			set_hash_attr_numeric(get_dom_node_attr_table(ptr),key,n)

#define get_dom_node_attr_bool(ptr,key)					((xscmp(get_dom_node_attr_ptr(ptr,key,-1),TF_TRUE) == 0)? 1 : 0)

#define set_dom_node_attr_bool(ptr,key,b)				set_dom_node_attr(ptr,key,-1,((b)? TF_TRUE : TF_FALSE),-1)

/**********************************************************************************************************************/

XDL_API link_t_ptr create_dom_doc(void);

XDL_API void destroy_dom_doc(link_t_ptr ptr);

XDL_API bool_t is_dom_child_node(link_t_ptr ilk,link_t_ptr plk);

XDL_API bool_t is_dom_doc_node(link_t_ptr ilk);

XDL_API bool_t is_dom_node_attr(link_t_ptr ilk,const wchar_t* key,int len);

XDL_API link_t_ptr get_dom_from_node(link_t_ptr ilk);

XDL_API void copy_dom_node(link_t_ptr ptr_dest,link_t_ptr ptr_src);

XDL_API link_t_ptr get_dom_node_attr_table(link_t_ptr ptr);

XDL_API void attach_dom_node(link_t_ptr ilk,link_t_ptr pnew);

XDL_API link_t_ptr detach_dom_node(link_t_ptr ilk,link_t_ptr plk);

XDL_API link_t_ptr enum_dom_node(link_t_ptr ptr,link_enum_call pf,void* param);

XDL_API link_t_ptr insert_dom_node(link_t_ptr ilk,link_t_ptr pos);

XDL_API void delete_dom_node(link_t_ptr ilk);

XDL_API void delete_dom_child_node(link_t_ptr ilk);

XDL_API int get_dom_child_node_count(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_first_child_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_last_child_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_parent_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_next_sibling_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_prev_sibling_node(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_child_node_at(link_t_ptr ilk,int index);

XDL_API link_t_ptr find_dom_node_by_attr(link_t_ptr ptr,const wchar_t* condi);

XDL_API link_t_ptr find_dom_node_by_name(link_t_ptr ptr,const wchar_t* sz_name);

XDL_API link_t_ptr get_dom_node_attr_table(link_t_ptr ilk);

XDL_API link_t_ptr get_dom_node_options_table(link_t_ptr ilk);

XDL_API void set_dom_node_options(link_t_ptr ilk,const wchar_t* token,int len);

XDL_API int get_dom_node_options_len(link_t_ptr ilk);

XDL_API int get_dom_node_options(link_t_ptr ilk,wchar_t* buf,int max);

XDL_API const wchar_t* get_dom_node_options_text_ptr(link_t_ptr ilk,const wchar_t* szKey,int klen);

XDL_API void set_dom_node_options_text(link_t_ptr ilk,const wchar_t* szKey,int klen,const wchar_t* szVal,int vlen);

XDL_API void set_dom_node_name(link_t_ptr ilk,const wchar_t* sz_name,int len);

XDL_API const wchar_t* get_dom_node_name_ptr(link_t_ptr ilk);

XDL_API void set_dom_node_text(link_t_ptr ilk,const wchar_t* sz_text,int len);

XDL_API const wchar_t* get_dom_node_text_ptr(link_t_ptr ilk);

XDL_API int get_dom_node_text_length(link_t_ptr ilk);

XDL_API int get_dom_node_text(link_t_ptr ilk,wchar_t* buf,int max);

XDL_API void set_dom_node_cdata(link_t_ptr ilk,const void* sz_text,int len);

XDL_API const void* get_dom_node_cdata_ptr(link_t_ptr ilk);

XDL_API int get_dom_node_cdata_size(link_t_ptr ilk);

XDL_API int get_dom_node_cdata(link_t_ptr ilk,void* buf,int max);

/**********************************************runtime enable function***********************************/
XDL_API void set_dom_page_height(link_t_ptr ptr,long ph);

XDL_API long get_dom_page_height(link_t_ptr ptr);

XDL_API void set_dom_page_width(link_t_ptr ptr,long pw);

XDL_API long get_dom_page_width(link_t_ptr ptr);

XDL_API void set_dom_page_xoff(link_t_ptr ptr,long xoff);

XDL_API long get_dom_page_xoff(link_t_ptr ptr);

XDL_API void set_dom_page_yoff(link_t_ptr ptr,long yoff);

XDL_API long get_dom_page_yoff(link_t_ptr ptr);

XDL_API handle_t get_dom_node_delta(link_t_ptr ilk);

XDL_API void set_dom_node_delta(link_t_ptr ilk,handle_t h);

XDL_API unsigned long get_dom_node_mask(link_t_ptr ilk);

XDL_API void set_dom_node_mask(link_t_ptr ilk,unsigned long ul);

XDL_API void set_dom_node_mask_check(link_t_ptr ilk,unsigned long ul,bool_t b);

XDL_API bool_t get_dom_node_mask_check(link_t_ptr ilk,unsigned long ul);

XDL_API int get_dom_child_node_mask_check_count(link_t_ptr ilk,unsigned long ul);

XDL_API void clear_dom_child_node_mask_check(link_t_ptr ilk,unsigned long ul);


#ifdef __cplusplus
}
#endif

#endif /*_DOMDOC_H*/
