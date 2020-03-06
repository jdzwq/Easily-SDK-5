/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdu system call interface document

	@module	xduiml.h | interface file

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

#ifndef _XDUIML_H
#define	_XDUIML_H

#include "xdudef.h"

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef XDU_SUPPORT_SHELL
bool_t	_shell_get_runpath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_curpath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_apppath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_docpath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_tmppath(tchar_t* pathbuf, int pathlen);
#ifdef XDU_SUPPORT_SHELL_DIALOG
bool_t	_shell_get_filename(res_win_t owner, const tchar_t* defpath, const tchar_t* filter, const tchar_t* defext, bool_t saveit, tchar_t* pathbuf, int pathlen, tchar_t* filebuf, int filelen);
bool_t	_shell_get_pathname(res_win_t owner, const tchar_t* defpath, bool_t createit, tchar_t* pathbuf, int pathlen);
#endif
#endif /*XDU_SUPPORT_SHELL*/
/***************************************************************************************************************************/
#ifdef XDU_SUPPORT_CONTEXT
int		_context_startup(void);
void	_context_cleanup(void);
res_ctx_t _create_display_context(res_win_t wt);
res_ctx_t _create_compatible_context(res_ctx_t rdc, int cx, int cy);
void	_destroy_context(res_ctx_t rdc);
void	_get_device_caps(res_ctx_t rdc, dev_cap_t* pcap);
void	_render_context(res_ctx_t src, int srcx, int srcy, res_ctx_t dst, int dstx, int dsty, int dstw, int dsth);

float	_pt_per_mm(res_ctx_t rdc, bool_t horz);
void	_text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, float* pfx, float* pfy);
void	_text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, int* pcx, int* pcy);
void	_text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, float* pfx, float* pfy);
void	_text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, int* pcx, int* pcy);
float	_cast_pt_to_mm(res_ctx_t rdc, int pt, bool_t horz);
int	_cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz);
int		_font_size(res_ctx_t rdc, int height);
#ifdef XDU_SUPPORT_CONTEXT_REGION
res_rgn_t _create_region(const tchar_t* shape, const xrect_t* pxr);
void	_delete_region(res_rgn_t rgn);
bool_t	_pt_in_region(res_rgn_t rgn, const xpoint_t* ppt);
#endif
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
res_bmp_t _create_color_bitmap(res_ctx_t rdc, const xcolor_t* pxc, int w, int h);
res_bmp_t _create_pattern_bitmap(res_ctx_t rdc, const xcolor_t* pxc_front, const xcolor_t* pxc_back, int w, int h, const tchar_t* lay);
res_bmp_t _create_gradient_bitmap(res_ctx_t rdc, const xcolor_t* pxc_near, const xcolor_t* pxc_center, int w, int h, const tchar_t* lay);
res_bmp_t _create_code128_bitmap(res_ctx_t rdc, int w, int h, const unsigned char* bar_buf, dword_t bar_len, const tchar_t* text);
res_bmp_t _create_pdf417_bitmap(res_ctx_t rdc, int w, int h, const unsigned char* bar_buf, dword_t bar_len, int rows, int cols);
res_bmp_t _create_qrcode_bitmap(res_ctx_t rdc, int w, int h, const unsigned char* bar_buf, dword_t bar_len, int rows, int cols);
res_bmp_t _create_storage_bitmap(res_ctx_t rdc, const tchar_t* filename);
res_bmp_t _load_bitmap_from_bytes(res_ctx_t rdc, const unsigned char* pb, dword_t len);
dword_t	_save_bitmap_to_bytes(res_ctx_t rdc, res_bmp_t rb, unsigned char* pb, dword_t max);
dword_t	_get_bitmap_bytes(res_bmp_t rdc);
void	_get_bitmap_size(res_bmp_t rb, int* pw, int* ph);
void	_destroy_bitmap(res_bmp_t bmp);
#ifdef XDU_SUPPORT_SHELL
res_bmp_t _load_bitmap_from_icon(res_ctx_t rdc, const tchar_t* iname);
res_bmp_t _load_bitmap_from_thumb(res_ctx_t rdc, const tchar_t* fname);
#endif
#endif
#ifdef XDU_SUPPORT_CONTEXT_PRINTER
bool_t _default_printer_mode(dev_prn_t* pmod);
bool_t _setup_printer_mode(res_win_t wnd, dev_prn_t* pmod);
res_ctx_t _create_printer_context(const dev_prn_t* pmod);
void	_destroy_printer_context(res_ctx_t rdc);
void	_begin_page(res_ctx_t rdc);
void	_end_page(res_ctx_t rdc);
void	_begin_doc(res_ctx_t rdc, const tchar_t* docname);
void	_end_doc(res_ctx_t rdc);
#endif

