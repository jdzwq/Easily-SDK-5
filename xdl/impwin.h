/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget document

	@module	impwin.h | interface file

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

#ifndef _IMPWIN_H
#define _IMPWIN_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_WIDGET


#ifdef	__cplusplus
extern "C" {
#endif

XDL_API bool_t fetch_message(msg_t* pmsg, res_win_t wt);

XDL_API bool_t peek_message(msg_t* pmsg, res_win_t wt);

XDL_API bool_t	translate_message(const msg_t* pmsg);

XDL_API result_t dispatch_message(const msg_t* pmsg);

XDL_API int		translate_accelerator(res_win_t wt, res_acl_t acl, msg_t* pmsg);

XDL_API void	message_position(xpoint_t* ppt);

/*
@FUNCTION widget_create: create frame, popup, child widget or dialog window.
@INPUT const tchar_t* wname: widget name.
@INPUT dword_t wstyle: widget style, it can be WD_STYLE_CONTROL, WD_STYLE_POPUP, WD_STYLE_DIALOG, WD_STYLE_FRAME.
@INPUT const xrect_t* pxr: rect struct for widget initialize position and size.
if wstyle is WD_STYLE_CONTROL the rect belong to parent window client, otherwise the rect is screen coordinate based.
@INPUT res_win_t wparent: the parent window resource handle, child widget must have a parent window.
@INPUT if_event_t* pev: the window message dispatch struct.
@RETURN res_win_t: if succeeds retur window resource handle, fails return NULL.
*/
XDL_API res_win_t widget_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev);

/*
@FUNCTION widget_destroy: destroy the widget.
@INPUT res_win_t wt: widget resource handle.
@RETURN void: none.
*/
XDL_API	void	widget_destroy(res_win_t wt);

/*
@FUNCTION widget_close: close the widget and return a state value.
@INPUT res_win_t wt: widget resource handle.
@INPUT int ret: the return value.
@RETURN void: none.
*/
XDL_API	void	widget_close(res_win_t wt, int ret);

/*
@FUNCTION widget_get_return: get widget returned state value at closing.
@INPUT res_win_t wt: widget resource handle.
@RETURN int: the closed widget return value, default is zero.
*/
XDL_API int		widget_get_return(res_win_t wt);

/*
@FUNCTION widget_get_dispatch: get the widget message dispatch struct.
@INPUT res_win_t wt: widget resource handle.
@RETURN if_event_t: return the dispatch struct if exists, else return NULL.
*/
XDL_API if_event_t* widget_get_dispatch(res_win_t wt);

/*
@FUNCTION widget_set_style: reset the widget style.
@INPUT res_win_t wt: widget resource handle.
@INPUT dword_t ws: widget style, it can be WD_STYLE_HSCROLL, WD_STYLE_VSCROLL, WD_STYLE_PAGING, or combined.
@RETURN void: none.
*/
XDL_API void	widget_set_style(res_win_t wt, dword_t ws);

/*
@FUNCTION widget_get_style: get the widget style.
@INPUT res_win_t wt: widget resource handle.
@RETURN dword_t: widget style, it can be WD_STYLE_HSCROLL, WD_STYLE_VSCROLL, WD_STYLE_PAGING, or combined.
*/
XDL_API dword_t widget_get_style(res_win_t wt);

/*
@FUNCTION widget_set_core_delta: set the widget defined data.
@INPUT res_win_t wt: widget resource handle.
@INPUT var_long pd: the widget defined data.
@RETURN void: none.
*/
XDL_API void	widget_set_core_delta(res_win_t wt, var_long pd);

/*
@FUNCTION widget_get_core_delta: get the widget defined data.
@INPUT res_win_t wt: widget resource handle.
@RETURN var_long: widget defined data.
*/
XDL_API var_long	widget_get_core_delta(res_win_t wt);

/*
@FUNCTION widget_set_user_delta: set the user defined data.
@INPUT res_win_t wt: widget resource handle.
@INPUT var_long pd: the user defined data.
@RETURN void: none.
*/
XDL_API void	widget_set_user_delta(res_win_t wt, var_long pd);

/*
@FUNCTION widget_get_user_delta: get the user defined data.
@INPUT res_win_t wt: widget resource handle.
@RETURN var_long: user defined data.
*/
XDL_API var_long	widget_get_user_delta(res_win_t wt);

