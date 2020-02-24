/*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the MPL as stated above or under the terms of the GNU
* Library General Public License as published by the Free Software Foundation;
* either version 2 of the License, or any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Library general Public License for more
* details.
*/


#ifndef _DCMRAW_H
#define	_DCMRAW_H

#include "dcmdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

	extern dicm_context_t* dicm_alloc(void);

	extern void dicm_free(dicm_context_t* ctx);

	extern void dicm_set_read(dicm_context_t* pdcm, void* rp, PF_DICM_READ pf_read);

	extern void dicm_set_write(dicm_context_t* pdcm, void* wp, PF_DICM_WRITE pf_write);

	extern void dicm_set_error(dicm_context_t* pdcm, void* ep, PF_DICM_ERROR pf_error);

	extern void dicm_set_parse(dicm_context_t* pdcm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data, PF_DICM_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_DICM_PARSE_SEQUENCE_END pf_parse_sequence_end, PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN pf_parse_sequence_item_begin, PF_DICM_PARSE_SEQUENCE_ITEM_END pf_parse_sequence_item_end);

	extern void dicm_set_format(dicm_context_t* pdcm, void* pp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data, PF_DICM_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_DICM_FORMAT_SEQUENCE_END pf_format_sequence_end, PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN pf_format_sequence_item_begin, PF_DICM_FORMAT_SEQUENCE_ITEM_END pf_format_sequence_item_end);

	extern int dicm_parse(dicm_context_t* pdcm);

	extern int dicm_format(dicm_context_t* pdcm);

	extern const dicm_item_t* dicm_item_info(unsigned long tag);

	extern const dicm_group_t* dicm_group_info(unsigned long tag);

#ifdef	__cplusplus
}
#endif

#endif	/* _DCMRAW_H */

