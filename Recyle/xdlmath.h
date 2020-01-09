/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl math document

	@module	xdlmath.h | xdl math interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#ifndef _XDLMATH_H
#define _XDLMATH_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_MATH

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API double fcos(double f);

XDL_API double facos(double f);

XDL_API double fsin(double f);

XDL_API double fasin(double f);

XDL_API double ftan(double f);

XDL_API double fatan(double f);

XDL_API void frand(int digs, int* pa, int max);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_MATH*/

#endif /*_XDLMATH_H*/