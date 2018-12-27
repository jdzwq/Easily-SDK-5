/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmpdg.c | dicom document implement file

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

#include "dicmpdg.h"
#include "dicmctx.h"
#include "dicmtag.h"

typedef struct _PDG_PARSE_PARAM{
	union{
		dicm_context_file* ph;
		dicm_conext_command* pc;
		dicm_context_dataset* pd;
	};
	pdg_file_t df;
	pdg_info_t di;
	pdg_bits_t bo;
	byte_t** pp;
	dword_t* pn;
}PDG_PARSE_PARAM;

static bool_t _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	stream_t stm = (stream_t)rp;

	return stream_read_bytes(stm, buf, pb);
}

static void _on_error(void* ep, const tchar_t* errtext)
{

}


static bool_t _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	PDG_PARSE_PARAM* pa = (PDG_PARSE_PARAM*)pp;
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
		if (DICM_IS_SENS_VR(vr) && pa->pd->n_CharSet == DICM_CHARSET_UTF)
		{
			n = utf8_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			utf8_to_ucs(vf, vl, tval, n);
		}
		else
		{
			n = mbs_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			mbs_to_ucs(vf, vl, tval, n);
		}
	}

	switch (tag)
	{
	case 0x00080060:
		a_xsncpy(pa->di.moda, vf, 16);
		break;
	case 0x00280002: //image colors
		pa->di.psize = xstos(tval);
		break;
	case 0x00280010: //image rows
		pa->di.height = xstol(tval);
		break;
	case 0x00280011: //image cols
		pa->di.width = xstol(tval);
		break;
	case 0x00280030: //pixel spaceing
		if (!is_null(tval))
		{
			tmp = tval;
			pa->di.xmm_pixel = xstof(tmp);
			while (*tmp != _T('/') && *tmp != _T('\\') && *tmp != _T('\0'))
				tmp++;
			if (*tmp != _T('\0'))
				tmp++;
			pa->di.ymm_pixel = xstof(tmp);
		}
		break;
	case 0x00280100: //bit size
		pa->bo.bit_all = xstos(tval);
		break;
	case 0x00280101: //bit valid
		pa->bo.bit_len = xstos(tval);
		break;
	case 0x00280102: //bit high
		pa->bo.bit_pos = xstos(tval);
		break;
	case 0x00280103: //is pimg->bit_sig
		pa->di.psign = xstos(tval);
		break;
	case 0x00281050: //windows center
		pa->di.win_center = (long)(xstof(tval) * 10000);
		break;
	case 0x00281051: //windows width
		pa->di.win_width = (long)(xstof(tval) * 10000);
		break;
	case 0x00281052: //rescale Intercept
		pa->di.win_inter = (long)(xstof(tval) * 10000);
		break;
	case 0x00281053: //rescale Slope
		pa->di.win_slope = (long)(xstof(tval) * 10000);
		break;
	case 0x7FE00010: //image bytes
		pa->di.psize *= (pa->bo.bit_all / 8);
		pa->df.flag = 0x4d434944;
		pa->df.fsize = pa->di.width * pa->di.height * pa->di.psize;
		pa->df.offset = sizeof(pdg_file_t) + sizeof(pdg_info_t);
		
		*(pa->pp) = buffer_realloc(pa->pp, pa->df.fsize);

		*(pa->pn) = dicm_fill_pdg(*(pa->pp), pa->df.fsize, &pa->df, &pa->di, &pa->bo, vf, vl);
		
		break;
	}

	xmem_free(tval);

	return 1;
}

dword_t parse_dicm_pdg_from_stream(byte_t** pbuf, dword_t op, stream_t stm)
{
	PDG_PARSE_PARAM pp = { 0 };
	bool_t b;
	dword_t total = 0;

	if (op & DICM_OPERA_HEAD)
	{
		pp.ph = dicm_alloc_head();
		pp.pp = pbuf;
		pp.pn = &total;

		dicm_set_head_read(pp.ph, (void*)stm, _on_read);
		dicm_set_head_parse(pp.ph, (void*)&pp, NULL);

		b = dicm_parse_head(pp.ph);
		
		dicm_free_head(pp.ph);
	}
	else if (op & DICM_OPERA_COMMAND)
	{
		pp.pc = dicm_alloc_command();
		pp.pp = pbuf;
		pp.pn = &total;

		dicm_set_command_read(pp.pc, (void*)stm, _on_read);
		dicm_set_command_parse(pp.pc, (void*)&pp, NULL, NULL);

		b = dicm_parse_command(pp.pc);

		dicm_free_command(pp.pc);
	}
	else
	{
		b = 1;
	}

	if (!b)
		return 0;

	if (op & DICM_OPERA_DATASET)
	{
		pp.pd = dicm_alloc_dataset();
		pp.pp = pbuf;
		pp.pn = &total;

		dicm_set_dataset_read(pp.pd, (void*)stm, _on_read);
		dicm_set_dataset_parse(pp.pd, (void*)&pp, NULL, _on_parse_data, NULL, NULL, NULL, NULL);

		b = dicm_parse_dataset(pp.pd);

		dicm_free_dataset(pp.pd);
	}
	else
	{
		b = 1;
	}

	return (b)? total : 0;
}

