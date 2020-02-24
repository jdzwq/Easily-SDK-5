﻿/***********************************************************************
	Easily MQTT 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc mqtt defination document

	@module	mqttdef.h | definition interface file

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


#ifndef _MQTTDEF_H
#define	_MQTTDEF_H

#include <xdl.h>

#if defined(_OS_WINDOWS) && defined(_USRDLL)
#define MQ_API __declspec(dllexport)
#else
#define MQ_API extern
#endif




#endif	/* _MQTTDEF_H */

