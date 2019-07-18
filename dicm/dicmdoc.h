/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmdoc.h | dicom document interface file

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

#ifndef _DICMDOC_H
#define _DICMDOC_H

#include "dicmdef.h"
#include "dicmattr.h"
#include "dicmsum.h"

#define DICM_MAX_POINTS		16

typedef struct _dicm_graphic_anno_t{
	tchar_t type[RES_LEN];
	bool_t fill;
	int count;
	xpoint_t points[DICM_MAX_POINTS];
}dicm_graphic_anno_t;

typedef struct _dicm_text_anno_t{
	tchar_t text[RES_LEN];
	tchar_t align[INT_LEN];
	xpoint_t points[3];
}dicm_text_anno_t;


#define DICM_CHARSET_ENG	0
#define DICM_CHARSET_UTF	1
#define DICM_CHARSET_GBK	2

/**********************************************************************************************************/
#define get_dicm_item_name_ptr(ilk)						get_dom_node_name_ptr(ilk)

#define set_dicm_item_name(ilk,token)					set_dom_node_name(ilk,token,-1)

#define get_dicm_item_tag_ptr(ilk)						get_dom_node_attr_ptr(ilk,_T("VX"),-1)

#define set_dicm_item_tag(ilk,token)					set_dom_node_attr(ilk,_T("VX"),-1,token,-1)
/***********************************************************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

	DCM_API link_t_ptr create_dicm_doc(void);

	DCM_API void destroy_dicm_doc(link_t_ptr ptr);

	DCM_API void clear_dicm_doc(link_t_ptr ptr);

	DCM_API bool_t is_dicm_doc(link_t_ptr ptr);

	DCM_API bool_t is_dicm_item(link_t_ptr ptr, link_t_ptr ilk);

	DCM_API link_t_ptr get_dicm_item(link_t_ptr ilk, const tchar_t* iname);

	DCM_API link_t_ptr get_dicm_item_by_tag(link_t_ptr ilk, const tchar_t* tag);

	DCM_API link_t_ptr insert_dicm_item(link_t_ptr ilk, link_t_ptr pos);

	DCM_API link_t_ptr insert_dicm_item_by_tag(link_t_ptr ilk, const tchar_t* tag);

	DCM_API void delete_dicm_item(link_t_ptr ilk);

	DCM_API link_t_ptr get_dicm_first_child_item(link_t_ptr ilk);

	DCM_API link_t_ptr get_dicm_last_child_item(link_t_ptr ilk);

	DCM_API link_t_ptr get_dicm_parent_item(link_t_ptr ilk);

	DCM_API link_t_ptr get_dicm_next_sibling_item(link_t_ptr ilk);

	DCM_API link_t_ptr get_dicm_prev_sibling_item(link_t_ptr ilk);

	DCM_API int get_dicm_charset(link_t_ptr ptr);

	DCM_API void clear_dicm_annotation(link_t_ptr ptr);

	DCM_API int load_dicm_graphic_annotation(link_t_ptr ptr, dicm_graphic_anno_t* pga, int max);

	DCM_API void save_dicm_graphic_annotation(link_t_ptr ptr, const dicm_graphic_anno_t* pga, int count);

	DCM_API int load_dicm_text_annotation(link_t_ptr ptr, dicm_text_anno_t* pta, int max);

	DCM_API void save_dicm_text_annotation(link_t_ptr ptr, const dicm_text_anno_t* pta, int count);

	DCM_API dword_t get_dicm_pdg(link_t_ptr ptr, byte_t* buf, dword_t max);

	DCM_API void set_dicm_pdg(link_t_ptr ptr, const byte_t* buf, dword_t len);

	DCM_API link_t_ptr create_dicm_sop(const tchar_t* sopuid);

	DCM_API void get_dicm_summary(link_t_ptr ptr, dicm_summary_t* pds);

#ifdef	__cplusplus
}
#endif


#endif	/*_DICMDOC_H*/