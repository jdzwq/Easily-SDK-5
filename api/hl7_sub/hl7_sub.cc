/***********************************************************************
	Easily hl7 web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hl7 web api document

	@module	hl7_sub.cc | hl7 web api implement file
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

#include "hl7_sub.h"

typedef struct _hl7sub_block_t{
	hl7net_t* hl7net;

	tchar_t topic_name[PATH_LEN];

	radhex_t* radhex;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}hl7sub_block_t;

void _invoke_error(const slots_block_t* pb, const hl7sub_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[ERROR]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

void _invoke_subscribe(const slots_block_t* pb, const hl7sub_block_t* pd)
{
	tchar_t topic_name[PATH_LEN] = { 0 };

	hl7net_pdv_head_t pdv = { 0 };

	stream_t stm = NULL;

	byte_t* buf = NULL;
	dword_t len;

	int i;

	TRY_CATCH;

	stm = stream_alloc(hl7net_bio(pd->hl7net));

	if (!hl7net_poll_subscribe(pd->hl7net, topic_name, PATH_LEN))
	{
		raise_user_error(NULL, NULL);
	}

	if (is_null(topic_name))
	{
		hl7net_abort(pd->hl7net, HL7NET_ERROR_INVALIDTOPIC);
		raise_user_error(_T("_invoke_subscribe"), HL7NET_ERROR_INVALIDTOPIC_TEXT);
	}

	i = 2;
	while (i--)
	{
		xmem_copy((void*)pdv.type, "MSH", 3);
		pdv.size = 4;

		if (!hl7net_push_message(pd->hl7net, &pdv))
		{
			raise_user_error(NULL, NULL);
		}

		buf = (byte_t*)xmem_alloc(pdv.size + 1);
		a_xsprintf((schar_t*)buf, "msg%d", i);

		if (!stream_write_bytes(stm, buf, pdv.size))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
	} 

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return;
ONERROR:

	XDL_TRACE_LAST;

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	return;
}

int STDCALL slots_invoke(slots_block_t* pb)
{
	hl7sub_block_t* pd = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t file[PATH_LEN + 1] = { 0 };

	tchar_t addr[ADDR_LEN] = { 0 };
	tchar_t port[INT_LEN] = { 0 };
	tchar_t dbn[RES_LEN] = { 0 };

	xhand_t bio = NULL;

	TRY_CATCH;

	pd = (hl7sub_block_t*)xmem_alloc(sizeof(hl7sub_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/hl7sub.ini"), pb->path);

	if (!load_proper_doc_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(NULL, NULL);
	}

	read_proper(ptr_prop, _T("RADHEX"), -1, _T("SERVER"), -1, addr, ADDR_LEN);
	read_proper(ptr_prop, _T("RADHEX"), -1, _T("PORT"), -1, port, INT_LEN);
	read_proper(ptr_prop, _T("RADHEX"), -1, _T("DATABASE"), -1, dbn, RES_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	bio = xtcp_cli((unsigned short)xstol(port), addr);
	if (!bio)
	{
		raise_user_error(NULL, NULL);
	}

	pd->radhex = radhex_scu(bio);

	if (!radhex_connect(pd->radhex, dbn))
	{
		raise_user_error(NULL, NULL);
	}

	pd->hl7net = hl7net_scp(pb->slot, _HL7NET_TYPE_SCP_SUB);
	if (!pd->hl7net)
	{
		raise_user_error(NULL, NULL);
	}

	if (!hl7net_accept(pd->hl7net))
	{
		raise_user_error(NULL, NULL);
	}

	_invoke_subscribe(pb, pd);

	radhex_close(pd->radhex);
	pd->radhex = NULL;

	xtcp_close(bio);
	bio = NULL;

	hl7net_close(pd->hl7net);
	pd->hl7net = NULL;

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return SLOTS_INVOKE_SUCCEED;

ONERROR:

	_invoke_error(pb, pd);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
	{
		if (pd->hl7net)
			hl7net_close(pd->hl7net);

		if (pd->radhex)
			radhex_close(pd->radhex);

		xmem_free(pd);
	}

	if (bio)
		xtcp_close(bio);

	return SLOTS_INVOKE_WITHINFO;
}


