/***********************************************************************
	Easily tkb 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tkb defination document

	@module	tkv.h | definition interface file

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


#ifndef _TKV_H
#define	_TKV_H

#include "tdef.h"

#define TKV_MASK_PERSIST		0x00000001
#define TKV_MASK_UPDATED		0x00000002
#define TKV_MASK_DELETED		0x00000004

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API t_kv_t tkv_create(t_kb_t hdb);

EXP_API void tkv_destroy(t_kv_t hkv);

EXP_API void tkv_write(t_kv_t hkv, variant_t key, object_t val);

EXP_API bool_t tkv_read(t_kv_t hkv, variant_t key, object_t val);

EXP_API bool_t tkv_update(t_kv_t hkv, variant_t key, object_t val);

EXP_API void tkv_attach(t_kv_t hkv, variant_t key, object_t val);

EXP_API object_t tkv_detach(t_kv_t hkv, variant_t key);

EXP_API void tkv_delete(t_kv_t hkv, variant_t key);

EXP_API void tkv_flush(t_kv_t hkv);


#ifdef	__cplusplus
}
#endif


#endif	/* _TKV_H */

