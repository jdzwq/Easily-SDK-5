/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xml parse document

	@module	xdlxml.h | xml parse interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#ifndef _XDLXML_H
#define _XDLXML_H

#include "xdldef.h"
#include "xdlattr.h"

XDL_API bool_t parse_xml_doc_from_buffer(link_t_ptr ptr,const byte_t* str,int len);

XDL_API bool_t parse_xml_doc_from_stream(link_t_ptr ptr, xhand_t xs);

XDL_API bool_t parse_dom_doc_from_buffer(link_t_ptr ptr, const byte_t* str, int len, int encode);

XDL_API bool_t parse_dom_doc_from_stream(link_t_ptr ptr, xhand_t xs);

XDL_API int format_xml_doc_to_buffer(link_t_ptr ptr,byte_t* buf,int max);

XDL_API bool_t format_xml_doc_to_stream(link_t_ptr ptr, xhand_t xs);

XDL_API int format_dom_doc_to_buffer(link_t_ptr dom, byte_t* buf, int max, int encode);

XDL_API bool_t format_dom_doc_to_stream(link_t_ptr dom, xhand_t xs, int encode);

/***********************************************dom body enumerator*************************************/
typedef int (__callback *PF_READ_CHAR)(void* p_obj, int max, int pos, int encode, tchar_t* pch);
typedef int (__callback *PF_READ_ESCAPE)(void* p_obj, int max, int pos, int encode, tchar_t* pch);

typedef int (__callback *PF_WRITE_CHAR)(void* p_obj, int max, int pos, int encode, tchar_t ch);
typedef int (__callback *PF_WRITE_ESCAPE)(void* p_obj, int max, int pos, int encode, tchar_t ch);

typedef int (__callback *PF_WRITE_TOKEN)(void* p_obj, int max, int pos, int encode, const tchar_t* pch, int len);

typedef struct _BUFOPERATOR{
	void* obj;
	int max, pos;

	PF_READ_CHAR		pf_read_char;
	PF_READ_ESCAPE		pf_read_escape;

	PF_WRITE_CHAR		pf_write_char;
	PF_WRITE_ESCAPE		pf_write_escape;

	PF_WRITE_TOKEN		pf_write_token;
}BUFOPERATOR;

typedef int(__callback *PF_ON_DOM_NODE_BEGIN)(link_t_ptr nlk, const tchar_t* ns, const tchar_t* nname, void* pv);
typedef int(__callback *PF_ON_DOM_NODE_CLOSE)(link_t_ptr nlk, void* pv);
typedef int(__callback *PF_ON_DOM_NODE_END)(link_t_ptr nlk, bool_t b_close, const tchar_t* ns, const tchar_t* nname, void* pv);
typedef int(__callback *PF_ON_DOM_NODE_XMLNS)(link_t_ptr nlk, const tchar_t* ns, const tchar_t* url, void* pv);
typedef int(__callback *PF_ON_DOM_NODE_ATTR)(link_t_ptr nlk, const tchar_t* key, const tchar_t* val, void* pv);
typedef int(__callback *PF_ON_DOM_NODE_TEXT)(link_t_ptr nlk, bool_t b_cdata, const tchar_t* text, int len, void* pv);

typedef struct _DOMREADER{
	PF_ON_DOM_NODE_BEGIN		pf_on_node_begin;
	PF_ON_DOM_NODE_CLOSE		pf_on_node_close;
	PF_ON_DOM_NODE_XMLNS		pf_on_node_xmlns;
	PF_ON_DOM_NODE_ATTR			pf_on_node_attr;
	PF_ON_DOM_NODE_TEXT			pf_on_node_text;
	PF_ON_DOM_NODE_END			pf_on_node_end;
}DOMREADER;

XDL_API void read_dom_doc(link_t_ptr dom, const DOMREADER* pd, void* pv);

#endif	/*_XDLXML_H*/