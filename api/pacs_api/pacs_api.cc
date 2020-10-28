/***********************************************************************
	Easily pacs web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc pacs web api document

	@module	pacs.cc | pacs web api implement file
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

#include "pacs_api.h"

int STDCALL tcps_invoke(const tcps_block_t* pb)
{
	pacs_block_t* pd = NULL;
	pacs_t* pacs = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t file[PATH_LEN + 1] = { 0 };

	bool_t b_big = 0;
	dword_t n_max = 0;
	byte_t syntax[RES_LEN + 1] = { 0 };

	stream_t stm = NULL;
	bool_t rt = 1;

	TRY_CATCH;

	pacs = pacs_scp(pb->tcp);

	if (!pacs)
	{
		raise_user_error(NULL, NULL);
	}

	pd = (pacs_block_t*)xmem_alloc(sizeof(pacs_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/pacs.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
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
			stm = stream_alloc(pacs->pif);

			if (!parse_dicm_command_from_stream(&pd->cmd, b_big, n_max, stm))
			{
				raise_user_error(NULL, NULL);
			}

			switch (pd->cmd.CommandField)
			{
			case DICM_CMD_C_ECHO_RQ:
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

	if (pb->pf_track_eror)
	{
		(*pb->pf_track_eror)(pb->hand, pd->code, pd->text);
	}

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? TCPS_INVOKE_SUCCEED : TCPS_INVOKE_WITHINFO;

ONERROR:
	
	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pacs)
		pacs_close(pacs);

	if (stm)
		stream_free(stm);

	if (pd)
	{
		get_last_error(pd->code, pd->text, ERR_LEN);

		if (pb->pf_track_eror)
		{
			(*pb->pf_track_eror)(pb->hand, pd->code, pd->text);
		}

		xmem_free(pd);
	}

	return TCPS_INVOKE_WITHINFO;
}

