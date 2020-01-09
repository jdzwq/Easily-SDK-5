/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc xdl file function

	@module	xdlfile.c | xdl file implement file

	@devnote 张文权 2003.01 - 2007.12 v3.0
			 张文权 2008.01 - 2009.12 v4.0
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlfile.h"
#include "xdlmem.h"
#include "xdlerr.h"
#include "xdlstr.h"

typedef struct _file_node_t{
	int proto;
#if defined(_WINDOWS)
	HANDLE file;
	HINTERNET sess;
#endif
	size_t size;
	size_t pos;
	void* buf;
}file_node_t;

int parse_proto(const wchar_t* file)
{
	if(!file)
		return -1;

	if(xsnicmp(file,PROTO_HTTP,xslen(PROTO_HTTP)) == 0)
	{
		return _PROTO_HTTP;
	}else if(xsnicmp(file,PROTO_HTTP,xslen(PROTO_FTP)) == 0)
	{
		return _PROTO_FTP;
	}else if(xsnicmp(file,PROTO_NFS,xslen(PROTO_NFS)) == 0)
	{
		return _PROTO_NFS;
	}else
	{
		if(xsstr(file,L":"))
			return _PROTO_LOC;
		else
			return _PROTO_CUR;
	}
}

handle_t _inet_open_file(const wchar_t* file,const wchar_t* mode)
{
	file_node_t* pfn;
	int proto;	

#if defined(_WINDOWS)
	HINTERNET hSess,hFile;
#endif

	proto = parse_proto(file);
	if(proto < 0)
		return 0;

#if defined(_WINDOWS)
	hSess = InternetOpen(L"Powresuite",PRE_CONFIG_INTERNET_ACCESS,NULL,NULL,0);
	if(!hSess)
		return 0;
	hFile = InternetOpenUrl((HINTERNET)hSess,file,NULL,0,0,0);
	if(!hFile)
	{
		InternetCloseHandle(hSess);
		return 0;
	}

	if(0 == xsnicmp(mode,L"a+",2) || 0 == xsnicmp(mode,L"a",1))
		InternetSetFilePointer((HINTERNET)hFile,0,NULL,FILE_END,0);
#endif

	pfn = (file_node_t*)xmem_alloc(sizeof(file_node_t));
	pfn->proto = proto;	
#if defined(_WINDOWS)
	pfn->sess = hSess;
	pfn->file = (HANDLE)hFile;
#endif

	return (handle_t)pfn;
}

size_t _inet_file_size(handle_t fh)
{
	file_node_t* pfn;
	size_t total = 0;
#if defined(_WINDOWS)
	DWORD dw;
#endif

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);

#if defined(_WINDOWS)
	while(InternetQueryDataAvailable((HINTERNET)pfn->file,&dw,0,0))
#endif
	{
		if(!dw)
			break;

		if(!pfn->buf)
			pfn->buf = xmem_alloc(dw);
		else
			pfn->buf = xmem_realloc(pfn->buf,total + dw);

#if defined(_WINDOWS)
		if(!InternetReadFile((HINTERNET)pfn->file,(void*)((char*)pfn->buf + total),dw,&dw))
			break;
#endif
		total += dw;
	}
	pfn->size = total;
	
	return pfn->size;
}

size_t _inet_read_file(handle_t fh,void* buf,size_t size)
{
	file_node_t* pfn;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);
	assert(buf);

	if(!pfn->buf || pfn->size == pfn->pos)
		return 0;

	if(size > pfn->size - pfn->pos)
		size = pfn->size - pfn->pos;

	memcpy(buf,pfn->buf,size);
	pfn->pos += size;
	if(pfn->pos == pfn->size)
	{
		xmem_free(pfn->buf);
		pfn->buf = NULL;
	}
	
	return size;
}

size_t _inet_write_file(handle_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
#if defined(_WINDOWS)
	DWORD dw = 0;
#endif

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);
	assert(buf);

#if defined(_WINDOWS)
	if(!InternetWriteFile((HINTERNET)pfn->file,buf,(DWORD)size,&dw))
	{
		return 0;
	}
#endif

	return dw;
}

void _inet_close_file(handle_t fh)
{
	file_node_t* pfn;

	assert(fh);
	pfn = (file_node_t*)fh;

	if(pfn->buf)
		xmem_free(pfn->buf);

#if defined(_WINDOWS)
	if(pfn->file)
		InternetCloseHandle((HINTERNET)pfn->file);
	if(pfn->sess)
		InternetCloseHandle((HINTERNET)pfn->sess);
#endif

	xmem_free(pfn);
}

