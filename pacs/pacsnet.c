/***********************************************************************
	Easily PACS v1.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pacs document

	@module	pacsnet.c | pacs network implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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

/***********************************************************************************************/

pacs_t* pacs_scu(xhand_t bio)
{
	pacs_t* pacs = NULL;

	if (!bio)
	{
		return NULL;
	}

	pacs = (pacs_t*)xmem_alloc(sizeof(pacs_t));

	pacs->type = _PACS_TYPE_SCU;
	pacs->bio = bio;

	pacs->ver = 0x0001;
	pacs->udm = PACS_USER_DATA_MAXINUM;

	pacs->iid = (schar_t*)xmem_alloc(RES_LEN);
	pacs->uid = (schar_t*)xmem_alloc(RES_LEN);
	pacs->asn = (schar_t*)xmem_alloc(RES_LEN);
	pacs->tsn = (schar_t*)xmem_alloc(RES_LEN);

	return (pacs_t*)pacs;
}

pacs_t* pacs_scp(xhand_t bio)
{
	net_addr_t na = { 0 };
	pacs_t* pacs = NULL;

	if (!bio)
	{
		return NULL;
	}

	pacs = (pacs_t*)xmem_alloc(sizeof(pacs_t));

	pacs->type = _PACS_TYPE_SCP;
	pacs->bio = bio;

	pacs->udm = PACS_USER_DATA_MAXINUM;

	pacs->iid = (schar_t*)xmem_alloc(RES_LEN);
	pacs->uid = (schar_t*)xmem_alloc(RES_LEN);
	pacs->asn = (schar_t*)xmem_alloc(RES_LEN);
	pacs->tsn = (schar_t*)xmem_alloc(RES_LEN);

	return (pacs_t*)pacs;
}

xhand_t pacs_bio(pacs_t* pacs)
{
	return pacs->bio;
}

