/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl process document

	@module	impmodu.h | xdl module interface file

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

#ifndef _IMPMODU_H
#define _IMPMODU_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_PROCESS

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API res_modu_t load_library(const tchar_t* lname);

XDL_API void free_library(res_modu_t lib);

XDL_API void* get_address(res_modu_t lib, const schar_t* fname);

XDL_API void get_runpath(res_modu_t ins, tchar_t* buf, int max);

XDL_API bool_t create_process(const tchar_t* exename, const tchar_t* cmdline, int share, proc_info_t* ppi);

XDL_API void release_process(proc_info_t* ppi);

XDL_API void process_waitrun(res_hand_t ph);

XDL_API res_hand_t process_dupli(res_hand_t ph, res_hand_t vh);

XDL_API void* process_alloc(res_hand_t ph, dword_t dw);

XDL_API void process_free(res_hand_t ph, void* p);

XDL_API bool_t process_write(res_hand_t ph, void* p, void* data, dword_t dw);

XDL_API bool_t process_read(res_hand_t ph, void* p, void* data, dword_t dw);

XDL_API void release_handle(res_hand_t hh);

XDL_API bool_t inherit_handle(res_hand_t hh, bool_t b);

XDL_API void read_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, tchar_t* buf, int max);

XDL_API void write_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, const tchar_t* val);

XDL_API void system_info(sys_info_t* psi);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_PROCESS*/

#endif /*_IMPMODU_H*/