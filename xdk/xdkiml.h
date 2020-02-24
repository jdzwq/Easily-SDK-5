/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdk system call interface document

	@module	xdkiml.h | interface file

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

#ifndef _XDKIML_H
#define	_XDKIML_H

#include "xdkdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef XDK_SUPPORT_MEMO
#ifdef XDK_SUPPORT_MEMO_HEAP
/*heap memory*/
res_heap_t _process_heapo(void);
res_heap_t	_heapo_create(void);
void	_heapo_destroy(res_heap_t heap);
void	_heapo_clean(res_heap_t heap);
void*	_heapo_alloc(res_heap_t heap, dword_t size);
void*	_heapo_realloc(res_heap_t heap, void* p, dword_t size);
void	_heapo_free(res_heap_t heap, void* p);
void	_heapo_zero(res_heap_t heap, void* p, dword_t size);
#endif

#ifdef XDK_SUPPORT_MEMO_GLOB
/*glob memory*/
res_glob_t	_globo_alloc(dword_t size);
res_glob_t	_globo_realloc(res_glob_t glob, dword_t size);
void	_globo_free(res_glob_t glob);
dword_t	_globo_size(res_glob_t glob);
void*	_globo_lock(res_glob_t glob);
bool_t	_globo_unlock(res_glob_t glob);
#endif

#ifdef XDK_SUPPORT_MEMO_LOCAL
/*local memory*/
void*	_local_alloc(dword_t size);
void*	_local_realloc(void* p, dword_t size);
void	_local_free(void* p);
#endif

#ifdef XDK_SUPPORT_MEMO_PAGE
/*page memory*/
void*	_paged_alloc(dword_t size);
void*	_paged_realloc(void* p, dword_t size);
void	_paged_free(void* p);
dword_t	_paged_size(void* p);
void*	_paged_lock(void* p);
void	_paged_unlock(void* p);
bool_t	_paged_protect(void* p, bool_t b);
#endif

#ifdef XDK_SUPPORT_MEMO_CACHE
void*	_cache_open(void);
void	_cache_close(void* bh);
bool_t	_cache_write(void* bh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb);
bool_t	_cache_read(void* bh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb);
#endif

#endif /*XDK_SUPPORT_MEMO*/

#ifdef XDK_SUPPORT_ERROR
/*error*/
int		_error_text(tchar_t* buf, int max);
void	_error_exit(void);
void	_error_debug(const tchar_t* str);
void	_error_print(const char* str);
#endif

#ifdef XDK_SUPPORT_MBCS
int		_gbk_to_ucs(const schar_t* gbk, int len, wchar_t* ucs, int max);
int		_ucs_to_gbk(const wchar_t* ucs, int len, schar_t* gbk, int max);
int		_utf8_to_ucs(const schar_t* utf, int len, wchar_t* ucs, int max);
int		_ucs_to_utf8(const wchar_t* ucs, int len, schar_t* utf, int max);
#endif

#ifdef XDK_SUPPORT_DATE
void	_get_loc_date(xdate_t* pxd);
bool_t	_mak_loc_date(xdate_t* pxd);
void	_get_utc_date(xdate_t* pxd);
bool_t	_mak_utc_date(xdate_t* pxd);
dword_t	_get_times(void);
clock_t _get_ticks(void);
stamp_t _get_timestamp(void);
void	_utc_date_from_times(xdate_t* pxd, dword_t ms);
void	_utc_date_from_ticks(xdate_t* pxd, clock_t ts);
void	_utc_date_from_timestamp(xdate_t* pxd, stamp_t ts);
#endif

#ifdef XDK_SUPPORT_ASYNC
async_t* _async_alloc_lapp(int type, int ms, res_file_t fd);
void	_async_free_lapp(async_t* pas);
#endif

#ifdef XDK_SUPPORT_THREAD
//those thread function must implemented by each platform
void	_thread_begin(res_thread_t* ptr_hand, PF_THREADFUNC pf_func, void* param);
void	_thread_end(void);
void	_thread_create_tls(tls_key_t* pkey);
void	_thread_destroy_tls(tls_key_t key);
void*	_thread_get_tls(tls_key_t key);
void	_thread_set_tls(tls_key_t key, void* pval);
pid_t	_thread_get_id(void);
void	_thread_sleep(int ms);
void	_thread_yield(void);
void	_thread_join(res_thread_t th);
void	_thread_safe(void);

