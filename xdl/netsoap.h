/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc soap invoke document

	@module	netsoap.h | interface file

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

#ifndef _NETSOAP_H
#define _NETSOAP_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xhttp_invoke_soap: call soap action.
@INPUT xhand_t xhttp: the http handle.
@INPUT link_t_ptr send_soap: the input soap document.
@OUTPUT link_t_ptr recv_soap: the output soap document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xhttp_invoke_soap(xhand_t xhttp, link_t_ptr send_soap, link_t_ptr recv_soap);

/*
@FUNCTION xhttp_invoke_wsdl: get soap wsdl.
@INPUT const tchar_t* sz_url: the soap service url.
@OUTPUT link_t_ptr wsdl: the output wsdl document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xhttp_invoke_wsdl(const tchar_t* sz_url, link_t_ptr wsdl);

EXP_API bool_t get_wsdl_soap_info(link_t_ptr wsdl, const tchar_t* sz_srv, const tchar_t* sz_rpc, tchar_t* sz_loc, tchar_t* sz_act, tchar_t* sz_tns, link_t_ptr req_sch, link_t_ptr res_sch);

EXP_API void set_wsdl_soap_info(link_t_ptr wsdl, const tchar_t* sz_srv, const tchar_t* sz_rpc, const tchar_t* sz_loc, const tchar_t* sz_act, const tchar_t* sz_tns, link_t_ptr req_sch, link_t_ptr res_sch);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*NETSOAP_H*/