/***********************************************************************
	Easily tftp web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tftp web api document

	@module	tftp_api.cc | tftp web api implement file

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

#include "tftp_api.h"

typedef struct _tftp_block_t{
	xhand_t tftp;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}tftp_block_t;

/*********************************************************************************/
static bool_t _invoke_head(const udps_block_t* pb, tftp_block_t* ptb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_object[PATH_LEN] = { 0 };

	tchar_t fname[512] = { 0 };
	tchar_t ftime[DATE_LEN] = { 0 };
	tchar_t fsize[NUM_LEN] = { 0 };

	res_find_t fd = NULL;
	file_info_t fi = { 0 };

	TRY_CATCH;

	xtftp_object(ptb->tftp, fname, 512);
	xsprintf(sz_object, _T("%s%s"), ptb->local, fname);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: HEAD]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	fd = xuncf_find_first(&ptb->sd, sz_object, &fi);
	if (!fd)
	{
		xtftp_abort(ptb->tftp, TFTP_CODE_NOTFIND);
	}
	else
	{
		do
		{
			xtftp_set_isdir(ptb->tftp, fi.is_dir);
			xtftp_set_filename(ptb->tftp, fi.file_name);

			if (!fi.is_dir)
			{
				xtftp_set_filesize(ptb->tftp, fi.low_size);

				format_gmttime(&fi.create_time, ftime);
				xtftp_set_filetime(ptb->tftp, ftime);
			}

			xtftp_head(ptb->tftp);

		} while (xuncf_find_next(fd, &fi));

		xuncf_find_close(fd);

		//NULL FILE
		xtftp_set_isdir(ptb->tftp, 0);
		xtftp_set_filename(ptb->tftp, NULL);
		xtftp_set_filesize(ptb->tftp, 0);
		xtftp_set_filetime(ptb->tftp, NULL);

		xtftp_head(ptb->tftp);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

static bool_t _invoke_get(const udps_block_t* pb, tftp_block_t* ptb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_object[PATH_LEN] = { 0 };

	tchar_t fname[512] = { 0 };
	tchar_t ftime[DATE_LEN] = { 0 };
	tchar_t fsize[NUM_LEN] = { 0 };

	file_t xf = NULL;
	bool_t rt;

	byte_t pack[TFTP_PDV_SIZE] = { 0 };
	dword_t bys, size, pos = 0;

	TRY_CATCH;
	
	xtftp_object(ptb->tftp, fname, 512);
	xsprintf(sz_object, _T("%s%s"), ptb->local, fname);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: GET]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	if (!xfile_info(&ptb->sd, sz_object, ftime, fsize, NULL, NULL))
	{
		xtftp_abort(ptb->tftp, TFTP_CODE_NOTFIND);

		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize))
	{
		xtftp_abort(ptb->tftp, TFTP_CODE_DSKFULL);

		raise_user_error(_T("_invoke_get"), _T("not support large file\n"));
	}

	xf = xfile_open(&ptb->sd, sz_object, FILE_OPEN_READ);
	if (!xf)
	{
		xtftp_abort(ptb->tftp, TFTP_CODE_REJECT);

		raise_user_error(_T("_invoke_get"), _T("open local file failed\n"));
	}

	size = xstol(fsize);
	while (pos < size)
	{
		bys = (size - pos < TFTP_PDV_SIZE) ? (size - pos) : TFTP_PDV_SIZE;
		rt = xfile_read_range(xf, 0, pos, pack, bys);
		if (!rt)
		{
			xtftp_abort(ptb->tftp, TFTP_CODE_REJECT);

			raise_user_error(_T("_invoke_get"), _T("read local data failed\n"));
		}

		rt = xtftp_send(ptb->tftp, pack, &bys);
		if (!rt)
		{
			raise_user_error(NULL, NULL);
		}

		pos += bys;
	}

	xtftp_flush(ptb->tftp);

	xfile_close(xf);
	xf = NULL;

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (xf)
		xfile_close(xf);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

