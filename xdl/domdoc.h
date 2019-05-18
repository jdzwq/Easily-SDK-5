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

/*
@FUNCTION create_dom_doc: create a dom document.
@RETURN link_t_ptr: return the dom document link component.
*/
XDL_API link_t_ptr create_dom_doc(void);

/*
@FUNCTION destroy_dom_doc: destroy a dom document.
@INPUT link_t_ptr ptr: the dom document link component.
@RETURN void: none.
*/
XDL_API void destroy_dom_doc(link_t_ptr ptr);

/*
@FUNCTION is_dom_child_node: test the link component is a dom child node.
@INPUT link_t_ptr ptr: the dom link component.
@INPUT link_t_ptr plk: the node link component.
@RETURN bool_t: return nonzero for being the child, otherwise return zero.
*/
XDL_API bool_t is_dom_child_node(link_t_ptr ilk, link_t_ptr plk);

/*
@FUNCTION is_dom_doc_node: test the link component is a dom node.
@INPUT link_t_ptr ilk: the node link component.
@RETURN bool_t: return nonzero for being the dom node, otherwise return zero.
*/
XDL_API bool_t is_dom_doc_node(link_t_ptr ilk);

/*
@FUNCTION is_dom_node_attr: test the attribute is in dom node attributes table.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int len: the length of key token in characters.
@RETURN bool_t: return nonzero for being exists, otherwise return zero.
*/
XDL_API bool_t is_dom_node_attr(link_t_ptr ilk, const tchar_t* key, int len);

/*
@FUNCTION get_dom_from_node: track back to dom link component from node link component.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: if succeeds return the dom link component, fails return NULL.
*/
XDL_API link_t_ptr get_dom_from_node(link_t_ptr ilk);

/*
@FUNCTION attach_dom_node: attach a dom node after the position.
@INPUT link_t_ptr ilk: the parent node link component.
@INPUT link_t_ptr pos: the position, it can be node link component or indicator: LINK_FIRST, LINK_LAST.
@INPUT link_t_ptr pnew: the node link component to insert.
@RETURN void: none.
*/
XDL_API void attach_dom_node(link_t_ptr ilk, link_t_ptr pos, link_t_ptr pnew);

