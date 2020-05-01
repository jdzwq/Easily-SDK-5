/***********************************************************************
	Easily MDED 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc medded defination document

	@module	meddec.h | meddec definition interface file

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


#ifndef _MEDDEC_H
#define	_MEDDEC_H

#include "meddef.h"


#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API bool_t decode_mded_doc_from_stream(link_t_ptr ptr, dword_t op, stream_t stm);

#ifdef	__cplusplus
}
#endif


#endif	/* _MEDDEC_H */