#ifdef XDU_SUPPORT_CONTEXT_GRAPHIC
void _gdi_init(int osv);
void _gdi_uninit(void);
void _gdi_draw_line(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);
void _gdi_draw_polyline(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n);
void _gdi_draw_polygon(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int n);
void _gdi_draw_bezier(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);
void _gdi_draw_curve(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n);
void _gdi_draw_rect(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt);
void _gdi_gradient_rect(res_ctx_t rdc, const xgradi_t* pxg, const xrect_t* prt);
void _gdi_alphablend_rect(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity);
void _gdi_draw_round(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt);
void _gdi_draw_ellipse(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt);
void _gdi_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang);
void _gdi_draw_arc(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int rx, int ry, double fang, double tang);
void _gdi_draw_arrow(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt, int alen, double arc);
void _gdi_draw_text(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* prt, const tchar_t* txt, int len);
void _gdi_text_out(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);
void _gdi_exclip_rect(res_ctx_t rdc, const xrect_t* pxr);
void _gdi_inclip_rect(res_ctx_t rdc, const xrect_t* pxr);
void _gdi_text_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);
void _gdi_text_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
void _gdi_draw_image(res_ctx_t rdc, res_bmp_t bmp, const tchar_t* clr, const xrect_t* prt);
void _gdi_draw_bitmap(res_ctx_t rdc, res_bmp_t bmp, const xrect_t* prt);
#endif
#ifdef XDU_SUPPORT_CONTEXT_REGION
void _gdi_fill_region(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn);
#endif
#if defined(UNICODE) || defined(_UNICODE)
void _gdiplus_init(int osv);
void _gdiplus_uninit(void);
void _gdiplus_draw_line(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);
void _gdiplus_draw_polyline(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n);
void _gdiplus_draw_polygon(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int n);
void _gdiplus_draw_bezier(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);
void _gdiplus_draw_curve(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n);
void _gdiplus_draw_rect(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt);
void _gdiplus_draw_path(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa);
void _gdiplus_gradient_rect(res_ctx_t rdc, const xgradi_t* pxg, const xrect_t* prt);
void _gdiplus_alphablend_rect(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity);
void _gdiplus_draw_round(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt);
void _gdiplus_draw_ellipse(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt);
void _gdiplus_draw_pie(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang);
void _gdiplus_draw_arc(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int rx, int ry, double fang, double tang);
void _gdiplus_draw_arrow(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t*pxb, const xrect_t* prt, int alen, double arc);
void _gdiplus_draw_text(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* prt, const tchar_t* txt, int len);
void _gdiplus_text_out(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);
void _gdiplus_draw_image(res_ctx_t rdc, res_bmp_t bmp, const tchar_t* clr, const xrect_t* prt);
void _gdiplus_draw_bitmap(res_ctx_t rdc, res_bmp_t bmp, const xrect_t* prt);
void _gdiplus_fill_region(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn);
void _gdiplus_exclip_rect(res_ctx_t rdc, const xrect_t* pxr);
void _gdiplus_inclip_rect(res_ctx_t rdc, const xrect_t* pxr);
void _gdiplus_text_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);
void _gdiplus_text_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);
#endif
#endif /*XDU_SUPPORT_CONTEXT_GRAPHIC*/
#endif /*XDU_SUPPORT_CONTEXT*/

#ifdef XDU_SUPPORT_CLIPBOARD
bool_t	_clipboard_open(void);
void	_clipboard_close(void);
void	_clipboard_clean(void);
bool_t	_clipboard_put(int fmt, res_glob_t gb);
res_glob_t _clipboard_get(int fmt);
#endif

#ifdef XDU_SUPPORT_WIDGET

