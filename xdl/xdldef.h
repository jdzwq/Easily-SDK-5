/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	xdldef.h | interface file

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


#ifndef _XDLDEF_H
#define	_XDLDEF_H

#include <stdarg.h>
#include <setjmp.h>
#include <assert.h>

#include <xds.h>
#include <xdk.h>

#if defined(_OS_WINDOWS)
#include "windows/_cfg_win32.h"
#elif defined(_OS_MACOS)
#include "macos/_cfg_macos.h"
#elif defined(_OS_LINUX)
#include "linux/_cfg_linux.h"
#endif


#define MD5_SIZE		16
#define SHA1_SIZE		20
#define SHA2_SIZE		32
#define SHA4_SIZE		64
#define AES_IV_SIZE		16
#define AES_KEY_128		16
#define AES_KEY_256		32
#define DES_IV_SIZE		8
#define DES_KEY_112		16
#define DES_KEY_168		24
#define RC4_KEY_256		256
#define X509_CERT_SIZE	8192
#define RSA_KEY_SIZE	4096

#define BASE64_LENGTH(len)		((len % 3)? (len / 3 + 1) * 4 : len / 3 * 4)

#define SYS_MINDATE		_T("1970-01-01")
#define ISO_MINDATE		_T("1901-12-13")
#define ISO_MINTIME		_T("1901-12-13 20:45:54")
#define ISO_MAXDATE		_T("2038-01-19")
#define ISO_MAXTIME		_T("2038-01-19 03:14:07")

/*define xdl error code*/
#ifndef C_OK
#define C_OK		((int)0)
#endif

#ifndef C_ERR
#define C_ERR		((int)-1)
#endif

typedef dword_t		key32_t;
typedef lword_t		key64_t;
typedef struct{
	lword_t l;
	lword_t h;
}key128_t;

typedef struct _link_t* link_t_ptr ;
typedef struct _link_t{
	byte_t tag;
	byte_t lru[3];
	link_t_ptr prev;	/*previous component link*/
	link_t_ptr next;	/*next component link*/
}link_t;

#ifdef _OS_64
#define LINK_FIRST	((link_t_ptr)((unsigned long long)1))		
#define LINK_LAST	((link_t_ptr)((unsigned long long)-1))
#else
#define LINK_FIRST	((link_t_ptr)((unsigned int)1))		
#define LINK_LAST	((link_t_ptr)((unsigned int)-1))
#endif

#ifdef _OS_64
#define TypePtrFromLink(type,p) ((type*)((unsigned long long)p - (unsigned long long)&(((type*)0)->lk))) 
#define TypePtrFromChild(type,p) ((type*)((unsigned long long)p - (unsigned long long)&(((type*)0)->lkChild))) 
#define TypePtrFromSibling(type,p) ((type*)((unsigned long long)p - (unsigned long long)&(((type*)0)->lkSibling))) 
#else
#define TypePtrFromLink(type,p) ((type*)((unsigned int)p - (unsigned int)&(((type*)0)->lk))) 
#define TypePtrFromChild(type,p) ((type*)((unsigned int)p - (unsigned int)&(((type*)0)->lkChild))) 
#define TypePtrFromSibling(type,p) ((type*)((unsigned int)p - (unsigned int)&(((type*)0)->lkSibling))) 
#endif


/*define root link tag*/
#define lkRoot			0xFF
/*define free link tag*/
#define lkFree			0x00

#define lkDebug			0xFE

#define lkDoc			0x01
#define lkNode			0x02

#define lkHashTable		0x03
#define lkHashEntity	0x04

#define lkListTable		0x05
#define lkListNode		0x06

#define lkStringTable	0x07
#define lkStringEntity	0x08

#define lkDictTable		0x09
#define lkDictEntity	0x0A

#define lkWordsTable	0x0B
#define lkWordsItem		0x0C

#define lkTrieNode		0x0D
#define lkTrieLeaf		0x0E

#define lkStackTable	0x0F

#define lkBinaTree		0x10
#define lkBinaNode		0x11

#define lkBplusTree		0x12
#define lkBplusIndex	0x13
#define lkBplusData		0x14

#define lkACTable		0x15

#define lkMultiTree		0x16

#define lkFileTable		0x17

#define IS_DOM_DOC(ptr)		((ptr->tag == lkNode)? 1 : 0)
#define IS_XML_DOC(ptr)		((ptr->tag == lkDoc)? 1 : 0)


#define TEXT_SOH			0x01 //start of headline
#define TEXT_STX			0x02 //start of text
#define TEXT_ETX			0x03 //end of text
#define TEXT_FS				0x1c //file separator
#define TEXT_GS				0x1d //group separator
#define TEXT_RS				0x1e //record separator
#define TEXT_US				0x1f //unit separator

