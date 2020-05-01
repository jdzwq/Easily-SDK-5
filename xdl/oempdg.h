/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pdg document

	@module	oempdg.h | interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#ifndef _OEMPDG_H
#define	_OEMPDG_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_PDG

typedef struct _pdg_file_t{
	/*file info header*/
	dword_t flag;		/*file flag*/
	dword_t fsize;	/*file size*/
	dword_t reserved;
	dword_t offset;	/*image bytes position*/
}pdg_file_t;

typedef struct _pdg_info_t{
	/*pdg info header*/
	dword_t isize;	/*info struct size*/
	dword_t width;	/*image cols*/
	dword_t height;	/*image rows*/
	dword_t bytes;	/*image bytes*/
	sword_t psize;	/*bytes per pixel*/
	sword_t psign;	/*bytes is sign*/

	int win_width;		/*windows width*/
	int win_center;	/*windows center*/
	int win_inter;		/*rescale intercept*/
	int win_slope;		/*rescale slope*/
	int xmm_pixel;	/*x pixel spaceing*/
	int ymm_pixel;	/*x pixel spaceing*/

	byte_t moda[16]; /*modality*/
}pdg_info_t;

typedef struct _pdg_bits_t{
	sword_t bit_all;
	sword_t bit_len;
	sword_t bit_pos;
}pdg_bits_t;

#define PDG_FLAG		0x4d434944 /*"D","I","C","M"*/
#define PDG_FILEHEADER_SIZE		16
#define PDG_INFOHEADER_SIZE		60

#define PDGFILEHEADERPTR(p)			((byte_t*)p)
#define PDGINFOHEADERPTR(p)			((byte_t*)p + PDG_FILEHEADER_SIZE)


#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API dword_t xpdg_set_header(const pdg_file_t* pfi, const pdg_info_t* pbi, byte_t* buf, dword_t max);

	EXP_API dword_t xpdg_get_header(pdg_file_t* pfi, pdg_info_t* pbi, const byte_t* src, dword_t len);

	EXP_API	dword_t xpdg_revert(const byte_t* pdg_buf, dword_t pdg_len, byte_t* bmp_buf, dword_t bmp_size);

	EXP_API	dword_t xpdg_convert(const byte_t* bmp_buf, dword_t bmp_len, byte_t* pdg_buf, dword_t pdg_size);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_PDG*/

#endif	/*OEMPDG_H */