static dword_t _pacs_format_pdu(pacs_t* pacs, byte_t pdu_type, dword_t pdv_size, byte_t* buf, dword_t max)
{
	dword_t total = 0;
	short n;
	unsigned long* pul;
	unsigned short* pus;
	unsigned short* pis;
	byte_t pid = 0x01;	

	//PDU USE NETWORK BYTES ORDERS (BigEndian)

	//1: pdu type
	n = 1;
	if (total + n > max)
		return total;

	if (buf)
	{
		PUT_BYTE(buf, total, pdu_type);
	}
	total += n;

	//2: rev
	n = 1;
	if (total + n > max)
		return total;

	if (buf)
	{
		PUT_BYTE(buf, total, 0x00);
	}
	total += n;

	//3-6: length
	n = 4;
	if (total + n > max)
		return total;

	if (buf)
	{
		pul = (unsigned long*)(buf + total);
		*pul = 0;
	}
	total += n;

	if (pdu_type == PACS_PDU_ASSOCIATE_RQ)
	{
		//A-ASSOCIATE-RQ PDU{
		//	1: pdu type[0x01]
		//	2: rev[0x00]
		//	3-6: length
		//	7-8: version
		//	9-10: rev
		//	11-26: called ae title
		//	27-42: calling ae title
		//	43-74: rev
		//	75-xxxx:
		//	{
		//		Application Context Item Fields{
		//		1: item type[0x10]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: text 
		//		}
		//		Presentation Context Item Fields{
		//		1: item type[0x20]
		//		2: rev[0x00]
		//		3-4: length
		//		5: Presentation Context ID
		//		6-xx: Sub Items{
		//			Abstract Syntax Sub Item Fields{
		//			1: item type[0x30]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Abstract Syntax
		//			}
		//			Transfer Syntax Sub Item Fields{
		//			1: item type[0x40]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Transfer Syntax
		//			}
		//		}
		//		User Information Item Fields{
		//		1: item type[0x50]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: Sub Items{
		//			Maximum Length Sub Item Fields{
		//			1: item type[0x51]
		//			2: rev[0x00]
		//			3-4: length
		//			5-8: Maximum Length
		//			}
		//			Implement Context Sub Item Fields{
		//			1: item type[0x52]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Implement Context
		//			}
		//		}
		//	}
		//}

		//7-8: ver
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_SWORD_NET(buf, total, pacs->ver);
			*pul += n;
		}
		total += n;

		//9-10: rev
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_SWORD_NET(buf, total, 0x0000);
			*pul += n;
		}
		total += n;

		//11-26: called ae
		n = 16;
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->scp, n);
			*pul += n;
		}
		total += n;

		//27-42: calling ae
		n = 16;
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->scu, n);
			*pul += n;
		}
		total += n;

		//43-74: rev3
		n = 32;
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->ret, n);
			*pul += n;
		}
		total += n;

		//75-xxx: 
		//Application Context Item Fields
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x10);
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pus = (unsigned short*)(buf + total);
			*pus = 0;
			*pul += n;
		}
		total += n;

		//5-xxx:  Application Context Item Text
		n = a_xslen((schar_t*)pacs->uid);
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->uid, n);
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Application Context Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pus, 2);
			pus = NULL;
		}

		//Presentation Context Item Fields
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x20);
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pus = (unsigned short*)(buf + total);
			*pus = 0;
			*pul += n;
		}
		total += n;

		//5: Presentation context ID
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, pid);
			pid += 2;
			*pus += n;
			*pul += n;
		}
		total += n;

		//6: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//7: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//8: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//9-xx: Abstract Syntax Sub-Item Fields; Transfer Syntax Sub-Item Fields;
		//Abstract Syntax Sub-Item Fields;
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x30);
			*pus += n;
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pis = (unsigned short*)(buf + total);
			*pis = 0;
			*pus += n;
			*pul += n;
		}
		total += n;

		//5-xx: Abstract Syntax
		n = a_xslen((schar_t*)pacs->asn);
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->asn, n);
			*pis += n;
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Abstract Syntax Sub-Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pis, 2);
			pis = NULL;
		}

		// Transfer Syntax Sub-Item Fields;
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x40);
			*pus += n;
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pis = (unsigned short*)(buf + total);
			*pis = 0;
			*pus += n;
			*pul += n;
		}
		total += n;

		//5-xx: Transfer Syntax
		n = a_xslen((schar_t*)pacs->tsn);
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->tsn, n);
			*pis += n;
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Transfer Syntax Sub-Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pis, 2);
			pis = NULL;
		}

		//convert length to bigendian: Presentation Context Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pus, 2);
			pus = NULL;
		}

		//User Information Item Fields
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x50);
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pus = (unsigned short*)(buf + total);
			*pus = 0;
			*pul += n;
		}
		total += n;

		//5-xx: User Information Sub-Item Fields 

		//Maximum Length Sub - Item Field
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x51);
			*pus += n;
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pis = (unsigned short*)(buf + total);
			*pis = 0;
			*pus += n;
			*pul += n;
		}
		total += n;

		//5-8: Maximum Length
		n = 4;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_DWORD_BIG(buf, total, pacs->udm);
			*pis += n;
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Maximum Length Sub-Item 
		if (buf)
		{
			bytes_turn((byte_t*)pis, 2);
			pis = NULL;
		}

		//Implement Context Sub-Item
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x52);
			*pus += n;
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pis = (unsigned short*)(buf + total);
			*pis = 0;
			*pus += n;
			*pul += n;
		}
		total += n;

		//5-xx: Implement Context ID
		n = a_xslen((schar_t*)pacs->iid);
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->iid, n);
			*pis += n;
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Implement Context Sub-Item
		if (buf)
		{
			bytes_turn((byte_t*)pis, 2);
			pis = NULL;
		}

		//convert length to bigendian: User Information Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pus, 2);
			pus = NULL;
		}

		//convert length to bigendian: A-ASSOCIATE-RQ PDU
		if (buf)
		{
			bytes_turn((byte_t*)pul, 4);
			pul = NULL;
		}
	}
	else if (pdu_type == PACS_PDU_ASSOCIATE_AC)
	{
		//A-ASSOCIATE-AC PDU{
		//	1: pdu type[0x02]
		//	2: rev[0x00]
		//	3-6: length
		//	7-8: version
		//	9-10: rev
		//	11-26: called ae title
		//	27-42: calling ae title
		//	43-74: rev
		//	75-xxxx:
		//	{
		//		Application Context Item Fields{
		//		1: item type[0x10]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: text 
		//		}
		//		Presentation Context Item Fields{
		//		1: item type[0x21]
		//		2: rev[0x00]
		//		3-4: length
		//		5: Presentation Context ID
		//		6: rev[0x00]
		//		7: result[0x00,0x01,0x02,0x03,0x04]
		//		8: rev[0x00]
		//		9-xx: Sub Items{
		//			Transfer Syntax Sub Item Fields{
		//			1: item type[0x40]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Transfer Syntax
		//			}
		//		}
		//		User Information Item Fields{
		//		1: item type[0x50]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: Sub Items{
		//			Maximum Length Sub Item Fields{
		//			1: item type[0x51]
		//			2: rev[0x00]
		//			3-4: length
		//			5-8: Maximum Length
		//			}
		//			Implement Context Sub Item Fields{
		//			1: item type[0x52]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Implement Context ID
		//			}
		//		}
		//	}
		//}
		//7-8: ver
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_SWORD_BIG(buf, total, pacs->ver);
			*pul += n;
		}
		total += n;

		//9-10: rev
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_SWORD_BIG(buf, total, 0x0000);
			*pul += n;
		}
		total += n;

		//11-26: called ae
		n = 16;
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->scp, n);
			*pul += n;
		}
		total += n;

		//27-42: calling ae
		n = 16;
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->scu, n);
			*pul += n;
		}
		total += n;

		//43-74: rev3
		n = 32;
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->ret, n);
			*pul += n;
		}
		total += n;

		//75-xxx: 
		//Application Context Item Fields
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x10);
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pus = (unsigned short*)(buf + total);
			*pus = 0;
			*pul += n;
		}
		total += n;

		//5-xxx: Application Context Item Text
		n = a_xslen((schar_t*)pacs->uid);
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->uid, n);
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendain: Application Context Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pus, 2);
			pus = NULL;
		}

		//Presentation Context Item Fields
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x21);
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pus = (unsigned short*)(buf + total);
			*pus = 0;
			*pul += n;
		}
		total += n;

		//5: Presentation-context-ID
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, pid);
			pid += 2;
			*pus += n;
			*pul += n;
		}
		total += n;

		//6: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//7: ack
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, _ASSOCIATE_ACCEPT);
			*pus += n;
			*pul += n;
		}
		total += n;

		//8: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//9-xx: Transfer Syntax Sub-Item Fields;
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x40);
			*pus += n;
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pis = (unsigned short*)(buf + total);
			*pis = 0;
			*pus += n;
			*pul += n;
		}
		total += n;

		//5-xx: Transfer Syntax
		n = a_xslen((schar_t*)pacs->tsn);
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->tsn, n);
			*pis += n;
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Transfer Syntax Sub-Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pis, 2);
			pis = NULL;
		}

		//convert length to bigendian: Presentation Context Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pus, 2);
			pus = NULL;
		}

		//User Information Item Fields
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x50);
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pus = (unsigned short*)(buf + total);
			*pus = 0;
			*pul += n;
		}
		total += n;

		//5-xx: User Information Sub-Item Fields
		//Maximum Length Sub-Item
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x51);
			*pus += n;
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pis = (unsigned short*)(buf + total);
			*pis = 0;
			*pus += n;
			*pul += n;
		}
		total += n;

		//5-8: Maximum Length
		n = 4;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_DWORD_BIG(buf, total, pacs->udm);
			*pis += n;
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Maximum Length Sub-Item Fields 
		if (buf)
		{
			bytes_turn((byte_t*)pis, 2);
			pis = NULL;
		}

		//Implement Context Sub-Item
		//1: tag
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x52);
			*pus += n;
			*pul += n;
		}
		total += n;

		//2: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pus += n;
			*pul += n;
		}
		total += n;

		//3-4: length
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			pis = (unsigned short*)(buf + total);
			*pis = 0;
			*pus += n;
			*pul += n;
		}
		total += n;

		//5-xx: Implement Context ID
		n = a_xslen((schar_t*)pacs->iid);
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pacs->iid, n);
			*pis += n;
			*pus += n;
			*pul += n;
		}
		total += n;

		//convert length to bigendian: Implement Context Sub-Item
		if (buf)
		{
			bytes_turn((byte_t*)pis, 2);
			pis = NULL;
		}

		//convert length to bigendian: User Information Item Fields
		if (buf)
		{
			bytes_turn((byte_t*)pus, 2);
			pus = NULL;
		}

		//convert length to bigendian: A-ASSOCIATE-AC PDU
		if (buf)
		{
			bytes_turn((byte_t*)pul, 4);
			pul = NULL;
		}
	}
	else if (pdu_type == PACS_PDU_ASSOCIATE_RJ)
	{
		//A-ASSOCIATE-RJ PDU{
		//	1: pdu type[0x03]
		//	2: rev[0x00]
		//	3-6: length
		//	7: rev
		//	8: result[0x01,0x02]
		//	9: source[0x01,0x02,0x03]
		//	10: diagnose[0x01~0x0a]
		//}

		//7: rev
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, 0x00);
			*pul += n;
		}
		total += n;

		//8: result
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, _REJECTED_PERMANENT);
			*pul += n;
		}
		total += n;

		//9: source
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, _ASSOCIATE_SOURCE_SCP_PDU);
			*pul += n;
		}
		total += n;

		//10: diagnose
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, _ASSOCIATE_REASON_NONE);
			*pul += n;
		}
		total += n;

		//convert length to bigendian: A-ASSOCIATE-RJ PDU
		if (buf)
		{
			bytes_turn((byte_t*)pul, 4);
			pul = NULL;
		}
	}
	else if (pdu_type == PACS_PDU_DATA_TF)
	{
		//P-DATA-TF PDU{
		//	1: pdu type[0x04]
		//	2: rev[0x00]
		//	3-6: length
		//	7-xxxx: Presentation Data Value Item Fields{
		//		1-4: length
		//		5: id
		//		6-xxxx: pdv{
		//			1: control byte
		//			2-xxxx: command set / dataset
		//		}
		//	}
		//}

		//convert length to bigendian: A_DATA-TF PDU
		if (buf)
		{
			*pul = pdv_size;
			bytes_turn((byte_t*)pul, 4);
			pul = NULL;
		}
	}
	else if (pdu_type == PACS_PDU_RELEASE_RQ)
	{
		//A-RELEASE-RQ PDU{
		//	1: pdu type[0x05]
		//	2: rev[0x00]
		//	3-6: length
		//	7-10: rev
		//}

		//7-10: rev
		n = 4;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_DWORD_BIG(buf, total, 0x00000000);
			*pul += n;
		}
		total += n;

		//convert length to bigendian: A-RELEASE-RQ PDU
		if (buf)
		{
			bytes_turn((byte_t*)pul, 4);
			pul = NULL;
		}
	}
	else if (pdu_type == PACS_PDU_RELEASE_RP)
	{
		//A-RELEASE-RP PDU{
		//	1: pdu type[0x06]
		//	2: rev[0x00]
		//	3-6: length
		//	7-10: rev
		//}

		//7-10: rev
		n = 4;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_DWORD_BIG(buf, total, 0x00000000);
			*pul += n;
		}
		total += n;

		//convert length to bigendian: A-RELEASE-RP PDU
		if (buf)
		{
			bytes_turn((byte_t*)pul, 4);
			pul = NULL;
		}
	}
	else if (pdu_type == PACS_PDU_ABORT_RQ)
	{
		//A-ABORT-RQ PDU{
		//	1: pdu type[0x07]
		//	2: rev[0x00]
		//	3-6: length
		//	7-8: rev
		//	9: source
		//	10: diagnose
		//}

		//7-8: rev
		n = 2;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_SWORD_BIG(buf, total, 0x0000);
			*pul += n;
		}
		total += n;

		//9: source
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, pacs->src[0]);
			*pul += n;
		}
		total += n;

		//9: diagnose
		n = 1;
		if (total + n > max)
			return total;

		if (buf)
		{
			PUT_BYTE(buf, total, pacs->dag[0]);
			*pul += n;
		}
		total += n;

		//convert length to bigendian: A-ABORT-RQ PDU
		if (buf)
		{
			bytes_turn((byte_t*)pul, 4);
			pul = NULL;
		}
	}

	return total;
}