void	_widget_startup(int ver);
void	_widget_cleanup(void);
res_win_t _widget_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev);
void	_widget_destroy(res_win_t wt);
void	_widget_close(res_win_t wt, int ret);
if_event_t* _widget_get_dispatch(res_win_t wt);
void	_widget_set_core_delta(res_win_t wt, var_long pd);
var_long	_widget_get_core_delta(res_win_t wt);
void	_widget_set_user_delta(res_win_t wt, var_long pd);
var_long	_widget_get_user_delta(res_win_t wt);
void	_widget_set_user_id(res_win_t wt, uid_t uid);
uid_t	_widget_get_user_id(res_win_t wt);
void	_widget_set_user_prop(res_win_t wt, const tchar_t* pname, var_long pval);
var_long	_widget_get_user_prop(res_win_t wt, const tchar_t* pname);
var_long	_widget_del_user_prop(res_win_t wt, const tchar_t* pname);
void	_widget_set_user_result(res_win_t wt, int code);
int		_widget_get_user_result(res_win_t wt);
void	_widget_set_style(res_win_t wt, dword_t ws);
dword_t	_widget_get_style(res_win_t wt);
void	_widget_set_accel(res_win_t wt, res_acl_t acl);
res_acl_t _widget_get_accel(res_win_t wt);
void	_widget_set_owner(res_win_t wt, res_win_t win);
res_win_t _widget_get_owner(res_win_t wt);
res_win_t _widget_get_child(res_win_t wt, uid_t uid);
res_win_t _widget_get_parent(res_win_t wt);
void	_widget_set_capture(res_win_t wt, bool_t b);
void	_widget_create_caret(res_win_t wt, int w, int h);
void	_widget_destroy_caret(res_win_t wt);
void	_widget_show_caret(res_win_t wt, int x, int y, bool_t b);

res_ctx_t _widget_client_ctx(res_win_t wt);
res_ctx_t _widget_window_ctx(res_win_t wt);
void	_widget_release_ctx(res_win_t wt, res_ctx_t dc);
void	_widget_get_client_rect(res_win_t wt, xrect_t* prt);
void	_widget_get_window_rect(res_win_t wt, xrect_t* prt);
void	_widget_client_to_screen(res_win_t wt, xpoint_t* ppt);
void	_widget_screen_to_client(res_win_t wt, xpoint_t* ppt);
void	_widget_client_to_window(res_win_t wt, xpoint_t* ppt);
void	_widget_window_to_client(res_win_t wt, xpoint_t* ppt);
void	_widget_center_window(res_win_t wt, res_win_t owner);
void	_widget_set_cursor(res_win_t wt, int curs);
void	_widget_set_capture(res_win_t wt, bool_t b);
void	_widget_set_imm(res_win_t wt, bool_t b);
bool_t	_widget_get_imm(res_win_t wt);
void	_widget_set_focus(res_win_t wt);
bool_t	_widget_key_state(res_win_t wt, int key);
bool_t	_widget_is_ownc(res_win_t wt);
bool_t	_widget_is_valid(res_win_t wt);
bool_t	_widget_is_child(res_win_t wt);
bool_t	_widget_is_focus(res_win_t wt);
void	_widget_size(res_win_t wt, const xsize_t* pxs);
void	_widget_move(res_win_t wt, const xpoint_t* ppt);
void	_widget_take(res_win_t wt, int zor);
void	_widget_show(res_win_t wt, dword_t sw);
void	_widget_update(res_win_t wt);
void	_widget_layout(res_win_t wt);
void	_widget_erase(res_win_t wt, const xrect_t* prt);
void	_widget_paint(res_win_t wt);
void	_widget_enable(res_win_t wt, bool_t b);

void	_widget_set_title(res_win_t wt, const tchar_t* token);
int		_widget_get_title(res_win_t wt, tchar_t* buf, int max);
void	_widget_calc_border(dword_t ws, border_t* pbd);
void	_widget_adjust_size(dword_t ws, xsize_t* pxs);
bool_t	_widget_enum_child(res_win_t wt, PF_ENUM_WINDOW_PROC pf, var_long pv);

bool_t	_widget_is_maximized(res_win_t wt);
bool_t	_widget_is_minimized(res_win_t wt);

if_subproc_t* _widget_get_subproc(res_win_t wt, uid_t sid);
bool_t	_widget_set_subproc(res_win_t wt, uid_t sid, if_subproc_t* sub);
void	_widget_del_subproc(res_win_t wt, uid_t sid);
bool_t	_widget_set_subproc_delta(res_win_t wt, uid_t sid, var_long delta);
var_long _widget_get_subproc_delta(res_win_t wt, uid_t sid);
bool_t	_widget_has_subproc(res_win_t wt);