/******************************************************************************************************/
dword_t dicm_fill_pdg(byte_t* buf, int max, pdg_file_t* pdf, pdg_info_t* pdi, pdg_bits_t* pbo, const byte_t* vf, dword_t vl)
{
	dword_t dw, i, j;
	const byte_t* pval;
	byte_t* pbm;

	unsigned short us;
	short ss;
	unsigned char uc;
	char sc;

	memcpy((void*)buf, (void*)pdf, sizeof(pdg_file_t));
	memcpy((void*)(buf + sizeof(pdg_file_t)), (void*)pdi, sizeof(pdg_info_t));
	pbm = buf + sizeof(pdg_file_t) + sizeof(pdg_info_t);

	pval = vf;
	dw = 0;

	for (i = 0; i < pdi->height; i++)
	{
		for (j = 0; j < pdi->width; j++)
		{
			if (pdi->psize == 1)
			{
				if (pdi->psign)
					sc = (char)(pval[0]);
				else
					uc = (unsigned char)(pval[0]);

				if (pbo->bit_len < pbo->bit_all)
				{
					if (pbo->bit_pos < 7)
					{
						if (pdi->psign)
						{
							sc = sc << (pbo->bit_all - pbo->bit_len);
							sc = sc >> (pbo->bit_all - pbo->bit_len);
						}
						else
						{
							uc = uc << (pbo->bit_all - pbo->bit_len);
							uc = uc >> (pbo->bit_all - pbo->bit_len);
						}
					}
				}

				if (pdi->psign)
				{
					pbm[dw] = sc;
				}
				else
				{
					pbm[dw] = uc;
				}
			}
			else if (pdi->psize == 2)
			{
				if (pdi->psign)
					memcpy((void*)&ss, (void*)pval, 2);
				else
					memcpy((void*)&us, (void*)pval, 2);

				if (pbo->bit_len < pbo->bit_all)
				{
					if (pbo->bit_pos < 15)
					{
						if (pdi->psign)
						{
							ss = ss << (pbo->bit_all - pbo->bit_len);
							ss = ss >> (pbo->bit_all - pbo->bit_len);
						}
						else
						{
							us = us << (pbo->bit_all - pbo->bit_len);
							us = us >> (pbo->bit_all - pbo->bit_len);
						}
					}
					else
					{
						if (pdi->psign)
							ss = ss >> (pbo->bit_all - pbo->bit_len);
						else
							us = us >> (pbo->bit_all - pbo->bit_len);
					}
				}

				if (pdi->psign)
				{
					pbm[dw] = GETLBYTE(ss);
					pbm[dw + 1] = GETHBYTE(ss);
				}
				else
				{
					pbm[dw] = GETLBYTE(us);
					pbm[dw + 1] = GETHBYTE(us);
				}
			}
			else if (pdi->psize == 3)
			{
				pbm[dw] = pval[0];
				pbm[dw + 1] = pval[1];
				pbm[dw + 2] = pval[2];
			}

			pval += pdi->psize;
			dw += pdi->psize;

			if (dw >= vl)
				break;
		}
		if (dw >= vl)
			break;
	}

	return dw;
}

float dicm_get_pdg_win_width(const byte_t* pdg)
{
	float fw;

	pdg += sizeof(pdg_file_t);

	fw = (float)((double)GET_DWORD_LIT(pdg, 20) / 10000);

	return fw;
}

void dicm_set_pdg_win_width(byte_t* pdg, float fw)
{
	unsigned long ul;

	pdg += sizeof(pdg_file_t);

	ul = (unsigned long)(fw * 10000);

	PUT_DWORD_LIT(pdg, 20, ul);
}

float dicm_get_pdg_win_center(const byte_t* pdg)
{
	float fw;

	pdg += sizeof(pdg_file_t);

	fw = (float)((double)GET_DWORD_LIT(pdg, 24) / 10000);

	return fw;
}

void dicm_set_pdg_win_center(byte_t* pdg, float fw)
{
	unsigned long ul;

	pdg += sizeof(pdg_file_t);

	ul = (unsigned long)(fw * 10000);

	PUT_DWORD_LIT(pdg, 24, ul);
}

float dicm_get_pdg_xmm_pixel(const byte_t* pdg)
{
	float fw;

	pdg += sizeof(pdg_file_t);

	fw = (float)((double)GET_DWORD_LIT(pdg, 36) / 10000);

	return fw;
}

void dicm_set_pdg_xmm_pixel(byte_t* pdg, float fw)
{
	unsigned long ul;

	pdg += sizeof(pdg_file_t);

	ul = (unsigned long)(fw * 10000);

	PUT_DWORD_LIT(pdg, 36, ul);
}

float dicm_get_pdg_ymm_pixel(const byte_t* pdg)
{
	float fw;

	pdg += sizeof(pdg_file_t);

	fw = (float)((double)GET_DWORD_LIT(pdg, 40) / 10000);

	return fw;
}

void dicm_set_pdg_ymm_pixel(byte_t* pdg, float fw)
{
	unsigned long ul;

	pdg += sizeof(pdg_file_t);

	ul = (unsigned long)(fw * 10000);

	PUT_DWORD_LIT(pdg, 40, ul);
}