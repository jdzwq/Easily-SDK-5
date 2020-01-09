/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc soap document

	@module	xdlsoap.h | soap interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#ifndef _XDLSOAP_H
#define _XDLSOAP_H

#include "xdldef.h"
#include "xdlattr.h"



XDL_API xhand_t xsoap_create(const tchar_t* sz_wsdl, const tchar_t* sz_nsname, const tchar_t* sz_nsurl, const tchar_t* sz_service, const tchar_t* sz_method);

XDL_API void  xsoap_destroy(xhand_t soa);

XDL_API void xsoap_set_request_param(xhand_t soa, const tchar_t* pname, const tchar_t* ptype);

XDL_API link_t_ptr xsoap_get_request_param(xhand_t soa, const tchar_t* pname);

XDL_API void xsoap_set_response_param(xhand_t soap, const tchar_t* pname, const tchar_t* ptype);

XDL_API link_t_ptr xsoap_get_response_param(xhand_t soa, const tchar_t* pname);

XDL_API varval_t xsoap_invoke(xhand_t soa, varval_t* vs, int n);

XDL_API void xsoap_get_fault_code(xhand_t soa, tchar_t* code);

XDL_API void xsoap_get_fault_string(xhand_t soa, tchar_t* text, int max);

#endif //XDLSOAP_H