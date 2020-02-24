/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom dom document

	@module	dcmdom.h | dicom dom interface file

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


#ifndef _DCMDOM_H
#define	_DCMDOM_H

#include "dcmdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

	DCM_API bool_t dicm_to_dom(const secu_desc_t* psd, const tchar_t* fname, LINKPTR dom);

	DCM_API bool_t dicm_from_dom(const secu_desc_t* psd, const tchar_t* fname, LINKPTR dom);

	DCM_API int dicm_dom_charset(LINKPTR dom);

	DCM_API void dicm_dom_clear_annotation(LINKPTR dom);

	DCM_API int dicm_dom_load_graphic_annotation(LINKPTR dom, dicm_graphic_anno_t* pga, int max);

	DCM_API void dicm_dom_save_graphic_annotation(LINKPTR dom, const dicm_graphic_anno_t* pga, int count);

	DCM_API int dicm_dom_load_text_annotation(LINKPTR dom, dicm_text_anno_t* pta, int max);

	DCM_API void dicm_dom_save_text_annotation(LINKPTR dom, const dicm_text_anno_t* pta, int count);

	DCM_API void dicm_dom_sc_image(LINKPTR dom);

#ifdef	__cplusplus
}
#endif

#endif	/* _DCMDOM_H */

