/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl file document

	@module	xdlfile.h | xdl file interface file

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

#ifndef _XDLFILE_H
#define _XDLFILE_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API file_t		xfile_open(const secu_desc_t* psd, const tchar_t* fname, dword_t fmode);

	XDL_API bool_t		xfile_read(file_t fh, byte_t* buf, dword_t size);

	XDL_API bool_t		xfile_read_range(file_t fh, dword_t hoff, dword_t loff, byte_t* buf, dword_t size);

	XDL_API bool_t		xfile_write_range(file_t fh, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size);

	XDL_API bool_t		xfile_write(file_t fh, const byte_t* buf, dword_t size);

	XDL_API bool_t		xfile_flush(file_t fh);

	XDL_API void		xfile_close(file_t fh);

	XDL_API xhand_t		xfile_bio(file_t fh);

	XDL_API int			xfile_proto(file_t fh);

	XDL_API void		xfile_settime(file_t fh, const tchar_t* ftime);

	XDL_API void		xfile_setsince(file_t fh, int fs);

	XDL_API bool_t		xfile_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode);

	XDL_API bool_t		xfile_copy(const secu_desc_t* psd, const tchar_t* srcfile, const tchar_t* destfile, int fsince);

	XDL_API bool_t		xfile_delete(const secu_desc_t* psd, const tchar_t* fname);

	XDL_API bool_t		xfile_rename(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* nname);

	XDL_API bool_t		xfile_mkdir(const secu_desc_t* psd, const tchar_t* path);

	XDL_API bool_t		xfile_rmdir(const secu_desc_t* psd, const tchar_t* path);

#ifdef	__cplusplus
}
#endif


#endif /*XDLFILE_H*/