/*define data state*/
typedef enum{ 
	dsClean = 0, 
	dsDirty = 1, 
	dsNewClean = 2, 
	dsNewDirty = 4, 
	dsDelete = 8
}DATA_STATE;

#define dsAll		(dsClean | dsNewClean | dsDirty | dsNewDirty | dsDelete)
#define dsUpdate	(dsDirty | dsNewDirty | dsDelete)
#define dsNone		0

typedef struct _secu_desc_t {
	tchar_t scr_uid[KEY_LEN];	/*user id or public key*/
	tchar_t scr_key[KEY_LEN];	/*user key or private key*/
}secu_desc_t;


typedef enum {
	FILE_SINCE_NONE = 0,
	FILE_SINCE_TIME = 1,
	FILE_SINCE_ETAG = 2
}FILE_SINCE;

typedef enum{
	_SECU_NONE = 0,
	_SECU_SSL = 1,
	_SECU_SSH = 2
}SOCKET_SECU;

/* default net package size */
#define MTU_MAX_SIZE		1500
#define MTU_MID_SIZE		576
#define MTU_MIN_SIZE		46

#define ETH_HDR_SIZE		14
#define ETH_TAL_SIZE		4
#define ETH_PKG_SIZE		(ETH_HDR_SIZE + MTU_MAX_SIZE + ETH_TAL_SIZE)
#define ETH_PDU_SIZE		(ETH_PKG_SIZE - ETH_HDR_SIZE - ETH_TAL_SIZE)

#define MAX_ETH_SIZE		(ETH_HDR_SIZE + MTU_MAX_SIZE + ETH_TAL_SIZE)
#define MID_ETH_SIZE		(ETH_HDR_SIZE + MTU_MID_SIZE + ETH_TAL_SIZE)

#define IP_HDR_SIZE			20
#define IP_PKG_SIZE			ETH_PDU_SIZE
#define IP_PDU_SIZE			(IP_PKG_SIZE - IP_HDR_SIZE)

#define TCP_HDR_SIZE		20
#define TCP_PKG_SIZE		IP_PDU_SIZE
#define TCP_PDU_SIZE		(TCP_PKG_SIZE - TCP_HDR_SIZE)

#define UDP_HDR_SIZE		8
#define UDP_PKG_SIZE		(MTU_MID_SIZE - IP_HDR_SIZE)
#define UDP_PDU_SIZE		(UDP_PKG_SIZE - UDP_HDR_SIZE)

typedef struct _datadef_t{
	tchar_t data_type[INT_LEN];
	tchar_t data_len[INT_LEN];
	tchar_t data_dig[INT_LEN];
}datadef_t;

#define CALENDAR_COL	7
#define CALENDAR_ROW	6

typedef struct _calendar_t
{
	tchar_t calen_week[CALENDAR_COL][UTF_LEN +1];
	int calen_days[CALENDAR_ROW][CALENDAR_COL];
}calendar_t;


typedef struct _period_t {
	tchar_t base[DATE_LEN]; // base datetime
	tchar_t prec[2]; //time ruler, eg: 'Y','M','D','H','I','S'
	tchar_t feed[INT_LEN]; // 时效步长
}period_t;


#define UNIT_PT				_T("pt")
#define UNIT_MM				_T("mm")

#define AGES_YEAR			_T("Years")
#define AGES_MONTH			_T("Months")
#define AGES_DAY			_T("Days")

//define dom node mask
#define MSK_CDATA		0x00001000 //is cdata node
#define MSK_ARRAY		0x00002000 //is array node
#define MSK_LOCKED		0x00010000 //node locked
#define MSK_CHECKED		0x00020000 //node checked
#define MSK_HIDDEN		0x00040000 //node not visible
#define MSK_COLLAPSED	0x00080000 //node collapsed tag
#define MSK_SKIPPED		0x00100000 //node will be skiped
#define MSK_DELETED		0x00200000 //node will be deleted
#define MSK_SORTED		0x00400000 //node has been sorted
#define MSK_SPLITED		0x00800000 //node has been splited
#define MSK_FIXED		0x01000000 //node has been fixed
#define MSK_PASSWORD	0x02000000 //node has been fixed
#define MSK_DESIGN		0x80000000 //document in design mode

//define data verify code
typedef enum{ 
	veValid = 0, 
	veNull = 1, 
	veFormat = 2, 
	veDatatype = 3, 
	veOverflow = 4, 
	veTruncate = 5, 
	veCodeSystem = 6, 
	veUserReject = 7 
}VERIFY_CODE;

