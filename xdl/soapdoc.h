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

/*************************************************document node name****************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_soap_doc();

XDL_API void destroy_soap_doc(link_t_ptr ptr);

XDL_API bool_t is_soap_doc(link_t_ptr ptr);

XDL_API void set_soap_default_xmlns(link_t_ptr ptr);

XDL_API link_t_ptr get_soap_head_node(link_t_ptr ptr);

XDL_API link_t_ptr get_soap_body_node(link_t_ptr ptr);

XDL_API link_t_ptr get_soap_request_node(link_t_ptr ptr);

XDL_API link_t_ptr set_soap_request_node(link_t_ptr ptr, const tchar_t* sz_req, const tchar_t* sz_ns, const tchar_t* sz_url);

XDL_API link_t_ptr set_soap_request_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int n);

XDL_API int get_soap_request_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int max);

XDL_API link_t_ptr get_soap_response_node(link_t_ptr ptr);

XDL_API link_t_ptr set_soap_response_node(link_t_ptr ptr, const tchar_t* sz_res, const tchar_t* sz_ns, const tchar_t* sz_url);

XDL_API link_t_ptr set_soap_response_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int n);

XDL_API int get_soap_response_param(link_t_ptr nlk, const tchar_t* pname, const tchar_t* ptype, void* pdata, int max);

XDL_API link_t_ptr set_soap_fault_node(link_t_ptr ptr);

XDL_API link_t_ptr get_soap_fault_node(link_t_ptr ptr);

XDL_API void set_soap_fault_code(link_t_ptr nlk, const tchar_t* sz_code);

XDL_API void set_soap_fault_string(link_t_ptr nlk, const tchar_t* sz_text);

XDL_API int get_soap_fault_code(link_t_ptr nlk, tchar_t* buf, int max);

XDL_API int get_soap_fault_string(link_t_ptr nlk, tchar_t* buf, int max);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_SOAPDOC_H*/