/*
@FUNCTION widget_set_user_id: set the user control id.
@INPUT res_win_t wt: widget resource handle.
@INPUT dword_t uid: the user control id.
@RETURN void: none.
*/
XDL_API void	widget_set_user_id(res_win_t wt, dword_t uid);

/*
@FUNCTION widget_get_user_id: get the user control id.
@INPUT res_win_t wt: widget resource handle.
@RETURN dword_t: user control id.
*/
XDL_API dword_t widget_get_user_id(res_win_t wt);

/*
@FUNCTION widget_set_owner: set widget owner window, widget will send command, notice message to owner window.
@INPUT res_win_t wt: the widget resource handle.
@INPUT res_win_t owner: the owner window resource handle.
@RETURN void: none.
*/
XDL_API void	widget_set_owner(res_win_t wt, res_win_t owner);

/*
@FUNCTION widget_get_owner: get widget owner window, widget will send command, notice message to owner window.
@INPUT res_win_t wt: the widget resource handle.
@RETURN res_win_t: if succeeds return  owner window resource handle, fails return NULL.
*/
XDL_API res_win_t widget_get_owner(res_win_t wt);

/*
@FUNCTION widget_get_child: get child widget by control id.
@INPUT res_win_t wt: the widget resource handle.
@INPUT dword_t uid: the control id.
@RETURN res_win_t: if succeeds return child window resource handle, fails return NULL.
*/
XDL_API res_win_t widget_get_child(res_win_t wt, dword_t uid);

/*
@FUNCTION widget_get_parent: get parent widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN res_win_t: if succeeds return parent window resource handle, fails return NULL.
*/
XDL_API res_win_t widget_get_parent(res_win_t wt);

/*
@FUNCTION widget_set_user_prop: set widget property.
@INPUT res_win_t wt: the widget resource handle.
@INPUT const tchar_t* pkey: the property key token.
@INPUT var_long pval: the property value.
@RETURN void: none.
*/
XDL_API void	widget_set_user_prop(res_win_t wt, const tchar_t* pkey, var_long pval);

/*
@FUNCTION widget_get_user_prop: get widget property value by key.
@INPUT res_win_t wt: the widget resource handle.
@INPUT const tchar_t* pkey: the property key token.
@RETURN var_long: return the property value if exists, otherwise return zero.
*/
XDL_API var_long	widget_get_user_prop(res_win_t wt, const tchar_t* pkey);

/*
@FUNCTION widget_get_user_prop: delete widget property by key and return the value stored.
@INPUT res_win_t wt: the widget resource handle.
@INPUT const tchar_t* pkey: the property key token.
@RETURN var_long: return the property value if exists, otherwise return zero.
*/
XDL_API var_long	widget_del_user_prop(res_win_t wt, const tchar_t* pkey);

/*
@FUNCTION widget_client_ctx: reference a widget client device context, it clipped by widget client rectangle.
client context used to draw user view.
@INPUT res_win_t wt: the widget resource handle.
@RETURN res_ctx_t: if succeeds return device context resource handle, fails return NULL.
*/
XDL_API res_ctx_t widget_client_ctx(res_win_t wt);

/*
@FUNCTION widget_window_ctx: reference a widget window device context, it clipped by widget window rectangle.
window context used to draw frame.
@INPUT res_win_t wt: the widget resource handle.
@RETURN res_ctx_t: if succeeds return device context resource handle, fails return NULL.
*/
XDL_API res_ctx_t widget_window_ctx(res_win_t wt);

/*
@FUNCTION widget_release_ctx: release client or window device context.
@INPUT res_win_t wt: the widget resource handle.
@INPUT res_ctx_t dc: the device context handle.
@RETURN void: none.
*/
XDL_API void	widget_release_ctx(res_win_t wt, res_ctx_t dc);

/*
@FUNCTION widget_get_client_rect: get client rectangle, the coordinate is window client based.
@INPUT res_win_t wt: the widget resource handle.
@OUTPUT xrect_t* prt: the rect struct.
@RETURN void: none.
*/
XDL_API void	widget_get_client_rect(res_win_t wt, xrect_t* prt);

