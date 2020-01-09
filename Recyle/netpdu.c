/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl pdu document

	@module	netpdu.c | xdl pdu implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#include "netpdu.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"


PDU* pdu_alloc(int type)
{
	PDU* pdu;

	pdu = (PDU*)xmem_alloc(sizeof(PDU));
	pdu->type = type;

	switch (type)
	{
	case PDU_TYPE_IP:
		pdu->frm = (byte_t*)xmem_alloc(MAX_ETH_SIZE);
		pdu->pkg = pdu->frm + ETH_HDR_SIZE;
		pdu->pdu = pdu->pkg + IP_HDR_SIZE;

		break;
	case PDU_TYPE_UDP:
		pdu->frm = (byte_t*)xmem_alloc(MID_ETH_SIZE);
		pdu->pkg = pdu->frm + ETH_HDR_SIZE + IP_HDR_SIZE;
		pdu->pdu = pdu->pkg + UDP_HDR_SIZE;
		break;
	case PDU_TYPE_TCP:
		pdu->frm = (byte_t*)xmem_alloc(MAX_ETH_SIZE);
		pdu->pkg = pdu->frm + ETH_HDR_SIZE + IP_HDR_SIZE;
		pdu->pdu = pdu->pkg + TCP_HDR_SIZE;
		break;
	}

	return pdu;
}

void pdu_free(PDU* pdu)
{
	if (pdu->frm)
		xmem_free(pdu->frm);

	xmem_free(pdu);
}
