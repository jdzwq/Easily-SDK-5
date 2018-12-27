/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file document

	@module	impunc.c | unc file implement file

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

#include "impunc.h"
#include "xdlinit.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDK_SUPPORT_FILE

typedef struct _xunc_t{
	xhand_head head;		//reserved for xhand_t
	res_file_t file;

	async_t over;
}xunc_t;


bool_t xunc_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	file_info_t fi = { 0 };
	tchar_t sz_time[DATE_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	int encode = 0;

	bool_t rt;
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	rt = (*pif->pf_file_info)(fname, &fi);
	if (!rt)
	{
		set_system_error(_T("xunc_file_info"));
		return 0;
	}

	if (ftime)
		format_gmttime(&fi.write_time, ftime);

	if (fsize)
		format_longlong(fi.high_size, fi.low_size, fsize);

	if (fetag)
	{
		format_gmttime(&fi.write_time, sz_time);
		format_longlong(fi.high_size, fi.low_size, sz_size);
#ifdef XDL_SUPPORT_CRYPT
		file_info_etag(fi.file_name, sz_time, sz_size, fetag);
#endif
	}

	if (fencode)
	{
		encode = xunc_file_encode(psd, fname);

		format_encode(encode, fencode);
	}

	return 1;
}

int xunc_file_encode(const secu_desc_t* psd, const tchar_t* fname)
{
	xhand_t xh;
	byte_t ba[4] = { 0 };
	dword_t dw = 0;

	xh = xunc_open_file(psd, fname, 0);
	if (!xh)
		return 0;

	dw = 3;
	if (!xunc_read_file(xh, (void*)ba, &dw))
		return 0;

	xunc_close_file(xh);

	return parse_utfbom(ba, 3);
}

res_find_t xunc_find_first(const secu_desc_t* psd, const tchar_t* path, file_info_t* pfi)
{
	res_find_t fd;
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	fd = (*pif->pf_file_find_first)(path, pfi);
	if (!fd)
	{
		set_system_error(_T("xunc_find_first"));
	}

	return fd;
}

bool_t xunc_find_next(res_find_t fd, file_info_t* pfi)
{
	bool_t rt;
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	rt = (*pif->pf_file_find_next)(fd, pfi);
	if (!rt)
	{
		set_system_error(_T("xunc_find_next"));
	}

	return rt;
}

bool_t xunc_open_directory(const secu_desc_t* psd, const tchar_t* path, dword_t mode)
{
	bool_t rt;
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	rt = (*pif->pf_directory_open)(path, mode);
	if (!rt)
	{
		set_system_error(_T("xunc_open_dir"));
	}

	return rt;
}

static int _split_path_len(const tchar_t* file)
{
	int len;
	tchar_t* token;

	len = xslen(file);
	token = (tchar_t*)(file + len);

	while (*token != _T('/') && *token != _T('\\') && token != file)
		token--;

	return (int)(token - file);
}

xhand_t xunc_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t fmode)
{
	xunc_t *pcf;
	res_file_t fh;
	int pos;
	bool_t b_add;
	tchar_t path[PATH_LEN];
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	pos = _split_path_len(fname);
	xsncpy(path, fname, pos);

	b_add = ((fmode & FILE_OPEN_CREATE) || (fmode & FILE_OPEN_APPEND)) ? 1 : 0;

	if (b_add && !xunc_open_directory(psd, path, fmode))
	{
		return NULL;
	}

	fh = (*pif->pf_file_open)(fname, fmode);
	if (fh == INVALID_FILE)
	{
		set_system_error(_T("xunc_open_file"));
		return NULL;
	}

	pcf = (xunc_t*)xmem_alloc(sizeof(xunc_t));
	pcf->head.tag = _HANDLE_UNC;
	pcf->file = fh;

	if (fmode & FILE_OPEN_OVERLAP)
	{
		async_alloc_lapp(&pcf->over);
	}

	return (xhand_t)pcf;
}

void xunc_close_file(xhand_t unc)
{
	xunc_t* pcf = (xunc_t*)unc;
	if_file_t* pif;

	XDL_ASSERT(unc && unc->tag == _HANDLE_UNC);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_file_close)(pcf->file);

	async_free_lapp(&pcf->over);

	xmem_free(pcf);
}

bool_t xunc_read_file(xhand_t unc, byte_t* buf, dword_t* pcb)
{
	xunc_t* pcf = (xunc_t*)unc;
	size_t size;
	if_file_t* pif;

	XDL_ASSERT(unc && unc->tag == _HANDLE_UNC);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;

	if (!(*pif->pf_file_read)(pcf->file, (void*)(buf), size, &pcf->over))
	{
		set_system_error(_T("xunc_read_file"));
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)pcf->over.size;

	return 1;
}