static bool_t _invoke_put(const udps_block_t* pb, tftp_block_t* ptb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_object[PATH_LEN] = { 0 };

	tchar_t fname[512] = { 0 };

	file_t xf = NULL;
	bool_t rt;

	byte_t pack[TFTP_PDV_SIZE] = { 0 };
	dword_t bys, pos = 0;

	TRY_CATCH;

	xtftp_object(ptb->tftp, fname, 512);
	xsprintf(sz_object, _T("%s%s"), ptb->local, fname);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: PUT]"), -1);
	}

	xf = xfile_open(&ptb->sd, sz_object, FILE_OPEN_CREATE);
	if (!xf)
	{
		xtftp_abort(ptb->tftp, TFTP_CODE_REJECT);

		raise_user_error(_T("_invoke_put"), _T("open file failed\n"));
	}

	while (1)
	{
		bys = TFTP_PDV_SIZE;
		rt = xtftp_recv(ptb->tftp, pack, &bys);
		if (!rt)
		{
			raise_user_error(NULL, NULL);
		}

		rt = xfile_write_range(xf, 0, pos, pack, bys);
		if (!rt)
		{
			xtftp_abort(ptb->tftp, TFTP_CODE_REJECT);

			raise_user_error(_T("_invoke_put"), _T("write local file failed\n"));
		}

		pos += bys;
		if (bys < TFTP_PDV_SIZE)
			break;
	}

	xfile_close(xf);
	xf = NULL;

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (xf)
		xfile_close(xf);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

static bool_t _invoke_delete(const udps_block_t* pb, tftp_block_t* ptb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_object[PATH_LEN] = { 0 };

	tchar_t fname[512] = { 0 };
	tchar_t ftime[DATE_LEN] = { 0 };
	tchar_t fsize[NUM_LEN] = { 0 };

	res_find_t fd = NULL;
	file_info_t fi = { 0 };

	TRY_CATCH;

	xtftp_object(ptb->tftp, fname, 512);
	xsprintf(sz_object, _T("%s%s"), ptb->local, fname);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: DELETE]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	if (!xfile_delete(&ptb->sd, sz_object))
		xtftp_abort(ptb->tftp, TFTP_CODE_NOTFIND);
	else
		xtftp_abort(ptb->tftp, 0);

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}
/****************************************************************************************************/

int STDCALL udps_invoke(const udps_block_t* pb)
{
	tftp_block_t* ptb = NULL;

	tchar_t file[PATH_LEN] = { 0 };
	tchar_t token[RES_LEN] = { 0 };

	link_t_ptr ptr_prop = NULL;
	bool_t rt = 1;

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	ptb = (tftp_block_t*)xmem_alloc(sizeof(tftp_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/tftp.ini"), pb->path);

	if (!load_proper_doc_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load tftp config falied\n"));
	}

	read_proper(ptr_prop, _T("TFTP"), -1, _T("LOCATION"), -1, ptb->local, PATH_LEN);
	
	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	ptb->tftp = xtftp_server(pb->port, pb->addr, pb->pack, pb->size);
	if (!ptb->tftp)
	{
		raise_user_error(_T("tftp_api"), _T("create tftp service failed"));
	}

	xtftp_method(ptb->tftp, token, INT_LEN);

	if (compare_text(token, -1, _T("GET"), -1, 1) == 0)
		rt = _invoke_get(pb, ptb);
	else if(compare_text(token, -1, _T("PUT"), -1, 1) == 0)
		rt = _invoke_put(pb, ptb);
	else if (compare_text(token, -1, _T("HEAD"), -1, 1) == 0)
		rt = _invoke_head(pb, ptb);
	else if (compare_text(token, -1, _T("DELETE"), -1, 1) == 0)
		rt = _invoke_delete(pb, ptb);
	else
	{
		raise_user_error(_T("tftp_api"), _T("unknown tftp method"));
	}

	xtftp_close(ptb->tftp);
	ptb->tftp = NULL;

	xmem_free(ptb);

	END_CATCH;

	return (rt) ? UDPS_INVOKE_SUCCEED : UDPS_INVOKE_WITHINFO;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[TFTP: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (ptb)
	{
		if (ptb->tftp)
			xtftp_close(ptb->tftp);

		xmem_free(ptb);
	}

	return UDPS_INVOKE_WITHINFO;
}