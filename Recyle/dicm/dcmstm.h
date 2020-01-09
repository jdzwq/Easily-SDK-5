/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom stream document

	@module	dcmstm.h | dicom stream interface file

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


#ifndef _DCMSTM_H
#define	_DCMSTM_H

#include "dcmdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

	DCM_API bool_t dicm_to_stream(const secu_desc_t* psd, const tchar_t* fname, xhand_t stm);

#ifdef	__cplusplus
}
#endif

#endif	/* _DCMCON_H */

