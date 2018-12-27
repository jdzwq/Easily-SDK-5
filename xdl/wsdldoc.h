/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc wsdl document

	@module	wsdldoc.h | wsdl document interface file

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

XDL_API link_t_ptr create_wsdl_doc();

XDL_API void destroy_wsdl_doc(link_t_ptr ptr);

XDL_API bool_t is_wsdl_doc(link_t_ptr ptr);

XDL_API void set_wsdl_default_xmlns(link_t_ptr ptr);

XDL_API link_t_ptr get_wsdl_types_node(link_t_ptr ptr);

XDL_API link_t_ptr get_wsdl_next_message_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_wsdl_prev_message_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_wsdl_next_porttype_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_wsdl_prev_porttype_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_wsdl_next_binding_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_wsdl_prev_binding_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_wsdl_next_service_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_wsdl_service_node(link_t_ptr ptr);

XDL_API link_t_ptr add_wsdl_message_node(link_t_ptr ptr, const tchar_t* sz_name);

XDL_API link_t_ptr add_wsdl_message_part_element(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_element);

XDL_API link_t_ptr add_wsdl_message_part_param(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_type);

XDL_API link_t_ptr add_wsdl_porttype_node(link_t_ptr ptr, const tchar_t* sz_name);

XDL_API link_t_ptr add_wsdl_porttype_operation(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_inmsg, const tchar_t* sz_outmsg);

XDL_API link_t_ptr add_wsdl_binding_node(link_t_ptr ptr, const tchar_t* sz_name, const tchar_t* sz_type);

XDL_API link_t_ptr add_wsdl_binding_operation(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_action);

XDL_API link_t_ptr add_wsdl_service_node(link_t_ptr ptr, const tchar_t* sname);

XDL_API link_t_ptr add_wsdl_service_port(link_t_ptr ilk, const tchar_t* sz_name, const tchar_t* sz_binding, const tchar_t* sz_location);

XDL_API link_t_ptr add_wsdl_types_node(link_t_ptr ptr);

XDL_API link_t_ptr add_wsdl_types_schema(link_t_ptr ilk, const tchar_t* sz_tns);

XDL_API link_t_ptr add_wsdl_schema_element(link_t_ptr ilk, const tchar_t* sz_name, link_t_ptr sch);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_WSDLDOC_H*/