/*
@FUNCTION detach_dom_node: detach a dom node from parent node.
@INPUT link_t_ptr ilk: the parent node link component.
@INPUT link_t_ptr pos: the node link component or indicator (LINK_FIRST, LINK_LAST) to be detached.
@RETURN link_t_ptr: return the link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr detach_dom_node(link_t_ptr ilk, link_t_ptr pos);

/*
@FUNCTION enum_dom_node: enum a dom document nodes.
@INPUT link_t_ptr ptr: the dom document link component.
@INPUT CALLBACK_ENUMLINK pf: the callback function, if the function return zero, the enumerating will be breaked.
@INPUT void* param: the parameter translate into callback function.
@RETURN link_t_ptr: return the link component enumeration breaken at, otherwise return NULL.
*/
XDL_API link_t_ptr enum_dom_node(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

/*
@FUNCTION enum_dom_node: enum a dom document nodes by deep order.
@INPUT link_t_ptr ptr: the dom document link component.
@INPUT CALLBACK_ENUMLINK pf: the callback function, if the function return zero, the enumerating will be breaked.
@INPUT void* param: the parameter translate into callback function.
@RETURN link_t_ptr: return the link component enumeration breaken at, otherwise return NULL.
*/
XDL_API link_t_ptr enum_dom_node_deep(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

/*
@FUNCTION insert_dom_node_before: insert a new dom node before the position.
@INPUT link_t_ptr ilk: the parent link component.
@INPUT link_t_ptr pos: the position link component, it can be indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new link component inserted.
*/
XDL_API link_t_ptr insert_dom_node_before(link_t_ptr ilk, link_t_ptr pos);

/*
@FUNCTION insert_dom_node: insert a new dom node after the position.
@INPUT link_t_ptr ilk: the parent link component.
@INPUT link_t_ptr pos: the position link component, it can be indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new link component inserted.
*/
XDL_API link_t_ptr insert_dom_node(link_t_ptr ilk, link_t_ptr pos);

/*
@FUNCTION delete_dom_node: delete a dom node.
@INPUT link_t_ptr ilk: the node link component.
@RETURN void: none.
*/
XDL_API void delete_dom_node(link_t_ptr ilk);

/*
@FUNCTION delete_dom_child_nodes: delete all child nodes.
@INPUT link_t_ptr ilk: the parent link component.
@RETURN void: none.
*/
XDL_API void delete_dom_child_nodes(link_t_ptr ilk);

/*
@FUNCTION merge_dom_child_nodes: merge the source child nodes to destination child, then the source child nodes to be emptied.
@INPUT link_t_ptr dst: the destination link component.
@INPUT link_t_ptr src: the source link component.
@RETURN void: none.
*/
XDL_API void merge_dom_child_nodes(link_t_ptr dst, link_t_ptr src);

/*
@FUNCTION get_dom_child_node_root: get the child nodes root link component.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the root link component.
*/
XDL_API link_t_ptr get_dom_child_node_root(link_t_ptr ilk);

/*
@FUNCTION get_dom_child_node_count: get the child nodes count.
@INPUT link_t_ptr ilk: the node link component.
@RETURN int: return the count of child nodes.
*/
XDL_API int get_dom_child_node_count(link_t_ptr ilk);

/*
@FUNCTION sort_dom_child_node_by_name: sort the child nodes by name order.
@INPUT link_t_ptr ilk: the node link component.
@INPUT int desc: nonzero for descending sorting, zero for ascending sorting.
@RETURN void: none.
*/
XDL_API void sort_dom_child_node_by_name(link_t_ptr ilk, int desc);

/*
@FUNCTION sort_dom_child_node_by_text: sort the child nodes by text order.
@INPUT link_t_ptr ilk: the node link component.
@INPUT int desc: nonzero for descending sorting, zero for ascending sorting.
@RETURN void: none.
*/
XDL_API void sort_dom_child_node_by_text(link_t_ptr ilk, int desc);

/*
@FUNCTION sort_dom_child_node_by_attr: sort the child nodes by attribute value.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* attr: the attribute key token.
@INPUT int desc: nonzero for descending sorting, zero for ascending sorting.
@RETURN void: none.
*/
XDL_API void sort_dom_child_node_by_attr(link_t_ptr ilk, const tchar_t* attr, int desc);

/*
@FUNCTION get_dom_first_child_node: get the first child node.
@INPUT link_t_ptr ilk: the parent link component.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_first_child_node(link_t_ptr ilk);

/*
@FUNCTION get_dom_last_child_node: get the last child node.
@INPUT link_t_ptr ilk: the parent link component.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_last_child_node(link_t_ptr ilk);

/*
@FUNCTION get_dom_parent_node: get the parent node.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the parent link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_parent_node(link_t_ptr ilk);

/*
@FUNCTION get_dom_next_sibling_node: get the next sibling node.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_next_sibling_node(link_t_ptr ilk);

/*
@FUNCTION get_dom_prev_sibling_node: get the previous sibling node.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_prev_sibling_node(link_t_ptr ilk);

/*
@FUNCTION get_dom_node_level: get the node level.
@INPUT link_t_ptr ilk: the node link component.
@RETURN int: return the zero based level index.
*/
XDL_API int get_dom_node_level(link_t_ptr ilk);

/*
@FUNCTION get_dom_child_node_at: find the node by position.
@INPUT link_t_ptr ilk: the parent link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_child_node_at(link_t_ptr ilk, int index);

/*
@FUNCTION find_dom_node_by_name: find the node by name.
@INPUT link_t_ptr ptr: the dom link component.
@INPUT bool_t deep: nonzer for deep order serching, zero for level order searching.
@INPUT const tchar_t* sz_name: the name string token.
@INPUT int len: the name toke length in characters.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr find_dom_node_by_name(link_t_ptr ptr, bool_t deep, const tchar_t* sz_name, int len);

/*
@FUNCTION find_dom_node_by_text: find the node by text.
@INPUT link_t_ptr ptr: the dom link component.
@INPUT bool_t deep: nonzer for deep order serching, zero for level order searching.
@INPUT const tchar_t* sz_text: the text string token.
@INPUT int len: the text toke length in characters.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr find_dom_node_by_text(link_t_ptr ptr, bool_t deep, const tchar_t* sz_text, int len);

/*
@FUNCTION find_dom_node_by_attr: find the node by attribute.
@INPUT link_t_ptr ptr: the dom link component.
@INPUT bool_t deep: nonzer for deep order serching, zero for level order searching.
@INPUT const tchar_t* sz_key: the attribyte key token.
@INPUT int klen: the key toke length in characters.
@INPUT const tchar_t* sz_val: the attribyte value token.
@INPUT int vlen: the value toke length in characters.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr find_dom_node_by_attr(link_t_ptr ptr, bool_t deep, const tchar_t* sz_key, int klen, const tchar_t* sz_val, int vlen);

/*
@FUNCTION get_dom_node_attr_table: get the node attributes table.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the hash table link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_node_attr_table(link_t_ptr ilk);

/*
@FUNCTION get_dom_node_options_table: get the node options table.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the string table link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_node_options_table(link_t_ptr ilk);

/*
@FUNCTION set_dom_node_options: get the node options table.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the string table link component if exists, otherwise return NULL.
*/
XDL_API void set_dom_node_options(link_t_ptr ilk, const tchar_t* token, int len);

/*
@FUNCTION get_dom_node_options_len: calc the node options token length.
@INPUT link_t_ptr ilk: the node link component.
@RETURN int: return the options token length in characters if exists, otherwise return zero.
*/
XDL_API int get_dom_node_options_len(link_t_ptr ilk);

/*
@FUNCTION get_dom_node_options: format the node options token.
@INPUT link_t_ptr ilk: the node link component.
@OUTPUT tchar_t* buf: the string buffer for return options token.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return the options token length in characters if exists, otherwise return zero.
*/
XDL_API int get_dom_node_options(link_t_ptr ilk, tchar_t* buf, int max);

/*
@FUNCTION get_dom_node_options_text_ptr: get the node options sub item value pointer by key.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* szKey: the key string token.
@INPUT int klen: the key token length in characters.
@RETURN const tchar_t*: return the options sub item value string pointer if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_dom_node_options_text_ptr(link_t_ptr ilk, const tchar_t* szKey, int klen);

/*
@FUNCTION set_dom_node_options_text: add a sub item to node options, if the key exists in options, the value will be repleased.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* szKey: the key string token.
@INPUT int klen: the key token length in characters.
@INPUT const tchar_t* szVal: the value string token.
@INPUT int vlen: the value token length in characters.
@RETURN void: none.
*/
XDL_API void set_dom_node_options_text(link_t_ptr ilk, const tchar_t* szKey, int klen, const tchar_t* szVal, int vlen);

/*
@FUNCTION set_dom_node_name: set dom node name.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* sz_name: the name string token.
@INPUT int len: the name token length in characters.
@RETURN void: none.
*/
XDL_API void set_dom_node_name(link_t_ptr ilk, const tchar_t* sz_name, int len);

/*
@FUNCTION get_dom_node_name_ptr: get dom node name string pointer.
@INPUT link_t_ptr ilk: the node link component.
@RETURN const tchar_t*: the name string pointer.
*/
XDL_API const tchar_t* get_dom_node_name_ptr(link_t_ptr ilk);

/*
@FUNCTION set_dom_node_text: set dom node text.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* sz_text: the text string token.
@INPUT int len: the text token length in characters.
@RETURN void: none.
*/
XDL_API void set_dom_node_text(link_t_ptr ilk, const tchar_t* sz_text, int len);

/*
@FUNCTION get_dom_node_text: copy dom node text.
@INPUT link_t_ptr ilk: the node link component.
@OUTPUT tchar_t* buf: the string buffer for returning text.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the node text length in characters.
*/
XDL_API int get_dom_node_text(link_t_ptr ilk, tchar_t* buf, int max);

/*
@FUNCTION get_dom_node_text_ptr: get dom node text string pointer.
@INPUT link_t_ptr ilk: the node link component.
@RETURN const tchar_t*: the text string pointer.
*/
XDL_API const tchar_t* get_dom_node_text_ptr(link_t_ptr ilk);

/*
@FUNCTION attach_dom_node_text: attach a text buffer to dom node, the original text in node will be freed.
@INPUT link_t_ptr ilk: the node link component.
@INPUT tchar_t* text: the text string buffer.
@RETURN void: none.
*/
XDL_API void attach_dom_node_text(link_t_ptr ilk, tchar_t* text);

/*
@FUNCTION detach_dom_node_text: detach text buffer from dom node, then the node text will be empty.
@INPUT link_t_ptr ilk: the node link component.
@RETURN tchar_t*: return the text string buffer.
*/
XDL_API tchar_t* detach_dom_node_text(link_t_ptr ilk);

/*
@FUNCTION attach_dom_node_attr: attach a value string buffer to attrubutes of the dom node.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* key: the key string token.
@INPUT tchar_t* val: the value string buffer.
@RETURN void: none.
*/
XDL_API void attach_dom_node_attr(link_t_ptr ilk, const tchar_t* key, tchar_t* val);

/*
@FUNCTION detach_dom_node_attr: detach value buffer from attribytes of the dom node.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* key: the key string token.
@RETURN tchar_t*: return the value string buffer.
*/
XDL_API tchar_t* detach_dom_node_attr(link_t_ptr ilk, const tchar_t* key);

/*
@FUNCTION dom_node_text_get_chars: get some characters from dom node text.
@INPUT link_t_ptr ilk: the node link component.
@INPUT int pos: the zero based position.
@OUTPUT tchar_t* pch: the string buffer.
@INPUT int n: the number of characters wante to copy.
@RETURN int: return the number of characters copyed.
*/
XDL_API int dom_node_text_get_chars(link_t_ptr ilk, int pos, tchar_t* pch, int n);

/*
@FUNCTION dom_node_text_set_chars: replace some characters of dom node text start at position.
@INPUT link_t_ptr ilk: the node link component.
@INPUT int pos: the zero based start position.
@INPUT const tchar_t* pch: the characters array.
@INPUT int n: the number of characters wante to set.
@RETURN const tchar_t: return the text string pointer of the node text.
*/
XDL_API const tchar_t* dom_node_text_set_chars(link_t_ptr ilk, int pos, const tchar_t* pch, int n);

/*
@FUNCTION dom_node_text_ins_chars: insert some characters of dom node text start at position.
@INPUT link_t_ptr ilk: the node link component.
@INPUT int pos: the zero based start position.
@INPUT const tchar_t* pch: the characters array.
@INPUT int n: the number of characters wante to insert.
@RETURN const tchar_t: return the text string pointer of the node text.
*/
XDL_API const tchar_t* dom_node_text_ins_chars(link_t_ptr ilk, int pos, const tchar_t* pch, int n);

/*
@FUNCTION dom_node_text_del_chars: delete some characters of dom node text start at position.
@INPUT link_t_ptr ilk: the node link component.
@INPUT int pos: the zero based start position.
@INPUT int n: the number of characters wante to delete.
@RETURN const tchar_t: return the text string pointer of the node text.
*/
XDL_API const tchar_t* dom_node_text_del_chars(link_t_ptr ilk, int pos, int n);

/*
@FUNCTION dom_node_attr_set_chars: replace some characters of dom node arrtibute value start at position.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* attr: the attribute key token.
@INPUT int pos: the zero based start position.
@INPUT const tchar_t* pch: the characters array.
@INPUT int n: the number of characters wante to set.
@RETURN const tchar_t: return the attrubute value string pointer of the node.
*/
XDL_API const tchar_t* dom_node_attr_set_chars(link_t_ptr ilk, const tchar_t* attr, int pos, const tchar_t* pch, int n);

/*
@FUNCTION dom_node_attr_ins_chars: insert some characters of dom node attribute value start at position.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* attr: the attribute key token.
@INPUT int pos: the zero based start position.
@INPUT const tchar_t* pch: the characters array.
@INPUT int n: the number of characters wante to insert.
@RETURN const tchar_t: return the attrubute value string pointer of the node.
*/
XDL_API const tchar_t* dom_node_attr_ins_chars(link_t_ptr ilk, const tchar_t* attr, int pos, const tchar_t* pch, int n);

/*
@FUNCTION dom_node_attr_del_chars: delete some characters of dom node attribute value start at position.
@INPUT link_t_ptr ilk: the node link component.
@INPUT const tchar_t* attr: the attribute key token.
@INPUT int pos: the zero based start position.
@INPUT int n: the number of characters wante to delete.
@RETURN const tchar_t: return the attribute value string pointer of the node.
*/
XDL_API const tchar_t* dom_node_attr_del_chars(link_t_ptr ilk, const tchar_t* attr, int pos, int n);

/*
@FUNCTION get_dom_node_images: get dom node images document refrenced.
@INPUT link_t_ptr ilk: the node link component.
@RETURN link_t_ptr: return the images document link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_node_images(link_t_ptr ilk);

/*
@FUNCTION set_dom_node_images: set a refrenced images document to dom node.
@INPUT link_t_ptr ilk: the node link component.
@INPUT link_t_ptr il: the images document link component.
@RETURN void: none.
*/
XDL_API void set_dom_node_images(link_t_ptr ilk, link_t_ptr il);

/*
@FUNCTION copy_dom_node: copy source dom node and its child nodes to destination node.
@INPUT link_t_ptr dst: the destination node link component.
@INPUT link_t_ptr src: the source node link component.
@RETURN void: none.
*/
XDL_API void copy_dom_node(link_t_ptr dst, link_t_ptr src);

/*
@FUNCTION set_dom_node_nsurl: set dom node default xml namespace.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT const tchar_t* sz_url: the xml namespace token.
@RETURN void: none.
*/
XDL_API void set_dom_node_nsurl(link_t_ptr ilk, const tchar_t* sz_url, int len);

/*
@FUNCTION get_dom_node_nsurl_ptr: get dom node default xml namespace.
@INPUT link_t_ptr ilk: the dom node link component.
@RETURN const tchar_t*: return default xml namespace token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_dom_node_nsurl_ptr(link_t_ptr ilk);

/*
@FUNCTION set_dom_node_ns: set dom node namespace alias.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT const tchar_t* ns: the alias string token.
@INPUT int len: the alias token length in characters.
@RETURN void: none.
*/
XDL_API void set_dom_node_ns(link_t_ptr ilk, const tchar_t* ns, int len);

/*
@FUNCTION get_dom_node_ns_ptr: get dom node namespace alias.
@INPUT link_t_ptr ilk: the dom node link component.
@RETURN const tchar_t*: return the alias string token if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_dom_node_ns_ptr(link_t_ptr ilk);

/*
@FUNCTION get_dom_node_name_table: get dom node namespace string table.
@INPUT link_t_ptr ilk: the dom node link component.
@RETURN link_t_ptr: return the namespace string table link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dom_node_name_table(link_t_ptr ilk);

/*
@FUNCTION set_dom_node_xmlns: set dom node namespace.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT const tchar_t* nsname: the namespace alias string token.
@INPUT int klen: the alias token length in characters.
@INPUT const tchar_t* nsurl: the namespace url string token.
@INPUT int klen: the url token length in characters.
@RETURN void: none.
*/
XDL_API void set_dom_node_xmlns(link_t_ptr ilk, const tchar_t* nsname, int klen, const tchar_t* nsurl, int vlen);

/*
@FUNCTION get_dom_node_xmlns: get dom node namespace.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT const tchar_t* nsname: the namespace alias string token.
@INPUT int klen: the alias token length in characters.
@OUTPUT tchar_t* buf: the string buffer for returning namespace url.
@INPUT int max: the url buffer size in characters.
@RETURN int: return the namespace url token length in characters.
*/
XDL_API int get_dom_node_xmlns(link_t_ptr ilk, const tchar_t* nsname, int len, tchar_t* buf, int max);

/**********************************************runtime enable function***********************************/

/*
@FUNCTION get_dom_node_delta: get dom node extract data.
@INPUT link_t_ptr ilk: the dom node link component.
@RETURN var_long: return the extract data if exists, otherwise return zero.
*/
XDL_API var_long get_dom_node_delta(link_t_ptr ilk);

/*
@FUNCTION set_dom_node_delta: set dom node extract data.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT var_long delta: the extract data.
@RETURN void: none.
*/
XDL_API void set_dom_node_delta(link_t_ptr ilk, var_long delta);

/*
@FUNCTION get_dom_node_mask: get dom node mask value.
@INPUT link_t_ptr ilk: the dom node link component.
@RETURN dword_t: return mask value.
*/
XDL_API dword_t get_dom_node_mask(link_t_ptr ilk);

/*
@FUNCTION set_dom_node_mask: set dom node mask value.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT dword_t ul: the mask value.
@RETURN void: none.
*/
XDL_API void set_dom_node_mask(link_t_ptr ilk, dword_t ul);

/*
@FUNCTION set_dom_node_mask_check: set dom node mask check.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT dword_t ul: the mask value.
@INPUT bool_t b: nonzero for checking, zero for unchecking.
@RETURN void: none.
*/
XDL_API void set_dom_node_mask_check(link_t_ptr ilk, dword_t ul, bool_t b);

/*
@FUNCTION get_dom_node_mask_check: get dom node mask check.
@INPUT link_t_ptr ilk: the dom node link component.
@INPUT dword_t ul: the mask value.
@RETURN bool_t: return nonzero for checked, zero for unchecked.
*/
XDL_API bool_t get_dom_node_mask_check(link_t_ptr ilk, dword_t ul);

/*
@FUNCTION get_dom_child_node_mask_check_count: get dom child nodes mask checked count.
@INPUT link_t_ptr ilk: the parent node link component.
@INPUT dword_t ul: the mask value.
@RETURN bool_t: return mask checked count of the child nodes.
*/
XDL_API int get_dom_child_node_mask_check_count(link_t_ptr ilk, dword_t ul);

/*
@FUNCTION set_dom_child_node_mask_check: set dom child nodes mask check.
@INPUT link_t_ptr ilk: the parent node link component.
@INPUT dword_t ul: the mask value.
@INPUT bool_t b: nonzero for checking, zero for unchecking.
@RETURN int: return count of the child nodes with mask changed.
*/
XDL_API int set_dom_child_node_mask_check(link_t_ptr ilk, dword_t ul, bool_t b);

/************************************************runtime paging function************************************/

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

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_DOMDOC_H*/