#ifdef _OS_64
#define TypePtrFromHead(type,p) ((type*)((unsigned long long)p - (unsigned long long)&(((type*)0)->head))) 
#else
#define TypePtrFromHead(type,p) ((type*)((unsigned int)p - (unsigned int)&(((type*)0)->head))) 
#endif

/*define handle type*/
#define _HANDLE_UNKNOWN		0x00
#define _HANDLE_BLOCK		0x01
#define _HANDLE_INET		0x02
#define _HANDLE_CONS		0x03
#define _HANDLE_COMM		0x04
#define _HANDLE_PIPE		0x05
#define _HANDLE_SHARE		0x06
#define _HANDLE_CACHE		0x07
#define _HANDLE_UNCF		0x08
#define _HANDLE_UDP			0x09
#define _HANDLE_TCP			0x0A
#define _HANDLE_SSL			0x0B
#define _HANDLE_SSH			0x0C
#define _HANDLE_PNP			0x0D
#define _HANDLE_TFTP		0x0E
#define _HANDLE_MQTT		0x0F
#define _HANDLE_COAP		0x10

typedef struct _xhand_head{
	byte_t tag;
	byte_t lru[3];
}xhand_head, *xhand_t;

/*define object type*/
#define _OBJECT_UNKNOWN		0x00
#define _OBJECT_STRING		0x01
#define _OBJECT_VARIANT		0x02
#define _OBJECT_DOMDOC		0x03
#define _OBJECT_BINARY		0x04

typedef struct _object_head{
	byte_t tag;
	byte_t lru[3];
}object_head, *object_t;

typedef struct _string_head{
	byte_t tag;
	byte_t lru[3];
}string_head, *string_t;

typedef struct _stream_head{
	byte_t tag;
	byte_t lru[3];
}stream_head, *stream_t;

/*canvas type*/
#define _CANVAS_UNKNOWN		0x00
#define _CANVAS_DISPLAY		0x01
#define _CANVAS_PRINTER		0x02

typedef struct _canvas_head{
	byte_t tag;
	byte_t lru[3];
}canvas_head, *canvas_t;

//define file proto type
#define _PROTO_UNKNOWN		0x00
#define _PROTO_LOC			0x01
#define _PROTO_NFS			0x02
#define _PROTO_HTTP			0x04
#define _PROTO_SSH			0x06
#define _PROTO_TFTP			0x08
#define _PROTO_COAP			0x09

#define IS_INET_FILE(n)	(n == _PROTO_HTTP || n == _PROTO_SSH || n == _PROTO_TFTP || n == _PROTO_COAP)

typedef struct _acp_head{
	sword_t base;
}acp_head, *acp_t;

#define VV_NULL			0x00
#define VV_BOOL			0x01
#define VV_BYTE			0x02
#define VV_SCHAR		0x03
#define VV_WCHAR		0x04
#define VV_SHORT		0x05
#define VV_INT			0x06
#define VV_LONG			0x07
#define VV_FLOAT		0x08
#define VV_DOUBLE		0x09
#define VV_STRING		0x0A

#define VV_BOOL_ARRAY	(VV_BOOL | 0x40)
#define VV_BYTE_ARRAY	(VV_BYTE | 0x40)
#define VV_SCHAR_ARRAY	(VV_SCHAR | 0x40)
#define VV_WCHAR_ARRAY	(VV_WCHAR | 0x40)
#define VV_SHORT_ARRAY	(VV_SHORT | 0x40)
#define VV_INT_ARRAY	(VV_INT | 0x40)
#define VV_LONG_ARRAY	(VV_LONG | 0x40)
#define VV_FLOAT_ARRAY	(VV_FLOAT | 0x40)
#define VV_DOUBLE_ARRAY	(VV_DOUBLE | 0x40)
#define VV_STRING_ARRAY	(VV_STRING | 0x40)

#define VV_STRING_REF		(VV_STRING | 0x80)
#define VV_BOOL_ARRAY_REF	(VV_BOOL_ARRAY | 0x80)
#define VV_BYTE_ARRAY_REF	(VV_BYTE_ARRAY | 0x80)
#define VV_SCHAR_ARRAY_REF	(VV_SCHAR_ARRAY | 0x80)
#define VV_WCHAR_ARRAY_REF	(VV_WCHAR_ARRAY | 0x80)
#define VV_SHORT_ARRAY_REF	(VV_SHORT_ARRAY | 0x80)
#define VV_INT_ARRAY_REF	(VV_INT_ARRAY | 0x80)
#define VV_LONG_ARRAY_REF	(VV_LONG_ARRAY | 0x80)
#define VV_FLOAT_ARRAY_REF	(VV_FLOAT_ARRAY | 0x80)
#define VV_DOUBLE_ARRAY_REF	(VV_DOUBLE_ARRAY | 0x80)
#define VV_STRING_ARRAY_REF	(VV_STRING_ARRAY | 0x80)

