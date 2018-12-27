/***********************************************************************
	Easily xDesign v3.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xDesign interface document

	@module	xDesign interface file

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

#pragma once

#include "_Module.h"


#define NODE_PROJECT			_T("project")
#define ATTR_PROJECT_RDS		_T("RDS")
#define ATTR_PROJECT_SYN		_T("SYN")
#define NODE_PROJECT_RESOURCE	_T("project-resource")
#define NODE_PROJECT_DOMAIN		_T("project-domain")

LINKPTR	Project_Alloc(void);

void	Project_Free(LINKPTR ptr_prj);

bool_t	Project_Save(LINKPTR ptr_prj, const tchar_t* szPath);

bool_t	Project_Load(LINKPTR ptr_prj, const tchar_t* szPath);

bool_t	Project_SetConfig(LINKPTR ptr_prj, const tchar_t* key, const tchar_t* val, int len);

bool_t	Project_GetConfig(LINKPTR ptr_prj, const tchar_t* key, tchar_t* buf, int max);

bool_t	Project_SetDomain(LINKPTR ptr_prj, LINKPTR ptr_tree);

bool_t	Project_GetDomain(LINKPTR ptr_prj, LINKPTR ptr_tree);

bool_t	Project_GetResource(LINKPTR ptr_prj, LINKPTR ptr_tree);

bool_t	Project_SetResource(LINKPTR ptr_prj, LINKPTR ptr_tree);