bool_t _inet_find_first_file(const wchar_t* fname,find_data_t* pxf)
{
#if defined(_WINDOWS)
	WIN32_FIND_DATAW wfd = {0};
	SYSTEMTIME stm = {0};

	pxf->h_find = (handle_t)FtpFindFirstFileW(pxf->h_conn,fname,&wfd,INTERNET_FLAG_DONT_CACHE,0);
	if(!pxf->h_find)
	{
		return 0;
	}
	
	pxf->mask = (_PROTO_HTTP << 16);
	pxf->mask |= ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)? 1 : 0);
	pxf->low_size = wfd.nFileSizeLow;
	pxf->high_size = wfd.nFileSizeHigh;
	FileTimeToSystemTime(&wfd.ftCreationTime,&stm);
	xsprintf(pxf->create_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);	
	FileTimeToSystemTime(&wfd.ftLastAccessTime,&stm);
	xsprintf(pxf->access_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	FileTimeToSystemTime(&wfd.ftLastWriteTime,&stm);
	xsprintf(pxf->write_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	xscpy(pxf->long_name,wfd.cFileName);
	//xscpy(pxf->short_name,wfd.cAlternateFileName);
#endif

	return 1;
}

bool_t _inet_find_next_file(find_data_t* pxf)
{
#if defined(_WINDOWS)
	WIN32_FIND_DATAW wfd = {0};
	SYSTEMTIME stm = {0};

	if(!InternetFindNextFile((HINTERNET)pxf->h_find,(void*)&wfd))
	{
		return 0;
	}

	pxf->mask |= ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)? 1 : 0);
	pxf->low_size = wfd.nFileSizeLow;
	pxf->high_size = wfd.nFileSizeHigh;
	FileTimeToSystemTime(&wfd.ftCreationTime,&stm);
	xsprintf(pxf->create_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);	
	FileTimeToSystemTime(&wfd.ftLastAccessTime,&stm);
	xsprintf(pxf->access_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	FileTimeToSystemTime(&wfd.ftLastWriteTime,&stm);
	xsprintf(pxf->write_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	xscpy(pxf->long_name,wfd.cFileName);
	//xscpy(pxf->short_name,wfd.cAlternateFileName);
#endif

	return 1;
}

void _inet_find_close(find_data_t* pxf)
{
#if defined(_WINDOWS)
	FindClose(pxf->h_find);
#endif
	memset((void*)pxf,0,sizeof(find_data_t));
}

/****************************************************************************************************/

handle_t _unc_open_file(const wchar_t* file,const wchar_t* mode)
{
	file_node_t* pfn;
	HANDLE hFile = 0;
	DWORD dwAccess,dwCreate;
	BY_HANDLE_FILE_INFORMATION fi = {0};
	
	if(0 == xscmp(mode,L"r"))
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_READ;
	}else if(0 == xscmp(mode,L"w"))
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_WRITE;
	}else if(0 == xscmp(mode,L"a"))
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_WRITE;
	}else if(0 == xscmp(mode,L"r+"))
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}else if(0 == xscmp(mode,L"w+"))
	{
		dwCreate = CREATE_ALWAYS;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}else if(0 == xscmp(mode,L"a+"))
	{
		dwCreate = OPEN_ALWAYS;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}else
	{
		return NULL;
	}

	hFile = CreateFileW(file,dwAccess,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,dwCreate,FILE_ATTRIBUTE_NORMAL,NULL);
	if((HANDLE)hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	
	if(0 == xscmp(mode,L"a+") || 0 == xscmp(mode,L"a"))
		SetFilePointer(hFile,0,NULL,FILE_END);

	pfn = (file_node_t*)xmem_alloc(sizeof(file_node_t));
	pfn->file = (HANDLE)hFile;
	pfn->proto = _PROTO_LOC;

	if(GetFileInformationByHandle((HANDLE)pfn->file,&fi))
		pfn->size = fi.nFileSizeLow;
	else
		pfn->size = 0;

	return (handle_t)pfn;
}

size_t	_unc_read_file(handle_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
	DWORD dw = 0;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);

	if(!ReadFile((HANDLE)pfn->file,buf,(DWORD)size,&dw,NULL))
		return 0;
	else
		return dw;
}

size_t _unc_write_file(handle_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
	DWORD dw = 0;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);

	if(!WriteFile((HANDLE)pfn->file,buf,(DWORD)size,&dw,NULL))
		return 0;
	else
		return dw;
}

void _unc_close_file(handle_t fh)
{
	file_node_t* pfn;
	DWORD dw = 0;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);

	CloseHandle((HANDLE)pfn->file);
	xmem_free(pfn);
}

