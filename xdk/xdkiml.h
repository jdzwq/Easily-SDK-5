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
LOC_API res_heap_t _process_heapo(void);
LOC_API res_heap_t	_heapo_create(void);
LOC_API void	_heapo_destroy(res_heap_t heap);
LOC_API void	_heapo_clean(res_heap_t heap);
LOC_API void*	_heapo_alloc(res_heap_t heap, dword_t size);
LOC_API void*	_heapo_realloc(res_heap_t heap, void* p, dword_t size);
LOC_API void	_heapo_free(res_heap_t heap, void* p);
LOC_API void	_heapo_zero(res_heap_t heap, void* p, dword_t size);
#endif

#ifdef XDK_SUPPORT_MEMO_GLOB
/*glob memory*/
LOC_API res_glob_t	_globo_alloc(dword_t size);
LOC_API res_glob_t	_globo_realloc(res_glob_t glob, dword_t size);
LOC_API void	_globo_free(res_glob_t glob);
LOC_API dword_t	_globo_size(res_glob_t glob);
LOC_API void*	_globo_lock(res_glob_t glob);
LOC_API bool_t	_globo_unlock(res_glob_t glob);
#endif

#ifdef XDK_SUPPORT_MEMO_LOCAL
/*local memory*/
LOC_API void*	_local_alloc(dword_t size);
LOC_API void*	_local_realloc(void* p, dword_t size);
LOC_API void	_local_free(void* p);
#endif

#ifdef XDK_SUPPORT_MEMO_PAGE
/*page memory*/
LOC_API void*	_paged_alloc(dword_t size);
LOC_API void*	_paged_realloc(void* p, dword_t size);
LOC_API void	_paged_free(void* p);
LOC_API dword_t	_paged_size(void* p);
LOC_API void*	_paged_lock(void* p);
LOC_API void	_paged_unlock(void* p);
LOC_API bool_t	_paged_protect(void* p, bool_t b);
#endif

#ifdef XDK_SUPPORT_MEMO_CACHE
LOC_API void*	_cache_open(void);
LOC_API void	_cache_close(void* bh);
LOC_API bool_t	_cache_write(void* bh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb);
LOC_API bool_t	_cache_read(void* bh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb);
#endif

#endif /*XDK_SUPPORT_MEMO*/

#ifdef XDK_SUPPORT_ERROR
/*error*/
LOC_API int		_error_text(tchar_t* buf, int max);
LOC_API void	_error_exit(void);
LOC_API void	_error_debug(const tchar_t* str);
LOC_API void	_error_print(const char* str);
#endif

#ifdef XDK_SUPPORT_MBCS
LOC_API int		_gbk_to_ucs(const schar_t* gbk, int len, wchar_t* ucs, int max);
LOC_API int		_ucs_to_gbk(const wchar_t* ucs, int len, schar_t* gbk, int max);
LOC_API int		_utf8_to_ucs(const schar_t* utf, int len, wchar_t* ucs, int max);
LOC_API int		_ucs_to_utf8(const wchar_t* ucs, int len, schar_t* utf, int max);
#endif

#ifdef XDK_SUPPORT_DATE
LOC_API void	_get_loc_date(xdate_t* pxd);
LOC_API bool_t	_mak_loc_date(xdate_t* pxd);
LOC_API void	_get_utc_date(xdate_t* pxd);
LOC_API bool_t	_mak_utc_date(xdate_t* pxd);
LOC_API dword_t	_get_times(void);
LOC_API clock_t _get_ticks(void);
LOC_API stamp_t _get_timestamp(void);
LOC_API void	_utc_date_from_times(xdate_t* pxd, dword_t ms);
LOC_API void	_utc_date_from_ticks(xdate_t* pxd, clock_t ts);
LOC_API void	_utc_date_from_timestamp(xdate_t* pxd, stamp_t ts);
#endif

#ifdef XDK_SUPPORT_ASYNC
LOC_API async_t* _async_alloc_lapp(int type, int ms, res_file_t fd);
LOC_API void	_async_free_lapp(async_t* pas);
#endif

#ifdef XDK_SUPPORT_THREAD
//those thread function must implemented by each platform
LOC_API void	_thread_begin(res_thread_t* ptr_hand, PF_THREADFUNC pf_func, void* param);
LOC_API void	_thread_end(void);
LOC_API void	_thread_create_tls(tls_key_t* pkey);
LOC_API void	_thread_destroy_tls(tls_key_t key);
LOC_API void*	_thread_get_tls(tls_key_t key);
LOC_API void	_thread_set_tls(tls_key_t key, void* pval);
LOC_API pid_t	_thread_get_id(void);
LOC_API void	_thread_sleep(int ms);
LOC_API void	_thread_yield(void);
LOC_API void	_thread_join(res_thread_t th);
LOC_API void	_thread_safe(void);