typedef enum{
	IO_IN = 0,
	IO_OUT = 1,
	IO_INOUT = 2
}VARIANT_IO;

typedef struct _variant_t{
	byte_t io;
	byte_t vv;
	int size;
	int prec;
	int encode;

	union{
		bool_t bool_one;
		byte_t byte_one;
		schar_t schar_one;
		wchar_t wchar_one;
		short short_one;
		int int_one;
		long long long_one;
		float float_one;
		double double_one;
		tchar_t* string_one;

		bool_t* bool_ptr;
		byte_t* byte_ptr;
		schar_t* schar_ptr;
		wchar_t* wchar_ptr;
		short* short_ptr;
		int* int_ptr;
		long long* long_ptr;
		float* float_ptr;
		double* double_ptr;
		tchar_t** string_ptr;
	};
}variant_t;


#define PAPER_A4_WIDTH			210.0f
#define PAPER_A4_HEIGHT			297.0f

#define PAPER_A5_WIDTH			148.0f
#define PAPER_A5_HEIGHT			210.0f

#define PAPER_LETTER_WIDTH		210.0f
#define PAPER_LETTER_HEIGHT		280.0f

#define PAPER_MIN_WIDTH			2.6f
#define PAPER_MIN_HEIGHT		2.6f

#define PAPER_MAX_WIDTH			280.0f
#define PAPER_MAX_HEIGHT		558.7f

#define DEF_PAPER_WIDTH			PAPER_A4_WIDTH
#define DEF_PAPER_HEIGHT		PAPER_A4_HEIGHT

#define DEF_SPLIT_SPAN			1.5f
#define DEF_TOUCH_SPAN			7.0f
#define DEF_ATTR_ICON_SPAN		6.8f
#define DEF_TEXT_HEIGHT			8.0f
#define DEF_TEXT_WIDTH			20.0f
#define DEF_ITEM_HEIGHT			7.0f
#define DEF_ITEM_WIDTH			15.0f
#define DEF_PLOT_HEIGHT			20.0f
#define DEF_PLOT_WIDTH			30.0f

#define DEF_SMALL_ICON		3.2f
#define DEF_MIDD_ICON		6.4f
#define DEF_LARGE_ICON		8.6f

#define DEF_PAGE_FEED		10.0f
#define DEF_CORNER_FEED		3.0f
#define DEF_SPLIT_FEED		1.0f
#define DEF_SHADOW_FEED		2.0f

#define DEF_INNER_FEED		-1
#define DEF_OUTER_FEED		1
#define DEF_FOCUS_SPAN		5

#define SIZING_TOPLEFT			0x00000001
#define SIZING_TOPCENTER		0x00000002
#define SIZING_TOPRIGHT			0x00000004
#define SIZING_BOTTOMLEFT		0x00000008
#define SIZING_BOTTOMCENTER		0x00000010
#define SIZING_BOTTOMRIGHT		0x00000020
#define SIZING_LEFTCENTER		0x00000040
#define SIZING_RIGHTCENTER		0x00000080


#define WARNING_OPTIONS		_T("1~Red;2~Orange;3~Yellow;4~Blue;")

#define PAGENUM_GUID		_T("Page %d")
#define PAGENUM_NAME		_T("Page#")

typedef struct _page_cator_t{
	short page;
	short indi;
	int pos;
	int point;
	tchar_t pch[CHS_LEN + 1];
}page_cator_t;

typedef struct _viewbox_t{
	int px, py, pw, ph;
}viewbox_t;

typedef struct _canvbox_t{
	float fx, fy, fw, fh;
}canvbox_t;

typedef struct _xsort_t{
	int fact;
	void* data;
}xsort_t;

/*sort link node callback function*/
typedef int(*CALLBACK_SORTLINK)(link_t_ptr plk1, link_t_ptr plk2, void* pv);

/*enum link node callback function*/
typedef bool_t(*CALLBACK_ENUMLINK)(link_t_ptr plk, void* pv);

/*list file node callback function*/
typedef void(*CALLBACK_LISTFILE)(const file_info_t* pfi, void* pv);

typedef enum{
	ORDER_NONE = 0,
	ORDER_ASCEND = 1,
	ORDER_DESCEND = -1
}SORT_ORDER;

#if defined(_OS_WINDOWS)
#include "lang/_xdl_ansi.h"
#else
#include "lang/_xdl_utf8.h"
#endif

#include "xdlinf.h"

#endif	/* _XDLDEF_H */

