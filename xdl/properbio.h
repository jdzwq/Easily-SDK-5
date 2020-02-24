/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc proper bio document

	@module	properbio.h | interface file

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

#ifndef _PROPERBIO_H
#define _PROPERBIO_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDL_SUPPORT_BIO)


#ifdef	__cplusplus
extern "C" {
#endif

	/*
	@FUNCTION parse_proper_doc_from_ini: parse a proper content from ini content.
	@INPUT link_t_ptr ptr: the proper link component.
	@INPUT const tchar_t* ini: the ini content string buffer.
	@INPUT int len: the ini content length in characters.
	@RETURN void: non.
	*/
	XDL_API void parse_proper_from_ini(link_t_ptr ptr, const tchar_t* ini, int len);

	/*
	@FUNCTION format_proper_doc_to_ini: format proper content to ini content.
	@INPUT link_t_ptr ptr: the proper link component.
	@OUTPUT tchar_t* buf: the string buffer.
	@INPUT int max: the string buffer size in characters.
	@RETURN int: return the characters formated.
	*/
	XDL_API int format_proper_to_ini(link_t_ptr ptr, tchar_t* buf, int max);

	/*
	@FUNCTION load_proper_doc_from_ini_file: load a proper content from ini file.
	@INPUT link_t_ptr ptr: the proper link component.
	@INPUT const secu_desc_t* psd: the file security struct.
	@INPUT const tchar_t* fname: the destination file path name.
	@RETURN bool_t: if succeeds return nonzero, fails return zero.
	*/
	XDL_API bool_t load_proper_from_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

	/*
	@FUNCTION save_proper_doc_to_ini_file: save a proper document to csv file.
	@INPUT link_t_ptr ptr: the proper document.
	@INPUT bool_t head: nonzero for saving content with col header.
	@INPUT const secu_desc_t* psd: the file security struct.
	@INPUT const tchar_t* fname: the destination file path name.
	@RETURN bool_t: if succeeds return nonzero, fails return zero.
	*/
	XDL_API bool_t save_proper_to_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

	/*
	@FUNCTION parse_proper_doc_from_style: parse a proper content from style content.
	@INPUT link_t_ptr ptr: the proper link component.
	@INPUT const tchar_t* str: the style content string buffer.
	@INPUT int len: the style content length in characters.
	@RETURN void: non.
	*/
	XDL_API void parse_proper_from_style(link_t_ptr ptr, const tchar_t* str);

	/*
	@FUNCTION format_proper_doc_to_style: format proper content to style content.
	@INPUT link_t_ptr ptr: the proper link component.
	@OUTPUT tchar_t* buf: the string buffer.
	@INPUT int max: the string buffer size in characters.
	@RETURN int: return the characters formated.
	*/
	XDL_API int format_proper_to_style(link_t_ptr ptr, tchar_t* buf, int len);


#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*GRIDBIO_H*/