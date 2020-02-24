/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc clipboard document

	@module	if_clipboard_win.c | windows implement file

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

#include "xdkiml.h"

#ifdef XDK_SUPPORT_CLIPBOARD

bool_t _clipboard_open()
{
	return (bool_t)OpenClipboard(NULL);
}

void _clipboard_close()
{
	CloseClipboard();
}

void _clipboard_clean()
{
	EmptyClipboard();
}

bool_t _clipboard_put(int fmt, res_glob_t gb)
{
	EmptyClipboard();

	return (SetClipboardData(fmt, (HANDLE)gb)) ? 1 : 0;
}

res_glob_t _clipboard_get(int fmt)
{

	if (!IsClipboardFormatAvailable(fmt))
		return NULL;

	return (res_glob_t)GetClipboardData(fmt);
}

#endif //XDK_SUPPORT_CLIPBOARD