/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml operator document

	@module	xmlopera.h | interface file

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

#ifndef XMLOPERA_H
#define XMLOPERA_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

typedef struct _XMLOBJECT{
	link_t_ptr doc;
	link_t_ptr nlk;

	link_t_ptr stack;
}XMLOBJECT;

#ifdef	__cplusplus
extern "C" {
#endif

	void call_write_xml_begin(void* p_obj);

	bool_t call_write_xml_head_attr(void* p_obj, const tchar_t* key, int klen, const tchar_t* val, int vlen);

	bool_t call_write_xml_node_begin(void* p_obj);

	bool_t call_write_xml_node_retain(void* pv);

	bool_t call_write_xml_node_end(void* p_obj);

	bool_t call_write_xml_node_name(void* p_obj, const tchar_t* ns, int nslen, const tchar_t* na, int nalen);

	bool_t call_write_xml_node_attr(void* p_obj, const tchar_t* key, int klen, const tchar_t* val, int vlen);

	bool_t call_write_xml_node_xmlns(void* p_obj, const tchar_t* key, int klen, const tchar_t* val, int vlen);

	bool_t call_write_xml_node_text(void* p_obj, bool_t b_cdata, const tchar_t* text, int len);

	bool_t call_write_xml_node_mask(void* pv, dword_t mask, bool_t b_check);

	link_t_ptr call_peek_xml_node(void* pv);

	bool_t call_peek_xml_node_mask(void* pv, dword_t mask);

	const tchar_t* call_peek_xml_node_name(void* pv);

	void call_write_xml_end(void* p_obj, int code);

	bool_t call_write_xml_has_node(void* p_obj);


	bool_t call_read_xml_head_begin(void* pv);

	bool_t call_read_xml_head_attr(void* pv, const tchar_t* key, const tchar_t* val);

	bool_t call_read_xml_head_end(void* pv);

	bool_t call_read_xml_node_attr(void* pv, const tchar_t* key, const tchar_t* val);

	bool_t call_read_xml_node_begin(void* pv, int indent, bool_t b_parent, const tchar_t* ns, const tchar_t* nn);

	bool_t call_read_xml_node_close(void* pv, int indent, bool_t b_parent);

	bool_t call_read_xml_node_text(void* pv, bool_t b_cdata, const tchar_t* text, int len);

	bool_t call_read_xml_node_end(void* pv, int indent, bool_t b_parent, bool_t b_close, const tchar_t* ns, const tchar_t* nn);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*XMLOPERA_H*/