/*
@FUNCTION widget_get_window_rect: get window rectangle, the coordinate is screen based.
@INPUT res_win_t wt: the widget resource handle.
@OUTPUT xrect_t* prt: the rect struct.
@RETURN void: none.
*/
XDL_API void	widget_get_window_rect(res_win_t wt, xrect_t* prt);

/*
@FUNCTION widget_get_window_edge: get window frame edge width and height in points.
@INPUT res_win_t wt: the widget resource handle.
@OUTPUT xsize_t* pxs: the size struct.
@RETURN void: none.
*/
XDL_API void	widget_get_window_edge(res_win_t wt, xsize_t* pxs);

/*
@FUNCTION widget_client_to_screen: mapping client points to screen coordinate.
@INPUT res_win_t wt: the widget resource handle.
@INOUTPUT xpoint_t* pst: the point struct for inputing the client point and outputing the screen point.
@RETURN void: none.
*/
XDL_API void	widget_client_to_screen(res_win_t wt, xpoint_t* pst);

/*
@FUNCTION widget_screen_to_client: mapping screen points to client coordinate.
@INPUT res_win_t wt: the widget resource handle.
@INOUTPUT xpoint_t* pst: the point struct for inputing the screen point and outputing the client point.
@RETURN void: none.
*/
XDL_API void	widget_screen_to_client(res_win_t wt, xpoint_t* pst);

/*
@FUNCTION widget_client_to_window: mapping client points to window coordinate.
@INPUT res_win_t wt: the widget resource handle.
@INOUTPUT xpoint_t* pst: the point struct for inputing the client point and outputing the window point.
@RETURN void: none.
*/
XDL_API void	widget_client_to_window(res_win_t wt, xpoint_t* pst);

/*
@FUNCTION widget_window_to_client: mapping window points to client coordinate.
@INPUT res_win_t wt: the widget resource handle.
@INOUTPUT xpoint_t* pst: the point struct for inputing the window point and outputing the client point.
@RETURN void: none.
*/
XDL_API void	widget_window_to_client(res_win_t wt, xpoint_t* pst);

/*
@FUNCTION widget_calc_border: calc the window frame border information according to window style.
@INPUT dword_t ws: the widget style.
@OUTPUT border_t* pbd: the border struct.
@RETURN void: none.
*/
XDL_API void	widget_calc_border(dword_t ws, border_t* pbd);

/*
@FUNCTION widget_adjust_size: use window style to calcing window frame suitable size by the client request size.
@INPUT dword_t ws: the widget style.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widdow size.
@RETURN void: none.
*/
XDL_API void	widget_adjust_size(dword_t ws, xsize_t* pxs);

/*
@FUNCTION widget_size: resize widget client.
@INPUT res_win_t wt: the widget resource handle.
@INPUT const xsize_t* pxs: the new size.
@RETURN void: none.
*/
XDL_API void	widget_size(res_win_t wt, const xsize_t* pxs);

/*
@FUNCTION widget_move: move widget to new position.
@INPUT res_win_t wt: the widget resource handle.
@INPUT const xpoint_t* ppt: the new point.
@RETURN void: none.
*/
XDL_API void	widget_move(res_win_t wt, const xpoint_t* ppt);

/*
@FUNCTION widget_move: change widget z-order.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int zor: the new z-order, it can be WD_ZORDER_NOTOPMOST, WD_ZORDER_BOTTOM, WD_ZORDER_TOP, WD_ZORDER_TOPMOST.
@RETURN void: none.
*/
XDL_API void	widget_take(res_win_t wt, int zor);

/*
@FUNCTION widget_show: show or hide widget.
@INPUT res_win_t wt: the widget resource handle.
@INPUT dword_t sw: the show mode, it can be WD_SHOW_NORMAL, WD_SHOW_HIDE, WD_SHOW_MAXIMIZE, WD_SHOW_MINIMIZE, WD_SHOW_FULLSCREEN, WD_SHOW_POPUPTOP.
@RETURN void: none.
*/
XDL_API void	widget_show(res_win_t wt, dword_t sw);

/*
@FUNCTION widget_center_window: centre the child or popup widget posotion to parent or screen center.
@INPUT res_win_t wt: the widget resource handle.
@INPUT res_win_t owner: the owner widget, if NULL indicate screen, else indicate parent widget.
@RETURN void: none.
*/
XDL_API void	widget_center_window(res_win_t wt, res_win_t owner);

