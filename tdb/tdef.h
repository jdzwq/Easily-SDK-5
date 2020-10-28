/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tkdfs defination document

	@module	tkdef.h | definition interface file

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


#ifndef _TKDEF_H
#define	_TKDEF_H

#include <xdl.h>


typedef enum{
	TK_OBJ_KV = 1,
	TK_OBJ_DB = 2
}TKOBJ;

typedef struct _tk_db_hdr{
	sword_t tag;
	sword_t lru;
}tk_db_hdr;

typedef struct _tk_kv_hdr{
	sword_t tag;
	sword_t lru;
}tk_kv_hdr;

typedef tk_db_hdr*	tk_db_t;
typedef tk_kv_hdr*	tk_kv_t;

#endif	/* _TKDEF_H */

