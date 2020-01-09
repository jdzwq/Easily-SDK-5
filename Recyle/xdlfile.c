/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc file document

	@module	xdlfile.c | xdl file implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#include "xdlfile.h"
#include "xdlutil.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlstm.h"
#include "xdlcode.h"
#include "xdlxml.h"
#include "xmldoc.h"
#include "domdoc.h"
#include "filelist.h"
#include "xdlinet.h"
#include "oemmd5.h"

typedef struct _file_node_t{
	dword_t tag;		//reserved for xhand_t

	int proto;
	int bom;

	void* file;
}file_node_t;

xhand_t _inet_open_file(const wchar_t* file,const wchar_t* mode)
{
	file_node_t* pfn;
	xhand_t fh;
	int proto;

	proto = parse_proto(file);

	if (xsnicmp(mode, L"w",1) == 0)
		fh = xhttp_open(NULL, L"PUT", file);
	else
		fh = xhttp_open(NULL, L"GET", file);

	if(!fh)
		return 0;

	pfn = (file_node_t*)xmem_alloc(sizeof(file_node_t));
	pfn->proto = proto;	
	pfn->file = fh;

	return (xhand_t)pfn;
}

size_t _inet_read_file(xhand_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
	wchar_t gmt[50] = { 0 };
	wchar_t host[1024] = { 0 };
	wchar_t port[10] = { 0 };
	wchar_t ctlen[NUM_LEN + 1] = { 0 };
	byte_t* err;
	int rt,len;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);
	assert(buf);

	get_gmt_time(gmt);

	xhttp_get_url_host((xhand_t)pfn->file, host, 1024);
	xhttp_get_url_port((xhand_t)pfn->file, port, 10);

	if (!is_null(port) && xstos(port) != 80)
	{
		xscat(host, L":");
		xscat(host, port);
	}

	xhttp_set_request_header((xhand_t)pfn->file, L"Host", -1, host, -1);
	xhttp_set_request_header((xhand_t)pfn->file, L"Date", -1, gmt, -1);

	rt = xhttp_send((xhand_t)pfn->file, NULL, 0);
	if (rt < 0)
	{
		return -1;
	}

	wchar_t code[NUM_LEN + 1] = { 0 };
	xhttp_get_response_code((xhand_t)pfn->file, code);
	if (!XHTTP_SUCCEED(code))
	{
		xhttp_get_response_header((xhand_t)pfn->file, W_XHTTP_HEADER_CONTENTLENGTH, -1, ctlen, NUM_LEN);
		len = xstol(ctlen);
		if (len)
		{
			err = (byte_t*)xmem_alloc(len + 1);
			xhttp_recv(fh, err, len);
			MessageBoxA(NULL, (schar_t*)err, "", MB_OK);
			xmem_free(err);
		}
		return -1;
	}

	return xhttp_recv((xhand_t)pfn->file, (byte_t*)buf, size);
}

size_t _inet_write_file(xhand_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
	wchar_t gmt[50] = { 0 };
	wchar_t host[1024] = { 0 };
	wchar_t port[10] = { 0 };
	wchar_t ctlen[NUM_LEN + 1] = { 0 };
	byte_t* err;
	int rt,len;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);
	assert(buf);

	get_gmt_time(gmt);

	xhttp_get_url_host((xhand_t)pfn->file, host, 1024);
	xhttp_get_url_port((xhand_t)pfn->file, port, 10);

	if (!is_null(port) && xstos(port) != 80)
	{
		xscat(host, L":");
		xscat(host, port);
	}

	xhttp_set_request_header((xhand_t)pfn->file, L"Host", -1, host, -1);
	xhttp_set_request_header((xhand_t)pfn->file, L"Date", -1, gmt, -1);
	xhttp_set_request_header((xhand_t)pfn->file, L"Content-Type", -1, L"image/png", -1);
	xsprintf(ctlen, L"%d", size);
	xhttp_set_request_header((xhand_t)pfn->file, L"Content-Length", -1, ctlen, -1);

	rt = xhttp_send((xhand_t)pfn->file, (byte_t*)buf, size);
	if (rt < (int)size)
	{
		return -1;
	}

	wchar_t code[NUM_LEN + 1] = { 0 };
	xhttp_get_response_code((xhand_t)pfn->file, code);
	if (XHTTP_SUCCEED(code))
	{
		xhttp_get_response_header((xhand_t)pfn->file, W_XHTTP_HEADER_CONTENTLENGTH, -1, ctlen, NUM_LEN);
		len = xstol(ctlen);
		if (len)
		{
			err = (byte_t*)xmem_alloc(len + 1);
			xhttp_recv((xhand_t)pfn->file, err, len);
			MessageBoxA(NULL, (schar_t*)err, "", MB_OK);
			xmem_free(err);
		}
		return -1;
	}

	return xhttp_send((xhand_t)pfn->file, (byte_t*)buf, size);
}