size_t _unc_file_size(handle_t fh)
{
	file_node_t* pfn;
	size_t size;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);
	
	size = pfn->size;
	pfn->size = 0;
	return size;
}

bool_t _unc_find_first_file(const wchar_t* fname,find_data_t* pxf)
{
	WIN32_FIND_DATAW wfd;
	SYSTEMTIME stm;

	pxf->h_find = FindFirstFileW(fname,&wfd);
	if(pxf->h_find == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	pxf->mask = (_PROTO_LOC << 16);
	pxf->mask |= ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)? 1 : 0);
	pxf->low_size = wfd.nFileSizeLow;
	pxf->high_size = wfd.nFileSizeHigh;
	FileTimeToSystemTime(&wfd.ftCreationTime,&stm);
	xsprintf(pxf->create_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);	
	FileTimeToSystemTime(&wfd.ftLastAccessTime,&stm);
	xsprintf(pxf->access_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	FileTimeToSystemTime(&wfd.ftLastWriteTime,&stm);
	xsprintf(pxf->write_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	xscpy(pxf->long_name,wfd.cFileName);
	//xscpy(pxf->short_name,wfd.cAlternateFileName);

	return 1;
}

bool_t _unc_find_next_file(find_data_t* pxf)
{
	WIN32_FIND_DATAW wfd;
	SYSTEMTIME stm;

	if(!FindNextFileW(pxf->h_find,&wfd))
	{
		return 0;
	}

	pxf->mask &= 0xFFFF0000;
	pxf->mask |= ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)? 1 : 0);
	pxf->low_size = wfd.nFileSizeLow;
	pxf->high_size = wfd.nFileSizeHigh;
	FileTimeToSystemTime(&wfd.ftCreationTime,&stm);
	xsprintf(pxf->create_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);	
	FileTimeToSystemTime(&wfd.ftLastAccessTime,&stm);
	xsprintf(pxf->access_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	FileTimeToSystemTime(&wfd.ftLastWriteTime,&stm);
	xsprintf(pxf->write_time,L"%d-%02d-%02d %02d:%02d:%02d",stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
	xscpy(pxf->long_name,wfd.cFileName);
	//xscpy(pxf->short_name,wfd.cAlternateFileName);

	return 1;
}

void _unc_find_close(find_data_t* pxf)
{
	FindClose(pxf->h_find);
	memset((void*)pxf,0,sizeof(find_data_t));
}

handle_t xfile_open(const wchar_t* file,const wchar_t* mode)
{
	int proto;

	proto = parse_proto(file);
	if(proto < 0)
		return 0;

	if(proto == _PROTO_HTTP || proto == _PROTO_FTP)
	{
		return _inet_open_file(file,mode);
	}else
	{
		return _unc_open_file(file,mode);
	}
}

size_t	xfile_read(handle_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	if(pfn->proto == _PROTO_HTTP)
		return _inet_read_file(fh,buf,size);
	else
		return _unc_read_file(fh,buf,size);

}

size_t xfile_write(handle_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	if(pfn->proto == _PROTO_HTTP || pfn->proto == _PROTO_FTP)
		return _inet_write_file(fh,buf,size);
	else
		return _unc_write_file(fh,buf,size);
}

void xfile_close(handle_t fh)
{
	file_node_t* pfn;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	if(pfn->proto == _PROTO_HTTP || pfn->proto == _PROTO_FTP)
		_inet_close_file(fh);
	else
		_unc_close_file(fh);
}

size_t xfile_size(handle_t fh)
{
	file_node_t* pfn;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	if(pfn->proto == _PROTO_HTTP || pfn->proto == _PROTO_FTP)
		return _inet_file_size(fh);
	else
		return _unc_file_size(fh);
}

bool_t xfile_find_first(const wchar_t* fname,find_data_t* pxf)
{
	int proto;
	proto = parse_proto(fname);
	if(proto < 0)
		return 0;

	pxf->proto = proto;

	if(pxf->proto == _PROTO_HTTP || pxf->proto == _PROTO_FTP)
		return _inet_find_first_file(fname,pxf);
	else
		return _unc_find_first_file(fname,pxf);
}

bool_t xfile_find_next(find_data_t* pxf)
{
	if(pxf->proto == _PROTO_HTTP || pxf->proto == _PROTO_FTP)
		return _inet_find_next_file(pxf);
	else
		return _unc_find_next_file(pxf);
}

void xfile_find_close(find_data_t* pxf)
{
	if(pxf->proto == _PROTO_HTTP || pxf->proto == _PROTO_FTP)
		_inet_find_close(pxf);
	else
		_unc_find_close(pxf);
}