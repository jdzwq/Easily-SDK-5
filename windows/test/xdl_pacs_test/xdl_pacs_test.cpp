// xdl_pacs_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define SCP_ADDR	_T("127.0.0.1")
#define SCP_PORT	_T("8880")
//#define SCP_PORT	_T("8800")

//scp	0x00927e72 "ANY-SCP         ...	unsigned char[16]
//scu	0x00927e82 "ECHOSCU         ...	unsigned char[16]
//uid	0x00927ef0 "1.2.840.10008.3.1.1.1"	char *
//asn	0x00927f58 "1.2.840.10008.1.1"	char *
//tsn	0x00927fc0 "1.2.840.10008.1.2"	char *
//+vf	0x012ba5a8 "1.2.840.10008.1.1"	unsigned char *
//+buf	0x005ea800 "1.2.276.0.7230010.3.0.3.6.2"	unsigned char *

void test_c_echo()
{
	xhand_t pacs = NULL;
	pdv_head_t pdv = { 0 };
	dicm_command_t cmd = { 0 };

	tchar_t token[RES_LEN + 1] = { 0 };
	bool_t b_big = 0;
	dword_t n_max = 0;

	stream_t stm = NULL;
	dword_t n, dw = 0;

	TRY_CATCH;

	pacs = xpacs_scu(SCP_ADDR, SCP_PORT);
	if (!pacs)
	{
		raise_user_error(NULL, NULL);
	}

	xpacs_set_options(pacs, OPT_AET_SCP, (void*)"ANY-SCP", a_xslen("ANY-SCP"));
	xpacs_set_options(pacs, OPT_AET_SCU, (void*)"ECHOSCU", a_xslen("ECHOSCU"));
	xpacs_set_options(pacs, OPT_SYNTAX_ABSTRACT, SOP_SYNTAX_ABSTRACT, a_xslen(SOP_SYNTAX_ABSTRACT));
	xpacs_set_options(pacs, OPT_SYNTAX_TRANSFER, SOP_SYNTAX_TRANSFER, a_xslen(SOP_SYNTAX_TRANSFER));
	xpacs_set_options(pacs, OPT_CONTEXT_APPLICATION, SOP_CONTEXT_APPLICATION, a_xslen(SOP_CONTEXT_APPLICATION));
	xpacs_set_options(pacs, OPT_CONTEXT_IMPLEMENT, SOP_CONTEXT_IMPLEMENT, a_xslen(SOP_CONTEXT_IMPLEMENT));

	cmd.mask |= MSK_AffectedSOPClassUID;
	n = a_xslen(SOP_SYNTAX_ABSTRACT);
	a_xsncpy((schar_t*)cmd.AffectedSOPClassUID, SOP_SYNTAX_ABSTRACT, n);
	dw += (CMD_TAG_SIZE + CMD_LEN_SIZE + n);

	cmd.mask |= MSK_CommandField;
	cmd.CommandField = CMD_C_ECHO_RQ;
	dw += (CMD_TAG_SIZE + CMD_LEN_SIZE + 2);

	cmd.mask |= MSK_MessageID;
	cmd.MessageID = 0x01;
	dw += (CMD_TAG_SIZE + CMD_LEN_SIZE + 2);

	cmd.mask |= MSK_DataSetType;
	cmd.DataSetType = 0x0101;
	dw += (CMD_TAG_SIZE + CMD_LEN_SIZE + 2);

	cmd.mask |= MSK_GroupLength;
	cmd.GroupLength = dw;
	dw += (CMD_TAG_SIZE + CMD_LEN_SIZE + 4);

	pdv.did = 0x01;
	pdv.ctl = 0x03;
	pdv.size = PDV_SIZE_FROM_SET(dw);

	dw = pdu_size_from_pdv(&pdv, 1);
	xpacs_set_request_bytes(pacs, dw);

	if (!xpacs_send(pacs, &pdv))
	{
		raise_user_error(NULL, NULL);
	}

	b_big = sop_is_bigendian((byte_t*)SOP_SYNTAX_TRANSFER);
	n_max = 0;

	stm = stream_alloc(xpacs_bio(pacs));

	if (!format_dicm_command_to_stream(&cmd, b_big, n_max, stm))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	xmem_zero((void*)&pdv, sizeof(pdv));

	while (xpacs_recv(pacs, &pdv))
	{
		if (!pdv.size)
			break;

		stm = stream_alloc(xpacs_bio(pacs));

		if (!parse_dicm_command_from_stream(&cmd, b_big, n_max, stm))
		{
			raise_user_error(NULL, NULL);
		}

		stream_free(stm);
		stm = NULL;

		//the last pdv
		if (pdv.ctl & 0x02)
			break;

		xmem_zero((void*)&pdv, sizeof(pdv));
	}

	xpacs_close(pacs);
	pacs = NULL;

	END_CATCH;

	return;
ONERROR:

	if (stm)
		stream_free(stm);

	if (pacs)
		xpacs_close(pacs);

	XDL_TRACE_LAST;

	return;
}

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	test_c_echo();

	xdl_process_uninit();

	getch();

	return 0;
}