#ifdef XDK_SUPPORT_THREAD_EVENT
res_even_t	_event_create(void);
void		_event_destroy(res_even_t ev);
wait_t		_event_wait(res_even_t ev, int milsec);
void		_event_sign(res_even_t ev, bool_t b_sign);
#endif

#ifdef XDK_SUPPORT_THREAD_CRITI
res_crit_t	_criti_create(void);
void		_criti_destroy(res_crit_t cr);
bool_t		_criti_query(res_crit_t cr);
void		_criti_enter(res_crit_t cr);
void		_criti_leave(res_crit_t cr);
#endif

#ifdef XDK_SUPPORT_THREAD_MUTEX
res_mutx_t	_mutex_create(const tchar_t* mname);
void		_mutex_destroy(const tchar_t* mname, res_mutx_t mtx);
res_mutx_t	_mutex_open(const tchar_t* mname);
wait_t		_mutex_lock(res_mutx_t mtx, int milsec);
void		_mutex_unlock(res_mutx_t mtx);
#endif

#ifdef XDK_SUPPORT_THREAD_SEMAP
res_sema_t	_semap_create(const tchar_t* sname, int max);
void		_semap_destroy(const tchar_t* sname, res_sema_t sem);
res_sema_t	_semap_open(const tchar_t* sname);
wait_t		_semap_lock(res_sema_t sem, int milsec);
void		_semap_unlock(res_sema_t sem);
#endif

#ifdef XDK_SUPPORT_THREAD_QUEUE
res_queue_t	_queue_create(res_queue_t ep, res_file_t fd, int max);
void		_queue_destroy(res_queue_t ep);
wait_t		_queue_wait(res_queue_t ep, int ms);
#endif

#endif /*XDK_SUPPORT_THREAD*/

#ifdef XDK_SUPPORT_TIMER
res_queue_t _create_timer_queue(void);
void		_destroy_timer_queue(res_queue_t rq);
res_timer_t _create_timer(res_queue_t rq, clock_t duetime, clock_t period, PF_TIMERFUNC pf, void* pa);
void		_destroy_timer(res_queue_t rq, res_timer_t rt);
bool_t		_alter_timer(res_queue_t rq, res_timer_t rt, clock_t duetime, clock_t period);
#endif

#ifdef XDK_SUPPORT_SOCK
bool_t		_socket_startup(void);
void		_socket_cleanup(void);
res_file_t	_socket_tcp(int ver, dword_t flag);
res_file_t	_socket_udp(int ver, dword_t flag);
res_file_t	_socket_icmp(int ver, dword_t flag);
void	_socket_shutdown(res_file_t so, int how);
void	_socket_close(res_file_t so);
dword_t	_socket_wait(res_file_t so, dword_t msk, int ms);
bool_t	_socket_bind(res_file_t so, res_addr_t saddr, int slen);
bool_t	_socket_listen(res_file_t so, int max);
bool_t	_socket_connect(res_file_t so, res_addr_t saddr, int slen);
res_file_t _socket_accept(res_file_t so, res_addr_t saddr, int *slen, async_t* pb);
bool_t	_socket_sendto(res_file_t so, res_addr_t saddr, int alen, void* buf, dword_t len, async_t* pb);
bool_t	_socket_recvfrom(res_file_t so, res_addr_t saddr, int *plen, void* buf, dword_t len, async_t* pb);
bool_t	_socket_send(res_file_t so, void* buf, dword_t len, async_t* pb);
bool_t	_socket_recv(res_file_t so, void* buf, dword_t len, async_t* pb);
int		_socket_write(void* pso, unsigned char* buf, int len);
int		_socket_read(void* pso, unsigned char* buf, int len);
bool_t	_socket_setopt(res_file_t so, int optname, const char* optval, int optlen);
bool_t	_socket_getopt(res_file_t so, int optname, char* pval, int* plen);
bool_t	_socket_set_nonblk(res_file_t so, bool_t none);
bool_t	_socket_get_nonblk(res_file_t so);
bool_t	_socket_set_linger(res_file_t so, bool_t wait, int sec);
bool_t	_socket_set_sndbuf(res_file_t so, int size);
bool_t	_socket_set_rcvbuf(res_file_t so, int size);
bool_t	_host_addr(const char* host, char* addr);
void	_fill_addr(net_addr_t* paddr, unsigned short port, const char* saddr);
void	_conv_addr(net_addr_t* paddr, unsigned short* port, char* addr);
void	_socket_addr(res_file_t so, net_addr_t* paddr);
void	_socket_peer(res_file_t so, net_addr_t* paddr);
bool_t	_socket_share(pid_t procid, res_file_t procfd, res_file_t so, void* data, dword_t size);
res_file_t _socket_dupli(res_file_t procfd, void* data, dword_t* pcb);
int		_socket_error(tchar_t* buf, int max);
#endif