/*
@FUNCTION widget_resize: resize whole widndow.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_resize(res_win_t wt);

/*
@FUNCTION widget_redraw: redraw rect in widget client.
@INPUT res_win_t wt: the widget resource handle.
@INPUT const xrect_t* prt: the rect need to redraw.
@INPUT bool_t b_erase: if nonzero the background need to redraw.
@RETURN void: none.
*/
XDL_API void	widget_redraw(res_win_t wt, const xrect_t* prt, bool_t b_erase);

/*
@FUNCTION widget_update_window: redraw whole widndow immediately.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_paint(res_win_t wt);

/*
@FUNCTION widget_update: redraw whole widndow.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_update(res_win_t wt);

/*
@FUNCTION widget_enable: enable or disable window, the window disabled can not get focus for inputing.
@INPUT res_win_t wt: the widget resource handle.
@INPUT bool_t b: nonezero for enable, zero for disable.
@RETURN void: none.
*/
XDL_API void	widget_enable(res_win_t wt, bool_t b);

/*
@FUNCTION widget_set_cursor: set widget cursor type.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int cur: cursor type, it can be CURSOR_SIZENS, CURSOR_SIZEWE, CURSOR_SIZEALL, CURSOR_HAND, CURSOR_HELP, CURSOR_DRAG, CURSOR_ARROW, CURSOR_IBEAM.
@RETURN void: none.
*/
XDL_API void	widget_set_cursor(res_win_t wt,int curs);

/*
@FUNCTION widget_set_capture: let widget capture or discard mouse input.
@INPUT res_win_t wt: the widget resource handle.
@INPUT bool_t b: nonezero for capturing, zero for discarding.
@RETURN void: none.
*/
XDL_API void	widget_set_capture(res_win_t wt,bool_t b);

/*
@FUNCTION widget_create_caret: create a widget input caret.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int w: the caret width in points.
@INPUT int h: the caret height in points.
@RETURN void: none.
*/
XDL_API void	widget_create_caret(res_win_t wt, int w, int h);

/*
@FUNCTION widget_destroy_caret: destroy widget input caret.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_destroy_caret(res_win_t wt);

/*
@FUNCTION widget_show_caret: show widget input caret.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int w: the caret width in points.
@INPUT int h: the caret height in points.
@INPUT bool_t b: nonzero for showing, zero for hiding.
@RETURN void: none.
*/
XDL_API void	widget_show_caret(res_win_t wt, int x, int y, bool_t b);

/*
@FUNCTION widget_set_imm: open or close input context.
@INPUT res_win_t wt: the widget resource handle.
@INPUT bool_t b: nonzero for opening, zero for closing.
@RETURN void: none.
*/
XDL_API void	widget_set_imm(res_win_t wt, bool_t b);

/*
@FUNCTION widget_get_imm: test input context is opened.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate opened, zero indicate closed.
*/
XDL_API bool_t	widget_get_imm(res_win_t wt);

/*
@FUNCTION widget_set_focus: let the widget get focus.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_set_focus(res_win_t wt);

/*
@FUNCTION widget_key_state: test the key is pressed.
@INPUT res_win_t wt: the widget resource handle.
@INPUT byte_t key: the virtual key.
@RETURN bool_t: if return nonzero indicate is pressed, zero indicate released.
*/
XDL_API bool_t	widget_key_state(res_win_t wt,byte_t key);

/*
@FUNCTION widget_is_hotvoer: test widget can track mouse hot.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate is tracked, zero indicate not.
*/
XDL_API bool_t	widget_is_hotvoer(res_win_t wt);

/*
@FUNCTION widget_is_editor: test widget is editor control.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate is editor, zero indicate not.
*/
XDL_API bool_t	widget_is_editor(res_win_t wt);

/*
@FUNCTION widget_is_valid: test widget is valid.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate is valid, zero indicate not.
*/
XDL_API bool_t	widget_is_valid(res_win_t wt);

/*
@FUNCTION widget_is_child: test widget is child.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate is child, zero indicate not.
*/
XDL_API bool_t	widget_is_child(res_win_t wt);