#ifdef XDK_SUPPORT_THREAD_EVENT
LOC_API res_even_t	_event_create(void);
LOC_API void		_event_destroy(res_even_t ev);
LOC_API wait_t		_event_wait(res_even_t ev, int milsec);
LOC_API void		_event_sign(res_even_t ev, bool_t b_sign);
#endif

#ifdef XDK_SUPPORT_THREAD_CRITI
LOC_API res_crit_t	_criti_create(void);
LOC_API void		_criti_destroy(res_crit_t cr);
LOC_API bool_t		_criti_query(res_crit_t cr);
LOC_API void		_criti_enter(res_crit_t cr);
LOC_API void		_criti_leave(res_crit_t cr);
#endif

#ifdef XDK_SUPPORT_THREAD_MUTEX
LOC_API res_mutx_t	_mutex_create(const tchar_t* mname);
LOC_API void		_mutex_destroy(const tchar_t* mname, res_mutx_t mtx);
LOC_API res_mutx_t	_mutex_open(const tchar_t* mname);
LOC_API wait_t		_mutex_lock(res_mutx_t mtx, int milsec);
LOC_API void		_mutex_unlock(res_mutx_t mtx);
#endif

#ifdef XDK_SUPPORT_THREAD_SEMAP
LOC_API res_sema_t	_semap_create(const tchar_t* sname, int max);
LOC_API void		_semap_destroy(const tchar_t* sname, res_sema_t sem);
LOC_API res_sema_t	_semap_open(const tchar_t* sname);
LOC_API wait_t		_semap_lock(res_sema_t sem, int milsec);
LOC_API void		_semap_unlock(res_sema_t sem);
#endif

#ifdef XDK_SUPPORT_THREAD_QUEUE
LOC_API res_queue_t	_queue_create(res_queue_t ep, res_file_t fd, int max);
LOC_API void		_queue_destroy(res_queue_t ep);
LOC_API wait_t		_queue_wait(res_queue_t ep, int ms);
#endif

#endif /*XDK_SUPPORT_THREAD*/

#ifdef XDK_SUPPORT_TIMER
LOC_API res_queue_t _create_timer_queue(void);
LOC_API void		_destroy_timer_queue(res_queue_t rq);
LOC_API res_timer_t _create_timer(res_queue_t rq, clock_t duetime, clock_t period, PF_TIMERFUNC pf, void* pa);
LOC_API void		_destroy_timer(res_queue_t rq, res_timer_t rt);
LOC_API bool_t		_alter_timer(res_queue_t rq, res_timer_t rt, clock_t duetime, clock_t period);
#endif

