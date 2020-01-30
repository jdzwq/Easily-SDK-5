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

#include <stdlib.h>

#include "pdglib.h"

#define PUT_UC(buf,off,n)		(buf[off] = (unsigned char)((n) & 0xFF))
#define PUT_US(buf,off,n)		(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF))
#define PUT_UL(buf,off,n)		(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+2] = (unsigned char) (((n) >> 16) & 0xFF), buf[off+3] = (unsigned char) (((n) >> 24) & 0xFF))

#define GET_UC(buf,off)			((unsigned char)(buf[off] & 0xFF))
#define GET_US(buf,off)			(unsigned short)((buf[off] & 0x00FF) | ((buf[off + 1] << 8) & 0xFF00))
#define GET_UL(buf,off)			(unsigned int)((buf[off] & 0x000000FF) | ((buf[off + 1] << 8) & 0x0000FF00) | ((buf[off + 2] << 16) & 0x00FF0000) | ((buf[off + 3] << 24) & 0xFF000000))

/******************************************************************************************************************/

unsigned long pdg_set_header(const pdg_file_t* pfi, const pdg_info_t* pbi, unsigned char* buf, unsigned long max)
{
	unsigned char* pb;

	if (buf)
	{
		pb = PDGFILEHEADERPTR(buf);
		PUT_UL(pb, 0, pfi->flag); 
		PUT_UL(pb, 4, pfi->fsize);
		PUT_UL(pb, 8, 0);
		PUT_UL(pb, 12, pfi->offset);

		pb = PDGINFOHEADERPTR(buf);
		PUT_UL(pb, 0, PDG_INFOHEADER_SIZE);
		PUT_UL(pb, 4, pbi->width);
		PUT_UL(pb, 8, pbi->height);
		PUT_UL(pb, 12, pbi->bytes);
		PUT_US(pb, 16, pbi->psize);
		PUT_US(pb, 18, pbi->psign);
		PUT_UL(pb, 20, pbi->win_width); 
		PUT_UL(pb, 24, pbi->win_center);
		PUT_UL(pb, 28, pbi->win_inter);
		PUT_UL(pb, 32, pbi->win_slope);
		PUT_UL(pb, 36, pbi->xmm_pixel);
		PUT_UL(pb, 40, pbi->ymm_pixel);

		memcpy((void*)(pb + 44), (void*)pbi->moda, 16);
	}

	return PDG_FILEHEADER_SIZE + PDG_INFOHEADER_SIZE;
}

unsigned long pdg_get_header(pdg_file_t* pfi, pdg_info_t* pbi, const unsigned char* src, unsigned long len)
{
	unsigned char* pb;

	if (!src || !len)
		return 0;

	pb = PDGFILEHEADERPTR(src);
	pfi->flag = GET_UL(pb, 0);
	if (pfi->flag != PDG_FLAG)
		return 0;

	pfi->fsize = GET_UL(pb, 4);
	//GET_UL(pb, 8, 0);
	pfi->offset = GET_UL(pb, 12);

	pb = PDGINFOHEADERPTR(src);
	pbi->isize = GET_UL(pb, 0);
	pbi->width = GET_UL(pb, 4);
	pbi->height = GET_UL(pb, 8);
	pbi->bytes = GET_UL(pb, 12);
	pbi->psize = GET_US(pb, 16);
	pbi->psign = GET_US(pb, 18);
	pbi->win_width = (long)GET_UL(pb, 20); 
	pbi->win_center = (long)GET_UL(pb, 24);
	pbi->win_inter = (long)GET_UL(pb, 28);
	pbi->win_slope = (long)GET_UL(pb, 32);
	pbi->xmm_pixel = (long)GET_UL(pb, 36);
	pbi->ymm_pixel = (long)GET_UL(pb, 40);

	memcpy((void*)pbi->moda, (void*)(pb + 44), 16);

	return PDG_FILEHEADER_SIZE + PDG_INFOHEADER_SIZE;
}
