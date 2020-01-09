/***********************************************************************
	Easily pacs web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc pacs web api document

	@module	oss_api.cc | oss web api implement file

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

#include "pacsnet.h"


typedef struct _pacs_t{
	xhand_head head;		//head for xhand_t
	int type;

	xhand_t bio;

	bool_t b_response;
	bool_t b_request;

	byte_t tag[1];
	byte_t ver[2];
	byte_t aed[16];
	byte_t aes[16];

	byte_t ack[1];
	byte_t src[1];
	byte_t diag[1];
}pacs_t;

/***********************************************************************************************/

bool_t pacs_parse_request(xhand_t pacs)
{
	pacs_t* ppacs = (pacs_t*)pacs;

	dword_t n;
	unsigned short us = 0;
	unsigned long ul = 0;
	unsigned char rev[32];
	dword_t pdv_len = 0;

	stream_t stm = NULL;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	TRY_CATCH;

	stm = stream_alloc(ppacs->bio);

	//pdu type
	n = 1;
	stream_read_bytes(stm, (void*)ppacs->tag, &n);
	if (n < 1)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
	}

	//rev1
	n = 1;
	stream_read_bytes(stm, (void*)rev, &n);
	if (n < 1)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
	}

	//pdu length
	n = 4;
	stream_read_bytes(stm, (void*)&ul, &n);
	if (n < 4)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
	}
	bytes_turn((void*)&ul, 4);

	switch (ppacs->tag[0])
	{
	case PDU_ASSOCIATE_RQ:
		//version
		n = 2;
		stream_read_bytes(stm, (void*)ppacs->ver, &n);
		if (n < 2)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev2
		n = 2;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 2)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//destient ae
		n = 16;
		stream_read_bytes(stm, (void*)ppacs->aed, &n);
		if (n < 16)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//soruce ae
		n = 16;
		stream_read_bytes(stm, (void*)ppacs->aes, &n);
		if (n < 16)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev3
		n = 32;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 32)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	case PDU_ASSOCIATE_AC:
		//version
		n = 2;
		stream_read_bytes(stm, (void*)ppacs->ver, &n);
		if (n < 2)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev2
		n = 2;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 2)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev3
		n = 16;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 16)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev4
		n = 16;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 16)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev3
		n = 32;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 32)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	case PDU_ASSOCIATE_RJ:
		//rev
		n = 1;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//result
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->ack, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//source
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->src, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//diagnose
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->diag, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	case PDU_P_DATA_TF:
		//pdv
		pdv_len = ul;
		break;
	case PDU_A_RELEASE_RQ:
		//rev
		n = 2;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 2)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	case PDU_A_RELEASE_RP:
		//rev
		n = 2;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	case PDU_A_ABORT_RQ:
		//rev
		n = 1;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev
		n = 1;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//source
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->src, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//diagnose
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->diag, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

int pacs_format_request(xhand_t pacs, byte_t* buf, dword_t max)
{
	pacs_t* ppacs = (pacs_t*)pacs;
	dword_t total = 0;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	return total;
}

bool_t pacs_parse_response(xhand_t pacs, byte_t* data, dword_t len)
{
	pacs_t* ppacs = (pacs_t*)pacs;

	dword_t n;
	unsigned short us = 0;
	unsigned long ul = 0;
	unsigned char rev[32];

	dword_t pdv_len = 0;
	stream_t stm = NULL;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	TRY_CATCH;

	stm = stream_alloc(ppacs->bio);

	//pdu type
	n = 1;
	stream_read_bytes(stm, (void*)ppacs->tag, &n);
	if (n < 1)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
	}

	//rev1
	n = 1;
	stream_read_bytes(stm, (void*)rev, &n);
	if (n < 1)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
	}

	//pdu length
	n = 4;
	stream_read_bytes(stm, (void*)&ul, &n);
	if (n < 4)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
	}
	bytes_turn((void*)&ul, 4);

	switch (ppacs->tag[0])
	{
	case PDU_ASSOCIATE_AC:
		//version
		n = 2;
		stream_read_bytes(stm, (void*)ppacs->ver, &n);
		if (n < 2)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev2
		n = 2;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 2)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev3
		n = 16;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 16)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev4
		n = 16;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 16)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//rev3
		n = 32;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 32)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	case PDU_ASSOCIATE_RJ:
		//rev
		n = 1;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//result
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->ack, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//source
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->src, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//diagnose
		n = 1;
		stream_read_bytes(stm, (void*)ppacs->diag, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	case PDU_P_DATA_TF:
		//pdv
		pdv_len = ul;
		break;
	case PDU_A_RELEASE_RP:
		//rev
		n = 2;
		stream_read_bytes(stm, (void*)rev, &n);
		if (n < 1)
		{
			raise_user_error(_T("dcm_parse"), _T("not dicm pdu"));
		}
		ul -= n;

		//pdv size
		pdv_len = ul;
		break;
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (stm)
		stream_free(stm);

	return 0;
}

