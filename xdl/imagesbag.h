/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl images utility document

	@module	imagesbag.h | interface file

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

#ifndef _IMAGESBAG_H
#define _IMAGESBAG_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDK_SUPPORT_FILE) && defined(XDL_SUPPORT_BMP)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION insert_images_item_from_url: add a new image item from url token.
@INPUT link_t_ptr ptr: the images link component.
@INPUT const tchar_t* iname: the image name token.
@INPUT const tchar_t* url: the url token.
@RETURN link_t_ptr: return the new item link component.
*/
XDL_API link_t_ptr insert_images_item_from_url(link_t_ptr ptr, const tchar_t* iname, const tchar_t* url);

/*
@FUNCTION insert_images_item_from_file: add a new image item from file content.
@INPUT link_t_ptr ptr: the images link component.
@INPUT const tchar_t* iname: the image name token.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the file path name.
@RETURN link_t_ptr: return the new item link component.
*/
XDL_API link_t_ptr insert_images_item_from_file(link_t_ptr ptr, const tchar_t* iname, const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION load_image_bytes_from_file: load image bitmap bytes from file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the file path name.
@INPUT const tchar_t* itype: the image type, it can be GDI_ATTR_IMAGE_TYPE_JPG, GDI_ATTR_IMAGE_TYPE_PNG, GDI_ATTR_IMAGE_TYPE_BMP.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the bytes buffer size.
@RETURN dword_t: return the bitmap bytes loaded.
*/
XDL_API dword_t load_image_bytes_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* itype, byte_t* buf, dword_t max);

/*
@FUNCTION save_image_bytes_to_file: save image bitmap bytes to file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the file path name.
@INPUT const byte_t* buf: the bytes buffer.
@INPUT dword_t len: the buffer data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_image_bytes_to_file(const secu_desc_t* psd, const tchar_t* fname, const byte_t* buf, dword_t len);

/*
@FUNCTION load_image_base64_from_file: load image bitmap base64 characters from file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the file path name.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the base64 bitmap characters loaded.
*/
XDL_API int load_image_base64_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* buf, int max);

/*
@FUNCTION save_image_base64_to_file: save image bitmap base64 characters to file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the file path name.
@INPUT const tchar_t* buf: the string buffer.
@INPUT int len: the string data size in characters.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_image_base64_to_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* buf, int len);

/*
@FUNCTION load_bitmap_bytes_from_file: load bitmap bytes from file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the file path name.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the bytes buffer size.
@RETURN dword_t: return the bitmap bytes loaded.
*/
XDL_API dword_t load_bitmap_bytes_from_file(const secu_desc_t* psd, const tchar_t* fname, byte_t* buf, dword_t max);

/*
@FUNCTION save_bitmap_bytes_to_file: save bitmap bytes to file.
@INPUT const secu_desc_t* psd: the file security struct.
@INPUT const tchar_t* fname: the file path name.
@INPUT const byte_t* buf: the bytes buffer.
@INPUT dword_t len: the buffer data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t save_bitmap_bytes_to_file(const secu_desc_t* psd, const tchar_t* fname, const byte_t* buf, dword_t len);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif /*_IMAGESBAG_H*/
