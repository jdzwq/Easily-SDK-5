/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hexdfs defination document

	@module	hexdef.h | definition interface file

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


#ifndef _HEXDEF_H
#define	_HEXDEF_H

#include <xdl.h>


typedef enum{
	HEX_OBJ_KV = 1,
	HEX_OBJ_DB = 2
}HEXOBJ;

typedef struct _hex_obj_hdr{
	sword_t tag;
	sword_t lru;
}hex_obj_hdr;

typedef hex_obj_hdr*	hex_obj_t;

#endif	/* _HEXDEF_H */

