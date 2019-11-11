/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml bio document

	@module	xmlbio.h | interface file

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

#ifndef _XMLBIO_H
#define _XMLBIO_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API bool_t parse_xml_doc_from_bytes(link_t_ptr xml, const byte_t* str, dword_t len);

	XDL_API bool_t parse_dom_doc_from_bytes(link_t_ptr dom, const byte_t* str, dword_t len, int encode);

	XDL_API dword_t format_dom_doc_to_bytes(link_t_ptr dom, byte_t* buf, dword_t max, int encode);

	XDL_API dword_t format_xml_doc_to_bytes(link_t_ptr xml, byte_t* buf, dword_t max);


	XDL_API bool_t parse_xml_doc_from_string(link_t_ptr xml, string_t vs);

	XDL_API bool_t format_xml_doc_to_string(link_t_ptr xml, string_t vs);

	XDL_API bool_t parse_dom_doc_from_string(link_t_ptr dom, string_t vs);

	XDL_API bool_t format_dom_doc_to_string(link_t_ptr dom, string_t vs);


	XDL_API bool_t parse_xml_doc_from_stream(link_t_ptr xml, stream_t stm);

	XDL_API bool_t format_xml_doc_to_stream(link_t_ptr xml, stream_t stm);

	XDL_API bool_t parse_dom_doc_from_stream(link_t_ptr dom, stream_t stm);

	XDL_API bool_t format_dom_doc_to_stream(link_t_ptr dom, stream_t stm);


	XDL_API bool_t parse_xml_doc_from_memo(link_t_ptr xml, link_t_ptr txt);

	XDL_API bool_t format_xml_doc_to_memo(link_t_ptr xml, link_t_ptr txt);

	XDL_API bool_t parse_dom_doc_from_memo(link_t_ptr dom, link_t_ptr txt);

	XDL_API bool_t format_dom_doc_to_memo(link_t_ptr dom, link_t_ptr txt);


#ifdef	__cplusplus
}
#endif

#endif

#endif /*_XMLBIO_H*/
