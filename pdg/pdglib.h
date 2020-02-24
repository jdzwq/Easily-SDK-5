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

#ifndef __PDGLIB_H__
#define __PDGLIB_H__


typedef struct _pdg_file_t{
	/*file info header*/
	unsigned long flag;		/*file flag*/
	unsigned long fsize;	/*file size*/
	unsigned long reserved;
	unsigned long offset;	/*image bytes position*/
}pdg_file_t;

typedef struct _pdg_info_t{
	/*pdg info header*/
	unsigned long isize;	/*info struct size*/
	unsigned long width;	/*image cols*/
	unsigned long height;	/*image rows*/
	unsigned long bytes;	/*image bytes*/
	unsigned short psize;	/*bytes per pixel*/
	unsigned short psign;	/*bytes is sign*/

	long win_width;		/*windows width*/
	long win_center;	/*windows center*/
	long win_inter;		/*rescale intercept*/
	long win_slope;		/*rescale slope*/
	long xmm_pixel;	/*x pixel spaceing*/
	long ymm_pixel;	/*x pixel spaceing*/

	unsigned char moda[16]; /*modality*/
}pdg_info_t;

#define PDG_FLAG		0x4d434944 /*"D","I","C","M"*/
#define PDG_FILEHEADER_SIZE		16
#define PDG_INFOHEADER_SIZE		60

#define PDGFILEHEADERPTR(p)			((unsigned char*)p)
#define PDGINFOHEADERPTR(p)			((unsigned char*)p + PDG_FILEHEADER_SIZE)

#ifdef __cplusplus
	extern "C" {
#endif

	extern unsigned long pdg_set_header(const pdg_file_t* pfi, const pdg_info_t* pbi, unsigned char* buf, unsigned long max);

	extern unsigned long pdg_get_header(pdg_file_t* pfi, pdg_info_t* pbi, const unsigned char* src, unsigned long len);

#ifdef __cplusplus
}
#endif

#endif
