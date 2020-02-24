
/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc xdl file operation

	@module	xdlfile.h | xdl file operation interface file

	@devnote 张文权 2003.01 - 2007.12 v3.0
			 张文权 2008.01 - 2009.12 v4.0
*************************************************************/

#ifndef _XDLFILE_H
#define _XDLFILE_H

#include "xdldef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{_PROTO_CUR = 0,_PROTO_LOC = 1,_PROTO_NFS = 2,_PROTO_HTTP = 4,_PROTO_FTP = 8}FileProto;

/*define file protocol*/
#define PROTO_CUR		L"."

#define PROTO_LOC		L"\\"

#define PROTO_NFS		L"\\\\"

#define PROTO_HTTP		L"http"

#define PROTO_FTP		L"ftp"

#define PROTO_LEN		6

#define HTTP_VERB_LEN	6

#define XHTTP_VER		L"HTTP/1.0"

#define XHTTP_GET		L"GET"

#define XHTTP_POST		L"POST"

#define XHTTP_ACCEPT_XML		L"text/html"

typedef struct _find_data_t {
	int proto;
	handle_t h_conn;
	handle_t h_find;
	unsigned int mask;
	unsigned int high_size;
	unsigned int low_size;
	wchar_t long_name[ MAX_PATH ];
	wchar_t short_name[14];
	wchar_t create_time[DATE_LEN + 1];
	wchar_t access_time[DATE_LEN + 1];
	wchar_t write_time[DATE_LEN + 1];
} find_data_t,*find_data_t_ptr;

XDL_API handle_t	xfile_open(const wchar_t* file,const wchar_t* mode);

XDL_API size_t		xfile_read(handle_t fh,void* buf,size_t size);

XDL_API size_t		xfile_write(handle_t fh,void* buf,size_t size);

XDL_API void		xfile_close(handle_t fh);

XDL_API size_t		xfile_size(handle_t fh);

XDL_API bool_t		xfile_find_first(const wchar_t* fname,find_data_t* pxf);

XDL_API bool_t		xfile_find_next(find_data_t* pxf);

XDL_API void		xfile_find_close(find_data_t* pxf);

#ifdef __cplusplus
}
#endif


#endif /*_XFILE_H*/
