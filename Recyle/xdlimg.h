/*************************************************************
	Easily xdl v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@doc image document

	@module	xdlimg.h | image document interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

#ifndef _XDLIMG_H
#define _XDLIMG_H

#include "xdldef.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API int get_image_bytes(res_bmp_t rb);

XDL_API res_bmp_t load_image_from_bytes(const byte_t* pb, int bytes);

XDL_API int save_image_to_bytes(res_bmp_t rb, byte_t* buf, int max);

XDL_API res_bmp_t load_image_from_file(const wchar_t* filename);

XDL_API void free_image(res_bmp_t rb);

#ifdef __cplusplus
}
#endif

#endif