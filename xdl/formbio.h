/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc form bio document

	@module	formbio.h | interface file

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

#ifndef _FORMBIO_H
#define _FORMBIO_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDL_SUPPORT_BIO)


#ifdef	__cplusplus
extern "C" {
#endif

	/*
	@FUNCTION load_field_object_from_file: load a form embed field document from meta file.
	@INPUT link_t_ptr flk: the embed field link component.
	@INPUT const secu_desc_t* psd: the file security struct.
	@INPUT const tchar_t* fname: the destination file path name.
	@RETURN bool_t: if succeeds return nonzero, fails return zero.
	*/
	XDL_API bool_t load_field_object_from_file(link_t_ptr flk, const secu_desc_t* psd, const tchar_t* fname);


#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*FORMBIO_H*/