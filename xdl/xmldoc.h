/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml document

	@module	xmldoc.h | xml document interface file

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

#ifndef _XMLDOC_H
#define _XMLDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API int get_xml_encode(link_t_ptr ptr);

	XDL_API void set_xml_encode(link_t_ptr ptr, int encode);

	XDL_API int get_xml_encoding(link_t_ptr ptr, tchar_t* buf, int max);

	XDL_API void set_xml_encoding(link_t_ptr ptr, const tchar_t* token, int len);

	XDL_API int get_xml_version(link_t_ptr ptr, tchar_t* buf, int max);

	XDL_API void set_xml_version(link_t_ptr ptr, const tchar_t* token, int len);

	XDL_API link_t_ptr create_xml_doc(void);

	XDL_API void destroy_xml_doc(link_t_ptr ptr);

	XDL_API void clear_xml_doc(link_t_ptr ptr);

	XDL_API link_t_ptr get_xml_attr_table(link_t_ptr ptr);

	XDL_API link_t_ptr get_xml_dom_node(link_t_ptr ptr);

	XDL_API link_t_ptr upcast_dom_to_xml(link_t_ptr doc);

	XDL_API link_t_ptr downcast_xml_to_dom(link_t_ptr xml);


#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif	/*_XMLDOC_H*/