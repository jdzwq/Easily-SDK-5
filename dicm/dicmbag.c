/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmbag.c | dicom document implement file

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

#include "dicmbag.h"
#include "dicmdom.h"
#include "dicmctx.h"


bool_t load_dicm_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	xhand_t xf;
	stream_t stm;
	bool_t b;

	xf = xfile_open(psd, fname, FILE_OPEN_READ);
	if (!xf)
		return 0;

	stm = stream_alloc(xf);

	b = parse_dicm_doc_from_stream(ptr, DICM_OPERA_HEAD | DICM_OPERA_DATASET, stm);

	stream_free(stm);
	xfile_close(xf);

	return b;
}

bool_t save_dicm_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	xhand_t xf;
	stream_t stm;
	bool_t b;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
		return 0;

	stm = stream_alloc(xf);

	b = format_dicm_doc_to_stream(ptr, DICM_OPERA_HEAD | DICM_OPERA_DATASET, stm);

	stream_free(stm);
	xfile_close(xf);

	return b;
}

bool_t load_dicm_summary_from_file(dicm_summary_t* pds, const secu_desc_t* psd, const tchar_t* fname)
{
	xhand_t xf;
	stream_t stm;
	bool_t b;

	xf = xfile_open(psd, fname, FILE_OPEN_READ);
	if (!xf)
		return 0;

	stm = stream_alloc(xf);

	b = parse_dicm_summary_from_stream(pds, DICM_OPERA_HEAD | DICM_OPERA_DATASET, stm);

	stream_free(stm);
	xfile_close(xf);

	return b;
}

