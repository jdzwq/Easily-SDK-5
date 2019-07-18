/***********************************************************************
	Easily dimse web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dimse defination document

	@module	locapi.h | loc web api definition interface file
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

#ifndef _DIMSE_H
#define _DIMSE_H

#include <xdl.h>
#include <xds.h>

#include <dicm.h>
#include <pacs.h>

typedef struct _dimse_block_t{
	xhand_t scp;

	pacs_pdv_head_t pdv;
	dicm_command_t cmd;

	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}dimse_block_t;

#ifdef	__cplusplus
extern "C" {
#endif

	extern int STDCALL slots_invoke(const slots_block_t* pb);

#ifdef	__cplusplus
}
#endif

bool_t _invoke_c_echo(pacs_t* pacs, const dimse_block_t* pd);

#endif