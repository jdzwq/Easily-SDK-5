/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl oauth document

	@module	netoau.c | xdl http implement file

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

#include "netoau.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdlinit.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_SOCK


xhand_t oauth_send_user_auth_request(xhand_t xh, link_t_ptr ptr_json)
{
	link_t_ptr ilk;

	TRY_CATCH;

	ilk = get_json_item(ptr_json, OAUTH_RESPONSE_TYPE);
	xhttp_set_query_entity(xh, OAUTH_RESPONSE_TYPE, -1, ((ilk)? get_json_item_value_ptr(ilk) : NULL), -1);

	ilk = get_json_item(ptr_json, OAUTH_CLIENT_ID);
	xhttp_set_query_entity(xh, OAUTH_CLIENT_ID, -1, ((ilk) ? get_json_item_value_ptr(ilk) : NULL), -1);

	ilk = get_json_item(ptr_json, OAUTH_REDIRECT_URL);
	xhttp_set_query_entity(xh, OAUTH_REDIRECT_URL, -1, ((ilk) ? get_json_item_value_ptr(ilk) : NULL), -1);

	ilk = get_json_item(ptr_json, OAUTH_SCOPE);
	xhttp_set_query_entity(xh, OAUTH_SCOPE, -1, ((ilk) ? get_json_item_value_ptr(ilk) : NULL), -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return xh;
ONERROR:


	XDL_TRACE_LAST;

	return NULL;
}

#endif /*XDL_SUPPORT_SOCK*/