/*
@FUNCTION widget_is_focus: test widget is focused.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate is focused, zero indicate not.
*/
XDL_API bool_t	widget_is_focus(res_win_t wt);

/*
@FUNCTION widget_can_focus: test widget can be focused.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate can be focused, zero indicate can not be.
*/
XDL_API bool_t	widget_can_focus(res_win_t wt);

/*
@FUNCTION widget_can_paging: test widget can be paged.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate can be paged, zero indicate can not be.
*/
XDL_API bool_t	widget_can_paging(res_win_t wt);

/*
@FUNCTION widget_has_close: test widget has close button.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate so, zero indicate not so.
*/
XDL_API bool_t	widget_has_close(res_win_t wt);

/*
@FUNCTION widget_has_size: test widget has size button.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate so, zero indicate not so.
*/
XDL_API bool_t	widget_has_size(res_win_t wt);

/*
@FUNCTION widget_has_border: test widget has border.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if return nonzero indicate so, zero indicate not so.
*/
XDL_API bool_t	widget_has_border(res_win_t wt);

/*
@FUNCTION widget_get_scroll: get widget scroll information.
@INPUT res_win_t wt: the widget resource handle.
@INPUT bool_t horz: nonzero for horizon scroll, zero for vertical scroll.
@OUTPUT scroll_t* psl: scroll struct for returning information.
@RETURN void: none.
*/
XDL_API void	widget_get_scroll(res_win_t wt, bool_t horz, scroll_t* psl);

/*
@FUNCTION widget_set_scroll: set widget scroll information.
@INPUT res_win_t wt: the widget resource handle.
@INPUT bool_t horz: nonzero for horizon scroll, zero for vertical scroll.
@INPUT const scroll_t* psl: scroll struct for setting scroll information.
@RETURN void: none.
*/
XDL_API void	widget_set_scroll(res_win_t wt, bool_t horz, const scroll_t* psc);

/*
@FUNCTION widget_reset_scroll: reset widget scroll.
@INPUT res_win_t wt: the widget resource handle.
@INPUT bool_t horz: nonzero for horizon scroll, zero for vertical scroll.
@RETURN void: none.
*/
XDL_API void	widget_reset_scroll(res_win_t wt, bool_t horz);

/*
@FUNCTION widget_scroll: scroll the widget.
@INPUT res_win_t wt: the widget resource handle.
@INPUT bool_t horz: nonzero for horizon scroll, zero for vertical scroll.
@INPUT long line: the line to be scrolled
@RETURN void: none.
*/
XDL_API void	widget_scroll(res_win_t wt, bool_t horz, long line);

/*
@FUNCTION widget_post_char: post a char input message into windows message queue.
@INPUT res_win_t wt: the widget resource handle.
@INPUT tchar_t ch: the character.
@RETURN void: none.
*/
XDL_API void	widget_post_char(res_win_t wt, tchar_t ch);

/*
@FUNCTION widget_post_char: post a key press message into windows message queue, and not wait the message processed.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int key: the key code, eg: KEY_*.
@RETURN void: none.
*/
XDL_API void	widget_post_key(res_win_t wt, int key);

/*
@FUNCTION widget_send_notice: send notice message to owner window, and not wait the message processed.
@INPUT res_win_t wt: the widget resource handle.
@INPUT NOTICE* pnt: the notice message struct.
@RETURN int: the notice message return code, default is zero.
*/
XDL_API int		widget_send_notice(res_win_t wt, NOTICE* pnt);

/*
@FUNCTION widget_post_command: post command message to owner window, and not wait the message processed.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int code: the command message code.
@INPUT int cid: the control id of the widget.
@INPUT var_long data: the extract data posed with command message.
@RETURN void: none.
*/
XDL_API void	widget_post_command(res_win_t wt, int code, int cid, var_long data);

/*
@FUNCTION widget_send_command: send command message to owner window, and wait the message processed.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int code: the command message code.
@INPUT int cid: the control id of the widget.
@INPUT var_long data: the extract data posed with command message.
@RETURN int: the command message return code, default is zero.
*/
XDL_API int		widget_send_command(res_win_t wt, int code, int cid, var_long data);

