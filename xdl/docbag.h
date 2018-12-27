/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc document utility

	@module	docbag.h | bag for document interface file

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

#ifndef _DOCBAG_H
#define _DOCBAG_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDK_SUPPORT_FILE)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API bool_t	save_xml_doc_to_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t	load_xml_doc_from_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t	save_dom_doc_to_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t	load_dom_doc_from_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_form_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_form_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_grid_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_grid_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_graph_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_graph_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_dialog_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_dialog_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_chart_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_chart_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_topog_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_topog_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_rich_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_rich_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_schema_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_schema_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API link_t_ptr	create_images_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_images_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t load_field_object_from_file(link_t_ptr flk, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t load_grid_doc_from_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_grid_doc_to_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname);

XDL_API void parse_proper_doc_from_ini(link_t_ptr ptr, const tchar_t* ini, int len);

XDL_API int format_proper_doc_to_ini(link_t_ptr ptr, tchar_t* buf, int max);

XDL_API bool_t load_proper_doc_from_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_proper_doc_to_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API void parse_proper_doc_from_style(link_t_ptr ptr, const tchar_t* str);

XDL_API int format_proper_doc_to_style(link_t_ptr ptr, tchar_t* buf, int len);

XDL_API int load_text_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* buf, int max);

XDL_API int save_text_from_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* buf, int len);

XDL_API bool_t load_memo_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_memo_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t load_tag_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

XDL_API bool_t save_tag_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*DOCBAG_H*/