int pacs_format_response(xhand_t pacs, byte_t* buf, dword_t max)
{
	pacs_t* ppacs = (pacs_t*)pacs;
	dword_t total = 0;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	return total;
}

/************************************************************************************/

xhand_t pacs_scu(const tchar_t* dst_ae, const tchar_t* src_ae, const tchar_t* addr, const tchar_t* port)
{
	pacs_t* ppacs = NULL;
	xhand_t bio = NULL;

	TRY_CATCH;

	bio = xtcp_cli(xstos(port), addr);
	if (!bio)
	{
		raise_user_error(NULL, NULL);
	}

	ppacs = (pacs_t*)xmem_alloc(sizeof(pacs_t));
	ppacs->head.tag = _HANDLE_PACS;

	ppacs->type = _PACS_TYPE_SCU;
#ifdef _UNICODE
	ucs_to_mbs(dst_ae, -1, ppacs->aed, 16);
#else
	xmem_copy((void*)dst_ae,ppacs->aed,16);
#endif
	ppacs->bio = bio;

	END_CATCH;

	return (xhand_t)ppacs;
ONERROR:

	if (bio)
		xtcp_close(bio);

	if (ppacs)
		xmem_free(ppacs);

	return NULL;
}

xhand_t pacs_scp(xhand_t bio)
{
	net_addr_t na = { 0 };
	pacs_t* ppacs = NULL;

	TRY_CATCH;

	if (!bio)
	{
		raise_user_error(_T("pacs_scp"), _T("invalid bio"));
	}

	ppacs = (pacs_t*)xmem_alloc(sizeof(pacs_t));
	ppacs->head.tag = _HANDLE_PACS;
	ppacs->type = _PACS_TYPE_SCP;

	ppacs->bio = bio;

	END_CATCH;

	return (xhand_t)ppacs;
ONERROR:

	if (ppacs)
		xmem_free(ppacs);

	return NULL;
}

void pacs_close(xhand_t pacs)
{
	pacs_t* ppacs = (pacs_t*)pacs;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);
	XDL_ASSERT(ppacs->type == _PACS_TYPE_SCU || ppacs->type == _PACS_TYPE_SCP);

	if (ppacs->type == _PACS_TYPE_SCU && ppacs->bio)
		xtcp_close(ppacs->bio);

	xmem_free(ppacs);
}

unsigned short pacs_get_addr(xhand_t pacs, tchar_t* addr)
{
	pacs_t* ppacs = (pacs_t*)pacs;
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	xsocket_addr(xtcp_socket(ppacs->bio), &na);
	conv_addr(&na, &port, addr);

	return port;
}

bool_t pacs_send_response(xhand_t pacs)
{
	pacs_t* ppacs = (pacs_t*)pacs;
	
	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	if (ppacs->type == _PACS_TYPE_SCP)
	{
		return ppacs->b_response;
	}

	return 0;
}

bool_t pacs_recv_response(xhand_t pacs)
{
	pacs_t* ppacs = (pacs_t*)pacs;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	if (ppacs->type == _PACS_TYPE_SCU)
	{
		return ppacs->b_response;
	}

	return 0;
}

bool_t pacs_send_request(xhand_t pacs)
{
	pacs_t* ppacs = (pacs_t*)pacs;
	
	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	if (ppacs->type == _PACS_TYPE_SCU)
	{
		return  ppacs->b_request;
	}

	return 0;
}

bool_t pacs_recv_request(xhand_t pacs)
{
	pacs_t* ppacs = (pacs_t*)pacs;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	if (ppacs->type == _PACS_TYPE_SCP)
	{
		return ppacs->b_request;
	}

	return 0;
}

bool_t pacs_send(xhand_t pacs, const byte_t* data, dword_t len)
{
	pacs_t* ppacs = (pacs_t*)pacs;
	stream_t stm = NULL;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	TRY_CATCH;

	if (ppacs->type == _PACS_TYPE_SCU && !ppacs->b_request)
	{
		raise_user_error(_T("pacs_send"), _T("pacs not send request"));
	}

	if (ppacs->type == _PACS_TYPE_SCP && !ppacs->b_response)
	{
		raise_user_error(_T("pacs_send"), _T("pacs not recv response"));
	}

	stm = stream_alloc(ppacs->bio);

	if (!stream_write_bytes(stm, data, len))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

bool_t pacs_recv(xhand_t pacs,byte_t* buf,dword_t *pb)
{
	pacs_t* ppacs = (pacs_t*)pacs;
	stream_t stm = NULL;

	XDL_ASSERT(pacs && pacs->tag == _HANDLE_PACS);

	TRY_CATCH;

	if (ppacs->type == _PACS_TYPE_SCU && !ppacs->b_response)
	{
		raise_user_error(_T("pacs_recv"), _T("pacs not recv response"));
	}

	if (ppacs->type == _PACS_TYPE_SCP && !ppacs->b_request)
	{
		raise_user_error(_T("pacs_recv"), _T("pacs not send request"));
	}

	if (!stream_read_bytes(stm, buf, pb))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