bool_t _inet_delete_file(const wchar_t* fname)
{
	xhand_t xh;
	wchar_t gmt[50] = { 0 };
	wchar_t host[1024] = { 0 };
	wchar_t port[10] = { 0 };
	wchar_t code[NUM_LEN + 1] = { 0 };
	int rt;

	xh = xhttp_open(NULL, L"DELETE", fname);

	if (!xh)
		return 0;

	get_gmt_time(gmt);

	xhttp_get_url_host(xh, host, 1024);
	xhttp_get_url_port(xh, port, 10);

	if (!is_null(port) && xstos(port) != 80)
	{
		xscat(host, L":");
		xscat(host, port);
	}

	xhttp_set_request_header(xh, L"Host", -1, host, -1);
	xhttp_set_request_header(xh, L"Date", -1, gmt, -1);

	rt = xhttp_send(xh, NULL, 0);
	if (rt < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	xhttp_get_response_code(xh, code);

	return XHTTP_SUCCEED(code);
}

void _inet_close_file(xhand_t fh)
{
	file_node_t* pfn;

	assert(fh);
	pfn = (file_node_t*)fh;

	if(pfn->file)
		xhttp_close((xhand_t)pfn->file);

	xmem_free(pfn);
}

bool_t _inet_file_info(const wchar_t* fname, wchar_t* fsize,wchar_t* ftime)
{
	xhand_t fh;
	int rt;
	wchar_t gmt[50] = { 0 };
	wchar_t host[1024] = { 0 };
	wchar_t port[10] = { 0 };

	fh = xhttp_open(NULL, L"HEAD", fname);
	if (!fh)
		return 0;

	get_gmt_time(gmt);

	xhttp_get_url_host(fh, host, 1024);
	xhttp_get_url_port(fh, port, 10);

	if (!is_null(port) && xstos(port) != 80)
	{
		xscat(host, L":");
		xscat(host, port);
	}

	xhttp_set_request_header(fh, L"Host", -1, host, -1);
	xhttp_set_request_header(fh, L"Date", -1, gmt, -1);

	rt = xhttp_send(fh, NULL, 0);
	if (rt < 0)
	{
		xhttp_close(fh);
		return 0;
	}

	if (fsize)
		xhttp_get_response_header(fh, W_XHTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
	if (ftime)
		xhttp_get_response_header(fh, W_XHTTP_HEADER_LASTMODIFIED, -1, ftime, DATE_LEN);

	xhttp_close(fh);

	return 1;
}

bool_t _inet_list_dir(const wchar_t* path, link_t_ptr ptr)
{
	return 0;
}

/****************************************************************************************************/
int _unc_split_path(const wchar_t* file)
{
	int len;
	wchar_t* token;

	len = xslen(file);
	token = (wchar_t*)(file + len);

	while (*token != '/' && *token != '\\' && token != file)
		token--;

	return token - file;
}

bool_t _unc_list_dir(const wchar_t* path,link_t_ptr ptr)
{
	struct _wfinddata_t fd;
	struct tm loc;
	intptr_t rt;
	link_t_ptr nlk;
	wchar_t ftime[DATE_LEN + 1];
	wchar_t fsize[NUM_LEN + 1];
	wchar_t fetag[NUM_LEN + 1];

	rt = _wfindfirst(path, &fd);
	if (rt == -1L)
		return 0;

	do{
		if (fd.attrib & _A_SUBDIR)
			nlk = insert_directory_item(ptr, LINK_LAST);
		else
			nlk = insert_file_item(ptr, LINK_LAST);

		set_file_item_name(nlk, fd.name);

		if (fd.attrib & _A_HIDDEN)
			set_file_item_archive(nlk, ATTR_ARCHIVE_H);
		else if (fd.attrib & _A_RDONLY)
			set_file_item_archive(nlk, ATTR_ARCHIVE_R);
		else
			set_file_item_archive(nlk, ATTR_ARCHIVE_W);

		if (is_file_item(ptr,nlk))
		{
			xsprintf(fsize, L"%d", fd.size);
			set_file_item_size(nlk, (long)fd.size);

			if (fd.time_create == -1L)
				set_file_item_createtime(nlk, NULL);

			if (fd.time_access == -1L)
				set_file_item_accesstime(nlk, NULL);

			localtime_s(&loc, &fd.time_write);

			xsprintf(ftime, L"%04d-%02d-%02d %02d:%02d:%02d", loc.tm_year + 1900, loc.tm_mon + 1, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_sec);
			set_file_item_writetime(nlk, ftime);

			memset((void*)fetag, 0, sizeof(wchar_t)*(NUM_LEN + 1));
			xfile_etag(fd.name, ftime, fsize, fetag);
			set_file_item_etag(nlk, fetag);
		}

	} while (_wfindnext(rt, &fd) == 0);

	_findclose(rt);

	return 1;
}

bool_t _unc_open_dir(const wchar_t* path, bool_t bAdd)
{
	wchar_t cur_path[PATH_LEN];
	wchar_t* token = (wchar_t*)path;

	while (*token == L'\\' || *token == L'/')
		token++;

	while (*token != L'\0')
	{
		while (*token != L'\\' || *token != L'/')
		{
			token++;
		}

		xsncpy(cur_path, path, token - path);

		if (_waccess(cur_path, 0) != 0)
		{
			if (bAdd)
			{
				if (_wmkdir(cur_path) != 0)
					return 0;
			}
		}

		if (*token != L'\\' || *token != L'/')
			token++;
	}

	return 1;
}

xhand_t _unc_open_file(const wchar_t* file,const wchar_t* mode)
{
	file_node_t* pfn;
	FILE* fh;
	wchar_t path[PATH_LEN] = { 0 };
	int pos;
	
	if (xsstr(file, L"w") && xsstr(file, L"+"))
	{
		pos = _unc_split_path(file);
		xsncpy(path, file, pos);

		if (!_unc_open_dir(path, 1))
			return NULL;
	}

	fh = _wfopen(file, mode);
	if(!fh)
	{
		return NULL;
	}
	
	pfn = (file_node_t*)xmem_alloc(sizeof(file_node_t));
	pfn->file = (xhand_t)fh;
	pfn->proto = _PROTO_LOC;

	return (xhand_t)pfn;
}

size_t	_unc_read_file(xhand_t fh, void* buf, size_t size)
{
	file_node_t* pfn;
	DWORD dw = 0;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);

	return fread(buf, 1,size, (FILE*)pfn->file);
}

size_t _unc_write_file(xhand_t fh,void* buf,size_t size)
{
	file_node_t* pfn;
	size_t dw = 0;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);

	dw = fwrite(buf, 1, size, (FILE*)pfn->file);
	fflush((FILE*)pfn->file);

	return dw;
}

