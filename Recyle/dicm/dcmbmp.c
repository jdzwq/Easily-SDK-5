/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom bmp document

	@module	dcmbmp.c | dicom bmp implement file

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

#include "dcmbmp.h"
#include "dcmraw.h"

typedef struct _BMP_PARSE_PARAM{
	dicm_context_t* pdcm;
	dicm_image_t* pimg;
}BMP_PARSE_PARAM;

static bool_t __callback _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	XHANDLE xf = (XHANDLE)rp;

	return xfile_read(xf, buf, pb);
}

static void __callback _on_error(void* ep, const tchar_t* errtext)
{

}

static int __callback _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	BMP_PARSE_PARAM* pa = (BMP_PARSE_PARAM*)pp;
	tchar_t* tval = NULL;
	tchar_t* tmp;
	dword_t n;

	float fl;
	double fd;
	short ss;
	long sl;
	unsigned short us;
	unsigned long ul;

	if (vr[0] == 'F' && vr[1] == 'L') //4
	{
		memcpy((void*)&fl, (void*)vf, sizeof(float));
		tval = xsalloc(NUM_LEN);
		n = ftoxs(fl, tval, NUM_LEN);
	}
	else if (vr[0] == 'F' && vr[1] == 'D') //8
	{
		memcpy((void*)&fd, (void*)vf, sizeof(double));
		tval = xsalloc(NUM_LEN);
		n = numtoxs(fd, tval, NUM_LEN);
	}
	else if (vr[0] == 'S' && vr[1] == 'S') //2
	{
		memcpy((void*)&ss, (void*)vf, sizeof(short));
		tval = xsalloc(NUM_LEN);
		n = stoxs(ss, tval, NUM_LEN);
	}
	else if (vr[0] == 'S' && vr[1] == 'L') //4
	{
		memcpy((void*)&sl, (void*)vf, sizeof(long));
		tval = xsalloc(NUM_LEN);
		n = ltoxs(sl, tval, NUM_LEN);
	}
	else if (vr[0] == 'U' && vr[1] == 'S') //2
	{
		memcpy((void*)&us, (void*)vf, sizeof(unsigned short));
		tval = xsalloc(NUM_LEN);
		n = ustoxs(us, tval, NUM_LEN);
	}
	else if (vr[0] == 'U' && vr[1] == 'L') //4
	{
		memcpy((void*)&ul, (void*)vf, sizeof(unsigned long));
		tval = xsalloc(NUM_LEN);
		n = ultoxs(ul, tval, NUM_LEN);
	}
	else if ((vr[0] == '\0' && vr[1] == '\0') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'S' && vr[1] == 'Q'))
	{
		tval = NULL;
	}
	else
	{
		if (IS_SENS_VR(vr) && pa->pdcm->n_CharSet == DICM_CHARSET_UTF)
		{
			n = utf8_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			utf8_to_ucs(vf, vl, tval, n);
		}
		else
		{
			n = gb2312_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			gb2312_to_ucs(vf, vl, tval, n);
		}
	}

	switch (tag)
	{
	case 0x00080060:
		xsncpy(pa->pimg->dcm_mod, tval, 7);
		break;
	case 0x00280002: //image colors
		pa->pimg->bmp_clr = xstos(tval);
		break;
	case 0x00280010: //image rows
		pa->pimg->bmp_rows = xstol(tval);
		break;
	case 0x00280011: //image cols
		pa->pimg->bmp_cols = xstol(tval);
		break;
	case 0x00280030: //pixel spaceing
		if (!is_null(tval))
		{
			tmp = tval;
			pa->pimg->xmm_pixel = xstof(tmp);
			while (*tmp != _T('/') && *tmp != _T('\\') && *tmp != _T('\0'))
				tmp++;
			if (*tmp != _T('\0'))
				tmp++;
			pa->pimg->ymm_pixel = xstof(tmp);
		}
		break;
	case 0x00280100: //bit size
		pa->pimg->bit_all = xstos(tval);
		break;
	case 0x00280101: //bit valid
		pa->pimg->bit_len = xstos(tval);
		break;
	case 0x00280102: //bit high
		pa->pimg->bit_pos = xstos(tval);
		break;
	case 0x00280103: //is pimg->bit_sig
		pa->pimg->bit_sig = xstos(tval);
		break;
	case 0x00281050: //windows center
		pa->pimg->win_center = xstof(tval);
		break;
	case 0x00281051: //windows width
		pa->pimg->win_width = xstof(tval);
		break;
	case 0x00281052: //rescale Intercept
		pa->pimg->win_inter = xstof(tval);
		break;
	case 0x00281053: //rescale Slope
		pa->pimg->win_slope = xstof(tval);
		break;
	case 0x7FE00010: //image bytes
		if (vl)
		{
			pa->pimg->bytes = vl;
			pa->pimg->pixels = (byte_t*)xmem_alloc(vl);
			xmem_copy((void*)pa->pimg->pixels, (void*)vf, vl);
		}
		break;
	}

	xmem_free(tval);

	return 0;
}

