
/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc service log document

	@module	srvlog.cc | service log implement file

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

#include "srvlog.h"


void _write_log_title(file_t log, const tchar_t* sz_title, int len)
{
	byte_t* sz_log = NULL;
	dword_t n_br;
	byte_t br[2] = { '\r', '\n' };
    int n_log = 0;

#ifdef _UNICODE
	n_log = ucs_to_mbs(sz_title, -1, NULL, MAX_LONG);
#else
	n_log = mbs_to_mbs(sz_title, -1, NULL, MAX_LONG);
#endif

	sz_log = (byte_t*)xmem_alloc(n_log + 1);

#ifdef _UNICODE
	n_log = ucs_to_mbs(sz_title, -1, (schar_t*)sz_log, n_log);
#else
	n_log = mbs_to_mbs(sz_title, -1, (schar_t*)sz_log, n_log);
#endif

	n_br = 2;
	xfile_write(log, br, n_br);

	xfile_write(log, sz_log, n_log);

	n_br = 2;
	xfile_write(log, br, n_br);

	xmem_free(sz_log);
	sz_log = NULL;
}

void _write_log_error(file_t log, const tchar_t* sz_code, const tchar_t* sz_error, int len)
{
	byte_t* sz_log = NULL;
	dword_t n_br, n_log = 0;
	tchar_t token[INT_LEN] = { 0 };
    byte_t br[2] = { '\r', '\n' };
    
	format_charset(DEF_MBS, token);

	n_log = xhttp_format_error(0, token, sz_code, sz_error, len, NULL, MAX_LONG);

	sz_log = (byte_t*)xmem_alloc(n_log + 1);

	n_log = xhttp_format_error(0, token, sz_code, sz_error, len, sz_log, n_log);

	xfile_write(log, sz_log, n_log);

    n_br = 2;
    xfile_write(log, br, n_br);
    
	xmem_free(sz_log);
	sz_log = NULL;
}

void _write_log_data(file_t log, const byte_t* data, dword_t size)
{
	dword_t dw;
	byte_t ba[2] = { '\r', '\n' };

	dw = size;
	xfile_write(log, data, dw);

	dw = 2;
	xfile_write(log, ba, dw);
}

void _write_log_xml(file_t log, link_t_ptr ptr_xml)
{
	dword_t dw;
	byte_t ba[2] = { '\r', '\n' };
	byte_t* sz_log = NULL;

	dw = format_xml_doc_to_bytes(ptr_xml, NULL, MAX_LONG);
	sz_log = (byte_t*)xmem_alloc(dw + 1);
	format_xml_doc_to_bytes(ptr_xml, sz_log, dw);

	xfile_write(log, sz_log, dw);
	xmem_free(sz_log);

	dw = 2;
	xfile_write(log, ba, dw);
}

void _write_log_json(file_t log, link_t_ptr ptr_json)
{
	dword_t dw;
	byte_t ba[2] = { '\r', '\n' };
	byte_t* sz_log = NULL;

	dw = format_json_doc_to_bytes(ptr_json, NULL, MAX_LONG, DEF_MBS);
	sz_log = (byte_t*)xmem_alloc(dw + 1);
	format_json_doc_to_bytes(ptr_json, sz_log, dw, DEF_MBS);

	xfile_write(log, sz_log, dw);
	xmem_free(sz_log);

	dw = 2;
	xfile_write(log, ba, dw);
}

/***********************************************************************************/

void xportm_log_info(const tchar_t* str, int len)
{
	byte_t* sz_buf = NULL;
	int size = 0;

	stream_t stm = NULL;
	xhand_t pipe = NULL;

	xpipe_wait(XPORTM_PIPE_NAME, XPORTM_WAIT_TIMO);

	pipe = xpipe_cli(XPORTM_PIPE_NAME, FILE_OPEN_WRITE | FILE_OPEN_OVERLAP);
	if (!pipe)
	{
		return;
	}

	stm = stream_alloc(pipe);
	if (!stm)
	{
		xpipe_free(pipe);
		return;
	}

	stream_set_mode(stm, CHUNK_OPERA);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, NULL, MAX_LONG);
