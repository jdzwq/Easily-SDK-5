/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc event document

	@module	eventdoc.h | interface file

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

#ifndef _EVENTDOC_H
#define _EVENTDOC_H

#include "xdldef.h"
#include "variant.h"
#include "matrix.h"

#ifdef XDL_SUPPORT_DOC


#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************Functions**********************************************/

/*
@FUNCTION create_event_doc: create a event document.
@RETURN link_t_ptr: return the event document link component.
*/
EXP_API link_t_ptr create_event_doc(void);

/*
@FUNCTION destroy_event_doc: destroy a event document.
@INPUT link_t_ptr ptr: the event document link component.
@RETURN void: none.
*/
EXP_API void destroy_event_doc(link_t_ptr ptr);

/*
@FUNCTION clear_event_doc: clear event document, the parameter set will be emptied.
@INPUT link_t_ptr ptr: the event link component.
@RETURN void: node.
*/
EXP_API void clear_event_doc(link_t_ptr ptr);

/*
@FUNCTION is_event_doc: test is event document.
@INPUT link_t_ptr ptr: the event link component.
@RETURN bool_t: return nonzero if being a event, otherwise return zero.
*/
EXP_API bool_t is_event_doc(link_t_ptr ptr);

/*
@FUNCTION get_event_id: get event id.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning id.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_id(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_id_ptr: get event id string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_id_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_id: set event id.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* id: the event id.
@INPUT int len: the id string length.
@RETURN void: none.
*/
EXP_API void set_event_id(link_t_ptr ptr, const tchar_t* id, int len);

/*
@FUNCTION get_event_from: get event from.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning from.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_from(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_from_ptr: get event from string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_from_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_from: set event from.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* from: the event from.
@INPUT int len: the from string length.
@RETURN vofrom: none.
*/
EXP_API void set_event_from(link_t_ptr ptr, const tchar_t* from, int len);

/*
@FUNCTION get_event_sender: get event sender.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning sender.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_sender(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_sender_ptr: get event sender string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_sender_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_sender: set event sender.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* sender: the event sender.
@INPUT int len: the sender string length.
@RETURN vosender: none.
*/
EXP_API void set_event_sender(link_t_ptr ptr, const tchar_t* sender, int len);

/*
@FUNCTION get_event_send_time: get event send_time.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning send_time.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_send_time(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_send_time_ptr: get event send_time string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_send_time_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_send_time: set event send_time.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* send_time: the event send_time.
@INPUT int len: the send_time string length.
@RETURN vosend_time: none.
*/
EXP_API void set_event_send_time(link_t_ptr ptr, const tchar_t* send_time, int len);

/*
@FUNCTION get_event_code: get event code.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning code.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_code(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_code_ptr: get event code string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_code_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_code: set event code.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* code: the event code.
@INPUT int len: the code string length.
@RETURN vocode: none.
*/
EXP_API void set_event_code(link_t_ptr ptr, const tchar_t* code, int len);

/*
@FUNCTION get_event_describe: get event describe.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning describe.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_describe(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_describe_ptr: get event describe string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_describe_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_describe: set event describe.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* describe: the event describe.
@INPUT int len: the describe string length.
@RETURN vodescribe: none.
*/
EXP_API void set_event_describe(link_t_ptr ptr, const tchar_t* describe, int len);

/*
@FUNCTION get_event_medium: get event medium.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning medium.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_medium(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_medium_ptr: get event medium string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_medium_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_medium: set event medium.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* medium: the event medium.
@INPUT int len: the medium string length.
@RETURN vomedium: none.
*/
EXP_API void set_event_medium(link_t_ptr ptr, const tchar_t* medium, int len);

/*
@FUNCTION get_event_content: get event content.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning content.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_content(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_content_ptr: get event content string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_content_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_content: set event content.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* content: the event content.
@INPUT int len: the content string length.
@RETURN vocontent: none.
*/
EXP_API void set_event_content(link_t_ptr ptr, const tchar_t* content, int len);

/*
@FUNCTION get_event_to: get event to.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning to.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_to(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_to_ptr: get event to string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_to_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_to: set event to.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* to: the event to.
@INPUT int len: the to string length.
@RETURN voto: none.
*/
EXP_API void set_event_to(link_t_ptr ptr, const tchar_t* to, int len);

/*
@FUNCTION get_event_receiptor: get event receiptor.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning receiptor.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_receiptor(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_receiptor_ptr: get event receiptor string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_receiptor_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_receiptor: set event receiptor.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* receiptor: the event receiptor.
@INPUT int len: the receiptor string length.
@RETURN voreceiptor: none.
*/
EXP_API void set_event_receiptor(link_t_ptr ptr, const tchar_t* receiptor, int len);

/*
@FUNCTION get_event_receipt_time: get event receipt_time.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning receipt_time.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_receipt_time(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_receipt_time_ptr: get event receipt_time string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_receipt_time_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_receipt_time: set event receipt_time.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* receipt_time: the event receipt_time.
@INPUT int len: the receipt_time string length.
@RETURN voreceipt_time: none.
*/
EXP_API void set_event_receipt_time(link_t_ptr ptr, const tchar_t* receipt_time, int len);

/*
@FUNCTION get_event_receipt_state: get event receipt_state.
@INPUT link_t_ptr ptr: the event link component.
@INPUT tchar_t* buf: the characters buffer for returning receipt_state.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_event_receipt_state(link_t_ptr ptr, tchar_t* buf, int max);

/*
@FUNCTION get_event_receipt_state_ptr: get event receipt_state string token.
@INPUT link_t_ptr ptr: the event link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_event_receipt_state_ptr(link_t_ptr ptr);

/*
@FUNCTION set_event_receipt_state: set event receipt_state.
@INPUT link_t_ptr ptr: the event link component.
@INPUT const tchar_t* receipt_state: the event receipt_state.
@INPUT int len: the receipt_state string length.
@RETURN voreceipt_state: none.
*/
EXP_API void set_event_receipt_state(link_t_ptr ptr, const tchar_t* receipt_state, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*EVENTDOC_H*/
