/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tkdb defination document

	@module	tkdb.h | definition interface file

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


#ifndef _TKDB_H
#define	_TKDB_H

#include "tkdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

EXP_API tk_db_t tkdb_create(const tchar_t* dpath, const tchar_t* dname);

EXP_API void tkdb_destroy(tk_db_t hdb);

EXP_API bool_t tkdb_save(tk_db_t hdb, variant_t key, object_t val);

EXP_API bool_t tkdb_load(tk_db_t hdb, variant_t key, object_t val);

EXP_API bool_t tkdb_clean(tk_db_t hdb, variant_t key);


#ifdef	__cplusplus
}
#endif


#endif	/* _TKDB_H */