void dicm_image_free(dicm_image_t* pdi)
{
	if (pdi->pixels)
		xmem_free(pdi->pixels);

	xmem_free(pdi);
}

dicm_image_t* dicm_image_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	XHANDLE xf = NULL;
	BMP_PARSE_PARAM pp = { 0 };
	dicm_context_t* pdcm = NULL;
	dicm_image_t* pimg = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, 0);
	if (!xf)
	{
		raise_user_error(_T("dicm_file_to_image"), _T("open dicm file failed"));
	}

	pdcm = dicm_alloc();
	pimg = (dicm_image_t*)xmem_alloc(sizeof(dicm_image_t));
	
	pp.pdcm = pdcm;
	pp.pimg = pimg;

	dicm_set_read(pdcm, (void*)xf, _on_read);
	dicm_set_parse(pdcm, (void*)&pp, NULL, _on_parse_data, NULL, NULL, NULL, NULL);

	dicm_parse(pdcm);

	pimg->min_width = pimg->win_center - pimg->win_width / 2;
	pimg->max_width = pimg->win_center + pimg->win_width / 2;

	xfile_close(xf);
	xf = NULL;

	dicm_free(pdcm);
	pdcm = NULL;

	if (!pimg->bytes)
	{
		raise_user_error(_T("dicm_file_to_image"), _T("parse dicm file failed"));
	}

	END_CATCH;

	return pimg;

ONERROR:

	if (xf)
		xfile_close(xf);

	if (pdcm)
		dicm_free(pdcm);

	if (pimg)
	{
		if (pimg->pixels)
			xmem_free(pimg->pixels);

		xmem_free(pimg);
	}

	return NULL;
}

