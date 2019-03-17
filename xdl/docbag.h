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

/*
@FUNCTION save_xml_doc_to_file: save a xml document to file.
@INPUT link_t_ptr xml: the xml document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	save_xml_doc_to_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_xml_doc_from_file: load a xml document from file.
@INOUTPUT link_t_ptr xml: the xml document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	load_xml_doc_from_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_dom_doc_to_file: save a dom document to file.
@INPUT link_t_ptr dom: the dom document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	save_dom_doc_to_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_dom_doc_from_file: load a dom document from file.
@INOUTPUT link_t_ptr dom: the dom document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	load_dom_doc_from_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_form_doc_from_file: create a form document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the form link component, fails return NULL.
*/
XDL_API link_t_ptr	create_form_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_form_doc_to_file: save a form document to meta file.
@INPUT link_t_ptr ptr: the form document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_form_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_grid_doc_from_file: create a grid document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the grid link component, fails return NULL.
*/
XDL_API link_t_ptr	create_grid_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_grid_doc_to_file: save a grid document to meta file.
@INPUT link_t_ptr ptr: the grid document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_grid_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_graph_doc_from_file: create a graphic document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the graphic link component, fails return NULL.
*/
XDL_API link_t_ptr	create_graph_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_graph_doc_to_file: save a graphic document to meta file.
@INPUT link_t_ptr ptr: the graphic document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_graph_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_dialog_doc_from_file: create a dialog document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the dialog link component, fails return NULL.
*/
XDL_API link_t_ptr	create_dialog_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_dialog_doc_to_file: save a dialog document to meta file.
@INPUT link_t_ptr ptr: the dialog document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_dialog_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_chart_doc_from_file: create a chart document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the chart link component, fails return NULL.
*/
XDL_API link_t_ptr	create_chart_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_chart_doc_to_file: save a chart document to meta file.
@INPUT link_t_ptr ptr: the chart document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_chart_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_topog_doc_from_file: create a topog document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the topog link component, fails return NULL.
*/
XDL_API link_t_ptr	create_topog_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_topog_doc_to_file: save a topog document to meta file.
@INPUT link_t_ptr ptr: the topog document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_topog_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_rich_doc_from_file: create a rich document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the rich link component, fails return NULL.
*/
XDL_API link_t_ptr	create_rich_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_rich_doc_to_file: save a rich document to meta file.
@INPUT link_t_ptr ptr: the rich document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_rich_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_schema_doc_from_file: create a schema document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the schema link component, fails return NULL.
*/
XDL_API link_t_ptr	create_schema_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_schema_doc_to_file: save a schema document to meta file.
@INPUT link_t_ptr ptr: the schema document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_schema_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION create_images_doc_from_file: create a images document and load content from meta file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the images link component, fails return NULL.
*/
XDL_API link_t_ptr	create_images_doc_from_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_images_doc_to_file: save a images document to meta file.
@INPUT link_t_ptr ptr: the images document.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_images_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_field_object_from_file: load a form embed field document from meta file.
@INPUT link_t_ptr flk: the embed field link component.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t load_field_object_from_file(link_t_ptr flk, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_grid_doc_from_csv_file: load a grid content from csv file.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN link_t_ptr: if succeeds return the grid link component, fails return NULL.
*/
XDL_API bool_t load_grid_doc_from_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_grid_doc_to_csv_file: save a grid document to csv file.
@INPUT link_t_ptr ptr: the grid document.
@INPUT bool_t head: nonzero for saving content with col header.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_grid_doc_to_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION parse_proper_doc_from_ini: parse a proper content from ini content.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* ini: the ini content string buffer.
@INPUT int len: the ini content length in characters.
@RETURN void: non.
*/
XDL_API void parse_proper_doc_from_ini(link_t_ptr ptr, const tchar_t* ini, int len);

/*
@FUNCTION format_proper_doc_to_ini: format proper content to ini content.
@INPUT link_t_ptr ptr: the proper link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_proper_doc_to_ini(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION load_proper_doc_from_ini_file: load a proper content from ini file.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t load_proper_doc_from_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_proper_doc_to_ini_file: save a proper document to csv file.
@INPUT link_t_ptr ptr: the proper document.
@INPUT bool_t head: nonzero for saving content with col header.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_proper_doc_to_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION parse_proper_doc_from_style: parse a proper content from style content.
@INPUT link_t_ptr ptr: the proper link component.
@INPUT const tchar_t* str: the style content string buffer.
@INPUT int len: the style content length in characters.
@RETURN void: non.
*/
XDL_API void parse_proper_doc_from_style(link_t_ptr ptr, const tchar_t* str);

/*
@FUNCTION format_proper_doc_to_style: format proper content to style content.
@INPUT link_t_ptr ptr: the proper link component.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int format_proper_doc_to_style(link_t_ptr ptr, tchar_t* buf, int len);

/*
@FUNCTION load_text_from_file: load a text content from binary file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@OUTPUT tchar_t* buf: the text buffer.
@INPUT int max: the text buffer size in characters.
@RETURN int: if succeeds return the text size in characters, fails return C_ERR(-1).
*/
XDL_API int load_text_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* buf, int max);

/*
@FUNCTION save_text_from_file: save a text content to binary file using default encoding.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@INPUT const tchar_t* buf: the text buffer.
@INPUT int len: the text buffer size in characters.
@RETURN int: if succeeds return the text size in characters, fails return C_ERR(-1).
*/
XDL_API int save_text_from_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* buf, int len);

/*
@FUNCTION load_memo_doc_from_file: load a meta document from file.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t load_memo_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_memo_doc_to_file: save a meta document from file.
@INPUT link_t_ptr ptr: the meta link component.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_memo_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_tag_doc_from_file: load a tag document from file.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t load_tag_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION save_tag_doc_to_file: save a tag document from file.
@INPUT link_t_ptr ptr: the tag link component.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the destination file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_tag_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*DOCBAG_H*/