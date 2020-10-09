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

EXP_API void bytes_turn(byte_t* ba, int n);

EXP_API int parse_encode(const tchar_t* enstr);

EXP_API void format_encode(int encode, tchar_t* buf);

EXP_API int parse_charset(const tchar_t* enstr);

EXP_API void format_charset(int encode, tchar_t* buf);

EXP_API int parse_utfbom(const byte_t* buf, int len);

EXP_API int format_utfbom(int encode, byte_t* buf);

EXP_API int skip_utfbom(const byte_t* buf);

EXP_API bool_t is_ip(const tchar_t* addr);

EXP_API void parse_bytes_range(tchar_t* sz_range, dword_t* phoff, dword_t* ploff, dword_t* psize, long long* ptotal);

EXP_API void format_bytes_range(tchar_t* sz_range, dword_t hoff, dword_t loff, dword_t size, long long total);

EXP_API bool_t is_zero_size(const tchar_t* fsize);

EXP_API bool_t is_huge_size(const tchar_t* fsize);

EXP_API unsigned int parse_hexnum(const tchar_t* token, int len);

EXP_API int format_hexnum(unsigned int n, tchar_t* buf, int max);

EXP_API int fill_integer(int ln, tchar_t* buf, int max);

EXP_API int format_integer_ex(int n, const tchar_t* fmt, tchar_t* buf, int max);

EXP_API bool_t is_zero_numeric(double dbl, int scale);

EXP_API double parse_numeric(const tchar_t* token, int len);

EXP_API int format_numeric(double dbl, const tchar_t* fmt, tchar_t* buf, int max);

EXP_API int format_money_chs(double dbl, int n_span, tchar_t* buf, int max);

EXP_API int format_ages(const xdate_t* bday, const xdate_t* tday, tchar_t* buf);

EXP_API int format_long(unsigned int hl, unsigned int ll, tchar_t* buf);

EXP_API void parse_long(unsigned int* phl, unsigned int* pll, const tchar_t* str);

EXP_API void reset_date(xdate_t* pmd);

EXP_API void get_min_date(xdate_t* pdt);

EXP_API int format_date(const xdate_t* pmd,tchar_t* buf);

EXP_API int format_datetime(const xdate_t* pmd,tchar_t* buf);

EXP_API int format_time(const xdate_t* pmd, tchar_t* buf);

EXP_API void parse_date(xdate_t* pmd, const tchar_t* str);

EXP_API void parse_datetime(xdate_t* pmd, const tchar_t* str);

EXP_API int format_utctime(const xdate_t* pmd, tchar_t* buf);

EXP_API int format_gmttime(const xdate_t* pmd, tchar_t* buf);

EXP_API void parse_gmttime(xdate_t* pmd, const tchar_t* str);

EXP_API int format_datetime_ex(const xdate_t* pxd, const tchar_t* fmt, tchar_t* buf, int max);

EXP_API void parse_datetime_ex(xdate_t* pxd, const tchar_t* fmt, const tchar_t* str);

EXP_API bool_t verify_datetime(const xdate_t* pdt);

EXP_API bool_t is_datetime(const tchar_t* token);

EXP_API int max_mon_days(int year, int mon);

EXP_API int diff_years(const xdate_t* pdt1, const xdate_t* pdt2);

EXP_API int diff_months(const xdate_t* pdt1, const xdate_t* pdt2);

EXP_API int diff_days(const xdate_t* pdt1, const xdate_t* pdt2);

EXP_API int diff_hours(const xdate_t* pdt1, const xdate_t* pdt2);

EXP_API int diff_mins(const xdate_t* pdt1, const xdate_t* pdt2);

EXP_API int diff_secs(const xdate_t* pdt1, const xdate_t* pdt2);

EXP_API void plus_years(xdate_t* pdt, int years);

EXP_API void plus_months(xdate_t* pdt, int months);

EXP_API void plus_days(xdate_t* pdt, int days);

EXP_API void plus_weeks(xdate_t* pdt, int weeks);

EXP_API void plus_hours(xdate_t* pdt, int hours);

EXP_API void plus_minutes(xdate_t* pdt, int minutes);

EXP_API void plus_seconds(xdate_t* pdt, int seconds);

EXP_API void plus_millseconds(xdate_t* pdt, int ms);

EXP_API void calc_period(const period_t* ptp, tchar_t* sz_time);

#ifdef XDK_SUPPORT_DATE
EXP_API void default_calendar(calendar_t* pca);

EXP_API void fill_calendar(calendar_t* pca, const xdate_t* pdt);
#endif

EXP_API int format_password(const tchar_t* sz, tchar_t* buf, int max);

EXP_API int format_shield(const tchar_t* sz, tchar_t* buf, int max);

EXP_API int compare_date(const xdate_t* pmd1, const xdate_t* pmd2);

EXP_API int compare_datetime(const xdate_t* pmd1, const xdate_t* pmd2);

EXP_API int compare_data(const tchar_t* src, const tchar_t* dest, const tchar_t* datatype);

EXP_API int compare_time(const xdate_t* pmd1, const xdate_t* pmd2);

EXP_API int compare_numeric(const tchar_t* szSrc, const tchar_t* szDes, int digi);

EXP_API int compare_text(const tchar_t* src, int srclen, const tchar_t* dest, int destlen, int nocase);

EXP_API int verify_text(const tchar_t* str, const tchar_t* datatype, bool_t nullable, int len, const tchar_t* min, const tchar_t* max);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXP_API bool_t get_param_item(const tchar_t* sz_param, const tchar_t* key, tchar_t* val, int max);

EXP_API int split_line(const tchar_t* token, int len);

EXP_API bool_t split_xmlns(tchar_t* str, int* kat, int* klen, int* vat, int* vlen);

EXP_API const tchar_t* skip_xmlns(const tchar_t* str, int slen);

EXP_API int trim_xmlns(tchar_t* str, int slen);

EXP_API int compare_nons(const tchar_t* src, int srclen, const tchar_t* dest, int destlen);

EXP_API byte_t parse_proto(const tchar_t* file);

EXP_API void parse_url(const tchar_t* url, tchar_t** proat, int* prolen, tchar_t** addrat, int* addrlen, tchar_t** portat, int* portlen, tchar_t** objat, int* objlen, tchar_t** qryat, int* qrylen);

EXP_API int printf_path(tchar_t* fpath, const tchar_t* strfmt, ...);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXP_API float font_size(int px);

EXP_API float font_metric(float pt);

EXP_API int font_points(float pt);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXP_API bool_t inside_rowcol(int row, int col, int from_row, int from_col, int to_row, int to_col);

EXP_API int compare_rowcol(int from_row, int from_col, int to_row, int to_col);

EXP_API int ft_parse_points_from_token(xpoint_t* ppt, int max, const tchar_t* token, int len);

EXP_API int ft_format_points_to_token(const xpoint_t* ppt, int n, tchar_t* buf, int max);

EXP_API int pt_parse_points_from_token(xpoint_t* ppt, int max, const tchar_t* token, int len);

EXP_API int pt_format_points_to_token(const xpoint_t* ppt, int n, tchar_t* buf, int max);

EXP_API int parse_dicm_point(const tchar_t* token, int len, xpoint_t* ppt, int max);

EXP_API int format_dicm_point(const xpoint_t* ppt, int count, tchar_t* buf, int max);

#ifdef	__cplusplus
}
#endif

#endif