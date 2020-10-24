/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc etag document

	@module	filetag.c | implement file

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

#include "filetag.h"

#include "xdlimp.h"


void a_md5_token(const byte_t dig_buf[MD5_SIZE], schar_t str_buf[MD5_LEN])
{
	int i;
	for (i = 0; i < MD5_SIZE; i++)
	{
		a_xsprintf(&(str_buf[i * 2]), "%02x", dig_buf[i] & 0x00FF);
	}
}

void w_md5_token(const byte_t dig_buf[MD5_SIZE], wchar_t str_buf[MD5_LEN])
{
	int i;
	for (i = 0; i < MD5_SIZE; i++)
	{
		w_xsprintf(&(str_buf[i * 2]), L"%02x", dig_buf[i] & 0x00FF);
	}
}

void file_info_etag(const tchar_t* fname, const tchar_t* ftime, const tchar_t* fsize, tchar_t* buf)
{
	byte_t *in_buf;
	byte_t enc_buf[ETAG_LEN + 1] = { 0 };
	int len, total = 0;

#ifdef _UNICODE
	len = ucs_to_utf8(fname, -1, NULL, MAX_LONG) + ucs_to_utf8(ftime, -1, NULL, MAX_LONG) + ucs_to_utf8(fsize, -1, NULL, MAX_LONG);
#else
	len = mbs_to_utf8(fname, -1, NULL, MAX_LONG) + mbs_to_utf8(ftime, -1, NULL, MAX_LONG) + mbs_to_utf8(fsize, -1, NULL, MAX_LONG);
#endif

	in_buf = (byte_t*)xmem_alloc(len + 1);

#ifdef _UNICODE
	len = ucs_to_utf8(fname, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(fname, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(ftime, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(ftime, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(fsize, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(fsize, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

	md5(in_buf, total, enc_buf);

	xmem_free(in_buf);

	buf[0] = _T('\"');
	md5_token(enc_buf, buf + 1);
	total = xslen(buf);
	buf[total] = _T('\"');
	buf[total + 1] = _T('\0');
}
