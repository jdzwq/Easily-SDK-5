/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc encrypt document

	@module	oempdg.c | implement file

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

#include "oempdg.h"
#include "oembmp.h"
#include "imperr.h"

#ifdef XDL_SUPPORT_PDG

#define PUT_UC(buf,off,n)		(buf[off] = (unsigned char)((n) & 0xFF))
#define PUT_US(buf,off,n)		(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF))
#define PUT_UL(buf,off,n)		(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+2] = (unsigned char) (((n) >> 16) & 0xFF), buf[off+3] = (unsigned char) (((n) >> 24) & 0xFF))

#define GET_UC(buf,off)			((unsigned char)(buf[off] & 0xFF))
#define GET_US(buf,off)			(unsigned short)((buf[off] & 0x00FF) | ((buf[off + 1] << 8) & 0xFF00))
#define GET_UL(buf,off)			(unsigned int)((buf[off] & 0x000000FF) | ((buf[off + 1] << 8) & 0x0000FF00) | ((buf[off + 2] << 16) & 0x00FF0000) | ((buf[off + 3] << 24) & 0xFF000000))

/******************************************************************************************************************/

dword_t pdg_set_header(const pdg_file_t* pfi, const pdg_info_t* pbi, byte_t* buf, dword_t max)
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

dword_t xpdg_get_header(pdg_file_t* pfi, pdg_info_t* pbi, const byte_t* src, dword_t len)
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
	pbi->win_width = (int)GET_UL(pb, 20);
	pbi->win_center = (int)GET_UL(pb, 24);
	pbi->win_inter = (int)GET_UL(pb, 28);
	pbi->win_slope = (int)GET_UL(pb, 32);
	pbi->xmm_pixel = (int)GET_UL(pb, 36);
	pbi->ymm_pixel = (int)GET_UL(pb, 40);

	memcpy((void*)pbi->moda, (void*)(pb + 44), 16);

	return PDG_FILEHEADER_SIZE + PDG_INFOHEADER_SIZE;
}

/************************************************************************************************/

dword_t xpdg_revert(const byte_t* pdg_buf, dword_t pdg_len, byte_t* bmp_buf, dword_t bmp_size)
{
	pdg_file_t df = { 0 };
	pdg_info_t di = { 0 };

	bmp_file_t bf = { 0 };
	bmp_info_t bi = { 0 };
	dword_t dTotal, dRowSize = 0;
	byte_t* pbm = NULL;

	float p;
	dword_t i, j, dw;
	unsigned char c;
	unsigned char uc;
	char sc;
	unsigned short us;
	short ss;
	const byte_t* pval = NULL;

	float win_width, win_center, win_inter, win_slope;

	xpdg_get_header(&df, &di, pdg_buf, pdg_len);
	if (df.flag != PDG_FLAG)
		return 0;

	win_width = (float)((double)di.win_width / 10000);
	win_center = (float)((double)di.win_center / 10000);
	win_inter = (float)((double)di.win_inter / 10000);
	if (!di.win_slope)
		win_slope = 1.0;
	else
		win_slope = (float)((double)di.win_slope / 10000);

	bi.isize = BMP_INFOHEADER_SIZE;
	bi.width = di.width;
	bi.height = di.height;
	bi.planes = 1;
	if (di.psize < 3)
		bi.clrbits = 8;
	else
		bi.clrbits = 24;
	dRowSize = ((bi.width * bi.clrbits + 31) & ~31) / 8;
	bi.bytes = dRowSize * bi.height;
	if (bi.clrbits > 8)
		bi.clrused = 0;
	else
		bi.clrused = (1 << bi.clrbits);

	bf.flag = BMP_FLAG;
	bf.fsize = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + bi.clrused * BMP_RGBQUAD_SIZE + bi.bytes;
	bf.offset = BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE + bi.clrused * BMP_RGBQUAD_SIZE;

	if (!bmp_buf)
		return bf.fsize;

	pbm = bmp_buf + bf.offset;

	xbmp_set_header(&bf, &bi, bmp_buf, bmp_size);
	xbmp_fill_quad(&bi, bmp_buf, bmp_size);

	dTotal = bi.bytes;

	dw = 0;
	pval = pdg_buf + df.offset;

	for (i = 0; i < di.height; i++)
	{
		dTotal -= dRowSize;

		for (j = 0; j < di.width; j++)
		{
			if (di.psize == 1)
			{
				if (di.psign)
					sc = (char)(pval[0]);
				else
					uc = (unsigned char)(pval[0]);

				if (di.psign)
					p = (float)(sc * win_slope) + win_inter;
				else
					p = (float)(uc * win_slope) + win_inter;

				p = (float)((p - (win_center - win_width / 2)) * 255 / win_width);

				if (p > 255)
					c = 255;
				else if (p < 0)
					c = 0;
				else
					c = (unsigned char)p;

				pbm[dTotal + j] = c; //gray
			}
			else if (di.psize == 2)
			{
				if (di.psign)
					ss = (short)MAKESWORD(pval[0], pval[1]);
				else
					us = MAKESWORD(pval[0], pval[1]);

				if (di.psign)
					p = (float)(ss * win_slope) + win_inter;
				else
					p = (float)(us * win_slope) + win_inter;

				p = (float)((p - (win_center - win_width / 2)) * 255 / win_width);

				if (p > 255)
					c = 255;
				else if (p < 0)
					c = 0;
				else
					c = (unsigned char)p;

				pbm[dTotal + j] = c; //gray
			}
			else if (di.psize == 3)
			{
				pbm[dTotal + j * 3] = pval[0];
				pbm[dTotal + j * 3 + 1] = pval[1];
				pbm[dTotal + j * 3 + 2] = pval[2];
			}

			pval += di.psize;
			dw += di.psize;
			if (dw >= pdg_len)
				break;
		}
		if (dw >= pdg_len)
			break;
	}

	return bf.fsize;
}