/*
@FUNCTION widget_set_title: set widget title.
@INPUT res_win_t wt: the widget resource handle.
@INPUT const tchar_t* token: the title token.
@RETURN void: none.
*/
XDL_API void	widget_set_title(res_win_t wt, const tchar_t* token);

/*
@FUNCTION widget_get_title: get widget title.
@INPUT res_win_t wt: the widget resource handle.
@OUTPUT tchar_t* buf: string buffer for returning title.
@INPUT int max: string buffer maximize size, not include terminate character.
@RETURN int: the returned title length in characters.
*/
XDL_API int		widget_get_title(res_win_t wt, tchar_t* buf, int max);

/*
@FUNCTION widget_is_maximized: test widget is maximized.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if nonzero indicate so, zero indicate not so.
*/
XDL_API bool_t	widget_is_maximized(res_win_t wt);

/*
@FUNCTION widget_is_minimized: test widget is minimized.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: if nonzero indicate so, zero indicate not so.
*/
XDL_API bool_t	widget_is_minimized(res_win_t wt);

/*
@FUNCTION widget_set_subproc: set widget subclass routing.
@INPUT res_win_t wt: the widget resource handle.
@INPUT dword_t sid: the subclass id.
@INPUT if_subproc_t* sub: the subclass message dispatch struct.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	widget_set_subproc(res_win_t wt, dword_t sid, if_subproc_t* sub);

/*
@FUNCTION widget_del_subproc: delete widget subclass routing.
@INPUT res_win_t wt: the widget resource handle.
@INPUT dword_t sid: the subclass id.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API void	widget_del_subproc(res_win_t wt, dword_t sid);

/*
@FUNCTION widget_set_subproc_delta: set widget subclass routing extract data.
@INPUT res_win_t wt: the widget resource handle.
@INPUT dword_t sid: the subclass id.
@INPUT var_long delta: the subclass extract data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	widget_set_subproc_delta(res_win_t wt, dword_t sid, var_long delta);

/*
@FUNCTION widget_get_subproc_delta: get widget subclass routing extract data.
@INPUT res_win_t wt: the widget resource handle.
@INPUT dword_t sid: the subclass id.
@RETURN var_long: return the subclass extract data if exists, otherwise return zero.
*/
XDL_API var_long widget_get_subproc_delta(res_win_t wt, dword_t sid);

/*
@FUNCTION widget_has_subproc: test widget has subclass routing.
@INPUT res_win_t wt: the widget resource handle.
@RETURN bool_t: return nonzero if exists, otherwise return zero.
*/
XDL_API bool_t	widget_has_subproc(res_win_t wt);

/*
@FUNCTION widget_set_timer: set widget timer routing.
@INPUT res_win_t wt: the widget resource handle.
@INPUT int ms: the time period in millisecond.
@RETURN var_long: if succeeds return timer id, otherwise return zero.
*/
XDL_API var_long widget_set_timer(res_win_t wt, int ms);

/*
@FUNCTION widget_kill_timer: remove widget timer routing.
@INPUT res_win_t wt: the widget resource handle.
@INPUT var_long tid: the timer id.
@RETURN void: none.
*/
XDL_API void	widget_kill_timer(res_win_t wt, var_long tid);

/*
@FUNCTION widget_attach_accel: attach a accelerator to widget.
@INPUT res_win_t wt: the widget resource handle.
@INPUT res_acl_t acl: the accelerator resource handle.
@RETURN void: none.
*/
XDL_API void	widget_attach_accel(res_win_t wt, res_acl_t acl);

/*
@FUNCTION widget_get_accel: get the accelerator attached to widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN res_acl_t: return accelerator resource handle if exists, otherwise return zero.
*/
XDL_API res_acl_t widget_get_accel(res_win_t wt);

/*
@FUNCTION widget_detach_accel: detach the accelerator from widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN res_acl_t: return accelerator resource handle if exists, otherwise return zero.
*/
XDL_API res_acl_t widget_detach_accel(res_win_t wt);

/*
@FUNCTION widget_get_canvas: get the canvas from widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN canvas_t: return canvas handle if exists, otherwise return zero.
*/
XDL_API canvas_t widget_get_canvas(res_win_t wt);