dicm_image_t* dicm_image_from_dom(LINKPTR dom)
{
	dicm_image_t* pimg = NULL;
	LINKPTR nlk;

	const tchar_t* pBase = NULL;
	dword_t elen;
	const tchar_t* tmp;

	pimg = (dicm_image_t*)xmem_alloc(sizeof(dicm_image_t));

	nlk = get_dom_first_child_node(dom);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("SamplesPerPixel"), -1, 1) == 0) //image colors
		{
			pimg->bmp_clr = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Rows"), -1, 1) == 0) //image rows
		{
			pimg->bmp_rows = xstol(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Columns"), -1, 1) == 0) //image cols
		{
			pimg->bmp_cols = xstol(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelSpacing"), -1, 1) == 0) //pixel space
		{
			tmp = get_dom_node_text_ptr(nlk);
			if (!is_null(tmp))
			{
				pimg->xmm_pixel = xstof(tmp);
				while (*tmp != _T('/') && *tmp != _T('\\') && *tmp != _T('\0'))
					tmp++;
				if (*tmp != _T('\0'))
					tmp++;
				pimg->ymm_pixel = xstof(tmp);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsAllocated"), -1, 1) == 0) //bit size
		{
			pimg->bit_all = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsStored"), -1, 1) == 0) //bit valid
		{
			pimg->bit_len = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("HighBit"), -1, 1) == 0) //bit high
		{
			pimg->bit_pos = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelRepresentation"), -1, 1) == 0) //is sign
		{
			pimg->bit_sig = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowCenter"), -1, 1) == 0) //windows center
		{
			pimg->win_center = xstof(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowWidth"), -1, 1) == 0) //windows width
		{
			pimg->win_width = xstof(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleIntercept"), -1, 1) == 0) //rescale Intercept
		{
			pimg->win_inter = xstof(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleSlope"), -1, 1) == 0) //rescale Slope
		{
			pimg->win_slope = xstof(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelData"), -1, 0) == 0) //image bytes
		{
			pBase = get_dom_node_text_ptr(nlk);
		}

		nlk = get_dom_next_sibling_node(nlk);
	}

	elen = xslen(pBase);

	if (!pBase || !elen)
	{
		xmem_free(pimg);
		return 0;
	}

	pimg->min_width = pimg->win_center - pimg->win_width / 2;
	pimg->max_width = pimg->win_center + pimg->win_width / 2;

	pimg->bytes = base64_decode(pBase, elen, NULL, MAX_LONG);
	pimg->pixels = (byte_t*)xmem_alloc(pimg->bytes);
	base64_decode(pBase, elen, pimg->pixels, pimg->bytes);

	return pimg;
}

void dicm_image_to_dom(const dicm_image_t* pimg, LINKPTR dom)
{
	LINKPTR nlk;
	tchar_t vf[NUM_LEN];
	tchar_t vl[INT_LEN];
	const tchar_t* vr;
	dword_t len;
	tchar_t* buf;

	nlk = get_dom_first_child_node(dom);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("SamplesPerPixel"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), pimg->bmp_clr);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Rows"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), pimg->bmp_rows);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Columns"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), pimg->bmp_cols);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelSpacing"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%.4f/%.4f"), pimg->xmm_pixel, pimg->ymm_pixel);
			set_dom_node_text(nlk, vf, -1);

			xsprintf(vl, _T("%d"), xslen(vf));
			set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsAllocated"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), pimg->bit_all);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsStored"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), pimg->bit_len);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("HighBit"), -1, 0) == 0)
		{
			ustoxs(pimg->bit_pos, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelRepresentation"), -1, 0) == 0)
		{
			ustoxs(pimg->bit_sig, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowCenter"), -1, 0) == 0)
		{
			ftoxs(pimg->win_center, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowWidth"), -1, 0) == 0)
		{
			ftoxs(pimg->win_width, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleIntercept"), -1, 0) == 0)
		{
			ftoxs(pimg->win_inter, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleSlope"), -1, 0) == 0)
		{
			ftoxs(pimg->win_slope, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelData"), -1, 0) == 0)
		{
			len = base64_encode(pimg->pixels, pimg->bytes, NULL, MAX_LONG);
			buf = xsalloc(len + 1);
			base64_encode(pimg->pixels, pimg->bytes, buf, len);
			attach_dom_node_text(nlk, buf);
		}

		nlk = get_dom_next_sibling_node(nlk);
	}
	
}

dword_t dicm_image_to_bmp_bytes(const dicm_image_t* pimg, byte_t* buf, dword_t max)
{
	byte_t* pval;
	dword_t vlen;
	dword_t dw;

	bitmap_info_t bi = { 0 };
	dword_t iMapCount = 0;
	dword_t iRowSize = 0;
	unsigned long dFileSize = 0;
	unsigned long dTotal = 0;

	unsigned char* pbr;
	unsigned char* pbm;

	float p;
	int i, j;
	unsigned char c;
	int dbs;
	unsigned short us;
	short ss;
	unsigned char uc;
	char sc;

	bi.width = pimg->bmp_cols;
	bi.height = pimg->bmp_rows;
	
	if (pimg->bmp_clr == 1)
		bi.clrbits = 8;
	else
		bi.clrbits = 24;

	dFileSize = bitmap_file_size(&bi);

	if (!buf)
		return dFileSize;

	bitmap_set_header(&bi, buf, max);

	iMapCount = bitmap_quad_count(&bi);

	pbr = BITMAPQUADHEADERPTR(buf);
	for (i = 0; i < (int)iMapCount; i++)
	{
		BITMAP_FILL_QUAD(pbr, 0, i, i, i);

		pbr += BITMAP_RGBQUAD_SIZE;
	}
	pbm = pbr;

	dbs = pimg->bit_all / 8 * pimg->bmp_clr;
	dw = 0;

	iRowSize = bitmap_row_size(&bi);
	//seek to image end line
	dTotal = bitmap_image_size(&bi);

	vlen = pimg->bytes;
	pval = pimg->pixels;
	for (i = 0; i < pimg->bmp_rows; i++)
	{
		dTotal -= iRowSize;

		for (j = 0; j < pimg->bmp_cols; j++)
		{
			if (pimg->bmp_clr == 1)
			{
				if (dbs == 2)
				{
					if (pimg->bit_sig)
						memcpy((void*)&ss, (void*)pval, 2);
					else
						memcpy((void*)&us, (void*)pval, 2);

					if (pimg->bit_len < pimg->bit_all)
					{
						if (pimg->bit_pos < 15)
						{
							if (pimg->bit_sig)
							{
								ss = ss << (pimg->bit_all - pimg->bit_len);	
								ss = ss >> (pimg->bit_all - pimg->bit_len);
							}
							else
							{
								us = us << (pimg->bit_all - pimg->bit_len);
								us = us >> (pimg->bit_all - pimg->bit_len);
							}
						}
						else
						{
							if (pimg->bit_sig)
								ss = ss >> (pimg->bit_all - pimg->bit_len);
							else
								us = us >> (pimg->bit_all - pimg->bit_len);
						}
					}
				}
				else
				{
					if (pimg->bit_sig)
						sc = (char)(pval[0]);
					else
						uc = (unsigned char)(pval[0]);

					if (pimg->bit_len < pimg->bit_all)
					{
						if (pimg->bit_pos < 7)
						{
							if (pimg->bit_sig)
							{
								sc = sc << (pimg->bit_all - pimg->bit_len);
								sc = sc >> (pimg->bit_all - pimg->bit_len);
							}
							else
							{
								uc = uc << (pimg->bit_all - pimg->bit_len);
								uc = uc >> (pimg->bit_all - pimg->bit_len);
							}
						}
					}
				}

				if (dbs == 2)
				{
					if (pimg->bit_sig)
					{
						p = (float)(ss * pimg->win_slope) + pimg->win_inter;
					}
					else
					{
						p = (float)(us * pimg->win_slope) + pimg->win_inter;
					}
				}
				else
				{
					if (pimg->bit_sig)
					{
						p = (float)(sc * pimg->win_slope) + pimg->win_inter;
					}
					else
					{
						p = (float)(uc * pimg->win_slope) + pimg->win_inter;
					}
				}

				//if (pimg->min_width > p)
					//pimg->min_width = p;
				//if (pimg->max_width < p)
					//pimg->max_width = p;

				p = (float)((p - (pimg->win_center - pimg->win_width / 2)) * 255 / pimg->win_width);

				if (p > 255)
					c = 255;
				else if (p < 0)
					c = 0;
				else
					c = (unsigned char)p;

				pbm[dTotal + j] = c; //gray
			}
			else if (pimg->bmp_clr == 3)
			{
				BITMAP_FILL_RGB((pbm + dTotal + j * 3), pval[0], pval[1], pval[2]);
			}

			pval += dbs;
			dw += dbs;
			if (dw >= vlen)
				break;
		}
		if (dw >= vlen)
			break;
	}

	return dFileSize;
}

dicm_image_t* dicm_image_from_bmp_bytes(const byte_t* buf, dword_t len)
{
	dicm_image_t* pdi;
	bitmap_info_t bi = { 0 };

	int dbs;
	unsigned char* pbm;
	unsigned char* pbr;
	unsigned char* pvl;
	dword_t dRowSize, dImgPos;
	dword_t i, j;
	dword_t total = 0;

	pdi = (dicm_image_t*)xmem_alloc(sizeof(dicm_image_t));

	bitmap_get_header(&bi, buf, len);

	pdi->bmp_cols = bi.width;
	pdi->bmp_rows = bi.height;;

	if (bi.clrbits == 8)
		pdi->bmp_clr = 1;
	else
		pdi->bmp_clr = 3;

	pdi->bit_all = 8;
	pdi->bit_len = 8;
	pdi->bit_pos = 7;

	dbs = pdi->bit_all / 8 * pdi->bmp_clr;

	pbr = BITMAPQUADHEADERPTR(buf);
	pbm = buf + bitmap_image_offset(&bi);

	dRowSize = bitmap_row_size(&bi);
	//seek to image end line
	dImgPos = bitmap_image_size(&bi);

	pdi->bytes = pdi->bmp_cols * pdi->bmp_cols * dbs;
	pdi->pixels = (byte_t*)xmem_alloc(pdi->bytes);

	pvl = pdi->pixels;
	for (i = 0; i < pdi->bmp_rows; i++)
	{
		dImgPos -= dRowSize;

		for (j = 0; j < pdi->bmp_cols; j++)
		{
			if (total + dbs > len)
				break;

			if (pdi->bmp_clr == 1)
			{
				pvl[0] = pbm[dImgPos + j];
			}
			else if (pdi->bmp_clr == 3)
			{
				pvl[0] = pbm[dImgPos + j];
				pvl[1] = pbm[dImgPos + j + 1];
				pvl[2] = pbm[dImgPos + j + 2];
			}

			pvl += dbs;
		}
	}

	return pdi;
}

dword_t dicm_image_to_jpg_bytes(const dicm_image_t* pimg, byte_t* buf, dword_t max)
{
	dword_t n;
	byte_t* bmp_buf;

	if (!pimg->bytes)
		return 0;

	n = dicm_image_to_bmp_bytes(pimg, NULL, MAX_LONG);
	bmp_buf = (byte_t*)xmem_alloc(n);
	dicm_image_to_bmp_bytes(pimg, bmp_buf, n);

	n = xjpg_compress(bmp_buf, n, buf, max);
	xmem_free(bmp_buf);

	return n;
}

dword_t dicm_image_to_png_bytes(const dicm_image_t* pimg, byte_t* buf, dword_t max)
{
	dword_t n;
	byte_t* bmp_buf;

	if (!pimg->bytes)
		return 0;

	n = dicm_image_to_bmp_bytes(pimg, NULL, MAX_LONG);
	bmp_buf = (byte_t*)xmem_alloc(n);
	dicm_image_to_bmp_bytes(pimg, bmp_buf, n);

	n = xpng_compress(bmp_buf, n, buf, max);
	xmem_free(bmp_buf);

	return n;
}