#ifdef XDK_SUPPORT_FILE
res_file_t	_file_open(const tchar_t* fname, dword_t fmode);
void	_file_close(res_file_t fl);
bool_t	_file_size(res_file_t fh, dword_t* ph, dword_t* pl);
bool_t	_file_write(res_file_t fl, void* buf, dword_t size, async_t* pb);
bool_t	_file_flush(res_file_t fl);
bool_t	_file_read(res_file_t fl, void* buf, dword_t size, async_t* pb);
bool_t	_file_read_range(res_file_t fh, dword_t hoff, dword_t loff, void* buf, dword_t size);
bool_t	_file_write_range(res_file_t fh, dword_t hoff, dword_t loff, void* buf, dword_t size);
bool_t	_file_truncate(res_file_t fh, dword_t hoff, dword_t loff);
bool_t	_file_delete(const tchar_t* fname);
bool_t	_file_rename(const tchar_t* fname, const tchar_t* nname);
bool_t	_file_info(const tchar_t* fname, file_info_t* pxf);
bool_t	_file_settime(res_file_t fh, const xdate_t* pdt);
bool_t	_file_gettime(res_file_t fh, xdate_t* pdt);
bool_t	_directory_open(const tchar_t* path, dword_t mode);
bool_t	_directory_create(const tchar_t* path);
bool_t	_directory_remove(const tchar_t* path);
#ifdef XDK_SUPPORT_FILE_FIND
res_find_t	_file_find_first(const tchar_t* fpath, file_info_t* pfi);
bool_t	_file_find_next(res_find_t ff, file_info_t* pfi);
void _file_find_close(res_find_t ff);
#endif
#endif

#ifdef XDK_SUPPORT_SHARE
res_file_t _share_srv(const tchar_t* sname, const tchar_t* fpath, dword_t hoff, dword_t loff, dword_t size);
void	_share_close(const tchar_t* sname, res_file_t bh);
res_file_t	_share_cli(const tchar_t* fname, dword_t size);
bool_t	_share_write(res_file_t bh, dword_t off, void* buf, dword_t size, dword_t* pcb);
bool_t	_share_read(res_file_t bh, dword_t off, void* buf, dword_t size, dword_t* pcb);
void*	_share_lock(res_file_t fh, dword_t off, dword_t size);
void	_share_unlock(res_file_t fh, dword_t off, dword_t size, void* p);
#endif

#ifdef XDK_SUPPORT_PIPE
res_file_t _pipe_srv(const tchar_t* pname, dword_t fmode);
bool_t	_pipe_listen(res_file_t pip, async_t* pb);
void	_pipe_stop(res_file_t pip);
res_file_t _pipe_cli(const tchar_t* pname, dword_t fmode);
void	_pipe_close(const tchar_t* pname, res_file_t pip);
wait_t	_pipe_wait(const tchar_t* pname, int milsec);
bool_t	_pipe_write(res_file_t pipe, void* buf, dword_t len, async_t* pb);
bool_t	_pipe_flush(res_file_t pipe);
bool_t	_pipe_read(res_file_t pipe, void* buf, dword_t len, async_t* pb);
#endif

#ifdef XDK_SUPPORT_CONS
res_file_t	_cons_alloc(tchar_t* cname, int max);
void	_cons_free(res_file_t ch);
bool_t	_cons_sigaction(res_file_t con, PF_SIGHANDLER pf);
bool_t	_cons_write(res_file_t ch, void* buf, dword_t len, dword_t* pcb);
bool_t	_cons_flush(res_file_t ch);
bool_t	_cons_read(res_file_t ch, void* buf, dword_t len, dword_t* pcb);
res_file_t _cons_stdout(res_file_t ch);
res_file_t _cons_stdin(res_file_t ch);
#endif

