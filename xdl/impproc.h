/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl process document

	@module	impmodu.h | interface file

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

/*
@FUNCTION load_library: load dynamic library.
@INPUT const tchar_t* lname: the library file path and name.
@RETURN res_modu_t: if succeeds return dynamic library system resource handle, fails return NULL.
*/
XDL_API res_modu_t load_library(const tchar_t* lname);

/*
@FUNCTION free_library: free dynamic library.
@INPUT res_modu_t lib: the library system resource handle.
@RETURN void: none.
*/
XDL_API void free_library(res_modu_t lib);

/*
@FUNCTION get_address: get dynamic library exported function address.
@INPUT res_modu_t lib: the library system resource handle.
@INPUT const schar_t* fname: the function name.
@RETURN void*: if succeeds return function address, falis return NULL.
*/
XDL_API void* get_address(res_modu_t lib, const schar_t* fname);

/*
@FUNCTION get_runpath: get the module running path.
@INPUT res_modu_t ins: the module system resource handle.
@OUTPUT tchar_t* buf: the string buffer for return path.
@INPUT int max: the buffer max length in characters, not include the string terminate character space.
@RETURN void: none.
*/
XDL_API void get_runpath(res_modu_t ins, tchar_t* buf, int max);

/*
@FUNCTION create_process: create child process with command line and share mode, if child process created with share mode, 
it can use pipe or socket handle returned in proc_info_t struct write data to or read data from child process.
@INPUT const tchar_t* exename: the module path and name.
@INPUT const tchar_t* cmdline: the string parameter translated to child process.
@INPUT int share: the interactive mode between parent and child process, can be SHARE_NONE, SHARE_PIPE, SHARE_SOCK.
@OUTPUT proc_info_t* ppi: the process information struct for returning.
@RETURN bool_t: if succeeds fill the ppi struct and return 1, fails return 0.
*/
XDL_API bool_t create_process(const tchar_t* exename, const tchar_t* cmdline, int share, proc_info_t* ppi);

/*
@FUNCTION release_process: release system resource handle stored in proc_info_t struct.
@INPUT proc_info_t ppi: the process information struct.
@RETURN void: none.
*/
XDL_API void release_process(proc_info_t* ppi);

/*
@FUNCTION process_waitrun: pause process executing till the system resource handle released.
@INPUT res_proc_t ph: some system resource handle.
@RETURN void: none.
*/
XDL_API void process_waitrun(res_proc_t ph);

/*
@FUNCTION process_dupli: duplicate a system resource handle to target process.
@INPUT res_proc_t ph: current process handle.
@INPUT res_file_t fh: current process system resource handle.
@RETURN res_file_t: if succeeds return target process resource handle, fails return NULL.
*/
XDL_API res_file_t process_dupli(res_proc_t ph, res_file_t fh);

/*
@FUNCTION process_alloc: alloc memory address from target process heap.
@INPUT res_proc_t ph: current process handle.
@INPUT dword_t dw: the memory size request.
@RETURN void*: if succeeds return memory pointer in target process heap, fails return NULL.
*/
XDL_API void* process_alloc(res_proc_t ph, dword_t dw);

/*
@FUNCTION process_free: free memory in target process heap.
@INPUT res_proc_t ph: current process handle.
@INPUT void* p: the memory pointer from target process heap.
@RETURN void: none.
*/
XDL_API void process_free(res_proc_t ph, void* p);

/*
@FUNCTION process_write: write date into target process memory.
@INPUT res_proc_t ph: current process handle.
@INPUT void* p: the memory pointer from target process heap.
@INPUT void* data: the data buffer pointer.
@INPUT dword_t dw: size in bytes of data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t process_write(res_proc_t ph, void* p, void* data, dword_t dw);

/*
@FUNCTION process_read: read date from target process memory.
@INPUT res_proc_t ph: current process handle.
@INPUT void* p: the memory pointer from target process heap.
@INPUT void* data: the data buffer pointer.
@OUTPUT dword_t dw: the max size in bytes of buffer.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t process_read(res_proc_t ph, void* p, void* data, dword_t dw);

/*
@FUNCTION release_handle: release system resource handle.
@INPUT res_file_t hh: some syste resource handle.
@RETURN void: none.
*/
XDL_API void release_handle(res_file_t hh);

/*
@FUNCTION inherit_handle: make system resource can be or not be inherited by child process.
@INPUT res_file_t hh: some syste resource handle.
@INPUT bool_t b: if nonzero the resource handle can be inherited, otherwise can not be.
@RETURN bool_t: none.
*/
XDL_API bool_t inherit_handle(res_file_t hh, bool_t b);

/*
@FUNCTION read_profile: read some runtime config value from profile.
@INPUT const tchar_t* fname: profile file name.
@INPUT const tchar_t*sec: profile section name.
@INPUT const tchar_t* key: profile key name.
@OUTPUT tchar_t* buf: string buffer for returning key value.
@INPUT intmax: string buffer max length, not include terminated character space.
@RETURN void: none.
*/
XDL_API void read_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, tchar_t* buf, int max);

/*
@FUNCTION write_profile: write some runtime config value into profile.
@INPUT const tchar_t* fname: profile file name.
@INPUT const tchar_t* sec: profile section name.
@INPUT const tchar_t* key: profile key name.
@INPUT const tchar_t* val: profile key value.
@RETURN void: none.
*/
XDL_API void write_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, const tchar_t* val);

/*
@FUNCTION get_envvar: get environment variable.
@INPUT const tchar_t* ename: environment name.
@OUTPUT tchar_t* buf: string buffer for returning variable value.
@INPUT int max: string buffer max length, not include terminated character space.
@RETURN int: return zero if not find, otherwise return bytes copyed.
*/
XDL_API int get_envvar(const tchar_t* ename, tchar_t* buf, int max);

/*
@FUNCTION system_info: get system some environment value.
@OUTPUT sys_info_t* psi: system information struct pointer for returning.
@RETURN void: none.
*/
XDL_API void system_info(sys_info_t* psi);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_PROCESS*/

#endif /*_IMPMODU_H*/