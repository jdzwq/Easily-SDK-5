/***********************************************************************
	Easily hl7 web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hl7 web api document

	@module	hl7_api.cc | hl7 web api implement file
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

#include "hl7_api.h"

typedef struct _hl7api_block_t{
	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}hl7api_block_t;



void _invoke_error(const https_block_t* pb, hl7api_block_t* pos)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	d_recv = buffer_alloc();

	xhttp_recv_full(pb->http, d_recv, &n_size);

	buffer_free(d_recv);
	d_recv = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7API: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	hl7api_block_t* pos = NULL;

	tchar_t token[PATH_LEN + 1] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pos = (hl7api_block_t*)xmem_alloc(sizeof(hl7api_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(token, _T("%s/hl7api.ini"), pb->path);

	if (!load_proper_doc_from_ini_file(ptr_prop, NULL, token))
	{
		raise_user_error(_T("-1"), _T("load loc config falied\n"));
	}

	read_proper(ptr_prop, _T("HL7API"), -1, _T("LOCATION"), -1, pos->local, PATH_LEN);
	read_proper(ptr_prop, _T("HL7API"), -1, _T("PUBLICKEY"), -1, pos->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("HL7API"), -1, _T("PRIVATEKEY"), -1, pos->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	
	xmem_free(pos);

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pos);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	xmem_free(pos);

	return HTTPS_INVOKE_WITHINFO;
}
