/***********************************************************************
	Easily dimse web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dimse web api document

	@module	dimse.cc | dimse web api implement file
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

#include "dimse.h"

void _invoke_error(const slots_block_t* pb, const dimse_block_t* pd)
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

void _invoke_command(const slots_block_t* pb, const dimse_block_t* pd)
{

}

int STDCALL slots_invoke(slots_block_t* pb)
{
	dimse_block_t* pd = NULL;
	pacs_t* pacs = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t file[PATH_LEN + 1] = { 0 };

	bool_t b_big = 0;
	dword_t n_max = 0;
	byte_t syntax[RES_LEN] = { 0 };

	stream_t stm = NULL;
	bool_t rt = 1;

	TRY_CATCH;

	pacs = pacs_scp(pb->slot);

	if (!pacs)
	{
		raise_user_error(NULL, NULL);
	}

	pd = (dimse_block_t*)xmem_alloc(sizeof(dimse_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/dimse.ini"), pb->path);

	if (!load_proper_doc_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	pacs_set_options(pacs, PACS_OPT_AET_SCP, (void*)"ANY-SCP", a_xslen("ANY-SCP"));

	while (pacs_status(pacs) != _PACS_STATUS_RELEASE)
	{
		if (!pacs_recv(pacs, &pd->pdv))
			continue;

		pacs_get_options(pacs, PACS_OPT_SYNTAX_TRANSFER, syntax, RES_LEN);
		pacs_get_options(pacs, PACS_OPT_DATA_MAXINUM, (void*)&n_max, sizeof(long));
		b_big = sop_is_bigendian(syntax);

		//is command set
		if (pd->pdv.ctl & 0x01)
		{
			stm = stream_alloc(pacs_bio(pacs));

			if (!parse_dicm_command_from_stream(&pd->cmd, b_big, n_max, stm))
			{
				raise_user_error(NULL, NULL);
			}

			switch (pd->cmd.CommandField)
			{
			case PACS_CMD_C_ECHO_RQ:
				_invoke_c_echo(pacs, pd);
				break;
			}

			stream_free(stm);
			stm = NULL;
		}

		//the last pdv
		//if (pd->pdv.ctl & 0x02)
			//break;

		xmem_zero((void*)&(pd->pdv), sizeof(pacs_pdv_head_t));
	}
	
	pacs_close(pacs);
	pacs = NULL;

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? SLOTS_INVOKE_SUCCEED : SLOTS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pd);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pacs)
		pacs_close(pacs);

	if (stm)
		stream_free(stm);

	xmem_free(pd);

	return SLOTS_INVOKE_WITHINFO;
}

