/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl oauth document

	@module	netoau.h | xdl http interface file

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

#ifndef _NETOAU_H
#define _NETOAU_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_SOCK

#define OAUTH_CLIENT_ID			_T("client_id")
#define OAUTH_CLIENT_SECRET		_T("client_secret")
#define OAUTH_RESPONSE_TYPE		_T("response_type")
#define OAUTH_SCOPE				_T("scope")
#define OAUTH_STATE				_T("state")
#define OAUTH_REDIRECT_URL		_T("redirect_uri")
#define OAUTH_ERROR				_T("error")
#define OAUTH_ERROR_DESCRIPTION	_T("error_description")
#define OAUTH_ERROR_URL			_T("error_uri")
#define OAUTH_GRANT_TYP			_T("grant_type")
#define OAUTH_GRANT_TYPE_AUTH	_T("authorization_code")
#define OAUTH_CODE				_T("code")
#define OAUTH_ACCESS_TOKEN		_T("access_token")
#define OAUTH_TOKEN_TYPE		_T("token_type")
#define OAUTH_TOKEN_TYPE_BEARER	_T("bearer")
#define OAUTH_TOKEN_TYPE_MAC	_T("mac")
#define OAUTH_EXPIRES_IN		_T("expires_in")
#define OAUTH_USERNAME			_T("username")
#define OAUTH_PASSWORD			_T("password")
#define OAUTH_REFRESH_TOKEN		_T("refresh_token")

XDL_API xhand_t oauth_cli(const tchar_t* rourl, link_t_ptr ptr_json);

#endif /*XDL_SUPPORT_SOCK*/

#endif /*NETOAU_H*/