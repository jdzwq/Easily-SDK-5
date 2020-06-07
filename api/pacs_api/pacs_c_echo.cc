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

bool_t _invoke_c_echo(pacs_t* pacs, pacs_block_t* pd)
{
	dicm_command_t cmd = { 0 };
	dword_t n, dw = 0;

	pacs_pdv_head_t pdv = { 0 };

	byte_t syntax[RES_LEN + 1] = { 0 };
	bool_t b_big = 0;
	dword_t n_max = 0;

	stream_t stm = NULL;

	TRY_CATCH;

	cmd.mask |= MSK_AffectedSOPClassUID;
	n = a_xslen((schar_t*)pd->cmd.AffectedSOPClassUID);
	a_xsncpy((schar_t*)cmd.AffectedSOPClassUID, (schar_t*)pd->cmd.AffectedSOPClassUID, n);
	dw += (DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + n);

	cmd.mask |= MSK_CommandField;
	cmd.CommandField = DICM_CMD_C_ECHO_RSP;
	dw += (DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + 2);

	cmd.mask |= MSK_MessageIDBeingRespondedTo;
	cmd.MessageIDBeingRespondedTo = pd->cmd.MessageID;
	dw += (DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + 2);

	cmd.mask |= MSK_DataSetType;
	cmd.DataSetType = 0x0101;
	dw += (DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + 2);

	cmd.mask |= MSK_Status;
	cmd.Status = 0x0000;
	dw += (DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + 2);

	cmd.mask |= MSK_GroupLength;
	cmd.GroupLength = dw;
	dw += (DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + 4);

	pdv.did = pd->pdv.did;
	pdv.ctl = 0x03;
	pdv.size = PACS_PDV_SIZE_FROM_SET(dw);

	dw = pacs_pdv_group_size(&pdv, 1);
	pacs_set_response_bytes(pacs, dw);

	if (!pacs_send(pacs, &pdv))
	{
		raise_user_error(NULL, NULL);
	}

	pacs_get_options(pacs, PACS_OPT_SYNTAX_TRANSFER, syntax, RES_LEN);
	pacs_get_options(pacs, PACS_OPT_DATA_MAXINUM, (void*)&n_max, sizeof(long));
	b_big = sop_is_bigendian(syntax);

	stm = stream_alloc(pacs_bio(pacs));

	if (!format_dicm_command_to_stream(&cmd, b_big, n_max, stm))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	xscpy(pd->code, _T("_invoke_c_echo"));
	xscpy(pd->text, _T("Succeeded"));

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(pd->code, pd->text, ERR_LEN);

	if (stm)
		stream_free(stm);

	return 0;
}
