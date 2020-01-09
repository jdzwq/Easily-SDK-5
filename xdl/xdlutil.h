/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl utility document

	@module	xdlutil.h | interface file

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

#ifndef _XDLUTIL_H
#define _XDLUTIL_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void bytes_turn(byte_t* ba, int n);

XDL_API int parse_encode(const tchar_t* enstr);

XDL_API void format_encode(int encode, tchar_t* buf);

XDL_API int parse_charset(const tchar_t* enstr);

XDL_API void format_charset(int encode, tchar_t* buf);

XDL_API int parse_utfbom(const byte_t* buf, int len);

XDL_API int format_utfbom(int encode, byte_t* buf);

XDL_API int skip_utfbom(const byte_t* buf);

XDL_API bool_t is_ip(const tchar_t* addr);

XDL_API void parse_long_range(tchar_t* sz_range, dword_t* phoff, dword_t* ploff, dword_t* psize, long long* ptotal);

XDL_API void format_long_range(tchar_t* sz_range, dword_t hoff, dword_t loff, dword_t size, long long total);

XDL_API bool_t is_zero_size(const tchar_t* fsize);

XDL_API bool_t is_huge_size(const tchar_t* fsize);

XDL_API unsigned long parse_hexnum(const tchar_t* token, int len);

XDL_API int format_hexnum(unsigned long n, tchar_t* buf, int max);

XDL_API int fill_long(long ln, tchar_t* buf, int max);

XDL_API int format_long_ex(long n, const tchar_t* fmt, tchar_t* buf, int max);

XDL_API bool_t is_zero_numeric(double dbl, int scale);

XDL_API double parse_numeric(const tchar_t* token, int len);

XDL_API int format_numeric(double dbl, const tchar_t* fmt, tchar_t* buf, int max);

XDL_API int format_money_chs(double dbl, int n_span, tchar_t* buf, int max);

XDL_API int format_ages(const xdate_t* bday, const xdate_t* tday, tchar_t* buf);

XDL_API int format_longlong(unsigned long hl, unsigned long ll, tchar_t* buf);

XDL_API void parse_longlong(unsigned long* phl, unsigned long* pll, const tchar_t* str);

XDL_API void reset_date(xdate_t* pmd);

XDL_API void get_min_date(xdate_t* pdt);

XDL_API int format_date(const xdate_t* pmd,tchar_t* buf);

XDL_API int format_datetime(const xdate_t* pmd,tchar_t* buf);

XDL_API int format_time(const xdate_t* pmd, tchar_t* buf);

XDL_API void parse_date(xdate_t* pmd, const tchar_t* str);

XDL_API void parse_datetime(xdate_t* pmd, const tchar_t* str);

XDL_API int format_utctime(const xdate_t* pmd, tchar_t* buf);

XDL_API int format_gmttime(const xdate_t* pmd, tchar_t* buf);

XDL_API void parse_gmttime(xdate_t* pmd, const tchar_t* str);

XDL_API int format_datetime_ex(const xdate_t* pxd, const tchar_t* fmt, tchar_t* buf, int max);

XDL_API void parse_datetime_ex(xdate_t* pxd, const tchar_t* fmt, const tchar_t* str);

XDL_API bool_t verify_datetime(const xdate_t* pdt);

XDL_API bool_t is_datetime(const tchar_t* token);

XDL_API int max_mon_days(int year, int mon);

XDL_API int diff_years(const xdate_t* pdt1, const xdate_t* pdt2);

XDL_API int diff_months(const xdate_t* pdt1, const xdate_t* pdt2);

XDL_API int diff_days(const xdate_t* pdt1, const xdate_t* pdt2);

XDL_API void plus_years(xdate_t* pdt, int years);

XDL_API void plus_months(xdate_t* pdt, int months);

XDL_API void plus_days(xdate_t* pdt, int days);

XDL_API void plus_weeks(xdate_t* pdt, int weeks);

XDL_API void plus_hours(xdate_t* pdt, int hours);

XDL_API void plus_minutes(xdate_t* pdt, int minutes);

XDL_API void plus_seconds(xdate_t* pdt, int seconds);

#ifdef XDK_SUPPORT_DATE
XDL_API void default_calendar(calendar_t* pca);

XDL_API void fill_calendar(calendar_t* pca, const xdate_t* pdt);
#endif

XDL_API int format_password(const tchar_t* sz, tchar_t* buf, int max);

XDL_API long mul_div_long(long m1, long m2, long d);

XDL_API short mul_div_short(short m1, short m2, short d);

XDL_API int compare_date(const xdate_t* pmd1, const xdate_t* pmd2);

XDL_API int compare_datetime(const xdate_t* pmd1, const xdate_t* pmd2);

XDL_API int compare_data(const tchar_t* src, const tchar_t* dest, const tchar_t* datatype);

XDL_API int compare_time(const xdate_t* pmd1, const xdate_t* pmd2);

XDL_API int compare_numeric(const tchar_t* szSrc, const tchar_t* szDes, int digi);

XDL_API int compare_text(const tchar_t* src, int srclen, const tchar_t* dest, int destlen, int nocase);

XDL_API int verify_text(const tchar_t* str, const tchar_t* datatype, bool_t nullable, int len, const tchar_t* min, const tchar_t* max);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
XDL_API int split_line(const tchar_t* token, int len);