void	_widget_post_char(res_win_t wt, tchar_t ch);
void	_widget_post_key(res_win_t wt, int key);
void	_widget_post_notice(res_win_t wt, NOTICE* pnc);
int		_widget_send_notice(res_win_t wt, NOTICE* pnc);
void	_widget_post_command(res_win_t wt, int code, uid_t cid, var_long data);
int		_widget_send_command(res_win_t wt, int code, uid_t cid, var_long data);

var_long _widget_set_timer(res_win_t wt, int ms);
void	_widget_kill_timer(res_win_t wt, var_long tid);

void	_widget_scroll(res_win_t wt, bool_t horz, int line);
void	_widget_get_scroll_info(res_win_t wt, bool_t horz, scroll_t* psl);
void	_widget_set_scroll_info(res_win_t wt, bool_t horz, const scroll_t* psl);
bool_t	_widget_has_struct(res_win_t wt);
void	_widget_set_xfont(res_win_t wt, const xfont_t* pxf);
void	_widget_get_xfont(res_win_t wt, xfont_t* pxf);
const xfont_t*	_widget_get_xfont_ptr(res_win_t wt);
void	_widget_set_xface(res_win_t wt, const xface_t* pxa);
void	_widget_get_xface(res_win_t wt, xface_t* pxa);
const xface_t*	_widget_get_xface_ptr(res_win_t wt);
void	_widget_set_xbrush(res_win_t wt, const xbrush_t* pxb);
void	_widget_get_xbrush(res_win_t wt, xbrush_t* pxb);
const xbrush_t*	_widget_get_xbrush_ptr(res_win_t wt);
void	_widget_set_xpen(res_win_t wt, const xpen_t* pxp);
void	_widget_get_xpen(res_win_t wt, xpen_t* pxp);
const xpen_t*	_widget_get_xpen_ptr(res_win_t wt);
void	_widget_set_mask(res_win_t wt, const xcolor_t* pxc);
void	_widget_get_mask(res_win_t wt, xcolor_t* pxc);
const xcolor_t*	_widget_get_mask_ptr(res_win_t wt);
void	_widget_set_iconic(res_win_t wt, const xcolor_t* pxc);
void	_widget_get_iconic(res_win_t wt, xcolor_t* pxc);
const xcolor_t*	_widget_get_iconic_ptr(res_win_t wt);
void	_widget_set_color_mode(res_win_t wt, const clr_mod_t* pclr);
void	_widget_get_color_mode(res_win_t wt, clr_mod_t* pclr);
void	_widget_set_point(res_win_t wt, const xpoint_t* ppt);
void	_widget_get_point(res_win_t wt, xpoint_t* ppt);
void	_widget_set_size(res_win_t wt, const xsize_t* pst);
void	_widget_get_size(res_win_t wt, xsize_t* pst);

void	_message_quit(int code);
void	_message_fetch(msg_t* pmsg, res_win_t wt);
bool_t	_message_peek(msg_t* pmsg);
bool_t	_message_translate(const msg_t* pmsg);
result_t _message_dispatch(const msg_t* pmsg);
void	_message_position(xpoint_t* ppt);

int		_widget_do_normal(res_win_t wt);
int		_widget_do_modal(res_win_t wt);
void	_widget_do_trace(res_win_t wt);

void	_get_screen_size(xsize_t* pxs);
void	_get_desktop_size(xsize_t* pxs);
void	_screen_size_to_pt(xsize_t* pls);
void	_screen_size_to_tm(xsize_t* pxs);

res_acl_t _create_accel_table(const accel_t* pac, int n);
void	_destroy_accel_table(res_acl_t hac);

#ifdef XDU_SUPPORT_WIDGET_EX
void	_widget_track_mouse(res_win_t wt, dword_t mask);
void	_widget_set_alpha(res_win_t, unsigned char a);
unsigned char _widget_get_alpha(res_win_t wt);
void	_widget_set_region(res_win_t wt, res_rgn_t rgn);
#endif

#ifdef XDU_SUPPORT_CONTEXT_OPENGL
res_glc_t	_widget_get_glctx(res_win_t wt);
#endif

#endif /*XDU_SUPPORT_WIDGET*/


#ifdef	__cplusplus
}
#endif


#endif	/* _XDUIML_H */

