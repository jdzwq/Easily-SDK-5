/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc document utility

	@module	metabio.h | interface file

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

#ifndef _METABIO_H
#define _METABIO_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_BIO)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION save_xml_doc_to_file: save a xml document to file.
@INPUT link_t_ptr xml: the xml document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t	save_xml_doc_to_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_xml_doc_from_file: load a xml document from file.
@INOUTPUT link_t_ptr xml: the xml document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t	load_xml_doc_from_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_dom_doc_to_file: save a dom document to file.
@INPUT link_t_ptr dom: the dom document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t	save_dom_doc_to_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_dom_doc_from_file: load a dom document from file.
@INOUTPUT link_t_ptr dom: the dom document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t	load_dom_doc_from_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_schema_doc_from_file: create a schema document from schema xml file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the schema link component, fails return NULL.
*/
EXP_API link_t_ptr	create_schema_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_schema_doc_to_file: save a schema document to schema xml file.
@INPUT link_t_ptr ptr: the schema document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_schema_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_form_from_meta_file: create a form document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the form link component, fails return NULL.
*/
EXP_API link_t_ptr	create_form_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_form_to_meta_file: save a form document to meta file.
@INPUT link_t_ptr ptr: the form document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_form_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_grid_from_meta_file: create a grid document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the grid link component, fails return NULL.
*/
EXP_API link_t_ptr	create_grid_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_grid_to_meta_file: save a grid document to meta file.
@INPUT link_t_ptr ptr: the grid document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_grid_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_statis_from_meta_file: create a statisic document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the statisic link component, fails return NULL.
*/
EXP_API link_t_ptr	create_statis_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_statis_to_meta_file: save a statisic document to meta file.
@INPUT link_t_ptr ptr: the statisic document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_statis_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_dialog_from_meta_file: create a dialog document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the dialog link component, fails return NULL.
*/
EXP_API link_t_ptr	create_dialog_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_dialog_to_meta_file: save a dialog document to meta file.
@INPUT link_t_ptr ptr: the dialog document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_dialog_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_calendar_from_meta_file: create a calendar document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the calendar link component, fails return NULL.
*/
EXP_API link_t_ptr	create_calendar_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_calendar_to_meta_file: save a calendar document to meta file.
@INPUT link_t_ptr ptr: the calendar document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_calendar_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_diagram_from_meta_file: create a diagram document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the diagram link component, fails return NULL.
*/
EXP_API link_t_ptr	create_diagram_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_diagram_to_meta_file: save a diagram document to meta file.
@INPUT link_t_ptr ptr: the diagram document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_diagram_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_topog_from_meta_file: create a topog document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the topog link component, fails return NULL.
*/
EXP_API link_t_ptr	create_topog_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_topog_to_meta_file: save a topog document to meta file.
@INPUT link_t_ptr ptr: the topog document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_topog_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_rich_from_meta_file: create a rich document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the rich link component, fails return NULL.
*/
EXP_API link_t_ptr	create_rich_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_rich_to_meta_file: save a rich document to meta file.
@INPUT link_t_ptr ptr: the rich document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_rich_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_images_from_meta_file: create a images document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the images link component, fails return NULL.
*/
EXP_API link_t_ptr	create_images_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_images_doc_to_file: save a images document to meta file.
@INPUT link_t_ptr ptr: the images document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_images_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_plot_from_meta_file: create a plot document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the plot link component, fails return NULL.
*/
EXP_API link_t_ptr	create_plot_from_meta_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_plot_doc_to_file: save a plot document to meta file.
@INPUT link_t_ptr ptr: the plot document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t save_plot_to_meta_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*DOCMETA_H*/