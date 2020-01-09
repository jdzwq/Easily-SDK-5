/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom bmp document

	@module	dcmbmp.h | dicom bmp interface file

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


#ifndef _DCMBMP_H
#define	_DCMBMP_H

#include "dcmdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

	DCM_API dicm_image_t* dicm_image_from_file(const secu_desc_t* psd, const tchar_t* fname);

	DCM_API dicm_image_t* dicm_image_from_dom(LINKPTR dom);

	DCM_API void dicm_image_to_dom(const dicm_image_t* pimg, LINKPTR dom);

	DCM_API dicm_image_t* dicm_image_from_bmp_bytes(const byte_t* buf, dword_t len);

	DCM_API dicm_image_t* dicm_image_from_jpg_bytes(const byte_t* buf, dword_t len);

	DCM_API dicm_image_t* dicm_image_from_png_bytes(const byte_t* buf, dword_t len);

	DCM_API dword_t dicm_image_to_bmp_bytes(const dicm_image_t* pimg, byte_t* buf, dword_t max);

	DCM_API dword_t dicm_image_to_jpg_bytes(const dicm_image_t* pimg, byte_t* buf, dword_t max);

	DCM_API dword_t dicm_image_to_png_bytes(const dicm_image_t* pimg, byte_t* buf, dword_t max);

	DCM_API void dicm_image_free(dicm_image_t* pdi);

#ifdef	__cplusplus
}
#endif

#endif	/* _DCMBMP_H */

