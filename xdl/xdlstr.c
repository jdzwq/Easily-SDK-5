/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc string document

	@module	xdlstr.c | implement file

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

#include "xdlstr.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

schar_t* a_xsalloc(int len)
{
	return (schar_t*)xmem_alloc(len * sizeof(schar_t));
}

wchar_t* w_xsalloc(int len)
{
	return (wchar_t*)xmem_alloc(len * sizeof(wchar_t));
}

schar_t* a_xsrealloc(schar_t* str,int len)
{
	return (schar_t*)xmem_realloc(str, len * sizeof(schar_t));
}

wchar_t* w_xsrealloc(wchar_t* str,int len)
{
	return (wchar_t*)xmem_realloc(str, len * sizeof(wchar_t));
}

void a_xsfree(schar_t* str)
{
	xmem_free(str);
}

void w_xsfree(wchar_t* str)
{
	xmem_free(str);
}

schar_t* a_xsnclone(const schar_t* str, int len)
{
	schar_t* token;

	if (a_is_null(str))
		return NULL;

	token = a_xsalloc(len + 1);

	xmem_copy((void*)token, (void*)str, len * sizeof(schar_t));
	return token;
}

wchar_t* w_xsnclone(const wchar_t* str, int len)
{
	wchar_t* token;

	if (w_is_null(str))
		return NULL;

	token = w_xsalloc(len + 1);

	xmem_copy((void*)token, (void*)str, len * sizeof(wchar_t));
	return token;
}

schar_t* a_xsclone(const schar_t* str)
{
	return a_xsnclone(str, a_xslen(str));
}

wchar_t* w_xsclone(const wchar_t* str)
{
	return w_xsnclone(str, w_xslen(str));
}