XDL_API bool_t split_xmlns(tchar_t* str, int* kat, int* klen, int* vat, int* vlen);

XDL_API const tchar_t* skip_xmlns(const tchar_t* str, int slen);

XDL_API int trim_xmlns(tchar_t* str, int slen);

XDL_API int compare_nons(const tchar_t* src, int srclen, const tchar_t* dest, int destlen);

XDL_API byte_t parse_proto(const tchar_t* file);

XDL_API void parse_url(const tchar_t* url, tchar_t** proat, int* prolen, tchar_t** addrat, int* addrlen, tchar_t** portat, int* portlen, tchar_t** objat, int* objlen, tchar_t** qryat, int* qrylen);

XDL_API void split_path(const tchar_t* pathfile, tchar_t* path, tchar_t* file, tchar_t* ext);

XDL_API void split_file(const tchar_t* pathfile, tchar_t* path, tchar_t* file);

XDL_API int printf_path(tchar_t* fpath, const tchar_t* strfmt, ...);
/***************************************************************************************************************************/

XDL_API void parse_xpen_from_style(xpen_t* pxp, const tchar_t* token);

XDL_API int format_xpen_to_style(const xpen_t* pxp, tchar_t* buf, int len);

XDL_API void parse_xbrush_from_style(xbrush_t* pxb, const tchar_t* token);

XDL_API int format_xbrush_to_style(const xbrush_t* pxb, tchar_t* buf, int len);

XDL_API void parse_xfont_from_style(xfont_t* pxf, const tchar_t* token);

XDL_API int format_xfont_to_style(const xfont_t* pxf, tchar_t* buf, int len);

XDL_API void parse_xface_from_style(xface_t* pxa, const tchar_t* token);

XDL_API int format_xface_to_style(const xface_t* pxa, tchar_t* buf, int len);

XDL_API void parse_ximage_from_source(ximage_t* pxi, const tchar_t* token);

XDL_API int format_ximage_to_source(const ximage_t* pxi, tchar_t* buf, int len);

///////////////////////////////////////////////////////////////////////////////////////////
XDL_API bool_t inside_rowcol(int row, int col, int from_row, int from_col, int to_row, int to_col);

XDL_API int compare_rowcol(int from_row, int from_col, int to_row, int to_col);

XDL_API bool_t ft_inside(float x, float y, float x1, float y1, float x2, float y2);

XDL_API bool_t pt_inside(long x, long y, long x1, long y1, long x2, long y2);

XDL_API bool_t ft_in_rect(const xpoint_t* ppt, const xrect_t* prt);

XDL_API bool_t pt_in_rect(const xpoint_t* ppt, const xrect_t* prt);

XDL_API void ft_offset_point(xpoint_t* ppt, float cx, float cy);

XDL_API void pt_offset_point(xpoint_t* ppt, long cx, long cy);

XDL_API void ft_center_rect(xrect_t* pxr, float cx, float cy);

XDL_API void pt_center_rect(xrect_t* pxr, long cx, long cy);

XDL_API void ft_expand_rect(xrect_t* pxr, float cx, float cy);

XDL_API void pt_expand_rect(xrect_t* pxr, long cx, long cy);

XDL_API void ft_offset_rect(xrect_t* pxr, float cx, float cy);

XDL_API void pt_offset_rect(xrect_t* pxr, long cx, long cy);

XDL_API void ft_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt);

XDL_API void pt_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt);

XDL_API void ft_cell_rect(xrect_t* pxr, int rows, int cols, int index);

XDL_API void pt_cell_rect(xrect_t* pxr, int rows, int cols, int index);

XDL_API bool_t ft_clip_rect(xrect_t* pxr, const xrect_t* pxr_sub);

XDL_API bool_t pt_clip_rect(xrect_t* pxr, const xrect_t* pxr_sub);

XDL_API void ft_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub);

XDL_API void pt_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub);

XDL_API bool_t rect_is_empty(const xrect_t* pxr);

XDL_API void empty_rect(xrect_t* pxr);

XDL_API int ft_parse_points_from_token(xpoint_t* ppt, int max, const tchar_t* token, int len);

XDL_API int ft_format_points_to_token(const xpoint_t* ppt, int n, tchar_t* buf, int max);

XDL_API int pt_parse_points_from_token(xpoint_t* ppt, int max, const tchar_t* token, int len);

XDL_API int pt_format_points_to_token(const xpoint_t* ppt, int n, tchar_t* buf, int max);

XDL_API int parse_dicm_point(const tchar_t* token, int len, xpoint_t* ppt, int max);

XDL_API int format_dicm_point(const xpoint_t* ppt, int count, tchar_t* buf, int max);

/**********************************************************************************************/
#ifdef XDL_SUPPORT_CRYPT
XDL_API	void a_md5_token(const byte_t dig_buf[MD5_SIZE], schar_t str_buf[MD5_LEN]);

XDL_API	void w_md5_token(const byte_t dig_buf[MD5_SIZE], wchar_t str_buf[MD5_LEN]);

XDL_API void file_info_etag(const tchar_t* fname, const tchar_t* ftime, const tchar_t* fsize, tchar_t* buf);
#endif

#ifdef	__cplusplus
}
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define md5_token		w_md5_token
#else
#define md5_token		a_md5_token
#endif

#endif