#ifdef XDK_SUPPORT_COMM
void	_default_comm_mode(dev_com_t* pmod);
bool_t	_get_comm_mode(res_file_t fh, dev_com_t* pmod);
bool_t	_set_comm_mode(res_file_t fh, const dev_com_t* pmod);
res_file_t _comm_open(const tchar_t* pname, dword_t fmode);
void	_comm_close(res_file_t fh);
bool_t	_comm_read(res_file_t fh, void* buf, dword_t size, async_t* pb);
bool_t	_comm_write(res_file_t fh, void* buf, dword_t size, async_t* pb);
bool_t	_comm_flush(res_file_t fh);
dword_t	_comm_listen(res_file_t fh, async_t* pb);
#endif


#ifdef XDK_SUPPORT_PROCESS
res_modu_t _load_library(const tchar_t* lname);
void	_free_library(res_modu_t lib);
void*	_get_address(res_modu_t lib, const schar_t* fname);
void	_get_runpath(res_modu_t ins, tchar_t* buf, int max);
bool_t	_create_process(const tchar_t* exename, const tchar_t* cmdline, int share, proc_info_t* ppi);
void	_release_process(proc_info_t* ppi);
void	_process_safe(void);
void	_process_waitrun(res_proc_t child);
res_file_t _process_dupli(res_proc_t ph, res_file_t fh);
void*	_process_alloc(res_proc_t ph, dword_t dw);
void	_process_free(res_proc_t ph, void* p);
bool_t	_process_write(res_proc_t ph, void* p, void* data, dword_t size);
bool_t	_process_read(res_proc_t ph, void* p, void* data, dword_t size);
void	_release_handle(res_file_t fh);
bool_t	_inherit_handle(res_file_t fh, bool_t b);
void	_read_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, tchar_t* buf, int max);
void	_write_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, const tchar_t* val);
int		_get_envvar(const tchar_t* ename, tchar_t* buf, int max);
void	_system_info(sys_info_t* psi);
#endif

#ifdef XDK_SUPPORT_SHELL
bool_t	_shell_get_runpath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_curpath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_apppath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_docpath(tchar_t* pathbuf, int pathlen);
bool_t	_shell_get_tmppath(tchar_t* pathbuf, int pathlen);
#ifdef XDK_SUPPORT_SHELL_DIALOG
bool_t	_shell_get_filename(res_win_t owner, const tchar_t* defpath, const tchar_t* filter, const tchar_t* defext, bool_t saveit, tchar_t* pathbuf, int pathlen, tchar_t* filebuf, int filelen);
bool_t	_shell_get_pathname(res_win_t owner, const tchar_t* defpath, bool_t createit, tchar_t* pathbuf, int pathlen);
#endif
#endif /*XDK_SUPPORT_SHELL*/
/***************************************************************************************************************************/
#ifdef XDK_SUPPORT_CONTEXT
int		_context_startup(void);
void	_context_cleanup(void);
res_ctx_t _create_display_context(void);
res_ctx_t _create_compatible_context(res_ctx_t rdc);
void	_destroy_context(res_ctx_t rdc);
void	_get_device_caps(res_ctx_t rdc, dev_cap_t* pcap);
void	_render_context(res_ctx_t src, int srcx, int srcy, res_ctx_t dst, int dstx, int dsty, int dstw, int dsth);
res_pmp_t _select_pixmap(res_ctx_t rdc, res_pmp_t pmp);
res_pmp_t _create_compatible_pixmap(res_ctx_t rdc, int cx, int cy);
void	_destroy_pixmap(res_pmp_t pmp);

float	_pt_per_mm(res_ctx_t rdc, bool_t horz);
void	_text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, float* pfx, float* pfy);
void	_text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, int* pcx, int* pcy);
void	_text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, float* pfx, float* pfy);
void	_text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, int* pcx, int* pcy);
float	_cast_pt_to_mm(res_ctx_t rdc, int pt, bool_t horz);
int	_cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz);
int		_font_size(res_ctx_t rdc, int height);
#ifdef XDK_SUPPORT_CONTEXT_REGION
res_rgn_t _create_region(const tchar_t* shape, const xrect_t* pxr);
void	_delete_region(res_rgn_t rgn);
bool_t	_pt_in_region(res_rgn_t rgn, const xpoint_t* ppt);
#endif
#ifdef XDK_SUPPORT_CONTEXT_BITMAP
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
#ifdef XDK_SUPPORT_SHELL
res_bmp_t _load_bitmap_from_icon(res_ctx_t rdc, const tchar_t* iname);
res_bmp_t _load_bitmap_from_thumb(res_ctx_t rdc, const tchar_t* fname);
#endif
#endif
#ifdef XDK_SUPPORT_CONTEXT_PRINTER
bool_t _default_printer_mode(dev_prn_t* pmod);
bool_t _setup_printer_mode(res_win_t wnd, dev_prn_t* pmod);
res_ctx_t _create_printer_context(const dev_prn_t* pmod);
void	_destroy_printer_context(res_ctx_t rdc);
void	_begin_page(res_ctx_t rdc);
void	_end_page(res_ctx_t rdc);
void	_begin_doc(res_ctx_t rdc, const tchar_t* docname);
void	_end_doc(res_ctx_t rdc);
#endif

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC
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
#ifdef XDK_SUPPORT_CONTEXT_BITMAP
void _gdi_draw_image(res_ctx_t rdc, res_bmp_t bmp, const tchar_t* clr, const xrect_t* prt);
void _gdi_draw_bitmap(res_ctx_t rdc, res_bmp_t bmp, const xrect_t* prt);
#endif
#ifdef XDK_SUPPORT_CONTEXT_REGION
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
#endif /*XDK_SUPPORT_CONTEXT_GRAPHIC*/
#endif /*XDK_SUPPORT_CONTEXT*/

