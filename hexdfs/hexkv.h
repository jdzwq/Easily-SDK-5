/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hexdfs defination document

	@module	hexkv.h | definition interface file

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


#ifndef _HEXKV_H
#define	_HEXKV_H

#include "hexdef.h"

#define HEXKV_MASK_PERSIST		0x00000001
#define HEXKV_MASK_UPDATED		0x00000002
#define HEXKV_MASK_DELETED		0x00000004

#ifdef	__cplusplus
extern "C" {
#endif

	HEX_API hex_obj_t hexkv_create(hex_obj_t hdb);

	HEX_API void hexkv_destroy(hex_obj_t hkv);

	HEX_API void hexkv_write(hex_obj_t hkv, variant_t key, object_t val);

	HEX_API bool_t hexkv_read(hex_obj_t hkv, variant_t key, object_t val);

	HEX_API bool_t hexkv_update(hex_obj_t hkv, variant_t key, object_t val);

	HEX_API void hexkv_attach(hex_obj_t hkv, variant_t key, object_t val);

	HEX_API object_t hexkv_detach(hex_obj_t hkv, variant_t key);

	HEX_API void hexkv_delete(hex_obj_t hkv, variant_t key);

	HEX_API void hexkv_flush(hex_obj_t hkv);


#ifdef	__cplusplus
}
#endif


#endif	/* _HEXKV_H */

