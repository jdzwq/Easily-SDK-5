/***********************************************************************
	Easily mqtt web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc mqtt defination document

	@module	mqtt_api.h | mqtt mqttlish api definition interface file
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as mqttlished by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#ifndef _MQTTAPI_H
#define _MQTTAPI_H

#include <xdl.h>
#include <hdfs.h>


#ifdef	__cplusplus
extern "C" {
#endif

	extern int STDCALL tcps_invoke(const tcps_block_t* pb);

#ifdef	__cplusplus
}
#endif


#endif