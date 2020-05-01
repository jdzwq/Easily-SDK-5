/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmpdg.h | dicom document interface file

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


#ifndef _DCMPDG_H
#define	_DCMPDG_H

#include "dcmdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API dword_t dicm_fill_pdg(byte_t* buf, int max, pdg_file_t* pdf, pdg_info_t* pdi, pdg_bits_t* pbo, const byte_t* vf, dword_t vl);

	EXP_API float dicm_get_pdg_win_width(const byte_t* pdg);

	EXP_API void dicm_set_pdg_win_width(byte_t* pdg, float fw);

	EXP_API float dicm_get_pdg_win_center(const byte_t* pdg);

	EXP_API void dicm_set_pdg_win_center(byte_t* pdg, float fw);

	EXP_API float dicm_get_pdg_xmm_pixel(const byte_t* pdg);

	EXP_API void dicm_set_pdg_xmm_pixel(byte_t* pdg, float fw);

	EXP_API float dicm_get_pdg_ymm_pixel(const byte_t* pdg);

	EXP_API void dicm_set_pdg_ymm_pixel(byte_t* pdg, float fw);

	EXP_API dword_t parse_dicm_pdg_from_stream(byte_t** pbuf, dword_t op, stream_t stm);


#ifdef	__cplusplus
}
#endif

#endif	/* _DCMPDG_H */