#ifdef XDK_SUPPORT_SOCK
LOC_API bool_t		_socket_startup(void);
LOC_API void		_socket_cleanup(void);
LOC_API res_file_t	_socket_tcp(int ver, dword_t flag);
LOC_API res_file_t	_socket_udp(int ver, dword_t flag);
LOC_API res_file_t	_socket_icmp(int ver, dword_t flag);
LOC_API void	_socket_shutdown(res_file_t so, int how);
LOC_API void	_socket_close(res_file_t so);
LOC_API dword_t	_socket_wait(res_file_t so, dword_t msk, int ms);
LOC_API bool_t	_socket_bind(res_file_t so, res_addr_t saddr, int slen);
LOC_API bool_t	_socket_listen(res_file_t so, int max);
LOC_API bool_t	_socket_connect(res_file_t so, res_addr_t saddr, int slen);
LOC_API res_file_t _socket_accept(res_file_t so, res_addr_t saddr, int *slen, async_t* pb);
LOC_API bool_t	_socket_sendto(res_file_t so, res_addr_t saddr, int alen, void* buf, dword_t len, async_t* pb);
LOC_API bool_t	_socket_recvfrom(res_file_t so, res_addr_t saddr, int *plen, void* buf, dword_t len, async_t* pb);
LOC_API bool_t	_socket_send(res_file_t so, void* buf, dword_t len, async_t* pb);
LOC_API bool_t	_socket_recv(res_file_t so, void* buf, dword_t len, async_t* pb);
LOC_API int		_socket_write(void* pso, unsigned char* buf, int len);
LOC_API int		_socket_read(void* pso, unsigned char* buf, int len);
LOC_API bool_t	_socket_setopt(res_file_t so, int optname, const char* optval, int optlen);
LOC_API bool_t	_socket_getopt(res_file_t so, int optname, char* pval, int* plen);
LOC_API bool_t	_socket_set_nonblk(res_file_t so, bool_t none);
LOC_API bool_t	_socket_get_nonblk(res_file_t so);
LOC_API bool_t	_socket_set_linger(res_file_t so, bool_t wait, int sec);
LOC_API bool_t	_socket_set_sndbuf(res_file_t so, int size);
LOC_API bool_t	_socket_set_rcvbuf(res_file_t so, int size);
LOC_API bool_t	_host_addr(const char* host, char* addr);
LOC_API void	_fill_addr(net_addr_t* paddr, unsigned short port, const char* saddr);
LOC_API void	_conv_addr(net_addr_t* paddr, unsigned short* port, char* addr);
LOC_API void	_socket_addr(res_file_t so, net_addr_t* paddr);
LOC_API void	_socket_peer(res_file_t so, net_addr_t* paddr);
LOC_API bool_t	_socket_share(pid_t procid, res_file_t procfd, res_file_t so, void* data, dword_t size);
LOC_API LOC_API res_file_t _socket_dupli(res_file_t procfd, void* data, dword_t* pcb);
int		_socket_error(tchar_t* buf, int max);
#endif

#ifdef XDK_SUPPORT_FILE
LOC_API res_file_t	_file_open(const tchar_t* fname, dword_t fmode);
LOC_API void	_file_close(res_file_t fl);
LOC_API bool_t	_file_size(res_file_t fh, dword_t* ph, dword_t* pl);
LOC_API bool_t	_file_write(res_file_t fl, void* buf, dword_t size, async_t* pb);
LOC_API bool_t	_file_flush(res_file_t fl);
LOC_API bool_t	_file_read(res_file_t fl, void* buf, dword_t size, async_t* pb);
LOC_API bool_t	_file_read_range(res_file_t fh, dword_t hoff, dword_t loff, void* buf, dword_t size);
LOC_API bool_t	_file_write_range(res_file_t fh, dword_t hoff, dword_t loff, void* buf, dword_t size);
LOC_API bool_t	_file_truncate(res_file_t fh, dword_t hoff, dword_t loff);
LOC_API bool_t	_file_delete(const tchar_t* fname);
LOC_API bool_t	_file_rename(const tchar_t* fname, const tchar_t* nname);
LOC_API bool_t	_file_info(const tchar_t* fname, file_info_t* pxf);
LOC_API bool_t	_file_settime(res_file_t fh, const xdate_t* pdt);
LOC_API bool_t	_file_gettime(res_file_t fh, xdate_t* pdt);
LOC_API bool_t	_directory_open(const tchar_t* path, dword_t mode);
LOC_API bool_t	_directory_create(const tchar_t* path);
LOC_API bool_t	_directory_remove(const tchar_t* path);
#ifdef XDK_SUPPORT_FILE_FIND
LOC_API res_find_t	_file_find_first(const tchar_t* fpath, file_info_t* pfi);
LOC_API bool_t	_file_find_next(res_find_t ff, file_info_t* pfi);
LOC_API void _file_find_close(res_find_t ff);
#endif
#endif

#ifdef XDK_SUPPORT_SHARE
LOC_API res_file_t _share_srv(const tchar_t* sname, const tchar_t* fpath, dword_t hoff, dword_t loff, dword_t size);
LOC_API void	_share_close(const tchar_t* sname, res_file_t bh);
LOC_API res_file_t	_share_cli(const tchar_t* fname, dword_t size);
LOC_API bool_t	_share_write(res_file_t bh, dword_t off, void* buf, dword_t size, dword_t* pcb);
LOC_API bool_t	_share_read(res_file_t bh, dword_t off, void* buf, dword_t size, dword_t* pcb);
LOC_API void*	_share_lock(res_file_t fh, dword_t off, dword_t size);
LOC_API void	_share_unlock(res_file_t fh, dword_t off, dword_t size, void* p);
#endif

