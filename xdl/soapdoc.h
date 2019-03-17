/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc soap document

	@module	soapdoc.h | soap document interface file

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

#ifndef _SOAPDOC_H
#define _SOAPDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_soap_doc: create a soap document.
@RETURN link_t_ptr: return the soap document link component.
*/
XDL_API link_t_ptr create_soap_doc(void);

/*
@FUNCTION destroy_soap_doc: destroy soap document.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN void: none.
*/
XDL_API void destroy_soap_doc(link_t_ptr ptr);

/*
@FUNCTION is_soap_doc: test is a soap document.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN bool_t: return nonzero for soap document, otherwise return zero.
*/
XDL_API bool_t is_soap_doc(link_t_ptr ptr);

/*
@FUNCTION set_soap_default_xmlns: set default soap xml namespace.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN void: none.
*/
XDL_API void set_soap_default_xmlns(link_t_ptr ptr);

/*
@FUNCTION get_soap_head_node: get soap head node.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN link_t_ptr: return the soap head node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_soap_head_node(link_t_ptr ptr);

/*
@FUNCTION get_soap_body_node: get soap body node.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN link_t_ptr: return the soap body node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_soap_body_node(link_t_ptr ptr);

/*
@FUNCTION get_soap_request_node: get soap request node.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN link_t_ptr: return the soap request node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_soap_request_node(link_t_ptr ptr);

/*
@FUNCTION set_soap_request_node: set soap request node information.
@INPUT link_t_ptr ptr: the soap link component.
@INPUT const tchar_t* sz_req: the request name token.
@INPUT const tchar_t* sz_ns: the namespace alias token.
@INPUT const tchar_t* sz_url: the namespace url token.
@RETURN link_t_ptr: return the soap request node link component.
*/
XDL_API link_t_ptr set_soap_request_node(link_t_ptr ptr, const tchar_t* sz_req, const tchar_t* sz_ns, const tchar_t* sz_url);

/*
@FUNCTION set_soap_request_param: add a soap request node child parameter node.
@INPUT link_t_ptr nlk: the soap request link component.
@INPUT const tchar_t* pname: the parameter name token.
@INPUT const tchar_t* ptype: the parameter type token.
@INPUT const void* pdata: the parameter data or array pointer.
@INPUT int n: indicate the data array size.
@RETURN link_t_ptr: return the soap parameter node link component.
*/
XDL_API link_t_ptr set_soap_request_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int n);

/*
@FUNCTION set_soap_request_param: get a soap request node child parameter node value.
@INPUT link_t_ptr nlk: the soap request link component.
@INPUT const tchar_t* pname: the parameter name token.
@INPUT const tchar_t* ptype: the parameter type token.
@OUTPUT const void* pdata: the buffer for returning parameter data or array.
@INPUT int max: indicate the buffer size.
@RETURN int: return the soap parameter node text value length or array size.
*/
XDL_API int get_soap_request_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int max);

/*
@FUNCTION get_soap_response_node: get soap response node.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN link_t_ptr: return the soap response node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_soap_response_node(link_t_ptr ptr);

/*
@FUNCTION set_soap_response_node: set soap response node information.
@INPUT link_t_ptr ptr: the soap link component.
@INPUT const tchar_t* sz_res: the response name token.
@INPUT const tchar_t* sz_ns: the namespace alias token.
@INPUT const tchar_t* sz_url: the namespace url token.
@RETURN link_t_ptr: return the soap response node link component.
*/
XDL_API link_t_ptr set_soap_response_node(link_t_ptr ptr, const tchar_t* sz_res, const tchar_t* sz_ns, const tchar_t* sz_url);

/*
@FUNCTION set_soap_response_param: add a soap response node child parameter node.
@INPUT link_t_ptr nlk: the soap request link component.
@INPUT const tchar_t* pname: the parameter name token.
@INPUT const tchar_t* ptype: the parameter type token.
@INPUT const void* pdata: the parameter data or array pointer.
@INPUT int n: indicate the data array size.
@RETURN link_t_ptr: return the soap parameter node link component.
*/
XDL_API link_t_ptr set_soap_response_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int n);

/*
@FUNCTION get_soap_response_param: get a soap response node child parameter node value.
@INPUT link_t_ptr nlk: the soap request link component.
@INPUT const tchar_t* pname: the parameter name token.
@INPUT const tchar_t* ptype: the parameter type token.
@OUTPUT const void* pdata: the buffer for returning parameter data or array.
@INPUT int max: indicate the buffer size.
@RETURN int: return the soap parameter node text value length or array size.
*/
XDL_API int get_soap_response_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int max);

/*
@FUNCTION set_soap_fault_node: add a soap fault node.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN link_t_ptr: return the soap fault node link component.
*/
XDL_API link_t_ptr set_soap_fault_node(link_t_ptr ptr);

/*
@FUNCTION get_soap_fault_node: get soap fault node.
@INPUT link_t_ptr ptr: the soap link component.
@RETURN link_t_ptr: return the soap fault node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_soap_fault_node(link_t_ptr ptr);

/*
@FUNCTION set_soap_fault_code: set a soap fault node code.
@INPUT link_t_ptr ptr: the fault node link component.
@INPUT const tchar_t* sz_code: the fault code.
@RETURN void: none.
*/
XDL_API void set_soap_fault_code(link_t_ptr nlk, const tchar_t* sz_code);

/*
@FUNCTION set_soap_fault_string: set a soap fault node text.
@INPUT link_t_ptr ptr: the fault node link component.
@INPUT const tchar_t* sz_text: the fault text.
@RETURN void: none.
*/
XDL_API void set_soap_fault_string(link_t_ptr nlk, const tchar_t* sz_text);

/*
@FUNCTION get_soap_fault_code: get soap fault node code.
@INPUT link_t_ptr ptr: the fault node link component.
@OUTPUT tchar_t* buf: the string buffer for returning fault code.
@INPUT int max: the string buffer size in characters.
@RETURN int: return falult code token length in characters.
*/
XDL_API int get_soap_fault_code(link_t_ptr nlk, tchar_t* buf, int max);

/*
@FUNCTION get_soap_fault_string: get soap fault node text.
@INPUT link_t_ptr ptr: the fault node link component.
@OUTPUT tchar_t* buf: the string buffer for returning fault text.
@INPUT int max: the string buffer size in characters.
@RETURN int: return falult text token length in characters.
*/
XDL_API int get_soap_fault_string(link_t_ptr nlk, tchar_t* buf, int max);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_SOAPDOC_H*/