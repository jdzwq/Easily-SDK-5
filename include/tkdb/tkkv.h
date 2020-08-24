/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tkdb defination document

	@module	tkkv.h | definition interface file

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


#ifndef _TKKV_H
#define	_TKKV_H

#include "tkdef.h"

#define TKKV_MASK_PERSIST		0x00000001
#define TKKV_MASK_UPDATED		0x00000002
#define TKKV_MASK_DELETED		0x00000004

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API tk_kv_t tkkv_create(tk_db_t hdb);

EXP_API void tkkv_destroy(tk_kv_t hkv);

EXP_API void tkkv_write(tk_kv_t hkv, variant_t key, object_t val);

EXP_API bool_t tkkv_read(tk_kv_t hkv, variant_t key, object_t val);

EXP_API bool_t tkkv_update(tk_kv_t hkv, variant_t key, object_t val);

EXP_API void tkkv_attach(tk_kv_t hkv, variant_t key, object_t val);

EXP_API object_t tkkv_detach(tk_kv_t hkv, variant_t key);

EXP_API void tkkv_delete(tk_kv_t hkv, variant_t key);

EXP_API void tkkv_flush(tk_kv_t hkv);


#ifdef	__cplusplus
}
#endif


#endif	/* _TKKV_H */