#ifdef XDK_SUPPORT_PIPE
LOC_API res_file_t _pipe_srv(const tchar_t* pname, dword_t fmode);
LOC_API bool_t	_pipe_listen(res_file_t pip, async_t* pb);
LOC_API void	_pipe_stop(res_file_t pip);
LOC_API res_file_t _pipe_cli(const tchar_t* pname, dword_t fmode);
LOC_API void	_pipe_close(const tchar_t* pname, res_file_t pip);
LOC_API wait_t	_pipe_wait(const tchar_t* pname, int milsec);
LOC_API bool_t	_pipe_write(res_file_t pipe, void* buf, dword_t len, async_t* pb);
LOC_API bool_t	_pipe_flush(res_file_t pipe);
LOC_API bool_t	_pipe_read(res_file_t pipe, void* buf, dword_t len, async_t* pb);
#endif

#ifdef XDK_SUPPORT_CONS
LOC_API res_file_t	_cons_alloc(tchar_t* cname, int max);
LOC_API void	_cons_free(res_file_t ch);
LOC_API bool_t	_cons_sigaction(res_file_t con, PF_SIGHANDLER pf);
LOC_API bool_t	_cons_write(res_file_t ch, void* buf, dword_t len, dword_t* pcb);
LOC_API bool_t	_cons_flush(res_file_t ch);
LOC_API bool_t	_cons_read(res_file_t ch, void* buf, dword_t len, dword_t* pcb);
LOC_API res_file_t _cons_stdout(res_file_t ch);
LOC_API res_file_t _cons_stdin(res_file_t ch);
#endif

#ifdef XDK_SUPPORT_COMM
LOC_API void	_default_comm_mode(dev_com_t* pmod);
LOC_API bool_t	_get_comm_mode(res_file_t fh, dev_com_t* pmod);
LOC_API bool_t	_set_comm_mode(res_file_t fh, const dev_com_t* pmod);
LOC_API res_file_t _comm_open(const tchar_t* pname, dword_t fmode);
LOC_API void	_comm_close(res_file_t fh);
LOC_API bool_t	_comm_read(res_file_t fh, void* buf, dword_t size, async_t* pb);
LOC_API bool_t	_comm_write(res_file_t fh, void* buf, dword_t size, async_t* pb);
LOC_API bool_t	_comm_flush(res_file_t fh);
LOC_API dword_t	_comm_listen(res_file_t fh, async_t* pb);
#endif

#ifdef XDK_SUPPORT_BLUT
LOC_API int	_enum_blut_device(dev_blt_t* pdb, int max);
LOC_API res_file_t _blut_open(const tchar_t* addr, int chan, dword_t fmode);
LOC_API void	_blut_close(res_file_t fh);
LOC_API bool_t	_blut_read(res_file_t fh, void* buf, dword_t size, async_t* pb);
LOC_API bool_t	_blut_write(res_file_t fh, void* buf, dword_t size, async_t* pb);
LOC_API bool_t	_blut_flush(res_file_t fh);
LOC_API dword_t	_blut_listen(res_file_t fh, async_t* pb);
#endif


#ifdef XDK_SUPPORT_PROCESS
LOC_API res_modu_t _load_library(const tchar_t* lname);
LOC_API void	_free_library(res_modu_t lib);
LOC_API void*	_get_address(res_modu_t lib, const schar_t* fname);
LOC_API void	_get_runpath(res_modu_t ins, tchar_t* buf, int max);
LOC_API bool_t	_create_process(const tchar_t* exename, const tchar_t* cmdline, int share, proc_info_t* ppi);
LOC_API void	_release_process(proc_info_t* ppi);
LOC_API void	_process_safe(void);
LOC_API void	_process_waitrun(res_proc_t child);
LOC_API res_file_t _process_dupli(res_proc_t ph, res_file_t fh);
LOC_API void*	_process_alloc(res_proc_t ph, dword_t dw);
LOC_API void	_process_free(res_proc_t ph, void* p);
LOC_API bool_t	_process_write(res_proc_t ph, void* p, void* data, dword_t size);
LOC_API bool_t	_process_read(res_proc_t ph, void* p, void* data, dword_t size);
LOC_API void	_release_handle(res_file_t fh);
LOC_API bool_t	_inherit_handle(res_file_t fh, bool_t b);
LOC_API void	_read_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, tchar_t* buf, int max);
LOC_API void	_write_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, const tchar_t* val);
LOC_API int		_get_envvar(const tchar_t* ename, tchar_t* buf, int max);
LOC_API void	_system_info(sys_info_t* psi);
#endif

/***************************************************************************************************************************/

#ifdef	__cplusplus
}
#endif


#endif	/* _XDKIML_H */

