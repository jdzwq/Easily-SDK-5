/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pdu document

	@module	netpdu.h | pdu interface file

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

#ifndef _NETPDU_H
#define _NETPDU_H

#include "xdldef.h"

#define MTU_MAX_SIZE		1500
#define MTU_MID_SIZE		576
#define MTU_MIN_SIZE		46
	
#define ETH_HDR_SIZE		14
#define ETH_TAL_SIZE		4
#define ETH_PKG_SIZE		(ETH_HDR_SIZE + MTU_MAX_SIZE + ETH_TAL_SIZE)
#define ETH_PDU_SIZE		(ETH_PKG_SIZE - ETH_HDR_SIZE - ETH_TAL_SIZE)

#define MAX_ETH_SIZE		(ETH_HDR_SIZE + MTU_MAX_SIZE + ETH_TAL_SIZE)
#define MID_ETH_SIZE		(ETH_HDR_SIZE + MTU_MID_SIZE + ETH_TAL_SIZE)

#define IP_HDR_SIZE			20
#define IP_PKG_SIZE			ETH_PDU_SIZE
#define IP_PDU_SIZE			(IP_PKG_SIZE - IP_HDR_SIZE)

#define TCP_HDR_SIZE		20
#define TCP_PKG_SIZE		IP_PDU_SIZE
#define TCP_PDU_SIZE		(TCP_PKG_SIZE - TCP_HDR_SIZE)

#define UDP_HDR_SIZE		8
#define UDP_PKG_SIZE		(MTU_MID_SIZE - IP_HDR_SIZE)
#define UDP_PDU_SIZE		(UDP_PKG_SIZE - UDP_HDR_SIZE)

typedef enum{
	PDU_TYPE_ETH = 0,
	PDU_TYPE_IP = 1,
	PDU_TYPE_UDP = 2,
	PDU_TYPE_TCP = 4,
	PDU_TYPE_RTP = 5
}PDU_TYPE;

typedef struct _PDU{
	int type;
	byte_t* frm;
	byte_t* pkg;
	byte_t* pdu;
}PDU;


#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API PDU* pdu_alloc(int type);

	XDL_API void pdu_free(PDU* pdu);

#ifdef	__cplusplus
}
#endif


#endif /*NETPDU_H*/