#else
	size = mbs_to_mbs(str, len, NULL, MAX_LONG);
#endif

	sz_buf = (byte_t*)xmem_alloc(size + 1);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, (schar_t*)sz_buf, size);
#else
	size = mbs_to_mbs(str, len, (schar_t*)sz_buf, size);
#endif

	stream_write_chunk_size(stm, size);

	stream_write_bytes(stm, (byte_t*)sz_buf, size);

	xmem_free(sz_buf);
	sz_buf = NULL;

	stream_write_chunk_size(stm, 0);

	stream_free(stm);

	xpipe_flush(pipe);

	xpipe_free(pipe);
}

void xportm_log_error(const tchar_t* errcode, const tchar_t* errtext)
{
	byte_t* sz_buf = NULL;
	dword_t len,size = 0;

	stream_t stm = NULL;
	xhand_t pipe = NULL;

	xpipe_wait(XPORTM_PIPE_NAME, XPORTM_WAIT_TIMO);

	pipe = xpipe_cli(XPORTM_PIPE_NAME, FILE_OPEN_WRITE | FILE_OPEN_OVERLAP);
	if (!pipe)
	{
		return;
	}

	stm = stream_alloc(pipe);
	if (!stm)
	{
		xpipe_free(pipe);
		return;
	}

	stream_set_mode(stm, CHUNK_OPERA);

#ifdef _UNICODE
	size = ucs_to_mbs(errcode, -1, NULL, MAX_LONG) + ucs_to_mbs(errtext, -1, NULL, MAX_LONG) + 3;
#else
	size = mbs_to_mbs(errcode, -1, NULL, MAX_LONG) + mbs_to_mbs(errtext, -1, NULL, MAX_LONG) + 3;;
#endif

	sz_buf = (byte_t*)xmem_alloc(size + 1);

#ifdef _UNICODE
	len = ucs_to_mbs(errcode, -1, (schar_t*)sz_buf, size);

	a_xsncpy((schar_t*)(sz_buf + len) , ": ", 2);
	len += 2;

	len += ucs_to_mbs(errtext, -1, (schar_t*)(sz_buf + len), size - len);
	a_xsncpy((schar_t*)(sz_buf + len), "\n", 1);
	len++;
#else
	len = mbs_to_mbs(errcode, -1, (schar_t*)sz_buf, size);

	a_xsncpy((schar_t*)(sz_buf + len), ": ", 2);
	len += 2;

	len += mbs_to_mbs(errtext, -1, (schar_t*)(sz_buf + len), size - len);
	a_xsncpy((schar_t*)(sz_buf + len), "\n", 1);
	len++;
#endif

	size = len;
	stream_write_chunk_size(stm, size);

	stream_write_bytes(stm, (byte_t*)sz_buf, size);

	xmem_free(sz_buf);
	sz_buf = NULL;

	stream_write_chunk_size(stm, 0);

	stream_free(stm);

	xpipe_flush(pipe);

	xpipe_free(pipe);
}

void xportm_log_data(const byte_t* data, dword_t size)
{
	stream_t stm = NULL;
	xhand_t pipe = NULL;
    
	xpipe_wait(XPORTM_PIPE_NAME, XPORTM_WAIT_TIMO);

	pipe = xpipe_cli(XPORTM_PIPE_NAME, FILE_OPEN_WRITE | FILE_OPEN_OVERLAP);
	if (!pipe)
	{
		return;
	}

	stm = stream_alloc(pipe);
	if (!stm)
	{
		xpipe_free(pipe);
		return;
	}

	stream_set_mode(stm, CHUNK_OPERA);

	stream_write_chunk_size(stm, size);

	stream_write_bytes(stm, data, size);

	stream_write_chunk_size(stm, 0);

	stream_free(stm);

	xpipe_flush(pipe);

	xpipe_free(pipe);
}


