
/***********************************************************************
	Easily iot web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc iot web api document

	@module	iot_api.cc | iot web api implement file

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

#include "iot_api.h"

typedef struct _iot_block_t{
	secu_desc_t sd;
	tchar_t loca[PATH_LEN];

	PF_IOT_OPEN_ISP pf_open_isp;
	PF_IOT_CLOSE pf_close;
	PF_IOT_SESSION pf_session;
	PF_IOT_COMMAND pf_command;
	PF_IOT_ERROR pf_error;

	tchar_t isp_file[PATH_LEN];
}iot_block_t;


bool_t _invoke_iot_session(const https_block_t* pb, iot_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	bool_t rt;
	IOT iot = NULL;

	tchar_t session_key[KEY_LEN] = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk_row, nlk;

	TRY_CATCH;

	iot = (*pxb->pf_open_isp)(pxb->isp_file);
	if (!iot)
	{
		raise_user_error(NULL, NULL);
	}

	rt = (*pxb->pf_session)(iot, session_key, KEY_LEN);

	(*pxb->pf_error)(iot, sz_error, ERR_LEN);

	if (!rt)
	{
		raise_user_error(_T("0"), sz_error);
	}

	(*pxb->pf_close)(iot);
	iot = NULL;

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CACHECONTROL, -1, HTTP_HEADER_CACHECONTROL_NOSTORE, -1);

	ptr_json = create_json_doc();

	set_dom_node_name(ptr_json, _T("rowset"), -1);

	nlk_row = insert_dom_node(ptr_json, LINK_LAST);
	set_dom_node_name(nlk_row, _T("row"), -1);

	nlk = insert_dom_node(nlk_row, LINK_LAST);
	set_dom_node_name(nlk, _T("session_key"), -1);
	set_dom_node_text(nlk, session_key, -1);

	if (!xhttp_send_json(pb->http, ptr_json))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[IOT-Session]"), -1);

		xsprintf(sz_error, _T("IOT succeed"), 1);
		(*pb->pf_log_error)(pb->log, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (iot)
	{
		(*pxb->pf_close)(iot);
	}

	if (ptr_json)
		destroy_json_doc(ptr_json);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[IOT-Session]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_iot_command(const https_block_t* pb, iot_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	bool_t rt;
	IOT iot = NULL;

	iot_command_t ic = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk_row, nlk;

	TRY_CATCH;

	ptr_json = create_json_doc();

	rt = xhttp_recv_json(pb->http, ptr_json);
	if (!rt)
	{
		get_last_error(sz_code, sz_error, ERR_LEN);
	}

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易请求]"), -1);

		(*pb->pf_log_json)(pb->log, ptr_json);
	}

	if (!rt)
	{
		raise_user_error(sz_code, sz_error);
	}

	nlk_row = get_dom_first_child_node(ptr_json);
	while (nlk_row)
	{
		if (compare_text(get_dom_node_name_ptr(nlk_row), -1, _T("device_id"), -1, 1) == 0)
		{
			get_dom_node_text(nlk_row, ic.device_id, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk_row), -1, _T("device_data"), -1, 1) == 0)
		{
			get_dom_node_text(nlk_row, ic.command_input, KEY_LEN);
		}

		nlk_row = get_dom_next_sibling_node(nlk_row);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	if (is_null(ic.device_id))
	{
		raise_user_error(_T("_invoke_iot_command"), _T("device id empty"));
	}

	iot = (*pxb->pf_open_isp)(pxb->isp_file);
	if (!iot)
	{
		raise_user_error(NULL, NULL);
	}

	rt = (*pxb->pf_session)(iot, ic.session_key, KEY_LEN);

	if (!rt)
	{
		(*pxb->pf_error)(iot, sz_error, ERR_LEN);
		raise_user_error(_T("_invoke_iot_command"), sz_error);
	}

	rt = (*pxb->pf_command)(iot, &ic);

	if (!rt)
	{
		(*pxb->pf_error)(iot, sz_error, ERR_LEN);
		raise_user_error(_T("_invoke_iot_command"), sz_error);
	}

	(*pxb->pf_close)(iot);
	iot = NULL;

	//返回应答
	xscpy(sz_code, _T("_invoke_iot_command"));
	xsprintf(sz_error, _T("command succeed"));

	xhttp_send_error(pb->http, HTTP_CODE_200, HTTP_CODE_200_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[IOT-Session]"), -1);

		xsprintf(sz_error, _T("IOT succeed"), 1);
		(*pb->pf_log_error)(pb->log, _T("0"), sz_error, -1);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (iot)
	{
		(*pxb->pf_close)(iot);
	}

	if (ptr_json)
		destroy_json_doc(ptr_json);

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[IOT-Session]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, iot_block_t* pxb)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	d_recv = bytes_alloc();

	xhttp_recv_full(pb->http, d_recv, &n_size);

	bytes_free(d_recv);
	d_recv = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[IOT-Error]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

/****************************************************************************************************/

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	iot_block_t* pxb = NULL;

	tchar_t action[RES_LEN] = { 0 };
	int len;

	tchar_t token[RES_LEN] = { 0 };
	tchar_t file[PATH_LEN] = { 0 };
	LINKPTR ptr_prop = NULL;

	res_modu_t iot_lib = NULL;

	bool_t rt = 1;

	TRY_CATCH;

	pxb = (iot_block_t*)xmem_alloc(sizeof(iot_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/iot.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load iot config falied\n"));
	}

	xszero(file, PATH_LEN);
	read_proper(ptr_prop, _T("IOT"), -1, _T("LOCATION"), -1, file, PATH_LEN);
	read_proper(ptr_prop, _T("IOT"), -1, _T("PUBLICKEY"), -1, pxb->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("IOT"), -1, _T("PRIVATEKEY"), -1, pxb->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pxb->loca, file);

	if (xsnicmp(_T("telecom"), pb->object + 1, xslen(_T("telecom"))) == 0)
	{
#if defined(_OS_WINDOWS)
		iot_lib = load_library(_T("iot_telecom.dll"));
#elif defined(_OS_MACOS)
		iot_lib = load_library(_T("libiot_telecom.dylib"));
#elif defined(_OS_LINUX)
		iot_lib = load_library(_T("libiot_telecom.so"));
#endif
		len += xslen(_T("telecom"));
	}
	else
	{
		raise_user_error(_T("iot_api"), _T("unknown iot library"));
	}

	if (!iot_lib)
	{
		raise_user_error(_T("iot_api"), _T("load iot library falied\n"));
	}

	pxb->pf_open_isp = (PF_IOT_OPEN_ISP)get_address(iot_lib, "iot_open_isp");
	pxb->pf_close = (PF_IOT_CLOSE)get_address(iot_lib, "iot_close");
	pxb->pf_session = (PF_IOT_SESSION)get_address(iot_lib, "iot_session");
	pxb->pf_command = (PF_IOT_COMMAND)get_address(iot_lib, "iot_command");
	pxb->pf_error = (PF_IOT_ERROR)get_address(iot_lib, "iot_error");

	if (!pxb->pf_open_isp || !pxb->pf_close || !pxb->pf_session || !pxb->pf_command || !pxb->pf_error)
	{
		raise_user_error(_T("iot_api"), _T("get open/close functon falied"));
	}

	xsprintf(pxb->isp_file, _T("%s%s"), pb->path, pb->object);

	xhttp_get_url_query_entity(pb->http, _T("action"), -1, action, RES_LEN);

	if (compare_text(action, -1, _T("session_key"), -1, 1) == 0)
	{
		rt = _invoke_iot_session(pb, pxb);
	}
	else if (compare_text(action, -1, _T("command"), -1, 1) == 0)
	{
		rt = _invoke_iot_command(pb, pxb);
	}
	else
	{
		raise_user_error(_T("iot_api"), _T("unknown iot function\n"));
	}

	free_library(iot_lib);

	xmem_free(pxb);

	END_CATCH;

	return (rt)? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	if (pxb)
	{
		_invoke_error(pb, pxb);
	}

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (iot_lib)
		free_library(iot_lib);

	if (pxb)
	{
		xmem_free(pxb);
	}

	return HTTPS_INVOKE_WITHINFO;
}
