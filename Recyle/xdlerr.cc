/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlmem.h"
#include "xdlerr.h"

void get_sys_error(int code,wchar_t* buf,int max)
{
	void *token;	
	
	assert(buf && max > 0);

#if defined(_WINDOWS)
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,(DWORD)code,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPWSTR)&token,0,NULL);
#endif

	memset(buf,0,(max + 1) * sizeof(wchar_t));
	memcpy((void*)buf,token,max * sizeof(wchar_t));

#if defined(_WINDOWS)
	LocalFree(token);
#endif
}

int get_last_error(wchar_t* tmp,int max)
{
	int errcode;

#if defined(_WINDOWS)
	errcode = (int)GetLastError();
#endif

	get_sys_error((DWORD)errcode,tmp,max);
	return errcode;
}

void dump_error(const wchar_t* str)
{
}