bool_t xunc_write_file(xhand_t unc, const byte_t* buf, dword_t* pcb)
{
	xunc_t* pcf = (xunc_t*)unc;
	size_t size;
	if_file_t* pif;

	XDL_ASSERT(unc && unc->tag == _HANDLE_UNC);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;

	if (!(*pif->pf_file_write)(pcf->file, (void*)(buf), size, &pcf->over))
	{
		set_system_error(_T("xunc_write_file"));
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)pcf->over.size;

	return 1;
}

bool_t xunc_flush_file(xhand_t unc)
{
	xunc_t* pcf = (xunc_t*)unc;
	if_file_t* pif;

	XDL_ASSERT(unc && unc->tag == _HANDLE_UNC);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_file_flush)(pcf->file);
}

bool_t xunc_read_file_range(xhand_t unc, dword_t hoff, dword_t loff, byte_t* buf, dword_t* pcb)
{
	xunc_t* pcf = (xunc_t*)unc;
	size_t size;
	long long ll = 0;
	if_file_t* pif;

	XDL_ASSERT(unc && unc->tag == _HANDLE_UNC);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;

	if (!(*pif->pf_file_read_range)(pcf->file, hoff, loff, (void*)(buf), size, &size))
	{
		set_system_error(_T("xunc_read_file_range"));
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)size;

	return 1;
}

bool_t xunc_write_file_range(xhand_t unc, dword_t hoff, dword_t loff, const byte_t* buf, dword_t* pcb)
{
	xunc_t* pcf = (xunc_t*)unc;
	size_t size;
	long long ll = 0;
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;
	
	if (!(*pif->pf_file_write_range)(pcf->file,  hoff, loff, (void*)(buf), size, &size))
	{
		set_system_error(_T("xunc_write_file_range"));
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)size;

	return 1;
}

bool_t xunc_set_filetime(xhand_t unc, const tchar_t* ftime)
{
	xunc_t* pcf = (xunc_t*)unc;
	if_file_t* pif;
	xdate_t xd;

	XDL_ASSERT(unc && unc->tag == _HANDLE_UNC);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	parse_gmttime(&xd, ftime);

	if (!(*pif->pf_file_settime)(pcf->file, &xd))
	{
		set_system_error(_T("xunc_set_filetime"));
		return 0;
	}

	return 1;
}

bool_t xunc_get_filetime(xhand_t unc, tchar_t* ftime)
{
	xunc_t* pcf = (xunc_t*)unc;
	if_file_t* pif;
	xdate_t xd;

	XDL_ASSERT(unc && unc->tag == _HANDLE_UNC);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_file_gettime)(pcf->file, &xd))
	{
		set_system_error(_T("xunc_get_file_time"));
		return 0;
	}

	format_gmttime(&xd, ftime);

	return 1;
}

bool_t xunc_delete_file(const secu_desc_t* psd, const tchar_t* fname)
{
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_file_delete)(fname))
	{
		set_system_error(_T("xunc_delete_file"));
		return 0;
	}

	return 1;
}

bool_t xunc_rename_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* nname)
{
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_file_rename)(fname, nname))
	{
		set_system_error(_T("xunc_rename_file"));
		return 0;
	}

	return 1;
}

bool_t xunc_create_directory(const secu_desc_t* psd, const tchar_t* pname)
{
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_directory_create)(pname))
	{
		set_system_error(_T("xunc_directory_create"));
		return 0;
	}

	return 1;
}

bool_t xunc_remove_directory(const secu_desc_t* psd, const tchar_t* pname)
{
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_directory_remove)(pname))
	{
		set_system_error(_T("xunc_directory_remove"));
		return 0;
	}

	return 1;
}

bool_t xunc_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
{
	res_find_t fd;
	file_info_t fi = { 0 };

	fd = xunc_find_first(psd, path, &fi);
	if (!fd)
	{
		return 1;
	}

	do{
		if (compare_text(fi.file_name, -1, _T("."), -1, 0) == 0 || compare_text(fi.file_name, -1, _T(".."), -1, 0) == 0)
		{
			continue;
		}

		if (pf)
		{
			(*pf)(&fi, pa);
		}

		xmem_zero((void*)&fi, sizeof(file_info_t));

	} while (xunc_find_next(fd, &fi));

	return 1;
}



#endif //XDK_SUPPORT_FILE
