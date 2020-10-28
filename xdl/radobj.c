/***********************************************************************
	Easily xdl v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc radobj document

	@module	radobj.c | rad network document implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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

#include "radobj.h"
#include "varobj.h"
#include "miscell.h"

#include "xdlimp.h"

#include "xdlinit.h"

dword_t radobj_write(object_t val, const rad_hdr_t* phr, const byte_t* msg, dword_t len)
{
	byte_t* buf = NULL;
	dword_t total = 0;
	dword_t obj_len;
	byte_t* sub_buf;
	dword_t sub_len;
	byte_t* han_buf;
	sword_t han_len;
	byte_t* msg_buf;
	dword_t msg_len;

	dword_t new_len = 0;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	xdate_t dt_cur, dt_org;

	TRY_CATCH;

	if (is_null(phr->utc))
		get_utc_date(&dt_cur);
	else
		parse_datetime(&dt_cur, phr->utc);

	obj_len = object_get_bytes(val, NULL, MAX_LONG);
	new_len = 4 + (2 + MSGHAN_SIZE) + (4 + len);
	buf = (byte_t*)xmem_alloc(obj_len + new_len);
	//copy origin object_list
	object_get_bytes(val, buf, obj_len);

	total = 0;
	while (total < obj_len)
	{
		//the object size
		sub_buf = buf + total + 4;
		sub_len = GET_DWORD_NET((sub_buf - 4), 0);
		if (sub_len > MAX_LONG)
		{
			raise_user_error(_T("radobj_write"), _T("invalid message total size"));
		}
		total += 4;

		//the object handler
		han_buf = sub_buf + 2;
		han_len = GET_SWORD_NET((han_buf - 2), 0);
		if (han_len > MAX_SHORT)
		{
			raise_user_error(_T("radobj_write"), _T("invalid message handler size"));
		}
		total += 2;
		if (han_len >= MSGHAN_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((han_buf + 8), UTC_LEN, sz_date, UTC_LEN);
#else
			utf8_to_mbs((han_buf + 8), UTC_LEN, sz_date, UTC_LEN);
#endif
			parse_datetime(&dt_org, sz_date);
		}
		else
		{
			get_min_date(&dt_org);
		}
		total += han_len;

		//the object message
		msg_buf = han_buf + han_len + 4;
		msg_len = GET_DWORD_NET((msg_buf - 4), 0);
		if (msg_len > MAX_LONG)
		{
			raise_user_error(_T("radobj_write"), _T("invalid message element size"));
		}
		total += 4;

		total += msg_len;

		if (compare_datetime(&dt_cur, &dt_org) < 0)
		{
			total -= (4 + (2 + han_len) + (4 + msg_len));
			break;
		}
	}

	if (total < obj_len)
	{
		xmem_move((void*)(buf + total), (obj_len - total), new_len);
		xmem_zero((void*)(buf + total), new_len);
	}

	//the object total size
	sub_buf = buf + total + 4;
	sub_len = ((2 + MSGHAN_SIZE) + (4 + len));
	PUT_DWORD_NET((sub_buf - 4), 0, sub_len);
	total = 4;

	//the object handler size
	han_buf = sub_buf + 2;
	han_len = MSGHAN_SIZE;
	PUT_SWORD_NET((han_buf - 2), 0, han_len);
	total += 2;
	//the object handler
	xmem_copy((void*)(han_buf), (void*)phr->ver, MSGVER_SIZE);
	PUT_SWORD_NET(han_buf, 4, (phr->qos));
	PUT_SWORD_NET(han_buf, 6, phr->mid);
#if defined(_UNICODE) || defined(UNICODE)
	ucs_to_utf8(phr->utc, UTC_LEN, (han_buf + 8), UTC_LEN);
#else
	mbs_to_utf8(phr->utc, UTC_LEN, (han_buf + 8), UTC_LEN);
#endif
	total += han_len;

	//the object message size
	msg_buf = han_buf + MSGHAN_SIZE + 4;
	msg_len = len;
	PUT_DWORD_NET((msg_buf - 4), 0, msg_len);
	total += 4;
	//the object message
	xmem_copy((void*)(msg_buf), (void*)msg, len);
	total += len;

	total += obj_len;
	//set new object list
	object_set_bytes(val, _UTF8, buf, total);

	xmem_free(buf);
	buf = NULL;

	END_CATCH;

	return new_len;
ONERROR:

	if (buf)
		xmem_free(buf);

	return 0;
}

dword_t radobj_read(object_t val, rad_hdr_t* phr, byte_t* msg, dword_t max)
{
	byte_t* buf = NULL;
	dword_t total = 0;
	dword_t obj_len;
	byte_t* sub_buf;
	dword_t sub_len;
	byte_t* han_buf;
	sword_t han_len;
	byte_t* msg_buf;
	dword_t msg_len;

	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	xdate_t dt_cur, dt_org;

	TRY_CATCH;

	if (is_null(phr->utc))
		get_min_date(&dt_cur);
	else
		parse_datetime(&dt_cur, phr->utc);

	obj_len = object_get_bytes(val, NULL, MAX_LONG);
	buf = (byte_t*)xmem_alloc(obj_len);
	//copy origin object_list
	object_get_bytes(val, buf, obj_len);

	total = 0;
	while (total < obj_len)
	{
		//the object size
		sub_buf = buf + total + 4;
		sub_len = GET_DWORD_NET((sub_buf - 4), 0);
		if (sub_len > MAX_LONG)
		{
			raise_user_error(_T("radobj_read"), _T("invalid message total size"));
		}
		total += 4;

		//the object handler
		han_buf = sub_buf + 2;
		han_len = GET_SWORD_NET((han_buf - 2), 0);
		if (han_len > MAX_SHORT)
		{
			raise_user_error(_T("radobj_read"), _T("invalid message handler size"));
		}
		total += 2;
		if (han_len >= MSGHAN_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((han_buf + 8), UTC_LEN, sz_date, UTC_LEN);
#else
			utf8_to_mbs((han_buf + 8), UTC_LEN, sz_date, UTC_LEN);
#endif
			parse_datetime(&dt_org, sz_date);
		}
		else
		{
			get_min_date(&dt_org);
		}
		total += han_len;

		//the object message
		msg_buf = han_buf + han_len + 4;
		msg_len = GET_DWORD_NET((msg_buf - 4), 0);
		if (msg_len > MAX_LONG)
		{
			raise_user_error(_T("radobj_read"), _T("invalid message element size"));
		}
		total += 4;

		total += msg_len;

		if (compare_datetime(&dt_cur, &dt_org) <= 0)
		{
			total -= (4 + (2 + han_len) + (4 + msg_len));
			break;
		}
	}

	if (total < obj_len)
	{
		//the object size
		sub_buf = buf + total + 4;
		sub_len = GET_DWORD_NET((sub_buf - 4), 0);
		if (sub_len > MAX_LONG)
		{
			raise_user_error(_T("radobj_read"), _T("invalid message total size"));
		}
		total += 4;

		xmem_set((void*)phr, 0, sizeof(rad_hdr_t));

		//the object handler
		han_buf = sub_buf + 2;
		han_len = GET_SWORD_NET((han_buf - 2), 0);
		if (han_len > MAX_SHORT)
		{
			raise_user_error(_T("radobj_read"), _T("invalid message handler size"));
		}
		total += 2;
		xmem_copy((void*)phr->ver, (void*)han_buf, MSGVER_SIZE);
		phr->qos = GET_SWORD_NET(han_buf, 4);
		phr->mid = GET_SWORD_NET(han_buf, 6);
		if (han_len >= MSGHAN_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((han_buf + 8), UTC_LEN, phr->utc, UTC_LEN);
#else
			utf8_to_mbs((han_buf + 8), UTC_LEN, phr->utc, UTC_LEN);
#endif
		}
		total += han_len;

		//the object message
		msg_buf = han_buf + han_len + 4;
		msg_len = GET_DWORD_NET((msg_buf - 4), 0);
		if (msg_len > MAX_LONG)
		{
			raise_user_error(_T("radobj_read"), _T("invalid message element size"));
		}
		total += 4;

		max = (max < msg_len) ? max : msg_len;

		if (msg)
		{
			xmem_copy((void*)msg, (void*)msg_buf, msg_len);
		}
	}
	else
	{
		max = 0;
	}

	xmem_free(buf);
	buf = NULL;

	END_CATCH;

	return max;
ONERROR:

	if (buf)
		xmem_free(buf);

	return 0;
}