/***********************************************************************
	Easily tkb 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tdfs defination document

	@module	tdef.h | definition interface file

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


#ifndef _TDEF_H
#define	_TDEF_H

#include <xdl.h>


typedef enum{
	T_OBJ_KV = 1,
	T_OBJ_DB = 2
}TOBJ;

typedef struct _t_kb_hdr{
	sword_t tag;
	sword_t lru;
}t_kb_hdr;

typedef struct _t_kv_hdr{
	sword_t tag;
	sword_t lru;
}t_kv_hdr;

typedef t_kb_hdr*	t_kb_t;
typedef t_kv_hdr*	t_kv_t;

#endif	/* _TDEF_H */

