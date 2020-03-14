/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc wsdl document

	@module	wsdldoc.h | interface file

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

#ifndef _WSDLDOC_H
#define _WSDLDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_wsdl_doc: create a wsdl document.
@RETURN link_t_ptr: return the wsdl document link component.
*/
EXP_API link_t_ptr create_wsdl_doc();

/*
@FUNCTION destroy_wsdl_doc: destroy the wsdl document.
@INPUT link_t_ptr ptr: the wsdl document link component.
@RETURN void: none.
*/
EXP_API void destroy_wsdl_doc(link_t_ptr ptr);

/*
@FUNCTION is_wsdl_doc: test is a wsdl document.
@INPUT link_t_ptr ptr: the wsdl document link component.
@RETURN bool_t: return nonzero for being a wsdl document, otherwise return zero.
*/
EXP_API bool_t is_wsdl_doc(link_t_ptr ptr);

/*
@FUNCTION set_wsdl_default_xmlns: set wsdl default xml namespace.
@INPUT link_t_ptr ptr: the wsdl document link component.
@RETURN void: none.
*/
EXP_API void set_wsdl_default_xmlns(link_t_ptr ptr);

/*
@FUNCTION get_wsdl_types_node: get wsdl type node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@RETURN link_t_ptr: return the type node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_types_node(link_t_ptr ptr);

/*
@FUNCTION get_wsdl_next_message_node: get wsdl next message node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT link_t_ptr pos: the message node link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the message node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_next_message_node(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_wsdl_prev_message_node: get wsdl previous message node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT link_t_ptr pos: the message node link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the message node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_prev_message_node(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_wsdl_next_porttype_node: get wsdl next porttype node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT link_t_ptr pos: the porttype node link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the porttype node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_next_porttype_node(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_wsdl_prev_porttype_node: get wsdl previous porttype node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT link_t_ptr pos: the porttype node link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the porttype node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_prev_porttype_node(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_wsdl_next_binding_node: get wsdl next binding node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT link_t_ptr pos: the binding node link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the binding node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_next_binding_node(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_wsdl_prev_binding_node: get wsdl previous binding node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT link_t_ptr pos: the binding node link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the binding node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_prev_binding_node(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_wsdl_next_service_node: get wsdl next service node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT link_t_ptr pos: the service node link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the service node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_next_service_node(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_wsdl_service_node: get wsdl first service node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@RETURN link_t_ptr: return the service node link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_wsdl_service_node(link_t_ptr ptr);

/*
@FUNCTION add_wsdl_message_node: add a wsdl message node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT const tchar_t* sz_name: the message node name token.
@RETURN link_t_ptr: return the message node link component.
*/
EXP_API link_t_ptr add_wsdl_message_node(link_t_ptr ptr, const tchar_t* sz_name);

/*
@FUNCTION add_wsdl_message_part_element: add a wsdl message part node with element.
@INPUT link_t_ptr ilk: the wsdl message node link component.
@INPUT const tchar_t* sz_name: the part name token.
@INPUT const tchar_t* sz_element: the element value token.
@RETURN link_t_ptr: return the message part node link component.
*/
EXP_API link_t_ptr add_wsdl_message_part_element(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_element);

/*
@FUNCTION add_wsdl_message_part_param: add a wsdl message part node with type.
@INPUT link_t_ptr ilk: the wsdl message node link component.
@INPUT const tchar_t* sz_name: the part name token.
@INPUT const tchar_t* sz_type: the type value token.
@RETURN link_t_ptr: return the message part node link component.
*/
EXP_API link_t_ptr add_wsdl_message_part_param(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_type);

/*
@FUNCTION add_wsdl_porttype_node: add a wsdl porttype node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT const tchar_t* sz_name: the porttype node name token.
@RETURN link_t_ptr: return the porttype node link component.
*/
EXP_API link_t_ptr add_wsdl_porttype_node(link_t_ptr ptr, const tchar_t* sz_name);

/*
@FUNCTION add_wsdl_porttype_operation: add a wsdl porttype operation node.
@INPUT link_t_ptr ilk: the porttype link component.
@INPUT const tchar_t* sz_name: the operation node name token.
@INPUT const tchar_t* sz_inmsg: the operation node input message token.
@INPUT const tchar_t* sz_outmsg: the operation node output message token.
@RETURN link_t_ptr: return the porttype operation node link component.
*/
EXP_API link_t_ptr add_wsdl_porttype_operation(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_inmsg, const tchar_t* sz_outmsg);

/*
@FUNCTION add_wsdl_binding_node: add a wsdl binding node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT const tchar_t* sz_name: the binding node name token.
@INPUT const tchar_t* sz_type: the binding node type token.
@RETURN link_t_ptr: return the binding node link component.
*/
EXP_API link_t_ptr add_wsdl_binding_node(link_t_ptr ptr, const tchar_t* sz_name, const tchar_t* sz_type);

/*
@FUNCTION add_wsdl_binding_operation: add a wsdl binding operation node.
@INPUT link_t_ptr ilk: the binding link component.
@INPUT const tchar_t* sz_name: the operation node name token.
@INPUT const tchar_t* sz_action: the operation node action token.
@RETURN link_t_ptr: return the binding operation node link component.
*/
EXP_API link_t_ptr add_wsdl_binding_operation(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_action);

/*
@FUNCTION add_wsdl_service_node: add a wsdl service node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@INPUT const tchar_t* sz_name: the service node name token.
@RETURN link_t_ptr: return the service node link component.
*/
EXP_API link_t_ptr add_wsdl_service_node(link_t_ptr ptr, const tchar_t* sname);

/*
@FUNCTION add_wsdl_service_port: add a wsdl service port node.
@INPUT link_t_ptr ilk: the service link component.
@INPUT const tchar_t* sz_name: the service node name token.
@INPUT const tchar_t* sz_binding: the service node binding token.
@INPUT const tchar_t* sz_location: the service node location token.
@RETURN link_t_ptr: return the service port node link component.
*/
EXP_API link_t_ptr add_wsdl_service_port(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_binding, const tchar_t* sz_location);

/*
@FUNCTION add_wsdl_types_node: add a wsdl types node.
@INPUT link_t_ptr ptr: the wsdl document link component.
@RETURN link_t_ptr: return the types node link component.
*/
EXP_API link_t_ptr add_wsdl_types_node(link_t_ptr ptr);

/*
@FUNCTION add_wsdl_types_schema: add a wsdl types schema.
@INPUT link_t_ptr ilk: the service link component.
@INPUT const tchar_t* sz_tns: the types tns.
@RETURN link_t_ptr: return the types schema node link component.
*/
EXP_API link_t_ptr add_wsdl_types_schema(link_t_ptr ilk, const tchar_t* sz_tns);

/*
@FUNCTION add_wsdl_schema_element: add a wsdl schema element.
@INPUT link_t_ptr ilk: the schema link component.
@INPUT const tchar_t* sz_name: the schema name.
@INPUT link_t_ptr sch: the schema document link component.
@RETURN link_t_ptr: return the schema node link component.
*/
EXP_API link_t_ptr add_wsdl_schema_element(link_t_ptr ilk, const tchar_t* sz_name, link_t_ptr sch);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_WSDLDOC_H*/