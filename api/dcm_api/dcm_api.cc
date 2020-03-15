/***********************************************************************
	Easily dcm web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dcm web api document

	@module	dcm.cc | dcm web api implement file
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

#include "dcm_api.h"

int STDCALL slots_invoke(const slots_block_t* pb)
{
	dcm_block_t* pd = NULL;
	dcm_t* dcm = NULL;

	LINKPTR ptr_prop = NULL;

	tchar_t file[PATH_LEN + 1] = { 0 };

	bool_t b_big = 0;
	dword_t n_max = 0;
	byte_t syntax[RES_LEN] = { 0 };

	stream_t stm = NULL;
	bool_t rt = 1;

	TRY_CATCH;

	dcm = dcm_scp(pb->slot);

	if (!dcm)
	{
		raise_user_error(NULL, NULL);
	}

	pd = (dcm_block_t*)xmem_alloc(sizeof(dcm_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/dcm.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	dcm_set_options(dcm, DCM_OPT_AET_SCP, (void*)"ANY-SCP", a_xslen("ANY-SCP"));

	while (dcm_status(dcm) != _DCM_STATUS_RELEASE)
	{
		if (!dcm_recv(dcm, &pd->pdv))
			continue;

		dcm_get_options(dcm, DCM_OPT_SYNTAX_TRANSFER, syntax, RES_LEN);
		dcm_get_options(dcm, DCM_OPT_DATA_MAXINUM, (void*)&n_max, sizeof(long));
		b_big = sop_is_bigendian(syntax);

		//is command set
		if (pd->pdv.ctl & 0x01)
		{
			stm = stream_alloc(dcm_bio(dcm));

			if (!parse_dicm_command_from_stream(&pd->cmd, b_big, n_max, stm))
			{
				raise_user_error(NULL, NULL);
			}

			switch (pd->cmd.CommandField)
			{
			case DCM_CMD_C_ECHO_RQ:
				_invoke_c_echo(dcm, pd);
				break;
			}

			stream_free(stm);
			stm = NULL;
		}

		//the last pdv
		//if (pd->pdv.ctl & 0x02)
			//break;

		xmem_zero((void*)&(pd->pdv), sizeof(dcm_pdv_head_t));
	}
	
	dcm_close(dcm);
	dcm = NULL;

	if (pb->pf_track_eror)
	{
		(*pb->pf_track_eror)(pb->hand, pd->code, pd->text);
	}

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? SLOTS_INVOKE_SUCCEED : SLOTS_INVOKE_WITHINFO;

ONERROR:
	
	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (dcm)
		dcm_close(dcm);

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

	return SLOTS_INVOKE_WITHINFO;
}