dword_t xpdg_convert(const byte_t* bmp_buf, dword_t bmp_len, byte_t* pdg_buf, dword_t pdg_size)
{
	bmp_file_t bf = { 0 };
	bmp_info_t bi = { 0 };
	const byte_t* pbm = NULL;
	const byte_t* pbq = NULL;

	pdg_file_t df = { 0 };
	pdg_info_t di = { 0 };
	byte_t* pbd = NULL;

	dword_t i, j;
	dword_t dRowSize, dIndex, dTotal;
	unsigned short us;
	unsigned char uc;
	int n;

	if (bmp_len < BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE)
	{
		return 0;
	}

	xbmp_get_header(&bf, &bi, bmp_buf, bmp_len);

	if (bf.flag != BMP_FLAG)
	{
		return 0;
	}

	if (bmp_len < bf.fsize)
	{
		return 0;
	}

	di.isize = PDG_INFOHEADER_SIZE;
	di.width = bi.width;
	di.height = bi.height;
	if (bi.clrbits == 1 || bi.clrbits == 8)
		di.psize = 1;
	else
		di.psize = 3;
	di.bytes = di.width * di.height * di.psize;
	di.psign = 0;
	di.win_inter = 0;
	di.win_slope = 10000;
	di.xmm_pixel = 0;
	di.ymm_pixel = 0;
	strcpy((char*)di.moda, "CAPTURE");

	df.flag = PDG_FLAG;
	df.fsize = PDG_FILEHEADER_SIZE + PDG_INFOHEADER_SIZE + di.bytes;
	df.offset = PDG_FILEHEADER_SIZE + PDG_INFOHEADER_SIZE;

	if (!pdg_buf)
	{
		return df.fsize;
	}

	pbq = bmp_buf + BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE;
	pbm = bmp_buf + bf.offset;
	dRowSize = ((bi.width * bi.clrbits + 31) & ~31) / 8;
	dTotal = bi.bytes;

	pdg_set_header(&df, &di, pdg_buf, pdg_size);
	pbd = pdg_buf + df.offset;

	dIndex = 0;
	for (i = 0; i < bi.height; i++)
	{
		dTotal -= dRowSize;

		for (j = 0; j < bi.width; j++)
		{
			switch (bi.clrbits)
			{
			case 1:
				uc = pbm[dTotal + j / 8];
				uc = ((uc >> (j % 8)) & 0x01);
				if (uc)
					pbd[dIndex] = 255;
				else
					pbd[dIndex] = 0;
				dIndex++;
				break;
			case 4:
				if (j % 2)
					n = (pbm[dTotal + j - 1] & 0xF0) >> 4;
				else
					n = pbm[dTotal + j] & 0x0F;

				pbd[dIndex] = pbq[n * 4];
				pbd[dIndex + 1] = pbq[n * 4 + 1];
				pbd[dIndex + 2] = pbq[n * 4 + 2];
				dIndex += 3;
				break;
			case 8:
				pbd[dIndex] = pbm[dTotal + j];
				dIndex++;
				break;
			case 16:
				us = MAKESWORD(pbm[dTotal + j * 2], pbm[dTotal + j * 2 + 1]);
				pbd[dIndex] = (byte_t)(us & 0x001F);
				pbd[dIndex + 1] = (byte_t)((us >> 5) & 0x001F);
				pbd[dIndex + 2] = (byte_t)((us >> 10) & 0x001F);
				dIndex += 3;
				break;
			case 24:
				pbd[dIndex] = pbm[dTotal + j * 3];
				pbd[dIndex + 1] = pbm[dTotal + j * 3 + 1];
				pbd[dIndex + 2] = pbm[dTotal + j * 3 + 2];
				dIndex += 3;
				break;
			case 32:
				pbd[dIndex] = pbm[dTotal + j * 4];
				pbd[dIndex + 1] = pbm[dTotal + j * 4 + 1];
				pbd[dIndex + 2] = pbm[dTotal + j * 4 + 2];
				dIndex += 3;
				break;
			}
		}
	}

	return df.fsize;
}


#endif