void _unc_close_file(xhand_t fh)
{
	file_node_t* pfn;
	DWORD dw = 0;

	assert(fh);
	pfn = (file_node_t*)fh;
	assert(pfn->file);

	fclose((FILE*)pfn->file);
	xmem_free(pfn);
}

bool_t _unc_delete_file(const wchar_t* fname)
{
	return (_wremove(fname) == 0) ? 1 : 0;
}

bool_t _unc_file_info(const wchar_t* fname,wchar_t* fsize, wchar_t* ftime)
{
	struct _stat64i32 sb;
	struct tm* ptm;

	_wstat(fname, &sb);
	ptm = localtime(&sb.st_ctime);
	if (!ptm)
		return 0;

	if (fsize)
		xsprintf(fsize, L"%d", sb.st_size);
	if (ftime)
		xsprintf(ftime, L"%d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	return 1;
}


/****************************************************************************************************************************/
void xfile_etag(const wchar_t* fname, const wchar_t* ftime, const wchar_t* fsize, wchar_t* buf)
{
	byte_t *in_buf;
	byte_t enc_buf[XMD5_RESULT_LEN + 1] = { 0 };
	int len,total = 0;

	len = gb2312_encode(fname, -1, NULL, MAX_LONG) + gb2312_encode(ftime, -1, NULL, MAX_LONG) + gb2312_encode(fsize, -1, NULL, MAX_LONG);
	in_buf = (byte_t*)xmem_alloc(len + 1);

	len = gb2312_encode(fname, -1, in_buf + total, MAX_LONG);
	total += len;

	len = gb2312_encode(ftime, -1, in_buf + total, MAX_LONG);
	total += len;

	len = gb2312_encode(fsize, -1, in_buf + total, MAX_LONG);
	total += len;

	xmd5_encrypt_bytes(in_buf, total, enc_buf);

	xmem_free(in_buf);

	xmd5_token(enc_buf, buf);
}

xhand_t xfile_open(const wchar_t* file,const wchar_t* mode)
{
	int proto;

	proto = parse_proto(file);
	if(proto < 0)
		return 0;

	if (proto == _PROTO_HTTP || proto == _PROTO_FTP)
	{
		return _inet_open_file(file,mode);
	}else
	{
		return _unc_open_file(file,mode);
	}
}

void xfile_setbom(xhand_t fh, int bom)
{
	file_node_t* pfn;

	pfn = (file_node_t*)fh;

	if (bom == UTFBIGBOM || bom == UTFLITBOM)
		pfn->bom = bom;
	else
		pfn->bom = 0;
}

int xfile_getbom(xhand_t fh)
{
	file_node_t* pfn;

	pfn = (file_node_t*)fh;

	return pfn->bom;
}

size_t	xfile_read(xhand_t fh,byte_t* buf,size_t size)
{
	file_node_t* pfn;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	if (pfn->proto == _PROTO_HTTP || pfn->proto == _PROTO_FTP)
		return _inet_read_file(fh,(void*)buf,size);
	else
		return _unc_read_file(fh,(void*)buf,size);

}

size_t xfile_write(xhand_t fh,byte_t* buf,size_t size)
{
	file_node_t* pfn;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	if (pfn->proto == _PROTO_HTTP || pfn->proto == _PROTO_FTP)
		return _inet_write_file(fh,(void*)buf,size);
	else
		return _unc_write_file(fh,(void*)buf,size);
}

void xfile_close(xhand_t fh)
{
	file_node_t* pfn;
	
	assert(fh);
	pfn = (file_node_t*)fh;
	if (pfn->proto == _PROTO_HTTP || pfn->proto == _PROTO_FTP)
		_inet_close_file(fh);
	else
		_unc_close_file(fh);
}

bool_t xfile_delete(const wchar_t* fname)
{
	int proto;

	if (is_null(fname))
		return 0;

	proto = parse_proto(fname);

	if (proto == _PROTO_HTTP || proto == _PROTO_FTP)
	{
		return _inet_delete_file(fname);
	}
	else
	{
		return _unc_delete_file(fname);
	}
}

bool_t xfile_info(const wchar_t* file,wchar_t* fsize,wchar_t* ftime)
{
	int proto;

	if(is_null(file))
		return 0;

	proto = parse_proto(file);

	if (proto == _PROTO_HTTP || proto == _PROTO_FTP)
	{
		return _inet_file_info(file,fsize,ftime);
	}else 
	{
		return _unc_file_info(file,fsize,ftime);
	}
}

bool_t xfile_copy(const wchar_t* srcfile, const wchar_t* destfile)
{
	xhand_t fh_src, fh_dest;
	byte_t* buf;
	size_t len,rt;

	wchar_t fsize[NUM_LEN + 1] = { 0 };
	wchar_t ftime[DATE_LEN + 1] = { 0 };

	if (!xfile_info(srcfile, fsize, ftime))
		return 0;

	fh_src = xfile_open(srcfile, L"rb");
	if (!fh_src)
		return 0;

	fh_dest = xfile_open(destfile, L"wb+");
	if (!fh_dest)
	{
		xfile_close(fh_src);
		return 0;
	}

	len = xstol(fsize);
	buf = (byte_t*)xmem_alloc(len);
	rt = xfile_read(fh_src, buf, len);
	xfile_close(fh_src);

	if (rt != len)
	{
		xmem_free(buf);
		xfile_close(fh_dest);
		return 0;
	}

	rt = xfile_write(fh_dest, buf, len);
	xmem_free(buf);
	xfile_close(fh_dest);

	return (rt == len)? 1 : 0;
}

bool_t xfile_list(const wchar_t* path, link_t_ptr ptr)
{
	int proto;

	if (is_null(path))
		return 0;

	proto = parse_proto(path);

	if (proto == _PROTO_HTTP || proto == _PROTO_FTP)
	{
		return _inet_list_dir(path, ptr);
	}
	else
	{
		return _unc_list_dir(path, ptr);
	}
}