/*
@FUNCTION widget_copy: send a copy message to widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_copy(res_win_t wt);

/*
@FUNCTION widget_cut: send a cut message to widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_cut(res_win_t wt);

/*
@FUNCTION widget_paste: send a paste message to widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_paste(res_win_t wt);

/*
@FUNCTION widget_undo: send a undo message to widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_undo(res_win_t wt);

/*
@FUNCTION widget_enum_child: enumerate child widgets.
@INPUT res_win_t wt: the widget resource handle.
@INPUT PF_ENUM_WINDOW_PROC pf: the callback function for every child widget enumerated.
@INPUT var_long pv: the parameter translated into PF_ENUM_WINDOW_PROC function.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	widget_enum_child(res_win_t wt, PF_ENUM_WINDOW_PROC pf, var_long pv);

/*
@FUNCTION create_accel_table: create accelerate resource.
@INPUT const accel_t* pac: the accelerate table struct.
@INPUT int n: the entities of accelerate table.
@RETURN res_acl_t: if succeeds return accelerate resource handle, fails return zero.
*/
XDL_API res_acl_t create_accel_table(const accel_t* pac, int n);

/*
@FUNCTION create_accel_table: create accelerate resource.
@INPUT res_acl_t hac: the accelerate resource handle.
@RETURN void: none.
*/
XDL_API void	destroy_accel_table(res_acl_t hac);

/*
@FUNCTION get_screen_size: calc screen size in points.
@OUTPUT xsize_t* pxs: the size struct.
@RETURN void: none.
*/
XDL_API void	get_screen_size(xsize_t* pxs);

/*
@FUNCTION get_desktop_size: calc desktop size in points.
@OUTPUT xsize_t* pxs: the size struct.
@RETURN void: none.
*/
XDL_API void	get_desktop_size(xsize_t* pxs);

/*
@FUNCTION screen_size_to_pt: mapping points size to millimeter size.
@INOUTPUT xsize_t* pxs: the size struct for input points size and return millimeter size.
@RETURN void: none.
*/
XDL_API void	screen_size_to_pt(xsize_t* pxs);

/*
@FUNCTION screen_size_to_tm: mapping millimeter size to points size.
@INOUTPUT xsize_t* pxs: the size struct for input millimeter size and return points size.
@RETURN void: none.
*/
XDL_API void	screen_size_to_tm(xsize_t* pxs);

#ifdef XDK_SUPPORT_WIDGET_EX
/*
@FUNCTION widget_track_mouse: let widget track mouse input.
@INPUT res_win_t wt: the widget resource handle.
@INPUT dword_t mask: the mouse event mask, it can be TME_HOVER, TME_LEAVE, TME_CANCEL or combined.
@RETURN void: none.
*/
XDL_API void	widget_track_mouse(res_win_t wt, dword_t mask);

/*
@FUNCTION widget_set_alpha: set widget alphablend level.
@INPUT res_win_t wt: the widget resource handle.
@INPUT byte_t b: the alphablend level: 0~255, or use predefined value: ALPHA_SOLID(250), ALPHA_SOFT(128), ALPHA_TRANS(64).
@RETURN void: none.
*/
XDL_API void	widget_set_alpha(res_win_t wt, byte_t b);

/*
@FUNCTION widget_get_alpha: get widget alphablend level.
@INPUT res_win_t wt: the widget resource handle.
@RETURN byte_t: the alphablend level, 255 for not setting alphablend level.
*/
XDL_API byte_t	widget_get_alpha(res_win_t wt);

/*
@FUNCTION widget_set_region: set widget shape region.
@INPUT res_win_t wt: the widget resource handle.
@INPUT res_rgn_t rgn: the widget shape region resource handle.
@RETURN void: none.
*/
XDL_API void	widget_set_region(res_win_t wt, res_rgn_t rgn);
#endif

/*
@FUNCTION widget_do_modal: run the widget in modal mode, usually used by dialog.
@INPUT res_win_t wt: the widget resource handle.
@RETURN int: the widget modal ending code.
*/
XDL_API int		widget_do_modal(res_win_t wt);

/*
@FUNCTION widget_do_modal: run the widget in trace mode, usually used by memu.
@INPUT res_win_t wt: the widget resource handle.
@RETURN void: none.
*/
XDL_API void	widget_do_trace(res_win_t wt);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_WIDGET*/

#endif /*IMPWIN_H*/