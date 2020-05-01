/***********************************************************************
	Easily MDED 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc mded document

	@module	mdbag.h | dicom document interface file

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


#ifndef _MEDBAG_H
#define	_MEDBAG_H

#include "meddef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API bool_t	load_mded_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);

	EXP_API bool_t	save_mded_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname);



#ifdef	__cplusplus
}
#endif

#endif	/* _MEDBAG_H */