static bool_t _pacs_write_pdu(pacs_t* pacs, byte_t pdu_type, dword_t pdv_size)
{
	dword_t len;
	byte_t* buf = NULL;

	stream_t stm = NULL;

	TRY_CATCH;

	len = _pacs_format_pdu(pacs, pdu_type, pdv_size, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(len);

	_pacs_format_pdu(pacs, pdu_type, pdv_size, buf, len);

	stm = stream_alloc(pacs->bio);

	if (!stream_write_bytes(stm, buf, len))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _pacs_read_pdu(pacs_t* pacs, byte_t* pdu_type, dword_t* pdv_size)
{
	dword_t n;
	unsigned short us = 0;
	unsigned long ul = 0;
	unsigned char rev[4] = { 0 };

	unsigned char it = 0;
	unsigned short is = 0;
	unsigned char* buf = NULL;

	stream_t stm = NULL;

	TRY_CATCH;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)

	stm = stream_alloc(pacs->bio);

	//1: pdu type
	n = 1;
	if (!stream_read_bytes(stm, (void*)pdu_type, &n))
	{
		raise_user_error(NULL, NULL);
	}

	//2: rev1
	n = 1;
	if (!stream_read_bytes(stm, (void*)rev, &n))
	{
		raise_user_error(NULL, NULL);
	}

	//3-6: pdu length
	n = 4;
	if (!stream_read_dword_big(stm, (void*)&ul))
	{
		raise_user_error(NULL, NULL);
	}

	if (*pdu_type == PACS_PDU_ASSOCIATE_RQ)
	{
		//A-ASSOCIATE-RQ PDU{
		//	1: pdu type[0x01]
		//	2: rev[0x00]
		//	3-6: length
		//	7-8: version
		//	9-10: rev
		//	11-26: called ae title
		//	27-42: calling ae title
		//	43-74: rev
		//	75-xxxx:
		//	{
		//		Application Context Item Fields{
		//		1: item type[0x10]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: text 
		//		}
		//		Presentation Context Item Fields{
		//		1: item type[0x20]
		//		2: rev[0x00]
		//		3-4: length
		//		5: Presentation Context ID
		//		6-xx: Sub Items{
		//			Abstract Syntax Sub Item Fields{
		//			1: item type[0x30]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Abstract Syntax
		//			}
		//			Transfer Syntax Sub Item Fields{
		//			1: item type[0x40]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Transfer Syntax
		//			}
		//		}
		//		User Information Item Fields{
		//		1: item type[0x50]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: Sub Items{
		//			Maximum Length Sub Item Fields{
		//			1: item type[0x51]
		//			2: rev[0x00]
		//			3-4: length
		//			5-8: Maximum Length
		//			}
		//			Implement Context Sub Item Fields{
		//			1: item type[0x52]
		//			2: rev[0x00]
		//			3-4: length
		//			5-8: Implement Context ID
		//			}
		//		}
		//	}
		//}

		//7-8: version
		n = 2;
		if (!stream_read_sword_big(stm, (void*)&(pacs->ver)))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//9-10: rev2
		n = 2;
		if (!stream_read_bytes(stm, (void*)rev, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//11-26: called ae
		n = 16;
		if (!stream_read_bytes(stm, (void*)pacs->scp, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//27-42: calling ae
		n = 16;
		if (!stream_read_bytes(stm, (void*)pacs->scu, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//43-74: rev3
		n = 32;
		if (!stream_read_bytes(stm, (void*)pacs->ret, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//72-xxx: 
		while (ul)
		{
			//1: item field type
			n = 1;
			if (!stream_read_bytes(stm, (void*)&it, &n))
			{
				raise_user_error(NULL, NULL);
			}
			ul -= n;

			//2: rev
			n = 1;
			if (!stream_read_bytes(stm, (void*)rev, &n))
			{
				raise_user_error(NULL, NULL);
			}
			ul -= n;

			//3-4: length
			n = 2;
			if (!stream_read_sword_big(stm, (void*)&us))
			{
				raise_user_error(NULL, NULL);
			}
			ul -= n;

			switch (it)
			{
			case 0x10: //Application Context Item Fields
				//5-xxx: 
				n = us;
				if (!stream_read_bytes(stm, (void*)pacs->uid, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				break;
			case 0x20: //Presentation Context Item Fields
				//5: Presentation-context-ID
				n = 1;
				if (!stream_read_bytes(stm, (void*)rev, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				us -= n;

				//6-8: rev
				n = 3;
				if (!stream_read_bytes(stm, (void*)rev, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				us -= n;

				//9-xx: Sub-Item Fields
				while (us)
				{
					//1: item field type
					n = 1;
					if (!stream_read_bytes(stm, (void*)&it, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//2: rev
					n = 1;
					if (!stream_read_bytes(stm, (void*)rev, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//3-4: length
					n = 2;
					if (!stream_read_sword_big(stm, (void*)&is))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					switch (it)
					{
					case 0x30: //Abstract Syntax Sub-Item Fields
						//5-xxx:
						n = is;
						if (!stream_read_bytes(stm, (void*)pacs->asn, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;
						break;
					case 0x40: //Transfer Syntax Sub-Item Fields
						//5-xxx:
						n = is;
						if (!stream_read_bytes(stm, (void*)pacs->tsn, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;
						break;
					default:
						n = is;
						buf = (byte_t*)xmem_alloc(n);
						if (!stream_read_bytes(stm, (void*)buf, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;

						xmem_free(buf);
						buf = NULL;
						break;
					}
				}

				break;
			case 0x50: //User Information Item Fields
				//5-xxx: 
				while (us)
				{
					//1: item field type
					n = 1;
					if (!stream_read_bytes(stm, (void*)&it, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//2: rev
					n = 1;
					if (!stream_read_bytes(stm, (void*)rev, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//3-4: length
					n = 2;
					if (!stream_read_sword_big(stm, (void*)&is))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					switch (it)
					{
					case 0x51: //Maximum Length Sub - Item Fields
						n = is;
						if (!stream_read_dword_big(stm, (void*)&(pacs->udm)))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;
						break;
					case 0x52: //Implement Context Sub - Item Fields
						n = is;
						if (!stream_read_bytes(stm, (void*)pacs->iid, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;
						break;
					default:
						n = is;
						buf = (byte_t*)xmem_alloc(n);
						if (!stream_read_bytes(stm, (void*)buf, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;

						xmem_free(buf);
						buf = NULL;
						break;
					}
				}
				break;
			default:
				n = us;
				buf = (byte_t*)xmem_alloc(n);
				if (!stream_read_bytes(stm, (void*)buf, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;

				xmem_free(buf);
				buf = NULL;
				break;
			}
		}
	}
	else if (*pdu_type == PACS_PDU_ASSOCIATE_AC)
	{
		//A-ASSOCIATE-AC PDU{
		//	1: pdu type[0x02]
		//	2: rev[0x00]
		//	3-6: length
		//	7-8: version
		//	9-10: rev
		//	11-26: called ae title
		//	27-42: calling ae title
		//	43-74: rev
		//	75-xxxx:
		//	{
		//		Application Context Item Fields{
		//		1: item type[0x10]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: text 
		//		}
		//		Presentation Context Item Fields{
		//		1: item type[0x21]
		//		2: rev[0x00]
		//		3-4: length
		//		5: Presentation Context ID
		//		6: rev[0x00]
		//		7: result[0x00,0x01,0x02,0x03,0x04]
		//		8: rev[0x00]
		//		9-xx: Sub Items{
		//			Transfer Syntax Sub Item Fields{
		//			1: item type[0x40]
		//			2: rev[0x00]
		//			3-4: length
		//			5-xx: Transfer Syntax
		//			}
		//		}
		//		User Information Item Fields{
		//		1: item type[0x50]
		//		2: rev[0x00]
		//		3-4: length
		//		5-xx: Sub Items{
		//			Maximum Length Sub Item Fields{
		//			1: item type[0x51]
		//			2: rev[0x00]
		//			3-4: length
		//			5-8: Maximum Length
		//			}
		//			Implement Context Sub Item Fields{
		//			1: item type[0x52]
		//			2: rev[0x00]
		//			3-4: length
		//			5-8: Implement Context ID
		//			}
		//		}
		//	}
		//}

		//7-8: version
		n = 2;
		if (!stream_read_sword_big(stm, (void*)&pacs->ver))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//9-10: rev2
		n = 2;
		if (!stream_read_bytes(stm, (void*)rev, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//11-26: called ae
		n = 16;
		if (!stream_read_bytes(stm, (void*)pacs->scp, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//27-42: soruce ae
		n = 16;
		if (!stream_read_bytes(stm, (void*)pacs->scu, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//43-74: rev3
		n = 32;
		if (!stream_read_bytes(stm, (void*)pacs->ret, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//75-xxx: 
		while (ul)
		{
			//1: item field type
			n = 1;
			if (!stream_read_bytes(stm, (void*)&it, &n))
			{
				raise_user_error(NULL, NULL);
			}
			ul -= n;

			//2: rev
			n = 1;
			if (!stream_read_bytes(stm, (void*)rev, &n))
			{
				raise_user_error(NULL, NULL);
			}
			ul -= n;

			//3-4: length
			n = 2;
			if (!stream_read_sword_big(stm, (void*)&us))
			{
				raise_user_error(NULL, NULL);
			}
			ul -= n;

			switch (it)
			{
			case 0x10: //Application Context Item Fields
				//5-xxx: 
				n = us;
				if (!stream_read_bytes(stm, (void*)pacs->uid, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				break;
			case 0x21: //Presentation Context Item Fields
				//5: Presentation-context-ID
				n = 1;
				if (!stream_read_bytes(stm, (void*)rev, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				us -= n;

				//6: rev
				n = 1;
				if (!stream_read_bytes(stm, (void*)rev, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				us -= n;

				//7: reason
				n = 1;
				if (!stream_read_bytes(stm, (void*)pacs->ack, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				us -= n;

				//8: rev
				n = 1;
				if (!stream_read_bytes(stm, (void*)rev, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;
				us -= n;

				//9-xx: Sub-Item Fields
				while (us)
				{
					//1: item field type
					n = 1;
					if (!stream_read_bytes(stm, (void*)&it, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//2: rev
					n = 1;
					if (!stream_read_bytes(stm, (void*)rev, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//3-4: length
					n = 2;
					if (!stream_read_sword_big(stm, (void*)&is))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					switch (it)
					{
					case 0x40: //Transfer Syntax Sub-Item Fields
						//5-xxx:
						n = is;
						if (!stream_read_bytes(stm, (void*)pacs->tsn, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;
						break;
					default:
						n = is;
						buf = (byte_t*)xmem_alloc(n);
						if (!stream_read_bytes(stm, (void*)buf, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;

						xmem_free(buf);
						buf = NULL;
						break;
					}
				}

				break;
			case 0x50: //User Information Item Fields
				//5-xxx: 
				while (us)
				{
					//1: item field type
					n = 1;
					if (!stream_read_bytes(stm, (void*)&it, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//2: rev
					n = 1;
					if (!stream_read_bytes(stm, (void*)rev, &n))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					//3-4: length
					n = 2;
					if (!stream_read_sword_big(stm, (void*)&is))
					{
						raise_user_error(NULL, NULL);
					}
					ul -= n;
					us -= n;

					switch (it)
					{
					case 0x51: //Maximum Length Sub - Item Fields
						n = is;
						if (!stream_read_dword_big(stm, (void*)&(pacs->udm)))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;
						break;
					case 0x52: //Implement Context Sub - Item Fields
						n = is;
						if (!stream_read_bytes(stm, (void*)pacs->iid, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;
						break;
					default:
						n = is;
						buf = (byte_t*)xmem_alloc(n);
						if (!stream_read_bytes(stm, (void*)buf, &n))
						{
							raise_user_error(NULL, NULL);
						}
						ul -= n;
						us -= n;

						xmem_free(buf);
						buf = NULL;
						break;
					}
				}
				break;
			default:
				n = us;
				buf = (byte_t*)xmem_alloc(n);
				if (!stream_read_bytes(stm, (void*)buf, &n))
				{
					raise_user_error(NULL, NULL);
				}
				ul -= n;

				xmem_free(buf);
				buf = NULL;
				break;
			}
		}
	}
	else if (*pdu_type == PACS_PDU_ASSOCIATE_RJ)
	{
		//A-ASSOCIATE-RJ PDU{
		//	1: pdu type[0x03]
		//	2: rev[0x00]
		//	3-6: length
		//	7: rev
		//	8: result[0x01,0x02]
		//	9: source[0x01,0x02,0x03]
		//	10: diagnose[0x01~0x0a]
		//}

		//7: rev
		n = 1;
		if (!stream_read_bytes(stm, (void*)rev, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//8: result
		n = 1;
		if (!stream_read_bytes(stm, (void*)pacs->ack, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//9: source
		n = 1;
		if (!stream_read_bytes(stm, (void*)pacs->src, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//10: diagnose
		n = 1;
		if (!stream_read_bytes(stm, (void*)pacs->dag, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;
	}
	else if (*pdu_type == PACS_PDU_DATA_TF)
	{
		//P-DATA-TF PDU{
		//	1: pdu type[0x04]
		//	2: rev[0x00]
		//	3-6: length
		//	7-xxxx: Presentation Data Value Item Fields{
		//		1-4: length
		//		5: id
		//		6-xxxx: pdv{
		//			1: control byte
		//			2-xxxx: command set / dataset
		//		}
		//	}
		//}
	}
	else if (*pdu_type == PACS_PDU_RELEASE_RQ)
	{
		//A-RELEASE-RQ PDU{
		//	1: pdu type[0x05]
		//	2: rev[0x00]
		//	3-6: length
		//	7-10: rev
		//}

		//7-10: rev
		n = 4;
		if (!stream_read_bytes(stm, (void*)rev, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;
	}
	else if (*pdu_type == PACS_PDU_RELEASE_RP)
	{
		//A-RELEASE-RP PDU{
		//	1: pdu type[0x05]
		//	2: rev[0x00]
		//	3-6: length
		//	7-10: rev
		//}

		//7-10: rev
		n = 4;
		if (!stream_read_bytes(stm, (void*)rev, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

	}
	else if (*pdu_type == PACS_PDU_ABORT_RQ)
	{
		//A-ABORT-RQ PDU{
		//	1: pdu type[0x07]
		//	2: rev[0x00]
		//	3-6: length
		//	7-8: rev
		//	9: source[0x01,0x02,0x03]
		//	10: diagnose[0x01~0x0a]
		//}

		//7-8: rev
		n = 2;
		if (!stream_read_bytes(stm, (void*)rev, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//9: source
		n = 1;
		if (!stream_read_bytes(stm, (void*)pacs->src, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		//10: diagnose
		n = 1;
		if (!stream_read_bytes(stm, (void*)pacs->dag, &n))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;
	}

	if (pdv_size)
	{
		*pdv_size = ul;
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _pacs_valid_associate(pacs_t* pacs)
{
	return 1;
}

static bool_t _pacs_write_pdv(pacs_t* pacs, dword_t* psize, pacs_pdv_head_t* pdv)
{
	dword_t uw, ul = 0;
	dword_t n;

	stream_t stm = NULL;

	TRY_CATCH;

	//PDV USE NETWORK BYTES ORDER (BigEndian)

	//P-DATA-TF PDU{
	//	1: pdu type[0x04]
	//	2: rev[0x00]
	//	3-6: length
	//	7-xxxx: Presentation Data Value Item Fields{
	//		1-4: length
	//		5: id
	//		6-xxxx: pdv{
	//			1: control byte
	//			2-xxxx: command set / dataset
	//		}
	//	}
	//}

	stm = stream_alloc(pacs->bio);

	ul = *psize;
	uw = 0;

	while (ul)
	{
		//7-xxxx: Presentation Data Value Item Fields
		//1-4: length
		n = 4;
		if (!stream_write_dword_big(stm, pdv->size))
		{
			raise_user_error(NULL, NULL);
		}
		uw = pdv->size;
		ul -= n;

		//5: id
		n = 1;
		if (!stream_write_bytes(stm, &(pdv->did), n))
		{
			raise_user_error(NULL, NULL);
		}
		uw -= n;
		ul -= n;

		//6: control byte
		n = 1;
		if (!stream_write_bytes(stm, &(pdv->ctl), n))
		{
			raise_user_error(NULL, NULL);
		}
		uw -= n;
		ul -= n;

		break;
	}

	*psize = (ul - uw);

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

static bool_t _pacs_read_pdv(pacs_t* pacs, dword_t* psize, pacs_pdv_head_t* pdv)
{
	dword_t n;
	unsigned long uw = 0;
	unsigned long ul = 0;

	stream_t stm = NULL;

	TRY_CATCH;

	//PDV USE NETWORK BYTES ORDER (BigEndian)

	//P-DATA-TF PDU{
	//	1: pdu type[0x04]
	//	2: rev[0x00]
	//	3-6: length
	//	7-xxxx: Presentation Data Value Item Fields{
	//		1-4: length
	//		5: id
	//		6-xxxx: pdv{
	//			1: control byte
	//			2-xxxx: command set / dataset
	//		}
	//	}
	//}

	stm = stream_alloc(pacs->bio);

	ul = *psize;

	//7-xxxx: Presentation Data Value Item Fields
	while (ul)
	{
		//1-4: length
		n = 4;
		if (!stream_read_dword_big(stm, (void*)&uw))
		{
			raise_user_error(NULL, NULL);
		}
		ul -= n;

		pdv->size = uw;

		//5: pdv id
		n = 1;
		if (!stream_read_bytes(stm, (void*)&(pdv->did), &n))
		{
			raise_user_error(NULL, NULL);
		}
		uw -= n;
		ul -= n;

		//6-xxx: pdv 
		while (uw)
		{
			//1: control byte
			n = 1;
			if (!stream_read_bytes(stm, (void*)&(pdv->ctl), &n))
			{
				raise_user_error(NULL, NULL);
			}
			uw -= n;
			ul -= n;

			break;
		}
		break;
	}

	*psize = (ul - uw);

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

bool_t pacs_send(pacs_t* pacs, pacs_pdv_head_t* pdv)
{
	byte_t pdu_type = 0;

	TRY_CATCH;

	if (pacs->type == _PACS_TYPE_SCP)
	{
		switch (pacs->status)
		{
		case _PACS_STATUS_ASSOCIATE:
			if (!_pacs_read_pdu(pacs, &pdu_type, NULL))
			{
				raise_user_error(NULL, NULL);
			}

			if (pdu_type != PACS_PDU_ASSOCIATE_RQ)
			{
				raise_user_error(_T("-1"), _T("not recv associate rq"));
			}

			if (!_pacs_valid_associate(pacs))
			{
				_pacs_write_pdu(pacs, PACS_PDU_ASSOCIATE_RJ, 0);

				raise_user_error(_T("-1"), _T("server verify associate failed"));
			}

			if (!_pacs_write_pdu(pacs, PACS_PDU_ASSOCIATE_AC, 0))
			{
				raise_user_error(NULL, NULL);
			}
			
			pacs->status = _PACS_STATUS_TRANSF;
			//do not break switch, continue send pdv
		case _PACS_STATUS_TRANSF:
			//write pdu first
			if (!_pacs_write_pdu(pacs, PACS_PDU_DATA_TF, pacs->n_response))
			{
				raise_user_error(NULL, NULL);
			}
			//then write first pdv
			if (!_pacs_write_pdv(pacs, &(pacs->n_response), pdv))
			{
				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_response) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_PENDING:
			//continue write pdv
			if (!_pacs_write_pdv(pacs, &(pacs->n_response), pdv))
			{
				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_response) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_RELEASE:
			raise_user_error(_T("-1"), _T("server released"));
			break;
		}
	}
	else
	{
		switch (pacs->status)
		{
		case _PACS_STATUS_ASSOCIATE:
			if (!_pacs_write_pdu(pacs, PACS_PDU_ASSOCIATE_RQ, 0))
			{
				raise_user_error(NULL, NULL);
			}

			if (!_pacs_read_pdu(pacs, &pdu_type, NULL))
			{
				raise_user_error(NULL, NULL);
			}

			if (pdu_type != PACS_PDU_ASSOCIATE_AC)
			{
				raise_user_error(_T("-1"), _T("server rejected"));
			}

			pacs->status = _PACS_STATUS_TRANSF;
			//do not break switch, continue send pdv
		case _PACS_STATUS_TRANSF:
			//write pdu first
			if (!_pacs_write_pdu(pacs, PACS_PDU_DATA_TF, pacs->n_request))
			{
				raise_user_error(NULL, NULL);
			}
			//then write first pdv
			if (!_pacs_write_pdv(pacs, &(pacs->n_request), pdv))
			{
				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_request) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_PENDING:
			//continue write pdv
			if (!_pacs_write_pdv(pacs, &(pacs->n_request), pdv))
			{
				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_request) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_RELEASE:
			raise_user_error(_T("-1"), _T("client released"));
			break;
		}
	}

	END_CATCH;
	
	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t pacs_recv(pacs_t* pacs, pacs_pdv_head_t* pdv)
{
	byte_t pdu_type = 0;

	TRY_CATCH;

	if (pacs->type == _PACS_TYPE_SCP)
	{
		switch (pacs->status)
		{
		case _PACS_STATUS_ASSOCIATE:
			if (!_pacs_read_pdu(pacs, &pdu_type, NULL))
			{
				raise_user_error(NULL, NULL);
			}

			if (pdu_type != PACS_PDU_ASSOCIATE_RQ)
			{
				raise_user_error(_T("-1"), _T("not associate rq"));
			}

			if (!_pacs_valid_associate(pacs))
			{
				_pacs_write_pdu(pacs, PACS_PDU_ASSOCIATE_RJ, 0);
				break;
			}
			
			_pacs_write_pdu(pacs, PACS_PDU_ASSOCIATE_AC, 0);

			pacs->status = _PACS_STATUS_TRANSF;
			//do not break switch, continue recv pdv
		case _PACS_STATUS_TRANSF:
			//read pdu first
			if (!_pacs_read_pdu(pacs, &pdu_type, &(pacs->n_request)))
			{
				raise_user_error(NULL, NULL);
			}

			if (pdu_type == PACS_PDU_ABORT_RQ)
			{
				//client aborted
				break;
			}

			if (pdu_type == PACS_PDU_RELEASE_RQ)
			{
				_pacs_write_pdu(pacs, PACS_PDU_RELEASE_RP, 0);

				//reset status
				pacs->status = _PACS_STATUS_RELEASE;
				break;
			} 
			
			if (pdu_type != PACS_PDU_DATA_TF)
			{
				raise_user_error(_T("-1"), _T("invalid pdu type"));
			}

			//then read first pdv
			if (!_pacs_read_pdv(pacs, &(pacs->n_request), pdv))
			{
				//reset request
				pacs->n_request = 0;

				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_request) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_PENDING:
			//continue read pdv
			if (!_pacs_read_pdv(pacs, &(pacs->n_request), pdv))
			{
				//reset request
				pacs->n_request = 0;
				//reset status
				pacs->status = _PACS_STATUS_TRANSF;

				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_request) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_RELEASE:
			raise_user_error(_T("-1"), _T("client released"));
			break;
		}
	}
	else
	{
		switch (pacs->status)
		{
		case _PACS_STATUS_ASSOCIATE:
			if (!_pacs_write_pdu(pacs, PACS_PDU_ASSOCIATE_RQ, 0))
			{
				raise_user_error(NULL, NULL);
			}

			if (!_pacs_read_pdu(pacs, &pdu_type, NULL))
			{
				raise_user_error(NULL, NULL);
			}

			if (pdu_type == PACS_PDU_ASSOCIATE_RJ)
			{
				//server associate rejected
				break;
			}

			if (pdu_type != PACS_PDU_ASSOCIATE_AC)
			{
				raise_user_error(_T("-1"), _T("unknown server associate ack"));
			}

			pacs->status = _PACS_STATUS_TRANSF;
			//do not break switch, continue recv pdv
		case _PACS_STATUS_TRANSF:
			if (!_pacs_read_pdu(pacs, &pdu_type, &(pacs->n_response)))
			{
				raise_user_error(NULL, NULL);
			}

			if (pdu_type == PACS_PDU_ABORT_RQ)
			{
				//server aborted
				break;
			}

			if (pdu_type == PACS_PDU_RELEASE_RQ)
			{
				_pacs_write_pdu(pacs, PACS_PDU_RELEASE_RP, 0);

				//reset status
				pacs->status = _PACS_STATUS_RELEASE;
				break;
			}

			if (pdu_type != PACS_PDU_DATA_TF)
			{
				raise_user_error(_T("-1"), _T("invalid pdu type"));
			}

			if (!_pacs_read_pdv(pacs, &(pacs->n_response), pdv))
			{
				//reset response
				pacs->n_response = 0;

				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_response) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_PENDING:
			if (!_pacs_read_pdv(pacs, &(pacs->n_response), pdv))
			{
				//reset response
				pacs->n_response = 0;
				//reset status
				pacs->status = _PACS_STATUS_TRANSF;

				raise_user_error(NULL, NULL);
			}

			pacs->status = (pacs->n_response) ? _PACS_STATUS_PENDING : _PACS_STATUS_TRANSF;
			break;
		case _PACS_STATUS_RELEASE:
			raise_user_error(_T("-1"), _T("server released"));
			break;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

int pacs_status(pacs_t* pacs)
{
	return pacs->status;
}

void pacs_abort(pacs_t* pacs, int code)
{
	if (pacs->type == _PACS_TYPE_SCP)
		pacs->n_response = 0;
	else
		pacs->n_request = 0;

	_pacs_write_pdu(pacs, PACS_PDU_ABORT_RQ, 0);
}

void pacs_close(pacs_t* pacs)
{
	byte_t pdu_type = 0;

	XDL_ASSERT(pacs->type == _PACS_TYPE_SCU || pacs->type == _PACS_TYPE_SCP);

	if (pacs->status != _PACS_STATUS_RELEASE)
	{
		if (pacs->type == _PACS_TYPE_SCU)
		{
			pacs->n_request = 0;

			//first send release-rq
			if (_pacs_write_pdu(pacs, PACS_PDU_RELEASE_RQ, 0))
			{
				//then read release-rp
				_pacs_read_pdu(pacs, &pdu_type, NULL);
			}
		}
		else
		{
			pacs->n_response = 0;

			//send abort-rq
			_pacs_write_pdu(pacs, PACS_PDU_ABORT_RQ, 0);
		}

		pacs->status = _PACS_STATUS_RELEASE;
	}

	xmem_free(pacs->iid);
	xmem_free(pacs->uid);
	xmem_free(pacs->asn);
	xmem_free(pacs->tsn);

	xmem_free(pacs);
}

void pacs_set_response_bytes(pacs_t* pacs, dword_t size)
{
	XDL_ASSERT(pacs->type == _PACS_TYPE_SCP);

	pacs->n_response = size;
}

dword_t pacs_get_request_bytes(pacs_t* pacs)
{
	XDL_ASSERT(pacs->type == _PACS_TYPE_SCP);

	return pacs->n_request;
}

void pacs_set_request_bytes(pacs_t* pacs, dword_t size)
{
	XDL_ASSERT(pacs->type == _PACS_TYPE_SCU);

	pacs->n_request = size;
}

dword_t pacs_get_response_bytes(pacs_t* pacs)
{
	XDL_ASSERT(pacs->type == _PACS_TYPE_SCU);

	return pacs->n_response;
}

dword_t pacs_get_options(pacs_t* pacs, int opt, void* buf, dword_t max)
{
	switch (opt)
	{
	case PACS_OPT_AET_SCP:
		max = (max < 16) ? max : 16;
		xmem_copy(buf, pacs->scp, max);
		return max;
	case PACS_OPT_AET_SCU:
		max = (max < 16) ? max : 16;
		xmem_copy(buf, pacs->scu, max);
		return max;
	case PACS_OPT_SYNTAX_ABSTRACT:
		max = (max < 64) ? max : 64;
		xmem_copy(buf, pacs->asn, max);
		return max;
	case PACS_OPT_SYNTAX_TRANSFER:
		max = (max < 64) ? max : 64;
		xmem_copy(buf, pacs->tsn, max);
		return max;
	case PACS_OPT_DATA_MAXINUM:
		max = (max < 4) ? max : 4;
		xmem_copy(buf, (void*)&pacs->udm, max);
		return max;
	case PACS_OPT_CONTEXT_APPLICATION:
		max = (max < 64) ? max : 64;
		xmem_copy(buf, pacs->uid, max);
		return max;
	case PACS_OPT_CONTEXT_IMPLEMENT:
		max = (max < 64) ? max : 64;
		xmem_copy(buf, pacs->iid, max);
		return max;
	}

	return 0;
}

void pacs_set_options(pacs_t* pacs, int opt, void* buf, dword_t max)
{
	switch (opt)
	{
	case PACS_OPT_AET_SCP:
		max = (max < 16) ? max : 16;
		xmem_copy(pacs->scp, buf, max);
		break;
	case PACS_OPT_AET_SCU:
		max = (max < 16) ? max : 16;
		xmem_copy(pacs->scu, buf, max);
		break;
	case PACS_OPT_SYNTAX_ABSTRACT:
		max = (max < 64) ? max : 64;
		xmem_copy(pacs->asn, buf, max);
		break;
	case PACS_OPT_SYNTAX_TRANSFER:
		max = (max < 64) ? max : 64;
		xmem_copy(pacs->tsn, buf, max);
		break;
	case PACS_OPT_DATA_MAXINUM:
		pacs->udm = *((long*)buf);
		break;
	case PACS_OPT_CONTEXT_APPLICATION:
		max = (max < 64) ? max : 64;
		xmem_copy(pacs->uid, buf, max);
		break;
	case PACS_OPT_CONTEXT_IMPLEMENT:
		max = (max < 64) ? max : 64;
		xmem_copy(pacs->iid, buf, max);
		break;
	}
}

dword_t pacs_pdv_group_size(pacs_pdv_head_t* pdv, int n)
{
	dword_t dw = 0;
	int i;

	for (i = 0; i < n; i++)
	{
		dw += (pdv[i].size + 4);

		if (pdv[i].ctl & 0x02)
			break;
	}

	return dw;
}