#ifdef XDK_SUPPORT_CLIPBOARD
bool_t	_clipboard_open(void);
void	_clipboard_close(void);
void	_clipboard_clean(void);
bool_t	_clipboard_put(int fmt, res_glob_t gb);
res_glob_t _clipboard_get(int fmt);
#endif

#ifdef XDK_SUPPORT_WIDGET
bool_t	_fetch_message(msg_t* pmsg, res_win_t wt);
bool_t	_peek_message(msg_t* pmsg, res_win_t wt);
bool_t	_translate_message(const msg_t* pmsg);
result_t _dispatch_message(const msg_t* pmsg);
int		_translate_accelerator(res_win_t wt, res_acl_t acl, msg_t* pmsg);
void	_message_position(xpoint_t* ppt);

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
void	_widget_post_char(res_win_t wt, tchar_t ch);
void	_widget_post_key(res_win_t wt, int key);
void	_widget_size(res_win_t wt, const xsize_t* pxs);
void	_widget_move(res_win_t wt, const xpoint_t* ppt);
void	_widget_take(res_win_t wt, int zor);
void	_widget_show(res_win_t wt, dword_t sw);
void	_widget_update(res_win_t wt);
void	_widget_resize(res_win_t wt);
void	_widget_paint(res_win_t wt);
void	_widget_redraw(res_win_t wt, const xrect_t* prt, bool_t b_erase);
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

void	_widget_post_message(res_win_t wt, int msg, var_long wp, var_long lp);
int		_widget_send_message(res_win_t wt, int msg, var_long wp, var_long lp);
void	_widget_post_command(res_win_t wt, int code, uid_t cid, var_long data);
int		_widget_send_command(res_win_t wt, int code, uid_t cid, var_long data);

void	_widget_copy(res_win_t wt);
void	_widget_cut(res_win_t wt);
void	_widget_paste(res_win_t wt);
void	_widget_undo(res_win_t wt);

var_long _widget_set_timer(res_win_t wt, int ms);
void	_widget_kill_timer(res_win_t wt, var_long tid);

void	_widget_get_scroll_info(res_win_t wt, bool_t horz, scroll_t* psl);
void	_widget_set_scroll_info(res_win_t wt, bool_t horz, const scroll_t* psl);

int		_widget_do_modal(res_win_t wt);
void	_widget_do_trace(res_win_t wt);

void	_get_screen_size(xsize_t* pxs);
void	_get_desktop_size(xsize_t* pxs);
void	_screen_size_to_pt(xsize_t* pls);
void	_screen_size_to_tm(xsize_t* pxs);

res_acl_t _create_accel_table(const accel_t* pac, int n);
void	_destroy_accel_table(res_acl_t hac);

#ifdef XDK_SUPPORT_WIDGET_EX
void	_widget_track_mouse(res_win_t wt, dword_t mask);
void	_widget_set_alpha(res_win_t, unsigned char a);
unsigned char _widget_get_alpha(res_win_t wt);
void	_widget_set_region(res_win_t wt, res_rgn_t rgn);
#endif

#ifdef XDK_SUPPORT_CONTEXT_OPENGL
res_glc_t	_widget_get_glctx(res_win_t wt);
#endif

#endif /*XDK_SUPPORT_WIDGET*/


#ifdef	__cplusplus
}
#endif


#endif	/* _XDKIML_H */

