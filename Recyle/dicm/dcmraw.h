/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom raw document

	@module	dcmraw.h | dicom raw interface file

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


#ifndef _DCMRAW_H
#define	_DCMRAW_H

#include "dcmdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

	DCM_API int dicm_parse_point(const tchar_t* token, int len, xpoint_t* ppt, int max);

	DCM_API int dicm_format_point(const xpoint_t* ppt, int count, tchar_t* buf, int max);

	DCM_API dicm_context_t* dicm_alloc(void);

	DCM_API void dicm_free(dicm_context_t* ctx);

	DCM_API void dicm_set_read(dicm_context_t* pdcm, void* rp, PF_DICM_READ pf_read);

	DCM_API void dicm_set_write(dicm_context_t* pdcm, void* wp, PF_DICM_WRITE pf_write);

	DCM_API void dicm_set_error(dicm_context_t* pdcm, void* ep, PF_DICM_ERROR pf_error);

	DCM_API void dicm_set_parse(dicm_context_t* pdcm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data, PF_DICM_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_DICM_PARSE_SEQUENCE_END pf_parse_sequence_end, PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN pf_parse_sequence_item_begin, PF_DICM_PARSE_SEQUENCE_ITEM_END pf_parse_sequence_item_end);

	DCM_API void dicm_set_format(dicm_context_t* pdcm, void* pp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data, PF_DICM_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_DICM_FORMAT_SEQUENCE_END pf_format_sequence_end, PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN pf_format_sequence_item_begin, PF_DICM_FORMAT_SEQUENCE_ITEM_END pf_format_sequence_item_end);

	DCM_API bool_t dicm_parse(dicm_context_t* pdcm);

	DCM_API bool_t dicm_format(dicm_context_t* pdcm);

	DCM_API const dicm_item_t* dicm_item_info(dword_t tag);

	DCM_API const dicm_group_t* dicm_group_info(dword_t tag);

#ifdef	__cplusplus
}
#endif

#endif	/* _DCMRAW_H */

