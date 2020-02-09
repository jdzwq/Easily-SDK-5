/***********************************************************************
	Easily xdb

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	xdbdef.h | definition interface file

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


#ifndef _XDBDEF_H
#define	_XDBDEF_H

#include <xdl.h>

#define XDB_API_DBEXECUTE	_T("xdb_execute")
#define XDB_API_DBBATCH		_T("xdb_batch")
#define XDB_API_DBEXPORT	_T("xdb_export")
#define XDB_API_DBIMPORT	_T("xdb_import")
#define XDB_API_DBCALLFUNC	_T("xdb_callfunc")
#define XDB_API_DBCALLSOAP	_T("xdb_callsoap")
#define XDB_API_DBCALLJSON	_T("xdb_calljson")
#define XDB_API_DBWRITEBLOB	_T("xdb_writeblob")
#define XDB_API_DBREADBLOB	_T("xdb_readblob")
#define XDB_API_DBWRITECLOB	_T("xdb_writeclob")
#define XDB_API_DBREADCLOB	_T("xdb_readclob")
#define XDB_API_DBWRITEXDOC	_T("xdb_writexdoc")
#define XDB_API_DBREADXDOC	_T("xdb_readxdoc")
#define XDB_API_DBDATETIME	_T("xdb_datetime")


//#define XDB_ITEMFEED	0xFF
//#define XDB_LINEFEED	0xFE

#define DSN_ITEMFEED	_T('=')
#define DSN_LINEFEED	_T(';')

#define DSN_TYPE		_T("TYPE")
#define DSN_TYPE_ODBC	_T("ODBC")
#define DSN_TYPE_OCI	_T("OCI")
#define DSN_TYPE_MYSQL	_T("MYSQL")
#define DSN_TYPE_STUB	_T("STUB")

#define DSN_DRIVER		_T("DRIVER")
#define DSN_SERVER		_T("SERVER")
#define DSN_DATABASE	_T("DATABASE")
#define DSN_UID			_T("UID")
#define DSN_PWD			_T("PWD")
#define DSN_TRUSTED		_T("Trusted_Connection")

#define MAX_SQL_NAME	256
#define MAX_SQL_TOKEN	128
#define MAX_SQL_DATA	4000

#define MIN_SQL_LEN		1024
#define MID_SQL_LEN		2048
#define MAX_SQL_LEN		4096

#define XDB_PENDING		100
#define XDB_SUCCEED		1
#define XDB_FAILED		0

#define XDB_CHARSET_UCS _UTF16_LIT
#define XDB_CHARSET_LAT _GB2312
#define XDB_CHARSET_UTF _UTF8


#ifdef _CHS

#define ERR_TEXT_ROWCHANGED			_T("请求更新单行但处理了多行")

#define ERR_TEXT_ROWCANCEL			_T("请求更新单行但未成功")

#define ERR_TEXT_CANCELED			_T("执行语句在后台已经撤销")

#define ERR_TEXT_INVALIDTRAN		_T("事务提交次序错误")

#define ERR_TEXT_NOTCOMPELETED		_T("前一语句执行尚未结束")

#define ERR_TEXT_OBJECTFAILED		_T("创建对象失败")

#define ERR_TEXT_INVALIDSTMT		_T("数据句柄无效")

#else

#define ERR_TEXT_ROWCHANGED			_T("one row update request but mutiple rows changed")

#define ERR_TEXT_ROWCANCEL			_T("one row update request but no row updated")

#define ERR_TEXT_CANCELED			_T("sql processing has been canceled")

#define ERR_TEXT_INVALIDTRAN		_T("invalid transaction order")

#define ERR_TEXT_NOTCOMPELETED		_T("previous statement not compeleted")

#define ERR_TEXT_OBJECTFAILED		_T("create object failed")

#define ERR_TEXT_INVALIDSTMT		_T("invalid statement")

#endif

/*define db type*/
typedef enum{
	_DB_UNKNOWN = 0,
	_DB_STUB = 1,
	_DB_ODBC = 2,
	_DB_OCI = 3,
	_DB_MYSQL = 4
}DB_TYPE;

typedef struct _db_head{
	sword_t dbt;
	sword_t cbs;
}xdb_head, *xdb_t;

typedef xdb_t	XDB;

#endif	/* _XDBDEF_H */

