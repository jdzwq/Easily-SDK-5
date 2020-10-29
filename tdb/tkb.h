/***********************************************************************
	Easily tkb 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tkb defination document

	@module	tkb.h | definition interface file

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


#ifndef _TKB_H
#define	_TKB_H

#include "tdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

EXP_API t_kb_t tkb_create(const tchar_t* dpath, const tchar_t* dname);

EXP_API void tkb_destroy(t_kb_t hdb);

EXP_API bool_t tkb_save(t_kb_t hdb, variant_t key, object_t val);

EXP_API bool_t tkb_load(t_kb_t hdb, variant_t key, object_t val);

EXP_API bool_t tkb_clean(t_kb_t hdb, variant_t key);


#ifdef	__cplusplus
}
#endif


#endif	/* _TKB_H */

